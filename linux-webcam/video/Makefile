# Make file to make video applications
#
#

CFLAGS = -O2
CC     = g++
OBJS = read-camera.o 
LIBS = qt
LIBPATH = /usr/lib/qt-2.3.2/lib/
INCLUDEDIR = /usr/lib/qt-2.3.2/include/

ALL: main qt-interface

main: read-camera.o 
	$(CC) $(CFLAGS) main.cpp $(OBJS) -o main

qt-interface: read-camera.o
	$(CC) $(CFLAGS) qtinterface.cpp -L$(LIBPATH) \
	-l$(LIBS) -I$(INCLUDEDIR) $(OBJS) -o qt-interface

clean:
	echo Cleaning
	rm -rf read-camera.o 
	rm -rf main 
	rm -rf qt-interface 

.SUFFIXES: .cc.o .cpp.o

.cc.o:
	$(CC) -c $(CFLAGS)  $<

.cpp.o:
	$(CC) -c $(CFLAGS)  $<
