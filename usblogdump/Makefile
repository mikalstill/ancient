all:	usblog-dump

usblog-dump:	usblog-dump.o fileutil.o winlang.h md5.o
	gcc -ggdb -o usblogdump usblog-dump.o fileutil.o md5.o

usblog-dump.o: usblog-dump.c
	gcc -c -ggdb usblog-dump.c -o usblog-dump.o

fileutil.o: fileutil.c fileutil.h
	gcc -c -ggdb -o fileutil.o fileutil.c

md5.o: md5c.c md5.h md5-global.h
	gcc -c -ggdb -o md5.o md5c.c

clean:	
	rm -f *.o *~ core usblog-dump