

/*

________________________________________________________________

        regionIsPoint.c
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



/*F:regionIsPoint*

________________________________________________________________

		regionIsPoint
________________________________________________________________

Name:		regionIsPoint - Is (x, y) a point in the region/list

Syntax:         | #include <xite/region.h>
		|
                | int regionIsPoint( region* reg, int loop,
                |    int x, int y );
Description:    If the point (x, y) is a point in the region
                return TRUE otherwise return FALSE.
		If "loop" is true, the next pointer in reg is 
		followed until NIL. 
Return value:   | 0 => Point not in region (region List)
		| 1 => Point is in the region (region List)

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
int regionIsPoint(reg, loop, x, y)
region* reg;
int loop, x, y;
#else /* FUNCPROTO */
int regionIsPoint(region *reg, int loop, int x, int y)
#endif /* FUNCPROTO */
{
  yline* yl;

  if (reg EQ nilr) return(0);

  while(reg)
    {
      if (y>=reg->ymin && y<=reg->ymax)
	{
	  yl = reg->ylines[y];
	  while(yl)
	    { 
	      if (x>=yl->xmin)
		if (x<=yl->xmax) return(1); 
	      yl = yl->next;
	    }
	}
      reg = loop ? reg->next : nilr;
    }
  return(0);
}




/*F:regionPoint*

________________________________________________________________

		regionPoint
________________________________________________________________

Name:		regionPoint - return a point (x, y) in the region

Syntax:         | #include <xite/region.h>
		|
                | int regionPoint( region* reg, int* x, int* y );
Description:    Returns the leftmost point of the first line
                in the region 'reg'.
Return value:   | 0 => ok
                | 1 => reg == NULL

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
int regionPoint(reg, x, y)
region* reg;
int *x, *y;
#else /* FUNCPROTO */
int regionPoint(region *reg, int *x, int *y)
#endif /* FUNCPROTO */
{
  yline* yl;

  if (reg EQ nilr) return(1);
  yl = reg->ylines[reg->ymin];
  *x = yl->xmin;
  *y = reg->ymin;
  return(0);
}


