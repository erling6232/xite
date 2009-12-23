

/*

________________________________________________________________

        haar2d
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/haar.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/readarg.h>
#include <xite/message.h>

#define SQRT2 1.41421356237309504880
#define SWAP(a,b) { float tmp=(a); a = b; b = tmp;}

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

/*P:haar2d*

________________________________________________________________

		haar2d
________________________________________________________________

Name:		haar2d - Two dimensional forward or inverse Haar transform

Syntax:		haar2d [-i] [-b] [-r] <inimage> <outimage> [<outimage2>]

Description:    haar2d performs the two dimesional forward or inverse
                haar transform, using all bands of <inimage> as input,
		writing the output to <outimage>. The input image may
		have any pixel type, the bands will be converted to
		REAL (float) before processing.

		Switces:
		| -i : Perform inverse transform (Default: forward)
		| -b : Convert all output bands to byte.
		| -r : Convert all output bands to real. 

		By default, output bands are of type real when doing
		a forward transform, byte when inverse transform.
		If you give BOTH -b and -r, you should also give
		<outimage2>. The byte version will then be written to
		<outimage> and the real version to <outimage2>.
		
Files:	
See also:	
Diagnostics:

Restrictions:   Horizontal and vertical sizes must be some power of two.

Author:		Tor Loennestad, BLAB, ifi, UiO

Examples:       | haar2d mona.img monaHaar.img
		| haar2d -b mona.img monaHaar.img; xshow monaHaar.img
		| haar2d -b -r mona.img monaHaarB.img monaHaarR.img
		| haar2d -i monaHaar.img monaHaari.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int i, b, r, bn, status;
  IMAGE img;
  IR_IMAGE ir;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s  [-i] [-b] [-r] <inimage> <outimage> [<outimage2>]\n"));

  if (argc == 1) Usage(1, NULL);

  /* read and test input */
  i = (read_switch(&argc,argv,"-i",FALSE,(char*)0) NE (char*)0);
  b = (read_switch(&argc,argv,"-b",FALSE,(char*)0) NE (char*)0);
  r = (read_switch(&argc,argv,"-r",FALSE,(char*)0) NE (char*)0);

  if (argc < 3) Usage(1, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  ir = (IR_IMAGE) Init_image(Inbands(img), Ititle(img));

  /* perform transformation */
  FOR (bn=1; bn LE Inbands(img); bn++)
    if (Ipixtyp(img[bn]) EQ Ireal_typ) ir[bn] = (IR_BAND)img[bn];
    else ir[bn] = (IR_BAND)mkConvertBand(img[bn], Ireal_typ);

    if (i) status = haari2d(ir[bn], ir[bn]);
    else status = haar2d(ir[bn], ir[bn]);
    if (status) Error(2, "Error while processing band %d\n", bn);
  ENDFOR;

  /* write output */
  if ((NOT b) AND (NOT r)) {if (i) b = TRUE; else r = TRUE;}
  Icopy_text(img, (IMAGE) ir);
  if (i) Ihistory((IMAGE) ir, argv[0], " Inverse transform");
    else  Ihistory((IMAGE) ir, argv[0], " Forward transform");
  IF (r)
    IF (b)
      if (argc LT 4)
        Usage(1, "No file name for real image.\n");
      else Iwrite_image((IMAGE) ir, argv[3]);
    ELSE
      Iwrite_image((IMAGE) ir, argv[2]);
    ENDIF;
  ENDIF;

  IF (b)
    for (bn=1; bn LE Inbands(img); bn++)
      ir[bn] = (IR_BAND)mkConvertBand((IBAND) ir[bn], Iu_byte_typ);
    Iwrite_image((IMAGE) ir, argv[2]);
  ENDIF;
  return(0);
}
