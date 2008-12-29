/*

________________________________________________________________

        biff2raw
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
#include XITE_STDIO_H
#include <stdlib.h>
#include XITE_UNISTD_H
#include XITE_STRING_H
#include XITE_TYPES_H
#include <xite/blab.h>
#include <xite/utils.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*P:biff2raw*

________________________________________________________________

		biff2raw
________________________________________________________________

Name:		biff2raw - write part of an image as raw, binary data

Syntax:		| biff2raw [<option>...] <inimage> <outfile>
                | biff2raw [<option>...] -o <outfile> <inimage>

Description:    Writes all pixel values in all bands of 'inimage' to
                'outfile'. Use '-' for 'outfile' to write to stdout.
		Accepts all pixel types. The image is written in
		samples-lines-bands order.

Options:        &-o file
		Send dump to 'file'. This option is retained for backwards
		compatibility.

		&-pgm
		Start with a pgm header. This option is obsolete. Use
		'biff2pnm' instead.

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

		-&
		If start, stop as well as size options are given, and
		inconsistently, the stop option is ignored. The given
		sizes are clipped against the band.

See also:	raw2biff(1)

Restrictions:   Writing ('biff2raw') and reading ('raw2biff') on hosts
		with different byte order will only work for byte images.
		Use option '-swap' for 'raw2biff' to change the byte order.
 
Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$

Examples:	| biff2raw -o mona.raw mona.img
		| biff2raw  -xstart 10 -ystart 45 -xstop 20 -ystop 70 \\
		|   mona.img
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i;
  IBAND b;
  int pgm,xstart, ystart, xsize, ysize, xstop, ystop, bxsize, bysize, bn, stat;
  char *fileName;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> [<outfile>]\n"));

  if (argc == 1) Usage(1, NULL);

  pgm    = read_bswitch(&argc, argv, "-pgm");
  if (pgm)
    Error(1, "Option -pgm is obsolete. Use program biff2pnm instead.\n");
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

  if (xsize LT 1) Error(3, "Bad x-size arguments\n");
  if (ysize LT 1) Error(4, "Bad y-size arguments\n");

  fileName = read_switch(&argc, argv, "-o", TRUE, NULL);

  if (!fileName && argc != 3) Usage(1, "Illegal number of arguments.\n");
  if (fileName  && argc != 2) Usage(1, "Illegal number of arguments.\n");
  if (!fileName) fileName = argv[2];

  i = Iread_image(argv[1]);
  if (Inbands(i) > 1 && pgm)
    Error(2, "-pgm can be used for 1-band images only.\n");

  for (bn=1; bn LE Inbands(i); bn++) {
    if (xstart GT Ixsize(i[bn])) continue;
    if (ystart GT Iysize(i[bn])) continue;
    bxsize = Ixsize(i[bn])-xstart+1;
    bxsize = MIN(xsize, bxsize);
    bysize = Iysize(i[bn])-ystart+1;
    bysize = MIN(ysize, bysize);

    if (Verbose())
      fprintf(stderr,
	"xsta:%d, ysta:%d, xsi:%d, ysi:%d, xsto:%d, ysto:%d, bx:%d, by:%d\n",
        xstart, ystart, xsize, ysize, xstop, ystop, bxsize, bysize);

    b = Imake_subband(i[bn], xstart, ystart, bxsize, bysize);
    if (bn EQ 1) stat = biff2raw(b, fileName, "w", pgm);
    else stat = biff2raw(b, fileName, "a", pgm);
    if (stat) Warning(5, " Error in band %d\n", bn);
  }
  return(0);
}
