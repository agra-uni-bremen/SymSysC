THREADS := $(shell nproc)

all: bench

build/Makefile:
	mkdir build || true
	cd build && cmake ..

bench: build/Makefile
	make -C build -j$(THREADS)

bench-test:	bench
	cd build

clean-bench:
	rm -r build/

clean: clean-bench

docker-build:
	podman build --tag klee-more:1.0 . 

docker:			#todo: check if docker image exists
	podman run --rm -ti --ulimit='stack=-1:-1' \
	-v $(shell pwd):/home/klee/source:Z \
	klee-more:1.0

	
#git submodule deinit <path_to_submodule>
#git rm <path_to_submodule>
#git commit-m "Removed submodule "
#rm -rf .git/modules/<path_to_submodule>