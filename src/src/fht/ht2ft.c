

/*C*

________________________________________________________________

        ht2ft
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
#include <stdlib.h>
#include <xite/fht.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>



#ifndef MAIN

/*F:ht2ft*

________________________________________________________________

		ht2ft
________________________________________________________________

Name:		ht2ft - 2d hartley to fourier conversion

Syntax:         | #include <xite/fht.h>
                |
                | int ht2ft( IR_BAND inband, IC_BAND outband );

Description:    'ht2ft' converts a Hartley transform to a Fourier transform.
                The Fourier transform is given by the Hartley transform
		as follows:

		| F(u,v) = Re{F(u,v)} + j Im{F(u,v)}
		|        = H_e(u,v) - j H_o(u,v)
		|        = (H(u,v) + H(-u,-v))/2 - j (H(u,v) - H(-u,-v))/2

		For images with horizontal and vertical even symmetry
		H_o(u,v) equals zero, so that
		| F(u,v) = H_e(u,v) = H(u,v)

Restrictions:	bands should have equal size, be quadratic, and have
                size some power of two. Currently, only single precision
		float values are accepted.

See also:	ht2ft(1), fft2d(3), fht2d(3), ft2ht(3)

Return value:   | 0 => ok
                | 1 => bad pixel type input
		| 2 => bad pixel type output
		| 3 => bad sizes

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int ht2ft(IR_BAND inband, IC_BAND outband)
{
  int x, y, xsize, ysize, xsizep2, ysizep2;
  float even, odd;

  if (Ipixtyp((IBAND) inband) NE Ireal_typ) return(1);
  if (Ipixtyp((IBAND) outband) NE Icomplex_typ) return(2);
  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);
  if (xsize NE Ixsize((IBAND) outband)) return(3);
  if (ysize NE Iysize((IBAND) outband)) return(3);
  xsizep2 = xsize+2;
  ysizep2 = ysize+2;

  /* dc component */
  outband[1][1].re = inband[1][1];
  outband[1][1].im = 0.0;

  /* first line */
  FOR (x=2; x LE xsize; x++)
    even = (inband[1][x] + inband[1][xsizep2-x])/2.0;
    odd  = (inband[1][x] - inband[1][xsizep2-x])/2.0;
    outband[1][x].re         = even;
    outband[1][x].im         = -odd;
    outband[1][xsizep2-x].re = even;
    outband[1][xsizep2-x].im =  odd;
  ENDFOR;

  /* general line loop */
  FOR (y=2; y LE ysize/2+1; y++)
    /* first sample */
    even = (inband[y][1] + inband[ysizep2-y][1])/2.0;
    odd  = (inband[y][1] - inband[ysizep2-y][1])/2.0;
    outband[y][1].re         = even;
    outband[y][1].im         = -odd;
    outband[ysizep2-y][1].re = even;
    outband[ysizep2-y][1].im =  odd;

    /* general sample loop */
    FOR (x=2; x LE xsize; x++)
      even = (inband[y][x] + inband[ysizep2-y][xsizep2-x])/2.0;
      odd  = (inband[y][x] - inband[ysizep2-y][xsizep2-x])/2.0;
      outband[y][x].re = even;
      outband[ysizep2-y][xsizep2-x].re = even;
      outband[y][x].im                 = -odd;
      outband[ysizep2-y][xsizep2-x].im =  odd;
    ENDFOR;
  ENDFOR

  return(0);
}

#endif /* not MAIN */

/*P:ht2ft*

________________________________________________________________

		ht2ft
________________________________________________________________

Name:		ht2ft - 2d hartley to fourier conversion

Syntax:		ht2ft [-t <title>] <inimage> <outimage>

Description:	'ht2ft' transforms a (single precision) real Hartley
		transform to a (single precision) complex Fourier
                transform. Note that the Hartley transform is faster
		than Fourier. If you need Fourier it may be faster
		to compute the Hartley transform and then convert to
		Fourier by means of ht2ft.

                The Fourier transform is given by the Hartley transform
		as follows:

		| F(u,v) = Re{F(u,v)} + j Im{F(u,v)}
		|        = H_e(u,v) - j H_o(u,v)
		|        = (H(u,v) + H(-u,-v))/2 - j (H(u,v) - H(-u,-v))/2

		For images with horizontal and vertical even symmetry
		H_o(u,v) equals zero, so that
		| F(u,v) = H_e(u,v) = H(u,v)

See also:	ht2ft(3), fft2d(1), fht2d(1), ft2ht(1)

Restrictions:   Input must be a single precision hartley transform

Return value:   | 0 => ok
                | 1 => bad number of arguments
		| 2 => couldn't read input file
		| 3 => couldn't write output file

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	ht2ft monah.img monaf.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  int bn, nbands, stat;
  char *title, *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-t <title>] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  title = read_switch(&argc, argv, "-t", 1, (char*)0);
  title = read_switch(&argc, argv, "-title", 1, title);

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1 = Iread_image(argv[1]);
  if (NOT img1) Error(2, "Can't read input %s.\n", argv[1]);

  nbands = Inbands(img1);
  if (NOT title) title = Ititle(img1);
  
  img2 = Init_image(nbands, title);
  Icopy_text(img1, img2);

  FOR (bn=1; bn LE nbands; bn++)
    img2[bn] = Imake_band(Icomplex_typ, Ixsize(img1[bn]), Iysize(img1[bn]));
    stat = ht2ft((IR_BAND)img1[bn], (IC_BAND)img2[bn]);
    if (stat EQ 1)
      Error(2, "Bad input pixel type band %d.\n", bn);
    else if(stat EQ 3)
      Error(2, "Bad size in band %d.\n", bn);
  ENDFOR;

  Ihistory(img2, argv[0], args);
  if (Iwrite_image(img2, argv[2]))
    Error(2, "Can't write output %s.\n", argv[2]);

  return(0);
}

#endif /* MAIN */
