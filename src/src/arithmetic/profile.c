
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



/*F:profile*

________________________________________________________________

                profile
________________________________________________________________

Name:           profile - Calculate pixel values along a line.
Syntax:         | #include <xite/arithmetic.h>
                | 
                | int profile( IBAND band, double x, double y,
                |    double direction, double step, double dist,
                |    int ip, double* arr, int maxlen );
		|
Description:	profile calculates the pixel values along a line starting
                at (x,y) in the direction 'direction'.

		'band' is a BIFF image band of any type. 'x' and 'y' make up
		the start point of the profile. 'direction' is the direction
		of the profile in radians. 'step' is the distance between two
		points in the profile. 'dist' is the length of the profile.
		'ip' is the interpolation degree (only ip = 1 is implemented).
		'arr' is an array of return values, starting on array index 1
		where the profile is stored. 'maxlen' is the maximum length of
		the profile (length of arr)

Return value:   Length of profile, or negative value on error.

See also:       crossSection(3)

Author:         Otto Milvang
________________________________________________________________

*/

int profile(IBAND band, double x, double y, double direction, double step, 
	    double dist, int ip, double *arr, int maxlen)
{
  IUB_BAND ub_band;
  ISB_BAND sb_band;
  IUS_BAND us_band;
  ISS_BAND ss_band;
  II_BAND i_band;
  IR_BAND r_band;
  ID_BAND d_band;
  double ddx, ddy, s, dx, dy, r1, r2, r;
  int ix, iy, xsize, ysize, len;
  ub_band = (IUB_BAND) band;
  sb_band = (ISB_BAND) band;
  us_band = (IUS_BAND) band;
  ss_band = (ISS_BAND) band;
  i_band = (II_BAND) band;
  r_band = (IR_BAND) band;
  d_band = (ID_BAND) band;

  ddx = step * cos(direction);
  ddy = step * sin(direction);
  if (dist == 0.0) dist = 1.0E+10;  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  x = x - Ixstart(band) + 1.0;
  y = y - Iystart(band) + 1.0;
  len = 0;


  for(s = 0.0; dist >=0; dist -= step) {
    if (x < 1 || x > xsize || y < 1 || y > ysize) break;
    ix = x;
    iy = y;
    dx = x - ix;
    dy = y - iy;
    switch((int) Ipixtyp(band)) {
    case Iu_byte_typ:
      r1 = (dx > 0) ? 
	    (ub_band[iy][ix] * (1.0 - dx) + ub_band[iy][ix+1] * dx) : 
	    (ub_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (ub_band[iy+1][ix] * (1.0 - dx) + ub_band[iy+1][ix+1] * dx) : 
	    (ub_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    case Is_byte_typ:
      r1 = (dx > 0) ? 
	    (sb_band[iy][ix] * (1.0 - dx) + sb_band[iy][ix+1] * dx) : 
	    (sb_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (sb_band[iy+1][ix] * (1.0 - dx) + sb_band[iy+1][ix+1] * dx) : 
	    (sb_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    case Iu_short_typ:
      r1 = (dx > 0) ? 
	    (us_band[iy][ix] * (1.0 - dx) + us_band[iy][ix+1] * dx) : 
	    (us_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (us_band[iy+1][ix] * (1.0 - dx) + us_band[iy+1][ix+1] * dx) : 
	    (us_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    case Is_short_typ:
      r1 = (dx > 0) ? 
	    (ss_band[iy][ix] * (1.0 - dx) + ss_band[iy][ix+1] * dx) : 
	    (ss_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (ss_band[iy+1][ix] * (1.0 - dx) + ss_band[iy+1][ix+1] * dx) : 
	    (ss_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    case Integer_typ:
      r1 = (dx > 0) ? 
	    (i_band[iy][ix] * (1.0 - dx) + i_band[iy][ix+1] * dx) : 
	    (i_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (i_band[iy+1][ix] * (1.0 - dx) + i_band[iy+1][ix+1] * dx) : 
	    (i_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    case Ireal_typ:
      r1 = (dx > 0) ? 
	    (r_band[iy][ix] * (1.0 - dx) + r_band[iy][ix+1] * dx) : 
	    (r_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (r_band[iy+1][ix] * (1.0 - dx) + r_band[iy+1][ix+1] * dx) : 
	    (r_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    case Idouble_typ:
      r1 = (dx > 0) ? 
	    (d_band[iy][ix] * (1.0 - dx) + d_band[iy][ix+1] * dx) : 
	    (d_band[iy][ix]); 
      if (dy > 0) r2 = (dx > 0) ? 
            (d_band[iy+1][ix] * (1.0 - dx) + d_band[iy+1][ix+1] * dx) : 
	    (d_band[iy+1][ix]); 
      r = (dy > 0) ? (r1 * (1.0 - dy) + r2 * dy) : (r1); 
      break;
    }
    len++;
    arr[len] = r;
    if (len == maxlen) break;
    x += ddx;
    y += ddy;
  }
  return(len);
}
