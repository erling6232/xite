
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_TIME_H
 #include <time.h>
#endif
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#ifndef MAIN

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

Author:		H�vard Iversen, BLAB, Ifi, UiO

Modifyed by:    Tor L�nnestad, BLAB, Ifi, UiO

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

#endif /* not MAIN */

/*P:mkFractalSra*

________________________________________________________________

		mkFractalSra
________________________________________________________________

Name:		mkFractalSra - make fractal brownian motion noise

Syntax:		mkFractalSra [-sw] <output image> <D> [<xsize> [<ysize>]]

Description:    Generates fractional brownian motion noise
                with fractal dimension D. The algorithm is known
		as "Successive Random Addition"

                |   output image - name of generated image
                |   D            - fractal dimension between 2. and 3.
		|   xsize        - xsize of synthesized image
		|                  (default 512)
		|   ysize        - ysize of synthesized image
		|                  (default xsize)
		Switches:
		|   -mean        - mean pixel value for corners
		|                  (default 128.0)
		|   -cstd        - standard deviation for corners
		|                  (default 5.0)
		|   -std         - init. st. dev. for other pixels
		|                  (default 25.0)

		Note that 'mean' far from 128 combined with large
		standard deviations will give a lot of over/under-flows,
		and a very poor result. 'cstdev' should be small to avoid
		"tilting" the whole image.

See also:	mkGauss(1), mkFractalSra(3)

Author:		H�vard Iversen, BLAB, Ifi, UiO

Modifyed by:    Tor L�nnestad, BLAB, Ifi, UiO

Reference:      &'Fournier, A., Fussel, D., and Carpenter, L.'
                "Computer rendering of stochastic models",
		Commun. ACM 25, 6 (June 1982), 372-382.

Example:        | mkFractalSra -mean 100.0 fractal.img 2.5 200 400 

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IBAND b;
  IMAGE i;
  double D, mean, cstdev, stdev;
  int xsize, ysize, size, xpow, ypow;
  char arg[80];

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<options>] <file> <D> [<xsize> [<ysize>]] \n\
       where <D> is fractal dimension between 2.0 and 3.0 \n\
       and where <options> are \n\
	-mean <mean> - mean pixel value for corners (default 128.0)\n\
	-cstd <cstd> - standard deviation for corners (default 5.0)\n\
	-std  <std>  - init. st. dev. for other pixels (default 25.0)\n"));

  if (argc == 1) Usage(1, NULL);

  mean   = read_dswitch(&argc, argv, "-mean", 128.0);
  cstdev = read_dswitch(&argc, argv, "-cstd",   5.0);
  stdev  = read_dswitch(&argc, argv, "-std",   25.0);

  if ((argc < 3) || (argc > 5)) Usage(1, "Illegal number of arguments.\n");

  D = atof(argv[2]);
  xsize = (argc > 3) ? (atoi(argv[3])) : 512;
  ysize = (argc > 4) ? (atoi(argv[4])) : xsize;
  if ((D < 2.0) || (D > 3.0)) Error(2, "D should be between 2.0 and 3.0");
  if (xsize < 3) Error(2, "xsize too small");
  if (ysize < 3) Error(2, "ysize too small");

  for(xpow=1,size=2; (size+1)<xsize; xpow++, size<<=1);  
  for(ypow=1,size=2; (size+1)<ysize; ypow++, size<<=1);  

  b = mkFractalSra(D, xpow, ypow, mean, cstdev, stdev);
  if (! b) Error(2, "could not allocate memory");
  i = Init_image(1, "Fractal image");
  i[1] = Imake_subband(b, 1, 1, xsize, ysize);

  sprintf(arg, "  D: %s  mean:%.2f  cstd:%.2f  std:%.2f",
	  argv[2], mean, cstdev, stdev);
  Ihistory(i, argv[0], arg);
  Iwrite_image(i,argv[1]);
  Idel_image(&i);
  Idel_band(&b);

  return(0);
}

#endif /* MAIN */
