

/*

________________________________________________________________

        crossSection
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_MALLOC_H
#include <xite/draw.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAX
# define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a, b) ((a) <= (b) ? (a) : (b))
#endif



/*P:crossSection*

________________________________________________________________

		crossSection
________________________________________________________________

Name:		crossSection - Find cross sections, row or column of image

Syntax:		crossSection [<option>...] <inimage> <outimage>

Description:    'crossSection' reads a biff image and finds a cross-section,
                row or column. The output is an image with a band for each
		band of	the input image. Each output band displays a curve
		for the	chosen cross-section, row or column. The values of the
		pixels on the curve, below or above the curve can be chosen. 

		All pixeltypes are accepted. The pixeltype of each output
		band is the same as for the corresponding input band, unless
		the option '-pt' is used. For bands of a complex type, real or
		double bands are returned with absolute values of the input
		bands.

		To process only some of the bands in 'inimage', use the
		standard XITE sub-band notation.

Options:	An <option> is one of three kinds: <type>
		(at most one), <dir> (at most one) and <opt>.

		A <type> option may be one of

		&-M
		cross-section maximum (default)

		&-m
		cross-section minimum

		&-n num
		pick row or column number 'num'


		-&A <dir> option may be one of

		&-y
		y (vertical/column) direction (default)

		&-x
		x (horizontal/row) direction

		-&An <opt> option is chosen from

		&-t title
		Title of output image

		&-h height
		Height (ysize) of resulting band

		&-fg fg
		Value of area below curve (double)

		&-bg bg
		Value of area above curve (double)

		&-cur cur
		Value of curve pixels (double)

		&-s scale
		Scale curve height relative to image height (default is 1.0,
		which means that the curve peak will touch the top edge of the
		image)

		&-a
		Print curve values to stdout

		&-pt pixeltype
		Let all output bands be of type 'pixeltype'. Default unsigned
		byte.
                | ub or  3 => unsigned byte
                | sb or  4 => signed byte
                | us or  5 => unsigned short
                | ss or  6 => signed short
                | i  or  7 => integer
                | r  or  8 => real
                | d  or 10 => double

See also:       profile(1), crossSection(3)

Return value:   2 => Illegal arguments.

Examples:       Find the row maxima (maximum for each row) of an image:

                | crossSection -x -M /local/xite/img/mona.img out.img

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int mini, maxi, xdir, ydir, status, x, num, height, b, ascOut;
  int fieldWidth, nItems;
  long size;
  IMAGE img, out_img;
  IBAND inband, outband;
  double *vector, minPeak, maxPeak, fg, bg, cur, scale;
  char *outFile, *title, *options, *printFormat, fieldType[2], *pta;
  IPIXTYP pt;

  Iset_message(1);
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> <outimage>\n\
       where <option> is chosen from\n\
         -M          : Cross-section maximum (default) \n\
	 -m          : Cross-section minimum\n\
         -n <num>    : Pick row or column number <num> \n\
         -y          : y (vertical/column) direction (default) \n\
	 -x          : x (horizontal/row) direction.\n\
         -t <title>  : Title of output image\n\
	 -h <height> : Height (ysize) of resulting band\n\
	 -fg <fg>    : Value of area below curve (double)\n\
	 -bg <bg>    : Value of area above curve (double)\n\
	 -cur <cur>  : Value of curve pixels (double)\n\
         -s <scale>  : Scale curve height relative to image height\n\
         -a          : Print curve values to stdout\n\
         -pt <pixtyp>: Pixeltype of all output bands\n"));

  /* Concatenize options in *argv, for use by Ihistory. */

  if (argc == 1) Usage(1, NULL);
  options = argvOptions(argc, argv);
  
  if (argc < 3) Usage(2, "Illegal number of arguments.\n");

  mini   = read_bswitch(&argc, argv, "-m");
  maxi   = read_bswitch(&argc, argv, "-M");
  num    = read_iswitch(&argc, argv, "-n", -1);
  title  = read_switch (&argc, argv, "-t", 1, "");
  xdir   = read_bswitch(&argc, argv, "-x");
  ydir   = read_bswitch(&argc, argv, "-y");
  height = read_iswitch(&argc, argv, "-h", 512);
  fg     = read_dswitch(&argc, argv, "-fg", 255.0);
  bg     = read_dswitch(&argc, argv, "-bg", 0.0);
  cur    = read_dswitch(&argc, argv, "-cur", 255.0);
  scale  = read_dswitch(&argc, argv, "-s", 1.0);
  ascOut = read_bswitch(&argc, argv, "-a");
  pta    = read_switch( &argc, argv, "-pt", 1, "ub");

  pt = IparsePixtyp(pta);
  if (pt < Iu_byte_typ || pt == Icomplex_typ || pt >= Id_complex_typ)
    Usage(2, "Illegal pixeltype.\n");

  if (scale > 1.0 || scale < 0.0)
    Usage(2, "Scale must be between 0.0 and 1.0 inclusive.\n");

  if ( (mini && maxi) || (mini && (num != -1)) || (maxi && (num != -1)) )
    Usage(2, "Conflicting options");
  if (xdir && ydir) Usage(2, "Conflicting options");
  if (!(mini || maxi || (num != -1))) maxi = 1;
  if (!(xdir || ydir)) ydir = 1;
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  outFile = argv[2];
  if (strlen(title) == 0) {
    title = (char *) malloc(sizeof(char) * 81);
    title[0] = '\0';
    if (mini && xdir)
      strcat(title, "Row minima");
    else if (mini)
      strcat(title, "Column minima");
    else if (maxi && xdir)
      strcat(title, "Row maxima");
    else if (maxi)
      strcat(title, "Column maxima");
    else if (xdir) {
      strcat(title, "Row number ");
      sprintf(title + strlen(title), "%d", num);
    } else {
      strcat(title, "Column number ");
      sprintf(title + strlen(title), "%d", num);
    }
  } /* strlen == 0 */
  
  out_img = Init_image(Inbands(img), title);

  for (b=1; b<=Inbands(img); b++) {

    /* Extract information from one band. */

    inband  = img[b];

    if (mini && xdir)
      status = rowMinArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (mini)
      status = colMinArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (maxi && xdir)
      status = rowMaxArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (maxi)
      status = colMaxArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (xdir)
      status = rowBand(inband, num, &vector, &size, &minPeak, &maxPeak);
    else status = columnBand(inband, num, &vector, &size, &minPeak, &maxPeak);
    
    if (status == 1) {
      Error(2, "Illegal band number %d.\n", b);
    }
    else if (status == 2) {
      Error(2, "Illegal row or column number %d.\n", num);
    }

    if (ascOut) printFormat = malloc(80);

    /* Make output band for result. */

    if (pt != Iunknown_typ)
      outband = Imake_band(pt, size, height);
    else if (Ipixtyp(inband) == Icomplex_typ ||
	     Ipixtyp(inband) == Id_complex_typ)
      outband = Imake_band(Idouble_typ, size, height);
    else outband = Imake_band(Ipixtyp(inband), size, height);

    out_img[b] = outband;
    curveToBand(outband, vector, minPeak, maxPeak, scale, cur, fg, bg);

    if (ascOut) {
      /* Format control for printing. */
      switch( (int) Ipixtyp(inband)) {
      case Ibit_typ:
      case Ibit2_typ:
      case Inibble_typ:
      case Iu_byte_typ:
      case Is_byte_typ:
      case Iu_short_typ:
      case Is_short_typ:
      case Integer_typ: {
	int maxi;

	(void) strcpy(fieldType, "d");

	maxi = MAX(abs(minPeak), abs(maxPeak));
	if (maxi == 0) fieldWidth = 2;
	else {
	  fieldWidth = (int) log10((double) maxi) + 2;
	}
      }
	break;
      default:
	(void) strcpy(fieldType, "g");
	fieldWidth = 15;
	break;
      } /* switch */
      
      sprintf(printFormat, "%%%d%s", fieldWidth, fieldType);

      nItems = (int) 120 / (fieldWidth + 1);

      if (Ipixtyp(inband) <= Integer_typ) {
	
	for (x=1; x <= size; x++) {
	  printf(printFormat, (int) vector[x-1]);
	  if (x % (nItems - 1) == 0) printf("\n");
	}
      } else {
	for (x=1; x <= size; x++) {
	  printf(printFormat, vector[x-1]);
	  if (x % (nItems - 1) == 0) printf("\n");
	}
      }
      if ((x - 1) % (nItems - 1) != 0) printf("\n");
    }
  } /* for b */
  
  Icopy_text(img, out_img);
  Ihistory(out_img, argv[0], options);
  Iwrite_image(out_img, outFile);
  
  return(0);
}
