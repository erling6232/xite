
/*

________________________________________________________________

        thresMinErr
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>
#include XITE_STRING_H



#ifndef MAIN

/*F:thresMinErrIter*

________________________________________________________________

		thresMinErrIter
________________________________________________________________

Name:		thresMinErrIter - minimum error threshold, iteration method

Syntax:         | #include <xite/threshold.h>
		|
                | int thresMinErrIter( histogram h, int tinit );
Description:	'thresMinErrIter' calculates the threshold value by using
                the minimum error thresholding technique as in Kittler & 
                Illingworth (2nd method) and Gonzalez & Wintz.
		Initially, the threshold value t is set according to tinit.

		| 0 <= tinit <= 255, then t = tinit
		| tinit = -1, then t = the average of all the pixels
		| tinit = -2, then t = the result from thresRidCal(h)
		
		Iteratively, the new threshold is found by solving the equation
		(7.3-13) in Gonzalez & Wintz.
		The variances and the means are the variances and means above
		and belov the treshold. When the threshold value 
		remains constant, iteration terminates.

		The actual thresholding is not performed, see the
		routine "threshold".
		
Reference:      | J. Kittler & J. Illingworth: "Minimum Error Thresholding"
                  Pattern Recognition, Vol 19, nr 1. 1986, pp. 41-47.
                | Gonzales, Rafael C & Wintz, Paul:
		| "Digital Image Processing"
		| Addison-Wesley, 1977.
		| Q.-Z. Ye & P.-E. Danielsson:
		| "On Minimum Error Thresholding and Its Implementations"
		| Pattern Recognition Letters 7,
		| 1988, pp. 201-206.

Return value:	The threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Olav Borgli, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresMinErrIter(int *h, int tinit)
{
  int i, iter, t, newt, oldt, eps1, eps2;
  double mean1, mean2, var1, var2, sigma1, sigma2, A, B, sq, newt1, newt2;
  double hn[256], mu[256], cum[256];

  /* find norm histogram and cum histogram */
  cum[0] = h[0];
  for (i=1; i<=255; i++) cum[i] = cum[i-1] + h[i];
  for (i=0; i<=255; i++)
    { hn[i] = h[i]/cum[255];
      cum[i] = cum[i]/cum[255];
    }

  /* find mean value */
  mu[0] = 0.0;
  for (i=1; i<=255; i++)
    mu[i] = mu[i-1] + hn[i]*i;

  /* initial threshold value */  
  if ((tinit >= 0) AND (tinit <= 255)) t = tinit;
  else if (tinit == -1) t = (int) mu[255];
  else if (tinit == -2) t = thresRidCal(h);
  else t = (int) mu[255]; /* not legal argument value, using mean */
  
  oldt = 0;

  eps1 = 10; eps2 = 10; iter = 0;

  if (Verbose()) printf("tinit=%d, t=%d \n", tinit, t); 

  while (eps1!=0 && eps2!=0 && iter<30)
    {
      mean1 = mu[t]/cum[t];
      mean2 = (mu[255] - mu[t])/(1-cum[t]);

      var1 = 0;
      for (i=0; i<=t; i++)
	var1 += (i-mean1)*(i-mean1)*hn[i];
      var1 = var1/cum[t];
      sigma1 = sqrt(var1);

      var2 = 0;
      for (i=t+1; i<=255; i++)
	var2 += (i-mean2)*(i-mean2)*hn[i];
      var2 = var2/(1-cum[t]);
      sigma2 = sqrt(var2);

      if (Verbose())
	printf("m1=%.2f, m2=%.2f, s1=%.2f, s2=%.2f", mean1,mean2,var1,var2);

      A = var1 - var2 ;
      B = 2*(mean1*var2 - mean2*var1);
      sq = B*B - 4*A* (var1*mean2*mean2 - var2*mean1*mean1 +
		       2*var1*var2* log(sigma2*cum[t]/(sigma1*(1-cum[t]))));

      newt1 = (-B + sqrt(sq)) / (2*A);
      newt2 = (-B - sqrt(sq)) / (2*A);

      newt = (int) ( fabs(newt1-t)<fabs(newt2-t) ? newt1 : newt2 );

      if (Verbose()) printf("  t1=%.2f t2=%.2f t=%d \n", newt1, newt2, newt);

      eps1 = t - newt;  eps2 = newt - oldt;
      oldt = t;  t = newt;
      iter++;
    }
  if (Verbose()) printf("t = %d \n", t);
  return(t);
}



/*F:thresMinErrCrit*

________________________________________________________________

		thresMinErrCrit
________________________________________________________________

Name:		thresMinErrCrit - minimum error threshold,
                                   criterion function method

Syntax:         | #include <xite/threshold.h>
		|
                | int thresMinErrCrit( histogram h );
Description:	'thresMinErrCrit' calculates the threshold value by using
                the minimum error thresholding technique as in Kittler & 
                Illingworth (1nd method).

		The threshold is found by finding the minimum of the
		criterion function (15) in Kittler & Illingworth.
		The variances are the variances above
		and belov the treshold.

		The actual thresholding is not performed, see the
		routine "threshold".
		
Reference:      | J. Kittler & J. Illingworth: "Minimum Error Thresholding"
                  Pattern Recognition, Vol 19, nr 1.

Return value:	The threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Olav Borgli, BLAB, ifi, UiO
Bug fix:        Svein Bøe

Id:             $Id$
________________________________________________________________

*/

int thresMinErrCrit(int *h)
{
  int i, t, tmin;
  double mean1, mean2, var1, var2, sigma1, sigma2, P1, P2, j, jmin;
  double hn[256], mu[256], cum[256];

  jmin = DOUBLE_MAX;

  /* find norm histo and cum histo */
  cum[0] = h[0];
  for (i=1; i<=255; i++)
    cum[i] = cum[i-1] + h[i];
  for (i=0; i<=255; i++)
    { hn[i] = h[i]/cum[255];
      cum[i] = cum[i]/cum[255];
    }

  /* find mean value */
  mu[0] = 0.0;
  for (i=1; i<=255; i++)
    mu[i] = mu[i-1] + hn[i]*i;

  for (t=0; t<256; t++)
    {
      P1 = cum[t];

      if (P1 == 1) break;
      if (P1 != 0) {
	P2 = 1-cum[t];
	mean1 = mu[t]/P1;

	mean2 = (mu[255] - mu[t]) / P2;
	
	var1 = 0;
	for (i=0; i<=t; i++)
	  var1 += (i-mean1)*(i-mean1)*hn[i];
	sigma1 = sqrt(var1/P1);
	
	var2 = 0;
	for (i=t+1; i<=255; i++)
	  var2 += (i-mean2)*(i-mean2)*hn[i];
	sigma2 = sqrt(var2/P2);
	
	if (sigma1>0 && sigma2>0 && P1 && P2)
	  {
	    j = 1 + 2*(P1*log(sigma1) + P2*log(sigma2))
	      - 2*(P1*log(P1) + P2*log(P2));
	  }
	else j = 0;
	
	if (j<jmin && j)
	  {
	    jmin = j;
	    tmin = t;
	  }
	
	if (Verbose()) printf("*** %3d:%-8.2f ***", t, j);
      } /* if P1 */
    }
  if (Verbose()) printf("\nMinimum  %d:%.4f \n", tmin, jmin);

  return(tmin);
}



/*F:thresMinErrEqVar*

________________________________________________________________

		thresMinErrEqVar
________________________________________________________________

Name:		thresMinErrEqVar - minimum error thresholding 
                                        with equal variances. 

Syntax:         | #include <xite/threshold.h>
		|
                | int thresMinErrEqVar( histogram h, int tinit );
Description:	'thresMinErrEqVar' calculates the threshold value by using
                the minimum error thresholding technique with equal variances
                as in Gonzalez and Wintz.
		Initially, the threshold value t is set according to tinit.

		| 0 <= tinit <= 255, then t = tinit
		| tinit = -1, then t = the average of all the pixels
		| tinit = -2, then t = the result from thresRidCal(h)

		Then, iteratively, the threshold is set to the average of
		the mean of the pixels below the threshold, and the
		mean of the pixels above the threshold , plus a
		compensation factor depending on the variance of the
		distribution. This variance is the average of the variance 
		below and above the threshold. When the threshold value 
		remains constant, iteration terminates.

		The actual thresholding is not performed, see the
		routine "threshold".
		
Reference:      Gonzales, Rafael C & Wintz, Paul: "Digital Image Processing"
                Addison-Wesley, 1977.

Return value:	The threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Olav Borgli, Fritz Albregtsen, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresMinErrEqVar(int *h, int tinit)
{ 
  int i, t, newt, oldt, iter, eps, eps2;
  float mean1, mean2, var1, var2, var, w1, w2, P1, P2;
  float hn[256], mu[256], cum[256];

  /* find norm histo and cum histo */
  cum[0] = h[0];
  for (i=1; i<=255; i++)
    cum[i] = cum[i-1] + h[i];
  for (i=0; i<=255; i++)
    { hn[i] = h[i]/cum[255];
      cum[i] = cum[i]/cum[255];
    }

  /* find mean value */
  mu[0] = 0.0;
  for (i=1; i<=255; i++)
    mu[i] = mu[i-1] + hn[i]*i;

  /* initial threshold value */  
  if ((tinit >= 0) AND (tinit <= 255)) t = tinit;
  else if (tinit == -1) t = (int) mu[255];
  else if (tinit == -2) t = thresRidCal(h);
  else t = (int) mu[255]; /* not legal argument value, using mean */

  if (Verbose()) printf("t=%i \n",t);

  w1 = 0.5; w2 = 0.5;
  oldt = 0;
  eps = 10; eps2 = 10; iter = 0; /* initiellt */

  while (eps!=0 && eps2!=0 && iter<30)
    {
      P1 = cum[t]; P2 = 1-cum[t];

      mean1 = mu[t]/P1;
      mean2 = (mu[255] - mu[t])/P2;

      if (Verbose()) printf("%i: m1=%.2f m2=%.2f;   ", iter, mean1, mean2);

      var1 = 0;
      for (i=0; i<=t; i++)
	var1 += (i-mean1)*(i-mean1)*hn[i];
      var1 = var1/P1;

      var2 = 0;
      for (i=t+1; i<=255; i++)
	var2 += (i-mean2)*(i-mean2)*hn[i];
      var2 = var2/P2;

      var = w1*var1 + w2*var2;

      if (Verbose()) printf("s1=%.2f, s2=%.2f, s=%.2f;    ", var1, var2, var);

      newt = (int) (0.5*(mean1+mean2) +
		   var*log(P2/P1)/(mean1-mean2));
      
      eps = t - newt;  eps2 = newt - oldt;
      oldt = t;  t = newt;
      iter++;

      if (Verbose()) printf("t=%i \n",t);
    }

  return(t);
}

#endif /* not MAIN */



#ifdef MAIN

/*P:thresMinErr*

________________________________________________________________

		thresMinErr
________________________________________________________________

Name:		thresMinErr - minimum error threshold.

Syntax:		| thresMinErr [<switches>] <inimage> [<outimage>]

Description:	'thresMinErr' calculates, and eventually applies,
		the threshold value needed to threshold the 
                image according to the minimum error thresholding metods
                as described in Kittler & Illingworth and Gonzalez & Wintz.

		| -iter      - use iterative methode
		|              (thresMinErrIter)
		| -crit      - use criterion function method
		|              (thresMinErrCrit)
		| -eqVar     - use equal variances method
		|              (thresMinErrEqVar)

		| -mean      - initial threshold is mean (default)
		| -t <tinit> - initial threshold 0<=tinit<=255
		| -ridcal    - initial threshold = thresRidCal(h)

		| -l <lpv>   - output for low pixels (below thrs.).
		|              Default 0
		| -h <hpv>   - output for high pixels (above thrs.).
		|              Default 255

		| inimage  - input image
		| outimage - output image

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

Reference:      | J. Kittler & J. Illingworth: "Minimum Error Thresholding"
                | Pattern Recognition, Vol 19, nr 1. 1986, pp. 41-47.
		| Gonzales, Rafael C & Wintz, Paul:
		| "Digital Image Processing"
                | Addison-Wesley, 1977.
		| Q.-Z. Ye & P.-E. Danielsson:
		| "On Minimum Error Thresholding and Its Implementations"
		| Pattern Recognition Letters 7,
		| 1988, pp. 201-206.
 
Restrictions:	Only UNS_BYTE pixels are supported

Author:		Olav Borgli, BLAB, ifi, UiO

Examples:	| thresMinErr -iter -mean mona.img monaThres.img
		| thresMinErr -iter mona.img monaThres.img
		| thresMinErr -crit mona.img monaThres.img
		| thresMinErr -eqVar -t 112 mona.img

Id:             $Id$
________________________________________________________________
*/

int main(int argc, char **argv)
{
   IMAGE i;
   histogram h;
   int t, tinit, bn, lpv, hpv, output, j;
   int f_iter, f_crit, f_eqVar, f_mean, f_ridcal;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-iter] [-crit] [-eqVar] [-mean] [-t <tinit>] [-ridcal]\n\
       [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv      = read_iswitch(&argc, argv, "-l", 0);
   hpv      = read_iswitch(&argc, argv, "-h", 255);

   f_iter   = read_bswitch(&argc, argv, "-iter");
   f_crit   = read_bswitch(&argc, argv, "-crit");
   f_eqVar  = read_bswitch(&argc, argv, "-eqVar");

   f_mean   = read_bswitch(&argc, argv, "-mean");
   tinit    = read_iswitch(&argc, argv, "-t", -1);
   f_ridcal = read_bswitch(&argc, argv, "-ridcal");

   if ((argc < 2) OR (argc > 3)) Usage(1, "Illegal number of arguments.\n");

   /* Choose what initial threshold value to use */
   if (tinit != (-1)) {
     if (tinit<0 || tinit>255) {
       Error(2, "-t value was out of bounds.\n");
     }
   }
   else if (f_ridcal)  tinit=(-2);
   else tinit=(-1);

   i = Iread_image(argv[1]);
   output = (argc == 3);
   if (output) Ihistory(i, argv[0], args);

   for (bn=1; bn<=Inbands(i); INC bn) {
     mkHisto(i[bn], h);
     if      (f_crit)   t = thresMinErrCrit(h);
     else if (f_iter)   t = thresMinErrIter(h,tinit);
     else if (f_eqVar)  t = thresMinErrEqVar(h,tinit);
     else {
       Error(2, "No method selected.\n");
     }
     if (output) {
       thresholdSpOut(i[bn], i[bn], t, lpv, hpv);
     }
     else Message(0, "Threshold value in band %d: %d\n", bn, t);
   }
   if (output) Iwrite_image(i, argv[2]);
   return(0);
 }

#endif /* MAIN */
