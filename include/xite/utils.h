
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        utils.h
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

#ifndef _UTILS_H_
#define _UTILS_H_

_XITE_CPLUSPLUS_BEGIN
#include <xite/compu4.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/utl_malloc.h>

typedef enum {
  XITE_FILENAME_NONE,     /* NULL-string */
  XITE_FILENAME_EMPTY,    /* "\0" */
  XITE_FILENAME_STDIN,    /* "-0" */
  XITE_FILENAME_STDOUT,   /* "-1" */
  XITE_FILENAME_STDINOUT, /* "-" */
  XITE_FILENAME_PIPE,     /* "-n", 2 <= n <= 9 */
  XITE_FILENAME_OTHER     /* other string */
} xite_filename_type;

extern _XITE_WIN_DLL char *envfindfile  _XITE_PARAMS(( char *environment, char *file ));
extern _XITE_WIN_DLL char *envfindfilemulti _XITE_PARAMS(( char *environment, char *file ));
extern _XITE_WIN_DLL double gaussRandom  _XITE_PARAMS(( double my, double sigma ));
extern _XITE_WIN_DLL char *tilde_expand  _XITE_PARAMS(( char *filename ));
extern _XITE_WIN_DLL void NextFilename _XITE_PARAMS(( char **filenames, char **next ));
extern _XITE_WIN_DLL char *FilenamePart _XITE_PARAMS(( char *filename ));
extern _XITE_WIN_DLL char *realloc_string _XITE_PARAMS(( char *path, int max_path_len, int extra_len, int step_len ));
extern _XITE_WIN_DLL char *String_cat _XITE_PARAMS(( char *s1, char *s2, char *s3 ));
extern _XITE_WIN_DLL xite_filename_type xiteFilenameType  _XITE_PARAMS(( char *name ));
extern _XITE_WIN_DLL int xite_pipe_filedes _XITE_PARAMS(( char *filename, int reading ));

_XITE_CPLUSPLUS_END

#endif /* _UTILS_H_ */
