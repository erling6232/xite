
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

#ifndef MAIN

#ifndef FUNCPROTO
static int eqHisto(h1, h2)
histogram h1, h2;
#else /* FUNCPROTO */
static int eqHisto(int *h1, int *h2)
#endif /* FUNCPROTO */
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

Author:		Tor L�nnestad, BLAB, Ifi, UiO

Id: 		$Id$

________________________________________________________________

*/
#define MEDIAN median
#define MEAN ((int)(0.5+sumih)/sumh)
#define CENT ((method) ? MEDIAN : MEAN) 

#ifndef FUNCPROTO
int histoCentroid(hin, hout, m, method)
histogram hin, hout;
int m, method;
#else /* FUNCPROTO */
int histoCentroid(int *hin, int *hout, int m, int method)
#endif /* FUNCPROTO */
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

Author:		Tor L�nnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int thresMlCentroid(histo, thres, m, method, nt)
histogram histo, thres;
int m, method;
int *nt;
#else /* FUNCPROTO */
int thresMlCentroid(int *histo, int *thres, int m, int method, int *nt)
#endif /* FUNCPROTO */
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

#endif /* not MAIN */



/*P:thresMlCentroid*

________________________________________________________________

		thresMlCentroid
________________________________________________________________

Name:		thresMlCentroid - multi level thresholding by local centroid
                method

Syntax:		| thresMlCentroid [-m <m>] [-mean] [-median] [-eq] [-s <s>]
                |                 <inimage> [<output>]

Description:    For every band, the histogram is calculated. Then the
                histogram is modified by moving histogram mass to the
		local centroid of a histogram window of size (2*'m')+1.
		The mapping from old to new histogram is analyzed,
		corresponding thresholds are calculated, and if an
		output image is specified, the input image is thresholded.
		If no output image is specified, the obtained thresholds
		are written to standard output.

Options:        &-m m
                Use histogram window of size (2*'m')+1.

		&-mean
		Move histogram mass to local mean (default).

		&-median
		Move histogram mass to local median.

		&-eq
		If 'outimage' is given, distriibute output pixel values.

		&-s s
		If 'outimage' is given, scale pixel values with 's'.

		-&
		Use either '-mean' or '-median' (or none), not both.
		If no 'm' argument is given, a loop over the 'm'-values
		5, 10, 15, 20, 25, 30, 35, 40, 45 and 50 is performed,
		exiting if two consecutive runs give the same thresholding
		result.

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(3), mkComCur(3), thresMlComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Diagnostics:	| Warning: Bad pixel type in band ..
                | Warning: Clustering never stable
                | Error  : Bad m value: ..
		| Error  : Bad number of arguments

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                Bands are handled one at a time, not multi-dimensionally.

Reference:      &[1] 'R. Wilson. H Knutsson and G.H. Granlund'
		"The operational definition of the position of line
		and edge" Proc. ICPR-82, Munich 1982.

		&[2] 'R. Wilson'
		"A Class of local centroid algorithms for
		classification and quantization in spaces of arbitrary
		dimension"Linkoping Univ, Internal Report LiTH-ISY-I-0610
		(1983).

Author:		Tor L�nnestad, BLAB, Ifi, UiO

Examples:	| thresMlCentroid -median mona.img
		| thresMlCentroid -mean -eq mona.img res.img
		| thresMlCentroid  -s 2 -m 25 mona.img res.img
		| thresMlCentroid  -verbose mona.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
static void thres2map(thres, map, nt, s)
histogram thres, map;
int nt;
double s;
#else /* FUNCPROTO */
static void thres2map(int *thres, int *map, int nt, double s)
#endif /* FUNCPROTO */
{
  int g, l;

  for (g=0; g LE thres[0]; g++) map[g] = 0;
  for (l=0; l LT nt-1; l++)
    for (g=thres[l]; g LT thres[l+1]; g++)
      map[g] = (int)((l+1)*s + 0.5);
  for (g=thres[nt-1]; g LE 255; g++) map[g] = (int)(nt*s + 0.5);
    for (g=0; g LE 255; g++)
      if (map[g] LT 0) map[g]=0;
      else if  (map[g] GT 255) map[g]=255;
}


#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE img;
  histogram h, thres;
  int bn, nbands, m, nt, i, method, eq, stat;
  double scale;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [switches] <inimage> [<outimage>]\n"));
  Iset_message(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  m      = read_iswitch(&argc, argv, "-m", 0);
  method = (read_bswitch(&argc, argv, "-mean")) ? 0 : 0; /* mean is default */
  method = (read_bswitch(&argc, argv, "-median")) ? 1 : 0;
  eq     = read_bswitch(&argc, argv, "-eq");
  scale  = read_dswitch(&argc, argv, "-s", eq ? -1.0 : 1.0);

  if (argc>3) Usage(2, "Illegal number of arguments\n");

  img    = Iread_image(argv[1]);
  nbands = Inbands(img);

  for (bn=1; bn LE nbands; bn++) {
    stat = mkHisto(img[bn], h);
    if (stat) Warning(1, "Error in mkHisto().\n");
    stat = thresMlCentroid(h, thres, m, method, &nt);
    if (stat) Warning(1, "Error in thresMlCentroid.\n");

    if (argc==3) { /* threshold */
      if (eq) scale =  255.0/nt;
      thres2map(thres, h, nt, scale);
      stat = histoTransf(img[bn], img[bn], h);
      if (stat) Warning(1, "Error in histoTransf.\n");
    }
    if ((argc==2) || (Verbose())) {
      printf("Thresholds in Band %d: (%d)", bn, nt); 
      for (i=0;i LT nt; i++) printf(" %d ", thres[i]);
      printf("\n");
    }
  }

  if (argc==3) {
    Ihistory(img, argv[0], args);
    Iwrite_image(img, argv[2]);
  }

  return(0);
}

#endif /* MAIN */
