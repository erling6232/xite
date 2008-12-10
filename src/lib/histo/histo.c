

/*

________________________________________________________________

        histo
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


/*F:histo*

________________________________________________________________

		histo
________________________________________________________________

Name:		histo - Functions for histogram transformations
Description:    A collection of definitions and procedures for
                creating and manipulating histograms. Include
                the file <xite/histo.h> and see help about each
                individual routine:

                | combineTransf - combine two histogram transformations
                | histoEq       - perform histogram equalization
                | histoEqCol    - histoEq using color table
                | histoNorm     - perform histogram normalization
                | histoSpecify  - create specified histo. transf.
                | histoTransf   - perform specified histo. transf.
                | invertHisto   - invert histogram transformation
                | mkCumHisto    - make cumulative histogram
                | mkHisto       - make histogram
                | scaleHisto    - scale histogram to a range
                | scaled2col    - create color table from a transf.

See also:       histoEq(3), histoEqCol(3), histoNorm(3), histoTransf(3),
                scaleHisto(3), invertHisto(3), histoSpecify(3), scaled2col(3)
Author:		Tor Lønnestad
Id:             $Id$
________________________________________________________________

*/






