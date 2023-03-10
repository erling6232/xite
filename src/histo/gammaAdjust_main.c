

/*

________________________________________________________________

        gammaAdjust
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include XITE_STDIO_H
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:gammaAdjust*

________________________________________________________________

                gammaAdjust
________________________________________________________________

Name:           gammaAdjust - Simple gamma correction
Syntax:         gammaAdjust [-adjust <value>] <infile> <outfile>
Description:    A simple gamma corretion to make images
                darker or brigter. The 'adjust' parameter must be 
		in the range -1.0 (much darker) to 1 (much brighter).
		If adjust = 0.0 the band is not changed.

		| If adjust > 0:
		| a= adjust**2/0.16;
		| b= 255.0 / log(a*255.0+1.0);
		| c= 256.0/a * log(1.0 + (exp(a) - 1.0) * i / 256.0);
		| col[i] = c;
		|
		| If adjust < 0;
		| a= adjust**2/0.16;
		| b= 255.0 / log(a*255.0+1.0);
		| c= 256.0/a * log(1.0 + (exp(a) - 1.0) * (255-i) / 256.0);
		| col[i] = 255-c;

Return value:   | 0 - OK
                | 2 - Bad pixtype
		| 3 - Bad size
Restrictions:   Works on unsigned byte bands only.
Author:         Otto Milvang
Examples:       | gammaAdjust -adjust 0.4 file.img | biff2ps - | lpr
Id:             $Id$
________________________________________________________________

*/


int main(int argc, char **argv)
{
  IMAGE img;
  double adjust;
  int i;
  char *options;

  Iset_message(1);
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-adjust <value>] <infile> <outfile>\n"));

  if (argc == 1) Usage(0, NULL);
  options = argvOptions(argc, argv);

  adjust = read_dswitch(&argc, argv, "-adjust", 0.0);

  if (adjust < 0.01 && adjust > -0.01) adjust = 0.01;
  if (adjust > 1.0) adjust = 1.0;
  if (adjust < -1.0) adjust = -1.0;

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);

  for (i=1; i<=Inbands(img); i++)
    gammaAdjust(img[i], img[i], adjust);

  Ihistory(img, "gammaAdjust", options);
  Iwrite_image(img, argv[2]);

  return(0);
}
