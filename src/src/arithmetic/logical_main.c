
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
   if (lshift_ || rshift_) {
     if(argc >= 4) {n = atoi(argv[3]);} else {n = 1;}
   }
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
