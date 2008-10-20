/*

________________________________________________________________

        regionYline
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/region.h>
#include <xite/readarg.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif


/*F:regionYline*

________________________________________________________________

		regionYline
________________________________________________________________

Name:		regionYline - split an image into homogenous regions

Syntax:         | #include <xite/region.h>
		|
                | BiffStatus regionYline( IBAND input,
                |    region*** regions, int nc, int sort,
                |    int split, int* size );

Description:    Makes a yline representation of the regions in the
		input image. At return, the 'regions' pointer will
		point at an array of region pointers, indexable from
		0 to 'size'. Element 0 will always be nil, the other
		elements will point at region objects. The input image
		defines regions by the pixel values. All pixels with
		the same value belongs to the same object. A 'region'
		is a connected (4- or 8-) set of pixels from the same
		object.

		Every region struct has a pointer "ylines" to an array
		of yline pointers. This array is indexable on the 
		interval region->ymin .. region->ymax. The yline 
		pointers point to a list of yline structs, the list
		ends when yline->next == nil. Every such list represents
		all yline segments on the actual line in the input
		image, belonging to this region.

		'nc' is neighbour connectedness - should be 4 or 8. If 
		there is an object in the image with object number
		(pixel value) 0, the object is supposed to be the
		background, and the opposite nc value is applied to
		that object.

		'sort' - sort the regions with respect to object number.
		 The 'regions' array will then point at the first region
		found in every object, the region->next pointer can be
		followed to see the other regions in the object. The
		'regions' array may have a lot of nil pointers, as it is
		indexed with object numbers.

		'split' - split all objects, except an eventual
		background object with object number 0, so that every
		region of an object will form an individual object.
		Useful for thresholded images.

Restrictions:   'input' must have pixel type unsigned byte.

Return value:   | 0 => ok
		| 1 => Bad pixel type input band
		| 2 => Bad neighbour type, should be 4 or 8
		| 3 => Can't alloc enough virtual memory, too many regions

Example:	| IBAND i;
		| region **regs;
		| int size;
		| if (regionYline(i, &regs, 4, FALSE, TRUE, &size))
		|    ERROR(..);

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
		regionUnion(3), regionOverlap(3), regionNeighbor(3),
		regionVolume(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#define REGARRINITSIZE 20

#define SAMEOBJ(pyl, cyl) (input[y-1][(pyl)->xmin] EQ input[y][(cyl)->xmin])

#define PREV_TO_LEFT						\
      ((regArr[prevReg[pyln]]->nc EQ 4) ?			\
         (prevYl[pyln]->xmax LT curYl[cyln]->xmin)		\
         : (prevYl[pyln]->xmax LT curYl[cyln]->xmin-1))

#define PREV_TO_RIGHT(nc)					\
   ((nc EQ 4) ?							\
   (prevYl[pyln]->xmax GE curYl[cyln]->xmax)			\
   : (prevYl[pyln]->xmax GT curYl[cyln]->xmax))

#define INSERTYL(yl, reg)					\
      { while ((reg)->next) reg = (reg)->next;			\
        if ((yl)->xmin LE ((yline*)((reg)->ylines))->xmax+1)	\
          { (reg)->ymax++;  (reg)->amax++; }			\
        (yl)->next = (yline*)((reg)->ylines);			\
        (reg)->ylines = (yline**)(yl);				\
      }


#define MOVEFROM(oldyl, oldy, yl)				\
      { lastyl->next = oldyl;					\
        lastyl = lastyl->next;					\
        oldyl = (oldyl)->next;					\
        lastyl->next = nily;					\
        if (oldyl) {						\
          if (lastyl->xmin LE (oldyl)->xmax +1) --oldy;		\
        } else {						\
          lastyl->next = yl;					\
          break;						\
        }							\
      }
       

/*_______________________________________________________________________
*/

static int xsize, lastReg, regArrSize;
static region** regArr;
static int *curReg,   /* current regions  */
           *prevReg;  /* previous regions */

/*_______________________________________________________________________
*/

static int readyline(IBAND input, yline **yl, int *nyls, int y)
{
  int x, pv, yln=0, xstart=1;

  pv = input[y][1];
  FOR (x=2; x LE xsize; ++x)
    IF (input[y][x] NE pv)
      if (NOT (yl[++yln] = (yline*)malloc(sizeof(yline)))) 
        return(Error(3, "readyline: Malloc failed\n"));
      yl[yln]->xmin = xstart;
      yl[yln]->xmax = x-1;
      yl[yln]->next = nily;
      xstart = x;
      pv = input[y][x];
    ENDIF;
  ENDFOR;
  if (NOT (yl[++yln] = (yline*)malloc(sizeof(yline))))
    return(Error(3, "readyline: Malloc failed\n"));
  yl[yln]->xmin = xstart;
  yl[yln]->xmax = xsize;
  yl[yln]->next = nily;
  *nyls = yln;
  return(0);
}

/*_______________________________________________________________________
*/

static int mkregion(yline *yl, int y, int on, int nc)
{
  region* reg;
  if (NOT (reg = (region*)malloc(sizeof(region))))
    { Error(3, "mkregion: Malloc failed\n"); return(0); }
  reg->objectnr = on;
  reg->regionnr = 0;
  reg->ymin = reg->amin = y;
  reg->ymax = reg->amax = y;
  reg->nc = (on EQ 0) ? ((nc EQ 4) ? 8 : 4) : nc;
  reg->next = nilr;
  yl->next = nily;
  /* initially making a list of yl's, later rearrange to array */
  reg->ylines = (yline**)yl;
  IF (regArrSize EQ ++lastReg)
    regArrSize *= 2;
    if (NOT (regArr = (region**)realloc(regArr, regArrSize*sizeof(region*))))
      { Error(3, "mkregion: Malloc failed\n"); return(0); }
  ENDIF;
  regArr[lastReg] = reg;
  return(lastReg);
}

/*_______________________________________________________________________
*/

static int mayBeSamsort(int cyln, int pyln, int y)
{
  region *cReg, *pReg, *newReg;
  yline *oldyl1, *oldyl2, *newyl, *lastyl;
  int oldy1, oldy2;

  cReg = regArr[curReg[cyln]];
  while (cReg->next) cReg = cReg->next;
  pReg = regArr[prevReg[pyln]];
  while (pReg->next) pReg = pReg->next;
  if (cReg EQ pReg) return(0);
  oldyl1 = (yline*)(cReg->ylines);
  oldyl2 = (yline*)(pReg->ylines);
  cReg->ylines = (yline**)nily;
  pReg->ylines = (yline**)nily;
  oldy1 = y;
  oldy2 = pReg->ymax;
  newyl = (lastyl = oldyl1);
  oldyl1 = oldyl1->next;
  newyl->next = nily;

  if (oldyl1) {
    if (lastyl->xmin LE oldyl1->xmax + 1) --oldy1;
    while (TRUE) { /* MOVEFROM include break from the LOOP */
      if (oldy1 GT oldy2) {
	MOVEFROM(oldyl1, oldy1, oldyl2);
      } else if (oldy2 GT oldy1) {
	MOVEFROM(oldyl2, oldy2, oldyl1);
      } else if (oldyl1->xmin GT oldyl2->xmin) {
	MOVEFROM(oldyl1, oldy1, oldyl2);
      } else {
	MOVEFROM(oldyl2, oldy2, oldyl1);
      }
    }
  } else {
    lastyl->next = oldyl2;
  }

  if (NOT (newReg = (region*)malloc(sizeof(region))))
    return(Error(3, "mayBeSamsort: Malloc failed\n"));
  newReg->objectnr = cReg->objectnr;
  newReg->regionnr = 0;
  newReg->ymin = newReg->amin = MIN(cReg->ymin, pReg->ymin);
  newReg->ymax = newReg->amax = y;
  newReg->nc = cReg->nc;
  newReg->next = nilr;
  newReg->ylines = (yline**)newyl;
  cReg->next = newReg;
  pReg->next = newReg;
  IF (regArrSize EQ ++lastReg)
    regArrSize *= 2;
    if (NOT (regArr = (region**)realloc(regArr, regArrSize*sizeof(region*))))
      return(Error(3, "mayBeSamsort: Malloc failed\n"));
  ENDIF;
  regArr[lastReg] = newReg;
  return(0);
}

/*_______________________________________________________________________
*/

static void swap(int *a, int *b)
{
  int c;
  c = *a; *a = *b; *b = c;
}

/*_______________________________________________________________________
*/

static void regionSplit(region **ra, int size)
{
  int n1, n2=0;
  region* reg;

  FOR (n1=1; n1 LE size; n1++)
    reg = regArr[n1];
    if (reg->objectnr NE 0) reg->objectnr = ++n2;
  ENDFOR;
}

/*_______________________________________________________________________
*/

BiffStatus regionYline(IBAND input, region ***regions, int nc, int sort, int split, int *size)
{
  yline **curYl;   /* current ylines   */
  yline **prevYl;  /* previous ylines  */
  yline *yl1, *yl2, **yls;
  region *reg;
  int ncyl;        /* number of current yls */
  int ncir;        /* number of current identified regions */
  int npyl;        /* number of prev yls. */
  int cyln;        /* current y line number */
  int pyln;        /* prev y line number */
  int y, ysize, regnr1, regnr2;

  if (Ipixtyp(input) NE Iu_byte_typ)
    return(Error(1, "regionYline: Input pixel type must be unsigned byte.\n"));
  if ((nc NE 4) AND (nc NE 8))
    return(Error(2, "regionYline: Bad neighbour type, should be 4 or 8\n"));

  lastReg = 0;
  regArrSize = REGARRINITSIZE;
  xsize = Ixsize(input);
  ysize = Iysize(input);
  if (NOT (curReg = (int*)malloc((xsize+1)*sizeof(int))))
    return(Error(3, "Malloc failed\n"));
  if (NOT (prevReg = (int*)malloc((xsize+1)*sizeof(int))))
    return(Error(3, "Malloc failed\n"));
  if (NOT (curYl = (yline**)malloc((xsize+1)*sizeof(yline*))))
    return(Error(3, "Malloc failed\n"));
  if (NOT (prevYl = (yline**)malloc((xsize+1)*sizeof(yline*))))
    return(Error(3, "Malloc failed\n"));
  if (NOT (regArr = (region**)malloc(REGARRINITSIZE * sizeof(region*))))
    return(Error(3, "Malloc failed\n"));

  /* Special treatment of first line */
  readyline(input, prevYl, &npyl, 1);
  for (pyln=1; pyln LE npyl; pyln++)
    prevReg[pyln] = mkregion(prevYl[pyln], 1, input[1][(prevYl[pyln])->xmin], nc);

  FOR (y=2; y LE ysize; y++)
    readyline(input, curYl, &ncyl, y); /* read one image line, generate ylines */
    pyln = 1; ncir=0;
    FOR (cyln=1; cyln LE ncyl; cyln++) /* for every yline on line y.. */
      while (PREV_TO_LEFT) pyln++;

      LOOP /* while no overlapping yline from same object */
        reg = regArr[prevReg[pyln]];
        EXITIF((reg->nc EQ 4) AND (prevYl[pyln]->xmin GT curYl[cyln]->xmax)); 
        if (SAMEOBJ(prevYl[pyln], curYl[cyln])) {
          INSERTYL(curYl[cyln], reg);
          curReg[++ncir] = prevReg[pyln];
          break;
	}
        if (PREV_TO_RIGHT(8)) break;
        if (++pyln GT npyl) break;
      ENDLOOP;

      IF (ncir EQ cyln) /* have found an overlapping region, look for more */
        WHILE (NOT PREV_TO_RIGHT(regArr[curReg[cyln]]->nc))
          EXITIF(++pyln GT npyl);
          IF (SAMEOBJ(prevYl[pyln], curYl[cyln]))
            mayBeSamsort(cyln, pyln, y);
          ENDIF;
        ENDWHILE;

      ELSE /* no overlapping yl from same object, create new region */
        curReg[++ncir] = mkregion(curYl[cyln], y, input[y][(curYl[cyln])->xmin], nc);
      ENDIF;
      if ((pyln GT npyl) OR ((prevYl[pyln]->xmin) GT curYl[cyln]->xmax))
          pyln--;
    ENDFOR; /* cyln=1..ncyl */
    swap((int *)&curYl, (int *)&prevYl);
    swap((int *)&curReg, (int *)&prevReg);
    npyl = ncyl;
  ENDFOR;

  /* Delete and "sort" regions */
  regnr2 = 0;
  FOR (regnr1=1; regnr1 LE lastReg; regnr1++)
    reg = regArr[regnr1];
    WHILE ((reg->next) AND (reg->regionnr EQ 0))
      reg->regionnr = -1;
      reg = reg->next;
    ENDWHILE;
    IF (reg->regionnr EQ 0)
      regArr[++regnr2] = reg;
      reg->regionnr = regnr2;
    ENDIF;
  ENDFOR;
  if (NOT (regArr = (region**)realloc(regArr, (regnr2+1)*sizeof(region*))))
    return(Error(3, "regionYline: Malloc failed\n"));

  /* Reorganize ylines from list to array */
  FOR (regnr1=1; regnr1 LE regnr2; regnr1++)
    reg = regArr[regnr1];


    if (NOT (yls = (yline**)calloc((reg->ymax - reg->ymin+1), sizeof(yline*))))
      return(Error(3, "Malloc failed\n"));

    /* make the yline array indexable on reg->ymin .. reg->ymax */
    yls = (yline**)(yls - (reg->ymin));
    y = reg->ymax;
    yl1 = (yline*)reg->ylines;
    
    LOOP
      yl2 = yls[y];
      yls[y] = yl1;
      yl1 = yl1->next;
      yls[y]->next = yl2;
      EXITIF (NOT yl1);
      if (yls[y]->xmin LE (yl1->xmax+1)) y--;
      EXITIF (y LT reg->ymin);
    ENDLOOP;
    reg->ylines = yls;
  ENDFOR;

  if (split) regionSplit(regArr, regnr2);
  IF (sort)
    if (NOT regionSort(regArr, regions, regnr2, size)) return(3);
  ELSE
    *size = regnr2;
    *regions = regArr;
  ENDIF;

  /* Free temporary arrays */
  free(curReg);
  free(prevReg);
  free(curYl);
  free(prevYl);

  return(0);
}
