all:	fat-dump

fat-dump:	fat-dump.o fileutil.o winlang.h
	gcc -ggdb -o fat-dump fat-dump.o fileutil.o

fat-dump.o: fat-dump.c
	gcc -c -ggdb fat-dump.c -o fat-dump.o

fileutil.o: fileutil.c fileutil.h
	gcc -c -ggdb -o fileutil.o fileutil.c

clean:	
	rm -f *.o *~ core fat-dump