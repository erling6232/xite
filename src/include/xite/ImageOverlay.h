

/*

________________________________________________________________

        ImageOverlay.h
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


#ifndef _ImageOverlay_h
#define _ImageOverlay_h

_XITE_CPLUSPLUS_BEGIN

/* declare specific ImageWidget class and instance datatypes */

typedef struct _ImageOverlayClassRec*	ImageOverlayWidgetClass;
typedef struct _ImageOverlayRec*      	ImageOverlayWidget;

#ifdef IMAGE_OVERLAY
 typedef void (*resizefunc) _XITE_PARAMS(( ImageOverlayWidget wid, int x, int y, int width, int height ));
#endif

#include "Image.h"

#define ImageOverlayPixel          "ImageOverlayPixel"

/****************************************************************
 *
 * ImageOverlay widget
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


#define XtNimageOverlayResource		"imageOverlayResource"
#define XtCImageOverlayResource		"ImageOverlayResource"

#define XtNoverlay            		"overlay"
#define XtCOverlay            		"Overlay"
#define XtNoverlaytab                   "overlaytab"
#define XtCOverlaytab                   "Overlaytab"
#define XtNprotectOverlay		"protectOverlay"
#define XtCProtectOverlay		"ProtectOverlay"
#define XtNdisplayOverlay		"displayOverlay"
#define XtCDisplayOverlay		"DisplayOverlay"
#define XtNoverlayColorCells            "overlayColorCells"
#define XtCOverlayColorCells            "OverlayColorCells"
#define XtNoverlayColorPtr              "overlayColorPtr"
#define XtCOverlayColorPtr              "OverlayColorPtr"
#define XtNdrawColor                    "drawColor"
#define XtCDrawColor                    "DrawColor"
#define XtNdrawProc                     "drawProc"
#define XtCDrawProc                     "DrawProc"
#define XtNroiX                         "roiX"
#define XtCRoiX                         "RoiX"
#define XtNroiY                         "roiY"
#define XtCRoiY                         "RoiY"
#define XtNroiWidth                     "roiWidth"
#define XtCRoiWidth                     "RoiWidth"
#define XtNroiHeight                    "roiHeight"
#define XtCRoiHeight                    "RoiHeight"
#define XtNroiZoomPan                   "roiZoomPan"
#define XtCRoiZoomPan                   "RoiZoomPan"
#define XtNroiDisplay                   "roiDisplay"
#define XtCRoiDisplay                   "RoiDisplay"
#define XtNroiActivated                 "roiActivated"
#define XtCRoiActivated                 "RoiActivated"
#define XtNroiPermanent                 "roiPermanent"
#define XtCRoiPermanent                 "RoiPermanent"
#define XtNroiFill                      "roiFill"
#define XtCRoiFill                      "RoiFill"
#define XtNroiXaspect                   "roiXaspect"
#define XtCRoiXaspect                   "RoiXaspect"
#define XtNroiYaspect                   "roiYaspect"
#define XtCRoiYaspect                   "RoiYaspect"
#define XtNroiProc                      "roiProc"
#define XtCRoiProc                      "RoiProc"

/* declare the class constant */

extern WidgetClass imageOverlayWidgetClass;

typedef enum {
  ROI_CLEAR,
  ROI_SET,
  ROI_TOGGLE
} roi_mode;

typedef enum {
  ROI_NOP,
  ROI_STARTDRAG,
  ROI_DRAG,
  ROI_STOPDRAG,
  ROI_STARTMOVE,
  ROI_MOVE,
  ROI_STOPMOVE,
  ROI_EXTENDDRAG
} roi_callback;

typedef enum {
  OVERLAY_QUERY = -1,  /* Don't set */
  OVERLAY_CLEAR = 0,
  OVERLAY_SET,
  OVERLAY_TOGGLE
} overlay_mode;

typedef enum {
  DRAW_NOP,
  DRAW_ORIGO,
  DRAW_POINT,
  DRAW_LINE,
  DRAW_RECTANGLE,
  DRAW_BOX,
  DRAW_FILL,
  DRAW_CLEAR,
  DRAW_GRAPHIC,
  DRAW_PROTECT,
  DRAW_COLOR
} overlay_callback;

typedef struct 
{
  int reason;
  XEvent *event;
  struct imagestruct *img;
  struct bandstruct *band;
  struct bandstruct *overlay;
  int value;
  int x, y, width, height, xorigo, yorigo;
} ImageOverlayCallbackRec, *ImageOverlayCallback; 



/*
  The rest of this header-file is produced automatically (at least in part)
  by makeHeader of BLAB, Ifi, UiO.
*/


extern int ImageOverlayColor  _XITE_PARAMS(( ImageOverlayWidget wid, int col ));
extern void ImageOverlayCopyImageOverlayPart  _XITE_PARAMS(( ImageOverlayWidget widto, ImageOverlayWidget widfrom ));
extern void ImageOverlayCopyResources  _XITE_PARAMS(( ImageOverlayWidget widto, ImageOverlayWidget widfrom ));
extern void ImageOverlayCopyImagePart  _XITE_PARAMS(( ImageOverlayWidget widto, ImageOverlayWidget widfrom ));
extern void ImageOverlayGetResources _XITE_PARAMS(( ImageOverlayWidget wid, ArgList *args, Cardinal *num_args ));
extern void ImageOverlayGetImageOverlayResources _XITE_PARAMS(( ImageOverlayWidget wid, ArgList *args, Cardinal *num_args ));
extern int ImageOverlayDisplay  _XITE_PARAMS(( ImageOverlayWidget wid, int state ));
extern int ImageOverlayDrawBox  _XITE_PARAMS(( ImageOverlayWidget wid, int x1, int y1, int x2, int y2, int value, update_status update ));
extern int ImageOverlayDrawLine  _XITE_PARAMS(( ImageOverlayWidget wid, int x1, int y1, int x2, int y2, int value, update_status update ));
extern int ImageOverlayDrawOrigo  _XITE_PARAMS(( ImageOverlayWidget wid, int origo, int x, int y ));
extern int ImageOverlayDrawPoint  _XITE_PARAMS(( ImageOverlayWidget wid, int x, int y, int size, int value, update_status update ));
extern int ImageOverlayDrawRectangle  _XITE_PARAMS(( ImageOverlayWidget wid, int x1, int y1, int x2, int y2, int value, update_status update ));
extern int ImageOverlayFill  _XITE_PARAMS(( ImageOverlayWidget wid, int x, int y, int search, int fill, int nc, int mode, update_status update ));
extern ImageData ImageOverlayPosXYtoAdr  _XITE_PARAMS(( ImageOverlayWidget wid, int x, int y, int option ));
extern int ImageOverlayProtect  _XITE_PARAMS(( ImageOverlayWidget wid, int state ));
extern void ImageOverlayRedisplay  _XITE_PARAMS(( Widget wid, update_status update ));
extern ImageData ImageOverlayScreenXYtoAdr  _XITE_PARAMS(( ImageOverlayWidget wid, int x, int y, int option ));


_XITE_CPLUSPLUS_END

#endif  /* _ImageOverlay_h */
