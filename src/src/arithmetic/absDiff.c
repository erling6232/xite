
/*

________________________________________________________________

        absDiff
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


/*I*
________________________________________________________________

		include files
________________________________________________________________
*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <arithmetic.h>
#include <message.h>
#include <readarg.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:absDiff*

________________________________________________________________

		absDiff
________________________________________________________________

Name:		absDiff - absolute difference between two bands

Syntax:         | #include <xite/arithmetic.h>
		|
                | int absDiff( IBAND input1, IBAND input2,
                |    IBAND output );
		|
Description:	'absDiff' calculates, pixel by pixel, the
                absolute value of the grey level difference
		between the two input images.

                If the result is outside the pixeltype range, the result is
                limited.

Restrictions:   Accepts all pixeltypes, as long as all three bands
                have the same pixeltype.

Return value:	| 0 => ok
		| 1 => bands have different pixel types
		| 2 => unknown pixel type

See also:       absDiff(1), signDiff(3), addw(3), multiply(3), divide(3)

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int absDiff(IBAND input1, IBAND input2, IBAND output)
{
   ISB_BAND sbb1, sbb2, sbb3;
   IUS_BAND usb1, usb2, usb3;
   ISS_BAND ssb1, ssb2, ssb3;
   II_BAND  ib1,  ib2,  ib3;
   IR_BAND  rb1,  rb2,  rb3;
   IC_BAND  cb1,  cb2,  cb3;
   ID_BAND  db1,  db2,  db3;
   IDC_BAND dcb1, dcb2, dcb3;

   int x, y, idiff;
   long xsize, ysize, ldiff;
   IPIXTYP pt;
   double ddiff;

   pt = Ipixtyp(input1);
   if ((pt != Ipixtyp(input2)) || (pt != Ipixtyp(output)))
     return(Error(1, "absDiff(): bands have different pixel types.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   switch (pt)
     {
     case (Iu_byte_typ):
       { for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
             idiff = input1[y][x] - input2[y][x];
             output[y][x] = (UNS_BYTE) (idiff<0) ? -idiff : idiff;
	   }
         return(0);
       }

     case (Is_byte_typ):
       { sbb1 = (ISB_BAND)input1;
         sbb2 = (ISB_BAND)input2;
         sbb3 = (ISB_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
             idiff=sbb1[y][x]-sbb2[y][x];
             if (idiff<0) idiff = -idiff;
             sbb3[y][x] = (SIGN_BYTE)
	       (idiff>SIGN_BYTE_MAX)?SIGN_BYTE_MAX:idiff;
           }
         return(0);
       }

     case (Iu_short_typ):
       { usb1 = (IUS_BAND)input1;
         usb2 = (IUS_BAND)input2;
         usb3 = (IUS_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x)
             usb3[y][x] = (UNS_SHORT) (((idiff=usb1[y][x]-usb2[y][x]) < 0) ?
				       -idiff : idiff);
         return(0);
       }

     case (Is_short_typ):
       { ssb1 = (ISS_BAND)input1;
         ssb2 = (ISS_BAND)input2;
         ssb3 = (ISS_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
             idiff=ssb1[y][x]-ssb2[y][x];
             if (idiff<0) idiff = -idiff;
             ssb3[y][x] = (SIGN_SHORT)
	       (idiff>SIGN_SHORT_MAX) ? SIGN_SHORT_MAX : idiff;
	   }
         return(0);
       }

     case (Integer_typ):
       { ib1 = (II_BAND)input1;
         ib2 = (II_BAND)input2;
         ib3 = (II_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
	     ldiff=ib1[y][x]-ib2[y][x];
             if (ldiff<0) ldiff = -ldiff;
             ib3[y][x] = (INTEGER) (ldiff>INTEGER_MAX) ? INTEGER_MAX : ldiff;
	   }
         return(0);
       }

     case (Ireal_typ):
       { rb1 = (IR_BAND)input1;
         rb2 = (IR_BAND)input2;
         rb3 = (IR_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
	     ddiff = rb1[y][x]-rb2[y][x];
             if (ddiff < 0.0) ddiff = -ddiff;
	     rb3[y][x] = (REAL) (ddiff > REAL_MAX) ? REAL_MAX : (REAL)ddiff;
	   }
         return(0);
       }

     case (Icomplex_typ):
       { cb1 = (IC_BAND)input1;
         cb2 = (IC_BAND)input2;
         cb3 = (IC_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
	     ddiff = cb1[y][x].re-cb2[y][x].re;
             if (ddiff < 0.0) ddiff = -ddiff;
	     cb3[y][x].re = (REAL) (ddiff > REAL_MAX) ? REAL_MAX : (REAL)ddiff;
	     ddiff = cb1[y][x].im-cb2[y][x].im;
             if (ddiff < 0.0) ddiff = -ddiff;
	     cb3[y][x].im = (REAL) (ddiff > REAL_MAX) ? REAL_MAX : (REAL)ddiff;
	   }
         return(0);
       }

     case (Idouble_typ):
       { db1 = (ID_BAND)input1;
         db2 = (ID_BAND)input2;
         db3 = (ID_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
             ddiff = db1[y][x]-db2[y][x];
             db3[y][x] = (ddiff<0.0) ? -ddiff : ddiff;
	   }
         return(0);
       }

     case (Id_complex_typ):
       { dcb1 = (IDC_BAND)input1;
         dcb2 = (IDC_BAND)input2;
         dcb3 = (IDC_BAND)output;
         for (y=1; y <= ysize; ++y)
           for (x=1; x <= xsize; ++x) {
             ddiff = dcb1[y][x].re-dcb2[y][x].re;
             dcb3[y][x].re = (ddiff<0.0) ? -ddiff : ddiff;
             ddiff = dcb1[y][x].im-dcb2[y][x].im;
             dcb3[y][x].im = (ddiff<0.0) ? -ddiff : ddiff;
	   }
         return(0);
       }

     default: return(Error(2, "absDiff(): unknown pixel type.\n"));
   }
}  /* absDiff() */
