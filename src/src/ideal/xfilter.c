

/*C*

________________________________________________________________

        xfilter
        $Id$
        Copyright 1994, Blab, UiO
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

#ifdef MAIN

#include <xite/includes.h>
#include <math.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xfwf/Prompt.h>
#include <X11/Xaw/Scrollbar.h>
#include <xite/ShellWids.h>
#include <xite/biff.h>
#include <xite/Image.h>
#include <xite/ximage.h>
#include <xite/message.h>
#include <xite/convert.h>
#include <xite/arithmetic.h>
#include <xite/statistics.h>
#include <xite/fht.h>
#include <xite/geometry.h>
#include <xite/Xdialog.h>
#include <xite/readarg.h>
#include <xite/mallocp.h>
#include XITE_STDIO_H
#include XITE_MALLOC_H
#include XITE_STRING_H
#include <xite/ideal.h>
#include <xite/debug.h>

typedef enum {
  INIMAGE,
  F_INIMAGE,
  FILTIMAGE,
  F_FILTIMAGE,
  FILTCROSS,
  OUTIMAGE,
  F_OUTIMAGE,
  DIFFIMAGE
} image_widget_number;
static int num_images_s = DIFFIMAGE + 1;
static layout_image *layout_images_s;
#define image_wid(n) (layout_images_s ? \
		      WidgetOfLayout(layout_images_s[n]) : NULL)

typedef enum {
  LOW_SCROLL,
  HIGH_SCROLL,
  ORDER_SCROLL,
  F_INIMAGE_EPS_SCROLL,
  F_FILTIMAGE_EPS_SCROLL,
  F_OUTIMAGE_EPS_SCROLL,
  CROSS_MIN_SCROLL,
  CROSS_MAX_SCROLL
} scroll_widget_number;
static int num_scrolls_s = CROSS_MAX_SCROLL + 1;
static layout_scroll *layout_scrolls_s;
#define scroll_wid(n) (layout_scrolls_s ? \
		       WidgetOfLayout(layout_scrolls_s[n]) : NULL)

typedef enum {
  LOW_PROMPT,
  HIGH_PROMPT,
  ORDER_PROMPT,
  F_INIMAGE_EPS_PROMPT,
  F_FILTIMAGE_EPS_PROMPT,
  F_OUTIMAGE_EPS_PROMPT,
  CROSS_MIN_PROMPT,
  CROSS_MAX_PROMPT
} prompt_widget_number;
static int num_prompts_s = CROSS_MAX_PROMPT + 1;
static layout_prompt *layout_prompts_s;
#define prompt_wid(n) (layout_prompts_s ? \
		       WidgetOfLayout(layout_prompts_s[n]) : NULL)

typedef enum {
  READ_INPUT,
  SAVE_OUTPUT,
  SAVE_FILTER,
  QUIT
} command_widget_number;
static int num_commands_s = QUIT + 1;
static layout_command *layout_commands_s;
#define command_wid(n) (layout_commands_s ? \
			WidgetOfLayout(layout_commands_s[n]) : NULL)

typedef enum {
  FILTLABEL,
  LOW_LABEL,
  HIGH_LABEL,
  ORDER_LABEL,
  WINDLABEL,
  RMS_DIFF,
  F_INIMAGE_EPS_LABEL,
  F_FILTIMAGE_EPS_LABEL,
  F_OUTIMAGE_EPS_LABEL,
  CROSS_MIN_LABEL,
  CROSS_MAX_LABEL
} label_widget_number;
static int num_labels_s = CROSS_MAX_LABEL + 1;
static layout_label *layout_labels_s;
#define label_wid(n) (layout_labels_s ? \
		      WidgetOfLayout(layout_labels_s[n]) : NULL)

typedef enum {
  FILTMENU,
  WINDMENU
} menu_widget_number;
static int num_menus_s = WINDMENU + 1;
static layout_menu *layout_menus_s;
#define menu_wid(n) (layout_menus_s ? \
		     WidgetOfLayout(layout_menus_s[n]) : NULL)

static int num_entries_filt_s = EXP_LP + 1;
static layout_menu_entry  *layout_entries_filt_s;
#define filt_entry_wid(type) \
  (WidgetOfMenuEntry(EntriesOfMenuLayout(layout_menus_s[FILTMENU])[type]))

static int num_entries_wind_s = HANNING + 1;
static layout_menu_entry  *layout_entries_wind_s;
#define wind_entry_wid(type) \
  (WidgetOfMenuEntry(EntriesOfMenuLayout(layout_menus_s[WINDMENU])[type]))
           
static double      cut_low_s, cut_high_s;
static int         inband_xsize_s, inband_ysize_s, filter_order_s;
static filter_type filter_type_s;
static window_type window_type_s;
static double      disp_scale_s;
static double      cross_min_min_s, cross_max_max_s, cross_min_s, cross_max_s;
static double      minimum_legal_frequency_s = 0.001;
static double      f_inimage_eps_s   = 1.0e-8,
                   f_filtimage_eps_s = 1.0e-8,
                   f_outimage_eps_s  = 1.0e-8;
static double      eps_max_s, eps_min_s, log_min_s = DOUBLE_MAX;
static Boolean     f_inimage_log_s, f_filtimage_log_s, f_outimage_log_s;

static layout      *layouts_s; /* Array for all layout structures. */
static Widget      toplevel_s;
static int         num_widgets_s, num_image_widgets_s;
static int         fast_s, wdiff_s, win_s, wfin_s, wfilt_s, wffilt_s,
                   wout_s, wfout_s, wcross_s, rms_s = 1;

/* A structure to hold auxiliary information for each image widget. */
typedef struct {
  IBAND band;                  /* Any pixeltype. */
  Boolean redisplay;           /* Needs redisplay? */
  Boolean show;                /* Whether or not to display this widget. */
} _bandData_,  *bandData;
static bandData *band_data_s;  /* Array of these structures. */

typedef struct {
  Boolean widgetIn, widgetFourIn, widgetFilt, widgetFourFilt, widgetOut,
    widgetFourOut, widgetCross, widgetDiff, rmsCalc,
    linearFourIn, linearFourFilt, linearFourOut;
  float crossMinMin, crossMaxMax, crossMin, crossMax, epsMin, epsMax,
    cutLow, cutHigh, dispScale, logMin;
  int filterOrder;
  char *filterType, *windowType;
} AppData;

static AppData resources_result_s;

static XtResource resources_s[] = {
  {"widgetIn",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetIn),
   XtRString,
   (XtPointer) "True",
  },
  {"widgetFourIn",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetFourIn),
   XtRString,
   (XtPointer) "False",
  },
  {"widgetFilt",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetFilt),
   XtRString,
   (XtPointer) "False",
  },
  {"widgetFourFilt",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetFourFilt),
   XtRString,
   (XtPointer) "True",
  },
  {"widgetOut",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetOut),
   XtRString,
   (XtPointer) "True",
  },
  {"widgetFourOut",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetFourOut),
   XtRString,
   (XtPointer) "False",
  },
  {"widgetCross",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetCross),
   XtRString,
   (XtPointer) "True",
  },
  {"widgetDiff",
   "WidgetShow",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, widgetDiff),
   XtRString,
   (XtPointer) "False",
  },
  {"crossMinMin",
   "CrossMin",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, crossMinMin),
   XtRString,
   (XtPointer) "-200.0",
  },
  {"crossMaxMax",
   "CrossMax",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, crossMaxMax),
   XtRString,
   (XtPointer) "50.0",
  },
  {"crossMin",
   "CrossMin",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, crossMin),
   XtRString,
   (XtPointer) "-80.0",
  },
  {"crossMax",
   "CrossMax",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, crossMax),
   XtRString,
   (XtPointer) "10.0",
  },
  {"epsMin",
   "Eps",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, epsMin),
   XtRString,
   (XtPointer) "1.0e-20",
  },
  {"epsMax",
   "Eps",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, epsMax),
   XtRString,
   (XtPointer) "0.5",
  },
  {"cutLow",
   "cutFreq",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, cutLow),
   XtRString,
   (XtPointer) "0.2",
  },
  {"cutHigh",
   "cutFreq",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, cutHigh),
   XtRString,
   (XtPointer) "0.5",
  },
  {"dispScale",
   "DispScale",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, dispScale),
   XtRString,
   (XtPointer) "10.0",
  },
  {"logMin",
   "LogMin",
   XtRFloat,
   sizeof(float),
   XtOffsetOf(AppData, logMin),
   XtRString,
   NULL,
  },
  {"filterOrder",
   "FilterOrder",
   XtRInt,
   sizeof(int),
   XtOffsetOf(AppData, filterOrder),
   XtRString,
   (XtPointer) "41",
  },
  {"filterType",
   "FilterType",
   XtRString,
   sizeof(String),
   XtOffsetOf(AppData, filterType),
   XtRString,
   (XtPointer) "lp",
  },
  {"rmsCalc",
   "RmsCalc",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, rmsCalc),
   XtRString,
   (XtPointer) "True",
  },
  {"windowType",
   "WindowType",
   XtRString,
   sizeof(String),
   XtOffsetOf(AppData, windowType),
   XtRString,
   (XtPointer) "hm",
  },
  {"linearFourIn",
   "Linear",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, linearFourIn),
   XtRString,
   (XtPointer) "False",
  },
  {"linearFourFilt",
   "Linear",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, linearFourFilt),
   XtRString,
   (XtPointer) "False",
  },
  {"linearFourOut",
   "Linear",
   XtRBoolean,
   sizeof(Boolean),
   XtOffsetOf(AppData, linearFourOut),
   XtRString,
   (XtPointer) "False",
  },
};

static XrmOptionDescRec opTable_s[] = {
  /* Application resources */
  { "-win",     ".widgetIn",       XrmoptionNoArg,  (XtPointer) "False" },
  { "-wfin",    ".widgetFourIn",   XrmoptionNoArg,  (XtPointer) "True"  },
  { "-wfilt",   ".widgetFilt",     XrmoptionNoArg,  (XtPointer) "True"  },
  { "-wffilt",  ".widgetFourFilt", XrmoptionNoArg,  (XtPointer) "False" },
  { "-wout",    ".widgetOut",      XrmoptionNoArg,  (XtPointer) "False" },
  { "-wfout",   ".widgetFourOut",  XrmoptionNoArg,  (XtPointer) "True"  },
  { "-wcross",  ".widgetCross",    XrmoptionNoArg,  (XtPointer) "False" },
  { "-wdiff",   ".widgetDiff",     XrmoptionNoArg,  (XtPointer) "True"  },
  { "-cminmin", ".crossMinMin",    XrmoptionSepArg, (XtPointer) NULL    },
  { "-cmaxmax", ".crossMaxMax",    XrmoptionSepArg, (XtPointer) NULL    },
  { "-cmin",    ".crossMin",       XrmoptionSepArg, (XtPointer) NULL    },
  { "-cmax",    ".crossMax",       XrmoptionSepArg, (XtPointer) NULL    },
  { "-emin",    ".epsMin",         XrmoptionSepArg, (XtPointer) NULL    },
  { "-emax",    ".epsMax",         XrmoptionSepArg, (XtPointer) NULL    },
  { "-low",     ".cutLow",         XrmoptionSepArg, (XtPointer) NULL    },
  { "-high",    ".cutHigh",        XrmoptionSepArg, (XtPointer) NULL    },
  { "-scale",   ".dispScale",      XrmoptionSepArg, (XtPointer) NULL    },
  { "-logmin",  ".logMin",         XrmoptionSepArg, (XtPointer) NULL    },
  { "-order",   ".filterOrder",    XrmoptionSepArg, (XtPointer) NULL    },
  { "-ftype",   ".filterType",     XrmoptionSepArg, (XtPointer) NULL    },
  { "-rms",     ".rmsCalc",        XrmoptionNoArg,  (XtPointer) "False" },
  { "-wtype",   ".windowType",     XrmoptionSepArg, (XtPointer) NULL    },
  { "-ilin",    ".linearFourIn",   XrmoptionNoArg,  (XtPointer) "False" },
  { "-flin",    ".linearFourFilt", XrmoptionNoArg,  (XtPointer) "False" },
  { "-olin",    ".linearFourOut",  XrmoptionNoArg,  (XtPointer) "False" },
};

#ifndef FUNCPROTO
static void compress_and_shift_spec(wid_num)
image_widget_number wid_num;
#else /* FUNCPROTO */
static void compress_and_shift_spec(image_widget_number wid_num)
#endif /* FUNCPROTO */
{
  bandData data         = band_data_s[wid_num];
  layout_image l_image  = layout_images_s[wid_num];
  layout_image_part *ip = &(l_image->image);
  IBAND data_band, disp_band;
  double eps;

  if (!data->show) return;

  data_band = data->band;
  disp_band = ip->img[1];

  switch((int) wid_num) {
  case F_INIMAGE:
    eps   = f_inimage_eps_s;
    break;
  case F_FILTIMAGE:
    eps   = f_filtimage_eps_s;
    break;
  case F_OUTIMAGE:
    eps   = f_outimage_eps_s;
    break;
  default:
    Error(2, "compress_and_shift_spec: Illegal widget number.\n");
    break;
  } /* switch */

  /* Compute power spectrum from Hartley transform of general image. */
  fhtPower((IR_BAND) data_band, (IR_BAND) disp_band);

  logarithm(disp_band, disp_band, eps, 0.0, disp_scale_s, log_min_s);
  shift_band(disp_band, disp_band, inband_xsize_s / 2,inband_ysize_s / 2);

} /* compress_and_shift_spec() */

#ifndef FUNCPROTO
static void set_scrollbar_thumb(wid, value, min, max, logarithmic)
Widget wid;
double value, min, max;
Boolean logarithmic;
#else /* FUNCPROTO */
static void set_scrollbar_thumb(Widget wid, double value, double min, double max, Boolean logarithmic)
#endif /* FUNCPROTO */
{
  if (logarithmic) {
    value = log10(value);
    min   = log10(min);
    max   = log10(max);
  }

  XawScrollbarSetThumb(wid, (float) (value - min) / (max - min), 0.02);

  return;

} /* set_scrollbar_thumb() */

#ifndef FUNCPROTO
static void Quit(wid)
Widget wid;
#else /* FUNCPROTO */
static void Quit(Widget wid)
#endif /* FUNCPROTO */
{
  if (ConfirmationBox(TopLevelWidget(wid), "Really quit?", "No", "Yes",
			 "xfilter - confirmation", "xfilter - conf") != 1)
    return;

  XtDestroyApplicationContext(XtWidgetToApplicationContext(wid));
  exit(0);

} /* Quit() */

#ifndef FUNCPROTO
static void number_format(txt, places, before, after)
char *txt;
int *places, *before, *after;
#else /* FUNCPROTO */
static void number_format(char *txt, int *places, int *before, int *after)
#endif /* FUNCPROTO */
{
  char *ptr;

  *places = strlen(txt);
  ptr = strchr(txt, '.');
  if (ptr) {
    *before = ptr - txt;
    *after  = *places - *before - 1;
  } else {
    *before = *places;
    *after  = 0;
  }

  return;

} /* number_format() */

#ifndef FUNCPROTO
static void set_gen_number(wid, number, format, precision)
Widget wid;
double number;
char *format;
Boolean precision;
#else /* FUNCPROTO */
static void set_gen_number(Widget wid, double number, char *format, Boolean precision)
#endif /* FUNCPROTO */
{
  char *s, f[15], *t;
  int places, before, after;
  int   i_num = (int) (number + 0.5);

  XtVaGetValues(wid, XtNtext, &s, NULL);

  if (s) number_format(s, &places, &before, &after);
  else if (strcmp(format, "f") || strcmp(format, "e")) {
    after  = 6;
    places = after + 6;
  } else {
    after  = 0;
    places = 8;
  }

  if (precision) sprintf(f, "%%%d.%d", places, after);
  else sprintf(f, "%%%d", places);
  (void) strcat(f, format);

  t = (char *) malloc(50);
  if      (strcmp(format, "f")  || strcmp(format, "e"))  sprintf(t, f, number);
  else sprintf(t, f, i_num);

  XtVaSetValues(wid, XtNtext, t, NULL);

  return;

} /* set_gen_number() */

#ifndef FUNCPROTO
static void set_double_number(wid, number)
Widget wid;
double number;
#else /* FUNCPROTO */
static void set_double_number(Widget wid, double number)
#endif /* FUNCPROTO */
{
  set_gen_number(wid, number, "f", True);
  return;

} /* set_double_number() */

#ifndef FUNCPROTO
static void set_double_exp_number(wid, number)
Widget wid;
double number;
#else /* FUNCPROTO */
static void set_double_exp_number(Widget wid, double number)
#endif /* FUNCPROTO */
{
  set_gen_number(wid, (double) number, "e", False);
  return;

} /* set_double_exp_number() */

#ifndef FUNCPROTO
static void set_int_number(wid, number)
Widget wid;
int number;
#else /* FUNCPROTO */
static void set_int_number(Widget wid, int number)
#endif /* FUNCPROTO */
{
  int places;
  char *s, format[50], *t;

  XtVaGetValues(wid, XtNtext, &s, NULL);

  if (s) places = strlen(s);
  else places = 8;

  sprintf(format, "%%%dd", places);
  t = (char *) malloc(50);
  sprintf(t, format, number);
  XtVaSetValues(wid, XtNtext, t, NULL);

  return;

} /* set_int_number() */

#ifndef FUNCPROTO
static void set_number(wid_number)
prompt_widget_number wid_number;
#else /* FUNCPROTO */
static void set_number(prompt_widget_number wid_number)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("xfilter.c: set_number");

  switch(wid_number) {
  case LOW_PROMPT:
    set_double_number(prompt_wid(wid_number), cut_low_s);
    break;
  case HIGH_PROMPT:
    set_double_number(prompt_wid(wid_number), cut_high_s);
    break;
  case ORDER_PROMPT:
    set_int_number(prompt_wid(wid_number), filter_order_s);
    break;
  case CROSS_MIN_PROMPT:
    set_double_number(prompt_wid(wid_number), cross_min_s);
    break;
  case CROSS_MAX_PROMPT:
    set_double_number(prompt_wid(wid_number), cross_max_s);
    break;
  case F_INIMAGE_EPS_PROMPT:
    set_double_exp_number(prompt_wid(wid_number), f_inimage_eps_s);
    break;
  case F_FILTIMAGE_EPS_PROMPT:
    set_double_exp_number(prompt_wid(wid_number), f_filtimage_eps_s);
    break;
  case F_OUTIMAGE_EPS_PROMPT:
    set_double_exp_number(prompt_wid(wid_number), f_outimage_eps_s);
    break;
  }

  LEAVE_FUNCTION_DEBUG("xfilter.c: set_number");

} /* set_number() */

#ifndef FUNCPROTO
static void Make_input_band()
#else /* FUNCPROTO */
static void Make_input_band(void)
#endif /* FUNCPROTO */
{
  int xsize = -1, ysize = -1;
  BiffStatus status = 0;
  bandData data         = band_data_s[INIMAGE];
  layout_image l_image  = layout_images_s[INIMAGE];
  layout_image_part *ip = &(l_image->image);
  IBAND disp_band;

  /* Make input image and displayable band. */

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_input_band");

  data->redisplay = False;

  if (!ip->img || !ip->img[1]) {
    Error(2, "Missing inimage.\n");
    exit(1);
  }

  disp_band = ip->img[1];
  if (disp_band) {
    xsize = Ixsize(disp_band);
    ysize = Iysize(disp_band);
  }

  if (xsize == inband_xsize_s && ysize == inband_ysize_s &&
      data->band && (Ipixtyp(disp_band) == Ipixtyp(data->band))) {

    /* It is safe to reuse the memory allocated for data->band again.
     * Same pixeltypes, so just copy new pixels.
     */

    if ((status = Icopy_band(disp_band, data->band)) != Iok)
      InformationBox(toplevel_s, Ierr_message(status),
		     "xfilter - info", "xfilter - info");

    /* The disp_band variable did not change. The SetValues method will
     * not redisplay the image, unless the extrema have changed, so force
     * an explicit redisplay.
     */
    data->redisplay = True;
  } else if (xsize == inband_xsize_s && ysize == inband_ysize_s && data->band){

    /* It is safe to reuse the memory allocated for data->band again.
     * Different pixeltypes, so convert band.
     */

    if ((status = convertBand(disp_band, data->band)) != Iok) {
      Error(2, "Error in convertBand.\n");
      exit(1);
    }
    
  } else {
    /* Either data->band has not been allocated (or already been freed),
     * or sizes don't match. Allocate new memory.
     */

    Idel_band(&data->band);

    data->band = mkConvertBand(disp_band, Ireal_typ);

    if (!data->band) InformationBox(toplevel_s, "Could not convert band",
				    "xfilter info", "xfilter info");

    inband_xsize_s = xsize;
    inband_ysize_s = ysize;
  }

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_input_band");

  return;

} /* Make_input_band() */

#ifndef FUNCPROTO
static void free_and_or_allocate(wid_num, data_pt, disp_pt, title)
image_widget_number wid_num;
IPIXTYP data_pt, disp_pt;
char *title;
#else /* FUNCPROTO */
static void free_and_or_allocate(image_widget_number wid_num, IPIXTYP data_pt, IPIXTYP disp_pt, char *title)
#endif /* FUNCPROTO */
{
  int xsize = -1, ysize = -1;
  bandData data         = band_data_s[wid_num];
  layout_image l_image  = layout_images_s[wid_num];
  layout_image_part *ip = &(l_image->image);

  ENTER_FUNCTION_DEBUG("xfilter.c: free_and_or_allocate");

  data->redisplay = False;

  if (ip->img && ip->img[1]) {
    xsize = Ixsize(ip->img[1]);
    ysize = Iysize(ip->img[1]);
  }

  if (!ip->img || xsize != inband_xsize_s || ysize != inband_ysize_s) {
    /* Must allocate new memory. Free old memory. */

    Idel_image(&(ip->img));   /* Display band and image. */
    Idel_band(&(data->band)); /* Data band. */

    if (disp_pt != Iunknown_typ)
      ip->img    = Imake_image(1, title, disp_pt,
			       inband_xsize_s, inband_ysize_s);

    if (data_pt != Iunknown_typ)
      data->band = Imake_band(data_pt, inband_xsize_s, inband_ysize_s);
  }

  LEAVE_FUNCTION_DEBUG("xfilter.c: free_and_or_allocate");

  return;

} /* free_and_or_allocate() */

#ifndef FUNCPROTO
static void Make_input_four()
#else /* FUNCPROTO */
static void Make_input_four(void)
#endif /* FUNCPROTO */
{
  bandData indata          = band_data_s[INIMAGE],
           findata         = band_data_s[F_INIMAGE];
  layout_image fl_image  = layout_images_s[F_INIMAGE];
  layout_image_part *fip = &(fl_image->image);

  /* Make Fourier transform of input image and displayable band. */

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_input_four");

  free_and_or_allocate(F_INIMAGE, Ireal_typ, Ireal_typ, "log|FFT(input)|");

  /* Use Hartley transform because it's faster than Fourier. */
  fht2d((IR_BAND) indata->band, (IR_BAND) findata->band, 1);

  /* Force redisplay if fdata->d_band has not changed. */
  findata->redisplay = True;

  compress_and_shift_spec(F_INIMAGE);

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_input_four");

  return;

} /* Make_input_four() */

#ifndef FUNCPROTO
static void update_sensitive()
#else /* FUNCPROTO */
static void update_sensitive(void)
#endif /* FUNCPROTO */
{
  if (!scroll_wid(LOW_SCROLL)) return;

  ENTER_FUNCTION_DEBUG("xfilter.c: update_sensitive");

  switch (filter_type_s) {
  case LOWPASS:
      XtVaSetValues(scroll_wid(LOW_SCROLL),   XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(LOW_LABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(LOW_PROMPT),   XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(HIGH_SCROLL),  XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(HIGH_LABEL),    XtNsensitive, False, NULL);
      XtVaSetValues(prompt_wid(HIGH_PROMPT),  XtNsensitive, False, NULL);
      XtVaSetValues(scroll_wid(ORDER_SCROLL), XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(ORDER_LABEL),   XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(ORDER_PROMPT), XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(FILTMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(FILTLABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(WINDMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(WINDLABEL),     XtNsensitive, True,  NULL);
      break;
  case LOWPASS_IDEAL:
      XtVaSetValues(scroll_wid(LOW_SCROLL),   XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(LOW_LABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(LOW_PROMPT),   XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(HIGH_SCROLL),  XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(HIGH_LABEL),    XtNsensitive, False, NULL);
      XtVaSetValues(prompt_wid(HIGH_PROMPT),  XtNsensitive, False, NULL);
      XtVaSetValues(scroll_wid(ORDER_SCROLL), XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(ORDER_LABEL),   XtNsensitive, False, NULL);
      XtVaSetValues(prompt_wid(ORDER_PROMPT), XtNsensitive, False, NULL);
      XtVaSetValues(menu_wid(FILTMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(FILTLABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(WINDMENU),       XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(WINDLABEL),     XtNsensitive, False, NULL);
      break;
  case BUTTER_LP:
      XtVaSetValues(scroll_wid(LOW_SCROLL),   XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(LOW_LABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(LOW_PROMPT),   XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(HIGH_SCROLL),  XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(HIGH_LABEL),    XtNsensitive, False, NULL);
      XtVaSetValues(prompt_wid(HIGH_PROMPT),  XtNsensitive, False, NULL);
      XtVaSetValues(scroll_wid(ORDER_SCROLL), XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(ORDER_LABEL),   XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(ORDER_PROMPT), XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(FILTMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(FILTLABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(WINDMENU),       XtNsensitive, False,  NULL);
      XtVaSetValues(label_wid(WINDLABEL),     XtNsensitive, False,  NULL);
      break;
  case EXP_LP:
      XtVaSetValues(scroll_wid(LOW_SCROLL),   XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(LOW_LABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(LOW_PROMPT),   XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(HIGH_SCROLL),  XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(HIGH_LABEL),    XtNsensitive, False, NULL);
      XtVaSetValues(prompt_wid(HIGH_PROMPT),  XtNsensitive, False, NULL);
      XtVaSetValues(scroll_wid(ORDER_SCROLL), XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(ORDER_LABEL),   XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(ORDER_PROMPT), XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(FILTMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(FILTLABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(WINDMENU),       XtNsensitive, False,  NULL);
      XtVaSetValues(label_wid(WINDLABEL),     XtNsensitive, False,  NULL);
      break;
  case HIGHPASS:
      XtVaSetValues(scroll_wid(LOW_SCROLL),   XtNsensitive, False, NULL);
      XtVaSetValues(label_wid(LOW_LABEL),     XtNsensitive, False, NULL);
      XtVaSetValues(prompt_wid(LOW_PROMPT),   XtNsensitive, False, NULL);
      XtVaSetValues(scroll_wid(HIGH_SCROLL),  XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(HIGH_LABEL),    XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(HIGH_PROMPT),  XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(ORDER_SCROLL), XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(ORDER_LABEL),   XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(ORDER_PROMPT), XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(FILTMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(FILTLABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(WINDMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(WINDLABEL),     XtNsensitive, True,  NULL);
      break;
  case BANDPASS:
  case BANDSTOP:
      XtVaSetValues(scroll_wid(LOW_SCROLL),   XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(LOW_LABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(LOW_PROMPT),   XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(HIGH_SCROLL),  XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(HIGH_LABEL),    XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(HIGH_PROMPT),  XtNsensitive, True,  NULL);
      XtVaSetValues(scroll_wid(ORDER_SCROLL), XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(ORDER_LABEL),   XtNsensitive, True,  NULL);
      XtVaSetValues(prompt_wid(ORDER_PROMPT), XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(FILTMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(FILTLABEL),     XtNsensitive, True,  NULL);
      XtVaSetValues(menu_wid(WINDMENU),       XtNsensitive, True,  NULL);
      XtVaSetValues(label_wid(WINDLABEL),     XtNsensitive, True,  NULL);
      break;
  default: Error(2, "Unknown filter type.\n");
      break;
  }

  LEAVE_FUNCTION_DEBUG("xfilter.c: update_sensitive");

} /* update_sensitive() */

#ifndef FUNCPROTO
static void update_filter_menulabel()
#else /* FUNCPROTO */
static void update_filter_menulabel(void)
#endif /* FUNCPROTO */
{
  char *label;

  if (!menu_wid(FILTMENU)) return;
  if (filter_type_s < LOWPASS || filter_type_s > EXP_LP) return;

  ENTER_FUNCTION_DEBUG("xfilter.c: update_filter_menulabel");

  switch (filter_type_s) {
  case LOWPASS:
  case LOWPASS_IDEAL:
  case BUTTER_LP:
  case EXP_LP:
  case HIGHPASS:
  case BANDPASS:
  case BANDSTOP:
    XtVaGetValues(filt_entry_wid(filter_type_s), XtNlabel, &label, NULL);
    break;
  default: Error(2, "Unknown filter type.\n");
    break;
  }

  XtVaSetValues(menu_wid(FILTMENU), XtNlabel, label, NULL);

  LEAVE_FUNCTION_DEBUG("xfilter.c: update_filter_menulabel");

} /* update_filter_menulabel() */

#ifndef FUNCPROTO
static void update_window_menulabel()
#else /* FUNCPROTO */
static void update_window_menulabel(void)
#endif /* FUNCPROTO */
{
  char *label;

  if (!menu_wid(WINDMENU)) return;
  if (window_type_s < RECTANGLE || window_type_s > HANNING) return;

  ENTER_FUNCTION_DEBUG("xfilter.c: update_window_menulabel");

  switch (window_type_s) {
  case RECTANGLE:
  case BARTLETT:
  case TRIANGLE:
  case HAMMING:
  case HANNING:
    XtVaGetValues(wind_entry_wid(window_type_s), XtNlabel, &label, NULL);
    break;
  default: Error(2, "Unknown window type.\n");
    break;
  }

  XtVaSetValues(menu_wid(WINDMENU), XtNlabel, label, NULL);

  LEAVE_FUNCTION_DEBUG("xfilter.c: update_window_menulabel");

} /* update_window_menulabel() */

#ifndef FUNCPROTO
static void update_rms()
#else /* FUNCPROTO */
static void update_rms(void)
#endif /* FUNCPROTO */
{
  double val;
  char label[80];
  IBAND inband; 
  IBAND outband;

  if (!band_data_s) return;
  if (!rms_s) return;

  inband  = band_data_s[INIMAGE]->band;
  outband = layout_images_s[OUTIMAGE]->image.img[1];
  val     = rms(inband, outband, 0);

  if (val < 0) Error(2, "Error in rms processing.\n");

  sprintf(label, "%s%f", "Input to output rms difference: ", val);
  XtVaSetValues(label_wid(RMS_DIFF), XtNlabel, label, NULL);

} /* update_rms() */

#ifndef FUNCPROTO
static void Make_filter()
#else /* FUNCPROTO */
static void Make_filter(void)
#endif /* FUNCPROTO */
{
  double d_scale = 1.0;
  int fsize = 0;
  bandData data         = band_data_s[FILTIMAGE];
  layout_image l_image  = layout_images_s[FILTIMAGE];
  layout_image_part *ip = &(l_image->image);

  /* Make filter, and displayable band. */

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_filter");

  free_and_or_allocate(FILTIMAGE, Ireal_typ, Ireal_typ, "Filter");
  d_scale = (double) inband_xsize_s * (double) inband_ysize_s;

  if (filter_order_s > inband_xsize_s || filter_order_s > inband_ysize_s) {
    int min = inband_xsize_s;

    if (min > inband_ysize_s) min = inband_ysize_s;
    if (filter_order_s > min) filter_order_s = min;

    if (prompt_wid(ORDER_PROMPT)) {
      set_number(ORDER_PROMPT);
      set_scrollbar_thumb(scroll_wid(ORDER_SCROLL), (double) filter_order_s,
			  1.0, (double) min, False);
    }
  }

  fsize = filter_order_s;

  switch (filter_type_s) {
  case LOWPASS_IDEAL:
  case BUTTER_LP:
  case EXP_LP:
    /* Use Hartley transform because it's faster than Fourier. */

    fht2d((IR_BAND) band_data_s[F_FILTIMAGE]->band, (IR_BAND) data->band, 0);
    break;
  case LOWPASS:
    lowpass(data->band, cut_low_s,   filter_order_s,  window_type_s);
    break;
  case HIGHPASS:
    highpass(data->band, cut_high_s, &filter_order_s, window_type_s);
    break;
  case BANDPASS:
    bandpass(data->band, cut_low_s, cut_high_s, filter_order_s, window_type_s);
    break;
  case BANDSTOP:
    bandstop(data->band, cut_low_s, cut_high_s, &filter_order_s,window_type_s);
    break;
  default: Error(2, "Unknown filter type.\n");
    break;
  }

  if (fsize != filter_order_s) {
    int min = inband_xsize_s;

    if (min > inband_ysize_s) min = inband_ysize_s;

    if (prompt_wid(ORDER_PROMPT)) {
      set_number(ORDER_PROMPT);
      set_scrollbar_thumb(scroll_wid(ORDER_SCROLL), (double) filter_order_s,
			  1.0, (double) min, False);
    }
  }

  if (filter_type_s != LOWPASS_IDEAL && filter_type_s != BUTTER_LP &&
      filter_type_s != EXP_LP &&
      scale(data->band, data->band, d_scale, 0.0) != Iok) {
    Error(2, "Bad or different pixel types.\n");
    exit(2);
  }

  data->redisplay = True;

  convertBand(data->band, ip->img[1]);

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_filter");

  return;

} /* Make_filter() */

#ifndef FUNCPROTO
static void Make_filter_four()
#else /* FUNCPROTO */
static void Make_filter_four(void)
#endif /* FUNCPROTO */
{
  bandData data          = band_data_s[FILTIMAGE],
           fdata         = band_data_s[F_FILTIMAGE];
  layout_image fl_image  = layout_images_s[F_FILTIMAGE];
  layout_image_part *fip = &(fl_image->image);

  /* Make Fourier transform of filter and displayable band. */

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_filter_four");

  /* Both data-band and display-band can be of type real (when using Hartley
   * transform instead of FFT). Need two separate bands, because the display
   * band will be shifted and compressed, while the data band enters the
   * filtering multiplication.
   */
  free_and_or_allocate(F_FILTIMAGE, Ireal_typ, Ireal_typ,
		       "log|FFT(filter)|");

  if (filter_type_s != LOWPASS_IDEAL && filter_type_s != BUTTER_LP &&
      filter_type_s != EXP_LP) {

    /* Use Hartley transform because it's faster than Fourier. */
    fht2d((IR_BAND) data->band, (IR_BAND) fdata->band, 1);
  } else if (filter_type_s == LOWPASS_IDEAL)
    lowpassIdealf(fdata->band, cut_low_s);
  else if (filter_type_s == BUTTER_LP)
    butterworthf(fdata->band, cut_low_s, filter_order_s);
  else if (filter_type_s == EXP_LP)
    exponentialf(fdata->band, cut_low_s, filter_order_s);

  fdata->redisplay = True;

  compress_and_shift_spec(F_FILTIMAGE);

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_filter_four");

  return;

} /* Make_filter_four() */

#ifndef FUNCPROTO
static void Make_filter_cross()
#else /* FUNCPROTO */
static void Make_filter_cross(void)
#endif /* FUNCPROTO */
{
  int status = 0;
  long size;
  double min, max, *vector = NULL;
  bandData cdata         = band_data_s[FILTCROSS];
  layout_image fl_image  = layout_images_s[F_FILTIMAGE];
  layout_image cl_image  = layout_images_s[FILTCROSS];
  layout_image_part *fip = &(fl_image->image);
  layout_image_part *cip = &(cl_image->image);
  IBAND fdisp_band;
  IBAND cdisp_band;

  /* Make band with cross section of Fourier transform of input image.
   * No need to make separate display band.
   */

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_filter_cross");

  if (!cdata->show) {
    LEAVE_FUNCTION_DEBUG("xfilter.c: Make_filter_cross");
    return;
  }

  free_and_or_allocate(FILTCROSS, Iunknown_typ, Iu_byte_typ,
		       "Cross-section filter");

  fdisp_band = fip->img[1]; /* Filter display image. */
  cdisp_band = cip->img[1]; /* Cross-section display image. */

  if (cdisp_band == cdata->band) cdata->redisplay = True;
  cdata->band = cdisp_band; /* Set data band equal to display band. */

  status = rowBand(fdisp_band, inband_ysize_s/2, &vector, &size, &min, &max);
  if (status != Iok) {
    Error(2, "Error in rowBand.\n");
    exit(1);
  }

  status = curveToBand(cdisp_band, vector, cross_min_s, cross_max_s,
		       1.0, 255, 255, 0);
  if (status != Iok) {
    Error(2, "Error in curveToBand.\n");
    exit(1);
  }

  free(vector);

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_filter_cross");

  return;

} /* Make_filter_cross() */

#ifndef FUNCPROTO
static void Make_output_four()
#else /* FUNCPROTO */
static void Make_output_four(void)
#endif /* FUNCPROTO */
{
  bandData  out_data      = band_data_s[OUTIMAGE],
            fout_data     = band_data_s[F_OUTIMAGE],
            fin_data      = band_data_s[F_INIMAGE],
            ffilt_data    = band_data_s[F_FILTIMAGE];
  layout_image fol_image  = layout_images_s[F_OUTIMAGE];
  layout_image_part *foip = &(fol_image->image);

  /* Make Fourier transform of output image and displayable band. */

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_output_four");

  if (!out_data->show && !fout_data->show) {
    LEAVE_FUNCTION_DEBUG("xfilter.c: Make_output_four");
    return;
  }

  /* Both data-band and display-band can be of type real (when using Hartley
   * transform instead of FFT). Need two separate bands, because the display
   * band will be shifted and compressed, while the data band enters the
   * inverse Fourier transform.
   */
  free_and_or_allocate(F_OUTIMAGE, Ireal_typ, Ireal_typ,
		       "log|FFT(output)|");

  /* Filtering by multiplying the Hartley transforms. This is legal because
   * one of the images (the filter) is always even symmetrical.
   */
  multiply(fin_data->band, ffilt_data->band, fout_data->band, 1.0);

  fout_data->redisplay = True;

  compress_and_shift_spec(F_OUTIMAGE);

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_output_four");

  return;

} /* Make_output_four() */

#ifndef FUNCPROTO
static void Make_output()
#else /* FUNCPROTO */
static void Make_output(void)
#endif /* FUNCPROTO */
{
  bandData        data  = band_data_s[OUTIMAGE],
                  fdata = band_data_s[F_OUTIMAGE];
  layout_image l_image  = layout_images_s[OUTIMAGE];
  layout_image_part *ip = &(l_image->image);
  IBAND disp_band;

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_output");

  if (!data->show) {
    LEAVE_FUNCTION_DEBUG("xfilter.c: Make_output");
    return;
  }

  /* Both data-band and display-band can be of type real (when using Hartley
   * transform instead of FFT). No need for two separate bands.
   */
  free_and_or_allocate(OUTIMAGE, Ireal_typ, Ireal_typ, "Output");
  disp_band  = ip->img[1];

  /* Use Hartley transform because it's faster than Fourier. */
  fht2d((IR_BAND) fdata->band, (IR_BAND) data->band, 0);

  convertBand(data->band, disp_band);
  data->redisplay = True;

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_output");

  return;

} /* Make_output() */

#ifndef FUNCPROTO
static void Make_difference()
#else /* FUNCPROTO */
static void Make_difference(void)
#endif /* FUNCPROTO */
{
  bandData        diff_data  = band_data_s[DIFFIMAGE],
                  in_data    = band_data_s[INIMAGE],
                  out_data   = band_data_s[OUTIMAGE];
  layout_image l_image       = layout_images_s[DIFFIMAGE];
  layout_image_part *ip      = &(l_image->image);
  IBAND disp_band;

  ENTER_FUNCTION_DEBUG("xfilter.c: Make_difference");

  if (!diff_data->show) {
    LEAVE_FUNCTION_DEBUG("xfilter.c: Make_difference");
    return;
  }

  free_and_or_allocate(DIFFIMAGE, Iunknown_typ, Ireal_typ, "Difference");
  disp_band  = ip->img[1];

  signDiff(in_data->band, out_data->band, disp_band, 1.0, 0.0);
  diff_data->redisplay = True;

  LEAVE_FUNCTION_DEBUG("xfilter.c: Make_difference");

  return;

} /* Make_difference() */

#ifndef FUNCPROTO
static void update_widget(wid_num)
image_widget_number wid_num;
#else /* FUNCPROTO */
static void update_widget(image_widget_number wid_num)
#endif /* FUNCPROTO */
{
  bandData data         = band_data_s[wid_num];
  layout_image l_image  = layout_images_s[wid_num];
  layout_image_part *ip = &(l_image->image);
  IBAND disp_band;

  ENTER_FUNCTION_DEBUG("xfilter.c: update_widget");

  switch(wid_num) {
  case INIMAGE:     Make_input_band();
    break;
  case F_INIMAGE:   Make_input_four();
    break;
  case FILTIMAGE:   Make_filter();
    break;
  case F_FILTIMAGE: Make_filter_four();
    break;
  case FILTCROSS:   Make_filter_cross();
    break;
  case OUTIMAGE:    Make_output();
    break;
  case F_OUTIMAGE:  Make_output_four();
    break;
  case DIFFIMAGE:   Make_difference();
    break;
  default: Error(2, "Unknown image.\n");
    exit(1);
  } /* switch */

  if (!data->show) {
    LEAVE_FUNCTION_DEBUG("xfilter.c: update_widget");
    return;
  }

  disp_band = ip->img[1];

  /* Change resources but delay redisplay. */
  XtVaSetValues(image_wid(wid_num),
		XtNimageImage,  ip->img,
		XtNimageBand,   disp_band,
		XtNimageUpdate, UPDATE_NONE,
		NULL);

  /* Redisplay image explicitly, and recalculate the extrema to get a correct
   * display. Don't check data->redisplay because the redisplay did not
   * occur as a result of the above XtVaSetValues call, and it must be done
   * once because of new pixel values.
   */

  ImageRedisplay((ImageWidget) image_wid(wid_num),
		 UPDATE_ZOOMPAN | UPDATE_MINMAX);

  LEAVE_FUNCTION_DEBUG("xfilter.c: update_widget");

} /* update_widget() */

#ifndef FUNCPROTO
static void update_magnitude_widget(wid_num)
image_widget_number wid_num;
#else /* FUNCPROTO */
static void update_magnitude_widget(image_widget_number wid_num)
#endif /* FUNCPROTO */
{
  bandData data         = band_data_s[wid_num];
  layout_image l_image  = layout_images_s[wid_num];
  layout_image_part *ip = &(l_image->image);
  IBAND disp_band;

  if (!data->show) {
    LEAVE_FUNCTION_DEBUG("xfilter.c: update_magnitude_widget");
    return;
  }

  disp_band = ip->img[1];

  compress_and_shift_spec(wid_num);

  /* Change resources but delay redisplay. */
  XtVaSetValues(image_wid(wid_num),
		XtNimageImage,  ip->img,
		XtNimageBand,   disp_band,
		XtNimageUpdate, UPDATE_NONE,
		NULL);

  /* Redisplay image explicitly, and recalculate the extrema to get a correct
   * display. Don't check data->redisplay because the redisplay did not
   * occur as a result of the above XtVaSetValues call, and it must be done
   * once because of new pixel values.
   */
  ImageRedisplay((ImageWidget) image_wid(wid_num),
		 UPDATE_ZOOMPAN | UPDATE_MINMAX);

} /* update_magnitude_widget() */

#ifndef FUNCPROTO
static void update_filter_and_fht()
#else /* FUNCPROTO */
static void update_filter_and_fht(void)
#endif /* FUNCPROTO */
{
  if (filter_type_s != LOWPASS_IDEAL && filter_type_s != BUTTER_LP &&
      filter_type_s != EXP_LP) {
    update_widget(FILTIMAGE);
    update_widget(F_FILTIMAGE);
  } else {
    update_widget(F_FILTIMAGE);
    if (band_data_s[FILTIMAGE]->show) update_widget(FILTIMAGE);
  }

  return;

} /* update_filter_and_fht() */

#ifndef FUNCPROTO
static void update_from_filter_to_output()
#else /* FUNCPROTO */
static void update_from_filter_to_output(void)
#endif /* FUNCPROTO */
{
  update_filter_and_fht();
  update_widget(FILTCROSS);
  update_widget(F_OUTIMAGE);
  update_widget(OUTIMAGE);
  update_widget(DIFFIMAGE);

  return;

} /* update_from_filter_to_output() */

#ifndef FUNCPROTO
static void Read_image(wid)
Widget wid;
#else /* FUNCPROTO */
static void Read_image(Widget wid)
#endif /* FUNCPROTO */
{
  int stat = 0;
  char *name = NULL, msg[200];
  Boolean need_new_filter = 0;
  IMAGE img_extra;
  bandData data = band_data_s[INIMAGE];
  layout_image l_image = layout_images_s[INIMAGE];
  layout_image_part *ip = &(l_image->image);

  stat = SelectReadFile(toplevel_s, readBIFF, &name);
  if (stat != 1) {
    if (name) free(name);
    return;
  }
  
  if ((img_extra = Iread_image(name)) == 0) {
    sprintf(msg, "Can't read file %s", name);
    InformationBox(toplevel_s, msg, "xfilter - info", "xfilter - info");
    free(name);
    return;
  }

  if (ip->img) {
    if (inband_xsize_s != Ixsize(img_extra[1]) ||
	inband_ysize_s != Iysize(img_extra[1]))
      need_new_filter = 1;

    Idel_image(&ip->img);
    data->band = NULL;
  }

  ip->img = img_extra;

  update_widget(INIMAGE);
  update_widget(F_INIMAGE);

  if (need_new_filter) {
    set_scrollbar_thumb(scroll_wid(ORDER_SCROLL), (double) filter_order_s,
			1.0, (double) inband_xsize_s, False);
    update_filter_and_fht();
    update_widget(FILTCROSS);
  }
  update_widget(F_OUTIMAGE);
  update_widget(OUTIMAGE);
  update_widget(DIFFIMAGE);
  update_rms();

  free(name);

  return;

} /* Read_image() */

#ifndef FUNCPROTO
static void Save_image(wid)
Widget wid;
#else /* FUNCPROTO */
static void Save_image(Widget wid)
#endif /* FUNCPROTO */
{
  int stat;
  char *name = NULL;
  layout_image l_image = layout_images_s[OUTIMAGE];
  layout_image_part *ip = &(l_image->image);

  stat = SelectWriteFile(toplevel_s, writeBIFF, &name);
  if (stat != 1) {
    if (name) free(name);
    return;
  }

  Ihistory(ip->img, "xfilter", NULL);
  stat = Iwrite_image(ip->img, name);
  if (stat) InformationBox(toplevel_s, Ierr_message(stat),
                           "xfilter - info", "xfilter - info");

  free(name);

  return;

} /* Save_image() */

#ifndef FUNCPROTO
static void Save_filter(wid)
Widget wid;
#else /* FUNCPROTO */
static void Save_filter(Widget wid)
#endif /* FUNCPROTO */
{
  int stat;
  char *name = NULL;
  layout_image l_image = layout_images_s[FILTIMAGE];
  layout_image_part *ip = &(l_image->image);

  stat = SelectWriteFile(toplevel_s, writeBIFF, &name);
  if (stat != 1) {
    if (name) free(name);
    return;
  }

  Ihistory(ip->img, "xfilter", NULL);
  stat = Iwrite_image(ip->img, name);
  if (stat) InformationBox(toplevel_s, Ierr_message(stat),
                           "xfilter - info", "xfilter - info");

  free(name);

  return;

} /* Save_filter() */

#ifndef FUNCPROTO
static void filt_select(wid, client_data, call_data)
Widget wid;
XtPointer client_data, call_data;
#else /* FUNCPROTO */
static void filt_select(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  filter_type cl_data = (filter_type) *((int *) client_data);

  if (cl_data == filter_type_s) return;

  ENTER_FUNCTION_DEBUG("xfilter.c: filt_select");

  filter_type_s = cl_data;

  update_sensitive();
  update_filter_menulabel();
  update_from_filter_to_output();
  update_rms();

  LEAVE_FUNCTION_DEBUG("xfilter.c: filt_select");

} /* filt_select() */

#ifndef FUNCPROTO
static void window_select(wid, client_data, call_data)
Widget wid;
XtPointer client_data, call_data;
#else /* FUNCPROTO */
static void window_select(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  window_type cl_data = *((window_type *) client_data);

  if (cl_data == window_type_s) return;

  ENTER_FUNCTION_DEBUG("xfilter.c: window_select");

  window_type_s = cl_data;

  update_window_menulabel();
  update_from_filter_to_output();
  update_rms();

  LEAVE_FUNCTION_DEBUG("xfilter.c: window_select");

} /* window_select() */




/*L:scroll_incremental_cb*

________________________________________________________________

		scroll_incremental_cb
________________________________________________________________

Name:		scroll_incremental_cb - For incremental scrolling
Author:		Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
static void jump_value(value, min, max, jump, increase, logarithmic)
double *value, min, max, jump;
Boolean increase, logarithmic;
#else /* FUNCPROTO */
static void jump_value(double *value, double min, double max, double jump, Boolean increase, Boolean logarithmic)
#endif /* FUNCPROTO */
{
  double newmin, newmax, newval;
  double val = *value;

  if (!logarithmic) {
    newmin = min;
    newmax = max;
    newval = val;
  } else {
    newmin = log10(min);
    newmax = log10(max);
    newval = log10(val);
  }

  if (increase) {
    if (newval - newmin > (1.0 - jump) * (newmax - newmin)) val = newmax;
    else val = newval + jump * (newmax - newmin);
  } else {
    if (newval - newmin < jump * (newmax - newmin)) val = newmin;
    else val = newval - jump * (newmax - newmin);
  }

  if (logarithmic) val = pow(10.0, val);

  *value = val;
  return;

} /* jump_value() */

#ifndef FUNCPROTO
static void scroll_incremental_cb(wid, client_data, call_data)
Widget wid;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void scroll_incremental_cb(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  scroll_widget_number wid_num = (scroll_widget_number) *((int *) client_data);
  long position = (long) call_data;

  ENTER_FUNCTION_DEBUG("xfilter.c: scroll_incremental_cb");

  switch(wid_num) {
    case LOW_SCROLL:
      jump_value(&cut_low_s, 0.0, 1.0, 0.05, position < 0, False);

      if (cut_low_s <= minimum_legal_frequency_s)
	cut_low_s = minimum_legal_frequency_s;

      set_scrollbar_thumb(scroll_wid(wid_num), cut_low_s, 0.0, 1.0, False);
      set_number(LOW_PROMPT);

      update_from_filter_to_output();
      update_rms();

      break;
    case HIGH_SCROLL:
      jump_value(&cut_high_s, 0.0, 1.0, 0.05, position < 0, False);

      if (cut_high_s <= minimum_legal_frequency_s)
	cut_high_s = minimum_legal_frequency_s;

      set_scrollbar_thumb(scroll_wid(wid_num), cut_high_s, 0.0, 1.0, False);
      set_number(HIGH_PROMPT);

      update_from_filter_to_output();
      update_rms();

      break;
    case ORDER_SCROLL: {
      double f_o = (double) filter_order_s;
      double max;

      max  = (double) inband_xsize_s;

      jump_value(&f_o, 0.0, max, 0.05, position < 0, False);
      filter_order_s = (int) (f_o + 0.5);

      if (filter_order_s <= 0) filter_order_s = 1;

      set_scrollbar_thumb(scroll_wid(wid_num), (double) filter_order_s, 1.0,
			 max, False);
      set_number(ORDER_PROMPT);

      update_from_filter_to_output();
      update_rms();
    }
      break;
    case CROSS_MIN_SCROLL:
      jump_value(&cross_min_s, cross_min_min_s, cross_max_max_s,
		 0.05, position < 0, False);
      set_scrollbar_thumb(scroll_wid(wid_num), cross_min_s, cross_min_min_s,
			  cross_max_max_s, False);
      set_number(CROSS_MIN_PROMPT);
      update_widget(FILTCROSS);

      break;
    case CROSS_MAX_SCROLL:
      jump_value(&cross_max_s, cross_min_min_s, cross_max_max_s,
		 0.05, position < 0, False);
      set_scrollbar_thumb(scroll_wid(wid_num), cross_max_s, cross_min_min_s,
			  cross_max_max_s, False);
      set_number(CROSS_MAX_PROMPT);
      update_widget(FILTCROSS);

      break;
    case F_INIMAGE_EPS_SCROLL:
      jump_value(&f_inimage_eps_s, eps_min_s, eps_max_s, 0.05, position < 0,
		 f_inimage_log_s);

      set_scrollbar_thumb(scroll_wid(wid_num), f_inimage_eps_s,
			  eps_min_s, eps_max_s, f_inimage_log_s);
      set_number(F_INIMAGE_EPS_PROMPT);
      update_magnitude_widget(F_INIMAGE);

      break;
    case F_FILTIMAGE_EPS_SCROLL:
      jump_value(&f_filtimage_eps_s, eps_min_s, eps_max_s, 0.05, position < 0,
		 f_filtimage_log_s);
      set_scrollbar_thumb(scroll_wid(wid_num), f_filtimage_eps_s, eps_min_s,
			  eps_max_s, f_filtimage_log_s);
      set_number(F_FILTIMAGE_EPS_PROMPT);
      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);

      break;
    case F_OUTIMAGE_EPS_SCROLL:
      jump_value(&f_outimage_eps_s, eps_min_s, eps_max_s, 0.05, position < 0,
		 f_outimage_log_s);
      set_scrollbar_thumb(scroll_wid(wid_num), f_outimage_eps_s, eps_min_s,
			  eps_max_s, f_outimage_log_s);
      set_number(F_OUTIMAGE_EPS_PROMPT);
      update_magnitude_widget(F_OUTIMAGE);

      break;
    }

  LEAVE_FUNCTION_DEBUG("xfilter.c: scroll_incremental_cb");

} /* scroll_incremental_cb() */

#ifndef FUNCPROTO
static void prompt_activate_cb(wid, client_data, call_data)
Widget wid;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void prompt_activate_cb(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  prompt_widget_number wid_num = (prompt_widget_number) *((int *) client_data);

  switch(wid_num) {
    case LOW_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) cut_low_s = f;
      set_scrollbar_thumb(scroll_wid(LOW_SCROLL), cut_low_s, 0.0, 1.0, False);

      update_from_filter_to_output();
      update_rms();
    }
    break;
    case HIGH_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) cut_high_s = f;
      set_scrollbar_thumb(scroll_wid(HIGH_SCROLL), cut_high_s,
			  0.0, 1.0, False);

      update_from_filter_to_output();
      update_rms();
    }
      break;
    case ORDER_PROMPT: {
      char *s;
      int i;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%d", &i) != 0) filter_order_s = i;
      set_scrollbar_thumb(scroll_wid(ORDER_SCROLL), (double) filter_order_s,
			  1.0, (double) inband_xsize_s, False);

      update_from_filter_to_output();
      update_rms();
    }
      break;
    case CROSS_MIN_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) cross_min_s = f;
      set_scrollbar_thumb(scroll_wid(CROSS_MIN_SCROLL), cross_min_s,
			  cross_min_min_s, cross_max_max_s, False);

      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);
    }
      break;
    case CROSS_MAX_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) cross_max_s = f;
      set_scrollbar_thumb(scroll_wid(CROSS_MAX_SCROLL), cross_max_s,
			  cross_min_min_s, cross_max_max_s, False);
      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);
    }
      break;
    case F_INIMAGE_EPS_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) f_inimage_eps_s = f;
      set_scrollbar_thumb(scroll_wid(F_INIMAGE_EPS_SCROLL), f_inimage_eps_s,
			  eps_min_s, eps_max_s, f_inimage_log_s);

      update_magnitude_widget(F_INIMAGE);
    }
      break;
    case F_FILTIMAGE_EPS_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) f_filtimage_eps_s = f;
      set_scrollbar_thumb(scroll_wid(F_FILTIMAGE_EPS_SCROLL),
			  f_filtimage_eps_s, eps_min_s, eps_max_s,
			  f_filtimage_log_s);

      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);
    }
      break;
    case F_OUTIMAGE_EPS_PROMPT: {
      char *s;
      double f;

      XtVaGetValues(prompt_wid(wid_num), XtNtext, &s, NULL);
      if (sscanf(s, "%lf", &f) != 0) f_outimage_eps_s = f;
      set_scrollbar_thumb(scroll_wid(F_OUTIMAGE_EPS_SCROLL), f_outimage_eps_s,
			  eps_min_s, eps_max_s, f_outimage_log_s);

      update_magnitude_widget(F_OUTIMAGE);
    }
      break;
    } /* switch */

} /* prompt_activate_cb() */

#ifndef FUNCPROTO
static void scroll_jump_cb(wid, client_data, call_data)
Widget wid;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void scroll_jump_cb(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  scroll_widget_number wid_num = (scroll_widget_number) *((int *) client_data);

  ENTER_FUNCTION_DEBUG("xfilter.c: scroll_jump_cb");

  switch(wid_num) {
    case LOW_SCROLL: {
      float percent = *((float *) call_data);
      
      cut_low_s = percent;
      if (cut_low_s <= minimum_legal_frequency_s)
	cut_low_s = minimum_legal_frequency_s;
      set_number(LOW_PROMPT);

      update_from_filter_to_output();
      update_rms();
    }
      break;
    case HIGH_SCROLL: {
      float percent = *((float *) call_data);
      
      cut_high_s = percent;
      if (cut_low_s <= minimum_legal_frequency_s)
	  cut_low_s = minimum_legal_frequency_s;
      set_number(HIGH_PROMPT);

      update_from_filter_to_output();
      update_rms();
    }
      break;
    case ORDER_SCROLL: {
      float percent = *((float *) call_data);
      
      filter_order_s = inband_xsize_s * percent;
      if (filter_order_s <= 0) filter_order_s = 1;
      set_number(ORDER_PROMPT);
      set_scrollbar_thumb(scroll_wid(ORDER_SCROLL), (double) filter_order_s,
			  1.0, (double) inband_xsize_s, False);

      update_from_filter_to_output();
      update_rms();
    }
      break;
    case CROSS_MIN_SCROLL: {
      float percent = *((float *) call_data);
      
      cross_min_s = (cross_max_max_s - cross_min_min_s) * percent +
	cross_min_min_s;
      set_number(CROSS_MIN_PROMPT);

      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);
    }
      break;
    case CROSS_MAX_SCROLL: {
      float percent = *((float *) call_data);
      
      cross_max_s = (cross_max_max_s - cross_min_min_s) * percent +
	cross_min_min_s;
      set_number(CROSS_MAX_PROMPT);

      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);
    }
      break;
    case F_INIMAGE_EPS_SCROLL: {
      float percent = *((float *) call_data);
      
      f_inimage_eps_s = pow(10.0, log10(eps_min_s) +
			    (log10(eps_max_s) - log10(eps_min_s)) * percent);
      set_number(F_INIMAGE_EPS_PROMPT);

      update_magnitude_widget(F_INIMAGE);
    }
      break;
    case F_FILTIMAGE_EPS_SCROLL: {
      float percent = *((float *) call_data);
      
      f_filtimage_eps_s = pow(10.0, log10(eps_min_s) +
			      (log10(eps_max_s) - log10(eps_min_s)) * percent);
      set_number(F_FILTIMAGE_EPS_PROMPT);

      update_magnitude_widget(F_FILTIMAGE);
      update_widget(FILTCROSS);
    }
      break;
    case F_OUTIMAGE_EPS_SCROLL: {
      float percent = *((float *) call_data);

      f_outimage_eps_s = pow(10.0, log10(eps_min_s) +
			     (log10(eps_max_s) - log10(eps_min_s)) * percent);
      set_number(F_OUTIMAGE_EPS_PROMPT);

      update_magnitude_widget(F_OUTIMAGE);
    }
      break;
    } /* switch */

} /* scroll_jump_cb() */

#ifndef FUNCPROTO
static void ximage_work(wid, client_data, call_data)
Widget wid;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void ximage_work(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  command_widget_number wid_num =
    (command_widget_number) *((int *) client_data);

  ENTER_FUNCTION_DEBUG("xfilter.c: ximage_work");

  switch((int) wid_num) {
    case QUIT:
      Quit(wid);
      break;
    case READ_INPUT:
      Read_image(wid);
      break;
    case SAVE_OUTPUT:
      Save_image(wid);
      break;
    case SAVE_FILTER:
      Save_filter(wid);
      break;
    default:
      break;
    } /* switch */

  LEAVE_FUNCTION_DEBUG("xfilter.c: ximage_work");

} /* ximage_work() */

/*P:xfilter*

________________________________________________________________

		xfilter
________________________________________________________________

Name:		xfilter - filter image and display power spectra

Syntax:		xfilter [<option>...] <inimage>

Description:	Four images are displayed by default: The input image, the
                logarithm of a chosen filter''s discrete Fourier transform
		magnitude, the cross-section of the latter and the output
		(filtered) image. Refer to the options described below on
		how to display a different set of images involved in the
		filtering process.

		The actual filtering is performed in the Fourier domain by
		multiplication of the discrete Fourier transform of the input
		image by the discrete Fourier transform of the filter.
		(Actually, the FFT is not directly computed, rather the
		Fast Hartley transform is used because it is more effective
		than the FFT. The results are the same as if the FFT had been
		used, except for roundoff errors.)

		The filter can be chosen from a menu. The initial filter can
		be chosen with the option '-ftype' (see below).

		There are two main classes of filters available. One class
		contains realizable filters, the other contains non-realizable
		filters (realizable in this context means that the filters can
		be implemented in finite space).

		The realizable filters are "lp", "hp", "bp" and "bs". These
		are all based on the ideal Fourier transform magnitudes of the
		corresponding type. For these filters, the spatial domain
		filter functions are known analytically and they have infinite
		extent in the spatial domain. (They have been found
		analytically by inverse Fourier transformation of the ideal
		Fourier domain filter functions.) In this application the
		spatial domain filter functions are multiplied by
		2-dimensional window functions to make them realizable. The
		resulting realizable Fourier transform magnitudes are
		displayed.

		The non-relizable filters are "ilp", "blp" and "elp". "ilp"
		is made from the ideal Fourier transform magnitude. Instead
		of using the analytically correct spatial domain filter
		function, "ilp" computes the spatial domain filter function
		via inverse FFT (not equal to the inverse Fourier transform
		on the continuous frequency domain). This sampling in the
		frequency domain (from continuous frequency to discrete points
		in the FFT) results in aliasing in the spatial domain.
		"blp" and "elp" have also been constructed from closed-form
		Fourier domain filter functions. The spatial-domain functions
		have been found with inverse FFT.

Options:	&-low cutoff
		Lower cutoff frequency, normalized by the Nyquist frequency.
		This means that 'cutoff' equal to 1.0 corresponds to half the
		sampling frequency. 'cutoff' can be adjusted with a slider
		after startup. Default: 0.2.

		&-high cutoff
		Higher cutoff frequency (normalized by the Nyquist frequency).
		This can be adjusted with a slider after startup.
		Default: 0.5.

		&-ftype filter-type
		Type of filter. Use one of the symbols

                | Full name    Short name 
		| ------------------------
                | lowpass         lp      
                | highpass        hp      
                | bandpass        bp      
                | bandstop        bs      
                | lowpass_ideal   ilp     
		| butterworth_lp  blp     
		| exponential_lp  elp

		Default: lp.

		&-order filter-order
		For the window-based filters, this is the filter size in
		pixels (diameter of window function). For the Butterworth
		and Exponential filters, 'filter-order' is the parameter
		in the formula. For the "ideal" filter, this option has no
		meaning. It can be adjusted with a slider after startup.
		Default: 41.

		&-wtype window-type
		Type of window function to multiply with filter in the
		spatial domain. Use one of the symbols

		| Full name    Short name
		| -----------------------
		| rectangle        r
		| bartlett         b
		| triangle         t
		| hamming          hm
		| hanning          hn

		Default: hm.

		&-scale scale
                Use this 'scale' when displaying Fourier transform magnitudes.
		Refer to 'fftDisplay(3)' for a description of this parameter.
		Default: 10 (which will give the power spectra in dB).

		&-logmin min
		Set all values smaller than 'min' after logarithmic
		compression (for display of Fourier magnitudes) equal to 'min'.
		Default: -4.0 * 'scale' (from option '-scale').

		&-cmin min
		Clip cross-section display of filter spectrum at 'min'.
		This can be adjusted with a slider after startup.
		Default: -80.

		&-cmax max
		Clip cross-section display of filter spectrum at 'max'.
		This can be adjusted with a slider after startup.
		Default: 10.

		&-cminmin min
		Lowest possible value for the -cmin option. Default: -200.

		&-cmaxmax max
		Largest possible value for the -cmax option. Default: 50.

		&-emin min
		Smallest possible eps-value for logarithmic compression
		of FFT magnitude. Default: 1.0e-20.

		&-emax max
		Largest possible eps-value for logarithmic compression
		of FFT magnitude. Default: 0.5.

		&-ilin
		Use linear scrollbar for eps in logarithmic compression of
		FFT magnitude of input. Default: Logarithmic scrollbar.

		&-flin
		Use linear scrollbar for eps in logarithmic compression of
		FFT magnitude of filter. Default: Logarithmic scrollbar.

		&-olin
		Use linear scrollbar for eps in logarithmic compression of
		FFT magnitude of output. Default: Logarithmic scrollbar.

		&-rms
		Do not calculate and display the RMS difference between
		input and output.

                &-wdiff
		Show widget for difference image between output and input.
		
		&-win
		Do not show widget for input image.

		&-wfin
		Show widget for FFT(input).

		&-wfilt
		Show widget for filter.

		&-wffilt
		Do not show widget for FFT(filter).

		&-wout
		Do not show widget for output. Also implies option '-rms'.

		&-wfout
		Show widget for FFT(output).

		&-wcross
		Do not show widget for cross-section of FFT(filter).

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'xfilter'.
                In the notation below, indentation indicates hierarchical 
                structure.

		There are a few application resources in addition to the
		standard application resources provided by the 'ximage'
		toolkit. Each correspond to one of the command line options
		listed above. The additional application resources are:

		&widgetIn

		&widgetFourIn

		&widgetFilt

		&widgetFourFilt

		&widgetOut

		&widgetFourOut

		&widgetCross

		&widgetDiff

		&crossMinMin

		&crossMaxMax

		&crossMin

		&crossMax

		&epsMin

		&epsMax

		&cutLow

		&cutHigh

		&dispScale

		&logMin

		&filterOrder

		&filterType

		&rmsCalc

		&windowType

		&linearFourIn

		&linearFourFilt

		&linearFourOut

		-&The following hierarchy is constructed when the default
                resources/options for widget layout is used.

                The widgets of class XfwfButton can be replaced by class
                Command (from the Athena widget set) by using the 'ximage(3)'
                option '-widgetset' or corresponding application resource.

		&xfilter                   (class XFilter)
		|  popup                   (class TransientShell)
		|    dialog                (class Dialog)
		|      label               (class Label)
		|      command             (class Command)
		|  colorsmenu              (class SimpleMenu)
		|    menuLabel             (class SmeBSB)
		|    backgroundcol         (class SmeBSB)
		|    Work-map              (class SmeBSB)
		|    colorsep              (class SmeLine)
		|    White - 256           (class SmeBSB)
		|    Black - 256           (class SmeBSB)
		|    Red - 256             (class SmeBSB)
		|    Green - 256           (class SmeBSB)
		|    Blue - 256            (class SmeBSB)
		|    Spectrum - 256        (class SmeBSB)
		|    Hue - 256             (class SmeBSB)
		|    Rainbow - 256         (class SmeBSB)
		|  overlaysmenu            (class SimpleMenu)
		|    menuLabel             (class SmeBSB)
		|    backgroundovl         (class SmeBSB)
		|    Std-overlay           (class SmeBSB)
		|    colorsep              (class SmeLine)
		|    White - mono          (class SmeBSB)
		|    Black - mono          (class SmeBSB)
		|  optionsmenu             (class SimpleMenu)
		|    menuLabel             (class SmeBSB)
		|    optionsmenusep        (class SmeLine)
		|    optionsmenuentry      (class SmeBSB)
		|    optionsmenuentry      (class SmeBSB)
		|    optionsmenuentry      (class SmeBSB)
		|    optionsmenusep        (class SmeLine)
		|    optionsmenuentry      (class SmeBSB)
		|    optionsmenuentry      (class SmeBSB)
		|    optionsmenuentry      (class SmeBSB)
		|  toolsmenu               (class SimpleMenu)
		|    menuLabel             (class SmeBSB)
		|    imageinfo             (class SmeBSB)
		|    histogram             (class SmeBSB)
		|    slice                 (class SmeBSB)
		|  inimageShell            (class TopLevelShell)
		|    imageForm             (class ImageForm)
		|      menubar             (class XfwfMenuBar)
		|        colors            (class XfwfPullDown)
		|        tools             (class XfwfPullDown)
		|        options           (class XfwfPullDown)
		|      image               (class Image)
		|  xfilter control panel   (class TopLevelShell)
		|    topContainer          (class Form)
		|      Filter type         (class Label)
		|      Filter-menu         (class MenuButton)
		|	 Filter-menu       (class SimpleMenu)
		|	   lowpass         (class SmeBSB)
		|	   highpass        (class SmeBSB)
		|	   bandpass        (class SmeBSB)
		|	   bandstop        (class SmeBSB)
		|	   lowpass_ideal   (class SmeBSB)
		|	   butterworth_lp  (class SmeBSB)
		|	   exponential_lp  (class SmeBSB)
		|      Low cutoff          (class Label)
		|      lowPrompt           (class XfwfPrompt)
		|      lowScroll           (class Scrollbar)
		|      High cutoff         (class Label)
		|      highPrompt          (class XfwfPrompt)
		|      highScroll          (class Scrollbar)
		|      Filter order        (class Label)
		|      orderPrompt         (class XfwfPrompt)
		|      orderScroll         (class Scrollbar)
		|      Window type         (class Label)
		|      Window-menu         (class MenuButton)
		|	 Window-menu       (class SimpleMenu)
		|	   rectangle       (class SmeBSB)
		|	   bartlett        (class SmeBSB)
		|	   triangle        (class SmeBSB)
		|	   hamming         (class SmeBSB)
		|	   hanning         (class SmeBSB)
		|      rmsDiff             (class Label)
		|      fInimageEpsLabel    (class Label)
		|      fInimageEpsPrompt   (class XfwfPrompt)
		|      fInimageEpsScroll   (class Scrollbar)
		|      fFiltimageEpsLabel  (class Label)
		|      fFiltimageEpsPrompt (class XfwfPrompt)
		|      fFiltimageEpsScroll (class Scrollbar)
		|      fOutimageEpsLabel   (class Label)
		|      fOutimageEpsPrompt  (class XfwfPrompt)
		|      fOutimageEpsScroll  (class Scrollbar)
		|      crossMinLabel       (class Label)
		|      crossMinPrompt      (class XfwfPrompt)
		|      crossMinScroll      (class Scrollbar)
		|      crossMaxLabel       (class Label)
		|      crossMaxPrompt      (class XfwfPrompt)
		|      crossMaxScroll      (class Scrollbar)
		|      read inimage        (class XfwfButton)
		|      save outimage       (class XfwfButton)
		|      save filter         (class XfwfButton)
		|      quit                (class XfwfButton)

		Six additional image widgets belong to the hierarchy. Their
		description is exactly like the one for inimageShell above,
		except that the resource instance names of the TopLevelShell
		widget is f_inimageShell, filtimageShell, f_filtimageShell,
		filtcrossShell, outimageShell and f_outimageShell.

See also:	'lowpass(1)', 'highpass(1)', 'bandpass(1)', 'bandstop(1)',
                'lowpassIdeal(1)', 'butterworth(1)', 'exponential(1)',
		'fft2d(1)', 'fht2d(1)', 'ht2ft(1)', 'fhtPower(1)',
		'logarithm(1)', 'ximage(3)', 'Image(3)'

Files:          $XITE_HOME/etc/app_defaults/XFilter
Author:		Svein Bøe, Ifi, UiO
Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
static void check_options(argc, argv)
int *argc;
char **argv;
#else /* FUNCPROTO */
static void check_options(int *argc, char **argv)
#endif /* FUNCPROTO */
{

  /* Give usage message if arguments are missing */
  if (*argc == 1) Usage(1, NULL);
  if (*argc != 2) Usage(2, "Illegal number of arguments.\n");

  if (filter_order_s <= 0) {
    filter_order_s = 1;
    Warning(1, "Filter order can not be smaller than 1. Setting it to 1.\n");
  }
  if (cut_low_s <= 0.0) {
    cut_low_s = 0.01;
    Warning(1, "%s\n",
	    "Low cutoff frequency should be positive. Setting it to 0.01.");
  }
  if (cut_low_s > 1.0) {
    cut_low_s = 1.0;
    Warning(1, "%s\n",
	    "Low cutoff frequency should not exceed 1.0. Setting it to 1.0.");
  }
  if (cut_high_s <= 0.0) {
    cut_high_s = 0.01;
    Warning(1, "%s\n",
	    "High cutoff frequency should be positive. Setting it to 0.01.");
  }
  if (cut_high_s > 1.0) {
    cut_high_s = 1.0;
    Warning(1, "%s\n",
	    "High cutoff frequency should not exceed 1.0. Setting it to 1.0.");
  }

  return;

} /* check_options() */

#ifndef FUNCPROTO
static void init_band_data()
#else /* FUNCPROTO */
static void init_band_data(void)
#endif /* FUNCPROTO */
{
  int i;

  band_data_s = (bandData *) malloc(sizeof(bandData) * num_images_s);

  for (i = 0; i < num_images_s; i++) {
    band_data_s[i] = (bandData) malloc(sizeof(_bandData_));
    band_data_s[i]->band      = NULL;
    band_data_s[i]->redisplay = False;
    band_data_s[i]->show      = False;
  }

  num_image_widgets_s = 0;

  if (win_s == 1) {
    band_data_s[INIMAGE]->show     = True; num_image_widgets_s++;
  }
  if (wfin_s == 1) {
    band_data_s[F_INIMAGE]->show   = True; num_image_widgets_s++;
  }
  if (wfilt_s == 1) {
    band_data_s[FILTIMAGE]->show   = True; num_image_widgets_s++;
  }
  if (wffilt_s == 1) {
    band_data_s[F_FILTIMAGE]->show = True; num_image_widgets_s++;
  }
  if (wout_s == 1) {
    band_data_s[OUTIMAGE]->show    = True; num_image_widgets_s++;
  } else rms_s = False;

  if (wfout_s == 1) {
    band_data_s[F_OUTIMAGE]->show  = True; num_image_widgets_s++;
  }
  if (wcross_s == 1) {
    band_data_s[FILTCROSS]->show   = True; num_image_widgets_s++;
  }
  if (wdiff_s == 1) {
    band_data_s[DIFFIMAGE]->show   = True; num_image_widgets_s++;
  }

  return;

} /* init_band_data() */

#ifndef FUNCPROTO
static void assign_img_to_images()
#else /* FUNCPROTO */
static void assign_img_to_images(void)
#endif /* FUNCPROTO */
{
  Make_input_band();
  Make_input_four();
  if (filter_type_s != LOWPASS_IDEAL && filter_type_s != BUTTER_LP &&
	  filter_type_s != EXP_LP) {
    Make_filter();
    Make_filter_four();
  } else {
    Make_filter_four();
    if (band_data_s[FILTIMAGE]->show) Make_filter();
  }
  Make_filter_cross();
  Make_output_four();
  Make_output();
  Make_difference();

  return;

} /* assign_img_to_images() */

#ifndef FUNCPROTO
static void images_layout()
#else /* FUNCPROTO */
static void images_layout(void)
#endif /* FUNCPROTO */
{
  /* Allocate array of pointers. */
  layout_images_s = (layout_image *)
    malloc(sizeof(layout_image) * num_images_s);

  layout_images_s[INIMAGE]     =
    Make_image_layout(NULL, "Image", "image", "inimageShell",     NULL,
		      INIMAGE,     NULL, init_image_callbacks());

  layout_images_s[F_INIMAGE]   =
    Make_image_layout(NULL, "Image", "image", "f_inimageShell",   NULL,
		      F_INIMAGE,   NULL, init_image_callbacks());

  layout_images_s[FILTIMAGE]   =
    Make_image_layout(NULL, "Image", "image", "filtimageShell",   NULL,
		      FILTIMAGE,   NULL, init_image_callbacks());

  layout_images_s[F_FILTIMAGE] =
    Make_image_layout(NULL, "Image", "image", "f_filtimageShell", NULL,
		      F_FILTIMAGE, NULL, init_image_callbacks());

  layout_images_s[FILTCROSS]   =
    Make_image_layout(NULL, "Image", "image", "filtcrossShell",   NULL,
		      FILTCROSS,   NULL, init_image_callbacks());

  layout_images_s[OUTIMAGE]    =
    Make_image_layout(NULL, "Image", "image", "outimageShell",    NULL,
		      OUTIMAGE,    NULL, init_image_callbacks());

  layout_images_s[F_OUTIMAGE]  =
    Make_image_layout(NULL, "Image", "image", "f_outimageShell",  NULL,
		      F_OUTIMAGE,  NULL, init_image_callbacks());

  layout_images_s[DIFFIMAGE]  =
    Make_image_layout(NULL, "Image", "image", "diffimageShell",  NULL,
		      DIFFIMAGE,  NULL, init_image_callbacks());

  return;

} /* images_layout() */

#ifndef FUNCPROTO
static void prompts_layout()
#else /* FUNCPROTO */
static void prompts_layout(void)
#endif /* FUNCPROTO */
{
  char t[80];

  /* Allocate array of pointers. */
  layout_prompts_s = (layout_prompt *)
    malloc(sizeof(layout_prompt) * num_prompts_s);

  (void) sprintf(t, "%f", cut_low_s);
  layout_prompts_s[LOW_PROMPT] =
    Make_prompt_layout(NULL, "Prompt", "lowPrompt",           NULL, NULL,
			LOW_PROMPT, prompt_activate_cb, NULL, t);
			
  sprintf(t, "%f", cut_high_s);
  layout_prompts_s[HIGH_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "highPrompt",          NULL, NULL,
			HIGH_PROMPT, prompt_activate_cb, NULL, t);
			
  sprintf(t, "%d", filter_order_s);
  layout_prompts_s[ORDER_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "orderPrompt",         NULL, NULL,
			ORDER_PROMPT, prompt_activate_cb,
			(XtPointer) MallocIntP(ORDER_PROMPT), t);

  sprintf(t, "%e", f_inimage_eps_s);
  layout_prompts_s[F_INIMAGE_EPS_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "fInimageEpsPrompt",   NULL, NULL,
			F_INIMAGE_EPS_PROMPT, prompt_activate_cb,
			(XtPointer) MallocIntP(F_INIMAGE_EPS_PROMPT), t);

  sprintf(t, "%e", f_filtimage_eps_s);
  layout_prompts_s[F_FILTIMAGE_EPS_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "fFiltimageEpsPrompt", NULL, NULL,
			F_FILTIMAGE_EPS_PROMPT,	prompt_activate_cb,
			(XtPointer) MallocIntP(F_FILTIMAGE_EPS_PROMPT), t);

  sprintf(t, "%e", f_outimage_eps_s);
  layout_prompts_s[F_OUTIMAGE_EPS_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "fOutimageEpsPrompt",  NULL, NULL,
			F_OUTIMAGE_EPS_PROMPT, prompt_activate_cb,
			(XtPointer) MallocIntP(F_OUTIMAGE_EPS_PROMPT), t);

  sprintf(t, "%f", cross_min_s);
  layout_prompts_s[CROSS_MIN_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "crossMinPrompt",      NULL, NULL,
			CROSS_MIN_PROMPT, prompt_activate_cb,
			(XtPointer) MallocIntP(CROSS_MIN_PROMPT), t);

  sprintf(t, "%f", cross_max_s);
  layout_prompts_s[CROSS_MAX_PROMPT] = 
    Make_prompt_layout(NULL, "Prompt", "crossMaxPrompt",      NULL, NULL,
			CROSS_MAX_PROMPT, prompt_activate_cb,
			(XtPointer) MallocIntP(CROSS_MAX_PROMPT), t);

  return;

} /* prompts_layout() */

#ifndef FUNCPROTO
static void commands_layout()
#else /* FUNCPROTO */
static void commands_layout(void)
#endif /* FUNCPROTO */
{
  /* Allocate array of pointers. */
  layout_commands_s = (layout_command *)
    malloc(sizeof(layout_command) * num_commands_s);
  
  layout_commands_s[READ_INPUT]  =
    Make_command_layout(NULL, "Command", "read inimage",  NULL, NULL,
			READ_INPUT, ximage_work,
			(XtPointer) MallocIntP(READ_INPUT));

  layout_commands_s[SAVE_OUTPUT] =
    Make_command_layout(NULL, "Command", "save outimage", NULL, NULL,
			SAVE_OUTPUT, ximage_work,
			(XtPointer) MallocIntP(SAVE_OUTPUT));

  layout_commands_s[SAVE_FILTER] =
    Make_command_layout(NULL, "Command", "save filter",   NULL, NULL,
			SAVE_FILTER, ximage_work,
			(XtPointer) MallocIntP(SAVE_FILTER));

  layout_commands_s[QUIT]        =
    Make_command_layout(NULL, "Command", "quit",          NULL, NULL,
			QUIT, ximage_work, (XtPointer) MallocIntP(QUIT));

  return;

} /* commands_layout() */

#ifndef FUNCPROTO
static void labels_layout()
#else /* FUNCPROTO */
static void labels_layout(void)
#endif /* FUNCPROTO */
{
  /* Allocate array of pointers. */
  layout_labels_s = (layout_label *)
    malloc(sizeof(layout_label) * num_labels_s);
  
  layout_labels_s[FILTLABEL]             =
    Make_label_layout(NULL, "Label", "Filter type",
		      "xfilter control panel", NULL, FILTLABEL);

  layout_labels_s[LOW_LABEL]             =
    Make_label_layout(NULL, "Label", "Low cutoff",         NULL,
		       NULL, LOW_LABEL);

  layout_labels_s[HIGH_LABEL]            =
    Make_label_layout(NULL, "Label", "High cutoff",        NULL,
		       NULL, HIGH_LABEL);

  layout_labels_s[ORDER_LABEL]           =
    Make_label_layout(NULL, "Label", "Filter order",       NULL,
		       NULL, ORDER_LABEL);

  layout_labels_s[WINDLABEL]             =
    Make_label_layout(NULL, "Label", "Window type",        NULL,
		       NULL, WINDLABEL);

  layout_labels_s[RMS_DIFF]              =
    Make_label_layout(NULL, "Label", "rmsDiff",            NULL,
		       NULL, RMS_DIFF);

  layout_labels_s[F_INIMAGE_EPS_LABEL]   =
    Make_label_layout(NULL, "Label", "fInimageEpsLabel",   NULL,
		       NULL, F_INIMAGE_EPS_LABEL);

  layout_labels_s[F_FILTIMAGE_EPS_LABEL] =
    Make_label_layout(NULL, "Label", "fFiltimageEpsLabel", NULL,
		       NULL, F_FILTIMAGE_EPS_LABEL);

  layout_labels_s[F_OUTIMAGE_EPS_LABEL]  =
    Make_label_layout(NULL, "Label", "fOutimageEpsLabel",  NULL,
		       NULL, F_OUTIMAGE_EPS_LABEL);

  layout_labels_s[CROSS_MIN_LABEL]       =
    Make_label_layout(NULL, "Label", "crossMinLabel",      NULL, 
		       NULL, CROSS_MIN_LABEL);

  layout_labels_s[CROSS_MAX_LABEL]       =
    Make_label_layout(NULL, "Label", "crossMaxLabel",      NULL,
		       NULL, CROSS_MAX_LABEL);

  return;

} /* labels_layout() */

#ifndef FUNCPROTO
static void scrolls_layout()
#else /* FUNCPROTO */
static void scrolls_layout(void)
#endif /* FUNCPROTO */
{
  /* Allocate array of pointers. */
  layout_scrolls_s = (layout_scroll *)
    malloc(sizeof(layout_scroll) * num_scrolls_s);

  layout_scrolls_s[LOW_SCROLL]             =
    Make_scroll_layout(NULL, "Scroll", "lowScroll",           NULL, NULL,
			LOW_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(LOW_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(LOW_SCROLL));

  layout_scrolls_s[HIGH_SCROLL]            =
    Make_scroll_layout(NULL, "Scroll", "highScroll",          NULL, NULL,
			HIGH_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(HIGH_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(HIGH_SCROLL));

  layout_scrolls_s[ORDER_SCROLL]           =
    Make_scroll_layout(NULL, "Scroll", "orderScroll",         NULL, NULL,
			ORDER_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(ORDER_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(ORDER_SCROLL));

  layout_scrolls_s[F_INIMAGE_EPS_SCROLL]   =
    Make_scroll_layout(NULL, "Scroll", "fInimageEpsScroll",   NULL, NULL,
			F_INIMAGE_EPS_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(F_INIMAGE_EPS_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(F_INIMAGE_EPS_SCROLL));

  layout_scrolls_s[F_FILTIMAGE_EPS_SCROLL] =
    Make_scroll_layout(NULL, "Scroll", "fFiltimageEpsScroll", NULL, NULL,
			F_FILTIMAGE_EPS_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(F_FILTIMAGE_EPS_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(F_FILTIMAGE_EPS_SCROLL));

  layout_scrolls_s[F_OUTIMAGE_EPS_SCROLL]  =
    Make_scroll_layout(NULL, "Scroll", "fOutimageEpsScroll",  NULL, NULL,
			F_OUTIMAGE_EPS_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(F_OUTIMAGE_EPS_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(F_OUTIMAGE_EPS_SCROLL));
   
  layout_scrolls_s[CROSS_MIN_SCROLL]       =
    Make_scroll_layout(NULL, "Scroll", "crossMinScroll",      NULL, NULL,
			CROSS_MIN_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(CROSS_MIN_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(CROSS_MIN_SCROLL));

  layout_scrolls_s[CROSS_MAX_SCROLL]       =
    Make_scroll_layout(NULL, "Scroll", "crossMaxScroll",      NULL, NULL,
			CROSS_MAX_SCROLL,
			scroll_jump_cb,
			(XtPointer) MallocIntP(CROSS_MAX_SCROLL),
			scroll_incremental_cb,
			(XtPointer) MallocIntP(CROSS_MAX_SCROLL));

  return;

} /* scrolls_layout() */

#ifndef FUNCPROTO
static void menus_layout()
#else /* FUNCPROTO */
static void menus_layout(void)
#endif /* FUNCPROTO */
{
  layout_menus_s = (layout_menu *)
    malloc(sizeof(layout_menu) * num_menus_s);
  
  layout_menus_s[FILTMENU] =
    Make_menu_layout(NULL, "Menu", "Filter-menu", NULL, NULL, FILTMENU,
		      7, layout_entries_filt_s);

  layout_menus_s[WINDMENU] =
    Make_menu_layout(NULL, "Menu", "Window-menu", NULL, NULL, WINDMENU,
		      5, layout_entries_wind_s);

  return;

} /* menus_layout() */

#ifndef FUNCPROTO
static void entries_filt_layout()
#else /* FUNCPROTO */
static void entries_filt_layout(void)
#endif /* FUNCPROTO */
{
  /* Allocate array of pointers. */
  layout_entries_filt_s = (layout_menu_entry *)
    malloc(sizeof(layout_menu_entry) * num_entries_filt_s);
  
  layout_entries_filt_s[LOWPASS]       =
    Make_entry_layout(NULL, "lowpass",        NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(LOWPASS));

  layout_entries_filt_s[HIGHPASS]      =
    Make_entry_layout(NULL, "highpass",       NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(HIGHPASS));

  layout_entries_filt_s[BANDPASS]      =
    Make_entry_layout(NULL, "bandpass",       NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(BANDPASS));

  layout_entries_filt_s[BANDSTOP]      =
    Make_entry_layout(NULL, "bandstop",       NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(BANDSTOP));

  layout_entries_filt_s[LOWPASS_IDEAL] =
    Make_entry_layout(NULL, "lowpass_ideal",  NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(LOWPASS_IDEAL));

  layout_entries_filt_s[BUTTER_LP]     =
    Make_entry_layout(NULL, "butterworth_lp", NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(BUTTER_LP));

  layout_entries_filt_s[EXP_LP]        =
    Make_entry_layout(NULL, "exponential_lp", NULL,  NULL,
		       filt_select, (XtPointer) MallocIntP(EXP_LP));

  return;

} /* entries_filt_layout() */
  
#ifndef FUNCPROTO
static void entries_wind_layout()
#else /* FUNCPROTO */
static void entries_wind_layout(void)
#endif /* FUNCPROTO */
{
  /* Allocate array of pointers. */
  layout_entries_wind_s = (layout_menu_entry *)
    malloc(sizeof(layout_menu_entry) * num_entries_wind_s);

  layout_entries_wind_s[RECTANGLE] =
    Make_entry_layout(NULL, "rectangle", NULL, NULL,
		      window_select, (XtPointer) MallocIntP(RECTANGLE));

  layout_entries_wind_s[BARTLETT]  =
    Make_entry_layout(NULL, "bartlett",  NULL, NULL,
		      window_select, (XtPointer) MallocIntP(BARTLETT));

  layout_entries_wind_s[TRIANGLE]  =
    Make_entry_layout(NULL, "triangle",  NULL, NULL,
		      window_select, (XtPointer) MallocIntP(TRIANGLE));

  layout_entries_wind_s[HAMMING]   =
    Make_entry_layout(NULL, "hamming",   NULL, NULL,
		      window_select, (XtPointer) MallocIntP(HAMMING));

  layout_entries_wind_s[HANNING]   =
    Make_entry_layout(NULL, "hanning",   NULL, NULL,
		      window_select, (XtPointer) MallocIntP(HANNING));

  return;

} /* entries_wind_layout() */
  
#ifndef FUNCPROTO
static void prepare_for_layout()
#else /* FUNCPROTO */
static void prepare_for_layout(void)
#endif /* FUNCPROTO */
{
  int i = 0;

  labels_layout();
  commands_layout();
  prompts_layout();
  scrolls_layout();
  entries_filt_layout();
  entries_wind_layout();
  menus_layout();
  
  /* Put the widgets in the right order into layouts_s. */

  /* Allocate array of pointers. */  
  layouts_s = (layout *) malloc(sizeof(layout) * num_widgets_s);
  
  if (band_data_s[INIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[INIMAGE];
  if (band_data_s[F_INIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[F_INIMAGE];
  if (band_data_s[FILTIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[FILTIMAGE];
  if (band_data_s[F_FILTIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[F_FILTIMAGE];
  if (band_data_s[FILTCROSS]->show)
    layouts_s[i++] = (layout) layout_images_s[FILTCROSS];
  if (band_data_s[OUTIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[OUTIMAGE];
  if (band_data_s[F_OUTIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[F_OUTIMAGE];
  if (band_data_s[DIFFIMAGE]->show)
    layouts_s[i++] = (layout) layout_images_s[DIFFIMAGE];

  layouts_s[i++] = (layout) layout_labels_s[FILTLABEL];
  layouts_s[i++] = (layout) layout_menus_s[FILTMENU];
  layouts_s[i++] = (layout) layout_labels_s[LOW_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[LOW_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[LOW_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[HIGH_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[HIGH_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[HIGH_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[ORDER_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[ORDER_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[ORDER_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[WINDLABEL];
  layouts_s[i++] = (layout) layout_menus_s[WINDMENU];
  layouts_s[i++] = (layout) layout_labels_s[RMS_DIFF];
  layouts_s[i++] = (layout) layout_labels_s[F_INIMAGE_EPS_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[F_INIMAGE_EPS_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[F_INIMAGE_EPS_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[F_FILTIMAGE_EPS_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[F_FILTIMAGE_EPS_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[F_FILTIMAGE_EPS_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[F_OUTIMAGE_EPS_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[F_OUTIMAGE_EPS_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[F_OUTIMAGE_EPS_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[CROSS_MIN_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[CROSS_MIN_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[CROSS_MIN_SCROLL];
  layouts_s[i++] = (layout) layout_labels_s[CROSS_MAX_LABEL];
  layouts_s[i++] = (layout) layout_prompts_s[CROSS_MAX_PROMPT];
  layouts_s[i++] = (layout) layout_scrolls_s[CROSS_MAX_SCROLL];
  layouts_s[i++] = (layout) layout_commands_s[READ_INPUT];
  layouts_s[i++] = (layout) layout_commands_s[SAVE_OUTPUT];
  layouts_s[i++] = (layout) layout_commands_s[SAVE_FILTER];
  layouts_s[i++] = (layout) layout_commands_s[QUIT];

  return;

} /* prepare_for_layout() */

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  XtResource *all_resources;
  XrmOptionDescRec *all_options;
  int num_all_resources, num_all_options;
  char *local_usage = 
    "Usage: %s [<option>...] <inimage>\n\
    -low  <cut_l>    : Low cut-off frequency.\n\
    -high <cut_h>    : High cut-off frequency.\n\
    -order <size>    : Order of filter.\n\
    -ftype <type>    : Filter type.\n\
    -wtype <type>    : Window type.\n\
    -scale <scale>   : Scale-factor for display of Fourier magnitudes.\n\
    -logmin <min>    : Set all values smaller than 'min' after logarithmic\n\
                       compression for display of Fourier magnitudes equal\n\
                       to 'min'.\n\
    -cmin <min>      : Initial minimum value in cross-section display of\n\
                       filter FFT magnitude.\n\
    -cmax <max>      : Initial maximum value in cross-section display of\n\
                       filter FFT magnitude.\n\
    -cminmin <min>   : Smallest possible minimum value in cross-section\n\
		       display of filter FFT magnitude.\n\
    -cmaxmax <max>   : Largest possible maximum value in cross-section\n\
		       display of filter FFT magnitude.\n\
    -emin <min>      : Smallest possible eps for logarithmic compression of\n\
                       FFT magnitude.\n\
    -emax <max>      : Largest possible eps for logarithmic compression of\n\
                       FFT magnitude.\n\
    -ilin            : Use linear scrollbar for eps in logarithmic\n\
                       compression of FFT magnitude of input.\n\
    -flin            : Use linear scrollbar for eps in logarithmic\n\
                       compression of FFT magnitude of filter.\n\
    -olin            : Use linear scrollbar for eps in logarithmic\n\
                       compression of FFT magnitude of output.\n\
    -rms             : Do not calculate and display RMS difference between\n\
                       input and output images.\n\
    -wdiff           : Show widget for difference between output and input.\n\
    -win             : Do not show widget for input.\n\
    -wfin            : Show widget for FFT(input).\n\
    -wfilt           : Show widget for filter.\n\
    -wffilt          : Do not show widget for FFT(filter).\n\
    -wout            : Do not show widget for output.\n\
    -wfout           : Show widget for FFT(output).\n\
    -wcross          : Do not show widget for cross-section of\n\
                       FFT(filter).\n";

  /* Automatic error message display in BIFF format routines: on  */
  Iset_message(1);
  /* Automatic job abortion on error in BIFF format routines: off */
  Iset_abort(0);
  /* Install usage message (end multiple lines with \n\) */
			       
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));

  num_all_options   = XtNumber(opTable_s);
  all_options       = ximage_app_options(opTable_s, &num_all_options);

  toplevel_s = ximageInit(&argc, argv, "XFilter",
                        resources_s,            /* resources        */
                        XtNumber(resources_s),  /* num_resources    */
                        &resources_result_s,    /* resources result */
                        all_options,            /* options          */
                        num_all_options,        /* num_options      */
                        NULL,                   /* fallback         */
                        FALSE                   /* warn             */);

  win_s             = resources_result_s.widgetIn;
  wfin_s            = resources_result_s.widgetFourIn;
  wfilt_s           = resources_result_s.widgetFilt;
  wffilt_s          = resources_result_s.widgetFourFilt;
  wout_s            = resources_result_s.widgetOut;
  wfout_s           = resources_result_s.widgetFourOut;
  wcross_s          = resources_result_s.widgetCross;
  wdiff_s           = resources_result_s.widgetDiff;
  cross_min_min_s   = resources_result_s.crossMinMin;
  cross_max_max_s   = resources_result_s.crossMaxMax;
  cross_min_s       = resources_result_s.crossMin;
  cross_max_s       = resources_result_s.crossMax;
  eps_min_s         = resources_result_s.epsMin;
  eps_max_s         = resources_result_s.epsMax;
  cut_low_s         = resources_result_s.cutLow;
  cut_high_s        = resources_result_s.cutHigh;
  disp_scale_s      = resources_result_s.dispScale;
  log_min_s         = resources_result_s.logMin != 0 ?
                      resources_result_s.logMin : log_min_s;
  filter_order_s    = resources_result_s.filterOrder;
  filter_type_s     = parseFilterTypeName(resources_result_s.filterType);
  rms_s             = resources_result_s.rmsCalc;
  window_type_s     = parseWindowName(resources_result_s.windowType);
  f_inimage_log_s   = resources_result_s.linearFourIn;
  f_filtimage_log_s = resources_result_s.linearFourFilt;
  f_outimage_log_s  = resources_result_s.linearFourOut;

  check_options(&argc, argv);
  init_band_data();

  images_layout();
  layout_images_s[INIMAGE]->image.img = Iread_image(argv[1]);
  assign_img_to_images();

  num_widgets_s = num_image_widgets_s + num_prompts_s + num_scrolls_s + 
                  num_commands_s + num_labels_s  + num_menus_s;

  prepare_for_layout();
  ximageLayout(toplevel_s, layouts_s, num_widgets_s);
  update_rms();

  set_scrollbar_thumb(scroll_wid(LOW_SCROLL), cut_low_s, 0.0, 1.0, False);
  set_scrollbar_thumb(scroll_wid(HIGH_SCROLL), cut_high_s, 0.0, 1.0, False);
  set_scrollbar_thumb(scroll_wid(ORDER_SCROLL), (double) filter_order_s,
		      1.0, (double) inband_xsize_s, False);
  set_scrollbar_thumb(scroll_wid(F_INIMAGE_EPS_SCROLL), f_inimage_eps_s,
		      eps_min_s, eps_max_s, f_inimage_log_s);
  set_scrollbar_thumb(scroll_wid(F_FILTIMAGE_EPS_SCROLL), f_filtimage_eps_s,
		      eps_min_s, eps_max_s, f_filtimage_log_s);
  set_scrollbar_thumb(scroll_wid(F_OUTIMAGE_EPS_SCROLL), f_outimage_eps_s,
		      eps_min_s, eps_max_s, f_outimage_log_s);
  set_scrollbar_thumb(scroll_wid(CROSS_MIN_SCROLL), cross_min_s,
		      cross_min_min_s, cross_max_max_s, False);
  set_scrollbar_thumb(scroll_wid(CROSS_MAX_SCROLL), cross_max_s,
		      cross_min_min_s, cross_max_max_s, False);

  update_filter_menulabel();
  update_window_menulabel();
  update_sensitive();

  if (!band_data_s[F_INIMAGE]->show) {
    XtVaSetValues(scroll_wid(F_INIMAGE_EPS_SCROLL), XtNsensitive,  False,NULL);
    XtVaSetValues(label_wid(F_INIMAGE_EPS_LABEL),   XtNsensitive,  False,NULL);
    XtVaSetValues(prompt_wid(F_INIMAGE_EPS_PROMPT), XtNsensitive,  False,NULL);
  }
  if (!band_data_s[F_FILTIMAGE]->show) {
    XtVaSetValues(scroll_wid(F_FILTIMAGE_EPS_SCROLL),XtNsensitive, False,NULL);
    XtVaSetValues(label_wid(F_FILTIMAGE_EPS_LABEL),  XtNsensitive, False,NULL);
    XtVaSetValues(prompt_wid(F_FILTIMAGE_EPS_PROMPT),XtNsensitive, False,NULL);
  }
  if (!band_data_s[FILTCROSS]->show) {
    XtVaSetValues(scroll_wid(CROSS_MIN_SCROLL),XtNsensitive, False,NULL);
    XtVaSetValues(label_wid(CROSS_MIN_LABEL),  XtNsensitive, False,NULL);
    XtVaSetValues(prompt_wid(CROSS_MIN_PROMPT),XtNsensitive, False,NULL);
    XtVaSetValues(scroll_wid(CROSS_MAX_SCROLL),XtNsensitive, False,NULL);
    XtVaSetValues(label_wid(CROSS_MAX_LABEL),  XtNsensitive, False,NULL);
    XtVaSetValues(prompt_wid(CROSS_MAX_PROMPT),XtNsensitive, False,NULL);
  }
  if (!band_data_s[F_OUTIMAGE]->show) {
    XtVaSetValues(scroll_wid(F_OUTIMAGE_EPS_SCROLL), XtNsensitive, False,NULL);
    XtVaSetValues(label_wid(F_OUTIMAGE_EPS_LABEL),   XtNsensitive, False,NULL);
    XtVaSetValues(prompt_wid(F_OUTIMAGE_EPS_PROMPT), XtNsensitive, False,NULL);
  }

  ximage_mainloop();

  return(0);
}

#endif /* MAIN */
