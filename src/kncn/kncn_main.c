
/*
________________________________________________________________

        kncn
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

/*P:kncn*

________________________________________________________________

		kncn
________________________________________________________________

Name:		kncn - k nearest connected neighbour noise reduction

Syntax:		kncn <inimage> <outimage> <k>  

Description:    Perform k-Nearest Connected Neighbour filtering
                for noise removal, averaging over the center pixel
		and 'k' connected neighbours. All bands in the image
                are filtered, one band at a time.

                Small 'k' => small effect, large 'k' => large effect.

Restrictions:   Only input bands with pixel type unsigned byte and unsigned
                short are processed.

Reference:	| T. Lønnestad: "Connected Filters for Noise Removal"
                | Proc. of 9. ICPR, Rome, 1988, 848-850.

See also:	kncn(3), mbkncn(1), knn(1), mbknn(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       kncn mona.img monasmooth.img 8

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn, k;
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

  for (bn=1; bn LE Inbands(i1); bn++) kncn(i1[bn], i2[bn], k);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
