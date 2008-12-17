
/*

________________________________________________________________

        
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/segmentation.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H

/*P:segmRandom*

________________________________________________________________

		segmRandom
________________________________________________________________

Name:		segmRandom - Create image with two regions, random border.

Syntax:		| segmRandom [-xsize <xsize>] [-ysize <ysize>] \\
                |       [-ll <p(l|l)>] [-rl <p(r|l)>] [-ld <p(l|d)>] \\
		|	[-rd <p(r|d)>] [-lr <p(l|r>)] [-rr <p(r|r)>] \\
		|       [-lpv <lpv>] [-rpv <rpv>] [-title <title>] \\
		|       <outimage>

Description:	<outimage> is created with two regions, value <lpv>
                (left pixel value) on the left part, and <rpv> on the
		right. The border separating the regions is made with
		a kind of random walk, or 2. order markow process.

		The probablity for going in each of the two directions
		down-left and down-right given the prevous direction,
		are given in the switches -ll, -rl, -ld, -rd, -lr and -rr.
		The probability for going down is computed, because the
		total probability always is equal one.

		| The default values are:
		|           -xsize = 256
		|	    -ysize = 256
		|           -lpv   = 0
		|	    -rpv   = 1
                |           -ll    = 1/3 	p(left | left)
		|	    -rl    = 1/3 	p(right | left)
                |           -ld    = 1/3 	p(left | down)
		|	    -rd    = 1/3 	p(right | down)
                |           -lr    = 1/3 	p(left | rigth)
		|	    -rr    = 1/3 	p(right | right)

		Changing one of the probabilities wil not change any of
		the other probabilities.

Restrictions:	Left+right probability must be less than or equal one.
                (ll+rl<=1) , (ld+rd<=1) and (lr+rr<=1)

Return value:	| 0 : OK.
                | 1 : Error (Usage).
                | 2 : Bad number of arugments.

Author:		Martin Torpe Lie, BLAB, ifi, UiO 

Examples:	segmRandom test.img
                segmRandom -xsize 64 -ysize 64 -ll 0.5 -rr 0.5 test.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE outimage;
  char *title, args[80];
  int xsize, ysize, lpv, rpv;
  double prob[3][2];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-xsize <xsize>] [-ysize <ysize>] \n\
        [-ll <p(l|l)>] [-rl <p(r|l)>] [-ld <p(l|d)>] \n\
        [-rd <p(r|d)>] [-lr <p(l|r>)] [-rr <p(r|r)>] \n\
        [-lpv <lpv>] [-rpv >rpv>] [-title <title>]  <outimage>\n"));

  prob[0][0] = read_dswitch(&argc, argv, "-ll", 1/3.0);
  prob[0][1] = read_dswitch(&argc, argv, "-rl", 1/3.0);
  prob[1][0] = read_dswitch(&argc, argv, "-ld", 1/3.0);
  prob[1][1] = read_dswitch(&argc, argv, "-rd", 1/3.0);
  prob[2][0] = read_dswitch(&argc, argv, "-lr", 1/3.0);
  prob[2][1] = read_dswitch(&argc, argv, "-rr", 1/3.0);
  xsize = read_iswitch(&argc, argv, "-xsize", 256);
  ysize = read_iswitch(&argc, argv, "-ysize", 256);
  lpv   = read_iswitch(&argc, argv, "-lpv", 0);
  rpv   = read_iswitch(&argc, argv, "-rpv", 1);
  title = read_switch(&argc, argv, "-title", 1, "Random walk");

  if (argc == 1) Usage(1,(char*)0);
  if (argc != 2) Usage(2,"Bad number of arguments.\n");

  outimage = Imake_image(1, title, Iu_byte_typ, xsize, ysize);
  segmRandom(outimage[1], prob, lpv, rpv);

  sprintf(args, "-ll:%g -rl:%g -ld:%g -rd:%g -lr:%g -rr:%g ",
    prob[0][0], prob[0][1], prob[1][0], prob[1][1], prob[2][0], prob[2][1]);
  Ihistory(outimage, argv[0], args);
  Iwrite_image(outimage, argv[1]);
   return(0);
}
