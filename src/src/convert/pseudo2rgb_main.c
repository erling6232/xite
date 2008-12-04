
/*C*

________________________________________________________________

        pseudo2rgb
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
#include <stdlib.h>
#include <xite/convert.h>
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STDIO_H



/*P:pseudo2rgb*

________________________________________________________________

		pseudo2rgb
________________________________________________________________

Name:		pseudo2rgb - convert 1band image + colortable to rgb image

Syntax:		| pseudo2rgb <input image> <output image> <color table>

Description:    'pseudo2rgb' converts the first band of the input image
                to a three band rgb output image according to the
		specified color table.

See also:	colorquant(1)

Restrictions:   Accepts only UNS_BYTE images.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Revised:        Svein Bøe

Examples:	| pseudo2rgb mona.img monaRgb.img mona.col

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE pseudoImg, rgbImg;
  Color_tab coltab;
  char *ctname, *args;
  int ctlen;

  Iset_abort(1);
  Iset_message(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <input image> <output image> <color table>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 4) Usage(2, "Illegal number of arguments.\n");

  pseudoImg = Iread_image(argv[1]);
  color_read(argv[3], coltab, &ctname, &ctlen);
  rgbImg = Imake_image(3, Ititle(pseudoImg), Iu_byte_typ,
	     Ixsize(pseudoImg[1]), Iysize(pseudoImg[1]));

  if (pseudo2rgb(pseudoImg[1], rgbImg, coltab, ctlen))
    Error(2, "error occurred in pseudo2rgb().\n");

  Ihistory(rgbImg, argv[0], args);
  Iwrite_image(rgbImg, argv[2]);

  return(0);
}
