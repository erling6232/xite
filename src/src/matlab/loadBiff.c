
/*C*

________________________________________________________________

        loadBiff
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

#include <xite/includes.h>
#include <xite/matlab.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <mex.h>

#define FILENAME prhs[0]
#define BANDS prhs[1]



/*L:mexFunction*

________________________________________________________________

		mexFunction
________________________________________________________________

Name:		mexFunction
Syntax:		| #include <xite/matlab.h>
                |
		| void mexFunction( int nlhs, mxArray *plhs[],
		|    int nrhs, const mxArray *prhs[] );
		|
Description:    | Load XITE BIFF image file into MATLAB.
                | prhs (input arguments) are
                |   - BIFF filename
                |   - band numbers to get (starting at one). Non-existent
		|     or (-1) defaults to all bands.
                | plhs are
                |   - Pointers to the MATLAB matrices corresponding to each
		|     band.

Author:		Svein Bøe, BLAB, Ifi, UiO
Id:             $Id$
________________________________________________________________

*/

/*M*

%loadBiff  Load XITE BIFF image from file.
%
% SYNOPSIS:
%   [b1, b2, ...] = loadBiff(file, bands);
% OUTPUT:
%   b1, b2, ... : One matrix for each band requested from the image file.
% INPUT:
%   file        : String with name of XITE BIFF image file.
%   bands       : Scalar or vector containing requested band numbers
%                 (default all bands in the file).
% DESCRIPTION:
%   Load XITE BIFF image from file
%   

% AUTHOR:
%   Svein Bøe, Ifi Uio, 1994, 1997.
%

*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  IMAGE img;
  
  long nbands, n, i, *bandNo;
  char *biffFileName;

  Iset_message(1);

  if (nrhs < 1) mexErrMsgTxt("Missing file name argument.");
  if (nrhs > 2) mexErrMsgTxt("Too many input arguments.");

  if (!mxIsChar(FILENAME))
    mexErrMsgTxt("File name argument must be string.");

  n = (long) mxGetN(FILENAME);
  biffFileName = (char *) mxCalloc((size_t) (n + 1), sizeof(char));
  mxGetString(FILENAME, biffFileName, (int) (n + 1));

  if (Iopen_image(&img, biffFileName, Ireadonly) != Iok)
    mexErrMsgTxt("Could not open BIFF image file.");
    
  mxFree(biffFileName);

  nbands = Inbands(img);

  /*** Check right-hand side (input) arguments ***/

  if (nrhs == 2) {
    /* Get input band number argument and check validity */
    long m;

    /* Validate dimension */
    m = (long) mxGetM(BANDS);
    n = (long) mxGetN(BANDS);
    if (m > 1 && n > 1) mexErrMsgTxt("Band number argument must be vector.");
    if (m > n) n = m;

    if (m == 1 && n == 1 && (int) mxGetPr(BANDS)[0] < 0) {
      /* Set default, all bands */
      bandNo = (long *) malloc((size_t) (nbands * sizeof(long)));
      for (i = 1; i <= nbands; i++)
	bandNo[i - 1] = i;

      n = nbands;
    } else {
      /* Check validity */
      for (i = 1; i <= n; i++) {
	if ((int) mxGetPr(BANDS)[i - 1] > nbands
	    || (int) mxGetPr(BANDS)[i - 1] <  0)
	  mexErrMsgTxt("Band number does not exist in file.");
	else bandNo[i - 1] = (long) mxGetPr(BANDS)[i - 1];
      }
    }
  }
  else {
    /* Set default */

    bandNo = (long *) malloc((size_t) (nbands * sizeof(long)));
    for (i = 1; i <= nbands; i++)
      bandNo[i - 1] = i;

    n = nbands;
  }

  if (nlhs > n) mexErrMsgTxt("Too many output arguments.");

  /* Get the required BIFF bands */
  for (i = 1; i <= n; i++)
  {
    if (Iread_band(img, (int) bandNo[i-1]) != Iok)
      mexErrMsgTxt("Failure in reading band.");
  }

  Iclose_image(img);

  biff2matlab(img, bandNo, nbands, plhs, "");
}
