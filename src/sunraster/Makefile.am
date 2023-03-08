#
#      Makefile.am for src/sunraster
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = biff2sunraster sunraster2biff
MANP  = biff2sunraster.1 sunraster2biff.1
CINCL =  sunraster.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

biff2sunraster_SOURCES = biff2sunraster_main.c
sunraster2biff_SOURCES = sunraster2biff_main.c
