
/*

________________________________________________________________

        thresUniErr
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>
#include XITE_STRING_H



#ifndef MAIN

/*F:thresUniErr*

________________________________________________________________

		thresUniErr
________________________________________________________________

Name:		thresUniErr - Uniform Error Threshold

Syntax:         | #include <xite/threshold.h>
		|
                | int thresUniErr( IBAND band );
Description:	'thresUniErr' calculates
		the threshold value needed to threshold the 
                image according to the uniform error thresholding metod
                as described in Dunn & Harwood & Davis.

		The actual thresholding is not performed, see the
		routine "threshold".
		
Reference:      S. M. Dunn & D. Harwood & L. S. Davis: "Local Estimation 
                of the Uniform Error Threshold" IEEE Trans. on PAMI, 
                Vol PAMI-6, No 6, Nov 1984.

Return value:	The threshold value. Pixel values up to, and including,
		the return value, is below the threshold.
		Returns -1 if bad pixel type, accepts ony UNS_BYTE pixels.

Bugs:       	It only works well on images whith large objects.

Author:		Olav Borgli, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int thresUniErr(band)
IBAND band;
#else /* FUNCPROTO */
int thresUniErr(IBAND band)
#endif /* FUNCPROTO */
{
  int x, y, i, j, tmp, bottom, top, ant2x2, maks1, maks2, maks4, t;
  int xsize, ysize, min, jmin;
  double a, b, c, phi;
  int g[5], tab1[257], tab2[257], tab4[257];

  if (Ipixtyp(band) NE Iu_byte_typ) return(-1);
  xsize = Ixsize(band); ysize = Iysize(band);
  for (i=0; i<=256; i++)  tab1[i] = tab2[i] = tab4[i] = 0;
  
  /* examine all 2x2 neighborhoods */

  for (y=1; y<ysize; y+=2)
    for (x=1; x<xsize; x+=2) {
      g[1] = band[y][x];
      g[2] = band[y][x+1];
      g[3] = band[y+1][x];
      g[4] = band[y+1][x+1];

      /* sorting */
      for (i=1; i<4; i++) {
	min = 300;
	for (j=i; j<=4; j++) {
	  if (min>g[j]) {
	    min = g[j];
	    jmin = j;
	  }
	}
        tmp = g[i];
        g[i] = g[jmin];
        g[jmin] = tmp;
      }

      /* Accumulating */
      for (i=1; i<=4; i++)  tab1[g[i]] += 1; 
      tab2[g[1]] +=3;
      tab2[g[2]] +=2;
      tab2[g[3]] +=1;
      tab4[g[1]] +=1;
    }

  /* Summing */
  for (i=255; i>=0; i--) {
    tab1[i] += tab1[i+1];
    tab2[i] += tab2[i+1];
    tab4[i] += tab4[i+1];
  }
  
  /* Tables are ready, find threshold */
  bottom = 0; top = 255;
  ant2x2 = (xsize/2)*(ysize/2);
  maks1 = tab1[0]; /* = ant2x2 * 4; */
  maks2 = tab2[0]; /* = ant2x2 * 6; */
  maks4 = tab4[0]; /* = ant2x2;     */

  /* binary search */
  while (bottom != top-1) {
    t = (int) ((bottom+top)/2);
    /* Calculate probabilities */
    a = (double) tab1[t+1]/maks1;
    b = (double) tab2[t+1]/maks2;
    c = (double) tab4[t+1]/maks4;
    phi = sqrt( (b*b - c) / (a*a - b) );
    if ( (double)(phi-1) > 0 )  bottom = t;
    else 		        top    = t;
  }
  
  return t;
}

#endif /* not MAIN */



#ifdef MAIN

/*P:thresUniErr*

________________________________________________________________

		thresUniErr
________________________________________________________________

Name:		thresUniErr - Uniform Error Threshold

Syntax:		| thresUniErr [-<sw>] <inimage> [<outimage>]

Description:	'thresUniErr' calculates, and eventually applies,
		the threshold value needed to threshold the 
                image according to the uniform error thresholding metod
                as described in Dunn & Harwood & Davis.

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

		Switches:
		| -lpv <lpv>  - low pixel value, default 0
		| -hpv <hpv>  - high pixel value, default 255

Reference:      | S. M. Dunn & D. Harwood & L. S. Davis:
                | "Local Estimation of the Uniform Error Threshold"
		| IEEE Trans. on PAMI, Vol PAMI-6, No 6, Nov 1984.
 
Restrictions:	Only UNS_BYTE pixels are supported

Bugs:       	It only works well on images whith large objects.

Author:		Olav Borgli, BLAB, ifi, UiO

Examples:	| thresUniErr mona.img 
                | thresUniErr mona.img monaThres.img

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int main(argc, argv)
int argc;  char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE img;
   int t, bn, lpv, hpv, output;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-<sw>] %s <inimage> [<outimage>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = read_iswitch(&argc, argv, "-lpv", 0);
   hpv = read_iswitch(&argc, argv, "-hpv", 255);

   if ((argc < 2) OR (argc > 3)) Usage(1, "Illegal number of arguments.\n");

   img = Iread_image(argv[1]);
   output = (argc == 3);
   if (output) Ihistory(img, argv[0], args);

   for (bn=1; bn<=Inbands(img); bn++) {
     t = thresUniErr(img[bn]);
     if (output) {
       thresholdSpOut(img[bn], img[bn], t, lpv, hpv);
     }
     else printf("Threshold value in band %d : %d\n", bn, t);
   }
   if (output) Iwrite_image(img, argv[2]);

   return(0);
}

#endif /* MAIN */
