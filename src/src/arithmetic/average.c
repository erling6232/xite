

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



#ifndef MAIN

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


#ifndef FUNCPROTO
int average(inimg, outband, Scale)
IMAGE inimg;
ID_BAND outband;
double Scale;
#else /* FUNCPROTO */
int average(IMAGE inimg, ID_BAND outband, double Scale)
#endif /* FUNCPROTO */
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

#endif /* not MAIN */

/*P:average*

________________________________________________________________

		average
________________________________________________________________

Name:		average - computes the average of several bands in an 
                input image

Syntax:		average [<option>] <inimage> [<inimage>...] <outimage>

Description:	Computes the average image of two or several bands of one
                or more images. Grey levels of the output average image will be
                multiplied by a scale factor. 

		| (inimage[1]+inimage[2]+........+inimage[n])/n * scale

		The output image will be of pixeltype double.

		The size of the output image depends on the options '-max'
		and '-min' (see below)

Options:        &-scale factor
                Scale factor in calculation. Default: 1.0.

		&-max
		Size of the output image = N x M where N and M are
                | N=MAX(Ixsize(inimage[1]),...,Ixsize(inimage[n]))
		| M=MAX(Iysize(inimage[1]),...,Iysize(inimage[n]))
	        In this case, if the output image size is larger than
		an input image band, then zeros will be added to the rest of  
		the output image. 

		&-min
		Size of the output image = N x M where N and M are
                | N=MIN(Ixsize(inimage[1]),...,Ixsize(inimage[n]))
		| M=MIN(Iysize(inimage[1]),...,Iysize(inimage[n]))
                In this case, all the pixels having co-ordinates 
		(x,y), where x > N and y > M, will not be considered in 
		the calculation.

Restrictions:	Input bands should not have pixels of type "complex".

Return value:   | 0 => OK
                | 1 => Illegal number of arguments
		| 2 => Can't find input image
		| 3 => Only one band in input image
		| 4 => Error in calculation of average value

Author:		Yogesan, BLAB, Ifi, UiO

Examples:	| average <inimage> <outimage>
                | average <inimage1> <inimage2> <outimage>
                | average -max <inimage> <outimage>
		| average <inimage> <outimage> -scale 2.5
		| average -max -scale 2 <inimage> <outimage>

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  int nbands, bn, xsize, ysize, lxsize, lysize, stat, min, max, i, j;
  double scale;
  IMAGE inimg, img;
  ID_IMAGE outimg;
  char *args, *outfileName;

  Iset_message(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> [<inimage>...]<outimage>\n\
       where <option> is\n\
       -scale factor : Scale the average by this factor\n\
       -max          : Maximum size of output image\n\
       -min          : Minimum size of output image\n"));

  if (argc == 1) Usage(1, NULL);

  args  = argvOptions(argc, argv);

  scale =  read_dswitch(&argc, argv, "-scale", 1.0);
  max   =  read_bswitch(&argc, argv, "-max");
  min   =  read_bswitch(&argc, argv, "-min");

  if(argc < 3) Usage(1, "Illegal number of arguments.\n");

  outfileName = argv[argc - 1];

  inimg  = Init_image(1, "Average");
  nbands = 0;
  if (argc == 3) {
    /* Only one input image. */

    inimg = Iread_image(argv[1]);
    if (!inimg) Error(2, "Can't find image %s.\n", argv[1]);
    nbands = Inbands(inimg);
  } else {
    /* More than one input image. */

    /* Copy each band in each input image into a new band in the 'inimg'
       variable. */
    for (i = 0; i < argc - 2; i++) {
      img = Iread_image(argv[i + 1]);
      if (!img) Error(2, "Can't find image %s.\n", argv[i + 1]);
      
      for (j = 1; j <= Inbands(img); j++) {
	Iset_nbands(&inimg, ++nbands);
	inimg[nbands] = Imake_band(Ipixtyp(img[j]), Ixsize(img[j]),
				   Iysize(img[j]));
	if (Icopy_band(img[j], inimg[nbands]) != Iok)
	  Error(2, "Error in copying bands.\n");
      }
    }
  }

  lxsize = lysize = 0;
  if (nbands == 1) Warning(1, "There is only one band in the input.\n");

  if (max) {
    lxsize = lysize = 0;
    for (bn=1; bn <= nbands; bn++) {
      xsize = Ixsize(inimg[bn]);
      ysize = Iysize(inimg[bn]);
      if (xsize > lxsize) lxsize = xsize;
      if (ysize > lysize) lysize = ysize;
    }
  }
  else {
    lxsize = lysize = INTEGER_MAX;
    for (bn=1; bn <= nbands; bn++) {
      xsize = Ixsize(inimg[bn]);
      ysize = Iysize(inimg[bn]);
      if (xsize < lxsize) lxsize = xsize;
      if (ysize < lysize) lysize = ysize;
    }
  }

  outimg = (ID_IMAGE) Imake_image(1,"Average Image", Idouble_typ,
				  lxsize, lysize);

  if ((stat = average(inimg, outimg[1], scale)) != Iok)
    Error(4, "Error in function average.\n");

  Ihistory((IMAGE) outimg, argv[0], args);
  Iwrite_image((IMAGE) outimg, outfileName);
    
  return(0);
}

#endif /* MAIN */
