

/*C*

________________________________________________________________

        imag
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

#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include XITE_STDIO_H
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*P:fftImag=imag*/
/*P:imag*

________________________________________________________________

		imag
________________________________________________________________

Name:		imag, fftImag - extract imaginary part of complex band

Syntax:		| imag [-f] [-d] <inimage> <outimage>

                | fftImag [-f] [-d] <inimage> <outimage>

Description:	'imag' extracts the imaginary part of a complex image.

		By default, output bands originating from complex bands are
		of type	float, output bands originating from double complex
		bands are of type double.

		'fftImag' is retained for compatibility with previous XITE
		versions. It is a script which invokes 'imag'.

Options:        Use only one of the two switches

		&-f
		Output bands should be of type float

		&-d
		Output bands should be of type double

See also:	imag(3), real(1), power(1), phase(1), square(1), squareRoot(1),
                absValue(1), fftDisplay(1), biffConvert(1)

Restrictions:	Input bands should be complex or double complex.

Return value:	| 0 => ok
                | 1 => too few or many arguments or bad switch
		| 2 => Can't open input image
		| 3 => Can't open output image
		| 4 => Out of memory

Author:		Tor Lønnestad, BLAB, Ifi, UiO (wrote fftImag)

Examples:	| imag -d cmpl.img dbl.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  int f, d, nbands, bn, ipt, opt, stat;
  char *args;

  Iset_abort(1);
  Iset_message(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-f] [-d] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  f    = read_bswitch(&argc, argv, "-f") || read_bswitch(&argc, argv, "-r");
  d    = read_bswitch(&argc, argv, "-d");

  if (f && d)    Usage(1, "Use only one of options -f and -d.\n");
  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1 = Iread_image(argv[1]);
  if (!img1) {
    Error(2, "Can't open %s\n", argv[1]);
    exit(2);
  }

  nbands = Inbands(img1);
  img2 = Init_image(nbands, Ititle(img1));
  if (!img2) {
    Error(4, "Out of memory\n");
    exit(4);
  }

  for (bn=1; bn <= nbands; bn++) {
    ipt = Ipixtyp(img1[bn]);

    if (f) opt = Ireal_typ;
    else if (d) opt = Idouble_typ;
    else if (ipt == Icomplex_typ) opt = Ireal_typ;
    else if (ipt == Id_complex_typ)  opt = Idouble_typ;
    else {
      opt = 0;
      img2[bn] = img1[bn];
    }

    if (opt) {
      img2[bn] = Imake_band(opt, Ixsize(img1[bn]), Iysize(img1[bn]));
      stat = imag(img1[bn], img2[bn]);
      if (stat) {
        Error(5, "Error in imag.\n");
        exit(5);
      }
    }
  }

  Icopy_text(img1, img2);
  Ihistory(img2, argv[0], args);
  if (Iwrite_image(img2, argv[2])) {
    Error(3, "Can't open %s\n", argv[2]);
    exit(3);
  }

  return(0);
}
