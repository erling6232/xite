#
#      Makefile.am for src/PostScript
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = biff2ps histo2ps
MANP  = biff2ps.1 histo2ps.1

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

biff2ps_SOURCES  = biff2ps_main.c
histo2ps_SOURCES = histo2ps_main.c
