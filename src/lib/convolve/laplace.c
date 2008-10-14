
/*

________________________________________________________________

        laplace
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/convolve.h>
#include <xite/readarg.h>
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#include <xite/message.h>

/*F:laplace*

________________________________________________________________

		laplace
________________________________________________________________

Name:		laplace - edge and line detection
Syntax:         | #include <xite/convolve.h>
		|
                | void laplace( IBAND input, IBAND output, int a,
                |    int b, int c, double scale, double offset,
                |    int verbose );
Description:    'laplace' performs the convolution between
                a band 'input' and a convolution kernel 'conv'.
                |
		|        a          b          a
		|        b          c          b
                |        a          b          a
		|
                The result of the convolution is scaled
		|   pix = (conv. result) * scale + offset;
                If 'scale' is zero an autoscaling is performed.
		If 'verbose' flag is set, the number of underflows and
		overflows are printed. If 'verbose' flag is set
		and autoscale is performed the scale and offset 
		factors are displayed

		'conv3x3_overflow' and 'conv3x3_underflow' are 
		global variabels and may be declared as
		| extern int conv3x3_overflow, conv3x3_underflow
See also:	laplace(1), conv3x3(3), lapZeroCross(3)
Author:		Otto Milvang
Examples:       
Id: 		$Id$
________________________________________________________________

*/



void laplace(IBAND input, IBAND output, int a, int b, int c, double scale, double offset, int verbose)
{
  int conv[9];
  conv[0] = conv[2] = conv[6] = conv[8] = a;
  conv[1] = conv[3] = conv[5] = conv[7] = b;
  conv[4] = c;
  conv3x3(input, output, conv, scale, offset, verbose);
}
