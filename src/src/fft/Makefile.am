#
#      Makefile.am for src/fft
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG    = fft2d fftMagPhase
XPROG   = xfft
#SCRIPTS = fftReal fftImag fftAmp fftPhase fftPower fftMult fftMakeLp \
#	  fftMakeHp fftMakeBp fftMakeBs fftDisplay
MANP    = fft2d.m xfft.m fftMagPhase.m
CINCL   = fft.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

fft2d_SOURCES       = fft2d_main.c
fftMagPhase_SOURCES = fftMagPhase_main.c
xfft_SOURCES        = xfft_main.c
