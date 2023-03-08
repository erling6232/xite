
/* $XConsortium: Histogram.h                                           */

#ifndef _Histogram_h
#define _Histogram_h

_XITE_CPLUSPLUS_BEGIN

/****************************************************************
 *
 * Histogram widget
 *
 ****************************************************************/

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 destroyCallback     Callback		Pointer		NULL
 height		     Height		Dimension	0
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

*/

/* define any special resource names here that are not in <X11/StringDefs.h> */


#define XtNhistogramResource		"histogramResource"
#define XtCHistogramResource		"HistogramResource"

#define XtNhistogramHistogram               "histogramHistogram"
#define XtCHistogramHistogram               "HistogramHistogram"
#define XtNhistogramLength                  "histogramLength"
#define XtCHistogramLength                  "HistogramLength"
#define XtNhistogramTitle                   "histogramTitle"
#define XtCHistogramTitle                   "HistogramTitle"
#define XtNhistogramVisual                  "histogramVisual"
#define XtCHistogramVisual                  "HistogramVisual"
#define XtNhistogramColormap                "histogramColormap"
#define XtCHistogramColormap                "HistogramColormap"
#define XtNhistogramImage                   "histogramImage"
#define XtCHistogramImage                   "HistogramImage"
#define XtNhistogramBand                    "histogramBand"
#define XtCHistogramBand                    "HistogramBand"
#define XtNhistogramCrossing                "histogramCrossing"
#define XtCHistogramCrossing                "HistogramCrossing"
#define XtNhistogramButton                  "histogramButton"
#define XtCHistogramButton                  "HistogramButton"
#define XtNhistogramMotion                  "histogramMotion"
#define XtCHistogramMotion                  "HistogramMotion"
#define XtNhistogramResize                  "histogramResize"
#define XtCHistogramResize                  "HistogramResize"
#define XtNhistogramKill                    "histogramKill"
#define XtCHistogramKill                    "HistogramKill"


/* declare specific HistogramWidget class and instance datatypes */

typedef struct _HistogramClassRec*	HistogramWidgetClass;
typedef struct _HistogramRec*		HistogramWidget;

/* declare the class constant */

extern WidgetClass histogramWidgetClass;

typedef struct 
{
  XEvent *event;
  unsigned char ***image;
  unsigned char **band;
  int *histogram, *kumulativ;
  int length;
  int korh;  /* Kumulative == 1, Normal == 0 */ 
  int x, y;
} HistogramButtonCallbackRec, *HistogramButtonCallback; 



/*
  The rest of this header-file is produced automatically (at least in part)
  by makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Histogram.h
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


extern void Draw_all_lines  _XITE_PARAMS(( Widget wid, int func ));
extern void HistogramCrossing  _XITE_PARAMS(( Widget wid, caddr_t dummy, int *colormap ));
extern void HistogramDeletePoint  _XITE_PARAMS(( HistogramWidget wid, int point, int val, int col ));
extern void HistogramExitLines  _XITE_PARAMS(( HistogramWidget wid ));
extern void HistogramGet  _XITE_PARAMS(( HistogramWidget wid, XColor tab[], int *len ));
extern void HistogramInitLines  _XITE_PARAMS(( HistogramWidget wid, int col, int threshold ));
extern void HistogramInsertPoint  _XITE_PARAMS(( HistogramWidget wid, int point, int val, int col ));
extern void HistogramInspect  _XITE_PARAMS(( HistogramWidget wid, XColor tab[] ));
extern void HistogramKill  _XITE_PARAMS(( Widget wid, XEvent *event, String *params, Cardinal *num_params ));
extern void HistogramMovePoint  _XITE_PARAMS(( HistogramWidget wid, int point, int val, int col ));
extern void HistogramRedisplay  _XITE_PARAMS(( Widget wid ));
extern void HistogramSet  _XITE_PARAMS(( HistogramWidget wid, XColor tab[], int len ));
extern void HistogramThreshold  _XITE_PARAMS(( HistogramWidget wid, int point, int col ));


_XITE_CPLUSPLUS_END

#endif  /* _Histogram_h */
