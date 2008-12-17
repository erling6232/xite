
/*

________________________________________________________________

        snn
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

/*P:snn*

________________________________________________________________

		snn
________________________________________________________________

Name:		snn - symmetric nearest neighbour noise reduction

Syntax:		snn <inimage> <outimage> [<dx> [<dy>]] 

Description:    Perform Symmetric Nearest Neighbour filtering
                for noise removal. Filter window has size
                'dx' * 'dy'.

		'dx' has default value 3, 'dy' has default value 'dx'.

Restrictions:   'inimage' must have bands with pixel type unsigned byte or
                unsigned short.

See also:	lit(1), litSnn(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | snn mona.img monasnn.img
                | snn mona.img monasnn.img 5
		| snn mona.img monasnn.img 5 7

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn, dx, dy;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if ((argc < 3) || (argc > 5)) Usage(1, "Illegal number of arguments.\n");

  if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
  if (argc GE 5) dy = atoi(argv[4]); else dy = dx;

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);
  for (bn=1; bn LE Inbands(i1); bn++)
    if (snn(i1[bn], i2[bn], dx, dy))
      Error(1, "%s%d\n", "Error in snn() in band ", bn);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
