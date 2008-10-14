/*

________________________________________________________________

        biffConvert
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
#include <xite/convert.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#define FORALLYX for (y=1; y <= ysize; y++) {	\
        for (x=1; x <= xsize; x++) {
#define ENDFORALLYX } }



/*P:biffConvert*

________________________________________________________________

		biffConvert
________________________________________________________________

Name:		biffConvert - convert image between different pixel types

Syntax:		| biffConvert <inimage> <outimage> <pixeltype>
                |    [<pixeltype>...]

Description:    'biffConvert' reads the input image <inimage>, converts
                every band to specified pixel type, and writes output
		image to <outimage>. First pixel type specification is
		applied to first band, second to second,.. If more bands,
		the last pixel type specification is applied to the rest
		of the bands. Pixel types are specified by a pixel type
		number:

		| ub or  3 => convert to unsigned byte
		| sb or  4 => convert to signed byte
		| us or  5 => convert to unsigned short
		| ss or  6 => convert to signed short
		| i  or  7 => convert to integer
		| r  or  8 => convert to real
		| c  or  9 => convert to complex
		| d  or 10 => convert to double
		| dc or 11 => convert to double complex

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | # convert to real: 
                | biffConvert mona.img monaReal.img 8
                |
                | # convert to byte:
                | biffConvert spot.img spotByte.img 3
                |
                | # convert 1st to byte, 2nd to integer, 3rd to real:
                | biffConvert spot.img spotBIR.img 3 7 8 
                |
                | # convert first band to byte and short:
                | biffConvert mona.img:1:1 monaBS.img 3 5 

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn;
  IPIXTYP pt;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage> <pixtyp> [<pixtyp>...]\n\
        where pixtyp may be \n\
          ub or 3 => unsigned byte,    sb or  4 => signed byte, \n\
          us or 5 => unsigned short,   ss or  6 => signed short, \n\
          i  or 7 => integer,          r  or  8 => real, \n\
          c  or 9 => complex,          d  or 10 => double, \n\
          dc or 11 => double complex\n"));

  if (argc == 1) Usage(1, NULL);
  if (argc < 4) Usage(2, "Illegal number of arguments.\n");
  
  i1 = Iread_image(argv[1]);
  if (NOT i1) exit(Error(3, "Couldn't read %s\n", argv[1]));
  i2 = Init_image(Inbands(i1), Ititle(i1));
  if (NOT (size_t)i2) exit(Error(4, "Malloc failed\n"));
  Icopy_text(i1, i2);
  for (bn=1; bn LE Inbands(i1); bn++) {
    pt = (argc GE bn+3) ? IparsePixtyp(argv[bn+2]) : pt;
    i2[bn] = mkConvertBand(i1[bn], pt);
    if (NOT i2[bn]) {
      Warning(5, "Couldn't convert band %d\n", bn);
      i2[bn] = i1[bn];
    }
  }
  
  Ihistory(i2, argv[0], "");
  Iwrite_image(i2, argv[2]);
  return(0);
}
