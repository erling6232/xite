

/*C*

________________________________________________________________

        regionInsert
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
#include <stdlib.h>
#include <xite/region.h>
#include <xite/blab.h>
#include <xite/message.h>
#include XITE_MALLOC_H

#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif




/*F:regionInsertYline*

________________________________________________________________

		regionInsertYline
________________________________________________________________

Name:		regionInsertYline - insert yline into newly created region

Syntax:         | #include <xite/region.h>
		| 
                | yline *regionInsertYline( region* reg, int y,
                |    int xmin, int xmax );
Description:    Insert an yline, the line xmin..xmax on line y,
                into region reg. After all ylines have been inserted
		into the region, regionEndInsert should be called.

		Notice that if the new yline segment overlaps with two 
		or more existing ylines, the superfluous yline objects
		will be free'd.

Restrictions:   The new yline must be connected to the existing region,
		with the neighborhod connectedness of the region
		(4 or 8). The new yline must lie within the ymin..ymax
		interval of the region object.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionDeleteYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Diagnostics:	| Warning 2: y less than ymin
		| Warning 3: y greater than ymax
		| Warning 4: Object not connected
		
Return value:   The inserted or updated yline object if ok, nil otherwise.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
yline* regionInsertYline(reg, y, xmin, xmax)
region* reg;
int y, xmin, xmax;
#else /* FUNCPROTO */
yline* regionInsertYline(region *reg, int y, int xmin, int xmax)
#endif /* FUNCPROTO */
{
  yline *yl, *newyl;
  int empty = TRUE;
  int ok = FALSE;
  int ncoffset, yy, ay, ymin, ymax;
  yline **lines;

  if (y LT reg->amin || y GT reg->amax)
    { 
      ay = reg->amin;
      ymin = MIN(y, reg->amin);
      ymax = MAX(y, reg->amax);

      lines = (yline **) calloc((ymax-ymin+1), sizeof (yline *));
      if (lines == (yline **)0) 
        { Error(2, "regionInsertYline: Malloc failed\n"); return(nily); }
      lines = &lines[-ymin];

      for(yy=reg->ymin; yy<=reg->ymax; yy++)
	lines[yy] = reg->ylines[yy];

      free(&(reg->ylines[ay]));
      reg->ylines = lines;
      reg->amin = ymin;
      reg->amax = ymax;
      reg->ymin = MIN(y, reg->ymin);
      reg->ymax = MAX(y, reg->ymax);
    }

  if (y GT reg->ymax)
    { Warning(3, "regionInsertYline: y greater than ymax\n"); return(nily); }
  ncoffset = (reg->nc == 4) ? 0 : 1;

  /* test the same line */
  yl = reg->ylines[y];
  if (yl) {
    empty = FALSE;
    while ((yl) ? (yl->xmax LT xmin-1) : FALSE) yl = yl->next;
    if ((yl) ? (yl->xmin LE xmax+1) : FALSE) {
      yl->xmin = MIN(yl->xmin, xmin);
      yl->xmax = MAX(yl->xmax, xmax);
      WHILE ((yl->next) ? (yl->xmax GE yl->next->xmin-1) : FALSE)
        yl->xmax = MAX(yl->xmax, yl->next->xmax);
        newyl = yl->next;
        yl->next = yl->next->next;
        free(newyl);
      ENDWHILE;
      return(yl);
    }
  }

  /* test if touching line above */
  yl = (y GT reg->ymin) ? reg->ylines[y-1] : nily;
  IF (yl)
    empty = FALSE;
    while ((yl) ? (yl->xmax LT xmin-ncoffset) : FALSE) yl = yl->next;
    if ((yl) ? (yl->xmin LE xmax+ncoffset) : FALSE) ok = TRUE;
  ENDIF;

  /* test if touching line below */
  IF (! ok)
    yl = (y LT reg->ymax) ? reg->ylines[y+1] : nily;
    IF (yl)
      empty = FALSE;
      while ((yl) ? (yl->xmax LT xmin-ncoffset) : FALSE) yl = yl->next;
      if ((yl) ? (yl->xmin LE xmax+ncoffset) : FALSE) ok = TRUE;
    ENDIF;
  ENDIF;

 /* If the object is empty, isertion is ok */
  IF (empty)
    for (yy=reg->ymin; yy LE reg->ymax; yy++)
      if (reg->ylines[yy]) { Warning(4, "Object not connected\n"); return(nily); }
    ok = TRUE;
  ENDIF;

  if (! ok) { Warning(4, "Object not connected\n"); return(nily); }

  /* new yline object can be inserted */
  newyl = (yline*)malloc(sizeof(yline));
  newyl->xmin = xmin;
  newyl->xmax = xmax;

  yl = reg->ylines[y];
  if (! yl) {
    reg->ylines[y] = newyl;
    newyl->next = nily;
    return(newyl);
  }

  /* special case: first object */
  if (xmax LT yl->xmin) {
    newyl->next = yl;
    reg->ylines[y] = newyl;
    return(newyl);
  }

  /* general case */
  while ((yl->next) ? (yl->next->xmin LT xmin) : FALSE) yl = yl->next;
  newyl->next = yl->next;
  yl->next = newyl;
  return(newyl);
}





/*F:regionDeleteYline*

________________________________________________________________

		regionDeleteYline
________________________________________________________________

Name:		regionDeleteYline - delete yline from a region

Syntax:         | #include <xite/region.h>
		| 
                | yline *regionDeleteYline( region* reg, int y,
                |    int xmin, int xmax );
Description:    Delete an yline, the line xmin..xmax on line y,
                in region reg. After all ylines have been deleteed
		in the region, regionEndInsert should be called.

Restrictions:   After removing the yline, the region must still be
                4 or 8 connected.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Diagnostics:	| Warning 2: y less than ymin
		| Warning 3: y greater than ymax
		| Warning 4: object not connected.
		| Warning 5: no pixels in the given area.
		
Return value:   The deleted or updated yline object if ok, nil otherwise.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
yline* regionDeleteYline(reg, y, xmin, xmax)
region* reg;
int y, xmin, xmax;
#else /* FUNCPROTO */
yline* regionDeleteYline(region *reg, int y, int xmin, int xmax)
#endif /* FUNCPROTO */
{
  yline *yl, *newyl, *oldyl, **yptr;
  int empty = TRUE;
  int ncoffset;

  if (y LT reg->ymin)
    { Warning(2, "regionDeleteYline: y less than ymin\n"); return(nily); }
  if (y GT reg->ymax)
    { Warning(3, "regionDeleteYline: y greater than ymax\n"); return(nily); }
  ncoffset = (reg->nc == 4) ? 0 : 1;

  /* test the same line */
  yl = reg->ylines[y];
  yptr = &reg->ylines[y];
  empty = FALSE;
  if (!yl)
    { Warning(5, "regionDeleteYline: no pixels in the given area\n"); 
	return(nily); 
    }  
  oldyl = nily;
  while (yl != nily)
    { 
      if (yl->xmin > xmax) break;
      if (yl->xmax < xmin) 
	{  yptr = &yl; yl = yl->next; continue; }
      if (xmin <= yl->xmin && xmax>= yl->xmax)
	{ oldyl = yl; yl = yl->next; *yptr=yl; continue; }
      if (xmin > yl->xmin && xmax < yl->xmax)
	{  oldyl = newyl = (yline*)malloc(sizeof(yline));
           newyl->xmin = xmax+1;
           newyl->xmax = yl->xmax;
	   newyl->next = yl->next;
	   yl->xmax = xmin-1;
	   yl->next = newyl;
	   break;
       }
      if (xmin <= yl->xmin && xmax < yl->xmax)
	{ oldyl = yl; yl->xmin = xmax+1; break; }
      if (xmin > yl->xmin && xmax >= yl->xmax)
	{ oldyl = yl; yl->xmax = xmin-1; break; }
      Error(6, "regionDeleteYline: internal xmin=%d, xmax=%d, yl->xmin=%d, yl->xmax=%d\n", xmin, xmax, yl->xmin, yl->xmax);
  }
  if (!oldyl) Warning(5, "regionDeleteYline: no pixels in the given area\n"); 
  return(oldyl);
}





/*F:regionEndInsert*

________________________________________________________________

		regionEndInsert
________________________________________________________________

Name:		regionEndInsert - clean up region at end of yline insertion

Syntax:         | #include <xite/region.h>
		| 
                | BiffStatus regionEndInsert( region* reg );
Description:	the "ylines" array in reg is searched for first and
                last occurrence of a pointer, and the array is then
		reallocated to fit exactly the required size.
		Finally reg->ymin and reg->ymax is updated.

		This routine should always be called after inserting
		or deleting ylines into an region, unless you are sure
		that the object really spans ymin..ymax.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Return value:   | 0 => ok
                | 2 => No ylines in the region	(Warning)
		| 3 => Malloc failed		(Error)

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
BiffStatus regionEndInsert(reg)
region* reg;
#else /* FUNCPROTO */
BiffStatus regionEndInsert(region *reg)
#endif /* FUNCPROTO */
{
  int ymin, ymax, y;

  for (y=reg->ymin; y LE reg->ymax  AND(! reg->ylines[y]); y++);
  ymin = y;
  for (y=reg->ymax; y GE reg->ymin  AND(! reg->ylines[y]); y--);
  ymax = y;

  if (ymin GT ymax) return(Warning(2, "regionEndInsert: No ylines in the region\n"));
  IF ((ymin != reg->ymin) OR (ymax != reg->ymax))
 /*   yls = (yline**)malloc((ymax-ymin+1) * sizeof(yline*));
    if (! yls) return(Error(3, "regionEndInsert: Malloc failed\n"));
    yls = &(yls[-ymin]);
    for (y=ymin; y LE ymax; y++) yls[y] = reg->ylines[y];
    free(&(reg->ylines[reg->ymin]));
    reg->ylines = yls; */
    reg->ymin = ymin;
    reg->ymax = ymax;
  ENDIF;
  return(0);
}




#ifdef TEST_CREATE

main(argc, argv)
int argc;
char *argv[];
{
  region *reg;
  yline *yl;
  int y, xmin, xmax;
  char c;
  char buf[100];

  reg = regionCreate(1, 2, 4, 1, 100);
  while(1)
    {
      printf("> ");
      gets(buf);
      sscanf(buf, "%c %d %d %d", &c, &y, &xmin, &xmax);
      switch(c)
	{
	  case 'i':
	  case 'I':
	    regionInsertYline(reg, y, xmin, xmax);
	    break;
	  case 'd':
	  case 'D':
	    regionDeleteYline(reg, y, xmin, xmax);
	    break;
	  case 'e':
	  case 'E':
	    regionEndInsert(reg);
	    break;
	  case 'q':
	  case 'Q':
            exit(0);
	}
      for(y=reg->ymin; y<=reg->ymax; y++)
	{
          if ((yl = reg->ylines[y]))
	    {
	      printf("Line %d:", y);
	      while(yl) {
		  printf(" %d %d", yl->xmin, yl->xmax); yl=yl->next; }
	      printf("\n");
	    }
        }
    }
}

#endif







