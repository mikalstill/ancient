all: codebook encode

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

report: output.sgml output.ps output.pdf
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml 
	builddb < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

clean:
	rm -f *.o codebook encode output.sgml output.ps output.pdf
