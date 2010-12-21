#
#      Makefile.am for src/dic2biff
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = dic2biff
MANP  = dic2biff.1
CINCL =  dic2biff.i

bin_PROGRAMS=$(PROG) $(XPROG)
dist_bin_SCRIPTS = dcbiff
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

dic2biff_SOURCES = dic2biff.c read_dic.c read_spi.c wspibiff.c wacrbiff.c
