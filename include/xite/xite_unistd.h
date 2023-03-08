

/*

________________________________________________________________

        xite_unistd.h
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

/* Author: Svein Bøe, BLAB, Ifi, UiO */

#ifndef _XITE_UNISTD_H_
#define _XITE_UNISTD_H_

_XITE_CPLUSPLUS_BEGIN

#include XITE_PARAM_H
#if !defined(_WIN32)
# include <unistd.h>
#else
# include XITE_IO_H
# include <stdlib.h>
# define MAXPATHLEN _MAX_PATH
#endif

#if defined(SunOS) || defined(ULTRIX)
  char *getwd _XITE_PARAMS(( char *pathname ));
#endif

#if defined(SunOS5)
  /* These declarations are needed because the source is compiled and linked
   * in BSD compatibility mode.
   * The declarations are taken from SunOS 4.
   */

  /* In <unistd.h> if not BSD mode. */
  void _exit _XITE_PARAMS(( int status ));
  int access _XITE_PARAMS(( char *path, int amode ));
  int chdir _XITE_PARAMS(( char *path ));
  int execlp  _XITE_PARAMS(( char *file, ... ));
  int execvp  _XITE_PARAMS(( char *file, char *argv[] ));
  int fork _XITE_PARAMS(( void ));
  char *getcwd _XITE_PARAMS(( char *, size_t ));
  int link _XITE_PARAMS(( char *existing, char *new ));
  int pipe _XITE_PARAMS(( int fildes[2] ));
  int sleep _XITE_PARAMS(( unsigned seconds ));
  int unlink _XITE_PARAMS(( char *path ));
#endif /* SunOS5 */

#ifndef HAVE_GETWD
#  define getwd(path) getcwd(path, MAXPATHLEN)
#endif

_XITE_CPLUSPLUS_END

#endif /* _XITE_UNISTD_H_ */

