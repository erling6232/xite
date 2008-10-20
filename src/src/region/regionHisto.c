

/*

________________________________________________________________

        regionHisto
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
#include <xite/histo.h>
#include <xite/region.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:regionHistoUpdate=regionHisto*/
/*F:regionHisto*

________________________________________________________________

		regionHisto
________________________________________________________________

Name:		regionHisto, regionHistoUpdate - make a histogram of pixels
                in a region

Syntax:         | #include <xite/region.h>
		| #include <xite/region.h>
		|
                | int regionHisto( IBAND band, region* reg,
                |    int loop, histogram histo );
		|
                | int regionHistoUpdate( IBAND band,
                |    region* reg, int loop, histogram histo );
Description:    'regionHisto' creates a histogram over the pixels in "band"
		that are inside region "reg". If "loop" is true,
		the next pointer in "reg" is followed until NIL,
		and the pixels in all regions are included in
		the histogram. If "reg" contains pixels that are
		not inside "band", these pixels will not influence
		the output histogram. If 'reg' == NULL the histogram
		if filled with 0.

		'regionHistoUpdate' updates a histogram over the pixels in
		'band' that are inside region "reg". If "loop" is true,
                the next pointer in "reg" is followed until NIL,
                and the pixels in all regions are included in
                the histogram. If "reg" contains pixels that are
                not inside "band", these pixels will not influence
                the output histogram.

Return value:   | 0 => OK
		| 1 => bad pixel type in "band", accepts only UNS_BYTE.
		| 2 => reg points to NIL

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3),
		regionDraw(3), regionFree(3),
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor L|nnestad, BLAB, ifi, UiO

Example 1:      Compute the histogram of a region:
		| IBAND b;
		| region *reg;
		| histogram h;
		| ..create reg..
		| regionHisto(b, reg, FALSE, h);
Example 2:      Compute the histogram of all regions with objectnr == 2
		| IBAND b;
		| region *reg;
		| histogram h;
		| ..create reg..
		| regionHisto(b, NULL, FALSE, h);  / * Zero histogram * /
		| for (rnr=1; rnr<=rsize; r++)
		|   if (regs[rnr]->objectnr == 2) 
		|     regionHistoUpdate(b, regs[rnr], FALSE, h);
Id: 		$Id$
________________________________________________________________

*/

int regionHisto(IBAND band, region *reg, int loop, int *histo)
{
  int x, y, ymin, ymax, xmin, xmax, xsize, ysize, h;
  yline *yl;

  for (h=0; h LE 255; h++) histo[h] = 0;
  if (!band   || Ipixtyp(band) NE Iu_byte_typ) return(1);
  if (reg EQ nilr) return(2);
  xsize = Ixsize(band);
  ysize = Iysize(band);

  LOOP
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    FOR (y=ymin; y LE ymax; y++)
      yl = reg->ylines[y];
      WHILE (yl)
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        for (x=xmin; x LE xmax; x++) histo[band[y][x]]++;
        yl = yl->next;
      ENDWHILE;
    ENDFOR;
    EXITIF(NOT loop);
    EXITIF(NOT (reg=reg->next));
  ENDLOOP;
  return(0);
}

int regionHistoUpdate(IBAND band, region *reg, int loop, int *histo)
{
  int x, y, ymin, ymax, xmin, xmax, xsize, ysize;
  yline *yl;

  if (!band || Ipixtyp(band) NE Iu_byte_typ) return(1);
  if (reg EQ nilr) return(2);
  xsize = Ixsize(band);
  ysize = Iysize(band);

  LOOP
    ymin = MAX(1, reg->ymin);
    ymax = MIN(ysize, reg->ymax);
    FOR (y=ymin; y LE ymax; y++)
      yl = reg->ylines[y];
      WHILE (yl)
        xmin = MAX(1, yl->xmin);
        xmax = MIN(xsize, yl->xmax);
        for (x=xmin; x LE xmax; x++) histo[band[y][x]]++;
        yl = yl->next;
      ENDWHILE;
    ENDFOR;
    EXITIF(NOT loop);
    EXITIF(NOT (reg=reg->next));
  ENDLOOP;
  return(0);
}


