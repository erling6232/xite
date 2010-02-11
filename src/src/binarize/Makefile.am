#
#      Makefile.am for src/biff
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for arithmetic
#
PROG  = binarize expand_edge
XPROG =
MANP	= binarize.m \
	expand_edge.m
CINCL    = binarize.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

binarize_SOURCES = binarize_main.c
expand_edge_SOURCES = expand_edge_main.c
