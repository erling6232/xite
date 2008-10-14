
/*

________________________________________________________________

        biffDump
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
#include <xite/copy.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/readarg.h>
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

/*P:biffDump*

________________________________________________________________

		biffDump
________________________________________________________________

Name:		biffDump - dump a part of an image to stdout or file

Note:           This program is replaced by 'biff2raw' and 'biff2ascii',
                these should be used in stead. biffDump will be removed
		some day.

Syntax:		| biffDump [-o <file>] [-c] [-b]
                |    <image> <xstart> <ystart> <xsize> <ysize>

Description:    Writes all pixel values in the specified window to
                stdout or specified <file>. Can only dump a part of
		first band, specify "filename:n" as input image to
		dump a part of band n.

		Switches:

                | -o <file> : send dump to <file>
		| -c        : include a coordinate system in the dump
		| -b        : dump bytes binary (default: formatted) 

		Arguments:

                | image - input image to be dumped
		| xstart - horizontal start of window to be dumed
		| ystart - vertical start of window to be dumped
		| xsize - horizontal size of window to be dumped
		| ysize - vertical size of window to be dumped

Files:	

See also:	biff2raw(1), biff2ascii(1)

Diagnostics:

Restrictions:   Assuming BIFF file with UNS_BYTE pixels.  

Author:		Tor Loennestad, BLAB, ifi, UiO

Id:             $Id$

Examples:	| biffDump mona.img -c 10 45 20 70
                |    # (suitable for human reading)
		| biffDump mona.img -o mona -b 10 45 20 70
		|    # (suitable for computer reading) 
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i;
  int x, y, xstart, ystart, xsize, ysize, xstop, ystop, c, b;
  char *fileName;
  FILE *f;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s: You should rather use biff2raw or biff2ascii!\n"));

  Usage(2, NULL);

  c = (read_switch(&argc, argv, "-c", FALSE, 0) NE 0);
  b = (read_switch(&argc, argv, "-b", FALSE, 0) NE 0);
  fileName = read_switch(&argc, argv, "-o", TRUE, 0);
  IF (argc LT 6)
    fprintf(stderr,"Usage: %s [-o <file>] [-c] [-b]\n", argv[0]);
    fprintf(stderr,"    <image> <xstart> <ystart> <xsize> <ysize>\n");
    exit(1);
  ENDIF;

  if ((size_t)fileName) f = fopen(fileName, "w");
  else f = stdout;

  i = Iread_image(argv[1]);
  xstart = MAX(atoi(argv[2]),1);
  ystart = MAX(atoi(argv[3]),1);
  xsize = atoi(argv[4]);
  ysize = atoi(argv[5]);
  xstop = MIN(xstart+xsize-1,Ixsize(i[1]));
  ystop = MIN(ystart+ysize-1,Iysize(i[1]));

  IF (Ipixtyp(i[1]) NE Iu_byte_typ)
    fprintf(stderr, "Can only handle unsigned byte pixels.\n"); 
    exit(1);
  ENDIF;

  IF (c)
    fprintf(f, "     |");
    FOR (x=xstart; x LE xstop; INC x) fprintf(f, " %3d",x); ENDFOR;
    fprintf(f, "\n------");
    FOR (x=xstart; x LE xstop; INC x) fprintf(f, "----"); ENDFOR;
    fprintf(f, "--> X");
  ENDIF
  FOR (y=ystart; y LE ystop; INC y)
    if (c) fprintf(f, "\n %3d |",y);
    else if (NOT b) fprintf(f, "\n");
    if (b)
      FOR (x=xstart; x LE xstop; INC x) fprintf(f, "%c",i[1][y][x]); ENDFOR
    else
      FOR (x=xstart; x LE xstop; INC x) fprintf(f, " %3d",i[1][y][x]); ENDFOR;
  ENDFOR;
  if (c) fprintf(f, "\n     |\n     v\n     Y\n");
  if (NOT b) fprintf(f, "\n");
  if (f NE stdout) fclose(f);
   return(0);
}
