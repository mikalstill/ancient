# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4-p5 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# How do I add a test to this file?
# Follow the numbered comments...

# 1: If it is a new test subset, add it's name here (starting with test). Note
# that you _do_not_ have to add targets here, especially if you only want them
# as part of huge.


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/cep
pkglibdir = $(libdir)/cep
pkgincludedir = $(includedir)/cep

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
CC = gcc
CXX = c++
MAINT = #
MAKEINFO = makeinfo
PACKAGE = cep
RANLIB = ranlib
VERSION = 0.1

noinst_PROGRAMS = testall testwindows testmatrix testconfig testls testdataset testnumeric
EXTRA_PROGRAMS = testallhuge

testwindows_SOURCES = alltests.cpp cep_testWindows.cpp
testwindows_LDADD = ../cepDataWindower.o ../cepWindowAlg.o ../cepWindowRect.o ../cepwindowhamming.o ../cepwindowchebyshev.o ../cepwindowblackman.o

testmatrix_SOURCES = alltests.cpp cep_testMatrix.cpp

testconfig_SOURCES = alltests.cpp cep_testConfig.cpp

testls_SOURCES = alltests.cpp cep_testLs.cpp
testls_LDADD = ../cepLs.o

testdataset_SOURCES = alltests.cpp cep_testDataset.cpp
testdataset_LDADD = ../cepDataset.o

testnumeric_SOURCES = alltests.cpp cep_testNumeric.cpp

# 2: Above this line, you need to add a line test<name>_SOURCES, which lists
# your .cpp files, and (if you need it), a line test<name>_LDADD, which lists
# object files from the source directory which you need for the test

# 3: Add the .cpp files here, and add a variable if you used objects from the
# source directory. Sometimes, if it is heaps of tests which you only want to
# run very occassionally, it might belong in (4) instead...
testall_SOURCES = alltests.cpp cep_testWindows.cpp cep_testMatrix.cpp cep_testConfig.cpp cep_testLs.cpp cep_testDataset.cpp cep_testNumeric.cpp
testall_LDADD = $(testwindows_LDADD) $(testls_LDADD) $(testdataset_LDADD) $(testnumeric_LDADD)

# 4: A special target for really long winded tests (the other tests are not
# included in this target). Add a new variable, and then add that variable to
# to the testallhuge_SOURCES variable
testpresentation_SOURCES = alltests.cpp cep_testPresentation000.cpp cep_testPresentation1000.cpp cep_testPresentation1200.cpp cep_testPresentation1400.cpp cep_testPresentation1600.cpp cep_testPresentation1800.cpp cep_testPresentation2000.cpp cep_testPresentation200.cpp cep_testPresentation2200.cpp cep_testPresentation2400.cpp cep_testPresentation2600.cpp cep_testPresentation2800.cpp cep_testPresentation3000.cpp cep_testPresentation3200.cpp cep_testPresentation3400.cpp cep_testPresentation3600.cpp cep_testPresentation3800.cpp cep_testPresentation4000.cpp cep_testPresentation400.cpp cep_testPresentation4200.cpp cep_testPresentation4400.cpp cep_testPresentation600.cpp cep_testPresentation800.cpp

testallhuge_SOURCES = $(testpresentation_SOURCES)

# These get built into everything
LDFLAGS = ../cepError.o ../cepUtility.o -lcppunit -lm
mkinstalldirs = $(SHELL) $(top_srcdir)/config/mkinstalldirs
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(noinst_PROGRAMS)


DEFS =  -DPACKAGE=\"cep\" -DVERSION=\"0.1\" -DHAVE_LIBM=1 -DHAVE_LIBPTHREAD=1 -DHAVE_LIBWX_GTK_2_2=1 -DHAVE_LIBPNG=1 -D__WXGTK__=1 -D_GNU_SOURCE=1  -I. -I$(srcdir) 
CPPFLAGS =  -D_REENTRANT -Wall
LIBS = -lpng -lwx_gtk-2.2 -lpthread -lm 
testallhuge_OBJECTS =  alltests.o cep_testPresentation000.o \
cep_testPresentation1000.o cep_testPresentation1200.o \
cep_testPresentation1400.o cep_testPresentation1600.o \
cep_testPresentation1800.o cep_testPresentation2000.o \
cep_testPresentation200.o cep_testPresentation2200.o \
cep_testPresentation2400.o cep_testPresentation2600.o \
cep_testPresentation2800.o cep_testPresentation3000.o \
cep_testPresentation3200.o cep_testPresentation3400.o \
cep_testPresentation3600.o cep_testPresentation3800.o \
cep_testPresentation4000.o cep_testPresentation400.o \
cep_testPresentation4200.o cep_testPresentation4400.o \
cep_testPresentation600.o cep_testPresentation800.o
testallhuge_LDADD = $(LDADD)
testallhuge_DEPENDENCIES = 
testallhuge_LDFLAGS = 
testall_OBJECTS =  alltests.o cep_testWindows.o cep_testMatrix.o \
cep_testConfig.o cep_testLs.o cep_testDataset.o cep_testNumeric.o
testall_DEPENDENCIES =  ../cepDataWindower.o ../cepWindowAlg.o \
../cepWindowRect.o ../cepwindowhamming.o ../cepwindowchebyshev.o \
../cepwindowblackman.o ../cepLs.o ../cepDataset.o
testall_LDFLAGS = 
testwindows_OBJECTS =  alltests.o cep_testWindows.o
testwindows_DEPENDENCIES =  ../cepDataWindower.o ../cepWindowAlg.o \
../cepWindowRect.o ../cepwindowhamming.o ../cepwindowchebyshev.o \
../cepwindowblackman.o
testwindows_LDFLAGS = 
testmatrix_OBJECTS =  alltests.o cep_testMatrix.o
testmatrix_LDADD = $(LDADD)
testmatrix_DEPENDENCIES = 
testmatrix_LDFLAGS = 
testconfig_OBJECTS =  alltests.o cep_testConfig.o
testconfig_LDADD = $(LDADD)
testconfig_DEPENDENCIES = 
testconfig_LDFLAGS = 
testls_OBJECTS =  alltests.o cep_testLs.o
testls_DEPENDENCIES =  ../cepLs.o
testls_LDFLAGS = 
testdataset_OBJECTS =  alltests.o cep_testDataset.o
testdataset_DEPENDENCIES =  ../cepDataset.o
testdataset_LDFLAGS = 
testnumeric_OBJECTS =  alltests.o cep_testNumeric.o
testnumeric_LDADD = $(LDADD)
testnumeric_DEPENDENCIES = 
testnumeric_LDFLAGS = 
CXXFLAGS = -g -O2
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
DEP_FILES =  .deps/alltests.P .deps/cep_testConfig.P \
.deps/cep_testDataset.P .deps/cep_testLs.P .deps/cep_testMatrix.P \
.deps/cep_testNumeric.P .deps/cep_testPresentation000.P \
.deps/cep_testPresentation1000.P .deps/cep_testPresentation1200.P \
.deps/cep_testPresentation1400.P .deps/cep_testPresentation1600.P \
.deps/cep_testPresentation1800.P .deps/cep_testPresentation200.P \
.deps/cep_testPresentation2000.P .deps/cep_testPresentation2200.P \
.deps/cep_testPresentation2400.P .deps/cep_testPresentation2600.P \
.deps/cep_testPresentation2800.P .deps/cep_testPresentation3000.P \
.deps/cep_testPresentation3200.P .deps/cep_testPresentation3400.P \
.deps/cep_testPresentation3600.P .deps/cep_testPresentation3800.P \
.deps/cep_testPresentation400.P .deps/cep_testPresentation4000.P \
.deps/cep_testPresentation4200.P .deps/cep_testPresentation4400.P \
.deps/cep_testPresentation600.P .deps/cep_testPresentation800.P \
.deps/cep_testWindows.P
SOURCES = $(testallhuge_SOURCES) $(testall_SOURCES) $(testwindows_SOURCES) $(testmatrix_SOURCES) $(testconfig_SOURCES) $(testls_SOURCES) $(testdataset_SOURCES) $(testnumeric_SOURCES)
OBJECTS = $(testallhuge_OBJECTS) $(testall_OBJECTS) $(testwindows_OBJECTS) $(testmatrix_OBJECTS) $(testconfig_OBJECTS) $(testls_OBJECTS) $(testdataset_OBJECTS) $(testnumeric_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .cpp .o .s
$(srcdir)/Makefile.in: # Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu tests/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

testallhuge: $(testallhuge_OBJECTS) $(testallhuge_DEPENDENCIES)
	@rm -f testallhuge
	$(CXXLINK) $(testallhuge_LDFLAGS) $(testallhuge_OBJECTS) $(testallhuge_LDADD) $(LIBS)

testall: $(testall_OBJECTS) $(testall_DEPENDENCIES)
	@rm -f testall
	$(CXXLINK) $(testall_LDFLAGS) $(testall_OBJECTS) $(testall_LDADD) $(LIBS)

testwindows: $(testwindows_OBJECTS) $(testwindows_DEPENDENCIES)
	@rm -f testwindows
	$(CXXLINK) $(testwindows_LDFLAGS) $(testwindows_OBJECTS) $(testwindows_LDADD) $(LIBS)

testmatrix: $(testmatrix_OBJECTS) $(testmatrix_DEPENDENCIES)
	@rm -f testmatrix
	$(CXXLINK) $(testmatrix_LDFLAGS) $(testmatrix_OBJECTS) $(testmatrix_LDADD) $(LIBS)

testconfig: $(testconfig_OBJECTS) $(testconfig_DEPENDENCIES)
	@rm -f testconfig
	$(CXXLINK) $(testconfig_LDFLAGS) $(testconfig_OBJECTS) $(testconfig_LDADD) $(LIBS)

testls: $(testls_OBJECTS) $(testls_DEPENDENCIES)
	@rm -f testls
	$(CXXLINK) $(testls_LDFLAGS) $(testls_OBJECTS) $(testls_LDADD) $(LIBS)

testdataset: $(testdataset_OBJECTS) $(testdataset_DEPENDENCIES)
	@rm -f testdataset
	$(CXXLINK) $(testdataset_LDFLAGS) $(testdataset_OBJECTS) $(testdataset_LDADD) $(LIBS)

testnumeric: $(testnumeric_OBJECTS) $(testnumeric_DEPENDENCIES)
	@rm -f testnumeric
	$(CXXLINK) $(testnumeric_LDFLAGS) $(testnumeric_OBJECTS) $(testnumeric_LDADD) $(LIBS)
.cpp.o:
	$(CXXCOMPILE) -c $<

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = tests

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu tests/Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

-include $(DEP_FILES)

mostlyclean-depend:

clean-depend:

distclean-depend:
	-rm -rf .deps

maintainer-clean-depend:

%.o: %.c
	@echo '$(COMPILE) -c $<'; \
	$(COMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.c
	@echo '$(LTCOMPILE) -c $<'; \
	$(LTCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp

%.o: %.cpp
	@echo '$(CXXCOMPILE) -c $<'; \
	$(CXXCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.cpp
	@echo '$(LTCXXCOMPILE) -c $<'; \
	$(LTCXXCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-noinstPROGRAMS mostlyclean-compile \
		mostlyclean-tags mostlyclean-depend mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-noinstPROGRAMS clean-compile clean-tags clean-depend \
		clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-noinstPROGRAMS distclean-compile distclean-tags \
		distclean-depend distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-depend maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-noinstPROGRAMS distclean-noinstPROGRAMS \
clean-noinstPROGRAMS maintainer-clean-noinstPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir mostlyclean-depend \
distclean-depend clean-depend maintainer-clean-depend info-am info \
dvi-am dvi check check-am installcheck-am installcheck install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# 5: cd ..; automake; autoconf; ./configure; cd tests; make

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
