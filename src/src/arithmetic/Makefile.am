#
#      Makefile.am for src/arithmetic
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions for arithmetic
#
PROG  = absDiff    addw      average     crossSection   divide     logical \
	multiply   profile   rms         scale          signDiff   square  \
	squareRoot real      imag        power          phase      logarithm
XPROG = xadd
MANP  = absDiff.m  addw.m    average.m   crossSection.m divide.m   logical.m \
	multiply.m profile.m rms.m       scale.m        signDiff.m square.m \
	squareRoot.m xadd.m  real.m      imag.m         power.m    phase.m \
	logarithm.m
CINCL = arithmetic.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

absDiff_SOURCES = absDiff_main.c
addw_SOURCES = addw_main.c
average_SOURCES = average_main.c
crossSection_SOURCES = crossSection_main.c
divide_SOURCES = divide_main.c
imag_SOURCES = imag_main.c
logarithm_SOURCES = logarithm_main.c
logical_SOURCES = logical_main.c
multiply_SOURCES = multiply_main.c
phase_SOURCES = phase_main.c
power_SOURCES = power_main.c
profile_SOURCES = profile_main.c
real_SOURCES = real_main.c
rms_SOURCES = rms_main.c
scale_SOURCES = scale_main.c
signDiff_SOURCES = signDiff_main.c
square_SOURCES = square_main.c
squareRoot_SOURCES = squareRoot_main.c
xadd_SOURCES = xadd_main.c
xadd_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
