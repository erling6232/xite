
/*

________________________________________________________________

        absValue
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
#include <xite/negate.h>
#include <math.h>
#include XITE_STDIO_H
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_UNISTD_H

/*P:fftAmp=absValue*/
/*P:absValue*

________________________________________________________________

		absValue
________________________________________________________________

Name:		absValue, fftAmp - Take the absolute value of an image

Syntax:		| absValue [-real] <inimage> <outimage>
                
                | fftAmp <inimage> <outimage>

Description:    'absValue' takes the absolute value of an image,
		pixel by pixel. Only largest common rectangle is handled.

		Accepts all pixel types. Corresponding bands must have the
		same pixel type, except that if 'b1' is complex, then 'b2' may
		be either complex or real, and if 'b1' is double complex, then
		'b2' may be either double complex or double.

		Absolute value of a complex pixel is
		| sqrt(re * re + im * im)

		'fftAmp' is retained for compatibility with previous XITE
		versions. It is a script which invokes 'absValue' with
		option '-real'.

Options:        &-real
                For each complex input-band, let the resulting output band
		be real. For each double complex input-band, let the
		resulting output band be double.

See also:	negate(1)

Author:		Tor Lønnestad, BLAB, IfI, UiO
Modified:       Svein Bøe

Examples:       absValue mona.img monabs.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  IBAND bnd1, bnd2;
  int bn, stat, real;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-real] <inimage> <outimage>\n"));
  Iset_message(1);

  real = read_bswitch(&argc, argv, "-real");

  if (argc == 1) Usage(1, (char*)0);
  if (argc != 3) Usage(2, "Bad number of arguments\n");

  img1 = Iread_image(argv[1]);
  img2 = Init_image(Inbands(img1), Ititle(img1));

  if ((stat = Icopy_text(img1, img2)) != Iok) {
    Error(stat, "Error in Icopy_text.\n");
    exit(stat);
  }

  for (bn=1; bn <= Inbands(img1); bn++) {
    bnd1 = img1[bn];

    if (Ipixtyp(bnd1) == Icomplex_typ && real) {
      bnd2 = Imake_band(Ireal_typ, Ixsize(bnd1), Iysize(bnd1));
    } else if (Ipixtyp(bnd1) == Id_complex_typ && real) {
      bnd2 = Imake_band(Idouble_typ, Ixsize(bnd1), Iysize(bnd1));
    } else bnd2 = bnd1;

    if (bnd2 != bnd1) {
      Iset_start(bnd2, Ixstart(bnd1), Iystart(bnd1));
      Iset_mag(bnd2, Ixmag(bnd1), Iymag(bnd1));
    }

    if ((stat = absValue(bnd1, bnd2)) != Iok)
      Warning(stat, "Error in band %d\n", bn);

    img2[bn] = bnd2;
  }

  Ihistory(img2, argv[0], "");
  Iwrite_image(img2, argv[2]);

  return(0);
}
