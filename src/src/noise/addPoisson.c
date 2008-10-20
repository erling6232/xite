
/*

________________________________________________________________

        addPoisson
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h> 
#include <xite/biff.h>
#include <xite/noise.h>
#include <xite/message.h>
#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_TIME_H
#  include <time.h>
#endif



#ifndef MAIN

/*F:addPoisson*

________________________________________________________________

		addPoisson
________________________________________________________________

Name:		addPoisson - add Poisson noise to the output band

Syntax:         | #include <xite/noise.h>
                |
                | int addPoisson( IBAND input, IBAND output );

Description:    addPoisson generates Poisson distributed noise in the
                'output' band. For each 'output' pixel, the corresponding
		pixelvalue in 'input' is used as the mean value in the
		distribution. If the resulting random value is larger than
		255, output is set to 255.

Restrictions:   Only unsigned byte pixeltype is accepted for 'input' and
                'output'.

Return value:   | 0 => ok
                | 1 => Bad pixel type
		 
Author:		Helene Schulerud, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________
 
*/

int addPoisson(IBAND input, IBAND output)
{
  int x, y, xsize, ysize;
  int n;
  long rl;
  double rl2,p,ek,xl;
 
  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(1, "addPoisson: Bad pixel type.\n"));

  xsize = Ixsize(input);
  ysize = Iysize(input);

  srandom(time(0)); /* random initialization of random generator */

  for ( y=1; y<=ysize; y++ )
    for ( x=1; x<=xsize; x++ )
      {
	n=0;
	p=1;
	rl=random();
	rl2=(double)rl/RANDOM_MAX;
	xl=(double)input[y][x];
	p=p*rl2;
        ek = (double)(exp(-xl));
	while ( p >= ek )
	  {
	    n++;
	    rl=random();
	    rl2=(double)rl/RANDOM_MAX;
	    xl=(double)input[y][x];
	    p=p*rl2;
	    ek = (exp(-xl));
	  }
	
	if (n<=255)
	  output[y][x] = n ;
	else
	  output[y][x] = 255; 

      }
 
  return(0);
}

#endif /* not MAIN */



#ifdef MAIN

/*P:addPoisson*

________________________________________________________________

		addPoisson
________________________________________________________________

Name:		addPoisson  - add Poisson noise to the output image

Syntax:         addPoisson <inimage> <outimage>

Description:    addPoisson generates Poisson distributed noise in 'outimage'.
                For each output pixel, the corresponding pixelvalue in
		'inimage' is used as the mean value in the distribution.
		If the resulting random value is larger than 255, output is
		set to 255.

See also:	mkPoisson(1), mkGauss(1), addGauss(1), multGauss(1)

Author:		Helene Schulerud, BLAB, Ifi, UiO

Examples:       addPoisson mona.img monaaddP.img

Id:             $Id$                
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1,i2;
  int xsize,ysize,nbands,bn,pt;
  char arg[60];
  BiffStatus status = Iok;

  Iset_message(TRUE);		/* warnings from blab-software */
  Iset_abort(FALSE);		/* but don't abort in BIFF routines */

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <infile> <outfile>\n"));

  if (argc == 1) Usage(1, NULL);
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  
  pt     = Ipixtyp(i1[1]);
  xsize  = Ixsize(i1[1]);
  ysize  = Iysize(i1[1]);
  nbands = Inbands(i1);

  i2 = Init_image(nbands, Ititle(i1));

  for (bn=1; bn<=nbands; bn++) {
    i2[bn] = Imake_band(pt, xsize, ysize);
    status = addPoisson(i1[bn], i2[bn]);
    if (status) Warning(status, "Error in band %d.\n", bn);
  }

  Ihistory(i2, argv[0], arg);
  Iwrite_image(i2, argv[2]);

  return(0);
}

#endif /* MAIN */
