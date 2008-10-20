/*C*

________________________________________________________________

        regionMean
        $Id$
        Copyright 1990, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and its
  documentation for any purpose and without fee is hereby granted, 
  provided that this copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in supporting
  documentation and that the name of B-lab, Department of Informatics or
  University of Oslo not be used in advertising or publicity pertaining 
  to distribution of the software without specific, written prior permission.

  B-LAB DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL B-LAB
  BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 

*/

static char *Id = "$Id$, Blab, UiO";


#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/region.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif

/*F:regionXmean=regionMean*/
/*F:regionYmean=regionMean*/
/*F:regionMean*

________________________________________________________________

		regionMean
________________________________________________________________

Name:		regionMean, regionXmean, regionYmean - calculate xmean
                or ymean of one or more regions

Syntax:         | #include <xite/region.h>
		|
                | int regionXmean( IBAND band, region* reg,
                |    int loop, double* xmean );
		|
                | int regionYmean( IBAND band, region* reg,
                |    int loop, double* ymean );
		|
                | int regionMean( IBAND band, region* reg,
                |    int loop, double* xmean, double* ymean );

Description:    Calculates xmean (ymean) of one or several regions.
		If "loop" is false, xmean (ymean) of "reg" is measured.
		If true, the next pointer in reg is followed until nil,
		and xmean (ymean) of the whole region set is calculated.
		If "band" is nil, the simple mean of the region is
		calculated. If "band" is a band, the pixel values
                are used as weights.

Return value:   | 0 => ok
		| 1 => bad pixeltype, accepts only unsigned byte
		| 2 => "reg" points to nil. Area is set to 0.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmin(3), regionXmax(3),
		regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

/*--------------------------------------------------------------
                regionXmean
----------------------------------------------------------------
*/
int regionXmean(IBAND band, region *reg, int loop, double *xmean)
{
  int xsize, ysize, x, y, xmin, xmax, ymin, ymax, ylxsize;
  double xsum=0.0, sum=0.0;
  yline *yl;
  if (reg EQ nilr) {
    *xmean=0;
    return(2);
  }

  if (NOT band) {
    while (TRUE) {
      ymax = reg->ymax;
      for (y=reg->ymin; y LE ymax; y++) {
        for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
          ylxsize = YLXSIZE(yl);
          sum += (double)ylxsize;
          xsum += ((yl->xmin + yl->xmax)*ylxsize)/2.0;
	}
      }
      if (NOT loop) break;
      if (NOT (reg = reg->next)) break;
    }
    *xmean = xsum/sum;
    return(0);  
  }
  
  if (Ipixtyp(band) NE Iu_byte_typ) return(1);
  xsize = Ixsize(band);
  ysize = Iysize(band);
  while (TRUE) {
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    for (y=reg->ymin; y LE ymax; y++) {
      for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        for (x=xmin; x LE xmax; x++) {
          xsum += (double)(x*band[y][x]);
          sum += (double)band[y][x];
	}
      }
    }
    if (NOT loop) break;
    if (NOT (reg = reg->next)) break;
  }
  *xmean = xsum/sum;
  return(0);
}

/*--------------------------------------------------------------
                regionYmean
----------------------------------------------------------------
*/

int regionYmean(IBAND band, region *reg, int loop, double *ymean)
{
  int xsize, ysize, x, y, xmin, xmax, ymin, ymax, ylxsize;
  double ysum=0.0, sum=0.0;
  yline *yl;

  if (reg EQ nilr) {
    *ymean=0;
    return(2);
  }

  if (NOT band) {
    while (TRUE) {
      for (y=reg->ymin; y LE reg->ymax; y++) {
        for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
          ylxsize = YLXSIZE(yl);
          sum += (double)ylxsize;
          ysum += (double)(y*ylxsize);
	}
      }
      if (NOT loop) break;
      if (NOT (reg = reg->next)) break;
    }
    *ymean = ysum/sum;
    return(0);
  }

  if (Ipixtyp(band) NE Iu_byte_typ) return(1);
  xsize = Ixsize(band);
  ysize = Iysize(band);
  while (TRUE) {
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    for (y=reg->ymin; y LE ymax; y++) {
      for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        ylxsize = xmax - xmin + 1;
        for (x=xmin; x LE xmax; x++) {
          ysum += (double)(y*band[y][x]);
          sum += (double)band[y][x];
	}
      }
    }
    if (NOT loop) break;
    if (NOT (reg = reg->next)) break;
  }
  *ymean = ysum/sum;
  return(0);
}

/*--------------------------------------------------------------
                regionMean
----------------------------------------------------------------
*/
int regionMean(IBAND band, region *reg, int loop, double *xmean, double *ymean)
{
  int xsize, ysize, x, y, xmin, xmax, ymin, ymax, ylxsize;
  double xsum=0.0, ysum=0.0, sum=0.0;
  yline *yl;

  if (reg EQ nilr) {
    *xmean = 0.0;
    *ymean=0.0;
    return(2);
  }

  if (NOT band) {
    while (TRUE) {
      for (y=reg->ymin; y LE reg->ymax; y++) {
        for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
          ylxsize = YLXSIZE(yl);
          sum += (double)ylxsize;
          xsum += (double)((yl->xmin + yl->xmax)*ylxsize)/2.0;
          ysum += (double)(y*ylxsize);
	}
      }
      if (NOT loop) break;
      if (NOT (reg = reg->next)) break;
    }
    *xmean = xsum/sum;
    *ymean = ysum/sum;
    return(0);
  }

  if (Ipixtyp(band) NE Iu_byte_typ) return(1);
  xsize = Ixsize(band);
  ysize = Iysize(band);
  while (TRUE) {
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    for (y=reg->ymin; y LE ymax; y++) {
      for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        ylxsize = xmax - xmin + 1;
        for (x=xmin; x LE xmax; x++) {
          xsum += (double)(x*band[y][x]);
          ysum += (double)(y*band[y][x]);
          sum += (double)band[y][x];
	}
      }
    }
    if (NOT loop) break;
    if (NOT (reg = reg->next)) break;
  }
  *xmean = xsum/sum;
  *ymean = ysum/sum;
  return(0);
}
