all:	usblog-dump

usblog-dump:	usblog-dump.o fileutil.o winlang.h
	gcc -ggdb -o usblog-dump usblog-dump.o fileutil.o

usblog-dump.o: usblog-dump.c
	gcc -c -ggdb usblog-dump.c -o usblog-dump.o

fileutil.o: fileutil.c fileutil.h
	gcc -c -ggdb -o fileutil.o fileutil.c

clean:	
	rm -f *.o *~ core usblog-dump