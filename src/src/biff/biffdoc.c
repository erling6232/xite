
/*

________________________________________________________________

        bit-handling
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



/*D:Igetbit*

________________________________________________________________

		Igetbit
________________________________________________________________

Name:		Igetbit - get a single bit from a byte band

Syntax: 	| #include <xite/biff.h>
		|
		| Igetbit(band, y, x, b)  
                | IBAND band;
		| int y, x, b;

Description:    Returns bit number <b> from pixel band[y][x].
                Defined as a macro:
		| #define Igetbit(band,y,x,b) \\
		|   ((band[y][x] & Ibitmask[b])?1:0)

		<b> should be between 1 (least signif.) and 8 (most signif.)

Return value:   The bit value, 0 or 1.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

/*D:Isetbit*

________________________________________________________________

		Isetbit
________________________________________________________________

Name:		Isetbit - set a single bit in a byte band

Syntax: 	| #include <xite/biff.h>
		|
		| Isetbit(band, y, x, b)  
                | IBAND band;
		| int y, x, b;

Description:    Sets bit number <b> in pixel band[y][x].
                Defined as a macro:
		| #define Isetbit(band,y,x,b) (band[y][x] |= Ibitmask[b])

		<b> should be between 1 (least signif.) and 8 (most signif.)

Return value:   The whole new pixel value

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

/*D:Iclearbit*

________________________________________________________________

		Iclearbit
________________________________________________________________

Name:		Iclearbit - clear a single bit in a byte band

Syntax: 	| #include <xite/biff.h>
		|
		| Iclearbit(band, y, x, b)  
                | IBAND band;
		| int y, x, b;

Description:    Clears bit number <b> from pixel band[y][x].
                Defined as a macro:
		| #define Iclearbit(band,y,x,b) \\
		|   (band[y][x] &= Inegbitmask[b])

		<b> should be between 1 (least signif.) and 8 (most signif.)

Return value:   The whole new pixel value.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

/*D:Inegbit*

________________________________________________________________

		Inegbit
________________________________________________________________

Name:		Inegbit - negate a single bit in a byte band

Syntax: 	| #include <xite/biff.h>
		|
		| Inegbit(band, y, x, b)  
                | IBAND band;
		| int y, x, b;

Description:    Negates bit number <b> from pixel band[y][x].
                Defined as a macro:
		| #define Inegbit(band,y,x,b) (band[y][x] ^= Ibitmask[b])

		<b> should be between 1 (least signif.) and 8 (most signif.)

Return value:   The whole new pixel value.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/
