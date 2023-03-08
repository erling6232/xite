
/*C*

________________________________________________________________

        thresMlReddi
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/thresMl.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*F:thresMlReddi*

________________________________________________________________

		thresMlReddi
________________________________________________________________

Name:		thresMlReddi - multi-level thresholding by Reddi et al.

Syntax:         | #include <xite/thresMl.h>
		|
                | int thresMlReddi( histogram h, int* t, int nt );

Description:	'thresMlReddi' performs multi-level thresholding according to
                the method by Reddi et al.
		'h is input histogram, 't' is output threshold array,
		indexable on 0..'nt'-1.

Reference:      &S. S. Reddi, S. F. Rudin and H. R. Keshavan
                "An Optimal Multiple Threshold Scheme for Image Segmentation"
		IEEE Transactions on Systems, Man and Cybernetics,
		Vol SMC-14, pp. 661-665, 1984.

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(3), thresMlCentroid(3), mkComCur(3),
                thresMlComCur(3), thresMlComCur(1), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Return value:	| 0 => ok
                | 1 => nt less than 1

Author:		Martin Torpe Lie, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int thresMlReddi(int *h, int *t, int nt)
{
  histogram cum;
  int l, split, split_val, flag, temp, diff;
  double  mean1, mean2;
  long cumi[256];

  if (nt LT 1) return(Error(1, "thresMlReddi: nt less than 1\n"));

  cum[0]=h[0];
  cumi[0]=0;
  FOR (l=1; l LE 255; INC l)
    cum[l] = cum[l-1] + h[l];
    cumi[l] = cumi[l-1] + h[l] * l;
  ENDFOR;

  split_val=0;
  split=((cum[255])/(nt+1));
  FOR (l=0; l LT nt; INC l)
    while (cum[split_val] LT split*(l+1)) INC split_val;
    t[l]=split_val;
  ENDFOR;

  flag=1;
  WHILE (flag)
    flag=0;
    mean1 = (double)cumi[t[0]]/cum[t[0]];
    FOR (l=0; l LT nt-1; INC l)
      diff = cum[t[l+1]]-cum[t[l]];
      mean2 = (diff) ? (((double)cumi[t[l+1]]-cumi[t[l]])/diff) 
                : ((double)cumi[t[l]]/cum[t[l]]);
      temp = (int)((mean1+mean2)/2.0 + 0.5);
      if (temp NE t[l]) flag=1;
      t[l] = temp;
      mean1 = mean2;
    ENDFOR;
    diff = cum[255]-cum[t[nt-1]];
    mean2 = (diff) ? (((double)cumi[255]-cumi[t[nt-1]])/diff) 
             : ((double)cumi[255]/cum[255]);
    temp = (int)((mean1+mean2)/2.0 + 0.5);
    if (temp NE t[nt-1]) flag=1;
    t[nt-1] = temp;
  ENDWHILE;
  return(0);
}
