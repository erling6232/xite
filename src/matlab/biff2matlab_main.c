

/*C*

________________________________________________________________

        biff2matlab
        $Id$
        Copyright 1994, Blab, UiO
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

#include <mat.h>

#include <xite/includes.h>
#include <xite/matlab.h>
#ifndef mxMAXNAM
# define mxMAXNAM 20
#endif
#include <xite/message.h>
#include XITE_STRING_H

static char *biff2matlab_error_s[] = {
"OK\n",
"Failure in creating MATLAB matrix.\n",
"Failure in converting BIFF band to MATLAB matrix.\n",
"Failure in opening MATLAB matrix file.\n",
"Failure in writing MATLAB matrix to file.\n"
};



/*P:biff2matlab*

________________________________________________________________

		biff2matlab
________________________________________________________________

Name:		biff2matlab - Convert from XITE BIFF image file format to
                MATLAB matrix file format.

Syntax:		biff2matlab <biffFile> <matFile>

Description:    Convert from XITE BIFF image file format to MATLAB version 5
                MAT-file format. Each band may consist of any of the
		following pixel	types: UNS_BYTE, SIGN_BYTE, UNS_SHORT,
		SIGN_SHORT, INTEGER, REAL, COMPLEX, DOUBLE, DCOMPLEX.

		"-", "" or "-0" can be used for 'stdin'. The input filename
		may be followed by band specifications, as in file:3 for the
		third band (see 'Iread_image(3)').
		
		If the BIFF image has a non-empty title, the name (names)
		is (are) generated from it (with a filename extension for
		multiband images). If the BIFF image has no non-empty title,
		the name(s) is (are) created from the MATLAB matrix filename
		(and band number). With an empty filename, the name is not
		set.

		A MATLAB matrix name must start with a letter and only
		consist of letters, digits or underscores (a total of
		maximum mxMAXNAM characters). An illegal initial character is
		replaced by the letter "a", any other character is replaced
		by an underscore.

		If the MATLAB matrix file exists, the matrix is appended to
		the file (unless a matrix with the same name already exists,
		in which case the new matrix replaces the old).

See also:	'matlab2biff(1)', 'Iread_image(5)'

Return value:   | 0 => OK
                | 1 => Usage message
                | 2 => Illegal command line
		| 3 => Failure in creating MATLAB matrix
		| 4 => Failure in converting BIFF band to MATLAB matrix
                | 5 => Failure in opening MATLAB matrix file
		| 6 => Failure in writing MATLAB matrix to file

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
    IMAGE img;
    int status;

    /* Initialize XITE program */

    Iset_message(1);
    Iset_abort(1);
    InitMessage(&argc, argv, xite_app_std_usage_text(
      "Usage: %s <biffFile> <matlabFile>\n\
        '-' can be used for stdin.\n"));

    /* stdin if name is "-", "" or "-0" (XITE standard) */

    if (argc == 1) Usage(1, NULL);
    if (argc != 3) Usage(2, "Illegal number of arguments.\n");

    img = Iread_image(argv[1]);

    if ((status = saveBiffAsMatlab(img, argv[2])) != Iok)
      Error(status, biff2matlab_error_s[status]);

    return(0);
}
