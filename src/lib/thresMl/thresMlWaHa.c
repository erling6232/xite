

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

#define DARK        0
#define GRAY        128
#define BRIGHT      255



static int makeTriImage (IBAND gray, IBAND edge, IBAND min, IBAND max, IBAND tri, int n)
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



static int splitHisto (IBAND gray, IBAND tri, IBAND min, IBAND max, int *Hd, int *Hb, int gmin, int gmax, int *popD, int *popB)
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



static int findThres (int *gmin, int *gmax, int *Hd, int *Hb)
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
int thresMlWaHa (IBAND gray, IBAND tri, IBAND edge, int *thVect, int edgeTh, int numTh, int population, int n, int *totTh)
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
