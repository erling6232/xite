#
#      Makefile.am for src/resample
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = resample
MANP  = resample.1
CINCL =  resample.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

resample_SOURCES = resample_main.c
