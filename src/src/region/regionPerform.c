

/*C*

________________________________________________________________

        regionPerform
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/message.h>
#include <xite/region.h>



/*F:regionPerform*

________________________________________________________________

		regionPerform
________________________________________________________________

Name:		regionPerform - perform any procedure for each point in region

Syntax:         | #include <xite/region.h>
                |
                | BiffStatus regionPerform( void* data,
                |    region* reg, int loop,
                |    regionfunc performproc, void* performdata );
Description:    'regionPerform' calls the specified procedure performproc
                for every point (x,y) in the region reg. The procedure
		call syntax is

                |  performproc(data, x, y, performdata)
		|  void *data;
		|  int x, y;
		|  void *performdata;

		The specified procedure should thus match this syntax.
		data is typically, but need not be, a band or an image.
		performdata is pointer to any other data structure needed
		by performproc. data and/or performdata may be nil.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Return value:   Status, 0 => ok. See below for error numbers and messages.

Diagnostics:    | Warning 2: region argument nil
		| Warning 3: performproc argument nil

Example:	| To measure the "volume" under a region:
		|
		|   int volumeproc(band, x, y, vol)
		|   IBAND band;
		|   int x, y;
		|   int *vol;
		|   {
		|     *vol += band[y][x];
		|     return(0);
		|   }
		|
		|   region *reg;
		|   int volume=0;
		|   <create reg>
		|
		|   regionPerform(band, reg, 0, volumeproc, &volume);

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


BiffStatus regionPerform(void *data, region *reg, int loop, regionfunc performproc, void *performdata)
{
  yline *yl;
  int x, y;

  if (NOT reg) return(Warning(2, "regionPerform: region argument nil\n"));
  if (NOT performproc)
    return(Warning(3, "regionPerform: performproc argument nil\n"));

  WHILE(reg)
    FOR (y=reg->ymin; y LE reg->ymax; y++)
      yl = reg->ylines[y];
      WHILE (yl)
        for (x=yl->xmin; x LE yl->xmax; x++)
          performproc(data, x, y, performdata);
        yl = yl->next;
      ENDWHILE;
    ENDFOR;
    reg = (loop ? reg->next : nilr);
  ENDWHILE
  return(0);
}


