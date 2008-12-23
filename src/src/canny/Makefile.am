#
#      Makefile.am for src/canny
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions for canny
#
PROG  = canny
XPROG =
MANP	= canny.m
CINCL    = canny.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

canny_SOURCES = canny_main.c
