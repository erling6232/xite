

/*

________________________________________________________________

        bin_tfj.c
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

/*F:bin_tfj*

________________________________________________________________

                bin_tfj
________________________________________________________________


Name:           bin_tfj - binarize a gray-scale band using 
                Taxt, Flynn and Jain`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_tfj( IBAND inband, IBAND outband,
                |    int region_size, int step_size,
                |    int training_size );

Description:    The input band 'inband' is binarized using the method of
                Taxt, Flynn and Jain. The result is returned in 'outband'.

		Taxt, Flynn and Jain`s method divides the image into
		non-overlapping windows of size ('small_window_size' x
		'small_window_size'). For each window, a larger concentric
		window of size ('region_size' x 'region_size') pixels. The
		histogram of each of the larger windows is approximated by a
		mixture of two Gaussian distributions. The parameters of the
		mixture are estimated using an expectation-maximization (EM)
		algorithm. In each small window, the pixels are classified
		using the quadratic Bayes' classifier.

		A training set of 'training_size' pixels are picked on random,
		but edge pixels are avoided in the training set. The EM
		algorithm requires global start values for the estimated class
		means mu1 and mu2, and estimated standard deviations sigma1
		and sigma2. These are obtained by k-means clustering of the
		whole input band. The EM algorithm also needs an initial
		estimate of the mixing weight pi, and pi = 0.5 is used.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

See also:       binarize(1)

References:     &'T. Taxt, P.J. Flynn & A.K. Jain'
                "Segmentation of document images",
		IEEE Transactions on Pattern Analysis and Machine
		Intelligence,
		vol T-PAMI 11, no 12, pp 1322-1329, 1989.

Files:  	src/binarize/bin_tfj.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_RANDOM_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "EM.c"
#include "k_means.c"

#define TFJ_SMALL 1e-3
#define TFJ_TRN_CLS 2
#define TFJ_TRN_THR 1


static int tfj_pick_training_pixels(IBAND inband, IBAND picked, int train_size)
{
  int xsize, ysize;
  int x, y, i;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      picked[y][x] = FALSE;

  srandom(1);
  for (i=1; i<= train_size; i++)
    {
      x = (random() % xsize) + 1;
      y = (random() % ysize) + 1;
      if (!picked[y][x])
	picked[y][x] = TRUE;
      else
	i--;
    }

  return 1;
}



static int tfj_make_histo(IBAND inband, IBAND picked, histogram H)
{
  int x, y, xsize, ysize, i;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  for (i=0; i<=255; i++)
    H[i] = 0;

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      if (picked[y][x])
	H[inband[y][x]]++;
  return 1;
} /* END tfj_make_histo */



static int tfj_init_parameters(histogram H, int threshold, int *n1, int *n2,
			double *mean1, double *mean2, 
			double *sdev1, double *sdev2, double *pi)
{
  int i;
  
  n1[0] = 0;
  n2[0] = 0;
  for (i=0; i<=threshold; i++)
    n1[0] += H[i];
  for (i=threshold+1; i<=BIN_BACKGROUND; i++)
    n2[0] += H[i];
  EM_init_parameters(H, threshold, mean1, mean2, sdev1, sdev2, pi);

  pi[0] = 0.5;
  return 1;
}


static int tfj_classify(IBAND inband, IBAND outband, 
		 double pi, double mean1, double mean2, 
		 double sdev1, double sdev2)
{
  int xsize, ysize;
  int x, y;

  xsize = Ixsize(inband);	
  ysize = Iysize(inband);


      for (x=1; x<=xsize; x++)
	for (y=1; y<=ysize; y++)
	  {
/*
          if (Verbose()) printf("pixel[%i][%i] = %i\n", x, y, inband[y][x]);
*/
	    if ( (1-pi) * gauss(inband[y][x], mean1, sdev1*sdev1) 
		> pi * gauss(inband[y][x], mean2, sdev2*sdev2))
	      outband[y][x] = 0;
	    else
	      outband[y][x] = 255;	      
/*
          if (Verbose())
	    printf("f(%i|1)=%7.4f, f(%i|2)=%7.4f\n",
		    inband[y][x], gauss(inband[y][x], mean1, sdev1*sdev1),
		    inband[y][x], gauss(inband[y][x], mean2, sdev2*sdev2));
*/
	  }
    
  return 1;
} /* END tfj_classify */



static int tfj_remove_edge_cluster(histogram H, int *threshold, 
			    int nof_clusters)
{
  int i;
  int a, b;
  a = (nof_clusters-1) / 2;
  b = nof_clusters / 2 ;
  for (i = threshold[a] + 1; i <= threshold[b]; i++)
    H[i] = 0;
  return 1;
}


static int tfj_reduce_training_set(histogram H, int training_size)
{
  int i;
  int sum = 0;
  int max = 0;


  srandom (0);
  for (i=BIN_FOREGROUND; i <= BIN_BACKGROUND; i++)
    {
      sum += H[i];
      if (max < H[i])
	max = H[i];
    }
  if (sum <= training_size)
    {
      if (Verbose())
	printf("[tfj_reduce_training_set] sum = %i, tr.size=%i\n",
	       sum, training_size);
      return -1;
    }

  while (sum > training_size)
    {
      i = random()  & BIN_BACKGROUND;
      if (random() % (max+1) < H[i])
	{
	  H[i]--;	
	  sum--;
/*
        if (Verbose())
	  printf("[tfj_reduce_training_set] sum = %6i\n", sum);
*/
	}
    }

  return 1;
}	


int bin_tfj (IBAND inband, IBAND outband, int region_size, int step_size,
	     int training_size)
{
  int xsize, ysize;
  int xstart, ystart;
  int xend, yend;
  int xwidth, yheight;
  int nof_reg_x, nof_reg_y;
  int rx, ry;
  int threshold[TFJ_TRN_THR];
  histogram H, H_train;
  IBAND expanded;
  IBAND in_window;
  IBAND out_window;  
  IBAND large_window;
  IBAND train_band;
  int expand_depth;

  double mean1_0, mean2_0;        /* Global estimates used as initial */
  double sdev1_0, sdev2_0, pi_0;  /* values for local EM-algorithm */
  double mean1, mean2;            /* Local estimates produced by */
  double sdev1, sdev2, pi;        /* EM-algorithm */
  int n1, n2;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_tfj: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_tfj: Output pixel type must be unsigned byte.\n"));

  if (Verbose()) {
    printf("Taxt/Flynn/Jain's binarization method.\n");	
    printf("[-r] region_size   = %i\n", region_size);
    printf("[-s] step_size     = %i\n", step_size);
    printf("[-t] training_size = %i\n", training_size);
  }

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  nof_reg_x = xsize / step_size;
  nof_reg_y = ysize / step_size;

  if (training_size >= (xsize * ysize))
    training_size = 0;


  if (step_size %2 != region_size %2)
    region_size++;
  expand_depth = (region_size - step_size)/2;
  expanded = Imake_band(Iu_byte_typ, xsize + 2*expand_depth, 
			ysize + 2*expand_depth);
  expand_edge(inband, 0, 0, expand_depth, expanded);

  if (training_size == 0)
    {
      mkHisto(inband, H_train);
      k_means (H_train, 2, threshold);
      EM_init_parameters(H_train, threshold[0], &mean1_0, &mean2_0, 
			 &sdev1_0, &sdev2_0, &pi_0);
      n1 = 0;
      n2 = 0;
    }
  else
    {
      train_band = Imake_band(Iu_byte_typ, xsize, ysize);
      tfj_pick_training_pixels(inband, train_band, 5 * training_size);
      tfj_make_histo(inband, train_band, H_train);
      k_means(H_train, TFJ_TRN_CLS, threshold);
      if (TFJ_TRN_CLS > 2)
	{
	  tfj_remove_edge_cluster(H_train, threshold, TFJ_TRN_CLS);
/*
        if (Verbose()) printf("[bin_tfj] center cluster removed\n");
*/
	  tfj_reduce_training_set(H_train, training_size);
/*
        if (Verbose()) printf("[bin_tfj] training set reduced.\n");
*/
	}
      tfj_init_parameters(H_train, threshold[0], &n1, &n2,
			  &mean1_0, &mean2_0, &sdev1_0, &sdev2_0, &pi_0);
/*
      if (Verbose()) printf("pi_0 = %f\n", pi_0);
*/
    }

  xend = 0;
  for (rx=1; rx<=nof_reg_x; rx++)
    {
      if (Verbose()) printf(".\n");
      xstart = xend+1;
      xend = (xsize * rx) / nof_reg_x;
      xwidth = xend+1-xstart;
      yend = 0;	
      for (ry=1; ry<=nof_reg_y; ry++)
	{
/*
	  if (Verbose()) printf("window: rx=%3i, ry=%3i\n", rx, ry);
*/
	  ystart = yend+1;
	  yend = (ysize * ry) / nof_reg_y;
	  yheight = yend+1-ystart;
	  in_window = Imake_subband (inband, xstart, ystart, 
				     xwidth, yheight);
	  out_window = Imake_subband (outband, xstart, ystart, 
				      xwidth, yheight);
	  large_window = Imake_subband(expanded, xstart, ystart, 
				       xwidth + 2*expand_depth,
				       yheight + 2*expand_depth);
	  
	  pi = pi_0;
	  mean1 = mean1_0;
	  mean2 = mean2_0;
	  sdev1 = sdev1_0;
	  sdev2 = sdev2_0;
	  mkHisto(large_window, H);
/*
      k_means(H, 2, &threshold);
      EM_init_parameters(H, threshold,
			  &mean1, &mean2, &sdev1, &sdev2, &pi);
*/
	  EM(H, H_train, n1, n2, &pi, &mean1, &mean2, &sdev1, &sdev2);

/*
        if (Verbose())
	  printf("pi=%6.3f, m1=%6.2f, s1=%6.2f, m2=%6.2f, s2=%6.2f\n\n",
		  pi, mean1, sdev1, mean2, sdev2);
*/
	  tfj_classify(in_window, out_window, pi, mean1, mean2, sdev1, sdev2);
	}
    }
  return 0;
}
