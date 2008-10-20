
/*

________________________________________________________________

        thresOtsu
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>



#ifndef MAIN

/*F:thresOtsu*

________________________________________________________________

		thresOtsu
________________________________________________________________

Name:		thresOtsu - threshold by the method of Otsu

Syntax:         | #include <xite/threshold.h>
		|
                | int thresOtsu( histogram h );

Description:	'thresOtsu' calculates the threshold value needed  
                to threshold the image that the histogram 'h' is
		created from, according to the method of Otsu.
		The actual thresholding is not performed, see the
		"threshold" routine.

Reference:	&Otsu, N.
                "A Threshold Selection Method from Gray-Level histograms"
		IEEE Trans. Syst., Man and Cyb.,
		Vol SMC-9, No.1, pp.62-66.		

Return value:	The threshold value.

See also:	thresOtsu(1), threshold(3), thresholdSpOut(3), thresPtile(3),
                thresRidCal(3),	thresLloyd(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/


int thresOtsu(int *h)
{
   int i, bestsplitpos, cumh255;
   double mean, splitval, bestsplitval, diff;
   histogram cumh;
   double cumhi[256];

   cumh[0] = h[0]; cumhi[0] = 0.0;
   for (i=1; i LE 255; INC i) {
     cumh[i] = cumh[i-1] + h[i];
     cumhi[i] = cumhi[i-1] + i*h[i];
   }
   cumh255 = cumh[255];
   mean = cumhi[255]/cumh255;

   bestsplitval = 0.0;
   for (i=0; i LE 255; INC i) {
     /* calc. split value */
     if ((cumh[i] NE 0) AND (cumh[i] NE cumh255)) {
       diff = mean*cumh[i] - cumhi[i];
       splitval = (diff/((double)cumh[i]*(cumh255-cumh[i])))*diff;
       if (splitval GT bestsplitval) {
         bestsplitval = splitval; bestsplitpos = i;
       }
     }
   }

   return(bestsplitpos);

}  /*  thresOtsu  */

#endif /* not MAIN */



/*P:thresOtsu*

________________________________________________________________

		thresOtsu
________________________________________________________________

Name:		thresOtsu - threshold by the method of Otsu

Syntax:		| thresOtsu [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]

Description:	'thresOtsu' calculates, and eventually applies, the
		threshold value by using the method of Otsu. The
		method attempts to maximize the between-class variance
		between the class of pixels above the threshold, and
		the class of pixels below.

                If an output image is given, the input image is
                thresholded and written to the output image.
                Otherwise the threshold value is only reported
                to standard output.

Options:        &-l lpv
                Output for low pixels (below threshold). Default 0.

                &-h hpv
		Output for high pixels (above threshold). Default 255.

Reference:	&Otsu, N.
                "A Threshold Selection Method from Gray-Level histograms"
		IEEE Trans. Syst., Man and Cyb.,
		Vol SMC-9, No.1, pp.62-66.

See also:	thresOtsu(3), threshold(1), thresPtile(1),
                thresRidCal(1),	thresLloyd(1)

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Author:         Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | thresOtsu mona.img
                | thresOtsu -h 1 mona.img monaThres.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE input, output;
   histogram h;
   int t, bn, lpv, hpv;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s  [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = atoi(read_switch(&argc, argv, "-l", TRUE, "0"));
   hpv = atoi(read_switch(&argc, argv, "-h", TRUE, "255"));

   if (argc < 2) Usage(1, "Illegal number of arguments.\n");

   input = Iread_image(argv[1]);
   if (argc >= 3) output = Icopy_init(input); else output = 0; 

   for (bn=1; bn <= Inbands(input); ++ bn) {
     mkHisto(input[bn], h);
     t = thresOtsu(h);
     if (output) thresholdSpOut(input[bn], output[bn], t, lpv, hpv);
     else Message(0, "Threshold value in band %d: %d.\n", bn, t);
   }
   if (output) {
     Ihistory(output, argv[0], args);
     Iwrite_image(output,argv[2]);
   }

   return(0);
}

#endif /* MAIN */
