

/*

________________________________________________________________

        includes.h
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

#ifndef _XITE_INCLUDES_H_
#define _XITE_INCLUDES_H_

#if defined(ANSI) && !defined(__STDC__)
/* Define __STDC__ */
# define __STDC__
#endif

#if defined(__STDC__) && !defined(ANSI)
/* Define ANSI */
# define ANSI
#endif

#include <xite/xite_funcproto.h>

/*
   <strings.h> contains declarations of index() and rindex() on some platforms,
   not on others.
*/

/* Common definitions */

#define XITE_ENDIAN_H     <xite/xite_endian.h>
#define XITE_FILE_H       <xite/xite_file.h>
#define XITE_FORK_H       <xite/xite_fork.h>
#define XITE_LIMITS_H     <xite/xite_limits.h>
#define XITE_MKTEMP_H     <xite/xite_mktemp.h>
#define XITE_RANDOM_H     <xite/xite_random.h>
#define XITE_STAT_H       <xite/xite_stat.h>
#define XITE_STDARG_H     <xite/xite_stdarg.h>
#define XITE_STDIO_H      <xite/xite_stdio.h>
#define XITE_STRTOL_H     <xite/xite_strtol.h>
#define XITE_TIME_H       <xite/xite_time.h>
#define XITE_TOUPPER_H    <xite/xite_toupper.h>
#define XITE_TYPES_H      <xite/xite_types.h>
#define XITE_UNISTD_H     <xite/xite_unistd.h>

#ifdef SunOS
/*
# ifndef SYSV
#  define XITE_STRING_H    <strings.h>
# else
#  define XITE_STRING_H    <string.h>
# endif
*/

# define XITE_FCNTL_H      <sys/fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <memory.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* SunOS */

#ifdef SunOS5
# define XITE_FCNTL_H      <sys/fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* SunOS5 */

#ifdef ULTRIX
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* ULTRIX */

#ifdef AIX
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* AIX */

#if defined(IRIX) || defined(IRIX64)
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* IRIX || IRIX64 */

#ifdef HPUX
# ifndef SYSV
#  define XITE_MALLOC_H    <malloc.h>
#  define XITE_STRING_H    <strings.h>
# else
#  define XITE_MALLOC_H    <stdlib.h>
#  define XITE_STRING_H    <string.h>
# endif
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* HPUX */

#ifdef OSF1
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* OSF1 */

#ifdef CYGWIN32
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* CYGWIN32 */

#ifdef _WIN32
# define XITE_FCNTL_H      <fcntl.h>
# define XITE_IO_H         <io.h>
# define XITE_MALLOC_H     <malloc.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <xite/dummy.h>
#endif /* _WIN32 */

#ifndef XITE_MALLOC_H
# define XITE_FCNTL_H      <sys/fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>
#endif /* Default case */

#endif /* _XITE_INCLUDES_H_ */

