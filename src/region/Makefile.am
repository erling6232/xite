#
#      Makefile.am for src/region
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = regionAnalyse regionConvexHull
MANP  = regionAnalyse.1 regionConvexHull.1 xregion.1
CINCL = region.i
XPROG = xregion

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

regionAnalyse_SOURCES    = regionAnalyse_main.c
regionConvexHull_SOURCES = regionConvexHull_main.c
xregion_SOURCES          = xregion_main.c
xregion_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
