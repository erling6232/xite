#
#      Makefile.am for src/thin
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG	= thn_har thn_zs thn_lc
MANP	= thn_har.1 thn_lc.1 thn_zs.1 
CINCL   = thin.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

thn_har_SOURCES = thn_har_main.c
thn_zs_SOURCES  = thn_zs_main.c
thn_lc_SOURCES  = thn_lc_main.c
