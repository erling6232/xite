lib_LTLIBRARIES = libxite.la libxiteX.la libxite_pnm.la libxite_tiff.la
libxite_la_SOURCES =
libxite_la_LIBADD = \
	arithmetic/libxitearithmetic.la \
	biff/libxitebiff.la \
	binarize/libxitebinarize.la \
	canny/libxitecanny.la \
	cdoc/libxitecdoc.la \
	classification/libxiteclassification.la \
	color/libxitecolor.la \
	combine/libxitecombine.la \
	contour/libxitecontour.la \
	convert/libxiteconvert.la \
	convolve/libxiteconvolve.la \
	copy/libxitecopy.la \
	dither/libxitedither.la \
	draw/libxitedraw.la \
	extrema/libxiteextrema.la \
	fft/libxitefft.la \
	fht/libxitefht.la \
	fractal/libxitefractal.la \
	geometry/libxitegeometry.la \
	gradInv/libxitegradInv.la \
	histo/libxitehisto.la \
	hough/libxitehough.la \
	haar/libxitehaar.la \
	ideal/libxiteideal.la \
	ihs/libxiteihs.la \
	kncn/libxitekncn.la \
	knn/libxiteknn.la \
	median/libxitemedian.la \
	minmax/libxiteminmax.la \
	morph/libxitemorph.la \
	negate/libxitenegate.la \
	noise/libxitenoise.la \
	principal/libxiteprincipal.la \
	pyramid/libxitepyramid.la \
	region/libxiteregion.la \
	resample/libxiteresample.la \
	scatter/libxitescatter.la \
	segmentation/libxitesegmentation.la \
	sigma/libxitesigma.la \
	snn/libxitesnn.la \
	statistics/libxitestatistics.la \
	stdiff/libxitestdiff.la \
	sunraster/libxitesunraster.la \
	texture/libxitetexture.la \
	thin/libxitethin.la \
	threshold/libxitethreshold.la \
	thresMl/libxitethresMl.la \
	utils/libxiteutils.la \
	xshow/libxitexshow.la \
	zernike/libxitezernike.la
libxite_la_LDFLAGS = -version-info 3:0:0

libxiteX_la_SOURCES =
libxiteX_la_LIBADD = \
	Image/libxiteXImage.la \
	ximage/libxiteXximage.la \
	libxite.la
libxiteX_la_LDFLAGS = $(LIBGUI) -version-info 3:0:0
# libxiteX_la_LDFLAGS = $(LIBGUI) -lxite -version-info 3:0:0

libxite_pnm_la_SOURCES =
libxite_pnm_la_LIBADD = \
	pnm/libxitepnm.la \
	libxite.la
libxite_pnm_la_LDFLAGS = -lnetpbm -version-info 3:0:0
# libxite_pnm_la_LDFLAGS = -lnetpbm -lxite -version-info 3:0:0

libxite_tiff_la_SOURCES =
libxite_tiff_la_LIBADD = \
	tiff/libxitetiff.la \
	libxite.la
libxite_tiff_la_LDFLAGS = -ltiff -version-info 3:0:0
# libxite_tiff_la_LDFLAGS = -ltiff -lxite -version-info 3:0:0

SUBDIRS = \
	arithmetic \
	biff \
	binarize \
	canny \
	cdoc \
	classification \
	color \
	combine \
	contour \
	convert \
	convolve \
	copy \
	dither \
	draw \
	extrema \
	fft \
	fht \
	fractal \
	geometry \
	gradInv \
	histo \
	hough \
	haar \
	ideal \
	ihs \
	Image \
	kncn \
	knn \
	matlab \
	median \
	minmax \
	morph \
	negate \
	noise \
	pnm \
	principal \
	pyramid \
	region \
	resample \
	scatter \
	segmentation \
	sigma \
	snn \
	statistics \
	stdiff \
	sunraster \
	texture \
	thin \
	threshold \
	thresMl \
	tiff \
	utils \
	ximage \
	xshow \
	zernike
