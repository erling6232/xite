

/*

________________________________________________________________

        conv3x3
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



#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/convolve.h>
#include XITE_STDIO_H
#include <xite/utils.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

#ifndef MAIN

/*F:conv3x3*

________________________________________________________________

		conv3x3
________________________________________________________________

Name:		conv3x3 - convolution between band and 3x3 matrix

Syntax:         | #include <xite/convolve.h>
		|
                | BiffStatus conv3x3( IBAND input, IBAND output,
                |    int conv[9], double scale, double offset,
                |    int verbose );
		|

Description:    'conv3x3' performs the convolution between
                a band 'input' and a convolution kernel 'conv'.
                |
		|   conv[0]    conv[1]    conv[2]
		|   conv[3]    conv[4]    conv[5]
		|   conv[6]    conv[7]    conv[8]
		|
                The result of the convolution is scaled
		|   pix = (conv. result) * scale + offset;
                If 'scale' is zero, autoscaling is performed.
		If 'verbose' is nonzero, the number of underflows and
		overflows are printed. If 'verbose' is nonzero
		and autoscaling is performed, the scale and offset 
		factors are displayed.

		'conv3x3_overflow' and 'conv3x3_underflow' are 
		global variabels and may be declared as
		| extern int conv3x3_overflow, conv3x3_underflow

Return value:   | 0 => ok
		| 1 => bad input pixel type
		| 2 => bad output pixel type

See also:	conv3x3(1), convolve(3), laplace(3), gradient(3), sobel(3),
                prewitt(3), roberts(3)

Restrictions:	Accepts only unsigned byte pixels

Author:		Otto Milvang & Tor Lønnestad

Id: 		$Id$
________________________________________________________________

*/


int conv3x3_overflow;
int conv3x3_underflow;



#define ONESTEP(sa, sb, sc)\
	      sum  = p11[*(p0 sa)];\
	      sum += p12[*(p0 sb)];\
	      sum += p13[*(p0 sc)];\
	      sum += p21[*(p1 sa)];\
	      sum += p22[*(p1 sb)];\
	      sum += p23[*(p1 sc)];\
	      sum += p31[*(p2 sa)];\
	      sum += p32[*(p2 sb)];\
	      sum += p33[*(p2 sc)];

#define SCALE\
      sum *= iscale;\
      sum += ioffset;\
      if (sum & ~0xffff)\
	{\
	  if (sum < 0)\
	    {\
	      conv3x3_underflow++;\
	      *r++ = 0;\
	    } else {\
	      conv3x3_overflow++;\
	      *r++ = 255;\
	    }\
	} else {\
	  *r++ = sum >> 8;\
	}

#define	ARRAY(p, c)\
  p = (int*)malloc(256 * sizeof(int));\
  for(x=0; x <=255; x++)\
  p[x] = conv[c] *x;

#ifndef FUNCPROTO
BiffStatus conv3x3(input, output, conv, scale, offset, verbose)
IBAND input, output;
int conv[9];
double scale, offset;
int verbose;
#else /* FUNCPROTO */
BiffStatus conv3x3(IBAND input, IBAND output, int *conv, double scale, double offset, int verbose)
#endif /* FUNCPROTO */
{
  UNS_BYTE *p0, *p1, *p2, *r;
  int *p11, *p12, *p13, *p21, *p22, *p23, *p31, *p32, *p33;
  int sum, count, ioffset;
  int xsize, ysize, min, max, x, y, iscale;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(1, "conv3x3: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "conv3x3: Output pixel type must be unsigned byte.\n"));

  iscale  = scale  * 256.0;
  ioffset = offset * 256.0;

  xsize = Ixsize(input);
  ysize = Iysize(input);
  conv3x3_overflow = conv3x3_underflow = 0;

  ARRAY(p11, 0); ARRAY(p12, 1); ARRAY(p13, 2);
  ARRAY(p21, 3); ARRAY(p22, 4); ARRAY(p23, 5);
  ARRAY(p31, 6); ARRAY(p32, 7); ARRAY(p33, 8);
  
  if (iscale == 0) {
    min = 1000000; max = -1000000;
    for (y=2; y < ysize; y=y+16)
      for (x=2; x < xsize; x=x+16) { 
	p0 = &input[y-1][x-1];
	p1 = &input[y][x-1];
	p2 = &input[y+1][x-1];
	ONESTEP(++,++,--);
	if (sum<min) min = sum;
	if (sum>max) max = sum;
      }
    if (max > 0) max = 1.1 * max;
    if (min < 0) min = 1.1 * min;
    if (max < 0) max = 0.9 * max;
    if (min > 0) min = 0.9 * min;
    
    
    if (max-min) {
      iscale = 0x10000 / (max-min);
      ioffset = - (min*iscale) + 1;
    } else {
      iscale = 0;
      ioffset = 0;
    }
    if (verbose) {
      printf("Scale  = %.2f\n", (double) iscale / 256.0);
      printf("Offset = %d\n", ioffset/256);
    }
  }
  
  for (y=1; y <= ysize; y++) { 
    count = xsize -2;
    p0 = &input[y==1 ? 1 : y-1][1];
    p1 = &input[y][1];
    p2 = &input[y == ysize ? ysize : y+1][1];
    r  = &output[y][1];
    ONESTEP(+=0,++,--);
    SCALE;
    
    while(count--) {
      ONESTEP(++,++,--);
      SCALE;
    }
    ONESTEP(++,+=0,+=0);
    SCALE;
  }
  
  free(p11); free(p12); free(p13);
  free(p21); free(p22); free(p23); 
  free(p31); free(p32); free(p33);
  
  if (verbose) {
    printf("%6d Underflow%c\n", conv3x3_underflow,
	    conv3x3_underflow == 1 ? ' ' : 's');
    printf("%6d Overflow%c\n" , conv3x3_overflow, 
	    conv3x3_overflow == 1  ? ' ' : 's');
  }
  
  return(0);
}

#endif /* not MAIN */



/*P:conv3x3*

________________________________________________________________

		conv3x3
________________________________________________________________

Name:		conv3x3 - convolution between image and 3x3 matrix

Syntax:		| conv3x3 [-scale <f>] [-offset <m>] [-title <title>] 
                |       [-verbose] <inimage> <outimage> <v1> .. <v9>
                |  or: 
		| conv3x3 [-scale <f>] [-offset <m>] [-title <title>]  
                |        [-verbose] -conv <datafile> <inimage> <outimage>

Description:    'conv3x3' performs a 3x3 convolution on the input image
                'inimage' and writes the result to 'outimage'.

		The convolution kernel is specified as a list of
                nine integers, on the command line or in an ascii file.

Options:        &-conv filename
                'filename' specifies an Ascii file with the nine integers
		for the convolution kernel.
           
		&-scale scale
		Scale convolution result. Default: Autoscaling.
		| pix = (conv. result) * scale

		&-offset offset
		Offset (scaled) convolution result.
		| pix = (conv. result) * scale + offset

		&-title title
		Use 'title' as title for the output image.

                &-verbose
		With this standard XITE option,	a message is written if the
		convolution sum is less than 0 or greater than 255.
                If autoscaling is in effect, the scale and offset factors are
                displayed.

See also:       conv3x3(3), convolve(1), laplace(1), gradient(1), sobel(1),
                prewitt(1), roberts(1)

Example:	| conv3x3 mona.img m.img -scale 1.5 1 1 1 1 1 1 1 1 1
                | conv3x3 mona.img m.img - s 2.0 -o 33 -c conv.fil

Author:		Otto Milvang & Tor Lønnestad

Id: 		$Id$
________________________________________________________________

*/



#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE img, img2;
  int i;
  char *title, *conv3x3_file, *args;
  double scale, offset;
  int log;
  int conv[9];
  FILE *cf;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-scale <scale>] [-offset <offset>] [-t <title>] \n\
              <inimage> <outimage> <v1> .. <v9> \n\
   or: %s [-scale <scale>] [-offset <offset>] [-t <title>] \n\
                 -conv <convfile> <inimage> <outimage>\n"));
  Iset_message(1);
  Iset_abort(1);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  conv3x3_file = read_switch(&argc, argv, "-conv", 1, NULL);
  title        = read_switch(&argc, argv, "-title", 1, NULL);
  scale        = read_dswitch(&argc, argv, "-scale", 0.0);
  offset       = read_dswitch(&argc, argv, "-offset", 0.0);
  log          = Verbose();
  conv3x3_file = read_switch(&argc, argv, "-c", 1, conv3x3_file);
  title        = read_switch(&argc, argv, "-t", 1, title);
  scale        = read_dswitch(&argc, argv, "-s", scale);
  offset       = read_dswitch(&argc, argv, "-o", offset);

  if ( (argc != 3 && conv3x3_file != NULL) ||
       (argc != 12 && conv3x3_file == NULL ))
    Usage(2, "Bad number of arguments\n");

  if (conv3x3_file)
    {
      cf = fopen(tilde_expand(conv3x3_file),"r");
      for (i=0; i < 9; i++)
	fscanf(cf," %d",&conv[i]);
      fclose(cf);
    } else {
      for (i=0; i < 9; i++)
	conv[i] = atoi(argv[i+3]);
    }  

  img = Iread_image(argv[1]);
  if (img == NULL)
    exit(Error(2, "Can't read input file %s\n", argv[1]));

  img2 = Icopy_init(img);
  for (i=1; i <= Inbands(img); i++)
    if (conv3x3(img[i], img2[i], conv, scale, offset, log))
      Warning(3, "bad pixel type in band %d\n");

  Ihistory(img2,argv[0], args);
  if (title) Iset_title(img2, title);
  Iwrite_image(img2,argv[2]);

  return(0);
}

#endif /* MAIN */
