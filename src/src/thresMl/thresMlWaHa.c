

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

#ifndef MAIN

#define DARK        0
#define GRAY        128
#define BRIGHT      255



#ifndef FUNCPROTO
static int makeTriImage ( gray , edge, min , max , tri, n )
IBAND   gray, edge, min, max, tri;
int     n;
#else /* FUNCPROTO */
static int makeTriImage (IBAND gray, IBAND edge, IBAND min, IBAND max, IBAND tri, int n)
#endif /* FUNCPROTO */
{
  int     x, y;
  int     xsize, ysize, pixval;
  double  my, delta;

  xsize = Ixsize( gray ); ysize = Iysize( gray );
  
  for ( x=1 ; x <= xsize ; x++ )
    for ( y=1 ; y <= ysize ; y++ ) {
      if ( edge[y][x] != DARK ) {
	my = ( min[y][x] + max[y][x] ) / 2.0;
	delta = ( max[y][x] - min[y][x] ) / ( 2.0 * n );
	pixval= gray[y][x];
	/* Classifying edgepixels as either DARK, BRIGHT or GRAY    */
	if ( (pixval >= min[y][x]) && (pixval <= ( my - delta )) )
	  tri[y][x] = DARK;
	else if ( (pixval >= ( my + delta )) && (pixval <= max[y][x]) )
	  tri[y][x] = BRIGHT;
	else
	  tri[y][x] = GRAY;
      }   
      else
	tri[y][x] = GRAY;                    /* Not edge pixel       */
    }
  return(0);
}



#ifndef FUNCPROTO
static int splitHisto ( gray, tri, min, max, Hd, Hb, gmin, gmax, popD, popB )
IBAND   gray, tri, min, max;
int     Hd[], Hb[], gmin, gmax, *popD, *popB;
#else /* FUNCPROTO */
static int splitHisto (IBAND gray, IBAND tri, IBAND min, IBAND max, int *Hd, int *Hb, int gmin, int gmax, int *popD, int *popB)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;

  for (x = 0; x <= 255; x++ ) Hd[x] = Hb[x] = 0;
  *popD = *popB = 0;
  xsize = Ixsize(gray); ysize = Iysize(gray);

  /* Generating graytone histograms over dark/bright edgepixels           */  
  for ( x = 1 ; x <= xsize ; x++ )
    for ( y = 1 ; y <= ysize; y++ )
      if ( tri[y][x] == DARK ) {
	if ( min[y][x]>= gmin && max[y][x] <= gmax )
	  { Hd[ gray[y][x] ]++; (*popD)++; }
      }
      else if ( tri[y][x] == BRIGHT ) {
	if ( min[y][x]>= gmin && max[y][x] <= gmax )
	  { Hb[ gray[y][x] ]++; (*popB)++; }
      }
  return(0);
}



#ifndef FUNCPROTO
static int findThres ( gmin, gmax, Hd, Hb )
int     *gmin, *gmax, Hd[], Hb[];
#else /* FUNCPROTO */
static int findThres (int *gmin, int *gmax, int *Hd, int *Hb)
#endif /* FUNCPROTO */
{
  int     i, Td, Tb;
  double  HdTdSum, HdTbSum, HbTdSum, HbTbSum, Pd, Pb;

  HdTdSum = HdTbSum = HbTdSum = HbTbSum = Pd = Pb = 0.0;

  /* Finding thresholds where dark/bright histograms are maximum          */
  Td = *gmin ; Tb = *gmax ;
  for ( i = *gmin ; i <= *gmax ; i++ ) {
    if ( Hd[ i ] >  Hd[ Td ] ) Td = i;
    if ( Hb[ i ] >= Hb[ Tb ] ) Tb = i;
  }

  /* Finding total number of pixels in dark/bright histograms in graytone */
  /* interval gmin to Td ( selected threshold from dark edge pixels )     */
  for ( i = *gmin ; i <= Td ; i++ ) {
    HdTdSum += Hd[ i ]; HbTdSum += Hb[ i ];
  }

  /* Finding total number of pixels in dark/bright histograms in graytone */
  /* interval Tb to gmax ( selected threshold from bright edge pixels )   */
  for ( i = Tb ; i <= *gmax ; i++ ) {
    HdTbSum += Hd[ i ]; HbTbSum += Hb[ i ];
  }

  /* Finding the probability of getting purely bright/dark regions of     */
  /* both thresholds                                                      */
  Pd = HdTdSum / ( HdTdSum + HbTdSum );
  Pb = HbTbSum / ( HbTbSum + HdTbSum );

  /* Selecting the threshold which gives purest regions.                  */
  if (Pd >= Pb) {
    *gmin = Td + 1; return(Td);
  } else {
    *gmax = Tb - 1; return(Tb);
  }
}



/*F:thresMlWaHa*

________________________________________________________________

        thresMlWaHa
________________________________________________________________

Name:           thresMlWaHa - Finds multiple thresholds by Wang & Haralick
                method.

Syntax:         | #include <xite/thresMl.h>
		|
		| int thresMlWaHa( IBAND gray, IBAND tri,
		|    IBAND edge, int thVect[], int edgeTh,
		|    int numTh, int population, int n,
		|    int* totTh );

Description:    The pixels in 'gray' are first classified as either edge
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

Parameters:     &gray
                The inband to be thresholded.
	      
		&tri
		A band to hold the tri-tone edge band. Can be NULL.

		&edge
		A band to hold the sobel magnitude edge band. Can be NULL.

		&thVect
		A 256 element int vector returning the threshold values in
		increasing order. The number of thresholds found are returned
		through the argument 'totTh'.
	      
		&edgeTh
		The threshold-value at wich to threshold the 'edge' band.

	        &numTh
		The number of thresholds wanted.

		&pop
		The minimum population allowed for the dark/bright edge
		histograms. One and only one of 'numTh' and 'pop' must be
		specified, the other one should be zero.

		&n
		An integer used to adjust the number of edge pixels classified
		as either dark or bright. The number of pixels classified as
		either dark or bright increases with increasing 'n'. Must be
		positive and non-zero, usually 'n' = 3.
	      
		&totTh
		The number of thresholds found.

Reference:      &S. Wang and R. M. Haralick
                "Automatic Multithreshold Selection".
	        Computer Vision, Graphics, and Image Processing,
		25, 46-67 (1984).

Restrictions:   'gray', 'tri' and 'edge' must have pixel type unsigned byte.

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(3), thresMlCentroid(3), mkComCur(3),
                thresMlComCur(3), thresMlComCur(1), thresMlReddi(3),
                thresMlReddi(3), thresMlWaHa(1), sobel(3)

Return value: | Status value:
              |    0 => ok
	      |    1 => None or both "numTh" and "pop" specified.
	      |    2 => Wrong pixeltype in "gray".
	      |    3 => Wrong pixeltype in "tri".
	      |    4 => Wrong pixeltype in "edge".
	      |    5 => Malloc error. Not able to create temporary image.
	      |    6 => "tri" size different from "gray" size.
	      |    7 => "edge" size different from "gray" size.
	      |    8 => "n" < 1.
	      |    9 => No thresholds found.
				
Author:       Jørn Tilley Jacobsen, BLAB, Ifi, UiO

Id:           $Id$
________________________________________________________________

*/
#ifndef FUNCPROTO
int thresMlWaHa (gray,tri,edge,thVect,edgeTh,numTh,population,n, totTh)
IBAND   gray, tri, edge;
int     thVect[], edgeTh, numTh, population, n, *totTh;
#else /* FUNCPROTO */
int thresMlWaHa (IBAND gray, IBAND tri, IBAND edge, int *thVect, int edgeTh, int numTh, int population, int n, int *totTh)
#endif /* FUNCPROTO */
{
  IBAND   minBand, maxBand;
  int     x, y, finished, xsize, ysize;
  int     Hd[256], Hb[256], gmin, gmax, popD, popB;

  xsize = Ixsize(gray); ysize = Iysize(gray);
  
  /* Error checking and image initialization                          */
  if ((population < 1 && numTh < 1) ||
      (population > 0 && numTh > 0))
    return(Error(1, "thresMlWaHa: None or both numTh and pop specified\n"));
  if (n < 1) return(Error(8, "thresMlWaHa: argument n < 1.\n"));
  if (Ipixtyp(gray) != Iu_byte_typ)
    return(Error(2, "thresMlWaHa: gray pixel type must be unsigned byte.\n"));
  
  if (tri) {
    if (Ipixtyp(tri) != Iu_byte_typ)
      return(Error(3, "thresMlWaHa: tri pixel type must be unsigned byte.\n"));
    if ((xsize != Ixsize(tri)) || (ysize != Iysize(tri)))
      return(Error(6, "thresMlWaHa: tri size different from gray size\n"));
  }
  else {
    tri = Imake_band(Iu_byte_typ, xsize, ysize);
    if (! tri) return(Error(5, "thresMlWaHa: Malloc error\n"));
  }
  
  if (edge) {   
    if (Ipixtyp(edge) != Iu_byte_typ)
      return(Error(4,
		   "thresMlWaHa: edge pixel type must be unsigned byte.\n"));
    if ((xsize != Ixsize(edge)) || (ysize != Iysize(edge)))
      return(Error(7, "thresMlWaHa: edge size different from gray size\n"));
  }
  else {
    edge = Imake_band(Iu_byte_typ, xsize, ysize);
    if (! edge ) return(Error(5, "thresMlWaHa: Malloc error\n"));
  }
  
  minBand = Imake_band(Iu_byte_typ,xsize,ysize);
  if (! minBand ) return(Error(5, "thresMlWaHa: Malloc error\n"));
  
  maxBand  = Imake_band(Iu_byte_typ,xsize,ysize);
  if (! maxBand ) return(Error(5, "thresMlWaHa: Malloc error\n"));
  
  /* Making edgeimage by applying a sobel-operator returning a magnitude  */
  /* image of <gray> in <edge>. To consider real edges only, the edge-    */
  /* image is thresholded at <edgeTh>                                     */
  sobel( gray, edge, NULL, NULL, Verbose() );
  /* threshold( edge, edge, edgeTh); */
  xsize = Ixsize(edge); ysize = Iysize(edge);
  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      if (edge[y][x] > edgeTh) edge[y][x] = BRIGHT;
      else edge[y][x] = DARK;
  
  
  /* Finding min and max in each 8-neighbour by minarea and maxarea       */
  minarea( gray, minBand, 3, 3);
  maxarea( gray, maxBand, 3, 3);
  
  /* Classifying edgepixels as either DARK, GRAY or BRIGHT                */
  makeTriImage( gray, edge, minBand, maxBand, tri, n );
  
  /* Iteratively finding thresholds in graytone interval gmin-gmax,       */
  /* starting with entire graytone range.                                 */
  gmin = 0; gmax = 255; finished = 0; *totTh = 0;
  splitHisto(gray,tri,minBand,maxBand,Hd,Hb,gmin,gmax,&popD,&popB);
  
  /* Look for thresholds as long as significant populations OR until      */
  /* <numTh> thresholds are found, stop if impossible to find more.       */
  while ( ! finished ) {
    if (population)
      finished = ! ( popD >= population && 
		    popB >= population && gmax > gmin );
    else
      finished = ! ( numTh > (*totTh) && gmax > gmin && 
		    popD > 0 && popB > 0);
    if (! finished) {
      thVect[ (*totTh)++ ] = findThres( &gmin, &gmax, Hd, Hb);
      if (Verbose()) {
	printf("Threshold at %3d",thVect[(*totTh) - 1] );
	printf(", dark population %5d",popD);
	printf(", bright population %5d\n",popB);
	printf("Remaining graytone range %d-%d\n",gmin,gmax);
      }
      splitHisto(gray,tri,minBand,maxBand,Hd,Hb,gmin,gmax,&popD,&popB);
    }
  }
  
  if ( !totTh) return(Error(9, "thresMlWaHa: No thresholds found\n"));
  
  /* Shell-sorting threshold vector                                       */
  {
    int i,j,gap,temp;
    for (gap = (*totTh) / 2; gap > 0 ; gap /= 2)
      for (i = gap; i < (*totTh); i++ )
	for (j = i - gap; j >= 0 && thVect[j] > thVect[j+gap]; j -= gap)
	  {
	    temp          = thVect[j];
	    thVect[j]     = thVect[j+gap];
	    thVect[j+gap] = temp;
	  }
  }
  return(0);
}

#endif /* not MAIN */



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

#ifdef MAIN

#ifndef FUNCPROTO
int main( argc, argv )
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
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

#endif /* MAIN */
