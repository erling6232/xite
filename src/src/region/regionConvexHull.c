

/*C*

________________________________________________________________

        regionConvexHull
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
#include <xite/blab.h>
#include <xite/draw.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/region.h>
#include XITE_MALLOC_H



#ifndef MAIN

/*F:regionConvexHull*

________________________________________________________________

		regionConvexHull
________________________________________________________________

Name:		regionConvexHull - find the convex hull of a region

Syntax:         | #include <xite/region.h>
		|
                | region *regionConvexHull( region* reg );

Description:	'regionConvexHull' finds the convex hull of the input
                region 'reg'. The convex hull is the smallest convex
		region containing the whole input region. The convex
		hull is returned as function value.

Restrictions:   reg should not be nil.

See also:       regionAnalyse(1), regionConvexHull(1), regionArea(3),
		regionCreate(3), regionDraw(3),
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

Return value:   The convex hull if ok, nil otherwise.

Diagnostics:	| error 2 : "input region is nil"
                | error 3 : "malloc failed", ran out of virtual memory

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


region* regionConvexHull(region *reg)
{
  int *X, *Y, i, imax, y, ymin, ymax, ysize, xstart, xstop;
  double *A, a, inc, x;
  region* newreg;
  yline* yl;

  if (NOT reg) {
    Error(2, "regionConvexHull: input region is nil\n");
    return(nilr);
  }
  ymin = reg->ymin;
  ymax = reg->ymax;
  newreg = regionCreate(reg->objectnr, reg->regionnr, reg->nc, ymin, ymax);
  if (NOT newreg) {
    Error(2, "regionConvexHull: malloc failed\n");
    return(nilr);
  }
  ysize = ymax-ymin+2;
  X = (int*)malloc(ysize*sizeof(int));
  Y = (int*)malloc(ysize*sizeof(int));
  A = (double*)malloc(ysize*sizeof(double));
  if (NOT X OR NOT Y OR NOT A) {
    Error(3, "regionConvexHull: malloc failed\n");
    return(nilr);
  }

  /* search from top of object down along right border */
  for (yl=reg->ylines[ymin]; yl->next; yl = yl->next) ;
  Y[0] = ymin;
  X[0] = yl->xmax;
  A[0] =  -1000000000.0;
  for (i=0, y=ymin+1; y LE ymax; y++) {
    for (yl=reg->ylines[y]; yl->next; yl = yl->next) ;
    xstop = yl->xmax;
    a = ((double)X[i] - xstop)/(y - Y[i]);
/*printf("Line %d, angle %f\n", y, a); */
    WHILE (a LE A[i])
      i--;
      a = ((double)X[i] - xstop)/(y - Y[i]);
/*printf("    Recomputing Line %d, angle %f\n", y, a); */
    ENDWHILE;
    X[++i] = xstop;
    Y[i] = y;
    A[i] = a;
  }

  /* insert list "tail" */
  imax = i;
  Y[imax+1] = ymin;

  /* create ylines and insert xmax */
  FOR (i=0; i LE imax; i++)
    y = Y[i];
    newreg->ylines[y] = (yline*)malloc(sizeof(yline));
    if (NOT newreg->ylines[y])
      { Error(3, "regionConvexHull: malloc failed\n"); return(nilr); }
    newreg->ylines[y]->xmax = X[i];
    newreg->ylines[y]->next = nily;
    IF (y+1 LT Y[i+1])
      inc = ((double)X[i+1]-X[i])/(Y[i+1]-Y[i]);
      x = (double)X[i] + 0.5;
      WHILE (++y LT Y[i+1]) 
        x += inc;
        newreg->ylines[y] = (yline*)malloc(sizeof(yline));
        if (NOT newreg->ylines[y]) 
          { Error(3, "regionConvexHull: malloc failed\n"); return(nilr); }
        newreg->ylines[y]->xmax = (int)x;
        newreg->ylines[y]->next = nily;
      ENDWHILE;
    ENDIF;
  ENDFOR;

  /* search from top of object down along left border */
  yl=reg->ylines[ymin];
  Y[0] = ymin;
  X[0] = yl->xmin;
  A[0] =  -1000000000.0;
  for (i=0, y=ymin+1; y LE ymax; y++) {
    xstart = reg->ylines[y]->xmin;
    a = ((double)xstart - X[i])/(y - Y[i]);
    WHILE (a LE A[i])
      i--;
      a = ((double)xstart - X[i])/(y - Y[i]);
    ENDWHILE;
    X[++i] = xstart;
    Y[i] = y;
    A[i] = a;
  }

  /* insert list "tail" */
  imax = i;
  Y[imax+1] = ymin;

  /* insert xmin into yline objects */
  FOR (i=0; i LE imax; i++)
    y = Y[i];
    newreg->ylines[y]->xmin = X[i];
    IF (y+1 LT Y[i+1])
      inc = ((double)X[i+1]-X[i])/(Y[i+1]-Y[i]);
      x = (double)X[i] + 0.5;
      WHILE (++y LT Y[i+1]) 
        x += inc;
        newreg->ylines[y]->xmin = (int)x;
      ENDWHILE;
    ENDIF;
  ENDFOR;

  free(X);
  free(Y);
  free(A);
  return(newreg);
}

#endif /* not MAIN */

/*P:regionConvexHull*

________________________________________________________________

		regionConvexHull
________________________________________________________________

Name:		regionConvexHull - find the convex hull of regions

Syntax:		| regionConvexHull [-nc <nc>] [-sort] [-split] [-bg <bg>]
                    [-opv <opv>] [-orn] [-oon] <inimage> <outimage>

Description:    Finds the convex hull of all regions in the input
                image with pixel value LARGER than 0, and draws
		these convex regions into the output image. These
		regions may naturally overlap, and the result may
		thus (apparently) contain non-convex regions.

Options:        Use only one of '-opv', '-orn' and '-oon'.

                &-nc nc
                Neighbour connectedness: 4 or 8 (default 4). 'nc' applies to
                objects (pixel value > 0). The opposite applies to the
                background (=0).

                &-sort
		Sort regions with respect to object number, store in an
		object-array.

		&-split
		Split objects (except the background, the object with object
		number 0) so that every region forms an independent object.

		&-bg bg
		Initialize output with background 'bg' (def: 0).

		&-opv opv
		Output Pixel Value. Draw regions with 'opv' (def: 255).

		&-orn
		Output Region Number. Draw regions with regnr.

		&-oon
		Output Object Number. Draw regions with objnr.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                'outimage' will have bands with pixel type unsigned byte.

Return value:   | 0 => ok 
                | 1 => Usage (no arguments)
		| 2 => Bad number of arguments

See also:       regionAnalyse(1), regionConvexHull(3), regionArea(3),
		regionCreate(3), regionDraw(3),
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

Files:		region.h

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	| regionConvexHull -nc 8 -bg 255 -opv 0 \\
                |     mask.img maskNegConvex.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  region **regs;
  region *reg, *newreg;
  int r, regno, nc, sort, split, opv, orn, oon, bg, bn;
  IMAGE img;
  IBAND band;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-nc <nc>] [-sort] [-split] [-bg <bg>] \n\
                    [-opv <opv>] [-orn] [-oon] <inimage> <outimage>\n"));
  Iset_message(TRUE);
  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  nc    = read_iswitch(&argc, argv, "-nc", 4);
  sort  = read_bswitch(&argc, argv, "-sort");
  split = read_bswitch(&argc, argv, "-split");
  bg    = read_iswitch(&argc, argv, "-bg", 0);
  opv   = read_iswitch(&argc, argv, "-opv", 255);
  orn   = read_bswitch(&argc, argv, "-orn");
  oon   = read_bswitch(&argc, argv, "-oon");
  if ((opv<0) OR (opv>255)) opv = 255;
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);

  FOR (bn=1; bn LE Inbands(img); bn++)
    band = img[bn];
    regionYline(band, &regs, nc, 0, 0, &regno);
    drawBand(band, (double)bg, 0.0);

    FOR (r = 1; r<=regno; r++)
      reg = regs[r];
      WHILE (reg)
        IF (reg->objectnr != 0)
	  newreg = (region *) regionConvexHull(reg);
	  if (orn) regionDraw(band, newreg, 0, reg->regionnr);
          else if (oon) regionDraw(band, newreg, 0, reg->objectnr);
          else regionDraw(band, newreg, 0, opv);
        ENDIF;
        reg = reg->next;
      ENDWHILE;
    ENDFOR;
  ENDFOR;
  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);
  return(0);
}

#endif /* MAIN */
