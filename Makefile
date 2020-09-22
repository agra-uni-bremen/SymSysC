THREADS := $(shell nproc)

all: bench

build/Makefile:
	mkdir build || true
	cd build && cmake ..

bench: build/Makefile klee/include
	make -C build -j$(THREADS)

clean-bench:
	rm -r build/

# @phony test-bench: klee/build/bin/klee build/CMakeFiles/testbench_plic.dir/main.cpp.o
	# klee/build/bin/klee -libcxx --only-output-states-covering-new build/CMakeFiles/testbench_plic.dir/main.cpp.o


clean: clean-bench

klee/include:
	git submodule update --init --recursive

docker:
	podman pull klee/klee
	podman run --rm -ti --ulimit='stack=-1:-1' klee/klee
