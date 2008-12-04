
/*
________________________________________________________________

        kncn
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H

#ifndef MAIN

#ifndef NIL
# define NIL 0
#endif

#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

#define INSTALL(xx, yy)                         			\
      diff = centerpixval - b1[yy][xx];					\
      if (diff LT 0) diff = -diff;					\
      ptr = points[diff];						\
      LOOP								\
        if (ptr EQ pool[diff]) break;					\
        if ((ptr->x EQ xx) AND (ptr->y EQ yy))				\
          { ptr = (point*)0; break; }					\
        INC ptr;							\
      ENDLOOP;								\
      IF (ptr) THEN							\
        IF (ptr NE last[diff]) THEN		    			\
          ptr->x = xx;							\
          ptr->y = yy;							\
          INC pool[diff];						\
          if (ptr EQ points[diff]) used[nused++] = diff;	       	\
          if (diff LT best) best = diff;			       	\
        ENDIF;					       			\
      ENDIF;



/*L:kncn8*

________________________________________________________________

		kncn8
________________________________________________________________

*/

static int kncn8(IBAND b1, IBAND b2, int k)
{
   typedef struct { int x, y; } point;

   point* points[256];
   point* candidates[256]; 
   point* pool[256];
   point* last[256];
   int    used[256];
   int nused, x, y, K, i, xsize, ysize, centerpixval, sumpixval;
   int best, nk, newx, newy, diff;
   point* ptr;

   xsize = MIN(Ixsize(b1), Ixsize(b2));
   ysize = MIN(Iysize(b1), Iysize(b2));

   /* create point lists */
   FOR (i=0; i LE 255; INC i)
     points[i] = candidates[i] = pool[i] = (point*)malloc((k+2)*sizeof(point));
     last[i] = &points[i][k+2];
   ENDFOR;

   FOR (y=1; y LE ysize; INC y)
     FOR (x=1; x LE xsize; INC x)
       if (x GT 1)
         if (b1[y][x] EQ b1[y][x-1]) { b2[y][x] = b2[y][x-1]; continue; }
       if (y GT 1)
         if (b1[y][x] EQ b1[y-1][x]) { b2[y][x] = b2[y-1][x]; continue; }

       /* decide how large K can be to avoid walking outside the band */
       K = MIN(MIN(MIN(MIN(x-1,xsize-x),y-1),ysize-y),k);

       /* select current pixel */
       sumpixval = centerpixval = b1[y][x];
       ptr = candidates[0]++;
       pool[0]++;
       newx = ptr->x = x;
       newy = ptr->y = y;
       used[0] = 0;
       nused = 1;
       best = 1000;

       FOR (nk = 1; nk LE K; INC nk)
         /* update point list: neighbours of last selected */
         INSTALL(newx-1, newy);
         INSTALL(newx+1, newy);
         INSTALL(newx, newy-1);
         INSTALL(newx, newy+1);

         /* select best candidate */
         while (candidates[best] EQ pool[best]) INC best;
         newx = candidates[best]->x;
         newy = candidates[best]->y;
         sumpixval += b1[newy][newx];
         INC candidates[best];
       ENDFOR;  /* all K */

       /* calc. average */
       b2[y][x] = sumpixval/(K+1); /* USING center + K neighbours!!! */
       /* restore pointer arrays */
       LOOP
         diff = used[--nused];
         candidates[diff] = pool[diff] = points[diff];
         EXITIF(NOT nused)
       ENDLOOP;

     ENDFOR;  /* all x */
   ENDFOR;  /* all y */

   /* free temporary data area */
   for (i=0; i LE 255; INC i) free(points[i]);
   return(0);
}  /* kncn8 */




/*L:kncn16*

________________________________________________________________

		kncn16
________________________________________________________________

   kncn16 should be static, but keep it global for a while for
   historical reasons. January 1992.
*/

static int kncn16(IUS_BAND b1, IUS_BAND b2, int k)
{
  typedef struct point_struct
  {
    int x,y,diff;
    struct point_struct *next;
  } point;

  point *selected, *candidate, *lastcandidate, *free;
  point *ptr1, *ptr2;
  int   x, y, i, xsize, ysize, nk, newx, newy, sum;
  int   xx, yy, diff, l;

  xsize = MIN(Ixsize((IBAND) b1), Ixsize((IBAND) b2));
  ysize = MIN(Iysize((IBAND) b1), Iysize((IBAND) b2));

  /* create (3k+3) objects in free-list */
  selected = ptr1 = (point*)malloc(sizeof(point));
  FOR (nk = 1; nk LE 3*k+3; INC nk)
    ptr1 = ptr1->next = (point*)malloc(sizeof(point));
  ENDFOR;

  FOR (y=1; y LE ysize; INC y)
    FOR (x=1; x LE xsize; INC x)

      /* initiate the lists */
      candidate = selected->next;
      lastcandidate = candidate;
      free = candidate->next;
      candidate->next = NIL;
      candidate->diff = 1000; /* list tail */
   
      /* select current pixel */
      newx = selected->x = x;
      newy = selected->y = y;
      selected->diff = 0;
      sum = b1[y][x];
  
      FOR (nk = 1; nk LE k; INC nk)
	/* update point list: neighbours of last selected */
        FOR (i=1; i LE 4; INC i)
          IF (i EQ 1)
            THEN xx = newx-1; yy = newy;
            IF (xx LT 1) THEN INC i; ENDIF;
          ENDIF;
          IF (i EQ 2)
            THEN xx = newx; yy = newy-1;
            IF (yy LT 1) THEN INC i; ENDIF;
          ENDIF;
          IF (i EQ 3)
            THEN xx = newx; yy = newy+1;
            IF (yy GT ysize) THEN INC i; ENDIF;
          ENDIF;
          IF (i EQ 4)
            THEN xx = newx+1; yy = newy;
            IF (xx GT xsize) THEN INC i; ENDIF;
          ENDIF;
          EXITIF (i GT 4);

          /* is the pixel already selected? */
          ptr1 = selected;
          LOOP
            EXITIF ((xx EQ ptr1->x) AND (yy EQ ptr1->y));
            EXITIF (ptr1->next EQ candidate);
            ptr1 = ptr1->next;
          ENDLOOP;

          IF ((xx NE ptr1->x) OR (yy NE ptr1->y)) THEN
            /* is the pixel already candidate? */
            diff = abs(b1[yy][xx] - b1[y][x]);
            FOR (l=nk; l LE k; INC l)
              /* stop search at k'th object */
              EXITIF ((ptr1->next)->diff GT diff);
              EXITIF ((xx EQ ptr1->x) AND (yy EQ ptr1->y));
              ptr1 = ptr1->next;
            ENDFOR;

            IF ((l LE k) AND ((xx NE ptr1->x) OR (yy NE ptr1->y)))
            THEN
              /* insert new candidate. */
              ptr2 = free;
              free = free->next;
              ptr2->next = ptr1->next;
              ptr1->next = ptr2;
              ptr2->x = xx;
              ptr2->y = yy;
              ptr2->diff = diff;
              IF (ptr2->next EQ candidate) THEN candidate = ptr2 ENDIF;
            ENDIF;
          ENDIF;
        ENDFOR; /* all four neighbours */

        /* choose new point */
	newx = candidate->x;
	newy = candidate->y;
	candidate = candidate->next;
        sum += b1[newy][newx];
      ENDFOR;  /* all k */

      /* calc. average */
      b2[y][x] = sum/(k+1); /* USING center + k neighbours!!! */
      lastcandidate->next = free;
    ENDFOR;
  ENDFOR;
  return(0);
}   /* kncn16 */



/*F:kncn*

________________________________________________________________

		kncn
________________________________________________________________

Name:		kncn - k nearest connected neighbours noise reduction

Syntax:         | #include <xite/kncn.h>
		|
                | int kncn( IBAND b1, IBAND b2, int k );

Description:    'kncn' performs the kncn-filter for noise removal,
		averaging over the center pixel and 'k' connected
		neighbours in the input band 'b1'. The result appears
		in 'b2'. 

                'k', the filtering parameter, gives the number of neighbours.

Reference:	| T. Lønnestad: "Connected Filters for Noise Removal"
                | Proc. of 9. ICPR, Rome, 1988, 848-850.

Restrictions:   'b1' and 'b2' must both have either pixel type unsigned byte
                or unsigned short.

Return value:   | 0 => ok
                | 1 => Bad pixel type
                | 2 => Bands have different pixel types
		| 3 => Bad k value (less than 1)

See also:       kncn(1), mbkncn(3), knn(3), mbknn(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

int kncn(IBAND b1, IBAND b2, int k)
{
   int pt;

   pt = Ipixtyp(b1);
   if (pt NE Ipixtyp(b2))
     return(Error(2, "kncn: Bands have different pixel types\n"));
   if (k LT 1) return(Error(3, "kncn: Bad k value (less than 1)"));

   if (pt == Iu_byte_typ) return(kncn8(b1, b2, k));
   else if (pt == Iu_short_typ) return(kncn16((IUS_BAND)b1, (IUS_BAND)b2, k));
   return(Error(1,
         "kncn: Input pixel type must be unsigned byte or unsigned short.\n"));
}

#endif /* not MAIN */



/*P:kncn*

________________________________________________________________

		kncn
________________________________________________________________

Name:		kncn - k nearest connected neighbour noise reduction

Syntax:		kncn <inimage> <outimage> <k>  

Description:    Perform k-Nearest Connected Neighbour filtering
                for noise removal, averaging over the center pixel
		and 'k' connected neighbours. All bands in the image
                are filtered, one band at a time.

                Small 'k' => small effect, large 'k' => large effect.

Restrictions:   Only input bands with pixel type unsigned byte and unsigned
                short are processed.

Reference:	| T. Lønnestad: "Connected Filters for Noise Removal"
                | Proc. of 9. ICPR, Rome, 1988, 848-850.

See also:	kncn(3), mbkncn(1), knn(1), mbknn(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       kncn mona.img monasmooth.img 8

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn, k;
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

  for (bn=1; bn LE Inbands(i1); bn++) kncn(i1[bn], i2[bn], k);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}

#endif /* MAIN */
