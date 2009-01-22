#
#      Makefile.am for src/tiff
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
TIFFPROG = biff2tiff tiff2biff
MANP     = biff2tiff.m tiff2biff.m
CINCL    = tiff.i

bin_PROGRAMS=$(TIFFPROG) $(XPROG)

LDADD = ../../lib/libxite.la ../../lib/libxite_tiff.la -ltiff

biff2tiff_SOURCES = biff2tiff_main.c
tiff2biff_SOURCES = tiff2biff_main.c
