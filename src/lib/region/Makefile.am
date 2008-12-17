#
#      Makefile.am for lib/region
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteregion.la

libxiteregion_la_SOURCES = regionArea.c      regionCreate.c     regionDraw.c \
	regionFree.c regionHisto.c regionMean.c regionMinMax.c regionMoment.c  \
        regionPerimeter.c regionSearch.c regionSort.c regionVolume.c  \
	regionYline.c     regionConvexHull.c regionUnion.c  regionPerform.c \
	regionPoint.c     regionImoment.c    regionInsert.c
