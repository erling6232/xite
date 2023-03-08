
/*

________________________________________________________________

        warp
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

/*P:warp*

________________________________________________________________

		warp
________________________________________________________________

Name:		warp - polynomial control point mapping

Syntax:		| warp [<option>...] <inimage> <outimage>

Description:    Perform an output to input coordinate mapping (k,l) = M(i,j).
                Given an input image I(k, l), calculate O(i,j). The coordinates
		on which the mapping is based, must be supplied in a file
		or on standard input.

		If start, stop as well as size options are given
		inconsistently for the output image, the stop option is
		ignored. The given sizes are clipped against the result of the
		transform.

Options:        Accepts all standard XITE options as well as the following

		&-cp cpfile
		Ascii control point file. Use '-' to supply the information
		on standard input. The input will be interpreted by the
		'ascii2biff' XITE function.

		The required information consists of pairs of points, real
		numbers. Each line is a set of 'i', 'j', 'k' and 'l'. 'i' and
		'j' are coordinates in the reference image. 'k' and 'l' are
		coordinates in the image to be transformed. Lines with # in
		coloumn 1 are treated as comment lines. Blank lines are
		ignored. Pixels inside a line are separated by blanks, lines
		are separated by newline.
		| #
                | # Example 1
                | #
		| #   i   j    k   l
		| #------------------
		|    57  44   61 144
                |    27  72   38 180
                |   114 122  132 201
                |   180 199  213 255
                |   193 166  215 217
                |   191 288  241 336
		
		&-p poldeg
		Degree of polynomials used in coordinate mapping. Do not
		use along with option '-c'. Option '-c' takes precedence.
		The default value is 1.
		If 'poldeg' == 1, there will be three coefficients,
		| k = Q(i, j) = q0 + i * q1 + j * q2
		| l = R(i, j) = r0 + i * r1 + j * r2
		If 'poldeg' == 2, there will be six coefficients,
		| k = Q(i, j) = q0 + i*q1 + j*q2
		|     + i*i*q3 + i*j*q4 + j*j*q5
		| l = R(i, j) = r0 + i*r1 + j*r2
		|     + i*i*r3 + i*j*r4 + j*j*r5

		&-c r
		The number of polynomial coefficients used in coordinate
		mapping. Do not use along with option '-p'. Option '-c'
		takes precedence. The default value is 3.

		&-ip n
                'n' determines the degree of the gray level interpolation
                polynomial used when determining pixel values for the warp
                function. Legal values are 'n'=0 for nearest neighbour,
                'n'=1 for bilinear interpolation, and 'n'=3 for cubic
                interpolation. Default is 1.

		Bilinear interpolation:
		|   g(x) = bi(f(x), f(x+1), d)
		|   bi(x, y, d) = x(1-d) + yd
		Cubic interpolation:
		|   g(x) = cc(f(x-1), f(x), f(x+1), f(x+2), d)
		|   cc(x, y, z, v, d) = y + d((-x+z)
		|          + d((2x-2y+z-v) + d(-x+y-z+v)))

		&-xstart xstart
		Horizontal start coordinate of output image bands.

		&-xstop xstop
		Horizontal stop coordinate of output image bands.

		&-xsize xsize
		Horizontal size of output image bands.

		&-ystart ystart
		Vertical start coordinate of output image bands.

		&-ystop ystop
		Vertical stop coordinate of output image bands.

		&-ysize xsize
		Vertical size of output image bands.

		&-rss
		Print all control points and root mean square errors
		(Root Sum of Squares),
		| dk   = k - Q(i,j)
		| dl   = l - R(i,j)
		| r(m) = sqrt(dk*dk + dl*dl)

                &-qr
                Print the polynomial coefficients used by the warp function
                when mapping coordinates. The output is on the form (with
		three polynomial coefficients):
                
                |   # Q = 
                |      -230.173    1.46376  0.0805173
                |   # R = 
                |     -63.7828  0.0869576    1.36232
                
                &-bg c
                The background pixel value is set to 'c'. The default equals 0.

Reference:	Wayne Niblack. Digital Image Processing, p 131-150

Restrictions:   Works on unsigned byte images only.

See also:       warp(3), xwarp(1), affine(1), quadratic(1), reorganize(1),
                rotate(1), shift_img(1), transpose_img(1), ascii2biff(3)

Author:		Lennart Flem 1/3-91.

Modified:       Otto Milvang and Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <xite/convert.h>
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

#define MAX_NCOEFFS  6

#define DEFPOLDEGREE 1

int main(int argc, char *argv[])
{
  IMAGE imgi, imgo;
  IR_BAND rband;
  int oargc, npoints, ncoeffs = -1, i, rss, qr, ip, bg, poldeg = -1;
  long xstart, xsize, ystart, ysize;
  char *infile, *outfile, *cpfile, *cxstart, *cxstop, *cxsize, *cystart,
      *cystop, *cysize, *args;
  char *local_usage =
"Usage: %s [<option>...] <inimage> <outimage>\n\
    where <option> may be \n\
    -cp <cpfile>     : Ascii control point file. \n\
    -p <poldeg>      : Degree of polynomials used in coordinate mapping. \n\
    -c <r>           : The number of polynomial coefficients used in \n\
                       coordinate mapping. \n\
    -ip <n>          : Determines the degree of the gray level interpolation\n\
                       polynomial for the warp function. \n\
    -xstart <xstart> : Horizontal start coordinate of output image bands. \n\
    -xstop <xstop>   : Horizontal stop coordinate of output image bands. \n\
    -xsize <xsize>   : Horizontal size of output image bands. \n\
    -ystart <ystart> : Vertical start coordinate of output image bands. \n\
    -ystop <ystop>   : Vertical stop coordinate of output image bands. \n\
    -ysize <xsize>   : Vertical size of output image bands. \n\
    -rss             : Print all control points and root mean square errors \n\
                       (Root Sum of Squares). \n\
    -qr              : Print the polynomial coefficients used by the warp \n\
                       function when mapping coordinates. \n\
    -bg <c>          : The background pixel value. \n";

  Iset_message(TRUE);
  Iset_abort(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(local_usage));

  if (argc == 1) Usage(1, NULL);
  args    = argvOptions(argc, argv);

  cpfile  = read_switch(&argc, argv, "-cp", 1, NULL);
  ncoeffs = read_iswitch(&argc, argv, "-c", 3);
  rss     = read_bswitch(&argc, argv, "-rss");
  qr      = read_bswitch(&argc, argv, "-qr");
  ip      = read_iswitch(&argc, argv, "-ip", 1);
  bg      = read_iswitch(&argc, argv, "-bg", 0);
  if (ncoeffs == -1) poldeg = read_iswitch(&argc, argv, "-p", DEFPOLDEGREE);
  cxstart = read_switch(&argc, argv, "-xstart", 1, NULL);  
  cystart = read_switch(&argc, argv, "-ystart", 1, NULL);  
  oargc = argc;
  cxstop  = read_switch(&argc, argv, "-xstop",  1, NULL);  
  cxsize  = read_switch(&argc, argv, "-xsize",  1, NULL);  
  cystop  = read_switch(&argc, argv, "-ystop",  1, NULL);  
  cysize  = read_switch(&argc, argv, "-ysize",  1, NULL);  

  if (argc != 1 && argc != 3) Usage(1, "Illegal number of arguments.\n");
  if (poldeg != -1 && poldeg != 1 && poldeg != 2)
    Usage(2, "Polynomial degree must be 1 or 2.\n");
  if (ncoeffs != -1 && (ncoeffs < 1 || ncoeffs > MAX_NCOEFFS))
    Usage(2, "Polynomial must have at least one coefficient, maximum six.\n");

  if (ncoeffs == -1) ncoeffs = (poldeg+1)*(poldeg+2)/2;
  if (poldeg  == -1) poldeg  = ncoeffs <= 3 ? 1 : 2;

  Info(0, "Verbose switch is on.\nPolynomial degree ");
  Info(0, "%d, minimum %d control points\n ", poldeg, ncoeffs);

  if (cpfile && !strcmp(cpfile, "-"))
    Info(0, "Expecting control points from stdin.\n");

  ascii2biff((IBAND *) &rband, cpfile, Ireal_typ);
  if (Ixsize((IBAND) rband) < 4) Error(6, "Error in cpfile.\n");
  npoints = Iysize((IBAND) rband);

  infile  = argv[1];
  outfile = argv[2];
  imgi    = Iread_image(infile);
  if (Ipixtyp(imgi[1]) != Iu_byte_typ)
    Error(2, "Input band must have pixel type unsigned byte.\n");
  xsize   = Ixsize(imgi[1]);
  ysize   = Iysize(imgi[1]);

  if (argc == oargc) {
    /* No size or stop options given. */
    imgo = Icopy_init(imgi);

    if (cxstart || cystart) {
      for(i = 1; i <= Inbands(imgo); i++) {
	xstart = cxstart ? atoi(cxstart) : Ixstart(imgo[i]);
	ystart = cystart ? atoi(cystart) : Iystart(imgo[i]);
	Iset_start(imgo[i], xstart, ystart);
      }
    }
  } else {
    xsize = cxsize ? atoi(cxsize) : ( (cxstop && cxstart) ?
				      atoi(cxstop) - atoi(cxstart) + 1 : 
				      Error(2, "Bad xsize specification.\n")); 
    ysize = cysize ? atoi(cysize) : ( (cystop && cystart) ?
				      atoi(cystop) - atoi(cystart) + 1 : 
				      Error(2, "Bad xsize specification.\n")); 
    imgo = Imake_image(Inbands(imgi), Ititle(imgi), Iu_byte_typ, xsize, ysize);
    for(i=1; i<=Inbands(imgo); i++) {
      xstart = cxstart ? atoi(cxstart) : Ixstart(imgi[i]);
      ystart = cystart ? atoi(cystart) : Iystart(imgi[i]);
      Iset_start(imgo[i], xstart, ystart);
    }
    Icopy_text(imgi, imgo);
  }
  
  if (warp(imgi, imgo, (IBAND) rband, npoints, ncoeffs, ip, bg, rss, qr))
    Error(2, "Error in the warp function.\n");

  Ihistory(imgo, argv[0], args);
  Iwrite_image(imgo, outfile);
  
  return(0);
}
