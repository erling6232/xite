
/*

________________________________________________________________

        mode
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
#include <xite/median.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
   register int x,y;
   for (x=0; x LE 255; INC x) h[x] = 0;
   for (y=hystart; y LE hystop; INC y)
     for (x=hxstart; x LE hxstop; INC x)
       INC h[input[y][x]];
}
 
/*F:mode*

________________________________________________________________

		mode
________________________________________________________________

Name:		mode - noise removal and edge sharpening

Syntax:         | #include <xite/median.h>
		|
                | int mode( IBAND input, IBAND output, int dx,
                |    int dy );

Description:    'mode' filters the 'input' band into the 'output' band
                using a mode filter with window size 'dx' x 'dy';
                the pixel value occurring most frequently in the
                window becomes the new center value. If no pixel
		value occurs more than once, the original pixel
		value is maintained. Even values of 'dx' or 'dy' are
		increased by one.
                Uses histogram updating when moving horizontally
		from pixel to pixel.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 1 => bad pixel type input band
                | 2 => bad pixel type output band
                | 3 => bad dx value (less than 1 or greater than xsize)
                | 4 => bad dy value (less than 1 or greater than ysize)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int mode(IBAND input, IBAND output, int dx, int dy)
{
   int i, j, hxstart, hxstop, hystart, hystop, hy, pixval, Number;
   int x, y, xsize, ysize, hysize, dxhalf, dyhalf, Pixval;
   histogram h,       /* histogram of local window area */
             pixvals, /* list of frequent occurring pixel values */
             many;    /* boolean, many[i]=TRUE if i is in pixvals */

   xsize  = MIN(Ixsize(input),Ixsize(output));
   ysize  = MIN(Iysize(input),Iysize(output));
   dxhalf = dx/2;
   dyhalf = dy/2;
   dx     = dxhalf*2+1;
   dy     = dyhalf*2+1;

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "Output pixel type must be unsigned byte.\n"));
   if ((dx LT 1) OR (dx GT xsize))
     return(Error(3, "Illegal value for horizontal window size.\n"));
   if ((dy LT 1) OR (dy GT ysize))
     return(Error(4, "Illegal value for vertical window size.\n"));

   FOR (y=1; y LE ysize; INC y)
     /* calc available area */
     hystart = MAX(1,y-dyhalf);
     hystop  = MIN(ysize,y+dyhalf);
     hysize  = hystop-hystart+1;

     /* calc. initial histogram */
     hxstart  = 1;
     hxstop   = dxhalf+1;
     hist(input,hxstart,hystart,hxstop,hystop,h);

     /* find all "frequent" pixel values */
     j=0;
     for (i=0; i LE 255; INC i) many[i]=FALSE;
     for (i=0; i LE 255; INC i)
       if (h[i] GE 2) {pixvals[j++]=i; many[i]=TRUE;}

     /* find the most frequent one */
     IF (j GT 0) THEN
       Pixval = pixvals[0]; Number = h[Pixval];
       for (i=1; i LT j; INC i)
         if (h[pixvals[i]] GT Number) {Pixval=pixvals[i]; Number=h[Pixval];}
       output[y][1] = Pixval;
     ELSE output[y][1] = input[y][1];
     ENDIF;

     /* update along the line until window has grown to dx*dy */
     FOR (x=2; hxstop LT dx; INC x)
       INC hxstop;
       /* update histgr */
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstop];
         if ((INC h[pixval]) EQ 2) {pixvals[j++]=pixval; many[pixval]=TRUE;}
       ENDFOR;
       /* calc most freq. */
       IF (j GT 0) THEN
         Pixval = pixvals[0]; Number = h[Pixval];
         for (i=1; i LT j; INC i)
           if (h[pixvals[i]] GT Number) {Pixval=pixvals[i]; Number=h[Pixval];}
         output[y][x] = Pixval;
       ELSE output[y][x] = input[y][x];
       ENDIF;
     ENDFOR;

     /* update along the line while window is dx*dy */
     FOR ( ; hxstop LT xsize; INC x)
       /* update histgr */
       INC hxstop;
       FOR (hy=hystart; hy LE hystop; INC hy)
         DEC h[input[hy][hxstart]];
         pixval = input[hy][hxstop];
         if (((INC h[pixval]) EQ 2) AND (NOT many[pixval]))
           {pixvals[j++]=pixval; many[pixval]=TRUE;}
       ENDFOR;
       INC hxstart;
       /* calc the most frequent */
       Pixval = input[y][x]; Number = 1;
       for (i=0; i LT j; )
         if (h[pixvals[i]] GT Number) /* new record */
           {Pixval=pixvals[i++]; Number=h[Pixval];}
         else if (h[pixvals[i]] LT 2) /* hasn't got many */
           {many[pixvals[i]]=FALSE; pixvals[i]=pixvals[DEC j];}
         else INC i;
       output[y][x] = Pixval;
     ENDFOR;

     /* update along the line until end of line */
     FOR ( ; x LE xsize; INC x)
       /* update histgr */
       for (hy=hystart; hy LE hystop; INC hy) DEC h[input[hy][hxstart]];
       INC hxstart;
       /* calc most frequent */
       Pixval = input[y][x]; Number = 1;
       for (i=0; i LT j; )
         if (h[pixvals[i]] GT Number) /* new record */
           {Pixval=pixvals[i++]; Number=h[Pixval];}
         else if (h[pixvals[i]] LT 2) /* hasn't got many */
           {many[pixvals[i]]=FALSE; pixvals[i]=pixvals[DEC j];}
         else INC i;
       output[y][x] = Pixval;
     ENDFOR;
   ENDFOR; /* all y... */
   return(0);
}  /* END of mode */

#endif /* not MAIN */



/*P:mode*

________________________________________________________________

		mode
________________________________________________________________

Name:		mode - noise removal and edge sharpening

Syntax:		| mode <inimage> <outimage> [<dx> [<dy>]]

Description:    'mode' noise filtering with window 'dx' x 'dy'
                using histogram updating technique. The pixel
                value occurring most frequently within the window
		becomes the new pixel value in the window center.

		'dx' is the horisontal window size, default value 3.
		'dy' is the vertical window size, default value 'dx'.

Restrictions:   Input image bands must have pixel type unsigned byte.

See also:	median(1), pseudomedian3x3(1), sigma(1), snn(1), knn(1),
                kncn(1), convolve(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | mode mona.img monamode.img
                | mode mona.img monamode.img 5
		| mode mona.img monamode.img 5 7

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, dx, dy;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc < 3) Usage(1, "Illegal number of arguments.\n");

   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc GE 5) dy = atoi(argv[4]); else dy = dx;
   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window sizes should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (mode(i1[bn], i2[bn], dx, dy))
       Error(2, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], args); 
   Iwrite_image(i2,argv[2]);

   return(0);
}

#endif /* MAIN */
