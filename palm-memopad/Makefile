all:	memopad-dump memopad-write

memopad-dump:	memopad-dump.o fileutil.o
	gcc -o memopad-dump memopad-dump.o fileutil.o

memopad-dump.o: memopad-dump.c
	gcc -c -ggdb memopad-dump.c -o memopad-dump.o

memopad-write:	memopad-write.o fileutil.o
	gcc -o memopad-write memopad-write.o fileutil.o

memopad-write.o: memopad-write.c
	gcc -c -ggdb memopad-write.c -o memopad-write.o

fileutil.o: fileutil.c fileutil.h
	gcc -c -ggdb -o fileutil.o fileutil.c

clean:	
	rm -f *.o *~ core memopad-write memopad-dump