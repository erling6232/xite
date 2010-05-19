#
#      Makefile.am for lib/zernike
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitezernike.la

MANF =  zer_con.3 zer_rec.3 zer_pol.3 zer_mom.3

man3_MANS=$(MANF)

libxitezernike_la_SOURCES = zernike.c

zer_con.3:	zer_mom.3
zer_rec.3:	zer_mom.3
zer_pol.3:	zer_mom.3
zer_mom.3:	zernike.3
