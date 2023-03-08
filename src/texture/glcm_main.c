/*C*


________________________________________________________________

        glcm
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


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/texture.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/draw.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H

/*P:glcm*

________________________________________________________________

		glcm
________________________________________________________________

Name:		glcm - Computes Grey Level Cooccurrence Matrix 

Syntax:		glcm [-heq] <inimage> <outimage> <dx> <dy> [<num_greylevels>]

Description:    Computes the Grey Level Cooccurence Matrix of an image.
                The angle and the displacement will be computed from the
                given 'dx' and 'dy'. 'dx' and 'dy' can be negative. The
		output image (or grey level cooccurrence matrix) will be of
		size 'num_greylevels' x 'num_greylevels'.

		All the pixel values in the input image will be increased by
		one prior to computing the GLCM. This means that if there is a
		greylevel combination (0,p) in the input image, then the value
		at (1,p+1) in the output image will be increased.

		'dx', horizontal distance, must fulfill (-50 <= 'dx' <= 50).
                'dy', vertical distance, must fulfill (-50 <= dy <= 50).

		Note that the number of gray levels is not set to
		'num_graylevel'. This command line argument is merely used to
		check whether 'inimage' contains too large pixel values, and
		an error message is given to this effect. If a reduction of
		the number of gray levels is desired, this should be taken
		care of prior to calling this program, or by using the
		'-heq' option.

Options:        &-heq
                Histogram equalize 'inimage' and set the number of grey levels
                equal to 'num_greylevels'. Default is 256.

Reference:      &R. M. Haralick, K. Shanmugam, and I. Dinstein
                "Textural Features for Image Classification",
                IEEE Trans. on Systems, Man and Cybernetics,
                Vol. SMC-3, pp. 610-621, 1973.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Author:		Yogesan K., BLAB, Ifi, UiO

Examples:	| glcm mona.img monaglcm.img 1 1 100
                | glcm -heq mona.img monaglcm.img 1 1 100
		| glcm mona.img monaglcm.img 1 1 
		| glcm -heq mona.img monaglcm.img 1 1 

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int dx, dy, num_greylevel, nbands, bn, heq;
  IMAGE inimg;
  ID_IMAGE outimg;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-heq] <inimage> <outimage> <dx> <dy> [<num_greylevels>]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  heq = read_bswitch(&argc, argv, "-heq");

  if ((argc < 5) || (argc > 6)) Usage(1, "Illegal number of arguments.\n");

  if (abs(dx = atoi(argv[3])) > 50 || abs(dy = atoi(argv[4])) > 50)
    Usage(2, "-50 <= dx, dy <= 50\n");

  num_greylevel = (argc == 5) ? 256 :  atoi(argv[5]);
  if(num_greylevel > 256)
    Usage(2, "Number of grey levels should be <= 256.\n");

  inimg = Iread_image(argv[1]);
  if (NOT inimg) Error(2, "Can't find %s.\n", argv[1]);

  nbands = Inbands(inimg);
  outimg = (ID_IMAGE) Imake_image(nbands, "glcm-image", Idouble_typ,
				  num_greylevel, num_greylevel);

  for (bn=1; bn <= nbands; bn++) {
    if (heq) 
      if (histoEq(inimg[bn], inimg[bn], num_greylevel))
	Error(2, "histoEq() error.\n");

    if (drawBand((IBAND) outimg[bn], 0.0, 0.0))
      Error(2, "drawBand() error.\n");;
    if (glcm(inimg[bn], outimg[bn], dx, dy, num_greylevel) > 1)
      Error(2, "glcm() error.\n");
  }

  Ihistory((IMAGE) outimg, argv[0], args);
  Iwrite_image((IMAGE) outimg, argv[2]);

  return(0);
}
