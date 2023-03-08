

/*

________________________________________________________________

        mct.c
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


/*P:mct*

________________________________________________________________

		mct
________________________________________________________________

Name:		mct - Create a color table from ascii data.

Syntax:		| mct [-title <title>] [<input-file> [<output-file>]]

Description:    'mct' creates a color table from an ascii file 
                of color information. The resulting colortable may be
		used on 8-bit PseudoColor visuals as well as on 24-bit
		composite-colormap visuals (DirectColor and TrueColor).

		Each line in the file describes a color in the colortable. The
		line-format is

		| <colornumber> <red-val> <green-val> <blue-val>

		'colornumber' is an unsigned integer. It is used as an index to
		lookup 'red-val', 'green-val' and 'blue-val'.

		'red-val', 'green-val' and 'blue-val' are the intensities of
		the colors in the range 0 - 65535. A special notation
		| c1 r1 g1 b1
		| :
		| c2 r2 g2 b2
		defines a linear stretch in the range 'c1' to 'c2', i.e.
		the colornumbers in the range 'c1' to 'c2' will get red
		intensities in the range 'r1' to 'r2', green in the range 'g1'
		to 'g2' and blue in the range 'b1' to 'b2'. The ranges are
		linearly interpolated.

		Lines with a # in coloumn 1 are treated as comments.

Modification:   The format has been modified to allow comments, starting with
                a `#`,  at the end of the lines. This can be used e.g. to 
                specify the name of the color.

Author:		Otto Milvang, modified 24.07.92 Knut-Andreas Lie (allow
                comments).
Revised:        Svein Bøe

See also:       read_colormap(3), color2ascii(1), color2ascii(3),
                PrintColortable(3)

Examples:	| # xshow.map
                | # Generate colortable with "mct xshow.map xshow.col"
                | #
                | # My private colortab
                | # Pix       0 -> Red
                | # Pix 1 - 254 -> Grayscale
                | # Pix     255 -> Green
		| 0 65535 0 0
		| 1 0 0 0    
		| :
		| 254 65535 65535 65535 
		| 255 0 65535 0 
                or:
                | # xshow.map
                | # Generate colortable with "mct xshow.map xshow.col"
                | #
                | # My private colortab
		| 0 65535 0 0           # red
		| 1 0 0 0               # black  
		| :                     # -> 
		| 254 65535 65535 65535 # white
		| 255 0 65535 0         # green
		
Id: 		$Id$

________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/color.h>
#include <xite/utils.h>
#include XITE_STDIO_H
#include XITE_STRING_H

int main(int argc, char **argv)
{
  Color_cell ctab[4096];
  FILE *inp;
  char outFileName[50];
  char *title = "Colortab";
  int len;

  InitMessage(&argc, argv, xite_app_std_usage_text(
	      "Usage: %s [-t <title>] [<inputfile> [<outputfile>]]\n"));

  title = read_switch(&argc, argv, "-title", 1, title);
  title = read_switch(&argc, argv, "-t", 1, title);

  if (argc > 1) {
    if (strcmp(argv[1], "-") == 0 || strcmp(argv[1], "-0") == 0) {
      inp = stdin;
    } else {
      inp = fopen((char *) tilde_expand(argv[1]), "r");
      if (inp == NULL) Error(2, "Cannot open file %s.\n", argv[1]);
    }
    if (argc > 2) {
      if (strcmp(argv[1], "-") == 0 || strcmp(argv[1], "-1") == 0)
	(void) strcpy(outFileName, "-1");
      else (void) strcpy(outFileName, argv[2]);
    }
  } else Usage(1, NULL);

  read_colormap(inp, ctab, &len);

  Info(1, "Created color_table of %d colors\n", len);

  color_write(outFileName, ctab, title, len);

  return(0);
}
