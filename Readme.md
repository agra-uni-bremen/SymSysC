Symbolic Execution of SystemC TLM Peripherals
=============================================

This is a Mini-Kernel, designed to replace (parts of) the SystemC TLM kernel and optimized for symbolic execution frameworks,
especially like [klee](https://klee.github.io/).
It is the experimental proof-of-concept of a paper yet to be published.

How to test:
------------
Start docker container: `make docker-build docker`
and run tests:
```bash
$ ./make.sh # optional
$ ./source/run_all_tests.sh
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
cp -P src/libsystemc* /home/klee/source/systemc-dist/lib_llvm/

#test it?
klee --libcxx --libc=uclibc -posix-runtime -only-output-states-covering-new build/testbench_sysc_plic interface_test_write
```


There is also the possibility of building this with [wllvm](https://github.com/travitch/whole-program-llvm):
```bash
export LLVM_COMPILER=clang
./configure CC=wllvm CXX=wllvm++ --prefix="$(pwd)/../systemc-dist"
# not working: ./configure CC=wllvm CXX=wllvm++ --prefix="$(pwd)/../systemc-dist" CXXFLAGS="--target=x86_64-pc-linux-gnu" CFLAGS="--target=x86_64-pc-linux-gnu"
make -j20 && make install
```
