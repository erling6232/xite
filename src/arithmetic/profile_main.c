
/*

________________________________________________________________

        profile
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



#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include <xite/arithmetic.h>
#include <xite/biff.h>
#include <xite/readarg.h>
#include <xite/message.h>



/*P:profile*

________________________________________________________________

                profile
________________________________________________________________

Name:           profile - Calculate pixel values along a line

Syntax:         | profile [-o outfile] [-x <x> -y <y>] [-d dir]
                |         [-ip <val>] [-ex <x> -ey <y>] [-s <step>] <image>

Description:    profile calculates the pixel values along a line starting
                at '(x,y)' in the direction 'dir' (in radians).
		
		An alternative is to specify the startpoint '(x,y)' and the
		endpoint '(ex,ey)' of the profile.

		The profile has one sample per 'step' pixel, and an
		interpolation of degree 'ip' is used (only degree 1 is
		implemented).

		The result is written to stdout or to a file specified
		by the -o switch.

See also:       crossSection(1)

Author:         Otto Milvang

Examples:       profile -d 2.5 t.img | perl -nae '' $i++; print "$i $F[0]\n"; '' | xgraph 
Id:             $Id$
________________________________________________________________

*/

int main(int argc, char *argv[])
{
  double x, y, d, ex, ey, dist, step;
  IMAGE img;
  IBAND band;
  ID_BAND dband;
  int i, nbands, b, xsize, ysize, len, max;
  char *filename;
  FILE *f;  

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage>\n\
       where <option> is \n\
       -o outfile : File to which the result is written (default: stdout)\n\
       -x x       : Horizontal coordinate of start-point of line\n\
       -y y       : Vertical coordinate of start-point of line\n\
       -d dir     : Direction of line (in radians)\n\
       -ex x      : Horizontal coordinate of end-point of line\n\
       -ey y      : Vertical coordinate of end-point of line\n\
       -s step    : Distance between two points in the profile\n"));
  
  Iset_message(1);
  Iset_abort(1);
  
  x = read_dswitch(&argc, argv, "-x", 0.0);
  y = read_dswitch(&argc, argv, "-y", 0.0);
  ex = read_dswitch(&argc, argv, "-ex", 0.0);
  ey = read_dswitch(&argc, argv, "-ey", 0.0);
  d = read_dswitch(&argc, argv, "-d", 0.0);
  step = read_dswitch(&argc, argv, "-s", 0.5);
  filename = read_switch(&argc, argv, "-o", 1, NULL);

  if (argc == 1) Usage(1, NULL);

  img = Iread_image(argv[1]);
  nbands = Inbands(img);
  band = img[1];
  xsize = Ixsize(band);  
  ysize = Iysize(band);  
  max = sqrt(xsize*xsize + ysize*ysize)/step + 100;
  dband = (ID_BAND) Imake_band(Idouble_typ, max, Inbands(img));

  if (x == 0.0) x = xsize/2;
  if (y == 0.0) y = ysize/2;
  if (ex > 0 || ey > 0) {
    dist = sqrt((x-ex)*(x-ex)+(y-ey)*(y-ey));
    d = atan2(ey-y, ex-x);
  } else dist = 0;
  for (b=1; b<=nbands; b++)
    len = profile(img[b], x, y, d, step, dist, 1, dband[b], max);

  if (filename) f = fopen(filename, "w"); else f = stdout;
  for(i=1; i<= len; i++) {
    for (b=1; b<=nbands; b++)
      fprintf(f, "% 8.3f", dband[b][i]);
    fprintf(f, "\n");
  }
  if (filename) fclose(f);

  return(0);
}
