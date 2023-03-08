

/*C*

________________________________________________________________

        minima
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/extrema.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*P:minima*

________________________________________________________________

		minima
________________________________________________________________

Name:		minima - detection of local minima

Syntax:		minima [-bg <bg>] [-fg <fg>] <inimage> <outimage>

Description:	Local minima are detected in 'inimage', and marked
		in 'outimage' with value 'fg', foreground. All other
		pixels are marked 'bg', background.

		The default for 'fg' is 255, the 'bg' default is 0.

See also:	minima(3), extrema1(1), extrema2(1), maxima(1)

Restrictions:	Supports ony unsigned byte images. bg and fg should
                not be equal, and they should be in 0..255.

Reference:      B.B. Chaudhuri and B.Uma Shankar:
                "An efficient algorithm for extrema detection in
		digital images"
		Pattern Recognition Letters 10 (1989), 81-85.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| minima -fg 0 -bg 1 mona.img monamin.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE inimage, outimage;
  int bg, fg, bn, nbands;
  char arg[50];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-bg <bg>] [-fg <fg>] <inimage> <outimage>\n"));

  bg = read_iswitch(&argc, argv, "-bg", 0);
  fg = read_iswitch(&argc, argv, "-fg", 255);

  if (argc == 1) Usage(0, (char*)0);
  if (argc != 3) Usage(1, "bad number of arguments\n");

  inimage = Iread_image(argv[1]);
  if (!inimage) exit(Error(2, "couldn't open file %s\n", argv[1]));
  nbands = Inbands(inimage);
  outimage = Icopy_init(inimage);
  if (!outimage) exit(3);

  for (bn=1; bn<=nbands; bn++)
    if (minima(inimage[bn], outimage[bn], fg, bg)) exit(3);

  sprintf(arg, " fg = %d, bg = %d", fg, bg);
  Ihistory(outimage, argv[0], arg);
  Iwrite_image(outimage, argv[2]);
  return(0);
}
