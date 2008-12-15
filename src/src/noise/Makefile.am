#
#      Makefile.am for src/noise
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = mkGauss addGauss multGauss mkPoisson addPoisson
MANP  = mkGauss.m addGauss.m multGauss.m mkPoisson.m addPoisson.m
CINCL =  noise.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

mkGauss_SOURCES    = mkGauss_main.c
addGauss_SOURCES   = addGauss_main.c
multGauss_SOURCES  = multGauss_main.c
mkPoisson_SOURCES  = mkPoisson_main.c
addPoisson_SOURCES = addPoisson_main.c
