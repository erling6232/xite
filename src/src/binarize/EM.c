 

/*

________________________________________________________________

        EM.c
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

/* static char *Id = "$Id$, Blab, UiO"; */

/*L:EM_init_parameters=EM*/
/*L:EM*

________________________________________________________________

                EM
________________________________________________________________


Name:           EM, EM_init_parameters - Expectation-maximation algorithm
Syntax:         | int EM_init_parameters(histogram H, int threshold,
		|                        double *mean_1, double *mean_2, 
		|                        double *sdev_1, double *sdev_2, 
		|                        double *pi_2)
		| int EM (histogram H, histogram H_train, 
		|         int n_1, int n_2,
		|         double *pi_2, double *mean_1, double *mean_2,  
		|         double *sdev_1, double *sdev_2)


Description:    'EM'

                Input parameters: 'H': Histogram of the unclassified samples.
                'n_1', 'n_2': Number of training set samples from class 1 and 
		class 2, respectively.
		
		In-out parameters: 'pi_2': Mixing weight for class 2. Mixing 
		weight for class 1 is given as 1.0-'pi_2'. 'mean_1', 
		'mean_2': estimates of the class means. 'sdev_1', 'sdev_2':
		estimates of the class standard deviations. These five
		parameters must be initialized before EM is called.
		This can be done by calling 'EM_init_parameters'. Some
		algorithms prefer the EM algorithm to be started with
		'pi_2' = 0.5, ignoring the value calculated by
		e.g. 'EM_init_parameters'. 

		'EM_init_parameters'

		Given a histogram and a threshold, calculate sample
		mean values and standard deviation values, and the
		relative occurence of elements of the class having
		values larger than the threshold. 

Files:		src/binarize/EM.c

See also:       k_means (static function)
                
Diagnostics:
Return value:   | 1 - ok
Restrictions:   Works on unsigned byte bands.
Author:         Oivind Due Trier
Examples:       
Id:             $Id$
________________________________________________________________

*/


#include <xite/includes.h>
/* #include XITE_STDIO_H */
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <math.h>
#include <xite/binarize.h>
#include <xite/message.h>

#define EM_LOG_L_LIMIT 0.00001
#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif

#ifdef FUNCPROTO
static int EM_init_parameters(histogram H, int threshold,
		       double *mean_1, double *mean_2, 
		       double *sdev_1, double *sdev_2, 
		       double *pi_2)
#else
static int EM_init_parameters(H, threshold,
		       mean_1, mean_2, 
		       sdev_1, sdev_2, 
		       pi_2)
     histogram H;
     int threshold;
     double *mean_1;
     double *mean_2;
     double *sdev_1;
     double *sdev_2;
     double *pi_2;
#endif
{
  int i;
  int n1, n2;
  double sum1, sum2;
  double sum_sq1, sum_sq2;
  double var_1, var_2;

  n1 = n2 = 0;
  sum1 = sum2 = 0.0;
  sum_sq1 = sum_sq2 = 0.0;
  
  for (i=0; i<threshold; i++)
    {
      n1 += H[i];
      sum1 += i*H[i];
      sum_sq1 += i*i*H[i];
    }
  for (i=threshold; i<=255; i++)
    {
      n2 += H[i];
      sum2 += i*H[i];
      sum_sq2 += i*i*H[i];
    }
  pi_2[0] = 1.0* n2/(n1+n2);
  mean_1[0] = 1.0/n1*sum1;
  mean_2[0] = 1.0/n2*sum2;
  var_1 = 1.0/n1*sum_sq1 - mean_1[0]*mean_1[0];
  var_2 = 1.0/n2*sum_sq2 - mean_2[0]*mean_2[0];
  sdev_1[0] = sqrt(var_1);
  sdev_2[0] = sqrt(var_2);

  return 1;
}




#ifdef FUNCPROTO
static double gauss(double x, double mean, double var)
#else
static double gauss(x, mean, var)
     double x;
     double mean;
     double var;
#endif
{
  double a, b, c;
  a = sqrt(2*M_PI*var);
  b = -0.5*(x-mean)*(x-mean)/var;
  c = 1.0/a * exp(b);
  return c;
}




#ifdef FUNCPROTO
static int EM (histogram H,       /* In: Histogram of the unclassified samples. */
	histogram H_train, /* In: Histogram of the training samples. */
	int n_1,         /* In: #class 1 training set samples */
	int n_2,         /* In: #class 2 training set samples */
	double *pi_2,    /* In: Initial estimate. Out: Final estimate. */
	double *mean_1,    /* In: Initial estimate. Out: Final estimate. */
	double *mean_2,    /* In: Initial estimate. Out: Final estimate. */
	double *sdev_1, /* In: Initial estimate. Out: Final estimate. */
	double *sdev_2) /* In: Initial estimate. Out: Final estimate. */
#else
static int EM (H, H_train, n_1, n_2, pi_2, mean_1, mean_2, sdev_1, sdev_2)
     histogram H;
     histogram H_train;
     int n_1;        
     int n_2;   
     double *pi_2; 
     double *mean_1; 
     double *mean_2; 
     double *sdev_1; 
     double *sdev_2;
#endif
{
  double old_pi[3];
  double new_pi[3];
  double init_mu[3];
  double old_mu[3];
  double new_mu[3];
  double old_Sigma[3];
  double new_Sigma[3];
  double init_Sigma[3];
  int init_n[3];
  double sum_P[3];
  double sum_P_x[3];
  double sum_P_xx[3];
  double f[3];
  double sum_pi_f;
  double P;
  double x;

  int convergence = FALSE;
  int count = 0;
  int i, j, c;
  int M;

  int train_count, class;
  double sum, log_L, log_L_train, log_L_uncl;
  double old_log_L;
/*
  printf("EM-alg.\n");
*/

  init_n[1] = n_1;
  init_n[2] = n_2;
  init_mu[1] = old_mu[1] = mean_1[0];
  init_mu[2] = old_mu[2] = mean_2[0];
  init_Sigma[1] = old_Sigma[1] = sdev_1[0]*sdev_1[0];
  init_Sigma[2] = old_Sigma[2] = sdev_2[0]*sdev_2[0];
  old_pi[1] = 1-pi_2[0];
  old_pi[2] = pi_2[0];

  M = 0;
  for (i=0; i<256; i++)
    M += H[i];
  
  old_log_L = 0.0;

  while (!convergence)
    {
/*
      for (c=1; c<=2; c++)
	printf("class %1i: pi[%i]=%8.3f, mu[%i]=%7.3f, Sigma[%i]=%7.3f\n",
		c, c, old_pi[c], c, old_mu[c], c, old_Sigma[c]);
*/
      for (c=1; c<=2; c++)
	{
	  sum_P[c] = 0.0;
	  sum_P_x[c] = 0.0;
	  sum_P_xx[c] = 0.0;
	}
      for (i=0; i<256; i++)
	{
	  sum_pi_f = 0.0;
	  for (c=1; c<=2; c++)
	    { 
	      x = 1.0 * i;
	      f[c] = gauss(x, old_mu[c], old_Sigma[c]);
	      sum_pi_f += old_pi[c]*f[c];
	    }
	  for (c=1; c<=2; c++)
	    {
	      P = (old_pi[c] * f[c]) / sum_pi_f;
/*
	      printf ("old_pi[%i]=%7.3f, f[%i]=%6.3f, sum_pi_f=%6.3f,  ",
		      c, old_pi[c], c, f[c], sum_pi_f);
	      printf ("P(%i|%i) = %7.3f\n", c,i, P);
*/
	      sum_P[c] += H[i] * P;
	      sum_P_x[c] += H[i] * P * x;
	      sum_P_xx[c] += H[i] * P * (x-old_mu[c]) * (x-old_mu[c]);
	    }
	}
      
      new_pi[1] = 1.0/M * sum_P[1];
      new_pi[2] = 1.0/M * sum_P[2];
/*
      new_pi[1] = 1.0 - new_pi[2];
*/
      for (c=1; c<=2; c++)
	{

/*
	  printf("sum_P[%i]=%8.3f,  sum_P_x[%i]=%8.3f,  sum_P_xx[%i]=%8.3f\n",
		  c, sum_P[c], c, sum_P_x[c], c, sum_P_xx[c]);
*/
	  new_mu[c] = (  init_n[c]*init_mu[c] 
		       + sum_P_x[c])/(init_n[c] + sum_P[c]);
	  new_Sigma[c] = sum_P_xx[c]/(init_n[c]+sum_P[c]);
	  if (init_n[c]> 0)
	    new_Sigma[c] += ( (init_n[c]*init_Sigma[c] 
			       + init_n[c]*(old_mu[c]-init_mu[c])
			       *(old_mu[c]-init_mu[c]))
			     /(init_n[c] + sum_P[c]));
	}
      
      
      for (c=1; c<=2; c++)
	{
	  old_mu[c]    = new_mu[c];
	  old_Sigma[c] = new_Sigma[c];
	  old_pi[c]    = new_pi[c];
	}


      /* Total likelihood for the observed data in the window.*/
      log_L_train = 0.0;
      if (n_1 + n_2 > 0)
	{
	  train_count = 0;
	  for (i=0; i<256; i++)
	    {	
	      if (H_train[i] > 0)
		{
		  if (train_count < n_1)
		    for (j=1; j<=H_train[i]; j++)
		      log_L_train += log(gauss((double)i, old_mu[1], 
					       old_Sigma[1]));
		  else
		    for (j=1; j<=H_train[i]; j++)
		      log_L_train += log(gauss((double)i, old_mu[2], 
					       old_Sigma[2]));
		  train_count += H_train[i];	
		}
	    }
	  if (train_count != (n_1 + n_2))
	    Message(1, "[EM] Error! train_count = %i, n_1 = %i, n_2 = %i\n",
		    train_count, n_1, n_2);
	}
      log_L_uncl = 0.0;
      for (i=0; i<256; i++)
	{	
	  if (H[i] > 0)
	    {
	      sum = 0.0;
	      for (class = 1; class <=2; class++)
		sum += H[i]*old_pi[class] * 
		  gauss((double)i, old_mu[class], old_Sigma[class]);
	      log_L_uncl += log(sum);
	    }
	}
/*
      printf ("log L train. = %10.5f, log L uncl. = %10.5f, ", 
	      log_L_train, log_L_uncl);
*/
      log_L = log_L_train + log_L_uncl;
/*
      printf ("log L = %10.5f\n", log_L);
*/
      count++;
      if (fabs(old_log_L-log_L) < EM_LOG_L_LIMIT)
	convergence = TRUE;
      else if (count > 50)
	convergence = TRUE;
      old_log_L = log_L;
    }

  
/*
  for (c=1; c<=2; c++)
    printf("class %1i: pi[%i]=%8.3f, mu[%i]=%7.3f, sigma[%i]=%7.3f\n",
	    c, c, old_pi[c], c, old_mu[c], c, old_Sigma[c]);
*/

  mean_1[0] = new_mu[1];
  mean_2[0] = new_mu[2];
  sdev_1[0] = sqrt(new_Sigma[1]);
  sdev_2[0] = sqrt(new_Sigma[2]);
  pi_2[0] = new_pi[2];
  
  return 1;
}
