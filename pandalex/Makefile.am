YFLAGS = -d
LDADD = -lpandalex -L.libs
INCLUDES = -I..

lib_LTLIBRARIES = libpandalex.la
libpandalex_la_SOURCES = parser.y lexer.l tdb.c spinlock.c

pandalexincludedir = $(includedir)
pandalexinclude_HEADERS = pandalex.h

noinst_PROGRAMS = sample
sample_SOURCES = samples.c

