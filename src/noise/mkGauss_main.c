
/*

________________________________________________________________

        mkGauss
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/noise.h>
#include <xite/message.h>
#include <xite/utils.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H

/*P:mkGauss*

________________________________________________________________

		mkGauss
________________________________________________________________

Name:		mkGauss - make an image with gaussian random noise

Syntax:		mkGauss <output image> <my> <sigma> [<xsize> [<ysize>]]

Description:    'mkGauss' creates a one-band image of size xsize*ysize
                with UNS_BYTE pixels, and inserts gaussian random noise.

                | my - mean value of gaussian distribution
                | sigma - standard deviation of gaussian distribution
		| xsize - horizontal image size, default 512
		| ysize - vertical image size, default xsize

See also:	addGauss(1), multGauss(1), addPoisson(1), mkPoisson(1)

Return value:   0 when ok, positive otherwise.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:       mkGauss monaGauss.img 100 25

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  char arg[50];
  double my, sigma;
  int xsize, ysize, stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <outimage> <my> <sigma>  [<xsize> [<ysize>]]\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);
  if (argc == 1) Usage(1, (char*)0);
  if ((argc < 4) OR (argc > 6)) Usage(2, "Bad number of arguments\n");

  my = atof(argv[2]);
  sigma = atof(argv[3]);
  if (argc GT 4) xsize = atoi(argv[4]); else xsize = 512;
  if (argc GT 5) ysize = atoi(argv[5]); else ysize = xsize;

  img = Imake_image(1, "Gaussian noise", Iu_byte_typ, xsize, ysize);

  if ((stat = mkGauss(img[1], my, sigma))) exit(stat);

  sprintf(arg, " with my = %f and sigma = %f", my, sigma);
  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);
   return(0);
}
