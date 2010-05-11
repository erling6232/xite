#
#      Makefile.am for lib/haar
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitehaar.la

MANF =  haar1d.3 haari1d.3 haar2d.3 haari2d.3 haar.3 haarTexture.3

man3_MANS=$(MANF)

libxitehaar_la_SOURCES = haar1d.c haar2d.c haarTexture.c

haari1d.3:	haar1d.3
haari2d.3:	haar2d.3
haar.3:	haar2d.3
