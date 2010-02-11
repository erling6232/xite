#
#      Makefile.am for src/canny
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for canny
#
PROG  = canny
XPROG =
MANP	= canny.m
CINCL    = canny.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

canny_SOURCES = canny_main.c
