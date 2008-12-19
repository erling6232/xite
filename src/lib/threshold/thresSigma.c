

/*

________________________________________________________________

        thresSigma
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
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/threshold.h>
#include <xite/message.h>
#include XITE_STRING_H



/*F:thresSigma*

________________________________________________________________

		thresSigma
________________________________________________________________

Name:		thresSigma - threshold at fraction of standard deviation

Syntax:         | #include <xite/threshold.h>
		|
		| int thresxsigma(h, x)  
                | histogram h;
		| double x;

                | int thresSigma( histogram h, double x );
                |
Description:	'thresSigma' calculates the threshold value by finding
                the mean (mu) and the standard deviation (sigma) 
		of the image, and then finding the threshold by 
		the formula:

		t = mu + (x * sigma)

		The actual thresholding is not performed, see the
		routine "threshold".
		
Return value:	The threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Olav Borgli, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresSigma(int *h, double x)
{ 
  int t, i, size;
  float mu, hn[256];
  double sigma;

  /* find size and norm. histo */
  size = h[0];
  for (i=1; i<=255; i++) size += h[i];
  for (i=0; i<=255; i++) hn[i] = h[i]/(float)size;

  /* find mean value */
  mu = 0.0;
  for (i=1; i<=255; i++) mu += hn[i]*i;

  /* find standard deviation */
  sigma = 0.0;
  for (i=0; i<=255; i++) sigma += (i-mu)*(i-mu)*hn[i];
  sigma = sqrt(sigma);
  t = (int)(mu + x*sigma);

  return(t);
}
