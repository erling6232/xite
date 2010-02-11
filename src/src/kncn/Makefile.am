#
#      Makefile.am for src/kncn
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = kncn mbkncn
MANP  = kncn.m mbkncn.m
CINCL =  kncn.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

kncn_SOURCES   = kncn_main.c
mbkncn_SOURCES = mbkncn_main.c
