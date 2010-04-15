#
#      Makefile.am for src/scatter
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = scatter
MANP  = scatter.1
CINCL =  scatter.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

scatter_SOURCES = scatter_main.c
