

/*

________________________________________________________________

        Xpty.c
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
#include <ctype.h>
#include <stdlib.h>
#include <xite/message.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <xite/ShellWids.h>
#include <xite/Xpty.h>
#include <xite/Xdialog.h>
#include <xite/Xcolor.h>
#include <xite/Image.h>
#include <xite/ImageOverlay.h>
#include <signal.h>
#include <errno.h>
#include XITE_FILE_H
#include XITE_FORK_H
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_UNISTD_H
#include <xite/debug.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/CascadeBG.h>
#include <Xm/FileSB.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>
#else
#include <X11/Xaw/AsciiText.h>
#endif /* MOTIF */

#ifdef FUNCPROTO
static int initpty(void);
#else
static int initpty();
#endif /* FUNCPROTO */

extern Atom wm_delete_window;  /* for ICCCM delete window */

static program progs_s[MAXJOBS];
static int    max_prog_s = 0;
static Display *dpy_s;
static int     screen_s;
static Widget  Control_s;
static XtAppContext appcon_s;

static Widget subjobmenu_s;

#ifdef FUNCPROTO
static void SubjobSelect(Widget w, XtPointer closure, XtPointer call_data);
#ifdef MOTIF
static void AsciiSend(Widget ascii, program *prog,
		      XmTextVerifyCallbackStruct *cbs);
#else /* MOTIF */
static void AsciiSend(Widget wid, XEvent *event, String *params, Cardinal *num_params);
#endif /* MOTIF */
#else /* FUNCPROTO */
static void SubjobSelect(/* Widget w, XtPointer closure, XtPointer call_data */);
static void AsciiSend(/* Widget wid, XEvent *event, String *params, Cardinal *num_params */);
#endif /* FUNCPROTO */



#define BUFSIZE 1025


/*F:XptyMessage=XptyInit*/
/*F:XptyClose=XptyInit*/
/*F:XptyStartProgram=XptyInit*/
/*F:XptyJobs=XptyInit*/
/*F:XptyKill=XptyInit*/
/*F:XptyList=XptyInit*/
/*F:XptyInit*

________________________________________________________________

		Xpty
________________________________________________________________

Name:		XptyInit, XptyMessage, XptyStartProgram, XptyClose, XptyKill,
                XptyJobs, XptyList - Start and stop child process to execute
		jobs

Syntax:         | #include <xite/Xpty.h>
                | 
		| int XptyInit( Display *display,
		|    int screenno, Widget parent );
		|
		| int XptyStartProgram( char *entry, char *vec[],
		|    int shell, ptyfunc1 openfunc,
		|    ptyfunc1 closefunc, ptyfunc2 stdoutfunc,
		|    ptyfunc2 stderrfunc, ptyfunc1 imgfunc );
		|
                | void XptyMessage( program* prog, char* txt );
                |
		| int XptyJobs( void );
		|
		| void XptyClose( program *prog, int pipe, int typ );
		| 
		| void XptyKill( program *prog );
		|
		| void XptyList( Widget wid, XtPointer c_data,
		|    XtPointer call_data );
Description:
                'XptyInit' prepares for getting ptys. A popup menu is
		initialized with two entries, one labeled "Save Buffer", the
		other labeled "Kill Window". The callback function for these
		menu entries will execute the indicated action, using
		'XawAsciiSaveAsFile' or 'XtDestroyWidget' respectively. This
		menu should be used for AsciiText windows.

		Also, 'XptyInit' installs an action function AsciiSend for the
		application referenced by 'parent'.
		
		'XptyStartProgram' initializes a 'prog' structure. If
		"<infile>" occurs in 'vec', a pipe is opened for communicating
		an image from the process calling 'XptyStartProgram' to the
		child process given by 'vec'[0]. If "<outfile>" occurs in
		'vec', a pipe is opened for communicating an image from the
		child process indicated by 'vec'[0] to the process calling
		'XtpyStartProgram'. The strings "<infile>" and "<outfile>" in
		'vec' are replaced by the negative of the file-descriptors
		opened. The BIFF read- and write-routines (and thus most XITE
		programs) will use these file-descriptors for reading and
		writing images. An additional pipe is opened for text error
		messages.

		'XptyStartProgram' allocates a pty (pseudo terminal) (either
		by calling system function '_getpty' (if source is compiled
		with GETPTY defined)) or by trying to open devices /dev/ptyxy
		(master) and /dev/ttyxy (slave) (where "x" is one of the
		characters "pqrstuvwxyz", "y" is one of the characters
		"0123456789abcdef"). If successful, forks out a child process
		to execute the command in 'vec'. The child process will use
		the slave half of the pty for its stdout, and the text error
		message pipe for its stderr. Output images will be sent into
		image pipes. The parent process registers input event sources
		with the X Toolkit Intrinsics, to wait for input from the
		master half of the pty (child''s stdout), from the text error
		message pipe (child''s stderr) and from image pipes.

		'XptyMessage' uses the widget referred to by prog->wid, or
		if this is NULL, opens an AsciiText widget (shell created with
		'CreatePopTopShell') with size 80x24 characters in the
		widget''s font. A text indicating the job number and job name
		is written to the widget, followed by 'txt'.

		The following translation binding is installed by 'XptyMessage'
		in the AsciiText widget (if prog->wid was NULL upon entry):

		| Shift <Btn2Down>: PopupMenu(subjobmenu)
		| <Key>: AsciiSend(<a>, <b>)

		where <a> is the job-number, <b> is the channel output-number.
		'subjobmenu' is the name of the menu initialized by 'XptyInit'.

		The 'AsciiSend' action function (installed by 'XptyInit')
		interprets the key events "q" and "Q" as commands to kill the
		widget.

		'XptyJobs' returns the number of jobs started by
		'XptyStartProgram', and not yet stopped by 'XptyClose' or
		'XptyKill'.

		'XptyClose' closes 'pipe' and calls 'prog->closefunc'. 'typ' is
		not used. If no more pipes are open for 'prog', then write a
		message (with 'XptyMessage') telling that job is terminated.

		'XptyKill' kills (using the system function 'kill') the
		process indicated by 'prog', and decrements the count of active
		processes. With 'prog' equal to NULL, kill all processes
		started by 'XptyStartProgram' and set the active process-count
		equal to zero.

		'XptyList' prints a list of the active processes (started by
		'XptyStartProgram'), using the 'Message(3)' function.

See also:       'Message(3)'

Author:		Otto Milvang
Doc:            Svein Bøe
________________________________________________________________

*/


#ifndef MOTIF
# ifndef FUNCPROTO
static int subjobs2()
# else /* FUNCPROTO */
static int subjobs2(void)
# endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xpty.c: subjobs2");

  subjobmenu_s = PopupInit("subjobmenu", Control_s, "Subjob menu"); 

  PopupAdd("savebuffer", subjobmenu_s, "  Save Buffer ",
	   SubjobSelect, 0);
  PopupAdd("killwindow", subjobmenu_s, "  Kill Window ",
	   SubjobSelect, 1);

  LEAVE_FUNCTION_DEBUG("Xpty.c: subjobs2");
  return(1);
}
#endif /* not MOTIF */


#ifndef FUNCPROTO
int XptyInit(display, screenno, parent)
Display *display;
int screenno;
Widget parent;
#else
int XptyInit(Display *display, int screenno, Widget parent)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xpty.c: XptyInit");

  dpy_s = display;
  screen_s = screenno;
  Control_s = parent;
  appcon_s = XtWidgetToApplicationContext(parent);
  if (initpty() == -1) {
    LEAVE_FUNCTION_DEBUG("Xpty.c: XptyInit");
    return(-1);
  }
#ifndef MOTIF
  AddAction(Control_s, "AsciiSend", AsciiSend, 0);
  XtAppAddWorkProc(appcon_s, (XtWorkProc) subjobs2, NULL);
#endif /* not MOTIF */

  LEAVE_FUNCTION_DEBUG("Xpty.c: XptyInit");
  return(0);
}


#ifdef MOTIF

# ifndef FUNCPROTO
static void menu_save(w, prog, call_data)
Widget w;
program *prog;
char *call_data;
# else
static void menu_save(Widget w, program *prog, char *call_data)
# endif
{
}

# ifndef FUNCPROTO
static void menu_close(w, prog, call_data)
Widget w;
program *prog;
char *call_data;
# else
static void menu_close(Widget w, program *prog, char *call_data)
# endif /* FUNCPROTO */
{
  Widget wid;

  ENTER_FUNCTION_DEBUG("Xpty.c: menu_close");

  wid = prog->wid;
  while(!XtIsSubclass(wid, shellWidgetClass))
    wid = XtParent(wid);
  XtDestroyWidget(wid);

  LEAVE_FUNCTION_DEBUG("Xpty.c: menu_close");
}


#else /* MOTIF */

# ifndef FUNCPROTO
static void SubjobSelect(w, clos, call_data)
Widget w;
XtPointer clos;
XtPointer call_data;
# else
static void SubjobSelect(Widget w, XtPointer clos,  XtPointer call_data)
# endif /* FUNCPROTO */
{
  Widget GV;
  int closure = *((int *) (clos));
  char *filename, *name;

/*  free(clos); */
  ENTER_FUNCTION_DEBUG("Xpty.c: SubjobSelect");

  GV = (Widget) PopupWidget();
  switch(closure)
    {
    case 0:

      if (SelectWriteFile(Control_s, writefile, &filename) == 0)
	return;

      XtVaGetValues(XtParent(GV), XtNname, &name, NULL);

      if (XawAsciiSaveAsFile(XawTextGetSource(GV), filename))
	Info(0,"%s saved as %s.\n", name, filename);
      else
	Warning(1,"Cannot save %s as %s.\n", name, filename);
      break;
    case 1:
      XtDestroyWidget((Widget) TopLevelWidget(GV));
      break;
    }

  LEAVE_FUNCTION_DEBUG("Xpty.c: SubjobSelect");
}
#endif /* MOTIF */



#ifndef FUNCPROTO
static int def_errfunc(prog, buf)
program *prog;
char *buf;
#else
static int def_errfunc(program *prog, char *buf)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xpty.c: def_errfunc");
  Message(0, "Job %5d message  - %s",prog->jobno, buf);
  LEAVE_FUNCTION_DEBUG("Xpty.c: def_errfunc");
  return(0);
}


#ifndef FUNCPROTO
void XptyMessage(prog, txt)
program *prog;
char *txt;
#else
void XptyMessage(program *prog, char *txt)
#endif /* FUNCPROTO */
{
  char header[200], *p;
  char *ptr1, *ptr2, c, st2[2];
  char *pt2 = st2;
  Widget ascii, top;
  XFontStruct *fontinfo;
  XtTranslations edit_trans;
  int ch, cw;
#ifdef MOTIF
  XmString st;
#endif /* MOTIF */

  ENTER_FUNCTION_DEBUG("Xpty.c: XptyMessage");

  st2[1] = 0;
  if (prog->wid == NULL) {
    if (prog->terminated) {
      LEAVE_FUNCTION_DEBUG("Xpty.c: XptyMessage");
      return;
    }
    
    /* Create new AsciiText widget. */
    
    sprintf(header, "%s subjob %d", prog->jobname, prog->jobno);
#ifdef MOTIF
    n = 0;
    top = XtAppCreateShell("Xipi", NULL, applicationShellWidgetClass,
			   dpy_s, args, n);
    main = 
      (Widget) XmCreateMainWindow(top, "main", args, n);
    XtManageChild(main);
    
    n = 0;
    bar = (Widget) XmCreateMenuBar(main, "xptymenubar", args, n);
    XtManageChild(bar);
    
    
    n = 0;
    st = XmStringCreate(header, XmSTRING_DEFAULT_CHARSET );
    XtSetArg(args[n], XmNlabelString, st); n++;
    cmd = (Widget) XmCreateLabel(main, "jobname", args, n);
    XtManageChild(cmd);
    XmStringFree(st);
    
    n = 0;
    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
    XtSetArg(args[n], XmNeditable, TRUE); n++;
    XtSetArg(args[n], XmNrows, 24); n++; 
    XtSetArg(args[n], XmNcolumns, 80); n++;
    /*      XtSetArg(args[n], XmNvisualPolicy, XmCONSTANT); n++;
	    XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;*/
    XtSetArg(args[n], XmNautoShowCursorPosition, TRUE); n++;
    XtSetArg(args[n], XmNpendingDelete, FALSE); n++;
    XtSetArg(args[n], XmNverifyBell, FALSE); n++;
    
    ascii = (Widget) XmCreateScrolledText(main, "pty", args, n);
    XtAddCallback(ascii, XmNmodifyVerifyCallback, AsciiSend, prog);
    XtAddCallback(ascii, XmNmotionVerifyCallback, AsciiSend, prog);
    XtManageChild(ascii);
    
    prog->wid = ascii;
    
    file = (Widget) XmCreatePulldownMenu(bar, "File", NULL, 0);
    n=0;
    XtSetArg(args[n], XmNsubMenuId, file); n++; 
    XtSetArg(args[n], XmNmnemonic, 'F'); n++;
    casc = (Widget) XmCreateCascadeButtonGadget(bar, "File", 
						args, n);
    XtManageChild(casc);
#define MENU_ENTRY(parent, name, mnemonic, callback) \
  n=0;\
  XtSetArg(args[n], XmNmnemonic, mnemonic); n++; \
  casc = (Widget) XmCreatePushButtonGadget(parent, name, args, n); \
  XtAddCallback(casc, XmNactivateCallback, callback, prog); \
  XtManageChild(casc);

    MENU_ENTRY(file, "Save", 'S', menu_save);
    MENU_ENTRY(file, "Close", 'C', menu_close);
    
    XmMainWindowSetAreas (main, bar, cmd, NULL, NULL, XtParent(ascii));
    XtRealizeWidget(top);
    sprintf(header, "***** Job %d: %s *****\n",prog->jobno, prog->jobname);
    XmTextInsert(prog->wid, 0, header);
    
#else /* MOTIF */
    top = CreatePopTopShell("termShell", Control_s);
    
    ascii =
      XtVaCreateManagedWidget("subxterm", asciiTextWidgetClass, top,
			      XtNtype, XawAsciiString,
			      XtNeditType, XawtextEdit,
			      XtNscrollVertical, XawtextScrollWhenNeeded,
			      XtNscrollHorizontal, XawtextScrollWhenNeeded,
			      XtNdisplayNonprinting, "False",
			      NULL);
    sprintf(header, "<Key>: AsciiSend(%d, %d)\n\
              Shift <Btn2Down>: PopupMenu(subjobmenu)",
	    prog->jobno,  prog->ch_out);
    edit_trans = XtParseTranslationTable(header);
    XtOverrideTranslations(ascii, edit_trans); 
    prog->wid = ascii;
    XtVaGetValues(ascii, XtNfont, &fontinfo, NULL);
    cw = XTextWidth(fontinfo, "m", strlen("m"));
    /* cw = fontinfo->max_bounds.width; */
    ch = fontinfo->ascent + fontinfo->descent;
    XtVaSetValues(ascii, XtNwidth, 80*cw, XtNheight, 24*ch+4, NULL);
    XtRealizeWidget(top);
    XtPopup(top, XtGrabNone);
    sprintf(header, "#***** Job %d: %s *****\n",prog->jobno, prog->jobname);
    p = header;
    XtCallActionProc(prog->wid, "insert-string", NULL, &p, 1);
    XawTextDisplayCaret(prog->wid, TRUE);
    
    XmbSetWMProperties(dpy_s, XtWindow(top),
		       "xshow - terminal", "xshow - term",
		       NULL, 0, NULL, NULL, NULL);
    
    (void) XSetWMProtocols(dpy_s, XtWindow(top), &wm_delete_window, 1);
    
#endif /* MOTIF */

  } /* if (prog->wid == NULL) */   

  p = (char *) malloc(strlen(txt)+1);
  if (p == NULL) {
    LEAVE_FUNCTION_DEBUG("Xpty.c: XptyMessage");
    return;
  }
  strcpy(p, txt);
  ptr1 = p;
  while(*ptr1) {
    ptr2 = ptr1;
    while(isprint(*ptr2)) ptr2++;
    c = *ptr2;
    *ptr2 = 0;
#ifdef MOTIF
    XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), ptr1);
#else /* MOTIF */
    XtCallActionProc(prog->wid, "insert-string", NULL, &ptr1, 1);
#endif /* MOTIF */
    *ptr2 = c;
    st2[0] = c;
    ptr1 = ptr2;
    switch(c)
      {
      case 0:
	break;
      case 3:
#ifdef MOTIF
	XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), ptr2);
	st2[0] = '\n';
	XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), st2);
#else
	XtCallActionProc(prog->wid, "insert-string", NULL, &pt2, 1);
	XtCallActionProc(prog->wid, "newline", NULL, NULL,0);
#endif
	kill(prog->jobno, 1);
	sleep(1);
	kill(prog->jobno, 9);
	break;
#ifdef MOTIF
      case 10:
	XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), st2);
	break;
#endif
      case 13:
	break;
      case 21:
#ifdef MOTIF
	XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), ptr2);
	st2[0] = '\n';
	XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), st2);
#else
	XtCallActionProc(prog->wid, "insert-string", NULL, &pt2, 1);
	XtCallActionProc(prog->wid, "newline", NULL, NULL,0);
#endif /* MOTIF */
	break;
      case 8:
      case 127:
#ifdef MOTIF
	XmTextReplace(prog->wid, XmTextGetInsertionPosition(prog->wid)-1,
		      XmTextGetInsertionPosition(prog->wid), "");
#else
	XtCallActionProc(prog->wid, "delete-previous-character",
			 NULL, NULL,0);
#endif /* MOTIF */
	break;	      
      default:
#ifdef MOTIF
	XmTextInsert(prog->wid, XmTextGetInsertionPosition(prog->wid), ptr2);
#else
	XtCallActionProc(prog->wid, "insert-string", NULL, &pt2, 1);
#endif /* MOTIF */
	break;
      }
    if (c) ptr1++;
  }	
  free(p);
  if (prog->terminated) {
#ifdef MOTIF
    XtVaSetValues(prog->wid, XmNeditable, 0, 
		  XmNcursorPositionVisible, 0, 
		  NULL);
    
#else
    XtVaSetValues(prog->wid, XtNeditType, XawtextRead, NULL);
    XawTextDisplayCaret(prog->wid, FALSE);
#endif /* MOTIF */
  }
#ifdef MOTIF
  XmTextSetInsertionPosition(prog->wid, XmTextGetInsertionPosition(prog->wid));
#endif

  LEAVE_FUNCTION_DEBUG("Xpty.c: XptyMessage");
} 



#ifndef FUNCPROTO
void XptyClose(prog, pipe, typ)
program *prog;
int pipe, typ;
#else
void XptyClose(program *prog, int pipe, int typ)
#endif /* FUNCPROTO */
{
  int jno;

  ENTER_FUNCTION_DEBUG("Xpty.c: XptyClose");

  if (prog->ch_in     == pipe) prog->ch_in     = -1; 
  if (prog->ch_out    == pipe) prog->ch_out    = -1; 
  if (prog->ch_err    == pipe) prog->ch_err    = -1; 
  if (prog->ch_imgin  == pipe) prog->ch_imgin  = -1; 
  if (prog->ch_imgout == pipe) prog->ch_imgout = -1; 

/*  if (prog->ch_err == -1)
    {
      if (prog->ch_in != -1) 
	{
	  close(prog->ch_in);
	  prog->ch_in = -1;
	}
      if (prog->ch_imgin != -1) 
	{
	  close(prog->ch_imgin);
	  prog->ch_imgin = -1;
	}
    } */
  if (prog->ch_out == -1 && prog->ch_err == -1 && prog->ch_imgout == -1)
    prog->terminated = 1;
  FPRINTF3("    pipe: %d prog->terminated: %d\n", pipe, prog->terminated); 
  close(pipe);
  prog->closefunc(prog);
  jno = prog->jobno;
  if (prog->terminated) {
    if (prog->ch_in != -1) {
      close(prog->ch_in);
      prog->ch_in = -1;
    }
    if (prog->ch_imgin != -1) {
      close(prog->ch_imgin);
      prog->ch_imgin = -1;
    }
    XptyMessage(prog, "#***** Job terminated *****\n");
    max_prog_s--;
    *prog = progs_s[max_prog_s];
  }
  
  LEAVE_FUNCTION_DEBUG("Xpty.c: XptyClose");
} 


#ifndef FUNCPROTO
static void CloseStdout(prog)
program *prog;
#else
static void CloseStdout(program *prog)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xpty.c: CloseStdout");
  if (prog->terminated) return;

  if (prog->ch_out != -1) XptyClose(prog, prog->ch_out, 1); 
  if (prog->id1) XtRemoveInput(prog->id1);
  prog->id1 = (XtInputId) NULL;

  LEAVE_FUNCTION_DEBUG("Xpty.c: CloseStdout");
}

#ifndef FUNCPROTO
static void Read_stderr(pr, fid, id)
XtPointer pr;
int *fid;
XtInputId *id;
#else
static void Read_stderr(XtPointer pr, int *fid, XtInputId *id)
#endif /* FUNCPROTO */
{
  char buf[BUFSIZE];
  int nbytes;
  program *prog;

  ENTER_FUNCTION_DEBUG("Xpty.c: Read_stderr");

  prog = (program *) pr;
  if ((nbytes = read(*fid, buf, BUFSIZE-1)) == -1) {
    Error(2, "Read_stderr: Error reading pipe %d, %s\n", *fid,
	  strerror(errno));
    XptyClose(prog, prog->ch_err, 1); 
    if (prog->id2) XtRemoveInput(*id);
    prog->id2 = (XtInputId) NULL;
  } else {
    if (nbytes) {
      buf[nbytes] = 0;
      (prog->stderrfunc)(prog, buf);
    }  else {
      XptyClose(prog, prog->ch_err, 1);
      if (prog->id2) XtRemoveInput(*id);
      prog->id2 = (XtInputId) NULL;
      XtAppAddWorkProc(appcon_s, (XtWorkProc) CloseStdout,
		       (XtPointer) prog);
    } 
  }

  LEAVE_FUNCTION_DEBUG("Xpty.c: Read_stderr");
}


#ifndef FUNCPROTO
static int def_outfunc(prog, buf)
program *prog;
char *buf;
#else
static int def_outfunc(program *prog, char *buf)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Xpty.c: def_outfunc");

  XptyMessage(prog, buf);

  LEAVE_FUNCTION_DEBUG("Xpty.c: def_outfunc");

  return(0);
}

#ifndef FUNCPROTO
static void Read_stdout(pr, fid, id)
XtPointer pr;
int *fid;
XtInputId *id;
#else
static void Read_stdout(XtPointer pr, int *fid, XtInputId *id)
#endif /* FUNCPROTO */
{
  char buf[BUFSIZE];
  int nbytes;
  program *prog;

  ENTER_FUNCTION_DEBUG("Xpty.c: Read_stdout");

  prog = (program *) pr;
  if ((nbytes = read(*fid, buf, BUFSIZE-1)) == -1) {
    /* Failure. */

    XptyClose(prog, prog->ch_out, 1); 
    if (prog->id1) XtRemoveInput(*id);
    prog->id1 = (XtInputId) NULL;
  } else {
    /* Success. */

    if (nbytes) {
      buf[nbytes] = 0;
      (prog->stdoutfunc)(prog, buf);
    } else {
      /* End of file reached, read no bytes. */

      XptyClose(prog, prog->ch_out, 1);
      if (prog->id1) XtRemoveInput(*id);
      prog->id1 = (XtInputId) NULL;
    } 
  }
  LEAVE_FUNCTION_DEBUG("Xpty.c: Read_stdout");
}


#ifndef FUNCPROTO
static int def_imgfunc(prog)
program *prog;
#else
static int def_imgfunc(program *prog)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Xpty.c: def_imgfunc");
  LEAVE_FUNCTION_DEBUG("Xpty.c: def_imgfunc");

  return(0);
}


#ifndef FUNCPROTO
static void Read_image(pr, fid, id)
XtPointer pr;
int *fid;
XtInputId *id;
#else
static void Read_image(XtPointer pr, int *fid, XtInputId *id)
#endif /* FUNCPROTO */
{
  program *prog;

  ENTER_FUNCTION_DEBUG("Xpty.c: Read_image");

  prog = (program *) pr;

  if ((prog->imgfunc)(prog) == 0) {
    /* No more input from pipe. */

    WidgetList list;
    int num_widgets;

    XptyClose(prog, prog->ch_imgout, 1);
    if (prog->id3) XtRemoveInput(*id);
    prog->id3 = (XtInputId) NULL;

    list = imageWidgetList(&num_widgets);

    if (prog->num_colortabs_read && prog->num_images_read) {
      int i;

      /* A colortable was produced by the program along with some
       * image widgets. Set this colortable for all the image widgets
       * which were produced by the program.
       */

      for (i = 1; i <= prog->num_images_read; i++) {
	Color_set((ImageWidget) list[num_widgets - i], Color_max());
      }
    } /* if */
    else if (prog->num_images_read) {
      int i, col_num;
      ImageWidget iw;

      /* Set the work-map. */
      for (i = 1; i <= prog->num_images_read; i++) {
	iw = (ImageWidget) list[num_widgets - i];

	if (!ImageHasColormap(iw)) col_num = 0;
	else col_num = 1;

	Color_set(iw, col_num);
      }
    }

    if (prog->num_ovltabs_read && prog->num_images_read) {
      int i;

      /* An overlay colortable was produced by the program along with
       * some image widgets. Set this overlay-table for all the image widgets
       * which were produced by the program.
       */

      for (i = 1; i <= prog->num_images_read; i++) {
	Color_ovl_set((ImageWidget) list[num_widgets - i], Color_ovl_max());
      }
    } else if (prog->num_images_read) {
      int i, col_num;
      ImageWidget iw;

      /* Set the standard overlay-map. */

      for (i = 1; i <= prog->num_images_read; i++) {
	iw = (ImageWidget) list[num_widgets - i];

	if (!ImageHasColormap(iw)) col_num = 0;
	else col_num = 1;

	Color_ovl_set(iw, col_num);
      }
    }
  } /* if */

  LEAVE_FUNCTION_DEBUG("Xpty.c: Read_image");

} /* Read_image() */

#ifndef GETPTY
# define PTYCHAR1        "pqrstuvwxyz"
# define PTYCHAR2        "0123456789abcdef"
# define TTYDEV          "/dev/ttyxx"
# define PTYDEV          "/dev/ptyxx"

  static char *ttydev_s, *ptydev_s;
  static char *ptyl1_s, *ptyl2_s;
  static int tlen_s, plen_s;
#endif /* GETPTY */

#ifndef FUNCPROTO
static int initpty()
#else
static int initpty(void)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("Xpty.c: initpty");

#ifndef GETPTY
  ttydev_s = (char *) malloc (strlen (TTYDEV) + 1);
  ptydev_s = (char *) malloc (strlen (PTYDEV) + 1);
  if (!ttydev_s || !ptydev_s) {
    Warning(1, "Unable to allocate memory for ttydev_s or ptydev_s, %s\n",
	    strerror(errno));
    LEAVE_FUNCTION_DEBUG("Xpty.c: initpty");
    return(-1);
  }
  strcpy (ttydev_s, TTYDEV);
  strcpy (ptydev_s, PTYDEV);
  tlen_s = strlen(ttydev_s);
  plen_s = strlen(ptydev_s);
  ptyl1_s = PTYCHAR1;
  ptyl2_s = PTYCHAR2;
#endif /* GETPTY */

  LEAVE_FUNCTION_DEBUG("Xpty.c: initpty");
  return(0);
}

#ifndef FUNCPROTO
static int getpty(master, slave)
int *master, *slave;
#else
static int getpty(int *master, int *slave)
#endif /* FUNCPROTO */
{
#ifdef GETPTY
/* SGI has a library function called _getpty() */
  char *_getpty(int *, int, mode_t, int);
  int filedes[2];
  char *line;

  ENTER_FUNCTION_DEBUG("Xpty.c: getpty");

  line = _getpty(filedes, O_RDWR, 0600, 0);
  if (line == NULL) {
    Error(2, "getpty: Couldn't find pty, %s.\n", strerror(errno));
    return(-1);
  }
  if ((filedes[1] = open(line, O_RDWR)) < 0 ) {
    Error(2, "getpty: Couldn't open slave read/write, %s.\n", strerror(errno));
    close(filedes[0]);
    return(-1);
  }
  *master = filedes[0];
  *slave  = filedes[1];

  LEAVE_FUNCTION_DEBUG("Xpty.c: getpty");

  return(0);

#else /* GETPTY */
  int first;

  ENTER_FUNCTION_DEBUG("Xpty.c: getpty");

  first = 0;
  for(; *ptyl1_s; ptyl1_s[1] ? ptyl1_s++ : (ptyl1_s = PTYCHAR1)) {
    ttydev_s[tlen_s-2] = ptydev_s[plen_s-2] = *ptyl1_s;

    for(ptyl2_s = *ptyl2_s ? ptyl2_s : PTYCHAR2; *ptyl2_s; ptyl2_s++) {
      ttydev_s[tlen_s-1] = ptydev_s[plen_s-1] = *ptyl2_s;
      *master = open(ptydev_s, O_RDWR);

      if (*master >= 0) {

	if (access(ttydev_s, R_OK|W_OK) == 0) {
	  FPRINTF3("    ttydev_s: %s ptydev_s: %s\n",
		   ttydev_s, ptydev_s);
	  *slave = open(ttydev_s, O_RDWR);
	  if (*slave < 0) {
	    Error(2, "getpty: Couldn't open slave %s read/write, %s.\n",
		  ttydev_s, strerror(errno));
	  }
	  LEAVE_FUNCTION_DEBUG("Xpty.c: getpty");

	  return(0);
	} else {
	  FPRINTF3("getpty: Couldn't access slave %s read/write, %s.\n",
		ttydev_s, strerror(errno));
	  close(*master);
	}
      } else {
	FPRINTF3("getpty: Couldn't open master %s read/write, %s.\n",
	      ptydev_s, strerror(errno));
      }
    }
    first ++;
    if (first > strlen(PTYCHAR1)) break;
  }
  Error(2, "getpty: No free ptys.\n");

  return(-1);

#endif /* GETPTY */
}

#ifndef FUNCPROTO
static int def_openfunc(prog)
program *prog;
#else
static int def_openfunc(program *prog)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Xpty.c: def_openfunc");
  LEAVE_FUNCTION_DEBUG("Xpty.c: def_openfunc");

  return(0);
}

#ifndef FUNCPROTO
static int def_closefunc(prog)
program *prog;
#else
static int def_closefunc(program *prog)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Xpty.c: def_closefunc");
  LEAVE_FUNCTION_DEBUG("Xpty.c: def_closefunc");

  return(0);
}

static int Xpty_status, Xpty_execstat;


#ifndef FUNCPROTO
int XptyStartProgram(entry, vec, shell, openfunc, closefunc, 
		     stdoutfunc, stderrfunc, imgfunc)
char *entry, *vec[];
int shell;
ptyfunc1 openfunc, closefunc, imgfunc;
ptyfunc2 stdoutfunc, stderrfunc;
#else
int XptyStartProgram(char *entry, char *vec[], int shell, ptyfunc1 openfunc,
ptyfunc1 closefunc, ptyfunc2 stdoutfunc, ptyfunc2 stderrfunc, ptyfunc1 imgfunc)
#endif /* FUNCPROTO */
{
  int slave, master;
  int pipe_ii[2], pipe_io[2], pipe_te[2];
  int i, inimages, outimages;
  program *prog;

  ENTER_FUNCTION_DEBUG("Xpty.c: XptyStartProgram");

  if (max_prog_s == MAXJOBS) {
    Error(2,"XptyStartProgram: Job limit = %d\n", MAXJOBS);
    return(-1);
  }

  prog = &progs_s[max_prog_s];
  prog->jobno 	      = 0;
  prog->ch_in 	      = -1;
  prog->ch_out 	      = -1;
  prog->ch_err 	      = -1;
  prog->ch_imgin      = -1;
  prog->ch_imgout     = -1;
  prog->terminated    = 0;
  prog->openfunc      = openfunc   ? openfunc   : def_openfunc;
  prog->closefunc     = closefunc  ? closefunc  : def_closefunc;
  prog->stdoutfunc    = stdoutfunc ? stdoutfunc : def_outfunc;
  prog->stderrfunc    = stderrfunc ? stderrfunc : def_errfunc;
  prog->imgfunc       = imgfunc    ? imgfunc    : def_imgfunc;
  prog->wid           = 0;
  prog->id1           = (XtInputId) NULL;  
  prog->id2           = (XtInputId) NULL;  
  prog->id3           = (XtInputId) NULL;
  prog->num_images_read    = 0;
  prog->num_overlays_read  = 0;
  prog->num_colortabs_read = 0;
  prog->num_ovltabs_read   = 0;

  if (getpty(&master, &slave)) return(-1);

  inimages  = 0;
  outimages = 0;
  for (i = 0; vec[i]; i++) {
    if ( ! strcmp(vec[i], XSHOWINFILE) ) {
      if (!inimages) {
	/* Prepare for communication between parent and child processes,
	 * by setting up pipe for image input to child. */

	pipe(pipe_ii);
	prog->ch_imgin = pipe_ii[1];      /* The writing end of the pipe. */
      }
      inimages++;
      sprintf(vec[i], "-%d", pipe_ii[0]); /* The reading end of the pipe. */
    }
    if ( ! strcmp(vec[i], XSHOWOUTFILE)) {
      if (!outimages) {
	/* Prepare for communication between parent and child processes,
	 * by setting up pipe for image output from child. */

	pipe(pipe_io); /* Image output */
	prog->ch_imgout = pipe_io[0];     /* The reading end of the pipe. */
      }
      outimages++;
      sprintf(vec[i], "-%d", pipe_io[1]); /* The writing end of the pipe. */
    }
    if ( ! strcmp(vec[i], XTERMOUT)) {
      /* Replace the XTERMOUT code in vec by "-1", the negative of the
       * file-descriptor for stdout. This is the way to specify textual
       * output from the program to stdout. This means that the program
       * given by vec[0] must be able to interpret "-1" as stdout.
       */
      shell++;
      vec[i] = "-1";
    }
  }

  /* Prepare for communication between parent and child processes,
   * by setting up pipe for textual error output from child. */
  pipe(pipe_te);
  prog->ch_err = pipe_te[0]; /* The reading end of the pipe. */
  
  FPRINTF3("    Pipe_ii: %2d %2d", pipe_ii[0], pipe_ii[1]);
  FPRINTF3(", io: %2d %2d\n", pipe_io[0], pipe_io[1]);
  FPRINTF2("    master: %2d", master);
  FPRINTF3(", te: %2d %2d\n", pipe_te[0], pipe_te[1]);
  
#ifdef DEBUG
  i=1;
  for(i=0; vec[i]; i++) FPRINTF2("    |%s|",vec[i]);
  FPRINTF1("\n");
#endif

  Xpty_execstat = Xpty_status = 1;

  if ((i=vfork()) == 0) {
    /* Child context. The child process will use the slave half of the pty for
     * its stdout, and the text error message pipe for its stderr. Output
     * images will be sent into image pipes. */

    close(master);
    FPRINTF3("    master %d, slave %d\n", master, slave);
    if (inimages)  close(pipe_ii[1]); /* The writing end of the pipe. */
    if (outimages) close(pipe_io[0]); /* The reading end of the pipe. */
    close(pipe_te[0]);                /* The reading end of the pipe. */
    close(0);                         /* stdin */
    close(1);                         /* stdout */
    close(2);                         /* stderr */
    dup2(slave, 0);
    dup2(slave, 1);
    dup2(pipe_te[1], 2);
    close(slave);
    close(pipe_te[1]);
    
    Xpty_execstat = execvp(vec[0],vec);

    /* We get here only if execvp failed. In this case Xpty_execstat = -1. */

    prog->jobno = 0;
    Xpty_status = 0;
    _exit(0);
  }
  
  FPRINTF3("    Status=%d, Exec status=%d\n", Xpty_status, Xpty_execstat); 
  if (Xpty_status && Xpty_execstat != -1) {
    /* Parent context. The parent process registers input event sources with
     * the X Toolkit Intrinsics, to wait for input from the master half of the
     * pty (child''s stdout), from the text error message pipe (child''s
     * stderr) and from image pipes. */

    close(slave);
    if (inimages) close(pipe_ii[0]);  /* The reading end of the pipe. */
    if (outimages) close(pipe_io[1]); /* The writing end of the pipe. */
    close(pipe_te[1]);                /* The writing end of the pipe. */
    prog->jobno     = i;
    prog->jobname   = entry;
    prog->ch_in     = master;
    prog->ch_out    = master;
    prog->inimages  = inimages;
    prog->outimages = outimages;
    prog->id1       = XtAppAddInput(appcon_s, prog->ch_out, 
				    (XtPointer)XtInputReadMask, 
				    Read_stdout,
				    (XtPointer) prog);
    prog->id2       = XtAppAddInput(appcon_s, prog->ch_err, 
				    (XtPointer) XtInputReadMask, 
				    Read_stderr,
				    (XtPointer) prog);
    if (outimages) 
      prog->id3     = XtAppAddInput(appcon_s, prog->ch_imgout, 
				    (XtPointer) XtInputReadMask, 
				    Read_image,
				    (XtPointer) prog);
    if (shell) (prog->stdoutfunc)(prog, "");
    
    FPRINTF3("    Inimages = %d, Outimages = %d\n", inimages, outimages);
    max_prog_s++;
    prog->openfunc(prog);
    LEAVE_FUNCTION_DEBUG("Xpty.c: XptyStartProgram");
    return(0);
  } else {
    /* We should never get here. */

    if (inimages)  close(pipe_ii[0]);
    if (inimages)  close(pipe_ii[1]); 
    if (outimages) close(pipe_io[0]);
    if (outimages) close(pipe_io[1]);
    close(master);
    close(slave);
    close(pipe_te[0]);
    close(pipe_te[1]); 
    progs_s->terminated = 1;
    LEAVE_FUNCTION_DEBUG("Xpty.c: XptyStartProgram");
    return(-1);
  }
}



/*L*

________________________________________________________________

		XptyKill
________________________________________________________________

Name:		XptyKill
Syntax:		| XptyKill(prog)
Description:	Kill a subjobs, or all subjobs
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
int XptyJobs()
#else
int XptyJobs(void)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Xpty.c: XptyJobs");
  LEAVE_FUNCTION_DEBUG("Xpty.c: XptyJobs");

  return(max_prog_s);
}


#ifndef FUNCPROTO
void XptyKill(prog)
program *prog;
#else
void XptyKill(program *prog)
#endif /* FUNCPROTO */
{
  int i, j, k;

  ENTER_FUNCTION_DEBUG("Xpty.c: XptyKill");

  k = 0;
  j = max_prog_s;
  if (prog) {
    while(&progs_s[k] != prog) k++;
    j = k+1;
  }      

  for(i=k; i<j; i++)
    if (progs_s[i].terminated == 0) kill(progs_s[i].jobno ,1);

  sleep(2);

  for(i=k; i<j; i++)
    if (progs_s[i].terminated == 0) {
      kill(progs_s[i].jobno, 9);
      Message(0, "Job %5d killed - %s\n", progs_s[i].jobno, 
	      progs_s[i].jobname);
      if (progs_s[i].id1) XtRemoveInput(progs_s[i].id1);
      if (progs_s[i].id2) XtRemoveInput(progs_s[i].id2);
      if (progs_s[i].id3) XtRemoveInput(progs_s[i].id3);
      if (progs_s[i].ch_in     != -1) close(progs_s[i].ch_in);
      if (progs_s[i].ch_out    != -1) close(progs_s[i].ch_out);
      if (progs_s[i].ch_err    != -1) close(progs_s[i].ch_err);
      if (progs_s[i].ch_imgin  != -1) close(progs_s[i].ch_imgin);
      if (progs_s[i].ch_imgout != -1) close(progs_s[i].ch_imgout);
    }
  if (prog) {
    max_prog_s--;
    progs_s[k] = progs_s[max_prog_s];
  } else
    max_prog_s = 0;

  LEAVE_FUNCTION_DEBUG("Xpty.c: XptyKill");
}




/*L*

________________________________________________________________

		XptyProg
________________________________________________________________

Name:		XptyProg
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
void XptyList(wid, c_data, call_data)
Widget wid;
XtPointer c_data, call_data;
#else
void XptyList(Widget wid, XtPointer c_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  int i;

  ENTER_FUNCTION_DEBUG("Xpty.c: XptyList");

  Message(0, "\nJobs:\n");
  for(i=0; i<max_prog_s; i++)
    if (progs_s[i]. terminated == 0) 
      Message(0,"Job %5d running  - %s\n", progs_s[i].jobno, 
		progs_s[i].jobname);
  Message(0,"\n");

  LEAVE_FUNCTION_DEBUG("Xpty.c: XptyList");
}






#ifdef MOTIF

#ifndef FUNCPROTO
static void AsciiSend(ascii, prog, cbs)
Widget ascii;
program *prog;
XmTextVerifyCallbackStruct *cbs;
#else
static void AsciiSend(Widget ascii, program *prog, 
XmTextVerifyCallbackStruct *cbs)
#endif /* FUNCPROTO */
{
  char st[2];

  ENTER_FUNCTION_DEBUG("Xpty.c: AsciiSend");

  switch (cbs->reason)
    { 
    case XmCR_MOVING_INSERT_CURSOR:
      cbs->doit = FALSE;
      break;
    case XmCR_MODIFYING_TEXT_VALUE:
      if (cbs->event == NULL) return;
      if (cbs->text->ptr == NULL)
	{
	  st[0] = 127; st[1] = 0;
	  write(prog->ch_out, st, 1);
	} else {
	  write(prog->ch_out, cbs->text->ptr, cbs->text->length);
	}
      cbs->doit = FALSE;
      break;
    default:
      break;
    }

  LEAVE_FUNCTION_DEBUG("Xpty.c: AsciiSend");
}

#else /* MOTIF */

#ifndef FUNCPROTO
static void AsciiSend(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else
static void AsciiSend(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  char buf[200];
  XawTextEditType type;
  int len;

  ENTER_FUNCTION_DEBUG("Xpty.c: AsciiSend");

  len = XLookupString((XKeyEvent *) event, buf, 196, NULL, NULL);
  XtVaGetValues(wid, XtNeditType, &type, NULL);
  if (len) {
    if (type == XawtextEdit) write(atoi(params[1]), buf, len);
    else if (buf[0] == 'q' || buf[0] == 'Q') 
      XtDestroyWidget(TopLevelWidget(wid));
  }

  LEAVE_FUNCTION_DEBUG("Xpty.c: AsciiSend");
}
 
#endif /* MOTIF */

