

/*

________________________________________________________________

        bin_ksw.c
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

/*F:kapur_threshold=bin_ksw*/
/*F:bin_ksw*

________________________________________________________________


________________________________________________________________


Name:           bin_ksw, kapur_threshold - binarize a gray-scale band using 
                Kapur, Sahoo and Wong`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_ksw( IBAND inband, IBAND outband );
                | int kapur_threshold( IBAND inband );

Description:    The input band 'inband' is binarized using 
                the method of Kapur, Sahoo and Wong, the result returned
		in 'outband'.

		'kapur_threshold' finds the threshold used in 'bin_ksw'.

Files:  	src/binarize/bin_ksw.c
See also:       binarize(1)

References:     &'Kapur, Sahoo & Wong'
                "A new method for Gray-level picture 
		thresholding using the entropy of the histogram",
		Computer Vision, Graphics, and Image Processing,
		vol 29, pp 273-285, 1985.
Return value:   For 'bin_ksw': 0 - ok
Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.
Author:         Oivind Due Trier
Examples:       
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

#ifdef FUNCPROTO
int kapur_threshold(IBAND inband)
#else
int kapur_threshold(inband)
     IBAND inband;
#endif
{
  histogram histo;
  int threshold;
  double Phi, Phi_max;
  int i, k;
  double p[256];
  double P[256];
  double H[256];
  double expr1, expr2, expr3;
  int sum;

  mkHisto(inband, histo);

  sum = 0;
  for (i = 0; i < 256; i++)
    sum += histo[i];

  for (i=0; i<256; i++)
    p[i] = histo[i] * 1.0 / sum;

  P[0] = p[0];
  for (i=1; i<256; i++)
    P[i] = P[i-1] + p[i];

  if (histo[0] == 0)
    H[0] = 0;
  else
    H[0] = -p[0] * log(p[0]);

  for(i=1; i<256; i++)
    {
      if (histo[i] == 0)
	H[i] = H[i-1];
      else 
	H[i] = H[i-1] - p[i] * log(p[i]);
    }
  
  Phi_max = -10e10;
  threshold = 127;

  for (k = 0; k <= 255 ; k++)
    {
      if ((P[k] > BIN_TINY) && (1-P[k] > BIN_TINY))
	{
	  expr1 = log(P[k]*(1-P[k])) ;
	  expr2 = H[k]/P[k] ;
	  expr3 = (H[255]-H[k])/(1.0-P[k]);
	  Phi = log(P[k]*(1-P[k])) 
	    + H[k]/P[k] 
	      + (H[255]-H[k])/(1.0-P[k]);

	  if (Phi_max < Phi)
	    {	
	      Phi_max = Phi;
	      threshold = k;
	    }
	}
    }
  return threshold;
}



#ifdef FUNCPROTO
int bin_ksw (IBAND inband, IBAND outband)
#else
int bin_ksw (inband, outband)
     IBAND inband;
     IBAND outband;
#endif
{
  int xsize, ysize;
  int x, y;
  int threshold;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  threshold = kapur_threshold(inband);

  if (Verbose()) printf("Kapur, Sahoo and Wong's method. Threshold = %i\n", 
			threshold);

  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (inband[y][x] <= threshold)
	outband[y][x] = 0;
      else	
	outband[y][x] = 255;

  return 0;
}




