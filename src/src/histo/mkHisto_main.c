

/*

________________________________________________________________

        mkHisto
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:mkHisto*

________________________________________________________________

		mkHisto
________________________________________________________________

Name:		mkHisto - make and print a histogram

Syntax:		mkHisto [-header] [-i] <inimage> <lim0> [<lim1> [<lim2> ...]]

Description:    Creates a histogram and writes the histogram values
                to stdout. The limits (one or more) define the
		borders of the accumulator cells. The limits must
		be sorted in ascending order. The specified limits
		are applied to all bands in the image. The limits
		-Inf and Inf are always added. 

		The histogram will contain the following values:
		| histo[0] = #pixels in <-Inf .. limits[0]]
		| histo[i] = #pixels in <limits[i-1] .. limits[i]]
		| histo[n] = #pixels in <limits[n-1] .. Inf>

Options:	&-header
                A textual header will be given. By default the histogram
		values are given in ascii without any explanation.

		&-i
		The limits are interpreted as integer values. By default,
		they are assumed to be floating point (double).

See also:	

Restrictions:   Does not accept complex and double complex pixel type.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | # how many pixels are below (or equal) and above 100?
                | mkHisto mona.img -i 100
		| # how many pixels are in 0..50, 50..100 etc.?
		| mkHisto mona.img -header 50 100 150 200 250

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int i, n, bn, header, integer;
  int *limits, *histo;
  double *dlimits;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-header] [-i] <inimage> <lim0> [<lim1> [<lim2> ... ]]\n"));

  if (argc == 1) Usage(1, NULL);

  header  = read_bswitch(&argc, argv, "-header");
  integer = read_bswitch(&argc, argv, "-i");

  if (argc < 3) Usage(2, "Illegal number of arguments.\n");
  img = Iread_image(argv[1]);
  if (NOT img) Error(3, "Couldn't read %s.\n", argv[1]);
  n = argc-2;
  histo  = (int*)malloc((n+1)*sizeof(int));

  if (integer) {
    limits = (int*)malloc(n*sizeof(int));
    for (i=0; i<n; i++) limits[i] = atoi(argv[i+2]);
  } else {
    dlimits = (double*)malloc(n*sizeof(double));
    for (i=0; i<n; i++) dlimits[i] = atof(argv[i+2]);
  }

  for (bn=1; bn LE Inbands(img); bn++) {
    if (integer) {
      mkIhistogram(img[bn], histo, limits, n);
      if (header) {
        printf("Low limit  High limit  Number of pixels\n");
        printf("---------------------------------------\n");
        printf("  -inf       %4d         %6d\n", limits[0], histo[0]);
        for (i=1; i<n; i++)
	  printf("  %4d       %4d         %6d\n",
		 limits[i-1]+1, limits[i], histo[i]);
        printf("  %4d        inf         %6d\n", limits[n-1]+1, histo[n]);
      } else {
        for (i=0; i<=n; i++) printf(" %d ", histo[i]);
      }
      printf("\n");
    } else {
      mkDhistogram(img[bn], histo, dlimits, n);
      if (header) {
        printf("Low limit  High limit  Number of pixels\n");
        printf("---------------------------------------\n");
        printf("    -inf     %7.2f       %6d\n", dlimits[0], histo[0]);
        for (i=1; i<n; i++)
	  printf(" %7.2f     %7.2f       %6d\n",
		 dlimits[i-1], dlimits[i], histo[i]);
        printf(" %7.2f         inf       %6d\n", dlimits[n], histo[n]);
      } else {
        for (i=0; i<=n; i++) printf(" %d ", histo[i]);
      }
      printf("\n");
    }
  }
  return(0);
}
