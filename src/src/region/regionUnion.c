

/*C*

________________________________________________________________

        regionUnion
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
#include <xite/region.h>
#include <xite/message.h>
#include XITE_MALLOC_H

#define NEW(type) (type*)malloc(sizeof(type))
#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:regionCopy*

________________________________________________________________

		regionCopy
________________________________________________________________

Name:		regionCopy - create a copy of a region

Syntax:         | #include <xite/region.h>
		|
                | region *regionCopy( region* reg, int loop );
Description:    Every part of 'reg' is copied into a new region,
                returned as function argument. If loop is TRUE, the 
		next-pointer in reg is followed, all regions are
		copied. The next-pointer in the last region object
		is set to nil.

Return value:   | The region copy
		| 0 => an error occurred

Diagnostics:    | Warning 2 : region argument nil
                | Error 3 : malloc failed

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionPoint(3), regionSearch(3), regionSort(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor L|nnestad, BLAB, ifi, UiO

Example:        | region *r1, *r2;
		| ..create r1..
		| r2 = regionCopy(r1, FALSE);

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
region* regionCopy(reg, loop)
region* reg;
int loop;
#else /* FUNCPROTO */
region* regionCopy(region *reg, int loop)
#endif /* FUNCPROTO */
{
  int y;
  region *r, *r1;
  yline *yl1, *yl2;

  if (NOT reg) {
    Warning(2, "regionCopy: region argument nil\n");
    return(nilr);
  }

  LOOP
    r = (r1 = regionCreate(reg->objectnr, reg->regionnr,
			   reg->nc, reg->ymin, reg->ymax));
    if (NOT r) {
      Error(3, "regionCopy: malloc failed\n");
      return(nilr);
    }

    r1->sons	= reg->sons;
    FOR (y=r1->ymin; y LE r1->ymax; y++)
      yl1 = (r1->ylines[y] = NEW(yline));
      yl2 = reg->ylines[y];
      WHILE (yl1)
        yl1->xmin = yl2->xmin;
        yl1->xmax = yl2->xmax;
        IF ((yl2 = yl2->next))
          yl1 = (yl1->next = NEW(yline));
          if (NOT yl1) {
	    Error(3, "regionCopy: malloc failed\n");
	    return(nilr);
	  }
        ELSE
          yl1 = (yl1->next = nily);
        ENDIF;
      ENDWHILE;
    ENDFOR;
    if (NOT loop) break;
    if (NOT (reg = reg->next)) break;
    r1 = (r1->next = regionCreate(reg->objectnr, reg->regionnr,
				  reg->nc, reg->ymin, reg->ymax));
    if (NOT r1) {
      Error(3, "regionCopy: malloc failed\n");
      return(nilr);
    }
  ENDLOOP;
  r1->next = nilr;
  return(r);
}   




#define TESTNIL(yl) if (NOT yl) { Error(4, "regionUnion: malloc failed\n"); return(nilr); }
#define COPYYL(fromyl,toyl) { toyl->xmin = fromyl->xmin; toyl->xmax = fromyl->xmax; }
#define TOUCH(yl1, yl2) ((yl1 AND yl2) ? \
           ((NOT (yl1->xmax LT yl2->xmin-1)) AND (NOT (yl1->xmin GT yl2->xmax+1))) \
	   : FALSE)
#define COPYMERGE(yl1, yl2, yl) 						\
        IF (NOT yl1)								\
          yl->xmin = yl2->xmin;							\
          yl->xmax = yl2->xmax;							\
          yl2 = yl2->next;							\
        ELSEIF (NOT yl2)							\
          yl->xmin = yl1->xmin;							\
          yl->xmax = yl1->xmax;							\
          yl1 = yl1->next;							\
        ELSE									\
          yl->xmin = MIN(yl1->xmin, yl2->xmin);					\
          while (TOUCH(yl1, yl2))						\
            if (yl1->xmax LT yl2->xmax) yl1 = yl1->next;			\
            else yl2 = yl2->next;						\
          IF (NOT yl1) yl->xmax = yl2->xmax; yl2 = yl2->next;			\
          ELSEIF (NOT yl2)  yl->xmax = yl1->xmax; yl1 = yl1->next;		\
          ELSEIF (yl1->xmin LT yl2->xmin) yl->xmax = yl1->xmax; yl1 = yl1->next;\
          ELSE yl->xmax = yl2->xmax; yl2 = yl2->next;				\
          ENDIF;								\
        ENDIF;

/*F:regionUnion*

________________________________________________________________

		regionUnion
________________________________________________________________

Name:		regionUnion - calculate the union of two regions

Syntax:         | #include <xite/region.h>
		|
                | region *regionUnion( region* reg1,
                |    region* reg2, int nc );
Description:    'regionUnion' creates and returns the union of the
                two input regions reg1 and reg2. nc specifies
		the neighborhood connectedness of the new region:

		| nc=1 => use reg1->nc
		| nc=2 => use reg2->nc
		| nc=4 => use nc = 4-neighbor
		| nc=8 => use nc = 8-neighbor

		If the two regions are not connected, they are simply
		copied individually, and connected by setting

                | copy(reg1)->next = copy(reg2)

		- and a pointer to the copy of reg1 is returned.

Return value:   | The region union,
                | 0 => an error occurred

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionPoint(3), regionSearch(3), regionSort(3), regionCopy(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Diagnostics:    | Warning 2 : region argument nil
                | Error 3 : bad nc argument
		| Error 4 : malloc failed

Author:		Tor L|nnestad, BLAB, ifi, UiO

Example:        | region *reg1, *reg2, *reg3;
		| ..create reg1 and reg2..
		| reg3 = regionUnion(reg1, reg2, 4);

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
region* regionUnion(reg1, reg2, nc)
region *reg1, *reg2;
int nc;
#else /* FUNCPROTO */
region* regionUnion(region *reg1, region *reg2, int nc)
#endif /* FUNCPROTO */
{
  int overlap, adjacent, y, y1, y2;
  yline *yl, *yl1, *yl2;
  region *newreg, *reg;

  if ((NOT reg1) OR (NOT reg2)) {
    Warning(2, "regionUnion: region argument nil\n");
    return(nilr);
  }

  if (nc==1) nc = reg1->nc;
  else if (nc==2) nc = reg2->nc;
  else if ((nc != 4) AND (nc !=8)) {
    Error(3, "regionUnion: bad nc argument\n");
    return(nilr);
  }

  regionNeighbor(reg1, reg2, &overlap, &adjacent, nc);
  IF (overlap OR adjacent)
    /* combine */
    newreg = regionCreate(reg1->objectnr, reg1->regionnr, nc,
             MIN(reg1->ymin, reg2->ymin), MAX(reg1->ymax, reg2->ymax));
    if (NOT newreg) return(nilr);
    /* Copy first ylines */
    reg = (reg1->ymin < reg2->ymin) ? reg1 : reg2;
    y1 = MAX(reg1->ymin, reg2->ymin);
    y2 = MIN(reg1->ymax, reg2->ymax);
    FOR (y=reg->ymin; y LT y1; y++)
      yl1 = reg->ylines[y];
      yl = newreg->ylines[y] = NEW(yline); TESTNIL(yl);
      COPYYL(yl1, yl);
      WHILE ((yl1 = yl1->next))
        yl = yl->next = NEW(yline); TESTNIL(yl);
        COPYYL(yl1, yl);
      ENDWHILE;
      yl->next = nily;
    ENDFOR;

    /* Merge.. */
    FOR (y=y1; y LE y2; y++)
      yl1 = reg1->ylines[y];
      yl2 = reg2->ylines[y];
      yl = newreg->ylines[y] = NEW(yline); TESTNIL(yl);
      COPYMERGE(yl1, yl2, yl);
      WHILE (yl1 OR yl2)
        yl = yl->next = NEW(yline); TESTNIL(yl);
        COPYMERGE(yl1, yl2, yl);
      ENDWHILE;
      yl->next = nily;
    ENDFOR;

    /* Copy last ylines..*/
    reg = (reg1->ymax > reg2->ymax) ? reg1 : reg2;
    FOR (y=y2+1; y LE reg->ymax; y++)
      yl1 = reg->ylines[y];
      yl = newreg->ylines[y] = NEW(yline); TESTNIL(yl);
      COPYYL(yl1, yl);
      WHILE ((yl1 = yl1->next))
        yl = yl->next = NEW(yline); TESTNIL(yl);
        COPYYL(yl1, yl);
      ENDWHILE;
      yl->next = nily;
    ENDFOR;
  ELSE
    newreg = regionCopy(reg1, FALSE);
    if (NOT newreg) return(nilr);
    newreg->next = regionCopy(reg2, FALSE);
    if (NOT newreg->next) return(nilr);
    newreg->next->next = nilr;
  ENDIF;
  return(newreg);
}

/*F:regionOverlap*

________________________________________________________________

		regionOverlap
________________________________________________________________

Name:		regionOverlap - do two regions overlap?

Syntax:         | #include <xite/region.h>
		|
                | int regionOverlap( region* r1, region* r2 );
Description:    Returns TRUE if reg1 and reg2 overlap: if there is
                one or more pixels that are inside reg1 as well as
		reg2.

Return value:   | TRUE  => reg1 and reg2 overlap
                | FALSE => reg1 and reg2 do not overlap - or argument nil

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionPoint(3), regionSearch(3), regionSort(3), regionCopy(3),
		regionUnion(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Diagnostics:    | Warning 2 : region argument nil

Author:		Tor L|nnestad, BLAB, ifi, UiO

Example:        | region *reg1, *reg2;
		| ..create reg1 and reg2..
		| if (regionOverlap(reg1, reg2)) ....

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int regionOverlap(r1, r2)
region *r1, *r2;
#else /* FUNCPROTO */
int regionOverlap(region *r1, region *r2)
#endif /* FUNCPROTO */
{
  int y, ymin, ymax;
  yline *yl1, *yl2;

  if ((r1 == nilr) || (r2 == nilr)) {
    Warning(2, "regionOverlap: region argument nil\n"); return(FALSE);
  }

  ymin = MAX(r1->ymin, r2->ymin);
  ymax = MIN(r1->ymax, r2->ymax);
  FOR (y=ymin; y LE ymax; y++)
    yl1 = r1->ylines[y];
    yl2 = r2->ylines[y];
    WHILE(yl1 && yl2)
      IF (yl1->xmin < yl2->xmin)
        if (yl1->xmax >= yl2->xmin) return(TRUE);
        else yl1 = yl1->next;
      ELSE 
        if (yl2->xmax >= yl1->xmin) return(TRUE);
        else yl2 = yl2->next;
      ENDIF;
    ENDWHILE;
  ENDFOR
  return(FALSE);
}




/*F:regionNeighbor*

________________________________________________________________

		regionNeighbor
________________________________________________________________

Name:		regionNeighbor - are two regions neighbors?

Syntax:         | #include <xite/region.h>
		|
                | int regionNeighbor( region* reg1, region* reg2,
                |    int* overlap, int* adjacent, int nc );
Description:    Returns overlap=TRUE if reg1 and reg2 overlaps,
                adjacent=TRUE if the two regions are adjacent
		but do not overlap. If the two regions are not
		neighbors, both overlap and adjacent are FALSE.
		nc, neighborhood connectedness, may be one of:

		| 1 : use the nc value found in reg1
		| 2 : use the nc value found in reg2
		| 4 : use 4-neighbours
		| 8 : use 8-neighbours

Return value:   | 0 => ok
                | 1 => reg1 or reg2 are nil
		| 2 => bad nc value

Diagnostics:    | Warning 2 : region argument nil
                | Error 3 : bad nc argument

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionPoint(3), regionSearch(3), regionSort(3), regionCopy(3),
		regionUnion(3), regionOverlap(3), regionVolume(3),
		regionYline(3)

Author:		Tor L|nnestad, BLAB, ifi, UiO
                New algorithm by Otto Milvang

Example:        | region *reg1, *reg2;
		| int overlap, adjacent;
		| ..create reg1 and reg2..
		| regionNeighbor(reg1, reg2, &overlap, &adjacent, 4);

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int regionNeighbor(reg1, reg2, overlap, adjacent, nc)
region *reg1, *reg2;
int *overlap, *adjacent, nc;
#else /* FUNCPROTO */
int regionNeighbor(region *reg1, region *reg2, int *overlap, int *adjacent, int nc)
#endif /* FUNCPROTO */
{
  int y, ymin, ymax, xmin, xmax, ncoffset;
  yline *yl1, *yl2;

  *overlap = FALSE;
  *adjacent = FALSE;
  if ((reg1 == nilr) || (reg2 == nilr))
    return(Warning(2, "regionNeighbor: region argument nil\n"));
  if (nc == 1) nc = reg1->nc;
  else if (nc == 2) nc = reg2->nc;
  if ((nc != 4) && (nc != 8))
    return(Error(3, "regionOverlap: bad nc value\n"));
  ncoffset = (nc==4) ? (0) : (1);
  
  /* overlap? */
  if (regionOverlap(reg1, reg2)) {
    *overlap = TRUE;
    return(0);
  }

  /* adjacent? */
  ymin = MAX(reg1->ymin, reg2->ymin - 1);
  ymax = MIN(reg1->ymax, reg2->ymax + 1);
  FOR (y=ymin; y LE ymax; y++)
    yl1 = reg1->ylines[y];
    WHILE (yl1)
      xmin = yl1->xmin;
      xmax = yl1->xmax;
      /* adjacent above? */
      if (reg2->ymin <= y-1 && reg2->ymax >= y-1)
      {
        yl2 = reg2->ylines[y-1];
        while (yl2)
	{
          if (yl2->xmin > xmax + ncoffset) break;
          if (yl2->xmax < xmin - ncoffset) {yl2 = yl2->next; continue;}
          *adjacent=TRUE;
          return(0);
        }
      }

      /* adjacent on the line? */
      if (reg2->ymin <= y && reg2->ymax >= y)
      {
        yl2 = reg2->ylines[y];
        while (yl2)
	{
          if (yl2->xmin > xmax + 1) break;
          if (yl2->xmax < xmin - 1) {yl2 = yl2->next; continue;}
          *adjacent=TRUE;
          return(0);
        }
      }

      /* adjacent below? */
      if (reg2->ymin <= y+1 && reg2->ymax >= y+1)
      {
        yl2 = reg2->ylines[y+1];
        while (yl2)
	{
          if (yl2->xmin > xmax + ncoffset) break;
          if (yl2->xmax < xmin - ncoffset) {yl2 = yl2->next; continue;}
          *adjacent=TRUE;
          return(0);
        }
      }

      yl1 = yl1->next;
    ENDWHILE
  ENDFOR;
  return(0);
}
  
