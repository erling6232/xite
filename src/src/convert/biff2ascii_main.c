

/*

________________________________________________________________

        biff2ascii
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
#include <xite/convert.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif
#include <xite/blab.h>
#include <xite/utils.h>
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*P:biff2ascii*

________________________________________________________________

		biff2ascii
________________________________________________________________

Name:		biff2ascii - dump part of an image in ascii format

Syntax:		| biff2ascii [<options>...] <inimage> <outfile>
                | biff2ascii [<options>...] -o <outfile> <inimage>

Description:    Writes pixel values in the specified 'inimage' to
                'outfile' in ascii format. Use '-' for 'outfile' to write
		to stdout. Specify "filename:n" as input image to dump
		band n only.

Options:        &-o file
                Send output to 'file'. This option is retained for backwards
		compatibility.

		&-xstart xstart
		Start dump at 'xstart'. Default 1.

		&-ystart ystart
		Start dump at 'ystart'. Default 1.

		&-xstop xstop
		Stop dump at 'xstop'. Default xsize.

		&-ystop ystop
		Stop dump at 'ystop'. Default ysize.

		&-xsize xsize
		Horizontal window size.

		&-ysize ysize
		Vertical window size.

		&-co
		Add coordinate system.

		-&If 'start', 'stop' as well as size options are given, and
		inconsistently, the 'stop' option is ignored. The given
		sizes are clipped against the band.

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Revised:        | Otto Milvang: Added all pixeltypes,
                | Svein Bøe: Cleaned up.
                
Id:             $Id$

Examples:	| biff2ascii -o mona.txt mona.img
		| biff2ascii -co -xsize 10 -ysize 20 mona.img 
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE i;
  IBAND b;
  int xstart, ystart, xsize, ysize, xstop, ystop, bxsize, bysize, bn, stat, co;
  char *fileName;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(
"Usage: %s [<option>...] <inimage> <outfile>\n\
        %s [<option>...] -o <outfile> <inimage>\n"));

  if (argc == 1) Usage(1, NULL);

  xstop  = read_iswitch(&argc, argv, "-xstop", 0);
  ystop  = read_iswitch(&argc, argv, "-ystop", 0);
  xstart = read_iswitch(&argc, argv, "-xstart", 1);
  xstart = MAX(xstart, 1);
  ystart = read_iswitch(&argc, argv, "-ystart", 1);
  ystart = MAX(ystart, 1);

  if (xstop) xsize = read_iswitch(&argc, argv, "-xsize", xstop-xstart+1);
  else       xsize = read_iswitch(&argc, argv, "-xsize", INT_MAX);
  if (ystop) ysize = read_iswitch(&argc, argv, "-ysize", ystop-ystart+1);
  else       ysize = read_iswitch(&argc, argv, "-ysize", INT_MAX);

  if (xsize LT 1) Error(2, "Illegal value for xsize.\n");
  if (ysize LT 1) Error(2, "Illegal value for ysize.\n");

  fileName = read_switch(&argc, argv, "-o", TRUE, NULL);
  co       = read_bswitch(&argc, argv, "-co");

  if (!fileName && argc != 3) Usage(1, "Illegal number of arguments.\n");
  if (fileName  && argc != 2) Usage(1, "Illegal number of arguments.\n");
  if (!fileName) fileName = argv[2];

  i = Iread_image(argv[1]);
  for (bn=1; bn LE Inbands(i); bn++) {
    if (xstart GT Ixsize(i[bn])) continue;
    if (ystart GT Iysize(i[bn])) continue;
    bxsize = Ixsize(i[bn])-xstart+1;
    bxsize = MIN(xsize, bxsize);
    bysize = Iysize(i[bn])-ystart+1;
    bysize = MIN(ysize, bysize);
#ifdef TEST
    fprintf(stderr,
      "xsta:%D, ysta:%d, xsi:%d, ysi:%d, xsto:%d, ysto:%d, bx:%d, by:%d\n",
      xstart, ystart, xsize, ysize, xstop, ystop, bxsize, bysize);
#endif
    b = Imake_subband(i[bn], xstart, ystart, bxsize, bysize);
    if (bn EQ 1) stat = biff2ascii(b, fileName, "w", co);
    else stat = biff2ascii(b, fileName, "a", co);
  }

  return(0);
}
