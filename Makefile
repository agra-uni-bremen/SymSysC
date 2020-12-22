THREADS := $(shell nproc)

container_prog:= podman
ifeq (, $(shell which podman))
 container_prog:= docker
endif

default: native

all: native bytecode

build/:
	mkdir build || true

build/native/Makefile: build/
	mkdir build/native || true
	cd build/native && cmake -DBUILD=native ../..
	
build/bytecode/Makefile: build/
	mkdir build/bytecode || true
	cd build/bytecode && cmake -DBUILD=bytecode ../..

native: build/native/Makefile
	make -C build/native -j$(THREADS)

bytecode: build/bytecode/Makefile
	make -C build/bytecode -j$(THREADS)

clean-native:
	rm -r build/native

clean-bytecode:
	rm -r build/bytecode

clean: clean-native clean-bytecode

docker-build:
	echo "if problems occur, execute"
	echo "sudo mkdir /sys/fs/cgroup/systemd"
	echo "sudo mount -t cgroup -o none,name=systemd cgroup /sys/fs/cgroup/systemd"
	$(container_prog) build --tag klee-more:1.0 . 

docker:			#todo: check if docker image exists
	$(container_prog) run -ti --ulimit='stack=-1:-1' -v $(shell pwd):/home/klee/source:Z klee-more:1.0
	#$(container_prog) run --rm -ti --ulimit='stack=-1:-1' -v $(shell pwd):/home/klee/source:Z klee-more:1.0


#git submodule deinit <path_to_submodule>
#git rm <path_to_submodule>
#git commit-m "Removed submodule "
#rm -rf .git/modules/<path_to_submodule>
