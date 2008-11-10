#
#      Makefile.am for src/fht
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = fht2d ht2ft ft2ht fhtPower fhtPhase
XPROG =
MANP  = fht2d.m ht2ft.m ft2ht.m fhtPower.m fhtPhase.m
CINCL =  fht.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

fht2d_SOURCES     = fht2d_main.c
ht2ft_SOURCES     = ht2ft_main.c
ft2ht_SOURCES     = ft2ht_main.c
fhtPower_SOURCES  = fhtPower_main.c
fhtPhase_SOURCES  = fhtPhase_main.c
