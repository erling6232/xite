#
#      Makefile.am for src/arithmetic
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for arithmetic
#
PROG  = absDiff    addw      average     crossSection   divide     logical \
	profile   rms         scale          signDiff   \
	squareRoot real      imag        power          phase      logarithm
XPROG = xadd
MANP  = absDiff.1  addw.1    average.1   crossSection.1 divide.1   logical.1 \
	profile.1 rms.1       scale.1        signDiff.1 \
	squareRoot.1 xadd.1  real.1      imag.1         power.1    phase.1 \
	logarithm.1
#MANP = fftImag.1 fftMult.1 fftPhase.1 fftPower.1 fftReal.1
CINCL = arithmetic.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

absDiff_SOURCES = absDiff_main.c
addw_SOURCES = addw_main.c
average_SOURCES = average_main.c
crossSection_SOURCES = crossSection_main.c
divide_SOURCES = divide_main.c
imag_SOURCES = imag_main.c
logarithm_SOURCES = logarithm_main.c
logical_SOURCES = logical_main.c
phase_SOURCES = phase_main.c
power_SOURCES = power_main.c
profile_SOURCES = profile_main.c
real_SOURCES = real_main.c
rms_SOURCES = rms_main.c
scale_SOURCES = scale_main.c
signDiff_SOURCES = signDiff_main.c
squareRoot_SOURCES = squareRoot_main.c
xadd_SOURCES = xadd_main.c
xadd_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
