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
