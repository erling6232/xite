

/*

________________________________________________________________

        regionSort
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



/*F:regionSort*

________________________________________________________________

		regionSort
________________________________________________________________

Name:		regionSort - sort regions according to object number

Syntax:         | #include <xite/region.h>
		|
                | int regionSort( region** regArr,
                |    region*** objArr, int rsize, int* osize );
Description:    Sort all regions accessible from vector "regArr"
                according to their object number. Put all regions 
                with the same object number in a list. The resulting
		list is accessed through the vector "objArr", accessible
		from 0 to "*osize". "regArr" should be accessible from
		0 to "rsize".

Return value:   0 => ok
                1 => Can't alloc memory

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionPoint(3), regionSearch(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor Loennestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int regionSort(region **regArr, region ***objArr, int rsize, int *osize)
{
  int n1, n2, maxonr=0;
  region **lastReg, *reg;

  FOR (n1=0; n1 LE rsize; n1++)
    reg = regArr[n1];
    WHILE (reg)
      if (reg->objectnr GT maxonr) maxonr = reg->objectnr;
      reg = reg->next;
    ENDWHILE;
  ENDFOR;

  *objArr = (region**)calloc((maxonr+1),sizeof(region*));
  lastReg = (region**)malloc((maxonr+1)*sizeof(region*));
  if (NOT *objArr) return(1);
  if (NOT lastReg) return(1);

  FOR (n1=0; n1 LE rsize; n1++)
    reg = regArr[n1];
    WHILE (reg)
      n2 = reg->objectnr;
      if ((*objArr)[n2]) lastReg[n2] = lastReg[n2]->next = reg;
      else (*objArr)[n2] = lastReg[n2] = reg;
      reg = reg->next;
    ENDWHILE;
  ENDFOR;
  for (n1=0;  n1 LE maxonr; n1++)
    if ((*objArr)[n1]) lastReg[n1]->next = nilr;
  free(lastReg);
  *osize = maxonr;
  return(0);
}
