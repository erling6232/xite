
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

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	| thresMlCentroid -median mona.img
		| thresMlCentroid -mean -eq mona.img res.img
		| thresMlCentroid  -s 2 -m 25 mona.img res.img
		| thresMlCentroid  -verbose mona.img

Id: 		$Id$
________________________________________________________________

*/

static void thres2map(int *thres, int *map, int nt, double s)
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


int main(int argc, char **argv)
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
