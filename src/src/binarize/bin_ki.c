

/*

________________________________________________________________

        bin_ki.c
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

/*F:kittler_threshold=bin_ki*/
/*F:bin_ki*

________________________________________________________________

                bin_ki
________________________________________________________________


Name:           bin_ki, kittler_threshold - binarize a gray-scale band using 
                Kittler and Illingworth`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_ki( IBAND inband, IBAND outband );
                | int kittler_threshold( IBAND inband,
                |    double* mu_1, double* mu_2, double* mu );

Description:    'bin_ki' binarizes 'inband' using
                the global method of Kittler and Illingworth. 
		The result is returned in 'outband'.

		'kittler_threshold' returns Kittler and Illingworth`s
		threshold for 'inband'. The total mean 'mu' and
		the two cluster means 'mu1' and 'mu2' are calculated.

Files:  	src/binarize/bin_ki.c
See also:       binarize(1), bin_ot(3)

References:     &'Josef Kittler & John Illingworth'
                "Minimum error thresholding",
		Pattern Recognition, vol 19, pp 41-47, 1986.

Return value:   | bin_ki            : 0 - OK
                | kittler_threshold : Kittler and Illingworth`s 
		|                     threshold is returned.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/




#include <math.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>

#ifdef ANSA
int kittler_threshold (IBAND inband, double *mu_1, double *mu_2, double *mu)
#else
int kittler_threshold (inband, mu_1, mu_2, mu)
     IBAND inband;
     double *mu_1;
     double *mu_2;
     double *mu;
#endif
{

  histogram h;
  int threshold;
  double criterion;
  int g;
  int n;
  int T_low, T_high;
  int P_1_T, P_2_T, P_tot;
  double mu_1_T, mu_2_T;
  double sum_gh_1, sum_gh_2, sum_gh_tot;
  double sum_ggh_1, sum_ggh_2, sum_ggh_tot;
  double sigma_1_T, sigma_2_T;
  double J_T;


  mkHisto(inband, h);

  criterion = 1e10;
  threshold = 129;
  J_T = criterion;
  
  T_low = 0;
  while((h[T_low] == 0) && (T_low < 255))
    T_low++;

  T_high = 255;
  while((h[T_high] == 0) && (T_high > 0))
    T_high--;

  n = 0;
  for (g=T_low; g<=T_high; g++)
    n += h[g];

  P_1_T = h[T_low];
  P_tot = 0;
  for (g=T_low; g<= T_high; g++)
    P_tot += h[g];

  sum_gh_1 = T_low * h[T_low];
  sum_gh_tot = 0.0;
  for (g=T_low; g<=T_high; g++)
    sum_gh_tot += g*h[g];

  *mu = sum_gh_tot * 1.0 / n;

  sum_ggh_1 = T_low*T_low*h[T_low];
  sum_ggh_tot = 0.0;
  for (g=T_low; g<=T_high; g++)
    sum_ggh_tot += g*g*h[g];
  
  for (g=T_low+1; g<T_high-1; g++)
    {
      P_1_T += h[g];
      P_2_T = P_tot - P_1_T;

      sum_gh_1 += g*h[g];
      sum_gh_2 = sum_gh_tot - sum_gh_1;

      mu_1_T = sum_gh_1 / P_1_T;
      mu_2_T = sum_gh_2 / P_2_T;

      sum_ggh_1 += g*g*h[g];
      sum_ggh_2 = sum_ggh_tot - sum_ggh_1;

      sigma_1_T = sum_ggh_1/P_1_T - mu_1_T * mu_1_T;
      sigma_2_T = sum_ggh_2/P_2_T - mu_2_T * mu_2_T;

      /* Equation (15) in the article */
      if ((sigma_1_T != 0.0) && (P_1_T != 0) &&
	  (sigma_2_T != 0.0) && (P_2_T != 0))
	J_T = 1 + 2*(P_1_T*log(sigma_1_T) + P_2_T*log(sigma_2_T))
	  - 2*(P_1_T*log((double)P_1_T) + P_2_T*log((double)P_2_T) );

      if (criterion > J_T)
	{
	  criterion = J_T;
	  threshold = g;
	  *mu_1 = mu_1_T;
	  *mu_2 = mu_2_T;
	}
    }
  return threshold;
}


#ifdef FUNCPROTO
int bin_ki (IBAND inband, IBAND outband)
#else
int bin_ki (inband, outband)
     IBAND inband;
     IBAND outband;
#endif
{
  int xsize, ysize;
  int x, y;
  int threshold;
  double dummy_1, dummy_2, dummy_3;


  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  threshold = kittler_threshold(inband, &dummy_1, &dummy_2, &dummy_3);

  if (Verbose()) {
    printf("Kitter and Illingworth's method. Threshold = %i\n", threshold);
    printf("mean 1 = %6.2f, mean 2 = %6.2f, total mean = %6.2f\n",
	   dummy_1, dummy_2, dummy_3);
  }

  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (inband[y][x] <= threshold)
	outband[y][x] = 0;
      else	
	outband[y][x] = 255;

  return 0;
}





/*

#ifdef ANSA
static int kittler_threshold_old (IBAND inband, double *mu_1, double *mu_2, 
			   double *mu)
#else
static int kittler_threshold_old (inband, mu_1, mu_2, mu)
     IBAND inband;
     double *mu_1;
     double *mu_2;
     double *mu;
#endif
{

  histogram h;
  int threshold;
  double criterion;
  double expr_1;
  int g, T;
  int sum;
  int T_low, T_high;
  int P_1_T, P_2_T, P_tot;
  double mu_1_T, mu_2_T;
  double alpha_1, alpha_2, alpha_tot;
  double beta_1, beta_2, beta_tot;
  double gamma_1, gamma_2;
  double delta_1, delta_2;
  double sigma_1_T, sigma_2_T;
  double J_T;


  mkHisto(inband, h);

  criterion = 1e10;
  threshold = 129;
  J_T = criterion;
  
  T_low = 0;
  while((h[T_low] == 0) && (T_low < 255))
    T_low++;

  T_high = 255;
  while((h[T_high] == 0) && (T_high > 0))
    T_high--;

  sum = 0;
  for (g=T_low; g<=T_high; g++)
    sum += h[g];

  expr_1 = 0.0;
  for (g=T_low; g<=T_high; g++)
    expr_1 += g*h[g];
  *mu = expr_1 * 1.0 / sum;
      
  P_1_T = h[T_low];
  P_tot = 0;
  for (g=T_low; g<= T_high; g++)
    P_tot += h[g];

  alpha_1 = T_low * h[T_low];
  alpha_tot = 0.0;
  for (g=T_low; g<=T_high; g++)
    alpha_tot += g*h[g];

  beta_1 = T_low*T_low*h[T_low];
  beta_tot = 0.0;
  for (g=T_low; g<=T_high; g++)
    beta_tot += g*g*h[g];
  
  for (T=T_low+1; T<T_high-1; T++)
    {
      P_1_T += h[T];
      P_2_T = P_tot - P_1_T;

      alpha_1 += T*h[T];
      alpha_2 = alpha_tot - alpha_1;

      mu_1_T = alpha_1 / P_1_T;
      mu_2_T = alpha_2 / P_2_T;

      beta_1 += T*T*h[T];
      beta_2 = beta_tot - beta_1;

      gamma_1 = -2*mu_1_T*alpha_1;
      gamma_2 = -2*mu_2_T*alpha_2;

      delta_1 = mu_1_T * mu_1_T * P_1_T;
      delta_2 = mu_2_T * mu_2_T * P_2_T;

      sigma_1_T = (beta_1 + gamma_1 + delta_1)/P_1_T;
      sigma_2_T = (beta_2 + gamma_2 + delta_2)/P_2_T;

      \* Equation (15) in the article *\
      if ((sigma_1_T != 0.0) && (P_1_T != 0) &&
	  (sigma_2_T != 0.0) && (P_2_T != 0))
	J_T = 1 + 2*(P_1_T*log(sigma_1_T) + P_2_T*log(sigma_2_T))
	  - 2*(P_1_T*log((double)P_1_T) + P_2_T*log((double)P_2_T) );

      if (criterion > J_T)
	{
	  criterion = J_T;
	  threshold = T;
	  *mu_1 = mu_1_T;
	  *mu_2 = mu_2_T;
	}
    }
  return threshold;
}

*/
