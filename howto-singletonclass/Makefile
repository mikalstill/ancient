.SUFFIXES: .cpp

all: sing

sing: main.o singleton.o
	c++ main.o singleton.o -o singleton

.cpp.o:
	c++ -c $<