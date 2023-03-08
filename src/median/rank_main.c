
/*

________________________________________________________________

        rank
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
#include <xite/median.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>

/*P:rank*

________________________________________________________________

		rank
________________________________________________________________

Name:		rank - noise reduction filter

Syntax:		rank <inimage> <outimage> <rank> [<dx> [<dy>]]

Description:    'rank' noise filtering with window 'dx'*'dy'
                using the histogram update technique.
		Every input pixel is replaced by the rank-th
		pixel in a sorted vector containg all pixels
		in the dx*dy window. rank=1 gives the min
		filter, rank=dx*dy/2 gives the median filter,
		and rank = dx*dy gives the max filter.

                | inimage - input image
		| outimage - output image
		| rank - the rank of the output pixel
		| dx - horizontal window size. Default value 3
		| dy - vertical window size. Default value dx

Files:	
See also:	
Diagnostics:

Restrictions:   Accepts only UNS_BYTE pixels. dx, dy must be
                larger than 0. rank should be in [1..dx*dy]

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:       | rank mona.img monaRank.img 1
                | rank mona.img monaRank.img 3 7
		| rank mona.img monaRank.img 35 7 5

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1,i2;
   int bn, dx, dy, rnk;
   char arg[40];

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage> <rank> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   if ((argc < 4) OR (argc GT 6)) Usage(1, "Illegal number of arguments.\n");

   rnk = atoi(argv[3]);
   if (argc GE 5) dx = atoi(argv[4]); else dx = 3;
   if (argc GE 6) dy = atoi(argv[5]); else dy = dx;
   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window size should be greater than 0.\n");

   if ((rnk GT dx*dy) OR (rnk LT 1))
     Error(2, "Rank value should be in 1..dx*dy\n");
  
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (rank(i1[bn], i2[bn], rnk, dx, dy))
       Error(2, "Error in band %d\n", bn);

   sprintf(arg, " rank = %d, dx = %d, dy = %d", rnk, dx, dy);
   Ihistory(i2, argv[0], arg);
   Iwrite_image(i2,argv[2]);
   return(0);
}
