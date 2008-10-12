#
#      Makefile.am for src/arithmetic
#


@SET_MAKE@

SHELL = /bin/sh
VPATH = @srcdir@:@top_srcdir@/include:@configdir@/include
srcdir = @srcdir@
top_srcdir = @top_srcdir@
configdir = @configdir@

include $(configdir)/@common_makefile@

# infodir = $(prefix)/info


# all: install-include install-bin

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
include_HEADERS=arithmetic.h

absDiff_SOURCES = absDiff_main.c absDiff.c arithmetic.h
addw_SOURCES = addw_main.c addw.c arithmetic.h
average_SOURCES = average_main.c average.c arithmetic.h
crossSection_SOURCES = crossSection_main.c crossSection.c arithmetic.h
divide_SOURCES = divide_main.c divide.c arithmetic.h
imag_SOURCES = imag_main.c imag.c arithmetic.h
logarithm_SOURCES = logarithm_main.c logarithm.c arithmetic.h
logical_SOURCES = logical_main.c logical.c arithmetic.h
multiply_SOURCES = multiply_main.c multiply.c arithmetic.h
phase_SOURCES = phase_main.c phase.c arithmetic.h
power_SOURCES = power_main.c power.c arithmetic.h
profile_SOURCES = profile_main.c profile.c arithmetic.h
real_SOURCES = real_main.c real.c arithmetic.h
rms_SOURCES = rms_main.c rms.c arithmetic.h
scale_SOURCES = scale_main.c scale.c arithmetic.h
signDiff_SOURCES = signDiff_main.c signDiff.c arithmetic.h
square_SOURCES = square_main.c square.c arithmetic.h
squareRoot_SOURCES = squareRoot_main.c squareRoot.c arithmetic.h
xadd_SOURCES = xadd_main.c arithmetic.h
