

/*C*

________________________________________________________________

        thresMlWaHa
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
#include XITE_STDIO_H
#include <xite/convolve.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/readarg.h>

/*P:thresMlWaHa*

________________________________________________________________

        thresMlWaHa
________________________________________________________________

Name:         thresMlWaHa - Finds multiple thresholds by Wang & Haralick method

Syntax:       | thresMlWaHa [-n <n>] [-nt <nt>] [-p <p>] [-et <et>] \\
              |       [-verbose] [-tri <trilevel-image>] \\
              |       [-edge <edge-image>] <inimage> <outimage>

Description:  The pixels in 'inimage' are first classified as either edge
              pixels or nonedge pixels. Edge pixels are then classified, on
              the basis of their neighborhoods, as being relatively dark,
              relatively bright or gray. A histogram of the graytone
              intensities is obtained for those pixels which are edge
              pixels and relatively dark, and another histogram is obtained
              for those which are edge pixels and relatively bright. Two
              candidate thresholds are found at the intensity values
              corresponding to peaks in the two histograms. The threshold
              giving the highest probability of getting pure dark or bright
              regions is selected. If the selected threshold corresponds to
              the peak in the dark edge histogram, the process is repeated
              for the graytone range above the threshold, if the threshold
              corresponds to the peak in the bright edge histogram, the
              process is repeated for the graytone range below the
              threshold.
                                
              You can specify the number of thresholds to be found, or a
              minimum population of the two histograms at which to stop.
              The population of the two histograms decreases rapidly, and
              might reach zero before the specified number of thresholds is
              found. There is no peak in a histogram with only zero entries
              and the process stops.
                                
Options:      &-n n
              An integer 'n' used to adjust the number of edge pixels
	      classified as either dark or bright. The number of pixels
	      classified as either dark or bright increases with
	      increasing 'n'. Default 'n' = 3.
              	       
              &-et et
	      Edge threshold 'et'. The sobel magnitude operator is used to find
	      the edge strenght in every pixel. The edge threshold value
	      'et' is then used to classify pixels as edge pixels or
	      nonedge pixels. Default 'et' = 100.
              	       
              &-nt nt
	      Number 'nt' of thresholds wanted.
              	       
              &-p p
	      The minimum population 'p' in dark or bright edge histograms
	      allowed before stopping. Only one of 'nt' and 'p' may be
	      specified. If none of them are specified, the default value
	      'p' = 100 is used as stop criteria.
                         
              &-verbose
	      Verbose. Will cause certain messages to be written to
	      "stdout". ie. the thresholds found, the poulation of the
	      dark and bright histograms and the graytone range, and the
	      total number of thresholds found.
              	       
              &-edge edge-image
	      Specifies that the thresholded sobel magnitude image
	      should be written to 'edge-image'.

	      &-tri trilevel-image
	      Specifies that the classified edge image should be
	      written to 'trilevel-image'. Edge pixels classified as dark
	      will be dark, edgepixels classified as bright will be bright,
	      and other pixels will be gray.
                
              The result written to 'outimage' will be a multithresholded
              version of 'inimage' with either 'nt' thresholds (if
              possible), or as many thresholds found with significant
              populations in both dark and bright histograms.

Reference:    &S. Wang and R. M. Haralick
              "Automatic Multithreshold Selection".
              Computer Vision, Graphics, and Image Processing,
	      25, 46-67 (1984).

See also:     thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
              thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
              thresMlCentroid(3), thresMlCentroid(3), mkComCur(3),
              thresMlComCur(3), thresMlComCur(1), thresMlReddi(3),
              thresMlReddi(3), thresMlWaHa(3), sobel(1)

Restrictions: 'inimage' must have bands with pixel type unsigned byte.

Return value: 0 => OK.
              1 => Bad number of arguments.
              2 => Can't open inimage.
              3 => Malloc error.
              4 => Error in thresMlWaHa function.
                                
Author:       Jørn Tilley Jacobsen, BLAB, Ifi, UiO

Examples:     | thresMlWaHa solflekk.img solSeg.img
              | thresMlWaHa -nt 5 -n 2 -et 50 -verbose -edge solE.img \\
	      |     -tri solT.img solflekk.img solSeg.img

Id:           $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE     grayImg, edgeImg, triImg;
  histogram thVect, grayVect;
  int       numTh, n, edgeTh, population, totTh;
  int       step, grayTone, x, g, bn, noBands;
  char      *triName, *edgeName, *title, *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
  "Usage: %s [-n <n>] [-nt <nt>] [-et <et>] [-title <title>]\n\
    [-p <p>] [-verbose] [-edge <edgeImg>] \n\
    [-tri <triImg>] <grayImg> <segImg>\n"));

  if ( argc == 1 ) Usage(1, NULL);
  args = argvOptions(argc, argv);

  n           = read_iswitch( &argc, argv, "-n",    3);
  edgeTh      = read_iswitch( &argc, argv, "-et", 128);
  numTh       = read_iswitch( &argc, argv, "-nt",    0);
  title	      = read_switch(  &argc, argv, "-title", 1, NULL );
  population  = read_iswitch( &argc, argv, "-p", numTh? 0:100);
  edgeName    = read_switch(  &argc, argv, "-edge", 1, NULL );
  triName     = read_switch(  &argc, argv, "-tri",  1, NULL );


  if ( argc != 3 ) Usage(1, "Illegal number of arguments.\n");

  grayImg = Iread_image( argv[1] );
  if (! grayImg ) Error(2, " couldn't open %s\n", argv[1]);
  noBands = Inbands(grayImg);
  if (title) Iset_title(grayImg, title);
  if ( edgeName ) {
    edgeImg = Icopy_init(grayImg);
     if (! edgeImg ) Error(3, " malloc failed \n");
  }
  else edgeImg = NULL;

  if ( triName ) {
    triImg = Icopy_init(grayImg);
     if (! triImg ) Error(3, " malloc failed \n");
  } else triImg = NULL;

  for ( bn = 1; bn <= noBands; bn++) {
    if (thresMlWaHa(grayImg[bn], (triImg?triImg[bn]:NULL),
		    (edgeImg?edgeImg[bn]:NULL), thVect, edgeTh, numTh,
		    population, n, &totTh))
      Warning(2, "Error in thresMlWaHa.\n");

     if (Verbose()) {
       printf("\n%s found %2d thresholds in band %2d of %s :\n",
	   argv[0], totTh, bn, argv[1]);
	for (x=0; x < totTh; x++) printf( " %d ", thVect[x]);
	printf( "\n");
     }
     grayTone = 0;
     step = 255 / totTh;
     for ( x=0, g=0; g < 256; g++ ) {
       grayVect[g] = grayTone;
	if (g == thVect[x]) {
	  if (x == totTh) x=0; else x++; grayTone += step;
	}
     }
     if (histoTransf(grayImg[bn], grayImg[bn], grayVect))
       Warning(2, "Error in histoTransf.\n");
  }

  Ihistory(grayImg, argv[0], args);
  Iwrite_image(grayImg, argv[2] );
  if ( triImg) Iwrite_image( triImg, triName );
  if ( edgeImg) Iwrite_image( edgeImg, edgeName );

  return(0);
}
