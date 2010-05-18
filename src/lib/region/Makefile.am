#
#      Makefile.am for lib/region
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteregion.la

MANF =  regionArea.3 regionConvexHull.3 regionCreate.3 regionDraw.3 \
	regionFree.3 regionHistoUpdate.3 regionHisto.3 regionImoment.3 \
	regionInsertYline.3 regionDeleteYline.3 regionEndInsert.3 \
	regionXmean.3 regionYmean.3 regionMean.3 regionXmin.3 \
	regionXmax.3 regionYmin.3 regionYmax.3 regionMinMax.3 \
	regionCMoment.3 regionPMoment.3 regionMoment.3 regionPerform.3 \
	regionPerimeter.3 regionIsPoint.3 regionPoint.3 regionSearch.3 \
	regionSort.3 regionCopy.3 regionUnion.3 regionOverlap.3 \
	regionNeighbor.3 regionVolume.3 regionYline.3

libxiteregion_la_SOURCES = regionArea.c      regionCreate.c     regionDraw.c \
	regionFree.c regionHisto.c regionMean.c regionMinMax.c regionMoment.c  \
        regionPerimeter.c regionSearch.c regionSort.c regionVolume.c  \
	regionYline.c     regionConvexHull.c regionUnion.c  regionPerform.c \
	regionPoint.c     regionImoment.c    regionInsert.c

regionHistoUpdate.3:	regionHisto.3
regionInsertYline.3:	regionInsert.3
regionDeleteYline.3:	regionInsert.3
regionEndInsert.3:	regionInsert.3
regionXmean.3:	regionMean.3
regionYmean.3:	regionMean.3
regionXmin.3:	regionMinMax.3
regionXmax.3:	regionMinMax.3
regionYmin.3:	regionMinMax.3
regionYmax.3:	regionMinMax.3
regionCMoment.3:	regionMoment.3
regionPMoment.3:	regionMoment.3
regionIsPoint.3:	regionPoint.3
regionCopy.3:	regionUnion.3
regionOverlap.3:	regionUnion.3
regionNeighbor.3:	regionUnion.3
