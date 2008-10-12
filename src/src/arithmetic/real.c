
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



#ifndef MAIN

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

#endif /* not MAIN */

/*P:fftReal=real*/
/*P:real*

________________________________________________________________

		real
________________________________________________________________

Name:		real, fftReal - extract real part of complex band

Syntax:		| real [-f] [-d] <inimage> <outimage>

                | fftReal [-f] [-d] <inimage> <outimage>

Description:	'real' extracts the real part of a complex image.

		By default, output bands originating from complex bands are of
		type float, output bands originating from double complex bands
		are of type double.

		'fftReal' is retained for compatibility with previous XITE
		versions. It is a script which invokes 'real'.

Options:        Use only one of the two switches

                &-f
                Output bands should be of type float

		&-d
		Output bands should be of type double

See also:	real(3), imag(1), power(1), phase(1), absValue(1), square(1),
                squareRoot(1), fftDisplay(1), biffConvert(1)

Restrictions:	Input bands should be complex or double complex.

Return value:	| 0 => ok
                | 1 => too few or many arguments or bad switch
		| 2 => Can't open input image
		| 3 => Can't open output image
		| 4 => Out of memory

Author:		Tor Lønnestad, BLAB, ifi, UiO

Examples:	| real -d cmpl.img dbl.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  int f, d, nbands, bn, ipt, opt, stat;
  char *args;

  Iset_message(1);
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-f] [-d] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  f = read_bswitch(&argc, argv, "-f") || read_bswitch(&argc, argv, "-r");
  d = read_bswitch(&argc, argv, "-d");

  if (f && d)    Usage(1, "Use only one of options -f and -d\n");
  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1 = Iread_image(argv[1]);

  if (!img1) {
    Error(2, "Can't open %s\n", argv[1]);
    exit(2);
  }

  nbands = Inbands(img1);
  img2   = Init_image(nbands, Ititle(img1));

  if (!img2) {
    Error(4, "Out of memory\n");
    exit(4);
  }

  for (bn=1; bn <= nbands; bn++) {
    ipt = Ipixtyp(img1[bn]);

    if (f) opt = Ireal_typ;
    else if (d) opt = Idouble_typ;
    else if (ipt == Icomplex_typ) opt = Ireal_typ;
    else if (ipt == Id_complex_typ)  opt = Idouble_typ;
    else {
      opt = 0;
      img2[bn] = img1[bn];
    }

    if (opt) {
      img2[bn] = Imake_band(opt, Ixsize(img1[bn]), Iysize(img1[bn]));
      stat = real(img1[bn], img2[bn]);
      if (stat) {
        Error(5, "Error in real.\n");
	exit(5);
      }
    }
  }

  Icopy_text(img1, img2);
  Ihistory(img2, argv[0], args);

  if (Iwrite_image(img2, argv[2])) {
    Error(3, "Can't open %s\n", argv[2]);
    exit(3);
  }

  return(0);
}

#endif /* MAIN */
