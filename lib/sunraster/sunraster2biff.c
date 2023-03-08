

/*

________________________________________________________________

        sunraster2biff.c
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
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/utils.h>
#include XITE_TYPES_H
#include XITE_FILE_H
#include XITE_FCNTL_H
#include XITE_MALLOC_H
#include "rast.h"

#define ERR_NONE   0
#define ERR_BIFF   1
#define ERR_MALLOC 2
#define ERR_OPEN   3
#define ERR_READ   4
#define ERR_MAGIC  5
#define ERR_FMT    6
#define ERR_WRITE  7
#define ERR_ARGV   8



/*L*

________________________________________________________________

		uncompress
________________________________________________________________

Name:		uncompress
Syntax:		
Description:
Return value:
Author:		Otto Milvang
Revised:        Svein Bøe and Olivier Barette <ob@info.ucl.ac.be>
________________________________________________________________

*/

static void uncompress(IBAND band, unsigned char *bit_data, int depth)
{
  unsigned char *byte_data;
  register int count, w;
  register unsigned char pix;
  int xsize, ysize, y, skip;

  xsize = Ixsize(band);
  ysize = Iysize(band);

  if (depth == 1) { 
    /* One-bit pixels. */
    
    if (xsize % 16) {
      if (xsize % 16 >= 8) skip = 0;
      else skip = 1;
    } else skip = 0;

  } else {
    /* Eight-bit pseudocolor or 24/32-bit BGR/XBGR/RGB/XRGB pixels.
     * In the multi-byte composite color case, this routine will be called
     * once for each band.
     */
    skip = xsize % 2;
  }
  
  for(y=1; y<= ysize; y++) {
    byte_data = &band[y][1];
    w = xsize;
    if (depth == 1) {
      /* One-bit pixels. */
      
      count = 8; /* Size of *bit_data, number of pixels to collect. */
      pix   = *bit_data++;
      
      /* Get one row of b/w image. */
      while (w--) { 
	/* Black or white? Test the bit under question (MSB). */
	*byte_data++ =  (pix & 0x80) ? 0 : 0xff;
	
	pix = pix + pix; /* Shift left to get next bit into MSB position. */
	if ((--count == 0) && w) {
	  /* Prepare for next 8 bits. */
	  count = 8;
	  pix   = *bit_data++;
	}
      }
    } else {
      /* Eight-bit pseudocolor pixels or multi-byte composite color. */
      
      /* Get one row of pseudocolor image or one band-row of composite-color
       * image.
       */
      while (w--) {
	*byte_data++ = *bit_data++;

	/* For composite colors, skip over the other two or three colors
	 * (BGR, ...) or (XBGR, ...) or (RGB, ...) or (XRGB, ...)
	 */

	bit_data += (depth - 1) / 8;
      }
    } /* else */
    
    bit_data += skip;

  } /* for y */
  
} /* uncompress() */

static int _Iread_(int fd, unsigned char *ptr, int size)
{
  int blokk, total = 0;

  for(;;) {
    blokk = read(fd, (char *) ptr, size);
    if (blokk <= 0) break;
    ptr = (unsigned char*) (ptr+blokk);
    size -= blokk;
    total += blokk;
    if(size == 0) break;
  }
  return(total);
}



/*F:read_sunraster*

________________________________________________________________

		read_sunraster
________________________________________________________________

Name:		read_sunraster - read Sun rasterfile to BIFF image

Syntax:         | #include <xite/sunraster.h>
                | #include <xite/color.h>
		| 
                | int read_sunraster( IMAGE* img, Color_tab tab,
                |    char* filename, int* depth );

Description:	Read a Sun rasterfile into a BIFF image (and colortab).
                'filename' is the name of a Sun rasterfile, 'img' will
		return a pointer to a new BIFF image, and 'tab' will
	        return the colormap. 'tab' may be NULL.
		'depth' will return the depth of the raster image.

		Accepts the standard XITE filenames "", "-", "-0" for
		'filename'.

Restriction:	Accepts the following types of Sun rasterfiles: 'standard',
                'old' and 'rgb'. For 24- and 32-bit rasterfiles (i.e. composite
		color), the 'standard' type means that the color order is
		BGR or XBGR (the new Sun color ordering), whereas the 'rgb'
		type is used for color order RGB and XRGB.

Return value:	| 0: Ok
                | 1: BIFF error
		| 2: Memory allocation error
		| 3: Sun rasterfile open error 
		| 4: Read error on Sun rasterfile
		| 5: Bad magic number (not a Sun rasterfile)
		| 6: Rasterfile format not supported

See also:       sunraster2biff(1), write_sunraster(3)

Author:		Otto Milvang
Revised:        Svein Bøe and Olivier Barette <ob@info.ucl.ac.be>
________________________________________________________________

*/

int read_sunraster(IMAGE *img, Color_cell *tab, char *filename, int *depth)
{
  struct rasterfile ras;
  int fd;
  int len, i, num_colors, opened = 0;
  char *col, *bit;
  IBAND b;
  
  col        = NULL;
  fd         = xite_pipe_filedes(filename, 1);
  if (fd == -1) {
    filename = tilde_expand(filename);

    fd = open(filename, O_RDONLY | FNDELAY, 0);
    if (fd == -1) return(ERR_OPEN);
    opened = 1;
  }
  
  len = _Iread_(fd, (unsigned char *) &ras, sizeof(struct rasterfile));
  if (len != sizeof(struct rasterfile)) return(ERR_READ);
  if (ras.ras_magic != RAS_MAGIC) return(ERR_MAGIC);

  if (ras.ras_type != RT_OLD      &&
      ras.ras_type != RT_STANDARD &&
      ras.ras_type != RT_FORMAT_RGB)
    return(ERR_FMT);

  if (ras.ras_maplength) {
    /* Read colormap from file. */
    col = malloc( (unsigned) ras.ras_maplength);
    if (col == NULL) return(ERR_MALLOC);
    len  = _Iread_(fd, (unsigned char *) col, ras.ras_maplength);
    if (len != ras.ras_maplength) return(ERR_READ);
  }
  
  if (ras.ras_type == RT_OLD) {
    /* The ras_length field used to be a ras_encoding field with value zero.
     * Must compute the ras_length.
     */

    if (ras.ras_length == 0) {
      if (ras.ras_depth == 1) {
	/* Number of bytes per row. */
	ras.ras_length = ((ras.ras_width + 7) / 8);

	/* Each row is multiple of 16 bits (2 bytes). */
	ras.ras_length += (ras.ras_length % 2);

	/* Bytes for whole image. */
	ras.ras_length *= ras.ras_height;

      } else {
	/* Depth is 8, 24 or 32. */
	int bytes_per_pix = ras.ras_depth / 8;

	/* Each row (of each color, I guess) is multiple of 2 bytes. */
	ras.ras_length  = ras.ras_width + (ras.ras_width % 2);

	/* Row for all colors. */
	ras.ras_length *= bytes_per_pix;

	/* Complete raster. */
	ras.ras_length *= ras.ras_height;
      }
    } /* Compute ras_length */
  } /* RT_OLD */
  
  if (ras.ras_length) {
    /* Read raster from file. */
    bit =  malloc( (unsigned) ras.ras_length);
    if (bit == NULL) return(ERR_MALLOC);
    len  = _Iread_(fd, (unsigned char *) bit, ras.ras_length);
    if (len != ras.ras_length) return(ERR_READ);
  }

  if (opened) (void) close(fd);
  
  /* Create a 3-band image if Sun 24/32 bit BGR/RGB. */
  if (ras.ras_depth == 24 || ras.ras_depth == 32)
    *img = Imake_image(3, "Converted from Sun Raster", Iu_byte_typ, 
		       ras.ras_width, ras.ras_height );
  else
    *img = Imake_image(1, "Converted from Sun Raster", Iu_byte_typ, 
                       ras.ras_width, ras.ras_height );
  
  if (*img == NULL) return(ERR_BIFF);
  
  *depth = ras.ras_depth;

  if (*depth == 24 || *depth == 32) {
    /* Decode raster. */

    int offset = 0;

    if (*depth == 32) {
      b = (*img)[1];
      uncompress(b, (unsigned char *) bit, *depth);
      
      offset = 1;
    }

    if (ras.ras_type == RT_STANDARD) {
      /* New standard Sun color ordering is: BGR or XBGR. */

      b = (*img)[3]; /* Third BIFF band is blue. */
      uncompress(b, (unsigned char *) (bit + offset), *depth);

      b = (*img)[2];
      uncompress(b, (unsigned char *) (bit + offset + 1), *depth);

      b = (*img)[1]; /* First BIFF band is red. */
      uncompress(b, (unsigned char *) (bit + offset + 2), *depth);
    } else {
      /* Color ordering: RGB or XRGB. */

      b = (*img)[1]; /* First BIFF band is red. */
      uncompress(b, (unsigned char *) (bit + offset), *depth);

      b = (*img)[2];
      uncompress(b, (unsigned char *) (bit + offset + 1), *depth);  

      b = (*img)[3]; /* Third BIFF band is blue. */
      uncompress(b, (unsigned char *) (bit + offset + 2), *depth);
    }
  } else {
    /* Depth is 1 or 8. */

    b = (*img)[1];
    uncompress(b, (unsigned char *) bit, *depth);
  }

  /* A ras map is of any size from 0 to 256*3 */
  num_colors = ras.ras_maplength / 3;
  
  if (tab && (*depth == 8)) {
    for(i = 0; i < num_colors; i++) {
      tab[i].pixel = RgbPixel(i);
      tab[i].red   = col ? (col[i ] << 8   ) : (i << 8);
      tab[i].green = col ? (col[i+num_colors] << 8) : (i << 8);
      tab[i].blue  = col ? (col[i+num_colors*2] << 8) : (i << 8);
    }
    /* fill the other with 0 (unused anyway) */
    for(i = num_colors; i < 256; i++) {
      tab[i].pixel = RgbPixel(i);
      tab[i].red   = 0;
      tab[i].green = 0;
      tab[i].blue  = 0;
    }
  }
  return(ERR_NONE);

} /* read_sunraster() */
