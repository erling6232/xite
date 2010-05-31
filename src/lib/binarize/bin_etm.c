

/*

________________________________________________________________

        bin_etm.c
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

/*F:bin_etm*

________________________________________________________________

bin_etm
________________________________________________________________


Name:           bin_etm - binarize a gray-scale band using 
                Eikvil, Taxt and Moen`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_etm( int region_size, int step_size,
                |    double dist_limit, double weight,
                |    int glob_min, int glob_max, int bin_method,
                |    IBAND inband, IBAND outband );

Description:    The input band 'inband' is binarized using 
                the method of Eikvil, Taxt and Moen, the result returned
		in 'outband'.

		The pixels inside a small window 'S' of size
		('small_window_size' x 'small_window_size') are thresholded on
		basis of the clustering of the pixels inside a larger
		concentric window 'L' of size ('region_size' x 'region_size')
		'S' and 'L' slide across the image in steps equal to the side
		of 'S'. For all pixels inside 'L', a threshold 'T' is
		calculated to divide the pixels into two clusters. If the two
		estimated cluster means mu1 and mu2 are further apart than a
		limit, mu2 - mu1 > 'mean_diff_limit' the pixels inside 'S' are
		binarized using the threshold value 'T'. When mu2 - mu1 <
		'mean_diff_limit', the pixels inside 'S' are all assigned to
		the class with the closest updated mean value, being it print
		or background.

		'bin_method' is used to select the global binarization method
		which finds 'T'. Use 'bin_method' = OTSU or 'bin_method' =
		KITTLER_ILLINGWORTH.

		'weight' is used to calculate the updated mean values MU1 and
		MU2. For each small window 'S', the new values are caluculated
		as

		| MU1 = 'weight' * MU1 + (1.0-'weight') * mu1
		| MU2 = 'weight' * MU2 + (1.0-'weight') * mu2

		'weight' = 1.0 means that the initial estimates for MU1 and
		MU2 are kept forever, while 'weight' = 0.0 means that the mean
		values mu1 and mu2 of the two clusters in 'L' are used in the
		next step.
		
		For extremely difficult images, a preprocessing step can be
		used on the image. Replace all pixel values higher than the
		value 'glob_max' by ' glob_max' itself, and similarly, replace
		all pixel values lower than 'glob_min' by 'glob_min'.

Files:  	src/binarize/bin_etm.c
See also:       binarize(1), bin_ot(3), bin_ki(3)

References:	&'L. Eikvil, T. Taxt & K. Moen'
                "An Adaptive Method for Binarization of Grey Level Images",
		NOBIM National Conference on Image Processing and Pattern
		Recognition,
		pp 123-131, June 1991

Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "BinM.h"

int bin_etm (int region_size, int step_size, double dist_limit,
	     double weight, int glob_min, int glob_max, int bin_method,
	     IBAND inband, IBAND outband)
{
  int xsize, ysize;
  int X, x, y, u, v;
  int expand_depth;
  int threshold;
  double mean_backgr;
  double mean_forgr;
  double common_mean;
  double C_mean_backgr;
  double C_mean_forgr;
  
  IBAND tempband, inband_copy;
  IBAND big_window;

  if (step_size %2 != region_size%2)
    region_size++;

  expand_depth = (region_size - step_size)/2;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  if ( (MIN(xsize, ysize)) < expand_depth )
    {
      Error(2, "bin_etm: Value of 'expand_depth' too large. Max %i %s\n",
		MIN(xsize,ysize) , "for this image");
      exit(-1);
    }

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_etm: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_etm: Output pixel type must be unsigned byte.\n"));
  
  /* Make data structure for temporary image */
  inband_copy = Imake_band(Iu_byte_typ, xsize, ysize);
  tempband = Imake_band(Iu_byte_typ, xsize + 2*expand_depth, 
			ysize + 2*expand_depth);

  /* Preprocessing: pixels lighter than glob_max are set to glob_max, 
     and pixels darker than glob_min to glob_min. */

  Icopy_band(inband, inband_copy);

  if (glob_max < BIN_BACKGROUND)
    for (y=1; y<=ysize; y++)
      for (x=1; x<=xsize; x++)
	if (inband_copy[y][x] > glob_max)
	  inband_copy[y][x] = glob_max;

  if (glob_min > BIN_FORGROUND)
    for (y=1; y<=ysize; y++)
      for (x=1; x<=xsize; x++)
	if (inband_copy[y][x] < glob_min)
	  inband_copy[y][x] = glob_min;


  if (bin_method == KITTLER_ILLINGWORTH) {
    kittler_threshold (inband_copy, &C_mean_forgr, &C_mean_backgr,
		       &common_mean);
    if (Verbose()) printf("Global clustering done (Kitt./Ill.).\n");
  }
  else {
    otsu_threshold (inband_copy, &C_mean_forgr, &C_mean_backgr, &common_mean);
    if (Verbose()) printf("Global clustering done (Otsu).\n");
  }
  if (Verbose()) printf("Mean, backgr = %5.1f, forgr = %5.1f\n",
			C_mean_backgr, C_mean_forgr);

  (void) expand_edge(inband_copy, 0, 0, expand_depth,
			  tempband);


  if (Verbose()) printf("Image edge expanded\n");

  for (y = 1; y <= ysize; y+=step_size)
    {	
      if (y > (ysize+1-step_size))	  
	y = ysize+1-step_size;
      for (X = 1; X <= xsize; X+=step_size)
	{
	  if (X > (xsize+1-step_size))
	    X = xsize+1-step_size;
	  
	  /* x = X; */
	  x = ((y%2==1)? X : xsize+2-step_size-X);


	  big_window = Imake_subband(tempband, x, y, 
				     region_size, region_size);

	  if (bin_method == KITTLER_ILLINGWORTH)
	    threshold = kittler_threshold(big_window, &mean_forgr, 
					  &mean_backgr, &common_mean);
	  else
	    threshold = otsu_threshold(big_window, &mean_forgr, 
					 &mean_backgr, &common_mean);

	  if (fabs(mean_forgr - mean_backgr) > dist_limit)
	    {
	      for (u=0; u<step_size; u++)
		for (v=0; v<step_size; v++)
		  if (inband_copy[y+v][x+u] <= threshold)
		    outband[y+v][x+u] = 0;
		  else
		    outband[y+v][x+u] = 255;
	      C_mean_forgr = weight * C_mean_forgr + (1-weight) * mean_forgr;
	      C_mean_backgr= weight * C_mean_backgr+ (1-weight) * mean_backgr;
	    }
	  else if (fabs(common_mean - C_mean_backgr) 
		   < fabs (common_mean - C_mean_forgr))
	    {
	      /* Classify all in local window as background */
	      for (u=0; u<step_size; u++)
		for (v=0; v<step_size; v++)
		  outband[y+v][x+u] = 255;
	      C_mean_backgr= weight * C_mean_backgr+ (1-weight) * mean_backgr;
	    }
	  else
	    {
	      /* Classify all in local window as forground */
	      for (u=0; u<step_size; u++)
		for (v=0; v<step_size; v++)
		  outband[y+v][x+u] = 0;
	      C_mean_forgr = weight * C_mean_forgr + (1-weight) * mean_forgr;
	    }

	}
    }

  Idel_band(&tempband);

  return 0; /* Unix commands should return 0 */
}
