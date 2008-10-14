/*

________________________________________________________________

        overlap.c
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
#include <stdlib.h>
#include <xite/copy.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif



/*P:overlap*

________________________________________________________________

		overlap
________________________________________________________________

Name:		overlap - calculate overlapping band areas

Syntax:		| overlap <input1> <input2> <output1> <output2>

Description:    For every pair of bands, input1[i] and input2[i],
                calculate the overlapping area when taking the
		xstart and ystart parameters into consideration.
		Create two new images containing the overlapping
		parts of the two input images. If any pair of
		bands have no overlapping area, no output band
		will be created for the pair.

Restrictions:   Only accepts unsigned byte pixeltype.

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Examples:       | overlap mona.img lenna.img monaOlp.img lennaOlp.img
Id: 		$Id$
________________________________________________________________

*/

#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif

int main(int argc, char **argv)
{
   IMAGE ii1, ii2, oi1, oi2;
   int ibn, obn, ibns, obns;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <input1> <input2> <output1> <output2>\n"));

   if (argc == 1) Usage(1, NULL);
   if (argc != 5) Usage(1, "Illegal number of arguments.\n");

   ii1  = Iread_image(argv[1]);
   ii2  = Iread_image(argv[2]);
   ibns = MIN(Inbands(ii1), Inbands(ii2));
   oi1  = Init_image(ibns, Ititle(ii1));
   oi2  = Init_image(ibns, Ititle(ii2));
   for (ibn=1, obn=1; ibn <= ibns; ++ ibn)
     if (overlap(ii1[ibn], ii2[ibn], &oi1[obn], &oi2[obn]) == 0)
       ++ obn;
   obns = obn-1;

   if (obns < 1) Error(2, "No overlap.\n");

   Iset_nbands(&oi1, obns);
   Iset_nbands(&oi2, obns);
   Iwrite_image(oi1, argv[3]);
   Iwrite_image(oi2, argv[4]);
  return(0);
}
