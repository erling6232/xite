/*

________________________________________________________________

        multiply
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
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:fftMult=multiply*/
/*P:multiply*

________________________________________________________________

		multiply
________________________________________________________________

Name:		multiply, fftMult - multiply two bands pixel by pixel

Syntax:		| multiply [-s <scale>] [-t <title>]
                |   <input1> <input2> <output>
		|
		| fftMult <inimage1> <inimage2> <outimage>
		|
Description:    'multiply' calculates, pixel by pixel, for each pair of
                bands, the product of 'input1', 'input2' and 'scale'.

		| output(x,y) = input1(x,y) * input2(x,y) * scale

		'fftMult' is retained for compatibility with previous XITE
		versions. It is a script which invokes 'multiply' without
		options.

Options:        &-s scale
		Default 1.0.
		&-t title
		Optional title of image. Default is "Image product".

See also:       multiply(3), divide(1), absDiff(1), signDiff(1), addw(1),
                average(1), scale(1)

Restrictions:   All pixel types are accepted, as long as the two input images
		have the same pixel type for each pair of bands. There are two
		exceptions: A band in 'input2' may be real if the
		corresponding band in 'input1' is complex, and a band in
		'input2' may be double if the corresponding band in 'input1'
		is double complex. The output band will be of the same type
		as the corresponding band in 'input1'.

		Resulting values outside the domain of the actual pixel type
		of 'input1' are truncated to the nearest legal value.

		Only the largest rectangle common to each pair of bands
                in the two input images is multiplied.

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Revised:        Svein Bøe

Examples:       | multiply i1.img mask.img prod.img
		| multiply -scale 0.004 mona1.img mona2.img monaBoth.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2, i3;
  int bands, bn;
  IPIXTYP pt;
  char *args, *title;
  double scale;

  Iset_message(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s <scale>] [-t <title>] <inimage1> <inimage2> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args  = argvOptions(argc, argv);

  scale = read_dswitch(&argc, argv, "-s", 1.0);
  scale = read_dswitch(&argc, argv, "-scale", scale);
  title = read_switch(&argc, argv, "-t", 1, "Image product");
  title = read_switch(&argc, argv, "-title", 1, title);

  if (argc != 4) Usage(1, "Illegal number of arguments.\n");

  /* read images */
  i1 = Iread_image(argv[1]);
  i2 = Iread_image(argv[2]);
  if ((!i1) || (!i2)) exit(2); 

  bands = MIN(Inbands(i1),Inbands(i2));
  i3 = Init_image(bands, title);
  if (!i3) Error(2, "Error in initializing image structure.\n");

  for (bn=1; bn <= bands; ++ bn) {
    pt = Ipixtyp(i1[bn]);

    i3[bn] = Imake_band(pt,
			MIN(Ixsize(i1[bn]),Ixsize(i2[bn])),
			MIN(Iysize(i1[bn]),Iysize(i2[bn])));

    if (multiply(i1[bn], i2[bn], i3[bn], scale))
      Error(3, "Error in multiplying band %d\n", bn);
  }

  Ihistory(i3, argv[0], args);
  Iwrite_image(i3, argv[3]);

  return(Iok);
}
