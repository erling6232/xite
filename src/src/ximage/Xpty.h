
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Xpty.h
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

#ifndef _XPTY_H_
#define _XPTY_H_

_XITE_CPLUSPLUS_BEGIN

#define MAXJOBS 16

/* To be defined later. Makes the following prototype possible. */
struct _program;

typedef int (*ptyfunc1) _XITE_PARAMS(( struct _program *prog ));
typedef int (*ptyfunc2) _XITE_PARAMS(( struct _program *prog, char *buf ));

#include <X11/Intrinsic.h>

typedef struct _program {
  int jobno;
  char *jobname;
  int ch_in,     /* Pty for child stdout.                     */
      ch_out,    /* Pty for child stdout.                     */
      ch_err,    /* Pipe number for child stderr.             */
      ch_imgin,  /* Pipe number for input images for child.   */
      ch_imgout; /* Pipe number for output images from child. */
  ptyfunc1 openfunc, closefunc, imgfunc;
  ptyfunc2 stdoutfunc, stderrfunc;
  int inimages,  /* The number of input images for child program.   */
      outimages; /* The number of output images from child program. */
  int terminated;
  Widget wid;
  XtInputId id1, /* ID for event source from child stdout.        */
            id2, /* ID for event source from child stderr.        */
            id3; /* ID for event source from child output images. */
  int num_images_read,   /* The number of images received back from child. */
      num_overlays_read, /* The number of overlays received.               */
      num_colortabs_read,/* The number of color tables received.           */
      num_ovltabs_read;  /* The number of overlay color tables received.   */
} program;


extern void XptyClose  _XITE_PARAMS(( program *prog, int pipe, int typ ));
extern int XptyInit  _XITE_PARAMS(( Display *display, int screenno, Widget parent ));
extern int XptyJobs  _XITE_PARAMS(( void ));
extern void XptyKill  _XITE_PARAMS(( program *prog ));
void XptyList _XITE_PARAMS((Widget wid, XtPointer c_data, XtPointer call_data));
extern void XptyMessage  _XITE_PARAMS(( program *prog, char *txt ));
extern int XptyStartProgram  _XITE_PARAMS(( char *entry, char *vec[], int shell, ptyfunc1 openfunc, ptyfunc1 closefunc, ptyfunc2 stdoutfunc, ptyfunc2 stderrfunc, ptyfunc1 imgfunc ));

_XITE_CPLUSPLUS_END

#endif /* _XPTY_H_ */
