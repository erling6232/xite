Summary: X-based Image processing Tools and Environment
Name: xite-suite
Version: 3.4.4
Release: 1
Group: Productivity/Scientific/Physics
Copyright: Copyright 1990-1999, Blab, UiO
Packager: Erling Andersen, Haukeland University Hospital, Bergen, Norway <Erling.Andersen@Helse-Bergen.NO>
URL: http://www.ifi.uio.no/forskning/grupper/dsb/Programvare/Xite/
Source: xite3_44.tar.gz
Vendor: Helse Bergen HF, Bergen, Norway

%description
XITE consists of display programs with image widget and graphical user
interface as well as more than 200 command line programs and 600 subroutines,
all documented on-line. XITE uses its own image file format (BIFF), but
comes with converters between BIFF and TIFF, pnm, sunraster, raw, MATLAB and
ascii.

The command line programs and subroutine library are written in C and run
under UNIX, Windows NT and Windows 95.

The display programs run under UNIX. They work with images of arbitrary size
and pixel type on 8-bit PseudoColor and 24-bit DirectColor and TrueColor X11
displays. Images can be zoomed and panned, and colortables can be selected
from a menu. The main display program, xshow, gives access to most of the
other command line programs via a menu interface which the user can customize
and extend to include local programs. Input images for the menu entries
can be selected with the mouse, and output images appear on the display.

The available programs include operations such as statistics, merging,
resampling, arithmetic/logical/relational operations, rotation, mirroring,
affine transformations, convolution, filter design, Fourier/Hartley/Haar/Hough
transforms, color manipulation, histogram transformations, global and
local thresholding, binary thinning, edge detection, morphological operations,
classification, image analysis and texture estimation.

A toolkit is supplied to simplify development of X11 based applications.

%package -n xite
Summary: User applications
Group: Productivity/Scientific/Physics
Requires: libxite, libfwf

%description -n xite
This package contains command line and graphical applications, like xshow.

%package -n xite-doc
Summary: Documentation for the xite and libxite packages
Group: Documentation/Other

%description -n xite-doc
Documentation for the xite and libxite packages. In HTML, PostScript
and PDF format.

%package -n libxite-devel
Summary: Development libraries and header files
Group: Development/Libraries/C and C++
Requires: libxite, xdevel, xinclude

%description -n libxite-devel
This development package contains the static libraries and header files
for the XITE image processing routines and for the BIFF image file format.

%package -n libxite
Summary: Shared libraries for XITE
Group: System/Libraries

%description -n libxite
This package contains the shared libraries used by XITE.

%package -n libfwf
Summary: Shared libraries for Xfwf widgets
Group: System/Libraries

%description -n libfwf
This package contains the shared libraries used by the
Xfwf Free Widget Foundation widgets.

%package -n libfwf-devel
Summary: Development libraries and header files for Xfwf widgets
Group: Development/Libraries/C and C++
Requires: libfwf, xdevel, xinclude, libxite-devel

%description -n libfwf-devel
This development package contains the static libraries and header files
for the Xfwf Free Widget Foundation widgets.

%build
%prep
%setup -n xite
tar cf xite.tar *

# Modify install_xite to make man pages
patch -p0 < rpm/install_xite_rpm.patch
cp install_xite install_xite_rpm

chmod a+rx install_xite_rpm

# install_xite will not run when some installation directories are missing
mkdir -p /etc/xite
mkdir -p /usr/include/xite
mkdir -p /usr/share/doc/packages/xite

./install_xite_rpm -x `pwd` -b /usr/bin -l `pwd`/lib \
	-i /usr/include \
	-m /usr/share/man -e /usr/share/doc/packages/xite \
	-xi /usr/include -xl /usr/X11R6/lib \
	-ti /usr/include -tl /usr/lib \
	-pi /usr/include -pl /usr/lib \
        -mh - -mi - -ml - -mm - \
        -p Linux -cc cc \
        -t xite.tar \
        -nomail \
        -s -c

# Do not use -rpath when linking
patch -p0 < debian/makeMakefile_no_rpath.patch

# Modified startProg.sh script to define XSHOWPATH
patch -p0 < debian/startProg.patch

LD_LIBRARY_PATH=`pwd`/lib\:$LD_LIBRARY_PATH ./install_xite_rpm -p Linux -f xite_params -nomail -d -c

rm -rf /usr/share/doc/packages/xite/ReferenceManual/CVS
rm -rf data/xshow/CVS etc/app-defaults/CVS
install -m 644 data/xshow/* /etc/xite
install -m 644 etc/app-defaults/* /usr/X11R6/lib/X11/app-defaults

# Re-build libfwf.so without -rpath
mkdir libfwf.tmp
(cd libfwf.tmp; \
ar -x ../lib/libfwf.a; \
cc -shared -Xlinker -soname -Xlinker libfwf.so.3.4 *.o -o /usr/lib/libfwf.so.3.4)
rm -rf libfwf.tmp
(cd /usr/lib; \
rm -f libfwf.so; \
ln -s libfwf.so.3.4 libfwf.so)
cp lib/libfwf.a /usr/lib

# Re-build libxite.so without -rpath
mkdir libxite.tmp
(cd libxite.tmp; \
ar -x ../lib/libxite.a; \
cc -shared -Xlinker -soname -Xlinker libxite.so.3.4 *.o -o /usr/lib/libxite.so.3.4)
rm -rf libxite.tmp
(cd /usr/lib; \
rm -f libxite.so; \
ln -s libxite.so.3.4 libxite.so)
cp lib/libxite.a /usr/lib

# Re-build libxite_pnm.so without -rpath
mkdir libxite_pnm.tmp
(cd libxite_pnm.tmp; \
ar -x ../lib/libxite_pnm.a; \
cc -shared -Xlinker -soname -Xlinker libxite_pnm.so.3.4 *.o -o /usr/lib/libxite_pnm.so.3.4)
rm -rf libxite_pnm.tmp
(cd /usr/lib; \
rm -f libxite_pnm.so; \
ln -s libxite_pnm.so.3.4 libxite_pnm.so)
cp lib/libxite_pnm.a /usr/lib

# Re-build libxite_tiff.so without -rpath
mkdir libxite_tiff.tmp
(cd libxite_tiff.tmp; \
ar -x ../lib/libxite_tiff.a; \
cc -shared -Xlinker -soname -Xlinker libxite_tiff.so.3.4 *.o -o /usr/lib/libxite_tiff.so.3.4)
rm -rf libxite_tiff.tmp
(cd /usr/lib; \
rm -f libxite_tiff.so; \
ln -s libxite_tiff.so.3.4 libxite_tiff.so)
cp lib/libxite_tiff.a /usr/lib

# Re-build libxiteX.so without -rpath
mkdir libxiteX.tmp
(cd libxiteX.tmp; \
ar -x ../lib/libxiteX.a; \
cc -shared -Xlinker -soname -Xlinker libxiteX.so.3.4 *.o -o /usr/lib/libxiteX.so.3.4)
rm -rf libxiteX.tmp
(cd /usr/lib; \
rm -f libxiteX.so; \
ln -s libxiteX.so.3.4 libxiteX.so)
cp lib/libxiteX.a /usr/lib

# Re-build libxpm.so without -rpath
mkdir libxpm.tmp
(cd libxpm.tmp; \
ar -x ../lib/libxpm.a; \
cc -shared -Xlinker -soname -Xlinker libxpm.so.3.4 *.o -o /usr/lib/libxpm.so.3.4)
rm -rf libxpm.tmp
(cd /usr/lib; \
rm -f libxpm.so; \
ln -s libxpm.so.3.4 libxpm.so)
cp lib/libxpm.a /usr/lib

%files -n xite
/usr/bin/ReadMenuList
/usr/bin/absDiff
/usr/bin/absValue
/usr/bin/addGauss
/usr/bin/addPoisson
/usr/bin/add_html_toc
/usr/bin/addw
/usr/bin/affine
/usr/bin/ascii2biff
/usr/bin/average
/usr/bin/bandpass
/usr/bin/bandstop
/usr/bin/bdf2biff
/usr/bin/biff2ascii
/usr/bin/biff2pnm
/usr/bin/biff2ps
/usr/bin/biff2raw
/usr/bin/biff2sunraster
/usr/bin/biff2tiff
/usr/bin/biffConvert
/usr/bin/biffDump
/usr/bin/biffcopy
/usr/bin/biffinfo
/usr/bin/biffmerge
/usr/bin/biffswap
/usr/bin/bifftext
/usr/bin/biffwrite
/usr/bin/binarize
/usr/bin/butterworth
/usr/bin/canny
/usr/bin/cdoc
/usr/bin/closestMinMax
/usr/bin/color2ascii
/usr/bin/colorquant
/usr/bin/combine
/usr/bin/complexConjug
/usr/bin/contour
/usr/bin/conv3x3
/usr/bin/convolve
/usr/bin/crossSection
/usr/bin/crossing
/usr/bin/cxite
/usr/bin/dither
/usr/bin/divide
/usr/bin/expand_edge
/usr/bin/exponential
/usr/bin/extrema1
/usr/bin/extrema2
/usr/bin/fft2d
/usr/bin/fftAmp
/usr/bin/fftDisplay
/usr/bin/fftImag
/usr/bin/fftMagPhase
/usr/bin/fftMakeBp
/usr/bin/fftMakeBs
/usr/bin/fftMakeHp
/usr/bin/fftMakeLp
/usr/bin/fftMult
/usr/bin/fftPhase
/usr/bin/fftPower
/usr/bin/fftReal
/usr/bin/fht2d
/usr/bin/fhtPhase
/usr/bin/fhtPower
/usr/bin/fork_xshow
/usr/bin/fractile
/usr/bin/ft2ht
/usr/bin/gammaAdjust
/usr/bin/glRunLength
/usr/bin/glcm
/usr/bin/glcmParameter
/usr/bin/grad
/usr/bin/gradInv
/usr/bin/gradient
/usr/bin/haar2d
/usr/bin/haarTexture
/usr/bin/highpass
/usr/bin/histo2ps
/usr/bin/histoEq
/usr/bin/histoEqCol
/usr/bin/histoMeanStd
/usr/bin/histoNorm
/usr/bin/houghCircle
/usr/bin/houghLine
/usr/bin/ht2ft
/usr/bin/ihs2rgb
/usr/bin/imag
/usr/bin/image_minmax
/usr/bin/isoCluster
/usr/bin/kMeansCluster
/usr/bin/kncn
/usr/bin/knn
/usr/bin/lapZeroCross
/usr/bin/laplace
/usr/bin/linearTrans
/usr/bin/lit
/usr/bin/litSnn
/usr/bin/logarithm
/usr/bin/logical
/usr/bin/lowpass
/usr/bin/lowpassIdeal
/usr/bin/makepseudo
/usr/bin/maxHisto
/usr/bin/maxSimilar3x3
/usr/bin/maxarea
/usr/bin/maxima
/usr/bin/mbkncn
/usr/bin/mbknn
/usr/bin/mct
/usr/bin/mean
/usr/bin/median
/usr/bin/minarea
/usr/bin/minima
/usr/bin/minmaxTexture
/usr/bin/mkFractalSra
/usr/bin/mkGauss
/usr/bin/mkHisto
/usr/bin/mkImg
/usr/bin/mkMorph
/usr/bin/mkPoisson
/usr/bin/mode
/usr/bin/morphClose
/usr/bin/morphDilate
/usr/bin/morphErode
/usr/bin/morphOpen
/usr/bin/mosaic
/usr/bin/multGauss
/usr/bin/multiply
/usr/bin/negate
/usr/bin/newtitle
/usr/bin/overlap
/usr/bin/peanoScan
/usr/bin/phase
/usr/bin/pixel_mapper
/usr/bin/pnm2biff
/usr/bin/power
/usr/bin/prewitt
/usr/bin/profile
/usr/bin/pseudo2rgb
/usr/bin/pseudomedian3x3
/usr/bin/pyramid
/usr/bin/quadratic
/usr/bin/rainbow
/usr/bin/rank
/usr/bin/raw2biff
/usr/bin/real
/usr/bin/regionAnalyse
/usr/bin/regionConvexHull
/usr/bin/reorganize
/usr/bin/resample
/usr/bin/rgb2ihs
/usr/bin/rms
/usr/bin/roberts
/usr/bin/rotate
/usr/bin/saturmod
/usr/bin/scale
/usr/bin/scatter
/usr/bin/segmRandom
/usr/bin/segmSpannWilson
/usr/bin/shift_img
/usr/bin/sigma
/usr/bin/signDiff
/usr/bin/snn
/usr/bin/sobel
/usr/bin/square
/usr/bin/squareRoot
/usr/bin/startProg
/usr/bin/statistics
/usr/bin/stdev
/usr/bin/stdiff
/usr/bin/stvar
/usr/bin/subcopy
/usr/bin/sunraster2biff
/usr/bin/thn_har
/usr/bin/thn_lc
/usr/bin/thn_zs
/usr/bin/thresBernsen
/usr/bin/thresLloyd
/usr/bin/thresMinErr
/usr/bin/thresMl
/usr/bin/thresMlCentroid
/usr/bin/thresMlComCur
/usr/bin/thresMlReddi
/usr/bin/thresMlWaHa
/usr/bin/thresOtsu
/usr/bin/thresPtile
/usr/bin/thresRidCal
/usr/bin/thresSigma
/usr/bin/thresUniErr
/usr/bin/threshold
/usr/bin/tiff2biff
/usr/bin/transpose_img
/usr/bin/warp
/usr/bin/warpExAnalysis
/usr/bin/window
/usr/bin/xadd
/usr/bin/xadd.bin
/usr/bin/xfft
/usr/bin/xfft.bin
/usr/bin/xfilter
/usr/bin/xfilter.bin
/usr/bin/xhistogram
/usr/bin/xhistogram.bin
/usr/bin/ximage_example
/usr/bin/ximage_example.bin
/usr/bin/xiteEcho
/usr/bin/xiteStdOpt
/usr/bin/xmovie
/usr/bin/xmovie.bin
/usr/bin/xpyramid
/usr/bin/xpyramid.bin
/usr/bin/xregion
/usr/bin/xregion.bin
/usr/bin/xshow
/usr/bin/xshow.bin
/usr/bin/xwarp
/usr/bin/xwarp.bin
/usr/bin/zernike
/usr/X11R6/lib/X11/app-defaults/XAdd
/usr/X11R6/lib/X11/app-defaults/XFft
/usr/X11R6/lib/X11/app-defaults/XFilter
/usr/X11R6/lib/X11/app-defaults/XHistogram
/usr/X11R6/lib/X11/app-defaults/XImageExample
/usr/X11R6/lib/X11/app-defaults/XMovie
/usr/X11R6/lib/X11/app-defaults/XPyramid
/usr/X11R6/lib/X11/app-defaults/XRegion
/usr/X11R6/lib/X11/app-defaults/XShow
/usr/X11R6/lib/X11/app-defaults/XWarp
/usr/X11R6/lib/X11/app-defaults/xiteFileSel
/usr/X11R6/lib/X11/app-defaults/xiteHisto
/usr/X11R6/lib/X11/app-defaults/xiteImage
/usr/X11R6/lib/X11/app-defaults/xiteImageMenubar
/usr/X11R6/lib/X11/app-defaults/xiteImageOverlay
/usr/X11R6/lib/X11/app-defaults/xiteImagePopup
/usr/X11R6/lib/X11/app-defaults/xitePopup
/usr/X11R6/lib/X11/app-defaults/xitePrompt
/usr/X11R6/lib/X11/app-defaults/xiteSlice
/usr/X11R6/lib/X11/app-defaults/xiteTerm
/usr/X11R6/lib/X11/app-defaults/xshowMenuHier
/etc/xite/black.col
/etc/xite/black.map
/etc/xite/black.ovl.col
/etc/xite/black.ovl.map
/etc/xite/blue.col
/etc/xite/blue.map
/etc/xite/comp_e
/etc/xite/comp_n
/etc/xite/comp_ne
/etc/xite/comp_nw
/etc/xite/comp_s
/etc/xite/comp_se
/etc/xite/comp_sw
/etc/xite/comp_w
/etc/xite/glatt
/etc/xite/green.col
/etc/xite/green.map
/etc/xite/hue1.col
/etc/xite/hue1.map
/etc/xite/laplace
/etc/xite/laplace2
/etc/xite/laplace5
/etc/xite/laplace8
/etc/xite/laplace9
/etc/xite/rainbow.col
/etc/xite/rainbow.map
/etc/xite/red.col
/etc/xite/red.map
/etc/xite/spectrum.col
/etc/xite/spectrum.map
/etc/xite/white.col
/etc/xite/white.map
/etc/xite/white.ovl.col
/etc/xite/white.ovl.map
/etc/xite/xshow_colortabs
/etc/xite/xshow_menues

%files -n xite-doc
/usr/share/doc/packages/xite/ProgrammersManual.ps
/usr/share/doc/packages/xite/BUGS
/usr/share/doc/packages/xite/CHANGES
/usr/share/doc/packages/xite/ReferenceManual
/usr/share/doc/packages/xite/SysAdmManual.p*
/usr/share/doc/packages/xite/UsersManual.p*
/usr/share/doc/packages/xite/WHAT_IS_XITE*
/usr/share/doc/packages/xite/bugs.html
/usr/share/doc/packages/xite/changes.html
/usr/share/doc/packages/xite/mona_canny.jpg

%files -n libxite-devel
/usr/include/xite/Cmap.h
/usr/include/xite/FormDialog.h
/usr/include/xite/Histogram.h
/usr/include/xite/Image.h
/usr/include/xite/ImageForm.h
/usr/include/xite/ImageFormat.h
/usr/include/xite/ImageOptionsMenu.h
/usr/include/xite/ImageOverlay.h
/usr/include/xite/ImageOverlayP.h
/usr/include/xite/ImageP.h
/usr/include/xite/ImageToolsMenu.h
/usr/include/xite/ImageVisualsMenu.h
/usr/include/xite/ShellWids.h
/usr/include/xite/Visual.h
/usr/include/xite/WidUtils.h
/usr/include/xite/Xcolor.h
/usr/include/xite/Xdialog.h
/usr/include/xite/Xhistogram.h
/usr/include/xite/XmenuBar.h
/usr/include/xite/Xpty.h
/usr/include/xite/Xslice.h
/usr/include/xite/arithmetic.h
/usr/include/xite/biff.h
/usr/include/xite/binarize.h
/usr/include/xite/blab.h
/usr/include/xite/callbacks.h
/usr/include/xite/canny.h
/usr/include/xite/cdoc.h
/usr/include/xite/classification.h
/usr/include/xite/color.h
/usr/include/xite/combine.h
/usr/include/xite/compu4.h
/usr/include/xite/contour.h
/usr/include/xite/convert.h
/usr/include/xite/convolve.h
/usr/include/xite/copy.h
/usr/include/xite/debug.h
/usr/include/xite/dither.h
/usr/include/xite/draw.h
/usr/include/xite/dummy.h
/usr/include/xite/extrema.h
/usr/include/xite/fft.h
/usr/include/xite/fht.h
/usr/include/xite/fork_xshow.h
/usr/include/xite/fractal.h
/usr/include/xite/geometry.h
/usr/include/xite/gradInv.h
/usr/include/xite/haar.h
/usr/include/xite/histo.h
/usr/include/xite/hough.h
/usr/include/xite/ideal.h
/usr/include/xite/ihs.h
/usr/include/xite/includes.h
/usr/include/xite/kncn.h
/usr/include/xite/knn.h
/usr/include/xite/layout.h
/usr/include/xite/logPos.h
/usr/include/xite/mallocp.h
/usr/include/xite/matrix.h
/usr/include/xite/median.h
/usr/include/xite/message.h
/usr/include/xite/minmax.h
/usr/include/xite/morph.h
/usr/include/xite/negate.h
/usr/include/xite/noise.h
/usr/include/xite/patchlevel.h
/usr/include/xite/principal.h
/usr/include/xite/pyramid.h
/usr/include/xite/readarg.h
/usr/include/xite/region.h
/usr/include/xite/resample.h
/usr/include/xite/scatter.h
/usr/include/xite/segmentation.h
/usr/include/xite/sigma.h
/usr/include/xite/snn.h
/usr/include/xite/statistics.h
/usr/include/xite/stdiff.h
/usr/include/xite/strings.h
/usr/include/xite/sunraster.h
/usr/include/xite/texture.h
/usr/include/xite/thin.h
/usr/include/xite/thresMl.h
/usr/include/xite/threshold.h
/usr/include/xite/tiff.h
/usr/include/xite/utils.h
/usr/include/xite/utl_malloc.h
/usr/include/xite/ximage.h
/usr/include/xite/ximage_display.h
/usr/include/xite/xite_endian.h
/usr/include/xite/xite_file.h
/usr/include/xite/xite_fork.h
/usr/include/xite/xite_funcproto.h
/usr/include/xite/xite_limits.h
/usr/include/xite/xite_mktemp.h
/usr/include/xite/xite_pnm.h
/usr/include/xite/xite_random.h
/usr/include/xite/xite_stat.h
/usr/include/xite/xite_stdarg.h
/usr/include/xite/xite_stdio.h
/usr/include/xite/xite_strtol.h
/usr/include/xite/xite_time.h
/usr/include/xite/xite_toupper.h
/usr/include/xite/xite_types.h
/usr/include/xite/xite_unistd.h
/usr/include/xite/zernike.h
/usr/include/xpm/xpm.h
/usr/lib/libxite.a
/usr/lib/libxiteX.a
/usr/lib/libxite_pnm.a
/usr/lib/libxite_tiff.a
/usr/lib/libxite.so
/usr/lib/libxiteX.so
/usr/lib/libxite_pnm.so
/usr/lib/libxite_tiff.so
/usr/lib/libxpm.a
/usr/lib/libxpm.so

%files -n libxite
/usr/lib/libxite.so.3.4
/usr/lib/libxiteX.so.3.4
/usr/lib/libxite_pnm.so.3.4
/usr/lib/libxite_tiff.so.3.4
/usr/lib/libxpm.so.3.4

%files -n libfwf
/usr/lib/libfwf.so.3.4

%files -n libfwf-devel
/usr/include/Xfwf/Board.h
/usr/include/Xfwf/BoardP.h
/usr/include/Xfwf/Button.h
/usr/include/Xfwf/ButtonP.h
/usr/include/Xfwf/Common.h
/usr/include/Xfwf/CommonP.h
/usr/include/Xfwf/Converters.h
/usr/include/Xfwf/DirMgr.h
/usr/include/Xfwf/Directory.h
/usr/include/Xfwf/FileSel.h
/usr/include/Xfwf/FileSelP.h
/usr/include/Xfwf/Frame.h
/usr/include/Xfwf/FrameP.h
/usr/include/Xfwf/Group.h
/usr/include/Xfwf/GroupP.h
/usr/include/Xfwf/Label.h
/usr/include/Xfwf/LabelP.h
/usr/include/Xfwf/MenuBar.h
/usr/include/Xfwf/MenuBarP.h
/usr/include/Xfwf/MultiList.h
/usr/include/Xfwf/MultiListP.h
/usr/include/Xfwf/OptButton.h
/usr/include/Xfwf/OptButtonP.h
/usr/include/Xfwf/Pen.h
/usr/include/Xfwf/Prompt.h
/usr/include/Xfwf/PromptP.h
/usr/include/Xfwf/PullDown.h
/usr/include/Xfwf/PullDownP.h
/usr/include/Xfwf/RegExp.h
/usr/include/Xfwf/RowCol.h
/usr/include/Xfwf/RowColP.h
/usr/include/Xfwf/Rows.h
/usr/include/Xfwf/RowsP.h
/usr/include/Xfwf/ScrList.h
/usr/include/Xfwf/ScrListP.h
/usr/include/Xfwf/StrToPmap.h
/usr/include/Xfwf/TabString.h
/usr/include/Xfwf/TextMenu.h
/usr/include/Xfwf/TextMenuP.h
/usr/include/Xfwf/Toggle.h
/usr/include/Xfwf/ToggleP.h
/usr/include/Xfwf/choosecol.h
/usr/include/Xfwf/icon.h
/usr/include/Xfwf/long.h
/usr/include/Xfwf/stip4.h
/usr/include/Xfwf/strarray.h
/usr/lib/libfwf.a
/usr/lib/libfwf.so
