all:	usblog-dump decoder-usbstepper decoder-ignore

usblog-dump:	usblog-dump.o fileutil.o winlang.h md5.o linux.o decode.o utility.o
	gcc -Wall -ggdb -o usblog-dump usblog-dump.o fileutil.o md5.o linux.o decode.o utility.o

usblog-dump.o: usblog-dump.c
	gcc -Wall -c -ggdb usblog-dump.c -o usblog-dump.o

fileutil.o: fileutil.c fileutil.h
	gcc -Wall -c -ggdb -o fileutil.o fileutil.c

md5.o: md5c.c md5.h md5-global.h
	gcc -Wall -c -ggdb -o md5.o md5c.c

linux.o: linux.c
	gcc -Wall -c -ggdb -o linux.o linux.c

utility.o: utility.c
	gcc -Wall -c -ggdb -o utility.o utility.c

decode.o: decode.c
	gcc -Wall -c -ggdb -o decode.o decode.c

decoder-usbstepper: decoder-usbstepper.c
	gcc -Wall decoder-usbstepper.c -o decoder-usbstepper

decoder-ignore: decoder-ignore.c
	gcc -Wall decoder-ignore.c -o decoder-ignore

clean:	
	rm -f *.o *~ core usblog-dump decoder-usbstepper decoder-ignore
