

/*C*

________________________________________________________________

        regionMoment
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

#include <math.h>
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


/*F:regionCMoment=regionMoment*/
/*F:regionPMoment=regionMoment*/
/*F:regionMoment*

________________________________________________________________

		regionMoment
________________________________________________________________

Name:		regionMoment, regionCMoment, regionPMoment - the moment of
                one or more regions

Syntax:         | #include <xite/region.h>
		| #include <xite/region.h>
		|
                | int regionMoment( IBAND band, region* reg,
                |    int p, int q, int loop, double* moment );
		|
                | int regionCMoment( IBAND band, region* reg,
                |    int p, int q, int loop, double* moment );
		|
                | int regionPMoment( IBAND band, region* reg,
                |    int p, int q, double xc, double yc,
                |    int loop, double* moment );
Description:    If the band argument is nil, 'regionMoment' calculates
		the moment of order p+q around origo of one or several
		regions.

                | moment  =  SUM SUM (x**p * y**q)
                |          (x,y)in reg

		'regionCMoment' calculates the central moment of order p+q
		of one or several regions.

                | moment  =  SUM SUM ((x-xmean)**p * (y-ymean)**q)
                |          (x,y)in reg

		'regionPMoment' calculates the moment of order p+q around
		a given point (xc, yc) of one or several regions.

                | moment  =  SUM SUM ((x-xc)**p (y-yc)**q)
                |          (x,y)in reg

		If "loop" is false, the moment of "reg" is measured.
		If true, the next pointer in reg is followed until
		nil, and the moment of the whole region set is
		calculated. 

		If the band argument specifies a band, then the pixel
		values are used to calculate the moments:

                | moment  =  SUM SUM (x**p * y**q * band[x,y])
                |          (x,y)in reg
		|
                | moment  =  SUM SUM ((x-xmean)**p * (y-ymean)**q
		|                    * band[x,y])
		|          (x,y)in reg
		|
                | moment  =  SUM SUM ((x-xc)**p (y-yc)**q * band[x,y])
                |          (x,y)in reg
		
		The result is returned through the 'moment' parameter.

Return value:   | 0 => ok
		| 1 => bad pixel type in band, accepts only unsigned byte
		| 2 => "reg" points to nil. Area is set to 0.
		| 3 => p or q is negative.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor L|nnestad, BLAB, ifi, UiO

Example:	| #include <xite/biff.h>
		| #include <xite/region.h>
		| region* reg;
		| double moment;
		| ...
		| regionMoment(0, reg, 1, 2, FALSE, &moment);

Id: 		$Id$
________________________________________________________________

*/


/*-------------------------------------------
             regionMoment
-------------------------------------------*/

#ifndef FUNCPROTO
int regionMoment(band, reg, p, q, loop, moment)
IBAND band;
region *reg;
int p, q, loop;
double* moment;
#else /* FUNCPROTO */
int regionMoment(IBAND band, region *reg, int p, int q, int loop, double *moment)
#endif /* FUNCPROTO */
{
  int x, y, xmin, ymin, xmax, ymax, xsize, ysize;
  yline *yl;
  double ypow, xpow, pd, qd, mom=0.0;
  
  if (reg EQ nilr) { *moment=0; return(2); }
  if ((p LT 0) OR (q LT 0)) return(3);
  pd = (double)p;
  qd = (double)q;

  if (NOT band) {
    while (TRUE) {
      ymax = reg->ymax;
      for (y=reg->ymin; y LE ymax; y++) {
        ypow = pow((double)y ,qd);
        xpow = 0.0;
        for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
          xmax = yl->xmax;
          for (x=yl->xmin; x LE xmax; x++)
            xpow += pow((double)x, pd);
	}
        mom += xpow*ypow;
      }
      if (NOT loop) break;
      if (NOT (reg = reg->next)) break;
    }
    *moment = mom;
    return(0);
  }

  if (Ipixtyp(band) NE Iu_byte_typ) return(1);
  xsize = Ixsize(band);
  ysize = Iysize(band);
  LOOP
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    FOR (y=reg->ymin; y LE ymax; y++)
      ypow = pow((double)y ,qd);
      xpow = 0.0;
      FOR (yl=reg->ylines[y]; yl NE nily; yl = yl->next)
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        for (x=xmin; x LE xmax; x++)
          xpow += pow((double)x, pd) * (double)band[y][x];
      ENDFOR;
      mom += xpow*ypow;
    ENDFOR;
    EXITIF(NOT loop);
    EXITIF(NOT (reg = reg->next));
  ENDLOOP;
  *moment = mom;
  return(0);
}


/*-------------------------------------------
             regionCMoment
-------------------------------------------*/

#ifndef FUNCPROTO
int regionCMoment(band, reg, p, q, loop, moment)
IBAND band;
region *reg;
int p, q, loop;
double* moment;
#else /* FUNCPROTO */
int regionCMoment(IBAND band, region *reg, int p, int q, int loop, double *moment)
#endif /* FUNCPROTO */
{
  double xmean, ymean;
  
  regionXmean(band, reg, loop, &xmean);
  regionYmean(band, reg, loop, &ymean);
  return(regionPMoment(band, reg, p, q, xmean, ymean, loop, moment));
}

/*-------------------------------------------
             regionPMoment
-------------------------------------------*/

#ifndef FUNCPROTO
int regionPMoment(band, reg, p, q, xc, yc, loop, moment)
IBAND band;
region *reg;
int p, q, loop;
double xc, yc, *moment;
#else /* FUNCPROTO */
int regionPMoment(IBAND band, region *reg, int p, int q, double xc, double yc, int loop, double *moment)
#endif /* FUNCPROTO */
{
  int x, y, xmin, ymin, xmax, ymax, xsize, ysize;
  yline *yl;
  double ypow, xpow, pd, qd, mom=0.0;
  
  if (reg EQ nilr) { *moment=0; return(2); }
  if ((p LT 0) OR (q LT 0)) return(3);
  pd = (double)p;
  qd = (double)q;

  if (NOT band) {
    while (TRUE) {
      ymax = reg->ymax;
      for (y=reg->ymin; y LE ymax; y++) {
        ypow = pow((double)y - yc ,qd);
        xpow = 0.0;
        for (yl=reg->ylines[y]; yl NE nily; yl = yl->next) {
          xmax = yl->xmax;
          for (x=yl->xmin; x LE xmax; x++)
            xpow += pow((double)x - xc, pd);
	}
        mom += xpow*ypow;
      }
      if (NOT loop) break;
      if (NOT (reg = reg->next)) break;
    }
    *moment = mom;
    return(0);
  }

  if (Ipixtyp(band) NE Iu_byte_typ) return(1);
  xsize = Ixsize(band);
  ysize = Iysize(band);
  LOOP
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    FOR (y=reg->ymin; y LE ymax; y++)
      ypow = pow((double)y - yc ,qd);
      xpow = 0.0;
      FOR (yl=reg->ylines[y]; yl NE nily; yl = yl->next)
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        for (x=xmin; x LE xmax; x++)
          xpow += pow((double)x - xc, pd) * (double)band[y][x];
      ENDFOR;
      mom += xpow*ypow;
    ENDFOR;
    EXITIF(NOT loop);
    EXITIF(NOT (reg = reg->next));
  ENDLOOP;
  *moment = mom;
  return(0);
}





