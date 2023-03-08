
/*C*

________________________________________________________________

        matlab2biff
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

#include <mat.h>

#include <xite/includes.h>
#include <xite/matlab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

static char *matlab2biff_error_s[] = {
  "OK\n",
  "Error\n",
  "Illegal command line.\n",
  "Failure in opening the MAT-file.\n",
  "Failure in reading the MAT-file directory.\n",
  "Did not find all requested matrices in file.\n",
  "Failure in initializing BIFF image.\n",
  "Failure in creating BIFF image band.\n",
  "Failure in converting MATLAB matrix to XITE BIFF band.\n",
  "Failure in opening BIFF image file.\n",
  "Failure in writing BIFF image band to file.\n"
};



/*P:matlab2biff*

________________________________________________________________

		matlab2biff
________________________________________________________________

Name:		matlab2biff - Convert from MATLAB MAT-file format to
                XITE BIFF image file format.

Syntax:		matlab2biff <MAT-file> <BIFF-file> [-s <matrixName>] ...

Description:    Convert from MATLAB MAT-file format to XITE BIFF image file
                format. Only numeric matrices are converted. If more than
		one matrix, they are all converted into the same BIFF file.
		Default is all numeric matrices. The XITE BIFF image bands
		will get pixel type double or double complex.

		"-", "" or "-1" can be used for 'stdout'. The output filename
		may be followed by band specifications, as in "file:3" for the
		third band (see 'Iwrite_image(3)').

		The BIFF image title will be the same as the name of the first
		MATLAB matrix with a non-empty name string, or else the BIFF
		filename (if not "-" or "" or "-1" for 'stdout'), or the
		empty string.

		Will read both MATLAB version 4 and version 5 MAT-files.

Restrictions:   Will not read sparse matrices.

See also:	'biff2matlab(1)', 'Iwrite_image(5)'

Return value:   | 0 => OK
                | 1 => Usage message
                | 2 => Illegal command line
                | 3 => Failure in opening the MAT-file.
		| 4 => Failure in reading the MAT-file directory.
		| 5 => Did not find all requested matrices in file.
                | 6 => Failure in initializing BIFF image.
		| 7 => Failure in creating BIFF image band.
		| 8 => Failure in converting MATLAB matrix to
		|      XITE BIFF band.
		| 9 => Failure in opening BIFF image file.
		| 10 => Failure in writing BIFF image band to file.

Author:		Svein Bøe, BLAB, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
    mxArray **matrices;
    char *matNames[100], *name;
    long nMat, nNames;
    int status;

    /* Initialize XITE program */

    Iset_message(1);
    Iset_abort(1);
    InitMessage(&argc, argv, xite_app_std_usage_text(
      "Usage: %s <matFile> <biffFile> [-s <matrixName>] ... \n\
        '-' can be used for stdout.\n"));

    /* stdout if name is "-", "" or "-1" (XITE standard) */

    if (argc == 1) Usage(1, NULL);
    if ((argc % 2) != 1) Usage(2, "Illegal number of arguments.\n");

    nNames = 0;
    while (argc > 3) {
      if (strlen(name = read_switch(&argc, argv, "-s", 1, "")) > 0)
	matNames[nNames++] = name;
      else break;
    }

    if ((argc % 2) != 1) Usage(2, "Illegal number of arguments.\n");

    /* nNames == 0: Read all numeric matrices (default) */
    if (( status = readMatrices(&matrices, &nMat, matNames, nNames, argv[1]))
	!= Iok)
      Error(status + 2, matlab2biff_error_s[status+2]);

    if ((status = saveMatlabAsBiff((const mxArray **) matrices, nMat,
				   argv[2], argv[0]))
	!= Iok)
      Error(status + 5, matlab2biff_error_s[status+5]);

    return(0);
}
