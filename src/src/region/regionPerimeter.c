

/*

________________________________________________________________

        regionPerimeter
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
#include <xite/biff.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/region.h>
#include <xite/readarg.h>

#define MOVETO(N) 
/* printf(" N :Go to X=%d, Y=%d total per=%d\n", x, y, per); */
/*L*

________________________________________________________________

		region4perimeter
________________________________________________________________

Name:		regionPerimeter - Calculate 4-perimeter of a region

Syntax:		| #include <xite/region.h>
		|
		| static int regionPerimeter(reg, perimeter)
		| region *reg;
		| float* perimeter; 

Description:    Calculates the 4-perimeter of a region.
                
Return value:   0 : ok
                1 : "reg" was nil
		2 : "reg" had inconsistent structure

Author:		Tor Loennestad, BLAB, ifi, UiO
________________________________________________________________

*/


#ifndef FUNCPROTO
static int region4perimeter(reg, perimeter)
region *reg;
int* perimeter;
#else /* FUNCPROTO */
static int region4perimeter(region *reg, int *perimeter)
#endif /* FUNCPROTO */
{
  int x, y, dir, ymin, ymax, moved, per=0;
  yline *prevYl, *thisYl, *nextYl, *first, *second;

  if (reg EQ nilr) {
    *perimeter=0;
    return(1);
  }

  ymin = reg->ymin;
  ymax = reg->ymax;
  first = prevYl = reg->ylines[ymin];
  if (ymin EQ ymax) {
    *perimeter = 2 * (YLXSIZE(prevYl) - 1);
    return(0);
  }

  y = ymin+1;
  thisYl = reg->ylines[y];
  while ((thisYl->next) ? (thisYl->next->xmin LE prevYl->xmax) : FALSE)
    thisYl = thisYl->next;
  if ((thisYl->xmin GT prevYl->xmax) OR (thisYl->xmax LT prevYl->xmin)) {
    /* In this case the region is not connected, using nc=4 */
    *perimeter = 2 * (YLXSIZE(prevYl) - 1);
    return(0);
  }
  second = thisYl;
  if (prevYl->xmax LE thisYl->xmax) x = prevYl->xmax;
  else x = thisYl->xmax;
  dir = SOUTH;

  while (TRUE) {  /* rotate clockwise */
    moved = FALSE;
    switch (dir) {
    case SOUTH:
      
      /* look for next at north-east */
      
      if ((nextYl = prevYl->next) ?
          (nextYl->xmin LE thisYl->xmax) : FALSE) { /* go north */
	prevYl = thisYl;
	thisYl = nextYl;
	per += thisYl->xmin - x + 1;
	y--;
	x = thisYl->xmin;
	dir = NORTH;
	moved = TRUE; MOVETO(1);
      } else { /* look further south */
	if (y LT ymax) {
	  nextYl = reg->ylines[y+1];
	  while((nextYl->next) ?
		(nextYl->next->xmin LE thisYl->xmax) : FALSE)
	    nextYl = nextYl->next;
	  if ((nextYl->xmin LE thisYl->xmax)
	      AND (nextYl->xmax GE thisYl->xmin)) { /* go south */
	    if (thisYl->xmax LE nextYl->xmax) {
	      per += thisYl->xmax - x + 1;
	      x = thisYl->xmax;
	    } else {
	      per += 2*thisYl->xmax - x - nextYl->xmax + 1;
	      x = nextYl->xmax;
	    }
	    y++;
	    /* dir = SOUTH */
	    prevYl = thisYl;
	    thisYl = nextYl;
	    moved = TRUE; MOVETO(2);
	  }
	}
	if (NOT moved) { /* have to go north again!! */
	  nextYl = reg->ylines[y-1];
	  while (nextYl->xmax LT thisYl->xmin) nextYl = nextYl->next;
	  if (nextYl->xmin LE thisYl->xmin) {
	    per += 2*thisYl->xmax - thisYl->xmin - x + 1;
	    x = thisYl->xmin;
	  } else {
	    per += 2*(thisYl->xmax - thisYl->xmin) - x + nextYl->xmin + 1;
	    x = nextYl->xmin;
	  }
	  y--;
	  dir = NORTH;
	  prevYl = thisYl;
	  thisYl = nextYl;
	  moved = TRUE; MOVETO(3);
	}
      } /* else */
      break; /* SOUTH */
      
    case NORTH:
      /* first look south-west */

      nextYl = reg->ylines[y+1];
      while ((nextYl->next) AND (nextYl->next NE prevYl))
	nextYl = nextYl->next; /* May be made more effective; Check if prev->xmin LT this->xmin.. */
      if ((nextYl->next) AND (nextYl->xmax GE thisYl->xmin)) { /* go south */
	per += x - nextYl->xmax + 1;
	x = nextYl->xmax;
	y++;
	dir = SOUTH;
	prevYl = thisYl;
	thisYl = nextYl;
	moved = TRUE; MOVETO(4);
      } else { /* look further north */
	if (y GT ymin) {
	  nextYl = reg->ylines[y-1];
	  while (TRUE) {
	    if (NOT nextYl) break;
	    if (nextYl->xmax GE thisYl->xmin) break;
	    nextYl = nextYl->next;
	  }
	  if ((nextYl) ? (nextYl->xmin LE thisYl->xmax) : FALSE) {
	    /* go north */
	    if (nextYl->xmin LE thisYl->xmin) {
	      per += x - thisYl->xmin + 1;
	      x = thisYl->xmin;
	    } else {
	      per += x + nextYl->xmin - (2*thisYl->xmin) + 1;
	      x = nextYl->xmin;
	    }
	    y--;
	    /* dir = NORTH */
	    prevYl = thisYl;
	    thisYl = nextYl;
	    moved = TRUE; MOVETO(5);
	  }
	}
	if (NOT moved) { /* look back south again */
	  nextYl = prevYl;
	  while ((nextYl->next) ?
		 (nextYl->next->xmin LE thisYl->xmax) : FALSE)
	    nextYl = nextYl->next;
	  if (nextYl->xmin LE thisYl->xmax) { /* go south */
	    if (nextYl->xmax GE thisYl->xmax) {
	      per += thisYl->xmax - 2*thisYl->xmin + x + 1;
	      x = thisYl->xmax;
	    } else {
	      per += 2*(thisYl->xmax - thisYl->xmin) - nextYl->xmax + x + 1;
	      x = nextYl->xmax;
	    }
	    y++;
	    dir = SOUTH;
	    prevYl = thisYl;
	    thisYl = nextYl;
	    moved = TRUE; MOVETO(6);
	  } else { *perimeter = 0; return(2); } /* error, no place to go */
	}
      } /* else */
      break; /* NORTH */
    } /* switch */
    
    if ((prevYl EQ first) AND (thisYl EQ second)) break;
  } /* while */
  
  *perimeter = per;
  return(0);
  
} /* end of region4perimeter */

/*_____________________________________________________________________
*/


#ifndef FUNCPROTO
static int region8perimeter(reg, perimeter)
region *reg;
int* perimeter; 
#else /* FUNCPROTO */
static int region8perimeter(region *reg, int *perimeter)
#endif /* FUNCPROTO */
{
  int x, y, dir, ymin, ymax, moved, per=0;
  yline *prevYl, *thisYl, *nextYl, *first, *second;

  if (reg EQ nilr) {
    *perimeter=0;
    return(1);
  }

  ymin = reg->ymin;
  ymax = reg->ymax;
  first = prevYl = reg->ylines[ymin];
  if (ymin EQ ymax) { *perimeter = 2 * (YLXSIZE(prevYl) - 1); return(0); }
  y = ymin+1;
  thisYl = reg->ylines[y];
  while ((thisYl->next) ? (thisYl->next->xmin LE prevYl->xmax + 1) : FALSE)
    thisYl = thisYl->next;
  second = thisYl;
  if (prevYl->xmax LT thisYl->xmax)
    x = prevYl->xmax + 1;
  else x = thisYl->xmax;
  dir = SOUTH;

  while (TRUE) {   /* rotate WITH the sun */
    moved = FALSE;
    switch (dir) {
    case SOUTH:
      
      /* look for next at north-east */
      
      if ((nextYl = prevYl->next) ?
          (nextYl->xmin LE thisYl->xmax + 1) : FALSE) { /* go north */
	per += nextYl->xmin - x;
	x = nextYl->xmin;
	y--;
	prevYl = thisYl;
	thisYl = nextYl;
	dir = NORTH;
	moved = TRUE; MOVETO(7);
      } else { /* look further south */
	if (y LT ymax) {
	  nextYl = reg->ylines[y+1];
	  while((nextYl->next) ?
		(nextYl->next->xmin LE thisYl->xmax + 1) : FALSE)
            nextYl = nextYl->next;
	  if ((nextYl->xmax GE thisYl->xmin - 1)
              AND (thisYl->xmax GE nextYl->xmin - 1)) { /* go south */
	    if (thisYl->xmax LT nextYl->xmax) {
	      per += thisYl->xmax - x + 1;
	      x = thisYl->xmax + 1;
	    } else if (thisYl->xmax GT nextYl->xmax) {
	      per += 2*thisYl->xmax - x - nextYl->xmax;
	      x = nextYl->xmax;
	    } else { /* thisYl->xmax EQ nextYl->xmax */
	      per += thisYl->xmax - x + 1;
	      x = thisYl->xmax;
	    }
	    y++;
	    /* dir = SOUTH */
	    prevYl = thisYl;
	    thisYl = nextYl;
	    moved = TRUE; MOVETO(8);
	  }
	}
	if (NOT moved) { /* have to go north again!! */
	  nextYl = reg->ylines[y-1];
	  while (nextYl->xmax LT thisYl->xmin - 1) nextYl = nextYl->next;
	  if (nextYl->xmin LT thisYl->xmin) {
	    per += 2*thisYl->xmax - thisYl->xmin - x + 1;
	    x = thisYl->xmin - 1;
	  } else if (nextYl->xmin GT thisYl->xmin) {
	    per += 2*(thisYl->xmax - thisYl->xmin) - x + nextYl->xmin;
	    x = nextYl->xmin;
	  } else { /* (nextYl->xmin EQ thisYl->xmin) */
	    per += 2*thisYl->xmax - thisYl->xmin - x + 1;
	    x = thisYl->xmin;
	  }
	  y--;
	  dir = NORTH;
	  prevYl = thisYl;
	  thisYl = nextYl;
	  moved = TRUE; MOVETO(9);
	}
      } /* else */
      break; /* SOUTH */
      
    case NORTH:
      /* first look sout-west */
      nextYl = reg->ylines[y+1];
      while ((nextYl->next) AND (nextYl->next NE prevYl))
	nextYl = nextYl->next; /* Can be made more effective; check if prev->xmin LT this->xmin.. */
      if ((nextYl->next) AND (nextYl->xmax GE thisYl->xmin - 1)) {
	/* go south */
	per += x - nextYl->xmax;
	x = nextYl->xmax;
	y++;
	dir = SOUTH;
	prevYl = thisYl;
	thisYl = nextYl;
	moved = TRUE; MOVETO(10);
      } else { /* look further north */
	if (y GT ymin) {
	  nextYl = reg->ylines[y-1];
	  while (TRUE) {
	    if (NOT nextYl) break;
	    if (nextYl->xmax GE thisYl->xmin - 1) break;
	    nextYl = nextYl->next;
	  }
	  if ((nextYl) ? (nextYl->xmin LE thisYl->xmax + 1) : FALSE) {
	    /* go north */
	    if (nextYl->xmin LT thisYl->xmin) {
	      per += x - thisYl->xmin + 1;
	      x = thisYl->xmin - 1;
	    } else if (nextYl->xmin GT thisYl->xmin) {
	      per += x + nextYl->xmin - (2*thisYl->xmin);
	      x = nextYl->xmin;
	    } else { /* (nextYl->xmin EQ thisYl->xmin) */
	      per += x - thisYl->xmin + 1;
	      x = thisYl->xmin;
	    }
	    y--;
	    /* dir = NORTH */
	    prevYl = thisYl;
	    thisYl = nextYl;
	    moved = TRUE; MOVETO(11);
	  }
	}
	if (NOT moved) { /* look back south again */
	  nextYl = prevYl;
	  while ((nextYl->next) ?
		 (nextYl->next->xmin LE thisYl->xmax + 1) : FALSE)
	    nextYl = nextYl->next;
	  if (nextYl->xmin LE thisYl->xmax + 1) { /* go south */
	    if (nextYl->xmax GT thisYl->xmax) {
	      per += thisYl->xmax - 2*thisYl->xmin + x + 1;
	      x = thisYl->xmax + 1;
	    } else if (nextYl->xmax LT thisYl->xmax) {
	      per += 2*(thisYl->xmax - thisYl->xmin) + x - nextYl->xmax;
	      x = nextYl->xmax;
	    } else { /* (nextYl->xmax EQ thisYl->xmax) */
	      per += thisYl->xmax - 2*thisYl->xmin + x + 1;
	      x = thisYl->xmax;
	    }
	    y++;
	    dir = SOUTH;
	    prevYl = thisYl;
	    thisYl = nextYl;
	    moved = TRUE; MOVETO(12);
	  } else { printf("Error, no place to go!!\n"); }
	}
      }
      break;
    } /* switch */
    
    if ((prevYl EQ first) AND (thisYl EQ second)) break;
  } /* while */
  
  *perimeter = per;
  return(0);
  
} /* end of region8perimeter */



/*F:regionPerimeter*

________________________________________________________________

		regionPerimeter
________________________________________________________________

Name:		regionPerimeter - find the perimeter of one or more regions

Syntax:         | #include <xite/region.h>
		|
                | int regionPerimeter( region* reg, int nc,
                |    int loop, int* perimeter );
Description:    Calculates the perimeter of one or several regions.
		If "loop" is false, the perimeter of "reg" is measured.
		If true, the next pointer in reg is followed until nil,
		and the perimeters of all the regions are accumulated.
		"nc" decides the perimeter type used - 4 means 4-perimeter,
		8 means 8-perimeter, 0 means use the nc type of the region.
                
Return value:   | 0 => ok
		| 1 => "reg" points to nil. Area is set to 0.
		| 2 => bad nc value, should be 0, 4 or 8.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Tor Loennestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int regionPerimeter(reg, nc, loop, perimeter)
region *reg;
int nc;
int loop;
int* perimeter;
#else /* FUNCPROTO */
int regionPerimeter(region *reg, int nc, int loop, int *perimeter)
#endif /* FUNCPROTO */
{
  int perim;

  *perimeter = 0;
  if (reg EQ nilr) return(1);
  if (NOT ((nc EQ 0) OR (nc EQ 4) OR (nc EQ 8))) return(2);

  LOOP
    if (nc EQ 4) region4perimeter(reg, &perim);
    else if (nc EQ 8) region8perimeter(reg, &perim);
    else if (reg->nc EQ 4) region4perimeter(reg, &perim);
    else region8perimeter(reg, &perim);

    *perimeter += perim;
    EXITIF (NOT loop);
    EXITIF (NOT (reg=reg->next));
  ENDLOOP;

  return(0);
}

