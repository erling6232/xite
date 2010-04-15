#
#      Makefile.am for src/noise
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = mkGauss addGauss multGauss mkPoisson addPoisson
MANP  = mkGauss.1 addGauss.1 multGauss.1 mkPoisson.1 addPoisson.1
CINCL =  noise.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

mkGauss_SOURCES    = mkGauss_main.c
addGauss_SOURCES   = addGauss_main.c
multGauss_SOURCES  = multGauss_main.c
mkPoisson_SOURCES  = mkPoisson_main.c
addPoisson_SOURCES = addPoisson_main.c
