AM_YFLAGS = -d

SUBDIRS = . examples

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pandalex.pc

lib_LTLIBRARIES = libpandalex.la
libpandalex_la_SOURCES = parser.y lexer.l

pandalexincludedir = $(includedir)
pandalexinclude_HEADERS = pandalex.h

