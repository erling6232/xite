

/*

________________________________________________________________

        regionDraw
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




/*F:regionDraw*

________________________________________________________________

		regionDraw
________________________________________________________________

Name:		regionDraw - draw one or more y-line regions in a band

Syntax:         | #include <xite/region.h>
		|
                | int regionDraw( IBAND band, region* reg,
                |    int loop, int pv );
Description:    The region specified by "reg" is drawn in the "band"
                with pixel value "pv". The region is clipped against
		the border of the image; eventual parts of the region 
		outside the band are not drawn. If "loop" is true,
		the next pointer in reg is followed until NIL, and all
		regions are drawn. If "pv" = -1, the object number of
		each region is used as pixel value. If "pv" = -2, the
		region number of each region is used as pixel value.

Return value:   | 0 => ok
                | 1 => bad band pixel type
		| 2 => bad region specification
		| 3 => pixel value outside -2..255

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3),
		regionFree(3), regionHistoUpdate(3), regionHisto(3),
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor Loennestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
int regionDraw(band, reg, loop, pv)
IBAND band;
region* reg;
int loop, pv;
#else /* FUNCPROTO */
int regionDraw(IBAND band, region *reg, int loop, int pv)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, ymax, xmin, xmax, lpv;
  yline* yl;

  if (Ipixtyp(band) NE Iu_byte_typ) return(1);
  if (reg EQ nilr) return(2);
  if ((pv LT -2) OR (pv GT 255)) return(3);

  xsize = Ixsize(band);
  ysize = Iysize(band);

  while (TRUE) {
    lpv = (pv EQ -2) ? reg->regionnr : ((pv EQ -1) ? reg->objectnr : pv);
    ymax = MIN(ysize, reg->ymax);

    FOR (y=reg->ymin; y LE ymax; y++)
      yl = reg->ylines[y];
      WHILE (yl)
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        for (x=xmin; x LE xmax; x++) band[y][x] = lpv;
       yl = yl->next;
      ENDWHILE;
    ENDFOR;
    if (! loop) break;
    if (! (reg=reg->next)) break;
  }
  return(0);
}


