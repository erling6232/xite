

/*

________________________________________________________________

        regionArea
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

/*F:regionArea*

________________________________________________________________

		regionArea
________________________________________________________________

Name:		regionArea - find the area of one or more regions

Syntax:         | #include <xite/region.h>
		|
                | int regionArea( region* reg, int loop,
                |    int* area );
Description:    Calculates the area of one or several regions.
		If "loop" is false, only the area of "reg" is
		measured. If true, the "next" pointer in reg is
		followed until nil, and all areas are accumulated.
                
Return value:   | 0 => ok
		| 1 => "reg" points to nil

See also:       regionAnalyse(1),
		regionConvexHull(3), regionCreate(3), regionDraw(3),
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

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int regionArea(region *reg, int loop, int *area)
{
  int y, ymax, ar=0;
  yline *yl;

  if (reg EQ nilr) {
    *area=0;
    return(1);
  }

  while (TRUE) {
    ymax = reg->ymax;
    for (y=reg->ymin; y LE ymax; y++)
      for (yl=reg->ylines[y]; yl NE nily; yl = yl->next)
        ar += YLXSIZE(yl);
    if (! loop) break;
    if (! (reg = reg->next)) break;
  }

  *area = ar;
  return(0);
}
