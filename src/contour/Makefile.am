#
#      Makefile.am for src/contour
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = contour
XPROG   =
MANP  = contour.1
CINCL =  contour.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

contour_SOURCES = contour_main.c
