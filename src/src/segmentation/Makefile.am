#
#      Makefile.am for src/segmentation
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = segmSpannWilson   segmRandom
MANP  = segmSpannWilson.m segmRandom.m
CINCL  = segmentation.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

segmSpannWilson_SOURCES = segmSpannWilson_main.c
segmRandom_SOURCES = segmRandom_main.c
