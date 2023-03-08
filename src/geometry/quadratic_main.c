 
/*

________________________________________________________________

        quadratic.c
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



/*P:quadratic*

________________________________________________________________

		quadratic
________________________________________________________________

Name:		quadratic - quadratic geometric transform

Syntax:		| quadratic [-ip <n>] [-bg <bg>] <inimage> <outimage> \\
                |     x0 .. x5 y0 .. y5
                | quadratic [-ip <n>] [-bg <bg>] -f <datafile> \\
		|     <inimage> <outimage>

Description:	'quadratic' performs a quadratic transform of the coordinate
                system (x, y) to another coordinate system (u,v)
		All coordinates are adjusted acording to (xstart, ystart).
		| outimage (x,y) = inimage (u,v)
                | u = x0 + x1 * x + x2 * y 
		|   + x3 * x * x + x4 * x * y + x5 * y * y 
                | v = y0 + y1 * x + y2 * y 
		|   + y3 * x * x + y4 * x * y + y5 * y * y 

		The interpolation method '-ip' is
		| -ip 0 : Nearest neighbor 
		| -ip 1 : Bilinear   interpolation
		|         g(x) = bi(f(x), f(x+1), d)
		|         bi(x, y, d) = x(1-d) + yd
		| -ip 3 ; Cubic      interpolation
		|         g(x) = cc(f(x-1), f(x), f(x+1), f(x+2), d)
		|         cc(x, y, z, v, d) = y + d((-x+z) +
		|                             d((2x-2y+z-v) + d(-x+y-z+v)))

		The values x0 .. x5 and y0 .. y5 may be specified on the
		command line or in a datafile. The format on the datafile is
		| # Lines with # or ; in pos 1 are treated as comments
		| x0 x1 x2 x3 x4 x5
		| y0 y1 y2 y3 y4 y5
		All matrix of size n x 2 (n >= 6) will be accepted.
		If n > 6 only element 0..5 will be used.

		Default is bilinear interpolation (-ip 1)
		
		If the output image should have another size or startpoint
		than the input image this may be specified by the
		'-xstart', '-xstop', '-xsize', '-ystart', '-ystop' and '-ysize'
		switch. If start, stop as well as size switches are given, and
		inconsistently, the stop switch is ignored. The given
		sizes are clipped against the result of the transform.
		| -bg is used to specify the background pixel value,
		|     default = 0.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Reference:	Wayne Niblack. Digital Image Processing, p 131-150

See also:	quadratic(3), affine(1), rotate(1), reorganize(1),
                shift_img(1), transpose_img(1), warp(1)

Author:		Otto Milvang

Examples:	| quadratic mona.img monarot.img  30 .5 .714 -40 -.0714 .5

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{ 
  IMAGE img, img2;
  IR_BAND rband;
  char *args, *datafile;
  int i, ip, oargc, bg;
  int xstart, xsize, ystart, ysize;
  char *cxstart, *cxstop, *cxsize, *cystart, *cystop, *cysize;
  double xa[6], ya[6];

  InitMessage(&argc, argv, xite_app_std_usage_text(
   "Usage: %s [-ip n] \n\
    [-xstart <xstart>] [-xstop <xstop>] [-xsize <xsize>]\n\
    [-ystart <ystart>] [-ystop <ystop>] [-ysize <xsize>]\n\
    <infile> <outfile> x0 x1 x2 x3 x4 x5 y0 y1 y2 y3 y4 y5\n\
   or: %s [-ip n] -f <datafile> \n\
    [-xstart <xstart>] [-xstop <xstop>] [-xsize <xsize>]\n\
    [-ystart <ystart>] [-ystop <ystop>] [-ysize <xsize>]\n\
    <infile> <outfile>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  ip       = read_iswitch(&argc, argv, "-ip", 1);
  bg       = read_iswitch(&argc, argv, "-bg", 0);
  datafile = read_switch(&argc, argv, "-f", 1, NULL);
  cxstart  = read_switch(&argc, argv, "-xstart", 1, NULL);  
  cystart  = read_switch(&argc, argv, "-ystart", 1, NULL);  
  oargc    = argc;
  cxstop   = read_switch(&argc, argv, "-xstop",  1, NULL);  
  cxsize   = read_switch(&argc, argv, "-xsize",  1, NULL);  
  cystop   = read_switch(&argc, argv, "-ystop",  1, NULL);  
  cysize   = read_switch(&argc, argv, "-ysize",  1, NULL);  

  if (argc != (datafile ? 3 : 15)) Usage(2, "Illegal number of arguments.\n");

  if (datafile) {
    ascii2biff((IBAND *)&rband, datafile, Ireal_typ);
    if (Ixsize((IBAND) rband) < 6 || Iysize((IBAND) rband) != 2)
      Error(2, "Bad size in file %s (size %d x %d, expexted 6 x 2)\n",
	    Ixsize((IBAND) rband), Iysize((IBAND) rband));
    for (i=0; i<6; i++) xa[i] = rband[1][i+1];
    for (i=0; i<6; i++) ya[i] = rband[2][i+1];
  } else {
    for(i=0; i<6; i++) xa[i] = atof(argv[i+3]);
    for(i=0; i<6; i++) ya[i] = atof(argv[i+9]);
  }
  
  if ((img = Iread_image(argv[1])) == NULL)
    Error(3, "Can't read input file %s\n", argv[1]);
  
  if (argc == oargc) {
    img2 = Icopy_init(img);
    if (cxstart || cystart)
      for(i=1; i<=Inbands(img2); i++)
	{
	  xstart = cxstart ? atoi(cxstart) : Ixstart(img2[i]);
	  ystart = cystart ? atoi(cystart) : Iystart(img2[i]);
	  Iset_start(img2[i], xstart, ystart);
	}
  } else {
    xsize = cxsize ? atoi(cxsize) : ( (cxstop && cxstart) ?
				     atoi(cxstop) - atoi(cxstart) + 1 : 
				     Error(2, "Bad xsize specification\n")); 
    ysize = cysize ? atoi(cysize) : ( (cystop && cystart) ?
				     atoi(cystop) - atoi(cystart) + 1 : 
				     Error(2, "Bad xsize specification\n")); 
    img2 = Imake_image(Inbands(img), Ititle(img), 3, xsize, ysize);
    for(i=1; i<=Inbands(img2); i++)
      {
	xstart = cxstart ? atoi(cxstart) : Ixstart(img[i]);
	ystart = cystart ? atoi(cystart) : Iystart(img[i]);
	Iset_start(img2[i], xstart, ystart);
      }
    Icopy_text(img, img2);
  }
  
  for (i=1; i <= Inbands(img); i++)
    quadratic(img[i], img2[i], xa, ya, ip, bg);
  
  Ihistory(img2, argv[0], args);
  Iwrite_image(img2, argv[2]);

  return(0);
}
