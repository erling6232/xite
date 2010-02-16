#
#      Makefile.am for src/copy
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = biffDump mosaic overlap subcopy
XPROG   =
MANP  = biffDump.1 mosaic.1 overlap.1 subcopy.1
CINCL =  copy.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

biffDump_SOURCES = biffDump_main.c
mosaic_SOURCES   = mosaic_main.c
overlap_SOURCES  = overlap_main.c
subcopy_SOURCES  = subcopy_main.c
