
/*C*

________________________________________________________________

        Image.h
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


#ifndef _Image_h
#define _Image_h

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>
#include <X11/Core.h>

typedef enum {
  UPDATE_NONE      = 0,
  UPDATE_RESIZE    = 1,
  UPDATE_ZOOMPAN   = 2,
  UPDATE_REDISPLAY = 4,
  UPDATE_NEWSIZE   = 8,
  UPDATE_MINMAX    = 16
} update_status;

/****************************************************************
 *
 * Image Widget: Public functions
 *
 ****************************************************************/

typedef unsigned char *ImageData;

/****************************************************************
 *
 * Image format depend
 *
 ****************************************************************/

/* int ImageXoffset(x)		((x) + 1)
int ImageYoffset(y)		((y) + 1)

*/


/****************************************************************
 *
 * Image widget
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

#define XtNimageResource		"imageResource"
#define XtCImageResource		"ImageResource"

#define XtNimageImage                   "imageImage"
#define XtCImageImage                   "ImageImage"
#define XtNimageBand                    "imageBand"
#define XtCImageBand                    "ImageBand"
#define XtNimageRedBand                 "imageRedBand"
#define XtCImageRedBand                 "ImageRedBand"
#define XtNimageGreenBand               "imageGreenBand"
#define XtCImageGreenBand               "ImageGreenBand"
#define XtNimageBlueBand                "imageBlueBand"
#define XtCImageBlueBand                "ImageBlueBand"
#define XtNimageRgb                     "imageRgb"
#define XtCImageRgb                     "ImageRgb"
#define XtNimageData                    "imageData"
#define XtCImageData                    "ImageData"
#define XtNimageRedData                 "imageRedData"
#define XtCImageRedData                 "ImageRedData"
#define XtNimageGreenData               "imageGreenData"
#define XtCImageGreenData               "ImageGreenData"
#define XtNimageBlueData                "imageBlueData"
#define XtCImageBlueData                "ImageBlueData"
#define XtNimagePixtype                 "imagePixtype"
#define XtCImagePixtype                 "ImagePixtype"
#define XtNimageTitle                   "imageTitle"
#define XtCImageTitle                   "ImageTitle"
#define XtNimageWidth                   "imageWidth"
#define XtCImageWidth                   "ImageWidth"
#define XtNimageHeight                  "imageHeight"
#define XtCImageHeight                  "ImageHeight"
#define XtNvariableAspect               "variableAspect"
#define XtCVariableAspect               "VariableAspect"
#define XtNimageLineLength              "imageLineLength"
#define XtCImageLineLength              "ImageLineLength"
#define XtNimageXstart                  "imageXstart"
#define XtCImageXstart                  "ImageXstart"
#define XtNimageYstart                  "imageYstart"
#define XtCImageYstart                  "ImageYstart"
#define XtNdisplayMethod              	"displayMethod"
#define XtCDisplayMethod                "DisplayMethod"
#define XtNprintValues			"printValues"
#define XtCPrintValues			"PrintValues"
#define XtNcolortab                    	"colortab"
#define XtCColortab                	"Colortab"
#define XtNcolorCells                   "colorCells"
#define XtCColorCells                	"ColorCells"
#define XtNcolorPtr                    	"colorPtr"
#define XtCColorPtr                	"ColorPtr"
#define XtNimageUpdate                  "imageUpdate"
#define XtCImageUpdate                	"ImageUpdate"
#define XtNfont1                	"font1"
#define XtCFont1			"Font1"
#define XtNfont2                	"font2"
#define XtCFont2			"Font2"
#define XtNfont3                	"font3"
#define XtCFont3			"Font3"
#define XtNfont4                	"font4"
#define XtCFont4			"Font4"
#ifndef XtNvisual
#define XtNvisual                	"visual"
#define XtCVisual			"Visual"
#endif
#define XtNscaletype               	"scaletype"
#define XtCScaletype               	"Scaletype"
#define XtNscalea               	"scalea"
#define XtCScalea               	"Scalea"
#define XtNscaleb               	"scaleb"
#define XtCScaleb               	"Scaleb"
#define XtNminval               	"minval"
#define XtCMinval               	"Minval"
#define XtNmaxval               	"maxval"
#define XtCMaxval               	"Maxval"
#define XtNrange                        "range"
#define XtCRange                        "Range"
#define XtNresizeCallback		"resizeCallback"
#define XtCResizeCallback		"ResizeCallback"
#define XtNmenubar                      "menubar"
#define XtCMenubar                      "Menubar"

/* Scale Methods   */

typedef enum {
  SCALE_NONE 	  = 0,
  SCALE_THRESHOLD = 1,
  SCALE_LINEAR 	  = 2
} scale_type;

/* Display Methods */

typedef enum {
  STATIC_GRAY_1	    = 0x0010,
  STATIC_GRAY_2     = 0x0020,
  STATIC_GRAY_4     = 0x0040,
  STATIC_GRAY_8	    = 0x0080,
  GRAY_SCALE_4	    = 0x1040,
  GRAY_SCALE_8	    = 0x1080,
  PSEUDO_COLOR_8    = 0x3080, /* 128-entry colormaps. */
  PSEUDO_COLOR_8_A  = 0x3080, /* 128-entry colormaps. */
  PSEUDO_COLOR_8_B  = 0x3081, /* 256-entry colormaps. */
  TRUE_COLOR_24	    = 0x4180,
  DIRECT_COLOR_24   = 0x5180, /* 128-entry composite colormaps. */
  DIRECT_COLOR_24_A = 0x5180, /* 128-entry composite colormaps. */
  DIRECT_COLOR_24_B = 0x5181  /* 256-entry composite colormaps. */
} display_method;

/* Callback Reason */

#define ACTIVATE		10
#define RESIZE 			39

/* declare specific ImageWidget class and instance datatypes */

typedef struct _ImageClassRec*	ImageWidgetClass;
typedef struct _ImageRec*	ImageWidget;

/* declare the class constant */

extern WidgetClass imageWidgetClass;

#ifndef IMAGE_OVERLAY
 typedef void (*resizefunc) _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
#endif
typedef char * (*imageprintffunc) _XITE_PARAMS(( char *stream, void *pix_value ));

typedef struct ipixelstruct
{
  char *pixelname;
  char *printformat;
  int  pixelsize;
  resizefunc resize;
  imageprintffunc printf;
  int txtlen;
  scale_type scaletype;            /* For scaling of color. */

  /* The Image Widget Initialization method may take default values from
   * the following, for the various pixel types.
   */
  float scalea, scaleb;     /* For scaling of color (old * scalea + scaleb) */
  float minval, maxval;

  /* One extra attribute in order to make a list of pixel structures
   * referenced by the Image class record's 'pixel_class' attribute.
   */
  struct ipixelstruct *next;
} ipixel, *ipixelptr;

#define ImageBytePixel          "ImageBytePixel"
#define ImageSignedBytePixel    "ImageSignedBytePixel"
#define ImageUnsignedShortPixel "ImageUnsignedShortPixel"
#define ImageShortPixel         "ImageShortPixel"
#define ImageLongPixel          "ImageLongPixel"
#define ImageFloatPixel         "ImageFloatPixel"
#define ImageComplexPixel       "ImageComplexPixel"
#define ImageDoublePixel        "ImageDoublePixel"
#define ImageDoubleComplexPixel "ImageDoubleComplexPixel"

typedef struct 
{
  int reason;
  XEvent *event;
  struct imagestruct *img;
  struct bandstruct *band;
  int x, y;
  ImageData data;
  ipixelptr pixtype;
  float zoom, magnification;
  int xpan, ypan;
} ImageCallbackRec, *ImageCallback; 


/*
  The rest of this  header-file is produced automatically (at least in part)
  by makeHeader of BLAB, Ifi, UiO.
*/



extern void ImageCopyImagePart  _XITE_PARAMS(( ImageWidget widto, ImageWidget widfrom ));
extern void ImageCopyResources  _XITE_PARAMS(( ImageWidget widto, ImageWidget widfrom ));
extern void ImageGetResources _XITE_PARAMS(( ImageWidget wid, ArgList *args, Cardinal *num_args ));
extern void ImageGetImageResources _XITE_PARAMS(( ImageWidget wid, ArgList *args, Cardinal *num_args ));
extern void ImageError  _XITE_PARAMS(( Widget wid, int messnr ));
extern void ImageGetZoom  _XITE_PARAMS(( ImageWidget wid, float *zoom, int *xpan, int *ypan, double *factor ));
extern int ImageGetZoomAll  _XITE_PARAMS(( Widget wid ));
extern int ImageHasColormap  _XITE_PARAMS(( ImageWidget wid ));
extern void ImageInstallPixelType  _XITE_PARAMS(( ipixelptr pix ));
extern void ImageKill  _XITE_PARAMS(( Widget wid, XEvent *event, String *params, Cardinal *num_params ));
extern int ImagePlanes  _XITE_PARAMS(( ImageWidget wid ));
extern ImageData ImagePosXYtoAdr  _XITE_PARAMS(( ImageWidget wid, int x, int y, int options ));
extern int ImagePosXtoScreen  _XITE_PARAMS(( ImageWidget wid, int x, int right ));
extern int ImagePosYtoScreen  _XITE_PARAMS(( ImageWidget wid, int y, int bottom ));
extern int ImageWidthToScreen _XITE_PARAMS(( ImageWidget wid, int width, int maximum ));
extern int ImageHeightToScreen _XITE_PARAMS(( ImageWidget wid, int height, int maximum ));
extern void ImageRedisplay  _XITE_PARAMS(( ImageWidget wid, update_status update ));
extern int ImageRedisplayArea  _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageRedisplayScreen  _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern ImageData ImageScreenXYtoAdr  _XITE_PARAMS(( ImageWidget wid, int x, int y, int options ));
extern int ImageScreenXtoPos  _XITE_PARAMS(( ImageWidget wid, int x ));
extern int ImageScreenYtoPos  _XITE_PARAMS(( ImageWidget wid, int y ));
extern int ImageScreenToWidth _XITE_PARAMS(( ImageWidget wid, int width ));
extern int ImageScreenToHeight _XITE_PARAMS(( ImageWidget wid, int height ));
extern void ImageSetZoom  _XITE_PARAMS(( ImageWidget wid, float zoom, int x, int y ));
extern int ImageSetZoomAll  _XITE_PARAMS(( Widget wid, int on ));
extern void ImageTranslateZoomPanArgs  _XITE_PARAMS(( ImageWidget wid, char *zoomarg, char *panxarg, char *panyarg, int mousex, int mousey, float *zoom, int *xpan, int *ypan ));
extern void ImageUpdateMinMax  _XITE_PARAMS(( Widget wid ));
extern Widget *imageWidgetList _XITE_PARAMS(( int *num_widgets ));

_XITE_CPLUSPLUS_END

#endif  /*_Image_h */
