
/*

________________________________________________________________

        thresRidCal
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
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/threshold.h>



/*F:thresRidCal*

________________________________________________________________

		thresRidCal
________________________________________________________________

Name:		thresRidCal - threshold by the Ridler and Calvard method

Syntax:         | #include <xite/threshold.h>
		|
                | int thresRidCal( histogram h );
Description:	'thresRidCal' calculates the threshold value by using 
		the method of Ridler and Calvard, 1972. Initially, 
		the threshold value is set to the average brightness.
		Then, iteratively, the threshold is set to the average
		of the mean of the pixels below the threshold, and the
		mean of the pixels above the threshold. When the
		threshold value is constant, iteration terminates.

		The actual thresholding is not performed, see the
		routine "threshold".
		
Restrictions:    

Return value:	threshold value. Pixel values up to, and including,
		the returned threshold value, is below the threshold.
		If any kind of error, -1 is returned.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresRidCal(int *h)
{
   int t, i, newt, sumh, sumhi;
   double meanbelow, meanabove;

   sumh = 0; sumhi = 0;
   FOR (i=0; i LE 255; INC i)
     sumh += h[i];
     sumhi += h[i]*i;
   ENDFOR;
   if (NOT sumh)
     return(Error(-1, "thresRidCal: No pixels?\n"));
   t = (int)((float)sumhi/(float)sumh + 0.5);

   LOOP
     sumh = 0; sumhi = 0;
     FOR (i=0; i LE t; INC i)
       sumh += h[i];
       sumhi += h[i]*i;
     ENDFOR;
     meanbelow = (sumh) ? (double)sumhi/(double)sumh : (double)t;
     sumh = 0; sumhi = 0;
     FOR (i=t+1; i LE 255; INC i)
       sumh += h[i];
       sumhi += h[i]*i;
     ENDFOR;
     meanabove = (sumh) ? (double)sumhi/(double)sumh : (double)t;
     newt = (int)((meanbelow+meanabove)/2.0 + 0.5);
     if (newt EQ t) return(t);
     t = newt;
   ENDLOOP
} /*  thresRidCal  */
