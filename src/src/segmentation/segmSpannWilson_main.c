
/*C*

________________________________________________________________

        segmSpannWilson
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
#include <xite/segmentation.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/pyramid.h>
#include <xite/readarg.h>
#include <xite/thresMl.h>

/*P:segmSpannWilson*

________________________________________________________________

                segmSpannWilson
________________________________________________________________

Name:           segmSpannWilson - segmentation by the Spann and Wilson method

Syntax:         | segmSpannWilson [-m <m>] [-mean] [-median] [-eq] [-s <s>]
                |                 <inimage> <outimage>

Description:    Performs segmentation of every band in the 'inimage' image 
		(separately) into the 'outimage' image. The algorithm has
		three steps:

		&Quad-tree smoothing

		A quad-tree is created, and pixels are averaged 4 by 4
		up in the quad-tree. 'n' quad-tree levels are created,
		including the bottom level, which is 'inband'.

		&Local centriod clustering

		The topmost quad-tree level is clustered by moving histogram
		mass to the centroid (mean or median) of a local window moved
		over the histogram. Applied iteratively, until stable. See
		'thresMlCentroid'.
		
		&Boundary estimation

		The classification result is copied down the quad-tree, but at
		every level, all pixels close to a differently classified pixel
		are reclassified to determine the border more precisely.

Options:        &-n n
                Quad-tree levels. Default = 4.

                &-m m
		Use histogram window of size (2*m)+1.
		Default = 0, implying search for stable value.

                &-mean
		Move histogram mass to local mean (default).

                &-median
		Move histogram mass to local median.

		-&

                Use either '-mean' or '-median' (or none), not both.
                If no '-m' option is given, a loop over the 'm'-values
                5, 10, 15, 20, 25, 30, 35, 40, 45 and 50 is performed,
                exiting if two consecutive runs give the same thresholding
                result.

See also:       segmSpannWilson(3), thresMlCentroid(1), segmRandom(1)

Diagnostics:    | Warning: Bad pixel type in band ..
                | Warning: Clustering never stable
                | Error  : Bad m value: ..
                | Error  : Bad number of arguments

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                Bands are handled one at a time, not multi-dimensionally.

Reference:      &M. Spann and R. Wilson,
                "A Quad-three Approach to Image Segmentation which
		Combines Statistical and Spatial Information"
		Pattern Recognition, Vol 18, Nos 3/4, pp 257-269, 1985

Author:         Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | segmSpannWilson -median mona.img monaseg.img
                | segmSpannWilson -mean -n 3 mona.img monaseg.img
                | segmSpannWilson -m 25 -verbose mona.img monaseg.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  int bn, nbands, m, n, method;
  char *args;

  Iset_message(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-m <m>] [-n <n>] [-mean] [-median] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  m      = read_iswitch(&argc, argv, "-m", 0);
  n      = read_iswitch(&argc, argv, "-n", 4);
  method = (read_bswitch(&argc, argv, "-mean")) ? 0 : 0; /* mean is default */
  method = (read_bswitch(&argc, argv, "-median")) ? 1 : 0;

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1 = Iread_image(argv[1]);
  img2 = Icopy_init(img1);
  nbands = Inbands(img1);
  for (bn=1; bn LE nbands; bn++) {
    if (segmSpannWilson(img1[bn], img2[bn], n, m, method))
      Warning(1, "Error in segmSpannWilson for band %d.\n", bn);
  }

  Ihistory(img2, argv[0], args);
  Iwrite_image(img2, argv[2]);

  return(0);
}
