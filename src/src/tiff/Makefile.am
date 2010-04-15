#
#      Makefile.am for src/tiff
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
TIFFPROG = biff2tiff tiff2biff
MANP     = biff2tiff.1 tiff2biff.1
CINCL    = tiff.i

bin_PROGRAMS=$(TIFFPROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite_tiff.la ../../lib/libxite.la -ltiff

biff2tiff_SOURCES = biff2tiff_main.c
tiff2biff_SOURCES = tiff2biff_main.c
