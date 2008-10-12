

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



/*F:fftImag=imag*/
/*F:imag*

________________________________________________________________

		imag
________________________________________________________________

Name:		imag, fftImag - extract imaginary part of complex band

Syntax:         | #include <xite/arithmetic.h>
		|
                | int imag( IBAND input, IBAND output );
		|
		| #include <xite/fft.h>
		|
		| int fftImag( IC_BAND input, IR_BAND output );
		|
Description:	'imag' extracts the imaginary part of a complex 'input'
                band.

		'fftImag' is retained for compatibility with previous XITE
		versions. It is a macro call to 'imag'.

Restrictions:	The 'input' may be either complex or double complex,
		the 'output' may be float or double.

See also:	imag(1), real(3), power(3), phase(3), square(3), squareRoot(3),
                absValue(3), fftDisplay(3), convertBand(3)

Return value:	| 0 => Ok
                | 1 => Bad input pixel type
		| 2 => Bad output pixel type

Author:		Tor Lønnestad, BLAB, Ifi, UiO (wrote fftImag)
Revised:        Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

int imag(IBAND input, IBAND output)
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
        out[y][x] = in[y][x].im;
  } else if ((ipt == Icomplex_typ) && (opt == Idouble_typ)) {
    IC_BAND in  = (IC_BAND) input;
    ID_BAND out = (ID_BAND) output;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = (double) in[y][x].im;
  } else if ((ipt == Id_complex_typ) && (opt == Ireal_typ)) {
    IDC_BAND in  = (IDC_BAND) input;
    IR_BAND  out = (IR_BAND)  output;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = (float) in[y][x].im;
  } else if ((ipt == Id_complex_typ) && (opt == Idouble_typ)) {
    IDC_BAND in  = (IDC_BAND) input;
    ID_BAND  out = (ID_BAND)  output;
    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        out[y][x] = in[y][x].im;
  }

  return(0);
} /* imag() */
