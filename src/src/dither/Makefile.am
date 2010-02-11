#
#      Makefile.am for src/copy
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = dither
XPROG   =
MANP  = dither.m
CINCL =  dither.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

dither_SOURCES = dither_main.c
