#
#      Makefile.am for src/stdiff
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = maxSimilar3x3 mean stdev stdiff stvar
MANP  = maxSimilar3x3.m mean.m stdev.m stdiff.m stvar.m
CINCL =  stdiff.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

maxSimilar3x3_SOURCES = maxSimilar3x3_main.c
mean_SOURCES          = mean_main.c
stdev_SOURCES         = stdev_main.c
stdiff_SOURCES        = stdiff_main.c
stvar_SOURCES         = stvar_main.c
