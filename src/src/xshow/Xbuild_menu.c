

/*

________________________________________________________________

        Xbuild_menu.c
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
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <stdlib.h>
#include <xite/biff.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/message.h>
#include <xite/Xdialog.h>
#include <xite/FormDialog.h>
#include "Xbuild_menu.h"
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#ifdef HAVE_SYS_FILE_H
# include <sys/file.h>
#else
# ifdef HAVE_SYS_IO_H
#  include <sys/io.h>
# endif
#endif
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Shell.h>
#include "SubMenuCommand.h"
#include "MenuCommand.h"
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <X11/Xutil.h>
#include "MenuBox.h"
#include <xite/ShellWids.h>
#include "Xprog.h"
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/utils.h>
#include <xite/strings.h>
#include <xite/debug.h>



static void select_menu(Widget w, XtPointer client_data, XtPointer call_data);
static void select_entry(Widget w, XtPointer client_data, XtPointer call_data);

typedef struct _dialog_entry {
   char *name;
   char *help_text;
   char *dialog_text;
   struct _dialog_entry *next;
} dialog_entry;

typedef struct _command_entry {
   char *command;
   struct _command_entry *next;
} command_entry;

#define MAX_LEVEL 20

extern Atom wm_delete_window; /* For ICCCM deletion of windows. */

Boolean dump_commands_e;

static int level_s = 0;
static char *stack_s[MAX_LEVEL];
static Widget wstack_s[MAX_LEVEL];
static char oldBuf_s[2048];
static char submenu_indicator_s[4] = "...";
static Widget lastComWid_s = NULL;
static command_entry *command_list_s, *command_ptr_s;
static char *key_modifiers[] = {
  "Ctrl",
  "Shift",
  "Alt",
  "Meta",
  "Lock",
  "Hyper",
  "Super",
  "Mod1",
  "Mod2",
  "Mod3",
  "Mod4",
  "Mod5",
  "Button1",
  "Button2",
  "Button3",
  "Button4",
  "Button5",
};
static int num_modifiers_s = 17;

typedef struct _keptMenu {
  char *name;
  Widget wid;
  struct _keptMenu *next;
} keptMenu, *keptMenuP;

static keptMenu kept_head_s = {
  "!",
  NULL,
  NULL
};
static keptMenu kept_tail_s = {
  "~",
  NULL,
  NULL
};

static void InitKeptMenu(keptMenuP *kept)
{
  *kept = XtNew(keptMenu);
  (*kept)->name = NULL;
  (*kept)->wid  = NULL;
  (*kept)->next = NULL;

} /* InitKeptMenu() */

static keptMenuP NewKeptMenu(Widget wid)
{
  char *t;
  keptMenuP p;

  InitKeptMenu(&p);
  if (XtIsSubclass(wid, overrideShellWidgetClass))
    t = XtName(wid);
  else
    XtVaGetValues(wid, XtNlabel, &t, NULL);

  p->name = t;
  p->wid  = wid;

  return(p);

} /* NewKeptMenu() */

static int Find_kept_menu(keptMenuP kept, keptMenuP *after)
{
  keptMenuP p, pp;

  /* "kept" belongs right after "after" or they have same name. */

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Find_kept_menu");

  pp = p = &kept_head_s;
  while (pp != NULL && strcmp(kept->name, pp->name) > 0) {
    p  = pp;
    pp = pp->next;
  }

  if (after) *after = p;
  if (strcmp(kept->name, pp->name) == 0) {
    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Find_kept_menu");
    return(1);
  } else {
    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Find_kept_menu");
    return(0);
  }

} /* Find_kept_menu() */

static int Find_kept_menu_wid(Widget wid)
{

  return(Find_kept_menu(NewKeptMenu(wid), NULL));

} /* Find_kept_menu_wid() */

#define Find_kept_menu_wid(a) 0

static void Insert_kept_menu(keptMenuP after, keptMenuP new)
{
  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Insert_kept_menu");

  if (strcmp(after->name, new->name) == 0) {
    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Insert_kept_menu");
    return;
  } else {
    new->next   = after->next;
    after->next = new;
  }

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Insert_kept_menu");

} /* Insert_kept_menu() */

static void Add_kept_menu(keptMenuP new)
{
  keptMenuP p;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Add_kept_menu");

  (void) Find_kept_menu(new, &p);
  Insert_kept_menu(p, new);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Add_kept_menu");

} /* Add_kept_menu() */

static dialog_entry *dialog_list_s = NULL;

static void DisplayMenu(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void UndisplayMenu(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void KeepMenu(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void CopyMenu(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void quit_xshow(Widget wid, XEvent *event, String *params, Cardinal *num_params);
static void quit_menu(Widget wid, XEvent *event, String *params, Cardinal *num_params);

static XtActionsRec actions[] =
{
  /* {name, procedure}, */
    {"DisplayMenu",    (XtActionProc) DisplayMenu},
    {"UndisplayMenu",  (XtActionProc) UndisplayMenu},
    {"CopyMenu",       (XtActionProc) CopyMenu},
    {"QuitXshow",      (XtActionProc) quit_xshow},
    {"QuitMenu",       (XtActionProc) quit_menu},
};
  
static Widget menutoplevel_s, Control_s;
static Display *dpy_s;



static void quit_menu(Widget wid, XEvent *event, String *params, Cardinal *
num_params)
{
  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: quit_menu");

  if (XtIsSubclass(wid, transientShellWidgetClass))
    XtPopdown(wid);
  else {
    Widget w;

    w = TopLevelWidget(wid);
    if (XtIsSubclass(w, transientShellWidgetClass))
      XtPopdown(w);
  }

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: quit_menu");
  return;
} /* quit_menu() */

/* Same as static function in Xcontrol.c. Redefined here to avoid needing
 * to include Xcontrol functions in libxite.
 */
static void quit_xshow(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: quit_xshow");

  PopMessage();
  Message(2, "xshow aborted ...\n");
  XptyKill(NULL);
  exit(2);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: quit_xshow");

} /* quit_xshow() */

static void Make_submenu(char *menuname, Widget *prev, Widget *next)
{
  Widget wid, mainForm;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Make_submenu");
  if (menuname) {
    FPRINTF2("    |%s|\n", menuname);
  }
  if (*prev)  {
    XtRealizeWidget(*prev); /* Null the first time. */
    XmbSetWMProperties(dpy_s, XtWindow(*prev),
		       "xshow menu", "xshow menu",
		       NULL, 0, NULL, NULL, NULL);

    (void) XSetWMProtocols(dpy_s, XtWindow(*prev), &wm_delete_window, 1);
  }
  if (strcasecmp(menuname, "Xshow") == 0) {
    /* Top-level menu entry. */
    if (menutoplevel_s) {
      int i;

      XtDestroyWidget(menutoplevel_s);
      menutoplevel_s = NULL;

      /* Initialize the global data structure. */
      for (i = 0; i <= level_s; i++) {
	stack_s[i]  = NULL;
	wstack_s[i] = NULL;
      }
    }

    menutoplevel_s = *prev = CreatePopTopShell("mainMenu", Control_s);
    XtAppAddActions(XtWidgetToApplicationContext(menutoplevel_s),
		    actions, 5);
    level_s = 0;
    stack_s[level_s]  = XtNewString(menuname);
    wstack_s[level_s] = menutoplevel_s;
  }
  else {
    if (menutoplevel_s == NULL) {
      fprintf(stderr, "%s\n",
	    "The first menufile must have a top-level menu with name Xshow.");
      exit(1);
    }

    /* OverrideShells cannot receive keyboard input and therefore keyboard
     * translations will not work. TransientShell would work, but we don't
     * want the submenus decorated. Telling the window-manager to turn off
     * decoration for TransientShell in general would mean that also permanent
     * submenus (via the CopyMenu() action) would be without decoration.
     * This is not what we want.
     */
    *prev = CreatePopOverShell(menuname, menutoplevel_s);
  }
			     
  *next = XtCreateManagedWidget("menuBox", menuboxWidgetClass, 
				*prev, NULL,0);

  wid = XtVaCreateManagedWidget("menuTopLabel", labelWidgetClass, *next,
				XtNlabel, menuname, NULL);

  mainForm = XtNameToWidget(Control_s, "mainForm");
  XtInstallAccelerators(mainForm, wid);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Make_submenu");
}

static void dispAcc(Widget wid, String str)
{
  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: dispAcc");

  if (XtClass(wid) == menuCommandWidgetClass ||
      XtClass(wid) == subMenuCommandWidgetClass) {
    char *key;

    key = strstr(str, "<Key");

    if (key) {
      /* Found keyboard accelerator. Make string containing accelerator
       * description. Add this string to the present widget label-string.
       */

      char *modifier, *new_modifier = NULL, *detail, *label, *new_label,
           *tmp, ext[512], *str_cpy;
      int i, len;

      /* Search for modifiers in the acceleration. Find one which has no
       * initial modifier symbols.
       */
      ext[0]   = '\0';
      modifier = str;
      str_cpy = (char *) malloc(sizeof(char) * (strlen(str) + 1));
      (void) strcpy(str_cpy, str);
      
      while (modifier < key) {

        if (modifier == str) modifier = strtok(str, "!~: ");
	else modifier = strtok(NULL, "!~: ");

	if (modifier) {
	  for (i = 0; i < num_modifiers_s; i++) {
	    new_modifier = strstr(modifier, key_modifiers[i]);

	    if (new_modifier) break;
	  } /* for i */
	} else new_modifier = NULL;

	if (!new_modifier) {
	  /* Found no legal modifier in acceleration. */
	  break;
	}

	tmp = str_cpy + (new_modifier - str);
	if (new_modifier > str && (*(tmp-1)  == '!' ||
				   *(tmp -1) == '~')) {
	  /* Initial modifier symbol found.
	   * Search for the next modifier.
	   */
	  modifier = new_modifier+strlen(key_modifiers[i]);
	} else {
	  /* No initial modifier symbol found. Add this modifier. */

	  tmp = new_modifier + strlen(key_modifiers[i]);

	  len = strlen(ext);
	  (void) strncat(ext, new_modifier, tmp - new_modifier);
	  ext[len + tmp - new_modifier] = ' ';
	  ext[len + tmp - new_modifier + 1] = '\0';

	  modifier = new_modifier+strlen(key_modifiers[i]);
	}
      } /* while modifier */

      detail   = strchr(key, '>') + 1;
      (void) strcat(ext, " ");
      (void) strcat(ext, detail);

      XtVaGetValues(wid, XtNlabel, &label, NULL);
      new_label = (char *) malloc(sizeof(char) *
				  (strlen(label) + strlen(ext) + 2));
      strcpy(new_label, label);
      key = new_label + strlen(new_label);
      sprintf(key, "#%s", ext);

      XtVaSetValues(wid, XtNlabel, new_label, NULL);

      free(new_label); new_label = NULL;
      free(str_cpy); str_cpy = NULL;
    }
  }

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: dispAcc");
}



static void Make_menuentry(char *entryname, char *command, Widget parent)
{
  char entry[500];
  int i;
  Widget wid, mainForm;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Make_menuentry");
  strcpy(entry, entryname);
  i = strlen(entry) -1;
  while (isspace(entry[i]) && i> 0) entry[i--] = 0;

  if (entry && strlen(entry) > 0) {
    FPRINTF2("    |%s|\n", entry);
  }

  wid = XtVaCreateManagedWidget(entry, menuCommandWidgetClass, parent,
				XtNlabel, entry, NULL);
  XtAddCallback(wid, XtNcallback, select_entry, command);

  (XtClass(wid)->core_class).display_accelerator = dispAcc;

  mainForm = XtNameToWidget(Control_s, "mainForm");
  XtInstallAccelerators(mainForm, wid);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Make_menuentry");
}

static void Make_menuline(char *entryname, Widget parent)
{
  char entry[500];
  int i;
  Widget wid;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Make_menuline");
  strcpy(entry, entryname);
  i = strlen(entry) -1;
  while (isspace(entry[i]) && i> 0) entry[i--] = 0;

  if (entry && strlen(entry) > 0) {
    FPRINTF2("    |%s|\n", entry);
  }

  wid = XtVaCreateManagedWidget("menuSepLabel", labelWidgetClass, parent,
				XtNlabel, entry, NULL);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Make_menuline");
}

static void Make_menumenu(char *entryname, Widget parent)
{
  char entry[500], label[500];
  int i;
  Widget wid, mainForm;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Make_menumenu");

  strcpy(entry, entryname);
  i = strlen(entry) -1;
  while (isspace(entry[i]) && i> 0) entry[i--] = 0;

  if (entry && strlen(entry) > 0 && entryname && strlen(entryname) > 0) {
    FPRINTF3("    |%s|, |%s|\n", entry, entryname);
  }
  (void) strcpy(label, entry);
  (void) strcat(label, submenu_indicator_s);

  wid = XtVaCreateManagedWidget(entry, subMenuCommandWidgetClass, parent,
				XtNlabel, label, NULL);
  XtAddCallback(wid, XtNcallback, select_menu, entryname);

  (XtClass(wid)->core_class).display_accelerator = dispAcc;

  mainForm = XtNameToWidget(Control_s, "mainForm");
  XtInstallAccelerators(mainForm, wid);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Make_menumenu");
}



typedef enum {
  UNKNOWN   = -1,
  MENUENTRY = 0,
  MENULINE,
  NEWMENU,
  SUBMENU,
  DIALOG_ENTRY,
  COMMENT
} entry_type;

static entry_type EntryType(char *buf)
{

  if (buf[0] == ':') return(SUBMENU);
  if (buf[0] == '+') return(NEWMENU);
  if (buf[0] == '?') return(DIALOG_ENTRY);
  if (buf[0] == '!' || buf[0] == ';' || buf[0] == '#') return(COMMENT);
  if (buf[0] == '-') return(MENULINE);
  if (buf[0] == ' ') return(MENUENTRY);

  return(UNKNOWN);

} /* EntryType() */

static entry_type interpret_line(char *buf, int *entry_start, int *entry_end, int *command_start, int *line_end)
{
  int first1, first2, last1, last2;
  entry_type kind;

  kind = EntryType(buf);
  if (kind == UNKNOWN) return(kind);

  last2 = strlen(buf)-1;

  /* "Remove" trailing white-space. */
  while(last2 >= 0 && isspace(buf[last2])) buf[last2--] = 0;
  
  first1 = 1; /* Skip the initial code character. */
  
  /* "Remove" initial white-space, after the first character. */
  while (first1 <= last2 && isspace(buf[first1])) buf[first1++] = 0;
  
  first2 = first1;
  
  /* Search for possible ';' delimiter, separating menu entry name from
     command. */
  while (first2 <= last2 && buf[first2] != ';') {
    first2++;
    
    /* Escaped ';' ? */
    if (buf[first2] == ';' && buf[first2-1] == '\\') first2++;
  }

  if ((buf[first2] == ';' && kind != MENUENTRY) ||
      (buf[first2] != ';' && kind == MENUENTRY))
    return(UNKNOWN);

  if (kind != MENUENTRY) {
    *entry_end = last2;

  } else {
    /* Buf contains a menu entry, not a menu name, menu line or menu title */
    
    last1 = first2-1;
    
    /* "Remove" trailing white-space from menu entry name */
    while(last1 >= first1 && isspace(buf[last1])) buf[last1--] = 0;
    
    buf[first2] = 0;
    first2++;
    
    /* "Remove" initial white-space from command */
    while (first2 <= last2 && isspace(buf[first2])) buf[first2++] = 0;

    *entry_end     = last1;
  }
  
  *entry_start   = first1;
  *command_start = first2;
  *line_end      = last2;

  return(kind);

} /* interpret_line() */

static int find_level(Widget w)
{
  int i = level_s;
  Widget wid;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: find_level");

  /* Find parent of menuBox. This is either a PopupShell or an
   * applicationShell.
   */
  wid = XtParent(XtParent(w));
  if (wid) FPRINTF2("    wid->core.name: %s\n", wid->core.name);

  while (i >= 0 && wstack_s[i] != wid) i--; 

  FPRINTF3("    level %d (level_s = %d)\n", i, level_s);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: find_level");
  return(i);
}

static void UndisplayMenu(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
  int nlevel;
  Dimension width, height;
  char *lab, *nam;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: UndisplayMenu");

  XtVaGetValues(w, XtNlabel, &lab, NULL);
  nam = XtName(w);
		   
  FPRINTF3("    w->core.name: %s, label: %s\n", nam, lab);

  if (event->type != LeaveNotify) {
    lastComWid_s = NULL;
    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: UndisplayMenu");
    return;
  }
  FPRINTF4("    level_s=%d, event->xcrossing.x=%d event->xcrossing.y=%d\n",
	   level_s, event->xcrossing.x, event->xcrossing.y);
  XtVaGetValues(w, XtNwidth, &width, XtNheight, &height, NULL);
  if (/* Leaving via the left side of the toplevel popup menu window. */
      event->xcrossing.x < 1 ||
      /* Leaving a menu command entry or separation label on the right side. */
      (event->xcrossing.x > width &&
       (XtClass(w) == menuCommandWidgetClass ||
	strcmp(nam, "menuSepLabel") == 0)) ||
      /* Leaving the top label through the top or because the menu was
       * copied (in which case the pointer also leaves the top label to enter
       * the popped-up new menu). */
      (strcmp(nam, "menuTopLabel") == 0 && event->xcrossing.y < height)) {
    nlevel = find_level(w);
    FPRINTF2("    nlevel= %d\n", nlevel);
    while(level_s != nlevel-1 && level_s >= 1) {
      if (wstack_s[level_s] && XtIsRealized(wstack_s[level_s]) &&
	  Find_kept_menu_wid(wstack_s[level_s]) == 0) {
	XtPopdown(wstack_s[level_s]);
	wstack_s[level_s] = NULL;
      }
      level_s--;
    }

    if (level_s < 0) level_s = 0;
    FPRINTF2("    level_s=%d\n", level_s);
  }

  lastComWid_s = NULL;
  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: UndisplayMenu");
}

static void KeepMenu(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: KeepMenu");

  Add_kept_menu(NewKeptMenu(w));

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: KeepMenu");
  
} /* KeepMenu() */

static void DisplayMenu(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  Dimension width;
  char *label, labelCpy[132], *label_extension;
  int nlevel;
  Window root_w, child_w;
  int root_x, root_y, win_x, win_y;
  unsigned int obut;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: DisplayMenu");

  nlevel = find_level(wid);
  FPRINTF3("    level_s=%d, nlevel=%d\n", level_s, nlevel);

  /* Only keep the next popup menu-level on screen, remove any higher
   * levels. level_s is maximum level on screen at the moment. nlevel is
   * the level which the mouse pointer is on now. Also keep menus which
   * the user has explicitly chosen to keep.
   */
  while(level_s > nlevel+1 && level_s >= 1) {
    if (wstack_s[level_s] && XtIsRealized(wstack_s[level_s]) &&
	Find_kept_menu_wid(wstack_s[level_s]) == 0) {
      XtPopdown(wstack_s[level_s]); 
      wstack_s[level_s] = NULL;
    }
    level_s--;
  }
  if (level_s < 0) level_s = 0;

  /* level_s is now the possibly new maximum popup menu-level. */
  FPRINTF2("    level_s=%d\n", level_s);

  XtVaGetValues(wid, XtNlabel, &label, NULL);
  FPRINTF2("    label: %s\n", label);
  FPRINTF1("    Here.\n");
  (void) strcpy(labelCpy, label);
  if ((label_extension = strstr(labelCpy, submenu_indicator_s)) != NULL) {
    label_extension[0] = 0;
  }

  if (level_s == nlevel+1) {
    /* One more menu-level available on screen. */
    if (labelCpy && stack_s[level_s]) {
      FPRINTF3("    |%s| = |%s| ? \n", labelCpy, stack_s[level_s] );
    }
    if(strcmp(labelCpy, stack_s[level_s]) == 0) {
      LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: DisplayMenu");
      return;
    } else {
      if (level_s > 0 && wstack_s[level_s] &&
	  XtIsRealized(wstack_s[level_s]) &&
	  Find_kept_menu_wid(wstack_s[level_s]) == 0) {
	XtPopdown(wstack_s[level_s]);
	wstack_s[level_s] = NULL;
      }
      level_s--;
      if (level_s < 0) level_s = 0;
    }
  }
  if (! XtIsRealized(wid)) {
    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: DisplayMenu");
    return;
  }

  XQueryPointer(dpy_s, XtWindow(wid), 
		&root_w, &child_w, &root_x, &root_y, &win_x, &win_y, &obut);
  XtVaGetValues(wid, XtNwidth,  &width, NULL);
  if ((event->type == KeyPress || event->type == ButtonRelease ||
       ((event->type == EnterNotify || event->type == MotionNotify) && 
	win_x * 4 > width *3)) &&
      (lastComWid_s != wid || lastComWid_s == NULL)) {
    /* Popup sub-menu. */
    XtCallCallbacks(wid, XtNcallback, (XtPointer) event);

  }

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: DisplayMenu");
}

static void CopyMenu(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  Widget menShell, menBox, newMenu, newBox, child, child2;
  Cardinal num_children;
  WidgetList children;
  char *t, *menuLabel, winName[100];
  int childNum, command=0, i;
  XtCallbackList callbacks;
  Window root_w, child_w;
  int root_x, root_y, win_x, win_y, disp_w, disp_h;
  unsigned int obut;
  Dimension wid_w, wid_h, button_h;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: CopyMenu");

  menBox   = XtParent(wid);
  menShell = XtParent(menBox);

  if (XtClass(menShell) == transientShellWidgetClass) {
    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: CopyMenu");
    /* Don't make copy of copy. */
    return;
  }

  /* menShell is overrideShellWidget or topLevelShellWidget.
   * In the former case, wid should be Label class, in the
   * latter case, wid should be SubMenuCommand class.
   */
  if (XtClass(wid) == subMenuCommandWidgetClass) {
    /* Find menu widget to be copied. */
    menShell = XtNameToWidget(menutoplevel_s, XtName(wid));
    if (menShell) {
      menBox = XtNameToWidget(menShell, "menuBox");
    } else {
      Warning(1, "Cannot find menu %s\n", XtName(wid));
      LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: CopyMenu");
      return;
    }
  }

  XtVaGetValues(wid, XtNlabel, &menuLabel, NULL);
  newMenu = CreatePopupShell(menuLabel, menutoplevel_s);
  newBox  = XtCreateManagedWidget("menuBox", menuboxWidgetClass, 
				newMenu, NULL,0);
  XtVaGetValues(menBox, XtNnumChildren,   &num_children,
		        XtNchildren,      &children, NULL);
  XtVaGetValues(children[num_children-1], XtNheight, &button_h, NULL);

  /* Copy each menu entry which is either a command or a label (top or
   * separation). Do not copy submenu-button.
   * Do not copy label if no command follows.
   */
  for (childNum = 0; childNum < num_children; childNum++) {
    command = 0;
    child   = children[childNum];
    if (!XtIsSubclass(child, subMenuCommandWidgetClass)) {
      if (strcmp(XtName(child), "menuSepLabel") == 0) {
	/* Check that a command entry comes before the end or the next
	 * separation label.
	 */
	for (i = childNum + 1; i < num_children; i++) {
	  child2 = children[i];
	  if (strcmp(XtName(child2), "menuSepLabel") == 0) {
	    /* New separation label without any prior command entry. */
	    break;
	  }
	  if (XtClass(child2) == menuCommandWidgetClass) {
	    command = 1;
	    break;
	  }
	} /* for i */

	if (command) {
	  XtVaGetValues(child, XtNlabel, &t, XtNcallback, &callbacks, NULL);
	  XtVaCreateManagedWidget(XtName(child), XtClass(child),
				  newBox, XtNlabel, t,
				  XtNcallback, callbacks, NULL);
	}
      } /* menuSepLabel */
      else {
	XtVaGetValues(child, XtNlabel, &t, XtNcallback, &callbacks, NULL);
	XtVaCreateManagedWidget(XtName(child), XtClass(child), newBox,
				XtNlabel, t,
				XtNcallback, callbacks, NULL);
      } /* not menuSepLabel */
    } /* not subMenuCommandWidget button */
  } /* for childNum */

  XtVaGetValues(newBox, XtNnumChildren, &num_children, NULL);

  if (num_children <= 1) {
    /* Found only top label. */

    Warning(1, "%s%s%s\n", "No entry for a simple command in menu ",
	    menuLabel, ". No menu copied.");
    XtDestroyWidget(newMenu);

    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: CopyMenu");
    return;
  }

  XtRealizeWidget(newMenu);
  XtVaGetValues(newBox, XtNwidth,  &wid_w,
		        XtNheight, &wid_h, NULL);

  if (event->type == ButtonRelease) {
    root_x = event->xcrossing.x_root;
    root_y = event->xcrossing.y_root;
  } else
    XQueryPointer(dpy_s, XtWindow(wid), 
		  &root_w, &child_w, &root_x, &root_y, &win_x, &win_y, &obut);

  disp_w = DisplayWidth(dpy_s, 0);
  disp_h = DisplayHeight(dpy_s, 0);
  root_x -= wid_w / 2;
  root_y -= (2 * button_h) / 3;
  if (root_x + wid_w > disp_w) root_x = disp_w - wid_w;
  /* Compensate for the window manager title decoration. */
  if (root_y + wid_h + button_h > disp_h) root_y = disp_h - wid_h - button_h;
  if (root_x < 1) root_x = 1;
  if (root_y < 1) root_y = 1;
  XtVaSetValues(newMenu, XtNx, root_x, XtNy, root_y, NULL);

  (void) strcpy(winName, XtName(newMenu));
  if ((t = strstr(winName, submenu_indicator_s)) != NULL) {
    t[strlen(submenu_indicator_s)] = '\0';
  }

  XmbSetWMProperties(dpy_s, XtWindow(newMenu),
		     winName, winName,
		     NULL, 0, NULL, NULL, NULL);

  (void) XSetWMProtocols(dpy_s, XtWindow(newMenu), &wm_delete_window, 1);

  XtPopup(newMenu, XtGrabNone);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: CopyMenu");

} /* CopyMenu() */



static void select_menu(Widget sub_btn, XtPointer client_data, XtPointer call_data)
{
  Widget sub_menu;
  XEvent *event = (XEvent *) call_data;
  /* call_data is Non-NULL if called from DisplayMenu(). */

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: select_menu");
  
  lastComWid_s = sub_btn;

  /* The sub_btn widget is of type SubMenuCommand.
   * Find the shell widget which holds the submenu with the name
   * given by this SubMenuCommand.
   */
  sub_menu = XtNameToWidget(menutoplevel_s, (char *) client_data);

  if (client_data && strlen((char *)client_data) > 0) {
    FPRINTF2("    Chose submenu %s\n", (char *) client_data);
  }
  if (sub_menu) {
    int xPos, yPos, x_root, y_root, x_win, y_win, disp_h;
    Dimension wid_w, wid_h, sub_menu_w, sub_menu_h;

    level_s++;
    stack_s[level_s]  = (char *) client_data;
    wstack_s[level_s] = sub_menu;

    /* Determine location for popping up submenu. */

    XtVaGetValues(sub_btn,  XtNwidth,  &wid_w,   /* Width of submenu button.*/
		            XtNheight, &wid_h, NULL);  
    XtVaGetValues(sub_menu, XtNheight, &sub_menu_h,
		            XtNwidth,  &sub_menu_w, NULL);

    if (!event) {
      /* Not called from DisplayMenu(). */
      Window root_w, child_w;
      unsigned int obut;

      XQueryPointer(dpy_s, XtWindow(sub_btn), 
		    &root_w, &child_w, &x_root, &y_root,
		    &x_win, &y_win, &obut);
    } else if (event->type == ButtonRelease) {
      x_root = ((XButtonEvent *) event)->x_root;
      y_root = ((XButtonEvent *) event)->y_root;
      x_win  = ((XButtonEvent *) event)->x;
      y_win  = ((XButtonEvent *) event)->y;
    } else if (event->type == MotionNotify) {
      x_root = ((XMotionEvent *) event)->x_root;
      y_root = ((XMotionEvent *) event)->y_root;
      x_win  = ((XMotionEvent *) event)->x;
      y_win  = ((XMotionEvent *) event)->y;
    } else if (event->type == EnterNotify || event->type == LeaveNotify) {
      x_root = ((XCrossingEvent *) event)->x_root;
      y_root = ((XCrossingEvent *) event)->y_root;
      x_win  = ((XCrossingEvent *) event)->x;
      y_win  = ((XCrossingEvent *) event)->y;
    } else if (event->type == KeyPress) {
      x_root = ((XKeyEvent *) event)->x_root;
      y_root = ((XKeyEvent *) event)->y_root;
      x_win  = ((XKeyEvent *) event)->x;
      y_win  = ((XKeyEvent *) event)->y;
    }

    if (event && event->type != ButtonRelease && event->type != KeyPress)
      /* Place submenu just to the right of present menu. */
      xPos = x_root - x_win + wid_w + 5;
    else if (event) {
      xPos = x_root - 10;
    } else {
      /* Center top label of submenu over cursor. */
      xPos = x_root - sub_menu_w / 2;
    }

    disp_h = DisplayHeight(dpy_s, 0);

    if (y_root + sub_menu_h > disp_h)
      yPos = sub_menu_h < disp_h ? disp_h - sub_menu_h : 1;
    else yPos = y_root - wid_h / 2;

    if (xPos < 1) xPos = 1;
    if (yPos < 1) yPos = 1;
    XtVaSetValues(sub_menu, XtNx, xPos, XtNy, yPos, NULL);

    FPRINTF1("    Prior to XtPopup\n");
    XtPopup(sub_menu, XtGrabNone);
    FPRINTF1("    After XtPopup\n");
  } else {
    Warning(1, "Cannot find menu %s\n",client_data);
  }
  
  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: select_menu");
}

static void select_entry(Widget w, XtPointer client_data, XtPointer call_data)
{
  int i;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: select_entry");
  if (client_data && strlen((char *)client_data) > 0) {
    FPRINTF2("    %s \n",(char *) client_data);
  }

  for(i=level_s; i>=1; i--) {
    if (stack_s[i] && strlen(stack_s[i]) > 0) {
      FPRINTF3("    Popdown %i ?, %s\n", i, stack_s[i]);
    }
    if (wstack_s[i]  && XtIsRealized(wstack_s[i]) &&
	Find_kept_menu_wid(wstack_s[i]) == 0) {
      FPRINTF1("    Prior to XtPopdown\n");
      XtPopdown(wstack_s[i]);
      FPRINTF1("    After XtPopdown\n");
      wstack_s[i] = NULL;
    }
  }
  level_s = 0;

  Start_program((char *) client_data);
  FPRINTF2("    i=%d.\n", i);  
  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: select_entry");
}

static int Make_dialog(char *name, FILE *menu)
{
  char buf[5000], *tmp;
  int len, tot, line_num;
  dialog_entry *entry;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Make_dialog");
  FPRINTF2("    Name: %s\n", name);

  entry = (dialog_entry *) malloc(sizeof(dialog_entry));
  entry->next = dialog_list_s;
  dialog_list_s = entry;
  entry->name   = XtNewString(name);
  tmp = buf;
  tot = 5000;
  line_num = 0;

  /* Read the introductory text in a dialog. */
  while(1) {
    line_num++;
    fgets(tmp,tot,menu); /* Read one line or until buf is full. */
    len = strlen(tmp)-1;
    if (len<0) len=0;
    tot -= len;          /* Space left in buf. */
    tmp[len] = 0;
    if (len>0 && tmp[len-1] != '\\') break; /* Found end of text. */
    tmp[len-1] = '\n';   /* Additional lines of text in file. */
    tmp = &tmp[len];
  }
  if (strlen(buf) <= 0)
    Error(2, "%s%s\n", "Error in menu-file for introductory text in dialog ",
	  name);
  else {
    char *p;

    p = nextNonSpace(buf);
    if (*p == '\0')
      entry->help_text = NULL;
    else {
      entry->help_text = (char *) malloc(strlen(buf)+1);
      strcpy(entry->help_text, buf);
    }
  }

  tmp = buf;
  tot = 5000;

  /* Read the text containing input values. */
  while(1) {
    line_num++;
    fgets(tmp,tot,menu); /* Read one line or until buf is full. */
    len = strlen(tmp)-1;
    if (len<0) len=0;
    tot -= len;          /* Space left in buf. */
    tmp[len] = 0;
    if (len>0 && tmp[len-1] != '\\') break;
    tmp[len-1] = '\n';
    tmp = &tmp[len];
  }
  if (strlen(buf) <= 0)
    Error(2, "%s%s\n", "Error in menu-file for prompt text in dialog ",
	  name);
  else entry->dialog_text = (char *) malloc(strlen(buf)+1);
  strcpy(entry->dialog_text, buf);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Make_dialog");

  return(line_num);
}
      


static int Read_menulist(char *filename)
{
  FILE *menu;
  char *next, *command, buf[2048];
  int entry_start, command_start, entry_end, command_end, line_num;
  entry_type kind = UNKNOWN;
  Widget CurrentMenu = NULL, CurrentBox = NULL;
  command_entry *command_ptr;

  ENTER_FUNCTION_DEBUG("Xbuild_menu.c: Read_menulist");

  menu = fopen(filename,"r");
  if(menu==0) {
    if (filename)
      Warning(1,"Can't find menufile %s.\n",filename);
    else
      Warning(1,"No menufile to read.\n");

    LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Read_menulist");
    return(2);
  }

  /* fgets reads chunks up to and including NEWLINE or until 2047 are read. */
  /* Terminates with null character. */
  line_num = 0;
  while(fgets(buf, 2048, menu) != NULL) {

    line_num++;
    kind = interpret_line(buf, &entry_start, &entry_end,
			  &command_start, &command_end);

    switch(kind) {
    case MENUENTRY:
      next    = (char *) malloc(entry_end-entry_start+2);
      command = (char *) malloc(command_end-command_start+2);
      strcpy(next, &buf[entry_start]);
      strcpy(command, &buf[command_start]);
      Make_menuentry(next, command, CurrentBox);

      strcpy(oldBuf_s, next);
      free(next); next = NULL;

      if (dump_commands_e) {
	command_ptr = (command_entry *) malloc(sizeof(command_entry));
	command_ptr->command = command;
	command_ptr->next    = NULL;

	if (!command_list_s) {
	  command_list_s = command_ptr_s  = command_ptr;
	} else {
	  command_ptr_s->next = command_ptr;
	  command_ptr_s       = command_ptr;
	}
      }
      break;
    case COMMENT:
      break;
    case UNKNOWN:
      fprintf(stderr, "%s%d%s\n%s\n", "Error around menu line no ", line_num,
	      ": ", buf);
      break;
    default: {
      next = (char *) malloc(command_end-entry_start+2);
      (void) strcpy(next, &buf[entry_start]);

      switch (kind) {
      case SUBMENU:
	Make_submenu(next, &CurrentMenu, &CurrentBox);
	strcpy(oldBuf_s, next);
	free(next); next = NULL;
	break;
      case NEWMENU:
	Make_menumenu(next, CurrentBox);
	strcpy(oldBuf_s, next);
	break;
      case MENULINE:
	Make_menuline(next, CurrentBox);
	strcpy(oldBuf_s, next);
	free(next); next = NULL;
	break;
      case DIALOG_ENTRY: 
	line_num += Make_dialog(next, menu);
	strcpy(oldBuf_s, next);
	free(next); next = NULL;
	break;
      default:
	fprintf(stderr, "%s%d%s\n%s%s\n%s%s\n",
		"Error in menu entry at line no ",
		line_num, ": ", "  ", next, "  Previous: ", oldBuf_s);
	strcpy(oldBuf_s, next);
	free(next); next = NULL;
	break;
      } /* switch() */

      break;
    } /* default: */
    } /* switch() */
  } /* while fgets() */
  
  fclose(menu);
  XtRealizeWidget(CurrentMenu);
  XmbSetWMProperties(dpy_s, XtWindow(CurrentMenu),
		     XtName(CurrentMenu), XtName(CurrentMenu),
		     NULL, 0, NULL, NULL, NULL);

  (void) XSetWMProtocols(dpy_s, XtWindow(CurrentMenu), &wm_delete_window, 1);

  XtPopup(menutoplevel_s, XtGrabNone);

  LEAVE_FUNCTION_DEBUG("Xbuild_menu.c: Read_menulist");
  return(0);

} /* Read_menulist() */

void Init_menu(char *filename, char *menuname, Widget parent)
{
  char *fname, *fname2 = NULL;

  kept_head_s.next = &kept_tail_s;

  dpy_s = XtDisplay(parent);
  Control_s = parent;
  fname = filename;
  while (fname && *fname) {
   NextFilename(&fname, &fname2);
    if (fname2) Read_menulist(fname2);
  }

  if (fname == filename) Read_menulist(filename);
  if (fname2) free(fname2);

  if (dump_commands_e) {
    command_entry *command_ptr = command_list_s;

    while (command_ptr != NULL) {
      Start_program(command_ptr->command);
      command_ptr = command_ptr->next;
    } /* while */

    quit_xshow(NULL, NULL, NULL, NULL);

  } /* if */
} /* Init_menu() */



/*L*

________________________________________________________________

		CallDialog
________________________________________________________________

Name:           CallDialog - Popup dialog widget for the dialog list
                entry identified by "name".

Syntax:		| #include "Xbuild_menu.h"
                | 
                | int CallDialog( char *dialog_name, char *command_name,
		|    char **txt );
		|
Description:    Search through the list of dialogs until an entry with
                name 'dialog_name' is found. Popup this dialog with an "abort"
		button and an "accept" button.

Return value:   | -1: Could not find the dialog.
                |  0: The "abort" button was pushed or dialog killed by
		|     window manager.
		|  1: The "accept" button was pushed.

See also:       'SimpleDialog(3)'

Author:		Otto Milvang and Svein Bøe

Doc:            Svein Bøe
________________________________________________________________

*/

static int type_of_dialog(char *dialog)
{
  char *d, *p;
  int found = 0;

  d = XtNewString(dialog);
  p = d;

  /* Old-style dialog has two (or no) '#' per line. New-style has 4 '#'. */

  while (*p && *p != '\n') {
    if (*p++ == '#') found++;
  }

  XtFree(d);

  return(found != 4);

} /* type_of_dialog() */

int CallDialog(char *dialog_name, char *command_name, char **return_txt)
{
  int status;
  dialog_entry *entry;
  Boolean old_style;

  entry = dialog_list_s;
  while (entry != NULL && strcmp(dialog_name, entry->name))
    entry = entry->next;
  if (entry == NULL) return(-1);

  old_style = type_of_dialog(entry->dialog_text);

  if (old_style)
    status = SimpleDialog(menutoplevel_s, entry->help_text, entry->dialog_text,
			  return_txt, "abort","accept",
			  "xshow - dialog", "xshow - dialog");
  else {
    char *label = NULL;
    button_table buttons;

    if (command_name) {
      label = (char *) malloc((strlen("Program: ") + strlen(command_name) + 1)
			      * sizeof(char));
      (void) strcpy(label, "Program: ");
      (void) strcat(label, command_name);
    }

    buttons = init_button_table(NULL, 3);
    buttons[2].closure = (XtPointer) entry->help_text;

    status = FormDialog(menutoplevel_s, label, entry->dialog_text,
			return_txt, buttons, 3,
			"xshow - dialog", "xshow - dialog");
  }

  if (status == -1) status = 0;

  return(status);

} /* CallDialog() */
