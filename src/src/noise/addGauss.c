
/*

________________________________________________________________

        addGauss
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/noise.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/utils.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H

#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


/*F:addGauss*

________________________________________________________________

		addGauss
________________________________________________________________

Name:		addGauss - add gaussian random noise to a band

Syntax:         | #include <xite/noise.h>
		|
                | BiffStatus addGauss( IBAND b1, IBAND b2,
                |    double my, double sigma );
Description:    Adds random noise with Gaussian distribution to
                band b1, places the result in band b2. The
                distribution has mean value my and standard
                deviation sigma. If resulting value is larger
                than 255 output is set to 255, and if resulting
                value is less than 0 output is set to 0.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type

Author:		Tor L|nnestad, BLAB, IfI, UiO

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
BiffStatus addGauss(b1, b2, my, sigma)
  IBAND b1, b2;
  double my, sigma;
#else /* FUNCPROTO */
BiffStatus addGauss(IBAND b1, IBAND b2, double my, double sigma)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, tmp;

  if (Ipixtyp(b1) NE Iu_byte_typ)
    return(Error(1, "addGauss: Bad input pixel type\n"));
  if (Ipixtyp(b2) NE Iu_byte_typ)
    return(Error(2, "addGauss: Bad output pixel type\n"));
  xsize = MIN(Ixsize(b1),Ixsize(b2));
  ysize = MIN(Iysize(b1),Iysize(b2));
  my += 0.5; /* due to conversion to int later on */

  srandom(time(0)); /* random initialization of random generator */
  FOR (y=1; y LE ysize; INC y)
    FOR (x=1; x LE xsize; INC x)
      tmp = (int)((double)b1[y][x] + gaussRandom(my,sigma));
      if (tmp LT 0) b2[y][x] = 0;
      else if (tmp GT 255) b2[y][x] = 255;
      else b2[y][x] = (UNS_BYTE)tmp;
    ENDFOR;
  ENDFOR;
  return(0);
} /* int addGauss */

#endif /* not MAIN */



/*P:addGauss*

________________________________________________________________

		addGauss
________________________________________________________________

Name:		addGauss - add gaussian random noise to an image

Syntax:		addGauss <input image> <output image> [<my> [<sigma>]]

Description:    Adds random noise with gaussian distribution to
                an image. 
                | my - mean value of gaussian distribution
                | sigma - standard deviation of gaussian distribution
		Default values:
                | my = 0.0
		| sigma = 20.0

See also:	multGauss(1), mkGauss(1), addPoisson(1), mkPoisson(1)

Restrictions:   UNS_BYTE pixels are assumed.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:       addGauss mona.img monaGauss.img 0 25

Id:             $Id$
________________________________________________________________

*/


#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE i1, i2;
  double my, sigma;
  int bn, stat;
  char arg[50];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<my> [<sigma>]]\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);
  if (argc == 1) Usage(1, NULL);
  if ((argc < 3) OR (argc GT 5)) Usage(2, "Bad number of arguments\n");
  
  if (argc GE 4) my = atof(argv[3]); else my = 0.0;
  if (argc GE 5) sigma = atof(argv[4]); else sigma = 20.0;

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);

  for (bn = 1; bn LE Inbands(i1); bn++)
    if ((stat = addGauss(i1[bn], i2[bn], my, sigma)))
      Warning(stat, "Error in band %d\n", bn);

  sprintf(arg,"  my = %.3f  sigma = %.3f\n", my, sigma);
  Ihistory(i2, argv[0], arg);
  Iwrite_image(i2, argv[2]);
  return(0);
}

#endif /* MAIN */
