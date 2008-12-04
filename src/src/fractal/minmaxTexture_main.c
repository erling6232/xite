

/*

________________________________________________________________

        minmaxTexture
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


/*P:minmaxTexture*

________________________________________________________________

		minmaxTexture
________________________________________________________________

Name:		minmaxTexture - Min-Max filters for texture measurement

Syntax:		minmaxTexture [-t <title>] <inimage> <outimage>
                    <texdim_1> .. <texdim_n> 

Description:    'minmaxTexture' uses multi-scale MIN-MAX operators in order
                to characterize the local texure on different geometric
		scales ('texdim_1',...., 'texdim_n').
		'minmaxTexture' creates an image where the number of
		bands equals the number of window sizes 
		('texdim_1',..., 'texdim_n') used. These bands can be used to 
		find a signature for the input image.

		In band no. 1, we identify all structures less than 'texdim1'
		using window ('texdim_1' x 'texdim_1'). In the same way, using
		window ('texdim_n' x 'texdim_n'), all structures less 
		than 'texdim_n' have been identified from the input image 
		and shown in band no. n.

	        'texdim_n' (where n=3,5,..) are window sizes used to
		characterize the local texture on different geometric scales
		'texdim_1' < 'texdim_2' <...< 'texdim_n'.

Options:        &-t title
		Specify a 'title' for the output image 

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

References: 	&F. Albregtsen,
                "Fractal Texture Signature Estimated by Multiscale 
		LIT-SNN and MIN-MAX Operators on LANDSAT-5 MSS Images
		of the Antarctic"
		Proceedings, 6SCIA, Oulu, Finland, 
		June 1989, pp. 995-1002.

		&K. Yogesan,
		"Detection of Malignant Cells by Texture Analysis"
		Thesis, Image Processing Laboratory,
		University of Oslo. 
		Report No. 47, Nov. 1990. 

Example:	minmaxTexture mona.img monaMM.img 5 9 15

Author:		Otto Milvang/Yogesan

Id: 		$Id$
________________________________________________________________

*/


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/fractal.h>
#include <xite/blab.h>
#include <xite/arithmetic.h>
#include <xite/histo.h>
#include <xite/minmax.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STRING_H
#include XITE_UNISTD_H

int main(int argc, char **argv)
{
  IMAGE img2,img3,img;
  IBAND b1, b2, b3;
  int i, xsize, ysize, area;
  char *args;
  char* title;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
	      "Usage: %s <inimage> <outimage> <texdim1> .. <texdimn>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  title = read_switch(&argc, argv, "-t", TRUE, "MinmaxTexture");
  if (argc < 4) Usage(1, "Illegal number of arguments.\n");

  img  = Iread_image(argv[1]);
  xsize=Ixsize(img[1]);
  ysize=Iysize(img[1]);
  img2 = Imake_image(argc-3, title, 3, xsize, ysize);
  img3 = Imake_image(3, title, 3, xsize, ysize);
  b1 = img3[1];
  b2 = img3[2];
  b3 = img3[3];

  for (i=4; i<=argc; i++) {
    area = atoi(argv[i-1]);
    minarea(img[1], b1, area, area);
    maxarea(b1, b2, area, area);
    maxarea(img[1], b1, area, area);
    minarea(b1, b3, area, area);
    absDiff(b2, b3,img2[i-3]);
    histoEq(img2[i-3],img2[i-3],256);
    if (i != argc) {
      maxarea(b2, b1, area, area);
      minarea(b1, img[1], area, area);
    }
  }
  
  Ihistory(img, argv[0], args);
  Iwrite_image(img2,argv[2]);

  return(0);
}
