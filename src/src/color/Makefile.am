#
#      Makefile.am for src/color
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG    = colorquant mct saturmod color2ascii
XPROG   =
MANP    = colorquant.1 mct.1 saturmod.1 color2ascii.1
MANP_SH = makepseudo.1
CINCL   = color.i
# SCRIPTS = makepseudo

bin_PROGRAMS=$(PROG) $(XPROG)
bin_SCRIPTS = makepseudo
man1_MANS=$(MANP) $(MANP_SH)

LDADD = ../../lib/libxite.la

makepseudo:	makepseudo.sh
	cp -p $(srcdir)/makepseudo.sh makepseudo

makepseudo.m:	makepseudo.sh

color2ascii_SOURCES = color2ascii_main.c
colorquant_SOURCES  = colorquant_main.c
mct_SOURCES         = mct_main.c
saturmod_SOURCES    = saturmod_main.c
# makepseudo_SOURCES  = makepseudo.sh
