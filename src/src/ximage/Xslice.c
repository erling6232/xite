
/*

________________________________________________________________

        Xslice.c
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



#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <xite/Xslice.h>
#include <X11/StringDefs.h>
#include <xite/Image.h>
#include "Colormap.h"
#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Form.h>
#include <xite/color.h>
#include <xite/ihs.h>
#include <xite/message.h>
#include <xite/ShellWids.h>
#include <xite/Visual.h>
#include <xite/Cmap.h>
#include <xite/Xcolor.h>
#include <xite/Xdialog.h>
#include <xite/FormDialog.h>
#include <xite/ximage_display.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRTOL_H
#include XITE_TYPES_H
#include <xite/debug.h>

#define PATCH_PIXEL 192
#define PATCH_COLOR_CELL 0

#ifndef MIN
# define MIN(a, b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif

extern Atom wm_delete_window;

typedef struct
{
  Widget toplevel, Image, colorStripe, colorPatch, bar[6];
  Widget labelIhs, labelRgb, colorIndex;
  Dimension colorIndex_w;
  Colormap map,           /* Reduced color colormap for the color stripe,
                           * supplied on xslice entry. */
           orig_map_full; /* Full color colormap for the colortable active
			   * prior to xslice entry. */
  IMAGE img;
  int planes;
  int x, y,
      active;             /* Number of active mouse button, or 0. */
  double red, green, blue,  intensity, hue, saturation;
  XColor col[256], /* Hold the full-color color table for the color stripe. */
         old[256];
  XColor ihscol[257];           /* Use element 0 to hold patch color. */
  XColor old_patch_red_cell[1]; /* Save the color for pixel value PATCH_PIXEL
                                 * in the last colormap, to restore on exit. */
  Visual *visual;
} private_data;

static Display *dpy_s;

static char *scrollbarNames[] = {
  "scrollbarIntensity",
  "scrollbarHue",
  "scrollbarSaturation",
  "scrollbarRed",
  "scrollbarGreen",
  "scrollbarBlue",
};

static char *scrollLabelNames[] = {
  "labelIntensity",
  "labelHue",
  "labelSaturation",
  "labelRed",
  "labelGreen",
  "labelBlue",
};

typedef enum {
  I_BAR,
  H_BAR,
  S_BAR,
  R_BAR,
  G_BAR,
  B_BAR,
  COLOR_STRIPE
} wid_num_t;



#ifndef FUNCPROTO
static void xslice_notify(wid, data, call_data)
Widget wid;
private_data *data;
XtPointer call_data;
#else /* FUNCPROTO */
static void xslice_notify(Widget wid, private_data *data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  char buff[80];
  size_t scroll;

  ENTER_FUNCTION_DEBUG("Xslice.c: xslice_notify");

  scroll = (size_t) call_data;

  sprintf(buff, "I=%6.4f H=%6.4f S=%6.4f", 
	  data->intensity, data->hue, data->saturation);
  XtVaSetValues(data->labelIhs, XtNlabel, buff, NULL);

  sprintf(buff, "R=%6.4f G=%6.4f B=%6.4f", 
	  data->red, data->green, data->blue);
  XtVaSetValues(data->labelRgb, XtNlabel, buff, NULL);

  sprintf(buff, "R=0x%04x G=0x%04x B=0x%04x", 
	  data->ihscol[PATCH_COLOR_CELL].red,
	  data->ihscol[PATCH_COLOR_CELL].green,
	  data->ihscol[PATCH_COLOR_CELL].blue);

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_notify");
}


#ifndef FUNCPROTO
static void xslice_work(wid, data, wnr)
Widget wid;
private_data *data;
wid_num_t wnr;
#else /* FUNCPROTO */
static void xslice_work(Widget wid, private_data *data, wid_num_t wnr)
#endif /* FUNCPROTO */
{
  unsigned short r, g, b, i, h, s;

  ENTER_FUNCTION_DEBUG("Xslice.c: xslice_work");

  if (wnr <= S_BAR || wnr == COLOR_STRIPE) {
    ihs2rgb(data->intensity, data->hue, data->saturation,
	    &data->red, &data->green, &data->blue, 1);
    XawScrollbarSetThumb(data->bar[R_BAR], (float) (1.0-data->red),    1.0);
    XawScrollbarSetThumb(data->bar[G_BAR], (float) (1.0-data->green),  1.0);
    XawScrollbarSetThumb(data->bar[B_BAR], (float) (1.0-data->blue),   1.0);
  }
  if (wnr > S_BAR) {
    rgb2ihs(data->red, data->green, data->blue,
	    &data->intensity, &data->hue, &data->saturation, 1);
    XawScrollbarSetThumb(data->bar[I_BAR], (float) (1.0-data->intensity), 1.0);
    XawScrollbarSetThumb(data->bar[H_BAR], (float) (1.0-data->hue),       1.0);
    XawScrollbarSetThumb(data->bar[S_BAR], (float) (1.0-data->saturation),1.0);
  }

  r = data->red   * 65535.0;
  g = data->green * 65535.0;
  b = data->blue  * 65535.0;

  data->ihscol[PATCH_COLOR_CELL].red   = r;
  data->ihscol[PATCH_COLOR_CELL].green = g;
  data->ihscol[PATCH_COLOR_CELL].blue  = b;

  rgb2ihs_word(r, g, b, &i, &h, &s);

  XStoreColors(XtDisplay(wid), data->map, data->ihscol, 1);
  xslice_notify(wid, data, 0);
  ImageRedisplay((ImageWidget) data->colorPatch, UPDATE_MINMAX);

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_work");
}

#ifndef FUNCPROTO
static void fillmap(data)
private_data *data;
#else /* FUNCPROTO */
static void fillmap(private_data *data)
#endif /* FUNCPROTO */
{
  XColor col[128];
  int i;

  ENTER_FUNCTION_DEBUG("Xslice.c: fillmap");

  /* Take every other color entry from data->col and insert into
   * the first 128 cells of data->map, with pixelvalues in the range 64..191.
   */
  if (ClassOfVisual(data->visual) == PseudoColor) {
    for(i=0; i<128; i++) {
      col[i] = data->col[2*i];
      col[i].pixel = 64+i;
    }
  } else {
    for(i=0; i<128; i++) {
      col[i] = data->col[2*i];
      col[i].pixel = RgbPixel(64+i);
    }
  }

  XStoreColors(XtDisplay(data->toplevel), data->map, col, 128);

  LEAVE_FUNCTION_DEBUG("Xslice.c: fillmap");
}  


#ifndef FUNCPROTO
static void xslice_exit(wid, p_data, c_data)
Widget wid;
XtPointer p_data, c_data;
#else /* FUNCPROTO */
static void xslice_exit(Widget wid, XtPointer p_data, XtPointer c_data)
#endif /* FUNCPROTO */
{
  private_data *data;

  ENTER_FUNCTION_DEBUG("Xslice.c: xslice_exit");

  /* Restore the old/original colors for pixel PATCH_PIXEL. */
  data = (private_data *) p_data;

  XStoreColors(XtDisplay(data->toplevel), data->map,
	       data->old_patch_red_cell, 1);
  ImageKill(data->colorStripe, (XEvent*) 0, (String*) 0, (Cardinal *) 0);
  free(data);

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_exit");
}

#ifndef FUNCPROTO
static void quit_slice(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void quit_slice(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  Widget quit;
  char name[50];

  ENTER_FUNCTION_DEBUG("Xslice.c: quit_slice");

  quit = XtNameToWidget(wid, strcat(strcpy(name, "*"), "quit"));

  if (quit) XtCallCallbacks(quit, XtNcallback, NULL);

  LEAVE_FUNCTION_DEBUG("Xslice.c: quit_slice");

} /* quit_slice() */


#ifndef FUNCPROTO
static void setcol(data, x)
private_data *data;
int x;
#else /* FUNCPROTO */
static void setcol(private_data *data, int x)
#endif /* FUNCPROTO */
{
  int i, xf, xt, len, dir;
  XColor *c;

  ENTER_FUNCTION_DEBUG("Xslice.c: setcol");

  /* Set color from patch into colortable between color cells x and data->x.
   * x must be in the range [1..256]. */

  xf  = MIN(x, data->x); /* From */
  xt  = MAX(x, data->x); /* To   */
  len = xt - xf + 1;

  if (ClassOfVisual(data->visual) == PseudoColor) {
    for(i=1; i<=len; i++)
      {
	/* Is x in the open interval  <data->x, data->y> (or reversed) ?
	 * If it is, copy from stripe to patch.
	 * If not, copy from patch to stripe. */
	dir = (x > data->y && x < data->x) || (x < data->y && x > data->x);

	c = &data->ihscol[i];
	if (dir) {
	  /* From stripe to cell i in ihscol. */
	  *c = data->old[xf-1+i];
	} else {
	   /* From patch to cell i in ihscol. */
	  *c = data->ihscol[PATCH_COLOR_CELL];
	}
	
	/* Pixel value in ihscol. */
	c->pixel = (xf - 1 + i) / 2 + 64;

	/* Pixel value and rgb values in col. */
	data->col[xf - 1 + i]       = *c;
	data->col[xf - 1 + i].pixel = xf - 1 + i;
      }
  } else {
    for(i=1; i<=len; i++)
      {
	dir = (x>data->y && x < data->x) || (x<data->y && x > data->x);
	c = &data->ihscol[i];
	if (dir)
	  *c = data->old[xf-1+i];
	else
	  *c = data->ihscol[PATCH_COLOR_CELL];
	
	c->pixel = RgbPixel((xf-1+i)/2 + 64);
	data->col[xf-1+i] = *c;
	data->col[xf-1+i].pixel = RgbPixel(xf-1+i);
      }
  }

  XStoreColors(XtDisplay(data->toplevel), data->map, data->ihscol, 1 + len);
  data->x = x;

  LEAVE_FUNCTION_DEBUG("Xslice.c: setcol");
}

#ifndef FUNCPROTO
static void xslice_mark(wid, dta, imag)
Widget wid;
XtPointer dta, imag;
#else /* FUNCPROTO */
static void xslice_mark(Widget wid, XtPointer dta, XtPointer imag)
#endif /* FUNCPROTO */
{
  private_data *data;
  ImageCallback img_cb;
  int i, x, min, max;
  char buff[80];

  ENTER_FUNCTION_DEBUG("Xslice.c: xslice_mark");

  data      = (private_data *) dta;
  img_cb    = (ImageCallback) imag;
  img_cb->x = (img_cb->x - 1)/2;
  x         = img_cb->x;

  /* This function is called by the Image action function 'notify'.
   * x and y of imag are in the BIFF image coordinate system.
   * x was divided by two because the colorbar widget has width 512
   * while a colortab has 256 elements.
   */

  if (img_cb->event->type != LeaveNotify && img_cb->event->type != EnterNotify)
    {
      if (x < 0 || x > 255) {
	LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_mark");
	return;
      }
      sprintf(buff, "%3d", x);
    } else sprintf(buff, "   ");

  XtVaSetValues(data->colorIndex, XtNlabel, buff,
		                  XtNwidth, data->colorIndex_w,
                                  NULL);
  switch(img_cb->event->type)
    {
    case ButtonPress:
      data->active = img_cb->event->xbutton.button;

      /* Make sure that setcol() copies from patch to stripe and that only
       * one stripe color is affected. */
      data->x = x;
      data->y = x;

      for (i=0; i<256; i++) data->old[i] = data->col[i];

      switch(data->active)
	{
	case 1:
	  /* Btn1. Set color from patch into stripe at this position. */
	  setcol(data, x);
	  break;
	case 2:
	  /* Btn2. Set color from stripe at this position into patch. */
	  data->red   = 1.0 * data->col[x].red   / 0x0ffff;
	  data->green = 1.0 * data->col[x].green / 0x0ffff;
	  data->blue  = 1.0 * data->col[x].blue  / 0x0ffff;
	  rgb2ihs(data->red, data->green, data->blue,
		  &data->intensity, &data->hue, &data->saturation, 1);
	  xslice_work(wid, data, COLOR_STRIPE);
	  break;
	case 3:
	  /* Btn3. Replace a constant part of LUT values at the cursor position
	   * with the palette color.
	   */

	  min = max = img_cb->x;
	  while (min > 0 && data->col[x].red == data->col[min-1].red &&
		 data->col[x].green == data->col[min-1].green &&
		 data->col[x].blue == data->col[min-1].blue) min--;
	  while (max < 255 && data->col[x].red == data->col[max+1].red &&
		 data->col[x].green == data->col[max+1].green &&
		 data->col[x].blue == data->col[max+1].blue) max++;

	  if (ClassOfVisual(data->visual) == PseudoColor) {
	    for (i=min; i<=max; i++) {
	      data->col[i]       = data->ihscol[PATCH_COLOR_CELL];
	      data->col[i].pixel = i;
	      data->old[i]       = data->col[i];
	    }
	  } else {
	    for (i=min; i<=max; i++) {
	      data->col[i]       = data->ihscol[PATCH_COLOR_CELL];
	      data->col[i].pixel = RgbPixel(i);
	      data->old[i]       = data->col[i];
	    }
	  }
	  fillmap(data);
	  break;
	}
      break;
    case MotionNotify:
      if (data->active == 0) break;
      setcol(data, img_cb->x);
      break;
    case ButtonRelease:
      if (data->active == 0) break;
      setcol(data, img_cb->x);
      data->active = 0;
      break;      
    case LeaveNotify:
      if (data->active == 0) break;
      for (i=0; i<256; i++) data->col[i] = data->old[i];
      data->y = data->x = 0;
      fillmap(data);
      data->active = 0;
      break;
    case EnterNotify:
      data->active = 0;
      break;
    }
  XFlush(XtDisplay(data->toplevel));

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_mark");
}

#ifndef FUNCPROTO
static void SendColtabToXshow(wid, dta, imag)
Widget wid;
XtPointer dta, imag;
#else /* FUNCPROTO */
static void SendColtabToXshow(Widget wid, XtPointer dta, XtPointer imag)
#endif /* FUNCPROTO */
{
  private_data *data;
 
  ENTER_FUNCTION_DEBUG("Xslice.c: SendColtabToXshow");

  data = (private_data *) dta;

  Color_add(data->col, "ColorSlice", 256);
  Message(0, "Colortab added to image color menu.\n");

  LEAVE_FUNCTION_DEBUG("Xslice.c: SendColtabToXshow");
}

#ifndef FUNCPROTO
static void SendColtabToFile(wid, dta, imag)
Widget wid;
XtPointer dta, imag;
#else /* FUNCPROTO */
static void SendColtabToFile(Widget wid, XtPointer dta, XtPointer imag)
#endif /* FUNCPROTO */
{
  char *filename;
  private_data *data;
  BiffStatus status;
 
  ENTER_FUNCTION_DEBUG("Xslice.c: SendColtabToFile");

  data = (private_data *) dta;

  if (SelectWriteFile(data->toplevel, writeColortab, &filename) ==0) {
    return;
  }
  
  status = color_write(filename, (Color_cell *) data->col, "ColorSlice", 256);
  if (status != Iok) {
    InformationBox(data->toplevel, Ierr_message(status),
		   "slice - information", "slice - info");
  }

  LEAVE_FUNCTION_DEBUG("Xslice.c: SendColtabToFile");
}

#ifndef FUNCPROTO
static void LoadColtabFromFile(wid, dta, imag)
Widget wid;
XtPointer dta, imag;
#else /* FUNCPROTO */
static void LoadColtabFromFile(Widget wid, XtPointer dta, XtPointer imag)
#endif /* FUNCPROTO */
{
  char *filename;
  int len;
  private_data *data;
  ImageCallback img;
  char *colName;
  BiffStatus status;
 
  ENTER_FUNCTION_DEBUG("Xslice.c: LoadColtabFromFile");

  data = (private_data *) dta;
  img = (ImageCallback) imag;

  if (SelectReadFile(data->toplevel, readColortab, &filename) ==0) {
    return;
  }
  
  status = color_read(filename, (Color_cell *) data->col, &colName, &len);
  if (status == Iok) {
    fillmap(data);
  } else {
    Message(2, "%s\n", Ierr_message(status));
    free(filename);
  }

  LEAVE_FUNCTION_DEBUG("Xslice.c: LoadColtabFromFile");

} /* LoadColtabFromFile() */

#ifndef FUNCPROTO
static void LoadColtabFromXshow(wid, dta, imag)
Widget wid;
XtPointer dta, imag;
#else /* FUNCPROTO */
static void LoadColtabFromXshow(Widget wid, XtPointer dta, XtPointer imag)
#endif /* FUNCPROTO */
{
  private_data *data;
  ImageCallback img;
 
  ENTER_FUNCTION_DEBUG("Xslice.c: LoadColtabFromXshow");

  data = (private_data *) dta;
  img = (ImageCallback) imag;

  InitColortablePixels(data->col, 256, 0,
		       ClassOfVisual(data->visual) == DirectColor ||
		       ClassOfVisual(data->visual) == TrueColor);
  
  XQueryColors(dpy_s, data->orig_map_full, data->col, 256);
  fillmap(data);

  LEAVE_FUNCTION_DEBUG("Xslice.c: LoadColtabFromXshow");

} /* LoadColtabFromXshow() */

#ifndef FUNCPROTO
static void SetRange(wid, dta, imag)
Widget wid;
XtPointer dta, imag;
#else /* FUNCPROTO */
static void SetRange(Widget wid, XtPointer dta, XtPointer imag)
#endif /* FUNCPROTO */
{
  int mini, maxi, status;
  char *return_text;
  private_data *data;
  ImageCallback img;

  ENTER_FUNCTION_DEBUG("Xslice.c: SetRange");

  data = (private_data *) dta;
  img = (ImageCallback) imag;
 
  status = FormDialog(data->toplevel, " ",
" # Set minimum value for range # # d # 0 \n\
 # Set maximum value for range # # d # 32",
           &return_text, NULL, 3, "Slice dialog", "Slice dialog");

  if (status != 1) return;

  if (return_text) {
    (void) sscanf(return_text, "%d%d", &mini, &maxi);
    data->x = data->y = mini;
    data->active = 1;
    setcol(data, maxi);
    data->active = 0;
  }

  LEAVE_FUNCTION_DEBUG("Xslice.c: SetRange");
}


#ifndef FUNCPROTO
static void xslice_jump(wid, p_data, c_data)
Widget wid;
XtPointer p_data, c_data;
#else /* FUNCPROTO */
static void xslice_jump(Widget wid, XtPointer p_data, XtPointer c_data)
#endif /* FUNCPROTO */
{
  float f1;
  wid_num_t wnr;
  private_data *data;

  ENTER_FUNCTION_DEBUG("Xslice.c: xslice_jump");

  data = (private_data *) p_data;
  wnr = I_BAR;
  while ((wnr <= B_BAR) && (wid != data->bar[wnr])) wnr++;
  if (wnr == B_BAR + 1) {
    LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_jump");
    return;
  }

  f1 = 1.0 - *(float*) c_data;
/*  printf("%d: %10.6f\n",client_data, f1); */
  switch(wnr)
    {
    case I_BAR: data->intensity = f1; break;
    case H_BAR: data->hue = f1; break;
    case S_BAR: data->saturation = f1; break;
    case R_BAR: data->red = f1; break;
    case G_BAR: data->green = f1; break;
    case B_BAR: data->blue = f1; break;
    }
  xslice_work(wid, data, wnr);

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_jump");
}

#ifndef FUNCPROTO
static void xslice_scroll(wid, p_data, c_data)
Widget wid;
XtPointer p_data, c_data;
#else /* FUNCPROTO */
static void xslice_scroll(Widget wid, XtPointer p_data, XtPointer c_data)
#endif /* FUNCPROTO */
{
  wid_num_t wnr;
  ptrdiff_t scroll; 
  private_data *data;
  
  ENTER_FUNCTION_DEBUG("Xslice.c: xslice_scroll");

  data = (private_data *) p_data;
  wnr = I_BAR;
  while ((wnr <= B_BAR) && (wid != data->bar[wnr])) wnr++;
  if (wnr == B_BAR + 1) {
    LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_scroll");
    return;
  }

  scroll = (size_t) c_data;
/*  printf("%d: %10.6f\n",client_data, f1); */

  scroll = -scroll;
  if(scroll >= 0) {
    switch(wnr)
      {
#define P05(f) if (f > 0.95)  f = 1.0; else f += 0.05
      case I_BAR: P05(data->intensity);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->intensity), 1.0);
	break;
      case H_BAR: P05(data->hue);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->hue), 1.0);
	break;
      case S_BAR: P05(data->saturation);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->saturation), 1.0);
	break;
      case R_BAR: P05(data->red);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->red), 1.0);
	break;
      case G_BAR: P05(data->green);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->green), 1.0);
	break;
      case B_BAR: P05(data->blue);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->blue), 1.0);
	break;
#undef P05
      }
  } else {
    switch(wnr)
      {
#define P05(f) if (f < 0.05)  f = 0.0; else f -= 0.05
      case I_BAR: P05(data->intensity);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->intensity), 1.0);
	break;
      case H_BAR: P05(data->hue);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->hue), 1.0);
	break;
      case S_BAR: P05(data->saturation);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->saturation), 1.0);
	break;
      case R_BAR: P05(data->red);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->red), 1.0);
	break;
      case G_BAR: P05(data->green);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->green), 1.0);
	break;
      case B_BAR: P05(data->blue);
	XawScrollbarSetThumb(data->bar[wnr],
			     (float) (1.0-data->blue), 1.0);
	break;
#undef P05
      }
  }
  xslice_work(wid, data, wnr);

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice_scroll");
}


#define CW XtVaCreateManagedWidget
#define X2 292
#define X3 432

static float scrollbar_init_value[6] = { 0.3, 1.0, 0.2, 0.42, 0.42, 0.06, };

#ifndef FUNCPROTO
static void CopyToPrivate(wid, image, planes, cmap, visual, private)
Widget wid;
IMAGE image;
int planes;
Colormap cmap;
Visual *visual;
private_data **private;
#else /* FUNCPROTO */
static void CopyToPrivate(Widget wid, IMAGE image, int planes, Colormap cmap, Visual *visual, private_data **private)
#endif /* FUNCPROTO */
{
  int colorNumber;

  ENTER_FUNCTION_DEBUG("Xslice.c: CopyToPrivate");

  XtVaGetValues(wid, XtNcolortab, &colorNumber, NULL);

  (*private) = (private_data*) malloc(sizeof(private_data));
  (*private)->Image         = wid;
  (*private)->img           = image;
  (*private)->active        = 0;
  (*private)->planes        = planes;
  (*private)->map           = cmap;
  (*private)->orig_map_full = Color_map2(colorNumber, 256, visual);
  (*private)->red           = 1 - scrollbar_init_value[R_BAR];
  (*private)->green         = 1 - scrollbar_init_value[G_BAR];
  (*private)->blue          = 1 - scrollbar_init_value[B_BAR];
  (*private)->intensity     = 1 - scrollbar_init_value[I_BAR];
  (*private)->hue           = 1 - scrollbar_init_value[H_BAR];
  (*private)->saturation    = 1 - scrollbar_init_value[S_BAR];
  (*private)->visual        = visual;

  LEAVE_FUNCTION_DEBUG("Xslice.c: CopyToPrivate");

} /* CopyToPrivate() */

#ifndef FUNCPROTO
static void MakeColortab(coltab, visual)
XColor *coltab;
Visual *visual;
#else /* FUNCPROTO */
static void MakeColortab(XColor *coltab, Visual *visual)
#endif /* FUNCPROTO */
{
  double r, g, b, i, h, s;
  int j, k, rr, gg, bb;
  XColor *c;

  ENTER_FUNCTION_DEBUG("Xslice.c: MakeColortab");

  /* Eight different color fields. */
  for (j=0; j<256; j+= 32) {

    i = 0.3 + 0.4 / 256.0 * j;
    s = 0.8;
    h = 1.0 - 0.9 / 256.0 * j;

    ihs2rgb(i, h, s, &r, &g, &b, 1);

    rr = r * 0x010000; if (rr >= 0x010000) rr = 0x0ffff;
    gg = g * 0x010000; if (gg >= 0x010000) gg = 0x0ffff;
    bb = b * 0x010000; if (bb >= 0x010000) bb = 0x0ffff;

    /* All the color cells within the current color field get the same
     * color.
     */
    if (ClassOfVisual(visual) == PseudoColor) {
      for (k=0; k<32; k++) {
	c = &coltab[j+k];
	c->pixel = j+k;
	c->red   = rr;
	c->green = gg;
	c->blue  = bb;
	c->flags = DoRed | DoGreen | DoBlue;
	c->pad   = 0;
      } /* for k */
    } else {
      for (k=0; k<32; k++) {
	c = &coltab[j+k];
	c->pixel = RgbPixel(j+k);
	c->red   = rr;
	c->green = gg;
	c->blue  = bb;
	c->flags = DoRed | DoGreen | DoBlue;
	c->pad   = 0;
      } /* for k */
    } /* else */
  } /* for j */

  LEAVE_FUNCTION_DEBUG("Xslice.c: MakeColortab");

} /* MakeColortab() */

#ifndef FUNCPROTO
IMAGE colorbar_image(xsize, ysize, low, high, visual, title)
long xsize, ysize, low, high;
Visual *visual;
char *title;
#else /* FUNCPROTO */
IMAGE colorbar_image(long xsize, long ysize, long low, long high, Visual *visual, char *title)
#endif /* FUNCPROTO */
{
  int nbands;
  IMAGE img;
  IBAND bnd;
  long x, y;

  if (ClassOfVisual(visual) == PseudoColor) nbands = 1;
  else nbands = 3;

  img = Imake_image(nbands, title, Iu_byte_typ, xsize, ysize);
  bnd = img[1];
  
  if (low > high) {
    /* Swap */
    long n;

    n    = high;
    high = low;
    low  = n;
  }

  /* Top line: Values from low..high. */
  for(x = 1; x <=xsize; x++)
    bnd[1][x] = low + (high - low + 1) * (x - 1) / xsize;
  
  /* Copy the top line to the other lines. */
  for(y = 2; y <=ysize; y++)
    for(x = 1; x <=xsize; x++)
      bnd[y][x] = bnd[y-1][x];

  if (nbands == 3) {
    IBAND bnd_g, bnd_b;

    bnd_g = img[2];
    bnd_b = img[3];

    for (y = 1; y <= ysize; y++)
      for (x = 1; x <= xsize; x++) {
	bnd_g[y][x] = bnd[y][x];
	bnd_b[y][x] = bnd[y][x];
      }
  }

  return(img);

} /* colorbar_image() */

#ifndef FUNCPROTO
Widget colorbar_widget(parent, resource_name, shell_resource_name, colortab, xsize, ysize, low, high, visual, depth, title, callbacks)
Widget parent;
char *resource_name, *shell_resource_name;
int colortab, depth;
long xsize, ysize, low, high;
Visual *visual;
char *title;
image_callbacks *callbacks;
#else /* FUNCPROTO */
Widget colorbar_widget(Widget parent, char *resource_name, char *shell_resource_name, int colortab, long xsize, long ysize, long low, long high, Visual *visual, int depth, char *title, image_callbacks *callbacks)
#endif /* FUNCPROTO */
{
  Arg args[2];
  IMAGE img;
  Widget wid;

  img = colorbar_image(xsize, ysize, low, high, visual, title);

  args[0].name  = XtNcolortab;
  args[0].value = (XtArgVal) (colortab >= 0 && colortab <= Color_max() ?
			      colortab : Color_max());
  args[1].name  = XtNmenubar;
  args[1].value = (XtArgVal) False;

  wid = ximage_display(parent, resource_name, shell_resource_name,
		       img, 0, imageWidgetClass, visual, depth, OPTIONS_BUTTON,
		       args, XtNumber(args), callbacks);

  return(wid);

} /* colorbar_widget() */



/*F:colorbar_image=xslice*/
/*F:colorbar_widget=xslice*/
/*F:xslice*

________________________________________________________________

		xslice
________________________________________________________________

Name:		xslice, colorbar_image, colorbar_widget - Mix image colors
                in a palette

Syntax:         | #include <xite/Xslice.h>
		| 
                | void xslice( Widget wid, IMAGE image,
                |    Colormap map, int planes );
		|
		| IMAGE colorbar_image( long xsize, long ysize,
		|    long low, long high, Visual *visual,
		|    char *title );
		|
		| Widget colorbar_widget( Widget parent,
		|    char *resource_name,
		|    char *shell_resource_name, int colortab,
		|    long xsize, long ysize, long low, long high,
		|    Visual *visual, int depth, char *title,
		|    image_callbacks *callbacks );
		
Description:	'xslice' makes a piecewise constant lookup table (LUT) of
                pseudocolors. Colors may be mixed in a palette by specifying
		rgb or ihs values.

		'colorbar_image' returns a BIFF image where each row
		contains pixel values from 'low' to 'high', linearly
		increasing. The image size is determined by 'xsize'
		(horizontal) and 'ysize' (vertical). The image is one-band
		if the class of 'visual' is PseudoColor, otherwise it is
		three-band. If 'low' equals 'high', the default values 0
		and 255 are used for 'low' and 'high' respectively.

		'colorbar_widget' returns a colorbar widget. 'colortab'
		is an index into the list of available colortables for the
		images (shown by the "Colors" menu in an image). With a
		negative value for 'colortab', a default value equal to the
		last colortable in the "Colors" menu will be used.

Buttons:        &Send colortable to xshow
                Send LUT back to xshow

		&Send colortable to file
                Save LUT in a file

		&Load original colortable
		Load the colortable which the corresponding image used when
		the slice application was started

		&Load colortable from file
		Read LUT from file

                &Set patch in color range
		Select a color index-range to fill with the color in the
		palette

Actions:        The following actions may be invoked in the colormap field in
                the bottom of the slice window.

		&<Btn1Down>
		Fill LUT and colorfield at cursor position with the palette
		color. This will most likely influence the image.

		&<Btn2Down>
		Set the palette color equal to the LUT value at the cursor
		position. This will influence the color palette.

		&<Btn3Down>
		Replace a constant part of LUT values at the cursor position
		with the palette color.

		&Drag <Btn>
		Same as <Btn1Down>

See also:	'xshow(1)'

Author:		Otto Milvang

Revised:        Svein Bøe
Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
static void set_color_for_patch(data)
private_data *data;
#else /* FUNCPROTO */
static void set_color_for_patch(private_data *data)
#endif /* FUNCPROTO */
{
  /* Set color for pixel PATCH_PIXEL. Use ihscol[PATCH_COLOR_CELL] to feed
   * this to the colormap with XStoreColors().
   * PATCH_PIXEL is chosen outside the range of colorcells used for the
   * images.
   */
  Colormap cmap = data->map;

  /* First save the old colors. */
  InitColortablePixels(data->old_patch_red_cell,  1, PATCH_PIXEL,
		       ClassOfVisual(data->visual) == DirectColor ||
		       ClassOfVisual(data->visual) == TrueColor);
  XQueryColors(dpy_s, cmap, data->old_patch_red_cell,  1);

  if (ClassOfVisual(data->visual) == PseudoColor)
    data->ihscol[PATCH_COLOR_CELL].pixel = PATCH_PIXEL;
  else
    data->ihscol[PATCH_COLOR_CELL].pixel = RgbPixel(PATCH_PIXEL);

  data->ihscol[PATCH_COLOR_CELL].pad   = 0;
  data->ihscol[PATCH_COLOR_CELL].flags = DoRed | DoGreen | DoBlue;
  data->ihscol[PATCH_COLOR_CELL].red =
    (1 - scrollbar_init_value[R_BAR]) * 0x0ffff;
  data->ihscol[PATCH_COLOR_CELL].green =
    (1 - scrollbar_init_value[G_BAR]) * 0x0ffff;
  data->ihscol[PATCH_COLOR_CELL].blue =
    (1 - scrollbar_init_value[B_BAR]) * 0x0ffff;

  XStoreColors(dpy_s, cmap,     data->ihscol, 1);

} /* set_color_for_patch() */

#ifndef FUNCPROTO
static void CreateWidgets(private)
private_data *private;
#else /* FUNCPROTO */
static void CreateWidgets(private_data *private)
#endif /* FUNCPROTO */
{
  Dimension maxWidth = 0, sendToXshow_w = 0, sendToFile_w = 0,
            loadFromXshow_w = 0, loadFromFile_w = 0, range_w = 0, quit_w = 0,
            labelRgb_w = 0, stripe_w = 0, patch_w = 0,
            bar_w = 0, commandWidth = 0, color_w = 0;
  int sendToXshow_d = 0, sendToFile_d = 0, loadFromXshow_d = 0,
      loadFromFile_d = 0, range_d = 0, quit_d = 0, labelRgb_d = 0,
      stripe_d = 0, patch_d = 0, bar_d = 0, color_d = 0, colorIndex_d = 0;
  int j;
  XFontStruct *font;
  char *label;
  Widget composite, topLabel, sendToXshow, sendToFile, loadFromXshow,
         loadFromFile, range, quit, color, wid, wid2;
  String trans_stripe = "\
             <EnterWindow>:     WMColormap(Enter) notify()\n\
             <LeaveWindow>:     notify() WMColormap(Leave)\n\
             None <BtnDown>:	notify()\n\
             Ctrl <Btn1Down>:   nop()\n\
             Ctrl <Btn2Down>:   nop()\n\
             Ctrl <Btn3Down>:   nop()\n\
             Shift <Btn1Down>:  nop()\n\
             Shift <Btn2Down>:  nop()\n\
             Shift <Btn3Down>:  nop()\n";
  String trans_patch = "\
             <EnterWindow>:     WMColormap(Enter) notify()\n\
             <LeaveWindow>:     notify() WMColormap(Leave)\n\
             <BtnDown>:         nop()\n";

  composite = CW("sliceForm", formWidgetClass, private->toplevel, 
		 NULL);
  topLabel = CW("topLabel", labelWidgetClass, composite,
		XtNresizable, TRUE, NULL);

  /* Scrollbar widgets. */

  for(j=0; j<6; j++) {
    private->bar[j] = wid = CW(scrollbarNames[j], scrollbarWidgetClass,
			       composite,
			       XtNfromVert,  topLabel,
			       XtNfromHoriz, j > 0 ? private->bar[j-1] : NULL,
			       NULL);
    XawScrollbarSetThumb(wid, scrollbar_init_value[j], 1.0);
    XtAddCallback(wid, XtNscrollProc, xslice_scroll, (XtPointer) private);
    XtAddCallback(wid, XtNjumpProc,   xslice_jump,   (XtPointer) private);
  } 

  /* Scrollbar widget names. */
  wid2 = NULL;
  for(j=0; j<6; j++) {
    wid = CW(scrollLabelNames[j], labelWidgetClass, composite,
	     XtNfromVert,  private->bar[j],
	     XtNfromHoriz, wid2,
	     NULL);
    wid2 = wid;
  }

  /* Color patch. */

  wid = private->colorPatch =
    colorbar_widget(composite, "colorPatch", NULL, -1, 202, 80,
		    PATCH_PIXEL, PATCH_PIXEL,
		    private->visual, private->planes, "ColorPatch", NULL);

  /* Use full-color display, to make color number PATCH_PIXEL appear in
   * the patch, i.e. do not compress pixels [0..255] into [64..191]. */
  XtVaSetValues(wid, XtNfromVert,      topLabel,
                     XtNfromHoriz,     private->bar[B_BAR],
                     XtNdisplayMethod, 1,
		     XtNrange,         "dynamic",
		     NULL);

  XtOverrideTranslations(wid, XtParseTranslationTable(trans_patch));

  /* Patch label. */

  private->labelIhs = CW("labelIhs", labelWidgetClass, composite,
			 XtNlabel,     "I=0.7000 H=0.0000 S=0.8000",
			 XtNfromVert,  private->colorPatch,
			 XtNfromHoriz, private->bar[B_BAR],
			 NULL);
  private->labelRgb = CW("labelRgb", labelWidgetClass, composite,
			 XtNlabel,     "R=0.5800 G=0.5800 B=0.9400",
			 XtNfromVert,  private->labelIhs,
			 XtNfromHoriz, private->bar[B_BAR],
			 NULL);

  /* Command buttons. */

  sendToXshow = wid =
    CW("sendColtabToXshow", commandWidgetClass, composite,
       XtNfromVert,  private->labelRgb,
       XtNfromHoriz, private->bar[B_BAR],
       XtNresizable, TRUE,
       NULL);
  XtAddCallback(wid, XtNcallback, SendColtabToXshow, (XtPointer) private);

  sendToFile = wid =
    CW("sendColtabToFile", commandWidgetClass, composite,
       XtNfromVert,  sendToXshow,
       XtNfromHoriz, private->bar[B_BAR],
       XtNresizable, TRUE,
       NULL);
  XtAddCallback(wid, XtNcallback, SendColtabToFile, (XtPointer) private);
  
  loadFromXshow = wid =
    CW("loadColtabFromXshow", commandWidgetClass, composite,
       XtNfromVert,  sendToFile,
       XtNfromHoriz, private->bar[B_BAR],
       XtNresizable, TRUE,
       NULL);
  XtAddCallback(wid, XtNcallback, LoadColtabFromXshow, (XtPointer) private);

  loadFromFile = wid =
    CW("loadColtabFromFile", commandWidgetClass, composite,
       XtNfromVert,  loadFromXshow,
       XtNfromHoriz, private->bar[B_BAR],
       XtNresizable, TRUE,
       NULL);
  XtAddCallback(wid, XtNcallback, LoadColtabFromFile, (XtPointer) private);

  range = wid = CW("range",  commandWidgetClass, composite,
		   XtNfromVert,  loadFromFile,
		   XtNfromHoriz, private->bar[B_BAR],
		   XtNresizable, TRUE,
		   NULL);
  XtAddCallback(wid, XtNcallback, SetRange, (XtPointer) private);
  
  quit = wid = CW("quit", commandWidgetClass, composite,
		  XtNfromVert,  range,
		  XtNfromHoriz, private->bar[B_BAR],
		  XtNresizable, TRUE,
		  NULL);
  XtAddCallback(wid, XtNcallback, xslice_exit, (XtPointer) private);

  /* Color index label. */

  color = CW("color", labelWidgetClass, composite,
	     XtNfromVert,  quit,
	     XtNfromHoriz, private->bar[B_BAR],
	     XtNresizable, TRUE,
	     NULL);

  private->colorIndex = CW("colorIndex", labelWidgetClass, composite,
			   XtNfromVert,  quit,
			   XtNfromHoriz, color,
			   XtNresizable, TRUE,
			   NULL);

  /* Color stripe at the bottom. */

  wid = private->colorStripe =
    colorbar_widget(composite, "colorStripe", NULL, -1, 512, 64, 0, 255,
		    private->visual, private->planes, "Colorbar", NULL);

  XtVaSetValues(wid, XtNfromVert,  wid2,
		     XtNfromHoriz, NULL,                NULL);

  XtOverrideTranslations(wid, XtParseTranslationTable(trans_stripe));
  XtAddCallback(wid, XtNcallback, xslice_mark, (XtPointer) private);

  XtRealizeWidget(private->toplevel);

  ImageRedisplay((ImageWidget) private->colorPatch, UPDATE_MINMAX);

  /* Adjust widget geometry. */

  XtVaGetValues(sendToXshow, XtNfont,          &font,
		             XtNlabel,         &label,
		             XtNhorizDistance, &sendToXshow_d, NULL);
  sendToXshow_w = XTextWidth(font, label, strlen(label)) +
                  XTextWidth(font, "XX", strlen("XX"));

  XtVaGetValues(sendToFile, XtNfont,          &font,
		            XtNlabel,         &label,
		            XtNhorizDistance, &sendToFile_d, NULL);
  sendToFile_w  = XTextWidth(font, label, strlen(label)) +
                  XTextWidth(font, "XX", strlen("XX"));

  XtVaGetValues(loadFromXshow, XtNfont,          &font,
		               XtNlabel,         &label,
		               XtNhorizDistance, &loadFromXshow_d, NULL);
  loadFromXshow_w = XTextWidth(font, label, strlen(label)) +
                    XTextWidth(font, "XX", strlen("XX"));

  XtVaGetValues(loadFromFile, XtNfont,          &font,
		              XtNlabel,         &label,
		              XtNhorizDistance, &loadFromFile_d, NULL);
  loadFromFile_w = XTextWidth(font, label, strlen(label)) +
                   XTextWidth(font, "XX", strlen("XX"));

  XtVaGetValues(range, XtNfont,          &font,
		       XtNlabel,         &label,
		       XtNhorizDistance, &range_d, NULL);
  range_w = XTextWidth(font, label, strlen(label)) +
            XTextWidth(font, "XX", strlen("XX"));

  XtVaGetValues(quit, XtNfont,          &font,
		      XtNlabel,         &label, 
		      XtNhorizDistance, &quit_d, NULL);
  quit_w = XTextWidth(font, label, strlen(label)) +
           XTextWidth(font, "XX", strlen("XX"));

  commandWidth = MAX(MAX(sendToXshow_w + sendToXshow_d,
			 sendToFile_w + sendToFile_d),
		 MAX(loadFromXshow_w + loadFromXshow_d,
		     loadFromFile_w + loadFromFile_d));
  commandWidth = MAX(commandWidth, MAX(range_w + range_d, quit_w + quit_d));

  XtVaSetValues(sendToXshow,   XtNwidth, commandWidth, NULL);
  XtVaSetValues(sendToFile,    XtNwidth, commandWidth, NULL);
  XtVaSetValues(loadFromXshow, XtNwidth, commandWidth, NULL);
  XtVaSetValues(loadFromFile,  XtNwidth, commandWidth, NULL);
  XtVaSetValues(range,         XtNwidth, commandWidth, NULL);
  XtVaSetValues(quit,          XtNwidth, commandWidth, NULL);

  XtVaGetValues(color, XtNfont,          &font,
		       XtNlabel,         &label,
		       XtNhorizDistance, &color_d, NULL);
  color_w = XTextWidth(font, label, strlen(label)) +
            XTextWidth(font, "XX", strlen("XX"));

  XtVaGetValues(private->colorIndex, XtNfont,          &font,
		                     XtNhorizDistance, &colorIndex_d, NULL);
  private->colorIndex_w = XTextWidth(font, "WWW", strlen("WWW")) +
                          XTextWidth(font, "X", strlen("X"));
  XtVaSetValues(private->colorIndex, XtNwidth, private->colorIndex_w, NULL);

  XtVaGetValues(private->colorStripe, XtNwidth,         &stripe_w,
		                      XtNhorizDistance, &stripe_d, NULL);
  XtVaGetValues(private->colorPatch,  XtNwidth,         &patch_w,
		                      XtNhorizDistance, &patch_d,    NULL);
  XtVaGetValues(private->bar[I_BAR],  XtNwidth,         &bar_w,
		                      XtNhorizDistance, &bar_d,      NULL);
  XtVaGetValues(private->labelRgb,    XtNfont,          &font,
		                      XtNhorizDistance, &labelRgb_d, NULL);
  labelRgb_w = XTextWidth(font, "R=0.5800 G=0.5800 B=0.9400",
	                 strlen("R=0.5800 G=0.5800 B=0.9400")) +
	       XTextWidth(font, "XX", strlen("XX"));

  maxWidth = MAX(MAX(patch_w + patch_d, labelRgb_w + labelRgb_d),
		 MAX(commandWidth,
		     color_w + color_d +
		     private->colorIndex_w + colorIndex_d));
  maxWidth = MAX(6 * bar_w + 5 * bar_d + maxWidth, stripe_w + stripe_d);

  XtVaSetValues(topLabel, XtNwidth, maxWidth, NULL);

} /* CreateWidgets() */

#ifndef FUNCPROTO
void xslice(wid, image, imageCmap, planes)
Widget wid;
IMAGE image;
Colormap imageCmap;
int planes;
#else /* FUNCPROTO */
void xslice(Widget wid, IMAGE image, Colormap imageCmap, int planes)
#endif /* FUNCPROTO */
{
  private_data *private;
  Window top_win;
  char name[80], *title, *pixname;
  Visual *visual;

  ENTER_FUNCTION_DEBUG("Xslice.c: xslice");

  dpy_s  = XtDisplay(wid);

  XtVaGetValues(wid, XtNvisual,       &visual,
		     XtNimagePixtype, &pixname, NULL);

  if (pixname && (!strcmp(pixname, ImageComplexPixel) ||
		  !strcmp(pixname, ImageDoubleComplexPixel))) {
    Error(2, "xslice(): Pixeltype can not be complex.\n");
    return;
  }

  CopyToPrivate(wid, image, planes, imageCmap, visual, &private);

  private->toplevel =
    (Widget) CreatePopTopShell("sliceShell", TopLevelWidget(wid));
  AddAction(private->toplevel, "QuitXslice", quit_slice, 0);

  /* Make a piecewise constant colortable for the initial colorstripe. */
  MakeColortab(private->col, visual);

  /* Set the patch-color into PATCH_COLOR_CELL of imageCmap, with
   * pixelvalue PATCH_PIXEL and predefined rgb values.
   */
  set_color_for_patch(private);

  /* Fill the pixel value range [64, 191] of private->map with every other
   * color from the piecewise constant private->col.
   */
  fillmap(private);

  /* Color_max() refers to imageCmap, including the changes made above in
   * in set_color_for_patch and fillmap. */
  Color_set((ImageWidget) wid, Color_max());

  CreateWidgets(private);

  XtPopup(private->toplevel, XtGrabNone);

  top_win = XtWindow(private->toplevel);

  (void) strcpy(name, "Slice ");
  XtVaGetValues(wid, XtNimageTitle, &title, NULL);
  (void) strcat(name, title);
  XmbSetWMProperties(dpy_s, top_win, name, name,
                      NULL, 0, NULL, NULL, NULL);
  (void) XSetWMProtocols(dpy_s, top_win, &wm_delete_window, 1);

  XFlush(dpy_s);

  LEAVE_FUNCTION_DEBUG("Xslice.c: xslice");
}
