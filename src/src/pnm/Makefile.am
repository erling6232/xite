#
#      Makefile.am for src/pnm
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PNMPROG = biff2pnm pnm2biff
MANP    = biff2pnm.m pnm2biff.m
CINCL   = xite_pnm.i

bin_PROGRAMS=$(PNMPROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite_pnm.la ../../lib/libxite.la -lnetpbm

biff2pnm_SOURCES = biff2pnm_main.c
pnm2biff_SOURCES = pnm2biff_main.c
