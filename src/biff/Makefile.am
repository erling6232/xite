#
#      Makefile.am for src/biff
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for arithmetic
#
PROG  = biffcopy biffinfo biffmerge newtitle bifftext
XPROG =
MANP  = biffcopy.1 biffinfo.1 biffmerge.1 newtitle.1\
	bifftext.1
CINCL = biff.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

biffcopy_SOURCES = biffcopy_main.c
biffinfo_SOURCES = biffinfo_main.c
biffmerge_SOURCES = biffmerge_main.c
newtitle_SOURCES = newtitle_main.c
bifftext_SOURCES = bifftext_main.c
