

/*

________________________________________________________________

        biffmerge.c
        $Id$
        Copyright 1990, B-lab
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and its
  documentation for any purpose and without fee is hereby granted, 
  provided that the above copyright notice appear in all copies and that 
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

/*P:biffmerge*

________________________________________________________________

		biffmerge
________________________________________________________________

Name:		biffmerge - merge several images into one image 

Syntax:		| biffmerge <input1> [<input2> .... <inputn>] <output>

Description:    The program is used to combine bands from several
                images into a new image. It can also be used to turn
		around the band sequence of an image, or to select
		some of the bands in an image.

		Along with each input image file name you may specify
		one or several band ranges. A band range is either a
		band number, or a band number followed by a "-"
		followed by another band number. The last type means
		all bands between and including the two band numbers.
		Band ranges are separated with a ":", and there may be
		no white space between the band ranges, or between the
		file name and the first band range.

Author:		Tor Lønnestad, BLAB, ifi, UiO

Examples:	| biffmerge lenna.img mona.img girls.img
                | biffmerge rgb.img:2 g.img
		| biffmerge rgb.img:3-1 bgr.img
		| biffmerge r.img:1:1:1 rrr.img
		| biffmerge lenna.img rgb.img:1:3-1:2:2:1 chaos.img

Id:		$Id$ $Revision$
________________________________________________________________

*/
static char *Id = "$Id$ $Revision$";

#ifdef MAIN

#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/message.h>
#include XITE_STDIO_H
#include XITE_TYPES_H

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char *argv[])
#endif /* FUNCPROTO */
{
  IMAGE i1, i2;
  int imgnr, bandnr1, bandnr2=0;

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inp1> [<inp2> .. <inpn>] <output>\n"));

  if (argc == 1) Usage(1, NULL);
  if (argc <  3) Usage(1, "Illegal number of arguments.\n");

  FOR (imgnr=1; imgnr LT argc-1; INC imgnr)
    i1 = Iread_image(argv[imgnr]);
    IF ((size_t)i1)
      IF (bandnr2)
        Iset_nbands(&i2,Inbands(i2)+Inbands(i1));
        for (bandnr1=1; bandnr1 LE Inbands(i1); INC bandnr1)
          i2[++bandnr2] = i1[bandnr1];
      ELSE
        i2 = i1;
        bandnr2 = Inbands(i2);
      ENDIF;
    ELSE
      fprintf(stderr,"%s: couldn't find or open image: %s\n",
        argv[0], argv[imgnr]);
    ENDIF;
  ENDFOR;

  IF (bandnr2)
    Ihistory(i2, argv[0], "");
    Iwrite_image(i2,argv[argc-1]);
  ENDIF;
   return(0);
}

#endif /* MAIN */
