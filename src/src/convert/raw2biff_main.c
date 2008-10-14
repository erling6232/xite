/*

________________________________________________________________

        raw2biff
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_SYS_FILE_H
#  include <sys/file.h>
#endif
#ifdef HAVE_MALLOC_H
#  include <malloc.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif

#define MAX_TITLE_LENGTH 32


/*P:raw2biff*

________________________________________________________________

		raw2biff
________________________________________________________________

Name:		raw2biff - convert raw data to BIFF file

Syntax:		| raw2biff [-t title] [-pt pixel_type] [-org orgtype] \\
                |          [-swap] [-ih <ih>] [-bh <bh>] [-ba <ba>] \\
                |          [-lh <lh>] [-la <la>] \\
                |          <raw-file> <BIFF-file> <xsize> <ysize> [<bands>]

Description:	Converts a raw data file, 'raw-file' into a BIFF output file,
                'BIFF-file'. 'xsize' and 'ysize' give the horizontal and
		vertical size (number of pixels) of the image. 'bands' denotes
		the number of bands/frames in the input image (default 1).

Options:	&-t title
                Set the output image title to 'title'.

                &-pt pixel_type
		One of
		  &&ub
		  Unsigned byte, default.
		  &&sb
		  Signed byte, 8 bits.
		  &&us
		  Unsigned short, 16 bits.
		  &&ss
		  Signed short, 16 bits.
		  &&i
		  Signed integer, 32 bits.
		  &&r
		  Real (float), 32 bits.
		  &&bl
		  Bit - least significant bit first.
		  &&bm
		  Bit - most  significant bit first.

		&-org orgtype
		How the pixels are organized, one of
		  &&bls
		  Band-line-sample (default).
		  &lbs
		  Line-band-sample.
		  &lsb
		  Line-sample-band

		&-swap
		Swap byte order.

                &-ih ih
		Number of bytes to skip for image header. Default 0.

		&-bh bh
		Number of bytes to skip for band header. Default 0.

		&-ba ba
		Number of bytes to skip "after" each band for alignment.
		Default 0.

		&-lh lh
		Number of bytes to skip for line header. Default 0.

		&-la la
		Number of bytes to skip "after" each line for alignment.
		Default 0.

		-&
                The input file must thus follow this format:

		| 1)  <ih> bytes in image header
		| 2)    <bh> bytes in band header
                | 3)      <lh> bytes in line header
                | 4)      <xsize> pixels in 1. line
		| 5)      <la> bytes for line alignment
		| 6-8)    like 3 to 5 for line 2
                | 9-?)    and so on for each line up to <ysize>
                | ??)   <ba> bytes for band alignment
                | ???)  like 2 to ?? for band 2
                | ????) and so on for every band up to <bands>

		After all pixel data have been read, the input file is closed,
		the last remaining alignment bytes are not required.

Restrictions:	If more than one band, all bands must have the same size and
		pixel type.
                
Bugs:           When 16 or 32 bits raw files are given, 'raw2biff' assumes the 
                input file is from a machine with same bytesex/byteordering as
		the one running 'raw2biff'. Use option '-swap' to change the
		byteordering.

Author:		Tor Lønnestad, BLAB, Ifi, UiO
                
Id:             $Id$

Examples:	raw2biff -ih 512 any.dat mona.img 512 512
________________________________________________________________

*/


static void skip(FILE *fd, int nbytes)
{
  char buf[512];
  while (nbytes GE 512) { fread(buf, 512, 1, fd); nbytes -= 512; }
  if (nbytes) fread( buf, nbytes, 1, fd);
}

static void org_bit_bl(IMAGE i, FILE *fd, int nbands, int ysize, int xsize, int bh, int ba, int lh, int la)
{
  int x, y, bn, xa, xb;
  unsigned char *buffer;
  la = la + (xsize+7)/8;
  buffer = (unsigned char *) malloc(la);
  FOR (bn = 1; bn LE nbands; INC bn)
    skip(fd, bh);
    FOR (y=1; y LE ysize; INC y)
      skip(fd, lh);
      /* first element is sizeof(pix_type)=pix_size bytes offset, not 0 */
      IF (fread(buffer, 1, la, fd) NE la)
        Warning(5, "Data stop at band %d, line %d\n", bn, y);
        y = ysize;
        bn = nbands;
      ENDIF;
      FOR(x=0; x<xsize; x++)
        xa = x BAND 7;
        xb = x / 8;
        i[bn][y][x+1] = (buffer[xb] >> xa) BAND 1 ? 255 : 0;
      ENDFOR
    ENDFOR;
    if (bn LT nbands) skip(fd, ba);
  ENDFOR;
}

static void org_bit_bm(IMAGE i, FILE *fd, int nbands, int ysize, int xsize, int bh, int ba, int lh, int la)
{
  int x, y, bn, xa, xb;
  unsigned char *buffer;
  la = la + (xsize+7)/8;
  buffer = (unsigned char *) malloc(la);
  FOR (bn = 1; bn LE nbands; INC bn)
    skip(fd, bh);
    FOR (y=1; y LE ysize; INC y)
      skip(fd, lh);
      /* first element is sizeof(pix_type)=pix_size bytes offset, not 0 */
      IF (fread(buffer, 1, la, fd) NE la)
        Warning(5, "Data stop at band %d, line %d\n", bn, y);
        y = ysize;
        bn = nbands;
      ENDIF;
      FOR(x=0; x<xsize; x++)
        xa = x BAND 7;
        xb = x / 8;
        i[bn][y][x+1] = (buffer[xb] << xa) BAND 0x80 ? 255 : 0;
      ENDFOR
    ENDFOR;
    if (bn LT nbands) skip(fd, ba);
  ENDFOR;
}



static void org_bls(IMAGE i, FILE *fd, int nbands, int ysize, int xsize, int pix_size, int bh, int ba, int lh, int la)
{
  int y, bn;
  FOR (bn = 1; bn LE nbands; INC bn)
    skip(fd, bh);
    FOR (y=1; y LE ysize; INC y)
      skip(fd, lh);
      /* first element is sizeof(pix_type)=pix_size bytes offset, not 0 */
      IF (fread(&(i[bn][y][pix_size]),pix_size, xsize, fd) NE xsize)
        Warning(5, "Data stop at band %d, line %d\n", bn, y);
        y = ysize;
        bn = nbands;
      ENDIF;
      if ((y LT ysize) OR (bn LT nbands)) skip(fd, la);
    ENDFOR;
    if (bn LT nbands) skip(fd, ba);
  ENDFOR;
}

static void org_lbs(IMAGE i, FILE *fd, int nbands, int ysize, int xsize, int pix_size, int bh, int ba, int lh, int la)
{
  int y, bn; 
  FOR (y=1; y LE ysize; INC y)
    skip(fd, lh);
    FOR (bn = 1; bn LE nbands; INC bn)
    skip(fd, bh);
        /* first element is sizeof(pix_type)=pix_size bytes offset, not 0 */
      IF (fread( &(i[bn][y][pix_size]), pix_size, xsize, fd) NE xsize)
        Warning(5, "Data stop at band %d, line %d\n", bn, y);
        y = ysize;
        bn = nbands;
      ENDIF;
      if ((y LT ysize) OR (bn LT nbands)) skip(fd, ba);
    ENDFOR;
    if ((y LT ysize)) skip(fd, la);
  ENDFOR;
}

static void org_lsb(IMAGE i, FILE *fd, int nbands, int ysize, int xsize, int pix_size, int bh, int ba, int lh, int la)
{
  int x, y, bn;
  FOR (y=1; y LE ysize; INC y)
    skip(fd, lh);
    /* first element is sizeof(pix_type)=pix_size bytes offset, not 0 */
    FOR (x=1; x<=xsize; x++)
      FOR (bn=1; bn<= nbands; bn++)
        skip(fd, bh);
        IF (fread(&(i[bn][y][pix_size*x]), pix_size, 1, fd) NE pix_size)
          Warning(5, "Data stop at band %d, line %d\n", bn, y);
          x = xsize;
          y = ysize;
          bn = nbands;
        ENDIF;
        skip(fd, ba);
      ENDFOR;
    ENDFOR
    if ((y LT ysize)) skip(fd, la);
  ENDFOR;
}

int main(int argc, char **argv)
{

   IMAGE i;
   FILE *fd;
   int  ih, bh, ba, lh, la, xsize, ysize, nbands, bn, 
       pix_size, pix_type, org, swap;
   char *title, *pt, *ot, *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-t title] [-pt pixel_type] [-org <org_type>] [-swap] \n\
         [-ih <image header bytes>] \n\
         [-bh <band header bytes>] [-ba <band alignment bytes>] \n\
         [-lh <line header bytes>] [-la <line alignment bytes>] \n\
         <raw-file> <BIFF-file> <xsize> <ysize> [<bands>]\n"));
   Iset_abort(TRUE);
   Iset_message(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   title = read_switch(&argc, argv, "-t", 1, "Converted from raw data");
   title = read_switch(&argc, argv, "-title", 1, title);
   pt    = read_switch(&argc, argv, "-pt", 1, "ub");
   ot    = read_switch(&argc, argv, "-org", 1, "bls");
   ih    = read_iswitch(&argc, argv, "-ih", 0);
   bh    = read_iswitch(&argc, argv, "-bh", 0);
   ba    = read_iswitch(&argc, argv, "-ba", 0);
   lh    = read_iswitch(&argc, argv, "-lh", 0);
   la    = read_iswitch(&argc, argv, "-la", 0);
   swap  = read_bswitch(&argc, argv, "-swap");
   if (argc LT 5) Usage(2, "Illegal number of arguments.\n");

   fd = fopen(argv[1], "r");
   if (fd == NULL) 
     exit(Error(3, "Couldn't open input file %s \n", argv[1]));

   /* Parse pixeltype */
   pix_type = IparsePixtyp(pt);
   if (pix_type < Iu_byte_typ || pix_type > Ireal_typ) {
     if (! strcmp("bl", pt)) {
       pix_type = Iu_byte_typ;
       pix_size = -1;
     } else if (! strcmp("bm", pt)) {
       pix_type = Iu_byte_typ;
       pix_size = -2;
     } else exit(Error(6, "Unknown pixeltype %s \n", pt));
   } else pix_size = Ipixsize(pix_type) / 8; 

   /* Parse oranization */
   if (NOT strcmp("bls", ot))      org = 0;
   else if (NOT strcmp("lbs", ot)) org = 1;
   else if (NOT strcmp("lsb", ot)) org = 2;
   else exit(Error(4, "Unknown organization %s.\n", ot));

   xsize = atoi(argv[3]);
   ysize = atoi(argv[4]);
   if (argc GE 6) nbands = atoi(argv[5]); else nbands = 1;

   i = Imake_image(nbands, title, pix_type, xsize, ysize);

   skip(fd, ih);

   if (pix_size > 0) {
     switch (org) {
     case 0:
       org_bls(i, fd, nbands, ysize, xsize, pix_size, bh, ba, lh, la); 
       break;
     case 1:
       org_lbs(i, fd, nbands, ysize, xsize, pix_size, bh, ba, lh, la); 
       break;
     case 2:
       org_lsb(i, fd, nbands, ysize, xsize, pix_size, bh, ba, lh, la); 
       break;
     }
   } else {
     switch (pix_size) {
     case -1:
       org_bit_bl(i, fd, nbands, ysize, xsize, bh, ba, lh, la);
       break;
     case -2: 
       org_bit_bm(i, fd, nbands, ysize, xsize, bh, ba, lh, la);
       break;
     }
   }

   fclose(fd);
   Ihistory(i, argv[0], args);
   if (swap) for(bn=1; bn<=nbands; bn++) biff_swap(i[bn]);
   Iwrite_image(i, argv[2]);

   return(0);
}
