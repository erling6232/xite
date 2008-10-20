/*C*

________________________________________________________________

        peanoScan
        $Id$
        Copyright 1991, Blab, UiO
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
#include <xite/texture.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>



#ifndef MAIN

/*F:peanoScan*

________________________________________________________________

		peanoScan
________________________________________________________________

Name:		peanoScan - scan a band in "peano" order

Syntax:         | #include <xite/texture.h>
		|
                | int peanoScan( IBAND inband, IBAND outband,
                |    int blockPow );
Description:	This procedure scans an input band according to the 
                Peano curve rules and write the pixel values in 
		horizontal lines in an output band, one dimensional
		representation of the input band.

		If power is set to zero, only the largest possible
		square block of size some power of two is transformed.
		| EX:
		|   128x128  : The whole band is transformed
		|   127x1024 : Only the upper, left 64x64 is transformed

		If power is set to 'n', the image is split up in 
		disjunct, square blocks of size (2**n)x(2**n). Each
		complete block (entirely inside	the band) is transformed
		separately.
		| EX blockPow = 3:
		|   128x128  : | The whole band is transformed in 16x16
		|              | separate blocks of size 8x8
		|   127x1024 : | The upper, left field of size 120x1024
		|              | is transformed in 15x128 separate blocks
		|              | of size 8x8

Restrictions:	Accepts only usigned byte pixels.

Return value:	| 0 => ok
		| 1 => Bad input pixel type
		| 2 => Bad output pixel type
		| 3 => Bad power argument
		| 4 => Warning: Output band to small

Reference: 	| 1) Joel Quinqueton and Marc Berthod
                |    "A Locally Adaptive Peano Scanning Algorithm"
		|    IEEE Transaction on Pattern Ana. and Machine Inte.
		|    Vol. PAMI-3, No. 4, July 1981
		| 2) P. T. Nguyen and J. Quinqueton
		|    "Space Filling Curves and Texture Analysis"
		|    IEEE Transaction on Pattern Ana. and Machine Inte.
		|    Vol. 2, 1982.

Author:		Yogesan and Lønnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif
#define COPY { ob[oy][ox++] = ib[y][x];		\
     if (ox > oxsize) {				\
        ox = 1; oy++;				\
        if (oy > oysize) { stop = TRUE; return; } } }


static int x, y, ox, oy, oxsize, oysize, stop;
static IBAND ib, ob;
static void right(int l);
static void left(int l);
static void up(int l);
static void down(int l);

static void right(int l)
{
/* printf("X:%d Y:%d OX:%d OY:%d L:%d\n", x, y, ox, oy, l); */
/* printf("X:%d Y:%d L:%d\n", x, y, l);  if (stop) return; */
  IF (l == 1)
    COPY;       y++;
    COPY;       x++;
    COPY;       y--;
    COPY;
  ELSE
    down(l-1);  y++;
    right(l-1); x++;
    right(l-1); y--;
    up(l-1);
  ENDIF;
}

static void left(int l)
{
  if (stop) return;
  IF (l == 1)
    COPY;       y--;
    COPY;       x--;
    COPY;       y++;
    COPY;
  ELSE
    up(l-1);    y--;
    left(l-1);  x--;
    left(l-1);  y++;
    down(l-1);
  ENDIF;
}

static void up(int l)
{
  if (stop) return;
  IF (l == 1)
    COPY;       x--;
    COPY;       y--;
    COPY;       x++;
    COPY;
  ELSE
    left(l-1);  x--;
    up(l-1);    y--;
    up(l-1);    x++;
    right(l-1);
  ENDIF;
}

static void down(int l)
{
  if (stop) return;
  IF (l == 1)
    COPY;       x++;
    COPY;       y++;
    COPY;       x--;
    COPY;
  ELSE
    right(l-1); x++;
    down(l-1);  y++;
    down(l-1);  x--;
    left(l-1);
  ENDIF;
}


int peanoScan(IBAND inband, IBAND outband, int blockPow)
{
  int i, xsize, ysize, xstart, ystart, n, ws;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  n = MIN(xsize, ysize);
  oxsize = Ixsize(outband);
  oysize = Iysize(outband);
  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "peanoScan: Bad input pixel type\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "peanoScan: Bad output pixel type\n"));
  if (blockPow < 0)
    return(Error(3, "peanoScan: Bad power argument\n"));
  ib = inband;
  ob = outband;
  ox = oy = 1;
  stop = FALSE;

  IF (blockPow)
    ws = 1<<blockPow;
    if ((blockPow > 30) OR (ws > n))
      return(Error(3, "peanoScan: Bad power argument\n"));
    for (ystart=1; ystart+ws-1<=ysize; ystart+=ws)
      FOR (xstart=1; xstart+ws-1<=xsize; xstart+=ws)
        x = xstart;
        y = ystart;
        right(blockPow);
      ENDFOR;
  ELSE
    for (i=1; n>>i; i++);
    n = 1<<(--i);
    if (oxsize*oysize < n*n)
      Warning(4, "peanoScan: Output band to small\n");
    x = 1; y=1;
    right(i);
  ENDIF;

  return(Iok);
}

#endif /* not MAIN */

/*P:peanoScan*

________________________________________________________________

		peanoScan
________________________________________________________________

Name:		peanoScan - scan an image in "peano" order

Syntax:		| peanoScan [-p <power>] [-x <outxsize>] [-y <outysize>] 
                |           <inimage> <outimage>

Description:    This program will transform an image into a one 
                dimensional signal while conserving the neighborhood
		properties. This is done by using the space filling
		curve called 'Peano Curve'. An input image will be
		scanned according to the Peano curve rules and the
		pixel values will be written in horizontal lines in
		an output image.

		This transformation is well suited for texture analysis
		and for compression, as it utilizes the correlation between
		neighboring pixels in both directions.

		If no '-p power' argument is supplied, scanning is
		continuous over one maximal square window of size some
		power of two.

Options:	&-p power
                Peano scanning over windows of size (2**power)*(2**power).
		Default: one large window.

		&-x outxsize
		Width of the output image. Default = 512.

		&-y outysize
		Height of the output image. Default: High enough to have
		room for the whole transformation.

Examples:       &Size of inimage: 128x128
		The whole band is transformed.

		&Size of inimage: 127x1024
		Only the upper left block of size 64x64 is transformed.

		&power=3
		The image is scanned in sub-windows of size 8x8.

		&&Size of inimage: 128x128
		The whole band is transformed in 16x16 separate blocks, each
		of size 8x8

		&&Size of inimage: 127x1024
		The upper, left block of size 120x1065 is transformed in
		15x128 separate blocks, each of size 8x8.

Restrictions:	Accepts only usigned byte images.

Reference: 	| 1) Joel Quinqueton and Marc Berthod
                |    "A Locally Adaptive Peano Scanning Algorithm"
		|    IEEE Transaction on Pattern Ana. and Machine Inte.
		|    Vol. PAMI-3, No. 4, July 1981
		| 2) P. T. Nguyen and J. Quinqueton
		|    "Space Filling Curves and Texture Analysis"
		|    IEEE Transaction on Pattern Ana. and Machine Inte.
		|    Vol. 2, 1982.

Known bugs:	One happy day there will be a "-i" switch for the inverse
                transform.

Author:		Yogesan and Lønnestad, BLAB, ifi, UiO

Examples:  	| peanoScan mona.img mona_peano.img
		| peanoScan -p 4 mona.img mona_peano.img
                | peanoScan -p 3 -x 128 mona.img mona_peano.img
		| peanoScan -x 64 -y 64 mona.img mona_peano.img 

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE inimg, peanoimg;
  int oxsize, oysize, power, bn;
  char *title, arg[100];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-p <power>] [-x <outxsize>] [-y <outysize>] \n\
              <inimage> <outimage> \n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);

  oxsize = read_iswitch(&argc, argv, "-x", 512);
  oysize = read_iswitch(&argc, argv, "-y", -1);
  power  = read_iswitch(&argc, argv, "-p", 0);
  title  = read_switch(&argc, argv, "-t", 1, "Peano Scan");
  title  = read_switch(&argc, argv, "-title", 1, title);

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  inimg = Iread_image(argv[1]);
  peanoimg = Init_image(Inbands(inimg), title);
  for (bn=1; bn <= Inbands(inimg); bn++) {
    peanoimg[bn] = Imake_band(Iu_byte_typ, oxsize,
      ((oysize == -1) ? ((Ixsize(inimg[bn])*Iysize(inimg[bn])+oxsize-1)/oxsize)
                      : oysize));
    peanoScan(inimg[bn], peanoimg[bn], power);
  }
    
  sprintf(arg, " input image: %s  power:%d", argv[1], power);
  Ihistory(peanoimg, argv[0], arg); 
  Iwrite_image(peanoimg, argv[2]);

  return(0);
}

#endif /* MAIN */
