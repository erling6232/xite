#
#      Makefile.am for src/arithmetic
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
noinst_LTLIBRARIES = libxitearithmetic.la

#MANF =  absDiff.3 addw.3    average.3   crossSection.3 divide.3   logical.3 \
#        multiply.3 profile.3 rms.3       scale.3        signDiff.3 square.3 \
#	squareRoot.3 real.3      imag.3         power.3    phase.3 \
#	logarithm.3
MANF =	absDiff.3 addw.3 average.3 colMax.3 colMaxArr.3 colMaxArrBand.3 colMaxBand.3 \
	colMin.3 colMinArr.3 colMinArrBand.3 colMinBand.3 column.3 columnBand.3 \
	crossSection.3 curveToBand.3 divide.3 fftDisplay.3 fftImag.3 fftMult.3 \
	fftPhase.3 fftPower.3 fftReal.3 imag.3 logarithm.3 logical.3 logical_and.3 \
	logical_band.3 logical_bandc.3 logical_bnot.3 logical_bor.3 logical_borc.3 \
	logical_bxor.3 logical_bxorc.3 logical_eq.3 logical_eqc.3 logical_ge.3 \
	logical_gec.3 logical_gt.3 logical_gtc.3 logical_le.3 logical_lec.3 \
	logical_lshift.3 logical_lt.3 logical_ltc.3 logical_mask.3 logical_ne.3 \
	logical_nec.3 logical_not.3 logical_or.3 logical_rshift.3 logical_xor.3 \
	multiply.3 phase.3 power.3 profile.3 real.3 rms.3 row.3 rowBand.3 rowMax.3 \
	rowMaxArr.3 rowMaxArrBand.3 rowMaxBand.3 rowMin.3 rowMinArr.3 rowMinArrBand.3 \
	rowMinBand.3 scale.3 signDiff.3 square.3 squareRoot.3 vectorPeak.3

man3_MANS=$(MANF)

libxitearithmetic_la_SOURCES = \
	absDiff.c addw.c average.c crossSection.c divide.c \
	logical.c multiply.c profile.c rms.c scale.c signDiff.c \
	square.c squareRoot.c real.c imag.c power.c phase.c \
	logarithm.c

vectorPeak.3:	crossSection.c
rowMax.3:	crossSection.c
colMax.3:	crossSection.c
rowMin.3:	crossSection.c
colMin.3:	crossSection.c
rowMaxBand.3:	crossSection.c
colMaxBand.3:	crossSection.c
rowMinBand.3:	crossSection.c
colMinBand.3:	crossSection.c
colMaxArr.3:	crossSection.c
rowMaxArr.3:	crossSection.c
colMinArr.3:	crossSection.c
rowMinArr.3:	crossSection.c
column.3:	crossSection.c
row.3:		crossSection.c
colMaxArrBand.3: crossSection.c
rowMaxArrBand.3: crossSection.c
colMinArrBand.3: crossSection.c
rowMinArrBand.3: crossSection.c
columnBand.3:	crossSection.c
rowBand.3:	crossSection.c
fftImag.3:	imag.c
fftDisplay.3:	logarithm.c
logical_and.3:	logical.c
logical_or.3:	logical.c
logical_xor.3:	logical.c
logical_not.3:	logical.c
logical_band.3:	logical.c
logical_bor.3:	logical.c
logical_bxor.3:	logical.c
logical_bnot.3:	logical.c
logical_lshift.3: logical.c
logical_rshift.3: logical.c
logical_eq.3:	logical.c
logical_ne.3:	logical.c
logical_lt.3:	logical.c
logical_le.3:	logical.c
logical_gt.3:	logical.c
logical_ge.3:	logical.c
logical_mask.3:	logical.c
logical_bandc.3: logical.c
logical_borc.3:	logical.c
logical_bxorc.3: logical.c
logical_eqc.3:	logical.c
logical_nec.3:	logical.c
logical_ltc.3:	logical.c
logical_lec.3:	logical.c
logical_gtc.3:	logical.c
logical_gec.3:	logical.c
fftMult.3:	multiply.c
fftPhase.3:	phase.c
fftPower.3:	power.c
fftReal.3:	real.c
