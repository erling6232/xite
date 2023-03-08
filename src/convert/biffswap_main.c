

/*

________________________________________________________________

        biffswap
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
#include <xite/convert.h>
#include XITE_STDIO_H
#include <xite/message.h>



/*P:biffswap*

________________________________________________________________

		biffswap
________________________________________________________________

Name:		biffswap - swap byte order for bands in biff-file
Syntax:		| biffswap <infile> <outfile>
Description:	Swap byte order for bands in biff-file. Works for all pixel
                types, including complex and double complex.

		There is usually no need to use this program, because XITE
		takes care of swapping bytes automatically. If an image
		(with multi-byte pixeltype) is written with an XITE version
		prior to 3.0 and read with version 3.0 or later, or vice
		versa, use the following procedure.

		&Determine the version of your XITE copy either by running
		'xshow' with option '-ver' or by reading the file
		$XITE_HOME/etc/.install/VERSION. If the option is not
		recognized and the file does not exist, the version number
		is less	than 3.0.

		&Determine the endian-ness of the writing and reading hosts.
		Examples of little-endian computers: DecStation, Dec alpha,
		PC (Intel x86).

		Examples of big-endian computers: Sun, Silicon Graphics.

		&Image written with version < 3.0, read with version >= 3.0
		If the writing host was little-endian, convert the image
		first with the 'biffswap' program (of version >= 3.0) on the
		reading host (regardless of reading host''s endian-ness).
		Otherwise, no explicit conversion is necessary.

		&Image written with version >= 3.0, read with version < 3.0
		This should not happen too often.
		&&Writing host was little-endian
		With a little-endian reading-host, convert the image with
		'biffswap' (of version >= 3.0) on a big-endian machine.
		With a big-endian reading-host, the image can not be used.
		In this case, solve your problem by upgrading XITE on the
		reading-host.

		&&Writing host was big-endian
		With a little-endian reading-host, convert the image with
		'biffswap' (of version >= 3.0) on a big-endian host or with
		'biffswap' (of version < 3.0) on a little-endian host.
		With a big-endian reading-host, no explicit conversion is
		necessary.

Return value:   0
See also:	biff_swap(3)
Author:		Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int i;
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <infile> <outfile>\n"));
  if (argc != 3) Usage(0, NULL);
  Iset_abort(1);
  Iset_message(1);
  img = Iread_image(argv[1]);
  for (i=1; i<=Inbands(img); i++)
    biff_swap(img[i]);
  Ihistory(img, "biffswap", "");
  Iwrite_image(img, argv[2]);
  return(0);
}
