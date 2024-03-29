

/*C*

________________________________________________________________

        histoMeanStd
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/arithmetic.h>
#include <xite/histo.h>
#include <xite/statistics.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:histoMeanStd*

________________________________________________________________

		histoMeanStd
________________________________________________________________

Name:		histoMeanStd - linear scaling to desired mean and std.

Syntax:		histoMeanStd <inimage> <outimage> <mean> [<std>]

Description:    'histoMeanStd' performs a linear adjustment of
                the input image into the output image, pixel by pixel,
		so that the output band receives the specified
		mean and standard deviation, if possible.

		Output values below minimum for the actual pixel type are set
		to this minimum, and analogically for values above the
		maximum.

		Default value for 'std' is 50.0.

		The program accepts all pixel types except complex and double
		complex.

See also:	histoEq(1), histoNorm(1), scale(1)

Return value:   | 0 => ok
                | 1 => bad number of arguments
                | 2 => Couldn't open input file
                | 3 => Couldn't open output file

Author:		Tor L�nnestad, BLAB, Ifi, UiO

Examples:       | histoMeanStd mona.img 100.0
                | histoMeanStd mona.img 100.0 25.5

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int bn;
  double mean, std;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <mean> [<std>]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if ((argc < 4) || (argc > 5)) Usage(1, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  if (!img) Error(2, "Can't open input %s.\n", argv[1]);

  mean = atof(argv[3]);
  std = (argc == 5) ? atof(argv[4]) : 50.0;

  for (bn=1; bn<=Inbands(img); bn++)
    if (histoMeanStd(img[bn], img[bn], mean, std))
      Error(2, "Error in band %d.\n", bn);

  Ihistory(img, argv[0], args);
  if (Iwrite_image(img, argv[2])) Error(2, "Can't open output %s.\n", argv[2]);

  return(0);
}
