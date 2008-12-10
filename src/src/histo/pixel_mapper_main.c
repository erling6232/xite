/*C*

________________________________________________________________

        pixel_mapper
        $Id$
	Copyright 1990, NCC (Norwegian Computing Center)
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_MALLOC_H

/*P:pixel_mapper*

________________________________________________________________

		pixel_mapper
________________________________________________________________

Name:		pixel_mapper - map pixel values into new value
Syntax:		| pixel_mapper <inimage> <outimage> [-b <b>] 
                |    <o1> [<o2> <o3> ...] <n1>

Description:    Maps pixelvalues 'o1' ... 'on' to value 'n1'.

Options:        &-b
                Process band number 'b' is processed ('b' defaults to all).

Restrictions:   Works only on unsigned byte images.

Author:		gutorm høgåsen, NCC

Developed by:   BILD, Norwegian Computing Center, Oslo, Norway

Examples:       pixel_mapper mona.biff mona.mask 2 4 6 8

See also:	combine(1), logical(1)

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  char *in_name, *out_name;
  IUB_IMAGE im8;
  IUB_BAND  band8;
  int xsize,  ysize, mapto, *mapfrom, nmaps, bandnr, bn;
  int x, y, i;
  histogram h;
  char message[511];
  
  Iset_message(TRUE);		/* warnings from blab-software */
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-b b] <biff-file> <resultfile> <o1> [<o2> ...] <n1>\n"));

  if (argc == 1) Usage(1, NULL);

  bandnr = atoi(read_switch(&argc, argv, "-b", 1, "0"));
  if (argc < 5) Usage(1, "Illegal number of arguments.\n");

  in_name  = argv[1];
  out_name = argv[2];

  nmaps = argc - 3;   /* nmaps will later be the number of values to map from*/
  mapfrom = (int *)malloc( sizeof(int)*nmaps );
  for (i=3; i < argc; i++)
    mapfrom[i-3] = atoi(argv[i]);
  mapto = mapfrom[--nmaps];     /* forget last value */
  for (i=0; i<=nmaps; i++)      /* test pixel values */
    if ((mapfrom[i]>255) || (mapfrom[i]<0))
      Error(2, "Invalid pixel value: %d.\n", mapfrom[i]);

  for (i=0; i<256; i++) h[i] = i;
  for (i=0; i<nmaps; i++) h[mapfrom[i]] = mapto;

  im8 = Iread_image(in_name);
  if ((bandnr < 0) || (bandnr > Inbands(im8)))
    Error(2, "Bad band number: %d in file '%s'\n", bandnr, in_name);

  if (bandnr) { im8[1] = im8[bandnr]; Iset_nbands(&im8, 1); }
  for (bn = 1; bn <= Inbands(im8); bn++)
  { band8 = im8[bn];  	/* pick correct band */
    if ( (Ipixtyp(band8) != Iu_byte_typ) )
      Error(2, "Invalid pixel type %ld for band %d\n",
	    Ipixtyp(band8), bn), exit (1);
    xsize = Ixsize(band8);
    ysize = Iysize(band8);
  
    for ( y = 1; y <= ysize; y++ )
      for ( x = 1; x <= xsize; x++ )
        band8[y][x] = h[band8[y][x]];
  }

  sprintf(message, ":\n    File %s: Merged values ", in_name);
  for (i = 0; i < nmaps; i++)
    sprintf(message,"%s%d ", message, mapfrom[i]);
  sprintf(message, "%sto %d\n", message, mapto);
  Ihistory(im8, argv[0], message);
  Iwrite_image(im8, out_name);
  return(0);

} /* main() */
