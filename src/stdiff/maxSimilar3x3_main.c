
/*

________________________________________________________________

        maxSimilar3x3.c
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
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/stdiff.h>

/*P:maxSimilar3x3*

________________________________________________________________

		maxSimilar3x3
________________________________________________________________

Name:		maxSimilar3x3 - maximum similarity 3x3 noise reduction

Syntax:		maxSimilar3x3 <input image> <output image>

Description:    For every pixel p, calculate the five 3x3 averages:

                | x x x   x x x   x x x   x x p   p x x
		| x x x   x x x   x p x   x x x   x x x
		| x x p   p x x   x x x   x x x   x x x

		Replace p in the output image with the average that
		is closest to p. The 1 pixel wide edge of the image
		is copied from input without modification. The next
		line/sample is partly processed.

Reference:      W. Niblack: "Digital Image Processing", Prentice/Hall, p80.

See also:	

Restrictions:   Bands must have pixel type UNS_BYTE.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       maxSimilar3x3 mona.img monaSim.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn, stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage>\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  for (bn=1; bn LE Inbands(i1); bn++)
    if ((stat = maxSimilar3x3(i1[bn], i2[bn])))
      Warning(stat, "Error in band %d\n", bn);

  Ihistory(i2, argv[0], "");
  Iwrite_image(i2, argv[2]);

  return(0);
}
