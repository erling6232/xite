
/* $XConsortium: HistogramP.h,v 1.2 88/10/25 17:37:59 swick Exp $ */
/* Copyright	Massachusetts Institute of Technology	1987, 1988 */

#ifndef _HistogramP_h
#define _HistogramP_h

_XITE_CPLUSPLUS_BEGIN

#include "Histogram.h"
/* include superclass private header file */
#include <X11/CoreP.h>

/* define unique representation types not found in <X11/StringDefs.h> */

#define XtRHistogramResource		"HistogramResource"

typedef struct {
  int count;
} HistogramClassPart;

typedef struct _HistogramClassRec {
    CoreClassPart	core_class;
    HistogramClassPart	histogram_class;
} HistogramClassRec;

extern HistogramClassRec histogramClassRec;

typedef struct {

    /* resources */
    int               *histogram;    /* Histogram array. */
    int               length;        /* Number of histogram bins. */
    char              *title;
    int               planes;        /* Depth of visual. */
    Visual            *visual;
    Colormap          colormap;
    unsigned char     ***image;
    unsigned char     **band;
    XtCallbackList    crossing_action;
    XtCallbackList    button_action;
    XtCallbackList    motion_action;
    XtCallbackList    kill_action;

    /* private state */
    GC       gc;
    XFontStruct *font_info;
    int      korh;                   /* Boolean for cumulative or histogram. */
    int      *kumulativ;             /* Cumulative histogram array. */
    int      *dhistogram;            /* Display-ready normalized histogram. */
    int      *dkumulativ;            /* Display-ready normalized cumulative. */
    unsigned char red[256];          /* Red manipulating curve. */
    unsigned char green[256];        /* Green mainpulating curve. */
    unsigned char blue[256];         /* Blue manipulating curve. */
    unsigned char mask[256];         /* Only set when curve must be drawn. */
} HistogramPart;

typedef struct _HistogramRec {
    CorePart      core;
    HistogramPart histogram;
} HistogramRec;

_XITE_CPLUSPLUS_END

#endif  /* _HistogramP_h */
