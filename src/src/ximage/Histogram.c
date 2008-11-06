

/*

________________________________________________________________

        Histogram.c
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

static char *Id = "$Id";



/*F:Histogram*

________________________________________________________________

		Histogram
________________________________________________________________

Name:		Histogram - Widget class for histograms in XITE programs

Application header file: <xite/Histogram.h>

Class header file: <xite/HistogramP.h>

Class:          histogramWidgetClass

Class name:     Histogram

Superclass:     Core

Description:    The Histogram widget is part of the XITE (X-based Image
                Processing Tools and Environment) software. It is used by the
                histogram application window included with several of the
		display programs supplied with XITE (and available via the
		ximage toolkit).

                See the documentation on the main XITE display program
                'xshow(1)' as well as the XITE ximage toolkit 'ximage(3)' for
                more information on how this widget can be used in an
                application.

Resources:      Apart from the resources of the superclass:

		&XtNhistogramHistogram (class XtCHistogramHistogram)
		Type: Pointer, 
		Default: NULL

		A reference to an integer array with histogram values.

		&XtNhistogramLength (class XtCHistogramLength)
		Type: int, 
		Default: NULL

		Length of histogram and cumulative histogram arrays.

		&XtNhistogramTitle (class XtCHistogramTitle)
		Type: Pointer, 
		Default: NULL

		Title of histogram. Used to form window manager decoration
		title.

		&XtNhistogramVisual (class XtCHistogramVisual)
		Type: Pointer, 
		Default: NULL

		Visual of Histogram widget. See the "Xlib Programming Manual"
                by Adrian Nye (O''Reilly & Associates, Inc.), chapter 7 for
                more information about visuals.

		&XtNhistogramColormap (class XtCHistogramColormap)
		Type: Pointer, 
		Default: NULL

		Colormap used by the histogram widget.

		&XtNhistogramImage (class XtCHistogramImage)
		Type: Pointer, 
		Default: NULL

		A reference to the BIFF image data structure.

		&XtNhistogramBand (class XtCHistogramBand)
		Type: Pointer, 
		Default: NULL

		A reference to the BIFF band data structure for which this
		histogram is made.

		&XtNhistogramCrossing (class XtCCallback)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this callback list will be called by the
		'Crossing' action function described below.

		&XtNhistogramButton (class XtCCallback)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this callback list may be called by the
		'Button' action function described below.

		&XtNhistogramMotion (class XtCHistogramMotion)
		Type: XtCallbackList, 
		Default: NULL

		The functions on this callback list may be called by the
		'Button' action function described below.

		&XtNhistogramKill (class XtCHistogramKill)
		Type: XtCallbackList, 
		Default: NULL

		Not invoked by the Histogram widget.

Histogram actions:
                &Crossing
		Invokes the functions on the callback list
		XtNhistogramCrossing.

		&Button
		This action assigns values to the attributes of the
		HistogramButtonCallbackRec structure and invokes the callback
		functions on the XtNhistogramButton (if the event was
		ButtonPress or ButtonRelease) or XtNhistogramMotion (if the
		event was MotionNotify) callback-list with a reference to
		the HistogramButtonCallbackRec structure.

                The attributes of the HistogramButtonCallbackRec structure are

		    &&event
		    An "XEvent *" reference to the event which triggered the
		    action function.

		    &&image
		    Identical to the resource XtNhistogramImage.

		    &&band
		    Identical to the resource XtNhistogramBand.

		    &&histogram
		    Identical to the resource XtNhistogramHistogram.

		    &&kumulativ
		    A reference to an integer array with cumulative histogram
		    values.

		    &&length
		    Idential to the resource XtNhistogramLength.

		    &&korh
		    Integer which is set if the displayed histogram is
		    cumulative, unset if it is normal.

		    &&x, y
		    Two integers which represent the coordinates where one of
		    the events ButtonPress, ButtonRelease or MotionNotify took
		    place. 'y' is 255 minus the actual event vertical
		    coordinate, so that it represents the height above the
		    baseline in the Histogram widget.

		&ChangeMode

		This action function changes mode between display of
		histogram and display of cumulative histogram.

		&HistogramKill

		Same as the HistogramKill convenience function described below.

Default translation bindings:

		| <EnterWindow>:         Crossing()
		| <LeaveWindow>:         Crossing()
		| <Key>Q:                HistogramKill()
		| Ctrl <BtnDown>:        ChangeMode()
		| Ctrl <BtnUp>:          ChangeMode()
		| <BtnDown>:             Button()
		| <BtnUp>:               Button()
		| <Motion>:              Button()

Histogram callbacks:

                See the resources XtNhistogramCrossing, XtNhistogramButton,
		XtNhistogramMotion and XtNhistogramKill.

Convenience routines:

                &HistogramRedisplay
                | void HistogramRedisplay( Widget wid );

		Invoke the expose method, with NULL as event and region
		arguments.

		&Draw_all_lines
		| void Draw_all_lines( Widget wid, int func );

		Draw the manipulating lines in the Histogram widget.
		'func' gives the logical drawing function, typically
		GXor (draw) or GXandInverted (erase).

		&HistogramKill
		| void HistogramKill ( Widget wid, XEvent *event,
		|    String *params, Cardinal *num_params );

		Kill the widget tree starting with the ancestor shell.

		&HistogramInitLines
		| void HistogramInitLines( HistogramWidget wid,
		|    int col, int threshold );

		Setup to draw a unit ramp or a threshold line (depending on
		whether 'threshold' is set or not) in the Histogram widget,
		for the primaries given by 'col' (which may be inclusive or of
		DoRed, DoGreen and DoBlue). Then draw all three manipulating
		lines (one or more of which has now been changed).

		&HistogramThreshold
		| void HistogramThreshold( HistogramWidget wid,
		|    int point, int col );

		Make treshold manipulating lines for the primaries indicated
		by 'col' (which may by inclusive or of DoRed, DoGreen and
		DoBlue). Use 'point' as the bin number where the threshold
		will be. Draw all three manipulating lines (of which some
		may now be thresholds, some are as before).

		&HistogramInsertPoint
		| void HistogramInsertPoint( HistogramWidget wid,
		|    int point, int val, int col );

		Insert a point at 'point' (for a broken line) in the
		manipulating primary given by 'col'.

		&HistogramMovePoint
		| void HistogramMovePoint( HistogramWidget wid,
		|    int point, int val, int col );

		Move the line breakpoint (in primaries given by 'col') closest
		to 'point' to new location 'point' (if the closest point is
		not too far off).
		
		&HistogramDeletePoint
		| void HistogramDeletePoint( HistogramWidget wid,
		| int point, int val, int col );

		Delete line breakpoint closest to 'point' for primaries given
		by 'col'. Only delete point if it is not too far off 'point'.
		'val' is not used.

		&HistogramInspect
		| void HistogramInspect( HistogramWidget wid,
		|    XColor *colortab );

		Return in 'colortab' (which must be allocated by the caller)
		a colortable which reflects the manipulating primary lines
		in the Histogram widget.

		&HistogramGet
		| void HistogramGet( HistogramWidget wid,
		|    XColor *tab, int *len );

		Return in 'tab' the entries in the Histogram widget
		manipulating lines which are set (i.e. breakpoints).
		the flags attribute of a colortable entry is taken from
		the mask attribute of the Histogram widget (inclusive or of
		DoRed, DoGreen and DoBlue).

		'len' returns the number of cells set in 'tab' (from cell
		number zero to cell number 'len'-1.
		
		&HistogramSet
		| void HistogramSet( HistogramWidget wid,
		|    XColor *tab, int len );

		Set the manipulating primary lines from 'tab'.

Author:         Otto Milvang and Svein Bøe

See also:       'ximage(3)', 'Image(3)', 'ImageOverlay(3)', 'xhistogram(3)',
                'xshow(1)'

Doc:            Svein Bøe
________________________________________________________________

*/



#include <xite/includes.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/XawInit.h>
#include "HistogramP.h"
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/Visual.h>
#include <xite/Cmap.h>
#include <xite/ShellWids.h>
#include <xite/color.h>
#include <xite/Xcolor.h>
#include <xite/debug.h>



static void ClassPartInitialize(WidgetClass wc);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Realize(Widget wid, XtValueMask *valueMask, XSetWindowAttributes *attrs);
static void Redisplay(Widget wid, XEvent *event, Region region);
static void Crossing(Widget wid, XEvent *event, String *params, Cardinal *num_params);
static void Button(Widget wid, XEvent *event, String *params, Cardinal *num_params);
static void ChangeMode(Widget wid, XEvent *event, String *params, Cardinal *num_params);
static void Kill(Widget wid);

static int screen_s = 0;
 
static XtActionsRec actions[] =
{
  /* {name, procedure}, */
    {"Crossing",	Crossing},
    {"Button",	        Button},
    {"ChangeMode",	ChangeMode},
    {"HistogramKill",	HistogramKill},
};

 static char translations[] =
"<EnterWindow>:         Crossing()\n\
 <LeaveWindow>:         Crossing()\n\
 <Key>Q:                HistogramKill()  \n\
 Ctrl <BtnDown>:	ChangeMode() \n\
 Ctrl <BtnUp>:  	ChangeMode() \n\
 <BtnDown>:		Button()\n\
 <BtnUp>:               Button()\n\
 <Motion>:		Button()"; 




static XtResource resources[] = {
#define ioffset(field) XtOffsetOf(HistogramRec, histogram.field)
#define coffset(field) XtOffsetOf(HistogramRec, core.field)
    /* {name, class, type, size, ioffset, default_type, default_addr}, */
    { XtNhistogramHistogram, XtCHistogramHistogram, XtRPointer, sizeof(int *),
	  ioffset(histogram), XtRPointer , NULL }, 
    { XtNhistogramLength, XtCHistogramLength, XtRInt, sizeof(int),
	  ioffset(length), XtRInt, NULL }, 
    { XtNhistogramTitle, XtCHistogramTitle, XtRPointer, sizeof(int),
	  ioffset(title), XtRString, NULL }, 
    { XtNhistogramVisual, XtCHistogramVisual, XtRPointer, sizeof(char *),
	  ioffset(visual), XtRPointer, NULL },
    { XtNhistogramColormap, XtCHistogramColormap, XtRPointer, sizeof(Colormap),
	  ioffset(colormap), XtRPointer, NULL },
    { XtNhistogramImage, XtCHistogramImage, XtRPointer, sizeof(char *),
	  ioffset(image), XtRPointer, NULL }, 
    { XtNhistogramBand, XtCHistogramBand, XtRPointer, sizeof(char *),
	  ioffset(band), XtRPointer, NULL }, 
    { XtNhistogramCrossing, XtCCallback, XtRCallback, sizeof(caddr_t),
	  ioffset(crossing_action), XtRCallback,(caddr_t) NULL },
    { XtNhistogramButton, XtCCallback, XtRCallback, sizeof(caddr_t),
	  ioffset(button_action), XtRCallback, (caddr_t) NULL },
    { XtNhistogramMotion, XtCHistogramMotion, XtRCallback, sizeof(caddr_t),
	  ioffset(motion_action), XtRCallback, (caddr_t) NULL },
    { XtNhistogramKill, XtCHistogramKill, XtRCallback, sizeof(caddr_t),
	  ioffset(kill_action), XtRCallback, (caddr_t) NULL },

#undef ioffset
#undef coffset
};



HistogramClassRec histogramClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &widgetClassRec,
    /* class_name		*/	"Histogram",
    /* widget_size		*/	(Cardinal) sizeof(HistogramRec),
    /* class_initialize		*/	NULL,
    /* class_part_initialize	*/	ClassPartInitialize,
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
    /* resize			*/	NULL,
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
  },
  { /* histogram fields */
    /* count			*/	0,
  }
};

WidgetClass histogramWidgetClass = (WidgetClass)&histogramClassRec;



static void ClassPartInitialize(WidgetClass wc)
{
  ENTER_FUNCTION_DEBUG("Histogram.c: ClassPartInitialize");
  XawInitializeWidgetSet();
  FPRINTF2("    core_class.class_name: %s.\n", wc->core_class.class_name);
  LEAVE_FUNCTION_DEBUG("Histogram.c: ClassPartInitialize");
} 





static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
  HistogramWidget ic = (HistogramWidget) current;
  HistogramWidget ir = (HistogramWidget) request;
  HistogramWidget in = (HistogramWidget) new;
  Display *dpy;

  ENTER_FUNCTION_DEBUG("Histogram.c: SetValues");

  dpy = XtDisplay(new);

  if (ic->histogram.visual != ir->histogram.visual)
    XtErrorMsg("Histogram", "Initialize", "Histogram",
    "Can't change Visual", NULL,0);

  if (ic->histogram.length != ir->histogram.length)
  {
   /* ?? */
  }
  in->core.colormap = in->histogram.colormap;

  LEAVE_FUNCTION_DEBUG("Histogram.c: SetValues");
  return(FALSE);
}



static void CalcCumulativeHistogram(HistogramPart *hPart, int numBins)
{
  int i, *hptr, *kptr;

  hptr = hPart -> histogram;
  kptr = hPart -> kumulativ = (int *) malloc(numBins * sizeof(int));
  kptr[0] = hptr[0];

  for (i=1; i < numBins; i++)
    kptr[i] = kptr[i-1] + hptr[i];

} /* CalcCumulativeHistogram() */

static int MaxHisto(HistogramPart *hPart, int numBins)
{
  int i, max, *hi;

  hi = hPart -> histogram;
  for (max=0, i=0; i < numBins; i++)
    if (hi[i] > max) max = hi[i];

  return(max);

} /* MaxHisto() */

static void CalcDhistogram(HistogramPart *hPart, int numBins)
{
  int i, *dptr, *hptr, max, tmp;

  dptr  = hPart -> dhistogram = (int *) malloc(numBins * sizeof(int));
  hptr  = hPart -> histogram;
  max   = MaxHisto(hPart, numBins) + 1;

  for (i=0; i < numBins; i++)
    dptr[i] = (tmp = ((double) hptr[i] / (double) max)*256) > 255 ? 255 : tmp;

} /* CalcDhistogram() */

static void CalcDkumulativ(HistogramPart *hPart, int numBins, int height)
{
  int i, *dptr, *kptr, max;

  dptr = hPart -> dkumulativ = (int *) malloc(numBins * sizeof(int));
  kptr = hPart -> kumulativ;  
  max  = hPart -> kumulativ[numBins - 1] + 1;

  for (i=0; i < numBins; i++)
    dptr[i] = (int) (((double) kptr[i] * (double) height) / (double) max);

} /* CalcDkumulativ() */

static void InitMask(HistogramPart *hPart, int numBins)
{
  int i;

  /* No manipulating lines to be drawn for any primary color component. */
  for (i=0; i < numBins; i++)
    hPart->mask[i] = 0;

} /* InitMask() */



static void Initialize(Widget request, Widget new, ArgList args,
			     Cardinal *num_args)
{
  HistogramWidget newhw = (HistogramWidget) new;
  HistogramPart *newhp;
  int numBins = 256,
      height  = 256;
  Display *dpy;

  ENTER_FUNCTION_DEBUG("Histogram.c: Initialize");
  dpy = XtDisplay(new);
  screen_s = DefaultScreen(dpy);

  if (new == NULL) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: Initialize");
    return;
  }
  newhp = &(newhw->histogram);
  FPRINTF3("    newhp->histogram=%d, newhp->length=%d\n",
	   *newhp->histogram, newhp->length);

  if (newhp->histogram == 0 || newhp->length == 0)
    XtErrorMsg("Histogram", "Initialize", "Histogram",
    "Unknown histogram address or length", NULL,0);

  if (newhp->title == 0) {
    newhp-> title = (char *) malloc(33);
    strcpy(newhp->title, "Histogram");
  }

/*  
  newhp->planes = 0;
  GetImageVisualAndColormap(dpy, screen_s, &newhp->planes, &newhp->visual,
			    &newhp->colormap);
*/

  newhp->planes        = newhw->core.depth;
  newhw->core.colormap = newhp->colormap;

  CalcCumulativeHistogram(newhp, numBins);
  CalcDhistogram(newhp, numBins);
  CalcDkumulativ(newhp, numBins, height);
  InitMask(newhp, numBins);

  if (newhw->core.width == 0 || newhw->core.height == 0)
     XtResizeWidget(new, numBins, height, 1);

  newhp -> korh = 0;
  histogramClassRec.histogram_class.count ++; 
  LEAVE_FUNCTION_DEBUG("Histogram.c: Initialize");
  FPRINTF3("    %d %d \n", *newhp->histogram, newhp->length);
}



static void Realize(Widget wid, XtValueMask *valueMask, XSetWindowAttributes *attrs)
{
  Display *dpy;
  HistogramWidget hWid = (HistogramWidget) wid;
  HistogramPart *hPart;
  XGCValues gcvalues;
  XColor rgb_def;

  ENTER_FUNCTION_DEBUG("Histogram.c: Realize");

  dpy = XtDisplay(wid);

  hPart = &(hWid->histogram);

/*  *valueMask |= (CWColormap | CWBackPixel | CWBackPixmap); */
  *valueMask |= (CWColormap | CWBackPixel);
/*  attrs->background_pixmap = ParentRelative; */

  if (Has_changeable_map(XtDisplay(wid),
			       DefaultScreen(XtDisplay(wid)),
			       (hPart)->visual)) {
    /* Changeable colormap, which means that the whole colormap has been
     * allocated read/write, so a call to XAllocColor will not work.
     * However, certain specific colors have been allocated at known pixel
     * values.
     */
    attrs->background_pixel = RGBPixel(192, hPart->visual);

  } else {
    /* Immutable colormap. This means we're using the default colormap,
     * which is read/write, and a call to XAllocColor will work.
     */

    if (hPart->planes == 1) rgb_def.pixel = WhitePixel(dpy, screen_s);
    else {
      if (!XParseColor(dpy, hPart->colormap, "white", &rgb_def)) {
	FPRINTF1("    White color not in database.\n");
      } else {
	FPRINTF4("    RGB values from database: %d, %d, %d\n",
		 rgb_def.red, rgb_def.green, rgb_def.blue);
      }

      if (!XAllocColor(dpy, hPart->colormap, &rgb_def)) {
	FPRINTF1("    All colorcells allocated and read/write.\n");
	rgb_def.pixel = WhitePixel(dpy, screen_s);
      }
    }

    attrs->background_pixel = rgb_def.pixel;
  }

  XtCreateWindow(wid, InputOutput, hPart->visual, *valueMask, attrs);

  gcvalues.background = attrs->background_pixel;
  if (Has_changeable_map(XtDisplay(wid), DefaultScreen(XtDisplay(wid)),
			       (hPart)->visual)) {
    /* Changeable colormap, which means that the whole colormap has been
     * allocated read/write, so a call to XAllocColor will not work.
     * However, certain specific colors have been allocated at known pixel
     * values.
     */

    gcvalues.foreground = RGBPixel(200, hPart->visual);
  } else {
    /* Immutable colormap. This means we're using the default colormap,
     * which is read/write, and a call to XAllocColor will work.
     */

    if (hPart->planes == 1) rgb_def.pixel = BlackPixel(dpy, screen_s);
    else {
      if (!XParseColor(dpy, hPart->colormap, "black", &rgb_def)) {
	FPRINTF1("    Black color not in database.\n");
      } else {
	FPRINTF4("    RGB values from database: %d, %d, %d\n",
		 rgb_def.red, rgb_def.green, rgb_def.blue);
      }

      if (!XAllocColor(dpy, hPart->colormap, &rgb_def)) {
	FPRINTF1("    All colorcells allocated and read/write.\n");
	rgb_def.pixel = BlackPixel(dpy, screen_s);
      }
    }

    gcvalues.foreground = rgb_def.pixel;
  }

  hWid->histogram.gc = 
    XCreateGC(dpy, XtWindow(hWid), GCForeground | GCBackground, &gcvalues);

  XDefineCursor(dpy, XtWindow(wid), XCreateFontCursor(dpy,XC_crosshair));

  LEAVE_FUNCTION_DEBUG("Histogram.c: Realize");
}



void HistogramRedisplay(Widget wid)
{
  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramRedisplay");
  Redisplay(wid, NULL, NULL);
  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramRedisplay");
}


static void Draw_lines(Display *dpy, Widget wid, GC gc, XColor *coltab, unsigned char *mask, unsigned char *line, int plane, int func)
{
  /* plane: Mask to determine whether to handle red, green and/or blue color.
   * line: Manipulating line.
   */

  int i, j, last, wplane;
  HistogramWidget hWid = (HistogramWidget) wid;
  HistogramPart *hPart;
  Window win;

  ENTER_FUNCTION_DEBUG("Histogram.c: Draw_lines");

  if (mask[0] == 0) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: Draw_lines");
    return;
  }

  hPart = &(hWid->histogram);

  if (!Has_changeable_map(XtDisplay(wid),
				DefaultScreen(XtDisplay(wid)),
				(hPart)->visual)) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: Draw_lines");
    return;
  }

  if (plane) {
    /* Not monochrome. */

    long pix;
    
    if (Has_changeable_map(XtDisplay(wid),
				 DefaultScreen(XtDisplay(wid)),
				 (hPart)->visual)) {
      /* Changeable colormap, which means that the whole colormap has been
       * allocated read/write, so a call to XAllocColor will not work.
       * However, certain specific colors have been allocated at known pixel
       * values.
       */
      
      pix = RGBPixel(200+plane, hPart->visual);
    
    } else {
      /* Immutable colormap. This means we're using the default colormap,
       * which is read/write, and a call to XAllocColor will work.
       */
      
      if (hPart->planes == 1) pix = BlackPixel(dpy, screen_s);
      else {
	char name[10];
	XColor rgb_def;
	
	if      (plane & DoRed)   (void) strcpy(name, "red");
	else if (plane & DoGreen) (void) strcpy(name, "green");
	else if (plane & DoBlue)  (void) strcpy(name, "blue");
	
	if (!XParseColor(dpy, hPart->colormap, name, &rgb_def)) {
	  FPRINTF2("    Color %s not in database.\n", name);
	} else {
	  FPRINTF4("    RGB values from database: %d, %d, %d\n",
		   rgb_def.red, rgb_def.green, rgb_def.blue);
	}
	
	if (!XAllocColor(dpy, hPart->colormap, &rgb_def)) {
	  FPRINTF1("    All colorcells allocated and read/write.\n");
	  pix = BlackPixel(dpy, screen_s);
	} else pix = rgb_def.pixel;
      }
    }
    
    XSetForeground(dpy, gc, pix);

    /* The change in pixel-value is limited to three bits (plane={1,2,4}).
     * Only process these three planes.
     */
    XSetPlaneMask(dpy, gc, RGBPixel(7, hPart->visual));
    XSetFunction(dpy, gc, func);
  } else {
    FPRINTF1("    Black and White.\n");
    XSetForeground(dpy, gc, BlackPixel(dpy, DefaultScreen(dpy)));
    XSetFunction(dpy, gc, GXinvert);
  }
  
  win    = XtWindow(wid);
  last   = 0;      /* Indicate the bin-number for the last point processed. */
  wplane = (plane == 0 ? DoRed : plane);
  
  /* Set primary components of colortable */
  for (i=1; i<256; i++) {
    
    /* Only draw line when a new point exists for the particular primary. */
    if (mask[i] & wplane) {
      
      if (last) XDrawPoint(dpy, win, gc, last, 255-line[last]);

      XDrawLine(dpy, win, gc, last, 255-line[last], i, 255-line[i]);

      if (wplane & DoRed) {
	/* Change red colortable component, according to the data for the
	   manipulating line. */
	for (j=last; j<=i; j++)
	  coltab[j/2].red = ((int) line[last]<<8) +
	    (((int) line[i] - (int) line[last]) << 8) *
	      (j-last) / (i-last);
      }

      if (wplane & DoGreen) {
	/* Change green colortable component. */
	for (j=last; j<=i; j++)
	  coltab[j/2].green = ((int) line[last]<<8) +
	    (((int) line[i] - (int) line[last]) << 8) *
	      (j-last) / (i-last);
      }

      if (wplane & DoBlue) {
	/* Change blue colortable component. */
	for (j=last; j<=i; j++)
	  coltab[j/2].blue = ((int) line[last]<<8) +
	    (((int) line[i] - (int) line[last]) << 8) *
	      (j-last) / (i-last);
      }

      last=i;
    } /* if (mask[i] & wplane) */
  } /* for i */

  XSetFunction(dpy, gc, GXcopy);
  if (plane) XSetPlaneMask(dpy, gc, AllPlanes);
  LEAVE_FUNCTION_DEBUG("Histogram.c: Draw_lines");
} 
        

void Draw_all_lines(Widget wid, int func)
{
  XColor coltab[128];
  Display *dpy;
  HistogramWidget hWid = (HistogramWidget) wid;
  HistogramPart *hPart;

  ENTER_FUNCTION_DEBUG("Histogram.c: Draw_all_lines");
  dpy = XtDisplay(wid);
 
  hPart = &(hWid->histogram);
  InitColortablePixels(coltab, 128, 64,
		       ClassOfVisual(hPart->visual) == DirectColor ||
		       ClassOfVisual(hPart->visual) == TrueColor);

  if (hPart->planes >= 7) {
    /* Assume not monochrome */
    FPRINTF1("    Assume not monochrome.\n");

    /* Fill into 'coltab', for the pixels listed in 'coltab', the rgb values of
       the histogram colormap. */
    XQueryColors(dpy, hPart->colormap, coltab, 128);

    /* Draw manipulating lines for red, green and blue component.
     * Use the hPart->red, hPart->green and hPart->blue as line data and let
     * the bit-pattern in hPart->mask determine whether or not to draw the
     * line for the particular primary.
     * Change each of the primary components in the colortable 'coltab'.
     */

    Draw_lines(dpy,wid,hPart->gc,coltab,hPart->mask,hPart->red,  DoRed,  func);
    Draw_lines(dpy,wid,hPart->gc,coltab,hPart->mask,hPart->green,DoGreen,func);
    Draw_lines(dpy,wid,hPart->gc,coltab,hPart->mask,hPart->blue, DoBlue, func);

    /* Visuals with read/write colormaps have odd numbered visual class */
    if (ClassOfVisual(hPart->visual) % 2 == 1) {
      FPRINTF1("    Read/write colormap.\n");

      XStoreColors(dpy, hPart->colormap, coltab, 128);
    } else {
      /* Readonly colormap */
      FPRINTF1("    Readonly colormap.\n");
    }
  } else {
    /* Assume monochrome */
    FPRINTF1("    Assume monochrome.\n");
    
    Draw_lines(dpy, wid, hPart->gc, coltab, hPart->mask, hPart->red, 0, func);
  }
  
  XFlush(dpy);
  LEAVE_FUNCTION_DEBUG("Histogram.c: Draw_all_lines");
}

static void Redisplay(Widget wid, XEvent *event, Region region)
{
  HistogramWidget hWid = (HistogramWidget) wid;
  HistogramPart *hPart;
  int i;
  Display *dpy;

  ENTER_FUNCTION_DEBUG("Histogram.c: Redisplay");

  dpy    = XtDisplay(wid);
  hPart  = &(hWid->histogram);
  XClearWindow(dpy, XtWindow(wid));

  if (Has_changeable_map(XtDisplay(wid),
			       DefaultScreen(XtDisplay(wid)),
			       (hPart)->visual)) {
    XSetForeground(dpy, hPart->gc, RGBPixel(200, hPart->visual));
  } else {
    long pix;
/*
    XColor rgb_def;

    if (!XParseColor(dpy, hPart->colormap, "black", &rgb_def)) {
      FPRINTF2("    Color %s not in database.\n", "black");
    } else {
      FPRINTF4("    RGB values from database: %d, %d, %d\n",
	       rgb_def.red, rgb_def.green, rgb_def.blue);
    }
*/

/*
    if (!XAllocColor(dpy, hPart->colormap, &rgb_def)) {
      FPRINTF1("    All colorcells allocated and read/write.\n");
      pix = BlackPixel(dpy, screen_s);
    } else pix = rgb_def.pixel;
*/

    pix = BlackPixel(dpy, screen_s);
    XSetForeground(dpy, hPart->gc, pix);
  }
  
  /* Draw histogram bars. */
  for (i=0; i<256; i++)
    XDrawLine(dpy, XtWindow(wid), hPart->gc, i, 255, i, 
      255-(hPart->korh ? hPart->dkumulativ[i] : hPart->dhistogram[i]));

  /* Draw manipulating lines. */
  Draw_all_lines(wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: Redisplay");
}  

static void Crossing(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  int enter = 1;
  Widget shell, form;
  Window win[2];

  ENTER_FUNCTION_DEBUG("Histogram.c: Crossing");

  if (*num_params > 0) {
    if (params[0][0] != 'E') enter = 0;
  } else if (event != NULL) {
    enter = event->xcrossing.type == EnterNotify ? 1 : 0;
  }

  if (XtHasCallbacks(wid, XtNhistogramCrossing) == XtCallbackHasSome)
    XtCallCallbacks(wid, XtNhistogramCrossing , &enter);

  if (!XtIsRealized(wid)) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: Crossing");
    return;
  }
 
  shell = TopLevelWidget(wid);
  form  = XtParent(wid);
 
  if (enter) {
    win[0] = XtWindow(wid);
    win[1] = XtWindow(shell);
  } else {
    win[0] = XtWindow(shell);
    win[1] = XtWindow(wid);
  }
 
  XSetWMColormapWindows(XtDisplay(wid), XtWindow(shell), win, 2);
 
  LEAVE_FUNCTION_DEBUG("Histogram.c: Crossing");
}



static void Button(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  HistogramWidget hWid = (HistogramWidget) wid;
  HistogramButtonCallbackRec button_callback;
  int x,y;
/*  FPRINTF7("Type=%6d,  Serial=%6d,  Time=%6d  ,State=%6d,\
          Button=%6d, SS=%2d\n", event->xbutton.type,
          event->xbutton.serial, event->xbutton.time,
          event->xbutton.state, event->xbutton.button,
          event->xbutton.same_screen); */

  ENTER_FUNCTION_DEBUG("Histogram.c: Button");

  if (event != NULL)
    switch(event->type) {
    case ButtonPress:
    case ButtonRelease:
      x = event->xbutton.x;
      y = event->xbutton.y;
      break;
    case MotionNotify:
      x = event->xmotion.x;
      y = event->xmotion.y;
      break;
    default:
      FPRINTF2("    Bad event %04x\n",event->type);
      LEAVE_FUNCTION_DEBUG("Histogram.c: Button");
      return;
    }

  button_callback.event     = event;
  button_callback.image     = hWid->histogram.image;
  button_callback.band      = hWid->histogram.band;
  button_callback.histogram = hWid->histogram.histogram;
  button_callback.kumulativ = hWid->histogram.kumulativ;
  button_callback.korh      = hWid->histogram.korh;
  button_callback.length    = hWid->histogram.length;
  button_callback.x         = x;
  button_callback.y         = 255-y;

  switch(event->type) {
  case ButtonPress:
  case ButtonRelease:
    XtCallCallbacks(wid, XtNhistogramButton, &button_callback);  
    break;
  case MotionNotify:
    XtCallCallbacks(wid, XtNhistogramMotion, &button_callback);  
    break;
  }

  LEAVE_FUNCTION_DEBUG("Histogram.c: Button");
} 



static void ChangeMode(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  HistogramWidget hWid = (HistogramWidget) wid;

  ENTER_FUNCTION_DEBUG("Histogram.c: ChangeMode");

  if  (event->type == ButtonRelease) {
    hWid->histogram.korh = 1 - hWid->histogram.korh;
    HistogramRedisplay(wid);
    LEAVE_FUNCTION_DEBUG("Histogram.c: ChangeMode");
    return;
  }

  LEAVE_FUNCTION_DEBUG("Histogram.c: ChangeMode");
} 



static void Kill(Widget wid)
{
  ENTER_FUNCTION_DEBUG("Histogram.c: Kill");

  XtUnmanageChild(wid);
  free(((HistogramWidget) wid)->histogram.kumulativ);
  free(((HistogramWidget) wid)->histogram.dhistogram);
  free(((HistogramWidget) wid)->histogram.dkumulativ);
  histogramClassRec.histogram_class.count--;

  LEAVE_FUNCTION_DEBUG("Histogram.c: Kill");
}

void HistogramKill(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  Widget w;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramKill");

  w = TopLevelWidget(wid);
  XtDestroyWidget(w);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramKill");
}



void HistogramInitLines(HistogramWidget wid, int col, int threshold)
{
  int i;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramInitLines");

  if (wid->histogram.mask[0] > 0) {
    /* This will not be called the very first time this procedure is called. */
    Draw_all_lines((Widget) wid, GXandInverted);
  }

  /* Give sufficient data-points to draw either a unit ramp or a threshold. */

  if (col & DoRed) {
    wid->histogram.red[0]   =   0;
    wid->histogram.red[127] =   0;
    wid->histogram.red[128] = 255;
    wid->histogram.red[255] = 255;
  }

  if (col & DoGreen) {
    wid->histogram.green[0]   =   0;
    wid->histogram.green[127] =   0;
    wid->histogram.green[128] = 255;
    wid->histogram.green[255] = 255;
  }

  if (col & DoBlue) {
    wid->histogram.blue[0]   =   0;
    wid->histogram.blue[127] =   0;
    wid->histogram.blue[128] = 255;
    wid->histogram.blue[255] = 255;
  }

  /* Clear the mask-bits of those primary components which where treated
   * above.
   */
  for (i=1; i<255; i++) wid->histogram.mask[i] &= (7-col);

  /* Set the mask-bits for the above treated primary components, for
   * histogram bin number 0 and number 255. These two bins are sufficient
   * to draw a manipulating unit ramp for linear stretching of the histogram.
   */
  wid->histogram.mask[0]     |= col;
  wid->histogram.mask[255]   |= col;

  /* Set mask to enable drawing of thresholding line, if necessary. */
  if (threshold == 1) {
    wid->histogram.mask[127] |= col;
    wid->histogram.mask[128] |= col;
  }

  /* Draw manipulating lines for all three primary color components. */
  Draw_all_lines((Widget) wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramInitLines");
}



void HistogramExitLines(HistogramWidget wid)
{
  int i;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramExitLines");

  if (wid->histogram.mask[0] > 0) {
    Draw_all_lines((Widget) wid, GXandInverted);

    /* Reset mask. */
    for(i=0; i<255; i++) wid->histogram.mask[i] = 0;
  }

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramExitLines");
}



void HistogramThreshold(HistogramWidget wid, int point, int col)
{
  int i;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramThreshold");

  if (wid->histogram.mask[0] == 0) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramThreshold");
    return;
  }
  if (point < 0 || point > 255) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramThreshold");
    return;
  }

  Draw_all_lines((Widget) wid, GXandInverted);

  /* 'point' is the bin number where the new threshold is to be. */

  if (col & DoRed) {
    for(i=0; i<=point; i++)     wid->histogram.red[i] =   0;
    for(i=point+1; i<=255; i++) wid->histogram.red[i] = 255;
  }

  if (col & DoGreen) {
    for(i=0; i<=point; i++)     wid->histogram.green[i] =   0;
    for(i=point+1; i<=255; i++) wid->histogram.green[i] = 255;
  }

  if (col & DoBlue) {
    for(i=0; i<=point; i++)     wid->histogram.blue[i] =   0;
    for(i=point+1; i<=255; i++) wid->histogram.blue[i] = 255;
  }

  /* Clear mask-bits for the above treated primary color components. */
  for (i=1; i<255; i++)         wid->histogram.mask[i] &= (7-col);

  /* Set mask-bits for the above treated primaries, for the new threshold
   * bin, since this needs to be drawn.
   */
  wid->histogram.mask[point] |= col; 

  /* The threshold goes from one bin to the next. */
  if (point<255) wid->histogram.mask[point+1] |= col; 

  /* Draw manipulating lines for all the three primary color components. */
  Draw_all_lines((Widget) wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramThreshold");
}



void HistogramInsertPoint(HistogramWidget wid, int point, int val, int col)
{
  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramInsertPoint");

  if (wid->histogram.mask[0] == 0) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramInsertPoint");
    return;
  }
  if (point < 0 || point > 255) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramInsertPoint");
    return;
  }

  /* 'point' is the bin-number for the new point.
   * 'val' is the height above the baseline for the point.
   */
  Draw_all_lines((Widget) wid, GXandInverted);

  if (col & DoRed)   wid->histogram.red[point]   = val;  
  if (col & DoGreen) wid->histogram.green[point] = val; 
  if (col & DoBlue)  wid->histogram.blue[point]  = val;

  /* Enable a line breakpoint at this bin. */
  wid->histogram.mask[point] |= col;

  /* Draw manipulating lines for all the three primary color components. */
  Draw_all_lines((Widget) wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramInsertPoint");
}



void HistogramMovePoint(HistogramWidget wid, int point, int val, int col)
{
  int i, x, d;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramMovePoint");

  d = 1000;
  if (wid->histogram.mask[0] == 0) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramMovePoint");
    return;
  }

  /* Find the existing line breakpoint (where mask-bit is set) closest to
   * the bin number given by 'point'.
   */
  for (i=0; i<256; i++) {
    if (wid->histogram.mask[i] & col)
      if ((i-point)*(i-point) < d * d) {
	d = (i-point);
	x = i;
      }
  }

  /* Consider bin number 'point' a miss for an existing line breakpoint
   * if it is more than 10 bins off.
   */
  if (d*d > 100) { 
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramMovePoint");
    return;
  }

  /* Clip the height value. */
  if (val < 0) val = 0; else if (val > 255) val = 255;

  Draw_all_lines((Widget) wid, GXandInverted);
  if (col & DoRed)   wid->histogram.red[x]   = val;  
  if (col & DoGreen) wid->histogram.green[x] = val; 
  if (col & DoBlue)  wid->histogram.blue[x]  = val;
  Draw_all_lines((Widget) wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramMovePoint");
}



void HistogramDeletePoint(HistogramWidget wid, int point, int val, int col)
{
  int dev;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramDeletePoint");

  dev = 0;
  if (wid->histogram.mask[0] == 0) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramDeletePoint");
    return;
  }
  if (point <= 0 || point >= 255) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramDeletePoint");
    return;
  }

  while((((point-dev<=0) || ((wid->histogram.mask[point-dev] & col) ==0))
	 && dev<16)
	&& (((point+dev>=255) || ((wid->histogram.mask[point+dev] & col) ==0))
	    && dev<16))
    dev++;
  if (dev == 16) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramDeletePoint");
    return;
  }
  if (point <= 0 || point >= 255) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramDeletePoint");
    return;
  }

  Draw_all_lines((Widget) wid, GXandInverted);
  if ((point-dev>0) && ((wid->histogram.mask[point-dev] & col) !=0))
    point = point-dev;
  if ((point+dev<255) && ((wid->histogram.mask[point+dev] & col) !=0))
    point = point+dev;
  wid->histogram.mask[point] &= 7-col;
  Draw_all_lines((Widget) wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramDeletePoint");
  
}



void HistogramInspect(HistogramWidget wid, XColor *colortab)
{
  int last, i, j;
  unsigned char *red, *green, *blue, *mask;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramInspect");

  mask = wid->histogram.mask;
  if (mask[0] == 0) {
    LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramInspect");
    return;
  }

  red   = wid->histogram.red;
  green = wid->histogram.green;
  blue  = wid->histogram.blue;

  InitColortablePixels(colortab, 256, 0,
		       ClassOfVisual(wid->histogram.visual) == DirectColor ||
		       ClassOfVisual(wid->histogram.visual) == TrueColor);

  colortab[0].red   = red[0];
  colortab[0].green = green[0];
  colortab[0].blue  = blue[0];

  last = 0;

  /* Go through each histogram bin.
   * Use the data for the manipulating line to change the colortable primary
   * accordingly. Update the colortable for each new linear stretch.
   */
  for (i=1; i<256; i++) {
    if (mask[i] & DoRed) {
      for (j=last; j<=i; j++)
	/* Starting from the 'last' point treated, increase or decrease
	 * an amount which is related to the line slope.
	 */
	colortab[j].red = ((int) red[last]<<8) +
	  (((int) red[i] - (int) red[last]) << 8) * (j-last) / (i-last);
      last = i;
    }
  }

  last = 0;
  for (i=1; i<256; i++) {
    if (mask[i] & DoGreen) {
      for (j=last; j<=i; j++)
	colortab[j].green = ((int) green[last]<<8) +
	  (((int) green[i] - (int) green[last]) << 8) * (j-last) / (i-last);
      last = i;
    }
  }

  last = 0;
  for (i=1; i<256; i++)
    if (mask[i] & DoBlue) {
      for (j=last; j<=i; j++)
	colortab[j].blue = ((int) blue[last]<<8) +
	  (((int) blue[i] - (int) blue[last]) << 8) * (j-last) / (i-last);
      last=i;
    }

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramInspect");
}



void HistogramGet(HistogramWidget wid, XColor *tab, int *len)
{
  int i, j;
  unsigned char *red, *green, *blue, *mask;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramGet");

  red   = wid->histogram.red;
  green = wid->histogram.green;
  blue  = wid->histogram.blue;
  mask  = wid->histogram.mask;

  j = 0;
  if (ClassOfVisual(wid->histogram.visual) == DirectColor || 
      ClassOfVisual(wid->histogram.visual) == TrueColor) {
    for (i=0; i<256; i++) {
      if (mask[i]) {
	tab[j].pixel = RgbPixel(i);
	tab[j].flags = mask[i];
	tab[j].red   = red[i];
	tab[j].green = green[i];
	tab[j].blue  = blue[i];
	tab[j].pad   = 0;
	j++;
      }
    }
  } else {
    for (i=0; i<256; i++) {
      if (mask[i]) {
	tab[j].pixel = i;
	tab[j].flags = mask[i];
	tab[j].red   = red[i];
	tab[j].green = green[i];
	tab[j].blue  = blue[i];
	tab[j].pad   = 0;
	j++;
      }
    }
  }
  *len = j;
  
  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramGet");
}



void HistogramSet(HistogramWidget wid, XColor *tab, int len)
{
  int i, j;
  unsigned char *red, *green, *blue, *mask;

  ENTER_FUNCTION_DEBUG("Histogram.c: HistogramSet");

  Draw_all_lines((Widget) wid, GXandInverted);
  red   = wid->histogram.red;
  green = wid->histogram.green;
  blue  = wid->histogram.blue;
  mask  = wid->histogram.mask;

  for (i=0; i<256; i++) mask[i] = 0;

  if (ClassOfVisual(wid->histogram.visual) == PseudoColor ||
      ClassOfVisual(wid->histogram.visual) == GrayScale ||
      ClassOfVisual(wid->histogram.visual) == StaticColor) {
    for (i=0; i<len; i++) {
      j = PseudoBasePart(tab[i].pixel);
      mask[j] |= tab[i].flags;
      if (tab[i].flags & DoRed)   red[j]   = tab[i].red;
      if (tab[i].flags & DoGreen) green[j] = tab[i].green;
      if (tab[i].flags & DoBlue)  blue[j]  = tab[i].blue;
    }
  } else {
    for (i=0; i<len; i++) {
      j = PseudoBasePart(tab[i].pixel);
      mask[j] |= tab[i].flags;

      if (tab[i].flags & DoRed) {
	j = RedBasePart(tab[i].pixel);
	red[j] = tab[i].red;
      }

      if (tab[i].flags & DoGreen) {
	j = GreenBasePart(tab[i].pixel);
	green[j] = tab[i].green;
      }

      if (tab[i].flags & DoBlue) {
	j = BlueBasePart(tab[i].pixel);
	blue[j] = tab[i].blue;
      }
    }
  }

  Draw_all_lines((Widget) wid, GXor);

  LEAVE_FUNCTION_DEBUG("Histogram.c: HistogramSet");
}
