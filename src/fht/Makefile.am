#
#      Makefile.am for src/fht
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = fht2d ht2ft ft2ht fhtPower fhtPhase
XPROG =
MANP  = fht2d.1 ht2ft.1 ft2ht.1 fhtPower.1 fhtPhase.1
CINCL =  fht.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

fht2d_SOURCES     = fht2d_main.c
ht2ft_SOURCES     = ht2ft_main.c
ft2ht_SOURCES     = ft2ht_main.c
fhtPower_SOURCES  = fhtPower_main.c
fhtPhase_SOURCES  = fhtPhase_main.c
