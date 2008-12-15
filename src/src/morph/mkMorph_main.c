

/*C*

________________________________________________________________

        mkMorph.c
        $Id$
        Copyright 1994, Blab, UiO
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H
#include <xite/morph.h>

/*P:mkMorph*

________________________________________________________________

		mkMorph
________________________________________________________________

Name:		mkMorph - make a morphological structuring element

Syntax:	        | mkMorph [<option>] <outimage> <xsize> [<ysize>]

Description:	'mkMorph' generates a specified object (or structuring element)
                in the middle of 'outimage'. 'outimage' is always square with
		edge

		| sqrt(2)*MAX(xsize, ysize),

		rounded up to the nearest odd integer.

		The structuring element has a base shape/form (a footprint)
		and a height profile. The area outside the base shape is
		called background. The background pixels are set equal to the
		minimum for a signed short pixel (-32768).

		By default, the base shape is square. The values 'xsize' and
		'ysize' specify the size of the object base shape in the
		horizontal and vertical direction respectively.

		By default, the height profile is constant and has the value
		zero, i.e. the image is two-valued (binary). A different
		height profile can be chosen to make a gray level image.

		For a structuring element with a gray level profile, the
		pixels inside the base shape are replaced by the chosen
		profile, while the background is left unchanged. The resulting
		profile inside the base shape has a minimum value which is not
		greater than zero. See also option '-neg'.

		To make a gray level profile in the structuring element, the
		option '-zsize' must also be set to a value different from
		zero.

Options:        &-angle angle
                Rotate the shape of the	object by 'angle' radians.

		&-zsize zsize
		Set the peak of the height profile equal to 'zsize'. This may
		be modified with option '-neg'.

		&-neg
		Reduce all pixel values inside the structuring element by
		subtracting a constant value so that the largest pixel value
		inside the structuring element becomes zero. This is to
		prevent too large or small values when eroding or dilating an
		image. Pixel values are clipped outside the range of a signed
		short pixel.

		Without this option, the structuring element will rise from
		zero (instead of rising to zero).

		&base_shape_option

		&&-cir
		Circular base shape

		&&-ell
		Elliptic base shape

		&&-lin
		Line base shape

		&&-rec
		Rectangular base shape

		&&-sqr
		Square (default) base shape

		-&&For the '-lin' option, 'ysize' is not used. It is
		hardcoded to a width of 1.

		&height_profile_option

		&&-zell
		Elliptic height profile

		&&-zlin
		Linear height profile

		&&-zrec
		Rectangular height profile

		&&-zpar
		Parabolic height profile

		&&-zcon
		Conic height profile

See also:	morphDilate(1), mkMorphForm(3), mkMorphHeight(3), MorphScale(3)

Author:		Martin Torpe Lie
Revised:        Svein Bøe
Doc:            Martin Torpe Lie and Svein Bøe

Examples:	| mkMorph test.img 7 -cir
                | mkMorph test.img 17 5 -zsize 30 -angle 0.5 -ell -zlin

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int zsize, neg;
  morph_height_type typ, ztyp;
  char *arg;
  double angle, xsize, ysize;
  char *local_usage="Usage: %s <outimage> <xsize> [<ysize>] [-zsize <zsize>]\n\
       [-angle <angle>] [-neg] [-ell] [-lin] [-cir] [-rec]\n\
       [-zell] [-zlin] [-zcir] [-zrec] [-zpar] [-zcon]\n";

  Iset_message(TRUE); 
  Iset_abort(TRUE); 
  InitMessage(&argc, argv, xite_app_std_usage_text(local_usage));

  if (argc == 1) Usage(1, NULL);
  arg = argvOptions(argc, argv);

  typ   = read_bswitch(&argc, argv, "-sqr") ? SQUARE : SQUARE;
  typ   = read_bswitch(&argc, argv, "-ell") ? ELLIPSE : typ;
  typ   = read_bswitch(&argc, argv, "-lin") ? LINE : typ;
  typ   = read_bswitch(&argc, argv, "-cir") ? CIRCLE : typ;
  typ   = read_bswitch(&argc, argv, "-rec") ? RECTANGLE : typ;

  ztyp  = read_bswitch(&argc, argv, "-zell") ? ELLIPSE : SQUARE;
  ztyp  = read_bswitch(&argc, argv, "-zlin") ? LINEAR : ztyp;
  ztyp  = read_bswitch(&argc, argv, "-zcir") ? CIRCLE : ztyp;
  ztyp  = read_bswitch(&argc, argv, "-zrec") ? RECTANGLE : ztyp;
  ztyp  = read_bswitch(&argc, argv, "-zpar") ? PARABOL : ztyp;
  ztyp  = read_bswitch(&argc, argv, "-zcon") ? CONE : ztyp;

  neg   = read_bswitch(&argc, argv, "-neg");

  zsize = read_iswitch(&argc, argv, "-zsize", 0);
  angle = read_dswitch(&argc, argv, "-angle", 0.0);

  if ((argc != 3) && (argc != 4)) Usage(1, "Illegal number of arguments.\n");
  if (argc == 4) ysize = (double)atof(argv[3]);
  else ysize = (double)atof(argv[2]);

  xsize = (double)atof(argv[2]);

  img    = Init_image(1, argv[1]);
  img[1] = mkMorphForm(typ, xsize, ysize, angle);
  mkMorphHeight(img[1], ztyp, xsize - 1, ysize - 1, zsize, angle);
  MorphScale(img[1], neg);

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}
