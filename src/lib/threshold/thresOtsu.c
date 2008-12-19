
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>



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
