
/*

________________________________________________________________

        mbkncn
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
#include <xite/kncn.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

/*P:mbkncn*

________________________________________________________________

		mbkncn
________________________________________________________________

Name:		mbkncn - multi band k nearest connected neighbour
                noise reduction

Syntax:		mbkncn <inimage> <outimage> <k>

Description:    Performs multi band 'k' nearest connected
                neighbour noise removal. All bands are filtered
                simultaneously. Taxi distance is used as distance
                measure.

                | inp image  - multi band input image
                | outp image - output image 
                | k - filtering parameter
                |   small k => small effect
                |   large k => large effect

Restrictions:   All bands of 'inimage' must have pixel type unsigned byte.

See also:	mbkncn(3), kncn(1), knn(1), mbknn(1)

Return value:   0 when ok, positive otherwise

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       mbkncn color.img smooth.img 8

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int k;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <oldfile> <newfile> <k>\n"));
  Iset_message(TRUE);
  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 4) Usage(2, "Illegal number of arguments.\n");

  k = atoi(argv[3]);
  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  if (mbkncn(i1, i2, k)) exit(3);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
