Symbolic Execution of SystemC TLM Peripherals
=============================================

This is the "library" that only contains the minikernel and the required build-system for SystemC in LLVM Bitcode.
For some experiments/examples, see [symsysc-experiments](https://github.com/agra-uni-bremen/symsysc-experiments)

REQUIREMENTS
------------

clang, llvm-link, libc++ (get by installing g++) or libcxx-devel.
Optional, but nice: wllvm (`pip install wllvm`)


If klee not available, build it yourself (warn: Will take some time)

```
BASE="$HOME/shitgit" MINISAT_VERSION="master" STP_VERSION="2.3.3" SOLVERS="stp" COVERAGE=0 USE_TCMALLOC=1 LLVM_VERSION=14 ENABLE_OPTIMIZED=1 ENABLE_DEBUG=1 DISABLE_ASSERTIONS=0 REQUIRES_RTTI=0 SOLVERS=STP:Z3 GTEST_VERSION=1.11.0 UCLIBC_VERSION=klee_uclibc_v1.2 TCMALLOC_VERSION=2.7 SANITIZER_BUILD= STP_VERSION=2.3.3 MINISAT_VERSION=master Z3_VERSION=4.8.14 USE_LIBCXX=1 KLEE_RUNTIME_BUILD="Debug+Asserts" ENABLE_DOXYGEN=0 ./scripts/build/build.sh klee
```


How to Build LLVM-SystemC:
--------------------------
```bash
https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
tar xf systemc-2.3.3.tar.gz && cd systemc-2.3.3

mkdir -C build && cd build
cmake -D CMAKE_C_COMPILER=clang -DCMAKE_C_COMPILER_TARGET=x86_64-pc-linux-gnu -DCMAKE_CXX_COMPILER_TARGET=x86_64-pc-linux-gnu -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_FLAGS="-g -fcxx-exceptions -stdlib=libc++ -std=c++14 -emit-llvm -flto -c -Xclang -disable-O0-optnone" -D CMAKE_C_FLAGS="-g -emit-llvm -flto -c -Xclang -disable-O0-optnone" -D CMAKE_CXX_CREATE_SHARED_LIBRARY="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET>" -D CMAKE_CXX_LINK_EXECUTABLE="llvm-link <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>" -DCMAKE_LINKER=llvm-link ..
make -j$(nproc) || echo "Just ignore errors, lol"
llvm-link -o src/libsystemc.so.2.3.3 $(find -iname *.c*.o | xargs echo)
cp -P src/libsystemc* /home/klee/source/systemc-dist/lib-llvm/

#test it?
klee --libcxx --libc=uclibc -posix-runtime -only-output-states-covering-new build/testbench_sysc_plic interface_test_write
```


There is also the possibility of building this with [wllvm](https://github.com/travitch/whole-program-llvm):
```bash
export LLVM_COMPILER=clang
../configure CC=wllvm CXX=wllvm++ --prefix="$(pwd)/../systemc-dist" --build=x86_64-pc-linux-gnu --enable-debug  CXXFLAGS='-std=c++17' --with-arch-suffix=-wllvm #--enable-shared=NO --enable-static=YES
make -j20 && make install
```
