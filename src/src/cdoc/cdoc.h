
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        cdoc.h
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

#ifndef _CDOC_H_
#define _CDOC_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/includes.h>
#include XITE_STDIO_H

extern _XITE_WIN_DLL int att_optind;
extern _XITE_WIN_DLL char *att_optarg;

extern _XITE_WIN_DLL int open_files  _XITE_PARAMS(( int argc, char *argv[], FILE **inp, FILE **outp, char *definp, char *defoutp, int owerwrite, char **infilename, char **outfilename ));
extern _XITE_WIN_DLL void close_files  _XITE_PARAMS(( FILE *inp, FILE *outp ));
extern _XITE_WIN_DLL int attgetopt  _XITE_PARAMS(( int argc, char **argv, char *opts ));
extern _XITE_WIN_DLL void add_html_toc _XITE_PARAMS(( FILE *FILEin, char **headers, int num_headers, FILE *FILEout, int add_toc, int add_ref ));

_XITE_CPLUSPLUS_END

#endif /* _CDOC_H_ */
