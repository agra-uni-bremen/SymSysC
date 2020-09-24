THREADS := $(shell nproc)

container_prog:= podman
ifeq (, $(shell which podman))
 container_prog:= docker
endif

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
	$(container_prog) build --tag klee-more:1.0 . 

docker:			#todo: check if docker image exists
	$(container_prog) run --rm -ti --ulimit='stack=-1:-1' -v $(shell pwd):/home/klee/source:Z klee-more:1.0

	
#git submodule deinit <path_to_submodule>
#git rm <path_to_submodule>
#git commit-m "Removed submodule "
#rm -rf .git/modules/<path_to_submodule>