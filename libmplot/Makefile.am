lib_LTLIBRARIES = libplot.la
noinst_PROGRAMS = sample graph

libplot_la_SOURCES = libplot.c
include_HEADERS = libplot.h

sample_SOURCES = example.c
sample_LDADD = -lplot -L.libs
graph_SOURCES = exgraph.c
graph_LDADD = -lplot -L.libs
