
/*

________________________________________________________________

        negate
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
#include <xite/negate.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>

/*P:negate*

________________________________________________________________

		negate
________________________________________________________________

Name:		negate - negate an image

Syntax:		negate <inimage> <outimage>

Description:    'negate' negates an image. Accepts all pixel types. 
		For pixel types including negative values, the
		result is
		| output = -input
		For pixel types NOT including negative values,
		the result is
		| output = MAX - input
		where MAX is the largest value of the actual pixel
		type.

See also:	absValue(1)

Author:		Tor L|nnestad, BLAB, IfI, UiO

Examples:       negate mona.img moneg.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int bn, stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage>\n"));
  Iset_message(TRUE);

  if (argc == 1) Usage(1, NULL);
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  for (bn=1; bn LE Inbands(img); ++bn)
    if ((stat = negate(img[bn], img[bn])))
      Warning(stat, "Error in band %d\n", bn);
  Ihistory(img, argv[0], "");
  Iwrite_image(img, argv[2]);

  return(0);
}
