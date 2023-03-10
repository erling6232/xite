

/*

________________________________________________________________

        biff2sunraster.c
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

#ifndef lint

static char *Id = "$Id$, Blab, UiO";

#endif



#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/sunraster.h>
#include XITE_STDIO_H
#include XITE_UNISTD_H
#include <xite/color.h>
#include <xite/readarg.h>
#include <xite/utils.h>
#include <xite/convert.h>
#include XITE_TYPES_H
#include XITE_FCNTL_H
#include XITE_FILE_H
#include XITE_MALLOC_H
#include "rast.h"

#define ERR_NONE   0
#define ERR_BIFF   1
#define ERR_OPEN   2
#define ERR_WRITE  3
#define ERR_BANDS  4
#define ERR_PIXTYP 5
#define ERR_MALLOC 6
#define ERR_DIM    7
#define ERR_DEPTH  8
#define ERR_ARGV   9



/*L*

________________________________________________________________

		compress
________________________________________________________________

Name:		compress
Syntax:		
Description:
Return value:
Author:		Otto Milvang
Revised:        Svein B?e
________________________________________________________________

*/

static void compress(IBAND band, unsigned char *bit_data, int depth, int threshold, int is_first_component)
{
  unsigned char *byte_data;
  register int count, w;
  register unsigned char pix;
  int xsize, ysize, y, skip = 0;

  xsize = Ixsize(band);
  ysize = Iysize(band);

  if (depth == 1) {
    if (xsize % 16 >= 8) {
      skip = 0;
    } else if (xsize % 16) {
      skip = 1;
    } else skip = 0;
  } else {
    skip = xsize % 2;
  }

  if (depth == 1 || depth == 8) is_first_component = 1;

  for(y=1; y<= ysize; y++) {
    byte_data = &band[y][1];
    w         = xsize;

    if (depth == 1) {
      /* One-bit pixels. */
      count = 8;
      pix   = 0;

      /* Encode one row of b/w image. */
      while (w--) { 
	pix = pix + pix; /* Shift left. */
	if(*byte_data++ <= threshold) pix++;

	if (--count == 0) {
	  *bit_data++ = pix; /* Save the compressed pixels in this byte. */

	  /* Prepare for next 8 bits. */
	  count       = 8;
	  pix         = 0;
	}
      } /* while in the same row */

      if (count < 8) {	
	pix  <<= count;      /* Move the compressed info to the edge. */
	*bit_data++ = pix;
      }
    } else {
      /* Copy one row of pseudocolor image or one band-row of composite-color
       * image.
       */

      while (w--) {
	*bit_data++ = *byte_data++;

        /* For composite colors, skip over the other two or three colors
         * (BGR, ...) or (XBGR, ...) or (RGB, ...) or (XRGB, ...)
         */

	bit_data += (depth - 1) / 8;
      }
    } /* Not binary image. */

    if (skip) {
      if (is_first_component) *bit_data = 0;
      *bit_data++;
    }

  } /* for each row */

} /* compress() */

static int _Iwrite_(int fd, unsigned char *ptr, int size)
{
  int blokk, total = 0;
  
  for(;;) {
    blokk = write(fd, (char *) ptr, size);
    if (blokk <= 0) break;
    ptr    = (unsigned char*) (ptr+blokk);
    size  -= blokk;
    total += blokk;
    if(size == 0) break;
  }
  return(total);
}



/*F:write_sunraster*

________________________________________________________________

		write_sunraster
________________________________________________________________

Name:		write_sunraster - write BIFF image to Sun rasterfile

Syntax:         | #include <xite/sunraster.h>
                | #include <xite/color.h>
		| 
                | int write_sunraster( IMAGE img, Color_tab tab,
                |    int len, char* filename, int depth,
                |    int threshold );

Description:	Write a BIFF image (and possibly a colortable) to a Sun
                rasterfile. 'filename' is the name of a Sun rasterfile,
                'img' is a BIFF image, and 'tab' is the colortable.

		'depth' is the depth of the raster image. If 'depth' equals 24
		or 'tab' is NULL, no colormap is written. Composite color
		images (24 bit) will have the new standard Sun color ordering
		of BGR.

		When 'depth' equals one, a bit in the Sun rasterfile will be
		set if the corresponding BIFF pixel value is <= 'threshold'.
		This applies only to one-band BIFF images.

		The resulting Sun rasterfile has type 'standard'.

		Accepts the standard XITE filenames "", "-" and "-1" for
		'filename'.

Restriction:	Accepts only one-band and three-band unsigned byte BIFF images.
                Depth must be 1 (binary image) or 8 (pseudocolor) for one-band
		BIFF images or 24 for three-band BIFF images (composite color).
		For three-band images, the width and height must be the same
		for all the bands.

Return value:	| 0: Ok
                | 1: Error in handling BIFF file
		| 2: Open error on Sun rasterfile
		| 3: Write error on Sunrasterfile
		| 4: BIFF file must have 1 or 3 band(s)
                | 5: Illegal BIFF pixel type
		| 6: Memory allocation error
		| 7: Image bands must have the same dimensions
		| 8: Illegal combination of depth and number of bands

See also:       biff2sunraster(1), read_sunraster(3)
Author:		Otto Milvang
Revised:        Svein B?e

Id: 		$Id$
________________________________________________________________

*/

int write_sunraster(IMAGE img, Color_cell *tab, int len, char *filename, int depth, int threshold)
{
  char *col;
  struct rasterfile ras;
  IBAND b;
  char *bit;
  int fd, i;
  int size, width, height, opened = 0;
  unsigned long p;
  char used[256];
  
  if (tab && depth == 8) {
    /* Write colortable for 8-bit pseudocolor image. */

    ras.ras_maptype   = RMT_EQUAL_RGB;
    ras.ras_maplength = 256 * 3;
    ras.ras_depth     = depth;
    col = (char *) malloc( (unsigned) ras.ras_maplength);
    if (col == NULL) return(ERR_MALLOC);
    
    for (i = 0; i < 256; i++) used[i] = 0;
    for (i = 0; i < len; i++) {
      p       = PseudoBasePart(tab[i].pixel);
      used[p] = 1;
      col[ p       ] = ((tab[i].red   >> 8) & 0x0ff);
      col[ p + 256 ] = ((tab[i].green >> 8) & 0x0ff);
      col[ p + 512 ] = ((tab[i].blue  >> 8) & 0x0ff);
    }
    for (i=0; i<256; i++) {
      if (!used[i]) {
	col[i]       = 0;
	col[i + 256] = 0;
	col[i + 512] = 0;
      }
    }
  } else {
    /* No colortable. */
    ras.ras_maptype   = RMT_NONE;
    ras.ras_maplength = 0;
    ras.ras_depth     = depth;
    col               = NULL;
  }
  
  if (img == NULL) return(ERR_BIFF);
  if (Inbands(img) != 1 && Inbands(img) != 3) return(ERR_BANDS);
  if ((Inbands(img) == 1 && depth != 1 && depth != 8) ||
      (Inbands(img) == 3 && depth != 24)) return(ERR_DEPTH);

  b = img[1];
  width = Ixsize(b); height = Iysize(b);
  for (i = 1; i <= Inbands(img); i++) {
    b = img[i];
    if (Ipixtyp(b) != Iu_byte_typ) return(ERR_PIXTYP);
    if (Ixsize(b) != width || Iysize(b) != height) return(ERR_DIM);
  }

  b = img[1];
  ras.ras_magic  = RAS_MAGIC;
  ras.ras_width  = width;
  ras.ras_height = height;
  ras.ras_type   = RT_STANDARD;
  
  if (depth == 1) {
    size = ((width + 7) / 8);
    size += size % 2;   /* Each row multiple of 16 bits. */
    size *= height;     /* Complete raster. */
    ras.ras_length = size;
    bit = malloc((unsigned) size);
    if (bit == NULL) return(ERR_MALLOC);
    compress(b, (unsigned char *) bit, depth, threshold, 1);
  } else {
    int bytes_per_pix = ras.ras_depth / 8;

    size  = width + (width % 2); /* One row, one color component. */
    size *= bytes_per_pix;       /* One row, all color components. */
    size *= height;              /* Complete raster. */
    ras.ras_length = size;

    bit = malloc((unsigned) size);
    if (bit == NULL) return(ERR_MALLOC);

    /* BGR color order. */
    if (depth == 24) {
      compress(img[3], (unsigned char *) bit,       depth, threshold, 1);
      compress(img[2], (unsigned char *) (bit + 1), depth, threshold, 0);
      compress(img[1], (unsigned char *) (bit + 2), depth, threshold, 0);
    } else compress(img[1], (unsigned char *) bit,  depth, threshold, 1);
  }
  
  fd = xite_pipe_filedes(filename, 0);
  if (fd == -1) {
    filename = tilde_expand(filename);

    unlink(filename);
    fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd == -1) return(ERR_OPEN);
    opened = 1;
  }

  len = _Iwrite_(fd, (unsigned char *) &ras, sizeof(struct rasterfile));
  if (len != sizeof(struct rasterfile)) return(ERR_WRITE);
  
  if (ras.ras_maplength) {
    len = _Iwrite_(fd, (unsigned char *) col, ras.ras_maplength);
    if (len != ras.ras_maplength) return(ERR_WRITE);
  }
  
  if (ras.ras_length) {
    len = _Iwrite_(fd, (unsigned char *) bit, ras.ras_length);
    if (len != ras.ras_length) return(ERR_WRITE);
  }

  if (opened) (void) close(fd);

  return(ERR_NONE);

} /* write_sunraster() */
