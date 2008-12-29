#
#      Makefile.am for src/pnm
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PNMPROG = biff2pnm pnm2biff
MANP    = biff2pnm.m pnm2biff.m
CINCL   = xite_pnm.i

bin_PROGRAMS=$(PNMPROG) $(XPROG)

LDADD = ../../lib/libxite.la ../../lib/libxitepnm.la -lnetpbm

biff2pnm_SOURCES = biff2pnm_main.c
pnm2biff_SOURCES = pnm2biff_main.c
