#
#      Makefile.am for src/pyramid
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = pyramid
XPROG = xpyramid
MANP  = pyramid.m xpyramid.m
CINCL =  pyramid.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

pyramid_SOURCES  = pyramid_main.c
xpyramid_SOURCES = xpyramid_main.c
xpyramid_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
