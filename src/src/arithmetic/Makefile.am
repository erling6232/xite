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

noinst_LTLIBRARIES = libxitearithmetic.la
libxitearithmetic_la_SOURCES = \
	absDiff.c addw.c average.c crossSection.c divide.c \
	logical.c multiply.c profile.c rms.c scale.c signDiff.c \
	square.c squareRoot.c real.c imag.c power.c phase.c \
	logarithm.c
# LDADD = ../../lib/libxite.la

absDiff_SOURCES = absDiff_main.c arithmetic.h
addw_SOURCES = addw_main.c arithmetic.h
average_SOURCES = average_main.c arithmetic.h
crossSection_SOURCES = crossSection_main.c arithmetic.h
divide_SOURCES = divide_main.c arithmetic.h
imag_SOURCES = imag_main.c arithmetic.h
logarithm_SOURCES = logarithm_main.c arithmetic.h
logical_SOURCES = logical_main.c arithmetic.h
multiply_SOURCES = multiply_main.c arithmetic.h
phase_SOURCES = phase_main.c arithmetic.h
power_SOURCES = power_main.c arithmetic.h
profile_SOURCES = profile_main.c arithmetic.h
real_SOURCES = real_main.c arithmetic.h
rms_SOURCES = rms_main.c arithmetic.h
scale_SOURCES = scale_main.c arithmetic.h
signDiff_SOURCES = signDiff_main.c arithmetic.h
square_SOURCES = square_main.c arithmetic.h
squareRoot_SOURCES = squareRoot_main.c arithmetic.h
xadd_SOURCES = xadd_main.c arithmetic.h
