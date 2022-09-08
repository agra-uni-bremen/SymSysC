#!/usr/bin/env bash

set -e # exit on error

DIR="$(pwd)"

PREFIX="$DIR/systemc-dist"

version=2.3.3
source=systemc-$version.tar.gz

if [ ! -f "$source" ]; then
	wget http://www.accellera.org/images/downloads/standards/systemc/$source
fi

tar xf $source

mkdir -p $PREFIX
SOURCEFOLDER=$DIR/systemc-$version
cd $SOURCEFOLDER

echo "first, build the normal systemc (also for include files)"
mkdir -p native-build && cd native-build
../configure --quiet CC=clang CCFLAGS="" CXX=clang++ CXXFLAGS="-std=c++17 -stdlib=libc++" --prefix=$PREFIX --with-arch-suffix=-native
make --no-print-directory -j$(nproc)
make -s install

cd $SOURCEFOLDER
# now building bytecode
LIBNAME=llvm
echo "Now building bytecode manually ($LIBNAME)"
mkdir -p $LIBNAME-build && cd $LIBNAME-build
# not that important what is set here, as we overwrite it anyways in cmake command
../configure --quiet --target=x86_64-pc-linux-gnu CC=clang CFLAGS='' CXX=clang++ CXXFLAGS='-std=c++17' --prefix=$PREFIX --with-arch-suffix=-$LIBNAME
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -D CMAKE_CXX_FLAGS="-DUSE_KLEE -fcxx-exceptions -stdlib=libc++ -std=c++17 -emit-llvm -flto -c -Xclang -disable-O0-optnone" -D CMAKE_CXX_CREATE_STATIC_LIBRARY="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" -D CMAKE_CXX_LINK_EXECUTABLE="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -DCMAKE_LINKER=llvm-link -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PREFIX -DINSTALL_TO_LIB_TARGET_ARCH_DIR=$LIBNAME ..
echo "Building in first pass, to let the C-Checks pass"
make --no-print-directory
echo "changing c compiler flags to also emit bitcode. This is needed as checks (and include path search) fail with this set to on."
cmake -DCMAKE_C_FLAGS="-emit-llvm" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -D CMAKE_CXX_FLAGS="-DUSE_KLEE -fcxx-exceptions -stdlib=libc++ -std=c++17 -emit-llvm -flto -c -Xclang -disable-O0-optnone" -D CMAKE_CXX_CREATE_STATIC_LIBRARY="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" -D CMAKE_CXX_LINK_EXECUTABLE="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -DCMAKE_LINKER=llvm-link -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PREFIX -DINSTALL_TO_LIB_TARGET_ARCH_DIR=$LIBNAME ..
#cmake -D CMAKE_C_COMPILER=clang -D CMAKE_C_COMPILER_WORKS=1 -DCMAKE_C_FLAGS='-emit-llvm' -DCMAKE_LINKER=$(which llvm-link) -DCMAKE_C_LINK_FLAGS="" -D CMAKE_C_CREATE_STATIC_LIBRARY="llvm-link <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -D CMAKE_C_LINK_EXECUTABLE="llvm-link <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -DCMAKE_CXX_COMPILER=clang++ -D CMAKE_CXX_FLAGS="-DUSE_KLEE -fcxx-exceptions -stdlib=libc++ -std=c++17 -emit-llvm -flto -c -Xclang -disable-O0-optnone" -D CMAKE_CXX_CREATE_STATIC_LIBRARY="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" -D CMAKE_CXX_LINK_EXECUTABLE="llvm-link -only-needed <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -DCMAKE_LINKER=llvm-link -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PREFIX -DINSTALL_TO_LIB_TARGET_ARCH_DIR=$LIBNAME ..
make --no-print-directory -j$(nproc) || echo "Ignore Errors, as some ASM files can't be built in bytecode"
cd src
set -x # enable echo
llvm-link -o libsystemc.so.$version $(find -iname *.c*.o | xargs echo)
set +x # disable again

# manual install in different dir for lib
INSTALLDIR=$PREFIX/lib-$LIBNAME
mkdir -p $INSTALLDIR
cp -p libsystemc* $INSTALLDIR
# It is important that it ends with bc for CMake to determine correct linker script.
if [ -f $INSTALLDIR/libsystemc.so.bc ]; then
	rm $INSTALLDIR/libsystemc.so.bc
fi
ln -sT $INSTALLDIR/libsystemc.so{,.bc}
cd $SOURCEFOLDER 

echo "Now building with WLLVM"
LIBNAME=wllvm
INSTALLDIR=$PREFIX/lib-$LIBNAME

mkdir -p $LIBNAME-build && cd $LIBNAME-build
export LLVM_COMPILER=clang
../configure --quiet CC=wllvm CXX=wllvm++ --prefix="$PREFIX" --build=x86_64-pc-linux-gnu --enable-debug  CXXFLAGS='-std=c++17' --with-arch-suffix=-wllvm #--enable-shared=NO --enable-static=YES
make --no-print-directory install

cd $INSTALLDIR
extract-bc libsystemc.a
extract-bc -b libsystemc.a
cd $SOURCEFOLDER

echo "SystemC libs now available at $(ls -l $PREFIX/lib-*/libsystemc.*)"
