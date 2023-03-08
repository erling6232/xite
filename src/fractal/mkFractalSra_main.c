
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

Author:		Håvard Iversen, BLAB, Ifi, UiO

Modifyed by:    Tor Lønnestad, BLAB, Ifi, UiO

Reference:      &'Fournier, A., Fussel, D., and Carpenter, L.'
                "Computer rendering of stochastic models",
		Commun. ACM 25, 6 (June 1982), 372-382.

Example:        | mkFractalSra -mean 100.0 fractal.img 2.5 200 400 

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
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
