

/*C*

________________________________________________________________

        regionCreate
        $Id$
        Copyright 1991, Blab, UiO
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
#include <xite/biff.h>
#include <xite/region.h>
#include <xite/blab.h>
#include <xite/message.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif




/*F:regionCreate*

________________________________________________________________

		regionCreate
________________________________________________________________

Name:		regionCreate - create a new region

Syntax:         | #include <xite/region.h>
		| 
                | region *regionCreate( int objnr, int regnr,
                |    int nc, int ymin, int ymax );
Description:    Creates a new region object, and initializes the
                attributes corresponding to the routine arguments.
		The "ylines" vector is allocated and indexable on
		ymin..ymax, but all pointers are set to nil, no
		yline objects are created. The region is thus not
		in a consistent state. Use regionInsertYline repeatedly
		and end with regionEndInsert to get the region object
		in a consistent state.

Restrictions:   nc==4 or nc==8, ymin <= ymax

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionDraw(3),
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

Diagnostics:	| Error 2: Malloc failed

Return value:   the newly created region. Nil if error

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


region* regionCreate(int objnr, int regnr, int nc, int ymin, int ymax)
{
  region* newreg;
  yline** newyls;

  newreg = (region*)malloc(sizeof(region));
  if (! newreg) { Error(2, "regionCreate: Malloc failed\n"); return(nilr); }
  newreg->objectnr = objnr;
  newreg->regionnr = regnr;
  newreg->ymin = newreg->amin = ymin;
  newreg->ymax = newreg->amax = ymax;
  newreg->sons = 0;
  newreg->nc = nc;
  newreg->next = nilr;
  newreg->previous = nilr;
  newreg->userdata = (void*)0;
  newyls = (yline**)calloc(ymax-ymin+1, sizeof(yline*));
  if (! newyls) { free(newreg); Error(2, "regionCreate: Malloc failed\n"); return(nilr); }
  newreg->ylines = &(newyls[-ymin]);
  return(newreg);
}



