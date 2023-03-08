#
#      Makefile.am for lib/fht
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitefht.la

MANF =  fhtInitTables.3 fht1d.3 fht2d.3 fhtPhase.3 fhtPower.3 ft2ht.3 ht2ft.3

man3_MANS=$(MANF)

libxitefht_la_SOURCES = fht2d.c ht2ft.c ft2ht.c fhtPower.c fhtPhase.c

fhtInitTables.3:	fht2d.3
fht1d.3:	fht2d.3
fht2d.3:	fht2d.3
