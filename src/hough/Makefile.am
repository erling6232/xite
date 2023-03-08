#
#      Makefile.am for src/hough
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = houghCircle houghLine
MANP  = houghCircle.1 houghLine.1
CINCL =  hough.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

houghCircle_SOURCES = houghCircle_main.c  
houghLine_SOURCES = houghLine_main.c
