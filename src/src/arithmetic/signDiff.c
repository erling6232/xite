
/*

________________________________________________________________

        signDiff
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
#include XITE_STDIO_H
#include <xite/readarg.h>
#include <stdlib.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:signDiff*

________________________________________________________________

		signDiff
________________________________________________________________

Name:		signDiff - signed difference between two bands

Syntax:         | #include <xite/arithmetic.h>
		|
                | int signDiff( IBAND input1, IBAND input2,
                |    IBAND output, double scale, double offset );
		|
Description:	'signDiff' calculates, pixel by pixel, the signed
                difference between the two input images:

		| output(x,y) = 
		|     (input1(x,y)-input2(x,y))*scale + offset

		For complex and double-complex bands, the result is

		| output(x,y).re =
		|     (input1(x,y).re-input2(x,y).re)*scale + offset
		| output(x,y).im =
		|     (input1(x,y).im-input2(x,y).im)*scale + offset

                If the result is outside the pixeltype range, the result is
                truncated. Within the pixeltype range, the result is
		rounded to the nearest possible pixelvalue.

Restrictions:   All bands must be of the same type. Only the
                largest rectangle common to alle three bands is
		processed.

Return value:	| 0 => ok
                | 1 => bands have different pixel types
		| 2 => unknown pixel types

See also:       signDiff(1), absDiff(3), addw(3), multiply(3), divide(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Revised:        Svein Bøe, Ifi, UiO. Implemented for all pixel types, not
                only unsigned byte.

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int signDiff(input1, input2, output, scale, offset)
IBAND input1, input2, output;
double scale;
double offset;
#else /* FUNCPROTO */
int signDiff(IBAND input1, IBAND input2, IBAND output, double scale, double offset)
#endif /* FUNCPROTO */
{
   int x, y, xsize, ysize, pt;

   pt = Ipixtyp(input1);
   if ((pt != Ipixtyp(input2)) || (pt != Ipixtyp(output)))
     return(Error(1, "signDiff: bands have different pixel types.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   switch(pt)
     {
     case (Iu_byte_typ):
       {
	 double result;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = ((float)input1[y][x] - (float)input2[y][x])*scale
	       + offset;
	     output[y][x] =
	       CLIP_RANGE_ROUND_CAST(result, 0, UNS_BYTE_MAX, UNS_BYTE);
	   }
	 return(0);
       }

     case (Is_byte_typ):
       {
	 ISB_BAND band1, band2, band3;
	 double result;

	 band1 = (ISB_BAND)input1;
         band2 = (ISB_BAND)input2;
         band3 = (ISB_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = ((float)band1[y][x] - (float)band2[y][x])*scale + offset;
	     band3[y][x] =
	       CLIP_RANGE_ROUND_CAST(result, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
				     SIGN_BYTE);
	   }
	 return(0);
       }

     case (Iu_short_typ):
       {
	 IUS_BAND band1, band2, band3;
	 double result;

	 band1 = (IUS_BAND)input1;
         band2 = (IUS_BAND)input2;
         band3 = (IUS_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = ((float)band1[y][x] - (float)band2[y][x])*scale + offset;
	     band3[y][x] =
	       CLIP_RANGE_ROUND_CAST(result, 0, UNS_SHORT_MAX, UNS_SHORT);
	   }
	 return(0);
       }

     case (Is_short_typ):
       {
	 ISS_BAND band1, band2, band3;
	 double result;

	 band1 = (ISS_BAND)input1;
         band2 = (ISS_BAND)input2;
         band3 = (ISS_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = ((float)band1[y][x] - (float)band2[y][x])*scale + offset;
	     band3[y][x] =
	       CLIP_RANGE_ROUND_CAST(result, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
				     SIGN_SHORT);
	   }
	 return(0);
       }

     case (Integer_typ):
       {
	 II_BAND band1, band2, band3;
	 double result;

	 band1 = (II_BAND)input1;
         band2 = (II_BAND)input2;
         band3 = (II_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = ((float)band1[y][x] - (float)band2[y][x])*scale + offset;
	     band3[y][x] =
	       CLIP_RANGE_ROUND_CAST(result, INTEGER_MIN, INTEGER_MAX,
				     INTEGER);
	   }
	 return(0);
       }

     case (Ireal_typ):
       {
	 IR_BAND band1, band2, band3;
	 double result;

	 band1 = (IR_BAND)input1;
         band2 = (IR_BAND)input2;
         band3 = (IR_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = (band1[y][x] - band2[y][x])*scale + offset;
	     band3[y][x] = CLIP_RANGE_CAST(result, -REAL_MAX, REAL_MAX, REAL);
	   }
	 return(0);
       }

     case (Icomplex_typ):
       {
	 IC_BAND band1, band2, band3;
	 double result;

	 band1 = (IC_BAND)input1;
         band2 = (IC_BAND)input2;
         band3 = (IC_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = (band1[y][x].re - band2[y][x].re)*scale + offset;
	     band3[y][x].re =
	       CLIP_RANGE_CAST(result, -REAL_MAX, REAL_MAX, REAL);
	     result = (band1[y][x].im - band2[y][x].im)*scale + offset;
	     band3[y][x].im =
	       CLIP_RANGE_CAST(result, -REAL_MAX, REAL_MAX, REAL);
	   }
	 return(0);
       }

     case (Idouble_typ):
       {
	 ID_BAND band1, band2, band3;
	 double result;

	 band1 = (ID_BAND)input1;
         band2 = (ID_BAND)input2;
         band3 = (ID_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = (band1[y][x] - band2[y][x])*scale + offset;
	     band3[y][x] = result;
	   }
	 return(0);
       }

     case (Id_complex_typ):
       {
	 IDC_BAND band1, band2, band3;
	 double result;

	 band1 = (IDC_BAND)input1;
         band2 = (IDC_BAND)input2;
         band3 = (IDC_BAND)output;

	 for (y=1; y <= ysize; y++)
	   for (x=1; x <= xsize; x++) {
	     result = (band1[y][x].re - band2[y][x].re)*scale + offset;
	     band3[y][x].re = result;
	     result = (band1[y][x].im - band2[y][x].im)*scale + offset;
	     band3[y][x].im = result;
	   }
	 return(0);
       }

     default: return(Error(2, "signDiff: Unknown pixel type.\n"));

     }

}  /* signDiff() */

#endif /* not MAIN */



/*P:signDiff*

________________________________________________________________

		signDiff
________________________________________________________________

Name:		signDiff - signed difference between two images

Syntax:		| signDiff <input1> <input2> <output>
                |        [-scale <f>] [-offset <r>]

Description:    'signDiff' calulates the signed difference between
                two input images pixel by pixel:

		| output = (input1 - input2)*scale + offset

		For complex and double-complex bands, the result is

		| output(x,y).re =
		|     (input1(x,y).re-input2(x,y).re)*scale + offset
		| output(x,y).im =
		|     (input1(x,y).im-input2(x,y).im)*scale + offset

                If the result is outside the pixeltype range, the result is
                truncated. Within the pixeltype range, the result is
		rounded to the nearest possible pixelvalue.

		Default argument values:
		| scale = 1.0
		| offset = 0.0

Restrictions:   Corresponding bands in each input image must be of the
                same type. Only the largest rectangle common to two
		corresponding bands is processed.

See also:       signDiff(3), absDiff(1), addw(1), multiply(1), divide(1)

Author:		Tor Lønnestad, BLAB, ifi, UiO
Revised:        Svein Bøe, Ifi, UiO. Implemented for all pixel types, not
                only unsigned byte.

Examples:       | signDiff mona1.img mona2.img monaDiff.img
                | signDiff m1.img m2.img mdiff.img -scale 1.0 -offset 0

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE i1, i2, i3;
   int bands, bn;
   double scale, offset;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [<option>...] <input1> <input2> <output>\n\
       where <option> is chosen from\n\
         -scale <f>  : floating point scale-factor (default 1.0)\n\
         -offset <n> : floating point offset (default 0.0)\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   scale  = read_dswitch(&argc, argv, "-scale",  1.0);
   offset = read_dswitch(&argc, argv, "-offset", 0.0);

   if (argc != 4) Usage(2, "Illegal number of arguments.\n");

   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   i3 = Icopy_init(i1);
   bands = MIN(Inbands(i1),Inbands(i2));

   for (bn=1; bn <= bands; ++bn) {
     if (signDiff(i1[bn], i2[bn], i3[bn], scale, offset)) {
       fprintf(stderr, "Error in band %d\n", bn);
       exit(1);
     }
   }

   Ihistory(i3, argv[0], args);
   Iwrite_image(i3, argv[3]);

   return(0);
}

#endif /* MAIN */
