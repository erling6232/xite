

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
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/blab.h>
#include <xite/utils.h>
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:biff2ascii*

________________________________________________________________

		biff2ascii
________________________________________________________________

Name:		biff2ascii - write a biff band to an ascii data file

Syntax:         | #include <xite/convert.h>
                |
                | int biff2ascii( IBAND band, char* filename,
                |    char* type, int co );

Description:    'biff2ascii' dumps the contents of 'band' as ascii
		data to the file 'filename'. Give filename = "", '-' or
		NULL for writing to stdout. 'mode' may be one of:

		&w
		Truncate or create for writing
		&"a"
		Append; open for writing at end of file, or create for writing

		-&If 'co' is TRUE, a coordinate system is also written.

Return value:   | 0 - ok
		| 1 - bad pixeltype in 'band'
		| 2 - could not open file
		| 3 - bad 'type' argument, should be "w" or "a"
		| 4 - Error in writing file. File-system full?

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Revised:        Otto Milvang: Add all pixeltypes

Id:		$Id$
________________________________________________________________

*/


int biff2ascii(IBAND band, char *filename, char *type, int co)
{
  FILE *f;
  char f1[80], f2[80], int_format[7];
  int x, y, xsize, ysize, xoff, yoff, pixtyp;
  static int slen[] = { 0, 0 ,0 , 4, 4, 7, 7, 11, 11, 24, 13, 28,};
 
  ISB_BAND sband    = (ISB_BAND) band;
  IUS_BAND us_band  = (IUS_BAND) band;
  ISS_BAND ss_band  = (ISS_BAND) band;
  II_BAND ii_band   = (II_BAND) band;
  IR_BAND ir_band   = (IR_BAND) band;
  IC_BAND ic_band   = (IC_BAND) band;
  ID_BAND id_band   = (ID_BAND) band;
  IDC_BAND idc_band = (IDC_BAND) band;
  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  pixtyp = Ipixtyp(band);
  Info(0, "biff2ascii: Dump %d x %d pixels of type %s\n", xsize, ysize,
       Ipixname(pixtyp));
  if (pixtyp LT Iu_byte_typ) return(Error(1, "biff2ascii: Bad pixtype\n"));
  if (strcmp(type, "w") AND strcmp(type, "a")) 
      return(Error(3, "biff2ascii: type must be \"a\" or \"w\"\n")); 
  if (!filename || strlen(filename) == 0 || !strcmp(filename, "-")) f = stdout;
  else f = fopen(tilde_expand(filename), type);
  if (!f) return(Error(2, "biff2ascii: Couldn't open file %s\n", filename));

  sprintf(int_format, " %%%2d%s", 10, XITE_INTEGER_FORMAT);

  xoff = Ixstart(band)-1;
  yoff = Iystart(band)-1;
  strcpy(f1, "----------------------------------------------");
  f1[slen[pixtyp]] = 0;
  sprintf(f2, " %c%dd", '%', slen[pixtyp]-1);

  if (co) {
    if (fprintf(f, "      |") < 0) return(Error(4, "File-system full?\n"));

    for (x=1; x LE xsize; x++)
      if (fprintf(f, f2,x+xoff) < 0) return(Error(4, "File-system full?\n"));

    if (fprintf(f, "\n-------") < 0) return(Error(4, "File-system full?\n"));

    for (x=1; x LE xsize; x++)
      if (fprintf(f, f1) < 0) return(Error(4, "File-system full?\n"));

    if (fprintf(f, "--> X") < 0) return(Error(4, "File-system full?\n"));
  }

  for (y=1; y LE ysize; y++) {
    if (co) {
      if (fprintf(f, "\n %4d |", y+yoff) < 0)
	return(Error(4, "File-system full?\n"));
    }
    else if (fprintf(f, "\n") < 0) return(Error(4, "File-system full?\n"));
#define WLINE(fmt, band) for (x=1; x LE xsize; x++) \
  if (fprintf(f, fmt, band[y][x]) < 0) return(Error(4, "File-system full?\n"));
#define ZLINE(fmt, band) for (x=1; x LE xsize; x++)\
  if (fprintf(f, fmt, band[y][x].re, band[y][x].im) < 0) \
    return(Error(4, "File-system full?\n"));

    switch (pixtyp)
      {
	case Iu_byte_typ: WLINE(" %3d", band); break;
	case Is_byte_typ: WLINE(" %3d", sband); break;
	case Iu_short_typ: WLINE(" %6d",us_band); break;
	case Is_short_typ: WLINE(" %6d", ss_band); break;
	case Integer_typ: WLINE(int_format, ii_band); break;
	case Ireal_typ: WLINE(" %10g", ir_band); break;
	case Icomplex_typ: ZLINE(" (%10g,%10g)", ic_band); break;
	case Idouble_typ: WLINE(" %12g", id_band); break;
	case Id_complex_typ: ZLINE(" (%12g,%12g)", idc_band); break;
      }
#undef WLINE
#undef ZLINE
  }

  if (co)
    if (fprintf(f, "\n      |\n      v\n      Y\n") < 0)
      return(Error(4, "File-system full?\n"));

  if (fprintf(f, "\n") < 0) return(Error(4, "File-system full?\n"));
  if (f != stdout) fclose(f);
  return(0);
}
