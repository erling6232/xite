

/*

________________________________________________________________

        bin_ab.c
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

/*F:bin_ab*

________________________________________________________________

                bin_ab
________________________________________________________________


Name:           bin_ab - binarize a gray-scale band using 
                Abutaleb`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_ab( IBAND inband, IBAND outband );

Description:    The input band 'inband' is binarized using 
                the method of Abutaleb, the result returned
		in 'outband'.

Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

See also:       binarize(1)

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Return value:   
References:     &Ahmed S. Abutaleb
                "Automatic thresholding of gray-level pictures using
		two-dimensional entropy",
		Computer Vision, Graphics, and Image Processing,
		vol 47, pp 22-32, 1989.

Author:         Øivind Due Trier

Id:             $Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <math.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/stdiff.h>
#include <xite/binarize.h>
#include <xite/message.h>

#ifdef FUNCPROTO
int bin_ab (IBAND inband, IBAND outband)
#else
int bin_ab (inband, outband)
IBAND inband;
IBAND outband;
#endif
{
  int xsize, ysize;
  int x, y, a, b;
  int s, t;
  int s_max = 0;
  int t_max = 0;
  int threshold;
  int avg_thres;
  double H_sum, P_sum;
  double Phi, Phi_max;
  IMAGE average_im;
  II_IMAGE histogram_im;
  IBAND average;
  II_BAND histogr_band;
  ID_BAND p;
  ID_BAND P, H;
  double p_ln_p;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_ab: Input pixel type must be unsigned band.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_ab: Output pixel type must be unsigned band.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  average_im   = Imake_image(1, "Average", Iu_byte_typ, xsize, ysize);
  histogram_im = (II_IMAGE)Imake_image(1, "2D histogram", 
				       Integer_typ, 256, 256);
  average      = average_im[1];
  histogr_band = histogram_im[1];
  p = (ID_BAND) Imake_band(Idouble_typ, 256, 256);
  P = (ID_BAND) Imake_band(Idouble_typ, 256, 256);
  H = (ID_BAND) Imake_band(Idouble_typ, 256, 256);

  mean(inband, average, 3, 3);

  for (a = 1; a <= 256; a++)
    for (b = 1; b <= 256; b++)
      histogr_band [b][a] = 0;

  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      {
	a = inband[y][x] + 1;  /* pixel values in (0..255) */
	b = average[y][x] + 1; /* IBAND adressed by (1..256) */
	histogr_band[b][a] += 1;
      }

  for (a = 1; a <= 256; a++)
    for (b = 1; b <= 256; b++)
      p[b][a] = histogr_band [b][a] * 1.0 / (xsize*ysize);

  P_sum = 0.0;
  for (s=1; s<= 256; s++)
    {
      P_sum += p[1][s];
      P[1][s] = P_sum;
    }
  for (t=1; t<=256; t++)
    {
      P_sum = 0.0;
      for (s=1; s<= 256; s++)
	{
	  P_sum += p[t][s];
	  P[t][s] = P[t-1][s] + P_sum;
	}
    }

  H_sum = 0.0;
  for (s=1; s<= 256; s++)
    {
      p_ln_p = (p[1][s] == 0) ? 0 : (p[1][s] * log(p[1][s]));
      H_sum -=  p_ln_p;
      H[1][s] = H_sum;
    }
  for (t=1; t<=256; t++)
    {
      H_sum = 0.0;
      for (s=1; s<= 256; s++)
	{
	  p_ln_p = (p[t][s] == 0)? 0 : (p[t][s] * log(p[t][s]));
	  H_sum -= p_ln_p;
	  H[t][s] = H[t-1][s] + H_sum;
	}
    }

  if (Verbose()) printf("P[256][256] = %f\n", P[256][256]);

  Phi_max = -1e20;
  for (s=1; s<=256; s++)
    {
      for (t=1; t<=256; t++)
	{
	  if ((P[t][s] > BIN_TINY) && ((1-P[t][s]) > BIN_TINY))
	    {
	      Phi = log( P[t][s]*(1-P[t][s]) ) 
		+ H[t][s]/P[t][s] 
		+ ( H[256][256] - H[t][s] ) / ( 1-P[t][s] );

	      if (Phi > Phi_max)
		{
		  Phi_max = Phi;
		  s_max = s;
		  t_max = t;
		}
	    }
	}
    }
	
  threshold = s_max-1;
  avg_thres = t_max-1;

  if (Verbose())
    printf("2D threshold found, s=%i, t=%i\n", threshold, avg_thres);
    

  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if ((inband[y][x] <= threshold) && (average[y][x] <= avg_thres))
	outband[y][x] = 0;
      else	
	outband[y][x] = 255;

  return 0;
}

