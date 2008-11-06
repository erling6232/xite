
/*

________________________________________________________________

        Xhistogram.c
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



#include <xite/includes.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <xite/biff.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <xite/Xhistogram.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <Xfwf/MultiList.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xutil.h>
#include <xite/color.h>
#include <xite/ImageOverlay.h>
#include <xite/ImageP.h> /* For pixtype */
#include <xite/Xslice.h>
#include <xite/histo.h>
#include "Histogram.h"
#include <xite/message.h>
#include <xite/statistics.h>
#include <xite/Visual.h>
#include <xite/Cmap.h>
#include <xite/ShellWids.h>
#include <xite/ximage.h>
#include <xite/ximage_display.h>
#include <xite/Xcolor.h>
#include <xite/Xdialog.h>
#include <xite/readarg.h>
#include "Colormap.h"
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/debug.h>

extern Widget   Control_e;

#ifndef MAIN

#ifndef MAX
# define MAX(a,b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) ((a) <= (b) ? (a) : (b))
#endif

typedef enum {
  GRAY_MODE,
  RED_MODE,
  GREEN_MODE,
  BLUE_MODE
} color_mode;

typedef enum {
  PIECEWISE_EXIT,
  THRESHOLD_EXIT,
  LINEAR_EXIT,
  LOG_EXIT,
  EXP_EXIT,
  RESET_EXIT,
  HISTOEQ_EXIT,
  SENDPAR_EXIT,
  SENDCOL_EXIT, 
  SENDIMAGE_EXIT,
  QUIT_EXIT
} action_mode;

#define CHEIGHT 70     /* Height of colormap image, in pixels */
#define CWIDTH  256    /* Width of colormap image, in pixels */
#define HISTWIDTH 256  /* Width of histogram widget, in pixels */
#define HISTHEIGHT 256 /* Width of histogram widget, in pixels */



/*
________________________________________________________________

		Global data structure
________________________________________________________________

*/

extern Atom        wm_delete_window;   /* for ICCCM delete window */
extern menu_button image_menus_e;
extern Display     *dpy_e;

typedef struct 
{
  Widget Image_wid, Histogram_wid, Label_wid, ListExits_wid;
  int gplanes;
  IMAGE img;
  IBAND band;
  int col;       /* Mask for colors (7: red, green and blue). */
  action_mode hspes;     /* Action number chosen in histogram widget. */
  int x, y;
  double *fhist; /* Pointer to fractional histogram values */
  int *hist,     /* Pointer to histogram values */
      *khist;    /* Pointer to cumulative histogram values */
  Dimension labelWidth;
  Visual *visual;
  image_callbacks *callbacks;
} private_data;

/* Global variable to store the current display function to be used to
   send a histogram-manipulated image back to xshow as a new image widget */

static ximage_display_func dispfunc_s;

/* Upper menu entries */
static char *modes[] =
{
  "Gray",
  "Red",
  "Green",
  "Blue",
};

/* Lower menu entries */

static char *exits[] =
{
  "Piecewise linear",
  "Threshold",
  "Linear",
  "Logarithmic",
  "Exponential",
  "Reset",
  "Histogram EQ",
  "Send param.",  
  "Send colortab",
  "Send image",
  "Quit",
};



/*L*

________________________________________________________________

		Layout of histogram application
________________________________________________________________



                   
      +-----------------------------------------------+
      !                Label                          !
      +---------------------------------+-------------+
      !                                 !             !
      !                                 !             ! +--------------+
      !                                 !             ! !  Label 1     !
      !                                 !             ! +--------------+
      !                                 !             ! !              !
      !                                 !             ! !  List 1      !
      !                                 !             ! !              !
      !                                 !             ! +--------------+
      !           Histogram             !  Dialog     ->!              !
      !                                 !             ! +--------------+
      !                                 !             ! !  Label 2     !
      !                                 !             ! +--------------+
      !                                 !             ! !              !
      !                                 !             ! !  List 2      !
      !                                 !             ! !              !
      +---------------------------------+             ! +--------------+
      !           Colortab              !             !
      +---------------------------------+-------------+

*/

static char LabelText[81] = "Histogram";
static char Label1Text[81] = "Mode";
static char Label2Text[81] = "Actions";

static Boolean sens_modes_all[4] =
{
  TRUE, TRUE, TRUE, TRUE
};

static Boolean sens_exits_all[11] =
{
  TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE
};

static Boolean sens_modes_none[4] =
{
  FALSE, FALSE, FALSE, FALSE
};

static Boolean sens_exits_none[11] =
{
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE
};

static Boolean sens_exits_col[11] =
{
  TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE
};

static Boolean sens_exits_main[11] =
{
  TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE
};

static Boolean sens_exits_main_out[11] =
{
  TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE
};



static void linear(private_data *data, Widget wid, int xp, int yp)
{
  XColor coltab[520], *c;
  int i, j, len, x, col;
  double a, b;

  HistogramGet((HistogramWidget) wid, coltab, &len);
  j       = 0;
  data->x = xp;   /* Coordinates of mouse pointer in histogram window */
  data->y = yp;
  col     = data->col;
  for (i=0; i<len; i++) {
    coltab[i].flags &= ~col;  /* Clear bits corresponding to col mask */
    if (coltab[i].flags) coltab[j++] = coltab[i];
  }
  len = j;

  /* Determine parameters for the manipulated line in the histogram window */

  /* Line inclination, squared to make more rapid effect */
  a = (128.0 - (double) yp) / 32.0;
  a = a > 0 ? a*a : - a*a;

  /* Line elevation/crossing */
  b = 128.0 - a * (double) xp;

  c = &coltab[len];
  len ++;
  c->pixel = RGBPixel(0, data->visual);
  c->flags = col;

  if (b >= 0.0 && b <= 255.0) {
    c->red = c->green = c->blue = b;
  } else {
    len++;
    if (a > 0) {
      c->red   = c->green = c->blue = 0;
      c++;
      x        = -b/a;
      c->pixel = RGBPixel(x, data->visual);
      c->flags = col;
      c->red   = c->green = c->blue = 0;
    } else {
      c->red   = c->green = c->blue = 255;
      c++;
      x        = (255.0-b)/a;
      c->pixel = RGBPixel(x, data->visual);
      c->flags = col;
      c->red   = c->green = c->blue = 255;
    }
  }
  c++; len++;
  if (a*255.0+b >= 0.0 && a*255.0+b <= 255.0) {
    c->pixel = RGBPixel(255, data->visual);
    c->flags = col;
    c->red   = c->green = c->blue = a*255.0+b;
  } else {
    len++;
    if (a > 0) {
      c->red   = c->green = c->blue = 255;
      x        = (255.0 - b) / a;
      c->pixel = RGBPixel(x, data->visual);
      c->flags = col;
      c->red   = c->green = c->blue = 255;
    } else {
      c->red   = c->green = c->blue = 0;
      x        = -b/a;
      c->pixel = RGBPixel(x, data->visual);
      c->flags = col;
      c->red   = c->green = c->blue = 0;
    }
    c++;
    c->pixel = RGBPixel(255, data->visual);
    c->flags = col;
    c->red   = c->green = c->blue = (a > 0 ? 255 : 0);
  }

  HistogramSet((HistogramWidget) wid, coltab, len);

} /* linear() */



static void logarithmic(private_data *data, Widget wid, int x)
{
  XColor coltab[520], *c;
  int i, j, len, col;
  double a, b;

  col     = data->col;
  data->x = x;
  HistogramGet((HistogramWidget) wid, coltab, &len);
  j = 0;
  for (i=0; i<len; i++)
    {
      coltab[i].flags &= ~col;
      if (coltab[i].flags) coltab[j++] = coltab[i];
    }
  len = j;
            
  a = ((double) x) / 40.0;
  a = a*a;
  b = 255.0 / log(a * 255.0 + 1.0);
  for (i = len; i< 256+len; i += 5) {
    c        = &coltab[j++];
    c->pixel = RGBPixel(i-len, data->visual);
    c->flags = col;
    c->red   = c->green = c->blue = 
/*        	           b * log(a*(double)(i-len)+1.0); */
      256.0/a * log(1.0 + (exp(a) - 1.0) * (double) (i-len) / 256.0);
  }
  len += 52;

  HistogramSet((HistogramWidget) wid, coltab, len);

} /* logarithmic() */



static void expon(private_data *data, Widget wid, int x)
{
  XColor coltab[520], *c;
  int i, j, len, col;
  double a, b;

  data->x = x;
  col     = data->col;
  HistogramGet((HistogramWidget) wid, coltab, &len);
  j = 0;
  for (i=0; i<len; i++) {
    coltab[i].flags &= ~col;
    if (coltab[i].flags) coltab[j++] = coltab[i];
  }
  len = j;
            
  a = ((double) x) / 32.0;
  a = a*a / 256.0;
  b = exp(log(255.0) - a * 255.0);

  for (i=0; i<5; i++)
    b = b - (b * (exp(a * 255.0) - 1.0) - 255.0)/ (exp(a * 255.0) - 1.0);

  for (i = len; i< 256+len;i+=5) {
    c        = &coltab[j++];
    c->pixel = RGBPixel(i-len, data->visual);
    c->flags = col;
    c->red   = c->green = c->blue = b * exp(a*(double)(i-len)) - b;
  }
  len += 52;

  HistogramSet((HistogramWidget) wid, coltab, len);

} /* expon() */



static void Histoeq(private_data *data, int norm)
{
  int b;
  IMAGE img, img2;
  ImageWidget iw;

  if (!dispfunc_s) {
    Warning(2, "No display function installed\n");
    return;
  }
  img  = data -> img;
  img2 = Icopy_init(img);
  iw   = (ImageWidget)data->Image_wid;
  for (b = 1; b<= Inbands(img); b++) {
    if (norm)
      histoNorm(img[b], img2[b], 128.0, 50.0);
    else
      histoEq(img[b], img2[b], 256);
  }

  if (norm)
    Ihistory(img2, "histoNorm", " with my = 128.00 and sigma = 50.00");
  else
    Ihistory(img2, "histoEq",   " with 256 greylevels");

  if (data->Image_wid) {
    Widget wid;

    wid = (dispfunc_s)(Control_e, XtName((Widget) iw), NULL, img2, 0,
		       XtClass((Widget) iw), NULL, 0, image_menus_e, NULL, 0,
		       data->callbacks);
    XtPopup(TopLevelWidget(wid), XtGrabNone);
  }
} /* Histoeq() */



static void SendParam(private_data *data)
{
  ImageWidget wid;
  float a, b, scalea, scaleb, old_scalea = 1.0, old_scaleb = 0.0;
  int *iscalea = (int *) &scalea, *iscaleb = (int *) &scaleb;
 
  ENTER_FUNCTION_DEBUG("Xhistogram.c: SendParam");

  if (data->hspes != LINEAR_EXIT) {
    Warning(1, "Send parameters requires linear mode.\n");
    LEAVE_FUNCTION_DEBUG("Xhistogram.c: SendParam");
    return;
  }

  a = (128.0 - (float) data->y) / 32.0;
  a = a > 0 ? a*a : - a*a;
  b = 128.0 - a * (float) data->x;

  wid = (ImageWidget) data->Image_wid;

  if (wid == NULL) return;

  XtVaGetValues((Widget) wid, XtNscalea, &old_scalea,
		XtNscaleb, &old_scaleb, NULL);

  /* Linear transformation on a possibly previous linear transformation:
     y = a1*x + b1
     z = a2*y + b2 = a2*(a1*x + b1) + b2 = a1*a2*x + a2*b1 + b2
  */
  scalea = old_scalea * a;
  scaleb = a * old_scaleb + b;

  XtVaSetValues((Widget) wid, XtNscalea, (XtArgVal) *iscalea,
		XtNscaleb, (XtArgVal) *iscaleb, NULL);

  HistogramInitLines((HistogramWidget) data->Histogram_wid,
		     data->col, data->hspes);

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: SendParam");

} /* SendParam() */



static void SendColtab(private_data *data)
{
  XColor tab[256];
  int col;
  char *filename = NULL;
  int button;
 
  ENTER_FUNCTION_DEBUG("Xhistogram.c: SendColtab");

  col = data->col;
  HistogramInspect((HistogramWidget) data->Histogram_wid, tab);

  if (data->Image_wid) {
    if ((button = ConfirmationBox(TopLevelWidget(data->Histogram_wid),
				  "Send to program or file?",
				  "Program", "File", "histogram - dialog",
				  "histo - dialog")) == 1) {
      BiffStatus status;

      if (SelectWriteFile(TopLevelWidget(data->Histogram_wid), writeColortab,
			  &filename) == 0)
	return;

      status = color_write(filename, (Color_cell *) tab, "Histogram", 256);
      if (status != Iok) 
	InformationBox(data->Histogram_wid, Ierr_message(status),
		       "xhistogram - information", "xhistogram - info");
      else
	Message(0, "Colortab saved as %s\n",filename);
    } else if (button == 0) {
      Color_add(tab, "Histogram", 256);
      Message(0, "Colortab added to image color menu.\n");
    }

    /* button == -1: Dialog killed by window manager. Do nothing. */
  }

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: SendColtab");

} /* SendColtab() */



static void SendImage(private_data *data)
{
  XColor tab[256];
  int i, b, x, y, xsize, ysize, col;
  IBAND band;
  IMAGE img, img2;
  unsigned char *fptr, *tptr;
  int trans[256];
  char buff[200], *pixname;
  double a;
  ImageWidget iw;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: SendImage");
 
  if (!dispfunc_s) {
    Warning(2, "No display function installed\n");
    LEAVE_FUNCTION_DEBUG("Xhistogram.c: SendImage");
    return;
  }

  iw = (ImageWidget)data->Image_wid;
  if (iw == NULL) return;

  XtVaGetValues((Widget) iw, XtNimagePixtype, &pixname, NULL);
  if (strcmp(pixname, ImageBytePixel)) {
    Message(0, "%s%s\n",
	    "This function can only be used for images with unsigned byte",
	    " pixels.");
    LEAVE_FUNCTION_DEBUG("Xhistogram.c: SendImage");
    return;
  }

  col = data->col;
  img = data->img;

  HistogramInspect((HistogramWidget) data->Histogram_wid, tab);

  switch(col)
    {
    case 0:
    case DoRed:
    case (DoRed + DoGreen):
    case (DoRed + DoBlue):
    case (DoRed + DoGreen + DoBlue):
      for (i=0; i<256; i++) trans[i] = (tab[i].red   >> 8);
      break;
    case DoGreen:
    case (DoGreen + DoBlue):
      for (i=0; i<256; i++) trans[i] = (tab[i].green >> 8);
      break;
    case DoBlue:
      for (i=0; i<256; i++) trans[i] = (tab[i].blue  >> 8);
      break;
    }

  img2 = Icopy_init(img);
  for (b = 1; b<= Inbands(img); b++) {
    band  = img[b];
    xsize = Ixsize(band);
    ysize = Iysize(band);
    for (y=1; y<=ysize; y++) {
      fptr = &band[y][1];
      tptr = &img2[b][y][1];
      for(x=1; x<=xsize; x++)
	*tptr++ = trans[*fptr++];
    }
  }

  switch(data->hspes)
    {
    case THRESHOLD_EXIT: 
      sprintf(buff, " t = %d", data->x);
      Ihistory(img2, "threshold", buff);
      break;
    case LINEAR_EXIT: 
      a = (128.0 - (double) data->y) / 32.0;
      a = a > 0 ? a*a : - a*a;
      sprintf(buff, " a = %.2f, b = %.2f", a, 128.0 - a * (double) data->x );
      Ihistory(img2, "histoLinear", buff);
      break;
    default:
      Ihistory(img2, "HistoSpecify","");
      break;
    }

  if (data->Image_wid) {
    Widget wid;

    wid = (dispfunc_s)(Control_e, XtName((Widget) iw), NULL, img2, 0,
		       XtClass((Widget) iw), NULL, 0, image_menus_e, NULL, 0,
		       data->callbacks);
    XtPopup(TopLevelWidget(wid), XtGrabNone);
  }

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: SendImage");

} /* SendImage() */



static void Button(Widget wid, XtPointer p_data, XtPointer btn)
{
  int bt, col;
  float th;
  private_data *data;
  HistogramButtonCallback button;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: Button");

  data   = (private_data *) p_data;
  button = (HistogramButtonCallback) btn;

  if (data == NULL) return;

  col = data->col;
  if (button->event->type == ButtonPress   ||
      button->event->type == ButtonRelease || 
      button->event->type == MotionNotify) {

    if (button->event->type == ButtonPress ||
        button->event->type == ButtonRelease) 
      bt = button->event->xbutton.button;
    else bt = 1;

    switch(bt)
      {
      case 1:
      case 2:
        switch (data->hspes)
	  {
	  case PIECEWISE_EXIT:
            if (button->event->type == ButtonPress && bt == 1)
	      HistogramInsertPoint((HistogramWidget) wid, button->x,
				   button->y, col);
	    else
	      HistogramMovePoint((HistogramWidget) wid, button->x,
				 button->y, col);
	    break;
	  case THRESHOLD_EXIT:
	    data->x = button->x;
	    HistogramThreshold((HistogramWidget) wid, button->x, col);
            if (button->event->type == ButtonRelease 
		&& button->x >=0 && button->x < 256) {
	      th = button->x;

	      if (data->Image_wid && data->gplanes == 1) {
		int *iscalea = (int *) &th;

		XtVaSetValues((Widget) data->Image_wid,
			      XtNscalea,    (XtArgVal) *iscalea,
			      XtNscaletype, SCALE_THRESHOLD,
			      NULL);
	      }
	    }
	    break;
	  case LINEAR_EXIT:
	    linear(data, wid, button->x, button->y);
	    break;
	  case LOG_EXIT:
	    logarithmic(data, wid, button->x);
	    break;
	  case EXP_EXIT:
	    expon(data, wid, button->x);
	    break;
	  default:
	    break;
	  }
        break;
      case 3:
        if (data->hspes == PIECEWISE_EXIT &&
	    button->event->type == ButtonRelease)
          HistogramDeletePoint((HistogramWidget) wid, button->x, button->y,
			       col);
        break;
      default:
	fprintf(stderr, "Unknown detail mask %d \n", 
           button->event->xbutton.button);
        break;  
      }
  }	
  LEAVE_FUNCTION_DEBUG("Xhistogram.c: Button");

} /* Button() */



static void Motion(Widget wid, XtPointer p_data, XtPointer btn)
{
  private_data *data;
  HistogramButtonCallback button;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: Motion");

  data   = (private_data *) p_data;
  button = (HistogramButtonCallback) btn;
  
  if (data == NULL) {
    LEAVE_FUNCTION_DEBUG("Xhistogram.c: Motion");
    return;
  }

  if (button->x < 0 || button->x > 255) {
    LEAVE_FUNCTION_DEBUG("Xhistogram.c: Motion");
    return;
  }
  if (button->korh)
    sprintf(LabelText, "Cum[%d] = %d/%d, (%5.2f %%)",
	  button->x, data->khist[button->x], data->khist[button->length-1],
          data->fhist[button->x]); 
  else
    sprintf(LabelText, "Hist[%d] = %d/%d, (%5.2f %%)",
	  button->x, data->hist[button->x], data->khist[button->length-1],
          100.0 * data->hist[button->x] / data->khist[button->length-1]);

  XtVaSetValues(data->Label_wid, XtNlabel, LabelText, 
		XtNwidth, data->labelWidth, NULL);

  if (((button->event)->xmotion.state & 
       (Button1Mask | Button2Mask | Button1Mask)) && data -> gplanes > 1) 
         Button(wid, (XtPointer) data, (XtPointer)button);

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: Motion");

} /* Motion() */



static void Modes(Widget wid, XtPointer p_data, XtPointer lst)
{
  private_data *data;
  XfwfMultiListReturnStruct *list;
  static int trans[4] = {DoRed+DoGreen+DoBlue,DoRed,DoGreen,DoBlue};

  ENTER_FUNCTION_DEBUG("Xhistogram.c: Modes");
  data = (private_data *) p_data;
  list = (XfwfMultiListReturnStruct *) lst;

  if (data == NULL) return;
  if (list->num_selected == 0) return;

  /* Translate from list index to color mask. */
  data->col = trans[list->selected_items[0]];

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: Modes");

} /* Modes() */




static void Exits(Widget wid, XtPointer p_data, XtPointer lst)
{
  ipixelptr pptr;
  ImageWidget iw;
  XfwfMultiListReturnStruct *list;
  private_data *data;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: Exits");
  data = (private_data *) p_data;
  list = (XfwfMultiListReturnStruct *) lst;
 
  if (data == NULL) return;

  switch(list->selected_items[0])
    {
    case PIECEWISE_EXIT:
    case THRESHOLD_EXIT:
    case LINEAR_EXIT:
    case LOG_EXIT:
    case EXP_EXIT:
      data->hspes = (action_mode) list->selected_items[0];
    case RESET_EXIT: {
      float scalea, scaleb;

      HistogramInitLines((HistogramWidget) data->Histogram_wid, data->col,
			 data->hspes);

      if (list->selected_items[0] != RESET_EXIT) break;

      iw = (ImageWidget)data->Image_wid;
      if (iw == NULL) break;

      pptr = iw->image.pixtype;
      XtVaGetValues((Widget) iw, XtNscalea, &scalea,
		                 XtNscaleb, &scaleb, NULL);

      if (scalea != pptr->scalea || 
          scaleb != pptr->scaleb) {
	int *iscalea = (int *) &(pptr->scalea),
	    *iscaleb = (int *) &(pptr->scaleb);

	XtVaSetValues((Widget) iw, XtNscalea, (XtArgVal) *iscalea,
		                   XtNscaleb, (XtArgVal) *iscaleb, NULL);
      }
    }
      break;
    case HISTOEQ_EXIT:
      Histoeq(data, FALSE);
      break;
    case SENDPAR_EXIT:
      SendParam(data);
      break;
    case SENDCOL_EXIT:
      SendColtab(data);
      break;
    case SENDIMAGE_EXIT:
      SendImage(data);
      break;
    case QUIT_EXIT:
      HistogramKill(wid, NULL, NULL, NULL);
      break;
    }

  if (list->selected_items[0] < LINEAR_EXIT)
      data->hspes = (action_mode) list->selected_items[0];

  XfwfMultiListHighlightItem((XfwfMultiListWidget) data->ListExits_wid,
			     data->hspes);

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: Exits");

} /* Exits() */




static void Kill(Widget wid, XtPointer p_data, XtPointer ant)
{
  private_data *data;
  Boolean do_exit = False;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: Kill");

  data = (private_data *) p_data;
  if (data == NULL) return;

  if (!data->Image_wid) do_exit = True;

  free(data->hist);  data->hist  = NULL;
  free(data->khist); data->khist = NULL;
  free(data->fhist); data->fhist = NULL;
  free(data);
  data = NULL;

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: Kill");

  if (do_exit) exit(0);

} /* Kill() */



/*F:xhistogram*

________________________________________________________________

		xhistogram
________________________________________________________________

Name:		xhistogram - show the histogram of an image.
Syntax:         | #include <xite/Xhistogram.h>
                | 
                | void xhistogram( Widget wid, IMAGE image,
		|    IBAND band, Colormap map, int planes,
		|    ximage_display_func disp,
		|    image_callbacks *callbacks );

Description:    'xhistogram' displays a histogram widget.

		'wid' refers to the widget which contains the image for which
		the histogram should be displayed. It is used to determine
		certain characteristics of the visual and display and whether
		the image is a three-band rgb image. For a three-band rgb
		image, the histogram will be global for all three bands. If the
		image is not displayed in a widget, 'wid' may be NULL, and
		the default characteristics for the display are used. Also,
		in this latter case, some of the command buttons are not
		sensitive.

		'image' supplies the IMAGE data structure in case 'wid' is
		NULL. If it refers to a three-band image, it is assumed to be
		an rgb image, and a global histogram for the three bands will
		be displayed.

		'band' supplies the image data if 'wid' is NULL and only a
		specific band should be used for the histogram.

		'map' is the colormap to be used for the histogram widget.
		If NULL, use the XITE-initial colormap for the visual given
		by 'wid' (or the default visual if 'wid' is NULL) and 'planes'
		(or the planes corresponding to the visual).
		
                'disp' supplies a function which is called when the "Send
                image"- or "Histoeq"-button is pressed, to send a
                histogram-manipulated image back to xshow as a new image
                widget. This must be of type 'ximage_display_func'. Usually,
		it will be the function 'ximage_display'.

		'callbacks' is the callbacks argument passed along as the
		last argument in the call to 'disp'.

		'xhistogram' calls 'mkHisto(3)' for an unsigned byte band and
		'mkDhistogram(3)' for other pixel types.

		A copy of the currently active colortable is added to the
		Colors menu of 'wid'. The reduced-color version in the
		pixel range [64..191] is changed to gray-scale, and this
		colortable becomes the active one for the histogram window, as
		well as for the image window 'wid'.

		A special set of colors is installed in some entries outside
		the pixel range [64..191] of the colormap (in the overlay
		color entries). Some of these are used to give the correct
		colors to the primary manipulating lines available when a
		non-Gray Mode is chosen. By insisting on a visible overlay on
		top of the image, the overlay colors may therefore be
		incorrect.

Return value:   None
See also:       xhistogram(1), mkHisto(3), mkDhistogram(3), ximage_display(3)
Author:		Otto Milvang
Revised:        Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/

static void CopyToPrivate(Widget wid, IMAGE image, IBAND band, int planes, Visual *visual, image_callbacks *callbacks, private_data **private)
{

  ENTER_FUNCTION_DEBUG("Xhistogram.c: CopyToPrivate");
  
  *private = (private_data*) malloc(sizeof(private_data));
  (*private) -> Image_wid  = wid;
  (*private) -> img        = image;
  (*private) -> band       = band;
  (*private) -> gplanes    = planes;
  (*private) -> hist       = (int *)    malloc(256 * sizeof(int));
  (*private) -> khist      = (int *)    malloc(256 * sizeof(int));
  (*private) -> fhist      = (double *) malloc(256 * sizeof(double));
  (*private) -> col        = 7;
  (*private) -> labelWidth = 0;
  (*private) -> visual     = visual;
  (*private) -> callbacks  = callbacks;

  /* Initial action chosen is piecewise linear */
  (*private) -> hspes      = PIECEWISE_EXIT;

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: CopyToPrivate");
  
} /* CopyToPrivate */


static void CalcHistogram(private_data *private)
{
  Boolean multi = False, rgb = False;
  IBAND band, *bands = NULL;
  int nbands = 1;
  IPIXTYP pt;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: CalcHistogram");

  if (private->Image_wid) {
    /* xhistogram() called from other program, from Image menu. */

    XtVaGetValues(private->Image_wid, XtNimageRgb, &rgb, NULL);
    if (rgb) {
      multi  = True;
      nbands = 3;
    }
  }

  if (!multi && !private->band && ((nbands = Inbands(private->img)) > 1)) {
    /* xhistogram() probably called from xhistogram main program.
     * No Image widget present. */
    multi = True;
  }

  if (private->band) band = private->band;
  else               band = private->img[1];

  if (multi) {
    /* Make a band reference array, to avoid needing to test whether
     * to use widget bands or img bands further down. */
    bands = (IBAND *) calloc((nbands + 1), sizeof(IBAND));

    if (rgb && private->Image_wid) {
      Arg args[3];

      args[0].name = XtNimageRedBand;
      args[0].value= (XtArgVal) 0;
      args[1].name = XtNimageGreenBand;
      args[1].value= (XtArgVal) 0;
      args[2].name = XtNimageBlueBand;
      args[2].value= (XtArgVal) 0;

      XtGetValues(private->Image_wid, args, XtNumber(args));

      bands[1] = (IBAND) args[0].value;
      bands[2] = (IBAND) args[1].value;
      bands[3] = (IBAND) args[2].value;
    } else {
      int i;

      for (i = 1; i <= nbands; i++) {
	bands[i] = private->img[i];
      }
    }

    pt = Ipixtyp(bands[1]);
  } else pt = Ipixtyp(band);

  /* Calculate histogram */

  if (!multi && Ipixtyp(band) == Iu_byte_typ) {
    mkHisto(band, private->hist);

    LEAVE_FUNCTION_DEBUG("Xhistogram.c: CalcHistogram");
    return;
  }

  if (!multi) {
    /* Not Iu_byte_typ. */
    int i;
    double limits[256], minval, maxval;

    if (band_minmax(band, &minval, &maxval)) return;
    if (minval == maxval) maxval += 1.0;
    
    for (i=0; i<=254; i++)
      limits[i] = (maxval - minval) * (i+1) / 255.0 + minval;
    
    mkDhistogram(band, private->hist, limits, 255);

    LEAVE_FUNCTION_DEBUG("Xhistogram.c: CalcHistogram");
    return;
  }

  if (pt == Iu_byte_typ) {
    /* Multi */
    int *hist, i, j;

    for (i = 0; i < 256; i++) private->hist[i] = 0;
    hist = (int *) malloc(256 * sizeof(int));

    for (i = 1; i <= nbands; i++) {
      if (Ipixtyp(bands[i]) != pt)
	Error(2, "xhistogram: All bands must have the same pixel type.\n");

      mkHisto(bands[i], hist);
      for (j = 0; j < 256; j++) private->hist[j] += hist[j];
    }

    free(hist);

    LEAVE_FUNCTION_DEBUG("Xhistogram.c: CalcHistogram");
    return;
  }

  {
    double limits[256], minval, maxval, minv, maxv;
    int *hist, i, j;

    if (band_minmax(bands[1], &minval, &maxval)) return;

    for (i = 2; i <= nbands; i++) {
      if (Ipixtyp(bands[i]) != pt)
	Error(2, "xhistogram: All bands must have the same pixel type.\n");
      
      if (band_minmax(bands[i], &minv, &maxv)) return;
      minval = MIN(minval, minv);
      maxval = MAX(maxval, maxv);
    }

    if (minval == maxval) maxval += 1.0;

    for (i=0; i<=254; i++)
      limits[i] = (maxval - minval) * (i+1) / 255.0 + minval;

    for (i = 0; i < 256; i++) private->hist[i] = 0;
    hist = (int *) malloc(256 * sizeof(int));

    for (i = 1; i <= nbands; i++) {
      mkDhistogram(bands[i], hist, limits, 255);

      for (j = 0; j < 256; j++) private->hist[j] += hist[j];

    }

    free(hist);

    LEAVE_FUNCTION_DEBUG("Xhistogram.c: CalcHistogram");
    return;
  }

} /* CalcHistogram() */

static void CalcCumulativeHistogram(private_data *private)
{
  int i;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: CalcCumulativeHistogram");
  
  private -> khist[0] = private -> hist[0];

  for(i=1; i<256; i++) 
    private -> khist[i] = private -> khist[i-1] + private -> hist[i];

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: CalcCumulativeHistogram");

} /* CalcCumulativeHistogram() */

static void CalcFractionalHistogram(private_data *private)
{
  int i;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: CalcFractionalHistogram");
  
  for(i=0; i<256; i++)
    private -> fhist[i] = 100.0 * private -> khist[i]/ private -> khist[255];

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: CalcFractionalHistogram");

} /* CalcFractionalHistogram() */
  


static Widget CreateHistogramWidget(private_data *private, IMAGE image, IBAND band, Visual *visual, Colormap map, int planes, int realDepth, Widget fromVert, Widget parent, Dimension width, Dimension height)
{
  Widget histWid;

  ENTER_FUNCTION_DEBUG("Xhistogram.c: CreateHistogramWidget");


  histWid = XtVaCreateWidget("histogram", histogramWidgetClass, parent,
			     XtNwidth, width,
			     XtNheight, height,
			     XtNhistogramHistogram, private->hist,
			     XtNhistogramLength, 256,
			     XtNfromVert, fromVert,
			     XtNfromHoriz, NULL,
			     XtNhistogramColormap, map,
			     XtNhistogramImage, image,
			     XtNhistogramBand, band,
			     XtNbackground, ((planes >= 7) ?
					     RGBPixel(207, visual) :
					     WhitePixel(dpy_e,
							DefaultScreen(dpy_e))),
			     XtNresizable, TRUE,
			     XtNhistogramVisual, visual,
			     XtNdepth, realDepth,
			     NULL);

  XtAddCallback(histWid,  XtNhistogramButton, Button, (XtPointer) private);
  XtAddCallback(histWid,  XtNdestroyCallback, Kill,   (XtPointer) private);
  XtAddCallback(histWid,  XtNhistogramMotion, Motion, (XtPointer) private);

  LEAVE_FUNCTION_DEBUG("Xhistogram.c: CreateHistogramWidget");

  return(histWid);

} /* CreateHistogramWidget() */



void xhistogram(Widget wid, IMAGE image, IBAND band, Colormap map, int planes, ximage_display_func disp, image_callbacks *callbacks)
{
  /* map is full- or reduced-color version of colormap. */

   Window win[3];
   private_data *private;
   Widget Pix = NULL, Histogram;
   Widget tophistogram, Form, Formdialog;
   Widget Form1, Form2, Label, Label1, Label2, ListModes, ListExits;
   int realDepth, changeable;
   char name[80], *title;
   Dimension label1Width=0, label2Width=0, list1Width=0,
             list2Width=0, maxWidth=0;
   XFontStruct *font;
   Visual *visual;
   Boolean compl = False;

   ENTER_FUNCTION_DEBUG("Xhistogram.c: xhistogram");

   dispfunc_s = disp;

   if (wid) {
     char *pixname;

     XtVaGetValues(wid, XtNvisual,       &visual,
		        XtNimagePixtype, &pixname, NULL);

     changeable = ImageHasColormap((ImageWidget) wid);


     if (pixname && (!strcmp(pixname, ImageComplexPixel) ||
		     !strcmp(pixname, ImageDoubleComplexPixel)))
       compl = True;
   } else {
     IBAND b;
     IPIXTYP pt = -1;
     
     if (((b = band) != NULL) || (image && ((b = image[1]) != NULL)))
       pt = Ipixtyp(b);
     if (pt == Icomplex_typ || pt == Id_complex_typ) compl = True;

     visual = visualFromName(NULL, 0);
     changeable = Has_changeable_map(dpy_e, DefaultScreen(dpy_e), visual);
   }

   if (compl) {
     Error(2, "xhistogram(): Pixeltype can not be complex.\n");
     return;
   }

   if      (planes <= 0)  {
     realDepth = DepthOfVisual(visual);
     planes    = DepthToReducedPlanes(realDepth, visual);
   }
   else realDepth = PlanesToDepth(planes);

   if (!map) map = cmapOfVisual(visual, planes);

   /* Set known colors in some cells. */
   if (changeable)
     ColormapStd(dpy_e, DefaultScreen(dpy_e), map, ClassOfVisual(visual), 1);

   CopyToPrivate(wid, image, band, realDepth, visual, callbacks, &private);
   CalcHistogram(private);
   CalcCumulativeHistogram(private);
   CalcFractionalHistogram(private);

   if (!wid)
     tophistogram = (Widget) CreatePopTopShell("histogramShell", Control_e);
   else
     tophistogram = (Widget) CreatePopTopShell("histogramShell",
					       TopLevelWidget(wid));

   Form = XtCreateWidget("histogramForm", formWidgetClass, tophistogram,
			 NULL, 0);

   Label = XtVaCreateWidget("histogramLabel", labelWidgetClass, Form,
			    XtNlabel, LabelText,
			    XtNborderWidth, 3,
			    XtNresizable, FALSE,
			    NULL);
   private -> Label_wid = Label;

   Histogram =
     CreateHistogramWidget(private, image, band, visual, map, planes,
			   realDepth,
			   Label, /* fromVert */
			   Form,
			   HISTWIDTH,
			   HISTHEIGHT); /* parent */
   private -> Histogram_wid = Histogram;

   if (changeable) {
     Formdialog = XtVaCreateWidget("histogramMenuForm", formWidgetClass, Form,
				   XtNfromHoriz, Histogram,
				   XtNfromVert, Label,
				   XtNdefaultDistance, 10,
				   XtNborderWidth, 0,
				   NULL);
     
     Form1 = XtVaCreateWidget("histogramModeForm", formWidgetClass, Formdialog,
			      XtNvertDistance, 0,
			      XtNdefaultDistance, 0,
			      XtNborderWidth, 3,
			      XtNfromVert, NULL,
			      XtNfromHoriz, NULL,
			      NULL);
     
     Label1 = XtVaCreateWidget("histogramModeLabel", labelWidgetClass, Form1,
			       XtNlabel, Label1Text,
			       XtNfromVert, NULL,
			       XtNfromHoriz, NULL,
			       XtNborderWidth, 1,
			       NULL);

     XtVaGetValues(Label1, XtNfont, &font, NULL);
     label1Width = XTextWidth(font, Label1Text, strlen(Label1Text)) +
                   XTextWidth(font,   "XX", strlen("XX"));

     ListModes = XtVaCreateWidget("histogramModeList",
				  xfwfMultiListWidgetClass, Form1,
				  XtNfromVert, Label1,
				  XtNfromHoriz, NULL,
				  XtNnumberStrings, planes < 7 ? 1 : 4,
				  XtNlist, modes,
				  XtNvertDistance, 0,
				  XtNverticalList, TRUE,
				  XtNmaxSelectable, 1,
				  XtNsensitiveArray, sens_modes_all,
				  NULL);
     
     XtVaGetValues(ListModes, XtNfont, &font, NULL);
     list1Width = XTextWidth(font,   "Green",
			      strlen("Green")) + 
		  XTextWidth(font,   "XX", strlen("XX"));

     XtAddCallback(ListModes, XtNcallback, Modes, (XtPointer) private);
     
     Form2 = XtVaCreateWidget("histogramActionsForm", formWidgetClass,
			      Formdialog,
			      XtNfromVert, Form1,
			      XtNfromHoriz, NULL,
			      XtNvertDistance, 20,
			      XtNdefaultDistance, 0,
			      XtNborderWidth, 3,
			      NULL);

     Label2 = XtVaCreateWidget("histogramActionsLabel", labelWidgetClass,
			       Form2,
			       XtNlabel, Label2Text,
			       XtNfromVert, NULL,
			       XtNfromHoriz, NULL,
			       XtNborderWidth, 1,
			       NULL);

     XtVaGetValues(Label2, XtNfont, &font, NULL);
     label2Width = XTextWidth(font, Label2Text, strlen(Label2Text)) +
		  XTextWidth(font,   "XX", strlen("XX"));
     
     ListExits = XtVaCreateWidget("histogramActionsList",
				xfwfMultiListWidgetClass, Form2,
				XtNfromHoriz, NULL,
				XtNfromVert, Label2,
				XtNnumberStrings, 11,
				XtNlist, exits,
				XtNvertDistance, 0,
				XtNverticalList, TRUE,
				XtNdefaultColumns, 1,
				XtNmaxSelectable, 1,
				XtNsensitiveArray, sens_exits_all,
				NULL);

     XtVaGetValues(ListExits, XtNfont, &font, NULL);
     list2Width = XTextWidth(font,   "Piecewise linear",
			      strlen("Piecewise linear")) +
		  XTextWidth(font,   "XX", strlen("XX"));
     
     if (!wid)
       /* Must set XtNlist when changing XtNsensitiveArray. */
       XtVaSetValues(ListExits, XtNlist, exits,
		     XtNsensitiveArray, sens_exits_main, NULL);
     else if (wid && dispfunc_s == NULL)
       /* Must set XtNlist when changing XtNsensitiveArray. */
       XtVaSetValues(ListExits, XtNlist, exits,
		     XtNsensitiveArray, sens_exits_col, NULL);
     
     private -> ListExits_wid = ListExits;
     XtAddCallback(ListExits, XtNcallback, Exits, (XtPointer) private);
   } else {
     private -> ListExits_wid = NULL;
   }
   

   if (changeable) {
     maxWidth = MAX(MAX(label1Width, label2Width),MAX(list1Width, list2Width));
     private->labelWidth = HISTWIDTH + 18 + maxWidth;
     XtVaSetValues(Label1,    XtNwidth, maxWidth, NULL);
     XtVaSetValues(Label2,    XtNwidth, maxWidth, NULL);
     XtVaSetValues(ListModes, XtNwidth, maxWidth, NULL);
     XtVaSetValues(ListExits, XtNwidth, maxWidth, NULL);
   } else {
     maxWidth   = 0;
     private->labelWidth = HISTWIDTH+50;
   }

   XtVaSetValues(Label, XtNwidth, private->labelWidth, NULL);

   if (!changeable) {
     FPRINTF1("    StaticGray, StaticColor or TrueColor.\n");
   }

   XtManageChild(Form);
   if (changeable) {
     XtManageChild(Formdialog);
   }
   XtManageChild(Histogram);
   XtManageChild(Label);

   if (changeable) {
     String trans = "\
       <EnterWindow>: WMColormap(Enter) notify()\n\
       <LeaveWindow>: notify() WMColormap(Leave)\n\
       <BtnDown>:     nop()";

     Pix = colorbar_widget(Form, "colormap", NULL, -1, CWIDTH, CHEIGHT,
			   0, 255, visual, planes, "Colorstrip", NULL);

     XtVaSetValues(Pix, XtNfromVert, Histogram, NULL);
     XtOverrideTranslations(Pix, XtParseTranslationTable(trans));

     XtManageChild(Pix);
   }

   if (changeable) {
     XtManageChild(Form1);
     XtManageChild(Label1);
     XtManageChild(ListModes);
     XtManageChild(Form2);
     XtManageChild(Label2);
     XtManageChild(ListExits);
   }
   XtRealizeWidget(tophistogram);

   XtCallActionProc(Histogram, "Crossing", NULL, NULL, 0);

   XtPopup(tophistogram, XtGrabNone);

   win[0] = XtWindow(tophistogram);
   win[1] = XtWindow(Histogram);
   if (changeable) win[2] = XtWindow(Pix);

   if (changeable) {
     /* Tell the window manager that subwindows of the top-level histogram
      * shell window need their own colormaps installed.
      */
     XSetWMColormapWindows(dpy_e, win[0], win, 3);
   }

   (void) strcpy(name, "Histogram ");
   if (wid) {
     XtVaGetValues(wid, XtNimageTitle, &title, NULL);
     (void) strcat(name, title);
   }
   XmbSetWMProperties(dpy_e, XtWindow(tophistogram), name, name,
		      NULL, 0, NULL, NULL, NULL);
   (void) XSetWMProtocols(dpy_e, XtWindow(tophistogram), &wm_delete_window, 1);

   if (changeable) {
     XfwfMultiListHighlightItem((XfwfMultiListWidget) ListModes, 0);
   }

   /* Choose action number 0 (piecewise linear histogram specification) for
    * non-monochrome images, thresholding for monochrome images.
    */
   if (planes < 7) private->hspes = THRESHOLD_EXIT;
   else private->hspes = PIECEWISE_EXIT;

   if (changeable) {
     XfwfMultiListHighlightItem((XfwfMultiListWidget) ListExits,
				private->hspes);
   }
   HistogramInitLines((HistogramWidget) Histogram, private->col,
		      private->hspes);

   LEAVE_FUNCTION_DEBUG("Xhistogram.c: xhistogram");

} /* xhistogram() */

#endif /* not MAIN */



#ifdef MAIN

/*P:xhistogram*

________________________________________________________________

		xhistogram
________________________________________________________________

Name:		xhistogram - Show the histogram of an image in XITE.

Syntax:		xhistogram [-split] <inimage>

Description:    Display the histogram for the BIFF image read from 'inimage'.
                Some of the command buttons will be insensitive. If making
		a histogram from the Tools menu of an image widget in another
		XITE display application, e.g. xshow, these buttons will be
		sensitive.

Options:        'xhistogram' supports all standard X Toolkit command line
                arguments (see X(1)) as well as the XITE 'ximage(3)' toolkit
                command line options.

		Additional options:

		&-split
		If 'inimage' is multiband, the default behaviour is to
		show a total histogram for all the bands. This option
		forces one histogram per band.

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'xhistogram'.
                In the notation below, indentation indicates hierarchical 
                structure.

		&xhistogram                 (class XHistogram)
		| histogramShell            (class TopLevelShell)
		|  histogramForm            (class Form)
		|   histogramLabel          (class Label)
		|   histogram               (class Histogram)
		|   histogramMenuForm       (class Form)
		|    histogramModeForm      (class Form)
		|     histogramModeLabel    (class Label)
		|     histogramModeList     (class XfwfMultiList)
		|    histogramActionsForm   (class Form)
		|     histogramActionsLabel (class Label)
		|     histogramActionsList  (class XfwfMultiList)
		|   colormap                (class Colormap)

See also:       'xhistogram(3)', 'mkHisto(1)', 'ximage(3)', 'Histogram(3)',
                'XfwfMultiList(3)'

Files:          $XITE_HOME/etc/app-defaults/XHistogram

Author:		Otto Milvang

Revised:        Svein Bøe

Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int i;
  char *local_usage = "Usage: %s [-split] <inimage>\n";
  int split;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));

  Control_e = ximage_init(&argc, argv, "XHistogram", NULL, True);

  if (argc == 1) Usage(1, NULL);
  split = read_bswitch(&argc, argv, "-split");
  if (argc != 2) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  if (img == NULL) Error(2, "Can't open file %s\n", argv[1]);

  if (!split) xhistogram(NULL, img, NULL, 0, 0, ximage_display, NULL);
  else {
    for (i=1; i<=Inbands(img); i++) {
      xhistogram(NULL, img, img[i], 0, 0, ximage_display, NULL);
    }
  }

  ximage_mainloop();

  return(0);
}

#endif /* MAIN */
