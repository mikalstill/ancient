lib_LTLIBRARIES = libtrivsql.la
noinst_PROGRAMS = sample

libtrivsql_la_SOURCES = parser.y lexer.l tdb.c spinlock.c selectors.c interface.c internal.c

sample_SOURCES = example.c
sample_LDADD = -ltrivsql -L.

YFLAGS = -d
