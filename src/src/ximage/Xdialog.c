
#ifndef lint
static char XRNrcsid[] = "$Header$";
#endif

/*
 * xrn - an X-based NNTP news reader
 *
 * Copyright (c) 1988, 1989, Ellen M. Sentovich and Rick L. Spickelmier.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the University of California not
 * be used in advertising or publicity pertaining to distribution of 
 * the software without specific, written prior permission.  The University
 * of California makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 * THE UNIVERSITY OF CALIFORNIA DISCLAIMS ALL WARRANTIES WITH REGARD TO 
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * dialogs.c: create simple moded dialog boxes
 */

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Form.h>
#include <Xfwf/Button.h>
#include <Xfwf/FileSel.h>
#include <xite/ShellWids.h>
#include <xite/Xdialog.h>
#include XITE_MALLOC_H
#include <xite/utils.h>
#include <xite/mallocp.h>
#include <xite/strings.h>
#include <xite/debug.h>
#include <ctype.h>

#define BUTTON1_PUSHED 0
#define BUTTON2_PUSHED 1
#define WM_KILL_WINDOW (-1)
#define NOT_DONE (-2)

extern Atom wm_delete_window;
extern Display *dpy_e;

typedef struct {
  char *filename;
} FileSelInfo;

static int retVal_s = NOT_DONE;

/*F:FormDialog=SimpleDialog*/
/*F:CreateDialog=SimpleDialog*/
/*F:InformationBox=SimpleDialog*/
/*F:ConfirmationBox=SimpleDialog*/
/*F:SelectReadFile=SimpleDialog*/
/*F:SelectWriteFile=SimpleDialog*/
/*F:init_button_table=SimpleDialog*/
/*F:SimpleDialog*

________________________________________________________________

		Dialogs
________________________________________________________________

Name:		SimpleDialog, FormDialog, CreateDialog, InformationBox,
                ConfirmationBox, SelectReadFile, SelectWriteFile,
		init_button_table - Popup dialog widgets

Syntax:		| #include <xite/Xdialog.h>
                |
		| int SelectReadFile( Widget parent, char *kind,
		|    char **return_text );
		|
		| int SelectWriteFile( Widget parent, char *kind,
		|    char **return_text );
		|
		| int FormDialog( Widget parent, char *header,
		|    char *dialog_spec, char **return_text,
		|    button_table buttons, int nbuttons,
		|    char *winName, char *iconName );
		|
		| int InformationBox( Widget parent, char *message,
		|    char *winName, char *iconName );
		|
		| int ConfirmationBox( Widget parent,
		|    char *message, char *buttonname1,
		|    char *buttonname2, char *winName,
		|    char *iconName );
		|
                | int SimpleDialog(Widget parent, char *title,
                |    char *deftext, char **return_text,
		|    char *buttonname1, char *buttonname2);
		|
		| Widget CreateDialog( Widget parent, char *title,
		|    char *textField, char *winName, char *iconName,
		|    struct DialogArg *args, unsigned int count );
		|
		| button_table init_button_table(
		|    button_table buttons, int length );
		|
Description:    
                &SelectReadFile

		Creates and pops up a file selection window which returns the
		chosen filename through 'return_text'. The file-selection
		window contains a goto-button (change directory), a
		select-button (take the selected filename and return), an
		up-button (change to parent directory) and a cancel-button.
		The instance name of the shell widget is "fileSelectorShell".

		The file-selector widget (descendant of "fileSelectorShell")
		is of class XfwfFileSelector. It has instance name given by
		'kind' (without the first character). The instance name when
		used in XITE is one of the string constants

		| readBIFFfile
		| readColortabfile
		| readColormapfile
		| readTIFFfile
		| readMATfile
		| readPSfile
		| readfile

		&&Actions

		    The 'QuitFileSel' action function is installed, but not
		    bound to any event. It will typically be bound to a
		    ClientMessage event from the window manager, desiring to
		    close the window.

		&&Return value

		    | 0 : button labeled cancel was pushed or the
		    |     QuitFileSel action function was invoked.
		    | 1 : button labeled select was pushed (or one
		    |     of the accelerators were used).

		&&Accelerators

		    Accelerators can be installed by the application (e.g. in
		    an X resource file) so that the return key can invoke the
		    callbacks of the goto-button (while mouse pointer is in
		    the directory-name field) or the callbacks of the
		    select-button (while mouse pointer is in the filename
		    field). A <Btn2Down> event in the file list can also
		    invoke the select-button callbacks.

		&&Resources

		    You may find some of the resources for the file-selector
		    widget particularly useful.

		    The directory path used:
		    | <widget class or instance>*currentDirectory:

		    Substitute <widget class or instance> by
		    XfwfFileSelector (class) or one of the instance
		    names mentioned above.

		    Examples
		    | XShow*XfwfFileSelector*currentDirectory: .
		    | XShow*XfwfFileSelector*currentDirectory:
		    |    $HOME/img

		    A pattern which the filenames, displayed in the
		    file list, must match:
		    | <widget class or instance>*pattern:

		    Example
		    |  XShow*readBIFFfile*pattern: *.img

		    Whether or not to demand existence of the specified file:
		    | <widget class or instance>*checkExistence:

		    Examples
		    |  XShow*readColortabfile*checkExistence:  True
		    |  XShow*writeMacrofile*checkExistence: False

		&SelectWriteFile

		This function is similar to 'SelectReadFile', except that
		the instance name of the fileselector widget is chosen among
		the string constants

		| writeBIFFfile
		| writeColortabfile
		| writeColormapfile
		| writeTIFFfile
		| writeMATfile
		| writePSfile
		| writefile
		| writeMacrofile

		&FormDialog

		'FormDialog' creates a window which contains various user
		input-fields and buttons.

		'header' will appear at the top of the window. Use a blank
		string if you simply want some space.

		'dialog_spec' is expected to be a series of specifications of
		the form
		| # <Leading text> # <option> # <type> # <default>
		All the specifications will be presented in a window, expecting
		the user to take some action. <Leading text> is typically
		a question, <default> is the suggested answer. The user can
		change the answer. The results of the user actions will be
		returned in the string 'return_text'.

		The <option> field in the dialog specification will be
		returned in front of the answer. It may be empty.

		The <type> field has the following interpretation

		&&d
		Input should be integer. Only digits and the minus-sign
		are allowed in the input. XfwfPrompt is used.

		&&f
		Input should be float (or double). Only digits, "." and the
		minus-sign are allowed in the input. XfwfPrompt is used.

		&&s
		Input should be string. All characters are allowed. The
		resulting input will be surrounded by "", to protect a
		multiword string input as a single string. Use the function
		'remove_quotes_from_word'(3) to remove these quotes.
		XfwfPrompt is used.

		&&b
		Input should be boolean. Toggle button of class XfwfToggle is
		used.

		&&r0
		Field is part of radio-group (of which exactly one member must
		be chosen). Members of the same radio-group must have the same
		leading text. Return default only if member is chosen.
		XfwfGroup is used.

		This member will initially be off.

		&&r1
		Same as "r0", but this member will initially be on.

		&&r0-
		Same as "r0", but don''t return default value (use only as
		visible label).

		&&r1-
		Same as "r1", but don''t return default value (use only as
		visible label).

		&&m0
		Field is part of multiple-choice group (of which none, one or
		several may be chosen). Members of the same multiple-choice
		group must have the same leading text.
		XfwfGroup is used.

		This member will initially be off.

		&&m1
		Same as "m0", but this member will initially be on.

		&&m0-
		Same as "m0", but don''t return default value (use only as
		visible label).

		&&m1-
		Same as "m1", but don''t return default value (use only as
		visible label).

		-&&

		For input fields of type 'd', 'f', and 's':

		&&Translations

		    | <EnterWindow>: highlight(Always)
		    | <LeaveWindow>: unhighlight()
		    | <Btn1Down>,<Btn1Up>: set() notify()
		
		-&&

		At the bottom of the window, a number of buttons (of class
		XfwfButton) may appear, depending on the 'buttons' and
		'nbuttons' arguments. With 'buttons' equal to NULL and
		'nbuttons' greater than zero and at most three, 'nbuttons'
		default buttons are made, in the following order:

		&&First button
		
		X11 resource instance name: button0, label: "Abort".
		The callback function will close the dialog window, and
		'FormDialog' will return the value 0.

		&&Second button
		X11 resource instance name: button1, label: "Accept".
		The callback function will process the answers given and return
		them in 'return_text'. Finally, the dialog is closed and
		'FormDialog' returns the value 1.

		&&Third button
		X11 resource instance name: button2, label: "Help".
		The callback function will create a help window with the
		default	text content "For help, consult a manual page.".

		-&&
		Custom buttons can be supplied by giving a non-NULL reference
		for 'buttons'. 'buttons' is a table of button descriptions

		| typedef {
		|   char *name, *label;
		|   XtCallbackProc callback;
		|   XtPointer closure;
		| } button_element, *button_table;

		'name' is the X11 resource instance name of the button widget,
		'label' is the visible text in the button, 'callback' is the
		callback procedure added to the button''s XtNactivate callback
		list, 'closure' is the data passed to the callback function
		in its 'client_data' argument. The function 'init_button_table'
		can be used to create and initialize a button table with all
		element members pointing to NULL.

		If 'name' is NULL, a default X11 resource instance name for
		the button will be used. This is the concatenation of the
		string "button" and the button number (starting with "0").

		If 'label' is NULL, the same default values as for the above
		three default buttons will be used for the first three
		buttons. For additional buttons, no default is supplied. Use
		X11 resources for 'name' to set it.

		With 'callback' equal to NULL, a default will be used. For the
		first three buttons the same functions will be used as for the
		three default buttons above. For additional buttons, the
		default callback function will close the dialog window and
		let 'FormDialog' return the button number (starting with 0).

		If 'closure' is NULL, the same default values as for the above
		three default buttons will be used (i.e. NULL, NULL and the
		default help text respectively). For additional buttons,
		the default is a reference to an integer which contains the
		button number (starting with 0).

		'FormDialog' is used by 'xshow' (from XITE version 3.3) to
		interpret menu files. Prior versions used 'SimpleDialog', but
		'FormDialog' has a more user-friendly interface.

		&InformationBox

		Creates a window containing 'message' and a command button
		with label "Close", using 'CreateDialog'. The window is popped
		down when the user pushes the button. If a translation is
		installed for the QuitDialog action function, then this could
		also pop down the window (e.g. bound to a ClientMessage event
		from the window manager).

		&&Return value
		    | -1: Dialog window deleted by the QuitDialog
		    |     action function. This is often bound to
		    |     a desire by the window manager to close
		    |     the window.
		    |  0: ok-button was pushed.

		&ConfirmationBox

		'ConfirmationBox' creates a window containing 'message' and
		two command buttons labeled 'buttonname1' and 'buttonname2',
		using 'CreateDialog'. The window is popped down when the user
		pushes one of the buttons. If a translation is installed for
		the QuitDialog action function, then this could also pop down
		the window (e.g. bound to a ClientMessage event from the
		window manager).

		&&Return value
		    | -1: Dialog window deleted by the QuitDialog
		    |     action function. This is often bound to
		    |     a desire by the window manager to close
		    |     the window.
		    |  0: button labeled buttonname1 was pushed.
		    |  1: button labeled buttonname2 was pushed.

                &SimpleDialog

                Creates a dialog widget with a 'title', a text field 'deftext'
		and two buttons (using 'CreateDialog'). The user can edit the
		'deftext' field. The button labels are 'buttonname1' and
		'buttonname2'. The widget is popped up centered at the current
		mouse pointer position.

		&&Callbacks

		    An event handling loop is installed so that the calling
		    application waits for the dialog to finish. The callback
		    function for the two buttons will break the event handler
		    loop and return the (possibly user-edited) 'deftext'
		    string in 'return_text'.

		&&Actions

		    'SimpleDialog' installs the action function "returnok". It
		    will break the event-handling loop, just as if the user
		    had pushed one of the buttons. A copy of the (possibly
		    user-edited) 'deftext' string is returned in 'return_text'
		    in both cases.

		    "returnok" behaves as if the user had pushed the button
		    labeled 'buttonname2'. This action function is bound to
		    the return key by 'CreateDialog', as explained below.
		    Using the return key is in other words a keyboard shortcut
		    for pushing the button.
		
		&&Return value
		    | -1: Dialog window deleted by the QuitDialog
		    |     action function. This is often bound to
		    |     a desire by the window manager to close
		    |     the window.
		    |  0: button labeled buttonname1 was pushed.
		    |  1: button labeled buttonname2 was pushed (or
		    |     the keyboard shortcut was used)

		&CreateDialog

		Creates a dialog widget with 'title' at the top, a
		'textField', 'count' command-buttons, 'winName' as window
		manager decoration title and 'iconName' as window manager
		decoration title of iconified window. 'CreateDialog' does
		not actually pop up the window.

		The dialog widget is of class Dialog with instance
		name "dialog". It has two children, one of class
		Label (instance name "label") which holds 'title', the other
		of class AsciiText (instance name "value") which holds
		the user-editable 'textField'.
		
		'args' describes the 'count' buttons of class XfwfButton
		inside the window. It is an array of structures with the
		attributes 'buttonName' (type char *), 'handler' (type
		XtCallbackProc) and 'data' (type XtPointer). 'buttonName'
		will become the button label, 'handler' is a function
		which will be added to the XtNactivate list of all
		buttons, 'data' is the client-data which is passed into
		the callback function when it is activated. The resource
		instance names of the buttons will be "button1", "button2"
		etc.

		&&Actions

		    'CreateDialog' installs the action function QuitDialog.
		    The action function will invoke the callback-function of
		    the buttons, with client-data equal to -1 (or actually a
		    pointer to this value). The action function is not bound
		    to any event by 'CreateDialog', but will typically be
		    bound to the event which is sent from the window manager
		    to kill the window (<ClientMessage>WM_PROTOCOLS). This
		    must be done by the calling application (e.g. via an X
		    resource file).

		&&Translations

		    If 'textField' is not NULL, the following translation
		    binding is installed on the 'textField' part of the dialog
		    widget:

		    | <Key>0xff0a: returnok()
		    | <Key>Return: returnok()

		    where the two keysymbols represent newline and carriage
		    return respectively. The expected action function
		    "returnok" is not installed by 'CreateDialog'. It must be
		    done by the caller. If it is installed, it means that the
		    return key can be used as a shortcut to invoke "returnok"
		    (typically the same as pushing an ok-button).

		&&Return value

		    A reference to a transientShellWidgetClass widget with
		    instance name "popup".
		    
See also:       'xshow(1)', 'XfwfButton(3)', 'XfwfFileSelector(3)',
                'XfwfGroup(3)', 'XfwfPrompt(3)', 'XfwfToggle(3)'

Files:          $XSHOWPATH/xshow_menues

Author:		Otto Milvang and Svein Bøe

Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
static void Next_event(parent)
Widget parent;
#else /* FUNCPROTO */
static void Next_event(Widget parent)
#endif /* FUNCPROTO */
{
  XtAppContext appcon;
  XEvent event;
  appcon  = XtWidgetToApplicationContext(parent);
  XtAppNextEvent(appcon, &event);
  XtDispatchEvent(&event);
}

#ifndef FUNCPROTO
void xthCenterWidget(widget, x, y)
Widget widget;
int x, y;
#else /* FUNCPROTO */
void xthCenterWidget(Widget widget, int x, int y)
#endif /* FUNCPROTO */
{
  Dimension width, height;
  int s_w, s_h;

  ENTER_FUNCTION_DEBUG("Xdialog.c: xthCenterWidget");

  XtVaGetValues(widget, XtNwidth,  &width,
			XtNheight, &height, NULL);

  x  -= ((int) width) / 2;
  y  -= ((int) height) / 2;
  s_w = WidthOfScreen(XtScreen(widget));
  s_h = HeightOfScreen(XtScreen(widget));

  /* 30 compensates for the window manager title decoration. */
  if (x + (int) width > s_w)  x = s_w - (int) width;
  if (y + (int) height + 30 > s_h) y = s_h - (int) height - 30;

  if (x < 0) x = 0;
  if (y < 0) y = 0;

  XtVaSetValues(widget, XtNx, x, XtNy, y, NULL);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: xthCenterWidget");

  return;
}

#ifndef FUNCPROTO
void xthCenterWidgetOverCursor(widget)
Widget widget;
#else /* FUNCPROTO */
void xthCenterWidgetOverCursor(Widget widget)
#endif /* FUNCPROTO */
/*
 * center a window over the cursor
 *
 *   returns: void
 *
 */
{
  Window root, child;
  int x, y, dummy;
  unsigned int mask;

  ENTER_FUNCTION_DEBUG("Xdialog.c: xthCenterWidgetOverCursor");

  (void) XQueryPointer(XtDisplay(widget), XtWindow(widget),
		       &root, &child,
		       &x, &y, &dummy, &dummy,
		       &mask);

  xthCenterWidget(widget, x, y);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: xthCenterWidgetOverCursor");

  return;
}

#ifndef FUNCPROTO
static void quit_dialog(widget, event, params, num_params)
Widget widget;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void quit_dialog(Widget widget, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  Widget wid = NULL;
  char name[50];

  ENTER_FUNCTION_DEBUG("Xdialog.c: quit_dialog");

  /* Assume first that 'widget' is some ancestor of a dialogWidget. */

  wid = XtNameToWidget(widget, strcat(strcpy(name, "*"), "button2"));
  if (!wid)
    wid = XtNameToWidget(widget, strcat(strcpy(name, "*"), "button1"));

  if (!wid) {
    /* Assume that 'widget' refers to the "label" or "value" part of a
     * dialogWidget.
     */

    wid = XtNameToWidget(XtParent(widget),
			 strcat(strcpy(name, "*"), "button2"));

    if (!wid)
      wid = XtNameToWidget(XtParent(widget),
			   strcat(strcpy(name, "*"), "button1"));
  }

  if (!wid) Error(2, "quit_dialog: Could not find suitable widget.\n");

  XtCallCallbacks(wid, XtNactivate, (XtPointer) WM_KILL_WINDOW);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: quit_dialog");

} /* quit_dialog() */

#ifndef FUNCPROTO
Widget CreateDialog(parent, title, textField, winName, iconName, args, count)
Widget parent;           /* parent window                         */
char *title;             /* title of the dialog box               */
char *textField;	 /* default text field                    */
char *winName;           /* Name of window, for use by window manager. */
char *iconName;          /* Name of icon, for use by window manager. */
struct DialogArg *args;  /* description of the buttons            */
unsigned int count;      /* number of buttons                     */
#else /* FUNCPROTO */
Widget CreateDialog(Widget parent, char *title, char *textField, char *winName, char *iconName, struct DialogArg *args, unsigned int count)
                         /* parent window                         */
                         /* title of the dialog box               */
                	 /* default text field                    */
                         /* Name of window, for use by window manager. */
                         /* Name of icon, for use by window manager. */
                         /* description of the buttons            */
                         /* number of buttons                     */
#endif /* FUNCPROTO */
/*
 * create a simple moded dialog box
 */
{
  Widget popup, dialog, last_button, value = NULL, label = NULL, button = NULL,
    prev_button = NULL;
/*  Dimension borderW, hlThick; */
  int i;
  static char *transl = "#override\n\
                          <Key>0xff0a: returnok()\n\
                          <Key>Return: returnok()";
  Dimension width, height, top_margin, bottom_margin, left_margin,
            right_margin, frame_width, inner_offset, outer_offset;

  ENTER_FUNCTION_DEBUG("Xdialog.c: CreateDialog");
  
  /* override does not get titlebar, transient does */
  popup = (Widget) CreatePopupShell("popup", parent);

  AddAction(popup, "QuitDialog", quit_dialog, 0);

  /* create the dialog box */
  dialog = XtVaCreateManagedWidget("dialog", dialogWidgetClass, popup,
				   XtNvalue, textField,
				   XtNlabel, title,
				   XtNinput, True, NULL);

  label = XtNameToWidget(dialog, "label");

  if (textField) {
    Dimension w1, w2, h;
    int lines;
    char *t;

    value = XtNameToWidget(dialog, "value");

    t = textField;
    lines = 1; 
    while(*t) if (*t++ == '\n') lines++;
    if (lines > 1) {
      XtVaGetValues(value, XtNheight, &h, NULL);
      XtVaSetValues(value, XtNheight, (h-4) *lines + 4, NULL);
    }

    XtVaGetValues(value, XtNwidth, &w1, NULL);
    XtVaGetValues(label, XtNwidth, &w2, NULL);
    if (w2>w1) XtVaSetValues(value, XtNwidth, w2, NULL);

    XawTextSetInsertionPoint(value, strlen(textField));
    XtOverrideTranslations(value, XtParseTranslationTable(transl));
  }

  /* add the buttons */
  for (i = 0; i < count; i++) {
    char res_name[50];
    static XtCallbackRec callbacks[] = {
      {NULL, NULL},
      {NULL, NULL},
    };
    
    callbacks[0].callback = args[i].handler;
    callbacks[0].closure  = args[i].data;
    
    (void) strcpy(res_name, "button");
    sprintf(&(res_name[strlen(res_name)]), "%1d", i+1);
    button = XtVaCreateWidget(res_name, xfwfButtonWidgetClass, dialog,
				     XtNlabel,     args[i].buttonName,
				     XtNactivate,  callbacks,
				     XtNfromVert,  value ? value : label,
				     XtNfromHoriz, prev_button,
				     NULL);

    XtVaGetValues(button,
		  XtNtopMargin,    &top_margin,
		  XtNbottomMargin, &bottom_margin,
		  XtNleftMargin,   &left_margin,
		  XtNrightMargin,  &right_margin,
		  XtNframeWidth,   &frame_width,
		  XtNinnerOffset,  &inner_offset,
		  XtNouterOffset,  &outer_offset,
		  XtNwidth,        &width,
		  XtNheight,       &height, NULL);

    if (width <= left_margin + right_margin + 2 * frame_width +
		 inner_offset + outer_offset ||
	height <= top_margin + bottom_margin + 2 * frame_width +
	inner_offset + outer_offset) {
      XtDestroyWidget(button);

      button = XtVaCreateManagedWidget(res_name, xfwfButtonWidgetClass, dialog,
				       XtNlabel,       args[i].buttonName,
				       XtNactivate,    callbacks,
				       XtNfromVert,    value ? value : label,
				       XtNfromHoriz,   prev_button,
				       XtNshrinkToFit, "True",
				       NULL);
    } else XtManageChild(button);

    prev_button = button;
    if (i == count - 1) {
      last_button = button;
    }
  }

/*
  if (count > 1) {
    XtVaGetValues(last_button, XtNborderWidth,        &borderW,
		  XtNhighlightThickness, &hlThick, NULL);  
    XtVaSetValues(last_button, XtNborderWidth,        hlThick + 1,
		  XtNhighlightThickness, hlThick + 1, NULL); 
  }
*/
  
  XtRealizeWidget(popup);

  XmbSetWMProperties(dpy_e, XtWindow(popup),
		     winName, iconName,
		     NULL, 0, NULL, NULL, NULL);
  (void) XSetWMProtocols(dpy_e, XtWindow(popup), &wm_delete_window, 1);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: CreateDialog");

  return(popup);

} /* CreateDialog() */

#ifndef FUNCPROTO
void FocusPopUp(popup, data, event, dummy)
Widget popup;
XtPointer data;
XEvent *event;
Boolean *dummy;
#else /* FUNCPROTO */
void FocusPopUp(Widget popup, XtPointer data, XEvent *event, Boolean *dummy)
#endif /* FUNCPROTO */
{
    if (event->type == MapNotify) {
	XSetInputFocus(XtDisplay(popup), XtWindow(popup),
		     RevertToPointerRoot,  CurrentTime);
	XtRemoveEventHandler(popup, XtAllEvents, True, FocusPopUp, 0);
    }
}


#ifndef FUNCPROTO
void PopUpDialog(popup)
Widget popup;
#else /* FUNCPROTO */
void PopUpDialog(Widget popup)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xdialog.c: PopUpDialog");

  xthCenterWidgetOverCursor(popup);
  XtPopup(popup, XtGrabExclusive);
  /*  XtAddEventHandler(popup, StructureNotifyMask, False, FocusPopUp, 0); */

  LEAVE_FUNCTION_DEBUG("Xdialog.c: PopUpDialog");
  return;
}

#ifndef FUNCPROTO
void PopUpDialogNoGrab(popup)
Widget popup;
#else /* FUNCPROTO */
void PopUpDialogNoGrab(Widget popup)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xdialog.c: PopUpDialogNoGrab");

  xthCenterWidgetOverCursor(popup);
  XtPopup(popup, XtGrabNone);
  /*  XtAddEventHandler(popup, StructureNotifyMask, False, FocusPopUp, 0); */

  LEAVE_FUNCTION_DEBUG("Xdialog.c: PopUpDialogNoGrab");
  return;
}

#ifndef FUNCPROTO
void PopDownDialog(dialog)
Widget dialog;
#else /* FUNCPROTO */
void PopDownDialog(Widget dialog)
              
#endif /* FUNCPROTO */
/*
 * pop down the dialog.
 */
{
  ENTER_FUNCTION_DEBUG("Xdialog.c: PopDownDialog");

  XtPopdown(dialog);
  XtDestroyWidget(dialog);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: PopDownDialog");
  return;
}

#ifndef FUNCPROTO
void cbPopDownDialog(wid, client_data, call_data)
Widget wid;
XtPointer client_data, call_data;
#else /* FUNCPROTO */
void cbPopDownDialog(Widget wid, XtPointer client_data, XtPointer call_data)
              
#endif /* FUNCPROTO */
{
  Widget shell;

  ENTER_FUNCTION_DEBUG("Xdialog.c: cbPopDownDialog");

  shell = TopLevelWidget(wid);
  XtPopdown(shell);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: cbPopDownDialog");
  return;
}

#ifndef FUNCPROTO
char *GetDialogValue(popup)
Widget popup;
#else /* FUNCPROTO */
char *GetDialogValue(Widget popup)
#endif /* FUNCPROTO */
{
    return XawDialogGetValueString(XtNameToWidget(popup, "dialog"));
}


/*
 * simple confirmation box
 */

#ifndef FUNCPROTO
static void quit_file_selector(widget, event, params, num_params)
Widget widget;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void quit_file_selector(Widget widget, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  WidgetList children;
  Widget child;
  Cardinal num_children;
  int child_num;

  ENTER_FUNCTION_DEBUG("Xdialog.c: quit_file_selector");
  
  XtVaGetValues(widget, XtNchildren,    &children,
		        XtNnumChildren, &num_children, NULL);

  for (child_num = 0; child_num < num_children; child_num++) {
    child = children[child_num];
    if (XtClass(child) == xfwfFileSelectorWidgetClass) {
      XtCallCallbacks(child, XtNcancelButtonCallback, NULL);
      break;
    }
  }

  LEAVE_FUNCTION_DEBUG("Xdialog.c: quit_file_selector");

} /* quit_file_selector() */

#ifndef FUNCPROTO
static void button2_action(widget, event, params, num_params)
Widget widget;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void button2_action(Widget widget, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xdialog.c: button2_action");

  retVal_s = BUTTON2_PUSHED;

  LEAVE_FUNCTION_DEBUG("Xdialog.c: button2_action");
  return;
}

#ifndef FUNCPROTO
static void dialog_callback(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void dialog_callback(Widget widget, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
    ENTER_FUNCTION_DEBUG("Xdialog.c: dialog_callback");
    FPRINTF2("    return-value=%d.\n", (int) client_data);

    retVal_s = (int) *(((int *) client_data));

    LEAVE_FUNCTION_DEBUG("Xdialog.c: dialog_callback");
    return;
}

#ifndef FUNCPROTO
int InformationBox(parent, message, winName, iconName)
Widget parent;
char *message, *winName, *iconName;
#else /* FUNCPROTO */
int InformationBox(Widget parent, char *message, char *winName, char *iconName)
#endif /* FUNCPROTO */
{
  Widget widget;
  struct DialogArg args[1];

  ENTER_FUNCTION_DEBUG("Xdialog.c: InformationBox");

  args[0].buttonName = "Close";
  args[0].handler    = dialog_callback;
  args[0].data       = (XtPointer) MallocIntP(BUTTON1_PUSHED);

  widget = CreateDialog(parent, message, NULL, winName, iconName, args, 1);
  PopUpDialog(widget);

  retVal_s = NOT_DONE;
  for(;;) {
    Next_event(parent);
    if (retVal_s != NOT_DONE) {
      PopDownDialog(widget);

      LEAVE_FUNCTION_DEBUG("Xdialog.c: InformationBox");
      return(retVal_s);
    }
  }

} /* InformationBox() */

#ifndef FUNCPROTO
Widget TextBox(parent, message, btn_text, winName, iconName)
Widget parent;
char *message, *winName, *iconName, *btn_text;
#else /* FUNCPROTO */
Widget TextBox(Widget parent, char *message, char *btn_text, char *winName, char *iconName)
#endif /* FUNCPROTO */
{
  Widget widget;
  struct DialogArg args[1];

  ENTER_FUNCTION_DEBUG("Xdialog.c: TextBox");

  args[0].buttonName = btn_text;
  args[0].handler    = cbPopDownDialog;
  args[0].data       = NULL;

  widget = CreateDialog(parent, message, NULL, winName, iconName, args, 1);

  xthCenterWidgetOverCursor(widget);

  return(widget);

} /* TextBox() */

#ifndef FUNCPROTO
int ConfirmationBox(parent, message, buttonname1, buttonname2, winName, iconName)
Widget parent;
char *message, *buttonname1, *buttonname2, *winName, *iconName;
#else /* FUNCPROTO */
int ConfirmationBox(Widget parent, char *message, char *buttonname1, char *buttonname2, char *winName, char *iconName)
#endif /* FUNCPROTO */
/*
 * pop up a confirmation box and return either 'BUTTON1_PUSHED' or
 * 'BUTTON2_PUSHED'. */
{
  Widget widget;
  struct DialogArg args[2];

  ENTER_FUNCTION_DEBUG("Xdialog.c: ConfirmationBox");
  FPRINTF3("    message=%s, buttonname1=%s", message, buttonname1);
  FPRINTF2("    buttonname2=%s\n", buttonname2);

  args[0].buttonName = buttonname1;
  args[0].handler    = dialog_callback;
  args[0].data       = (XtPointer) MallocIntP(BUTTON1_PUSHED);
  args[1].buttonName = buttonname2;
  args[1].handler    = dialog_callback;
  args[1].data       = (XtPointer) MallocIntP(BUTTON2_PUSHED);

  widget = CreateDialog(parent, message, NULL, winName, iconName,
			args, buttonname2 ? 2 : 1);
  PopUpDialog(widget);

  retVal_s = NOT_DONE;
  for(;;) {
    Next_event(parent);
    if (retVal_s != NOT_DONE) {
      PopDownDialog(widget);

      LEAVE_FUNCTION_DEBUG("Xdialog.c: ConfirmationBox");
      return(retVal_s);
    }
  }

} /* ConfirmationBox() */



#ifndef FUNCPROTO
int SimpleDialog(parent, title, deftext, return_text, buttonname1, buttonname2, winName, iconName)
Widget parent;
char *title, *deftext, *buttonname1,  *buttonname2, **return_text;
char *winName, *iconName;
#else /* FUNCPROTO */
int SimpleDialog(Widget parent, char *title, char *deftext, char **return_text, char *buttonname1, char *buttonname2, char *winName, char *iconName)
#endif /* FUNCPROTO */
{
  Widget dialog_shell, dialog;
  static char retval[2048];
  struct DialogArg args[2];

  ENTER_FUNCTION_DEBUG("Xdialog.c: SimpleDialog");

  args[0].buttonName = buttonname1;
  args[0].handler    = dialog_callback;
  args[0].data       = (XtPointer) MallocIntP(BUTTON1_PUSHED);
  args[1].buttonName = buttonname2;
  args[1].handler    = dialog_callback;
  args[1].data       = (XtPointer) MallocIntP(BUTTON2_PUSHED);

  AddAction(parent, "returnok", button2_action, 0);

  dialog_shell = CreateDialog(parent, title, deftext,
			      winName, iconName, args, 2);
  PopUpDialog(dialog_shell);

  dialog = XtNameToWidget(dialog_shell, "dialog");
  retVal_s = NOT_DONE;
  for(;;) {
    Next_event(parent);
    if (retVal_s != NOT_DONE) {
      PopDownDialog(dialog_shell);
      if (deftext) strcpy(retval, XawDialogGetValueString(dialog));
      *return_text = (deftext == NULL) ? NULL : XtNewString(retval);
      
      LEAVE_FUNCTION_DEBUG("Xdialog.c: SimpleDialog");
      return(retVal_s);
    }
  } /* for(;;) */
}

#ifndef FUNCPROTO
static void OkCallbackHandler(w, client_data, ret)
Widget w;
XtPointer client_data;
XfwfFileSelectorOkButtonReturnStruct *ret;
#else /* FUNCPROTO */
static void OkCallbackHandler(Widget w, XtPointer client_data, XfwfFileSelectorOkButtonReturnStruct *ret)
#endif /* FUNCPROTO */
{
  char *filename;

  ENTER_FUNCTION_DEBUG("Xdialog.c: OkCallbackHandler");
  FPRINTF4("    Path = <%s>, File = <%s>, File Box = <%s>\n",
	 ret->path, ret->file, ret->file_box_text);
  filename = XtMalloc(strlen(ret->file_box_text) + strlen(ret->path) + 1);
  (void) strcpy(filename, ret->path);
  (void) strcat(filename, ret->file_box_text);

  FPRINTF2("    Filename: %s\n", filename);

  ((FileSelInfo *) client_data)->filename = filename;
  retVal_s = BUTTON2_PUSHED;

  PopDownDialog(TopLevelWidget(w));

  LEAVE_FUNCTION_DEBUG("Xdialog.c: OkCallbackHandler");

} /* OkCallbackHandler() */

#ifndef FUNCPROTO
static void CancelCallbackHandler(w,client_data,call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void CancelCallbackHandler(Widget w, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xprog.c: CancelCallbackHandler");

  ((FileSelInfo *) client_data)->filename = NULL;
  retVal_s = BUTTON1_PUSHED;

  PopDownDialog(TopLevelWidget(w));

  LEAVE_FUNCTION_DEBUG("Xprog.c: CancelCallbackHandler");

} /* CancelCallbackHandler() */

#ifndef FUNCPROTO
static void SelectionChangeCallbackHandler(w,client_data,ret)
Widget w;
XtPointer client_data;
XfwfFileSelectorSelectionChangeReturnStruct *ret;
#else /* FUNCPROTO */
static void SelectionChangeCallbackHandler(Widget w, XtPointer client_data, XfwfFileSelectorSelectionChangeReturnStruct *ret)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xprog.c: SelectionChangeCallbackHandler");
  if (ret->file_selected == False)
    printf("No Items Selected!\n");
  else
    printf("Path = <%s>, File = <%s>\n",ret->path,ret->file);
  LEAVE_FUNCTION_DEBUG("Xprog.c: SelectionChangeCallbackHandler");

} /* SelectionChangeCallbackHandler() */

#ifndef FUNCPROTO
int SelectReadFile(parent, kind, return_text)
Widget parent;
char **return_text;
char *kind;
#else /* FUNCPROTO */
int SelectReadFile(Widget parent, char *kind, char **return_text)
#endif /* FUNCPROTO */
{
  Widget Popup, Selector;
  FileSelInfo *texts;

  ENTER_FUNCTION_DEBUG("Xdialog.c: SelectReadFile");

  Popup = CreatePopupShell("fileSelectorShell", parent);

  AddAction(Popup, "QuitFileSel", quit_file_selector, 0);

  kind++; /* Skip the '?'. */
  Selector =
    XtCreateManagedWidget(kind,
			  xfwfFileSelectorWidgetClass,
			  Popup, NULL, 0);
  
  texts = XtNew(FileSelInfo);
  XtAddCallback(Selector, XtNokButtonCallback,
		(XtCallbackProc)OkCallbackHandler, (XtPointer) texts);
  XtAddCallback(Selector, XtNcancelButtonCallback,
		(XtCallbackProc)CancelCallbackHandler, (XtPointer) texts);

  XtRealizeWidget(Popup);
  XmbSetWMProperties(dpy_e, XtWindow(Popup), "FileSelector", "FileSelector",
                      NULL, 0, NULL, NULL, NULL);
  (void) XSetWMProtocols(dpy_e, XtWindow(Popup), &wm_delete_window, 1);

  PopUpDialog(Popup);

  retVal_s = NOT_DONE;
  for(;;) {
    Next_event(parent);
    if (retVal_s != NOT_DONE) {
      *return_text = XtNewString(texts->filename);
      
      if (texts->filename) XtFree(texts->filename);
      XtFree((char *) texts);
      LEAVE_FUNCTION_DEBUG("Xdialog.c: SelectReadFile");
      return(retVal_s);
    }
  } /* for(;;) */

} /* SelectReadFile() */

#ifndef FUNCPROTO
int SelectWriteFile(parent, kind, return_text)
Widget parent;
char **return_text, *kind;
#else /* FUNCPROTO */
int SelectWriteFile(Widget parent, char *kind, char **return_text)
#endif /* FUNCPROTO */
{
  Widget Popup, Selector;
  FileSelInfo *texts;

  ENTER_FUNCTION_DEBUG("Xdialog.c: SelectWriteFile");

  Popup = CreatePopupShell("fileSelectorShell", parent);

  AddAction(Popup, "QuitFileSel", quit_file_selector, 0);
  kind++; /* Skip the '?'. */
  Selector =
    XtCreateManagedWidget(kind,
			  xfwfFileSelectorWidgetClass,
			  Popup, NULL, 0);
  
  texts = XtNew(FileSelInfo);
  XtAddCallback(Selector, XtNokButtonCallback,
		(XtCallbackProc)OkCallbackHandler, (XtPointer) texts);
  XtAddCallback(Selector, XtNcancelButtonCallback,
		(XtCallbackProc)CancelCallbackHandler, (XtPointer) texts);

  XtRealizeWidget(Popup);
  XmbSetWMProperties(dpy_e, XtWindow(Popup), "FileSelector", "FileSelector",
                      NULL, 0, NULL, NULL, NULL);
  (void) XSetWMProtocols(dpy_e, XtWindow(Popup), &wm_delete_window, 1);

  PopUpDialog(Popup);

  retVal_s = NOT_DONE;
  for(;;) {
    Next_event(parent);
    if (retVal_s != NOT_DONE) {
      *return_text = XtNewString(texts->filename);
      
      if (texts->filename) XtFree(texts->filename);
      XtFree((char *) texts);
      LEAVE_FUNCTION_DEBUG("Xdialog.c: SelectWriteFile");
      return(retVal_s);
    }
  } /* for(;;) */

} /* SelectWriteFile() */
