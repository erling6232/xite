

/*

________________________________________________________________

        bin_mh.c
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

/*F:bin_mh*

________________________________________________________________

                bin_mh
________________________________________________________________


Name:           bin_mh - binarize a gray-scale band using 
                Mardia and Hainsworth`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_mh( IBAND inband, IBAND outband,
                |    int use_sem_var, double convergence_limit );

Description:    The input band 'inband' is binarized using the method of
                Mardia and Hainsworth. The result is returned in 'outband'.

		Mardia and Hainsworth`s method first makes an initial
		binarization. Then several steps are iterated until
		convergence is reached. First, the estimated mean values mu1
		and mu2, and the number of pixels n_i in both foreground and
		background of the current binarization are calculated. Then a
		threshold 'T' based on these values is calculated. Then, for
		each pixel, a weighted mean, 'G', of the pixel and its eight
		neighbors is calculated. If 'G'<='T', the pixel is classified
		as ``black'', otherwise ``white''. The weights on the neighbor
		pixels, used to calculate 'G', may be equal, or they may be
		calculated based on a semivariogram approximation. If
		'use_sem_var' >= 1, the semivariogram approach is used. The
		last step of each iteration is to smooth the new binary image
		using a (3 x 3) median filter. 'convergence_limit' is used to
		test if convergence has been reached. The number of different
		pixels of two successive binarizations is counted, and
		compared to 'convergence_limit'.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1)

References:     'K.V. Mardia & T.J. Hainsworth'
                "A spatial thresholding method for image segmentation",
		IEEE Transactions on Pattern Analysis and Machine
		Intelligence, vol T-PAMI 10, no 6, pp 919-927, 1988.

Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/median.h>
#include <xite/binarize.h>
#include <xite/message.h>

#include "sem_var.c"

#ifdef FUNCPROTO
static int mh_threshold(IBAND inband, IBAND labels, double threshold)
#else
static int mh_threshold(inband, labels, threshold)
     IBAND inband;
     IBAND labels;
     double threshold;
#endif
{
  int x, y, xsize, ysize;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (inband[y][x] <= threshold)
	labels[y][x] = 0;
      else
	labels[y][x] = 255;

  return 0;

} /* END mh_threshold */




#ifdef FUNCPROTO
static int mh_class_means(IBAND inband, IBAND labels, 
		   double *mean0, double *mean1, int *n0, int *n1)
#else
static int mh_class_means(inband, labels, 
		   mean0, mean1, n0, n1)
     IBAND inband;
     IBAND labels;
     double *mean0;
     double *mean1;
     int *n0;
     int *n1;
#endif
{
  int x, y, xsize, ysize;
  double sum0 = 0.0;
  double sum1 = 0.0;
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  n0[0] = 0;
  n1[0] = 0;
  mean0[0] = 0.0;
  mean1[0] = 0.0;
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (labels[y][x] == 0)
	{
	  n0[0] ++;
	  sum0 += inband[y][x];
	}
      else
	{
	  n1[0] ++;
	  sum1 += inband[y][x];
	}	
  if (n0[0] != 0)
    mean0[0] = sum0 / n0[0];
  else	
    {
#ifdef BIN_MH_DEBUG_ON	
      fprintf(stderr, "[mh_class_means] n0 = 0\n");
#endif
      mean0[0] = 0;
    }
  if (n1[0] != 0)
    mean1[0] = sum1 / n1[0];  
  else	 
    {
#ifdef BIN_MH_DEBUG_ON	
      fprintf(stderr, "[mh_class_means] n1 = 0\n");
#endif
      mean1[0] = 255;
    }
  return 1;
} /* END mh_class_means */



#ifdef FUNCPROTO
static double mh_tot_variance(IBAND inband, double tot_mean)
#else
static double mh_tot_variance(inband, tot_mean)
     IBAND inband;
     double tot_mean;
#endif
{
  int x, y, xsize, ysize;
  double sum = 0.0;
  
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      sum += (inband[y][x]-tot_mean) * (inband[y][x]-tot_mean);

  return (sum/(xsize*ysize - 1));
} /* END mh_tot_variance */


#ifdef FUNCPROTO
static int mh_iso_corr(II_BAND inband, double sigma2, 
		       double *rho_1, double *rho_sq2, 
		       double *rho_2, double *rho_sq5, double *rho_2sq2)
#else
static int mh_iso_corr(inband, sigma2, rho_1, rho_sq2, 
		       rho_2, rho_sq5, rho_2sq2)
     II_BAND inband;
     double sigma2;
     double *rho_1;
     double *rho_sq2;
     double *rho_2;
     double *rho_sq5;
     double *rho_2sq2;
#endif
{
  double gamma_1, gamma_sq2, gamma_2;
  double gamma_sq5, gamma_2sq2;
  
  gamma_1    = 0.5  * (sem_var(inband,  1, 0) + sem_var(inband,  0, 1));
  gamma_sq2  = 0.5  * (sem_var(inband,  1, 1) + sem_var(inband, -1, 1));
  gamma_2    = 0.5  * (sem_var(inband,  2, 0) + sem_var(inband,  0, 2));
  gamma_2sq2 = 0.5  * (sem_var(inband,  2, 2) + sem_var(inband, -2, 2));
  gamma_sq5  = 0.25 * (sem_var(inband,  2, 1) + sem_var(inband,  1, 2) +
		       sem_var(inband, -2, 1) + sem_var(inband, -1, 2));
  
  rho_1[0]    = 1.0 - (gamma_1    / sigma2);
  rho_sq2[0]  = 1.0 - (gamma_sq2  / sigma2);
  rho_2[0]    = 1.0 - (gamma_2    / sigma2);
  rho_sq5[0]  = 1.0 - (gamma_sq5  / sigma2);
  rho_2sq2[0] = 1.0 - (gamma_2sq2 / sigma2);
  return 1;
} /* END  mh_iso_corr */



#ifdef FUNCPROTO
static int mh_segment(IBAND inband, IBAND labels, double threshold, 
	       double gamma_0, double gamma_1, double gamma_5)
#else
static int mh_segment(inband, labels, threshold, 
	       gamma_0, gamma_1, gamma_5)
     IBAND inband;
     IBAND labels;
     double threshold;
     double gamma_0;
     double gamma_1;
     double gamma_5;
#endif
{
  IBAND Z;
  int x, y, xsize, ysize;
  int i, j;
  double G;
  
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  Z = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(inband, 0, 0, 1, Z);
  
  for (x=1; x<=xsize; x++)
    {
      i = x+1;
      for (y=1; y<=ysize; y++)
	{
	  j = y+1;
	  G = gamma_0 * Z[j][i] 
	    + gamma_1 * (Z[j-1][i] + Z[j+1][i] + Z[j][i-1] + Z[j][i+1])
	      + gamma_5 * (  Z[j-1][i-1] + Z[j+1][i-1] 
			   + Z[j-1][i+1] + Z[j+1][i+1]);
	  if (G < threshold)
	    labels[y][x] = BIN_FORGROUND;
	  else
	    labels[y][x] = BIN_BACKGROUND;
	}
    }
  Idel_band(&Z);
  return 1;
} /* END mh_segment */


#ifdef FUNCPROTO
static int mh_count_diff(IBAND A, IBAND B)
#else
static int mh_count_diff(A, B)
     IBAND A;
     IBAND B;
#endif
{
  int xsize, ysize;
  int x, y;
  int count;
  
  count = 0;
  xsize = Ixsize(A);
  ysize = Iysize(B);
  
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (A[y][x] != B[y][x])
	count++;
  
  return count;
} /* END mh_count_diff */



#ifdef FUNCPROTO
int bin_mh (IBAND inband, IBAND outband, int use_sem_var, 
	    double convergence_limit)
#else
int bin_mh (inband, outband, use_sem_var, 
	    convergence_limit)
     IBAND inband;
     IBAND outband;
     int use_sem_var;
     double convergence_limit;
#endif
{
  int xsize, ysize;
  int x, y;
  int n1, n2;
  double threshold;
  int convergence = FALSE;
  int diff;
  
  IBAND labels, old_labels;
  II_BAND data;
  IBAND smooth_labels;
  II_BAND smooth_data;
  double mean_z1, mean_z2, tot_mean;
  
  double a, b, c, d, e;
  double A, B;
  double u, v, w;
  double gamma_0, gamma_1, gamma_5;
  double nu2;
  double rho_1, rho_sq2, rho_2;
  double rho_sq5, rho_2sq2;
  double sigma2;
  
  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_mh: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_mh: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  labels = Imake_band(Iu_byte_typ, xsize, ysize);
  old_labels = Imake_band(Iu_byte_typ, xsize, ysize);
  data   = (II_BAND)Imake_band(Integer_typ, xsize, ysize);
  smooth_labels  = Imake_band(Iu_byte_typ, xsize, ysize);
  smooth_data   = (II_BAND)Imake_band(Integer_typ, xsize, ysize);
  
  
  /* Spatial thresholding algorithm, section V-A and V-D of 
     Mardia and Hainswworth's article. 
     The basic algoritm is given in sec. V.A, and an extra step
     (v') is added in sec. V-D. */
     
     /*  step i) Initial classification */
       
#ifdef BIN_MH_DEBUG_ON
       fprintf(stderr, "step i)\n");
#endif
  
  threshold = (double)otsu_threshold(inband, &mean_z1, &mean_z2, &tot_mean);
  
  mh_threshold(inband, labels, threshold);
  
  /*  write_byteband(labels, "MH_init_labels.img");
   */
  
  sigma2 = mh_tot_variance(inband, tot_mean);
  
  
#ifdef BIN_MH_DEBUG_ON
  fprintf(stderr, "sigma2 = %6.3f\n", sigma2);
#endif
  
  
  
  
  /* Repeat until convergence */
  
  while (!convergence)
    {
      Icopy_band(labels, old_labels);
      
      /* step ii) calculate mean_z1, mean_z2, n1, n2 */
      
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step ii)\n");
#endif
      
      mh_class_means(inband, labels, &mean_z1, &mean_z2, &n1, &n2);
      
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr,
	      "mean_z1 = %6.2f,  mean_z2 = %6.2f,  n1 = %i,  n2 = %i\n",
	      mean_z1, mean_z2, n1, n2);
#endif
      
      
      /* step iii) Calculate estimates for the spatial parameters. */
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step iii)\n");
#endif
      
      /* a) */
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step iii) a)\n");
#endif
      
      for (x = 1; x <= xsize; x++)
	for (y = 1; y <= ysize; y++)
	  if (labels[y][x] == 0)
	    data[y][x] = inband[y][x] - mean_z1;
	  else
	    data[y][x] = inband[y][x] - mean_z2;
      
      /*      write_intband(data, "MH_data.img");
       */
      
      /* b) */
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step iii) b)\n");
#endif
      
      if (use_sem_var)
	{
	  
	  mh_iso_corr (data, sigma2, &rho_1, &rho_sq2, &rho_2, 
		       &rho_sq5, &rho_2sq2);	
	  
#ifdef BIN_MH_DEBUG_ON
	  fprintf(stderr, "rho_1 = %6.3f,  rho_sq2 = %6.3f,  rho_2 = %6.3f, ",
		  rho_1, rho_sq2, rho_2);
	  fprintf(stderr, " rho_sq5 = %6.3f,  rho_2sq2 = %6.3f\n", 
		  rho_sq5, rho_2sq2);
#endif  
	  
	  /* c) */
	  
#ifdef BIN_MH_DEBUG_ON
	  fprintf(stderr, "step iii) c)\n");
#endif
	  
	  
	  a = 0.25 * (1 + 2*rho_sq2 + rho_2);
	  b = rho_1;
	  c = 0.25 * (1 + 2*rho_2 + rho_2sq2);
	  d = rho_sq2;
	  e = 0.5 * (rho_1 + rho_sq5);
	  u = (1-b)/sqrt(a-b*b);
	  v = (1-d)/sqrt(c-d*d);
	  w = (e-b*d)/sqrt((a-b*b)*(c-d*d));
	  A = (u-v*w) / (4 * sqrt(1-w*w) * sqrt(a-b*b));
	  B = (v-u*w) / (4 * sqrt(1-w*w) * sqrt(c-d*d));
	  
	  gamma_0 = 1 - 4*b*A - 4*d*B;
	  gamma_1 = A;
	  gamma_5 = B;
	}
      else
	{
	  
	  gamma_0 = 1.0/9.0;
	  gamma_1 = 1.0/9.0;
	  gamma_5 = 1.0/9.0;
	}
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "gamma_0 = %6.3f,  gamma_1 = %6.3f,  gamma_5 = %6.3f\n",
	      gamma_0, gamma_1, gamma_5);
#endif
      nu2 = gamma_0 + 4*gamma_1 + 4*gamma_5;
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "nu2 = %6.3f\n", nu2);
#endif
      
      /* Smooth the data values by taking the mean inside a 3x3 window 
	 centered on each pixel. */
      /*
	mean (data, smooth_data, 3, 3);
	for (x = 1; x <= xsize; x++)
	for (y = 1; y <= ysize; y++)
	data[y][x] = smooth_data[y][x];
	*/	
      
      /* step iv) */
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step iv)\n");
#endif
      
      /* Threshold the image to obtain a set of labels */
      threshold = 0.5*nu2*(mean_z1 + mean_z2) 
	+ (sigma2/(mean_z1 - mean_z2)) * log(((double)n2)/((double)n1));  
      
#ifdef BIN_MH_DEBUG_ON

#endif
      if (Verbose()) {
	printf("mean_z1 = %6.2f, mean_z2 = %6.2f\n", mean_z1, mean_z2);
	printf("sigma2 = %6.2f\n", sigma2);
	printf("n1 = %5i, n2 = %5i, log (n2/n1) = %10.6f\n",
		n1, n2, log(((double)n2)/((double)n1)));
	printf("threshold = %6.2f\n", threshold);
      }
      
      /* step v) */
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step v)\n");
#endif
      
      mh_segment(inband, labels, threshold, gamma_0, gamma_1, gamma_5);
      /* write_byteband(labels, "MH_labels.img");	*/
      
      /* step v') */
      
#ifdef BIN_MH_DEBUG_ON
      fprintf(stderr, "step v\')\n");
#endif
      
      /* Smooth the labels by median filtering to obtain a new set of 
	 labels */
      median (labels, smooth_labels, 3, 3);
      Icopy_band (smooth_labels, labels);
      
      /* Control the number of iterations !!!!!! */
      if ((diff = mh_count_diff(labels, old_labels)) < convergence_limit)
	convergence = TRUE;
      if (Verbose()) printf("Convergence? diff = %i\n", diff);
    }
  Icopy_band(labels, outband);
  
  return 0;
} /* END bin_mh*/
