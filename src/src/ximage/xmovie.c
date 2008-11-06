

/*

________________________________________________________________

        xmovie.c
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

static char *Id = "$Id$, Blab, UiO";



/*P:xmovie*

________________________________________________________________

		xmovie
________________________________________________________________

Name:		xmovie - X-based animation (movie) program for BIFF images
                in XITE

Syntax:		xmovie [<option>...] <inimage>...

Description:    Display a sequence of every band in BIFF images. A scrollbar
                is used to set the movie speed. Three-band rgb images may be
                mixed with single- or multiband pseudocolor images.

		The images can be zoomed and panned (with the usual mouse
		button and key combinations). The image window can also be
		resized with the window manager. Refer to 'Image(3)' for
		more information.

		To get maximum movie speed, refer to the options below.

Options:        'xmovie' accepts all X toolkit command-line options
                as well as the XITE 'ximage(3)' toolkit options. Refer to
		'ximage(3)' for a description of these options.

		Additional options available in 'xmovie':

		&-f
		If you don''t intend to resize or zoom/pan the image window or
		use the Histogram or Slice entries in the image''s Tools menu,
		this allows for simpler calculations of each movie frame and
		may enable a faster movie, probably at least double speed.

		This option implies the '-e' option.

		&-e
		Do not recalculate image extrema for each frame in the movie,
		instead assume fixed global extrema for the entire movie. This
		may enable a faster movie. Using this option, by default the
		assumption will be that the global extremas equal the extreme
		values for the current pixel type. (For pixel type unsigned
		byte this assumption is true even without the use of this
		option.)

		The extreme values are used when each image is scaled to fit
		the available dynamic range offered by the display.

		Option '-f' will enable an even faster movie, but with
		more limitations on how you can manipulate the image.

		See also options '-min' and '-max'.

		&-min min
		Assume that 'min' is the global minimum for all frames of
		the movie. This implies option '-e'.

		&-max max
		Assume that 'max' is the global maximum for all frames of
		the movie. Be careful not to choose 'max' smaller than
		the actual maximum, because pixel values exceeding 'max'
		will appear corrupted. This implies option '-e'.

		-&
		Also, one of the 'ximage' options deserves some comment:

		&-rgb
		Any three-band image command-line argument will be interpreted
		as an rgb image. In this way, 'xmovie' can include pseudocolor
		as well as rgb images in the same movie. For an rgb image
		to appear as a color image, you must use a DirectColor or
		TrueColor visual. Refer to the 'ximage' option '-iv'.

Restrictions:	All bands and images must be of equal size and of the same
                pixel type.

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'xmovie'.
                In the notation below, indentation indicates hierarchical 
                structure.

                The following hierarchy is constructed when the default
                resources/options for widget layout is used. The widgets
                of class XfwfButton can be replaced by class Command (from
		the Athena widget set) by using the 'ximage(3)' option
		'-widgetset' or corresponding application resource.

                Assuming that the default xshow_colortabs file is used.

                &xmovie              (class XMovie)
		| imageShell         (class TopLevelShell)
		|   imageForm        (class ImageForm)
		|     image          (class Image)
                |     menubar        (class XfwfMenuBar)
                |       colors       (class XfwfPullDown)
                |       tools        (class XfwfPullDown)
                |       options      (class XfwfPullDown)
		| xmovie control panel (class TopLevelShell)
		|   topContainer     (class Form)
		|     ximagescroll   (class Scrollbar)
		|     Frame 1        (class Label)
		|     Quit           (class XfwfButton)
		|     stop           (class XfwfButton)
		|     prev           (class XfwfButton)
		|     next           (class XfwfButton)
		|     series         (class XfwfButton)
		|     movie          (class XfwfButton)
                | colorsmenu         (class SimpleMenu)
                |   menuLabel        (class SmeBSB)
                |   backgroundcol    (class SmeBSB)
                |   Work-map         (class SmeBSB)
                |   colorsep         (class SmeLine)
                |   White - 256      (class SmeBSB)
                |   Black - 256      (class SmeBSB)
                |   Red - 256        (class SmeBSB)
                |   Green - 256      (class SmeBSB)
                |   Blue - 256       (class SmeBSB)
                |   Spectrum - 256   (class SmeBSB)
                |   Hue - 256        (class SmeBSB)
                |   Rainbow - 256    (class SmeBSB)
                | toolsmenu          (class SimpleMenu)
                |   menuLabel        (class SmeBSB)
                |   imageinfo        (class SmeBSB)
                |   histogram        (class SmeBSB)
                |   slice            (class SmeBSB)
                | optionsmenu        (class SimpleMenu)
                |   menuLabel        (class SmeBSB)
                |   optionsmenusep   (class SmeLine)
                |   optionsmenuentry (class SmeBSB)
                |   optionsmenuentry (class SmeBSB)
                |   optionsmenuentry (class SmeBSB)
                |   optionsmenusep   (class SmeLine)
                |   optionsmenuentry (class SmeBSB)
                |   optionsmenuentry (class SmeBSB)
                |   optionsmenuentry (class SmeBSB)

See also:       'ximage(3)', 'Image(3)'

Files:          $XITE_HOME/etc/app-defaults/XMovie

Author:		Otto Milvang and Svein Bøe
Doc:            Svein Bøe

Examples:	| xmovie img1 img2 img3
		| xmovie -xrm "*quit*borderWidth: 3" nbandimg.img:1-5

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <sys/time.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Scrollbar.h>
#include <xite/biff.h>
#include <xite/ImageP.h>
#include <xite/ImageFormat.h>
#include <xite/ximage.h>
#include <xite/Xcolor.h>
#include <xite/ShellWids.h>
#include <xite/Visual.h>
#include <xite/statistics.h>
#include <xite/utils.h>
#include <xite/draw.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/mallocp.h>
#include <xite/debug.h>

#ifdef MAIN

#define NONE -1

enum {
  XIMAGE,
  SCROLL,
  LABEL,
  QUIT,
  STOP,
  PREV,
  NEXT,
  SERIES,
  MOVIE
};

#define WIDGETS (MOVIE + 1)

static int work(void);

extern int rgb_e;
extern menu_button image_menus_e;

static ImageWidget image_s, *band_s;
static int bandmax_s, bandp_s, dir_s, stop_s, fixed_size_s, action_s;
static int noMinMax_s;
static unsigned long sleeptime_s;
static char *title_s = NULL;
static Display *dpy_s;

String widgets_s[] = {
  "I:imageShell",
  "S:ximagescroll:xmovie control panel",
  "L:Frame 1",
  "C:quit",
  "C:stop",
  "C:prev",
  "C:next",
  "C:series",
  "C:movie",
};

Widget rwidgets_s[WIDGETS];



static void quit_xmovie(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  Widget     w = NULL;
  char name[50];

  ENTER_FUNCTION_DEBUG("xmovie.c: quit_xmovie");

  (void) strcpy(name, "*");
  w = XtNameToWidget(wid, strcat(name, "quit"));
  XtCallCallbacks(w, XtNcallback, (XtPointer) MallocIntP(QUIT));

  LEAVE_FUNCTION_DEBUG("xmovie.c: quit_xmovie");

} /* quit_xmovie() */



/*L*

________________________________________________________________

		X-interface
________________________________________________________________

*/

#define start_workproc(wid)\
 XtAppAddWorkProc(XtWidgetToApplicationContext(wid), (XtWorkProc) work, NULL)

static void ximage_work(Widget wid, XtPointer client_data, XtPointer call_data)
{
  float f;

  ENTER_FUNCTION_DEBUG("xmovie.c: ximage_work");

  /* If stop_s == 1, it means that the work-procedure has been removed.
   * It can thus be started again.
   */

  switch(*((int *) client_data))
    {
    case SCROLL:
      f = *(float*) call_data;
      f = (1.0 - f) * 1000000.0;
      sleeptime_s = (unsigned long) f;
      break;
    case QUIT:
      ximage_exit(wid, (XtPointer) 0, (XtPointer) 0);
    case STOP:
      stop_s = 1;
      action_s = STOP;
      break;
    case PREV:
      dir_s = -1;
      action_s = PREV;
      if (stop_s == 1) start_workproc(wid);  
      stop_s = 1;
      break;
    case NEXT:
      dir_s = 1;
      action_s = NEXT;
      if (stop_s == 1) start_workproc(wid);
      stop_s = 1;
      break;
    case SERIES:
      dir_s = 1;
      action_s = SERIES;
      bandp_s = bandmax_s - 1; /* Next band shown will be band no zero. */
      if (stop_s == 1) start_workproc(wid);
      /* Run through the bands once or until some button which leads to
       * setting stop_s=1, is pushed. */
      stop_s = 0;
      break;
    case MOVIE:
      dir_s = 1;
      action_s = MOVIE;
      if (stop_s == 1) start_workproc(wid);
      /* Run the work-procedure until some button is pushed which alters
       * the value of stop_s. */
      stop_s = -1;
      break;
    }

  LEAVE_FUNCTION_DEBUG("xmovie.c: ximage_work");

} /* ximage_work() */


/*
static void MyZoom(wid, event, params, num_params)
ImageWidget wid;
XEvent *event;
String *params;
Cardinal *num_params;
{
  float zoom;
  int i, xpan, ypan;
  
  ImageGetZoom(wid, &zoom, &xpan, &ypan, NULL);

  if (*num_params <= 3)
    {
      ImageTranslateZoomPanArgs(wid, params[0], params[1], params[2],
			  ImageScreenXtoPos(wid, event->xbutton.x),
			  ImageScreenYtoPos(wid, event->xbutton.y),
			  &zoom, &xpan, &ypan);
    };

  for(i=0; i<bandmax_s; i++) ImageSetZoom(band_s[i], zoom, xpan, ypan); 
  ImageCopyImagePart(image_s, band_s[bandp_s]);
  ImageRedisplay(image_s, FALSE);
}
*/



static int work(void)
{
  char lab[100];
  Boolean change_image = FALSE, check_title = FALSE;

  ENTER_FUNCTION_DEBUG("xmovie.c: work");

  FPRINTF3("    stop_s=%d bandp_s=%d\n", stop_s, bandp_s);

  if (action_s != STOP) {
    bandp_s += dir_s;
    if (bandp_s == bandmax_s) bandp_s = 0;
    if (bandp_s == -1) bandp_s = bandmax_s-1;
  }

  if (action_s == NEXT || action_s == PREV) {
    change_image = 1;
    check_title  = 1;
    action_s     = NONE;
  } else if (action_s == SERIES) {
    change_image = 1;
    check_title  = 0;
    if (stop_s == 0 && bandp_s == bandmax_s - 1) stop_s = 1;
  } else if (action_s == MOVIE) {
    change_image = 1;
    check_title  = 0;
  } else if (action_s == STOP) {
    change_image = 0;
    check_title  = 1;
    action_s     = NONE;
  }

  if (check_title == 1) {
    char *title = NULL;
    int set_title = 0;

    XtVaGetValues((Widget) band_s[bandp_s], XtNimageTitle, &title, NULL);

    if (!title_s && title) {
      title_s   = XtNewString(title);
      set_title = 1;
    } else if (title_s && !title) {
      XtFree(title_s);
      title_s   = XtNewString("");
      set_title = 1;
    } else if (title_s && title && strcmp(title_s, title)) {
      XtFree(title_s);
      title_s   = XtNewString(title);
      set_title = 1;
    }

    if (set_title) {
      XmbSetWMProperties(dpy_s, XtWindow(TopLevelWidget((Widget) image_s)), 
			 title_s, title_s, NULL, 0, NULL, NULL, NULL);
    }

  } else if (title_s) {
    /* Clear window manager title if SERIES or MOVIE. */
    title_s = XtNewString("");
    XmbSetWMProperties(dpy_s, XtWindow(TopLevelWidget((Widget) image_s)), 
		       title_s, title_s, NULL, 0, NULL, NULL, NULL);
  }

  if (change_image) {
    Dimension width;

    XtVaGetValues(rwidgets_s[LABEL], XtNwidth, &width, NULL);
    sprintf(lab, "Frame %2d / %2d", bandp_s+1, bandmax_s);
    XtVaSetValues(rwidgets_s[LABEL],
		  XtNlabel, lab,
		  XtNwidth, width, NULL);

    if (fixed_size_s) {
      /* Use existing pixmap for this image. */
      image_s->image.ximg = (band_s[bandp_s])->image.ximg;
      /* Just expose, don't recalculate anything. */
      ImageRedisplay(image_s, UPDATE_NONE);

    } else {
      /* The first four assignments are not necessary for display only, but is
       * required if e.g. the histogram is to be calculated (from an image
       * widget popup menu).

       * Delay resizing/exposure until the ImageRedisplay call below.
       */
      XtVaSetValues((Widget) image_s,
		    XtNimageBand,      (band_s[bandp_s])->image.band,
		    XtNimageRedBand,   (band_s[bandp_s])->image.red_band,
		    XtNimageGreenBand, (band_s[bandp_s])->image.green_band,
		    XtNimageBlueBand,  (band_s[bandp_s])->image.blue_band,
		    XtNimageUpdate,    UPDATE_NONE,
		    NULL);

      if (!noMinMax_s) {
	ImageRedisplay(image_s, UPDATE_ZOOMPAN | UPDATE_MINMAX);
      }
      else {
	ImagePart *imPart = &(((ImageWidget) image_s)->image);

	imPart->minval = (band_s[bandp_s])->image.minval;
	imPart->maxval = (band_s[bandp_s])->image.maxval;
	ImageRedisplay(image_s, UPDATE_ZOOMPAN);
      } /* noMinMax_s */
    } /* !fixed_size_s */
  } /* change_image */

  if (sleeptime_s) xite_usleep(sleeptime_s);

  LEAVE_FUNCTION_DEBUG("xmovie.c: work");

  /* Remove work procedure if stop_s == 1. Rerun otherwise. */
  return (stop_s == 1);

} /* work() */


void main(int argc, char **argv)
{
  IMAGE img;
  IBAND b;
  int i, j, width, height, bandno, pointers_allocated = 0;
  float minval, maxval;
  Widget toplevel;
  IPIXTYP pt = Iunknown_typ;
  ImagePart *imPart;

  bandmax_s = bandp_s = width =  height = 0;

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv,
    ximage_app_usage_text(
     "Usage:  %s [<option>...] <inimage>...\n\
	An <option> may be\n\
	-f         : Assume fixed image size (no resizing/zooming/panning).\n\
                     This may enable faster movie.\n\
	-e         : Assume equal minima/maxima for all frames in movie.\n\
                     This may enable faster movie.\n\
        -min <min> : Assume this is minimum image value throughout movie.\n\
        -max <max> : Assume this is maximum image value throughout movie.\n"));

  /* 'toplevel' will be an application shell of class 'XMovie'. */
  toplevel = ximage_init(&argc, argv, "XMovie", NULL, True);

  fixed_size_s = read_bswitch(&argc, argv, "-f");
  noMinMax_s   = read_bswitch(&argc, argv, "-e");
  minval       = read_dswitch(&argc, argv, "-min", REAL_MAX);
  maxval       = read_dswitch(&argc, argv, "-max", REAL_MIN);

  if (minval != REAL_MAX || maxval != REAL_MIN) noMinMax_s = 1;

  if (argc < 2) Usage(2, NULL);

  AddAction(toplevel, "QuitXmovie", quit_xmovie, 0);

  /* Create parent for widgets which will hold all image bands, one each.
   * This is never popped up, so they only become visible one at a time
   * when being copied into the image widget in the rwidgets array.
   */

  dpy_s  = XtDisplay(toplevel);

  for (j=1; j<argc; j++) {
    img = Iread_image(argv[j]);
    if (j == 1) title_s = Ititle(img);
    if (img == NULL) {
      fprintf(stderr, "Can't find image: %s\n",argv[j]);
      exit(2);
    }

    if (width  == 0) width  = Ixsize(img[1]);
    if (height == 0) height = Iysize(img[1]);

    for(i = 1; i <= Inbands(img); i++) {
      if (bandmax_s == pointers_allocated) {
	band_s = (ImageWidget *) realloc((char *) band_s,
			 (bandmax_s + 256) * sizeof(ImageWidget *));
	if (!band_s)
	  Error(2, "Could not allocate memory for ImageWidget array.\n");

	pointers_allocated = bandmax_s + 256;
      }

      if (pt == Iunknown_typ) {
	pt = Ipixtyp(img[i]); /* Pixeltype of first band. */

	switch(pt) {
	case Iu_byte_typ:
	  if (minval == REAL_MAX) minval = 0.0;
	  if (maxval == REAL_MIN) maxval = (float) (UNS_BYTE_MAX + 1);
	  break;
	case Is_byte_typ:
	  if (minval == REAL_MAX) minval = (float) SIGN_BYTE_MIN;
	  if (maxval == REAL_MIN) maxval = (float) SIGN_BYTE_MAX;
	  break;
	case Iu_short_typ:
	  if (minval == REAL_MAX) minval = (float) 0;
	  if (maxval == REAL_MIN) maxval = (float) UNS_SHORT_MAX;
	  break;
	case Is_short_typ:
	  if (minval == REAL_MAX) minval = (float) SIGN_SHORT_MIN;
	  if (maxval == REAL_MIN) maxval = (float) SIGN_SHORT_MAX;
	  break;
	case Integer_typ:
	  if (minval == REAL_MAX) minval = (float) INTEGER_MIN;
	  if (maxval == REAL_MIN) maxval = (float) INTEGER_MAX;
	  break;
	case Ireal_typ:
	case Idouble_typ:
	case Icomplex_typ:
	case Id_complex_typ:
	  if (minval == REAL_MAX) minval = (float) REAL_MIN;
	  if (maxval == REAL_MIN) maxval = (float) REAL_MAX;
	  break;
	} /* switch(pt) */
      } /* First band. */
      
      if (pt != Ipixtyp(img[i]))
	Error(2, "All bands must have the same pixel type.\n");

      if (!rgb_e) bandno = i;
      else {
	if (Inbands(img) == 3) bandno = 0;
	else bandno = i;
      }

      /* Add image widget under the XMovie application class. */
      band_s[bandmax_s++] = (ImageWidget)
	ximage_display(toplevel,      /* parent */
		       NULL,          /* resource_name */
		       NULL,          /* shell_resource_name */
		       img,
		       bandno,
                       imageWidgetClass,
		       NULL,          /* visual */
		       0,             /* depth */
		       image_menus_e, /* buttons */
		       NULL,          /* args */
		       0,             /* num_args */
		       NULL           /* callbacks */
		       );

      imPart = &(((ImageWidget) band_s[bandmax_s - 1])->image);
      imPart->minval = minval;
      imPart->maxval = maxval;

      fprintf(stderr, "frame %2d OK\n", bandmax_s);
      if (bandno == 0) break;
    }
  }

  bandp_s  = bandmax_s - 1;
  dir_s    = 1;
  stop_s   = 1;
  action_s = NEXT;
  img      = Imake_image(1, title_s, pt, width, height);
  b        = img[1];
  (void) drawBand(b, 0.0, 0.0);

  /* Add widgets under the XMovie application class. */
  (void) ximage_layout(toplevel, b, widgets_s, rwidgets_s, WIDGETS,
		       ximage_work, NULL);
  image_s = (ImageWidget) rwidgets_s[XIMAGE];

  if (title_s)
    XmbSetWMProperties(dpy_s, XtWindow(TopLevelWidget((Widget) image_s)),
		       title_s, title_s, NULL, 0, NULL, NULL, NULL);

  XawScrollbarSetThumb(rwidgets_s[SCROLL], 0.95, 1.0);
  ImageSetZoomAll((Widget) image_s, 1);

  start_workproc(toplevel);
  ximage_mainloop();
}

#endif /* MAIN */
