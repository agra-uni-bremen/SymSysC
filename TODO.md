Performance testing on side
	- compare C/C++

Think of concrete testbenches
	- priority vs order of interrupts
	- If interrupt is done, sometime (in timeout) it should be triggered

(nice to have: standard assertions (no crash, memory leak, ..))

Hashmap ersetzen durch normale map
UART, Sensor2 mit register range

klee/angr comparison?
	Kosten / Nutzen?

Hierachical waits in called functions?

Systemc basierte Metriken, automatisch herausfinden?
    Coverage?

Laufzeit messen, Sinvolle Tests schreiben.
    Eigenschaften definieren und dagegen testen (Beispieleigenschaften aus 2016 Datepaper)
    

PLIC Ausführungsgeschwindigkeit wo Probleme?

Erste Erkenntnis: Boost und Fremdlibs verzögern _extrem_ die Ausführung.

Getrennte Tabellen für jede Testbench.
    -> Fehlergruppen for jede TB
        -> Robustness (interface)
        -> Memory-Fehler
        -> Off-by-one
        -> Whitebox: Erwartete Werte
            -> Prio richtig?
            -> Interrupt auch claim-bar?


Error handling (generic values as input, watch behaviour),
Functional testing
Fault injection vs Correctness, how?


Howto Build LLVM-Sysc:

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

Note: Sadly, the build commands for checked-in systemc got lodst. :*


something something wllvm:
```bash
export LLVM_COMPILER=clang
./configure CC=wllvm CXX=wllvm++ --prefix="$(pwd)/../systemc-dist"
# not working: ./configure CC=wllvm CXX=wllvm++ --prefix="$(pwd)/../systemc-dist" CXXFLAGS="--target=x86_64-pc-linux-gnu" CFLAGS="--target=x86_64-pc-linux-gnu"
make -j20 && make install

```
