
/*

________________________________________________________________

        maxarea
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

#ifndef MAIN

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/*F:maxarea*

________________________________________________________________

		maxarea
________________________________________________________________

Name:		maxarea - max operator over a local window

Syntax:         | #include <xite/minmax.h>
		|
                | BiffStatus maxarea( IBAND in_band,
                |    IBAND out_band, int dx, int dy );

Description:	Compute maximum over a local region around (x,y)
                of size 'dx' * 'dy' in 'in_band'. This maximum replaces
		the center pixel in 'out_band'.

Restrictions:   'in_band' and 'out_band' must have pixel type unsigned byte.

Return value:	| 0 => ok
                | 1 => Bad input pixel type
		| 2 => Bad output pixel type
		| 3 => Bad dx value
		| 4 => Bad dy value

Author:		Otto Milvang and Tor Lønnestad, BLAB, Ifi, UiO

Examples:	maxarea(b1, b2, 3, 5);

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
BiffStatus maxarea(in_band, out_band, dx, dy)
IBAND in_band, out_band;
int dx, dy;
#else /* FUNCPROTO */
BiffStatus maxarea(IBAND in_band, IBAND out_band, int dx, int dy)
#endif /* FUNCPROTO */
{
   int xsize, ysize;
   int xdist, ydist, x, y;
   UNS_BYTE max;
   int first, last, dest;
 
   if (Ipixtyp(in_band) NE Iu_byte_typ)
     return(Error(1, "maxarea: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(out_band) NE Iu_byte_typ)
     return(Error(2, "maxarea: Output pixel type must be unsigned byte.\n"));
   if (dx < 1) return(Error(3, "maxarea: Bad dx value\n"));
   if (dy < 1) return(Error(4, "maxarea: Bad dy value\n"));

   xdist = dx/2;
   ydist = dy/2;
   dx = 2*xdist+1;
   dy = 2*ydist+1;
   xsize = Ixsize(in_band);
   ysize = Iysize(in_band);

   for (y=1; y LE ysize; y++) {
     max = 0;
     last = 1;
     for (first=1; first LE MIN(xsize, dx); first++)
       if (in_band[y][first] GT max) max = in_band[y][first];

     /* put everything one to the right of center of area */
     for (dest = 2; dest LE MIN(xsize, xdist+2); dest++)
       out_band[y][dest] = max;

     for (first = dx+1; first LE xsize; ) {
       if (in_band[y][first] GE max) max = in_band[y][first]; 
       else if (in_band[y][last] EQ max) {
         max = in_band[y][last+1];
         for (x = last+2; x LE first; x++)
           if (in_band[y][x] GT max) max = in_band[y][x];
       }
       out_band[y][dest] = max;
       ++ first; ++ last; ++ dest;
     }

     /* copy out to the margin */
     for (x=dest; x LE xsize; x++) out_band[y][x] = max;
   }  /* all y */

   for (x=2; x LE xsize; x++) { /* start in x=2!!*/
     max = 0;
     last = 1;
     for (first=1; first LE MIN(ysize, dy); first++)
       if (out_band[first][x] GT max) max = out_band[first][x];

     /* put everything one to the left of center of area */
     for (dest = 1; dest LE MIN(ysize, ydist+1); dest++)
       out_band[dest][x-1] = max;

     for (first = dy+1; first LE ysize; ) {
       if (out_band[first][x] GE max)  max = out_band[first][x]; 
       else if (out_band[last][x] EQ max) {
         max = out_band[last+1][x];
         for (y = last+2; y LE first; y++)
           if (out_band[y][x] GT max) max = out_band[y][x];
       }
       out_band[dest][x-1] = max;
       ++ first; ++ last; ++ dest;
     }

     /* copy out to the margin */
     for (y=dest; y LE ysize; y++) out_band[y][x-1] = max;
   }  /* all x */

   /* copy to last column */
   for (y=1; y LE ysize; y++)
     out_band[y][xsize] = out_band[y][xsize-1];

   return(0);
}  /* maxarea */

#endif /* not MAIN */



/*P:maxarea*

________________________________________________________________

		maxarea
________________________________________________________________

Name:		maxarea - max operator over a local window

Syntax:		| maxarea <inimage> <outimage> <dx> <dy>
                | maxarea <inimage> <outimage> <area>
		| maxarea <inimage> <outimage>

Description:	'maxarea' performs a maximum operation on the input image
                'inimage' and writes the result to 'outimage'.
                If two areas are supplied, they are taken to be 'dx'
                and 'dy' resp. If only one is given, the area is
                assumed to be quadratic. If none, area is set to 3x3. 

See also:	

Restrictions:	Bands of 'inimage' and 'outimage' must have pixel type
                unsigned byte.

Author:		Otto Milvang & Tor Lønnestad, BLAB, Ifi, UiO

Examples:	| max mona.img monamax.img 3 5
                | max mona.img monamax.img 7
                | max mona.img monamax.img
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE img1, img2;
   int bn, dx, dy, stat;
   char *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   if ((argc LT 3) OR (argc GT 5))
     Usage(2, "Illegal number of arguments.\n");

   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc EQ 5) dy = atoi(argv[4]); else dy = dx;

   if ((img1 = Iread_image(argv[1]))) {
     img2 = Icopy_init(img1);
     for (bn=1; bn LE Inbands(img1); bn++)
       if ((stat = maxarea(img1[bn],img2[bn],dx,dy)))
         Warning(stat, "Error in band %d\n", bn);

     Ihistory(img2, argv[0], args);
     Iwrite_image(img2,argv[2]);
   } else {
     Error(5, "Can't read input file %s\n", argv[1]);
   }

   return(0);

}  /* main */

#endif /* MAIN */
