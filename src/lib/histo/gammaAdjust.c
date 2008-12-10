

/*

________________________________________________________________

        gammaAdjust
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include XITE_STDIO_H
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>

static void logaritmic_color(int *col, double x)
{
  int i;
  double a, b;
  a = (x) / 0.400;
  a = a*a;
  b = 255.0 / log(a*255.0+1.0);
  for (i = 0; i<256; i++) 
    {
      col[i] = 256.0/a * log(1.0 + (exp(a) - 1.0) * (double) i / 256.0);
    }
}

static void exp_color(int *col, double x)
{
  int i;
  double a, b;
  a = (x) / 0.400;
  a = a*a;
  b = 255.0 / log(a*255.0+1.0);
  for (i = 0; i<256; i++) 
    {
      col[255-i] = 256.0/a * log(1.0 + (exp(a) - 1.0) * (double) i / 256.0);
      col[255-i] = 255-col[255-i];
    }
}





/*F:gammaAdjust*

________________________________________________________________

                gammaAdjust
________________________________________________________________

Name:           gammaAdjust - Simple gamma correction
Syntax:         | #include <xite/histo.h>
		|
                | BiffStatus gammaAdjust( IBAND inband,
                |    IBAND outband, double adjust );
Description:    A simple gamma corretion to make images
                darker or brigter. The 'adjust' parameter must be 
		in the range -1.0 (much darker) to 1 (much brighter).
		If adjust = 0.0 the band is not changed.

		| If adjust > 0:
		| a= adjust**2/0.16;
		| b= 255.0 / log(a*255.0+1.0);
		| c= 256.0/a * log(1.0 + (exp(a) - 1.0) * i / 256.0);
		| col[i] = c;
		|
		| If adjust < 0;
		| a= adjust**2/0.16;
		| b= 255.0 / log(a*255.0+1.0);
		| c= 256.0/a * log(1.0 + (exp(a) - 1.0) * (255-i) / 256.0);
		| col[i] = 255-c;

Return value:   | 0 - OK
                | 2 - Bad pixtype
		| 3 - Bad size
		| 4 - Illegal value for adjust.
Restrictions:   Works on unsigned byte bands only.
Author:         Otto Milvang
________________________________________________________________

*/


BiffStatus gammaAdjust(IBAND inband, IBAND outband, double adjust)
{
  int xsize, ysize, x, y;
  int col[256];
  
  if (adjust < -1.0 || adjust > 1.0)
    return(Error(4, "gammaAdjust: Bad pixeltype, must be Unsigned byte\n"));
  if (Ipixtyp(inband) !=  Iu_byte_typ || Ipixtyp(outband) !=  Iu_byte_typ)
    return(Error(2, "gammaAdjust: Bad pixeltype, must be Unsigned byte\n"));
  if (adjust < 0.01 && adjust > -0.01) adjust = 0.01;
  if (adjust > 1.0) adjust = 1.0;
  if (adjust < -1.0) adjust = -1.0;
  if (adjust > 0) logaritmic_color(col, adjust); else exp_color(col, adjust);

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  if (xsize != Ixsize(outband) || ysize != Iysize(outband))
   return(Error(3, "gammaAdjust: Inband and outband must be of equal size\n"));

  for(y=1; y<=ysize; y++)
    for(x=1; x<=xsize; x++)
      outband[y][x] = col[inband[y][x]];

  return(Iok);
}
