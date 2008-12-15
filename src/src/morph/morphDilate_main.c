
/*

________________________________________________________________

        
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
#include <xite/morph.h>
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*P:morphErode=morphDilate*/
/*P:morphClose=morphDilate*/
/*P:morphOpen=morphDilate*/
/*P:morphDilate*

________________________________________________________________

		morphology
________________________________________________________________

Name:		morphDilate, morphErode, morphClose, morphOpen - morphological
                grayscale operations on an image

Syntax:		| morphDilate <-a | -b> <struct_element>
                |    <inimage> <outimage>
		| morphErode <-a | -b> <struct_element>
                |    <inimage> <outimage>
		| morphClose <-a | -b> <struct_element>
                |    <inimage> <outimage>
		| morphOpen <-a | -b> <struct_element>
                |    <inimage> <outimage>

Description:    'morphDilate' calculates the morphological operation
                dilate on the image 'inimage', with a structuring
                element given by 'struct_element' (in combination with
		option '-a' or '-b'). The result is sent to the image specified
		in 'outimage'.

		'morphErode' calculates the morphological operation erode.

		'morphClose' calculates the morphological operation close.

		'morphOpen' calculates the morphological operation open.

		The actual calculation for the different operations are
		described by the expressions below (where i and j are inside
		the structuring element 'B', and (0,0) is the center pixel of
		'B').

		&dilate
		| outband(x,y) = max[inimage(x+i,y+j)+B(i,j)]

		&erode
		| outband(x,y) = min[inimage(x+i,y+j)-B(-i,-j)]

		&close
                | outband = morphErode(morphDilate(inband))

		&open
                | outband = morphDilate(morphErode(inband))

		-&The output values are clipped outside the interval [0, 255]
		to make the result fit in pixels of type unsigned byte.

		Be careful not to use too large values in 'B'. If, e.g. with
		dilation, 'inimage' is binary with the two values 0 and 255,
		and all the pixels in 'struct_element' equal 255, then all the
		pixels in 'outimage' will equal 255, regardless of the
		distribution of zeros in 'inimage'.

Restrictions:	'inimage' must have pixel type unsigned byte.
                'struct_element' must be of pixel type signed short
		(with option '-b') or ascii characters (with option '-a').

Options:        &-a struct_element
                'struct_element' is an ascii file which contains the
                structuring element. See ascii2biff(1) for file format.

                &-b struct_element
                'struct_element' is a BIFF image which contains the
                structuring element. Pixel type must be signed short.

See also:	morphDilate(3), mkMorph(1), ascii2biff(1)

Return value:	| 0 : OK
                | 1 : Usage message
		| 2 : Illegal number of arguments
		| 3 : No structuring element

Author:		Martin Torpe Lie, BLAB, Ifi, UiO

Examples:       morphDilate ~blab/img/mona.img mona.img -a filter.txt
                morphDilate ~blab/img/mona.img mona.img -b filter.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE inimage, outimage;
  ISS_IMAGE B;
  ISS_BAND B_band;
  int bn;
  char *args;
  char *a,*b;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> -a <ascii struct elem>\n\
       or: %s <inimage> <outimage> -b <biff struct elem>\n"));
  Iset_message(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  a = read_switch(&argc, argv, "-a", 1, NULL);
  b = read_switch(&argc, argv, "-b", 1, NULL);

  if (argc != 3) Usage(2,"Illegal number of arguments.\n");

  inimage  = Iread_image(argv[1]);

  if (a) ascii2biff((IBAND *) &B_band, a, Is_short_typ);
  else if (b) {
    B      = (ISS_IMAGE) Iread_image(b);
    B_band = B[1];
  } else Usage(3, "No structuring element.\n");

  outimage = Icopy_init(inimage);

  for (bn=1; bn LE Inbands(inimage); bn++)
    morphDilate(inimage[bn], outimage[bn], B_band);

  Ihistory(outimage, argv[0], args);
  Iwrite_image(outimage, argv[2]);

  return(0);
}
