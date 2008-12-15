

/*

________________________________________________________________

        lit
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAIN

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define OUTPUT						\
     IF (dp)						\
       hval = 0;					\
       hmin = MAX(0,((int)input[y][x] - dp));		\
       hmax = MIN(255, ((int)input[y][x] + dp));	\
       for (hi=hmin; hi LE hmax; INC hi) hval += h[hi];	\
     ELSE hval = h[input[y][x]];			\
     ENDIF;						\
     output[y][x] = MIN(hval,255);


static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
   int x, y;
   for (x=0; x LE 255; INC x) h[x] = 0;
   for (y=hystart; y LE hystop; ++y)
     for (x=hxstart; x LE hxstop; ++x)
       ++ h[input[y][x]];
}
 
/*F:lit*

________________________________________________________________

		lit
________________________________________________________________

Name:		lit - Local Information Transform texture measure

Syntax:         | #include <xite/median.h>
		|
                | int lit( IBAND input, IBAND output, int dx,
                |    int dy, int dp );
Description:    'lit' filters the input band into the output band
                using a local filter with window size 'dx' * 'dy'.
		Even values of 'dx' and 'dy' are increased by one.
                Uses histogram updating when moving horizontally
		from pixel to pixel.

		Algorithm:
		lit(x,y) is number of pixels inside the processing
		window (including the center pixel itself) with
		pixel value in the interval cp-dp..cp+dp, where cp 
		is center pixel value.

		If histogram values become larger than 255, the
		pixels receive the value 255.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Reference:      Lowitz, Gabriel E.: "Extracting the Local
		Information Content of a Spatial Image" in Proc.
		7.ICPR, Montral, Canada, Aug. 1984, 1266-1268.

Return value:   | 0 => ok
                | 1 => bad pixel value input band
                | 2 => bad pixel value output band
		| 3 => bad dx value (less than 1 or greater than xsize)
		| 4 => bad dy value (less than 1 or greater than ysize)
		| 5 => bad dp value (less than 0 or greater than 255)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/



int lit(IBAND input, IBAND output, int dx, int dy, int dp)
{
   int hy, hystart, hystop, hysize, hxstart, hxstop;
   int x, y, xsize, ysize, dxhalf, dyhalf, hval, hi, hmin, hmax;
   histogram h;

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));
   dxhalf = dx/2; dyhalf = dy/2;
   dx = dxhalf*2+1; dy = dyhalf*2+1;

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "Input band must have pixel type unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "Output band must have pixel type unsigned byte.\n"));
   if ((dx LT 1) OR (dx GT xsize))
     return(Error(3, "Illegal value for horizontal window size.\n"));
   if ((dy LT 1) OR (dy GT ysize))
     return(Error(4, "Illegal value for vertical window size..\n"));
   if ((dp LT 0) OR (dp GT 255))
     return(Error(5, "Illegal value for pixel interval.\n"));

   FOR (y=1; y LE ysize; INC y)
     /* calc available area */
     hystart = MAX(1,y-dyhalf);
     hystop  = MIN(ysize,y+dyhalf);
     hysize  = hystop-hystart+1;

     /* calc. initial histogram */
     hxstart  = 1;
     hxstop   = dxhalf+1;
     x =1;
     hist(input,hxstart,hystart,hxstop,hystop,h);
     OUTPUT;

     /* update along the line until window is dx*dy */
     FOR (x=2; hxstop LT dx; INC x)
       ++ hxstop;
       /* update histgr */
       for (hy=hystart; hy LE hystop; INC hy)
         ++ h[input[hy][hxstop]];
       OUTPUT;
     ENDFOR;

     /* update along the line while window is dx*dy */
     FOR ( ; hxstop LT xsize; INC x)
       /* update histgr */
       ++ hxstop;
       FOR (hy=hystart; hy LE hystop; INC hy)
         -- h[input[hy][hxstart]];
         ++ h[input[hy][hxstop]];
       ENDFOR;
       ++ hxstart;
       OUTPUT;
     ENDFOR;

     /* update along the line until end of line */
     FOR ( ; x LE xsize; INC x)
       /* update histgr */
       for (hy=hystart; hy LE hystop; INC hy)
         -- h[input[hy][hxstart]];
       ++ hxstart;
       OUTPUT;
     ENDFOR;
   ENDFOR;
   return(0);
}  /* END of lit */

#endif /* not MAIN */



/*P:lit*

________________________________________________________________

		lit
________________________________________________________________

Name:		lit - Local Information Transform texture measure

Syntax:		lit <inimage> <outimage> <dp> [<dx> [<dy>]]

Description:    lit transform with window size 'dx' * 'dy' using
                the histogram updating technique. lit counts the 
		number of pixels, inside the 'dx' * 'dy' filtering 
		window, having a pixel value in the interval
		cp-dp..cp+dp, where cp is the value of the
		center pixel, and 'dp' is the delta pixel value
		argument.

		'dx' is the horizontal window size. Default value 3.
		'dy' is the vertical window size. Default value 'dx'.

Restrictions:   Input image must have bands with pixel type unsigned byte.
                'dx', 'dy' must be larger than 0, but not larger than the
		input image size. 'dp' must be 0..255.

Reference:      Lowitz, Gabriel E.: "Extracting the Local 
		Information Content of a Spatial Image"
		in Proc. 7.ICPR, Montral, Canada, Aug. 1984,
		1266-1268.

See also:	litSnn(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       | lit mona.img monalit.img 0
                | lit mona.img monalit.img 5 7
		| lit mona.img monalit.img 5 7 11

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1,i2;
   int bn, dx, dy, dp;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage> <dp> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if ((argc < 4) OR (argc GT 6)) Usage(1, "Illegal number of arguments.\n");

   dp = atoi(argv[3]);
   if (argc GE 5) dx = atoi(argv[4]); else dx = 3;
   if (argc GE 6) dy = atoi(argv[5]); else dy = dx;
   if ((dp LT 0) OR (dp GT 255))
     Error(2, "Delta pixel value 'dp' must be 0..255.\n");

   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window size should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (lit(i1[bn], i2[bn], dx, dy, dp))
       Error(2, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}

#endif /* MAIN */
