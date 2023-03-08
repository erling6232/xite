#
#      Makefile.am for src/draw
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = mkImg bdf2biff biffwrite
XPROG   =
MANP  = mkImg.1 bdf2biff.1 biffwrite.1
CINCL  = draw.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

mkImg_SOURCES = mkImg_main.c
bdf2biff_SOURCES   = bdf2biff_main.c
biffwrite_SOURCES  = biffwrite_main.c
