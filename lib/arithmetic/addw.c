
/*

________________________________________________________________

        addw
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
#include <xite/message.h>
#include <stdlib.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:addw*

________________________________________________________________

		addw
________________________________________________________________

Name:		addw - add two bands pixel by pixel, with weights

Syntax:         | #include <xite/arithmetic.h>
		|
                | int addw( IBAND band1, IBAND band2,
                |    IBAND output, double w1, double w2,
                |    double offset );
		|
Description:	'addw' calculates, pixel by pixel, the weighted
                sum of band1 and band2.

		| output(x,y) = w1*band1(x,y) + w2*band2(x,y) + offset

		For complex or double complex bands, the result is

		| output(x,y).re = w1*band1(x,y).re + w2*band2(x,y).re
		|                  + offset
		| output(x,y).im = w1*band1(x,y).im + w2*band2(x,y).im
		|                  + offset

		If the result is outside the pixeltype range, the result is
		truncated. Within the pixeltype range, the result is
                rounded to the nearest possible pixelvalue.

Restriction:    Only the largest rectangle common to all three
                bands are added. All three bands must be of the same pixel
		type.

Return value:	| 0 => ok
		| 1 => bands have different pixel types
		| 2 => unknown pixel types

See also:	addw(1), absDiff(3), signDiff(3), multiply(3), divide(3)

Author:		Tor L?nnestad, BLAB, Ifi, UiO

Revised:        Svein B?e, Ifi, UiO. Implemented for all pixel types, not
                only unsigned byte.

Id:             $Id$
________________________________________________________________

*/

int addw(IBAND band1, IBAND band2, IBAND output, double w1, double w2, double offset)
{
   int x, y, xsize, ysize, pt;

   pt = Ipixtyp(band1);
   if ((pt != Ipixtyp(band2)) || (pt != Ipixtyp(output)))
     return(Error(1, "addw: bands have different pixel types.\n"));

   xsize = MIN(MIN(Ixsize(band1),Ixsize(band2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(band1),Iysize(band2)),Iysize(output));

   switch(pt)
     {
     case (Iu_byte_typ):
       {
	 float table1[UNS_BYTE_MAX + 1], table2[UNS_BYTE_MAX + 1];
	 double sum;

	 for (x=0; x <= 255; x++) {
	   table1[x] = w1*x;
	   table2[x] = w2*x;
	 }

	 for (y=1; y <= ysize; y++) {
	   for (x=1; x <= xsize; x++) {
	     sum = table1[band1[y][x]] + table2[band2[y][x]] + offset;
	     output[y][x] =
	       CLIP_RANGE_ROUND_CAST(sum, 0.0, UNS_BYTE_MAX, UNS_BYTE);
	   }
	 }
       }
       break;

     case (Is_byte_typ):
       {
         ISB_BAND b1, b2, b3;
	 float table1[UNS_BYTE_MAX + 1], table2[UNS_BYTE_MAX + 1];
	 double sum;

         b1 = (ISB_BAND)band1;
         b2 = (ISB_BAND)band2;
         b3 = (ISB_BAND)output;

	 for (x=0; x <= (SIGN_BYTE_MAX - SIGN_BYTE_MIN); x++) {
	   table1[x] = w1*(x + (SIGN_BYTE_MIN));
	   table2[x] = w2*(x + (SIGN_BYTE_MIN));
	 }

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = table1[b1[y][x] - SIGN_BYTE_MIN] +
	       table2[b2[y][x] - SIGN_BYTE_MIN] + offset;
             b3[y][x] =
	       CLIP_RANGE_ROUND_CAST(sum, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
				     SIGN_BYTE);
           }
	 }
       }
       break;

     case (Iu_short_typ):
       {
         IUS_BAND b1, b2, b3;
         double sum;

         b1 = (IUS_BAND)band1;
         b2 = (IUS_BAND)band2;
         b3 = (IUS_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x] + w2 * b2[y][x] + offset;
             b3[y][x] = CLIP_RANGE_ROUND_CAST(sum, 0, UNS_SHORT_MAX,
					      UNS_SHORT);
           }
	 }
       }
       break;

     case (Is_short_typ):
       {
         ISS_BAND b1, b2, b3;
         double sum;

         b1 = (ISS_BAND)band1;
         b2 = (ISS_BAND)band2;
         b3 = (ISS_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x] + w2 * b2[y][x] + offset;
             b3[y][x] =
	       CLIP_RANGE_ROUND_CAST(sum, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
				     SIGN_SHORT);
           }
	 }
       }
       break;

     case (Integer_typ):
       {
         II_BAND b1, b2, b3;
         double sum;

         b1 = (II_BAND)band1;
         b2 = (II_BAND)band2;
         b3 = (II_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x] + w2 * b2[y][x] + offset;
             b3[y][x] =
	       CLIP_RANGE_ROUND_CAST(sum, INTEGER_MIN, INTEGER_MAX, INTEGER);
           }
	 }
       }
       break;

     case (Ireal_typ):
       {
         IR_BAND b1, b2, b3;
         double sum;

         b1 = (IR_BAND)band1;
         b2 = (IR_BAND)band2;
         b3 = (IR_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x] + w2 * b2[y][x] + offset;
             b3[y][x] = CLIP_RANGE_CAST(sum, -REAL_MAX, REAL_MAX, REAL);
           }
	 }
       }
       break;

     case (Icomplex_typ):
       {
         IC_BAND b1, b2, b3;
         double sum;

         b1 = (IC_BAND)band1;
         b2 = (IC_BAND)band2;
         b3 = (IC_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x].re + w2 * b2[y][x].re + offset;
             b3[y][x].re = CLIP_RANGE_CAST(sum, -REAL_MAX, REAL_MAX, REAL);
	     sum = w1 * b1[y][x].im + w2 * b2[y][x].im + offset;
             b3[y][x].im = CLIP_RANGE_CAST(sum, -REAL_MAX, REAL_MAX, REAL);
           }
	 }
       }
       break;

     case (Idouble_typ):
       {
         ID_BAND b1, b2, b3;
         double sum;

         b1 = (ID_BAND)band1;
         b2 = (ID_BAND)band2;
         b3 = (ID_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x] + w2 * b2[y][x] + offset;
             b3[y][x] = sum;
           }
	 }
       }
       break;

     case (Id_complex_typ):
       {
         IDC_BAND b1, b2, b3;
         double sum;

         b1 = (IDC_BAND)band1;
         b2 = (IDC_BAND)band2;
         b3 = (IDC_BAND)output;

         for (y=1; y <= ysize; y++) {
           for (x=1; x <= xsize; x++) {
	     sum = w1 * b1[y][x].re + w2 * b2[y][x].re + offset;
             b3[y][x].re = sum;
	     sum = w1 * b1[y][x].im + w2 * b2[y][x].im + offset;
             b3[y][x].im = sum;
           }
	 }
       }
       break;

     default: return(Error(2, "addw: Unknown pixel type.\n"));

     }

   return(0);

}  /* addw() */
