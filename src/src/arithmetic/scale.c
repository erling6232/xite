
/*

________________________________________________________________

        scale
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:scale*

________________________________________________________________

		scale
________________________________________________________________

Name:		scale - linear scaling of pixel values

Syntax:         | #include <xite/arithmetic.h>
		|
                | int scale( IBAND input, IBAND output,
                |    double Scale, double offset );
		|
Description:	'scale' scales the input band into the output band:

		| output(x,y) = Scale*input(x,y) + offset

		Only the largest rectangle common to the two bands
		is scaled.

		Accepts all pixeltypes except the sub-byte types. 'input' and
		'output' must have the same pixeltype, otherwise use
		convertBiff.

		If the result is outside the domain of the actual
		pixeltype, the nearest possible value is returned.
 
		Identical arguments for input and output are allowed.

Return value:	| 0 => ok
		| 1 => bad or different pixel types

See also:       convertBand(3)
Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int scale(IBAND input, IBAND output, double Scale, double offset)
{
  ISB_BAND sbb1, sbb2;
  IUS_BAND usb1, usb2;
  ISS_BAND ssb1, ssb2;
  II_BAND  ib1,  ib2;
  IR_BAND  rb1,  rb2;
  IC_BAND  cb1,  cb2;
  ID_BAND  db1,  db2;
  IDC_BAND dcb1, dcb2;
  int x, y, xsize, ysize;
  double val;
  IPIXTYP pt;

  pt = Ipixtyp(input);
  if (pt != Ipixtyp(output))
    return(Error(1, "Bands have different pixeltypes.\n"));

  xsize = MIN(Ixsize(input),Ixsize(output));
  ysize = MIN(Iysize(input),Iysize(output));

  switch (pt) {
  case (Iu_byte_typ): {
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*input[y][x] + offset;
	output[y][x] = CLIP_RANGE_ROUND_CAST(val, 0.0, UNS_BYTE_MAX, UNS_BYTE);
      }
    }
    return(0);
  }

  case (Is_byte_typ): {
    sbb1 = (ISB_BAND)input;
    sbb2 = (ISB_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*sbb1[y][x] + offset;
	sbb2[y][x] = CLIP_RANGE_ROUND_CAST(val, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
					   SIGN_BYTE);
      }
    }
    return(0);
  }

  case (Iu_short_typ): {
    usb1 = (IUS_BAND)input;
    usb2 = (IUS_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*usb1[y][x] + offset;
	usb2[y][x] = CLIP_RANGE_ROUND_CAST(val, 0, UNS_SHORT_MAX, UNS_SHORT);
      }
    }
    return(0);
  }

  case (Is_short_typ): {
    ssb1 = (ISS_BAND)input;
    ssb2 = (ISS_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*ssb1[y][x] + offset;
	ssb2[y][x] = CLIP_RANGE_ROUND_CAST(val, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
					   SIGN_SHORT);
      }
    }
    return(0);
  }

  case (Integer_typ): {
    ib1 = (II_BAND)input;
    ib2 = (II_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*ib1[y][x] + offset;
	ib2[y][x] = CLIP_RANGE_ROUND_CAST(val, INTEGER_MIN, INTEGER_MAX,
					  INTEGER);
      }
    }
    return(0);
  }

  case (Ireal_typ): {
    rb1 = (IR_BAND)input;
    rb2 = (IR_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*rb1[y][x] + offset;
	rb2[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
      }
    }
    return(0);
  }

  case (Icomplex_typ): {
    cb1 = (IC_BAND)input;
    cb2 = (IC_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = Scale*(cb1[y][x].re) + offset;
	cb2[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);

	val = Scale*(cb1[y][x].im) + offset;
	cb2[y][x].im = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
      }
    }
    return(0);
  }

  case (Idouble_typ): {
    db1 = (ID_BAND)input;
    db2 = (ID_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	db2[y][x] = Scale*db1[y][x] + offset;
      }
    }
    return(0);
  }

  case (Id_complex_typ): {
    dcb1 = (IDC_BAND)input;
    dcb2 = (IDC_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	dcb2[y][x].re = Scale*(dcb1[y][x].re) + offset;
	dcb2[y][x].im = Scale*(dcb1[y][x].im) + offset;
      }
    }
    return(0);
  }

  default: return(Error(1, "Bands have different pixeltypes.\n"));

  } /* switch */
}  /* scale() */

#endif /* not MAIN */



/*P:scale*

________________________________________________________________

		scale
________________________________________________________________

Name:		scale - linear scaling of pixel values

Syntax:		| scale [-scale <scale>] [-offset <offset>] <input>
                |       <output>

Description:    'scale' scales the input image band by band, according
		to the formula:

		| output(x,y) = scale*input(x,y) + offset

		Accepts all pixel types. If the result is outside
		the domain of the actual pixel type, the nearest
		possible value is returned.

Options:        &-scale scale
                Default: 1.0.

		&-offset offset
		Default: 0.0.

See also:	biffConvert(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | scale -scale 0.5 mona.img darkmona.img 
		| scale -scale 0.5 -offset 100 mona.img lightmona.img
		| scale -scale -1.0 -offset 255.0 mona.img negmona.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{

   IMAGE img;
   int nbands, bn;
   char *args;
   double Scale, offset;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-scale <scale>] [-offset <offset>] <inimage> <outimage>\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   Scale  = read_dswitch(&argc, argv, "-scale",  1.0);
   offset = read_dswitch(&argc, argv, "-offset", 0.0);

   if (argc != 3) Usage(1, "Illegal number of arguments.\n");

   /* read images */
   img    = Iread_image(argv[1]);
   nbands = Inbands(img);

   for (bn=1; bn <= nbands; ++bn)
     if (scale(img[bn], img[bn], Scale, offset))
       Error(2, "Error in band %d\n", bn);

   Ihistory(img, argv[0], args);
   Iwrite_image(img, argv[2]);

   return(0);
}

#endif /* MAIN */
