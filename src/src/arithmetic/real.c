
/*C*

________________________________________________________________

        real
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



/*F:fftReal=real*/
/*F:real*

________________________________________________________________

		real
________________________________________________________________

Name:		real, fftReal - extract real part of complex band

Syntax:         | #include <xite/arithmetic.h>
		|
                | int real( IBAND input, IBAND output );
		|
		| #include <xite/fft.h>
		|
		| int fftReal( IC_BAND iband, IR_BAND oband );
		|
Description:	'real' extracts the real part of a complex 'input' band.

		'fftReal' is retained for compatibility with previous XITE
		versions. It is a macro call to 'real'.

Restrictions:	The 'input' may be either complex or double complex,
		the 'output' may be float or double.

See also:	real(1), imag(3), power(3), phase(3), absValue(3), square(3),
                squareRoot(3), fftDisplay(3), convertBand(3)

Return value:	| 0 => ok
                | 1 => bad input pixel type
		| 2 => bad output pixel type

Author:		Tor Lønnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int real(IBAND input, IBAND output)
{
  int xsize, ysize, x, y;
  IPIXTYP ipt, opt;

  ipt = Ipixtyp(input);
  opt = Ipixtyp(output);

  if ((ipt != Icomplex_typ) && (ipt != Id_complex_typ))
    return(Error(1, "Bad input pixeltype.\n"));
  if ((opt != Ireal_typ) && (opt != Idouble_typ))
    return(Error(2, "Bad output pixeltype.\n"));

  xsize = MIN(Ixsize(input), Ixsize(output));
  ysize = MIN(Iysize(input), Iysize(output));

  if ((ipt == Icomplex_typ) && (opt == Ireal_typ)) {
    IC_BAND in  = (IC_BAND) input;
    IR_BAND out = (IR_BAND) output;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = in[y][x].re;
  } else if ((ipt == Icomplex_typ) && (opt == Idouble_typ)) {
    IC_BAND in  = (IC_BAND) input;
    ID_BAND out = (ID_BAND) output;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = (double) in[y][x].re;
  } else if ((ipt == Id_complex_typ) && (opt == Ireal_typ)) {
    IDC_BAND in  = (IDC_BAND) input;
    IR_BAND  out = (IR_BAND)  output;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = (float) in[y][x].re;
  } else if ((ipt == Id_complex_typ) && (opt == Idouble_typ)) {
    IDC_BAND in  = (IDC_BAND) input;
    ID_BAND  out = (ID_BAND)  output;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = in[y][x].re;
  }

  return(0);
}
