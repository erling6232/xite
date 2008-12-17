#
#      Makefile.am for src/sunraster
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = biff2sunraster sunraster2biff
MANP  = biff2sunraster.m sunraster2biff.m
CINCL =  sunraster.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

biff2sunraster_SOURCES = biff2sunraster_main.c
sunraster2biff_SOURCES = sunraster2biff_main.c
