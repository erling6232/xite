

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

#include <xite/config.h>

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

#ifdef HAVE_FCNTL_H
#  define XITE_FCNTL_H      <fcntl.h>
#else
#  ifdef HAVE_SYS_FCNTL_H
#    define XITE_FCNTL_H      <sys/fcntl.h>
#  else
#    define XITE_FCNTL_H   Error: fcntl.h is required
#  endif
#endif

#ifdef HAVE_IO_H
#  define XITE_IO_H         <io.h>
#else
#  define XITE_IO_H         <xite/dummy.h>
#endif

#ifdef HAVE_MALLOC
#  define XITE_MALLOC_H     <stdlib.h>
#else
#  ifdef HAVE_MALLOC_H
#    define XITE_MALLOC_H   <malloc.h>
#  else
#    define XITE_MALLOC_H   Error: malloc() is required
#  endif
#endif

#ifdef HAVE_MEMORY_H
#  define XITE_MEMORY_H     <memory.h>
#else
#  ifdef HAVE_STRING_H
#    define XITE_MEMORY_H   <string.h>
#  else
#    define XITE_MEMORY_H   Error: memory.h or string.h is required
#  endif
#endif

#ifdef HAVE_STRING_H
#  define XITE_STRING_H     <string.h>
#else
#  ifdef HAVE_STRINGS_H
#    define XITE_STRING_H   <strings.h>
#  else
#    define XITE_STRING_H   Error: string.h or strings.h is required
#  endif
#endif

#ifdef HAVE_SYS_PARAM_H
#  define XITE_PARAM_H      <sys/param.h>
#else
#  define XITE_PARAM_H      <xite/dummy.h>
#endif

#endif /* _XITE_INCLUDES_H_ */

