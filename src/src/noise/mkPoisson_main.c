
/*

________________________________________________________________

        mkPoisson
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



#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/noise.h>
#include <xite/message.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H



/*P:mkPoisson*

________________________________________________________________

		mkPoisson
________________________________________________________________

Name:		mkPoisson - make an image with Poisson random noise.

Syntax:         mkPoisson <output-image> <alfa> [<xsize> [<ysize>]]

Description:    'mkPoisson' creates a one-band image of size
                'xsize' * 'ysize' with unsigned byte pixeltype and
		inserts Poisson random noise.

                &alfa
		Mean value of the distribution.

		&xsize
		Horizontal image size, default 512.

		&ysize
		Vertical image size, default xsize.

See also:	addPoisson(1), mkGauss(1), addGauss(1), multGauss(1)

Reference:      J. Banks: Discrete-event system simulation.

Author:		Helene Schulerud, BLAB, Ifi, UiO

Examples:       addPoisson monaaddP.img 2.0 256

Id:             $Id$                
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1;
  int xsize,ysize;
  char arg[50];
  double alfa;

  Iset_message(TRUE);		/* warnings from blab-software */
  Iset_abort(FALSE);		/* but don't abort */
  InitMessage(&argc, argv, xite_app_std_usage_text(
	      "Usage: %s <output-image> <alfa> [<xsize> [<ysize>]]\n"));

  if (argc == 1) Usage(1, NULL);
  if ((argc < 3) || (argc > 5 )) Usage(1, "Illegal number of arguments.\n");

  alfa = atof(argv[2]);

  if (argc > 3) xsize  = atoi(argv[3]); else xsize = 512;
  if (argc > 4) ysize  = atoi(argv[4]); else ysize = xsize;

  i1 = Imake_image(1, "Poisson noise", Iu_byte_typ, xsize, ysize);

  mkPoisson(alfa, i1[1]);

  sprintf(arg, " with alfa = %f", alfa);
  Ihistory(i1, argv[0], arg);
  Iwrite_image(i1, argv[1]);

  return(0);
}
