# Generated automatically from Makefile.in by configure.
# Makefile.in for kernel module

SHELL = /bin/sh
INSTALL = /usr/bin/install -c
mkinstalldirs = $(SHELL) ../mkinstalldirs
majver = 2.6

DISTFILES = Makefile.in dev.c dir.c file.c inode.c util.c fuse_i.h

fusemoduledir = /lib/modules/2.6.2/kernel/fs/fuse

ifeq ($(majver), 2.4)
fusemodule := fuse.o
else
fusemodule := fuse.ko
endif

all: all-spec

install: all
	@$(mkinstalldirs) $(DESTDIR)$(fusemoduledir)
	$(INSTALL) -m 644 $(fusemodule) $(DESTDIR)$(fusemoduledir)/$(fusemodule)
	-/sbin/depmod -a

uninstall:
	rm -f $(DESTDIR)$(fusemoduledir)/$(fusemodule)
	-/sbin/depmod -a

clean:
	-rm -f $(fusemodule) *.o .*.cmd *.mod.c *.ko *.s

distclean: clean
	rm -f Makefile

maintainer-clean: distclean

distdir: $(DISTFILES)
	cp -p $(DISTFILES) $(distdir)


ifeq ($(majver), 2.4)

CC = gcc
LD = ld
CFLAGS = -O2 -Wall -Wstrict-prototypes -fno-strict-aliasing -pipe
CPPFLAGS = -I/usr/src/linux-2.6.2-build/include -I../include -D__KERNEL__ -DMODULE -D_LOOSE_KERNEL_NAMES -DFUSE_VERSION=\"1.1-pre2\"

fuse_objs = dev.o dir.o file.o inode.o util.o

SUFFIXES = .c .o .s

all-spec: fuse.o

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

fuse.o: $(fuse_objs)
	$(LD) -r -o fuse.o $(fuse_objs)

fuse_headers = fuse_i.h ../include/linux/fuse.h

dev.o: $(fuse_headers)
dir.o: $(fuse_headers)
file.o: $(fuse_headers)
inode.o: $(fuse_headers)
util.o: $(fuse_headers)

else

EXTRA_CFLAGS := -I$(PWD)/../include -DFUSE_VERSION=\"1.1-pre2\"

obj-m := fuse.o
fuse-objs := dev.o dir.o file.o inode.o util.o

all-spec:
	$(MAKE) -C /usr/src/linux-2.6.2-build SUBDIRS=$(PWD) modules
endif




