all:	chm-dump

chm-dump:	chm-dump.o fileutil.o winlang.h
	gcc -ggdb -o chm-dump chm-dump.o fileutil.o

chm-dump.o: chm-dump.c
	gcc -c -ggdb chm-dump.c -o chm-dump.o

fileutil.o: fileutil.c fileutil.h
	gcc -c -ggdb -o fileutil.o fileutil.c

clean:	
	rm -f *.o *~ core chm-dump