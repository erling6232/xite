
/*C*

________________________________________________________________

        Image.c
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



/*F:ImageError=Image*/
/*F:ImageHasColormap=Image*/
/*F:ImageInstallPixelType=Image*/
/*F:ImageRedisplay=Image*/
/*F:ImageRedisplayScreen=Image*/
/*F:ImageRedisplayArea=Image*/
/*F:ImageUpdateMinMax=Image*/
/*F:ImageKill=Image*/
/*F:ImageScreenXtoPos=Image*/
/*F:ImageScreenYtoPos=Image*/
/*F:ImageScreenXYtoAdr=Image*/
/*F:ImageScreenToHeight=Image*/
/*F:ImageScreenToWidth=Image*/
/*F:ImageHeightToScreen=Image*/
/*F:ImageWidthToScreen=Image*/
/*F:ImagePosXYtoAdr=Image*/
/*F:ImageTranslateZoomPanArgs=Image*/
/*F:ImagePosXtoScreen=Image*/
/*F:ImagePosYtoScreen=Image*/
/*F:ImageGetZoom=Image*/
/*F:ImageSetZoom=Image*/
/*F:ImageCopyImagePart=Image*/
/*F:ImageCopyResources=Image*/
/*F:ImageGetResources=Image*/
/*F:ImageGetImageResources=Image*/
/*F:ImageGetZoomAll=Image*/
/*F:ImageSetZoomAll=Image*/
/*F:Image*

________________________________________________________________

		Image
________________________________________________________________

Name:		Image, ImageError, ImageHasColormap, ImageInstallPixelType,
		ImageRedisplay, ImageRedisplayScreen, ImageRedisplayArea,
		ImageKill, ImageScreenXtoPos, ImageScreenYtoPos,
		ImageScreenXYtoAdr, ImageScreenToHeight, ImageScreenToWidth,
		ImageHeightToScreen, ImageWidthToScreen, ImagePosXYtoAdr,
		ImageTranslateZoomPanArgs, ImagePosXtoScreen,
		ImagePosYtoScreen, ImageGetZoom, ImageSetZoom,
		ImageCopyImagePart, ImageCopyResources,	ImageGetResources,
		ImageGetImageResources, ImageGetZoomAll, ImageSetZoomAll,
		ImageUpdateMinMax - XITE X11 image widget

Application header file: <xite/Image.h>

Class header file: <xite/ImageP.h>

Class:          imageWidgetClass

Class Name:     Image

Superclass:     Composite

Description:    The Image widget is part of the XITE (X-based Image Processing
                Tools and Environment) software. It is used by all the
		display programs supplied with XITE.

		The widget is designed to work with images in the BIFF format.
		It can probably work also with other image formats, by
		supplying a different set of image format description routines
		(see 'ImageFormat(3)').

		This widget handles a number of pixeltypes, see the section
		'Predefined pixeltypes' below.

		The widget handles zooming and panning through a translation
		table and action functions. Various events can be caught
		and supplied to callback functions.

		See the documentation on the main XITE display program
		'xshow(1)' as well as the XITE ximage toolkit 'ximage(3)' for
		more information on how this widget can be used in an
		application.

Resources:      Apart from the resources of the superclass:

                &XtNimageImage (class XtCImageImage)
		Type: Pointer, 
		Default:  NULL 

		This refers to the complete data structure for an image.
		The pointer is general enough to refer to any kind of
		data structure. In most cases it will refer to a BIFF image,
		but it could be used to refer to other image formats.

		&XtNimageBand (class XtCImageBand)
		Type: Pointer, 
		Default: NULL 

		This refers to the complete data structure for the
		band/channel of the image. It does not refer to the first
		pixel (see the resource XtNimageData).

		Setting this resource while not setting the primary band
		resources (XtNimageRedBand, XtNimageGreenBand,
		XtNimageBlueBand) means that the image is PseudoColor (not
		rgb). This will automatically update the pixel data resources
		for the primaries (XtNimageRedData, XtNimageGreenData,
		XtNimageBlueData). This is necessary in case the image is
		displayed with a composite visual class.

		&XtNimageRedBand (class XtCImageRedBand)
		Type: Pointer, 
		Default: NULL 

		If the image is a three-band image which should be interpreted
		as an rgb image where the tree bands represent the amount of
		red, green and blue, then this resource refers to the complete
		data structure for the band/channel with the red component.
		Also refer to the 'XtNimageRgb' and 'XtNimageRedData'
		resources.

		&XtNimageGreenBand (class XtCImageGreenBand)
		Type: Pointer, 
		Default: NULL 

		Same as XtNimageRedBand, but for the green component.

		&XtNimageBlueBand (class XtCImageBlueBand)
		Type: Pointer, 
		Default: NULL 

		Same as XtNimageRedBand, but for the blue component.

		&XtNimageRgb (class XtCImageRgb)
		Type: Boolean,
		Default: False

		This is a read-only resource telling whether the widget
		displays a three-band rgb image or a one-band image.

		&XtNimageData (class XtCImageData)
		Type: Pointer, 
		Default: NULL

		This refers to the first pixel of the band given by
		XtNimageBand.
		
		&XtNimageRedData (class XtCImageRedData)
		Type: Pointer, 
		Default: NULL

		This refers to the first pixel of the red image component,
		given by XtNimageRedBand, if the image is supposed to be
		interpreted as a three-band rgb image. If the image is a
		one-band image, then this resource will equal 'XtNimageData'.

		&XtNimageGreenData (class XtCImageGreenData)
		Type: Pointer, 
		Default: NULL

		Same as XtNimageRed, but for the green component.

		&XtNimageBlueData (class XtCImageBlueData)
		Type: Pointer, 
		Default: NULL

		Same as XtNimageRed, but for the blue component.

		&XtNimageTitle (class XtCImageTitle)
		Type: Pointer, 
		Default: NULL 

		Title of the image.

		&XtNimagePixtype (class XtCImagePixtype)
		Type: Pointer, 
		Default: NULL 

		Name of pixeltype. Typically one of "ImageBytePixel",
		"ImageUnsignedShortPixel" etc.

		&XtNimageWidth (class XtCImageWidth)
		Type: int, 
		Default: NULL

		Width of image data in pixels.

		&XtNimageHeight (class XtCImageHeight)
		Type: int, 
		Default: NULL 

		Height of image data in pixels.

		&XtNvariableAspect (class XtCVariableAspect)
		Type: Boolean, 
		Default: "false"

		True if image can have variable aspect ratio. Otherwise
		it will be forced to have a fixed aspect ratio.

		&XtNimageLineLength (class XtCImageLineLength)
		Type: int, 
		Default: NULL 

		Number of bytes which each row of the image occupies.

		&XtNimageXstart (class XtCImageXstart)
		Type: int, 
		Default: NULL 

		The global horizontal coordinate of the upper-left corner-pixel
		of the image.

		&XtNimageYstart (class XtCImageYstart)
		Type: int, 
		Default: NULL 

		The global vertical coordinate of the upper-left corner-pixel
		of the image.

		&XtNdisplayMethod (class XtCDisplayMethod)
		Type: int, 
		Default: NULL

		This resource can be given one of the following values

		| 0x0000
		| 0x0001 
		| 0x0010 (STATIC_GRAY_1)
		| 0x0020 (STATIC_GRAY_2) 
		| 0x0040 (STATIC_GRAY_4) 
		| 0x0080 (STATIC_GRAY_8) 
		| 0x1040 (GRAY_SCALE_4)  
		| 0x1080 (GRAY_SCALE_8) 
		| 0x3080 (PSEUDO_COLOR_8, 128-entry colormaps
		|         (reduced-color))
		| 0x3080 (PSEUDO_COLOR_8_A, 128-entry colormaps
		|         (reduced-color))
		| 0x3081 (PSEUDO_COLOR_8_B, 256-entry colormaps)
		| 0x4180 (TRUE_COLOR_24)
		| 0x5180 (DIRECT_COLOR_24,
		|         128-entry composite colormaps
		|         (reduced-color))
		| 0x5180 (DIRECT_COLOR_24_A,
		|         128-entry composite colormaps
		|         (reduced-color))
		| 0x5181 (DIRECT_COLOR_24_B,
		|         256-entry composite colormaps)

		Setting XtNdisplayMethod to 0 (default value), means that
		the displaymethod is calculated from visual class and depth.
		The result will be a reduced-color displaymethod if visual
		class is PseudoColor or DirectColor (leaving some colors for
		overlays and window manager).

		Setting XtNdisplayMethod to 1 means that the displaymethod
		is calculated the same way as for the value 0, but in this
		case, the result will be a full-color displaymethod if
		visual class is PseudoColor or DirectColor.

		&XtNprintValues (class XtCPrintValues)
		Type: Boolean, 
		Default: "true"

		Whether or not to print the numeric value of each pixel in
		the image window. This will only be done when each pixel is
		large enough to give room for the textual representation.

		&XtNcolortab (class XtCColortab)
		Type: int, 
		Default: -1

		An index into an array of colortables. This resource identifies
		which colortable (in the array of colortables) is active in
		the image window. The array is not local to the image widget,
		but must be supplied by the application.

		The default value (of -1) will make the widget Initialize
		method choose the value 0.

		&XtNcolorCells (class XtCColorCells)
		Type: int, 
		Default: -1

		Number of colors actually used to display the image. This
		may be different from the length of the X colormap used by
		the image, because the Image widget is capable of using only
		a part of a colormap. This is to reduce the "technicolor"
		effect when the mouse is moved between different windows.

		For such "reduced-color" display of images, this resource will
		typically have the value 128 (whereas there are typically
		256 cells in an X colormap). For full-color display, the
		resource will be 256.

		The default value (of -1) will make the widget Initialize
		method calculate a legal value based on the XtNdisplayMethod
		resource and the number of cells in the colormaps for the
		chosen visual.

		&XtNcolorPtr (class XtCColorPtr)
		Type: Pointer, 
		Default: NULL

		A reference to an integer array of XtNcolorCells elements.

		Each pixel value is transformed into a new value before it is
		sent to the pixmap (which will be shown in a screen window).
		The original pixel value is used as an index into an integer
		array. The value in the array cell becomes the pixel value in
		the pixmap.

		The integer transformation array (which always has 256
		elements) is identical to the XtNcolorPtr array for the first
		XtNcolorCells elements. If XtNcolorCells < 256, then the
		XtNcolorPtr array is repeated in the transformation array.

		If the pixeltype is not unsigned byte, the pixelvalue is first
		scaled and translated to a legal interval (typically between 0
		and XtNcolorCells) before it is used as a lookup index.

		With e.g. XtNcolorCells equal to 128 (reduced-color display)
		and XtNcolorPtr an array from 64 to 191, the integer
		transformation array contains the 256 values
		64,...191,64,...,191. The scaled and translated original
		pixelvalues will then be transformed (by the first 128
		elements) into the range 64,...,191.
		
		The default value will make the widget Initialize method
		find an array based on the XtNdisplayMethod and
		XtNcolorCells resources. For "reduced-color" display, the
		array will have 128 elements with values from 64 to 191.
		For "full-color" display, the array will have 256 elements
		from 0 to 255.

		&XtNvisual (class XtCVisual)
		Type: Visual, 
		Default: NULL

		Class of visual. See the "Xlib Programming Manual" by
		Adrian Nye (O''Reilly & Associates, Inc.), chapter 7 for
		more information about visuals.

		&XtNscaletype (class XtCScaletype)
		Type: int, 
		Default: NULL (which implies SCALE_LINEAR)

		This resource takes the value SCALE_LINEAR or SCALE_THRESHOLD.
		In the first case, pixel values are scaled linearly during
		the transformation described for the XtNcolorPtr resource.
		The scaling factors are given by the resources XtNscalea and
		XtNscaleb. The scaling is relative to an assumed maximum
		range of values, as given by the XtNminval and XtNmaxval
		resources.

		For SCALE_TRESHOLD, the resource XtNscalea is used as a
		threshold level during the transformation described for
		the XtNcolorPtr resource. If the pixel value is below the
		threshold level, it is set equal to the minimum element in
		the integer transformation array, otherwise it is set equal
		to the maximum element in the transformation array.

		This resource is always interpreted as SCALE_LINEAR for all
		pixel types except unsigned byte and signed byte.

		&XtNrange (class XtCRange)
		Type: String,
		Default: "dynamic"

		This resource determines what range of values an image is
		assumed to have. It affects what values the resources
		XtNminval and XtNmaxval will have. The XtNrange resource
		takes the value "fixed", "pixelType" or "dynamic". In
		the latter two cases, values given by the user for the
		resources XtNminval or XtNmaxval will be neglected.

		"fixed" means that the resources XtNminval and XtNmaxval
		will not be updated to reflect the actual image value
		range. They will remain fixed, equal to their initial
		values, as given by the user. This can be handy when
		visually comparing images. Image values which exceed the
		range given by XtNminval and XtNmaxval will be clipped.

		The effect of using the value "pixelType", resembles that
		of "fixed" in that the resources XtNminval and XtNmaxval
		will not be updated. But with "pixelType", an image is
		assumed to have extreme values equal to the extreme values
		for that pixel type. The resources XtNminval and XtNmaxval
		will be set equal to the pixel type extreme values, so it
		does not matter whether the user specifies these
		resources. An image which actually only occupies a small
		range relative to the pixel type range, will appear with a
		small dynamic range (low contrast) on screen.

		The value "dynamic" means that the actual extreme values
		of the image will be used when scaling. The resources
		XtNminval and XtNmaxval will be set equal to these values,
		so it does not matter whether the user specifies these
		resources.

		By default, this resource has the value "dynamic".
		However, images with pixel type unsigned byte gets
		special treatment. For such images, "dynamic" is the
		same as "pixelType".

		&XtNscalea (class XtCScalea)
		Type: float, 
		Default: 1.0

		Inclination of linear transformation of pixel values during
		display, if XtNscaletype equals SCALE_LINEAR. For XtNscaletype
		equal to SCALE_THRESHOLD, this resource is the threshold
		level.

		Also refer to the description of the XtNscaletype and
		XtNrange resources.

		&XtNscaleb (class XtCScaleb)
		Type: float, 
		Default: 0.0

		Offset of linear transformation of pixel values during display.

		Also refer to the description of the XtNscaletype and
		XtNrange resources.

		&XtNminval (class XtCMinval)
		Type: float, 
		Default: NULL

		Minimum (or assumed minimum) pixel value for an image.
		This has an effect on the scaling of an image, because the
		scaling is relative to the range given by XtNminval and
		XtNmaxval. For unsigned byte images, XtNminval is often
		set to 0. If the same image is represented by a float
		pixel type and the XtNrange resource equals "dynamic",
		XtNminval is calculated from the particular image. These
		two images may look different on the screen, although their
		pixel values are actully equal.

		Also refer to the description of the XtNscaletype and XtNrange
		resources as well as the ImageUpdateMinMax routine.

		&XtNmaxval (class XtCMaxval)
		Type: float, 
		Default: NULL

		Maximum pixel value for an image. Refer to the description
		of the XtNminval resource.

		&XtNfont (class XtCFont)
		Type: FontStruct, 
		Default: "XtDefaultFont"

		This is one of the fonts which may be used to print the
		numerical pixel values in the image screen window if the
		resource XtNprintValues is set.

		The following three fonts are alternatives to XtNfont.
		Typically these fonts will be of different sizes. The widget
		will try the smallest one first. Pixelvalues will only
		be printed if the textual representation fits inside the
		pixel square.

		&XtNfont1 (class XtCFont1)
		Type: FontStruct, 
		Default: NULL

		Another font for printing pixel values.

		&XtNfont2, (class XtCFont2)
		Type: FontStruct, 
		Default: NULL

		Another font for printing pixel values.

		&XtNfont3, (class XtCFont3)
		Type: FontStruct, 
		Default: NULL

		Another font for printing pixel values.

		&XtNcallback (class XtCCallback)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this callback list will be called by the
		'notify' action function described below.

		&XtNresizeCallback (class XtCResizeCallback)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this list are called by the 'notify' action
		function described below, provided that the event which
		invokes the action function is NULL.

		&XtNimageUpdate (class XtCImageUpdate)
		Type: int, 
		Default: NULL

		This resource controls the resizing and display of an image.
		Usually, when calling the set_values widget method (via e.g.
		XtVaSetValues), this resource is set to one of the values
		UPDATE_NONE, UPDATE_REDISPLAY or UPDATE_NEWSIZE, depending on
		which resources are changed in the call and what needs to be
		done to the image display as a consequence of these changes.
		The set_values method invokes the resize method which takes
		care of any necessary changes to the image display, depending
		on the new value of this resource.

		By setting the XtNimageUpdate resource to UPDATE_NONE in the
		XtVaSetValues call, along with the setting of other resources,
		the XtNimageUpdate resource will not be changed because of the
		setting of the other resources. In other words, it will remain
		equal to UPDATE_NONE until the resize method is invoked by the
		set_values method. The reason for this is that one may want
		to change some of the resources but delay the costly
		resizing/recalculation of the image display until later.
		Typically, an explicit recalculation is initiated with a call
		to the ImageRedisplay routine, with the update parameter
		equal to UPDATE_ZOOMPAN.

		For further description, see the description of redisplay
		routines in the section 'Convenience routines' below.

		&XtNmenubar (class XtCMenubar)
		Type: Boolean, 
		Default: "true"

		Whether or not a menubar should be visible above the image.
		The menubar itself is not an attribute of the image widget.
		This resource merely is used to tell the application whether
		there is a menubar present. It is the responsibility of the
		application to take the proper action (or no action).

Image actions:
                &nop
		No operation.

		&notify

		This action can be used to log position, select images and so
		on. It assigns values to the attributes of the ImageCallbackRec
		structure and invokes the callback functions on the
		XtNcallback callback-list with a reference to the
		ImageCallbackRec structure.

		The attributes of the ImageCallbackRec structure are

		&&reason
		An integer with value ACTIVATE or RESIZE.

		&&event
		An XEvent reference to the event which triggered the action
		function.

		&&img
		A "struct imagestruct *" reference to the XtNimageImage
		resource of the widget in which the event took place.

		&&band
		A "struct bandstruct *" reference to the XtNimageBand resource
		of the widget in which the event took place.

		&&x, y
		Two integers which represent the coordinates where one of the
		events ButtonPress, ButtonRelease, MotionNotify or Expose
		took place (or where the mouse pointer happened to be located
		in case of other events).

		&&data
		An ImageData reference to the actual image data pixel at the
		location given by 'x' and 'y'.

		&&pixtype
		A "ipixel *" reference to the pixeltype of the image data.
		See the section 'Predefined pixeltypes' below.

		&&zoom
		A "float" with the current zoom factor.

		&&magnification
		A "float" with the current ratio of image pixels which are
		in the widget window.

		&&xpan, ypan
		Two integers describing the horizontal and vertical panning.

		&kill
		Identical to the 'ImageKill' function described below in the
		section 'Convenience routines'.

		&zoom_pan
                This action takes 3 arguments, 'zoom_pan(zoom, pan_x, pan_y)'.
                The arguments may be absolute or relative values. Position
                may also be translated from mouse position. If the pan
                arguments end in a 'C', the pan arguments specify the center
                position in the new image rather than the upper left corner.

                | <Zoom_pan> := zoom_pan(<zoom_arg>,
                |                        <pan_arg>, <pan_arg>)
                | <zoom_arg> := In | Out | Normal | 
                |               <zoom_op><float_value> |
                |               <float_value>
                | <zoom_op>  := + | - | / | *
                | <pan_arg>  := <pos_ulc> | <pos_cp>
                | <pos_ulc>  := MousePos | <pan_op><int_value> |
                |               <int_value>
                | <pos_cp>   := MousePosC | <int_value>C
                | <pan_op>   := + | -

		<zoom_arg> equal to "In" means double zoom factor, "Out"
		means half zoom factor, "Normal" means zoom factor 1.0.
		MousePos means get position from mouse position.

		See the section 'Default translation bindings' for examples.

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

Default translation bindings:

		| <Key>Q:          kill()
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
		| Ctrl <Btn1Down>: zoom_pan(*2, MousePosC, MousePosC)
		| Ctrl <Btn2Down>: zoom_pan(0, MousePosC, MousePosC)
		| Ctrl <Btn3Down>: zoom_pan(/2, MousePosC, MousePosC)
		| Shift Ctrl <Btn1Down>:
		|                  zoom_pan(*8, MousePosC, MousePosC)
		| Shift Ctrl <Btn2Down>:
		|                  zoom_pan(1, MousePosC, MousePosC)
		| Shift Ctrl <Btn3Down>:
		|                  zoom_pan(/8, MousePosC, MousePosC)
		| None <Btn1Up>:   notify()
		| None <Btn2Up>:   notify()
		| None <Btn3Up>:   notify()
		| <MouseMoved>:    notify()

Image callbacks:

                There are two callback lists, identified by the
		resources XtNcallback and XtNresizeCallback. See the
		description of these resources.

Predefined pixeltypes:

                Each pixeltype is of type 'ipixel', given by

		| typedef struct ipixelstruct
		| {
		|   char *pixelname;
		|   char *printformat;
		|   int  pixelsize;
		|   resizefunc resize;
		|   imageprintffunc printf;
		|   int txtlen;
		|   scale_type scaletype;
		|   float scalea, scaleb;
		|   float minval, maxval;
		|   struct ipixelstruct *next;
		| } ipixel;

		The various predefined pixel types are (the attributes
		"printformat", "txtlen" and "next" are only given proper values
		when the first Image widget is created, during widget class
		initialization)

		&bytepixel

		| pixelname : ImageBytePixel
		| pixelsize : 1
		| resize    : ImageByteResize
		| printf    : ImageBytePrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : 0.0
		| maxval    : UNS_BYTE_MAX

		&signedbytepixel

		| pixelname : ImageSignedBytePixel
		| pixelsize : 1
		| resize    : ImageSignedByteResize
		| printf    : ImageSignedBytePrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : SIGN_BYTE_MIN
		| maxval    : SIGN_BYTE_MAX

		&shortpixel

		| pixelname : ImageShortPixel
		| pixelsize : 2
		| resize    : ImageShortResize
		| printf    : ImageShortPrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : SIGN_SHORT_MIN
		| maxval    : SIGN_SHORT_MAX

		&unsignedshortpixel

		| pixelname : ImageUnsignedShortPixel
		| pixelsize : 2
		| resize    : ImageUnsignedShortResize
		| printf    : ImageUnsignedShortPrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : 0.0
		| maxval    : UNS_SHORT_MAX

		&longpixel
		This is a 4-byte integer, long on some platforms, int on
		others.

		| pixelname : ImageLongPixel
		| pixelsize : 4
		| resize    : ImageLongResize
		| printf    : ImageLongPrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : INTEGER_MIN
		| maxval    : INTEGER_MAX

		&floatpixel

		| pixelname : ImageFloatPixel
		| pixelsize : 4
		| resize    : ImageFloatResize
		| printf    : ImageFloatPrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : REAL_MIN
		| maxval    : REAL_MAX

		&complexpixel
		| pixelname : ImageComplexPixel
		| pixelsize : 8
		| resize    : ImageComplexResize
		| printf    : (imageprintffunc) ImageComplexPrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 0.0
		| scaleb    : 0.0
		| minval    : 0.0
		| maxval    : 0.0

		&doublepixel

		| pixelname : ImageDoublePixel
		| pixelsize : 8
		| resize    : ImageDoubleResize
		| printf    : ImageDoublePrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 1.0
		| scaleb    : 0.0
		| minval    : REAL_MIN
		| maxval    : REAL_MAX

		&doublecomplexpixel
		| pixelname : ImageDoubleComplexPixel
		| pixelsize : 16
		| resize    : ImageDoubleComplexResize
		| printf    : (imageprintffunc) 
		|             ImageDoubleComplexPrintf
		| scaletype : SCALE_LINEAR
		| scalea    : 0.0
		| scaleb    : 0.0
		| minval    : 0.0
		| maxval    : 0.0

Convenience routines:

		&Routines related to the visual class

		&&ImageHasColormap
                | int ImageHasColormap( ImageWidget wid );

		Return False if visual class is StaticGray, StaticColor or
		TrueColor (i.e. if visual does not have a changeable colormap,
		the colormap is immutable), True if Visual type is GrayScale,
		PseudoColor or DirectColor.

		-&&

		&Redisplay routines:

		&&ImageRedisplay
                | void ImageRedisplay( ImageWidget wid,
                |    update_status update );

		If 'update' is not equal to UPDATE_NONE, sets the private
		update-variable of the image widget equal to 'update'. Calls
		the resize method of the widget, followed by a call to the
		widget expose method, for the class which 'wid' belongs to
		(this may be a subclass, e.g. imageOverlayWidgetClass).

		The expose method does not depend on the value of the 'update'
		parameter. It is called with NULL as event and region
		arguments.

		The resize method is used to recalculate the image display.
		This may e.g. involve changes due to new pixel values in the
		original image data, a new zoom or panning state for the
		current image, a completely new image or changes to other
		resources such as XtNscaletype, XtNscalea, XtNscaleb etc,
		resources which influence the values in the display pixmap.
		However, if these resources have been changed with an explicit
		call to the set_values widget method (via XtVaSetValues or
		XtSetValues), then don''t invoke the resize method with a
		call to ImageRedisplay, because the set_values method
		automatically invokes the resize and exposure methods if
		necessary.

		For information on the pixeltype-specific resize functions,
		see 'ImageResize(3)'.

		The 'update' variable governs the behaviour of the resize
		method. 'update' may take on one of the following values:

		| UPDATE_NONE
		| UPDATE_REDISPLAY
		| (UPDATE_REDISPLAY | UPDATE_MINMAX)
		| UPDATE_RESIZE
		| (UPDATE_RESIZE | UPDATE_MINMAX)
		| UPDATE_ZOOMPAN
		| (UPDATE_ZOOMPAN | UPDATE_MINMAX)
		| UPDATE_NEWSIZE
		| (UPDATE_NEWSIZE | UPDATE_MINMAX)
		
		The meaning of the different values are as follows:

		| UPDATE_NONE:
		Use this if only exposure is needed, without any recalculation
		of image display pixel values.

		| UPDATE_REDISPLAY:
		Use this if only a rectangle inside the image needs to be
		redisplayed. The rectangle must be specified by the private
		widget variables updatex, updatey, updatew and updateh. A
		reason to redisplay only a part of the image may be that pixel
		values have been changed only inside this rectangle.
		
		The resize method will just call the pixeltype- (and
		displaymethod-) specific resize function on the rectangle.
		Afterwards, the private update variable is set equal to
		UPDATE_RESIZE before the resize method returns. This
		functionality can also be achieved with the function
		'ImageRedisplayScreen' or 'ImageRedisplayArea' (see below).

		| (UPDATE_REDISPLAY | UPDATE_MINMAX):
		Use this for the same reason as with UPDATE_REDISPLAY alone if
		also the minimum and maximum image values need to be
		recalculated. The new values will be used for the XtNminval
		and XtNmaxval resources. If they have changed, the scaling of
		the pixel values will change so that the exact available
		dynamic range is used. This leads to a call to the
		ImageUpdateMinMax routine. The UPDATE_MINMAX setting is
		only honoured when the XtNrange resource equals "dynamic".

		| UPDATE_RESIZE:
		Use this value if the widget size has changed, but the image
		data are unchanged. This is the default value of the image
		widgets, which means that resizing initiated by the window
		manager will be handled.

		| (UPDATE_RESIZE | UPDATE_MINMAX):
		Same as UPDATE_RESIZE, but also update the XtNminval and
		XtNmaxval resources in order to use the exact available
		dynamic range. The UPDATE_MINMAX setting is
		only honoured when the XtNrange resource equals "dynamic".

		| UPDATE_ZOOMPAN:
		Use this value if the widget size is unchanged, but the
		contents need to be remade because of changes to the zooming
		and panning state. Also, use this value if the widget is of
		class ImageOverlay (a subclass of Image) and the overlay data
		have changed. This is to make sure that the old overlay
		data are erased from the pixmap before new overlay data
		are considered.

		| (UPDATE_ZOOMPAN | UPDATE_MINMAX):
		Same as UPDATE_ZOOMPAN, but also update the XtNminval and
		XtNmaxval resources in order to use the exact available
		dynamic range. The UPDATE_MINMAX setting is
		only honoured when the XtNrange resource equals "dynamic".

		| UPDATE_NEWSIZE
		Use this if the image data are new (resources XtNimageImage,
		XtNimageBand, XtNimageData or the corresponding color-specific
		bands or data resources).

		| (UPDATE_NEWSIZE | UPDATE_MINMAX):
		Same as UPDATE_NEWSIZE,  but also update the XtNminval and
		XtNmaxval resources in order to use the exact available
		dynamic range. The UPDATE_MINMAX setting is
		only honoured when the XtNrange resource equals "dynamic".

		&&ImageRedisplayScreen
                | void ImageRedisplayScreen( ImageWidget wid,
                |    int x, int y, int width, int height );

		Calls the resize method with the private image update-variable
		equal to UPDATE_REDISPLAY. This will call the resize function
		for the particular pixeltype (and displaymethod), on the
		rectangle given by 'x', 'y', 'width' and 'height'. Finally,
		the expose method is called with region defined by the
		rectangle given by 'x', 'y', 'width' and 'height'. The
		coordinates are relative to the screen window.

		&&ImageRedisplayArea
                | int ImageRedisplayArea( ImageWidget wid,
                |    int x, int y, int width, int height );

		The coordinates are relative to the image (the actual data).
		Translates to screen coordinates and calls
		'ImageRedisplayScreen'.

		-&&

		&Zoom-handling routines:
		Also see the action function 'zoom_pan' in the section
		'Image actions' above.

		&&ImageTranslateZoomPanArgs
                | void ImageTranslateZoomPanArgs(
                |    ImageWidget wid, char* zoomarg,
                |    char* panxarg, char* panyarg, int mousex,
                |    int mousey, float* zoom, int* xpan,
                |    int* ypan );

		Interprets the arguments given to the 'zoom_pan' action
		function (here supplied as strings through 'zoomarg',
		'panxarg' and 'panyarg'). Uses this along with the supplied
		values for the current zoom factor ('zoom'), horizontal and
		vertical panning ('xpan' and 'ypan') to return updated values
		in 'zoom', 'xpan' and 'ypan'.

		&&ImageGetZoom
                | void ImageGetZoom( ImageWidget wid,
                |    float* zoom, int* xpan, int* ypan,
                |    double* factor );

		Returns the private image zoom-variable in 'zoom' and the
		private image factor-variable in 'factor'. The 'factor'
		variable holds the ratio of image pixels which are in the
		widget. The arguments 'xpan' and 'ypan' return the private
		pan-variables with the private xstart and ystart-variables
		added. (The latter two variables are inherited from the BIFF
		image data structure used for the XITE image format. They
		specify the relative position of the image in a global
		coordinate system.) The 'xpan' and 'ypan' variables are
		relative to the image data coordinate system.

		&&ImageSetZoom
                | void ImageSetZoom( ImageWidget wid,
                |    float zoom, int x, int y );

		Sets the zoom factor to 'zoom' and the horizontal and vertical
		panning to 'x' and 'y', respectively, after limiting the
		arguments to legal intervals. If the arguments are different
		from the previous zoom and pan values, the image is
		redisplayed and exposed. If all the widgets are bound to the
		same zoom factor (with the function 'ImageSetZoomAll')
		then all the widgets are treated equally.

		&&ImageSetZoomAll
                | int ImageSetZoomAll( Widget wid, int on );

		Description: Set zoom all flag, If 'on' = TRUE, all widgets
                will use the same zoom and pan parameters.

		&&ImageGetZoomAll
                | int ImageGetZoomAll( Widget wid );

		Return the value of the zoom all flag.

		&Routines for coordinates of image and screen window.

		These functions translate between the coordinate system
                of an image (the actual data) and the coordinate system
		of the window on screen. The upper left corner of a window,
		the origin, has coordinates (0, 0). The upper left corner
		of an image has coordinates (xstart, ystart).

		&&ImageScreenXtoPos
		| int ImageScreenXtoPos(ImageWidget wid, int x);

                'ImageScreenXtoPos' translates from horizontal coordinate on
                screen to horizontal coordinate in image data.
		The input coordinate 'x' is relative to the left edge of the
		screen window.

		&&ImageScreenYtoPos
		| int ImageScreenYtoPos(ImageWidget wid, int x);

		'ImageScreenYtoPos' behaves like 'ImageScreenXtoPos', except
		for the vertical coordinate. The input coordinate 'y' is
		relative to the upper edge of the screen window.

		&&ImageScreenToHeight
		| int ImageScreenToHeight(ImageWidget wid,
		|    int height);

		'ImageScreenToHeight' translates from 'height' in screen
		coordinate system to height in image coordinate system.
		It is allowed for 'height' to be larger than the height of
		the window.

		&&ImageScreenToWidth
		| int ImageScreenToWidth(ImageWidget wid,
		|    int width);

		'ImageScreenToWidth' translates from 'width' in screen
		coordinate system to width in image coordinate system.
		It is allowed for 'width' to be larger than the width of
		the window.

		&&ImagePosXtoScreen
		| int ImagePosXtoScreen(ImageWidget wid, int x,
		|    int right);

		'ImagePosXtoScreen' finds horizontal screen window coordinate
		from horizontal image coordinate.

		&&ImagePosYtoScreen
		| int ImagePosYtoScreen(ImageWidget wid, int y,
		|    int bottom);

		'ImagePosYtoScreen' finds vertical screen window coordinate
		from vertical image coordinate.

		&&ImageHeightToScreen
		| int ImageHeightToScreen(ImageWidget wid,
		|    int height, int maximum);

		'ImageHeightToScreen' translates from 'height' in image
		coordinate system to height in screen coordinate system.
		With nonzero 'maximum', round upwards to include
		an integer number of image pixels. Otherwise, round
		downwards.

		&&ImageWidthToScreen
		| int ImageWidthToScreen(ImageWidget wid,
		|    int width, int maximum);

		'ImageWidthToScreen' translates from 'width' in image
		coordinate system to width in screen coordinate system.
		With nonzero 'maximum', round upwards to include
		an integer number of image pixels. Otherwise, round
		downwards.

		&&ImageScreenXYtoAdr
		| ImageData ImageScreenXYtoAdr(ImageWidget wid,
		|    int x, int y, int options);

		'ImageScreenXYtoAdr' returns a pointer to the pixel which
		has screen window coordinates (x,y). 'options' determines
		what band the address is for (0: band, 1: red_band,
		2: green_band, 3: blue_band).

		&&ImagePosXYtoAdr
		| ImageData ImagePosXYtoAdr(ImageWidget wid,
		|    int x, int y, int options);

		'ImagePosXYtoAdr' returns a pointer to the pixel which has
		image coordinates (x,y). 'options' has the same meaning as
		for 'ImageScreenXYtoAdr'.

		-&&

		&Miscellaneous routines:

                &&ImageError
                | void ImageError( Widget wid, int messnr );

		Call the function XtAppErrorMsg with one of the error messages
		| ImageWidget: No error
		| ImageWidget: Not enough memory
		| ImageWidget: Bad parameters to zoom_pan
		| ImageWidget: Unknown DisplayMethod
		| ImageWidget: Unknown Pixeltype
		The 'messnr' argument determines the message printed.

		&&ImageInstallPixelType
                | void ImageInstallPixelType( ipixel* pix );
		|
		Insert the pixel type given by 'pix' into a list
		of pixeltypes. The first element inserted is at the end of
		the list with its 'next'-attribute referencing NULL. The last
		element inserted is referenced by the Image class record's
		'pixel_class' attribute.

		&&ImageKill
                | void ImageKill( Widget wid, XEvent* event,
                |    String* params, Cardinal* num_params );

		Calls XtDestroyWidget on the toplevel shell parent of 'wid'.

		&&ImageCopyImagePart
                | void ImageCopyImagePart( ImageWidget widto,
                |    ImageWidget widfrom );

		Sets the ImageWidget part of the widget 'widto' equal to the
		ImageWidget part of the widget 'widfrom'. No resizing or
		exposure is done.

		&&ImageCopyResources
                | void ImageCopyResources( ImageWidget widto,
                |    ImageWidget widfrom );

		Sets all the ImagePart resources of 'widto' equal to the
		ImagePart resources of 'widfrom'.

		&&ImageGetResources
		| void ImageGetResources( ImageWidget wid,
		|    ArgList *args, Cardinal *num_args );

		Returns a list of resource-value pairs for all the resources
		of 'wid'. If the function is called before the class is
		initialized, the resource list as specified in the class
		record is returned. If it is called after the class has been
		initialized, the returned list also contains the superclass
		resources.

		&&ImageGetImageResources
		| void ImageGetImageResources( ImageWidget wid,
		|    ArgList *args, Cardinal *num_args );

		Returns a list of resource-value pairs which are specific for
		the ImageWidget (not including super-class resources).

		&&ImageUpdateMinMax
		| void ImageUpdateMinMax( Widget wid );

		Search through the image for the minimum and maximum values
		and set the XtNminval and XtNmaxval resources equal to these
		values. This will affect the scaling of the image on its way
		from image data to pixmap. ImageUpdateMinMax does not by
		itself change the image display, it merely sets the private
		widget variables (but not via the SetValues method, so the
		resize method is not automatically invoked). The next call to
		the resize method or the public ImageRedisplay routine will
		update the image display.

		This routine is called by ImageRedisplay if the
		UPDATE_MINMAX bit is set. The UPDATE_MINMAX setting is
		only honoured when the XtNrange resource equals "dynamic".

		For a three-band rgb image, the XtNminval and XtNmaxval
		resources will be set equal to the global minimum and maximum.
		
See also:	'ImageResize(3)', 'ImageFormat(3)', 'ImageOverlay(3)',
                'ximage(3)', 'xshow(1)'

Doc:            Svein Bøe
Author:		Otto Milvang and Svein Bøe
________________________________________________________________

*/





/*I*

________________________________________________________________

		Include files and definitions
________________________________________________________________

*/


#include <xite/includes.h>
#include <ctype.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>
#include "ImageFormat.h"
#include "ImageP.h"
#include "ImageBytePix.h"
#include "ImageSignedBytePix.h"
#include "ImageUnsignedShortPix.h"
#include "ImageShortPix.h"
#include "ImageLongPix.h"
#include "ImageFloatPix.h"
#include "ImageDoublePix.h"
#include "ImageComplexPix.h"
#include "ImageDoubleComplexPix.h"
#include <xite/color.h>
#include <xite/Visual.h>
#include <xite/ShellWids.h>
#include XITE_MALLOC_H
#include <xite/message.h>
#include <xite/statistics.h>
#include <xite/debug.h>

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define Dist(a, b)    ( ((a) - (b)) < 0 ? ((b)-(a)) : ((a) - (b)) )
#define XimageWidth    ((ip->ximg)->width)
#define XimageHeight   ((ip->ximg)->height)
#define BandWidth      (ip->width)
#define BandHeight     (ip->height)

/* tab128_s and tab256_s are pointers to integer arrays with 128 or 256
  consecutive integer elements. The 128- and 256-element tables are used as
  XtNcolorPtr resources for Image or ImageOverlay widgets (128 for
  reduced-color display, 256 for full-color display). */

/* Linearly increasing table of 128 elements from 64 to 191. */
static int tab128_s[] = {  
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
};

/* Linearly increasing table of 256 elements from 0 to 255. */
static int tab256_s[] = {  
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
};



static XtResource resources[] = {
#define ioffset(field) XtOffsetOf(ImageRec, image.field)
#define coffset(field) XtOffsetOf(ImageRec, core.field)
  /* {name, class, type, size, ioffset, default_type, default_addr}, */
  { XtNimageImage, XtCImageImage, XtRPointer, sizeof(struct imagestruct *),
	ioffset(image), XtRPointer , NULL }, 
  { XtNimageBand, XtCImageBand, XtRPointer, sizeof(struct bandstruct *),
	ioffset(band), XtRPointer, NULL }, 
  { XtNimageRedBand, XtCImageRedBand, XtRPointer,sizeof(struct bandstruct *),
	ioffset(red_band), XtRPointer, NULL }, 
  { XtNimageGreenBand, XtCImageGreenBand, XtRPointer,
      sizeof(struct bandstruct *), ioffset(green_band), XtRPointer, NULL }, 
  { XtNimageBlueBand, XtCImageBlueBand, XtRPointer,
      sizeof(struct bandstruct *), ioffset(blue_band), XtRPointer, NULL }, 
  { XtNimageRgb, XtCImageRgb, XtRBoolean, sizeof(Boolean),
      ioffset(image_rgb), XtRString, (XtPointer) "False"},
  { XtNimageData, XtCImageData, XtRPointer, sizeof(ImageData),
	ioffset(data), XtRPointer, NULL },
  { XtNimageRedData, XtCImageRedData, XtRPointer, sizeof(ImageData),
	ioffset(red_data), XtRPointer, NULL },
  { XtNimageGreenData, XtCImageGreenData, XtRPointer, sizeof(ImageData),
	ioffset(green_data), XtRPointer, NULL },
  { XtNimageBlueData, XtCImageBlueData, XtRPointer, sizeof(ImageData),
	ioffset(blue_data), XtRPointer, NULL },
  { XtNimageTitle, XtCImageTitle, XtRPointer, sizeof(char *),
	ioffset(title), XtRString, NULL }, 
  { XtNimagePixtype, XtCImagePixtype, XtRPointer, sizeof(char *),
	ioffset(pixname), XtRString, NULL }, 
  { XtNimageWidth, XtCImageWidth, XtRInt, sizeof(int),
	ioffset(width), XtRInt, NULL },
  { XtNimageHeight, XtCImageHeight, XtRInt, sizeof(int),
	ioffset(height), XtRInt, NULL }, 
  { XtNvariableAspect, XtCVariableAspect, XtRBoolean, sizeof(Boolean),
      ioffset(variable_aspect), XtRString, (XtPointer) "False"},
  { XtNimageLineLength, XtCImageLineLength, XtRInt, sizeof(int),
	ioffset(line_length), XtRInt, NULL }, 
  { XtNimageXstart, XtCImageXstart, XtRInt, sizeof(int),
	ioffset(xstart), XtRInt, NULL }, 
  { XtNimageYstart, XtCImageYstart, XtRInt, sizeof(int),
	ioffset(ystart), XtRInt, NULL }, 
  { XtNdisplayMethod, XtCDisplayMethod, XtRInt, sizeof(int),
	ioffset(displaymethod), XtRInt, NULL },
  { XtNprintValues, XtCPrintValues, XtRBoolean, sizeof(Boolean),
	ioffset(print_values), XtRString, (XtPointer) "True"},
  { XtNcolortab, XtCColortab, XtRInt, sizeof(int),
	ioffset(colortab), XtRImmediate, (XtPointer) -1 },
  { XtNcolorCells, XtCColorCells, XtRInt, sizeof(int),
	ioffset(colorcells), XtRImmediate, (XtPointer) -1 },
  { XtNcolorPtr, XtCColorPtr, XtRPointer, sizeof(int *),
	ioffset(colorptr), XtRPointer, NULL },
  { XtNvisual, XtCVisual, XtRVisual, sizeof(Visual*),
	ioffset(visual), XtRPointer, NULL},
  { XtNscaletype, XtCScaletype, XtRInt, sizeof(int),
	ioffset(scaletype), XtRInt, NULL },
  { XtNscalea, XtCScalea, XtRFloat, sizeof(float),
	ioffset(scalea), XtRFloat, NULL },
  { XtNscaleb, XtCScaleb, XtRFloat, sizeof(float),
	ioffset(scaleb), XtRFloat, NULL },
  { XtNminval, XtCMinval, XtRFloat, sizeof(float),
	ioffset(minval), XtRFloat, NULL },
  { XtNmaxval, XtCMaxval, XtRFloat, sizeof(float),
	ioffset(maxval), XtRFloat, NULL },
  { XtNrange, XtCRange, XtRString, sizeof(String),
        ioffset(range), XtRString, "dynamic" },
  { XtNfont,  XtCFont, XtRFontStruct, sizeof(XFontStruct *),
	ioffset(font[0]),XtRString, "XtDefaultFont"},
  { XtNfont1,  XtCFont1, XtRFontStruct, sizeof(XFontStruct *),
	ioffset(font[1]),XtRString, NULL},
  { XtNfont2,  XtCFont2, XtRFontStruct, sizeof(XFontStruct *),
	ioffset(font[2]),XtRString, NULL},
  { XtNfont3,  XtCFont3, XtRFontStruct, sizeof(XFontStruct *),
	ioffset(font[3]),XtRString, NULL},
  { XtNcallback, XtCCallback, XtRCallback, sizeof(XtCallbackList),
	ioffset(button_action), XtRCallback, NULL },
  { XtNresizeCallback, XtCResizeCallback, XtRCallback, sizeof(XtCallbackList),
	ioffset(resize_action), XtRCallback, NULL },
  { XtNimageUpdate,  XtCImageUpdate, XtRInt, sizeof(int),
	ioffset(update), XtRInt, NULL},
  { XtNmenubar,  XtCMenubar, XtRBoolean, sizeof(Boolean),
	ioffset(menubar), XtRString, (XtPointer) "True"},

#undef ioffset
#undef coffset
};




/*I*

________________________________________________________________

		Action function declarations
________________________________________________________________

*/

static void Button( Widget wid, XEvent *event, String *params, Cardinal *num_params );
void ImageKill( Widget wid, XEvent *event, String *params, Cardinal *num_params );
static void Nop( Widget wid, XEvent *event, String *params, Cardinal *num_params );
static void ZoomPan( Widget wid, XEvent *event, String *params, Cardinal *num_params );
static void WMColormapAction( Widget wid, XEvent *event, String *params, Cardinal *num_params );


/*I*

________________________________________________________________

		Actions table and default tranlations table
________________________________________________________________

*/

static XtActionsRec actions[] =
{
  /* {name, procedure}, */
  {"nop", 	 Nop},
  {"notify",     Button},
  {"kill",	 ImageKill},
  {"zoom_pan",	 ZoomPan},
  {"WMColormap", WMColormapAction},
};

static char translations[] =
"<Key>Q:                kill()                             \n\
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
 None <Btn1Up>:         notify()                           \n\
 None <Btn2Up>:         notify()                           \n\
 None <Btn3Up>:         notify()                           \n\
 <MouseMoved>:          notify()";



/* Method function declarations. */

static void ClassInitialize(void );
static XtGeometryResult Geometry ( Widget wid, XtWidgetGeometry *request, XtWidgetGeometry *answer );
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Kill ( Widget wid );
static void Realize(Widget wid, XtValueMask *valueMask, XSetWindowAttributes *attrs);
static void Redisplay ( Widget wid, XEvent *event, Region region );
static void Resize ( Widget wid );
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static int SetZoomParam ( ImageWidget wid, double zoom, int x, int y );



/*I*

________________________________________________________________

		Image Class Record Initialization
________________________________________________________________

*/


ImageClassRec imageClassRec = {
  {
    /* core fields */
    /* superclass		*/	(WidgetClass) &compositeClassRec,
    /* class_name		*/	"Image",
    /* widget_size		*/	sizeof(ImageRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,/* No methods in the class part */
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	actions,
    /* num_actions		*/	XtNumber(actions),
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Kill,
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
    /* query_geometry		*/	Geometry,
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
    /* count, # wids in list    */	0,
    /* widget_list_length       */      0,
    /* widget_list              */      NULL,
    /* pixel_class              */	NULL,
    /* zoom_all                 */      0,
  }
};

WidgetClass imageWidgetClass = (WidgetClass)&imageClassRec;




/* Auxiliary structure definitions and static structure variables. */

typedef struct 
{
  short x,y;
  unsigned short width, height;
  Window window;
} EventRectangle;



#define ERR_OK		0
#define ERR_MEM		1
#define ERR_PARAM	2
#define ERR_DM		3
#define ERR_PT		4

static String err_mess[] = {
  "ImageWidget: No error",
  "ImageWidget: Not enough memory",
  "ImageWidget: Bad parameters to zoom_pan",
  "ImageWidget: Unknown DisplayMethod",
  "ImageWidget: Unknown Pixeltype",
};

void ImageError(Widget wid, int messnr)
{
  XtAppErrorMsg(
    XtWidgetToApplicationContext(wid),
    "Error","ImageWidget","XtToolkitError",
    err_mess[messnr],
    (String *)NULL, (Cardinal *)NULL);
}



/*L:ImageHasColormap*

________________________________________________________________

		ImageHasColormap
________________________________________________________________

Name:		ImageHasColormap
Syntax:		| int ImageHasColormap(wid)
                | ImageWidget wid;
Description:	
Return value:	| False if Visual Type is StaticGray, StaticColor or TrueColor
                | True if Visual type is GrayScale, PseudoColor or DirectColor
Author:		Otto Milvang
________________________________________________________________

*/



int ImageHasColormap(ImageWidget wid)
{
  ENTER_FUNCTION_DEBUG("Image.c: ImageHasColormap");
  LEAVE_FUNCTION_DEBUG("Image.c: ImageHasColormap");
  return(wid->image.has_colormap);
}



/*L:ImageGetDisplayMethod*

________________________________________________________________

		ImageGetDisplayMethod
________________________________________________________________

Name:		ImageGetDisplayMethod

Syntax:		| static display_method ImageGetDisplayMethod(int defaultdm,
                |    int class, int planes, int cells);

Description:	Find the display method from 'class', 'planes' and 'cells'.

		If 'defaultdm' >= 16, it is taken as a complete value for
		XtNdisplayMethod and not constructed from visual class and
		depth.

                If 'defaultdm' is less than 16, it is added to the display
		method determined from class, planes and cells. This can
		be used to switch from PSEUDO_COLOR_8A to PSEUDO_COLOR_8B
		and from DIRECT_COLOR_24_A to DIRECT_COLOR_24B (with
		'defaultdm' equal to 1).

		'cells' is currently not used.

Return value:	New display method.

Author:		Otto Milvang
________________________________________________________________

*/


static display_method ImageGetDisplayMethod(int defaultdm, int class, int planes, int cells)
{
  int dm;

  ENTER_FUNCTION_DEBUG("Image.c: ImageGetDisplayMethod");
  FPRINTF2("    defaultdm= %d\n", defaultdm);

  dm = defaultdm < 16 ? class * 0x1000 + planes * 0x10 : defaultdm;
  if (defaultdm < 16 &&
      (class == PseudoColor || class == GrayScale || class == DirectColor))
    dm = dm + defaultdm;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageGetDisplayMethod (soon)");

  if (dm/16 != defaultdm/16 && defaultdm >=16) return(0);
  if (dm == STATIC_GRAY_1)    return(STATIC_GRAY_1);
  if (dm == STATIC_GRAY_2)    return(STATIC_GRAY_2);
  if (dm == STATIC_GRAY_4)    return(STATIC_GRAY_4);
  if (dm == STATIC_GRAY_8)    return(STATIC_GRAY_8);
  if (dm == GRAY_SCALE_4)     return(GRAY_SCALE_4);
  if (dm == GRAY_SCALE_8)     return(GRAY_SCALE_8);
  if (dm == PSEUDO_COLOR_8_A) return(PSEUDO_COLOR_8_A);
  if (dm == PSEUDO_COLOR_8_B) return(PSEUDO_COLOR_8_B);
  if (dm == TRUE_COLOR_24)    return(TRUE_COLOR_24);
  if (dm == DIRECT_COLOR_24_A)  return(DIRECT_COLOR_24_A);
  if (dm == DIRECT_COLOR_24_B)  return(DIRECT_COLOR_24_B);

  return(0);
}





/*L:ImageInstallPixelType*

________________________________________________________________

		ImageInstallPixelType
________________________________________________________________

Name:		ImageInstallPixelType
Syntax:		| void ImageInstallPixelType(ipixel *pix);
Description:    Insert the pixel type given by 'pix' into a list of
                pixeltypes. The first element inserted, is at the end of the
		list with its 'next'-attribute referencing NULL. The last
		element inserted is referenced by the Image class record's
		'pixel_class' attribute. 
Author:		Otto Milvang
Doc:            Svein Bøe
________________________________________________________________

*/

void ImageInstallPixelType(ipixel *pix)
{
  ImageClassPart *ic = &imageClassRec.image_class;
  ipixelptr ptr;

  ENTER_FUNCTION_DEBUG("Image.c: ImageInstallPixelType");

  ptr = (ipixelptr) malloc(sizeof(ipixel));
  if (ptr == NULL) XtError("malloc() failed");

  /* Copy and determine attributes. */

  ptr->pixelname   = pix->pixelname;

  ptr->printformat =
    ImagePixPrintFormat(ImagePixNameToBIFFPixTyp(ptr->pixelname));
  if (!ptr->printformat) ptr->printformat = pix->printformat;
  
  sscanf(ptr->printformat, "%*c%*c%d", &ptr->txtlen); /* Skip "%-". */

  if (!strcmp(ptr->pixelname, ImageComplexPixel) ||
      !strcmp(ptr->pixelname, ImageDoubleComplexPixel))
    ptr->txtlen    = 2 * ptr->txtlen + 2;

  /* Copy the determined attributes back into the static class-specific
   * structure. */
  pix->printformat = ptr->printformat;
  pix->txtlen      = ptr->txtlen;

  /* Copy remaining attributes from class-specific structure. */

  ptr->pixelsize   = pix->pixelsize;
  ptr->resize      = pix->resize;
  ptr->printf      = pix->printf;

  ptr->scaletype   = pix->scaletype;
  ptr->scalea      = pix->scalea;
  ptr->scaleb      = pix->scaleb;
  ptr->minval      = pix->minval;
  ptr->maxval      = pix->maxval;

  /* Insert the object at the head of the list. */

  ptr->next = ic->pixel_class;
  ic->pixel_class = ptr;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageInstallPixelType");
}


Widget *imageWidgetList(int *num_widgets)
{

  *num_widgets =
    ((ImageWidgetClass) imageWidgetClass)->image_class.count;
  return(((ImageWidgetClass) imageWidgetClass)->image_class.widget_list);

} /* imageWidgetList() */



/*L*

________________________________________________________________

		ClassInitialize
________________________________________________________________

Name:		ClassInitialize
Syntax:		| static void ClassInitialize(void)
Description:    Install various pixeltypes, as a list of objects referenced
                by the Image class record's 'pixel_class' attribute.
		Allocate space for 16 widget pointers.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static void ClassInitialize(void)
{
  ImageClassPart *ic = &imageClassRec.image_class;

  ENTER_FUNCTION_DEBUG("Image.c: ClassInitialize");

  /* XawInitializeWidgetSet(); */

  /* Allocate space for an array of pointers to 16 widgets. The attribute
     'widget_list' will refer to the first Image widget instance created. */

  ic->widget_list_length = 16;
  ic->widget_list = (Widget *) malloc(16 * sizeof(Widget));
  if (ic->widget_list == NULL) XtError("malloc() failed");

  /* Make a list of the available pixel types, referenced by the Image class
     record's attribute pixel_class. */

  ImageInstallPixelType(BytePixel());
  ImageInstallPixelType(SignedBytePixel());
  ImageInstallPixelType(ShortPixel());
  ImageInstallPixelType(UnsignedShortPixel());
  ImageInstallPixelType(LongPixel());
  ImageInstallPixelType(FloatPixel());
  ImageInstallPixelType(ComplexPixel());
  ImageInstallPixelType(DoublePixel());
  ImageInstallPixelType(DoubleComplexPixel());

  LEAVE_FUNCTION_DEBUG("Image.c: ClassInitialize");
} 




/*L*

________________________________________________________________

		Initialize
________________________________________________________________

Name:		Initialize
Syntax:		| static void Initialize(Widget request, Widget new,
                |    ArgList args, Cardinal *num_args);
Description:    Initialize all the attributes of the Image Widget instance.
                Check public attributes (may have been set via resource
		specifications) and initialize private ones.
Chaining:       Downward.
Return value:
Author:		Otto Milvang and Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/

static void extractBiffAttributes(Widget wid)
{
  ImagePart *imPart;
  int i;
  ENTER_FUNCTION_DEBUG("Image.c: extractBiffAttributes");

  imPart = &(((ImageWidget) wid)->image);

  /* Use the green band if band is empty. */
  if (imPart->band == NULL && imPart->green_band != NULL)
    imPart->band = imPart->green_band;
  if (   (imPart->data   == 0 && imPart->band == 0)
      || (imPart->width  == 0 && imPart->band == 0)
      || (imPart->height == 0 && imPart->band == 0) )
    XtErrorMsg("Image", "Initialize", "Image",
	       "Unknown image address, width or height", NULL,0);

  /* Make sure image has a title, stripped for trailing blanks. */
  if (imPart->title == 0) {
    imPart-> title = (char *) malloc(33);
    if (imPart->title == NULL) ImageError(wid, ERR_MEM);
    imPart-> title[0] = 0;
    if (imPart->image)
      strcpy(imPart->title, ImageTitle((IMAGE) imPart->image));
    imPart->title[32] = 0;
  }
  i = strlen(imPart->title);
  while (i > 0 && imPart->title[i] == ' ') imPart->title[i--] = 0;
  if (strlen(imPart->title) == 0) strcpy(imPart->title,"Image");

  /* Copy attributes of the BIFF image structure to widget instance
     variables. */

  if (imPart->band) {
    /* imPart->data refers to the actual band pixel data. */
    imPart->data          = ImageAddress((IBAND) imPart->band);
    imPart->pixname       = ImagePixName((IBAND) imPart->band);
    imPart->width         = ImageWidth((IBAND) imPart->band);
    imPart->height        = ImageHeight((IBAND) imPart->band);
    imPart->line_length   = ImageLineLength((IBAND) imPart->band);
    imPart->xstart        = ImageXstart((IBAND) imPart->band);
    imPart->ystart        = ImageYstart((IBAND) imPart->band);
  }

  FPRINTF3("    pixname: %s, width: %d", imPart->pixname, imPart->width);
  FPRINTF3(", height: %d, line_length: %d\n", imPart->height,
	   imPart->line_length);

  if (imPart->red_band && imPart->green_band && imPart->blue_band &&
      !(imPart->red_band   == imPart->green_band &&
	imPart->green_band == imPart->blue_band))
    imPart->image_rgb = True;
  else imPart->image_rgb = False;

  /* Find addresses of actual image pixel data, not the BIFF band info
     pointer. */
  if (imPart->red_band)
    imPart->red_data   = ImageAddress((IBAND) imPart->red_band);
  if (imPart->green_band)
    imPart->green_data = ImageAddress((IBAND) imPart->green_band);
  if (imPart->blue_band)
    imPart->blue_data  = ImageAddress((IBAND) imPart->blue_band);

  /* Use the green data if data is empty. */
  if (imPart->data == NULL && imPart->green_data != NULL)
    imPart->data = imPart->green_data;

  /* Let any missing primary get its data from 'imPart->data'. This will
     create equal primaries for a one-band gray-scale image. */
  if (!imPart->red_data)   imPart->red_data   = imPart->data;
  if (!imPart->green_data) imPart->green_data = imPart->data;
  if (!imPart->blue_data)  imPart->blue_data  = imPart->data;

  LEAVE_FUNCTION_DEBUG("Image.c: extractBiffAttributes");

} /* extractBiffAttributes */


static ipixelptr findPixelType(ImagePart *imPart, ImageClassPart *imClass)
{
  ipixelptr pptr;

  ENTER_FUNCTION_DEBUG("Image.c: findPixelType");

  /* Search for the present pixel type in the list of registered
     pixel types. */
  if (imPart->pixname) {
    pptr = imClass->pixel_class;
    while (pptr && strcmp(pptr->pixelname, imPart->pixname))
      pptr = pptr->next;
  } else pptr = NULL;
  imPart->pixtype = pptr;

  /*  if (pptr == NULL) ImageError(iwid, ERR_PT); */

  if (pptr != NULL) {
    FPRINTF2("    pixelsize: %d", pptr->pixelsize);
    FPRINTF2(", pixelname: %s\n", pptr->pixelname);
  } else {
    FPRINTF1("    Pixeltype not found.\n");
  }

  LEAVE_FUNCTION_DEBUG("Image.c: findPixelType");
  return(pptr);

} /* findPixelType */

static int *deftab(int n)
{
  int *tab = NULL;

  switch(n) {
  case 128: tab = tab128_s; break;
  case 256: tab = tab256_s; break;
  default:  tab = NULL;     break;
  }

  return(tab);
}


static void check_color_resources(Widget current, Widget new)
{
  /* Let XtNdisplayMethod have priority in setting color resources. */

  int color_count, class;
  ImagePart *nip = &(((ImageWidget) new)->image);
  ImagePart *cip = NULL;

  if (current) cip = &(((ImageWidget) current)->image);

  class = ClassOfVisual(nip->visual);

  nip->displaymethod =
    ImageGetDisplayMethod(nip->displaymethod, class, new->core.depth,
			  nip->cells);
  FPRINTF2("    Displaymethod: %0x\n", nip->displaymethod);
  if (!nip->displaymethod) ImageError(new, ERR_DM);

  if (cip && (nip->displaymethod != cip->displaymethod)) {
    /* Changing display method. */

    nip->colorcells = -1;
    nip->colorptr   = NULL;
  }

  color_count = MapEntriesOfVisual(nip->visual);
  
  if ((nip->displaymethod & 3) == 0) {
    /* Reduced-color display. */
    
    if (nip->colorcells < 0) nip->colorcells = color_count / 2;
  } else {
    /* Full-color display. */
    
    if (nip->colorcells < 0) nip->colorcells = color_count;
  }

  nip->colorptr = nip->colorptr ? nip->colorptr : deftab(nip->colorcells);
  if (nip->colortab < 0) nip->colortab = 0;

  return;

} /* check_color_resources() */

static void getVisualParameters(Widget request, Widget new, ArgList args, Cardinal *num_args, Display **dpy, int *screen)
{
  /* 'request' and 'new' contain resources specified by user as well as those
   * given as 'args' by the application.
   */
  int class;
  ImagePart *ip = &(((ImageWidget) new)->image);

  ENTER_FUNCTION_DEBUG("Image.c: getVisualParameters");

  *dpy = XtDisplay(new);
  *screen = DefaultScreen(*dpy);

  if (ip->visual == NULL) {
    FPRINTF1("    Use default visual !!!!!!!!!!!!!!!\n");
    ip->visual = DefaultVisual(*dpy,*screen);
  }

  class = ClassOfVisual(ip->visual);
  ip->has_colormap =
    (class == PseudoColor || class == GrayScale || class == DirectColor);
  ip->cells = MapEntriesOfVisual(ip->visual);

  check_color_resources(NULL, new);

  LEAVE_FUNCTION_DEBUG("Image.c: getVisualParameters");

  return;

} /* getVisualParameters() */


static void initializeParams(ImagePart *ip)
{

  ENTER_FUNCTION_DEBUG("Image.c: initializeParams");

  ip -> factor      = 1.0;
  ip -> resize      = FALSE;
  ip -> update      = UPDATE_RESIZE;
  ip -> zoom        = 1.0;
  ip -> xpan        = 0;
  ip -> ypan        = 0;
  ip -> xptr        = NULL;
  ip -> yptr        = NULL;
  ip -> orig_x      = 0;
  ip -> orig_y      = 0;
  ip -> orig_width  = 0;
  ip -> orig_height = 0;

  if (!ip ->variable_aspect) {
    ip -> x_aspect = ip -> width;
    ip -> y_aspect = ip -> height;
  } else {
    ip -> x_aspect = 0;
    ip -> y_aspect = 0;
  }

  LEAVE_FUNCTION_DEBUG("Image.c: initializeParams");

} /* initializeParams */


static void inheritFromCore(ImageWidget iwid)
{
  ImagePart *ip = &(iwid->image);

  ENTER_FUNCTION_DEBUG("Image.c: inheritFromCore");

  ip->internal_width  = (int) iwid->core.width;
  ip->internal_height = (int) iwid->core.height;

  LEAVE_FUNCTION_DEBUG("Image.c: inheritFromCore");

} /* inheritFromCore */


static void getScaleAndExtrema(Widget wid)
{
  ImagePart *ip  = &(((ImageWidget) wid)->image);
  ipixelptr pptr = ip->pixtype;

  ENTER_FUNCTION_DEBUG("Image.c: getScaleAndExtrema");

  if (pptr && ip->scalea == 0.0 && ip->scaleb == 0.0) {
    /* Copy from installed pixel type. */
    ip->scalea = pptr->scalea;
    ip->scaleb = pptr->scaleb;
  }

  if (!ip->range) ip->range = "dynamic";

  if (strcmp(ip->pixtype->pixelname, ImageBytePixel) == 0 &&
      strcmp(ip->range, "dynamic") == 0)
    ip->range = "pixelType";

  if (strcmp(ip->range, "pixelType") == 0) {
    if (pptr) {
      /* Copy from installed pixel type. */
      ip->minval = pptr->minval;
      ip->maxval = pptr->maxval;
    } else ImageUpdateMinMax(wid);
  }
  else if (strcmp(ip->range, "dynamic") == 0) {
    ImageUpdateMinMax(wid);
  }
  else if (strcmp(ip->range, "fixed") == 0) {
    if (ip->minval >= ip->maxval) {
      ImageUpdateMinMax(wid);
    }
  } else ImageUpdateMinMax(wid);

  FPRINTF3("    ip->minval: %f, ip->maxval: %f\n", ip->minval, ip->maxval);

  LEAVE_FUNCTION_DEBUG("Image.c: getScaleAndExtrema");

} /* getScaleAndExtrema */


static void initializeZooming(ImageWidget imWid, ImageClassPart* imClass)
{

  ENTER_FUNCTION_DEBUG("Image.c: initializeZooming");

  if (imClass->zoom_all && imClass->count) {
    /* All images are supposed to be zoomed equally, and this is not the
     * first image widget created.
     */

    int xpan, ypan;
    float zoom;

    ImageGetZoom((ImageWidget) imClass->widget_list[0],
		 &zoom, &xpan, &ypan, NULL);

    /* Set the same zoom state for this new widget. */
    SetZoomParam(imWid, zoom, xpan, ypan);
  } else {
    /* Independent zooming, or this is the first image widget created. */

    ImagePart *imPart;
    int core_w, core_h, im_w, im_h;

    imPart = &(imWid->image);
    im_w   = imPart->width;
    im_h   = imPart->height;
    core_w = imWid->core.width;
    core_h = imWid->core.height;
    FPRINTF3("    Core w,h = %d, %d\n", core_w, core_h);

    /* zoom > 1 if core (widget) size greater than BIFF image. */
    imPart->zoom = ((double)core_w/im_w > (double)core_h/im_h) ? 
      (double)core_w/im_w : (double)core_h/im_h;
  }

  LEAVE_FUNCTION_DEBUG("Image.c: initializeZooming");

} /* initializeZooming */


static int initCoreSize(ImageWidget imWid, Dimension *new_w, Dimension *new_h)
{
  ImagePart *imPart = &(imWid->image);
  Dimension cw, ch, im_w, im_h;

  *new_w = cw = imWid->core.width;
  *new_h = ch = imWid->core.height;

  if (cw != 0 && ch != 0) {
    /* Non-zero sizes already (from some resource setting).
     * Check that they are valid (in relation to aspect ratio) and change
     * if necessary. */

    /* Give priority to width. */
    if (!imPart->variable_aspect) {
      if ((*new_h = (cw * imPart->y_aspect) / imPart->x_aspect) != ch) {

	return(1); /* Change in core height, call XtResizeWidget(). */
      }
    }

    return(0); /* No change in core sizes, call Resize(). */
  }

  /* None or only one has been set via resource. */

  im_w   = imPart->width;  /* BIFF image width. */
  im_h   = imPart->height;

  if (cw == 0 && ch == 0) {
    /* None set via resources. */

    *new_w = im_w;
    *new_h = im_h;
  } else if (cw != 0) {
    /* Width set via resource. */

    *new_w = cw;
    if (!imPart->variable_aspect) {
      *new_h = (*new_w * imPart->y_aspect) / imPart->x_aspect;
    } else {
      *new_h = *new_w; /* As good as any. */
    }
  } else if (ch != 0) {
    /* Height set via resource. */

    *new_h = ch;
    if (!imPart->variable_aspect) {
      *new_w = (*new_h * imPart->x_aspect) / imPart->y_aspect;
    } else {
      *new_w = *new_h; /* As good as any. */
    }
  }

  return(1); /* Change in core size, call XtResizeWidget(). */

} /* initCoreSize() */

static int limitCoreSize(ImageWidget imWid, Dimension *new_w, Dimension *new_h)
{
  ImagePart *imPart = &(imWid->image);

  /* Make sure width and height are not above 1000 and 800 respectively.
   * Reduce them both at the same rate.
   */

  if (*new_w <= 1000 && *new_h <= 800) return(0); /* Core size unchanged. */

  while (*new_w > 1000 || *new_h > 800) {
    if (*new_w == 1 || *new_h == 1) {
      /* Avoid zero width or height widget. */

      if (!imPart->variable_aspect) {
	imPart->variable_aspect = True;
	imPart->x_aspect        = 0;
	imPart->y_aspect        = 0;
	*new_w = MIN(*new_w, 1000);
	*new_h = MIN(*new_h, 800);
	Warning(1, "%s\n",
	  "Image.c: limitCoreSize: Had to turn off fixed aspect ratio.");
	break;
      }
    }
    *new_w >>= 1;  /* Divide by 2. */
    *new_h >>= 1;
  }

  return(1); /* Core size has changed, call XtResizeWidget(). */

} /* limitCoreSize() */

static void setCoreSizeAndResize(Widget new, ImageClassPart *imClass)
{
  ImageWidget imWid = (ImageWidget) new;
  int status;
  Dimension new_w, new_h;

  ENTER_FUNCTION_DEBUG("Image.c: setCoreSizeAndResize");

  status =  initCoreSize(imWid, &new_w, &new_h);
  status |= limitCoreSize(imWid, &new_w, &new_h);

  FPRINTF3("    new_w=%d, new_h=%d\n", new_w, new_h);

  imWid->image.orig_width  = new_w;
  imWid->image.orig_height = new_h;
  imWid->image.orig_x      = imWid->core.x;
  imWid->image.orig_y      = imWid->core.y;

  if (XtClass(new) != imageWidgetClass) {

    /* Don't call Resize method if widget is a subclass of "Image",
     * i.e. "ImageOverlay" because the "ImageOverlay" initialize method
     * will call its superclass resize method. */

    /* Communicate the determined geometry to the "ImageOverlay" subclass. */
    imWid->core.width  = new_w;
    imWid->core.height = new_h;

    LEAVE_FUNCTION_DEBUG("Image.c: setCoreSizeAndResize");
    return;
  }

  if (status) {
    /* Set the new size and call the Resize method. */

    /* The following call will call the widget resize method if new_w and/or
     * new_h is different from the current widget core sizes. Therefore
     * it is important that the initCoreSize() and limitCoreSize() functions
     * above did not change the core attributes, only returned new values.
     */
    XtResizeWidget(new, new_w, new_h, 1);
  } else {
    /* Unchanged core size. */

    Resize(new);
  }

  LEAVE_FUNCTION_DEBUG("Image.c: setCoreSizeAndResize");

} /* setCoreSizeAndResize */


static Widget *checkWidgetTable(Widget wid, ImageClassPart *imClass)
{
  Widget *widgetPtr = imClass->widget_list;

  ENTER_FUNCTION_DEBUG("Image.c: checkWidgetTable");

  /* Expand table of pointers to widgets, if needed. */
  if (imClass->count >= imClass->widget_list_length) {
    imClass->widget_list_length += 16;
    widgetPtr =
      (Widget *) realloc(imClass->widget_list, 
			 imClass->widget_list_length * sizeof(Widget));
    if (!widgetPtr) ImageError(wid, ERR_MEM);
    else imClass->widget_list = widgetPtr;
  }

  LEAVE_FUNCTION_DEBUG("Image.c: checkWidgetTable");
  return(widgetPtr);

} /* checkWidgetTable */


static void initFonts(ImagePart *imPart)
{
  int i, j;
  XFontStruct *fh;

  ENTER_FUNCTION_DEBUG("Image.c: initFonts");

  for(i=1;  i<=3; i++) {
    if (imPart->font[i] == NULL) imPart->font[i] = imPart->font[0];
    FPRINTF2("    Font %d\n", i);
  }
  
  /* Sort font[1], font[2] and font[3] with increasing width.
   * Don't include font[0] in this sorting. Why not?
   */
  for (i=1; i<=2; i++) {
    for (j=i+1; j<=3; j++)
      if (imPart->font[i]->max_bounds.width >
	  imPart->font[j]->max_bounds.width) {
	fh = imPart->font[i];
	imPart->font[i] = imPart->font[j];
	imPart->font[j] = fh;
      }
  }

  LEAVE_FUNCTION_DEBUG("Image.c: initFonts");

} /* initFonts */




static void Initialize(Widget request, Widget new, ArgList args,
                             Cardinal *num_args)
{
  ImageWidget iwid   = (ImageWidget) new;
  ImageClassPart *ic = &imageClassRec.image_class;
  ImagePart *ip;
  Display *dpy;
  int screen;

  ENTER_FUNCTION_DEBUG("Image.c: Initialize");

  if (new == NULL) {
    LEAVE_FUNCTION_DEBUG("Image.c: Initialize");
    return;
  }
  ip = &(iwid->image);

  getVisualParameters(request, new, args, num_args, &dpy, &screen);

  /* Allocate memory for XImage structure, but do not supply pointer to
   * image data (pass along NULL). The actual image is drawn into the
   * widget in the expose method, with the XPutImage function. */
  ip -> ximg = XCreateImage(dpy, ip->visual, new->core.depth,
			    ip->displaymethod == STATIC_GRAY_1 ?
			    XYBitmap : ZPixmap, 
			    0, NULL, 0, 0, 8, 0);
  (ip -> ximg)->bitmap_bit_order = BitmapBitOrder(dpy);

  extractBiffAttributes(new);
  (void) findPixelType(ip, ic); /* Depends on ip->name, found in prev. line. */
  /* if (!findPixelType(ip)) ImageError(iwid, ERR_PT); */
  if (ip->line_length == 0)
    ip->line_length = ip->width * (ip->pixtype ? ip->pixtype->pixelsize : 1);
  getScaleAndExtrema(new);
  initializeParams(ip);
  inheritFromCore(iwid);
  setCoreSizeAndResize(new, ic);
  initializeZooming(iwid, ic);
  checkWidgetTable(new, ic);
  ip->gc = DefaultGC(dpy, screen); /* In case Widget isn't realized */
  initFonts(ip);

  /* Put this widget in table of widgets for this class. */
  ic->widget_list[ic->count] = new;
  ic->count ++;

  LEAVE_FUNCTION_DEBUG("Image.c: Initialize");

} /* Initialize() */

/*L*

________________________________________________________________

		Realize
________________________________________________________________

Name:		Realize
Syntax:		| static void Realize (Widget wid,
                |    XtValueMask *valueMask, XSetWindowAttributes *attrs);
Description:    Set window attributes and create window for widget.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

static void Realize(Widget wid, XtValueMask *valueMask, XSetWindowAttributes *attrs)
{
  Display *dpy;
  int screen;
  XGCValues gcvalues;

  ENTER_FUNCTION_DEBUG("Image.c: Realize");
  FPRINTF2("    *valueMask: %04lx \n", *valueMask);

  dpy    = XtDisplay(wid);
  screen = DefaultScreen(dpy);
  gcvalues.foreground = WhitePixel(dpy, screen);
  gcvalues.background = BlackPixel(dpy, screen);

  XtCreateWindow(wid, InputOutput, ((ImageWidget) wid)->image.visual, 
		 *valueMask, attrs);
  ((ImageWidget) wid)->image.gc = 
    XCreateGC(dpy, XtWindow(wid),
	      GCForeground | GCBackground, &gcvalues);
  XDefineCursor(dpy, XtWindow(wid), XCreateFontCursor(dpy,XC_crosshair));

  LEAVE_FUNCTION_DEBUG("Image.c: Realize");
}



static void WMColormapAction(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  Boolean menubar, enter = 1;
  Widget shell, form;
  Window win[3];

  ENTER_FUNCTION_DEBUG("Image.c: WMColormapAction");

  if (!XtIsRealized(wid)) {
    FPRINTF1("    Not realized.\n");
    LEAVE_FUNCTION_DEBUG("Image.c: WMColormapAction");
    return;
  }

  if (!XtIsSubclass(wid, imageWidgetClass)) {
    FPRINTF1("    Not subclass of imageWidget.\n");
    LEAVE_FUNCTION_DEBUG("Image.c: WMColormapAction");
    return;
  }

  if (!ImageHasColormap((ImageWidget) wid)) {
    FPRINTF1("    Does not have colormap.\n");
    LEAVE_FUNCTION_DEBUG("Image.c: WMColormapAction");
    return;
  }

  if (*num_params > 0 && params[0][0] != 'E') enter = 0;

  shell = TopLevelWidget(wid);
  form  = XtParent(wid);

  XtVaGetValues(wid, XtNmenubar, &menubar, NULL);

  /* This test is to avoid "Segmentation fault" for applications which use
   * old versions of the ximage toolkit. In such applications the parent of
   * the image widget will not be class Form, but class Box, and no menubar
   * will be present even if the image widget thinks otherwise.
   */
  if (!XtNameToWidget(form, "*menubar")) menubar = False;

  if (menubar && enter) {
    win[0] = XtWindow(wid);
    win[1] = XtWindow(XtNameToWidget(form, "*menubar"));
    win[2] = XtWindow(shell);
  } else if (enter) {
    win[0] = XtWindow(wid);
    win[1] = XtWindow(shell);
  } else if (menubar) {
    win[0] = XtWindow(shell);
    win[1] = XtWindow(XtNameToWidget(form, "*menubar"));
    win[2] = XtWindow(wid);
  } else {
    win[0] = XtWindow(shell);
    win[1] = XtWindow(wid);
  }

  XSetWMColormapWindows(XtDisplay(wid), XtWindow(shell), win, menubar ? 3 : 2);

  LEAVE_FUNCTION_DEBUG("Image.c: WMColormapAction");
  return;

} /* WMColormapAction() */

/*L*

________________________________________________________________

		Kill
________________________________________________________________

Name:		Kill
Syntax:		| static void Kill(Widget wid);
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/



static void Kill(Widget wid)
{
  ImagePart *ip;
  int i;
  ImageClassPart *ic = &imageClassRec.image_class;
  ip = &(((ImageWidget) wid)->image);

  ENTER_FUNCTION_DEBUG("Image.c: Kill");

  i = 0;
  ic->count--;
  while(i < ic->count && ic->widget_list[i] != wid) i++;
  ic->widget_list[i] = ic->widget_list[ic->count];  
  if (ip->xptr) free(ip->xptr);
  if (ip->yptr) free(ip->yptr); 
  XDestroyImage(ip->ximg);

  LEAVE_FUNCTION_DEBUG("Image.c: Kill");
}



/*L*

________________________________________________________________

		Resize
________________________________________________________________

Name:		Resize - Resize method for XITE Image widget.

Syntax:		| static void Resize(Widget wid);

Description:    This function should be called when the corresponding
		pixeltype resize function needs to be called. See separate
		documentation for these functions.

		Three representations exist for an image shown on the screen.
		The first, which we call the 'image', is the data structure
		which holds the actual image data. The second is a pixmap
		which holds the data ready to be displayed on the screen. The
		third is the actual screen window.

		The sizes (horizontal and vertical) of these three
		representations may all be different. If an image of size
		512x512 is shown completely inside a screen window of size
		512x512, the sizes of all three representations are equal. If
		the image is shown completely inside a screen window of size
		256x256, the pixmap and screen window are both of size
		256x256. If the image is zoomed with a factor of two and shown
		in a screen window of size 256x256, the pixmap and screen
		window are both still of size 256x256, even though only a
		portion of the pixmap is shown in the screen window. If the
		window is resized (e.g. with the help of the window manager),
		the screen window size will differ from the pixmap size for a
		little while, until the widget resize and expose methods have
		repainted the pixmap data in the screen window.

		Two private widget variables are used to control how an image
		is transferred to a pixmap (screen window) and how the pixmap
		is shown in a screen window. These two variables are 'update'
		and 'resize'.

		&resize

		This is initialized to False. The widget resize method can
		change this variable if the 'update' variable has one of the
		bits UPDATE_RESIZE, UPDATE_NEWSIZE or UPDATE_ZOOMPAN (and
		UPDATE_NONE if the screen window size is different from the
		pixmap size) set. The variable is set to False if the image
		size equals the screen window size. Otherwise, the variable is
		set to True.

		With 'resize' equal to True, the pixeltype-specific resize
		function fills each pixmap pixel with the value from an
		image-location which is determined by the zoom factor and
		panning. With 'resize' equal to False, each pixmap value is
		copied from the corresponding location in the image (the
		geometric translation tables are not used).

		The 'resize' variable does not determine whether the pixmap
		data block is freed and allocated.

		&update
		This is initialized to UPDATE_RESIZE.

		If 'update' has the UPDATE_REDISPLAY bit set, the resize
		method just calls the pixeltype- (and displaymethod-) specific
		resize function and afterwards sets the private update
		variable equal to UPDATE_RESIZE before it returns.

		If 'update' equals UPDATE_NONE and the window pixmap size
		equals the data size, the method just returns. If the sizes
		are not equal, the private 'update' variable is set equal to
		UPDATE_NEWSIZE (typically, the window is resized by the window
		manager), and the method continues.

		The remaining cases are when 'update' has the UPDATE_RESIZE,
		UPDATE_ZOOMPAN or UPDATE_NEWSIZE bit set. If the widget size
		is unchanged and the UPDATE_RESIZE bit is set, just return.
		Otherwise, update widget instance variables for panning, flow,
		zoom and geometric translation. If the widget size has
		changed, free the memory used for the pixmap so far. Finally,
		in this case, call the resize function for the particular
		pixeltype and set the private 'update' variable equal to
		UPDATE_RESIZE just prior to the return from the resize method.

		The 'update' variable may also have the UPDATE_MINMAX bit
		set, in addition to any of the ones mentioned above. In
		this case, new minimum and maximum values are calculated
		for the image, and this may change the linear scaling of
		the image on its way from the image data to the pixmap, so
		that all the pixelvalues fit in the dynamic range
		available (also refer to the XtNscalea, XtNscaleb and
		XtNscaletype resources). This bit does not need to be set
		if the extrema are changed via a call to XtVaSetValues.
		The UPDATE_MINMAX setting is only honoured when the
		XtNrange resource equals "dynamic".

		-&For more information on the pixeltype-specific resize
		functions, see 'ImageResize(3)'.

Doc:            Svein Bøe
Author:		Otto Milvang and Svein Bøe
________________________________________________________________

*/

static void update_panning(Dimension core_w, Dimension core_h, int im_w, int im_h, int *xpan, int *ypan, float magnif, update_status update)
{
  int maxx, maxy;

  ENTER_FUNCTION_DEBUG("Image.c: update_panning");
  
  /* Find maximum number of pixels to be shown in width and height
   * direction (i.e. visible inside the window with the current
   * magnification factor).
   */

  maxx = (double) core_w/magnif; /* im_w if width ratio determined magnif. */
  maxx = im_w - maxx;            
  maxy = (double) core_h/magnif;
  maxy = im_h - maxy;
  
  /* Both maxx and maxy will be zero if the width and height ratios are
   * equal (as with fixed aspect ratios).
   *
   * Ex: im_w=im_h=512, core_w=1024, core_h=512 => magnif=2,
   *     maxx=0, maxy=256.
   *
   *     im_w=im_h=256, core_w=1024, core_h=256 => magnif=4,
   *     maxx=0, maxy=256-64=192.
   */

  FPRINTF3("    maxx: %d, maxy: %d\n", maxx, maxy);

  if ((update & UPDATE_NEWSIZE) != 0) {
    *xpan = 0;
    *ypan = 0;
  }

  if (maxx > 0 && *xpan > maxx) *xpan = maxx;
  if (maxy > 0 && *ypan > maxy) *ypan = maxy;

  FPRINTF5("    maxx: %d, maxy: %d, xpan: %d, ypan: %d\n", maxx, maxy,
	   *xpan, *ypan);
  
  LEAVE_FUNCTION_DEBUG("Image.c: update_panning");
  return;

} /* update_panning() */

static void find_flow(int *xflow, int *yflow, Dimension core_w, Dimension core_h, int im_w, int im_h, int xpan, int ypan, float magnif)
{

  ENTER_FUNCTION_DEBUG("Image.c: find_flow");

  *xflow = (double) core_w - magnif * (im_w - xpan);
  *yflow = (double) core_h - magnif * (im_h - ypan);
  
  /* Ex: im_w=im_h=512, core_w=1024, core_h=512 => magnif=2,
   *     xpan=ypan=0, xflow=0, yflow=512-1024= -512.
   
   *     im_w=im_h=256, core_w=1024, core_h=256 => magnif=4,
   *     xpan=ypan=0, xflow=0, yflow=256-1024= -768.
   */

  FPRINTF3("    xflow: %d, yflow: %d\n", *xflow, *yflow);

  if (xpan == 0 || *xflow < 0 || *xflow > magnif) *xflow = 0;
  if (ypan == 0 || *yflow < 0 || *yflow > magnif) *yflow = 0;

  FPRINTF3("    xflow: %d, yflow: %d\n", *xflow, *yflow);

  LEAVE_FUNCTION_DEBUG("Image.c: find_flow");
  return;

} /* find_flow() */

static void update_magnifs(Dimension core_w, Dimension core_h, update_status update, Dimension ximg_w, Dimension ximg_h, int im_w, int im_h, float *oldMagnif, float *zoom)
{
  float magnif;

  ENTER_FUNCTION_DEBUG("Image.c: update_magnifs");

  if ((update & (UPDATE_RESIZE | UPDATE_ZOOMPAN)) != 0)
    /* ximg_w and ximg_h are equal to previous pixmap-size.
     * '*oldMagnif' will become zero the first time. */
    *oldMagnif = ((float) ximg_w/im_w > (float) ximg_h/im_h) ? 
      (float) ximg_w/im_w : (float) ximg_h/im_h;
  else *oldMagnif = 0.0;
  
  /* Ratio of window size to actual BIFF image size (magnification
   * factor) (choose the larger of width and height ratios).
   */
  magnif = ((float) core_w/im_w > (float) core_h/im_h) ? 
    (float) core_w/im_w : (float) core_h/im_h;
  
  if (*oldMagnif > 0.0) magnif = (*zoom) * magnif/(*oldMagnif);

  *zoom = magnif;

  FPRINTF3("    oldMagnif: %f, magnif: %f\n", *oldMagnif, magnif);

  LEAVE_FUNCTION_DEBUG("Image.c: update_magnifs");
  return;

} /* update_magnifs() */

static void make_trans_table(Widget wid, int **xarr, int **yarr, Dimension core_w, Dimension core_h, int im_w, int im_h, int xflow, int yflow, int xpan, int ypan, float magnif)
{
  int x, y, *xptr, *yptr;

  /* New transformation vectors for resizing are needed.
   * Vector lengths equals width or height of window.
   * Window pixel index is used to lookup BIFF image pixel.
   */
  
  ENTER_FUNCTION_DEBUG("Image.c: make_trans_table");

  if (*xarr) free(*xarr);
  if (*yarr) free(*yarr); 
  *xarr = (int *) malloc(core_w * sizeof(int));
  *yarr = (int *) malloc(core_h * sizeof(int));

  xptr = *xarr;
  yptr = *yarr;

  if (xptr == NULL || yptr == NULL) ImageError(wid, ERR_MEM);
  
  /* (x - xflow)/magnif + xpan =
     (x - core_w + magnif*(im_w - xpan))/magnif + xpan =
     (x - core_w)/magnif + im_w - xpan + xpan, approx. equal to
     (x - core_w)/core_w*im_w + im_w =
     x/core_w*im_w - im_w + im_w =
     x * im_w/core_w = x/core_w * im_w. */
  
  for(x=0; x < core_w; x++) {
    xptr[x] = (double) (x-xflow) / magnif + xpan;
    if (xptr[x] < 0) xptr[x] = 0;
    if (xptr[x] >= im_w) break;
  }
  
  for(y=0; y < core_h; y++) {
    yptr[y] = (double) (y-yflow) / magnif + ypan;
    if (yptr[y] < 0) yptr[y] = 0;
    if (yptr[y] >= im_h) break;
  }
  
  LEAVE_FUNCTION_DEBUG("Image.c: make_trans_table");
  return;

} /* make_trans_table() */

void ImageUpdateMinMax(Widget wid)
{
  ImageWidget iwid = (ImageWidget) wid;
  ImagePart *ip;

  ENTER_FUNCTION_DEBUG("Image.c: make_trans_table");

  ip = &(iwid->image);

  if (!ip->image_rgb) {
    band_minmax_disp((IBAND) ip->band, &(ip->minval), &(ip->maxval));
  } else {
    float minval, maxval;
    
    band_minmax_disp((IBAND) ip->red_band, &(ip->minval),
		     &(ip->maxval));
    
    band_minmax_disp((IBAND) ip->green_band, &minval, &maxval);
    ip->minval = MIN(ip->minval, minval);
    ip->maxval = MAX(ip->maxval, maxval);
    
    band_minmax_disp((IBAND) ip->blue_band, &minval, &maxval);
    ip->minval = MIN(ip->minval, minval);
    ip->maxval = MAX(ip->maxval, maxval);
  }

  LEAVE_FUNCTION_DEBUG("Image.c: make_trans_table");

  return;

} /* ImageUpdateMinMax() */

static void Resize(Widget wid)
{
  ImageWidget iwid = (ImageWidget) wid;
  ImagePart *ip;
  Widget shellW;
  float oldMagnif = 0.0;
  int xflow, yflow, im_w, im_h, update;
  Dimension core_h, core_w, ximg_h, ximg_w;

  ENTER_FUNCTION_DEBUG("Image.c: Resize");

  ip     = &(iwid->image);

  FPRINTF2("    reason %d\n", ip->update);

  /* If 'wid' is an Image widget, then the core width and height are non-zero
   * because of the call to XtResizeWidget in the initialize method. If 'wid'
   * is an ImageOverlay widget (possible because this resize method is called
   * by the resize method in the ImageOverlay source), then the core width and
   * height have been set by the initialize method in the ImageOverlay source.
   */
  core_w = iwid->core.width;  /* Width of image widget. */
  core_h = iwid->core.height; /* Height of image widget. */
  im_w   = ip->width;         /* Width of actual BIFF image. */
  im_h   = ip->height;        /* Height of actual BIFF image. */
  ximg_w = (Dimension) XimageWidth; /* Width in pixels of window pixmap */
  ximg_h = (Dimension) XimageHeight;/* (not considering zoom/pan), before this
			       * call to Resize. Will equal present or previous
			       * core values. Will be zero the first time this
			       * is called. */

  if (ip->update == UPDATE_NONE) {
    if (core_w == ximg_w && core_h == ximg_h) {
      ip->update = UPDATE_RESIZE;
      LEAVE_FUNCTION_DEBUG("Image.c: Resize");
      return;
    } else {
      /* Resized by window-manager. */
      ip->update = UPDATE_NEWSIZE;
    }
  }

  FPRINTF3("    core_w: %d, core_h: %d", core_w, core_h);
  FPRINTF2(", factor: %.2f", ip->factor);
  FPRINTF2(", zoom: %.2f\n", ip->zoom);
  FPRINTF3("    ximg_w: %d, ximg_h: %d", ximg_w, ximg_h);
  FPRINTF3(", im_w: %d, im_h: %d\n",  im_w, im_h);
  FPRINTF3("    internal_width: %d, internal_height: %d", ip->internal_width,
	   ip->internal_height);
  FPRINTF3(", xstart: %d, ystart: %d\n", ip->xstart, ip->ystart);
  shellW = TopLevelWidget(wid);
  FPRINTF3("    shell_w: %d, shell_h: %d\n",
	   shellW->core.width, shellW->core.height);

  /* Update = UPDATE_RESIZE    -> Resize (windowmanager or initial mapping). */
  /* Update = UPDATE_REDISPLAY -> Redisplay area. */
  /* Update = UPDATE_ZOOMPAN   -> Zoom/pan values changed. */
  /* Update = UPDATE_NEWSIZE   -> New image width / image height set by the
   *                              application. */
  /* Update = UPDATE_MINMAX    -> Recalculate minval and maxval to get
   *                              correct pixel scaling. */

  /* Make sure ip->update only acts on the "strongest" bits set (i.e. those
   * that require the most of processing), to avoid multiple calls to the
   * pixeltype resize methods.
   */

  update = ip->update;
  if      ((ip->update & UPDATE_NEWSIZE)   != 0) ip->update = UPDATE_NEWSIZE;
  else if ((ip->update & UPDATE_ZOOMPAN)   != 0) ip->update = UPDATE_ZOOMPAN;
  else if ((ip->update & UPDATE_RESIZE)    != 0) ip->update = UPDATE_RESIZE;
  else if ((ip->update & UPDATE_REDISPLAY) != 0) ip->update = UPDATE_REDISPLAY;

  /* Note: checking on unchanged 'update' in the following test */
  if ((update & UPDATE_MINMAX) != 0) {
    if (strcmp(ip->range, "dynamic") == 0) {
      if (update == UPDATE_MINMAX)
	ip->update = UPDATE_MINMAX | UPDATE_REDISPLAY;
      else
	ip->update |= UPDATE_MINMAX;
    }
  }

  if ((ip->update & UPDATE_REDISPLAY) != 0) {

    if (ip->pixtype && (ip->pixtype)->resize) {
      if ((ip->update & UPDATE_MINMAX) != 0 && ip->band != NULL) {
	ImageUpdateMinMax(wid);
      }

      (ip->pixtype)->
	resize(iwid, ip->updatex, ip->updatey, ip->updatew, ip->updateh);
    }

    ip->update = UPDATE_RESIZE;

    LEAVE_FUNCTION_DEBUG("Image.c: Resize");
    return;
  } 

  if ((ip->update & (~UPDATE_REDISPLAY)) != 0) {
    Boolean free_and_alloc = False;

    /* One of the bits UPDATE_NEWSIZE, UPDATE_ZOOMPAN or
     * UPDATE_RESIZE set, possibly also the UPDATE_MINMAX bit. */

    if (ximg_w == core_w && ximg_h == core_h &&
	(ip->update & UPDATE_RESIZE) != 0) {
      /* No actual resizing. Pixmap is same size as widget.
       * Variables may be zero the first time.
       */

      /*
       * Do recalculate if told that min/max have changed.
       */

      if ((ip->update & UPDATE_MINMAX) != 0 && ip->band != NULL
	  && ximg_w != 0 && ximg_h != 0) {
	if (ip->pixtype && (ip->pixtype)->resize) {
	  ImageUpdateMinMax(wid);
	  (ip->pixtype)->resize(iwid, 0, 0, ximg_w, ximg_h);
	}
      }

      ip->update = UPDATE_RESIZE;

      LEAVE_FUNCTION_DEBUG("Image.c: Resize");
      return;
    }

    /* One of the bits UPDATE_NEWSIZE, UPDATE_ZOOMPAN or
     * UPDATE_RESIZE still set, possibly also the UPDATE_MINMAX bit. */

    update_magnifs(core_w, core_h, ip->update, ximg_w, ximg_h, im_w, im_h,
		   &oldMagnif, &(ip->zoom));

    update_panning(core_w, core_h, ip->width, ip->height,
		   &ip->xpan, &ip->ypan, ip->zoom, ip->update);

    find_flow(&xflow, &yflow, core_w, core_h, im_w, im_h,
	      ip->xpan, ip->ypan, ip->zoom);

    make_trans_table(wid, &(ip->xptr), &(ip->yptr), core_w, core_h, im_w, im_h,
		     xflow, yflow, ip->xpan, ip->ypan, ip->zoom);

    if (core_w == im_w && core_h == im_h && ip->zoom == 1.0)
      /* One-to-one mapping between window pixmap size and BIFF image size.
       * Don't use geometric translation tables. */
      ip->resize = FALSE;
    else
      ip->resize = TRUE;
    
    if (ximg_w != core_w || ximg_h != core_h) free_and_alloc = True;
      
    (ip->ximg) -> width  = ximg_w = core_w;
    (ip->ximg) -> height = ximg_h = core_h;

    FPRINTF3("    Set ximage size to %d %d\n", ximg_w, ximg_h);
    ip -> factor = (double)
      ((ip->xptr)[core_w - 1] - (ip->xptr)[0] + 1) *
	((ip->yptr)[core_h - 1] - (ip->yptr)[0] + 1) / (im_w * im_h);

    if (ip->internal_width != ximg_w || ip->internal_height != ximg_h) {
      int widNum;
      ImageWidget child;
      Dimension child_w = 0, child_h = 0;
      Position child_x = 0, child_y = 0;
      float w_fact = 1.0, h_fact = 1.0;

      /*	  FPRINTF3("    Resize request to %d %d\n", ximg_w, ximg_h);
		  stat = XtMakeResizeRequest(wid, ximg_w, ximg_h, &rw, &rh); 
		  FPRINTF3("    Resize requested %d %d\n", rw, rh); */

      /* Changed size of pixmap. */
      ip->internal_width  = (int) ximg_w;
      ip->internal_height = (int) ximg_h;

      FPRINTF5("    orig_width=%d, orig_height=%d, orig_x=%d, orig_y=%d\n",
	       ip->orig_width, ip->orig_height, ip->orig_x, ip->orig_y);
      if (ip->orig_width > 0)
	w_fact = (float) ximg_w / ip->orig_width;

      if (ip->orig_height > 0)
	h_fact = (float) ximg_h / ip->orig_height;

      for(widNum=0; widNum < iwid->composite.num_children; widNum++) {
	child = (ImageWidget) (iwid->composite.children[widNum]);

	FPRINTF4("    child num %d: width=%d, height=%d\n", widNum,
		 child->core.width,
		 child->core.height);
	FPRINTF4("    child num %d: x=%d, y=%d\n", widNum,
		 child->core.x,
		 child->core.y);

	/* child_x = widNum * (1.0 * iwid->core.width); */
	/* child_y = 0; */
	/* child_w = (1.0 * iwid->core.width)/iwid->composite.num_children; */
	/* child_h = iwid->core.height; */
	child_x = child->image.orig_x * w_fact;
	child_y = child->image.orig_y * h_fact;
	child_w = child->image.orig_width * w_fact;
	child_h = child->image.orig_height * h_fact;

	FPRINTF6("    child num %d: new_x=%d, new_y=%d, new_w=%d, new_h=%d\n",
		widNum, child_x, child_y, child_w, child_h);
	XtResizeWidget((Widget) child, child_w, child_h, 0);
	XtMoveWidget((Widget) child, child_x, child_y);
      }

    } /* XtResizeWidget(iwid->core.parent, w, h, 1); */

    if (free_and_alloc && (ip->ximg)->data != NULL) {
      /* New pixmap size. */

      free((ip->ximg)->data);
      (ip->ximg)->data = NULL;
    }

    if (ip->pixtype && (ip->pixtype)->resize) {
      if ((ip->update & UPDATE_MINMAX) != 0 && ip->band != NULL) {
	ImageUpdateMinMax(wid);
      }

      (ip->pixtype)->resize(iwid, 0, 0, ximg_w, ximg_h);
    }

  } /* update != UPDATE_REDISPLAY and still something to do. */

  ip->update = UPDATE_RESIZE;

  LEAVE_FUNCTION_DEBUG("Image.c: Resize");

} /* Resize() */





/*L:PrintValues*

________________________________________________________________

		PrintValues
________________________________________________________________

Name:		PrintValues
Syntax:		| static void PrintValues(ImageWidget wid,
                |    int x, int y, int width, int height)
Description:    If pixelsize on-screen is large enough to contain ascii
                printouts of pixel-values, then print values for each
		pixel in the rectangel given by corner ('x','y'),
		'width' and 'height'.
Author:		Otto Milvang
Revised:        Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/

static int find_font(ImagePart *ip, int *fontn, int *fontw, int *fonth, int *fonta)
{
  /* Search for a font narrow enough for the ascii numbers to fit inside
   * a pixel at the current zooming (zooming says how many screen-pixels one
   * image-pixel occupies). Start with the widest font (font[0] is not
   * sorted relative to the others).
   */
  for (*fontn = 3; *fontn >= 0; (*fontn)--) {
    *fontw = (ip->font[*fontn])->max_bounds.width;
    *fonta = (ip->font[*fontn])->ascent;
    *fonth = (*fonta) + (ip->font[*fontn])->descent;
    if (ip->zoom >= ((0.2 + (ip->pixtype)->txtlen) * (*fontw))) break;
  }
  if ((*fontn) <= 0) {
    return(1);
  }

  return(0);

} /* find_font() */

static void find_screen_coord(ImageWidget wid, int x, int y, int width, int height, int **xs, int **ys, int *n_xs, int *n_ys)
{
  int x1, x2, y1, y2;
  int xw, xh, cx, cy;
  int *xarr, *yarr, *xs2, *ys2;
  ImagePart *ip = &(wid->image);

  /* Auxiliary variables. */
  x1 = x;  x2 = x + width  - 1;
  y1 = y;  y2 = y + height - 1;
  xw = ip->internal_width; xh = ip->internal_height;

  /* Check auxiliary variables. */
  if (x2 >= xw) x2 = xw - 1;
  if (y2 >= xh) y2 = xh - 1;

  /* Transformation vectors from screen-pixel coordinates to image-pixel
   * coordinates.
   */
  xarr = ip->xptr;
  yarr = ip->yptr;

  /* Depending on zooming, one image-pixel may map to several screen-pixels.
   * Find the largest screen-rectangle which maps to the same image-rectangle
   * as (x1, x2, y1, y2).
   */
  while (x1 >  0   && xarr[x1] == xarr[x1-1]) x1--; 
  while (y1 >  0   && yarr[y1] == yarr[y1-1]) y1--; 
  while (x2 < xw-1 && xarr[x2] == xarr[x2+1]) x2++; 
  while (y2 < xh-1 && yarr[y2] == yarr[y2+1]) y2++;
 
  /* Allocate vectors which will store only the screen-pixel coordinates
   * which map to distinct image-pixel coordinates.
   */
  xs2 = (int *) malloc((xarr[x2] - xarr[x1] + 2) * sizeof(int));
  ys2 = (int *) malloc((yarr[y2] - yarr[y1] + 2) * sizeof(int));
  if (xs2 == NULL || ys2 == NULL) ImageError((Widget) wid, ERR_MEM);

  /* Store these screen-pixel coordinates. */
  cx = 0;
  xs2[cx++] = x1;
  for (x = x1 + 1; x <= x2; x++)
    if (xarr[x] != xarr[x-1]) xs2[cx++] = x;
  xs2[cx] = x2+1;

  cy = 0;
  ys2[cy++] = y1;
  for (y = y1 + 1; y <= y2; y++)
    if (yarr[y] != yarr[y-1]) ys2[cy++] = y;
  ys2[cy] = y2+1;

  *xs   = xs2;
  *ys   = ys2;
  *n_xs = cx;
  *n_ys = cy;

  return;

} /* find_screen_coord() */
  
static void find_fg_bg(Display *dpy, ImagePart *ip, int x, int y, unsigned long *foreg, unsigned long *backg)
{
  unsigned long fg = 0, bg = 0;
  int screen;

  screen = DefaultScreen(dpy);

  if (ip->ximg && ip->ximg->data) bg = XGetPixel(ip->ximg, x, y);

  switch(ip->displaymethod)
    {
    case STATIC_GRAY_1:
      bg = bg ? BlackPixel(dpy, screen) : WhitePixel(dpy, screen);
      fg = bg ? WhitePixel(dpy, screen) : BlackPixel(dpy, screen);
      break;
    case TRUE_COLOR_24:
    case DIRECT_COLOR_24_A:
    case DIRECT_COLOR_24_B: {
      int bgr, bgg, bgb, fgr, fgg, fgb;
      
      if (!ip->ximg || !ip->ximg->data) bg = ip->colorptr[0];
      
      bgr = RedBasePart(bg)   - ip->colorptr[0];
      bgg = GreenBasePart(bg) - ip->colorptr[0];
      bgb = BlueBasePart(bg)  - ip->colorptr[0];
      
      fgr = (bgr < ip->colorcells/2) ? (bgr + ip->colorcells/2) :
	(bgr - ip->colorcells/2);
      fgg = (bgg < ip->colorcells/2) ? (bgg + ip->colorcells/2) :
	(bgg - ip->colorcells/2);
      fgb = (bgb < ip->colorcells/2) ? (bgb + ip->colorcells/2) :
	(bgb - ip->colorcells/2);
      
      fgr += ip->colorptr[0];
      fgg += ip->colorptr[0];
      fgb += ip->colorptr[0];
      
      fg = ShiftToRed(fgr) | ShiftToGreen(fgg) | ShiftToBlue(fgb);
    }
    break;
    default:
      if (!ip->ximg || !ip->ximg->data) bg = ip->colorptr[0];
      
      bg = bg - ip->colorptr[0];
      fg = (bg < ip->colorcells/2) ? (bg + ip->colorcells/2) :
	(bg - ip->colorcells/2);
      fg += ip->colorptr[0];
      bg += ip->colorptr[0];
      break;
      /*
    case STATIC_GRAY_4:
      fg = bg < 8 ? bg + 8 : bg - 8; 
      break;
    case PSEUDO_COLOR_8_A: 
      fg = bg < color ? bg + 64 : bg - 64; 
      break;
    case STATIC_GRAY_8:
    case PSEUDO_COLOR_8_B: 
      fg = bg < 128 ? bg + 128 : bg - 128;
      break;
      */      
    } /* switch(ip->displaymethod) */

  *foreg = fg;
  *backg = bg;

} /* find_fg_bg() */

static int PrintValues(ImageWidget wid, int x, int y, int width, int height)
{
  Display *dpy;
  char *val;
  int *xs, *ys, n_xs, n_ys, xb, xl, yb, yl;
  int fontn, fontw, fonth, fonta;
  unsigned long bg, fg;
  ImagePart *ip;

  ENTER_FUNCTION_DEBUG("Image.c: PrintValues");

  ip = &(wid->image);

  if (find_font(ip, &fontn, &fontw, &fonth, &fonta)) {
    LEAVE_FUNCTION_DEBUG("Image.c: PrintValues");
    return(1);
  }

  dpy = XtDisplay(wid);

  FPRINTF2(" VFont:%d\n", fontn);
  XSetFont(dpy, ip->gc, (ip->font[fontn])->fid);

  FPRINTF5("    %d %d %d %d", x, y, width, height);

  find_screen_coord(wid, x, y, width, height, &xs, &ys, &n_xs, &n_ys);

  val = (char *) malloc((size_t) (ip->pixtype)->txtlen+1);

  /* For each screen pixel which maps to unique image pixels. */
  for(y = 0; y < n_ys; y++)
    for(x = 0; x < n_xs; x++) {            
      xb = xs[x];             /* Screen x-coordinate. */
      xl = xs[x+1] - xb;      /* Number of screen x-coordinates with same
			       * mapping (length of same mapping). */
      yb = ys[y];
      yl = ys[y+1] - yb;
      
      if (xl > fontw*2 && yl > fonth*2/3) {

	if ((ip->pixtype)->printf)
	  (ip->pixtype)->printf(val, ImageScreenXYtoAdr(wid, xb, yb, 0));

	find_fg_bg(dpy, ip, xb, yb, &fg, &bg);
	XSetState (dpy, ip->gc, fg, bg, GXcopy, AllPlanes); 
	XDrawString(dpy, XtWindow(wid), ip->gc, 
		    xb + (xl - strlen(val)*fontw)/2,
		    yb + (yl - fonth)/2 + fonta, val, strlen(val));
      } /* if */
    } /* for x */

  free(xs);
  free(ys);
  free(val);

  LEAVE_FUNCTION_DEBUG("Image.c: PrintValues");

  return(0);
}



static void PrintUnknown(ImageWidget wid)
{
  Display *dpy;
  int screen;
  int  fontn, fontw, fonth, fonta;
  unsigned long bg, fg;
  ImagePart *ip;
  char txt[100];
  Boolean compl = False;

  ENTER_FUNCTION_DEBUG("Image.c: PrintUnknown");

  ip = &(wid->image);
  fontn = 2;
  fontw = (ip->font[fontn])->max_bounds.width;
  fonta = (ip->font[fontn])->ascent;
  fonth = fonta + (ip->font[fontn])->descent;

  dpy = XtDisplay(wid);
  screen = DefaultScreen(dpy);
  FPRINTF2("    VFont:%d\n", fontn);
  XSetFont(dpy, ip->gc, (ip->font[fontn])->fid);

  if (ip->pixname) {
    if (!strcmp(ip->pixname, ImageComplexPixel) ||
	!strcmp(ip->pixname, ImageDoubleComplexPixel))
      compl = True;
    
    sprintf(txt, "Can't display complex-valued image.");
  }
  else
    sprintf(txt," Can't display this pixeltype. ");

  FPRINTF2("    Print text %s\n", txt);

  find_fg_bg(dpy, ip, wid->core.width/2, wid->core.height/2, &fg, &bg);
  XSetState (dpy, ip->gc, fg, bg, GXcopy, AllPlanes);
  XDrawString(dpy, XtWindow(wid), ip->gc, 
	      (wid->core.width  - strlen(txt)*fontw)/2,
	      (wid->core.height - fonth)/2 + fonta, txt, strlen(txt));

  if (compl) {
    (void) strcpy(txt, "To view, find real/imaginary part or");
    XDrawString(dpy, XtWindow(wid), ip->gc, 
		(wid->core.width  - strlen(txt)*fontw)/2,
		(wid->core.height - fonth)/2 + fonta + fonth,
		txt, strlen(txt));
    (void) strcpy(txt, "magnitude/phase.");
    XDrawString(dpy, XtWindow(wid), ip->gc, 
		(wid->core.width  - strlen(txt)*fontw)/2,
		(wid->core.height - fonth)/2 + fonta + 2*fonth,
		txt, strlen(txt));
    (void) strcpy(txt, "Zoom in to display numerical values.");
    XDrawString(dpy, XtWindow(wid), ip->gc, 
		(wid->core.width  - strlen(txt)*fontw)/2,
		(wid->core.height - fonth)/2 + fonta + 3*fonth,
		txt, strlen(txt));
  }

  LEAVE_FUNCTION_DEBUG("Image.c: PrintUnknown");
}



static int IfExpose(Display *display, XEvent *event, EventRectangle *arg)
{
  /* Return in 'arg' the rectangle which includes those defined by 'event'
     and 'arg'. */

  XExposeEvent *xevent;
  short start, stop;

  ENTER_FUNCTION_DEBUG("Image.c: IfExpose");

  if (event->type != Expose) {
    LEAVE_FUNCTION_DEBUG("Image.c: IfExpose");
    return(0);
  }
  xevent = &event->xexpose;
  if (xevent->window != arg->window) {
    LEAVE_FUNCTION_DEBUG("Image.c: IfExpose");
    return(0);
  }

  /* Minimum horizontal coordinate. */
  start = arg->x < xevent->x ? arg->x : xevent->x;

  /* Maximum horizontal coordinate. */
  stop  = (arg->x + arg->width) > (xevent->x + xevent->width) ? 
          (arg->x + arg->width) : (xevent->x + xevent->width);

  /* Returning horizontal parameters. */
  arg->x = start;
  arg->width = stop - start;

  start = arg->y < xevent->y ? arg->y : xevent->y;
  stop  = (arg->y + arg->height) > (xevent->y + xevent->height) ? 
          (arg->y + arg->height) : (xevent->y + xevent->height);
  arg->y = start;
  arg->height = stop - start;

  LEAVE_FUNCTION_DEBUG("Image.c: IfExpose");
  return(1);
}  


static void Redisplay(Widget wid, XEvent *event, Region region)
{
  XEvent hevent;
  EventRectangle rect;
  ImageWidget iwid = (ImageWidget) wid;
  ImagePart *ip;
  int x, y, w, h, internal_w, internal_h, core_w, core_h, screen;
  Display *dpy;
  Window win;

  ENTER_FUNCTION_DEBUG("Image.c: Redisplay");

  if (!XtIsRealized(wid)) {
    LEAVE_FUNCTION_DEBUG("Image.c: Redisplay");
    return;
  }

  dpy        = XtDisplay(wid);
  screen     = DefaultScreen(dpy);
  win        = XtWindow(wid);
  ip         = &(iwid->image);
  core_w     = ((ImageWidget) wid)->core.width;
  core_h     = ((ImageWidget) wid)->core.height;
  internal_w = ip->internal_width;
  internal_h = ip->internal_height;

  /* Find rectangle which is necessary to redraw. */
  if (region)
    XClipBox(region, (XRectangle *) &rect);
  else {
    rect.x      = 0;
    rect.y      = 0;
    rect.width  = ((ImageWidget) wid)->core.width;
    rect.height = ((ImageWidget) wid)->core.height;
  }
  rect.window = win;

  /* Check the event queue with the predicate procedure 'IfExpose', as long as
   * the event is an expose-event. Pass 'rect' to the function 'IfExpose'.
   * Return in 'rect' the rectangle which includes those defined by 'event'
   * and 'rect'. */

  while (XCheckIfEvent(dpy, &hevent, (Bool (*)(Display*, XEvent*, XPointer))
		       IfExpose, (XPointer) &rect));

  x = rect.x;
  y = rect.y;
  w = (x + rect.width  > internal_w) ? internal_w - x : rect.width;
  h = (y + rect.height > internal_h) ? internal_h - y : rect.height;
 
  FPRINTF5("    x = %d %d, y = %d %d\n", x, x+w-1, y, y+h-1);
 
  if (ip->pixtype) {
    int status = 0;

    if (w > 0 && h > 0) {
      XSetState (dpy, ip->gc, 
		 /*ip->foreground */ WhitePixel(dpy,screen), 
		 /*ip->background */ BlackPixel(dpy,screen), 
		 GXcopy, AllPlanes); 
      
      if ((ip->pixtype)->resize)
	XPutImage (dpy, win, ip->gc, ip->ximg, x, y, x, y, w, h);

      if (ip->print_values)
	status = PrintValues((ImageWidget) wid, x, y, w, h);
    }

    if (status && ip->pixname &&
	(!strcmp(ip->pixname, ImageComplexPixel) ||
	 !strcmp(ip->pixname, ImageDoubleComplexPixel))) {
      /* Did not print numerica pixel values, and image is complex. */
      PrintUnknown((ImageWidget) wid);
    }
    
    FPRINTF3("    rect.width: %d, w: %d", rect.width, w);
    FPRINTF3(", rect.height: %d, h: %d\n", rect.height, h);
    FPRINTF3("    internal_w: %d, core_w: %d", internal_w, core_w);
    FPRINTF3(" core_h: %d internal_h: %d\n", core_h, internal_h);
    
    /* If necessary, paint with the window's background pixel or pixmap.
     * Do not generate expose events. */
    if (rect.width-w > 0)
      XClearArea(dpy, win, internal_w, 0, core_w-internal_w, core_h, 0);
    if (rect.height-h > 0)
      XClearArea(dpy, win, 0, internal_h, core_w, core_h-internal_h, 0);
  } else PrintUnknown((ImageWidget) wid);
  
  Button(wid, event, NULL, 0);
  
  LEAVE_FUNCTION_DEBUG("Image.c: Redisplay");

} /* Redisplay() */  
  


/*L*

________________________________________________________________

		SetValues
________________________________________________________________

Name:		SetValues
Syntax:         | static Boolean SetValues(Widget current,
                |    Widget request, Widget new, ArgList args,
                |    Cardinal *num_args);
Description:    This method is downward chained.
Return value:
Author:		Otto Milvang and Svein Bøe
________________________________________________________________

*/

static void check_line_length_arg(ImagePart *ip, ArgList args, Cardinal *num_args, Boolean *do_redisplay, update_status *update)
{
  int i, w = 0, l = 0;

  /* For each argument in the list: */
  for (i=0; i < *num_args; i++) {
    if (strcmp(args[i].name, XtNimageWidth)) {
      /* Found argument different from XtNimageWidth. */
      w = i+1;
    }
    if (strcmp(args[i].name, XtNimageLineLength)) {
      /* Found argument different from XtNimageLineLength. */
      l = 1;
    }
  }

  if (w > 0 && l == 0 && (ip->pixtype != NULL)) {
    /* Found argument different from XtNimageWidth, but not different
     * from XtNimageLineLength. That is, found only one argument, which
     * is XtNimageLineLength.
     */
    ip->line_length = ip->width * (ip->pixtype)->pixelsize;
    *do_redisplay = 1;
    *update |= UPDATE_NEWSIZE;
  }

  return;

} /* check_line_length_arg() */

static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
  ImageWidget in = (ImageWidget) new;
  int screen;
  Boolean do_redisplay = False;
  update_status update;
  Display *dpy;
  ImagePart *cPart = &(((ImageWidget) current)->image);
  ImagePart *nPart = &(((ImageWidget) new)->image);

  ENTER_FUNCTION_DEBUG("Image.c: SetValues");

  dpy    = XtDisplay(new);
  screen = DefaultScreen(dpy);
  update = UPDATE_NONE;

  /* UPDATE_REDISPLAY is weaker than UPDATE_ZOOMPAN,
   * UPDATE_RESIZE and UPDATE_NEWSIZE. */     

  if (cPart->visual != nPart->visual)
    XtErrorMsg("Image", "Initialize", "Image",
    "Can't change Visual", NULL,0);

  FPRINTF2("    nPart->minval: %10.2f", nPart->minval);
  FPRINTF2(" , nPart->maxval: %10.2f\n", nPart->maxval);

  if (cPart->band       != nPart->band       ||
      cPart->red_band   != nPart->red_band   ||
      cPart->green_band != nPart->green_band ||
      cPart->blue_band  != nPart->blue_band) {

    Boolean assume_pseudo = False, assume_rgb = False;

    do_redisplay = 1;         /* Redraw, i.e. expose. */
    update |= UPDATE_NEWSIZE; /* "Full" resizing, everything must be
			       * recalculated. */

    /* One or more of the bands have changed. If only 'band' changed, then
     * also update the primaries data (but not their bands) because the image
     * is then most likely pseudocolor, not rgb. If it were rgb, then one of
     * the primary bands should have changed instead of (or in addition to)
     * 'band'. It is necessary to update the primaries data in case the image
     * is displayed with a composite visual class. */

    if (cPart->band       != nPart->band)      assume_pseudo = True;
    if (cPart->red_band   != nPart->red_band   ||
	cPart->green_band != nPart->green_band ||
	cPart->blue_band  != nPart->blue_band) assume_rgb = True;

    nPart->data = ImageAddress((IBAND) nPart->band);

    if (assume_pseudo && !assume_rgb) {
      nPart->red_data   = nPart->data;
      nPart->green_data = nPart->data;
      nPart->blue_data  = nPart->data;
    } else if (assume_rgb) {
      if (!nPart->red_band) nPart->red_data = NULL; else
      if (cPart->red_band   != nPart->red_band   && nPart->red_band)
	nPart->red_data      = ImageAddress((IBAND) nPart->red_band);

      if (!nPart->green_band) nPart->green_data = NULL; else
      if (cPart->green_band != nPart->green_band && nPart->green_band)
	nPart->green_data    = ImageAddress((IBAND) nPart->green_band);

      if (!nPart->blue_band) nPart->blue_data = NULL; else
      if (cPart->blue_band  != nPart->blue_band  && nPart->blue_band)
	nPart->blue_data     = ImageAddress((IBAND) nPart->blue_band);
    }

    if (nPart->red_band && nPart->green_band && nPart->blue_band &&
	!(nPart->red_band   == nPart->green_band &&
	  nPart->green_band == nPart->blue_band))
      nPart->image_rgb = True;
    else nPart->image_rgb = False;

    nPart->pixname     = ImagePixName((IBAND)    nPart->band);
    nPart->width       = ImageWidth((IBAND)      nPart->band);
    nPart->height      = ImageHeight((IBAND)     nPart->band);
    nPart->line_length = ImageLineLength((IBAND) nPart->band);
    nPart->xstart      = ImageXstart((IBAND)     nPart->band);
    nPart->ystart      = ImageYstart((IBAND)     nPart->band);
  }

  /* Use the green data if data is empty. */
  if (nPart->data == NULL && nPart->green_data != NULL)
    nPart->data = nPart->green_data;

  /* Let any missing primary get its data from 'nPart->data'. This will
     create equal primaries for a one-band pseudocolor image. */
  if (!nPart->red_data)   nPart->red_data   = nPart->data;
  if (!nPart->green_data) nPart->green_data = nPart->data;
  if (!nPart->blue_data)  nPart->blue_data  = nPart->data;

  (void) findPixelType(nPart, &imageClassRec.image_class);

  if (nPart->width  != cPart->width ||
      nPart->height != cPart->height) {
    /* No need to set do_redisplay, because the resize method will
     * be called by Xt when widget size is changed, and afterwards
     * Xt calls the expose method.
     */

    /* Actually image.width is not widget size (core.width), but it seems
     * to work anyhow (not setting do_redisplay). */
    update |= UPDATE_NEWSIZE;
  }

  if (nPart->variable_aspect != cPart->variable_aspect) {
    /* No need to resize or expose. */
    nPart->x_aspect = in->core.width;
    nPart->y_aspect = in->core.height;
  }

  if (nPart->scaletype     != cPart->scaletype     ||
      nPart->scalea        != cPart->scalea        ||
      nPart->scaleb        != cPart->scaleb        ||
      nPart->minval        != cPart->minval        ||
      nPart->maxval        != cPart->maxval) {
    do_redisplay = 1;
    update |= UPDATE_REDISPLAY;
    nPart->updatex = 0;
    nPart->updatey = 0;
    nPart->updatew = nPart->ximg->width;
    nPart->updateh = nPart->ximg->height;
  }

  if (nPart->colorcells    != cPart->colorcells    ||
      nPart->colorptr      != cPart->colorptr      ||
      nPart->displaymethod != cPart->displaymethod) {
    do_redisplay = 1;
    update |= UPDATE_REDISPLAY;
    nPart->updatex = 0;
    nPart->updatey = 0;
    nPart->updatew = nPart->ximg->width;
    nPart->updateh = nPart->ximg->height;
    check_color_resources(current, new);
  } else if (nPart->colortab != cPart->colortab) {
    check_color_resources(current, new);
  }

  if (nPart->font[0]       != cPart->font[0]       ||
      nPart->font[1]       != cPart->font[1]       ||
      nPart->font[2]       != cPart->font[2]       ||
      nPart->font[3]       != cPart->font[3]       ||
      nPart->print_values  != cPart->print_values) {
    /* No need to resize. PrintValues() function is called by expose
     * method.
     */
    do_redisplay = 1;
    initFonts(nPart);
  }

  check_line_length_arg(nPart, args, num_args, &do_redisplay, &update);

  if (nPart->update == UPDATE_NONE) {
    /* Wishing to delay recalculation of image display.
     * Leaving nPart->update equal to UPDATE_NONE, to avoid resizing,
     * and clearing do_redisplay to avoid exposure.
     */

    do_redisplay = 0;
  } else {
    nPart->update = update;
  }

  Resize(new);

  LEAVE_FUNCTION_DEBUG("Image.c: SetValues");

  return(do_redisplay); /* do_redisplay==True: Xt calls expose method. */

} /* SetValues() */




/*L*

________________________________________________________________

		Geometry
________________________________________________________________

Name:		Geometry
Syntax:		| static XtGeometryResult Geometry(wid, request, answer)
		|     Widget wid;
		|     XtWidgetGeometry *request, *answer;

Description:
Return value:
Author:		Svein Bøe
________________________________________________________________

*/

static XtGeometryResult Geometry(Widget wid, XtWidgetGeometry *request, XtWidgetGeometry *answer)
{
  XtGeometryResult result = XtGeometryYes;
  ImageWidget imWid       = (ImageWidget) wid;
  ImagePart   imPart;
  Dimension new_w = 0, new_h = 0;
  Screen *scr;
  int max_width, max_height;

  ENTER_FUNCTION_DEBUG("Image.c: Geometry");

  FPRINTF2("    request flag %d \n", request->request_mode);

  scr        = XtScreen(wid);
  max_width  = WidthOfScreen(scr);
  max_height = HeightOfScreen(scr);
  imPart     = imWid->image;
  
  if (request->request_mode & (CWWidth | CWHeight)) {

    answer->request_mode = CWWidth | CWHeight;

    if (request->width  == wid->core.width &&
	request->height == wid->core.height)
      result = XtGeometryNo;

    else if (imPart.variable_aspect == 0) {
      /* Fixed aspect ratio. */

      /* If width is changed, use this width, otherwise use height. */
      if (request->width != wid->core.width) {
	new_w = request->width;
	new_h = new_w * imPart.y_aspect / imPart.x_aspect;
      } 
      else {
	new_h = request->height;
	new_w  = new_h * imPart.x_aspect / imPart.y_aspect;
      }

      /* Make sure size is not too large. */
/*      if ((int) new_w > max_width ||
	  (int) new_h > max_height) {

	if ((int) new_w - max_width > (int) new_h - max_height) {
	  new_w = max_width;
	  new_h = new_w * imPart.y_aspect / imPart.x_aspect;
	} else {
	  new_h = max_height;
	  new_w  = new_h * imPart.x_aspect / imPart.y_aspect;
	}
      }
*/

      /* Make sure size is not too small. */
/*      if (new_w == 0) {
	new_w = 1;
	new_h = new_w * imPart.y_aspect / imPart.x_aspect;
      }

      if (new_h == 0) {
	new_h = 1;
	new_w  = new_h * imPart.x_aspect / imPart.y_aspect;
      }
*/

      /* If impossible, then change to variable aspect ratio. */
/*      if (new_w > max_width ||
	  new_h > max_height) {
	imPart.variable_aspect = 1;

	Warning(1, "%s\n%s\n",
		"Changed from fixed to variable aspect ratio because of",
		"difficult geometry request.");
      } else {
*/

	if (new_w > max_width)  new_w = max_width;
	if (new_h > max_height) new_h = max_height;

	if (new_w < 1) new_w = 1;
	if (new_h < 1) new_h = 1;

	answer->width  = new_w;
	answer->height = new_h;
	
	if (answer->width == request->width &&
	    answer->height == request->height)
	  result = XtGeometryYes;
	else result = XtGeometryAlmost;

      /* } */ /* Suggestion accepted. */
    }  /* Fixed aspect ratio. */

    if (imPart.variable_aspect == 1) {
      /* Variable aspect ratio. */

      answer->width  = request->width;
      answer->height = request->height;

      result = XtGeometryYes;
    }
  }

  FPRINTF3("    request->height: %d, request->width: %d\n",
	   request->height, request->width);
  FPRINTF3("    answer->height: %d, answer->width: %d\n",
	   answer->height, answer->width);

  LEAVE_FUNCTION_DEBUG("Image.c: Geometry");
  return(result);
}



int ImageScreenXtoPos(ImageWidget wid, int x)
{
  if (x < 0 || x >= wid->image.internal_width) return(-1);
  if (wid->image.resize)
    return(wid->image.xptr[x] + wid->image.xstart);
  else
    return(x + wid->image.xstart);
}

int ImageScreenYtoPos(ImageWidget wid, int y)
{
  if (y<0 || y >= wid->image.internal_height) return(-1);
  if (wid->image.resize)
    return(wid->image.yptr[y] + wid->image.ystart);
  else
    return(y + wid->image.ystart);
}

int ImageScreenToWidth(ImageWidget wid, int width)
{
  int i_w;

  if (width < 0) return(-1);

  i_w = wid->image.internal_width;

  if (i_w == 1) return(1);

  if (width < i_w) {
    /* Choose an "arbitrary" coordinate, one which is always legal. */
    int x = 0;

    return(ImageScreenXtoPos(wid, x + width) -
	   ImageScreenXtoPos(wid, x));
  }
  else {
    int i_w_pos, rest_pos, zero_pos, integer;

    i_w_pos  = ImageScreenXtoPos(wid, i_w - 1);
    rest_pos = ImageScreenXtoPos(wid, width % (i_w - 1));
    zero_pos = ImageScreenXtoPos(wid, 0);
    integer  = (int) (width / (i_w - 1));
    
    return(integer * (i_w_pos - zero_pos) + rest_pos - zero_pos);
  }
}

int ImageScreenToHeight(ImageWidget wid, int height)
{
  int i_h;

  if (height < 0) return(-1);

  i_h = wid->image.internal_height;

  if (i_h == 1) return(1);

  if (height < i_h) {
    int y = 0;
    /* 'y' is an "arbitrary" coordinate, one which is always legal. */

    return(ImageScreenYtoPos(wid, y + height) -
	   ImageScreenYtoPos(wid, y));
  }
  else {
    int i_h_pos, rest_pos, zero_pos, integer;

    i_h_pos  = ImageScreenYtoPos(wid, i_h - 1);
    rest_pos = ImageScreenYtoPos(wid, height % (i_h - 1));
    zero_pos = ImageScreenYtoPos(wid, 0);
    integer  = (int) (height / (i_h - 1));
    
    return(integer * (i_h_pos - zero_pos) + rest_pos - zero_pos);
  }
}

ImageData ImageScreenXYtoAdr(ImageWidget wid, int x, int y, int options)
{
  int xa, ya;
  ImageData data;

  if (wid->image.pixtype == NULL) return(NULL);
  switch(options)
    {
    case 0: data = wid->image.data; break;
    case 1: data = wid->image.red_data; break;
    case 2: data = wid->image.green_data; break;
    case 3: data = wid->image.blue_data; break;
    }
  if (data == NULL) data = wid->image.data;

  if (x == -1) {
    xa = 0;
  } else {
    if (x < 0 || x >= wid->image.internal_width) return(NULL);
    xa = (wid->image.resize) ? (wid->image.xptr[x]) : x;
  }

  if (y < 0 || y >= wid->image.internal_height) return(NULL);
  ya = (wid->image.resize) ? (wid->image.yptr[y]) : y;
  return((ImageData) (data + (wid->image.line_length) * ya
	+ xa * (wid->image.pixtype)->pixelsize));
}

ImageData ImagePosXYtoAdr(ImageWidget wid, int x, int y, int options)
{
  ImageData data;
  if (wid->image.pixtype == NULL) return(NULL);
  switch(options)
    {
    case 0: data = wid->image.data; break;
    case 1: data = wid->image.red_data; break;
    case 2: data = wid->image.green_data; break;
    case 3: data = wid->image.blue_data; break;
    }
  if (data == NULL) data = wid->image.data;
  
  x -= wid->image.xstart;
  y -= wid->image.ystart;
  if (x < 0 || x >= wid->image.width) return(NULL);
  if (y < 0 || y >= wid->image.height) return(NULL);

  return((ImageData) (data + (wid->image.line_length) * y
	+ x * (wid->image.pixtype)->pixelsize));
}

int ImagePosXtoScreen(ImageWidget wid, int x, int right)
{
  int xx, *xarr, width;
  x -=  wid->image.xstart;
  if (wid->image.resize == 0) return(x);
  xarr = wid->image.xptr;
  width= wid->image.internal_width;
  xx = (double) (x + (right ? 1 : 0) 
		 - wid->image.xpan) * wid->image.zoom;
  if (xx < 0) return(xx);
  if (xx >= width) return(xx);
  if (right == 0)
    {
      while(xx>0 && xarr[xx-1] >= x) xx--; 
      while(xx<width && xarr[xx] < x) xx++; 
    } else {
      while(xx<width-1 && xarr[xx+1] <= x) xx++; 
      while(xx>=0 && xarr[xx] > x) xx--; 
    } 
  return(xx);
}

int ImagePosYtoScreen(ImageWidget wid, int y, int bottom)
{
  int yy, *yarr, height;

  y -= wid->image.ystart;
  if (wid->image.resize == 0) return(y);
  yarr = wid->image.yptr;
  height= wid->image.internal_height;
  yy = (double) (y + (bottom ? 1 : 0) 
		 - wid->image.ypan) * wid->image.zoom;
  if (yy < 0) return(yy);
  if (yy >= height) return(yy);
  if (bottom == 0)
    {
      while(yy>0 && yarr[yy-1] >= y) yy--; 
      while(yy<height && yarr[yy] < y) yy++; 
    } else {
      while(yy<height-1 && yarr[yy+1] <= y) yy++; 
      while(yy>=0 && yarr[yy] > y) yy--; 
    } 
  return(yy);
}

int ImageWidthToScreen(ImageWidget wid, int width, int maximum)
{
  int x;

  /* Choose an "arbitrary" coordinate, one which is always legal. */
  x = wid->image.xpan + wid->image.xstart;

  return(ImagePosXtoScreen(wid, x + width, maximum) -
	 ImagePosXtoScreen(wid, x, 0));
}

int ImageHeightToScreen(ImageWidget wid, int height, int maximum)
{
  int y;

  /* Choose an "arbitrary" coordinate, one which is always legal. */
  y = wid->image.ypan + wid->image.ystart;

  return(ImagePosYtoScreen(wid, y + height, maximum) -
	 ImagePosYtoScreen(wid, y, 0));
}
 


/*L*

________________________________________________________________

	        Button
________________________________________________________________

Name:		Button
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/
static void Button(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageWidget iwid = (ImageWidget) wid;
  ImagePart *ip;  
  ImageCallbackRec button_callback;
  int x,y;
  ImageData data;
  Window wr, wm;
  int etype, rx, ry;
  unsigned int rm;

  ENTER_FUNCTION_DEBUG("Image.c: Button");

  ip = &(iwid->image);
/*  fprintf(stderr, "Type=%6d,  Serial=%6d,  Time=%6d  ,State=%6d,\
	   Button=%6d, SS=%2d\n", event->xbutton.type,
	   event->xbutton.serial, event->xbutton.time,
	   event->xbutton.state, event->xbutton.button,
	   event->xbutton.same_screen); */
  data = NULL;
  etype = event ? (event->type) : 0;
  switch(etype) 
    {
    case ButtonPress:
    case ButtonRelease:
      x = event->xbutton.x;
      y = event->xbutton.y;
      break;
    case MotionNotify:
      x = event->xmotion.x;
      y = event->xmotion.y;
      break;
    case Expose:
      x = event->xexpose.x;
      y = event->xexpose.y;
      break;
    default:
      XQueryPointer(XtDisplay(wid), XtWindow(wid), &wr, &wm, 
		    &rx, &ry, &x, &y, &rm); 
      break;
    }

  if (x < 0 || x >= ip->internal_width || 
      y < 0 || y >= ip->internal_height) {
    LEAVE_FUNCTION_DEBUG("Image.c: Button");
    return;
  }

  if (ip->pixtype) {
    data = (ImageData) ImageScreenXYtoAdr(iwid, x, y, 0); 
    if (data == NULL) {
      LEAVE_FUNCTION_DEBUG("Image.c: Button");
      return;
    }
  }
  button_callback.reason        = event ? ACTIVATE : RESIZE;
  button_callback.event         = event;
  button_callback.img           = ip->image;
  button_callback.band          = ip->band;
  button_callback.x             = ImageScreenXtoPos(iwid, x);
  button_callback.y             = ImageScreenYtoPos(iwid, y);
  button_callback.data          = data;
  button_callback.pixtype       = (ipixel *) ip->pixtype;
  button_callback.zoom          = ip->zoom;
  button_callback.magnification = ip->factor;
  button_callback.xpan          = ip->xpan + ip->xstart;
  button_callback.ypan          = ip->ypan + ip->ystart;
  XtCallCallbacks(wid, XtNcallback, (XtPointer) &button_callback);
  if (!event)  
    XtCallCallbacks(wid, XtNresizeCallback, (XtPointer) &button_callback);

  LEAVE_FUNCTION_DEBUG("Image.c: Button");

} /* Button */




/*L*

________________________________________________________________

		Nop
________________________________________________________________

Name:		Nop
Syntax:		| static void Nop(wid, event, params, num_params)
		| Widget wid;
		| XEvent *event;
		| String *params;
		| Cardinal *num_params;
Description:	No operation
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static void Nop(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ENTER_FUNCTION_DEBUG("Image.c: Nop");
  LEAVE_FUNCTION_DEBUG("Image.c: Nop");
}



/*L*

________________________________________________________________

		 ImageSetZoom
________________________________________________________________

Name:		ImageSetZoom
Syntax:         | void ImageSetZoom(wid, zoom, x, y)
		| Widget wid;
		| float zoom;
		| int x, y;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

void ImageTranslateZoomPanArgs(ImageWidget wid, char *zoomarg, char *panxarg, char *panyarg, int mousex, int mousey, float *zoom, int *xpan, int *ypan)
{
  char *ptr, *val;
  int ww, hh;

  ENTER_FUNCTION_DEBUG("Image.c: ImageTranslateZoomPanArgs");

  *zoom = wid->image.zoom;
  ptr = zoomarg;
  while (*ptr == ' ') ptr++;
  val = ptr;
  val++;
  switch(ptr[0])
    {
    case 'I': *zoom *= 2.0; break;
    case 'O': *zoom /= 2.0; break;
    case 'N': *zoom = 1.0;  break;
    case '+': *zoom += atof(val); break;
    case '-': *zoom -= atof(val); break;
    case '/': *zoom /= atof(val); break;
    case '*': *zoom *= atof(val); break;
    default:  *zoom = atof(ptr); break;
    }
  if (*zoom <= 0.0) *zoom = 1.0;

  ptr = panxarg;
  while (*ptr == ' ') ptr++;
  val = ptr;
  val++;
  switch(ptr[0])
    {
    case 'M': *xpan = mousex;  break;
    case '+': *xpan += atoi(val); break;
    case '-': *xpan -= atoi(val); break;
    default:  *xpan = atoi(ptr); break;
    }
  if (ptr[strlen(ptr)-1] == 'C')
    {
      ww = (double) wid->core.width  / *zoom + 0.5;
      *xpan = *xpan - ww/2;
    }
  ptr = panyarg;
  while (*ptr == ' ') ptr++;
  val = ptr;
  val++;
  switch(ptr[0])
    {
    case 'M': *ypan = mousey;  break;
    case '+': *ypan += atoi(val); break;
    case '-': *ypan -= atoi(val); break;
    default:  *ypan = atoi(ptr); break;
    }
  if (ptr[strlen(ptr)-1] == 'C')
    {
      hh = (double) wid->core.height / *zoom + 0.5;
      *ypan = *ypan - hh/2;
    }

  LEAVE_FUNCTION_DEBUG("Image.c: ImageTranslateZoomPanArgs");
}


static void ZoomPan(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  float zoom;
  int  xpan, ypan;

  ENTER_FUNCTION_DEBUG("Image.c: ZoomPan");

  ImageGetZoom((ImageWidget) wid, &zoom, &xpan, &ypan, NULL);
  if (*num_params <= 3)
    {
      ImageTranslateZoomPanArgs((ImageWidget) wid, params[0], params[1],
	  params[2], ImageScreenXtoPos((ImageWidget) wid, event->xbutton.x),
	  ImageScreenYtoPos((ImageWidget) wid, event->xbutton.y),
	  &zoom, &xpan, &ypan);
    } else ImageError(wid, ERR_PARAM);

  FPRINTF3("    Zoom: %.2f, xpan: %d", zoom, xpan);
  FPRINTF2(" ypan: %d\n", ypan);

  ImageSetZoom((ImageWidget) wid, zoom, xpan, ypan);

  LEAVE_FUNCTION_DEBUG("Image.c: ZoomPan");
}


static int SetZoomParam(ImageWidget wid, double zoom, int x, int y)
{
  int ww, wh, iw, ih, minx=0, miny=0, maxx, maxy;
  double minz, maxz;

  ENTER_FUNCTION_DEBUG("Image.c: SetZoomParam");

  x = x - wid->image.xstart;
  y = y - wid->image.ystart;
  ww = wid->core.width;        /* Width of widget. */
  wh = wid->core.height;       /* Height of widget. */
  iw = wid->image.width;       /* Width of BIFF image. */
  ih = wid->image.height;      /* Height of BIFF image. */

  /* Not all of a BIFF image may be visible in the widget, if e.g. the image
   * has been zoomed. Most of the time ww=iw and wh=ih (when it is possible
   * to fit the entire image inside the widget so that there is a one-to-one
   * correspondence between widget pixels and image pixels), unless the image
   * has been resized by the window manager or some size-option has been
   * used when the widget was created.
   */

  /* The zoom parameter may be less than, equal to or greater than 1.0 (but
   * never negative). The smallest possible zoom-factor occurs when the
   * whole image is visible inside the widget.
   */
  minz = ((double)ww/iw > (double)wh/ih) ? (double)ww/iw : (double)wh/ih;

  /* minz may be less than, equal to or greater than 1.0. */

  /* The largest possible zoom-factor occurs when a single BIFF image pixel
   * occupies the whole widget.
   */
  maxz = (ww > wh) ? ww : wh;

  /* Limit the zoom-factor to the possible interval. */
  if (zoom < minz) zoom = minz; else if (zoom > maxz) zoom = maxz;
  if (zoom == 0.0) zoom = 1.0;

  maxx = (double) wid->core.width/zoom;
  maxx = wid->image.width - maxx;
  maxy = (double) wid->core.height/zoom;
  maxy = wid->image.height - maxy;

  if (x <= minx) x = minx; else if (x > maxx) x = maxx;
  if (y <= miny) y = miny; else if (y > maxy) y = maxy;
 
  FPRINTF2("    Zoom %.2f", zoom);
  FPRINTF3(", x=%d, y=%d", x, y);
  FPRINTF3(", maxx=%d, maxy=%d\n", maxx, maxy);
  if (wid->image.zoom == zoom && wid->image.xpan == x && 
      wid->image.ypan == y) {
    LEAVE_FUNCTION_DEBUG("Image.c: SetZoomParam");    
    return(0);
  }
  
  wid->image.zoom = zoom;
  wid->image.xpan = x;
  wid->image.ypan = y;

  LEAVE_FUNCTION_DEBUG("Image.c: SetZoomParam");
  return(1);
}

void ImageSetZoom (ImageWidget wid, float zoom, int x, int y)
{
  ImageWidget wid2;
  ImageClassPart *ic = &imageClassRec.image_class;
  int on, count, i;
  int w1, h1, w2, h2; 

  ENTER_FUNCTION_DEBUG("Image.c: ImageSetZoom");

  on = ic -> zoom_all;
  count = on ? ic->count : 1;
  if (SetZoomParam(wid, zoom, x, y))
    ImageRedisplay(wid, UPDATE_ZOOMPAN);
  if (on == 0) {
    LEAVE_FUNCTION_DEBUG("Image.c: ImageSetZoom");
    return;
  }
  
  zoom = wid->image.zoom;
  w1 = (double) wid->core.width  / 2.0 / zoom + 0.5;
  h1 = (double) wid->core.height / 2.0 / zoom + 0.5;
  wid2 = wid;
  for (i=0; i<count; i++)  
    {
      wid2 = (ImageWidget) ic->widget_list[i];
      if (wid == wid2) continue;
      w2 = (double) wid2->core.width  / 2.0 / zoom + 0.5;
      h2 = (double) wid2->core.height / 2.0 / zoom + 0.5;
      if (SetZoomParam(wid2, zoom, x + w1 - w2, y + h1 - h2))
	ImageRedisplay(wid2, UPDATE_ZOOMPAN);
    }

  LEAVE_FUNCTION_DEBUG("Image.c: ImageSetZoom");
}

void ImageGetZoom(ImageWidget wid, float *zoom, int *xpan, int *ypan, double *factor)
{
  ENTER_FUNCTION_DEBUG("Image.c: ImageGetZoom");

  if (zoom) *zoom     = wid->image.zoom;
  if (xpan) *xpan     = wid->image.xpan + wid->image.xstart;
  if (ypan) *ypan     = wid->image.ypan + wid->image.ystart;
  if (factor) *factor = wid->image.factor;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageGetZoom");
}




/*L*

________________________________________________________________

		 ImageKill
________________________________________________________________

Name:		ImageKill
Syntax:		| static void ImageKill( Widget wid, XEvent *event,
                |    String *params, Cardinal *num_params )
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

void ImageKill( Widget wid, XEvent *event, String *params, Cardinal *num_params )
{
  ENTER_FUNCTION_DEBUG("Image.c: ImageKill");

  XtDestroyWidget(TopLevelWidget(wid));

  LEAVE_FUNCTION_DEBUG("Image.c: ImageKill");
}



/*L:ImageRedisplay*

________________________________________________________________

		ImageRedisplay
________________________________________________________________

Name:		ImageRedisplay
Syntax:		| void ImageRedisplay(wid, update)
                | ImageWidget wid;
                | update_status update;
Author:		Otto Milvang
________________________________________________________________

*/

void ImageRedisplay(ImageWidget wid, update_status update)
{
  WidgetClass wc;
  CoreClassPart *ic;

  ENTER_FUNCTION_DEBUG("Image.c: ImageRedisplay");
  FPRINTF2("    update=%d\n",update);

  wc = XtClass(wid);
  ic = &wc->core_class;
  if (update != UPDATE_NONE) wid->image.update = update;

  /* The next two calls may refer to subclass methods, depending on
   * class of wid. */
  (ic->resize)((Widget) wid);
  (ic->expose)((Widget) wid, NULL, NULL);

  LEAVE_FUNCTION_DEBUG("Image.c: ImageRedisplay");
}



/*L:ImageRedisplayScreen*

________________________________________________________________

		ImageRedisplayScreen
________________________________________________________________

Name:		ImageRedisplayScreen
Syntax:		| void ImageRedisplayScreen(wid, x, y, width, height)
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

void ImageRedisplayScreen(ImageWidget wid, int x, int y, int width, int height)
{
  WidgetClass wc;
  Region rect;
  CoreClassPart *ic;
  XPoint p[4];
  update_status update;
  ImagePart *ip;

  ENTER_FUNCTION_DEBUG("Image.c: ImageRedisplayScreen");
  FPRINTF3("    x=%4d, y=%4d ", x, y);
  FPRINTF3("    width=%4d, height=%4d\n",width, height);

  wc = XtClass(wid);
  ic = &wc->core_class;
  ip = &(wid->image);
  update = ip->update;
  ip->update  = UPDATE_REDISPLAY;
  ip->updatex = x;
  ip->updatey = y;
  ip->updatew = width;
  ip->updateh = height;
  (ic->resize)((Widget) wid);

  p[0].x = p[3].x = x;
  p[1].x = p[2].x = x + width;
  p[0].y = p[1].y = y;
  p[2].y = p[3].y = y + height;
  rect = XPolygonRegion(p, 4, EvenOddRule);
  (ic->expose)((Widget) wid, NULL, rect);
  ip->update = update;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageRedisplayScreen");
}




/*L:ImageRedisplayArea*

________________________________________________________________

		ImageRedisplayArea
________________________________________________________________

Name:		ImageRedisplayArea
Syntax:		| int ImageRedisplayArea(wid, x, y, width, height)
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

int ImageRedisplayArea(ImageWidget wid, int x, int y, int width, int height)
{
  int x1, x2, y1, y2, w, h;

  ENTER_FUNCTION_DEBUG("Image.c: ImageRedisplayArea");
  FPRINTF3("    x=%d, y=%d", x, y);
  FPRINTF3(", widht=%4d, height=%4d\n", width, height);

  x1 = ImagePosXtoScreen(wid, x, 0);
  x2 = ImagePosXtoScreen(wid, x+width-1, 1);
  y1 = ImagePosYtoScreen(wid, y, 0);
  y2 = ImagePosYtoScreen(wid, y+height-1, 1);
  FPRINTF3("    x1=%d, x2= %d", x1, x2);
  FPRINTF3(", y1=%4d, y2=%4d\n", y1, y2);
  w = x2 - x1 + 1;
  h = y2 - y1 + 1;
  if (x1 >= wid->image.internal_width || y1 >= wid->image.internal_height) {
    LEAVE_FUNCTION_DEBUG("Image.c: ImageRedisplayArea");
    return(0);
  }
  if (x1 < 0)
    {
      w += x1;
      x1 = 0;
    }
  if (y1 < 0)
    {
      h += y1;
      y1 = 0;
    }
  if (w < 0 || h < 0) {
    LEAVE_FUNCTION_DEBUG("Image.c: ImageRedisplayArea");
    return(0);
  }
  if (w > wid->image.internal_width - x1)  w = wid->image.internal_width  - x1;
  if (h > wid->image.internal_height - y1) h = wid->image.internal_height - y1;
  ImageRedisplayScreen(wid, x1, y1, w, h);

  LEAVE_FUNCTION_DEBUG("Image.c: ImageRedisplayArea");
  return(1);
}



/*L:ImageCopyImagePart*

________________________________________________________________

		ImageCopyImagePart
________________________________________________________________

Name:		ImageCopyImagePart
Syntax:		| void ImageCopyImagePart(widto, widfrom)
                | ImageWidget widto, widfrom;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/




void ImageCopyImagePart(ImageWidget widto, ImageWidget widfrom)
{
  ENTER_FUNCTION_DEBUG("Image.c: ImageCopyImagePart");

  widto->image = widfrom->image;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageCopyImagePart");
}

void ImageCopyResources(ImageWidget widto, ImageWidget widfrom)
{
  ImagePart tp, fp;

  ENTER_FUNCTION_DEBUG("Image.c: ImageCopyResources");

  tp = widto->image;
  fp = widfrom->image;

  tp.image           = fp.image;
  tp.band            = fp.band;
  tp.red_band        = fp.red_band;
  tp.green_band      = fp.green_band;
  tp.blue_band       = fp.blue_band;
  tp.data            = fp.data;
  tp.red_data        = fp.red_data;
  tp.green_data      = fp.green_data;
  tp.blue_data       = fp.blue_data;
  tp.title           = fp.title;
  tp.pixname         = fp.pixname;
  tp.width           = fp.width;
  tp.height          = fp.height;
  tp.variable_aspect = fp.variable_aspect;
  tp.line_length     = fp.line_length;
  tp.xstart          = fp.xstart;
  tp.ystart          = fp.ystart;
  tp.displaymethod   = fp.displaymethod;
  tp.print_values    = fp.print_values;
  tp.colortab        = fp.colortab;
  tp.colorcells      = fp.colorcells;
  tp.colorptr        = fp.colorptr;
  tp.visual          = fp.visual;
  tp.scaletype       = fp.scaletype;
  tp.scalea          = fp.scalea;
  tp.scaleb          = fp.scaleb;
  tp.minval          = fp.minval;
  tp.maxval          = fp.maxval;
  tp.font[0]         = fp.font[0];
  tp.font[1]         = fp.font[1];
  tp.font[2]         = fp.font[2];
  tp.font[3]         = fp.font[3];
  tp.button_action   = fp.button_action;
  tp.resize_action   = fp.resize_action;
  tp.update          = fp.update;
  tp.menubar         = fp.menubar;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageCopyResources");

} /* ImageCopyResources() */

void ImageGetResources(ImageWidget wid, ArgList *args, Cardinal *num_args)
{
  XtResourceList resource_list;
  int i;

  ENTER_FUNCTION_DEBUG("Image.c: ImageGetResources");

  XtGetResourceList(imageWidgetClass, &resource_list, num_args);
  *args = (Arg *) malloc(sizeof(Arg) * (*num_args));

  for (i = 0; i < (*num_args); i++) {
    (*args)[i].name  = resource_list[i].resource_name;
  }
  XtGetValues((Widget) wid, (*args), *num_args);

  LEAVE_FUNCTION_DEBUG("Image.c: ImageGetResources");

  return;

} /* ImageGetResources() */

void ImageGetImageResources(ImageWidget wid, ArgList *args, Cardinal *num_args)
{
  ImagePart ip;
  int i;

  ENTER_FUNCTION_DEBUG("Image.c: ImageGetImageResources");

/*  *num_args = 36; */
  *num_args = 35;
  *args = (Arg *) malloc(sizeof(Arg) * (*num_args));

  ip = wid->image;
  i  = 0;

  (*args)[i].name    = XtNimageImage;
  (*args)[i++].value = (XtArgVal) ip.image;
  (*args)[i].name    = XtNimageBand;
  (*args)[i++].value = (XtArgVal) ip.band;
  (*args)[i].name    = XtNimageRedBand;
  (*args)[i++].value = (XtArgVal) ip.red_band;
  (*args)[i].name    = XtNimageGreenBand;
  (*args)[i++].value = (XtArgVal) ip.green_band;
  (*args)[i].name    = XtNimageBlueBand;
  (*args)[i++].value = (XtArgVal) ip.blue_band;
  (*args)[i].name    = XtNimageRgb;
  (*args)[i++].value = (XtArgVal) ip.image_rgb;
  (*args)[i].name    = XtNimageData;
  (*args)[i++].value = (XtArgVal) ip.data;
  (*args)[i].name    = XtNimageRedData;
  (*args)[i++].value = (XtArgVal) ip.red_data;
  (*args)[i].name    = XtNimageGreenData;
  (*args)[i++].value = (XtArgVal) ip.green_data;
  (*args)[i].name    = XtNimageBlueData;
  (*args)[i++].value = (XtArgVal) ip.blue_data;
  (*args)[i].name    = XtNimageTitle;
  (*args)[i++].value = (XtArgVal) ip.title;
  (*args)[i].name    = XtNimagePixtype;
  (*args)[i++].value = (XtArgVal) ip.pixname;
  (*args)[i].name    = XtNimageWidth;
  (*args)[i++].value = (XtArgVal) ip.width;
  (*args)[i].name    = XtNimageHeight;
  (*args)[i++].value = (XtArgVal) ip.height;
  (*args)[i].name    = XtNvariableAspect;
  (*args)[i++].value = (XtArgVal) ip.variable_aspect;
  (*args)[i].name    = XtNimageLineLength;
  (*args)[i++].value = (XtArgVal) ip.line_length;
  (*args)[i].name    = XtNimageXstart;
  (*args)[i++].value = (XtArgVal) ip.xstart;
  (*args)[i].name    = XtNimageYstart;
  (*args)[i++].value = (XtArgVal) ip.ystart;
  (*args)[i].name    = XtNdisplayMethod;
  (*args)[i++].value = (XtArgVal) ip.displaymethod;
  (*args)[i].name    = XtNprintValues;
  (*args)[i++].value = (XtArgVal) ip.print_values;
  (*args)[i].name    = XtNcolortab;
  (*args)[i++].value = (XtArgVal) ip.colortab;
  (*args)[i].name    = XtNcolorCells;
  (*args)[i++].value = (XtArgVal) ip.colorcells;
  (*args)[i].name    = XtNcolorPtr;
  (*args)[i++].value = (XtArgVal) ip.colorptr;
  (*args)[i].name    = XtNvisual;
  (*args)[i++].value = (XtArgVal) ip.visual;
  (*args)[i].name    = XtNscaletype;
  (*args)[i++].value = (XtArgVal) ip.scaletype;
  (*args)[i].name    = XtNscalea;
  (*args)[i++].value = (XtArgVal) ip.scalea;
  (*args)[i].name    = XtNscaleb;
  (*args)[i++].value = (XtArgVal) ip.scaleb;
  (*args)[i].name    = XtNminval;
  (*args)[i++].value = (XtArgVal) ip.minval;
  (*args)[i].name    = XtNmaxval;
  (*args)[i++].value = (XtArgVal) ip.maxval;
  (*args)[i].name    = XtNfont;
  (*args)[i++].value = (XtArgVal) ip.font[0];
  (*args)[i].name    = XtNfont1;
  (*args)[i++].value = (XtArgVal) ip.font[1];
  (*args)[i].name    = XtNfont2;
  (*args)[i++].value = (XtArgVal) ip.font[2];
  (*args)[i].name    = XtNfont3;
  (*args)[i++].value = (XtArgVal) ip.font[3];
/*
  (*args)[i].name    = XtNcallback;
  (*args)[i++].value = (XtArgVal) ip.button_action;
  (*args)[i].name    = XtNresizeCallback;
  (*args)[i++].value = (XtArgVal) ip.resize_action;
*/
  (*args)[i].name    = XtNimageUpdate;
  (*args)[i++].value = (XtArgVal) ip.update;
  (*args)[i].name    = XtNmenubar;
  (*args)[i++].value = (XtArgVal) ip.menubar;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageGetImageResources");

  return;

} /* ImageGetImageResources() */



/*L:ImageSetZoomAll*

________________________________________________________________

		ImageSetZoomAll
________________________________________________________________

Name:		ImageSetZoomAll
Syntax:		| int ImageSetZoomAll(wid, on)
                | Widget wid;
                | int on;
Description:	Set zoom all flag, If 'on' = TRUE, all widgets
                will use the same zoom and pan parameters.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


int ImageSetZoomAll(Widget wid, int on)
{
  ImageClassPart *ic = &imageClassRec.image_class;

  ENTER_FUNCTION_DEBUG("Image.c: ImageSetZoomAll");

  if (on >= 0) ic -> zoom_all = on;

  LEAVE_FUNCTION_DEBUG("Image.c: ImageSetZoomAll");
  return(ic -> zoom_all);
}



/*L:ImageGetZoomAll*

________________________________________________________________

		ImageGetZoomAll
________________________________________________________________

Name:		ImageGetZoomAll
Syntax:		| int ImageGetZoomAll(wid)
                | Widget wid;
Description:	Return zoom_all flag.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

int ImageGetZoomAll(Widget wid)
{
  ImageClassPart *ic = &imageClassRec.image_class;

  ENTER_FUNCTION_DEBUG("Image.c: ImageGetZoomAll");
  LEAVE_FUNCTION_DEBUG("Image.c: ImageGetZoomAll");

  return(ic -> zoom_all);
}
