

/*

________________________________________________________________

        regionMinMax
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



/*F:regionXmin=regionMinMax*/
/*F:regionXmax=regionMinMax*/
/*F:regionYmin=regionMinMax*/
/*F:regionYmax=regionMinMax*/
/*F:regionMinMax*

________________________________________________________________

		regionMinMax
________________________________________________________________

Name:		regionMinMax, regionXmin, regionYmin, regionXmax, regionYmax
                - calc xmin, ymin, xmax, ymax of a region(s)

Syntax:         | #include <xite/region.h>
		|
                | int regionMinMax( region* reg, int loop,
                |    int* xmin, int* ymin, int* xmax, int* ymax );
		|
                | int regionXmin( region* reg, int loop,
                |    int* xmin );
		|
                | int regionYmin( region* reg, int loop,
                |    int* ymin );
		|
                | int regionXmax( region* reg, int loop,
                |    int* xmax );
		|
                | int regionYmax( region* reg, int loop,
                |    int* ymax );
Description:    Calculates the leftmost (xmin), rightmost (xmax), uppermost
                (ymin) and downmost (ymax) position of one or more regions.
                If "loop" is false, the region "reg" only is examined. If
                true, the next pointer in reg is followed until nil, and the
		extremum of the	whole region set is found.

Return value:   0 => ok
                1 => reg points to nil, output position is set to 0.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int regionMinMax(region *reg, int loop, int *xmin, int *ymin, int *xmax, int *ymax)
{
  int y, xmi, ymi, xma=0, yma;
  yline* yl;

  if (NOT reg) {
    *xmin=0; *ymin=0; *xmax=0; *ymax=0;
    return(1);
  }

  xmi = reg->ylines[reg->ymin]->xmin;
  *ymin = ymi = reg->ymin;
  *ymax = yma = reg->ymax;
  while (TRUE) {
    for (y=ymi; y LE yma; y++) {
      yl = reg->ylines[y];
      if (yl->xmin LT xmi)
        xmi = yl->xmin;
      while (yl->next) yl=yl->next;
        if (yl->xmax GT xma) xma = yl->xmax;
    }

    if (NOT loop) break;
    if (NOT (reg=reg->next)) break;
    ymi=reg->ymin;
    yma=reg->ymax;
    if (ymi LT *ymin) *ymin = ymi;
    if (yma GT *ymax) *ymax = yma;
  }
  *xmin = xmi;
  *xmax = xma;
  return(0);
}



int regionXmin(region *reg, int loop, int *xmin)
{
  int y, ymin, ymax, xmi;

  if (NOT reg) {
    *xmin = 0;
    return(1);
  }
  xmi = reg->ylines[reg->ymin]->xmin;
  while (TRUE) {
    ymin=reg->ymin;
    ymax=reg->ymax;
    for (y=ymin; y LE ymax; y++)
      if (reg->ylines[y]->xmin LT xmi)
        xmi = reg->ylines[y]->xmin;
    if (NOT loop) break;
    if (NOT (reg=reg->next)) break;
  }
  *xmin = xmi;
  return(0);
}


int regionXmax(region *reg, int loop, int *xmax)
{
  int y, ymin, ymax, xma=0;
  yline* yl;

  if (NOT reg) {
    *xmax = 0;
    return(1);
  }

  while (TRUE) {
    ymin=reg->ymin;
    ymax=reg->ymax;
    for (y=ymin; y LE ymax; y++) {
      yl = reg->ylines[y];
      if (yl) {
        while (yl->next) yl=yl->next;
        if (yl->xmax GT xma) xma = yl->xmax;
      }
    }
    if (NOT loop) break;
    if (NOT (reg=reg->next)) break;
  }
  *xmax = xma;
  return(0);
}


int regionYmin(region *reg, int loop, int *ymin)
{
  int ymi;

  if (NOT reg) {
    *ymin = 0;
    return(1);
  }

  ymi = reg->ymin;
  while (TRUE) {
    if (NOT loop) break;
    if (NOT (reg=reg->next)) break;
    if (reg->ymin LT ymi) ymi = reg->ymin;
  }
  *ymin = ymi;
  return(0);
}


int regionYmax(region *reg, int loop, int *ymax)
{
  int yma;
  if (NOT reg) {
    *ymax = 0;
    return(1);
  }

  yma = reg->ymax;
  while (TRUE) {
    if (NOT loop) break;
    if (NOT (reg=reg->next)) break;
    if (reg->ymax GT yma) yma = reg->ymax;
  }
  *ymax = yma;
  return(0);
}
