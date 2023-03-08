
/*C*

________________________________________________________________

        thresMlCentroid
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
#include <stdlib.h>
#include <xite/thresMl.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

static int eqHisto(int *h1, int *h2)
{
  int i;
  for (i=0; i<256; i++) if (h1[i] != h2[i]) return(FALSE);
  return(TRUE);
}




/*F:histoCentroid*

________________________________________________________________

		histoCentroid
________________________________________________________________

Name:		histoCentroid - move histogram mass to local centroid iteratively

Syntax:         | #include <xite/thresMl.h>
		|
                | int histoCentroid( histogram hin,
                |    histogram hout, int m, int method );

Description:    Iteratively, histogram mass is moved to local centroid.
                The centroid is the MEAN inside a histogram window if
		method=0, the MEDIAN inside the window if method=1.
		The size of the histogram window is (2*m)+1. This procedure
		is repeated until no changes, or at most 100 iterations.
		The "clustered" histogram result is returned in 'hout'.

Restrictions:   | 1 <= m <= 127
                | method = 0 OR method = 1

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), thresMlCentroid(3),
                thresMlCentroid(1), mkComCur(3), thresMlComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Return value:   | 0 => ok
                | 1 => Warning: The procedure didn't converge in
		|               100 iterations
		| 2 => Error:   Bad m argument
		| 3 => Error:   Bad method argument

Reference:      &R. Wilson
                "A Class of local centroid algorithms for
		classification and quantization in spaces of arbitrary
		dimension"Linkoping Univ, Internal Report LiTH-ISY-I-0610
		(1983).

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$

________________________________________________________________

*/
#define MEDIAN median
#define MEAN ((int)(0.5+sumih)/sumh)
#define CENT ((method) ? MEDIAN : MEAN) 

int histoCentroid(int *hin, int *hout, int m, int method)
{
  histogram H1, H2;
  int sumh, sumih, i, j, itera, median, mediansum;
  int *h1, *h2, *h3;

  if ((m<1) OR (m > 127))
    return(Error(2, "histoCentroid: Bad m value: %d\n", m));
  if ((method<0) OR (method > 1))
    return(Error(3, "histoCentroid: Bad method value: %d\n", method));

  h1=H1; h2=H2;
  for (i=0; i LT 256; i++) h1[i]=hin[i];

  FOR (itera=1; itera LT 100; itera++)
    j=0; sumh=0; sumih=0; mediansum=0;
    for (i=0; i LT 256; i++) h2[i] = 0;
    FOR (i=0; i LE 2*m; i++)
      sumh += h1[i];
      sumih += i*h1[i]
    ENDFOR;
    for (median=(-1); mediansum LT sumh/2; ) mediansum += h1[++median];
    if (sumh)
      for (i=0; i LE m; i++) h2[CENT] += h1[i];
    FOR (i=m+1; i LT 256-m; i++)
      sumh -= h1[i-m-1];
      sumih -= (i-m-1)*h1[i-m-1];
      mediansum -= h1[i-m-1];
      sumh += h1[i+m];
      sumih += (i+m)*h1[i+m];
      while (mediansum LT sumh/2) mediansum += h1[++median];
      if (sumh) h2[CENT] += h1[i];
    ENDFOR;
    if (sumh)
      for (i=256-m; i LT 256; i++)
        h2[CENT] += h1[i];
    EXITIF (eqHisto(h1, h2));
    h3=h2; h2=h1; h1=h3;
  ENDFOR;

  if (itera EQ 100)
    Warning(1, "thresMlCentroid: clustering doesn't terminate!\n");
  else
    Info(0,
	"histoCentroid: converged after %d iterations using m=%d\n", itera, m);
  /* return hout */
  for (i=0; i LT 256; i++) hout[i]=h2[i];

  return(0);
}


/*F:thresMlCentroid*

________________________________________________________________

		thresMlCentroid
________________________________________________________________

Name:		thresMlCentroid - multi level thresholding by local centroid method

Syntax:         | #include <xite/thresMl.h>
		|
                | int thresMlCentroid( histogram histo,
                |    histogram thres, int m, int method,
                |    int* nt );

Description:    Finds multi level thresholds.
                Iteratively, histogram mass is moved to local centroid.
                The centroid is the MEAN inside a histogram window if
		'method'=0, the MEDIAN inside the window if 'method'=1.
		The size of the histogram window is (2*'m')+1. This procedure
		is repeated until no changes, or at most 100 iterations.
		The mapping from old to new histogram is analyzed,
		corresponding thresholds are calculated, and returned.

		'histo' is the input band histogram, 'thres' is the output
		histogram to receive thresholds. 'nt' will return the number
		of thresholds.

		If 'm'=0, the above process is repeated in a loop over the 
		'm'-values 5, 10, 15, 20, 25, 30, 35, 40, 45 and 50, exiting
		if two consecutive runs give the same thresholding result.
		
		At return, 'thres' can be indexed on 0..'nt'-1.

Restrictions:   Histogram is defined on 0..255.

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(1), mkComCur(3), thresMlComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Return value:	| 0 => ok
                | 2 => Bad m value
		| 3 => Bad method value

Reference:      &R. Wilson
                "A Class of local centroid algorithms for
		classification and quantization in spaces of arbitrary
		dimension"Linkoping Univ, Internal Report LiTH-ISY-I-0610
		(1983).

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int thresMlCentroid(int *histo, int *thres, int m, int method, int *nt)
{
  histogram H1, H2;
  int i, j, low, high, sumi=0, sumj=0;
  int *h1, *h2, *h3;

  if ((m<0) OR (m > 127))
    return(Error(2, "thresMlCentroid: Bad m value: %d\n", m));
  if ((method<0) OR (method > 1))
    return(Error(3, "thresMlCentroid: Bad method value: %d\n", method));

  h1=H1; h2=H2;
  IF (m) histoCentroid(histo, h1, m, method);
  ELSE
    histoCentroid(histo, h1, 5, method);
    FOR (m=10; m LE 50; m+=5)
      histoCentroid(histo, h2, m, method);
      EXITIF (eqHisto(h1, h2));
      h3=h2; h2=h1; h1=h3;
    ENDFOR;
    if (m == 55) Warning(1, "thresMlCentroid: clustering never stable\n");
    else Info(0, "thresMlCentroid: Exiting with m=%d\n", m);
  ENDIF;

  IF (Verbose())
    printf("\nAfter clustering, the histogram contains:\n");
    for (i=0; i<256; i++) if (h1[i])
      printf("  Gray level %3d: %d\n", i, h1[i]);
    printf("\n");
  ENDIF;

  /* convert to thresholds */
  *nt = 0; j = -1;
  FOR (i=0; i LT 256; i++)
    IF (h1[i])
      sumi += h1[i];
      while ((sumj < sumi) AND (j<255)) sumj += histo[++j];
      low=j++;
      while ((j<256) AND (NOT histo[j])) ++j;
      IF (j<256)
        high = --j;
        thres[*nt] = (low+high)/2;
        (*nt)++;
      ENDIF;
    ENDIF;
  ENDFOR;
  return(0);
} 
