#
#      Makefile.am for src/copy
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = dither
XPROG   =
MANP  = dither.m
CINCL =  dither.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

dither_SOURCES = dither_main.c
