cc=g++
flags=--std=c++11
includes=./systemc-dist/include


all: main


main: main.cpp
	$(cc) $(flags) -I $(includes) main.cpp 