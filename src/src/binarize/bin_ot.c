

/*

________________________________________________________________

        bin_ot.c
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

/*F:otsu_threshold=bin_ot*/
/*F:bin_ot*

________________________________________________________________

                bin_ot
________________________________________________________________


Name:           bin_ot, otsu_threshold - binarize a gray-scale band using 
                Otsu`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_ot( IBAND inband, IBAND outband );
                | int otsu_threshold( IBAND inband, double* mu_0,
                |    double* mu_1, double* mu );

Description:    The input band 'inband' is binarized by 'bin_ot' using 
                the method of Otsu. The result is returned in 'outband'.

		'otsu_threshold' returns Otsu`s threshold for 'inband'.
		The total mean 'mu' and the two cluster means 'mu1' 
		and 'mu2' are calculated.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1)

References:     &'Nobuyuki Otsu'
                "A threshold selection method from gray-level histograms",
		IEEE transactions on Systems, man and cybernetics,
		vol T-SMC 9, no 1, pp 62-66, 1979.

Return value:   | bin_ot         :  0 : OK
                |                   1 : Bad input pixel type
		|                   2 : Bad output pixel type
                | otsu_threshold :  Otsu`s threshold

Files:  	src/binarize/bin_ot.c
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

#ifdef FUNCPROTO
int otsu_threshold(IBAND inband, double *mu_0, double *mu_1, double *mu)
#else
int otsu_threshold(inband, mu_0, mu_1, mu)
     IBAND inband;
     double *mu_0;
     double *mu_1;
     double *mu;
#endif
{
  histogram histo;
  int threshold;
  double criterion;
  double expr_1;
  int i, k;
  double p[256];
  double omega_k;
  double sigma_b_k;
  double sigma_T;
  double mu_T;
  double mu_k;
  int sum;
  int k_low, k_high;

  mkHisto(inband, histo);

  sum = 0;
  for (i = 0; i < 256; i++)
    sum += histo[i];

  for (i=0; i<256; i++)
    p[i] = histo[i] * 1.0 / sum;

  mu_T = 0.0;
  for (i=0; i<256; i++)
    mu_T += i*p[i];

  sigma_T = 0.0;
  for (i=0; i<256; i++)
    sigma_T += (i-mu_T)*(i-mu_T)*p[i];

  
  for (k_low = 0; (p[k_low] == 0) && (k_low < 255); k_low++);
  for (k_high =255; (p[k_high] == 0) && (k_high > 0); k_high--);

  criterion = 0.0;
  threshold = 127;
  *mu_0 = 126.0;
  *mu_1 = 128.0;

  omega_k = 0.0;
  mu_k = 0.0;
  for (k = k_low; k <= k_high ; k++)
    {
      omega_k += p[k];
      mu_k += k*p[k];

      expr_1 = (mu_T*omega_k - mu_k);
      sigma_b_k = expr_1 * expr_1 / (omega_k*(1-omega_k));
      if (criterion < sigma_b_k/sigma_T)
	{
	  criterion = sigma_b_k/sigma_T;
	  threshold = k;
	  *mu_0 = mu_k/omega_k;
	  *mu_1 = (mu_T-mu_k)/(1-omega_k);
	}
    }
  *mu = mu_T;
  return threshold;
}


#ifdef FUNCPROTO
int bin_ot (IBAND inband, IBAND outband)
#else
int bin_ot (inband, outband)
     IBAND inband;
     IBAND outband;
#endif
{
  int xsize, ysize;
  int x, y;
  int threshold;
  double mu_0, mu_1, mu;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_ot: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_ot: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  threshold = otsu_threshold(inband, &mu_0, &mu_1, &mu);

  if (Verbose()) 
    printf(
       "Otsu's method. Threshold = %i, mean_0 = %f, mean_1 = %f, mean = %f\n", 
       threshold, mu_0, mu_1, mu);

  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (inband[y][x] <= threshold)
	outband[y][x] = 0;
      else	
	outband[y][x] = 255;

  return 0;
}




