#
#      Makefile.am for src/draw
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = mkImg bdf2biff biffwrite
XPROG   =
MANP  = mkImg.m bdf2biff.m biffwrite.m
CINCL  = draw.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

mkImg_SOURCES = mkImg_main.c
bdf2biff_SOURCES   = bdf2biff_main.c
biffwrite_SOURCES  = biffwrite_main.c
