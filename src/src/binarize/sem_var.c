

/*

________________________________________________________________

        sem_var.c
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




/*L:sem_var*

________________________________________________________________

		sem_var
________________________________________________________________

Name:		sem_var - computes the semivariogram of an image
Syntax:		double sem_var (II_BAND inband, int dx, int dy)
Description:	Computes the semivariogram of an image. ('dx','dy') is the
                direction vector 'h'. The semivariogram is calculated
		as the covariance of pixels separated by the vector
		'h'. Thus, the semivariogram will reveal directional
		structure. 

		'sem_var' is used as part of Mardia and Hainsworth`s
		binarization method.
Files:		src/binarize/sem_var.c
References:	'K.V. Mardia & T.J. Hainsworth' "A spatial thresholding method
		for image segmentation", IEEE Transactions on Pattern
		Analysis and Machine Intelligence, vol T-PAMI 10, no 6, 
		pp 919-927, 1988.
Diagnostics:
Restrictions:
Return value:
Author:		Oivind Due Trier
Examples:	| gamma_1 = 0.5  * (  sem_var(inband,  1, 0) 
                |                   + sem_var(inband,  0, 1));

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
 /* #include XITE_STDIO_H */

#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <math.h>
 

#ifdef FUNCPROTO
static double sem_var (II_BAND inband, int dx, int dy)
#else
static double sem_var (inband, dx, dy)
     II_BAND inband;
     int dx;
     int dy;
#endif
{
  int x, y, xsize, ysize;
  int xstart, ystart, xend, yend;
  long n, d, sum;

  xsize = Ixsize((IBAND)inband);
  ysize = Iysize((IBAND)inband);
  if (dx < 0)
    {
      xstart = 1-dx;
      xend = xsize;
    }
  else
    {
      xstart = 1;
      xend = xsize-dx;
    }
  if (dy < 0)
    {
      ystart = 1-dy;
      yend = ysize;
    }
  else
    {
      ystart = 1;
      yend = ysize-dy;
    }
  sum = 0;
  n = 0;
  for (x=xstart; x<=xend; x++)
    for (y=ystart; y<=yend; y++)
      {
	d = inband[y][x] - inband[y+dy][x+dx];
	sum += d*d;
	n++;
      }

  return ((1.0/n) * sum) / 2.0;
}
