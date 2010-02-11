#
#      Makefile.am for src/extrema
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = extrema1   extrema2   minima   maxima
XPROG   =
MANP  = extrema1.m extrema2.m minima.m maxima.m
CINCL =  extrema.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

extrema1_SOURCES = extrema1_main.c
extrema2_SOURCES = extrema2_main.c
minima_SOURCES   = minima_main.c
maxima_SOURCES   = maxima_main.c
