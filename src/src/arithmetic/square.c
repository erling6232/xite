
/*

________________________________________________________________

        square
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
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <stdlib.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:square*

________________________________________________________________

		square
________________________________________________________________

Name:		square - take the square of each pixel value

Syntax:         | #include <xite/arithmetic.h>
		|
                | int square( IBAND input, IBAND output,
                |    double scale );
Description:	'square' squares the input band into the output band:

		| output(x,y) = input(x,y) * input(x,y) * scale

		Only the largest rectangle common to the two bands
		is squared.

		Accepts all pixel types, but input and output must have the
		same pixel type, otherwise use convertBiff. If the result is
		outside the domain of the actual pixel type, the nearest
		possible value is returned.

		 Identical arguments for input and output are allowed.

Return value:	| 0 => ok
		| 1 => Illegal combination of pixel types
		| 2 => Unknown pixel type

See also:	square(1), squareRoot(3), phase(3), power(3), absValue(3),
                imag(3), real(3), scale(3), multiply(3), divide(3),
                convertBand(3)
                
Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int square(IBAND input, IBAND output, double scale)
{
   int x, y, xsize, ysize;
   IPIXTYP ipt, opt;
   double val;

   ipt = Ipixtyp(input);
   opt = Ipixtyp(output);
   if (ipt != opt)
     return(Error(1, "square: Illegal combination of pixel types.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   switch ((int) ipt)
     {
     case (Iu_byte_typ): {
       IUB_BAND bnd1 = (IUB_BAND)input;
       IUB_BAND bnd2 = (IUB_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   val = bnd1[y][x] * bnd1[y][x] * scale;
	   bnd2[y][x] =
	     CLIP_RANGE_ROUND_CAST(val, 0.0, UNS_BYTE_MAX, UNS_BYTE);
	 }
       }
       return(0);
     }
       
     case (Is_byte_typ): {
       ISB_BAND bnd1 = (ISB_BAND)input;
       ISB_BAND bnd2 = (ISB_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   val = bnd1[y][x] * bnd1[y][x] * scale;
	   bnd2[y][x] =
	     CLIP_RANGE_ROUND_CAST(val, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
				   SIGN_BYTE);
	 }
       }
       return(0);
     }
       
     case (Iu_short_typ): {
       IUS_BAND bnd1 = (IUS_BAND)input;
       IUS_BAND bnd2 = (IUS_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   val = bnd1[y][x] * bnd1[y][x] * scale;
	   bnd2[y][x] =
	     CLIP_RANGE_ROUND_CAST(val, 0, UNS_SHORT_MAX, UNS_SHORT);
	 }
       }
       return(0);
     }
       
     case (Is_short_typ): {
       ISS_BAND bnd1 = (ISS_BAND)input;
       ISS_BAND bnd2 = (ISS_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   val = bnd1[y][x] * bnd1[y][x] * scale;
	   bnd2[y][x] =
	     CLIP_RANGE_ROUND_CAST(val, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
				   SIGN_SHORT);
	 }
       }
       return(0);
     }
       
     case (Integer_typ): {
       II_BAND bnd1 = (II_BAND)input;
       II_BAND bnd2 = (II_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   val = bnd1[y][x] * bnd1[y][x] * scale;
	   bnd2[y][x] =
	     CLIP_RANGE_ROUND_CAST(val, INTEGER_MIN, INTEGER_MAX, INTEGER);
	 }
       }
       return(0);
     }
       
     case (Ireal_typ): {
       IR_BAND bnd1 = (IR_BAND)input;
       IR_BAND bnd2 = (IR_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   val = bnd1[y][x] * bnd1[y][x] * scale;
	   bnd2[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	 }
       }
       return(0);
     }
       
     case (Icomplex_typ): {
       double a, b;
       IC_BAND bnd1 = (IC_BAND)input;
       IC_BAND bnd2 = (IC_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   a = bnd1[y][x].re;
	   b = bnd1[y][x].im;
	   
	   val = (a*a - b*b) * scale;
	   bnd2[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	   
	   val = 2.0 * a * b * scale;
	   bnd2[y][x].im = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	 }
       }
       return(0);
     }
       
     case (Idouble_typ): {
       ID_BAND bnd1 = (ID_BAND)input;
       ID_BAND bnd2 = (ID_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   bnd2[y][x] = bnd1[y][x] * bnd1[y][x] * scale;
	 }
       }
       return(0);
     }
       
     case (Id_complex_typ): {
       double a, b;
       IDC_BAND bnd1 = (IDC_BAND)input;
       IDC_BAND bnd2 = (IDC_BAND)output;
       for (y=1; y <= ysize; ++y) {
	 for (x=1; x <= xsize; ++x) {
	   a = bnd1[y][x].re;
	   b = bnd1[y][x].im;
	   bnd2[y][x].re = (a*a - b*b) * scale;
	   bnd2[y][x].im = 2.0 * a * b * scale;
	 }
       }
       return(0);
     }
       
     default: return(Error(2, "square: Unknown pixel type\n"));
     }
} /* square() */

#endif /* MAIN */



/*P:square*

________________________________________________________________

		square
________________________________________________________________

Name:		square - take the square of each pixel value

Syntax:		| square [-s <scale>] <input> <output>

Description:    'square' squares the input image band by band,
		according to the formula:

		| output(x,y) = input(x,y) * input(x,y) * scale

		Accepts all pixel types. If the result is outside
		the domain of the actual pixel type, the nearest
		possible value is returned. Output image will have
		the same pixel type as input.

Options:        &-s scale
		Default: 1.0

See also:	square(3), squareRoot(1), phase(1), power(1), absValue(1),
                imag(1), real(1), scale(1), multiply(1), divide(1),
		biffConvert(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | square mona.img sqmona.img 
		| square -scale 0.05 mona.img sqmona.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE img;
   int nbands, bn;
   char *args;
   double scale;

   Iset_message(1); 
   Iset_abort(1);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s <scale>] <input> <output>\n"));

   if (argc == 1) Usage(1, NULL);
   args  = argvOptions(argc, argv);

   scale = read_dswitch(&argc, argv, "-scale", 1.0);
   scale = read_dswitch(&argc, argv, "-s", scale);

   if (argc != 3) Usage(2, "Illegal number of arguments.\n");

   /* read images */
   img = Iread_image(argv[1]);
   nbands = Inbands(img);

   for (bn=1; bn <= nbands; ++bn)
     if (square(img[bn], img[bn], scale))
       Warning(3, "Error in band %d\n", bn);

   Ihistory(img, argv[0], args);
   Iwrite_image(img, argv[2]);

   return(0);
}

#endif /* MAIN */
