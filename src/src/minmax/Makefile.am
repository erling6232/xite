#
#      Makefile.am for src/minmax
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = closestMinMax maxarea minarea
MANP  = closestMinMax.m maxarea.m minarea.m
CINCL =  minmax.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

closestMinMax_SOURCES = closestMinMax_main.c
maxarea_SOURCES       = maxarea_main.c
minarea_SOURCES       = minarea_main.c
