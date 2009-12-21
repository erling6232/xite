#
#      Makefile.am for src/pyramid
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = pyramid
XPROG = xpyramid
MANP  = pyramid.m xpyramid.m
CINCL =  pyramid.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

pyramid_SOURCES  = pyramid_main.c
xpyramid_SOURCES = xpyramid_main.c
xpyramid_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
