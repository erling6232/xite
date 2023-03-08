
/*

________________________________________________________________

        saveBiff
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
#include XITE_MALLOC_H
#include <mex.h>



static void setBandNo(long **bandNo, long start, long nMat)
{
  long i;

  *bandNo = (long *) mxCalloc((size_t) nMat, sizeof(long));
  for (i = 1; i <= nMat; i++)
    (*bandNo)[i - 1] = i + start - 1;
}

static void checkBandNo(long **bandNo, mxArray *band, long nMat)
{
  long i;

  for (i = 1; i <= nMat; i++) {
    if ((int) mxGetPr(band)[i - 1] <  0)
      mexErrMsgTxt("Band number does not exist in file.");
    else (*bandNo)[i - 1] = (long) mxGetPr(band)[i - 1];
  }
}

/*L:extractBandNo*

________________________________________________________________

		extractBandNo
________________________________________________________________

Name:		extractBandNo

Description:    Extract band numbers from Matrix mband and return in array
                bandNo.

Author:		Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

static void extractBandNo(long **bandNo, mxArray *mband, long nMat)
{
  long m, n;

  /* Validate dimension */
  m = (long) mxGetM(mband);
  n = (long) mxGetN(mband);
  if (m > 1 && n > 1) mexErrMsgTxt("Band number argument must be vector.");
  if (m > n) n = m;
  /* "n" is now number of bands mentioned */

  if (n > 1 && n != nMat)
  mexErrMsgTxt("Band number argument inconsistent with number of matrices.");

  if (n == 1 && (long) mxGetPr(mband)[0] < 0) {
    /* Band number argument is negative scalar. Set default, all bands */

    setBandNo(bandNo, 1, nMat);

  } else if (n == 1) {
    /* Band number argument is non-negative scalar. Set consecutive
       from this scalar */
    setBandNo(bandNo, (long) mxGetPr(mband)[0], nMat);
  } else {
    /* Band number argument is vector. Check validity */
    checkBandNo(bandNo, mband, nMat);

  }
} /* extractBandNo */



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
		|   - Matrices (numeric)
                |   - BIFF filename (string)

Author:		Svein Bøe, BLAB, Ifi, UiO
Id:             $Id$
________________________________________________________________

*/

/*M*

%saveBiff  Save matrices as XITE BIFF images to file.
%
% SYNOPSIS:
%   saveBiff(m1, m2, ..., file);
% OUTPUT:
%   m1, m2, ... : Matrices.
% INPUT:
%   file        : String with name of XITE BIFF image file.
% DESCRIPTION:
%   Save matrices as XITE BIFF images to file
%   

% AUTHOR:
%   Svein Bøe, Ifi Uio, 1994, 1997.
%

*/

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  IMAGE img;
  long bandNoIndex, nMat, *bandNo, i, n;
  char *biffFileName;

  Iset_message(1);

  if (nrhs < 1) mexErrMsgTxt("Missing file name argument.");

  nMat = 0;
  while (!mxIsChar(prhs[nMat])) nMat++;

  if (nMat == 0) mexErrMsgTxt("Missing matrix arguments.");
  if (nMat > nrhs) mexErrMsgTxt("Missing filename argument.");
  if (!mxIsChar(prhs[nMat])) mexErrMsgTxt("Missing filename argument.");

  /* Extract filename argument */
  n = (long) mxGetN(prhs[nMat]);
  biffFileName = (char *) mxCalloc((size_t) (n + 1), sizeof(char));
  mxGetString(prhs[nMat], biffFileName, (int) (n + 1));

  /* Set default */
  setBandNo(&bandNo, 1, nMat);

  if ((img = Init_image(nMat, "")) == NULL)
    mexErrMsgTxt("Failure in initializing BIFF image.");

  if (matlab2biff(img, bandNo, nMat, prhs, "") != Iok)
    mexErrMsgTxt("Failure in converting matlab matrices to BIFF image.");

  if (Iopen_image(&img, biffFileName, Iwrite) != Iok)
    mexErrMsgTxt("Could not open BIFF image file.");

  if (nlhs > 0) mexErrMsgTxt("Too many output arguments.");

  /* Get the required BIFF bands */
  for (i = 1; i <= nMat; i++)
  {
    if (Iwrite_band(img, (int) bandNo[i-1]) != Iok)
      mexErrMsgTxt("Failure in writing band.");
  }

  Iclose_image(img);
}
