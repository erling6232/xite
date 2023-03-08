#
#      Makefile.am for src/binarize
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitebinarize.la

MANF =  bin_ab.3 bin_be.3 bin_etm.3 kittler_threshold.3 bin_ki.3 \
	kapur_threshold.3 bin_ksw.3 bin_man.3 bin_mh.3 bin_ni.3 \
	bin_nr.3 otsu_threshold.3 bin_ot.3 bin_pa.3 bin_tfj.3 \
	bin_wr1.3 bin_iwr.3 bin_wr2.3 bin_yb.3 expand_edge.3 \
	mark_ghost.3 remove_inv_ghost.3 mark_inv_ghost.3 remove_ghost.3 remove_small.3

man3_MANS=$(MANF)

libxitebinarize_la_SOURCES = \
	bin_ab.c \
        bin_be.c \
        bin_etm.c \
        bin_ki.c \
        bin_ksw.c \
        bin_man.c \
        bin_mh.c \
        bin_ni.c \
        bin_nr.c \
        bin_ot.c \
        bin_pa.c \
        bin_tfj.c \
        bin_wr1.c \
        bin_wr2.c \
        bin_yb.c \
        expand_edge.c \
        remove_ghost.c

kittler_threshold.3: bin_ki.c
kapur_threshold.3: bin_ksw.c
otsu_threshold.3: bin_ot.c
bin_iwr.3: bin_wr2.c
mark_ghost.3: remove_ghost.c
remove_inv_ghost.3: remove_ghost.c
mark_inv_ghost.3: remove_ghost.c
