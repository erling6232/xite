
/*

________________________________________________________________

        xite_file.h
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

#ifndef _XITE_FILE_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef SunOS
# define _XITE_FILE_H_
# include <sys/file.h>
  int flock _XITE_PARAMS(( int fd, int operation ));
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
#endif /* SunOS */

#ifdef SunOS5
# define _XITE_FILE_H_
# include <sys/file.h>
# define flock(a,b)     0
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
#endif /* SunOS5 */

#ifdef ULTRIX
# define _XITE_FILE_H_
# include <sys/file.h>
  int flock _XITE_PARAMS(( int fd, int operation ));
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
#endif /* ULTRIX */

#ifdef AIX
# define _XITE_FILE_H_
# include <sys/file.h>
# define flock(a,b)     0
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
#endif /* AIX */

#if defined(IRIX) || defined(IRIX64)
# define _XITE_FILE_H_
# include <sys/file.h>
# define flock(a,b)     0
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
#endif /* IRIX || IRIX64 */

#ifdef HPUX
# define _XITE_FILE_H_
# include <sys/file.h>
# define flock(a,b)     0
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
# ifndef LOCK_SH
#  define LOCK_SH	1
# endif /* LOCK_SH */
# ifndef LOCK_EX
#  define LOCK_EX	2
# endif /* LOCK_EX */
# ifndef LOCK_NB
#  define LOCK_NB	4
# endif /* LOCK_NB */
#endif /* HPUX */

#ifdef OSF1
# define _XITE_FILE_H_
# include <sys/file.h>
# define flock(a,b)     0
# ifndef O_BINARY
#  define O_BINARY      0
# endif /* O_BINARY */
# ifndef LOCK_SH
#  define LOCK_SH	1
# endif /* LOCK_SH */
# ifndef LOCK_EX
#  define LOCK_EX	2
# endif /* LOCK_EX */
# ifndef LOCK_NB
#  define LOCK_NB	4
# endif /* LOCK_NB */
#endif /* OSF1 */

#ifdef CYGWIN32
# define _XITE_FILE_H_
# include <sys/file.h>
# define flock(a,b)     0
# ifndef LOCK_SH
#  define LOCK_SH	1
# endif /* LOCK_SH */
# ifndef LOCK_EX
#  define LOCK_EX	2
# endif /* LOCK_EX */
# ifndef LOCK_NB
#  define LOCK_NB	4
# endif /* LOCK_NB */
#endif /* OSF1 */

#ifdef _WIN32
# define _XITE_FILE_H_
# include <io.h>
# define flock(a,b)     0
# ifndef LOCK_SH
#  define LOCK_SH	1
# endif /* LOCK_SH */
# ifndef LOCK_EX
#  define LOCK_EX	2
# endif /* LOCK_EX */
# ifndef LOCK_NB
#  define LOCK_NB	4
# endif /* LOCK_NB */
# ifndef FNDELAY
#  define FNDELAY       0
# endif /* FNDELAY */
# ifndef L_SET
#  define L_SET		0
# endif /* L_SET */
# ifndef L_INCR
#  define L_INCR	1
# endif /* L_INCR */
#endif /* _WIN32 */

#ifndef _XITE_FILE_H_
# define _XITE_FILE_H_
# include <sys/file.h>
  int flock _XITE_PARAMS(( int fd, int operation ));
# ifndef O_BINARY
#  define O_BINARY     0
# endif /* O_BINARY */
#endif /* Default case */

_XITE_CPLUSPLUS_END

#endif /* _XITE_FILE_H_ */
