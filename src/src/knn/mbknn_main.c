
/*

________________________________________________________________

        mbknn
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
#include <xite/knn.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:mbknn*

________________________________________________________________

		mbknn
________________________________________________________________

Name:		mbknn - multi band k nearest neighbour noise reduction

Syntax:		mbknn <inimage> <outimage> <n> <k>

Description:    Performs multi band 'k nearest neighbour
                noise removal. All bands are filtered 
                simultaneously. Window size 'n' x 'n', max 31.
                If 'n' is even, 'n'+1 is used. 'k' is number of
                neighbours to average over.

See also:	mbknn(3), knn(1), kncn(1), mbkncn(1)

Restrictions:   The bands of 'inimage' must have pixel type unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       mbknn color.img colorknn.img 5 9

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  char *args;
  int n, k, stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <n> <k>\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if (argc NE 5) Usage(2, "Illegal number of arguments.\n");

  n = atoi(argv[3]);
  k = atoi(argv[4]);

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  if ((stat = mbknn(i1, i2, n, k))) exit(stat);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
