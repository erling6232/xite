#
#      Makefile.am for src/minmax
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = closestMinMax maxarea minarea
MANP  = closestMinMax.1 maxarea.1 minarea.1
CINCL =  minmax.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

closestMinMax_SOURCES = closestMinMax_main.c
maxarea_SOURCES       = maxarea_main.c
minarea_SOURCES       = minarea_main.c
