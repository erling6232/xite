
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
