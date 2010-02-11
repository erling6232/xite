#
#      Makefile.am for src/snn
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = litSnn snn
MANP  = litSnn.m snn.m
CINCL =  snn.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

litSnn_SOURCES = litSnn_main.c
snn_SOURCES    = snn_main.c
