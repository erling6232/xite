
/*

________________________________________________________________

        logical.c
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

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:logical_and=logical*/
/*F:logical_or=logical*/
/*F:logical_xor=logical*/
/*F:logical_not=logical*/
/*F:logical_band=logical*/
/*F:logical_bor=logical*/
/*F:logical_bxor=logical*/
/*F:logical_bnot=logical*/
/*F:logical_lshift=logical*/
/*F:logical_rshift=logical*/
/*F:logical_eq=logical*/
/*F:logical_ne=logical*/
/*F:logical_lt=logical*/
/*F:logical_le=logical*/
/*F:logical_gt=logical*/
/*F:logical_ge=logical*/
/*F:logical_mask=logical*/
/*F:logical_bandc=logical*/
/*F:logical_borc=logical*/
/*F:logical_bxorc=logical*/
/*F:logical_eqc=logical*/
/*F:logical_nec=logical*/
/*F:logical_ltc=logical*/
/*F:logical_lec=logical*/
/*F:logical_gtc=logical*/
/*F:logical_gec=logical*/
/*F:logical*

________________________________________________________________

		logical
________________________________________________________________

Name:		logical, logical_and, logical_or, logical_xor, logical_not,
                logical_band, logical_bor, logical_bxor, logical_bnot,
                logical_lshift, logical_rshift, logical_eq, logical_ne,
                logical_lt, logical_le, logical_gt, logical_ge, logical_mask,
                logical_bandc, logical_borc, logical_bxorc, logical_eqc,
                logical_nec, logical_ltc, logical_lec, logical_gtc,
                logical_gec - logical operators on bands

Syntax:         | #include <xite/arithmetic.h>
		|
                | int logical_and( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_or( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_xor( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_not( IBAND input, IBAND output );
		|
                | int logical_band( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_bor( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_bxor( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_bnot( IBAND input, IBAND output );
		|
                | int logical_lshift( IBAND input, IBAND output,
                |    int n );
		|
                | int logical_rshift( IBAND input, IBAND output,
                |    int n );
		|
                | int logical_eq( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_ne( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_lt( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_le( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_gt( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_ge( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_mask( IBAND input1, IBAND input2,
                |    IBAND output );
		|
                | int logical_bandc( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_borc( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_bxorc( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_eqc( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_nec( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_ltc( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_lec( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_gtc( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
                | int logical_gec( IBAND input, IBAND output,
                |    UNS_BYTE c );
		|
Description:    logical is a collection of logical operators on bands.
		For every pixel position, the unary or binary operator
		is performed on the input band(s), and the result is
		assigned to the output band.

		| logical_and    - logical and (MIN operator)
		| logical_or     - logical or  (MAX operator)
		| logical_xor    - logical xor
		| logical_not    - logical not
		| logical_band   - bitwise and
		| logical_bor    - bitwise or
		| logical_bxor   - bitwise xor
		| logical_bnot   - bitwise not
		| logical_lshift - left shift
		| logical_rshift - right shift
		| logical_eq     - equality
		| logical_ne     - not equality
		| logical_lt     - less than
		| logical_le     - less equal
		| logical_gt     - greater than
		| logical_ge     - greater equal
		| logical_mask   - (input2) ? input1 : 0
		| logical_bandc  - bitwise and with constant c
		| logical_borc   - bitwise or with constant c
		| logical_bxorc  - bitwise xor with constant c
		| logical_eqc    - equal to constant
		| logical_nec    - not equal to constant
		| logical_ltc    - less than constant
		| logical_lec    - less equal constant
		| logical_gtc    - greater than constant
		| logical_gec    - greater equal constant

		Only the largest rectangle common for all bands
		is processed. Identical actual band arguments are
		allowed.

Restrictions:   Only pixeltype unsigned byte is allowed.

Return value:   | 0 => ok
		| 1 => bad pixel value input band 1
		| 2 => bad pixel value input band 2
		| 3 => bad pixel value output band

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/



int logical_and(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = MIN(input1[y][x], input2[y][x]);
   return(0);
}  /* END of logical_and */



int logical_or(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = MAX(input1[y][x], input2[y][x]);
   return(0);
}  /* END of logical_or */



#define XOR(a,b) ((a) ? (! (b)) : (b))

int logical_xor(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = XOR(input1[y][x], input2[y][x]);
   return(0);
}  /* END of logical_xor */



int logical_not(IBAND input, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = !input[y][x];
   return(0);
}  /* END of logical_not */



int logical_band(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = (input1[y][x] & input2[y][x]);
   return(0);
}  /* END of logical_band */



int logical_bor(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = (input1[y][x] | input2[y][x]);
   return(0);
}  /* END of logical_bor */



#define BXOR ^

int logical_bxor(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = (input1[y][x] BXOR input2[y][x]);
   return(0);
}  /* END of logical_bxor */



int logical_bnot(IBAND input, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = ~input[y][x];
   return(0);
}  /* END of logical_bnot */



int logical_lshift(IBAND input, IBAND output, int n)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] << n;
   return(0);
}  /* END of logical_lshift */



int logical_rshift(IBAND input, IBAND output, int n)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] >> n;
   return(0);
}  /* END of logical_rshift */



int logical_eq(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input1[y][x] == input2[y][x];
   return(0);
}  /* END of logical_eq */



int logical_ne(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input1[y][x] != input2[y][x];
   return(0);
}  /* END of logical_ne */



int logical_lt(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input1[y][x] < input2[y][x];
   return(0);
}  /* END of logical_lt */



int logical_le(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input1[y][x] <= input2[y][x];
   return(0);
}  /* END of logical_le */



int logical_gt(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input1[y][x] > input2[y][x];
   return(0);
}  /* END of logical_gt */



int logical_ge(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input1[y][x] >= input2[y][x];
   return(0);
}  /* END of logical_ge */



int logical_mask(IBAND input1, IBAND input2, IBAND output)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(input2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(MIN(Ixsize(input1),Ixsize(input2)),Ixsize(output));
   ysize = MIN(MIN(Iysize(input1),Iysize(input2)),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input2[y][x] ? input1[y][x] : 0;
   return(0);
}  /* END of logical_mask */



int logical_bandc(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] & c;
   return(0);
}  /* END of logical_bandc */



int logical_borc(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] | c;
   return(0);
}  /* END of logical_borc */



int logical_bxorc(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] BXOR c;
   return(0);
}  /* END of logical_bxorc */



int logical_eqc(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] == c;
   return(0);
}  /* END of logical_eqc */



int logical_nec(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] != c;
   return(0);
}  /* END of logical_nec */



int logical_ltc(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] < c;
   return(0);
}  /* END of logical_ltc */



int logical_lec(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] <= c;
   return(0);
}  /* END of logical_lec */



int logical_gtc(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] > c;
   return(0);
}  /* END of logical_gtc */



int logical_gec(IBAND input, IBAND output, UNS_BYTE c)
{
   int x, y, xsize, ysize;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte pixel type.\n"));
   if (Ipixtyp(output) != Iu_byte_typ)
     return(Error(3, "Output band must be unsigned byte pixel type.\n"));

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   for (y=1; y <= ysize; ++y)
     for (x=1; x <= xsize; ++x)
       output[y][x] = input[y][x] >= c;
   return(0);
}  /* END of logical_gec */

#endif /* not MAIN */



/*P:logical*

________________________________________________________________

		logical
________________________________________________________________

Name:		logical - logical pixel by pixel operations

Syntax:		| logical -and    <inimage1> <inimage2> <outimage>
		| logical -or     <inimage1> <inimage2> <outimage>
		| logical -xor    <inimage1> <inimage2> <outimage>
		| logical -not    <inimage>  <outimage>
		| logical -band   <inimage1> <inimage2> <outimage>
		| logical -bor    <inimage1> <inimage2> <outimage>
		| logical -bxor   <inimage1> <inimage2> <outimage>
		| logical -bnot   <inimage>  <outimage>
		| logical -eq     <inimage1> <inimage2> <outimage>
		| logical -ne     <inimage1> <inimage2> <outimage>
		| logical -lt     <inimage1> <inimage2> <outimage>
		| logical -le     <inimage1> <inimage2> <outimage>
		| logical -gt     <inimage1> <inimage2> <outimage>
		| logical -ge     <inimage1> <inimage2> <outimage>
		| logical -lshift <inimage>  <outimage> [<n>]
		| logical -rshift <inimage>  <outimage> [<n>]
		| logical -mask   <inimage1> <inimage2> <outimage>
		| logical -bandc  <inimage>  <outimage> <c>
		| logical -borc   <inimage>  <outimage> <c>
		| logical -bxorc  <inimage>  <outimage> <c>
		| logical -eqc    <inimage>  <outimage> <c>
		| logical -nec    <inimage>  <outimage> <c>
		| logical -ltc    <inimage>  <outimage> <c>
		| logical -lec    <inimage>  <outimage> <c>
		| logical -gtc    <inimage>  <outimage> <c>
		| logical -gec    <inimage>  <outimage> <c>

Description:    'logical' performs logical operations on images, pixel
                by pixel, unary and binary operators. The option
		specifies what kind of operator. There should be
		exactly one option. The following list shows the
		defined options, whether unary or binary, and the
		corresponding operator. Unary operators require
		exactly one input image, binary ones require exactly
		two. 

		| -and       binary         MIN(input1, input2) 
		| -or        binary         MAX(input1, input2)
		| -xor       binary         input1 xor input2
		| -not       unary          not input
                | -band      binary         input1 bitwise and input2
		| -bor       binary         input1 bitwise or input2
		| -bxor      binary         input1 bitwise xor input2
		| -bnot      unary          bitwise not input
		| -eq        binary         input1 equal input2
		| -ne        binary         input1 not equal input2
		| -lt        binary         input1 less than input2
		| -le        binary         input1 less equal input2
		| -gt        binary         input1 greater than input2
		| -ge        binary         input1 greater equal input2
		| -lshift    unary          left shift input n bits
		| -rshift    unary          right shift input n bits
		| -mask      binary         if (input2) then input1 else 0
		| -bandc     unary          input1 bitwise and constant c
		| -borc      unary          input1 bitwise or constant c
		| -bxorc     unary          input1 bitwise xor constant c
		| -eqc       unary          input1 equal constant c
		| -nec       unary          input1 not equal constant c
		| -ltc       unary          input1 less than constant c
		| -lec       unary          input1 less equal constant c
		| -gtc       unary          input1 greater than constant c
		| -gec       unary          input1 greater equal constant c

		All bands with UNS_BYTE pixels will be processed, 
		assuming (for binary operators) that the band exists in
		both images. Only the largest rectangle commmon to each
		pair of bands will be processed. For left shift and right
		shift, the default value of <n> is 1. For operations with
		constant argument <c>, the value of <c> should be an
		unsigned byte, 0..255.

See also:	combine(1), logical(3), logical_and(3), logical_or(3),
                logical_xor(3), logical_not(3), logical_band(3),
                logical_bor(3), logical_bxor(3), logical_bnot(3),
                logical_lshift(3), logical_rshift(3), logical_eq(3),
                logical_ne(3), logical_lt(3), logical_le(3), logical_gt(3),
                logical_ge(3), logical_mask(3), logical_bandc(3),
                logical_borc(3), logical_bxorc(3), logical_eqc(3),
                logical_nec(3), logical_ltc(3), logical_lec(3),
                logical_gtc(3), logical_gec(3)

Restrictions:   Accepts only unsigned-byte pixels.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | logical -mask mona.img mask.img monaface.img 

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2, i3;
   int bn, nbands, n, sum_sw, binary, xsize, ysize, status, cons;
   int and_, or_, xor_, not_, band_, bor_, bxor_, bnot_,
       eq_, ne_, lt_, le_, gt_, ge_, lshift_, rshift_,
       mask_, bandc_, borc_, bxorc_,
       eqc_, nec_, ltc_, lec_, gtc_, gec_;
   UNS_BYTE c;
   char *args;

   Iset_message(1);
   Iset_abort(1);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [<option>...} <inimage1> [<inimage2>] <outimage>\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

#define READ_SWITCH(sw) \
  ((read_switch(&argc, argv, #sw, 0, NULL) != NULL) ? 1 : 0)

   and_    = READ_SWITCH(-and);
   or_     = READ_SWITCH(-or);
   xor_    = READ_SWITCH(-xor);
   not_    = READ_SWITCH(-not);
   band_   = READ_SWITCH(-band);
   bor_    = READ_SWITCH(-bor);
   bxor_   = READ_SWITCH(-bxor);
   bnot_   = READ_SWITCH(-bnot);
   eq_     = READ_SWITCH(-eq);
   ne_     = READ_SWITCH(-ne);
   lt_     = READ_SWITCH(-lt);
   le_     = READ_SWITCH(-le);
   gt_     = READ_SWITCH(-gt);
   ge_     = READ_SWITCH(-ge);
   lshift_ = READ_SWITCH(-lshift);
   rshift_ = READ_SWITCH(-rshift);
   mask_   = READ_SWITCH(-mask);
   bandc_  = READ_SWITCH(-bandc);
   borc_   = READ_SWITCH(-borc);
   bxorc_  = READ_SWITCH(-bxorc);
   eqc_    = READ_SWITCH(-eqc);
   nec_    = READ_SWITCH(-nec);
   ltc_    = READ_SWITCH(-ltc);
   lec_    = READ_SWITCH(-lec);
   gtc_    = READ_SWITCH(-gtc);
   gec_    = READ_SWITCH(-gec);

   sum_sw = and_ + or_ + xor_ + not_ + band_ + bor_ + bxor_ + bnot_ +
            eq_ + ne_ + lt_ + le_ + gt_ + ge_ + lshift_ + rshift_ +
            mask_ + bandc_ + borc_ + bxorc_ +
            eqc_ + nec_ + ltc_ + lec_ + gtc_ + gec_;
   binary = (and_ || or_ || xor_ || band_ || bor_ || bxor_
            || eq_ || ne_ || lt_ || le_ || gt_ || ge_ || mask_);
   cons   = (bandc_ || borc_ || bxorc_ || eqc_ || nec_
            || ltc_ || lec_ || gtc_ || gec_ );

   if (sum_sw != 1) Usage(1, "Illegal number of arguments.\n");
 
   i1 = Iread_image(argv[1]);
   if (binary) {
     i2 = Iread_image(argv[2]);
     nbands = MIN(Inbands(i1), Inbands(i2));
   } else {
     nbands = Inbands(i1);
   }
   i3 = Init_image(nbands, "Result of logical");
   if (lshift_ || rshift_)
     if(argc >= 4) n = atoi(argv[3]); else n = 1;
   if (cons) c = (UNS_BYTE)atoi(argv[3]);

   for (bn=1; bn <= nbands; ++bn) {
     if (binary) {
       xsize = MIN(Ixsize(i1[bn]),Ixsize(i2[bn]));
       ysize = MIN(Iysize(i1[bn]),Iysize(i2[bn]));
     } else {
       xsize = Ixsize(i1[bn]);
       ysize = Iysize(i1[bn]);
     }
     i3[bn] = Imake_band(Iu_byte_typ, xsize, ysize);

          if (and_)    status = logical_and(   i1[bn], i2[bn], i3[bn]);
     else if (or_)     status = logical_or(    i1[bn], i2[bn], i3[bn]);
     else if (xor_)    status = logical_xor(   i1[bn], i2[bn], i3[bn]);
     else if (not_)    status = logical_not(   i1[bn],         i3[bn]);
     else if (band_)   status = logical_band(  i1[bn], i2[bn], i3[bn]);
     else if (bor_)    status = logical_bor(   i1[bn], i2[bn], i3[bn]);
     else if (bxor_)   status = logical_bxor(  i1[bn], i2[bn], i3[bn]);
     else if (bnot_)   status = logical_bnot(  i1[bn],         i3[bn]);
     else if (eq_)     status = logical_eq(    i1[bn], i2[bn], i3[bn]);
     else if (ne_)     status = logical_ne(    i1[bn], i2[bn], i3[bn]);
     else if (lt_)     status = logical_lt(    i1[bn], i2[bn], i3[bn]);
     else if (le_)     status = logical_le(    i1[bn], i2[bn], i3[bn]);
     else if (gt_)     status = logical_gt(    i1[bn], i2[bn], i3[bn]);
     else if (ge_)     status = logical_ge(    i1[bn], i2[bn], i3[bn]);
     else if (lshift_) status = logical_lshift(i1[bn],         i3[bn], n);
     else if (rshift_) status = logical_rshift(i1[bn],         i3[bn], n);
     else if (mask_)   status = logical_mask(  i1[bn], i2[bn], i3[bn]);
     else if (bandc_)  status = logical_bandc( i1[bn],         i3[bn], c);
     else if (borc_)   status = logical_borc(  i1[bn],         i3[bn], c);
     else if (bxorc_)  status = logical_bxorc( i1[bn],         i3[bn], c);
     else if (eqc_)    status = logical_eqc(   i1[bn],         i3[bn], c);
     else if (nec_)    status = logical_nec(   i1[bn],         i3[bn], c);
     else if (ltc_)    status = logical_ltc(   i1[bn],         i3[bn], c);
     else if (lec_)    status = logical_lec(   i1[bn],         i3[bn], c);
     else if (gtc_)    status = logical_gtc(   i1[bn],         i3[bn], c);
     else if (gec_)    status = logical_gec(   i1[bn],         i3[bn], c);

     if (status) Error(2, "Error in band %d.\n", bn);
   }

   Ihistory(i3, argv[0], args);
   if (binary) Iwrite_image(i3, argv[3]);
   else Iwrite_image(i3, argv[2]);

   return(0);
}

#endif /* MAIN */
