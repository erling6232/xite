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



#ifndef MAIN

/*F:overlap*

________________________________________________________________

		overlap
________________________________________________________________

Name:		overlap - calculate overlapping area of two bands

Syntax:         | #include <xite/copy.h>
		|
                | int overlap( IBAND ib1, IBAND ib2, IBAND* ob1,
                |    IBAND* ob2 );

Description:    Input bands 'ib1' and 'ib2' are placed in a common
		coordinate system according to their (xstart, ystart)
		attributes. The overlapping area is calculated, and
		two subbands 'ob1' and 'ob2' are created and returned,
		pointing out the overlapping area in each band.
		'ob1' will thus be a subband of 'ib1', and 'ob2' will
		be a subband of 'ib2'.

Restrictions:   Only accepts unsigned byte pixeltype.

Return value:   | 0 => ok, 
		| 1 => bad pixel type input band 1
		| 2 => bad pixel type input band 2
		| 3 => no overlap

Author:		Tor L�nnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

#ifndef FUNCPROTO
int overlap(ib1, ib2, ob1, ob2)
IBAND ib1, ib2;
IBAND *ob1, *ob2;
#else /* FUNCPROTO */
int overlap(IBAND ib1, IBAND ib2, IBAND *ob1, IBAND *ob2)
#endif /* FUNCPROTO */
{
   int xstart1, ystart1, xstart2, ystart2, xstart, ystart,
       xsize1, ysize1, xsize2, ysize2, xsize, ysize, diff;

   if (Ipixtyp(ib1) != Iu_byte_typ)
     return(Error(1, "Input band 1 must be unsigned byte.\n"));
   if (Ipixtyp(ib2) != Iu_byte_typ)
     return(Error(2, "Input band 2 must be unsigned byte.\n"));

   diff = Ixstart(ib2)-Ixstart(ib1)+1;
   xstart1 = MAX(diff, 1);
   diff = Iystart(ib2)-Iystart(ib1)+1;
   ystart1 = MAX(diff, 1);
   diff = Ixstart(ib1)-Ixstart(ib2)+1;
   xstart2 = MAX(diff, 1);
   diff = Iystart(ib1)-Iystart(ib2)+1;
   ystart2 = MAX(diff, 1);
   xsize1 = Ixsize(ib1) - xstart1 + 1;
   ysize1 = Iysize(ib1) - ystart1 + 1;
   xsize2 = Ixsize(ib2) - xstart2 + 1;
   ysize2 = Iysize(ib2) - ystart2 + 1;
   xsize = MIN(xsize1, xsize2);
   ysize = MIN(ysize1, ysize2);

   if ((xsize <= 0) || (ysize <= 0)) {
     ob1 = (IBAND*)0;
     ob2 = (IBAND*)0;
     return(3);
   }
   *ob1   = Imake_subband(ib1, xstart1, ystart1, xsize, ysize);
   *ob2   = Imake_subband(ib2, xstart2, ystart2, xsize, ysize);
   xstart = MAX(Ixstart(ib1), Ixstart(ib2));
   ystart = MAX(Iystart(ib1), Iystart(ib2));
   Iset_start(*ob1, xstart, ystart);
   Iset_start(*ob2, xstart, ystart);

   return(0);
}

#endif /* not MAIN */

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

Author:		Tor L�nnestad, BLAB, Ifi, UiO
Examples:       | overlap mona.img lenna.img monaOlp.img lennaOlp.img
Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#include XITE_STDIO_H

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
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

#endif /* MAIN */
