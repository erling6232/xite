

/*

________________________________________________________________

        bin_nr.c
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

/*F:bin_nr*

________________________________________________________________

                bin_nr
________________________________________________________________


Name:           bin_nr - binarize a gray-scale band using 
                Nakagawa and Rosenfeld`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_nr( double sdev_limit,
                |    double mean_limit, double min_ratio,
                |    double max_ratio, double valley_to_peak,
                |    int region_size, IBAND inband,
                |    IBAND outband );

Description:    The input band 'inband' is binarized using the method of
                Nakagawa and Rosenfeld. The method is a slight modification
		of Chow and Kaneko`s method. The result is returned in
		'outband'.

		This method calculates a thresholding surface. The image is
		divided into windows of size ('region_size' x 'region_size'),
		and the histograms for each window are tested for bimodality.
		Each histogram is approximated by a mixture of two Gaussian
		distributions. Let mu1 and mu2 be the estimated mean values,
		and sigma1 and sigma2 be the estimated standard deviations of
		the two distributions. Also, let sigma_tot be the total
		standard deviation, and vtp be the valley-to-peak ratio. The
		bimodality test requires the following five criteria to hold:

		(1) sigma_tot > 'sdev_limit'

		(2) mu2 - mu1 > 'mean_limit'

		(3) sigma1/sigma2 > 'min_ratio'

		(4) sigma1/sigma2 > 'min_ratio'

		(5) vtp < 'valley_to_peak_limit'

		For each bimodal window, a threshold is calculated based on
		the mu_1, mu_2, sigma_1 and sigma_2 parameters of the mixture
		distribution.

		Thresholds for the other windows are calculated on basis of
		the thresholds for the bimodal windows. The thresholds are
		first smoothed, and then interpolated to give a threshold
		surface, having a single threshold value for each image pixel.
		The image is binarized using the threshold surface.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1)

References:     &[1] 'C.K. Chow & T. Kaneko'
                "Automatic detection of the left ventricle from
		cineangiograms",
		Computers and biomedical research, vol. 5, pp 388-410, 1972.

		&[2] 'Yasuo Nakagawa & Azriel Rosenfeld'
		"Some experiments on variable thresholding",
		Pattern Recognition, vol 11, no 3, pp 191-204, 1979.
		
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Files:  	src/binarize/bin_nr.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "BinM.h"
#include "EM.c"
#include "k_means.c"

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif

#define NR_GAUSS 1/sqrt(2*M_PI)
#define NR_SMOOTH
#define NR_SQRT_05 0.70710678

#ifdef FUNCPROTO
static int nr_mean_dev(IBAND inband, IR_BAND reg_sdev, IR_BAND reg_mean)
#else
static int nr_mean_dev(inband, reg_sdev, reg_mean)
     IBAND inband;
     IR_BAND reg_sdev;
     IR_BAND reg_mean;
#endif
{
  int x, y, rx, ry;
  int xstart, ystart;
  int xend, yend;
  int xsize, ysize;
  int nof_reg_x, nof_reg_y;
  int n;
  float sum;
  float sum2;
  float mean;
  float variance;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  nof_reg_x = Ixsize((IBAND)reg_sdev);
  nof_reg_y = Iysize((IBAND)reg_sdev);

  xend = 0;
 
  for (rx=1; rx<=nof_reg_x; rx++)
    {
      xstart = xend+1;
      xend = (xsize * rx) / nof_reg_x;
      yend = 0;	
      for (ry=1; ry<=nof_reg_y; ry++)
	{
	  ystart = yend+1;
	  yend = (ysize * ry) / nof_reg_y;
	  n = 0;
	  sum = 0;
	  sum2 = 0;
	  for (x=xstart; x<=xend; x++)
	    for (y=ystart; y<=yend; y++)
	      {
		n++;
		sum+= inband[y][x];
		sum2+= inband[y][x]*inband[y][x];
	      }
	  mean = sum * 1.0/n;
	  reg_mean[ry][rx] = mean;
	  variance = (sum2 * 1.0/n) - mean*mean;	
	  reg_sdev[ry][rx] = sqrt(variance);
	}
    }
  return 1;
}



/* Weight matrix for 3x3 weighted mean smoothing, used by nr_interior_smooth
   for Nakagawa and Rosenfelds's method.

         0.7 1.0 0.7
         1.0 2.0 1.0
         0.7 1.0 0.7

   0.7 is replaced by 1/sqrt(2). 
*/


#ifdef FUNCPROTO
static float nr_interior_smooth(int center, 
			 int edge1, int edge2, int edge3, int edge4,
			 int corner1, int corner2, 
			 int corner3, int corner4)
#else
static float nr_interior_smooth(center, 
			 edge1, edge2, edge3, edge4,
			 corner1, corner2, 
			 corner3, corner4)
     int center;
     int edge1;
     int edge2;
     int edge3;
     int edge4;
     int corner1;
     int corner2;
     int corner3;
     int corner4;
#endif
{
  return (2.0 * center + (edge1 + edge2+ edge3 + edge4)
	  + 0.7 * (corner1 + corner2 + corner3 + corner4)) / 8.8;
}


    
/* Reduced weight matrix for 3x3 weighted mean smoothing, 
   used by nr_corner_smooth:

             2.0 1.0
             1.0 0.7
*/

#ifdef FUNCPROTO
static float nr_corner_smooth(int center, int edge1, int edge2, int corner)
#else
static float nr_corner_smooth(center, edge1, edge2, corner)
     int center;
     int edge1;
     int edge2;
     int corner;
#endif
{
  return (2.0 * center + (edge1 + edge2) + 0.7 * corner) / 4.7;
}



/* Reduced weight matrix for 3x3 weighted mean smoothing, 
   used by nr_edge_smooth:

             1.0 0.7
             2.0 1.0
             1.0 0.7
*/

#ifdef FUNCPROTO
static float nr_edge_smooth(int center, 
		     int edge1, int edge2, int edge3, 
		     int corner1, int corner2)
#else
static float nr_edge_smooth(center, 
		     edge1, edge2, edge3, 
		     corner1, corner2)
     int center;
     int edge1;
     int edge2;
     int edge3;
     int corner1;
     int corner2;
#endif
{
  return (2.0 * center + (edge1 + edge2+ edge3)
	  + 0.7 * (corner1 + corner2)         ) / 6.4;
}



#ifdef FUNCPROTO
static int nr_smooth(IBAND inband, IBAND outband)
#else
static int nr_smooth(inband, outband)
     IBAND inband;
     IBAND outband;
#endif
{
  int x, y, xsize, ysize;
  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  
  /* Maa lage float-versjon av expand-edge! */

  outband[1][1] = (int)nr_corner_smooth(inband[1][1], 
					inband[1][2], inband[2][1], 
					inband[2][2]);
  for (x=2; x<=xsize-1; x++)
    outband[1][x] = (int)nr_edge_smooth(inband[1][x], 
					inband[1][x-1], inband[1][x+1], 
					inband[2][x],
					inband[2][x-1], inband[2][x+1]);
  outband[1][xsize] = (int)nr_corner_smooth(inband[1][xsize], 
					    inband[1][xsize-1], 
					    inband[2][xsize], 
					    inband[2][xsize-1]);
  for (y=2; y<=ysize-1; y++)
    {
      outband[y][1] 
	= (int)nr_edge_smooth(inband[y][1], 
			      inband[y-1][1], inband[y+1][1], inband[y][2],
			      inband[y-1][2], inband[y+1][2]);

      for (x=2; x<=xsize-1; x++)
	outband[y][x] 
	  = (int)nr_interior_smooth(inband[y][x],
				    inband[y-1][x], inband[y][x-1],
				    inband[y][x+1], inband[y+1][x],
				    inband[y-1][x-1], inband[y-1][x+1],
				    inband[y+1][x-1], inband[y+1][x+1]);

      outband[y][xsize] 
	= (int)nr_edge_smooth(inband[y][xsize], 
			      inband[y-1][xsize], inband[y+1][xsize], 
			      inband[y][xsize-1],
			      inband[y-1][xsize-1],inband[y+1][xsize-1]);
    }
  outband[ysize][1] 
    = (int)nr_corner_smooth(inband[ysize][1], 
			    inband[ysize][2], inband[ysize-1][1], 
			    inband[ysize-1][2]);
  for (x=2; x<=xsize-1; x++)
    outband[ysize][x] 
      = (int)nr_edge_smooth(inband[ysize][x], 
			    inband[ysize][x-1], inband[ysize][x+1], 
			    inband[ysize-1][x],
			    inband[ysize-1][x-1], inband[ysize-1][x+1]);
  outband[ysize][xsize] 
    = (int)nr_corner_smooth(inband[ysize][xsize], 
			    inband[ysize][xsize-1], inband[ysize-1][xsize], 
			    inband[ysize-1][xsize-1]);

  return 1;
}

#ifdef FUNCPROTO
static int nr_interpolate(IBAND inband, IBAND outband)
#else
static int nr_interpolate(inband, outband)
     IBAND inband;
     IBAND outband;
#endif
{
  int x, y, rx, ry;
  int xstart, ystart;
  int xend, yend;
  int xsize, ysize;
  int nof_reg_x, nof_reg_y;
  

  xsize     = Ixsize(outband);
  ysize     = Iysize(outband);
  nof_reg_x = Ixsize(inband);
  nof_reg_y = Iysize(inband);
  xend      = (xsize) / nof_reg_x / 2 + 1;

  if (Verbose()) printf("   Interior pixels\n");
  for (rx=1; rx<nof_reg_x; rx++)
    {
      xstart = xend;
      xend = (xsize * (2*rx+1)) / nof_reg_x / 2 + 1;
      yend = (ysize) / nof_reg_y / 2 + 1;
      for (ry=1; ry<nof_reg_y; ry++)
	{
	  ystart = yend;
	  yend = (ysize * (2*ry+1)) / nof_reg_y / 2 + 1;
	  for (x=xstart; x<=xend; x++)
	    {
	      outband[ystart][x] 
		= (  (xend-x)   * inband[ry][rx] 
		   + (x-xstart) * inband[ry][rx+1]  ) / (xend-xstart);
	      outband[yend][x] 
		= (  (xend-x)   * inband[ry+1][rx] 
		   + (x-xstart) * inband[ry+1][rx+1]  ) / (xend-xstart);
	      for (y=ystart+1; y<=yend-1; y++)
		{	
		  outband[y][x]
		    = (  (yend-y)   * outband[ystart][x]
		       + (y-ystart) * outband[yend][x]  ) / (yend-ystart);
		}

	    }
	}
    }

  if (Verbose()) printf("   Edge pixels\n");
  xstart = (xsize                  ) / nof_reg_x / 2 + 1;
  ystart = (ysize                  ) / nof_reg_y / 2 + 1;
  xend   = (xsize * (2*nof_reg_x-1)) / nof_reg_x / 2 + 1;
  yend   = (ysize * (2*nof_reg_y-1)) / nof_reg_y / 2 + 1;
  for (x=xstart; x<=xend; x++)
    {
      for (y=1; y<ystart; y++)
	outband[y][x] = 2*outband[ystart][x] - outband[2*ystart-y][x];
      for (y=yend+1; y<=ysize; y++)
	outband[y][x] = 2*outband[yend][x] - outband[2*yend-y][x];
    }
  for (y=ystart; y<=yend; y++)
    {
      for (x=1; x<xstart; x++)
	outband[y][x] = 2*outband[y][xstart] - outband[y][2*xstart-x];
      for (x=xend+1; x<=xsize; x++)
	outband[y][x] = 2*outband[y][xend] - outband[y][2*xend-x];
    }
  outband[1][1] 
    = (  ystart * (2*outband[1][xstart] - outband[1][2*xstart-1])
       + xstart * (2*outband[ystart][1] - outband[2*ystart-1][1]))
      /(xstart+ystart); 
  outband[1][xsize] 
    = (  ystart       * (2*outband[1][xend]      -outband[1][2*xend-xsize])
       + (xsize-xend) * (2*outband[ystart][xsize]-outband[2*ystart-1][xsize]))
      /((xsize-xend)+ystart); 
  outband[ysize][1] 
    = ( (ysize-yend)* (2*outband[ysize][xstart]- outband[ysize][2*xstart-1])
       + xstart     * (2*outband[yend][1]      - outband[2*yend-ysize][1]))
      /(xstart+(ysize-yend)); 
  outband[ysize][xsize] 
    = ( (ysize-yend)* (2*outband[ysize][xend] - outband[ysize][2*xend-xsize])
       +(xsize-xend)* (2*outband[yend][xsize] - outband[2*yend-ysize][xsize]))
      /((xsize-xend)+(ysize-yend)); 

  if (Verbose()) printf("   Corner pixels\n");
  for (x=1; x<xstart; x++)
    {
      outband[1][x] 
	= (  (xstart-x) * outband[1][1]
	   + (x-1)      * outband[1][xstart]) / (xstart-1);
      outband[ysize][x] 
	= (  (xstart-x) * outband[ysize][1]
	   + (x-1)      * outband[ysize][xstart]) / (xstart-1);
      for (y=2; y<ystart; y++)
	outband[y][x] 
	  = (  (ystart-y) * outband[1][x]
	     + (y-1)      * outband[ystart][x]) / (ystart-1);
      for (y=yend+1; y<ysize; y++)
	outband[y][x] 
	  = (  (ysize-y)  * outband[yend][x]
	     + (y-yend)   * outband[ysize][x]) / (ysize-yend);
    }

  for (x=xend+1; x<=xsize; x++)
    {
      outband[1][x] 
	= (  (x-xend)    * outband[1][xsize]
	   + (xsize-x)   * outband[1][xend]) / (xsize-xend);
      outband[ysize][x] 
	= (  (x-xend)    * outband[ysize][xsize]
	   + (xsize-x)   * outband[ysize][xend]) / (xsize-xend);
      for (y=2; y<ystart; y++)
	outband[y][x] 
	  = (  (ystart-y) * outband[1][x]
	     + (y-1)      * outband[ystart][x]) / (ystart-1);
      for (y=yend+1; y<ysize; y++)
	outband[y][x] 
	  = (  (ysize-y)  * outband[yend][x]
	     + (y-yend)   * outband[ysize][x]) / (ysize-yend);
    }

  return 1;
}

/*
#ifdef FUNCPROTO
static int printf_histo(histogram H)
#else
static int printf_histo(H)
     histogram H;
#endif
{
  int i,j;

  for (i=0; i<=255; i+=8)
    {
      printf ("hist[%3i-%3i]:", i, i+7);
      for (j=0; j<8; j++)
	{
	  printf ("%4i", H[i+j]);
	}
      printf ("\n");
      
    }
  return 1;
}

#ifdef FUNCPROTO
static int nr_smooth_histo(histogram F, histogram F_new)
#else
static int nr_smooth_histo(F, F_new)
     histogram F;
     histogram F_new;
#endif
{
  int i;
  F_new[0] = (3*F[0] + 2*F[1] + F[2] + 3) / 6;
  F_new[1] = (2*F[0] + 3*F[1] + 2*F[2] + F[3] + 4) / 8;
  for (i=2; i<=253; i++)
    F_new[i] = (F[i-2] + 2*F[i-1] + 3*F[i] + 2*F[i+1] + F[i+2] + 4)/9;
  F_new[254] = (F[252] + 2*F[253] + 3*F[254] + 2*F[255] + 4) / 8;
  F_new[255] = (F[253] + 2*F[254] + 3*F[255] + 3) / 6;
  return 1;
}



#ifdef FUNCPROTO
static int nr_histo_valley(histogram H)
#else
static int nr_histo_valley(H)
     histogram H;
#endif
{
  int valley;
  int i, i_low, i_high;

  i=0;
  while((H[i] <= H[i+1]) && (i < 254))
    i++;
  i_low = i;
  i = 255;
  while((H[i] <= H[i-1]) && (i > 1))
    i--;
  i_high = i;
  if (Verbose()) printf("i_low = %i, i_high = %i\n", i_low, i_high);
  valley = i_low;
  for (i=i_low; i<=i_high; i++)
    if (H[i] < H[valley])
      {
	valley = i;
	if (Verbose()) printf("New valley = %i\n", valley);
      }
  return valley;
}
  
*/

#ifdef FUNCPROTO
static int nr_interpot_reg(IBAND inband, IBAND outband)
#else
static int nr_interpot_reg(inband, outband)
     IBAND inband;
     IBAND outband;
#endif
{  IBAND tempband;
  int xsize, ysize;
  int x, y, i, j;
  int not_finished = TRUE;
  double weight1, weight2;


  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  tempband = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(inband, 1, 0, 1, tempband);

  while (not_finished)
    {
      not_finished = FALSE;
      for (x=1; x<=xsize; x++)
	{
	  i = x+1;
	  for (y=1; y<=ysize; y++)
	    {
	      j = y+1;
	      outband[y][x] = tempband[j][i];
	      if (tempband[j][i] == 0)
		{
		  /* Diagonal neighbours */
		  /*
		  weight1 = (  SIGN(tempband[j-1][i-1]) 
			     + SIGN(tempband[j-1][i+1]) 
			     + SIGN(tempband[j+1][i-1]) 
			     + SIGN(tempband[j+1][i+1])) *1.0/sqrt(2);
		  		   */
		  weight1 = 0.0;
		  if (tempband[j-1][i-1] >= 1)
		      weight1 += 1.0;
		  if (tempband[j-1][i+1] >= 1)
		      weight1 += 1.0;
		  if (tempband[j+1][i-1] >= 1)
		      weight1 += 1.0;
		  if (tempband[j+1][i+1] >= 1)
		      weight1 += 1.0;
		  weight1 = weight1 / sqrt(2.0);

		  /* 4 closest neighbours */
		  /*
		  weight2 = (  SIGN(tempband[j][i-1])
			     + SIGN(tempband[j][i+1])
			     + SIGN(tempband[j-1][i])
			     + SIGN(tempband[j+1][i]))	;
			     */

		  weight2 = 0.0;
		  if (tempband[j][i-1] >= 1)
		      weight2 += 1.0;
		  if (tempband[j][i+1] >= 1)
		      weight2 += 1.0;
		  if (tempband[j-1][i] >= 1)
		      weight2 += 1.0;
		  if (tempband[j+1][i] >= 1)
		      weight2 += 1.0;

		  if (weight2 > 0)
		    {
		      outband[y][x] 
			= (int)( ((  tempband[j-1][i-1] 
				   + tempband[j-1][i+1] 
				   + tempband[j+1][i-1] 
				   + tempband[j+1][i+1]) *1.0/sqrt(2.0))
				+((  tempband[j]  [i-1]
				   + tempband[j]  [i+1]
				   + tempband[j-1][i  ]
				   + tempband[j+1][i  ]) *1.0)
				) / (weight1 + weight2);
		    }
		  else
		    not_finished = TRUE;
		}
	    }
	}
      for (x=1; x<=xsize; x++)
	for (y=1; y<=ysize; y++)
	  tempband[y+1][x+1] = outband[y][x];
    }
  return 1;
}



#ifdef FUNCPROTO
static float nr_threshold (double p1, double p2, double m1, double m2,
		    double s1, double s2, double estimate)
#else
static float nr_threshold (p1, p2, m1, m2,
		    s1, s2, estimate)
     double p1;
     double p2;
     double m1;
     double m2;
     double s1;
     double s2;
     double estimate;
#endif
{
  double a, b, c;
  double r, t1, t2;
  
  a = (1/(s1*s1) - 1/(s2*s2));
  b = 2*( m2/(s2*s2) - m1/(s1*s1) );
  c = (m1*m1)/(s1*s1) - (m2*m2)/(s2*s2) + 2*log((p2*s1)/(p1*s2));

  r = b*b-4*a*c;
  if (r<0.0)
    {
      Warning(1, "%s%f\n",
	    "Error in nr_threshold. Negative root! R = ", r);
      return 0.0;
    }
  t1 = (-b+sqrt(r)) / (2*a);
  t2 = (-b-sqrt(r)) / (2*a);
  if ((t1<=0.0) && (t2<=0.0))
    {
      Warning(1, "%s\n%s%f%s%f\n",
	    "Error in nr_threshold. Negative thresholds!",
	    "T1 = ", t1, " ,  T2 = ", t2);
      return 0.0;
    }
  if ((t1>=255.0) && (t2>=255.0))
    {
      Warning(1, "%s\n%s%f%s%f\n",
	    "Error in nr_threshold. Too large thresholds!",
	    "T1 = ", t1, " ,  T2 = ", t2);
      return 0.0;
    }
  if ((t1>0.0) && (t2>0.0) && (t1<255) && (t2<255))
    {
      Warning(1, "%s\n%s%f%s%f\n",
	    "Warning in nr_threshold. Two thresholds!",
	    "T1 = ", t1, " ,  T2 = ", t2);

      if (fabs (t1-estimate) < fabs(t2-estimate))
	return t1;
      else
	return t2;
    }
/*
  if (Verbose()) printf("t1 = %f, t2 = %f\n", t1, t2);
*/
  if ((t1>0.0) && (t1<255) )
    return t1;
  else
    return t2;
}


#ifdef FUNCPROTO
static int nr_find_min(histogram H, int low_index, int high_index)
#else
static int nr_find_min(H, low_index, high_index)
     histogram H;
     int low_index;
     int high_index;
#endif
{
  int i;
  int min_index;
  int min_value;

  min_index = low_index;
  min_value = H[min_index];
  for (i=low_index; i<=high_index; i++)
    if (H[i] < min_value)
      {
	min_value = H[i];
	min_index = i;
      }
  return min_index;
}


#ifdef FUNCPROTO
int bin_nr (double sdev_limit, double mean_limit, 
	    double min_ratio, double max_ratio, double valley_to_peak,
	    int region_size,
	    IBAND inband, IBAND outband)
#else
int bin_nr (sdev_limit, mean_limit, 
	    min_ratio, max_ratio, valley_to_peak,
	    region_size,
	    inband, outband)
     double sdev_limit;
     double mean_limit;
     double min_ratio;
     double max_ratio;
     double valley_to_peak;
     int region_size;
     IBAND inband;
     IBAND outband;
#endif
{
  int xsize, ysize;
  int x, y;
  int nof_regions_x;
  int nof_regions_y;
  IR_BAND sdev_1;
  IR_BAND mean_1;
  IBAND threshold;
  IBAND threshold_1;
  IBAND threshold_2;
  IBAND thresh_surface;
  IBAND window;
  int xstart, ystart;
  int xend, yend;
  histogram F;
  int v;
  int k_means_valley;
  double delta;

  double mu_1, mu_2;
  double sigma_1,sigma_2;
  double P_2;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_nr: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_nr: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  nof_regions_x = xsize / region_size ;
  nof_regions_y = ysize / region_size ;

  sdev_1  = (IR_BAND) Imake_band(Ireal_typ, nof_regions_x, nof_regions_y);
  mean_1  = (IR_BAND) Imake_band(Ireal_typ, nof_regions_x, nof_regions_y);
  threshold       = Imake_band(Iu_byte_typ, nof_regions_x, nof_regions_y);
  threshold_1     = Imake_band(Iu_byte_typ, nof_regions_x, nof_regions_y);
  threshold_2     = Imake_band(Iu_byte_typ, nof_regions_x, nof_regions_y);
  thresh_surface  = Imake_band(Iu_byte_typ, xsize, ysize);

  nr_mean_dev(inband, sdev_1, mean_1);
  
  for (x=1; x<= nof_regions_x; x++)
    for (y=1; y<= nof_regions_y; y++)
      {
	threshold[y][x] = 0;
	if (sdev_1[y][x] > sdev_limit)
	  {
	    xstart = (xsize * (x-1))/nof_regions_x + 1;
	    xend   = (xsize *  x   )/nof_regions_x;
	    ystart = (ysize * (y-1))/nof_regions_y + 1;	
	    yend   = (ysize *  y   )/nof_regions_y;	
	    window = Imake_subband(inband, xstart, ystart, 
				   xend+1-xstart, yend+1-ystart);

	    mkHisto(window, F);
/*
	    printf_histo(F);
*/
	    k_means(F, 2, &k_means_valley);

	    if (Verbose()) {
	      printf("[K-means] Valley = %i \n", k_means_valley);
	      printf("Get initial parameters for EM alg.\n");
	    }

	    EM_init_parameters (F, k_means_valley, &mu_1, &mu_2, 
				&sigma_1, &sigma_2, &P_2);

	    if (Verbose()) {
	      printf("Initial parameters:\n ");
	      printf("p2=%e, m1=%f, m2=%f, s1=%f, s2=%f \n",
		     P_2, mu_1, mu_2, sigma_1, sigma_2);
	      printf("EM alg. \n");
	    }

	    EM(F, F, 0, 0, &P_2, &mu_1, &mu_2, &sigma_1, &sigma_2);

	    if (Verbose()) {
	      printf("Final paramters: p2=%f, m1=%f, m2=%f, s1=%f, s2=%f \n",
		     P_2, mu_1, mu_2, sigma_1, sigma_2);
	    }

	    v = nr_find_min(F, (int)mu_1, (int)mu_2);
/*
	    if (Verbose()) printf("Valley = %i \n", v);
*/
	    delta = F[v] * 1.0 / MIN(F[(int)mu_1],F[(int)mu_2]);
/*
	    printf ("F[%i]=%i, F[%i]=%i, F[%i]=%i, Delta = %f\n", 
		    (int)mu_1, F[(int)mu_1], (int)mu_2, F[(int)mu_2], 
		    v, F[v], delta);
	    printf ("m2-m1 = %f, ( > %i) ,",(mu_2 - mu_1), mean_limit) ;
	    printf ("  (%f <) s1/s2 = %f (< %f)  ", 
		    min_ratio, (sigma_1/sigma_2) , max_ratio);
	    printf ("delta = %f (< %f)\n", delta , valley_to_peak);
*/
	    if (   ((mu_2 - mu_1) > mean_limit) 
		&& ((sigma_1/sigma_2) > min_ratio)
		&& ((sigma_1/sigma_2) < max_ratio)
		&& (delta < valley_to_peak))
	      {
/*
		if (Verbose()) printf("Calculated\n ");
*/
		threshold[y][x] = (int)nr_threshold(1-P_2, P_2, mu_1, mu_2, 
						    sigma_1, sigma_2,
						    (double)k_means_valley);
	      }
/*
	    if (Verbose())
              printf("Threshold (%i,%i) = %i\n", x, y, threshold[y][x]);
*/
	  }
     }
  
/*  write_byteband(threshold, "NR_bimod_regs_thresholds.img");
*/

  nr_interpot_reg(threshold, threshold_1);
/*  write_byteband(threshold_1, "NR_interp_1.img");
*/

  nr_smooth(threshold_1, threshold_2);
/*  write_byteband(threshold_2, "NR_interp_2.img");
*/

  nr_interpolate (threshold_2, thresh_surface);
/*  write_byteband (thresh_surface, "NR_thresh_surface.img");
*/

  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (inband[y][x] <= (int)thresh_surface[y][x])
	outband[y][x] = 0;
      else	
	outband[y][x] = 255;

  Idel_band(&threshold);
  Idel_band(&threshold_1);
  Idel_band(&threshold_2);
  Idel_band(&thresh_surface);

  return 0;
}

