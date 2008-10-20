
/*

________________________________________________________________

        mbkncn
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
#include <xite/kncn.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:mbkncn*

________________________________________________________________

		mbkncn
________________________________________________________________

Name:		mbkncn - multi band k nearest connected neighbour
                noise reduction

Syntax:         | #include <xite/kncn.h>
		|
                | int mbkncn( IMAGE i1, IMAGE i2, int k );

Description:    Performs multi band 'k' nearest connected neighbours
                noise filtering. All bands are filtered
                simultaneously. Taxi distance is used as distance
                measure.

                'i1' is the input image, 'i2' is the output image.

Restrictions:   The bands of 'i1' and 'i2' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 1 => Bad pixel type input image
		| 2 => Bad pixel type output image
		| 3 => Bad k value

See also:       mbkncn(1), kncn(3), knn(3), mbknn(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/


int mbkncn(IMAGE i1, IMAGE i2, int k)
{

#define NIL 0

  typedef struct point_struct
  {
    int x,y,diff;
    struct point_struct *next;
  } point;

  point *yes,*maybe,*lastmaybe,*no;
  point *ptr2, *ptr;
  int x,y;
  int *sum;
  int i,xsize,ysize,nk,newx,newy,xx,yy,diff,nbands,bn;

  /* test pixtyps and sizes of all bands!!!! */
  if (k LT 1) return(Error(3, "mbkncn: Bad k value.\n"));
  nbands = MIN(Inbands(i1), Inbands(i2));
  xsize = Ixsize(i1[1]);
  ysize = Iysize(i1[1]);
  FOR (bn=1; bn LE nbands; bn++)
    if (Ipixtyp(i1[bn]) NE Iu_byte_typ)
      return(Error(1, "mbkncn: Input pixel type must be unsigned byte.\n"));
    if (Ipixtyp(i2[bn]) NE Iu_byte_typ)
      return(Error(2, "mbkncn: Output pixel type must be unsigned byte.\n"));
    xsize = MIN(xsize, Ixsize(i1[bn]));
    ysize = MIN(ysize, Iysize(i1[bn]));
    xsize = MIN(xsize, Ixsize(i2[bn]));
    ysize = MIN(ysize, Iysize(i2[bn]));
  ENDFOR;

  sum = (int*)malloc(sizeof(int)*(nbands+1)); /* don't use 0 elem.*/

  /* create (3k+3) objects in no-list */
  yes = ptr = (point*)malloc(sizeof(point));
  for (nk = 1; nk LE 3*k+3; INC nk)
    ptr = ptr->next = (point*)malloc(sizeof(point));

  FOR (y=1; y LE ysize; INC y)
    FOR (x=1; x LE xsize; INC x)
      /* legg alle obj.-2 over i no-lista */
      no = yes->next->next;
      maybe = yes->next;
      lastmaybe = maybe;
      maybe->next = NIL;
      maybe->diff = 10000; /* listehale! */
   
      newx = yes->x = x;
      newy = yes->y = y;
      yes->diff = 0;
      for (bn=1; bn LE nbands; INC bn) sum[bn] = i1[bn][y][x];

      FOR (nk = 1; nk LE k; INC nk)
	/* update point list: neighbours of last selected */
        FOR (i=1; i LE 4; INC i)
          IF (i EQ 1)
            xx = newx-1; yy = newy;
            if (xx LT 1) INC i;
          ENDIF;
          IF (i EQ 2)
            xx = newx; yy = newy-1;
            if (yy LT 1) INC i;
          ENDIF;
          IF (i EQ 3)
            xx = newx; yy = newy+1;
            if (yy GT ysize) INC i;
          ENDIF;
          IF (i EQ 4)
            xx = newx+1; yy = newy;
            if (xx GT xsize) INC i;
          ENDIF;
          EXITIF (i GT 4);

          ptr = yes; 
          LOOP
            EXITIF ((xx EQ ptr->x) AND (yy EQ ptr->y));
            EXITIF (ptr->next EQ maybe);
            ptr = ptr->next;
          ENDLOOP;

          IF ((xx NE ptr->x) OR (yy NE ptr->y))
            diff = abs(i1[1][yy][xx] - i1[1][y][x]);
            FOR (bn=2; bn LE nbands; INC bn)
              diff += abs(i1[bn][yy][xx] - i1[bn][y][x]); /* taxiavst. */
            ENDFOR;
            LOOP
              EXITIF ((ptr->next)->diff GT diff);
              EXITIF ((xx EQ ptr->x) AND (yy EQ ptr->y));
              ptr = ptr->next;
            ENDLOOP;
            IF ((xx NE ptr->x) OR (yy NE ptr->y))
              /* insert new candidate. */
              ptr2 = no;
              no = no->next;
              ptr2->next = ptr->next;
              ptr->next = ptr2;
              ptr2->x = xx;
              ptr2->y = yy;
              ptr2->diff = diff;
              if (ptr2->next EQ maybe) maybe = ptr2;
            ENDIF;
          ENDIF;
        ENDFOR; /* all four neighbours */

        /* choose new point */
	newx = maybe->x;
	newy = maybe->y;
	maybe = maybe->next;
        for (bn=1; bn LE nbands; INC bn) sum[bn] += i1[bn][newy][newx];
      ENDFOR;  /* all k */

      /* calc. average using center + k neighbours!!! */
      FOR (bn=1; bn LE nbands; INC bn)
        i2[bn][y][x] = (int)(((float)sum[bn])/(k+1));
      ENDFOR; 
      lastmaybe->next = no;
    ENDFOR;
  ENDFOR;
  return(0);
}

#endif /* not MAIN */



/*P:mbkncn*

________________________________________________________________

		mbkncn
________________________________________________________________

Name:		mbkncn - multi band k nearest connected neighbour
                noise reduction

Syntax:		mbkncn <inimage> <outimage> <k>

Description:    Performs multi band 'k' nearest connected
                neighbour noise removal. All bands are filtered
                simultaneously. Taxi distance is used as distance
                measure.

                | inp image  - multi band input image
                | outp image - output image 
                | k - filtering parameter
                |   small k => small effect
                |   large k => large effect

Restrictions:   All bands of 'inimage' must have pixel type unsigned byte.

See also:	mbkncn(3), kncn(1), knn(1), mbknn(1)

Return value:   0 when ok, positive otherwise

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       mbkncn color.img smooth.img 8

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int k;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <oldfile> <newfile> <k>\n"));
  Iset_message(TRUE);
  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 4) Usage(2, "Illegal number of arguments.\n");

  k = atoi(argv[3]);
  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  if (mbkncn(i1, i2, k)) exit(3);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}

#endif /* MAIN */
