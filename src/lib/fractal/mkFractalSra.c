
/*C*

________________________________________________________________

        mkFractalSra
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/fractal.h>
#include <xite/readarg.h>
#include <xite/utils.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif


#define f3(std,x0,x1,x2)    (UNS_BYTE)(((x0)+(x1)+(x2)     )/3.0 + gaussRandom(0.5, std));
#define f4(std,x0,x1,x2,x3) (UNS_BYTE)(((x0)+(x1)+(x2)+(x3))/4.0 + gaussRandom(0.5, std));
/* ADDITION = 0 will be faster but give poorer result */
#define ADDITION 1



/*F:mkFractalSra*

________________________________________________________________

		mkFractalSra
________________________________________________________________

Name:		mkFractalSra - make fractal brownian motion noise

Syntax:         | #include <xite/fractal.h>
		|
                | IBAND mkFractalSra( double D, int xpow,
                |    int ypow, double mean, double cstdev,
                |    double stdev );
Description:    Generates fractional brownian motion noise
                based on the fractal dimension D as input.
		The algorithm is known as "Successive Random Addition"
		The generated band will have horizontal size 2**xpow + 1
                and vertical size  2**ypow + 1.

		| D        - fractional dimension (between 2.0 and 3.0)
		| xpow     - horizontal size: 2**xpow + 1
		| ypow     - vertical size: 2**ypow + 1
		| mean     - mean pixel value for corners
		| cstdev   - standard deviation for corners
		| stdev    - initial standard deviation for the rest
		|            of the image

		Note that 'mean' far from 128 combined with large
		standard deviations will give a lot of over/under-flows,
		and a very poor result. 'cstdev' should be small to avoid
		"tilting" the whole image.

Return value:   The generated band

Error handling: return (IBAND)0 in case of error:
                | Couldn't allocate memory
		| D not between 2.0 and 3.0
		| xpow or ypow below 1
		| mean outside 0.0 .. 255.0
		| negative cstdev or stdev

Example:        | IBAND b;
                | b = mkFractalSra(2.5, 7, 8, 100.0, 5.0, 25.0)

Reference:      &'Fournier, A., Fussel, D., and Carpenter, L.'
                "Computer rendering of stochastic models",
		Commun. ACM 25, 6 (June 1982), 372-382.

See also:       mkFractalSra(1)

Author:		Håvard Iversen, BLAB, Ifi, UiO

Modifyed by:    Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
IBAND mkFractalSra(D, xpow, ypow, mean, cstdev, stdev)
double D; 
int xpow, ypow;
double  mean, cstdev, stdev;
#else /* FUNCPROTO */
IBAND mkFractalSra(double D, int xpow, int ypow, double mean, double cstdev, double stdev)
#endif /* FUNCPROTO */
{
  IBAND band;
  double H;
  int x, y, xsize, ysize, dxy, Dxy, stage, maxlevel;

  if ((xpow < 1) || (ypow < 1)) return((IBAND)0);
  if ((D < 2.0) || (D > 3.0)) return((IBAND)0);
  if ((mean < 0.0) || (mean > 255.0)) return((IBAND)0);
  if ((cstdev < 0.0) || (stdev < 0.0)) return((IBAND)0);
  xsize = (1<<xpow)+1;
  ysize = (1<<ypow)+1;
  band = Imake_band(Iu_byte_typ, xsize, ysize);
  if (!band) return((IBAND)0);

  mean += 0.5; /* to compensate for truncation to byte */
  srandom(time(0));
  maxlevel = MAX(xpow, ypow);
  H = 3.0-D;
  band[1][1]         = (UNS_BYTE)(gaussRandom(mean,cstdev));
  band[1][xsize]     = (UNS_BYTE)(gaussRandom(mean,cstdev));
  band[ysize][1]     = (UNS_BYTE)(gaussRandom(mean,cstdev));
  band[ysize][xsize] = (UNS_BYTE)(gaussRandom(mean,cstdev));
  Dxy = MIN(xsize-1, ysize-1);
  dxy = MIN(xsize/2, ysize/2);

  /* DIRTY */
  for (y=1; y <= ysize; y++) band[y]++;
  band++;
  /* END OF DIRTY:matrix is now indexable from 0 */

  for (stage = 1; stage <= maxlevel; stage++ )
  {
    /* going from grid type I to II */
    stdev = stdev*pow((double)0.5,(double)(0.5*H));

    /* interpolate and offset points */
    for(y = dxy; y < ysize-dxy; y += Dxy )
      for (x = dxy; x < xsize-dxy; x += Dxy )
        band[y][x] = f4(stdev,band[y+dxy][x+dxy], band[y+dxy][x-dxy],
                         band[y-dxy][x+dxy], band[y-dxy][x-dxy]);

    /* displace other points also if needed */
    if(ADDITION) 
      for( y=0; y < ysize; y += Dxy )
        for( x=0; x < xsize; x += Dxy )
          band[y][x] = (UNS_BYTE)((double)band[y][x] + gaussRandom((double)0.5, stdev));

    /* going from grid type II to type I */
    stdev = stdev*pow((double)0.5,(double)(0.5*H));

    for(x=dxy; x < xsize-dxy; x += Dxy) {
      band[0][x] = f3(stdev,band[0][x+dxy],band[0][x-dxy],band[dxy][x]);
      band[ysize-1][x] = f3(stdev,band[ysize-1][x+dxy],band[ysize-1][x-dxy],band[ysize-1-dxy][x]);
    }
    for(y=dxy; y < ysize-dxy; y += Dxy) {
      band[y][0] = f3(stdev,band[y+dxy][0],band[y-dxy][0],band[y][dxy]);
      band[y][xsize-1] = f3(stdev,band[y+dxy][xsize-1],band[y-dxy][xsize-1],band[y][xsize-1-dxy]);
    }

    /* interpolate and offset interior grid points */
    for (y=dxy; y < ysize-dxy; y += Dxy)
      for (x=Dxy; x < xsize-dxy; x += Dxy)
         band[y][x] = f4(stdev,band[y][x+dxy],band[y][x-dxy],band[y+dxy][x],band[y-dxy][x]);
    for (y=Dxy; y < ysize-dxy; y += Dxy)
      for (x=dxy; x < xsize-dxy; x += Dxy)
         band[y][x] = f4(stdev,band[y][x+dxy],band[y][x-dxy],band[y+dxy][x],band[y-dxy][x]);

    /* displace other points also if needed */
    if(ADDITION)  {
      for (y=0; y < ysize; y += Dxy)
        for (x=0; x < xsize; x += Dxy)
          band[y][x] = (UNS_BYTE)((double)band[y][x] + gaussRandom((double)0.5, stdev));
      for (y=dxy; y < ysize-dxy; y += Dxy)
        for (x=dxy; x < xsize-dxy; x += Dxy)
          band[y][x] = (UNS_BYTE)((double)band[y][x] + gaussRandom((double)0.5, stdev));
    }

    Dxy = Dxy/2;
    dxy = dxy/2;
  }
  /* DIRTY CORRECTION*/
  band--;
  for (y=1; y LE ysize; y++) band[y]--;
  /* END OF DIRTY CORRECTION:matrix is now indexable from 1 */            

  return(band);
}
