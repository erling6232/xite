

/*

________________________________________________________________

        rotate.c
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



/*F:rotate*

________________________________________________________________

		rotate
________________________________________________________________

Name:		rotate - rotate transform

Syntax:         | #include <xite/geometry.h>
		|
                | int rotate( IBAND inband, IBAND outband,
                |    double theta, double rx, double ry, int ip,
                |    int bg );

Description:	The input band is rotated around the point (rx, ry) in the
		global coordinate system. Thus, a band of size 25x25 with
		xstart=ystart=100 should have rx=ry=112 to be rotated around
		its center. If xstart=ystart=1, global coordinates equal local
		coordinates.

		'theta' is in radians.

		| outband (x,y) = inband (u,v)
                | u = cos(theta) *  x - sin(theta) *  y 
		|   - cos(theta) * rx + sin(theta) * ry + rx
		| v = sin(theta) *  x + cos(theta) *  y
		|   - sin(theta) * rx - cos(theta) * ry + ry

		The interpolation method 'ip' is one of
		&0
		Nearest neighbor

		&1
		Bilinear interpolation
		|      g(x) = bi(f(x), f(x+1), d)
		|      bi(x, y, d) = x(1-d) + yd
		
		&3
		Cubic interpolation
		|      g(x) = cc(f(x-1), f(x), f(x+1), f(x+2), d)
		|      cc(x, y, z, v, d) = y + d((-x+z) +
		|             d((2x-2y+z-v) + d(-x+y-z+v)))

		-&
		'bg' specifies the background pixel value.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Return value:   | 0 : Ok
                | 1 : Bad input or output pixel type

Reference:	Wayne Niblack. Digital Image Processing, p 131-150

See also:       rotate(1), shift_band(3), transpose_band(3), affine(3),
                quadratic(3)

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/

int rotate(IBAND inband, IBAND outband, double theta, double rx, double ry, int ip, int bg)
{
  return(affine(inband, outband, 
		-cos(theta) * rx + sin(theta) * ry + rx,
		cos(theta),
		-sin(theta),
		-sin(theta) * rx - cos(theta) * ry + ry,
		sin(theta),
		cos(theta), 
		ip, 
		bg));
}
