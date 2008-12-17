
/*

________________________________________________________________

        regionFree
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
#include XITE_MALLOC_H



/*F:regionFree*

________________________________________________________________

		regionFree
________________________________________________________________

Name:		regionFree - free the memory occupied by a region

Syntax:         | #include <xite/region.h>
		|
                | void regionFree( region** reg, int loop );
Description:    Frees all memory occupied by the region object,
                including the yline array and all its yline objects,
		and assigns NIL to the reg pointer. If "loop" is
		true, the next pointer in reg is followed, and
		all regions are free'd.

Return value:   none

Author:		Tor Loennestad, BLAB, ifi, UiO

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3),
		regionDraw(3), regionHistoUpdate(3), regionHisto(3),
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Example:        | region *reg;
		| ..create and use reg..
		| regionFree(&reg, FALSE);

Id: 		$Id$
________________________________________________________________

*/


void regionFree(region **reg, int loop)
{
  int y, ymax, ymin;
  yline *yl1, *yl2;

  ymax = (*reg)->ymax;
  ymin = (*reg)->ymin;
  FOR (y=ymin; y LE ymax; y++)
    IF ((yl1 = (*reg)->ylines[y]))
      WHILE ((yl2 = yl1->next))
        free(yl1);
        yl1 = yl2;
      ENDWHILE;
      free(yl1);
    ENDIF;
  ENDFOR;
  free(&((*reg)->ylines[ymin]));
  if ((*reg)->next AND loop) regionFree(&((*reg)->next), loop);
  free(*reg);
  *reg = nilr;
}

