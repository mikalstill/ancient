all: codebook encode hmminit

codebook: gencode.o mfc.o lbg.o
	c++ gencode.o mfc.o lbg.o -o codebook

encode:	encode.o mfc.o lbg.o
	c++ encode.o mfc.o lbg.o -o encode

gencode.o: gencode.cpp world.h
	c++ -c -Wall -ggdb gencode.cpp -o gencode.o

encode.o: encode.cpp world.h
	c++ -c -Wall -ggdb encode.cpp -o encode.o

mfc.o: mfc.cpp mfc.h world.h
	c++ -c -Wall -ggdb mfc.cpp -o mfc.o

lbg.o: lbg.cpp lbg.h world.h
	c++ -c -Wall -ggdb lbg.cpp -o lbg.o

hmminit: hmminit.cpp
	c++ hmminit.cpp -o hmminit

clean:
	rm -f *.o codebook encode
