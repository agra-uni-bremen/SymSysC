cc=g++
flags=--std=c++14
includes=. #./systemc-dist/include


@phony all: main

  
main: main.cpp
	$(cc) $(flags) -I $(includes) main.cpp -o main