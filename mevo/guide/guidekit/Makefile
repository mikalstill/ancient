CFLAGS = -Wall -g

all: readguide writeguide

readguide: common.o readguide.o
	$(CC) $(CFLAGS) -o readguide common.o readguide.o

writeguide: common.o writeguide.o
	$(CC) $(CFLAGS) -o writeguide common.o writeguide.o

clean:
	rm -f *.o readguide writeguide
