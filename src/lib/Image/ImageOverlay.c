

/*C*

________________________________________________________________

        ImageOverlay.c
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

static char *Id = "$Id$, Otto Milvang, Blab, UiO";



/*F:ImageOverlayColor=ImageOverlay*/
/*F:ImageOverlayCopyImageOverlayPart=ImageOverlay*/
/*F:ImageOverlayCopyImagePart=ImageOverlay*/
/*F:ImageOverlayCopyResources=ImageOverlay*/
/*F:ImageOverlayGetResources=ImageOverlay*/
/*F:ImageOverlayDisplay=ImageOverlay*/
/*F:ImageOverlayDrawBox=ImageOverlay*/
/*F:ImageOverlayDrawLine=ImageOverlay*/
/*F:ImageOverlayDrawOrigo=ImageOverlay*/
/*F:ImageOverlayDrawPoint=ImageOverlay*/
/*F:ImageOverlayDrawRectangle=ImageOverlay*/
/*F:ImageOverlayFill=ImageOverlay*/
/*F:ImageOverlayPosXYtoAdr=ImageOverlay*/
/*F:ImageOverlayProtect=ImageOverlay*/
/*F:ImageOverlayRedisplay=ImageOverlay*/
/*F:ImageOverlayScreenXYtoAdr=ImageOverlay*/
/*F:ImageOverlay*

________________________________________________________________

		ImageOverlay
________________________________________________________________

Name:		ImageOverlay, ImageOverlayColor,
                ImageOverlayCopyImageOverlayPart, ImageOverlayCopyImagePart,
		ImageOverlayCopyResources, ImageOverlayGetResources,
		ImageOverlayDisplay, ImageOverlayDrawBox, ImageOverlayDrawLine,
		ImageOverlayDrawOrigo, ImageOverlayDrawPoint,
		ImageOverlayDrawRectangle, ImageOverlayFill,
		ImageOverlayPosXYtoAdr, ImageOverlayProtect,
		ImageOverlayRedisplay, ImageOverlayScreenXYtoAdr,
		- XITE X11 image overlay widget

Application header file: <xite/ImageOverlay.h>

Class header file: <xite/ImageOverlayP.h>

Class:          imageOverlayWidgetClass

Class Name:     ImageOverlay

Superclass:     Image

Description:    The ImageOverlay widget is part of the XITE (X-based Image
                Processing Tools and Environment) software. It is used by some
		of the display programs supplied with XITE.

		Most of the functionality is taken care of by the superclass
		Image widget (see 'Image(3)' for information on this).

		The ImageOverlay widget provides two extra facilities over
		the Image widget. They are the ability to display an overlay
		image and the ability to manipulate a region of interest (ROI).

		The overlay image is displayed by changing the color lookup
		for the underlying ordinary image (class Image) pixels. When
		the overlay data are changed, especially when some of the
		overlay is erased (overlay pixel value set to zero), the color
		lookup of the underlying ordinary image must reset. An
		application should do this by calling the class Image function
		ImageRedisplay, with the UPDATE_ZOOMPAN or UPDATE_NEWSIZE
		bit of the update parameter set.

                See the documentation on the main XITE display program
                'xshow(1)' as well as the XITE ximage toolkit 'ximage(3)' for
                more information on how this widget can be used in an
                application.

Resources:      Apart from the resources of the superclass:

                &XtNoverlay (class XtCOverlay)
		Type: Pointer, 
		Default: NULL

		This refers to the complete data structure for the
                band/channel of the overlay image. It does not necessarily
                refer to the first pixel (unless the data structure only
		consists of the pixels with no header information).

		&XtNprotectOverlay (class XtCProtectOverlay)
		Type: Boolean, 
		Default: "false"

		Whether or not to protect the overlay from being changed.

		&XtNdisplayOverlay (class XtCDisplayOverlay)
		Type: Boolean, 
		"true"

		Whether or not to show the overlay.

		&XtNoverlaytab (class XtCOverlaytab)
		Type: int, 
		Default: -1

		An index into an array of colortables. This resource
                identifies which colortable (in the array of colortables) is
                active for the image overlay part. The array is not local to
                the image widget, but must be supplied by the application.

                The default value (of -1) will make the widget Initialize
                method choose the value 0.

		&XtNoverlayColorCells (class XtCOverlayColorCells)
		Type: int, 
		Default: -1

		Number of colors actually used to display the overlay image.
		This will typically be different from the length of the X
		colormap used by the window, because the non-overlay part of
		the image will probably be using more colors than the overlay
		image.

                The default value (of -1) will make the widget Initialize
                method calculate a legal value based on the XtNdisplayMethod
                resource. Currently the only value used is 32.

		&XtNoverlayColorPtr (class XtCOverlayColorPtr)
		Type: Pointer, 
		Default: NULL

                A reference to an integer array of XtNoverlayColorCells
                elements.

                Each overlay pixel value is transformed into a new value
                before it is sent to the pixmap (which will be shown in a
                screen window). The original overlay pixel value is used as an
                index into an integer array. The value in the array cell
                becomes the pixel value in the pixmap.

                The integer transformation array (which always has 256
                elements) is identical to the XtNoverlayColorPtr array for the
                first XtNoverlayColorCells elements. If
		XtNoverlayColorCells < 256, then the XtNoverlayColorPtr array
		is repeated in the transformation array.

                With e.g. XtNoverlayColorCells equal to 32 and
		XtNoverlayColorPtr an array from 192 to 223, the integer
                transformation array contains the 256 values
                192,...223,192,...,223,.... The overlay pixelvalues will then
		be transformed (by the first 32 elements) into the range
		192,...,223.

		The default value (NULL) will make the widget Initialize
		method determine a value for this resource. With 
		the XtNdisplayMethod set to give "reduced-color" display,
		then this array will be a 32-element array with values from
		192 to 223.

		&XtNdrawColor (class XtCDrawColor)
		Type: int, 
		Default: NULL

		The color used to draw the overlay.

		&XtNdrawProc (class XtCDrawProc)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this callback list will be called by the
                'draw' action function described below.

		&XtNroiProc (class XtCRoiProc)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this callback list will be called by the
		'roi' action function when the first action-function argument
		is one of StartDrag, ExtendDrag, StopDrag, StopMove, Move,
		Drag.

		&XtNroiActivated (class XtCRoiActivated)
		Type: int, 
		Default: NULL

		True if a ROI is active (being changed).

		&XtNroiX (class XtCRoiX)
		Type: int, 
		Default: NULL

		Horizontal position of ROI, relative to screen.

		&XtNroiY (class XtCRoiY)
		Type: int, 
		Default: NULL

		Vertical position of ROI, relative to screen.

		&XtNroiWidth (class XtCRoiWidth)
		Type: int, 
		Default: NULL

		Horizontal width of ROI, relative to screen.

		&XtNroiHeight (class XtCRoiHeight)
		Type: int, 
		Default: NULL

		Vertical height of ROI, relative to screen.

		&XtNroiZoomPan (class XtCRoiZoomPan)
		Type: Boolean, 
		Default: "false"

		Whether or not ROI is to zoom and pan with the image.

		&XtNroiDisplay (class XtCRoiDisplay)
		Type: int, 
		Default: NULL

		Whether or not to show ROI.

		&XtNroiPermanent (class XtCRoiPermanent)
		Type: int, 
		Default: NULL

		Whether ROI should be permanently displayed (or only while
		being made).

		&XtNroiFill (class XtCRoiFill)
		Type: int, 
		Default: NULL

		Whether the inside of the ROI rectangle should be filled.

		&XtNroiXaspect (class XtCRoiXaspect)
		Type: int, 
		Default: NULL

		Nonzero if ROI should be square.

		&XtNroiYaspect (class XtCRoiYaspect)
		Type: int, 
		Default: NULL

		Nonzero if ROI should be square.

ImageOverlay actions:

                &draw

                Draw graphics in the overlay image. Some basic drawing
                routines are available:

                    &&draw(origo [, on/off])
                    Toggle origo (or on/off).

                    &&draw(point [, size])
                    Draw a point, or a cross of size "size".

                    &&draw(line)
                    Draw line from origo

                    &&draw(rectangle)
                    Draw rectancle from origo

                    &&draw(box)
                    Draw filled rectancle from origo

                    &&draw(fill)
                    Fill region (4-neighbour)

                    &&draw(clear)
                    Clear region (8-neighbour)

                    &&draw(graphic [, on/off])
                    Toggle overlay plane (or on/off)

                    &&draw(protect [, on/off])
                    Toggle protect switch (or on/off)

                    &&draw(color [, newval])
                    Increase draw color index by one (or set to newval).

                &roi
                Draw inverted region of interest. The available routines
                are

                    &&roi(StartDrag)
                    Set first corner of ROI

                    &&roi(Drag)     
                    Move second corner of ROI

                    &&roi(StopDrag) 
                    Set second corner of ROI

                    &&roi(StartMove)
                    Notify ROI

                    &&roi(Move)     
                    Move ROI

                    &&roi(StopDrag) 
                    Notify stop Move

                    &&roi(ExtendDrag)
                    Keep first corner of an existing ROI 

		&WMColormap
		WMColormap informs the window manager about desired colormaps.
		This will enable a visible menubar and visible popup menus in
		an image window also when the image is displayed with some
		combination of DirectColor visual and non-reduced-color mode
		display.

		    &&WMColormap(Enter)
		    Mouse pointer enters image.

		    &&WMColormap(Leave)
		    Mouse pointer leaves image.

		-&&
		-&The 'draw' and 'roi' action functions assign values to the
		attributes of the ImageOverlayCallbackRec structure and invoke
		the callback functions on the XtNdrawProc and XtNroiProc
		callback-lists respectively, with a reference to the
		ImageOverlayCallbackRec structure.

                The attributes of the ImageOverlayCallbackRec structure are

		&reason
		An integer with a value indicating which draw action was
		desired.

		&event
		An "XEvent *" reference to the event which triggered the
		action function.

		&img
		A "struct imagestruct *" reference to the XtNimageImage
		resource of the widget in which the event took place.

		&band
		A "struct bandstruct *" reference to the XtNimageBand
		resource of the widget in which the event took place.

		&overlay
		A "struct bandstruct *" reference to the XtNimageOverlay
		resource of the widget in which the event took place.

		&value
		An integer with the color index  used to draw the overlay
		plane.

		&x, y
		Two integers which represent the coordinates where one of
		the events ButtonPress, ButtonRelease or MotionNotify took
		place (or else where the previous event took place).

		&width, height
		Always zero.

		&xorigo, yorigo
		The private imageoverlay widget variabels xorigo and
		yorigo.

Default translation bindings:

		| <Key>Q:          kill()
		| <Key>C:          draw(color)
		| <Key>P:          draw(protect)
		| <Key>G:          draw(graphic)
		| <EnterWindow>:   WMColormap(Enter)
		| <LeaveWindow>:   WMColormap(Leave)
		| None<Key>Left:   zoom_pan(+0, -4, +0)
		| None<Key>Right:  zoom_pan(+0, +4, +0)
		| None<Key>Up:     zoom_pan(+0, +0, -4)
		| None<Key>Down:   zoom_pan(+0, +0, +4)
		| Shift<Key>Left:  zoom_pan(+0, -16, +0)
		| Shift<Key>Right: zoom_pan(+0, +16, +)
		| Shift<Key>Up:    zoom_pan(+0, +0, -16)
		| Shift<Key>Down:  zoom_pan(+0, +0, +16)
		| Ctrl<Key>Left:   zoom_pan(+0, -256, +0)
		| Ctrl<Key>Right:  zoom_pan(+0, +256, +0)
		| Ctrl<Key>Up:     zoom_pan(+0, +0, -256)
		| Ctrl<Key>Down:   zoom_pan(+0, +0, +256)
		| Shift Ctrl <Btn1Down>:
		|        zoom_pan(*8, MousePosC, MousePosC)
		| Shift Ctrl <Btn2Down>:
		|        zoom_pan(1, MousePosC, MousePosC)
		| Shift Ctrl <Btn3Down>:
		|        zoom_pan(/8, MousePosC, MousePosC)
		| Ctrl <Btn1Down>: zoom_pan(*2, MousePosC, MousePosC)
		| Ctrl <Btn2Down>: zoom_pan(0, MousePosC, MousePosC)
		| Ctrl <Btn3Down>: zoom_pan(/2, MousePosC, MousePosC)
		| None <Btn2Up>:   notify()
		| None <Btn3Up>:   notify()
		| <MouseMoved>:    notify()

ImageOverlay callbacks:

                There are two callback lists, identified by the resources
                XtNdrawProc and XtNroiProc. See the description of these
                resources.


Predefined pixeltype:
                Apart from the pixeltypes defined by the superclass:

                &overlaypixel
		| pixelname : ImageOverlayPixel,
		| pixelsize : 1,
		| resize    : ImageOverlayResize,
		| printf    : ImageOverlayPrintf,
		| txtlen    : 3,
		| scaletype : SCALE_NONE,
		| scalea    : 0.0,
		| scaleb    : 0.0,
		| next      : NULL,

Convenience routines:

                &Routines for coordinates of image and screen window.

                &&ImageOverlayScreenXYtoAdr
		| ImageData ImageOverlayScreenXYtoAdr(
		|    ImageOverlayWidget wid, int x, int y,
		|    int option );

		'ImageOverlayScreenXYtoAdr' returns a pointer to the pixel
		which has screen window coordinates (x,y). 'option' is not
		used.

		&&ImageOverlayPosXYtoAdr
		| ImageData ImageOverlayPosXYtoAdr(
		|    ImageOverlayWidget wid, int x, int y,
		|    int option );

		'ImageOverlayPosXYtoAdr' returns a pointer to the pixel which
		has image coordinates (x,y). 'option' is not used.

		&Routines for drawing in the overlay

		&&ImageOverlayProtect
		| int ImageOverlayProtect(
		|    ImageOverlayWidget wid, int state );
		
		Set new protect status of overlay plane. 'state' may be
                'OVERLAY_CLEAR', 'OVERLAY_SET' or 'OVERLAY_TOGGLE'.

		&&ImageOverlayFill
		| int ImageOverlayFill( ImageOverlayWidget wid,
		|    int x, int y, int search, int fill, int nc,
		|    int mode, update_status update );
		
		Fill an area with the pixelvalue 'fill'. Start in position
		'(x,y)'. Follow 'nc'. nc = 4 (4 connected neighbours) or nc = 8
		(8 connected neighbours).

                | 'mode' = 0, border fill.
		|             while (pix != search) fill; 
                | 'mode' = 1, flood fill.
		|             while (pix == search) fill;

                'update' != UPDATE_NONE will activate ImageRedisplayArea.

		&&ImageOverlayDrawRectangle
		| int ImageOverlayDrawRectangle(
		|    ImageOverlayWidget wid, int x1, int y1,
		|    int x2, int y2, int value,
		|    update_status update );
		
		Draw a filled rectangle in the overlay image at image
		coordinates '(x,y)' with size '(width, height)'. The rectangle
		is filled with 'value' pixels. The special value '-1' is used
		for drawing by inverting the pixels. 'update' != UPDATE_NONE
		will activate ImageRedisplayArea.

		&&ImageOverlayDrawPoint
		| int ImageOverlayDrawPoint(
		|    ImageOverlayWidget wid, int x, int y,
		|    int size, int value,
		|    update_status update );
		
		Draw a point in the overlay image at image coordinates (x,y).
                If size > 1, the point is written as a cross of size 'size'.
                The point is filled with 'value' pixels. The special value
                '-1' is used for drawing by inverting the pixels.
		'update' != UPDATE_NONE will activate ImageRedisplayArea.

		&&ImageOverlayDrawOrigo
		| int ImageOverlayDrawOrigo(
		|    ImageOverlayWidget wid, int origo, int x,
		|    int y );
		
		Set Boolean value origo and the coordinates to (x,y). Draw
                functions which need an origo will draw only if origo is true.

		&&ImageOverlayDrawLine
                | int ImageOverlayDrawLine(
                |    ImageOverlayWidget wid, int x1, int y1,
                |    int x2, int y2, int value,
		|    update_status update );
                
		Draw a line in the overlay image from image coordinates
                '(x1,y1)' to '(x2,y2)'. The line is written with 'value'
                pixels. The special value '-1' is used for drawing by
                inverting the pixels. 'update' != UPDATE_NONE will activate
                ImageRedisplayArea.

		&&ImageOverlayDrawBox
                | int ImageOverlayDrawBox(
                |    ImageOverlayWidget wid, int x1, int y1,
                |    int x2, int y2, int value,
		|    update_status update );
                
		Draw a filled rectangle in the overlay image at image
                coordinates '(x1,y1)' to '(x2, y2)'. The rectangle is filled
                with 'value' pixels. The special value '-1' is used for
                drawing by inverting the pixels. 'update' != UPDATE_NONE will
		activate ImageRedisplayArea.

		&&ImageOverlayColor
                | int ImageOverlayColor( ImageOverlayWidget wid,
                |    int col );
                
		Set new overlay color.

		&&ImageOverlayDisplay
                | int ImageOverlayDisplay(
                |    ImageOverlayWidget wid,
		|    update_status update );
                
		Set new display status of overlay plane. 'state' may be
                'OVERLAY_CLEAR', 'OVERLAY_SET' or 'OVERLAY_TOGGLE'.
		This determines whether or not the overlay image should be
		shown.

		&Miscellaneous routines:

		&&ImageOverlayRedisplay
		| void ImageOverlayRedisplay( Widget wid,
		|    update_status update );

		If 'update' != UPDATE_NONE, sets the private widget variable
		update equal to 'update'. Calls the resize and expose methods.
		For the expose method, the region and event arguments are
		NULL.

		&&ImageOverlayCopyImagePart
                | void ImageOverlayCopyImagePart(
                |    ImageOverlayWidget widto,
                |    ImageOverlayWidget widfrom );

		Sets the XtNimageImage resource in 'widto' equal to the
                XtNimageImage resource in 'widfrom'. No resizing or
                exposure is done.
		
		&&ImageOverlayCopyImageOverlayPart
                | void ImageOverlayCopyImageOverlayPart(
                |    ImageOverlayWidget widto,
                |    ImageOverlayWidget widfrom );

		Sets the image_overlay part of 'widto' equal to the
		image_overlay part of 'widfrom'. No resizing or exposure
		is done.

		&&ImageOverlayCopyResources
		| void ImageOverlayCopyResources(
		|    ImageOverlayWidget widto,
		|    ImageOverlayWidget widfrom );

		Sets the resources of the ImageOverlayPart of 'widto' equal
		to the resources of the ImageOverlayPart of 'widfrom'.

		&&ImageOverlayGetResources
                | void ImageOverlayGetResources(
		|    ImageOverlayWidget wid,
		|    ArgList *args, Cardinal *num_args );

                Returns a list of resource-value pairs for all the resources
                of 'wid'. If the function is called before the class is
                initialized, the resource list as specified in the class
                record is returned. If it is called after the class has been
                initialized, the returned list also contains the superclass
                resources.
		
		&&ImageOverlayGetImageOverlayResources
		| void ImageOverlayGetImageOverlayResources(
		|    ImageOverlayWidget wid, ArgList *args,
		|    Cardinal *num_args );

		Returns a list of resource-value pairs for the resources
		which are specific for an ImageOverlay widget (not including
		super-class resources).

See also:	'Image(3)', 'ImageFormat(3)', 'ImageRedisplay(3)',
                'xshow(1)', 'ximage(3)'

Author:		Otto Milvang
Revised:        Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/





/*I*

________________________________________________________________

		Include files and definitions
________________________________________________________________

*/

/* To get the typdef for resizefunc from ImageOverlay.h, not from Image.h. */

#define IMAGE_OVERLAY 1

#include <xite/includes.h>
#include <ctype.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <xite/ImageOverlay.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>
#include <xite/Visual.h>
#include <xite/ImageOverlayP.h>
#include <xite/ImageFormat.h>
#include "ImageOverlayPix.h"
#include <xite/message.h>
#include <xite/debug.h>

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/* Linearly increasing table of 32 elements from 192 to 223. The table is used
 * as XtNoverlayColorPtr resource for ImageOverlay widgets. */

static int tab32_s[] = {  
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
};

typedef struct
{
  ImageData data;
  int w, h, ll, xs, ys;
  int searchnr, fillnr, neighb, regionsize;
  int xmin, xmax, ymin, ymax, mode;
} filltype;

static int fillsegment ( filltype *info, int x, int y, int xb, int xe, int down );
static int floodsegment ( filltype *info, int x, int y, int xb, int xe, int down );



/*L*

________________________________________________________________

		Global data structure
________________________________________________________________

*/

static void ClassPartInitialize ( WidgetClass wc );
static void ImageOverlayDraw ( Widget wid, XEvent *event, String *params, Cardinal *num_params );
static void ImageRoiDrag ( ImageOverlayWidget iw, XEvent *event, int value );
static void ImageRoiDraw ( ImageOverlayWidget iw, int setmode );
static void ImageRoiExtend ( ImageOverlayWidget iw, XEvent *event, int value );
static void ImageRoiMove ( ImageOverlayWidget iw, XEvent *event, int value );
static void ImageRoiProc ( Widget iw, XEvent *event, String *params, Cardinal *num_params );
static void ImageRoiStartDrag ( ImageOverlayWidget iw, XEvent *event, int value );
static void ImageRoiStartMove ( ImageOverlayWidget iw, XEvent *event, int value );
static void ImageRoiStop ( ImageOverlayWidget iw, XEvent *event, int value );
static void Initialize ( Widget request, Widget new, ArgList args, Cardinal *num_args );
static void ROI_callback ( ImageOverlayWidget iw, XEvent *event, int value );
static void Redisplay ( Widget wid, XEvent *event, Region region );
static void Resize ( Widget wid );
static Boolean SetValues ( Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args );

static XtActionsRec actions[] =
{
  /* {name, procedure},*/
  {"draw", ImageOverlayDraw},
  {"roi",  ImageRoiProc},
};


static char translations[] =
"<Key>Q:             	kill()                             \n\
 <Key>C:             	draw(color)                        \n\
 <Key>P:             	draw(protect)                      \n\
 <Key>G:             	draw(graphic)                      \n\
 <EnterWindow>:         WMColormap(Enter)                  \n\
 <LeaveWindow>:         WMColormap(Leave)                  \n\
 None<Key>Left:		zoom_pan(+0, -4, +0)               \n\
 None<Key>Right:	zoom_pan(+0, +4, +0)               \n\
 None<Key>Up:		zoom_pan(+0, +0, -4)               \n\
 None<Key>Down:		zoom_pan(+0, +0, +4)               \n\
 Shift<Key>Left:	zoom_pan(+0, -16, +0)              \n\
 Shift<Key>Right:	zoom_pan(+0, +16, +)               \n\
 Shift<Key>Up:		zoom_pan(+0, +0, -16)              \n\
 Shift<Key>Down:	zoom_pan(+0, +0, +16)              \n\
 Ctrl<Key>Left:		zoom_pan(+0, -256, +0)             \n\
 Ctrl<Key>Right:	zoom_pan(+0, +256, +0)             \n\
 Ctrl<Key>Up:		zoom_pan(+0, +0, -256)             \n\
 Ctrl<Key>Down:		zoom_pan(+0, +0, +256)             \n\
 Shift Ctrl <Btn1Down>:	zoom_pan(*8, MousePosC, MousePosC) \n\
 Shift Ctrl <Btn2Down>: zoom_pan(1, MousePosC, MousePosC)  \n\
 Shift Ctrl <Btn3Down>: zoom_pan(/8, MousePosC, MousePosC) \n\
 Ctrl <Btn1Down>:       zoom_pan(*2, MousePosC, MousePosC) \n\
 Ctrl <Btn2Down>:       zoom_pan(0, MousePosC, MousePosC)  \n\
 Ctrl <Btn3Down>:       zoom_pan(/2, MousePosC, MousePosC) \n\
 None <Btn2Up>:      	notify()                           \n\
 None <Btn3Up>:      	notify()                           \n\
 <MouseMoved>:       	notify()";




static XtResource resources[] = {
#define ioffset(field) XtOffsetOf(ImageOverlayRec, image_overlay.field)
#define coffset(field) XtOffsetOf(ImageOverlayRec, core.field)
    /* {name, class, type, size, ioffset, default_type, default_addr}, */
    { XtNoverlay, XtCOverlay, XtRPointer, sizeof(struct bandstruct *),
	  ioffset(overlay), XtRPointer, NULL },
    { XtNoverlaytab, XtCOverlaytab, XtRInt, sizeof(int),
          ioffset(overlaytab), XtRImmediate, (XtPointer) -1 },
    { XtNprotectOverlay, XtCProtectOverlay, XtRBoolean, sizeof(Boolean),
	  ioffset(protect), XtRString, (XtPointer) "False" },
    { XtNdisplayOverlay, XtCDisplayOverlay, XtRBoolean, sizeof(Boolean),
	  ioffset(odisplay), XtRString, (XtPointer) "True" },
    { XtNoverlayColorCells, XtCOverlayColorCells, XtRInt, sizeof(int),
	  ioffset(cells), XtRImmediate, (XtPointer) -1 },
    { XtNoverlayColorPtr, XtCOverlayColorPtr, XtRPointer, sizeof(int *),
	  ioffset(colorptr), XtRPointer, NULL },
    { XtNdrawColor, XtCDrawColor, XtRInt, sizeof(int),
          ioffset(draw_value), XtRInt, NULL },
    { XtNdrawProc, XtCDrawProc, XtRCallback, sizeof(XtCallbackList),
          ioffset(draw_action), XtRCallback, NULL },
    { XtNroiProc, XtCRoiProc, XtRCallback, sizeof(XtCallbackList),
          ioffset(roi_action), XtRCallback, NULL },
    { XtNroiActivated, XtCRoiActivated, XtRInt, sizeof(int),
          ioffset(activated), XtRInt,  NULL },
    { XtNroiX, XtCRoiX, XtRInt, sizeof(int),
          ioffset(x), XtRInt,  NULL },
    { XtNroiY, XtCRoiY, XtRInt, sizeof(int),
          ioffset(y), XtRInt,  NULL },
    { XtNroiWidth, XtCRoiWidth, XtRInt, sizeof(int),
          ioffset(width), XtRInt,  NULL },
    { XtNroiHeight, XtCRoiHeight, XtRInt, sizeof(int),
          ioffset(height), XtRInt,  NULL },
    { XtNroiZoomPan, XtCRoiZoomPan, XtRBoolean, sizeof(Boolean),
          ioffset(roi_zoom_pan), XtRString,  "False" },
    { XtNroiDisplay, XtCRoiDisplay, XtRInt, sizeof(int),
          ioffset(display), XtRInt,  NULL },
    { XtNroiPermanent, XtCRoiPermanent, XtRInt, sizeof(int),
          ioffset(permanent), XtRInt,  NULL },
    { XtNroiFill, XtCRoiFill, XtRInt, sizeof(int),
          ioffset(fill), XtRInt,  NULL },
    { XtNroiXaspect, XtCRoiXaspect, XtRInt, sizeof(int),
          ioffset(xaspect), XtRInt,  NULL },
    { XtNroiYaspect, XtCRoiYaspect, XtRInt, sizeof(int),
          ioffset(yaspect), XtRInt,  NULL },
#undef ioffset
#undef coffset
};

#define SuperClass ((ImageWidgetClass)&imageClassRec)


ImageOverlayClassRec imageOverlayClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &imageClassRec,
    /* class_name		*/	"ImageOverlay",
    /* widget_size		*/	sizeof(ImageOverlayRec),
    /* class_initialize		*/	NULL,
    /* class_part_initialize	*/	ClassPartInitialize,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	actions,
    /* num_actions		*/      XtNumber(actions),
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	NULL,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	translations,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },{
    /* composite_class fields */
    /* geometry_manager   	*/    	XtInheritGeometryManager,
    /* change_managed     	*/    	XtInheritChangeManaged,
    /* insert_child       	*/    	XtInheritInsertChild,
    /* delete_child       	*/    	XtInheritDeleteChild,
    /* extension          	*/    	NULL,
  },{
    /* image fields */
    /* count			*/	0,
    /* widget_list_length       */      0,
    /* widget_list              */      NULL,
    /* pixel class              */	NULL,
    /* zoom_all                 */      0,
  },{ 
    /* image overlay fields */
    /* dummy    	        */	0,
  }
};

WidgetClass imageOverlayWidgetClass = (WidgetClass)&imageOverlayClassRec;





/*L*

________________________________________________________________

		SetValues
________________________________________________________________

Name:		SetValues
Syntax:		
Description:    Downward chained
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

static int *deftab(int n)
{
  int *tab = NULL;

  switch(n) {
  case 32: tab = tab32_s; break;
  default: tab = NULL;    break;
  }

  return(tab);
}

static void check_color_resources(ImageOverlayWidget cw, ImageOverlayWidget nw)
{
  int color_count;
  ImageOverlayPart *niop;
  ImagePart *nip, *cip = NULL;

  nip  = &(nw->image);
  niop = &(nw->image_overlay);
  if (cw) cip = &(cw->image);

  if (cip && (nip->displaymethod != cip->displaymethod)) {
    /* Changing display method. */

    niop->cells    = -1;
    niop->colorptr = NULL;
  }

  color_count = MapEntriesOfVisual(nip->visual);

  if ((nip->displaymethod & 3) == 0) {
    /* Reduced-color display. */

    if (niop->cells < 0) niop->cells = 32;
  } else {
    /* Full-color display. */

    if (niop->cells < 0) niop->cells = 32;
  }

  niop->colorptr = niop->colorptr ? niop->colorptr : deftab(niop->cells);
  if (niop->overlaytab < 0) niop->overlaytab = 0;

  return;

} /* check_color_resources() */

static void set_values_overlay(ImageOverlayWidget ic, ImageOverlayWidget ir, ImageOverlayWidget in, Boolean *do_redisplay, update_status *update)
{
  ImageOverlayPart *iopc, *iopn;
  ImagePart *ipn;

  iopc = &(ic->image_overlay);
  iopn = &(in->image_overlay);
  ipn  = &(in->image);

  if (iopc->overlay != iopn->overlay) {
    *do_redisplay = 1;
    *update |= UPDATE_NEWSIZE;
    iopn->data = ImageAddress((IBAND) iopn->overlay);
    iopn->line_length = 
      ImageLineLength((IBAND) iopn->overlay);
  }

  if (iopn->odisplay   != iopc->odisplay) {
    *do_redisplay = 1;
    ipn->updatex = 0;
    ipn->updatey = 0;
    ipn->updatew = ipn->ximg->width;
    ipn->updateh = ipn->ximg->height;
    *update |= UPDATE_REDISPLAY;
  }

  if (iopn->cells      != iopc->cells      ||
      iopn->colorptr   != iopc->colorptr) {
    *do_redisplay = 1;
    ipn->updatex = 0;
    ipn->updatey = 0;
    ipn->updatew = ipn->ximg->width;
    ipn->updateh = ipn->ximg->height;
    *update |= UPDATE_REDISPLAY;
    check_color_resources(ic, in);
  } if (iopn->overlaytab != iopc->overlaytab) {
    check_color_resources(ic, in);
  } else {
    /* Has the Image superclass widget instance changed its color resources in
     * such a way that it affects the ImageOverlay widget?
     * Or has the Image superclass widget been redrawn so that the ImageOverlay
     * part also should be redrawn?
     */

    ImagePart *nImPart = &(in->image);
    ImagePart *cImPart = &(ic->image);

    if (nImPart->displaymethod != cImPart->displaymethod ||
	nImPart->colorcells    != cImPart->colorcells) {
      ipn->updatex = 0;
      ipn->updatey = 0;
      ipn->updatew = ipn->ximg->width;
      ipn->updateh = ipn->ximg->height;
      *update |= UPDATE_REDISPLAY;
      check_color_resources(ic, in);
    }

    if (nImPart->colortab != cImPart->colortab) {
      *do_redisplay = 1;
      ipn->updatex = 0;
      ipn->updatey = 0;
      ipn->updatew = ipn->ximg->width;
      ipn->updateh = ipn->ximg->height;
      *update |= UPDATE_REDISPLAY;
    }
  }

  return;

} /* set_values_overlay() */

static void set_values_roi(ImageOverlayWidget ic, ImageOverlayWidget in, Boolean *do_redisplay, update_status *update)
{
  ImageOverlayPart *iopc, *iopn;

  iopc = &(ic->image_overlay);
  iopn = &(in->image_overlay);

  if (iopn->width      != iopc->width      ||
      iopn->height     != iopc->height     ||
      iopn->x          != iopc->x          ||
      iopn->y          != iopc->y) {
    ImageWidget iw = (ImageWidget) in;

    *do_redisplay = 1;

    iopn->x_im      = ImageScreenXtoPos(iw, iopn->x);
    iopn->x         = ImagePosXtoScreen(iw, iopn->x_im, 0);

    iopn->y_im      = ImageScreenYtoPos(iw, iopn->y);
    iopn->y         = ImagePosYtoScreen(iw, iopn->y_im, 0);

    iopn->width_im  = ImageScreenToWidth(iw, iopn->width);
    iopn->width     = ImageWidthToScreen(iw, iopn->width_im, 1);

    iopn->height_im = ImageScreenToHeight(iw, iopn->height);
    iopn->height    = ImageHeightToScreen(iw, iopn->height_im, 1);
  }

  if (iopn->fill       != iopc->fill       ||
      iopn->permanent  != iopc->permanent  ||
      iopn->draw_value != iopc->draw_value ||
      iopn->display    != iopc->display) {
    *do_redisplay = 1;
  }

  return;

} /* set_values_roi() */

static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
  ImageOverlayWidget ic = (ImageOverlayWidget) current;
  ImageOverlayWidget ir = (ImageOverlayWidget) request;
  ImageOverlayWidget in = (ImageOverlayWidget) new;
  
  Display *dpy;
  int screen;
  update_status update = UPDATE_NONE;
  Boolean do_redisplay = FALSE;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: SetValues");
  FPRINTF3("    Set Overlay Values %10.2f %10.2f \n",
	   in->image.minval, in->image.maxval);

  dpy    = XtDisplay(new);
  screen = DefaultScreen(dpy);

  set_values_overlay(ic, ir, in, &do_redisplay, &update);
  set_values_roi(ic, in, &do_redisplay, &update);
  in->image.update = update;
  Resize(new);

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: SetValues");
  return(do_redisplay); /* do_redisplay==TRUE: Xt calls the expose method */
}



/*L*

________________________________________________________________

		Initialize
________________________________________________________________

Name:		Initialize
Syntax:		| static void Initialize(Widget request, Widget new,
                |    ArgList args, Cardinal *num_args)
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

static void ClassPartInitialize(WidgetClass wc)
{

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ClassPartInitialize");
  ImageInstallPixelType(OverlayPixel());
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ClassPartInitialize");
}

static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args)
{
  /* This method is downward chained (starting from Core). */

  ImageOverlayWidget iw = (ImageOverlayWidget) new;
  ImageOverlayPart *iop;
  ImagePart *ip;
  ImageClassPart *ic = &imageClassRec.image_class;
  ipixelptr pptr;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: Initialize");

  ip  = &(iw->image);
  iop = &(iw->image_overlay);

  iop->data        = iop->overlay ? ImageAddress((IBAND) iop->overlay) : NULL;
  iop->line_length = iop->overlay ?
    ImageLineLength((IBAND) iop->overlay) : ip->line_length;
  pptr = ic->pixel_class;
  while (pptr && strcmp(pptr->pixelname, ImageOverlayPixel))
    pptr = pptr->next;
  iop->pixtype = pptr;

  check_color_resources(NULL, iw);

  if (XtIsSubclass(new, imageOverlayWidgetClass)) {
    if (new->core.width == 0 || new->core.height == 0) {
      /* Store new sizes and then call the Resize method. */

      int w, h;

      w = ip->width;  /* BIFF band width. */
      h = ip->height; /* BIFF band height. */

      XtResizeWidget(new, w, h, 1);
    } else {
      /* Non-zero size already (from some resource setting). */
      Resize(new);
    }
  }

  if (new == NULL) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: Initialize");
    return;
  }

/*
  if (iop->x == 0)       iop->x      = 1;
  if (iop->y == 0)       iop->y      = 1;
*/

  if (iop->width == 0)   iop->width  = ip->width;
  if (iop->height == 0)  iop->height = ip->height;
  
  iop->x_im      = ImageScreenXtoPos((ImageWidget) new, iop->x);
  iop->y_im      = ImageScreenYtoPos((ImageWidget) new, iop->y);
  iop->width_im  = ImageScreenToWidth((ImageWidget) new, iop->width);
  iop->height_im = ImageScreenToHeight((ImageWidget) new, iop->height);
  iop->origo     = 0;
  iop->xorigo    = 0;
  iop->yorigo    = 0;

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: Initialize");
}




/*L*

________________________________________________________________

		Redisplay 
________________________________________________________________

Name:		Redisplay, ImageOverlayRedisplay
Syntax:		| static void Redisplay(Widget wid, XEvent *event,
                |    Region region);
		|
		| void ImageOverlayRedisplay(Widget wid, update_status update);
Description:    
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static void Redisplay(Widget wid, XEvent *event, Region region)
{
  /* This method (expose) is self-contained, but calls superclass
   * method. */

  ImageOverlayWidget iowid = (ImageOverlayWidget) wid;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: Redisplay");
  if (iowid->image_overlay.display) {
    int width_im, height_im, x_im, y_im;

    /* Clear old ROI. */
    ImageRoiDraw(iowid, ROI_TOGGLE);
    (*SuperClass->core_class.expose)((Widget) wid, event, region);

    if (iowid->image_overlay.roi_zoom_pan) {
      /* Also zoom and pan the roi. */
      width_im  = iowid->image_overlay.width_im;
      height_im = iowid->image_overlay.height_im;
      x_im      = iowid->image_overlay.x_im;
      y_im      = iowid->image_overlay.y_im;

      iowid->image_overlay.x = ImagePosXtoScreen((ImageWidget) wid, x_im, 0);
      iowid->image_overlay.y = ImagePosYtoScreen((ImageWidget) wid, y_im, 0);

      iowid->image_overlay.width  =
	ImageWidthToScreen((ImageWidget) wid, width_im, 1);
      iowid->image_overlay.height =
	ImageHeightToScreen((ImageWidget) wid, height_im, 1);
    }

    /* Display ROI. */
    ImageRoiDraw(iowid, ROI_TOGGLE);
  } else {
    (*SuperClass->core_class.expose)((Widget) wid, event, region);
  }
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: Redisplay");
}  

void ImageOverlayRedisplay(Widget wid, update_status update)
{
  ImageOverlayWidget iowid = (ImageOverlayWidget) wid;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayRedisplay");
  FPRINTF2("    update: %d..... \n", update);
  if (update != UPDATE_NONE) iowid->image.update = update;
  Resize(wid);
  Redisplay(wid, NULL, NULL);
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayRedisplay");
}

void ImageOverlayCopyImagePart(ImageOverlayWidget widto, ImageOverlayWidget widfrom)
{
  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayCopyImagePart");
  widto->image = widfrom->image;
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayCopyImagePart");
}

void ImageOverlayCopyResources(ImageOverlayWidget widto, ImageOverlayWidget widfrom)
{
  ImageOverlayPart tp, fp;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayCopyResources");

  tp = widto->image_overlay;
  fp = widfrom->image_overlay;

  tp.overlay      = fp.overlay;
  tp.overlaytab   = fp.overlaytab;
  tp.protect      = fp.protect;
  tp.odisplay     = fp.odisplay;
  tp.cells        = fp.cells;
  tp.colorptr     = fp.colorptr;
  tp.draw_value   = fp.draw_value;
  tp.draw_action  = fp.draw_action;
  tp.roi_action   = fp.roi_action;
  tp.activated    = fp.activated;
  tp.x            = fp.x;
  tp.y            = fp.y;
  tp.width        = fp.width;
  tp.height       = fp.height;
  tp.roi_zoom_pan = fp.roi_zoom_pan;
  tp.display      = fp.display;
  tp.permanent    = fp.permanent;
  tp.fill         = fp.fill;
  tp.xaspect      = fp.xaspect;
  tp.yaspect      = fp.yaspect;

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayCopyResources");

} /* ImageOverlayCopyResources() */

void ImageOverlayGetResources(ImageOverlayWidget wid, ArgList *args, Cardinal *num_args)
{
  XtResourceList resource_list;
  int i;

  ENTER_FUNCTION_DEBUG("Image.c: ImageOverlayGetResources");

  XtGetResourceList(imageOverlayWidgetClass, &resource_list, num_args);
  *args = (Arg *) malloc(sizeof(Arg) * (*num_args));

  for (i = 0; i < (*num_args); i++) {
    (*args)[i].name  = resource_list[i].resource_name;
  }
  XtGetValues((Widget) wid, (*args), *num_args);

  LEAVE_FUNCTION_DEBUG("Image.c: ImageOverlayGetResources");

  return;

} /* ImageOverlayGetResources() */

void ImageOverlayGetImageOverlayResources(ImageOverlayWidget wid, ArgList *args, Cardinal *num_args)
{
  ImageOverlayPart ip;
  int i;

  ENTER_FUNCTION_DEBUG("Image.c: ImageOverlayGetImageOverlayResources");

/*  *num_args = 20; */
  *num_args = 18;
  *args     = (ArgList) malloc(sizeof(Arg) * (*num_args));

  ip = wid->image_overlay;
  i  = 0;

  (*args)[i].name    = XtNoverlay;
  (*args)[i++].value = (XtArgVal) ip.overlay;
  (*args)[i].name    = XtNoverlaytab;
  (*args)[i++].value = (XtArgVal) ip.overlaytab;
  (*args)[i].name    = XtNprotectOverlay;
  (*args)[i++].value = (XtArgVal) ip.protect;
  (*args)[i].name    = XtNdisplayOverlay;
  (*args)[i++].value = (XtArgVal) ip.odisplay;
  (*args)[i].name    = XtNoverlayColorCells;
  (*args)[i++].value = (XtArgVal) ip.cells;
  (*args)[i].name    = XtNoverlayColorPtr;
  (*args)[i++].value = (XtArgVal) ip.colorptr;
  (*args)[i].name    = XtNdrawColor;
  (*args)[i++].value = (XtArgVal) ip.draw_value;
/*
  (*args)[i].name    = XtNdrawProc;
  (*args)[i++].value = (XtArgVal) ip.draw_action;
  (*args)[i].name    = XtNroiProc;
  (*args)[i++].value = (XtArgVal) ip.roi_action;
*/
  (*args)[i].name    = XtNroiActivated;
  (*args)[i++].value = (XtArgVal) ip.activated;
  (*args)[i].name    = XtNroiX;
  (*args)[i++].value = (XtArgVal) ip.x;
  (*args)[i].name    = XtNroiY;
  (*args)[i++].value = (XtArgVal) ip.y;
  (*args)[i].name    = XtNroiWidth;
  (*args)[i++].value = (XtArgVal) ip.width;
  (*args)[i].name    = XtNroiHeight;
  (*args)[i++].value = (XtArgVal) ip.height;
  (*args)[i].name    = XtNroiZoomPan;
  (*args)[i++].value = (XtArgVal) ip.roi_zoom_pan;
  (*args)[i].name    = XtNroiDisplay;
  (*args)[i++].value = (XtArgVal) ip.display;
  (*args)[i].name    = XtNroiPermanent;
  (*args)[i++].value = (XtArgVal) ip.permanent;
  (*args)[i].name    = XtNroiFill;
  (*args)[i++].value = (XtArgVal) ip.fill;
  (*args)[i].name    = XtNroiXaspect;
  (*args)[i++].value = (XtArgVal) ip.xaspect;
  (*args)[i].name    = XtNroiYaspect;
  (*args)[i++].value = (XtArgVal) ip.yaspect;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageOverlayGetImageOverlayResources");

  return;

} /* ImageOverlayGetImageOverlayResources() */

void ImageOverlayCopyImageOverlayPart(ImageOverlayWidget widto, ImageOverlayWidget widfrom)
{
  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayCopyImageOverlayPart");
  widto->image_overlay = widfrom->image_overlay;
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayCopyImageOverlayPart");
}






/*L*

________________________________________________________________

		Resize
________________________________________________________________

Name:		Resize
Syntax:		| static void Resize(wid)
                | Widget wid;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/



static void Resize(Widget wid)
{
  /* This method (resize) is self-contained, but calls superclass
   * method. */

  ImageOverlayWidget iowid = (ImageOverlayWidget) wid;
  ImagePart *ip = &(iowid->image);
  ImageOverlayPart *iop = &(iowid->image_overlay);

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: Resize");

  FPRINTF2("    reason: %d\n", ip->update);

  (*SuperClass->core_class.resize)(wid);

  if (!iop->data || !iop->odisplay) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: Resize");
    return;
  }

  if (ip->update == UPDATE_NONE) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: Resize");
    return;
  }

  if ((ip->update & UPDATE_REDISPLAY) == 0) {
    (iop->pixtype)->resize(iowid, 0, 0,
			   iowid->image.internal_width,
			   iowid->image.internal_height);
  } else {
    (iop->pixtype)->resize(iowid, ip->updatex, ip->updatey,
			   ip->updatew, ip->updateh);
  }

  if (((ip->update & UPDATE_RESIZE) == 0) &&
      (!iop->permanent)) iop->display = 0;

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: Resize");

} /* Resize() */



/*L:ImageOverlayScreenXYtoAdr*

________________________________________________________________

		ImageOverlayScreenXYtoAdr
________________________________________________________________

Name:		ImageOverlayScreenXYtoAdr
Syntax:		| ImageData ImageOverlayScreenXYtoAdr(wid, x, y, option)
                | ImageOverlayWidget wid;
		| int x, y, option;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/



ImageData ImageOverlayScreenXYtoAdr(ImageOverlayWidget wid, int x, int y, int option)
{
  int xa, ya;
  ImageData data;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayScreenXYtoAdr");

  data = (ImageData) wid->image_overlay.data;
  if (!data) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayScreenXYtoAdr");    
    return(NULL);
  }
  if (x == -1)
    {
      xa = 0;
    } else {
      if (x < 0 || x >= wid->image.internal_width) {
	LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayScreenXYtoAdr");    
	return(NULL);
      }
      xa = (wid->image.resize) ? (wid->image.xptr[x]) : x;
    }
  if (y < 0 || y >= wid->image.internal_height) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayScreenXYtoAdr");    
    return(NULL);
  }
  ya = (wid->image.resize) ? (wid->image.yptr[y]) : y;

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayScreenXYtoAdr");
  return((ImageData) (data + (wid->image_overlay.line_length) * ya
	+ xa * (wid->image_overlay.pixtype)->pixelsize));
}



/*L:ImageOverlayPosXYtoAdr*

________________________________________________________________

		ImageOverlayPosXYtoAdr
________________________________________________________________

Name:		ImageOverlayPosXYtoAdr
Syntax:		| ImageData ImageOverlayPosXYtoAdr(wid, x, y, option)
                | ImageOverlayWidget wid;
		| int x, y, option;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/



ImageData ImageOverlayPosXYtoAdr(ImageOverlayWidget wid, int x, int y, int option)
{
  ImageData data;
  data = (ImageData) wid->image_overlay.data;
  if (!data) return(NULL);
  x -= wid->image.xstart;
  y -= wid->image.ystart;

  if (x < 0 || x >= wid->image.width) return(NULL);
  if (y < 0 || y >= wid->image.height) return(NULL);
  return((ImageData) (data + (wid->image_overlay.line_length) * y
	+ x * (wid->image_overlay.pixtype)->pixelsize));
}



/*L:ImageOverlayDrawOrigo*

________________________________________________________________

		ImageOverlayDrawOrigo
________________________________________________________________

Name:		ImageOverlayDrawOrigo
Syntax:		| int ImageOverlayDrawOrigo(wid, origo, x, y)
                | ImageOverlayWidget wid;
                | int origo, x, y;
Description:	Set Boolean value origo and the coordinates to (x,y).
                draw(xxx) functions which needs an origo will draw
                only if origo is true. 
Return value:	
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayDrawOrigo(ImageOverlayWidget wid, int origo, int x, int y)
{
  int old;
  ImageOverlayPart *io = &wid->image_overlay;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawOrigo");

  old = io->origo;
  if (origo != OVERLAY_CLEAR && origo != OVERLAY_SET &&
      origo != OVERLAY_TOGGLE) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawOrigo");
    return(old);
  }
  if (origo == OVERLAY_TOGGLE) origo = 1 - io->origo;
  if (origo != io->origo)
    {
      io->xsetorigo = x;
      io->ysetorigo = y;
    }
  io->origo = origo;
  io->xorigo = x;
  io->yorigo = y;
  FPRINTF2("    Set origo %d\n", origo);

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawOrigo");
  return(old);
}




/*L:ImageOverlayDrawPoint*

________________________________________________________________

		ImageOverlayDrawPoint
________________________________________________________________

Name:		ImageOverlayDrawPoint
Syntax:		| int ImageOverlayDrawPoint(wid, x, y, size, value, update)
                | ImageOverlayWidget wid;
                | int x, y, size, value;
		| update_status update;
Description:	Draw a point in the overlay image at image 
                coordinates (x,y). If size > 1, the point is
		written as a cross of size 'size'. The point 
		is filled with 'value' pixels. The special value
		'-1' is used for drawing by inverting the pixels.
		'update' != UPDATE_NONE will activate ImageRedisplayArea.
Return value:	
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayDrawPoint(ImageOverlayWidget wid, int x, int y, int size, int value, update_status update)
{
  ImageData pix, data;
  int u1, i, xmin, xmax, ymin, ymax, xs, ys, ll, protect;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawPoint");
  FPRINTF4("    ImageDrawPoint: (%4d %4d), value: %d", x, y, value);

  if (size > 0) size--; else size = 0;

  u1 = FALSE;
  data = wid->image_overlay.data;
  protect = wid->image_overlay.protect;
  if (!data) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawPoint");
    return(u1);
  }
  xs = wid->image.xstart;
  ys = wid->image.ystart;
  ll = wid->image_overlay.line_length;
  x -= xs;
  y -= ys;
  xmin = x - size < 0 ? 0 : x - size;
  ymin = y - size < 0 ? 0 : y - size;
  xmax = x + size >= wid->image. width  ? wid->image.width-1  : x + size;
  ymax = y + size >= wid->image. height ? wid->image.height-1 : y + size;

  pix = data + ymin*ll +x;
  for (i=ymin; i<=ymax; i++, pix+=ll)
    if ((value != *pix) && (!(*pix) || !protect)) {
      *pix = (value == -1) ? ~(*pix) : value;
      u1 = TRUE;
    }
  pix = data + y*ll +xmin;
  for (i=xmin; i<=xmax; i++, pix++)
    if ((value != *pix) && (!(*pix) || !protect)) {
      *pix = (value == -1) ? ~(*pix) : value;
      u1 = TRUE;
    }
  if (update != UPDATE_NONE && u1 && wid->image_overlay.odisplay)
    ImageRedisplayArea((ImageWidget) wid, xmin+xs, ymin+ys,
		       2*size+1, 2*size+1);
  
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawPoint");
  return(u1);
}	



/*L:ImageOverlayDrawLine*

________________________________________________________________

		ImageOverlayDrawLine
________________________________________________________________

Name:		ImageOverlayDrawLine
Syntax:		| int ImageOverlayDrawLine(wid, x1, y1, x2, y2, value, update)
                | ImageOverlayWidget wid;
                | int x, y, x1, y1, x2, y2, value;
		| update_status update;
Description:	Draw a line in the overlay image from image 
                coordinates (x1,y1) to (x2,y2). The line 
		is written with 'value' pixels. The special value
		'-1' is used for drawing by inverting the pixels.
		'update' != UPDATE_NONE will activate ImageRedisplayArea.
Return value:	
Author:		Otto Milvang
________________________________________________________________

*/


#define iabs(a) ((a) < 0 ? -(a) : (a))

int ImageOverlayDrawLine(ImageOverlayWidget wid, int x1, int y1, int x2, int y2, int value, update_status update)
{
  double s1, f;
  int x, y, xl, yl, cl, w, xs, ys, ll, xmin, ymin, lstep, step;
  int protect, len, tl, u;
  ImageData pix, data;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawLine");
  FPRINTF3("    (x1, y1): (%4d %4d)", x1, y1);
  FPRINTF3(", (x2, y2): (%4d %4d).\n", x2, y2);

  u       = FALSE;
  data    = wid->image_overlay.data;
  protect = wid->image_overlay.protect;
  if (!data) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawLine");
    return(u);
  }
  xs = wid->image.xstart;
  ys = wid->image.ystart;
  ll = wid->image_overlay.line_length;
  x1 -= xs;
  x2 -= xs;
  y1 -= ys;
  y2 -= ys;
  xmin = x1 < x2 ? x1 : x2;
  ymin = y1 < y2 ? y1 : y2;

  xl = iabs(x2-x1)+1;
  yl = iabs(y2-y1)+1;
  if (xl <=2 && yl <=2) {
    /* Short line. */

    for (pix = data + y1 * ll + x1, x=0; 
	 x<=1; x++, pix = data + y2 * ll + x2)
      if ((value != *pix)  && (!(*pix) || !protect)) {
	/* value is different from original pixel-value and nonzero or not
	 * protected. */
	*pix = value == -1 ? ~(*pix) : value;
	u = TRUE;
      }

    if (update != UPDATE_NONE && u && wid->image_overlay.odisplay) 
      ImageRedisplayArea((ImageWidget) wid, xmin+xs, ymin+ys, xl, yl);
    
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawLine");
    return(u);
  }

  if (x1 == x2 || y1 == y2) {
    /* Horizontal or vertical line. */

    if (x1 == x2) step = ll; else step = 1;
    len = xl > yl ? xl : yl;
    pix = data + ymin * ll + xmin;

    for (x=0; x<len; x++, pix+=step)
      if ((value != *pix)  && (!(*pix) || !protect)) {
	*pix = value == -1 ? ~(*pix) : value;
	u = TRUE;
      }

    if (update != UPDATE_NONE && u && wid->image_overlay.odisplay) 
      ImageRedisplayArea((ImageWidget) wid, xmin+xs, ymin+ys, xl, yl);
    
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawLine");
    return(TRUE);
  }
  
  if (xl > yl) {
    /* Horizontal extent larger than vertical extent. */

    f = (double) xl / (double) yl;
    tl = xl;
    step = 1;
    lstep = (y2-y1)*(x2-x1) > 0 ? ll : -ll;
    pix = data + (xmin == x1 ? y1 : y2)*ll + xmin;
    len = yl;
    
  } else {
    f = (double) yl / (double) xl;
    tl = yl;
    step = ll;
    lstep = (y2-y1)*(x2-x1) > 0 ? 1 : -1;
    pix = data + ymin*ll + (ymin==y1 ? x1 : x2);
    len = xl;
  }
  cl = 0;
  s1 = 0.5;

  for (x = 0; x<len; x++, pix +=lstep) {
    s1 += f;
    w = (int) s1 - cl;
    if (x == len-1) w = tl-cl;

    for (y=0; y<w; y++, pix+=step)	
      if ((value != *pix)  && (!(*pix) || !protect)) {
	*pix = value == -1 ? ~(*pix) : value;
	u = TRUE;
      }
    cl += w;
  }

  if (update != UPDATE_NONE && u && wid->image_overlay.odisplay) 
    ImageRedisplayArea((ImageWidget) wid, xmin+xs, ymin+ys, xl, yl);
  
  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDrawLine");
  return(TRUE);
}







/*L:ImageOverlayDrawBox*

________________________________________________________________

		ImageOverlayDrawBox
________________________________________________________________

Name:		ImageOverlayDrawBox
Syntax:		| int ImageOverlayDrawBox
                |   (wid, x1, y1, x2, y2, value, update)
                | ImageOverlayWidget wid;
                | int x1, y2, x2, y2, value;
		| update_status update;
Description:	Draw a filled rectangle in the overlay image at image 
                coordinates (x1,y1) to (x2, y2).
		The rectangle  is filled with 'value' pixels. 
		The special value '-1' is used for drawing by inverting 
		the pixels. 'update' != UPDATE_NONE will activate
		ImageRedisplayArea.
Return value:	
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayDrawBox(ImageOverlayWidget wid, int x1, int y1, int x2, int y2, int value, update_status update)
{
  ImageData data, pix;
  int xp, yp, xs, ys, ll, xmin, xmax, ymin, ymax, protect;

  data = wid->image_overlay.data;
  protect = wid->image_overlay.protect;
  if (!data) return(1);
  xs = wid->image.xstart;
  ys = wid->image.ystart;
  ll = wid->image_overlay.line_length;

  if (x1 > x2) { xmin = x1; xmax = x2;} else { xmin = x2; xmax = x1;}
  if (y1 > y2) { ymin = y1; ymax = y2;} else { ymin = y2; ymax = y1;}
  xmin -= xs;
  ymin -= ys;
  xmax -= xs;
  ymax -= ys;
  if (xmax < 0) xmax=0; 
  if (ymax < 0) ymax=0;
  if (xmin >= wid->image.width)  xmin = wid->image.width  - 1;
  if (ymin >= wid->image.height) ymin = wid->image.height - 1;
  x1 = xmax; x2 = xmin; y1 = ymax; y2 = ymin;

  for (yp = y1; yp <= y2; yp++)
    {
      pix = data + ll * yp + x1;
      for (xp = x1; xp <=x2; xp++, pix++)
	{
	  if (value == -1) *pix = ~(*pix); else
	    if ((value != *pix) && (!(*pix) || !protect))
	      {
		*pix = value;
		if (xp <xmin) xmin = xp;
		if (xp >xmax) xmax = xp;
		if (yp <ymin) ymin = yp;
		if (yp >ymax) ymax = yp;
	      }
	}
    }

  if (update != UPDATE_NONE && !wid->image_overlay.odisplay)
    {
      if (value == -1)
	return(ImageRedisplayArea((ImageWidget) wid, x1 + xs, y1 + ys,
				  x2-x1+1, y2-y1+1));
      else
	  if (xmin <= xmax || ymin <= ymax)
	    return(ImageRedisplayArea((ImageWidget)
	      wid, xmin+xs, ymin+ys, xmax-xmin+1, ymax-ymin+1));
    } 
  return(0);
}





/*L:ImageOverlayDrawRectangle*

________________________________________________________________

		ImageOverlayDrawRectangle
________________________________________________________________

Name:		ImageOverlayDrawRectangle
Syntax:		| int ImageOverlayDrawRectangle
                |   (wid, x, y, width, height, value, update)
                | ImageOverlayWidget wid;
                | int x, y, width, height, value;
		| update_status update;
Description:	Draw a filled rectangle in the overlay image at image 
                coordinates (x,y) with size (width, height).
		The rectangle  is filled with 'value' pixels. 
		The special value '-1' is used for drawing by inverting 
		the pixels. 'update' != UPDATE_NONE will activate
		ImageRedisplayArea.
Return value:	
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayDrawRectangle(ImageOverlayWidget wid, int x1, int y1, int x2, int y2, int value, update_status update)
{
  ImageData data;
  int xs, ys, ll, xmin, xmax, ymin, ymax, protect;

  data = wid->image_overlay.data;
  protect = wid->image_overlay.protect;
  if (!data) return(1);
  xs = wid->image.xstart;
  ys = wid->image.ystart;
  ll = wid->image_overlay.line_length;

  if (x1 > x2) { xmin = x1; xmax = x2;} else { xmin = x2; xmax = x1;}
  if (y1 > y2) { ymin = y1; ymax = y2;} else { ymin = y2; ymax = y1;}
  x1 = xmax; x2 = xmin; y1 = ymax; y2 = ymin;

  ImageOverlayDrawLine(wid, x1, y1, x2, y1, value, update);
  if (y1 == y2) return(0);
  ImageOverlayDrawLine(wid, x1, y2, x2, y2, value, update);
  if (y2-y1 == 1) return(0);
  ImageOverlayDrawLine(wid, x1, y1+1, x1, y2-1, value, update);
  ImageOverlayDrawLine(wid, x2, y1+1, x2, y2-1, value, update);
  return(0);
}





/*L:ImageOverlayFill*

________________________________________________________________

		ImageOverlayFill
________________________________________________________________

Name:		ImageOverlayFill
Syntax:		| ImageOverlayFill(wid, x, y, search, fill, nc, mode, update)
                | ImageOverlayWidget wid;
		| int x, y, search, fill, nc;
Description:	Fill an area with 'fill' pixels. Start in position (x,y).
                Follow 'nc' nc = 4  (4 connected neighbours) or
		nc = 8 (8 connected neighbours).
		| 'mode' = 0, border fill. while (pix != search) fill; 
		| 'mode' = 1, flood fill.  while (pix == search) fill;
		'update' != UPDATE_NONE will activate ImageRedisplayArea.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayFill(ImageOverlayWidget wid, int x, int y, int search, int fill, int nc, int mode, update_status update)
{
  filltype info;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayFill");
  FPRINTF3("    (x, y):  %3d %3d", x, y);
  FPRINTF3(", search: %3d fill: %3d\n", search, fill);
  FPRINTF3("    mode: %3d update: %3d\n", mode, update);

  info.data = wid->image_overlay.data;
  if (!info.data) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayFill");
    return(1);
  }
  info.xs = wid->image.xstart;
  info.ys = wid->image.ystart;
  info.ll = wid->image_overlay.line_length;
  info.w = wid->image.width;
  info.h = wid->image.height;
  x -= info.xs;
  y -= info.ys;

  info.searchnr = search;
  info.fillnr = fill;
  info.neighb = ( nc == 8 ) ? 1 : 0;        
  info.regionsize  = 0;
  info.mode = mode;
  info.xmin = x;
  info.xmax = x;
  info.ymin = y;
  info.ymax = y;


  switch(mode)
    {
    case 0: fillsegment(&info, x, y, x, x, -1); break;
    case 1: floodsegment(&info, x, y, x, x, -1); break;
    }
  if (info.regionsize && update != UPDATE_NONE && wid->image_overlay.odisplay)
    ImageRedisplayArea((ImageWidget) wid, info.xmin + info.xs,
		       info.ymin + info.ys, info.xmax - info.xmin +1,
		       info.ymax - info.ymin +1);

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayFill");
  return(info. regionsize);
}




#define REL !=
 		
static int fillsegment(filltype *info, int x, int y, int xb, int xe, int down)
{  							
    							
  int lx, x1, x2, ww, ll, searchnr, fillnr, r, y2, la, lb; 				
  ImageData data, ptr; 						
    			
  data = info->data;		
  ww = info->w-1;
  ll = info->ll;
  searchnr = info->searchnr;
  fillnr = info->fillnr;
  ptr = data + y*ll;
  
  if (!(ptr[x] REL searchnr)) return(0);
  for(x1=x;   x1 >=  0 && ptr[x1] REL searchnr; x1--) ptr[x1] = fillnr;
  for(x2=x+1; x2 <= ww && ptr[x2] REL searchnr; x2++) ptr[x2] = fillnr;
  x1 ++; x2--;
							
  info->regionsize += (x2-x1+1);	
  if ( x1 < info->xmin ) info->xmin = x1;
  if ( x2  > info->xmax ) info->xmax = x2;
  if ( y > info->ymax ) info->ymax = y;
  if ( y < info->ymin ) info->ymin = y;

  if ( info->neighb && (x1 > 0 ) ) x1--;     		
  if ( info->neighb && (x2 < ww) ) x2++;

  for (r = -1; r<=1; r++)
    {
      y2 = (r ==  1) ? y+down : y-down;
      la = (r ==  0) ? xe : x1;
      lb = (r == -1) ? xb : x2;  
      if ( y2 >= 0 &&  y2 < info->h)
	{
	  ptr = data + y2*ll;
	  for ( lx = la; lx <= lb; lx++ )		
	    if (ptr[lx]  REL  searchnr)		
	      lx += fillsegment(info, lx, y2, x1-1, x2+1, y2-y); 	
	}
    }
  return(x2-x);					
}						       

#undef REL



#define REL ==
 		
static int floodsegment(filltype *info, int x, int y, int xb, int xe, int down)
{  							
    							
  int lx, x1, x2, ww, ll, searchnr, fillnr, r, y2, la, lb; 				
  ImageData data, ptr; 						
    			
  data = info->data;		
  ww = info->w-1;
  ll = info->ll;
  searchnr = info->searchnr;
  fillnr = info->fillnr;
  ptr = data + y*ll;
  
  if (!(ptr[x] REL searchnr)) return(0);
  for(x1=x;   x1 >=  0 && ptr[x1] REL searchnr; x1--) ptr[x1] = fillnr;
  for(x2=x+1; x2 <= ww && ptr[x2] REL searchnr; x2++) ptr[x2] = fillnr;
  x1 ++; x2--;
							
  info->regionsize += (x2-x1+1);	
  if ( x1 < info->xmin ) info->xmin = x1;
  if ( x2  > info->xmax ) info->xmax = x2;
  if ( y > info->ymax ) info->ymax = y;
  if ( y < info->ymin ) info->ymin = y;

  if ( info->neighb && (x1 > 0 ) ) x1--;     		
  if ( info->neighb && (x2 < ww) ) x2++;

  for (r = -1; r<=1; r++)
    {
      y2 = (r ==  1) ? y+down : y-down;
      la = (r ==  0) ? xe : x1;
      lb = (r == -1) ? xb : x2;  
      if ( y2 >= 0 &&  y2 < info->h)
	{
	  ptr = data + y2*ll;
	  for ( lx = la; lx <= lb; lx++ )		
	    if (ptr[lx]  REL  searchnr)		
	      lx += floodsegment(info, lx, y2, x1-1, x2+1, y2-y); 	
	}
    }
  return(x2-x);					
}						       

#undef REL







/*L:ImageOverlayDisplay*

________________________________________________________________

		ImageOverlayDisplay
________________________________________________________________

Name:		ImageOverlayDisplay
Syntax:		| int ImageOverlayDisplay(wid, state)
                | ImageOverlayWidget wid;
		| int state;
Description:	Set New display status of overlay plane.
                'state' may be 'OVERLAY_CLEAR', 'OVERLAY_SET' or
		'OVERLAY_TOGGLE'
Return value:	Old state
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayDisplay(ImageOverlayWidget wid, int state)
{
  int old;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDisplay");

  old = wid->image_overlay.odisplay;
  if (state != OVERLAY_CLEAR && state != OVERLAY_SET &&
      state != OVERLAY_TOGGLE) {
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDisplay");
    return(old);
  }
  if (state == OVERLAY_TOGGLE)	
    wid->image_overlay.odisplay = 1 - old; 
  else
    wid->image_overlay.odisplay = state;
  if (wid->image_overlay.odisplay != old)
    ImageRedisplayScreen((ImageWidget) wid, 0, 0, wid->image.ximg->width,
			 wid->image.ximg->height);

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDisplay");
  return(old);
}





/*L:ImageOverlayProtect*

________________________________________________________________

		ImageOverlayProtect
________________________________________________________________

Name:		ImageOverlayProtect
Syntax:		| int ImageOverlayProtect(wid, state)
                | ImageOverlayWidget wid;
		| int state;
Description:	Set New protect status of overlay plane.
                'state' may be 'OVERLAY_CLEAR', 'OVERLAY_SET' or
		'OVERLAY_TOGGLE'
Return value:	Old state
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayProtect(ImageOverlayWidget wid, int state)
{
  int old;
  old = wid->image_overlay.protect;
  if (state != OVERLAY_CLEAR && state != OVERLAY_SET &&
      state != OVERLAY_TOGGLE)
    return(old);
  if (state == OVERLAY_TOGGLE)	
    wid->image_overlay.protect = 1 - old; 
  else
    wid->image_overlay.protect = state;
  return(old);
}




/*L:ImageOverlayColor*

________________________________________________________________

		ImageOverlayColor
________________________________________________________________

Name:		ImageOverlayColor
Syntax:		| int ImageOverlayColor(wid, col)
                | ImageOverlayWidget wid;
		| int col;
Description:	Set new overlay color.
Return value:	Old color
Author:		Otto Milvang
________________________________________________________________

*/


int ImageOverlayColor(ImageOverlayWidget wid, int col)
{
  int old;
  old = wid->image_overlay.draw_value;
  wid->image_overlay.draw_value = col;
  return(old);
}





/*L:ImageOvelayDraw*

________________________________________________________________

		ImageOvelayDraw
________________________________________________________________

Name:		ImageOvelayDraw
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static int read_state(char *arg)
{
  int state;
  if (strcmp(arg, "off") == 0) state = OVERLAY_CLEAR; else
  if (strcmp(arg, "on") == 0) state = OVERLAY_SET; else
  if (strcmp(arg, "toggle") == 0) state = OVERLAY_TOGGLE; else
  if (strcmp(arg, "clear") == 0) state = OVERLAY_CLEAR; else
  if (strcmp(arg, "set") == 0) state = OVERLAY_SET; else
    state = OVERLAY_TOGGLE;
  return(state);
}


static void ImageOverlayDraw(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageOverlayCallbackRec draw;
  ImageOverlayWidget iowid = (ImageOverlayWidget) wid;
  ImagePart *ip = &iowid->image;
  ImageOverlayPart *io = &iowid->image_overlay;
  update_status update = UPDATE_NONE;
  int x, y, origo, xo, yo, v, func;

  ENTER_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");  

  if (*num_params < 1)  {	
    XtWarning("ImageOverlay: draw() must have at least 1 parameter");
    LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");
    return;
  }
  if (event && (event->type == ButtonPress || event->type == ButtonRelease
		|| event->type == MotionNotify)) { 
    x = event->xbutton.x;
    y = event->xbutton.y;
    if (x < 0 || x >= ip->internal_width ||
	y < 0 || y>= ip->internal_height) {
      FPRINTF1("    Coordinates outside.\n");
      LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");
      return;
    }
    x = ImageScreenXtoPos((ImageWidget) wid,event->xbutton.x);
    y = ImageScreenYtoPos((ImageWidget) wid,event->xbutton.y);
    io->oldx = x;
    io->oldy = y;
  } else {
    x = io->oldx;
    y = io->oldy;
  }
  if (*num_params > 1 && strcmp(params[1], "close") == 0) {
    x = io->xsetorigo;
    y = io->ysetorigo;
  }
  origo = io->origo;
  xo = io->xorigo;
  yo = io->yorigo;
  
  if (strcmp(params[*num_params-1], "inverse") == 0) {
    (*num_params)--;
    v = -1;
  } else 
    v = io->draw_value;
  
  draw.event   = event;
  draw.img     = ip->image;
  draw.band    = ip->band;
  draw.overlay = io->overlay;
  draw.x       = x;
  draw.y       = y;
  draw.xorigo  = xo;
  draw.yorigo  = yo;
  draw.width   = 0;
  draw.height  = 0;
  draw.value   = v;
  FPRINTF3("    Draw %s %s\n", params[0], *num_params > 1 ? params[1] : "");

  if (strcmp(params[0], "origo") == 0)     func = 1;  else
  if (strcmp(params[0], "point") == 0)     func = 2;  else
  if (strcmp(params[0], "line") == 0)      func = 3;  else
  if (strcmp(params[0], "rectangle") == 0) func = 4;  else
  if (strcmp(params[0], "box") == 0)       func = 5;  else
  if (strcmp(params[0], "fill") == 0)      func = 6;  else
  if (strcmp(params[0], "clear") == 0)     func = 7;  else
  if (strcmp(params[0], "graphic") == 0)   func = 8;  else
  if (strcmp(params[0], "protect") == 0)   func = 9;  else
  if (strcmp(params[0], "color") == 0)     func = 10; else
    func = 0;

  draw.reason = func;
  update = UPDATE_REDISPLAY;
  switch(func)
    {
    case 0: break;
    case 1: 
      ImageOverlayDrawOrigo(iowid, 
	 *num_params > 1 ? (read_state(params[1]) == OVERLAY_SET) : origo, 
	 x, y); 
      break;
    case 2: 
      ImageOverlayDrawPoint(iowid, x, y, 
			    *num_params > 1 ? atoi(params[1]) : 1, v, update);
      break;
    case 3: 
      if (!origo) {
	LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");
	return;
      }
      ImageOverlayDrawLine(iowid, xo, yo, x, y, v, update);
      break;
    case 4: 
     if (!origo) {
       LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");
       return;
     }
      ImageOverlayDrawRectangle(iowid, xo, yo, x, y, v, update);
      break;
    case 5: 
     if (!origo) {
       LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");
       return;
     }
      ImageOverlayDrawBox(iowid, xo, yo, x, y, v, update);
      break;
    case 6: 
      ImageOverlayFill(iowid, x, y, 0, v, 4, 1, update);
      break;
    case 7: 
      ImageOverlayFill(iowid, x, y, *(ImageData)
		       ImageOverlayPosXYtoAdr(iowid, x, y, 0),
		       0, 8, 1, UPDATE_RESIZE);
      break;
    case 8: 
      ImageOverlayDisplay(iowid, read_state(*num_params > 1 ? params[1] : ""));
      draw.value = io->odisplay;
      break;
    case 9: 
      ImageOverlayProtect(iowid, read_state(*num_params > 1 ? params[1] : ""));
      draw.value = io->protect;
      break;
    case 10: 
      draw.value =  (*num_params > 1) ? 
	(atoi(params[1])) :
	  (iowid->image_overlay.draw_value + 1); 
      ImageOverlayColor(iowid, draw.value);
      break;
    }
  XtCallCallbacks(wid, XtNdrawProc, (XtPointer) &draw);

  LEAVE_FUNCTION_DEBUG("ImageOverlay.c: ImageOverlayDraw");
}



/*L*

________________________________________________________________

		ImageRoi
________________________________________________________________

Name:		ImageRoi
Syntax:		| void ImageRoiDraw(iw, setmode)
		| ImageOverlayWidget iw;
		| int setmode;
		|
		| void ImageRoiStartDrag(iw, event, param)
		| ImageOverlayWidget iw;
		| XEvent *event;
		| char **param;
		| unsigned long *num_param;
		|
		| void ImageRoiStop(iw, event, param, num_param)
		| ImageOverlayWidget iw;
		| XEvent *event;
		| char **param;
		| unsigned long *num_param;
		|
		| void ImageRoiDrag(w, event, param, num_param)
		| Widget w;
		| XEvent *event;
		| char **param;
		| unsigned long *num_param;

Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static void ROI_callback(ImageOverlayWidget iw, XEvent *event, int value)
{
  int x1, x2, y1, y2, x, y, w, h, ww, wh;
  ImageOverlayCallbackRec iroi;
  ImageOverlayPart *roi;
  roi = &(iw->image_overlay); 
  x = (roi->width  >= 0) ? roi->x : roi->x + roi->width;
  w = (roi->width  >= 0) ? roi->width : -roi->width;
  y = (roi->height >= 0) ? roi->y : roi->y + roi->height;
  h = (roi->height >= 0) ? roi->height : -roi->height;
  roi->xorigo = 0;
  roi->yorigo = 0;

  ww = iw->image.internal_width - 1;
  wh = iw->image.internal_height - 1;
  if (x <  0) x = 0;
  if (y <  0) y = 0;
  if (x + w >= ww) x = ww - w - 1;
  if (y + h >= wh) y = wh - h - 1;
  
  x1 = ImageScreenXtoPos((ImageWidget) iw, x);
  y1 = ImageScreenYtoPos((ImageWidget) iw, y);
  x2 = ImageScreenXtoPos((ImageWidget) iw, x + w + 1);
  y2 = ImageScreenYtoPos((ImageWidget) iw, y + h + 1);

  iroi.event      = event;
  iroi.img        = iw->image.image;
  iroi.band       = iw->image.band;
  iroi.value	  = value;
  iroi.x          = x1;
  iroi.y          = y1;
  iroi.width      = x2 - x1 + 1;
  iroi.height     = y2 - y1 + 1;
  XtCallCallbacks((Widget) iw, XtNroiProc, &iroi);

}


static void ImageRoiDraw(ImageOverlayWidget iw, int setmode)
{
  int x, y, w, h;
  
  if (iw->image_overlay.display == setmode) return;

  iw->image_overlay.display = 1 - iw->image_overlay.display;
  XSetFunction(XtDisplay((Widget) iw), iw->image.gc, GXinvert);

  x = iw->image_overlay.x;
  y = iw->image_overlay.y;
  w = iw->image_overlay.width;
  h = iw->image_overlay.height;

  if (w==1 && h==1) return;
  if (w<0)
    { 
      w = -w;
      x = x - w + 1;
    }
  if (h<0)
    { 
      h = -h;
      y = y - h + 1;
    }
  if (iw->image_overlay.fill)   
    XFillRectangle(XtDisplay(iw), XtWindow(iw), iw->image.gc,x,y,w,h);
  else
    XDrawRectangle(XtDisplay(iw), XtWindow(iw), iw->image.gc,x,y,w,h);
}

static void ImageRoiStartDrag(ImageOverlayWidget iow, XEvent *event, int value)
{
  ImageOverlayPart *roi;
  ImageWidget iw = (ImageWidget) iow;

  ImageRoiDraw(iow, ROI_CLEAR);

  roi = &(iow->image_overlay);
  roi->activated = 1;

  roi->x         = event->xbutton.x;
  roi->x_im      = ImageScreenXtoPos(iw, roi->x);
  roi->x         = ImagePosXtoScreen(iw, roi->x_im, 0);

  roi->y         = event->xbutton.y;
  roi->y_im      = ImageScreenYtoPos(iw, roi->y);
  roi->y         = ImagePosYtoScreen(iw, roi->y_im, 0);

  roi->width     = 1;
  roi->width_im  = ImageScreenToWidth(iw, roi->width);
  roi->width     = ImageWidthToScreen(iw, roi->width_im, 1);

  roi->height    = 1;
  roi->height_im = ImageScreenToHeight(iw, roi->height);
  roi->height    = ImageHeightToScreen(iw, roi->height_im, 1);

  ImageRoiDraw(iow, ROI_SET);

  ROI_callback(iow, event, value);
}

static void ImageRoiExtend(ImageOverlayWidget iw, XEvent *event, int value)
{
  iw->image_overlay.activated = 1;
  ImageRoiDrag(iw, event, value);
  ROI_callback(iw,  event, value);
}

static void ImageRoiStop(ImageOverlayWidget iow, XEvent *event, int value)
{
  
/* if (!iow->image_overlay.activated &&  
      (par == 1 || !iow->image_overlay.permanent) ) return;
*/
  if (iow->image_overlay.activated)
    if (! (iow->image_overlay.permanent)) 
      ImageRoiDraw(iow, ROI_CLEAR);
    else {
      ImageWidget iw = (ImageWidget) iow;
      ImageOverlayPart *iop;

      ImageRoiDraw(iow, ROI_CLEAR);

      iop = &(iow->image_overlay);

      iop->width  = ImageWidthToScreen( iw, iop->width_im,  1);
      iop->height = ImageHeightToScreen(iw, iop->height_im, 1);

      ImageRoiDraw(iow, ROI_SET);
    }

  iow->image_overlay.activated = 0;
  ROI_callback(iow, event, value);
}

static void ImageRoiStartMove(ImageOverlayWidget iw, XEvent *event, int value)
{
  iw->image_overlay.activated = 1;
  ImageRoiDraw(iw, ROI_SET);
}

static void ImageRoiMove(ImageOverlayWidget iw, XEvent *event, int value)
{
  int x, y;

  if (!(iw->image_overlay.activated)) return;

  ImageRoiDraw(iw, ROI_CLEAR);

  x = event->xbutton.x;
  y = event->xbutton.y;
  if (x + iw->image_overlay.width  >= iw->image.internal_width ) 
    x = iw->image.internal_width  - iw->image_overlay.width  - 1;
  if (y + iw->image_overlay.height >= iw->image.internal_height)  
    y = iw->image.internal_height - iw->image_overlay.height - 1;
  if (x + iw->image_overlay.width  <= 0)  x = 0;
  if (y + iw->image_overlay.height <= 0)  y = 0;

  iw->image_overlay.x = x;
  iw->image_overlay.x_im =
    ImageScreenXtoPos((ImageWidget) iw, iw->image_overlay.x);
  iw->image_overlay.y = y;
  iw->image_overlay.y_im =
    ImageScreenXtoPos((ImageWidget) iw, iw->image_overlay.y);

  ImageRoiDraw(iw, ROI_SET);
  ROI_callback(iw,  event, value);
}


static void ImageRoiDrag(ImageOverlayWidget iw, XEvent *event, int value)
{
  int w, h, x, y;

  if (!(iw->image_overlay.activated)) return;

  ImageRoiDraw(iw, ROI_CLEAR);

  w = event->xbutton.x - iw->image_overlay.x+1;
  h = event->xbutton.y - iw->image_overlay.y+1;
  if ((x = iw->image_overlay.xaspect) && (y = iw->image_overlay.yaspect))
    {
      if (abs(w)*y > abs(h)*x)
	h = h*w < 0 ? -w*y/x : w*y/x;
      else
	w = h*w < 0 ? -h*x/y : h*x/y;
    }

  iw->image_overlay.width     = w;
  iw->image_overlay.height    = h;
  iw->image_overlay.width_im  = ImageScreenToWidth((ImageWidget) iw, w);
  iw->image_overlay.height_im = ImageScreenToHeight((ImageWidget) iw, h);

  ImageRoiDraw(iw, ROI_SET);
  ROI_callback(iw,  event, value);
}

static void ImageRoiProc(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageOverlayWidget iw = (ImageOverlayWidget) wid;
  if (*num_params != 1) 
    {	
      XtWarning("ImageRoi: roi() must have 1 parameter");
      return;
    }
#define ifproc(name, proc, value)\
  if (strcmp(params[0], name) == 0)\
    { \
      proc(iw, event, value);\
      return; \
    }
  
  ifproc("Drag", 	ImageRoiDrag,		ROI_DRAG);
  ifproc("Move", 	ImageRoiMove,		ROI_MOVE);
  ifproc("StartDrag", 	ImageRoiStartDrag,	ROI_STARTDRAG);
  ifproc("StopDrag", 	ImageRoiStop,		ROI_STOPDRAG);
  ifproc("StartMove", 	ImageRoiStartMove,	ROI_STARTMOVE);
  ifproc("StopMove", 	ImageRoiStop,		ROI_STOPMOVE);
  ifproc("ExtendDrag", 	ImageRoiExtend,		ROI_EXTENDDRAG);
  XtWarning("ImageRoi: roi() Bad parameter");
#undef ifproc

}
