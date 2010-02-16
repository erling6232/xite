#
#      Makefile.am for src/cdoc
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for cdoc
#
PROG  = add_html_toc cdoc
XPROG =
MANP	= cdoc.1
CINCL    = cdoc.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

add_html_toc_SOURCES = add_html_toc_main.c
cdoc_SOURCES = cdoc_main.c
