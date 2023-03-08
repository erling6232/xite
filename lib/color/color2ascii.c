

/*

________________________________________________________________

        color2ascii
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
#include <stdlib.h>
#include <xite/color.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/readarg.h>
#include <xite/message.h>


#ifndef TRUE
# define TRUE 1
#endif
#ifndef FALSE
# define FALSE 0
#endif



/*F:color2ascii*

________________________________________________________________

		color2ascii
________________________________________________________________

Name:		color2ascii - write a biff band to an ascii data file

Syntax:         | #include <xite/color.h>
                |
                | int color2ascii( Color_cell* coltab, int len,
                |    char* filename, char* type, int hex );

Description:    'color2ascii' dumps the contents of 'coltab' with length
                'len' as ascii data to the file 'filename'. The format can
		be read by 'mct(1)'. Give filename "-" or "-1" for writing
		to stdout.

		For information on the output format, see 'PrintColortable(3)'.

Parameters:     &len
                This is the length of the colortable, i.e. the number of
		entries to be printed.

		&hex
		If nonzero, print pixelvalue as hexadecimal numbers, instead
		of decimal numbers.

		&type
		This may be one of the following
		&&w
		Truncate or create file for writing
		&&a
		Append. Open for writing at end of file, or create for writing.

Return value:   | 0 - ok
		| 1 - could not open file
		| 2 - bad 'type' argument, should be "w" or "a"

See also:       color2ascii(1), mct(1), PrintColortable(3), colorquant(1),
                makepseudo(1)

Author:		Svein Bøe

Id:		$Id$
________________________________________________________________

*/


int color2ascii(Color_cell *coltab, int len, char *filename, char *type, int hex)
{
  FILE *f;

  if (strcmp(type, "w") && strcmp(type, "a")) 
      return(Error(2, "color2ascii: type must be \"a\" or \"w\"\n"));

  if (filename == NULL || !strcmp(filename, "-") || !strcmp(filename, "-1"))
    f = stdout;
  else f = fopen(filename, type);
  if (! f)
    return(Error(1, "color2ascii: Couldn't open file %s\n", filename));

  PrintColortable(coltab, len, hex, f);

  if (f != stdout) fclose(f);
  return(0);
}
