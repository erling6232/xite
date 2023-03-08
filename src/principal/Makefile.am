#
#      Makefile.am for src/principal
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = linearTrans
MANP  = linearTrans.1
CINCL =  principal.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

linearTrans_SOURCES = linearTrans_main.c
