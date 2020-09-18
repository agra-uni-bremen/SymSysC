THREADS := $(shell nproc)


all: bench klee


build/Makefile:
	mkdir build || true
	cd build && cmake ..

bench: build/Makefile
	make -C build -j$(THREADS)

clean-bench:
	rm -r build/


deps:
	pip3 install tabulate wllvm
	sudo apt install build-essential curl libcap-dev git cmake libncurses5-dev python-minimal python-pip unzip libtcmalloc-minimal4 libgoogle-perftools-dev libsqlite3-dev doxygen d-shlibs
	sudo apt install clang-9 llvm-9 llvm-9-dev llvm-9-tools
	sudo apt install llvm-dev z3 libz3-dev


klee-uclibc/configure:
	git submodule update --init --recursive


klee-uclibc/lib: klee-uclibc/configure
	cd klee-uclibc
	./configure --make-llvm-lib
	make -j$(THREADS)

klee/scripts/build/build.sh:
	git submodule update --init --recursive

klee-libcxx: klee/scripts/build/build.sh
	LLVM_VERSION=9 SANITIZER_BUILD= BASE=$(pwd)/klee-libcxx REQUIRES_RTTI=1 DISABLE_ASSERTIONS=0 ENABLE_DEBUG=0 ENABLE_OPTIMIZED=1 ./klee/scripts/build/build.sh libcxx

clean-libcxx:
	rm -rf klee-libcxx

klee/build:
	mkdir klee/build

klee/build/Makefile: klee-libcxx klee/build
	cd klee/build && cmake -DENABLE_KLEE_LIBCXX=ON -DKLEE_LIBCXX_DIR=$(pwd)/../../klee-libcxx/libc++-install-9/ -DKLEE_LIBCXX_INCLUDE_DIR=$(pwd)/../../klee-libcx/libc++-install-9/include/c++/v1/ -DENABLE_SOLVER_Z3=ON ..


klee: klee/build/Makefile
	make -C klee/build -j$(THREADS)

clean-klee:
	rm -r klee/build

clean: clean-klee clean-libcxx clean-bench
