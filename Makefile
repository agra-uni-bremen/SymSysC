
@phony all: bench


build/Makefile:
	mkdir build || true
	cd build && cmake ..
  
bench: build/Makefile
	make -C build -j5
	
@phony clean:
	rm -r build/