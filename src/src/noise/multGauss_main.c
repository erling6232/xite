
/*

________________________________________________________________

        multGauss
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/noise.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/utils.h>

/*P:multGauss*

________________________________________________________________

		multGauss
________________________________________________________________

Name:		multGauss - multiply image with random gaussian noise

Syntax:		multGauss <inimage> <outimage> <my> <sigma>

Description:    'multGauss' multiplies random noise with gaussian
                distribution to an image. The distribution has mean
		value 'my' and standard deviation 'sigma'.

                | my - mean value of gaussian distribution
                | sigma - standard deviation of gaussian distribution

See also:	addGauss(1), mkGauss(1)

Restrictions:   UNS_BYTE pixels are assumed.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:       multGauss mona.img monaGauss.img 1 0.5

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  char arg[50];
  int bn, stat;
  double my, sigma;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <my> <sigma>\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);
  if (argc == 1) Usage(1, (char*)0);
  if (argc != 5) Usage(2, "Bad number of arguments\n");

  my = atof(argv[3]);
  sigma = atof(argv[4]);

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  for (bn=1; bn LE Inbands(i1); ++bn)
    if ((stat = multGauss(i1[bn], i2[bn], my, sigma)))
      Warning(stat, "Error in band %d\n", bn);

  sprintf(arg, " with my = %f and sigma = %f", my, sigma);
  Ihistory(i2, argv[0], arg);
  Iwrite_image(i2, argv[2]);
   return(0);
}
