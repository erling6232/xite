#
#      Makefile.am for src/contour
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = contour
XPROG   =
MANP  = contour.m
CINCL =  contour.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

contourbine_SOURCES = contour_main.c
