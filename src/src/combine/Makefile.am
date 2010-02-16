#
#      Makefile.am for src/color
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = combine
XPROG   =
MANP  = combine.1
CINCL =  combine.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

combine_SOURCES = combine_main.c
