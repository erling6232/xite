
/*

________________________________________________________________

        minarea
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
#include <xite/minmax.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*F:minarea*

________________________________________________________________

		minarea
________________________________________________________________

Name:		minarea - min operator over a local window

Syntax:         | #include <xite/minmax.h>
		|
                | BiffStatus minarea( IBAND in_band,
                |    IBAND out_band, int dx, int dy );

Description:	'minarea' computes minimum over a local region
		around (x,y) of size 'dx' * 'dy'. This minimum value
		replaces the center pixel in 'out_band'.

Restrictions:   'in_band' and 'out_band' must have pixel type unsigned byte.

Return value:	| 0 => ok
                | 1 => bad pixel type in_band
		| 2 => bad pixel type out_band
		| 3 => bad dx value
		| 4 => bad dy value

Author:		Otto Milvang and Tor Lønnestad, BLAB, Ifi, UiO

Examples:	minarea(b1, b2, 3, 5);

Id:             $Id$
________________________________________________________________

*/


BiffStatus minarea(IBAND in_band, IBAND out_band, int dx, int dy)
{
   int xsize, ysize;
   int xdist, ydist, x, y;
   UNS_BYTE min;
   int first, last, dest;
 
   if (Ipixtyp(in_band) NE Iu_byte_typ)
     return(Error(1, "maxarea: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(out_band) NE Iu_byte_typ)
     return(Error(2, "maxarea: Output pixel type must be unsigned byte.\n"));
   if (dx < 1) return(Error(3, "maxarea: Bad dx value.\n"));
   if (dy < 1) return(Error(4, "maxarea: Bad dy value.\n"));

   xdist = dx/2; ydist = dy/2;
   dx = 2*xdist+1; dy = 2*ydist+1;
   xsize = Ixsize(in_band);
   ysize = Iysize(in_band);

   FOR (y=1; y LE ysize; y++)
     min = 255;
     last = 1;
     for (first=1; first LE dx; first++)
       if (in_band[y][first] LT min) min = in_band[y][first];

     /* put everything one to the right of center of area */
     for (dest = 2; dest LE xdist+2; INC dest)
       out_band[y][dest] = min;

     FOR (first = dx+1; first LE xsize; )
       IF (in_band[y][first] LE min) min = in_band[y][first]; 
       ELSEIF(in_band[y][last] EQ min)
         min = in_band[y][last+1];
         for (x = last+2; x LE first; INC x)
           if (in_band[y][x] LT min) min = in_band[y][x];
       ENDIF;
       out_band[y][dest] = min;
       INC first; INC last; INC dest;
     ENDFOR;

     /* copy out to the margin */
     for (x=dest; x LE xsize; x++) out_band[y][x] = min;
   ENDFOR;  /* all y */

   FOR (x=2; x LE xsize; x++) /* start in x=2, no data in x=1!! */
     min = 255;
     last = 1;
     for (first=1; first LE dy; INC first)
       if (out_band[first][x] LT min) min = out_band[first][x];

     /* put everything one to the left of center of area */
     for (dest = 1; dest LE ydist+1; INC dest)
       out_band[dest][x-1] = min;

     FOR (first = dy+1; first LE ysize; )
       IF (out_band[first][x] LE min) min = out_band[first][x]; 
       ELSEIF(out_band[last][x] EQ min)
         min = out_band[last+1][x];
         for (y = last+2; y LE first; y++)
           if (out_band[y][x] LT min) min = out_band[y][x];
       ENDIF;
       out_band[dest][x-1] = min;
       INC first; INC last; INC dest;
     ENDFOR;

     /* copy out to the margin */
     for (y=dest; y LE ysize; INC y) out_band[y][x-1] = min;
   ENDFOR;  /* all x */

   /* copy to last column */
   for (y=1; y LE ysize; y++)
     out_band[y][xsize] = out_band[y][xsize-1];
   return(0);
}  /* minarea */
