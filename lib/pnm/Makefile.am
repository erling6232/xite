#
#      Makefile.am for lib/pnm
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitepnm.la

MANF =  biff2pnmg.3 biff2pnmc.3 writepnm.3 biff2pnm.3 ppm2biff.3 \
	pgm2biff.3 pbm2biff.3 pnm2biff.3

man3_MANS=$(MANF)

libxitepnm_la_SOURCES = biff2pnm.c pnm2biff.c

biff2pnmg.3:	biff2pnm.3
biff2pnmc.3:	biff2pnm.3
writepnm.3:	biff2pnm.3
ppm2biff.3:	pnm2biff.3
pgm2biff.3:	pnm2biff.3
pbm2biff.3:	pnm2biff.3
