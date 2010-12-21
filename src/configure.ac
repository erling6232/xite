# Prelude
AC_PREREQ(2.50)
AC_INIT([xite], [3.4.5], [Erling.Andersen@Helse-Bergen.NO])
AC_CONFIG_SRCDIR([include/xite/blab.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_SED

# AM_PROG_CC_C_O

# Checks for libraries
AC_CHECK_LIB([X11],  [XBell])
# Internal xpm # AC_CHECK_LIB([xpm],  [xpmReadFile])
# AC_CHECK_LIB([Xaw],  [XawAsciiSave])
AC_CHECK_LIB([Xmu],  [XmuMakeAtom])
# AC_CHECK_LIB([fwf],  [XfwfCallProcessMenu])
AC_CHECK_LIB([Xt],   [XtConvert])
#AC_CHECK_LIB([Xext], [XextAddDisplay])
AC_CHECK_LIB([m],    [sqrt])

# Checks for header files
AC_PATH_X
AC_ARG_ENABLE(gui,
	[AS_HELP_STRING([--enable-gui=auto], [Select GUI: motif, athena or auto])],
	[gui="$enableval"], [gui=auto])
LIBGUI=
AS_IF([test "x$gui" = xauto],
	[AC_CHECK_LIB([Xaw],  [XawAsciiSave],
		[AC_SUBST([LIBGUI], ["-lfwf -lXaw -lXmu -lXt"])
		 AC_DEFINE([HAVE_LIBXAW], [1], [Define if you have libXaw])
		],
		[AC_CHECK_LIB([Xm],  [XmStringFree],
			[AC_SUBST([LIBGUI], ["-lfwf -lXm"])
			 AC_DEFINE([HAVE_LIBMOTIF], [1], [Define if you have Motif])
			 AC_DEFINE([MOTIF], [], [Define when you have Motif])
			],
			[AC_MSG_FAILURE([Motif test failed])
			])
		])
	],
      [test "x$gui" = xathena],
	[AC_CHECK_LIB([Xaw],  [XawAsciiSave],
		[AC_SUBST([LIBGUI], ["-lfwf -lXaw -lXmu -lXt"])
		 AC_DEFINE([HAVE_LIBXAW], [1], [Define if you have libXaw])
		],
	)],
      [test "x$gui" = xmotif],
	[AC_CHECK_LIB([Xm],  [XmStringFree],
		[AC_SUBST([LIBGUI], ["-lfwf -lXm"])
		 AC_DEFINE([HAVE_LIBMOTIF], [1], [Define if you have Motif])
		],
	)]
)

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([ctype.h fcntl.h sys/fcntl.h float.h io.h sys/io.h limits.h malloc.h math.h memory.h stddef.h stdio.h stdlib.h sys/file.h sys/param.h sys/stat.h sys/time.h sys/times.h time.h unistd.h values.h])
AC_CHECK_HEADERS([direct.h varargs.h stdarg.h pwd.h process.h])
AC_CHECK_HEADER([regex.h],
                [AC_DEFINE([HAVE_REGEX_H], [1], [Define if you have regex.h])]
                [AC_DEFINE([REGEX], [1], [Define if you have regex.h])],
		[AC_CHECK_HEADER([regexp.h],
		                 [AC_DEFINE([HAVE_REGEXP_H], [1], [Define if you have regexp.h])],
				 [AC_DEFINE([NO_REGEXP], [1], [Define if you dont have regex])]
				)
	       ])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_MEMBERS([struct passwd.pw_dir])

#AM_CFLAGS = -O3 -DSYSTEM_V -DSYSV -Dvfork=fork \
#                  $(DEBUG) -DDEF_STRCASECMP -DREGEX $(DIRENT)
# if test "$x" = "$y" ; then ...
#if HAVE_REGEX_H
#AM_CFLAGS += -DREGEX
#endif


# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_FUNC_WAIT3
AC_CHECK_FUNCS([dup2 getcwd getwd isascii pow regcomp select sqrt strcasecmp strchr strcspn strerror strrchr strspn strstr strtod strtol])
AC_CHECK_FUNCS([link setmode symlink rand random srand srandom toupper tolower umask])
AC_CHECK_FUNCS([clock flock mktemp mkstemp tempnam time times])

# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>


# Common Makefile.def
AC_SUBST(configdir, [\${top_srcdir}])
AC_SUBST(common_makefile, [Makefile.def])

# Output files
AC_CONFIG_FILES([
    Makefile
    data/Makefile
    data/img/Makefile
    data/xshow/Makefile
    etc/Makefile
    etc/app-defaults/Makefile
    include/Makefile
    include/matlab/Makefile
    include/X11/Makefile
    include/xite/Makefile
    lib/Makefile
    lib/arithmetic/Makefile
    lib/biff/Makefile
    lib/binarize/Makefile
    lib/canny/Makefile
    lib/cdoc/Makefile
    lib/classification/Makefile
    lib/color/Makefile
    lib/combine/Makefile
    lib/contour/Makefile
    lib/convert/Makefile
    lib/convolve/Makefile
    lib/copy/Makefile
    lib/dither/Makefile
    lib/draw/Makefile
    lib/extrema/Makefile
    lib/fft/Makefile
    lib/fht/Makefile
    lib/fractal/Makefile
    lib/geometry/Makefile
    lib/gradInv/Makefile
    lib/histo/Makefile
    lib/hough/Makefile
    lib/haar/Makefile
    lib/ideal/Makefile
    lib/ihs/Makefile
    lib/Image/Makefile
    lib/kncn/Makefile
    lib/knn/Makefile
    lib/matlab/Makefile
    lib/median/Makefile
    lib/minmax/Makefile
    lib/morph/Makefile
    lib/negate/Makefile
    lib/noise/Makefile
    lib/pnm/Makefile
    lib/principal/Makefile
    lib/pyramid/Makefile
    lib/region/Makefile
    lib/resample/Makefile
    lib/scatter/Makefile
    lib/segmentation/Makefile
    lib/sigma/Makefile
    lib/snn/Makefile
    lib/statistics/Makefile
    lib/stdiff/Makefile
    lib/sunraster/Makefile
    lib/texture/Makefile
    lib/thin/Makefile
    lib/threshold/Makefile
    lib/thresMl/Makefile
    lib/tiff/Makefile
    lib/utils/Makefile
    lib/ximage/Makefile
    lib/xshow/Makefile
    lib/zernike/Makefile
    src/Makefile
    src/arithmetic/Makefile
    src/biff/Makefile
    src/binarize/Makefile
    src/canny/Makefile
    src/cdoc/Makefile
    src/classification/Makefile
    src/color/Makefile
    src/combine/Makefile
    src/contour/Makefile
    src/convert/Makefile
    src/convolve/Makefile
    src/copy/Makefile
    src/dic2biff/Makefile
    src/dither/Makefile
    src/draw/Makefile
    src/extrema/Makefile
    src/fft/Makefile
    src/fht/Makefile
    src/fractal/Makefile
    src/geometry/Makefile
    src/gradInv/Makefile
    src/histo/Makefile
    src/hough/Makefile
    src/haar/Makefile
    src/ideal/Makefile
    src/ihs/Makefile
    src/kncn/Makefile
    src/knn/Makefile
    src/matlab/Makefile
    src/median/Makefile
    src/minmax/Makefile
    src/morph/Makefile
    src/negate/Makefile
    src/noise/Makefile
    src/pnm/Makefile
    src/PostScript/Makefile
    src/principal/Makefile
    src/pyramid/Makefile
    src/region/Makefile
    src/resample/Makefile
    src/scatter/Makefile
    src/scripts/Makefile
    src/segmentation/Makefile
    src/sigma/Makefile
    src/snn/Makefile
    src/statistics/Makefile
    src/stdiff/Makefile
    src/sunraster/Makefile
    src/texture/Makefile
    src/thin/Makefile
    src/threshold/Makefile
    src/thresMl/Makefile
    src/tiff/Makefile
    src/ximage/Makefile
    src/xshow/Makefile
    src/zernike/Makefile
    ])
    #lib/xpm/Makefile

AC_CONFIG_LINKS([src/arithmetic/absDiff.c:src/arithmetic/absDiff_main.c])
AC_CONFIG_LINKS([src/arithmetic/addw.c:src/arithmetic/addw_main.c])
AC_CONFIG_LINKS([src/arithmetic/average.c:src/arithmetic/average_main.c])
AC_CONFIG_LINKS([src/arithmetic/crossSection.c:src/arithmetic/crossSection_main.c])
AC_CONFIG_LINKS([src/arithmetic/divide.c:src/arithmetic/divide_main.c])
AC_CONFIG_LINKS([src/arithmetic/imag.c:src/arithmetic/imag_main.c])
AC_CONFIG_LINKS([src/arithmetic/logarithm.c:src/arithmetic/logarithm_main.c])
AC_CONFIG_LINKS([src/arithmetic/logical.c:src/arithmetic/logical_main.c])
AC_CONFIG_LINKS([src/arithmetic/multiply.c:src/arithmetic/multiply_main.c])
AC_CONFIG_LINKS([src/arithmetic/phase.c:src/arithmetic/phase_main.c])
AC_CONFIG_LINKS([src/arithmetic/power.c:src/arithmetic/power_main.c])
AC_CONFIG_LINKS([src/arithmetic/profile.c:src/arithmetic/profile_main.c])
AC_CONFIG_LINKS([src/arithmetic/real.c:src/arithmetic/real_main.c])
AC_CONFIG_LINKS([src/arithmetic/rms.c:src/arithmetic/rms_main.c])
AC_CONFIG_LINKS([src/arithmetic/scale.c:src/arithmetic/scale_main.c])
AC_CONFIG_LINKS([src/arithmetic/signDiff.c:src/arithmetic/signDiff_main.c])
AC_CONFIG_LINKS([src/arithmetic/square.c:src/arithmetic/square_main.c])
AC_CONFIG_LINKS([src/arithmetic/squareRoot.c:src/arithmetic/squareRoot_main.c])
AC_CONFIG_LINKS([src/arithmetic/xadd.c:src/arithmetic/xadd_main.c])
AC_CONFIG_LINKS([src/biff/biffcopy.c:src/biff/biffcopy_main.c])
AC_CONFIG_LINKS([src/biff/biffinfo.c:src/biff/biffinfo_main.c])
AC_CONFIG_LINKS([src/biff/biffmerge.c:src/biff/biffmerge_main.c])
AC_CONFIG_LINKS([src/biff/bifftext.c:src/biff/bifftext_main.c])
AC_CONFIG_LINKS([src/biff/newtitle.c:src/biff/newtitle_main.c])
AC_CONFIG_LINKS([src/binarize/binarize.c:src/binarize/binarize_main.c])
AC_CONFIG_LINKS([src/binarize/expand_edge.c:src/binarize/expand_edge_main.c])
AC_CONFIG_LINKS([src/canny/canny.c:src/canny/canny_main.c])
AC_CONFIG_LINKS([src/cdoc/add_html_toc.c:src/cdoc/add_html_toc_main.c])
AC_CONFIG_LINKS([src/cdoc/cdoc.c:src/cdoc/cdoc_main.c])
AC_CONFIG_LINKS([src/classification/isoCluster.c:src/classification/isoCluster_main.c])
AC_CONFIG_LINKS([src/classification/kMeansCluster.c:src/classification/kMeansCluster_main.c])
AC_CONFIG_LINKS([src/color/color2ascii.c:src/color/color2ascii_main.c])
AC_CONFIG_LINKS([src/color/colorquant.c:src/color/colorquant_main.c])
AC_CONFIG_LINKS([src/color/mct.c:src/color/mct_main.c])
AC_CONFIG_LINKS([src/color/saturmod.c:src/color/saturmod_main.c])
AC_CONFIG_LINKS([src/combine/combine.c:src/combine/combine_main.c])
AC_CONFIG_LINKS([src/contour/contour.c:src/contour/contour_main.c])
AC_CONFIG_LINKS([src/convert/ascii2biff.c:src/convert/ascii2biff_main.c])
AC_CONFIG_LINKS([src/convert/biff2ascii.c:src/convert/biff2ascii_main.c])
AC_CONFIG_LINKS([src/convert/biff2raw.c:src/convert/biff2raw_main.c])
AC_CONFIG_LINKS([src/convert/biffConvert.c:src/convert/biffConvert_main.c])
AC_CONFIG_LINKS([src/convert/biffswap.c:src/convert/biffswap_main.c])
AC_CONFIG_LINKS([src/convert/pseudo2rgb.c:src/convert/pseudo2rgb_main.c])
AC_CONFIG_LINKS([src/convert/raw2biff.c:src/convert/raw2biff_main.c])
AC_CONFIG_LINKS([src/convolve/conv3x3.c:src/convolve/conv3x3_main.c])
AC_CONFIG_LINKS([src/convolve/convolve.c:src/convolve/convolve_main.c])
AC_CONFIG_LINKS([src/convolve/crossing.c:src/convolve/crossing_main.c])
AC_CONFIG_LINKS([src/convolve/gradient.c:src/convolve/gradient_main.c])
AC_CONFIG_LINKS([src/convolve/grad.c:src/convolve/grad_main.c])
AC_CONFIG_LINKS([src/convolve/laplace.c:src/convolve/laplace_main.c])
AC_CONFIG_LINKS([src/convolve/lapZeroCross.c:src/convolve/lapZeroCross_main.c])
AC_CONFIG_LINKS([src/convolve/prewitt.c:src/convolve/prewitt_main.c])
AC_CONFIG_LINKS([src/convolve/roberts.c:src/convolve/roberts_main.c])
AC_CONFIG_LINKS([src/convolve/sobel.c:src/convolve/sobel_main.c])
AC_CONFIG_LINKS([src/copy/biffDump.c:src/copy/biffDump_main.c])
AC_CONFIG_LINKS([src/copy/mosaic.c:src/copy/mosaic_main.c])
AC_CONFIG_LINKS([src/copy/overlap.c:src/copy/overlap_main.c])
AC_CONFIG_LINKS([src/copy/subcopy.c:src/copy/subcopy_main.c])
AC_CONFIG_LINKS([src/dither/dither.c:src/dither/dither_main.c])
AC_CONFIG_LINKS([src/draw/bdf2biff.c:src/draw/bdf2biff_main.c])
AC_CONFIG_LINKS([src/draw/biffwrite.c:src/draw/biffwrite_main.c])
AC_CONFIG_LINKS([src/draw/mkImg.c:src/draw/mkImg_main.c])
AC_CONFIG_LINKS([src/extrema/extrema1.c:src/extrema/extrema1_main.c])
AC_CONFIG_LINKS([src/extrema/extrema2.c:src/extrema/extrema2_main.c])
AC_CONFIG_LINKS([src/extrema/maxima.c:src/extrema/maxima_main.c])
AC_CONFIG_LINKS([src/extrema/minima.c:src/extrema/minima_main.c])
AC_CONFIG_LINKS([src/fft/fft2d.c:src/fft/fft2d_main.c])
AC_CONFIG_LINKS([src/fft/fftMagPhase.c:src/fft/fftMagPhase_main.c])
AC_CONFIG_LINKS([src/fft/xfft.c:src/fft/xfft_main.c])
AC_CONFIG_LINKS([src/fht/fht2d.c:src/fht/fht2d_main.c])
AC_CONFIG_LINKS([src/fht/fhtPhase.c:src/fht/fhtPhase_main.c])
AC_CONFIG_LINKS([src/fht/fhtPower.c:src/fht/fhtPower_main.c])
AC_CONFIG_LINKS([src/fht/ft2ht.c:src/fht/ft2ht_main.c])
AC_CONFIG_LINKS([src/fht/ht2ft.c:src/fht/ht2ft_main.c])
AC_CONFIG_LINKS([src/fractal/minmaxTexture.c:src/fractal/minmaxTexture_main.c])
AC_CONFIG_LINKS([src/fractal/mkFractalSra.c:src/fractal/mkFractalSra_main.c])
AC_CONFIG_LINKS([src/geometry/affine.c:src/geometry/affine_main.c])
AC_CONFIG_LINKS([src/geometry/quadratic.c:src/geometry/quadratic_main.c])
AC_CONFIG_LINKS([src/geometry/reorganize.c:src/geometry/reorganize_main.c])
AC_CONFIG_LINKS([src/geometry/rotate.c:src/geometry/rotate_main.c])
AC_CONFIG_LINKS([src/geometry/shift_img.c:src/geometry/shift_img_main.c])
AC_CONFIG_LINKS([src/geometry/transpose_img.c:src/geometry/transpose_img_main.c])
AC_CONFIG_LINKS([src/geometry/warp.c:src/geometry/warp_main.c])
AC_CONFIG_LINKS([src/geometry/xwarp.c:src/geometry/xwarp_main.c])
AC_CONFIG_LINKS([src/gradInv/gradInv.c:src/gradInv/gradInv_main.c])
AC_CONFIG_LINKS([src/histo/gammaAdjust.c:src/histo/gammaAdjust_main.c])
AC_CONFIG_LINKS([src/histo/histoEqCol.c:src/histo/histoEqCol_main.c])
AC_CONFIG_LINKS([src/histo/histoEq.c:src/histo/histoEq_main.c])
AC_CONFIG_LINKS([src/histo/histoNorm.c:src/histo/histoNorm_main.c])
AC_CONFIG_LINKS([src/histo/mkHisto.c:src/histo/mkHisto_main.c])
AC_CONFIG_LINKS([src/histo/pixel_mapper.c:src/histo/pixel_mapper_main.c])
AC_CONFIG_LINKS([src/hough/houghCircle.c:src/hough/houghCircle_main.c])
AC_CONFIG_LINKS([src/hough/houghLine.c:src/hough/houghLine_main.c])
AC_CONFIG_LINKS([src/haar/haar2d.c:src/haar/haar2d_main.c])
AC_CONFIG_LINKS([src/haar/haarTexture.c:src/haar/haarTexture_main.c])
AC_CONFIG_LINKS([src/ideal/bandpass.c:src/ideal/bandpass_main.c])
AC_CONFIG_LINKS([src/ideal/bandstop.c:src/ideal/bandstop_main.c])
AC_CONFIG_LINKS([src/ideal/butterworth.c:src/ideal/butterworth_main.c])
AC_CONFIG_LINKS([src/ideal/exponential.c:src/ideal/exponential_main.c])
AC_CONFIG_LINKS([src/ideal/highpass.c:src/ideal/highpass_main.c])
AC_CONFIG_LINKS([src/ideal/lowpassIdeal.c:src/ideal/lowpassIdeal_main.c])
AC_CONFIG_LINKS([src/ideal/lowpass.c:src/ideal/lowpass_main.c])
AC_CONFIG_LINKS([src/ideal/window.c:src/ideal/window_main.c])
AC_CONFIG_LINKS([src/ideal/xfilter.c:src/ideal/xfilter_main.c])
AC_CONFIG_LINKS([src/ihs/ihs2rgb.c:src/ihs/ihs2rgb_main.c])
AC_CONFIG_LINKS([src/ihs/rainbow.c:src/ihs/rainbow_main.c])
AC_CONFIG_LINKS([src/ihs/rgb2ihs.c:src/ihs/rgb2ihs_main.c])
AC_CONFIG_LINKS([src/kncn/kncn.c:src/kncn/kncn_main.c])
AC_CONFIG_LINKS([src/kncn/mbkncn.c:src/kncn/mbkncn_main.c])
AC_CONFIG_LINKS([src/knn/knn.c:src/knn/knn_main.c])
AC_CONFIG_LINKS([src/knn/mbknn.c:src/knn/mbknn_main.c])
AC_CONFIG_LINKS([src/matlab/biff2matlab.c:src/matlab/biff2matlab_main.c])
AC_CONFIG_LINKS([src/matlab/matlab2biff.c:src/matlab/matlab2biff_main.c])
AC_CONFIG_LINKS([src/median/fractile.c:src/median/fractile_main.c])
AC_CONFIG_LINKS([src/median/lit.c:src/median/lit_main.c])
AC_CONFIG_LINKS([src/median/maxHisto.c:src/median/maxHisto_main.c])
AC_CONFIG_LINKS([src/median/median.c:src/median/median_main.c])
AC_CONFIG_LINKS([src/median/mode.c:src/median/mode_main.c])
AC_CONFIG_LINKS([src/median/pseudomedian3x3.c:src/median/pseudomedian3x3_main.c])
AC_CONFIG_LINKS([src/median/rank.c:src/median/rank_main.c])
AC_CONFIG_LINKS([src/minmax/closestMinMax.c:src/minmax/closestMinMax_main.c])
AC_CONFIG_LINKS([src/minmax/maxarea.c:src/minmax/maxarea_main.c])
AC_CONFIG_LINKS([src/minmax/minarea.c:src/minmax/minarea_main.c])
AC_CONFIG_LINKS([src/morph/mkMorph.c:src/morph/mkMorph_main.c])
AC_CONFIG_LINKS([src/morph/morphClose.c:src/morph/morphClose_main.c])
AC_CONFIG_LINKS([src/morph/morphDilate.c:src/morph/morphDilate_main.c])
AC_CONFIG_LINKS([src/morph/morphErode.c:src/morph/morphErode_main.c])
AC_CONFIG_LINKS([src/morph/morphOpen.c:src/morph/morphOpen_main.c])
AC_CONFIG_LINKS([src/negate/absValue.c:src/negate/absValue_main.c])
AC_CONFIG_LINKS([src/negate/complexConjug.c:src/negate/complexConjug_main.c])
AC_CONFIG_LINKS([src/negate/negate.c:src/negate/negate_main.c])
AC_CONFIG_LINKS([src/noise/addGauss.c:src/noise/addGauss_main.c])
AC_CONFIG_LINKS([src/noise/addPoisson.c:src/noise/addPoisson_main.c])
AC_CONFIG_LINKS([src/noise/mkGauss.c:src/noise/mkGauss_main.c])
AC_CONFIG_LINKS([src/noise/mkPoisson.c:src/noise/mkPoisson_main.c])
AC_CONFIG_LINKS([src/noise/multGauss.c:src/noise/multGauss_main.c])
AC_CONFIG_LINKS([src/pnm/biff2pnm.c:src/pnm/biff2pnm_main.c])
AC_CONFIG_LINKS([src/pnm/pnm2biff.c:src/pnm/pnm2biff_main.c])
AC_CONFIG_LINKS([src/PostScript/biff2ps.c:src/PostScript/biff2ps_main.c])
AC_CONFIG_LINKS([src/PostScript/histo2ps.c:src/PostScript/histo2ps_main.c])
AC_CONFIG_LINKS([src/principal/linearTrans.c:src/principal/linearTrans_main.c])
AC_CONFIG_LINKS([src/pyramid/pyramid.c:src/pyramid/pyramid_main.c])
AC_CONFIG_LINKS([src/pyramid/xpyramid.c:src/pyramid/xpyramid_main.c])
AC_CONFIG_LINKS([src/region/regionAnalyse.c:src/region/regionAnalyse_main.c])
AC_CONFIG_LINKS([src/region/regionConvexHull.c:src/region/regionConvexHull_main.c])
AC_CONFIG_LINKS([src/region/xregion.c:src/region/xregion_main.c])
AC_CONFIG_LINKS([src/resample/resample.c:src/resample/resample_main.c])
AC_CONFIG_LINKS([src/scatter/scatter.c:src/scatter/scatter_main.c])
AC_CONFIG_LINKS([src/segmentation/segmRandom.c:src/segmentation/segmRandom_main.c])
AC_CONFIG_LINKS([src/segmentation/segmSpannWilson.c:src/segmentation/segmSpannWilson_main.c])
AC_CONFIG_LINKS([src/sigma/sigma.c:src/sigma/sigma_main.c])
AC_CONFIG_LINKS([src/snn/litSnn.c:src/snn/litSnn_main.c])
AC_CONFIG_LINKS([src/snn/snn.c:src/snn/snn_main.c])
AC_CONFIG_LINKS([src/statistics/histoMeanStd.c:src/statistics/histoMeanStd_main.c])
AC_CONFIG_LINKS([src/statistics/image_minmax.c:src/statistics/image_minmax_main.c])
AC_CONFIG_LINKS([src/statistics/statistics.c:src/statistics/statistics_main.c])
AC_CONFIG_LINKS([src/stdiff/maxSimilar3x3.c:src/stdiff/maxSimilar3x3_main.c])
AC_CONFIG_LINKS([src/stdiff/mean.c:src/stdiff/mean_main.c])
AC_CONFIG_LINKS([src/stdiff/stdev.c:src/stdiff/stdev_main.c])
AC_CONFIG_LINKS([src/stdiff/stdiff.c:src/stdiff/stdiff_main.c])
AC_CONFIG_LINKS([src/stdiff/stvar.c:src/stdiff/stvar_main.c])
AC_CONFIG_LINKS([src/sunraster/biff2sunraster.c:src/sunraster/biff2sunraster_main.c])
AC_CONFIG_LINKS([src/sunraster/sunraster2biff.c:src/sunraster/sunraster2biff_main.c])
AC_CONFIG_LINKS([src/texture/glcm.c:src/texture/glcm_main.c])
AC_CONFIG_LINKS([src/texture/glcmParameter.c:src/texture/glcmParameter_main.c])
AC_CONFIG_LINKS([src/texture/glRunLength.c:src/texture/glRunLength_main.c])
AC_CONFIG_LINKS([src/texture/peanoScan.c:src/texture/peanoScan_main.c])
AC_CONFIG_LINKS([src/thin/thn_har.c:src/thin/thn_har_main.c])
AC_CONFIG_LINKS([src/thin/thn_lc.c:src/thin/thn_lc_main.c])
AC_CONFIG_LINKS([src/thin/thn_zs.c:src/thin/thn_zs_main.c])
AC_CONFIG_LINKS([src/threshold/thresBernsen.c:src/threshold/thresBernsen_main.c])
AC_CONFIG_LINKS([src/threshold/threshold.c:src/threshold/threshold_main.c])
AC_CONFIG_LINKS([src/threshold/thresLloyd.c:src/threshold/thresLloyd_main.c])
AC_CONFIG_LINKS([src/threshold/thresMinErr.c:src/threshold/thresMinErr_main.c])
AC_CONFIG_LINKS([src/threshold/thresOtsu.c:src/threshold/thresOtsu_main.c])
AC_CONFIG_LINKS([src/threshold/thresPtile.c:src/threshold/thresPtile_main.c])
AC_CONFIG_LINKS([src/threshold/thresRidCal.c:src/threshold/thresRidCal_main.c])
AC_CONFIG_LINKS([src/threshold/thresSigma.c:src/threshold/thresSigma_main.c])
AC_CONFIG_LINKS([src/threshold/thresUniErr.c:src/threshold/thresUniErr_main.c])
AC_CONFIG_LINKS([src/thresMl/thresMlCentroid.c:src/thresMl/thresMlCentroid_main.c])
AC_CONFIG_LINKS([src/thresMl/thresMlComCur.c:src/thresMl/thresMlComCur_main.c])
AC_CONFIG_LINKS([src/thresMl/thresMl.c:src/thresMl/thresMl_main.c])
AC_CONFIG_LINKS([src/thresMl/thresMlReddi.c:src/thresMl/thresMlReddi_main.c])
AC_CONFIG_LINKS([src/thresMl/thresMlWaHa.c:src/thresMl/thresMlWaHa_main.c])
AC_CONFIG_LINKS([src/tiff/biff2tiff.c:src/tiff/biff2tiff_main.c])
AC_CONFIG_LINKS([src/tiff/tiff2biff.c:src/tiff/tiff2biff_main.c])
AC_CONFIG_LINKS([src/ximage/xhistogram.c:src/ximage/xhistogram_main.c])
AC_CONFIG_LINKS([src/ximage/ximage_example.c:src/ximage/ximage_example_main.c])
AC_CONFIG_LINKS([src/ximage/xmovie.c:src/ximage/xmovie_main.c])
AC_CONFIG_LINKS([src/xshow/fork_xshow.c:src/xshow/fork_xshow_main.c])
AC_CONFIG_LINKS([src/xshow/ReadMenuList.c:src/xshow/ReadMenuList_main.c])
AC_CONFIG_LINKS([src/xshow/xshow.c:src/xshow/xshow_main.c])
AC_CONFIG_LINKS([src/zernike/zernike.c:src/zernike/zernike_main.c])

AC_OUTPUT
