#
#      Makefile.am for src/biff
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions for arithmetic
#
PROG  = binarize expand_edge
XPROG =
MANP	= binarize.m \
	bin_ab.m \
	bin_be.m \
	bin_etm.m \
	bin_ki.m \
	bin_ksw.m \
	bin_man.m \
	bin_mh.m \
	bin_ni.m \
	bin_nr.m \
	bin_ot.m \
	bin_pa.m \
	bin_tfj.m \
	bin_wr1.m \
	bin_wr2.m \
	bin_yb.m \
	expand_edge.m \
	remove_ghost.m \
	remove_small.m
CINCL    = binarize.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

binarize_SOURCES = binarize_main.c
expand_edge_SOURCES = expand_edge_main.c
