lib_LTLIBRARIES = libmplot.la
noinst_PROGRAMS = sample graph parabola fill

libmplot_la_SOURCES = libplot.c
libmplot_la_CFLAGS = $(FREETYPE_CFLAGS)
libmplot_la_LIBADD = $(FREETYPE_LIBS)

include_HEADERS = libmplot.h

sample_SOURCES = example.c
sample_LDADD = -lmplot -L.libs
graph_SOURCES = exgraph.c
graph_LDADD = -lmplot -L.libs
parabola_SOURCES = parabola.c
parabola_LDADD = -lmplot -L.libs
fill_SOURCES = fill.c
fill_LDADD = -lmplot -L.libs
