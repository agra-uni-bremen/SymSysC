#!/usr/bin/env bash

set -e # exit on error

DIR="$(pwd)"

PREFIX=$DIR/systemc-dist
LIBNAME=llvm

version=2.3.3
source=systemc-$version.tar.gz

if [ ! -f "$source" ]; then
	wget http://www.accellera.org/images/downloads/standards/systemc/$source
fi

tar xf $source

mkdir -p $PREFIX

cd systemc-$version

# first, build the normal systemc (also for include files)
mkdir -p native-build && cd native-build
../configure CXX=clang CXXFLAGS='-std=c++17 -stdlib=libc++' --prefix=$PREFIX --with-arch-suffix=-native
make -j$(nproc) install

cd ..
# now building bytecode

mkdir -p $LIBNAME-build && cd $LIBNAME-build
# not that important what is set here, as we overwrite it anyways in cmake command
../configure CXX=clang CXXFLAGS='-std=c++17 -DUSE_KLEE -fcxx-exceptions -stdlib=libc++' --prefix=$PREFIX --with-arch-suffix=-$LIBNAME
cmake -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_CXX_FLAGS="-DUSE_KLEE -fcxx-exceptions -stdlib=libc++ -std=c++17 -emit-llvm -flto -c -Xclang -disable-O0-optnone" -D CMAKE_CXX_CREATE_STATIC_LIBRARY="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" -D CMAKE_CXX_LINK_EXECUTABLE="llvm-link -only-needed <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -DCMAKE_LINKER=llvm-link -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PREFIX -DINSTALL_TO_LIB_TARGET_ARCH_DIR=$LIBNAME ..
make -j$(nproc) || echo "Ignore Errors, as some ASM files can't be built in bytecode"
cd src
set -x # enable echo
# TODO: Is cpp all enough? There is one qt.c that breaks stuff
llvm-link -o libsystemc.so.$version $(find -iname *.cpp.o | xargs echo)
set +x # disable again

# manual install in different dir for lib
mkdir -p $PREFIX/lib-llvm
cp -p libsystemc* $PREFIX/lib-llvm
# It is important that it ends with bc for CMake to determine correct linker script.
ln -sT $PREFIX/lib-llvm/libsystemc.so{,.bc}

echo "SystemC libs now available as LLVM bitcode at $(ll $PREFIX/lib-llvm/libsystemc.*)"