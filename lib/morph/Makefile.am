#
#      Makefile.am for lib/morph
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitemorph.la

MANF =  mkMorphForm.3 mkMorphHeight.3 MorphScale.3 \
	morphErode.3 morphClose.3 morphOpen.3 morphDilate.3

man3_MANS=$(MANF)

libxitemorph_la_SOURCES = morphDilate.c morphErode.c morphOpen.c morphClose.c mkMorph.c

mkMorphForm.3:	mkMorph.3
mkMorphHeight.3:	mkMorph.3
MorphScale.3:	mkMorph.3
morphErode.3:	morphDilate.3
morphClose.3:	morphDilate.3
morphOpen.3:	morphDilate.3
