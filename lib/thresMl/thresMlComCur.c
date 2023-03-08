/*

________________________________________________________________

        thresMlComCur
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
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/thresMl.h>



/*F:mkComCur*

________________________________________________________________

		mkComCur
________________________________________________________________

Name:		mkComCur - make complexity curve for a band

Syntax:         | #include <xite/thresMl.h>
		|
                | int mkComCur( IBAND band, histogram cc );

Description:    'mkComCur' produces the complexity curve of an N-bit
                 image 'band' by calculating the lengths of black and white 
                 boundaries for every possible threshold.

		'cc' is the returned complexity curve, indexable on 0..255.

Restrictions:   'band' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 2 => Bad pixel type

Reference:      &Sei-ichiro Kamata, Richard O. Eason and Eiji Kawaguchi
                "Complexity Curves Versus Histograms and Their Application
                to Image Segmentation",	1070-1077 

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(3), thresMlCentroid(3), thresMlComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Author:		Hung Buu Huynh, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int mkComCur(IBAND band, int *cc)
{
  int B[257];
  int n, x, y, n1, n2, xsize, ysize;

  if (Ipixtyp(band) != Iu_byte_typ)
    return(Error(2, "mkComCur: Input pixel type must be unsigned byte.\n"));

  for (n=0; n<257; n++) B[n] = 0;
  xsize = Ixsize(band);
  ysize = Iysize(band);

  for (y=1; y<=ysize; y++) {
    for (x=1; x<=xsize-1; x++) {
      if (band[y][x] < band[y][x+1])
        { n1 = band[y][x] + 1; n2 = band[y][x+1] + 1; }
      else
        { n1 = band[y][x+1] + 1; n2 = band[y][x] + 1; }
      B[n1]++; B[n2]--;
    }
  }

  for (y=1; y<=ysize-1; y++) {
    for (x=1; x<=xsize; x++) {
      if (band[y][x] < band[y+1][x])
        { n1 = band[y][x] + 1; n2 = band[y+1][x] + 1; }
      else
        { n1 = band[y+1][x] + 1; n2 = band[y][x] + 1; }
      B[n1]++ ; B[n2]--  ;
    }
  }

  cc[0] = B[0] ;   
  for (n=1; n<256 ; n++) 
     cc[n] = cc[n-1] + B[n];

  return(0);

} /* end of mkComCur */




/*F:thresMlComCur*

________________________________________________________________

		thresMlComCur
________________________________________________________________

Name:		thresMlComCur - multi-level threshold using complexity curve

Syntax:         | #include <xite/thresMl.h>
		|
                | int thresMlComCur( IBAND band, int* thresholds,
                |    int nt );

Description:    'thresMlComCur' performs multi-level thresholding of
                the input 'band'. The result is a vector of 'thresholds',
		with useful values in 0..'nt'-1. The algorithm is based
		on Complexity Curves, see mkComCur(3). The complexity
		curve is created, and then thresholded by means of the
		algorithm or Reddi, see thresMlReddi(3).

		'nt' is the desired number of thresholds.

Restrictions:   'band' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 2 => Bad pixel type

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(3), thresMlCentroid(3), mkComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Reference:      &Sei-ichiro Kamata, Richard O. Eason and Eiji Kawaguchi
                "Complexity Curves Versus Histograms and Their Application
                to Image Segmentation",	1070-1077

Author:		Hung Buu Huynh, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int thresMlComCur(IBAND band, int *thresholds, int nt)
{
  histogram cc;
  int stat;

  if (Ipixtyp(band) != Iu_byte_typ)
    return(Error(2,
		 "thresMlComCur: Input pixel type must be unsigned byte.\n"));

  stat = mkComCur(band, cc);
  if (stat) Warning(2, "Error in mkComCur.\n");
  stat = thresMlReddi(cc, thresholds, nt);
  if (stat) Warning(2, "Error in thresMlReddi.\n");

  return(0);

} /* end of thresMlComCur */
