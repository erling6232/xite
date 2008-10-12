

/*C*

________________________________________________________________

        average
        $Id$
        Copyright 1991, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@uio.no
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
#include <xite/arithmetic.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H



/*F:average*

________________________________________________________________

		average
________________________________________________________________

Name:		average - computes the average of several bands in an 
                input image

Syntax:         | #include <xite/arithmetic.h>
		|
                | int average( IMAGE inimg, ID_BAND outband,
                |    int Scale );

Description:    Computes the average image of two or several bands of an
                image. Grey levels of the output average image will be
                multiplied by a Scale (default Scale is 1.0). 

		| (inimg[1] + inimg[2] +........+ inimg[n]) / n * Scale

		The output image will be of pixeltype double.

Restrictions:   Input image should not have pixels of type "complex".

Return value:   | 0 => OK
                | 1 => Unknown pixeltype of input band

Author:		Yogesan, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int average(IMAGE inimg, ID_BAND outband, double Scale)
{
  ISB_IMAGE sbb1;
  IUS_IMAGE usb1;
  ISS_IMAGE ssb1;
  II_IMAGE  ib1;
  IR_IMAGE  rb1;
  ID_IMAGE  db1;
  int x, y, xsize[300], ysize[300];
  int nbands, bn;
  IPIXTYP pixtyp;

  for (x=0; x<300; x++) xsize[x] = ysize[x] = 0;

  nbands   = Inbands(inimg);
  xsize[0] = Ixsize((IBAND) outband);
  ysize[0] = Iysize((IBAND) outband);

  for (bn=1; bn<=nbands; bn++) {
    xsize[bn] = Ixsize(inimg[bn]);
    ysize[bn] = Iysize(inimg[bn]);
  }

  if (Ipixtyp((IBAND) outband) != Idouble_typ)
    return(Error(2, "Output band must have pixel type double.\n"));
  
  for (bn = 1; bn <= nbands; bn++) {
    pixtyp = Ipixtyp(inimg[bn]);
    
    switch (pixtyp) {
    case(Iu_byte_typ): {
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] +=
	    (xsize[bn]>=x && ysize[bn]>=y) ? inimg[bn][y][x] : 0;
	}
    }
      break;
      
    case(Is_byte_typ): {
      sbb1 = (ISB_IMAGE)inimg;
      
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] +=
	    (xsize[bn]>=x && ysize[bn]>=y) ? sbb1[bn][y][x] : 0;
	}
    }
      break;
      
    case(Iu_short_typ): {
      usb1 = (IUS_IMAGE)inimg;
      
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] +=
	    (xsize[bn]>=x && ysize[bn]>=y) ? usb1[bn][y][x] : 0;
	}
    }
      break;
      
    case (Is_short_typ): {
      ssb1 = (ISS_IMAGE)inimg;
      
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] += 
	    (xsize[bn]>=x && ysize[bn]>=y) ? ssb1[bn][y][x] : 0;
	}
    }
      break;
      
    case (Integer_typ): {
      ib1 = (II_IMAGE)inimg;
      
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] +=
	    (xsize[bn]>=x && ysize[bn]>=y) ? ib1[bn][y][x] : 0;
	}
    }
      break;
      
    case (Ireal_typ): {
      rb1 = (IR_IMAGE)inimg;
      
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] +=
	    (xsize[bn]>=x && ysize[bn]>=y) ? rb1[bn][y][x] : 0;
	}
    }
      break;
      
    case (Idouble_typ): {
      db1 = (ID_IMAGE)inimg;
      
      for (y=1; y<=ysize[0]; y++)
	for (x=1; x<=xsize[0]; x++) {
	  outband[y][x] +=
	    (xsize[bn]>=x && ysize[bn]>=y) ? db1[bn][y][x] : 0;
	}
    }
      break;
      
    default:
      return(Error(1, "%s%s\n",
		   "Input bands can not have pixel type complex or double ",
		   "complex."));
      
    } /* switch */
  } /* for each band */
  
  if (scale((IBAND) outband, (IBAND) outband, Scale, 0.0) != Iok)
    return(Error(3, "Error in scale().\n"));

  return(0);

} /* average() */
