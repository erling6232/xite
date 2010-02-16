#
#      Makefile.am for src/gradInv
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = gradInv
MANP  = gradInv.1
CINCL =  gradInv.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

gradInv_SOURCES = gradInv_main.c
