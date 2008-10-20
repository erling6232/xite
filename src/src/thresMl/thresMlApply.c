
/*C*

________________________________________________________________

        thresMlApply
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>



/*F:thresMlAppScale=thresMlApply*/
/*F:thresMlAppMean=thresMlApply*/
/*F:thresMlAppMedian=thresMlApply*/
/*F:thresMlApply*

________________________________________________________________

		thresMlApply
________________________________________________________________

Name:		thresMlApply, thresMlAppScale, thresMlAppMean,
                thresMlAppMedian - routines that applies ml-thresholds.

Syntax:         | #include <xite/thresMl.h>
		|
                | int thresMlAppScale( IBAND inband,
                |    IBAND outband, int* thresholds, int nt,
                |    double scale );
		|
                | int thresMlAppMean( IBAND inband,
                |    IBAND outband, int* thresholds, int nt,
                |    histogram histo );
		|
                | int thresMlAppMedian( IBAND inband,
                |    IBAND outband, int* thresholds, int nt,
                |    histogram histo );

Description:    The 'thresMlApply' routines apply a vector of thresholds
                (Multi-Level threshold) to a band 'inband' giving 'outband'.
		'inband' and 'outband' may be identical.

		'thresMlAppScale' gives all pixels below the first threshold
		value 0, all pixels between the first and the second: 'scale',
		then 2*'scale',... If 'scale' is too large ('scale'>255*'nt')
		it is adjusted down to fit.

		'thresMlAppMean' sets all pixels between two thresholds
		to the mean of all these pixels in the input band.

		'thresMlAppMedian' sets all pixels between two thresholds
		to the median of all these pixels in the input band.

		'nt'is the number of thresholds.
		'thresholds' is the vector of thresholds, indexable on 0..nt-1.
		'scale' is the scale factor. 'histo' is the histogram of
		inband.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:	thresMl(1), histoCentroid(3), thresMlCentroid(3),
                thresMlCentroid(1), mkComCur(3), thresMlComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Return value:   | 0 : Ok
		| 1 : Bad input pixel type
		| 2 : Bad output pixel type
                | 3 : nt is smaller than 1
		| 4 : scale is negative

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int thresMlAppScale(IBAND inband, IBAND outband, int *thresholds, int nt, double scale)
{
  histogram transTable;
  int i, tn=0, g=0;

  if (nt<1)
    return(Error(3, "thresMlAppScale: nt<1\n")); 
  if (scale < 0.0)
    return(Error(4, "thresMlAppScale: scale<0\n"));

  if (scale*nt > 255.0) scale = 255.0/nt;

  for (g=0; g LE thresholds[0]; g++) transTable[g]=0;
  for (tn=1; tn LT nt; tn++) {
    i = (int)(tn*scale);
    for (g=thresholds[tn-1]+1; g LE thresholds[tn]; g++) transTable[g]=i;
  }
  i = (int)(nt*scale);
  for (g=thresholds[nt-1]+1; g LE 255; g++) transTable[g]=i;
  
  return(histoTransf(inband, outband, transTable));
}


int thresMlAppMean(IBAND inband, IBAND outband, int *thresholds, int nt, int *histo)
{
  histogram transTable;
  int min, max, sum, g, tn;
  double sumg;

  if (nt<1)
    return(Error(3,"thresMlAppMean: nt<1\n"));

  FOR (tn=0; tn LE nt; tn++)
    IF (tn==0) min=0; max = thresholds[0];
    ELSEIF (tn==nt) min = thresholds[nt-1]+1; max = 255;
    ELSE min = thresholds[tn-1]+1; max = thresholds[tn];
    ENDIF;

    sum=0; sumg=0.0;
    FOR (g=min; g LE max; g++)
      sum  += histo[g];
      sumg += (double)g*histo[g];
    ENDFOR;
    for (g=min; g LE max; g++) transTable[g] = (int)(sumg/sum);
  ENDFOR;

  return(histoTransf(inband, outband, transTable));
}


int thresMlAppMedian(IBAND inband, IBAND outband, int *thresholds, int nt, int *histo)
{
  histogram transTable;
  int min, max, sum, medsum, g, tn, med;

  if (nt<1)
    return(Error(3, "thresMlAppMedian: nt<1\n"));

  FOR (tn=0; tn LE nt; tn++)
    IF (tn==0) min=0; max = thresholds[0];
    ELSEIF (tn==nt) min = thresholds[nt-1]+1; max = 255;
    ELSE min = thresholds[tn-1]+1; max = thresholds[tn];
    ENDIF;

    sum = 0;
    for (g=min; g LE max; g++) sum += histo[g];
    medsum = sum/2;
    sum = 0;
    for (g=min; sum<medsum; g++) sum += histo[g];
    med = g-1;
    for (g=min; g LE max; g++) transTable[g] = med;
  ENDFOR;

  return(histoTransf(inband, outband, transTable));
}
