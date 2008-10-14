

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
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#include <xite/readarg.h>
#include <xite/message.h>


#ifndef TRUE
# define TRUE 1
#endif
#ifndef FALSE
# define FALSE 0
#endif



/*P:color2ascii*

________________________________________________________________

		color2ascii
________________________________________________________________

Name:		color2ascii - dump a colortable in ascii format

Syntax:		| color2ascii [<option>...] <image-file> <ascii-file>

Description:    Writes the colortable contents to 'ascii-file', in ascii
                format. Use "-" or "-1" for output to stdout.

		The ascii output can be given as input to 'mct(1)'.
		For details on the output format, refer to
		'PrintColortable(3)'.

		On a visual with composite colormaps (DirectColor or
		TrueColor visual), one part of the pixelvalue is used as an
		index into the colormap which controls the amount of red, one
		part is used as an index into the green colormap and one part
		for the blue colormap.

		On a visual with non-composite colormap (e.g. PseudoColor
		visual), only one of these parts is used as an index into the
		single colormap.

		When the contents of a colortable stored in a file in BIFF
		format is printed, only the PseudoColor part of
		the pixelvalue is shown because the file only stores this
		part of the pixel-value.

Options:        &-hex
		Print pixel value as hexademical number (default decimal)
		
See also:       color2ascii(3), mct(1), PrintColortable(3), colorquant(1),
                makepseudo(1)
Author:		Svein Bøe, BLAB, Ifi, UiO
                
Id:             $Id$

Examples:	| color2ascii -o mona.txt mona.col
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int len, hex, status;
  char *tabName;
  Color_tab coltab;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <colortable-file> <ascii-file> \n\
       where <option> is chosen from\n\
         -hex             : Print pixelvalues as hexadecimal numbers\n"));

  if (argc == 1) Usage(1, NULL);

  hex = read_bswitch(&argc, argv, "-hex");

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  if ((status = color_read(argv[1], coltab, &tabName, &len)) == Icolpixerr) {
    Error(2, "File is not colortable.\n");
  } else if (status != Iok) {
    Error(2, "Could not read colortable file.\n");
  }

  color2ascii(coltab, len, argv[2], "w", hex);

  return(0);
}
