

/*

________________________________________________________________

        regionImoment.c
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
#include <xite/biff.h>
#include <xite/region.h>





/*F:regionImoment*

________________________________________________________________

		regionImoment
________________________________________________________________

Name:		regionImoment - compute invariant moments
Syntax:         | #include <xite/region.h>
		|
                | int regionImoment( IBAND band, region* reg,
                |    int loop, double* moments );
Description:    regionImoment calculates 7 invariant moments.
                | moments[1] .. moments[7].
                If the band argument is nil, 'regionImoment' calculates
                the moments of the mask otherwise the monents inside
                reg for band.
See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionInsertYline(3), regionDeleteYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)
Reference:      Gonzalez and Woods
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
int regionImoment(band, reg, loop, moments)
IBAND band;
region *reg;
int loop;
double *moments;
#else /* FUNCPROTO */
int regionImoment(IBAND band, region *reg, int loop, double *moments)
#endif /* FUNCPROTO */
{

  int x, y, xmin, ymin, xmax, ymax, xsize, ysize;
  yline *yl;
  double f00, f10, f01, f20, f11, f02, f30, f21, f12, f03;
  double m00, m10, m01, m20, m11, m02, m30, m21, m12, m03;
  double xm, ym, u00, u20, u11, u02, u30, u21, u12, u03;
  double y20, y25;
  double s20, d20, s11, D31, D20, S31, S20;
 
  m00 = m10 = m01 = m20 = m11 = m02 = m30 = m21 = m12 = m03 = 0.0;

  if (band && Ipixtyp(band) != Iu_byte_typ) return(1);
  if (band) xsize = Ixsize(band);
  if (band) ysize = Iysize(band);

  while(reg)
   {
    ymin = reg-> ymin;
    if (band && ymin < 1) ymin = 1;

    ymax = reg->ymax;
    if (band && ymax > ysize) ymax = ysize;

    for (y=reg->ymin; y <= ymax; y++)
      {
        for (yl=reg->ylines[y]; yl != nily; yl = yl->next)
	  {
            xmin = yl->xmin;
	    if (band && xmin < 1) xmin = 1;

            xmax = yl->xmax;
	    if (band && xmax > xsize) xmax = xsize;

            for (x=xmin; x <= xmax; x++)
	      {
                 f00 = band ? (double) band[y][x] : 1.0; m00 += f00;
		 f10 = f00 * x; m10 += f10;
		 f01 = f00 * y; m01 += f01;
		 f20 = f10 * x; m20 += f20;
		 f11 = f10 * y; m11 += f11;
		 f02 = f01 * y; m02 += f02;
		 f30 = f20 * x; m30 += f30;
		 f21 = f20 * y; m21 += f21;
		 f12 = f11 * y; m12 += f12;
		 f03 = f02 * y; m03 += f03;
	      }
	  }
      }
    if (! loop) break;
    reg = reg->next;
  };

  xm  = m10/m00;
  ym  = m01/m00;
  u00 = m00;
  y20 = u00*u00;
  y25 = u00*u00*sqrt(u00);

  u20 = (m20 - m00*xm*xm)/y20;
  u11 = (m11 - m00*xm*ym)/y20;
  u02 = (m02 - m00*ym*ym)/y20;
  u30 = (m30 - 3.0*m20*xm + 2.0*m00*xm*xm*xm)/y25;
  u21 = (m21 - m20*ym - 2.0*m11*xm + 2.0*m00*xm*xm*ym)/y25;
  u12 = (m12 - m02*xm - 2.0*m11*ym + 2.0*m00*xm*ym*ym)/y25;
  u03 = (m03 - 3.0*m02*ym + 2.0*m00*ym*ym*ym)/y25;

  s20 = u20 + u02;
  d20 = u20 - u02;
  s11 = u11 + u11;
  
  D31 = u30 - 3.0*u12;
  D20 = 3.0*u21 - u03;
  S31 = u30 + u12;
  S20 = u21 + u03;

  moments[1] = s20;
  moments[2] = d20*d20 + s11*s11;
  moments[3] = D31*D31 + D20*D20;
  moments[4] = S31*S31 + S20*S20;
  moments[5] = D31 * S31 * (S31*S31 - 3.0 * S20*S20) +
               D20 * S20 * (3.0 * S31*S31 - S20*S20);
  moments[6] = d20 * (S31*S31 - S20*S20) + 2.0 * s11 * S31 * S20;
  moments[7] = D20 * S31 * (S31*S31 - 3.0 * S20*S20) -
               D31 * S20 * (3.0 * S31*S31 - S20*S20);

  return 0;
}


