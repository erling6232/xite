
/*

________________________________________________________________

        litSnn
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/snn.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:litSnn*

________________________________________________________________

		litSnn
________________________________________________________________

Name:		litSnn - texture measure, combination of lit and snn

Syntax:		litSnn <inimage> <outimage> <dp> [<dx> [<dy>]] 

Description:    Combination of lit - local information transform -
                and snn - symmetric nearest neighbour. Only those
		pixels selected by the snn filter are included in
		the counting of pixels within the pixel value
		interval [centerpixel-dp..centerpixel+dp]. The
		output pixel value is the number of such pixels
		within the window. Uses a rectangular processing
		window of size 'dx' * 'dy'.

		'dp' is the delta pixel value.
		'dx' has default value 3, 'dy' has default value 'dx'.

See also:	lit(1), snn(1)

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                Even values of 'dx' and	'dy' are increased by one.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | snn mona.img monals.img 10
                | snn mona.img monals.img 10 5
		| snn mona.img monals.img 10 5 7

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn, dp, dx, dy, stat;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <dp> [<dx> [<dy>]]\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if ((argc LT 4) OR (argc GT 6))
    Usage(2, "Illegal number of arguments.\n");

  dp = atoi(argv[3]);
  if (argc GE 5) dx = atoi(argv[4]); else dx = 3;
  if (argc GE 6) dy = atoi(argv[5]); else dy = dx;

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  for (bn=1; bn LE Inbands(i1); bn++)
    if ((stat = litSnn(i1[bn], i2[bn], dp, dx, dy)))
      Warning(stat, "Error in band %d\n", bn);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
