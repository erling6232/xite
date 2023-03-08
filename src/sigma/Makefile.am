#
#      Makefile.am for src/sigma
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = sigma
MANP  = sigma.1
CINCL =  sigma.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

sigma_SOURCES = sigma_main.c
