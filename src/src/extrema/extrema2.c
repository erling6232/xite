

/*C*

________________________________________________________________

        extrema2
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

/*F:extrema2*

________________________________________________________________

		extrema2
________________________________________________________________

Name:		extrema2 - detection of local minima and maxima

Syntax:         | #include <xite/extrema.h>
		|
                | int extrema2( IBAND inband, IBAND outband,
                |    int min, int max, int bg );
Description:    Local extrema (minima and maxima) are detected in
                'inband', and marked in 'outband'. Minima are
		marked with value 'min', maxima are marked 'max',
		and all other pixels are marked 'bg', background.
		If you do not want to distinguish between minima
		and maxima, see extrema1.

Restrictions:   Supports only unsigned byte.

See also:	extrema2(1), extrema1(3), minima(3), maxima(3)

Return value:   | 0 => ok
                | 1 => bad input pixel type
		| 2 => bad output pixel type
		| 3 => bad min, max or bg value

Reference:      B.B. Chaudhuri and B.Uma Shankar:
                "An efficient algorithm for extrema detection in
		digital images"
		Pattern Recognition Letters 10 (1989), 81-85.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

static IBAND sib, sob;
static int spv, sfv, sxsize, sysize;

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#define COMPARE1(xx,yy) \
  if (inband[yy][xx] > pv) larger = TRUE; \
  else if (inband[yy][xx] < pv) smaller = TRUE;

#define COMPARE2(xx,yy) \
  if (sob[yy][xx] != sfv) if (sib[yy][xx] == spv) fill(xx,yy)

#ifndef FUNCPROTO
static void fill(x, y)
int x, y;
#else /* FUNCPROTO */
static void fill(int x, int y)
#endif /* FUNCPROTO */
{
  /*printf(" %d %d \n", x, y); */
  sob[y][x] = sfv;
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
static void collapse(x, y, fillval)
int x, y, fillval;
#else /* FUNCPROTO */
static void collapse(int x, int y, int fillval)
#endif /* FUNCPROTO */
{  
  spv = sib[y][x];
  sfv = fillval;
  fill(x, y);
}

#ifndef FUNCPROTO
int extrema2(inband, outband, min, max, bg)
IBAND inband, outband;
int min, max, bg;
#else /* FUNCPROTO */
int extrema2(IBAND inband, IBAND outband, int min, int max, int bg)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, pv, tempmin;
  int smaller, larger;

  if (Ipixtyp(inband) != Iu_byte_typ) return(Error(1, "Bad input pixel type\n"));
  if (Ipixtyp(outband) != Iu_byte_typ) return(Error(2, "Bad output pixel type\n"));
  if ((min < 0) OR (min > 255) OR (max < 0) OR (max > 255) OR (bg < 0) 
    OR (bg > 255) OR (max == bg) OR (max == min) OR (min == bg))
    return(Error(3, "Bad min, max or bg value\n"));

  sxsize = xsize = MIN(Ixsize(inband), Ixsize(outband));
  sysize = ysize = MIN(Iysize(inband), Iysize(outband));
  sib = inband;
  sob = outband;

  tempmin=0;
  while ((tempmin==min) OR (tempmin==max) OR (tempmin==bg)) tempmin++;

  /* let all pixels be candidate pixels */
  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++)
      outband[y][x] = max;

  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++) {
      if (outband[y][x] == bg) continue;
      smaller = larger = FALSE;
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

      if (!larger) ;
      else if (!smaller) outband[y][x] = tempmin;
      else collapse(x, y, bg);
  }

  /* correct eventual misclassifications */
  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++)
      if (outband[y][x] == tempmin)
        collapse(x, y, min);

  return(0);
}

#endif /* not MAIN */

/*P:extrema2*

________________________________________________________________

		extrema2
________________________________________________________________

Name:		extrema2 - detection of local minima and maxima

Syntax:		extrema2 [-min <min>] [-max <max>] [-bg <bg>] 
                         <inimage> <outimage>

Description:	Local extrema (minima and maxima) are detected in
                'inband', and marked in 'outband'. Minima are
		marked with value 'min', maxima are marked 'max',
		and all other pixels are marked 'bg', background.
		If you do not want to distinguish between minima
		and maxima, see extrema1.

		The default for 'min' is 0, the 'max' default is 255,
		the 'bg' default is 128.

See also:	extrema2(3), extrema1(1), minima(1), maxima(1)

Restrictions:	Supports ony unsigned byte images. min, max and bg 
                should not be equal, and they should be in 0..255.

Reference:      B.B. Chaudhuri and B.Uma Shankar:
                "An efficient algorithm for extrema detection in
		digital images"
		Pattern Recognition Letters 10 (1989), 81-85.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| extrema2 -min 0 -max 1 -bg 2 mona.img monaextrem.img

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
  int min, max, bg, bn, nbands;
  char arg[50];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-min <min>] [-max <max>] [-bg <bg>] <inimage> <outimage>\n"));

  min = read_iswitch(&argc, argv, "-min", 0);
  max = read_iswitch(&argc, argv, "-max", 255);
  bg  = read_iswitch(&argc, argv, "-bg",  128);

  if (argc == 1) Usage(0, (char*)0);
  if (argc != 3) Usage(1, "bad number of arguments\n");

  inimage = Iread_image(argv[1]);
  if (!inimage) exit(Error(2, "couldn't open file %s\n", argv[1]));
  nbands = Inbands(inimage);
  outimage = Icopy_init(inimage);
  if (!outimage) exit(3);

  for (bn=1; bn<=nbands; bn++)
    if (extrema2(inimage[bn], outimage[bn], min, max, bg)) exit(3);

  sprintf(arg, " min = %d, max = %d, bg = %d", min, max, bg);
  Ihistory(outimage, argv[0], arg);
  Iwrite_image(outimage, argv[2]);

  return(0);
}

#endif /* MAIN */
