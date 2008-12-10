
/*

________________________________________________________________

        thresLloyd
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/threshold.h>



#ifndef MAIN

/*F:thresLloyd*

________________________________________________________________

		thresLloyd
________________________________________________________________

Name:		thresLloyd - threshold by the method of Lloyd 

Syntax:         | #include <xite/threshold.h>
		|
                | int thresLloyd( histogram h );
Description:	'thresLloyd' calculates the threshold value by using
		the method of Lloyd, 1985. Initially, the threshold 
		value is  set to the average brightness. Then, 
		iteratively, the threshold is set to the average of
		the mean of the pixels below the threshold, and the
		mean of the pixels above the threshold , PLUS a
		compensation factor depending on the varince of the
		distribution. When the threshold value remains
		constant, iteration terminates.

		The actual thresholding is not performed, see the
		routine "threshold".
		
Restrictions:    

Return value:	The threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresLloyd(int *h)
{
   int t, i, newt, sumh, sumh1, sumh2;
   float mean1, mean2, var, var1, var2, ridcal;
   double sumhi, sumhi1, sumhi2, sumhii1, sumhii2; 

   /* find initial threshold, mean value */
   sumh = 0; sumhi = 0.0;
   FOR (i=0; i LE 255; INC i)
     sumh += h[i];
     sumhi += (double)h[i]*i;
   ENDFOR;
   if (NOT sumh) return(-1);
   t = (int)(sumhi/sumh + 0.5);

   /* iterate */
   LOOP
     sumh1 = 0; sumhi1 = 0.0; sumhii1 = 0.0;
     FOR (i=0; i LE t; INC i)
       sumh1 += h[i];
       sumhi1 += (double)h[i]*i;
       sumhii1 += (double)h[i]*i*i;
     ENDFOR;
     mean1 = (sumh1) ? (float)(sumhi1/sumh1) : (float)t;

     sumh2 = 0; sumhi2 = 0.0; sumhii2 = 0.0;
     FOR (i=t+1; i LE 255; INC i)
       sumh2 += h[i];
       sumhi2 += (double)h[i]*i;
       sumhii2 += (double)h[i]*i*i;
     ENDFOR;
     mean2 = (sumh2) ? (float)(sumhi2/sumh2) : (float)t;
     sumh = sumh1 + sumh2;
     sumhi = sumhi1 + sumhi2;
     var1 = sumhii1/sumh1 - (sumhi1/sumh1)*(sumhi1/sumh1);
     var2 = sumhii2/sumh2 - (sumhi2/sumh2)*(sumhi2/sumh2);
     var = (sumh1*var1 + sumh2*var2)/sumh;
     ridcal = (mean1+mean2)/2.0;
     newt = (int)(ridcal + var*log((double)sumh2/sumh1)/(mean1+mean2) + 0.5);
     if (newt EQ t) return(t);
     t = newt;
   ENDLOOP
}  /*  thresLloyd  */

#endif /* not MAIN */



/*P:thresLloyd*

________________________________________________________________

		thresLloyd
________________________________________________________________

Name:		thresLloyd - threshold by the method of Lloyd

Syntax:		| thresLloyd [-l <lpv>] [-h <hpv>]
		|   <inimage> [<outimage>]

Description:	'thresLloyd' calculates, and eventually applies,
		the threshold value needed to threshold the 
                image according to Lloyd's method.
		
		| lpv - output for low pixels (below thrs.) Default 0
		| hpv - output for high pixels (above thrs.) Default 255
		| inimage - input image
		| outimage - output image

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

Files:		
See also:	
Diagnostics:	

Restrictions:	Only UNS_BYTE pixels are supported

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| thresLloyd mona.img
		| thresLloyd -h 1 mona.img monaThres.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i;
   histogram h;
   int t, bn, lpv, hpv, output;
   char *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s  [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = read_iswitch(&argc, argv, "-l", 0);
   hpv = read_iswitch(&argc, argv, "-h", 255);

   if (argc GT 3) Usage(2, "Illegal number of arguments.\n");

   i      = Iread_image(argv[1]);
   output = (argc GE 3);

   FOR (bn=1; bn LE Inbands(i); bn++)
     mkHisto(i[bn], h);
     t = thresLloyd(h);
     if (output) thresholdSpOut(i[bn], i[bn], t, lpv, hpv);
     else printf("Threshold value in band %d : %d\n", bn, t);
   ENDFOR;

   IF (output)
     Ihistory(i, argv[0], args);
     Iwrite_image(i, argv[2]);
   ENDIF;

   return(0);
}

#endif /* MAIN */
