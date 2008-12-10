

/*

________________________________________________________________

        affine.c
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
#include <xite/geometry.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/convert.h>
#include <xite/readarg.h>
#include <xite/message.h>



/*F:affine*

________________________________________________________________

		affine
________________________________________________________________

Name:		affine - affine geometric transform

Syntax:         | #include <xite/geometry.h>
		|
                | int affine( IBAND inband, IBAND outband,
                |    double x0, double x1, double x2, double y0,
                |    double y1, double y2, int ip, int bg );

Description:	affine performs a linear transform of the coordinate
                system (x, y) to another coordinate system (u,v)
		All coordinates are adjusted acording to (xstart, ystart).
		| outband(x,y) = inband(u,v)
                | u = x0 + x1 * x + x2 * y
		| v = y0 + y1 * x + y2 * y
		The interpolation method 'ip' is
		|  0 : Nearest neighbor 
		|  1 : Bilinear   interpolation
		|      g(x) = bi(f(x), f(x+1), d)
		|      bi(x, y, d) = x(1-d) + yd
		|  3 ; Cubic      interpolation
		|      g(x) = cc(f(x-1), f(x), f(x+1), f(x+2), d)
		|      cc(x, y, z, v, d) = y + d((-x+z) +
		|             d((2x-2y+z-v) + d(-x+y-z+v)))

		bg gives the background pixel value.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Return value:   | 0 : Ok
                | 1 : Bad input or output pixel type

Reference:	Wayne Niblack. Digital Image Processing, p 131-150

See also:       affine(1), quadratic(3), rotate(3), shift_band(3),
                transpose_band(3)

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/


#define BI(c, y1, y2, d) \
  {\
    double p1, p2;\
    p1 =  (double) y1; \
    p2 =  (double) y2 - (double) y1;\
    c = p1 + d*p2;\
   }


#define CC(c, y1, y2, y3, y4, d) \
  {\
    double p1, p2, p3, p4;\
    p1 =  (double) y2; \
    p2 =  - (double) y1 + (double) y3;\
    p3 = 2.0 * (double) y1 - 2.0 * (double) y2 + (double) y3 - (double) y4;\
    p4 = - (double) y1 + (double) y2 - (double) y3 + (double) y4;\
   c = p1 + d*(p2 + d*(p3 + d*p4));\
   }


static void affine0(IBAND ib, IBAND ob, double x0, double x1, double x2, double y0, double y1, double y2, int bg)
{
  int x, y, xc, yc;
  int xl, xu, yl, yu;
  int ixsize, iysize, ixstart, iystart;
  int oxsize, oysize, oxstart, oystart;
  double xt,yt;
  double xr, yr;

  ixsize = Ixsize(ib);
  iysize = Iysize(ib);
  ixstart = Ixstart(ib);
  iystart = Iystart(ib);
  oxsize = Ixsize(ob);
  oysize = Iysize(ob);
  oxstart = Ixstart(ob);
  oystart = Iystart(ob);

  xl = ixstart;
  xu = ixstart+ixsize-1;
  yl = iystart;
  yu = iystart+iysize-1;

  for (y = 1; y<=oysize; y++)
    for (x = 1; x<=oxsize; x++)
      {
	 xr = x - 1 + oxstart;
	 yr = y - 1 + oystart;
         xt = x0 + x1*xr + x2*yr;
         yt = y0 + y1*xr + y2*yr;
	 xc = xt + 0.5;
	 yc = yt + 0.5;
	 if (xc >= xl && xc <= xu && yc >= yl  && yc <= yu)
	   ob[y][x] =  ib[yc-iystart+1][xc-ixstart+1]; 
	 else 
	   ob[y][x] = bg;
       }
}	

static void affine1(IBAND ib, IBAND ob, double x0, double x1, double x2, double y0, double y1, double y2, int bg)
{
  int x, y, xc, yc;
  int xl, xu, yl, yu;
  int ixsize, iysize, ixstart, iystart;
  int oxsize, oysize, oxstart, oystart;
  double xt, yt, dx, dy;
  double bi1, bi2, bi3;
  double xr, yr;

  ixsize = Ixsize(ib);
  iysize = Iysize(ib);
  ixstart = Ixstart(ib);
  iystart = Iystart(ib);
  oxsize = Ixsize(ob);
  oysize = Iysize(ob);
  oxstart = Ixstart(ob);
  oystart = Iystart(ob);

  xl = ixstart;
  xu = ixstart+ixsize-2;
  yl = iystart;
  yu = iystart+iysize-2;

  for (y = 1; y<=oysize; y++)
    for (x = 1; x<=oxsize; x++)
      {
	 xr = x - 1 + oxstart;
	 yr = y - 1 + oystart;
         xt = x0 + x1*xr + x2*yr;
         yt = y0 + y1*xr + y2*yr;
	 xc = xt;
	 yc = yt;
	 if (xc >= xl && xc <= xu && yc >= yl  && yc <= yu)
	   {
	     dx = xt - xc;
	     dy = yt - yc;
	     BI(bi1, 
		ib[yc-iystart+1][xc-ixstart+1], 
		ib[yc-iystart+1][xc-ixstart+2], dx);
	     BI(bi2, 
		ib[yc-iystart+2][xc-ixstart+1],
		ib[yc-iystart+2][xc-ixstart+2], dx);
	     BI(bi3, bi1, bi2, dy);
	     ob[y][x] =  bi3 + 0.5;
	   } else ob[y][x] = bg;
       }
}	


static void affine3(IBAND ib, IBAND ob, double x0, double x1, double x2, double y0, double y1, double y2, int bg)
{
  int x, y, xc, yc;
  int xl, xu, yl, yu;
  int ixsize, iysize, ixstart, iystart;
  int oxsize, oysize, oxstart, oystart;
  double xt, yt, dx, dy;
  double  cu1, cu2, cu3, cu4, cu5;
  double xr, yr;

  ixsize = Ixsize(ib);
  iysize = Iysize(ib);
  ixstart = Ixstart(ib);
  iystart = Iystart(ib);
  oxsize = Ixsize(ob);
  oysize = Iysize(ob);
  oxstart = Ixstart(ob);
  oystart = Iystart(ob);

  xl = ixstart+1;
  xu = ixstart+ixsize-3;
  yl = iystart+1;
  yu = iystart+iysize-3;
 
 for (y = 1; y<=oysize; y++)
    for (x = 1; x<=oxsize; x++)
      {
	 xr = x - 1 + oxstart;
	 yr = y - 1 + oystart;
         xt = x0 + x1*xr + x2*yr;
         yt = y0 + y1*xr + y2*yr;
	 xc = xt;
	 yc = yt;
	 if (xc >= xl && xc <= xu && yc >= yl  && yc <= yu)
	   {
	     dx = xt - xc;
	     dy = yt - yc;
	     CC(cu1, 
		ib[yc-iystart][xc-ixstart],
		ib[yc-iystart][xc-ixstart+1], 
		ib[yc-iystart][xc-ixstart+2],
		ib[yc-iystart][xc-ixstart+3],dx);
	     CC(cu2, 
		ib[yc-iystart+1][xc-ixstart],
		ib[yc-iystart+1][xc-ixstart+1], 
		ib[yc-iystart+1][xc-ixstart+2],
		ib[yc-iystart+1][xc-ixstart+3],dx);
	     CC(cu3, 
		ib[yc-iystart+2][xc-ixstart],
		ib[yc-iystart+2][xc-ixstart+1], 
		ib[yc-iystart+2][xc-ixstart+2],
		ib[yc-iystart+2][xc-ixstart+3],dx);
	     CC(cu4, 
		ib[yc-iystart+3][xc-ixstart],
		ib[yc-iystart+3][xc-ixstart+1], 
		ib[yc-iystart+3][xc-ixstart+2],
		ib[yc-iystart+3][xc-ixstart+3],dx);
	     CC(cu5, cu1, cu2, cu3, cu4, dy);
	     if (cu5<0.0) cu5 = 0.0;
	     if (cu5>255.0) cu5 = 255.0;
	     ob[y][x] =  cu5 + 0.5;
	   } else ob[y][x] = bg;
       }
}	

int affine(IBAND inband, IBAND outband, double x0, double x1, double x2, double y0, double y1, double y2, int ip, int bg)
{

  if (Ipixtyp(inband) != Iu_byte_typ || Ipixtyp(outband) != Iu_byte_typ)
    return(Error(1, "Input and output pixel type must be unsigned byte.\n"));

  Info(0, "affine: ip %d, bg %d, %8g %8g %8g %8g %8g %8g\n", 
       ip, bg, x0, x1, x2, y0, y1, y2);
  switch(ip)
    {
    case 0:
      affine0(inband, outband, x0, x1, x2, y0, y1, y2, bg);
      break;
    case 1:
      affine1(inband, outband, x0, x1, x2, y0, y1, y2, bg);
      break;
    case 3:
      affine3(inband, outband, x0, x1, x2, y0, y1, y2, bg);
      break;
    default: 
      Error(2, "affine: Bad argument, ip = %d\n", ip);
      break;
    }

  return(0);
}
