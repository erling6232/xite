

/*

________________________________________________________________

        Xcontrol.c
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
#include XITE_MALLOC_H
#include <stdlib.h>
#include <sys/wait.h>
#include XITE_PARAM_H
#include <errno.h>
#include <ctype.h>
#include <xite/biff.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include "Xcontrol.h"
#include <xite/Xcolor.h>
#include "Xdisplay.h"
#include XITE_STDIO_H
#include <xite/utils.h>
#include XITE_STRING_H
#include XITE_FILE_H
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include <Xfwf/Toggle.h>
#include <Xfwf/Frame.h>
#include <Xfwf/Label.h>
#include <Xfwf/Button.h>
#include "Xprog.h"
#include <xite/Xpty.h>
#include <xite/ShellWids.h>
#include "Macro.h"
#include <xite/message.h>
#include <xite/debug.h>
#include XITE_UNISTD_H

#define CW XtCreateManagedWidget
#define VW XtVaCreateManagedWidget

extern Display *dpy_e;
extern Widget Control_e;
extern char *xiteHelper_e, *xiteHelperOption_e;
extern Atom wm_delete_window;   /* for ICCCM delete window */
extern int logpos_e;            /* Defined in xshow.c. */

static Widget Help_s, Macro_s, Quit_s, 
              Images_s, Jobs_s, Mouse_s, Name_s, Zoom_s, Pos_s, Stderr_s;
static int
  images_s=0, /* Number of image widgets displayed. */
  jobs_s=0,   /* Number of active jobs.  */
  mouse_s=0,  /* Current state of mouse. */
  message_to_stderr_s=0; /* Whether to send message to widget or stderr. */



#ifdef FUNCPROTO
void HelpXshow(Widget wid, XtPointer c_data, XtPointer call_data);
#else
void HelpXshow(/* Widget wid, XtPointer c_data, XtPointer call_data */);
#endif /* FUNCPROTO */

#ifndef FUNCPROTO
static void MessageAction(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void MessageAction(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  int width, height;
  float zoom;
  double factor;
  char *title;
  if (params[0][0] != 'L') {
    XtVaGetValues(wid,
		  XtNimageTitle, &title,
		  XtNimageWidth, &width,
		  XtNimageHeight, &height,
		  NULL);
    ImageGetZoom((ImageWidget) wid, &zoom, NULL, NULL, &factor);
    Message_name(title);
    Message_zoom(factor, zoom);
    if (!logpos_e) Message_size(width, height);
  } else {
    Message_name("");
    Message_zoom(0.0,0.0);
    Message_size(0,0);
  }

  return;

} /* MessageAction() */



/*L:cbHandler*

________________________________________________________________

		cbHandler
________________________________________________________________

Name:		cbHandler
Syntax:		static void cbHandler
Description:	Same as Handler in xshow.c, but with a parameter structure
                to fit as an XtCallbackProc
Files:		
References:	Handler in xshow.c
________________________________________________________________

*/

#ifndef FUNCPROTO
static void cbHandler(wid, closure, call_data)
Widget wid;
XtPointer closure, call_data;
#else /* FUNCPROTO */
static void cbHandler(Widget wid, XtPointer closure, XtPointer call_data)
#endif /* FUNCPROTO */
{
  PopMessage();
  Message(2, "xshow aborted ...\n");
  XptyKill(NULL);
  exit(2);
}



#ifndef FUNCPROTO
static void quit_xshow(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void quit_xshow(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Xcontrol.c: quit_xshow");

  PopMessage();
  Message(2, "xshow aborted ...\n");
  XptyKill(NULL);
  exit(2);

  LEAVE_FUNCTION_DEBUG("Xcontrol.c: quit_xshow");

} /* quit_xshow() */

/*L*

________________________________________________________________

		Init_control
________________________________________________________________

Name:		Init_control
Description:    Add the action routines Message and QuitXshow to the
		application context and register them with the translation
		manager.
Author:		Otto Milvang and Svein Bøe
________________________________________________________________

*/

#ifndef FUNCPROTO
void Init_control()
#else /* FUNCPROTO */
void Init_control(void)
#endif /* FUNCPROTO */
{
  Widget w[4];
  Dimension ww, wm, Mw, Cw, Iw, Mh;
  int MFd, CFd, IFd, Md, Cd, Id, i;
  Widget Form, MForm, MLabel, CForm, CLabel, IForm, ILabel;
  XSizeHints hints;
  log_macro_data *log_macro_dataP;
  XFontStruct *font;
  Atom *protocols;
  char txt[200];
  
  ENTER_FUNCTION_DEBUG("Xcontrol.c: Init_control");

  AddAction(Control_e, "Message",     (XtActionProc) MessageAction, 0);
  AddAction(Control_e, "QuitXshow",   quit_xshow,  0);

  Form   = VW("mainForm", formWidgetClass, Control_e, NULL);

  MForm  = VW("menuForm", formWidgetClass, Form, XtNleft, XtChainLeft, NULL);

  w[0] = MLabel  = VW("menuLabel",     xfwfLabelWidgetClass,   MForm, NULL);
  w[1] = Macro_s = VW("action1Button", xfwfToggleWidgetClass,  MForm,
		      XtNfromVert, MLabel,  NULL);
  w[2] = Help_s  = VW("action2Button", xfwfButtonWidgetClass, MForm,
		      XtNfromVert, Macro_s, NULL);
  w[3] = Quit_s  = VW("action3Button", xfwfButtonWidgetClass, MForm,
		      XtNfromVert, Help_s,  NULL);

  log_macro_dataP         = XtNew(log_macro_data);
  log_macro_dataP->wid    = Control_e;
  log_macro_dataP->dpy    = dpy_e;
  log_macro_dataP->screen = DefaultScreen(dpy_e);

  XtAddCallback(Macro_s, XtNonCallback, LogMacro, (XtPointer) log_macro_dataP);
  XtAddCallback(Macro_s, XtNoffCallback, LogMacro, (XtPointer)log_macro_dataP);
  XtAddCallback(Help_s,  XtNactivate,   HelpXshow, NULL);
  XtAddCallback(Quit_s,  XtNactivate,   cbHandler, NULL);

  wm = 0;
  for (i=0; i<4; i++) {
    XtVaGetValues(w[i], XtNwidth, &ww, NULL);
    if (ww > wm) wm = ww;
  }
  wm = 1.2 *wm;
  for (i=0; i<4; i++) XtVaSetValues(w[i], XtNwidth, wm, NULL);

  CForm  = VW("infoForm", formWidgetClass, Form, XtNfromHoriz, MForm,
	      XtNfromHoriz, MForm, NULL);
  w[0] = CLabel   = CW("infoLabel",   xfwfLabelWidgetClass,   CForm, NULL, 0);
  w[1] = Images_s = VW("imagesLabel", xfwfLabelWidgetClass,   CForm,
		       XtNfromVert, CLabel, NULL);
  w[2] = Jobs_s   = VW("jobsButton",  xfwfButtonWidgetClass, CForm,
		       XtNfromVert, Images_s, NULL);
  w[3] = Mouse_s  = VW("mouseButton", xfwfToggleWidgetClass,  CForm,
		       XtNfromVert, Jobs_s, NULL);

  XtAddCallback(Jobs_s,  XtNactivate,    XptyList, NULL);
  XtAddCallback(Mouse_s, XtNonCallback,  Prog_mouse, NULL);
  XtAddCallback(Mouse_s, XtNoffCallback, Prog_mouse, NULL);

  wm = 0;
  for (i=0; i<4; i++) {
    XtVaGetValues(w[i], XtNwidth, &ww, NULL);
    if (ww > wm) wm = ww;
  }
  wm = 1.2 *wm;
  for (i=0; i<4; i++) XtVaSetValues(w[i], XtNwidth, wm, NULL);
  
  IForm  = VW("activeForm", formWidgetClass, Form,
	      XtNfromHoriz, CForm,
	      XtNright,     XtChainRight, NULL);
  w[0] = ILabel = CW("activeLabel", xfwfLabelWidgetClass, IForm, NULL, 0);
  w[1] = Name_s = VW("imageNameLabel", xfwfLabelWidgetClass, IForm,
                     XtNlabel, "                                ",
		     XtNfromVert, ILabel, NULL);
  w[2] = Zoom_s = VW("zoomLabel", xfwfLabelWidgetClass, IForm,  
                     XtNlabel, "                                ",
		     XtNfromVert, Name_s, NULL);
  w[3] = Pos_s  = VW("dataLabel", xfwfLabelWidgetClass, IForm,  
                     XtNlabel, "                                ",
		     XtNfromVert, Zoom_s, NULL);

  XtVaGetValues(Zoom_s, XtNfont, &font, NULL);

  /* Longest exepcted string for position field. */
  (void) strcpy(txt, "x:99999, y=:99999, f(x,y):");
  (void) strcat(txt, "-1.32e-301+2.97e-301j");
  wm = XTextWidth(font, txt, strlen(txt));

  for (i=0; i<4; i++) {
    XtVaGetValues(w[i], XtNwidth, &ww, NULL);
    if (ww > wm) wm = ww;
  }
  for (i=0; i<4; i++) XtVaSetValues(w[i], XtNwidth, wm, NULL);

  XtVaGetValues(MForm,  XtNhorizDistance, &MFd, NULL);
  XtVaGetValues(CForm,  XtNhorizDistance, &CFd, NULL);
  XtVaGetValues(IForm,  XtNhorizDistance, &IFd, NULL);
  XtVaGetValues(MLabel, XtNwidth,         &Mw,
		        XtNheight,        &Mh,
		        XtNhorizDistance, &Md, NULL);
  XtVaGetValues(CLabel, XtNwidth,         &Cw,
		        XtNhorizDistance, &Cd, NULL);
  XtVaGetValues(ILabel, XtNwidth,         &Iw,
		        XtNhorizDistance, &Id, NULL);

  Stderr_s = VW("stderr", asciiTextWidgetClass,
		VW("stderr_frame", xfwfFrameWidgetClass, Form,
		XtNfromVert,         IForm,
		XtNleft,             XtChainLeft,
		XtNright,            XtChainRight, NULL),
		XtNwidth,            Mw  + CFd + Cd + Cw + IFd + Id + Iw + 12,
		XtNheight,           100 + Mh * 6,
		XtNhorizDistance,    MFd + Md,
		XtNscrollVertical,   XawtextScrollAlways,
		XtNscrollHorizontal, XawtextScrollWhenNeeded,
		NULL);
  XtRealizeWidget(Control_e);
  XawTextDisplayCaret(Stderr_s, 0);
  hints.flags = PPosition;
  hints.x = 250;
  hints.y = 10;

  XmbSetWMProperties(dpy_e, XtWindow(Control_e), 
		   "xshow - control", "xshow - control",
		   NULL, 0, &hints, NULL, NULL);
  protocols = (Atom *) malloc(sizeof(Atom));
  protocols[0] = wm_delete_window;
  (void) XSetWMProtocols(dpy_e, XtWindow(Control_e), protocols, 1);

  XtInstallAccelerators(Form, Quit_s);
  XtInstallAccelerators(Form, Help_s);
  XtInstallAccelerators(Form, Macro_s);

  LEAVE_FUNCTION_DEBUG("Xcontrol.c: Init_control");
}



#ifndef FUNCPROTO
void Message_images(delta)
int delta;
#else /* FUNCPROTO */
void Message_images(int delta)
#endif /* FUNCPROTO */
{
  static char mess[20];
  Dimension w;

  images_s += delta;
  sprintf(mess, "Images : %2d", images_s);
  XtVaGetValues(Images_s, XtNwidth, &w, NULL);
  XtVaSetValues(Images_s, XtNlabel, mess, XtNwidth, w, NULL);
}

#ifndef FUNCPROTO
void Message_jobs(delta)
int delta;
#else /* FUNCPROTO */
void Message_jobs(int delta)
#endif /* FUNCPROTO */
{
  static char mess[20];
  Dimension w;

  jobs_s += delta;
  sprintf(mess, "Jobs   : %2d", jobs_s);
  XtVaGetValues(Jobs_s, XtNwidth, &w, NULL);
  XtVaSetValues(Jobs_s, XtNlabel, mess, XtNwidth, w, NULL);
}

#ifndef FUNCPROTO
void Message_mouse(state)
char *state;
#else /* FUNCPROTO */
void Message_mouse(char *state)
#endif /* FUNCPROTO */
{
  Cardinal arg_cnt;
  Dimension w;
  static char mess[20];

  mouse_s = (*state == 'J');
  sprintf(mess, "Mouse  : %2s", state);
  arg_cnt = 0;
 
  XtVaGetValues(Mouse_s, XtNwidth, &w, NULL);
  XtVaSetValues(Mouse_s,
		XtNlabel,     mess, 
		XtNwidth,     w, 
		XtNon,        mouse_s, 
		XtNsensitive, mouse_s, NULL); 
}

#ifndef FUNCPROTO
void Message_name(name)
char *name;
#else /* FUNCPROTO */
void Message_name(char *name)
#endif /* FUNCPROTO */
{
  static char mess[80];
  Dimension w;

  sprintf(mess, "%s",name);
  XtVaGetValues(Name_s, XtNwidth, &w, NULL);
  XtVaSetValues(Name_s, XtNlabel, mess, XtNwidth, w, NULL);
}

#ifndef FUNCPROTO
void Message_zoom(f, zoom)
double f, zoom;
#else /* FUNCPROTO */
void Message_zoom(double f, double zoom)
#endif /* FUNCPROTO */
{
  static char mess[80];
  Dimension w;

  if (zoom) sprintf(mess, "Zoom:%.2f, %.2f%% of image", zoom, f*100);
  else mess[0] = '\0';
  XtVaGetValues(Zoom_s, XtNwidth, &w, NULL);
  XtVaSetValues(Zoom_s, XtNlabel, mess, XtNwidth, w, NULL);
}

#ifndef FUNCPROTO
void Message_size(x, y)
int x, y;
#else /* FUNCPROTO */
void Message_size(int x, int y)
#endif /* FUNCPROTO */
{
  static char mess[80];
  Dimension w;

  if (x == 0 && y == 0) mess[0] = '\0';
  else sprintf(mess, "Width:%d, Height:%d", x, y);

  XtVaGetValues(Pos_s, XtNwidth, &w, NULL);
  XtVaSetValues(Pos_s, XtNlabel, mess, XtNwidth, w, NULL);
}

#ifndef FUNCPROTO
void Message_roi(x, y, w, h)
int x, y, w, h;
#else /* FUNCPROTO */
void Message_roi(int x, int y, int w, int h)
#endif /* FUNCPROTO */
{
  static char mess[80];
  Dimension ww;

  sprintf(mess, "x:%3d, y:%3d, (%3dx%3d)",x, y, w, h);
  XtVaGetValues(Pos_s, XtNwidth, &ww, NULL);
  XtVaSetValues(Pos_s, XtNlabel, mess, XtNwidth, ww, NULL);
}



#ifndef FUNCPROTO
int MessageToWidget(id, message)
int id;
char *message;
#else /* FUNCPROTO */
int MessageToWidget(int id, char *message)
#endif /* FUNCPROTO */
{
  char *buffer;

  if (message) buffer = XtNewString(message);
  else {
    message_to_stderr_s = 1;
    return(1);
  }

  if (message_to_stderr_s) {
    fprintf(stderr, "%s", buffer);
  } else {
    XtVaSetValues(Stderr_s,    XtNeditType, XawtextEdit, NULL);
    XtCallActionProc(Stderr_s, "insert-string", NULL, &buffer, 1);
    XtVaSetValues(Stderr_s,    XtNeditType, XawtextRead, NULL);
  }
  
  return(0);

} /* MessageToWidget() */

#ifndef FUNCPROTO
int MessageToStdout(id, fmt)
int id;
char *fmt;
#else /* FUNCPROTO */
int MessageToStdout(int id, char *fmt)
#endif /* FUNCPROTO */
{
  char *buffer;

  if (!fmt) return(1);

  buffer = XtNewString(fmt);
  printf("%s", buffer);
  
  return(0);

} /* MessageToStdout() */



#ifndef FUNCPROTO
static int startHelper(browser, option, html_filename, URL)
char *browser, *option, *html_filename, *URL;
#else /* FUNCPROTO */
static int startHelper(char *browser, char *option, char *html_filename, char *URL)
#endif /* FUNCPROTO */
{
  int errNo;

  if (!browser) return(-1);

  if (strcmp(browser, "man") == 0) {
    if (option)
      (void) execlp("xterm", "xterm", "-e", browser,
		      option, "xshow", NULL);
    else
      (void) execlp("xterm", "xterm", "-e", browser,
		      "xshow", NULL);

    errNo  = errno;

  } else if (strcmp(browser, "xman") == 0) {

    if (option)
      (void) execlp(browser, browser, option, NULL);
    else
      (void) execlp(browser, browser, NULL);

    errNo  = errno;

  } else if (strcmp(browser, "netscape") == 0) {
    char *arg, *pre = "openURL(", *post=",new-window)";

    if (option && strcmp(option, "-remote") == 0) {
      arg = (char *) malloc(strlen(pre)  + strlen(URL) +
			    strlen(post) + 1);

      (void) sprintf(arg, "%s%s%s", pre, URL, post);
    } else {
      arg = URL;
    }

    /* Return from execlp only in case of error. */
    if (option) (void) execlp(browser, browser, option, arg, NULL);
    else (void) execlp(browser, browser, arg, NULL);
    errNo  = errno;
    if (arg != URL) free(arg);

  } else if (strcmp(browser, "Mosaic") == 0) {
    (void) execlp(browser, browser, URL, NULL);

  } else if (strcmp(browser, "lynx") == 0) {
    if (option)
      (void) execlp("xterm", "xterm", "-e", browser,
		    option, html_filename, NULL);
    else
      (void) execlp("xterm", "xterm", "-e", browser, html_filename, NULL);
    errNo  = errno;
  } else if (strcmp(browser, "more") == 0 || strcmp(browser, "less") == 0) {
    (void) execlp("xterm", "xterm", "-e", browser, option, NULL);
  } else if (strcmp(browser, "cat")  == 0) {
    (void) execlp(browser, browser, option, NULL);
  } else {
    if (option) (void) execlp(browser, browser, option, NULL);
    else        (void) execlp(browser, browser, NULL);
  }

  return(errNo);

} /* startHelper() */

#ifndef FUNCPROTO
static void getHelperEnv(xiteHelper, helperOption, html_filename, URL, cat_filename)
char **xiteHelper, **helperOption, **html_filename, **URL, **cat_filename;
#else /* FUNCPROTO */
static void getHelperEnv(char **xiteHelper, char **helperOption, char **html_filename, char **URL, char **cat_filename)
#endif /* FUNCPROTO */
{
  *xiteHelper   = getenv("XITE_HELPER");
  if (*xiteHelper == NULL && xiteHelper_e != NULL)
    *xiteHelper = xiteHelper_e;

  *helperOption = getenv("XITE_HELPER_OPTION");
  if (*helperOption == NULL && xiteHelperOption_e != NULL)
    *helperOption = xiteHelperOption_e;

  *html_filename = getenv("XITE_DOC");
  if (*html_filename) {
    *html_filename = strcpy((char *)
		       malloc(strlen(*html_filename) + 1 +
			      strlen("/ReferenceManual/Contents.html")),
		       *html_filename);
    (void) strcat(*html_filename, "/ReferenceManual/Contents.html");

    *URL = (char *) malloc(strlen(*html_filename) + strlen("file:") + 1);
    sprintf(*URL, "file:%s", *html_filename);
  }

  *cat_filename = getenv("XITE_MAN");
  if (*cat_filename) {
    *cat_filename = strcpy((char *) malloc(strlen(*cat_filename) + 1 +
					   strlen("/cat1/xshow.1")),
			   *cat_filename);
    (void) strcat(*cat_filename, "/cat1/xshow.1");
  }

  return;

} /* getHelperEnv() */

#ifndef FUNCPROTO
static int checkHelper(browser, option, html_filename, cat_filename, attempt)
char *browser, **option, *html_filename, **cat_filename;
int attempt;
#else /* FUNCPROTO */
static int checkHelper(char *browser, char **option, char *html_filename, char **cat_filename, int attempt)
#endif /* FUNCPROTO */
{
  if (browser == NULL && attempt == 1) {
    Warning(1, "No help program specified.\n");
    return(1);
  } else if (browser == NULL) {
    /* Last browser successful or all browser attempts failed. */
    return(1);
  }

  if (html_filename == NULL &&
      (strcmp(browser, "netscape") == 0 || strcmp(browser, "Mosaic") == 0 ||
      strcmp(browser, "lynx") == 0)) {
    Warning(1, "%s\n%s\n",
	    "The environment variable XITE_DOC is not set.",
	    "Aborting help request.");
    return(1);
  }

  if (strcmp(browser, "more") == 0 || strcmp(browser, "less") == 0 ||
       strcmp(browser, "cat")  == 0) {
    if (*cat_filename == NULL && (*option) == NULL) {
      Warning(1, "%s\n%s%s%s\n%s\n",
	      "The environment variable XITE_MAN is not set, and no filename ",
	      "specified for ", browser, " in XITE_HELPER_OPTION.",
	      "Aborting help request.");
      return(1);
    }
    if (*cat_filename && (*option) == NULL) {
      free(*option);
      *option = *cat_filename;
      *cat_filename = NULL;
    }
  }

  return(0);

} /* checkHelper() */

#ifndef FUNCPROTO
static pid_t runHelperChild(browser, option, html_filename, URL, fd)
char *browser, *option, *html_filename, *URL;
int fd[];
#else /* FUNCPROTO */
static pid_t runHelperChild(char *browser, char *option, char *html_filename, char *URL, int fd[])
#endif /* FUNCPROTO */
{
  pid_t pid;

  if (option)
    Message(0, "Trying to start help program %s %s.\n", browser, option);
  else Message(0, "Trying to start help program %s.\n", browser);

  /* Use a trick with forking first a child and then a grandchild.
   * This is in order to avoid zoombie processes.
   * The first child will not do any work, only be waited upon by
   * its parent. It is terminated prior to termination of the grandchild.
   * The grandchild will do the work and be waited upon
   * by the kernel 'init' process.
   */
  if ((pid = fork()) < 0) {
    /* Parent process.
     * Something went wrong in the fork call.
     * No child process created.
     */
    Warning(1, "%s\n", strerror(errno));
    return(-1);
  } else if (pid == 0) {
    /* First child. */

    if ((pid = fork()) < 0) {
    /* First child, which will not do any work.
     * Something went wrong in the fork call.
     * No second child process created.
     */
    } else if (pid > 0) {
      /* Parent of second child, i.e. first child.
       * Terminate (waited for by original parent further down).
       */
      _exit(0);
    } else {
      /* Child of child. This will do the work.
       * startHelper should never return if starting the help program
       * succeeds.
       * If it returns, then exit from child process.
       */

      char buf[MAXPATHLEN];
      pid_t pidc;

      /* Make sure first child terminates before second child.
       * First child is waited for by parent and is thus ok.
       * Second child will be adopted by kernel 'init' and waited for
       * when terminating.
       */

      sleep(2);     
      close(fd[0]); /* Close reading end of pipe. */

      pidc = getpid();

      /* Send message to parent, containing the pid. */
      sprintf(buf, "%d", pidc);
      write(fd[1], buf, strlen(buf));
      (void) startHelper(browser, option, html_filename, URL);

      /* This is reached only in case of failure.
       * Send pid again and then name of browser.
       * Exit from second child process (waited for by kernel 'init'). */

      sprintf(buf, " %d %s\n", pidc, browser);
      write(fd[1], buf, strlen(buf));
      close(fd[1]);
      _exit(0);
    } /* child of child */
  } /* pid == 0 */

  return(pid);

} /* runHelperChild() */

#ifndef FUNCPROTO
static int checkHelperSuccess(fd)
int fd[];
#else /* FUNCPROTO */
static int checkHelperSuccess(int fd[])
#endif /* FUNCPROTO */
{
  char  ptr[MAXPATHLEN], browser[MAXPATHLEN];
  int   n, ok = 1;
  pid_t pid, pid2;

  /* Read from pipe, message from child. */
  ptr[0] = '\0';
  if ((n = read(fd[0], ptr, MAXPATHLEN)) < 0) {
    Warning(1, "%s\n", strerror(errno));
    return(-1);
  }
  ptr[n] = '\0';

  /* Check whether child failed or not. Success is indicated by
   * occurence of only one number, while failure is indicated by
   * occurence of two numbers and a string.
   */
  n = sscanf(ptr, "%d %d", &pid, &pid2);
  if (n == 2) {
    /* Two numbers found. */
    ok = 0;
    sscanf(ptr, "%d %d %s", &pid, &pid2, browser);
    Warning(1, "Help program %s failed.\n", browser);
  }

  return(ok);

} /* checkHelperSuccess() */




/*F:HelpXshow*

________________________________________________________________

		HelpXshow
________________________________________________________________

Name:		HelpXshow - Start help program for xshow.
Syntax:         | #include "Xcontrol.h"
		| 
                | void HelpXshow( Widget wid, XtPointer c_data,
                |    XtPointer call_data );

Description:	Callback function which starts a help program. 'c_data' and
                'call_data' are not used.

Restrictions:	A program given by the environment variable XITE_HELPER or
                else the X application resource xiteHelper, is started.
		The environment variable XITE_HELPER_OPTION or else the
		X application resource xiteHelperOption, can provide an
		option for the program. See the section 'Environment' below.

		A few special programs are recognized and given special
		treatment: 'netscape', 'Mosaic', 'lynx', 'man', 'xman',
		'more', 'less', 'cat'.

		The 'WWW browsers' netscape, Mosaic and lynx will start with
		the page referred to by
		$XITE_DOC/ReferenceManual/Contents.html, i.e. with URL
		file:$XITE_DOC/ReferenceManual/Contents.html. Options are not
		passed on to Mosaic. lynx will run in a separate xterm window.

		'man' will run "man xshow" in a separate xterm window. 'xman'
		will simply start.

		'cat', 'more' and 'less' will start with
		$XITE_MAN/cat1/xshow.1 as argument, unless overridden by
		an option. 'more' and 'less' will run in a separate xterm
		window.

		If the specified program is not recognized, an attempt will be
		made to start it with the option (if present) as argument.

Return value:	None

Environment:    The following environment variables are used.

                &XITE_HELPER
		Determines which program is started. It may be a
		colon-separated list of help program names. The first list
		element is started. If it fails, try the second element, and
		so on. A typical list will be

		| netscape:Mosaic:lynx:man:xman:more:less:cat

		Put the preferred program in the front of the list. If not
		set, use the xshow application X resource 'xiteHelper'.

                &XITE_HELPER_OPTION
		Supplies an option or command line argument for the
		corresponding command in XITE_HELPER. Not used for Mosaic. If
		not set, use the xshow application X resource
		xiteHelperOption. This variable can be used to override the
		default filename if the help program is 'more', 'less' or
		'cat'.

		&XITE_DOC
		Determines what html page to start with for the WWW browsers
		netscape, Mosaic and lynx. The starting page will be
		$XITE_DOC/ReferenceManual/Contents.html.

		$XITE_MAN
		Determines where to find the formatted manual page if
		'more', 'less' or 'cat' is the help program started. The
		formatted manual page should be in $XITE_MAN/cat1/xshow.1.
		This can be overridden by setting XITE_HELPER_OPTION.

Author:		Svein Bøe. This is a complete replacement of an older
                HelpXshow function which opened an Ascii text widget with
		the contents of the file $XITE_HOME/man/cat1/xshow.1.

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
void HelpXshow(wid, c_data, call_data)
Widget wid;
XtPointer c_data, call_data;
#else /* FUNCPROTO */
void HelpXshow(Widget wid, XtPointer c_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  pid_t pid;
  char *browser = NULL, *option = NULL,
       *xiteHelper = NULL, *helperOption = NULL,
       *html_filename = NULL, *cat_filename = NULL,
       *URL = NULL;
  int  attempt = 0,
       fd[2], ok = 0;
  
  getHelperEnv(&xiteHelper, &helperOption, &html_filename, &URL,
	       &cat_filename);

  /* Prepare for communication between parent and child processes. */
  if (pipe(fd) < 0) {
    Warning(1, "%s\n", strerror(errno));
    return;
  }
  
  /* Run through list of browsers until successful or end of list. */
  while (ok == 0) {
    attempt++;
    NextFilename(&xiteHelper,   &browser);
    NextFilename(&helperOption, &option);
    
    if (checkHelper(browser, &option, html_filename, &cat_filename, attempt)) {
      close(fd[1]);
      break;
    }

    /* pid refers to first child. */
    if ((pid = runHelperChild(browser, option, html_filename, URL, fd)) < 0)
      break;

    /* Wait for first child which exits immediately. */
    if (waitpid(pid, NULL, 0) != pid) {
      Warning(1, "%s\n", strerror(errno));

      close(fd[1]);
      break;
    }

    close(fd[1]);   /* Close writing end of pipe. */

    ok = checkHelperSuccess(fd);

    if (browser) { free(browser); browser = NULL; }
    if (option)  { free(option);  option  = NULL; }

  } /* while more browsers */
  
  close(fd[0]);
  if (URL) free(URL);
  if (html_filename) free(html_filename);
  if (cat_filename)  free(cat_filename);
}
