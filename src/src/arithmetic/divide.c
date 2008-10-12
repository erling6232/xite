/*C*

________________________________________________________________

        divide
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
#include XITE_STDIO_H
#include <stdlib.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:divide*

________________________________________________________________

		divide
________________________________________________________________

Name:		divide - divide two bands pixel by pixel

Syntax:         | #include <xite/arithmetic.h>
		|
                | int divide( IBAND band1, IBAND band2,
                |    IBAND output, double scale );
		|
Description:	'divide' divides, pixel by pixel, 'band1' on 'band2',
                and multiplies the result by 'scale'. The result is
		assigned to 'output'.

		| output(x,y) = band1(x,y) / band2(x,y) * scale

		If divisor is zero, we look at dividend. If dividend
		is zero, result is set to zero, otherwise result is
		set to "max", where "max" is the maximum allowed positive
		value for the particular pixeltype.

		All pixeltypes are accepted, as long as all three bands have
		the same pixeltype. Resulting values outside the domain of the
		actual pixel type are truncated to the nearest legal value.

Restrictions:   Only the largest rectangle common to all three bands
                is multiplied.

Return value:	| 0 => ok
		| 1 => bands have different pixel types
		| 2 => unknown pixel type

See also:       divide(1), multiply(3), scale(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int divide(IBAND band1, IBAND band2, IBAND output, double scale)
{
   ISB_BAND sbb1, sbb2, sbb3;
   IUS_BAND usb1, usb2, usb3;
   ISS_BAND ssb1, ssb2, ssb3;
   II_BAND  ib1,  ib2,  ib3;
   IR_BAND  rb1,  rb2,  rb3;
   IC_BAND  cb1,  cb2,  cb3;
   ID_BAND  db1,  db2,  db3;
   IDC_BAND dcb1, dcb2, dcb3;

   int x, y, xsize, ysize, pt;
   double val;

   pt = Ipixtyp(band1);
   if (pt != Ipixtyp(band2)) return(1);
   if (pt != Ipixtyp(output)) return(1);

   xsize = MIN(MIN(Ixsize(band1),Ixsize(band2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(band1),Iysize(band2)),Iysize(output));

   switch (pt) {
   case (Iu_byte_typ): {
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 val = (band2[y][x] != 0) ?
	   ((double) band1[y][x] / band2[y][x] * scale) :
	     ((band1[y][x] == 0) ? 0.0 : (double) UNS_BYTE_MAX);
	 
	 output[y][x] =
	   CLIP_RANGE_ROUND_CAST(val, 0.0, UNS_BYTE_MAX, UNS_BYTE);
       }
     }
     return(0);
   }
     
   case (Is_byte_typ): {
     sbb1 = (ISB_BAND)band1;
     sbb2 = (ISB_BAND)band2;
     sbb3 = (ISB_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 val = (sbb2[y][x] != 0) ? ((double) sbb1[y][x] / sbb2[y][x] * scale) :
	   ((sbb1[y][x] == 0) ? 0.0 : (double) SIGN_BYTE_MAX);
	 
	 sbb3[y][x] = CLIP_RANGE_ROUND_CAST(val, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
					    SIGN_BYTE);
       }
     }
     return(0);
   }
     
   case (Iu_short_typ): {
     usb1 = (IUS_BAND)band1;
     usb2 = (IUS_BAND)band2;
     usb3 = (IUS_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 val = (usb2[y][x] != 0) ? ((double) usb1[y][x] / usb2[y][x] * scale) :
	   ((usb1[y][x] == 0) ? 0.0 : (double) UNS_SHORT_MAX);
	 
	 usb3[y][x] = CLIP_RANGE_ROUND_CAST(val, 0, UNS_SHORT_MAX, UNS_SHORT);
       }
     }
     return(0);
   }
     
   case (Is_short_typ): {
     ssb1 = (ISS_BAND)band1;
     ssb2 = (ISS_BAND)band2;
     ssb3 = (ISS_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 val = (ssb2[y][x] != 0) ? ((double) ssb1[y][x] / ssb2[y][x] * scale) :
	   ((ssb1[y][x] == 0) ? 0.0 : (double) SIGN_SHORT_MAX);
	 
	 ssb3[y][x] = CLIP_RANGE_ROUND_CAST(val, SIGN_SHORT_MIN,
					    SIGN_SHORT_MAX, SIGN_SHORT);
       }
     }
     return(0);
   }
     
   case (Integer_typ): {
     ib1 = (II_BAND)band1;
     ib2 = (II_BAND)band2;
     ib3 = (II_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 val = (ib2[y][x] != 0) ? ((double) ib1[y][x] / ib2[y][x] * scale) :
	   ((ib1[y][x] == 0) ? 0.0 : (double) INTEGER_MAX);
	 
	 ib3[y][x] = CLIP_RANGE_ROUND_CAST(val, INTEGER_MIN, INTEGER_MAX,
					   INTEGER);
       }
     }
     return(0);
   }
     
   case (Ireal_typ): {
     rb1 = (IR_BAND)band1;
     rb2 = (IR_BAND)band2;
     rb3 = (IR_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 val = (rb2[y][x] != 0.0) ? ((double) rb1[y][x] / rb2[y][x] * scale) :
	   ((rb1[y][x] == 0.0) ? 0.0 : REAL_MAX);
	 
	 rb3[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
       }
     }
     return(0);
   }
     
   case (Icomplex_typ): {
     double a1, a2, b1, b2, c;
     cb1 = (IC_BAND)band1;
     cb2 = (IC_BAND)band2;
     cb3 = (IC_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 a1 = cb1[y][x].re;
	 a2 = cb2[y][x].re;
	 b1 = cb1[y][x].im;
	 b2 = cb2[y][x].im;
	 c = a2*a2 + b2*b2;
	 val = (c != 0.0) ? ((a1*a2 + b1*b2)*scale/c) :
	   (((a1*a2 + b1*b2) == 0.0) ? 0.0 : REAL_MAX);
	 
	 cb3[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	 
	 val = (c != 0.0) ? ((a2*b1 - a1*b2)*scale/c) :
	   (((a2*b1 - a1*b2) == 0.0) ? 0.0 : REAL_MAX);
	 
	 cb3[y][x].im = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
       }
     }
     return(0);
   }
     
   case (Idouble_typ): {
     db1 = (ID_BAND)band1;
     db2 = (ID_BAND)band2;
     db3 = (ID_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 db3[y][x] = (db2[y][x] != 0.0) ? (db1[y][x] / db2[y][x] * scale) :
	   ((db1[y][x] == 0.0) ? 0.0 : DOUBLE_MAX);
       }
     }
     return(0);
   }
     
   case (Id_complex_typ): {
     double a1, a2, b1, b2, c;
     dcb1 = (IDC_BAND)band1;
     dcb2 = (IDC_BAND)band2;
     dcb3 = (IDC_BAND)output;
     for (y=1; y <= ysize; ++y) {
       for (x=1; x <= xsize; ++x) {
	 a1 = dcb1[y][x].re;
	 a2 = dcb2[y][x].re;
	 b1 = dcb1[y][x].im;
	 b2 = dcb2[y][x].im;
	 c = a2*a2 + b2*b2;
	 dcb3[y][x].re = (c != 0.0) ? ((a1*a2 + b1*b2)*scale/c) :
	   (((a1*a2 + b1*b2) == 0.0) ? 0.0 : DOUBLE_MAX);
	 dcb3[y][x].im = (c != 0.0) ? ((a2*b1 - a1*b2)*scale/c) :
	   (((a2*b1 - a1*b2) == 0.0) ? 0.0 : DOUBLE_MAX);
       }
     }
     return(0);
   }
     
   default: return(2);
   } /* switch */
   
}  /* divide() */

#endif /* not MAIN */



/*P:divide*

________________________________________________________________

		divide
________________________________________________________________

Name:		divide - divide two images pixel by pixel

Syntax:		| divide [-s <scale>] [-t <title>]
                |     <input1> <input2> <output>

Description:    'divide' divides, pixel by pixel, for each pair of
                bands, input1 on input2, and multiplies with scale.

		| output(x,y) = input1(x,y) / input2(x,y) * scale

		If divisor is zero, we look at dividend. If dividend
		is zero, result is set to zero, otherwise result is
		set to "max".

		All pixel types are accepted, as long as the two
		input images have the same pixel type for each pair
		of bands. The output image will be of the same type.
		Resulting values outside the domain of the actual
		pixel type is truncated to the nearest legal value.

		Default <scale> value is 1.0. <title> is optional
		title of image. Default is  "Image division".

See also:	multiply(1), scale(1)

Restrictions:   Only the largest rectangle common to each pair of bands
                in the two input images is divided.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | divide mona1.img mona2.img monaDiv.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2, i3;
   int bands, bn, pt;
   char *args, *title;
   double scale;

   Iset_message(1); 
   Iset_abort(1); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s <scale>] [-t <title>] <inimage1> <inimage2> <outimage>\n"));
 
   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   scale = read_dswitch(&argc, argv, "-s", 1.0);
   scale = read_dswitch(&argc, argv, "-scale", scale);
   title = read_switch(&argc, argv, "-t", 1, "Image division");
   title = read_switch(&argc, argv, "-title", 1, title);

   if (argc < 4) Usage(2, "Illegal number of arguments.\n");

   /* read images */
   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   if ((!i1) || (!i2)) exit(2); 
   bands = MIN(Inbands(i1),Inbands(i2));
   i3 = Init_image(bands, title);
   if (!i3) Error(2, "Error in initializing image structure.\n");

   for (bn=1; bn <= bands; ++bn) {
     pt = Ipixtyp(i1[bn]);

     if (pt != Ipixtyp(i2[bn])) {
       Error(3, "Different pixel types in band %d\n", bn);
     } else {
       i3[bn] = Imake_band(pt,
         MIN(Ixsize(i1[bn]),Ixsize(i2[bn])),
         MIN(Iysize(i1[bn]),Iysize(i2[bn])));

       if (divide(i1[bn], i2[bn], i3[bn], scale))
         Error(4, "Error in division of band %d\n", bn);
     }
   }

   Ihistory(i3, argv[0], args);
   Iwrite_image(i3, argv[3]);

   return(0);
}

#endif /* MAIN */
