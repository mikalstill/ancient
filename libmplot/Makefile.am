lib_LTLIBRARIES = libplot.la
noinst_PROGRAMS = sample graph parabola fill

libplot_la_SOURCES = libplot.c
libplot_la_CFLAGS = $(FREETYPE_CFLAGS)
libplot_la_LIBADD = $(FREETYPE_LIBS)

include_HEADERS = libplot.h

sample_SOURCES = example.c
sample_LDADD = -lplot -L.libs
graph_SOURCES = exgraph.c
graph_LDADD = -lplot -L.libs
parabola_SOURCES = parabola.c
parabola_LDADD = -lplot -L.libs
fill_SOURCES = fill.c
fill_LDADD = -lplot -L.libs
