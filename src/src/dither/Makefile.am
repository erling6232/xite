#
#      Makefile.am for src/copy
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = biffDump mosaic overlap subcopy
XPROG   =
MANP  = biffDump.m copyBand.m mosaic.m overlap.m subcopy.m
CINCL =  copy.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

biffDump_SOURCES = biffDump_main.c
mosaic_SOURCES   = mosaic_main.c
overlap_SOURCES  = overlap_main.c
subcopy_SOURCES  = subcopy_main.c
