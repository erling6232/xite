

/*C*

________________________________________________________________

        maxima
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/extrema.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>



#ifndef MAIN

/*F:maxima*

________________________________________________________________

		maxima
________________________________________________________________

Name:		maxima - detection of local maxima

Syntax:         | #include <xite/extrema.h>
		|
                | int maxima( IBAND inband, IBAND outband,
                |    int fg, int bg );
Description:    Local maxima are detected in 'inband', and marked
                in 'outband' with value 'fg', foreground. All
		other pixels are marked 'bg', background.

Restrictions:   Supports only unsigned byte.

See also:	maxima(1), extrema1(3), extrema2(3), minima(3)

Return value:   | 0 => ok
                | 1 => bad input pixel type
		| 2 => bad output pixel type
		| 3 => bad fg or bg value

Reference:      B.B. Chaudhuri and B.Uma Shankar:
                "An efficient algorithm for extrema detection in
		digital images"
		Pattern Recognition Letters 10 (1989), 81-85.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

static IBAND sib, sob;
static int spv, sbg, sxsize, sysize;

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#define COMPARE1(xx,yy) \
  if (inband[yy][xx] > pv) larger = TRUE;

#define COMPARE2(xx,yy) \
  if (sob[yy][xx] != sbg) if (sib[yy][xx] == spv) fill(xx,yy)

#ifndef FUNCPROTO
static void fill(x, y)
int x, y;
#else /* FUNCPROTO */
static void fill(int x, int y)
#endif /* FUNCPROTO */
{
/*printf(" %d %d \n", x, y); */
  sob[y][x] = sbg;
  if (x>1) {
    COMPARE2(x-1,y);
    if (y>1) COMPARE2(x-1,y-1);
    if (y<sysize)COMPARE2(x-1, y+1);
  }
  if (x<sxsize) {
    COMPARE2(x+1, y);
    if (y>1) COMPARE2(x+1,y-1);
    if (y<sysize) COMPARE2(x+1, y+1);
  }
  if (y>1) COMPARE2(x, y-1);
  if (y<sysize) COMPARE2(x, y+1);
}

#ifndef FUNCPROTO
static void collapse(ib, ob, x, y, bg)
IBAND ib, ob;
int x, y, bg;
#else /* FUNCPROTO */
static void collapse(IBAND ib, IBAND ob, int x, int y, int bg)
#endif /* FUNCPROTO */
{  
  sib = ib;
  sob = ob;
  spv = ib[y][x];
  sbg = bg;

  fill(x, y);
}

#ifndef FUNCPROTO
int maxima(inband, outband, fg, bg)
IBAND inband, outband;
int fg, bg;
#else /* FUNCPROTO */
int maxima(IBAND inband, IBAND outband, int fg, int bg)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, pv;
  int larger;

  if (Ipixtyp(inband) != Iu_byte_typ) return(Error(1, "Bad input pixel type\n"));
  if (Ipixtyp(outband) != Iu_byte_typ) return(Error(2, "Bad output pixel type\n"));
  if ((fg < 0) OR (fg > 255) OR (bg < 0) OR (bg > 255) OR (fg == bg))
    return(Error(3, "Bad fg or bg value\n"));

  sxsize = xsize = MIN(Ixsize(inband), Ixsize(outband));
  sysize = ysize = MIN(Iysize(inband), Iysize(outband));

  /* let all pixels be cap (candidate pixels) */
  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++)
      outband[y][x] = fg;

  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++) {
      if (outband[y][x] == bg) continue;
      larger = FALSE;
      pv = inband[y][x];
      if (x>1) {
        COMPARE1(x-1,y);
        if (y>1) COMPARE1(x-1,y-1);
        if (y<ysize) COMPARE1(x-1, y+1);
      }
      if (x<xsize) {
        COMPARE1(x+1, y);
        if (y>1) COMPARE1(x+1,y-1);
        if (y<ysize) COMPARE1(x+1, y+1);
      }
      if (y>1) COMPARE1(x, y-1);
      if (y<ysize) COMPARE1(x, y+1);

      if (larger) collapse(inband, outband, x, y, bg);
  }

  return(0);
}

#endif /* not MAIN */

/*P:maxima*

________________________________________________________________

		maxima
________________________________________________________________

Name:		maxima - detection of local maxima

Syntax:		maxima [-bg <bg>] [-fg <fg>] <inimage> <outimage>

Description:	Local maxima are detected in 'inimage', and marked
		in 'outimage' with value 'fg', foreground. All other
		pixels are marked 'bg', background.

		The default for 'fg' is 255, the 'bg' default is 0.
Files:	

See also:	maxima(3), extrema1(1), extrema2(1), minima(1)

Diagnostics:

Restrictions:	Supports ony unsigned byte images. bg and fg should
                not be equal, and they should be in 0..255.

Return value:

Reference:      B.B. Chaudhuri and B.Uma Shankar:
                "An efficient algorithm for extrema detection in
		digital images"
		Pattern Recognition Letters 10 (1989), 81-85.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| maxima -fg 0 -bg 1 mona.img monamax.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE inimage, outimage;
  int bg, fg, bn, nbands;
  char arg[50];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-bg <bg>] [-fg <fg>] <inimage> <outimage>\n"));

  bg = read_iswitch(&argc, argv, "-bg", 0);
  fg = read_iswitch(&argc, argv, "-fg", 255);

  if (argc == 1) Usage(0, (char*)0);
  if (argc != 3) Usage(1, "bad number of arguments\n");

  inimage = Iread_image(argv[1]);
  if (!inimage) exit(Error(2, "couldn't open file %s\n", argv[1]));
  nbands = Inbands(inimage);
  outimage = Icopy_init(inimage);
  if (!outimage) exit(3);

  for (bn=1; bn<=nbands; bn++)
    if (maxima(inimage[bn], outimage[bn], fg, bg)) exit(3);

  sprintf(arg, " fg = %d, bg = %d", fg, bg);
  Ihistory(outimage, argv[0], arg);
  Iwrite_image(outimage, argv[2]);

  return(0);
}

#endif /* MAIN */
