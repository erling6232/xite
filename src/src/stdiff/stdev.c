
/*

________________________________________________________________

        stdev.c
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
#include <xite/stdiff.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
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
#define isqrt(n) isqrtarr_s[n]
#define isqr(n) isqrarr_s[n]
#define OUTPUT isqrt((int) (((float)sumxx - (float)sumx*(float)sumx/N)/N ));

static int isqrtarr_s[129*129];
static int isqrarr_s[256];

/*F:stdev*

________________________________________________________________

		stdev
________________________________________________________________

Name:		stdev - calculation of local standard deviation 

Syntax:         | #include <xite/stdiff.h>
		|
                | int stdev( IBAND input, IBAND output, int dx,
                |    int dy );

Description:	'stdev' calculates and returns the standard
                deviation in a local window of size 'dx' * 'dy'.
		The border is also processed, calculating the
		standard deviation over the part of the window
		that is defined.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Return value:	0 => ok
                1 => bad pixel type input
                2 => bad pixel type output
                3 => bad dx value
                4 => bad dy value

See also:       stdev(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int stdev(IBAND input, IBAND output, int dx, int dy)
{
   int x, y, yystart, yystop, yysize, xxstart, xxstop;
   int xsize, ysize, pixval, N, sumx, sumxx, xx, yy, i, j, k, l;
   int dxhalf, dyhalf;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "stdev: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(2, "stdev: Output pixel type must be unsigned byte.\n"));
   if (dx < 0) return(Error(3, "stdev: Bad dx value.\n"));
   if (dy < 0) return(Error(4, "stdev: Bad dy value.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));
   dxhalf = (dx/2); dx = dxhalf*2+1;
   dyhalf = (dy/2); dy = dyhalf*2+1;
   for (i=0; i<=128; i++) {
     j = i*i; k = (i+1)*(i+1);
     for (l=j; l<k; l++) isqrtarr_s[l] = i;
   }
   for (i=0; i <= 255; ++ i) isqrarr_s[i] = i*i;

   for (y=1; y <= ysize; ++ y) {
     /* calc available area */
     yystart = MAX(1,y-dyhalf);
     yystop  = MIN(ysize,y+dyhalf);
     yysize  = yystop-yystart+1;
     xxstart  = 1;
     xxstop   = MIN(dxhalf+1, xsize);
     sumx = 0;
     sumxx = 0;
     for (yy=yystart; yy <= yystop; ++ yy) {
       for (xx=xxstart; xx <= xxstop; ++ xx) {
	 pixval = input[yy][xx];
         sumx += pixval;
         sumxx += isqr(pixval);
       }
     }
     N = yysize*(xxstop-xxstart+1);
     output[y][1] = OUTPUT;

     /* update along the line until window is n*n */
     for (x=2; xxstop < dx; ++ x) {
       ++ xxstop;
       for (yy=yystart; yy <= yystop; ++ yy) {
	 pixval = input[yy][xxstop];
         sumx += pixval;
         sumxx += isqr(pixval);
       }
       N += yysize;
       output[y][x] = OUTPUT;
     }

     /* update along the line while window is n*n */
     for ( ; xxstop < xsize; ++ x) {
       ++ xxstop;
       for (yy=yystart; yy <= yystop; ++ yy) {
	 pixval = input[yy][xxstart];
         sumx -= pixval;
         sumxx -= isqr(pixval);
	 pixval = input[yy][xxstop];
         sumx += pixval;
         sumxx += isqr(pixval);
       }
       ++ xxstart;
       output[y][x] = OUTPUT;
     }

     /* update along the line until end of line */
     for ( ; x <= xsize; ++ x) {
       for (yy=yystart; yy <= yystop; ++ yy) {
	 pixval = input[yy][xxstart];
         sumx -= pixval;
         sumxx -= isqr(pixval);
       }
       ++ xxstart;
       N -= yysize;
       output[y][x] = OUTPUT;
     }   /* all x.. */
   }   /* all y... */
   return(0);
}  /* END of stdev */

#endif /* not MAIN */



/*P:stdev*

________________________________________________________________

		stdev
________________________________________________________________

Name:		stdev - calculation of local standard deviation

Syntax:		| stdev <inimage> <outimage> [<dx> [<dy>]]

Description:    'stdev' calculates and returns the standard
                deviation in a local window of size 'dx' * 'dy'.
		The border is also processed, calculating the
		standard deviation over the part of the window
		that is defined.

		'dx' is the horizontal window size, default value 3.
		'dy' is the vertical window size, default value 'dx'.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                If 'dx' or 'dy' are even, they are increased by one.
		'dx' and 'dy' should be positive.

See also:	stdev(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       | stdev mona.img monaStd.img
                | stdev mona.img monaStd.img 15
                | stdev mona.img monaStd.img 5 9

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int dx, dy, bn;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>] ] \n"));

   if (argc == 1) Usage(0, NULL);
   args = argvOptions(argc, argv);
   if ((argc < 3) || (argc > 5)) Usage(2, "Illegal number of arguments.\n");

   if (argc >= 4) dx = atoi(argv[3]); else dx = 3; 
   if (argc >= 5) dy = atoi(argv[4]); else dy = dx;
   if ((dx < 0) || (dy < 0))
     exit(Error(3, " Window size should be greater than 0 \n"));
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn = 1; bn <= Inbands(i1); ++ bn)
     if (stdev(i1[bn], i2[bn], dx, dy))
       Warning(4, " Error in band %d\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}

#endif /* MAIN */
