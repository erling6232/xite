

/*C*

________________________________________________________________

        xite_funcproto.h
        $Id$
        Copyright 1995, Blab, UiO
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

#ifndef _XITE_FUNCPROTO_H_
#define _XITE_FUNCPROTO_H_

#include <xite/config.h>

#if defined(__STDC__) || defined(_ANSI_SOURCE) || defined(__cplusplus)
/* __STDC__ or _ANSI_SOURCE defined */
# ifndef FUNCPROTO
/* Define FUNCPROTO. */
#   define FUNCPROTO 1
# endif
#endif

#if defined(USE_PROTOTYPES) && !defined(FUNCPROTO)
/* USE_PROTOTYPES defined, typically from gcc. */
# define FUNCPROTO
#endif

#if defined(__USE_FIXED_PROTOTYPES__) && !defined(FUNCPROTO)
/* __USE_FIXED_PROTOTYPES__ defined, typically from gcc. */
# define FUNCPROTO
#endif

#if (defined(IRIX) || defined(IRIX64)) && !defined(FUNCPROTO)
/* SGI cc is capable of handling prototypes, even with the flag -cckr */
/* Define FUNCPROTO. */
# define FUNCPROTO
#endif

#if defined(OSF1) && !defined(FUNCPROTO)
/* Define _NO_PROTO. */
# define _NO_PROTO
#endif

#if defined(HPUX) && defined(FUNCPROTO) && !defined(_PROTOTYPES)
/* Define _PROTOTYPES */
# define _PROTOTYPES
#endif

#if defined(_WIN32) && !defined(FUNCPROTO)
# define FUNCPROTO
#endif

#ifdef FUNCPROTO
# define _XITE_PARAMS(PARS) PARS
#else
# define _XITE_PARAMS(PARS) ()
#endif /* FUNCPROTO */

#ifdef __cplusplus
# define _XITE_CPLUSPLUS_BEGIN extern "C" {
# define _XITE_CPLUSPLUS_END }
#else
# define _XITE_CPLUSPLUS_BEGIN
# define _XITE_CPLUSPLUS_END
#endif /* __cplusplus */

#ifdef _WIN32
# if defined(XITE_DLL) && !defined(MAIN)
#   define _XITE_WIN_DLL __declspec(dllexport)
# else /* XITE_DLL */
#   define _XITE_WIN_DLL __declspec(dllimport)
# endif /* XITE_DLL */

# if defined(XITE_TIFF_DLL) && !defined(MAIN)
#   define _XITE_TIFF_WIN_DLL __declspec(dllexport)
# else /* XITE_TIFF_DLL */
#   define _XITE_TIFF_WIN_DLL __declspec(dllimport)
# endif /* XITE_TIFF_DLL */

# if defined(XITE_MATLAB_DLL) && !defined(MAIN)
#   define _XITE_MATLAB_WIN_DLL __declspec(dllexport)
# else /* XITE_MATLAB_DLL */
#   define _XITE_MATLAB_WIN_DLL __declspec(dllimport)
# endif /* XITE_MATLAB_DLL */
#else /* _WIN32 */
# define _XITE_WIN_DLL
# define _XITE_TIFF_WIN_DLL
# define _XITE_MATLAB_WIN_DLL
#endif /* _WIN32 */

#endif /* _XITE_FUNCPROTO_H_ */
