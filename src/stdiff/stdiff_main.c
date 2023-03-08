

/*

________________________________________________________________

        stdiff.c
        $Id$
        Copyright 1990, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and it
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
#include <xite/stdiff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include XITE_UNISTD_H
#include <xite/blab.h>
#include XITE_MALLOC_H
#include <xite/message.h>
#include <xite/readarg.h>

/*P:stdiff*

________________________________________________________________

		stdiff
________________________________________________________________

Name:		stdiff - local statistical differencing

Syntax:		stdiff <inimage> <outimage> <deltax> <deltay>
                       <alpha> <beta> <m0> <s0>

Description:	Local Pixel Value Mapping.

                | r(x,y) = u(x,y) + s(x,y)
		| u(x,y) = alpha * m0 + (1-alpha) * bm(x,y)
		| s(x,y) = (b(x,y)-bm(x,y)) * (beta*s0 / (s0+bs(x,y)))
		|
		| r(x,y)  = output image
		| u(x,y)  = mean component
		| s(x,y)  = st. dev component
		| m0      = The mean to force locally
		| alpha   = fraction m0 <--> local mean
		| b(b,y)  = pixel value in (x,y)
		| bm(x,y) = local mean
		| bs(x,y) = local st. dev
		| s0	  = new st. dev
		| beta    = parameter to control s0 <--> local st.dev

		'deltax' is local area width, 'deltay' is local area height.

Se also:	Wayne Niblack: An introduction to Digital Image Processing

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE input, output;
  int i;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <deltax> <deltay>\n\
       <alpha> <beta> <m0> <s0>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 9) Usage(1, "Illegal number of arguments.\n");

  input  = Iread_image(argv[1]);
  output = Icopy_init(input);  

  for (i=1; i<=Inbands(input); i++)
    stdiff(input[i], output[i],
            atoi(argv[3]), atoi(argv[4]), atof(argv[5]), 
            atof(argv[6]), atof(argv[7]), atof(argv[8]));

  Ihistory(output, argv[0], args);
  Iwrite_image(output, argv[2]);

  return(0);
}
