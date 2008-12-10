
/*

________________________________________________________________

        haarTexture
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
#include <xite/haar.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>


#define ABS(v)    (((v) GT 0.0) ? (v) : -(v))
#define WL        32 /* word length */



#ifndef MAIN

/*F:haarTexture*

________________________________________________________________

		haarTexture
________________________________________________________________

Name:		haarTexture - texture extraction from 2D Haar transform

Syntax:         | #include <xite/haar.h>
		|
                | int haarTexture( IR_BAND input, IR_BAND output,
                |    int xp, int yp );

Description:    Extracts texture information from the input band,
                returns it through the output band. The input is
		assumed to be the result of a forward 2D haar
		transform, 'haar2d'.

		'xp' (and 'yp') specifies the kind of textural
		information, and may be varied independently between
		1 (high frequency information) and log2(XSIZE)+1
		(low frequency), where XSIZE is the horisontal size
		of the image. The horisontal size of the texture
		element will be 2^xp (2^yp) except for xp = log2(XSIZE)+1,
                which will use a texture element of size XSIZE.
		Correspondingly for yp/YSIZE/vertical. 

Return value:   | 0 => ok
		| 1 => bad pixel type input band
		| 2 => bad pixel type output band
		| 3 => bands have different sizes
		| 4 => sizes are not power of two
		| 5 => bad xp or yp arguments
		| 6 => malloc error

Restrictions:   Input and output band must be of type IR_BAND and have
		equal size, both horizontal and vertical size must be some
		power of two. The input band must be the result of a forward 2D
		Haar transform.

See also:       haar2d(3), haari2d(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/


int haarTexture(IR_BAND input, IR_BAND output, int xp, int yp)
{
  IR_BAND rb;
  int x1, y1,		/* input indexes */
      x2, y2,		/* output indexes */
      xsize, ysize,	/* band sizes */
      xlog, ylog,	/* log2 of band sizes */
      xsi, ysi,		/* size of subband to extract */
      xst, yst,		/* starting point of subband to extract */
      xrep, yrep,	/* magnification factors */
      rep;		/* loop control variable */
  float val;            /* pixel value */

  /* Veryfy that input and output have the same size; some power of two */
  if (Ipixtyp((IBAND) input)  NE Ireal_typ)
    return(Error(1, "Input pixel type must be real.\n"));
  if (Ipixtyp((IBAND) output) NE Ireal_typ)
    return(Error(2, "Output pixel type must be real.\n"));
  xsize = Ixsize((IBAND) input);
  ysize = Iysize((IBAND) input);
  if (xsize NE Ixsize((IBAND) output))
    return(Error(3, "Input and output have different horizontal size.\n"));
  if (ysize NE Iysize((IBAND) output))
    return(Error(3, "Input and output have different vertical size.\n"));

  for (x1=0; x1 LT WL; x1++)
    IF ((xsize>>x1) BAND 1) xlog=x1; x1=WL; ENDIF;
  for (y1=0; y1 LT WL; y1++)
    IF ((ysize>>y1) BAND 1) ylog=y1; y1=WL; ENDIF;
  if (xsize NE (1<<xlog))
    return(Error(4, "Horizontal size is not power of two.\n"));
  if (ysize NE (1<<ylog))
    return(Error(4, "Horizontal size is not power of two.\n"));
  if ((xp LT 1) OR (xp GT (xlog+1)))
    return(Error(5, "Textural parameter out of range.\n"));
  if ((yp LT 1) OR (yp GT (ylog+1)))
    return(Error(5, "Textural parameter out of range.\n"));

  /* calculate window size, startpoint and magnification factors */ 
  xsi = (xp EQ (xlog+1)) ? 1 : 1<<(xlog - xp);
  ysi = (yp EQ (ylog+1)) ? 1 : 1<<(ylog - yp);
  xrep = xsize/xsi;
  yrep = ysize/ysi;
  xst = (xp EQ (xlog+1)) ? 1 : (1<<(xlog - xp))+1;
  yst = (yp EQ (ylog+1)) ? 1 : (1<<(ylog - yp))+1;

  rb = (IR_BAND)Imake_subband((IBAND) input, xst, yst, xsi, ysi);
  if (NOT rb) return(Error(6, "Could not allocate memory for sub-band.\n"));

  y2 = 0;
  FOR (y1 = 1; y1 LE ysi; y1++)
    x2 = 1;
    y2++;
    FOR (x1 = 1; x1 LE xsi; x1++)
      val = rb[y1][x1];
      val = ABS(val);
      for (rep=xrep; rep; rep--) output[y2][x2++] = val;
    ENDFOR;
    FOR (rep=yrep-1; rep; rep--)
      y2++;
      for (x2=1; x2 LE xsize; x2++)
        output[y2][x2] = output[y2-1][x2];
    ENDFOR;
  ENDFOR;
  Idel_band((IBAND *) &rb);

  return(0);
}

#endif /* not MAIN */

/*P:haarTexture*

________________________________________________________________

		haarTexture
________________________________________________________________

Name:		haarTexture - texture extraction from 2D Haar transform

Syntax:		| haarTexture [-t <title>] <input> <output> <xp1> <yp1>
                |   [<xp2> <yp2>....[<xpn> <ypn>]..] 

Description:    Extracts texture information from the input image,
                returns it through the output image. The input is
		assumed to be the result of a forward 2D haar
		transform, 'haar2d'. Only the first band of the input
		image is used.

		'xp' (and 'yp') specifies the kind of textural
		information, and may be varied independently between
		1 (high frequency information) and log2(XSIZE)+1
		(low frequency), where XSIZE is the horisontal size
		of the image. The horisontal size of the texture
		element will be 2^xp except for xp = log2(XSIZE)+1,
                which will use a texture element of size XSIZE.
		Correspondingly for yp/YSIZE/vertical. 

		You can specify any number of 'xp', 'yp' pairs, the
		output image will have as many bands as such pairs.

See also:	haar2d(1)

Restrictions:   Input image must have bands of type IR_BAND, both horizontal
                and vertical size must be some power of two. The image must be
		the result of a forward 2D Haar transform.

		'xp' ('yp') must be between 1 and log2(XSIZE)+1
		(log2(YSIZE)+1).

Return value:   0 when ok

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       Extraction of four bands with high frequency
                texture information:

		| haarTexture monaHaar.img monaTexture.img \\
		|     1 1  1 2  2 1  2 2

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int xp, yp, ac, bn, stat;
  char* t, *args;

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-t <title>] <input> <output> \n\
      <xp1> <yp1> [<xp2> <yp2> ...[<xpn> <ypn>]..]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  t = read_switch(&argc, argv, "-t", TRUE, "Haar-Texture");

  if ((argc < 5) || (! (argc BAND 1)))
    Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  i2 = Init_image((argc-3)/2,t);
  ac = 3;
  for (bn=1; bn LE Inbands(i2); bn++) {
    i2[bn] = Imake_band(Ireal_typ, Ixsize(i1[1]), Iysize(i1[1]));
    xp = atoi(argv[ac++]);
    yp = atoi(argv[ac++]);
    if ( (stat = haarTexture((IR_BAND)i1[1], (IR_BAND)i2[bn], xp, yp)) )
      Error(stat, "Error %d in band %d\n", stat, bn);
  }

  Icopy_text(i1, i2);
  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}

#endif /* MAIN */
