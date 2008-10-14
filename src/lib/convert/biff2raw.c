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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include XITE_UNISTD_H
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#include <xite/blab.h>
#include <xite/utils.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



#ifndef MAIN

/*F:biff2raw*

________________________________________________________________

		biff2raw
________________________________________________________________

Name:		biff2raw - write a biff band to a raw data file

Syntax:         | #include <xite/convert.h>
		|
                | int biff2raw( IBAND band, char* filename,
                |    char* type, int pgm );

Description:    'biff2raw' dumps the contents of 'band' as raw, binary
		data to the file 'filename'. Give filename = "", '-' or NULL
		for writing to stdout. The band is written in samples-lines
		order. 'type' may be one of:

		&w
		Truncate or create for writing.

		&a
		Append; open for writing at end of file, or create for writing.

		-&
		If 'pgm' is true, a pgm header is written before the data.

		Accepts tilde in file name. Accepts all pixel types.

Restrictions:   Writing ('biff2raw') and reading ('raw2biff') on hosts
		with different byte order will only work for byte bands.

Return value:   | 0 - ok
		| 1 - bad 'type' argument, should be "w" or "a"
		| 2 - could not open file
		| 3 - could not write the whole band

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:		$Id$ $Revision$
________________________________________________________________

*/

int biff2raw(IBAND band, char *filename, char *type, int pgm)
{
  FILE *f;
  int y, xsize, ysize, pt, bytesPrPix, fd, linelen;
  char *ptr;
  char buf[100];

  pt = Ipixtyp(band);
  if (strcmp(type, "w") AND strcmp(type, "a"))
    return(Error(1,"biff2raw: Bad 'type' argument\n")); 
  if (!filename || strlen(filename) == 0 || !strcmp(filename, "-")) f = stdout;
  else f = fopen(tilde_expand(filename), type);
  if (!f) return(Error(2, "biff2raw: Could not open file %s\n", filename));
  fd = fileno(f);

  xsize      = Ixsize(band);
  ysize      = Iysize(band);
  bytesPrPix = Ipixsize(pt)/8;
  linelen    = xsize * bytesPrPix;
  if (pgm && Ipixtyp(band) != Iu_byte_typ) 
      Error(2, "%s\n",
	   "pgm is only implemented for bands with pixel type unsigned byte.");
  if (pgm) {
    sprintf(buf, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    write(fd, buf, strlen(buf));
  }
  for (y=1; y LE ysize; y++) {
    ptr = (char*)((size_t)(&(band[y][0])) + bytesPrPix);
    if (write(fd, ptr, linelen) NE linelen)
      return(Error(3, "biff2raw: Could not write the whole band\n"));
  }
  if (f != stdout) fclose(f);

  return(0);
}

#endif /* not MAIN */

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

#ifdef MAIN

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

#endif /* MAIN */
