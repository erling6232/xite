

/*C*

________________________________________________________________

        extrema2
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



/*P:extrema2*

________________________________________________________________

		extrema2
________________________________________________________________

Name:		extrema2 - detection of local minima and maxima

Syntax:		extrema2 [-min <min>] [-max <max>] [-bg <bg>] 
                         <inimage> <outimage>

Description:	Local extrema (minima and maxima) are detected in
                'inband', and marked in 'outband'. Minima are
		marked with value 'min', maxima are marked 'max',
		and all other pixels are marked 'bg', background.
		If you do not want to distinguish between minima
		and maxima, see extrema1.

		The default for 'min' is 0, the 'max' default is 255,
		the 'bg' default is 128.

See also:	extrema2(3), extrema1(1), minima(1), maxima(1)

Restrictions:	Supports ony unsigned byte images. min, max and bg 
                should not be equal, and they should be in 0..255.

Reference:      B.B. Chaudhuri and B.Uma Shankar:
                "An efficient algorithm for extrema detection in
		digital images"
		Pattern Recognition Letters 10 (1989), 81-85.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| extrema2 -min 0 -max 1 -bg 2 mona.img monaextrem.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE inimage, outimage;
  int min, max, bg, bn, nbands;
  char arg[50];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-min <min>] [-max <max>] [-bg <bg>] <inimage> <outimage>\n"));

  min = read_iswitch(&argc, argv, "-min", 0);
  max = read_iswitch(&argc, argv, "-max", 255);
  bg  = read_iswitch(&argc, argv, "-bg",  128);

  if (argc == 1) Usage(0, (char*)0);
  if (argc != 3) Usage(1, "bad number of arguments\n");

  inimage = Iread_image(argv[1]);
  if (!inimage) exit(Error(2, "couldn't open file %s\n", argv[1]));
  nbands = Inbands(inimage);
  outimage = Icopy_init(inimage);
  if (!outimage) exit(3);

  for (bn=1; bn<=nbands; bn++)
    if (extrema2(inimage[bn], outimage[bn], min, max, bg)) exit(3);

  sprintf(arg, " min = %d, max = %d, bg = %d", min, max, bg);
  Ihistory(outimage, argv[0], arg);
  Iwrite_image(outimage, argv[2]);

  return(0);
}
