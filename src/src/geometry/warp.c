
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

#ifndef MAIN

/*
static float polmap(q, polgrad, y, x)
float *q;
int polrad, y, x;
{
  int pg, ph, i = 0;
  float v = 0;
  for (pg=0; pg<=polgrad; pg++)
    for (ph=pg; ph>=0; ph--) 
      v += q[i++] * pow((float)x,(float)(ph)) * pow((float)y,(float)(pg-ph));
  return v;
}
*/

#ifndef FUNCPROTO
int warp_resample(inimg, outimg, xxa, yya, ncoeffs, ip, bg)
IMAGE inimg, outimg;
double *xxa, *yya;
int ncoeffs, ip, bg;
#else /* FUNCPROTO */
int warp_resample(IMAGE inimg, IMAGE outimg, double *xxa, double *yya, int ncoeffs, int ip, int bg)
#endif /* FUNCPROTO */
{
  int bandno;
  IBAND inband, outband;
 
  if (ncoeffs > MAX_NCOEFFS) 
    return(Error(1, "%s%s\n",
		 "warp_resample(): Number of coefficients must not ",
		 "exceed six."));
 
  for (bandno=1; bandno <= Inbands(inimg); bandno++) {
    outband = outimg[bandno];
    inband  = inimg[bandno];
 
    Info(0, "Output-to-input map band %d ... \n", bandno);

    if (ncoeffs <= 3) {
      affine(inband, outband, xxa[0], xxa[1], xxa[2],
             yya[0], yya[1], yya[2], ip, bg);
    } else {
      quadratic(inband, outband, xxa, yya, ip, bg);
    }
  }
 
  return(0);
 
} /* warp_resample() */

#ifndef FUNCPROTO
static void printCoeffs(Q, R)
xiteMatrix Q, R;
#else /* FUNCPROTO */
static void printCoeffs(xiteMatrix Q, xiteMatrix R)
#endif /* FUNCPROTO */
{
  int antelem, elem;

  antelem = Q->rows * Q->cols;
  printf("# Q = \n");

  for (elem=0; elem < antelem; elem++)
    printf(" %10g", Q->elem[elem]);

  printf("\n");

  antelem = R->rows * R->cols;
  printf("# R = \n");

  for (elem=0; elem < antelem; elem++)
    printf(" %10g", R->elem[elem]);

  printf("\n");

  return;

} /* printCoeffs() */

#ifndef FUNCPROTO
static void printResiduals(Q, R, points, npoints, ncoeffs)
xiteMatrix Q, R;
IBAND points;
int npoints, ncoeffs;
#else /* FUNCPROTO */
static void printResiduals(xiteMatrix Q, xiteMatrix R, IBAND points, int npoints, int ncoeffs)
#endif /* FUNCPROTO */
{
  IR_BAND rpt_band = (IR_BAND) points;
  float dk, dl, rs, err = 0.0, maxerr = 0.0;
  float ir, jr, kr, lr;
  int i;
  
  if (npoints == ncoeffs) {
    Message(0, "RSS equals zero, #pts = #coeff.");
    return;
  }

  printf("%s%s\n",
	 "#         i          j     Q(i,j)     R(i,j)",
	 "          k          l        rss");

  for (i = 0; i < npoints; i++) {
    ir = rpt_band[i+1][1]; 
    jr = rpt_band[i+1][2];
    kr = rpt_band[i+1][3]; 
    lr = rpt_band[i+1][4];

    dk = kr - Q->elem[0];
    dl = lr - R->elem[0];

    if (ncoeffs > 1) {
      dk = dk - ir * Q->elem[1];
      dl = dl - ir * R->elem[1];
    }
    if (ncoeffs > 2) {
      dk = dk - jr * Q->elem[2];
      dl = dl - jr * R->elem[2];
    }      
    if (ncoeffs > 3) {
      dk = dk - ir * ir * Q->elem[3];
      dl = dl - ir * ir * R->elem[3];
    }
    if (ncoeffs > 4) {
      dk = dk - ir * jr * Q->elem[4];
      dl = dl - ir * jr * R->elem[4];
    }
    if (ncoeffs > 5) {
      dk = dk - jr * jr * Q->elem[5];
      dl = dl - jr * jr * R->elem[5];
    }

    rs = sqrt(dk*dk + dl *dl);
    if (rs > maxerr) maxerr = rs;
    err += rs;

    printf(" %10g %10g %10g %10g %10g %10g %10g\n", 
	   ir, jr, kr-dk, lr-dl, kr, lr, rs);
  }

  Message(0, "Warp mean rss: %.2f, max rss: %.2f", err / npoints, maxerr);

  return;

} /* printResiduals() */

#ifndef FUNCPROTO
int oiWarpMapping(M, Mp, Q, R, K, L, points, npoints, ncoeffs)
xiteMatrix *M, *Mp, *Q, *R, *K, *L;
IBAND points;
int npoints, ncoeffs;
#else /* FUNCPROTO */
int oiWarpMapping(xiteMatrix *M, xiteMatrix *Mp, xiteMatrix *Q, xiteMatrix *R, xiteMatrix *K, xiteMatrix *L, IBAND points, int npoints, int ncoeffs)
#endif /* FUNCPROTO */
{
  float *ijdata, *kdata, *ldata;
  float ir, jr, kr, lr;
  int ind, i, j;
  IR_BAND rpt_band = (IR_BAND) points;

  ijdata = (float *) malloc(ncoeffs * npoints * sizeof(float));
  kdata  = (float *) malloc(npoints * sizeof(float));
  ldata  = (float *) malloc(npoints * sizeof(float));

  ind = 0;
  for (i=0; i < npoints; i++) {
    ir = rpt_band[i+1][1]; 
    jr = rpt_band[i+1][2]; 
    kr = rpt_band[i+1][3]; 
    lr = rpt_band[i+1][4]; 

    j = 0;
    if (j < ncoeffs) ijdata[ind + j++] = 1.0;
    if (j < ncoeffs) ijdata[ind + j++] = ir;
    if (j < ncoeffs) ijdata[ind + j++] = jr;
    if (j < ncoeffs) ijdata[ind + j++] = ir * ir;
    if (j < ncoeffs) ijdata[ind + j++] = ir * jr;
    if (j < ncoeffs) ijdata[ind + j++] = jr * jr;
    ind += j;

    kdata[i] = kr; 
    ldata[i] = lr;
  }

  Info(0, "Create transformation model ... ");
  *M  = MData(npoints, ncoeffs, ijdata);
  *K  = MData(npoints, 1, kdata);
  *L  = MData(npoints, 1, ldata);
  *Mp = MPseudoInv(*M);
  *Q  = MMult(*Mp, *K);
  *R  = MMult(*Mp, *L);
  Info(0, "Transformation model done.\n");

  return(0);

} /* oiWarpMapping() */

/*F:warp_resample=warp*/
/*F:oiWarpMapping=warp*/
/*F:warp*

________________________________________________________________

		warp
________________________________________________________________

Name:		warp, warp_resample, oiWarpMapping - polynomial control
                point mapping

Syntax:		| #include <xite/geometry.h>
		| 
		| int warp( IMAGE inimg, IMAGE outimg, IBAND points,
		|    int npoints, int ncoeffs, int ip, int bg,
		|    int rss, int qr );
		|
		| int warp_resample( IMAGE inimg, IMAGE outimg,
		|    double *xxa, double *yya, int ncoeffs, int ip,
		|    int bg );
		|
		| int oiWarpMapping( xiteMatrix *M, xiteMatrix *Mp,
		|    xiteMatrix *Q, xiteMatrix *R, xiteMatrix *K,
		|    xiteMatrix *L, IBAND points, int npoints,
		|    int ncoeffs );

Description:	'warp' performs an output to input coordinate mapping
                (k,l) = M(i,j). Given an input image I(k, l), calculate O(i,j).

		'points' must supply the coordinates on which the mapping is
		based. It must be an 'npoints' by 4 image band with real pixel
		type. Each row is a set of 'i', 'j', 'k' and 'l'. 'i' and 'j'
		are coordinates in the reference image. 'k' and 'l' are
		coordinates in the image to be transformed.

		'npoints' must be at least as large as 'ncoeffs'.

		'ncoeffs' is the number of polynomial coefficients used in the
		coordinate mapping.

		'ip' determines the degree of the gray level interpolation
                polynomial used when determining pixel values for the warp
                function. Legal values are 0 for nearest neighbour,
                1 for bilinear interpolation, and 3 for cubic
                interpolation.
		Bilinear interpolation:
		|   g(x) = bi(f(x), f(x+1), d)
		|   bi(x, y, d) = x(1-d) + yd
		Cubic interpolation:
		|   g(x) = cc(f(x-1), f(x), f(x+1), f(x+2), d)
		|   cc(x, y, z, v, d) = y + d((-x+z) +
		|                       d((2x-2y+z-v) + d(-x+y-z+v)))

                'bg' is used as value for the background pixels.

		'rss' determines whether or not to print all control points
		and root mean square errors (Root Sum of Squares),
		| dk   = k - Q(i,j)
		| dl   = l - R(i,j)
		| r(m) = sqrt(dk*dk + dl*dl)

                'qr' determines whether or not to print the polynomial
                coefficients used by the warp function when mapping
                coordinates. The output is on the form (with three polynomial
                coefficients):
                
                |   # Q = 
                |      -230.173    1.46376  0.0805173
                |   # R = 
                |     -63.7828  0.0869576    1.36232
                
		The xstart and ystart values of the images are honored.

		'warp_resample' is used by 'warp' to resample the input image
		after the coordinate mapping polynomials have been computed.
		'xxa' and 'yya' are arrays of these polynomial coefficients,
		maximum six of each, at least as many as 'ncoeffs'.

		'warp_resample' calls 'affine' or 'quadratic', depending on
		the value of 'ncoeffs'.

		'oiWarpMapping' computes the actual coordinate mapping
		polynomials. It is used by 'warp'. Memory is allocated for
		the returning matrices, and must be freed by the caller, with
		calls to 'MFree'. 'M' is the ('npoints' x 'ncoeffs')
		output-to-input coordinate mapping matrix in (k,l) = M(i,j),
		'Mp' is the pseudoinverse of 'M', 'Q' and 'R' are
		('ncoeffs' x 1) vectors given by k = Q(i,j) and l = R(i,j)
		respectively, 'K' and 'L' are ('npoints' x 1) vectors
		containing the 'k' and 'l' coordinates from 'points'.

Restrictions:	All the bands of 'inimg' and 'outimg' must have pixel type
                unsigned byte. The pixel type of 'points' must be real.

Return value:	| 0 : Ok
                | 1 : Error

See also:	warp(1), xwarp(1), affine(3), quadratic(3)
Reference:	Wayne Niblack. Digital Image Processing, p 131-150
Author:		Svein Bøe, based on program warp and xwarp.
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int warp(inimg, outimg, points, npoints, ncoeffs, ip, bg, rss, qr)
IMAGE inimg, outimg;
int ncoeffs, ip, rss, qr, bg, rnpoints;
IBAND points;
#else /* FUNCPROTO */
int warp(IMAGE inimg, IMAGE outimg, IBAND points, int npoints, int ncoeffs, int ip, int bg, int rss, int qr)
#endif /* FUNCPROTO */
{
  int status, i;
  xiteMatrix M, Mp, Q, R, K, L;
  double xxa[MAX_NCOEFFS], yya[MAX_NCOEFFS];

  if (ncoeffs > MAX_NCOEFFS) {
    return(Error(1, "%s%d%s\n",
		 "warp(): Maximum ", MAX_NCOEFFS,
		 "-coefficient polynomials (second degree) allowed."));
  }
  if (ncoeffs < 1) {
    return(Error(1, "%s\n",
		 "warp(): Polynomials must have at least one coefficient."));
  }
  if (npoints < ncoeffs) {
    return(Error(1, "%s%s%d%s\n",
		 "warp(): # of control points must be >= # of ",
		 "polynomial coefficients (", ncoeffs, ")."));
  }

  oiWarpMapping(&M, &Mp, &Q, &R, &K, &L, points, npoints, ncoeffs);
  if (qr)  printCoeffs(Q, R);
  if (rss) printResiduals(Q, R, points, npoints, ncoeffs);

  for (i = 0; i < ncoeffs; i++) xxa[i] = Q->elem[i];
  for (i = ncoeffs; i < MAX_NCOEFFS; i++) xxa[i] = 0;
  for (i = 0; i < ncoeffs; i++) yya[i] = R->elem[i];
  for (i = ncoeffs; i < MAX_NCOEFFS; i++) yya[i] = 0;

  if ((status = warp_resample(inimg, outimg, xxa, yya, ncoeffs, ip, bg)) != 0)
    return(status);

  MFree(M);
  MFree(Mp);
  MFree(Q);
  MFree(R);
  MFree(K);
  MFree(L);

  return(0);

} /* warp() */

#endif /* not MAIN */

#ifdef MAIN

#define DEFPOLDEGREE 1

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char *argv[])
#endif /* FUNCPROTO */
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

#endif /* MAIN */
