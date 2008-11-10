

/*C*

________________________________________________________________

        Macro
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

#include <xite/includes.h>
#include <stdlib.h>
#include <errno.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif
#ifdef HAVE_TIME_H
#  include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#include <stdlib.h>
#include "Macro.h"
#include <Xfwf/Toggle.h>
#include <xite/ShellWids.h>
#include <xite/Xdialog.h>
#include <xite/FormDialog.h>
#include <xite/debug.h>
#include <xite/utils.h>
#include <xite/strings.h>
#ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
#endif
#include "Xprog.h"

extern char *menufilename_e;  /* If non-NULL, don't read
			       * standard menufiles. */

macroEntryPtr current_macro_e = NULL;
int           make_macro_e    = 0;

static Display *dpy_s;
static Window screen_s;
static Widget Control_s;
static macroEntryPtr macro_entry_head_s   = NULL;
static macroEntryPtr current_macro_prev_s = NULL;
static FILE *logfile_s;
static char *logfileName_s                = NULL;
static char *file_args_s                  = NULL;

void Add_macro_entry(macroEntryPtr entry)
{
  macroEntryPtr p;

  if (!make_macro_e) return;

  if (macro_entry_head_s == NULL) {
    macro_entry_head_s = entry;
    current_macro_prev_s = NULL;
    current_macro_e = entry;
  }
  else {
    for (p = macro_entry_head_s; p->next_entry != NULL; p = p->next_entry) ;

    p->next_entry = entry;
    current_macro_prev_s = p;
    current_macro_e = entry;
  }
} /* Add_macro_entry() */

void Remove_current_entry(void)
{
  if (make_macro_e == 0) return;

  if (current_macro_prev_s == NULL) {
    /* Only the current_macro_e entry is registered in the chain. */

    XtFree((char *) current_macro_e); current_macro_e = NULL;
    macro_entry_head_s = NULL;
    return;
  }

  if (current_macro_prev_s->next_entry != current_macro_e) {
    /* This is an error condition. Should never happen. */

    return;
  }

  /* Bypass current_macro_e. */
  current_macro_prev_s->next_entry = current_macro_e->next_entry;

  XtFree((char *) current_macro_e);
  current_macro_e = current_macro_prev_s->next_entry;

} /* Remove_current_entry() */

void Add_macro_item(macroEntryPtr entry, macroItemPtr item)
{
  macroItemPtr p;

  if (!make_macro_e) return;

  if (entry->first_item == NULL) entry->first_item = item;
  else {
    for (p = entry->first_item; p->next_item != NULL; p = p->next_item) ;

    p->next_item = item;
  }

} /* Add_macro_item() */

void Add_macro_widget(macroEntryPtr entry, Widget wid)
{
  macroItemPtr p;

  if (!make_macro_e) return;

  if (entry->first_item == NULL) return;
  else {
    wid = TopLevelWidget(wid);

    for (p = entry->first_item; p != NULL; p = p->next_item) {
      if ((p->type == 1 || p->type == 2) && p->wid == NULL) {
	p->wid = wid;
	break;
      }
    }
  }

} /* Add_macro_widget() */

static void CheckItem(macroItemPtr itemPrev, macroItemPtr itemP, int *found)
{
  /* Check if itemPrev has the same widget reference as the input item itemP.
   */
  if (itemPrev->wid == itemP->wid) {

    if (itemPrev->type == 2) {
      /* Found an output item.
       * Use a temporary file for this correspondence.
       */
      
      XtFree(itemP->item);
      itemP->item = XtNewString(itemPrev->item);
    }

    if (itemPrev->type == 1 || itemPrev->type == 2) {
      
      *found = 1;
      itemP->mate    = itemPrev;
      itemPrev->mate = itemP;
    }
  }
} /* CheckItem() */

static void CheckEntry(macroEntryPtr entryP, macroItemPtr itemP, int *found)
{
  /* Search for an output item with the same widget reference as the input
   * item itemP.
   */
  
  macroItemPtr itemPrev;
  
  /* Check each macroItem in this macroEntry. */
  itemPrev = entryP->first_item;
  while (itemPrev != NULL && !(*found)) {
    
    CheckItem(itemPrev, itemP, found);
    itemPrev = itemPrev->next_item;
  }

} /* CheckEntry() */

static void MateItem(macroEntryPtr entryP, macroItemPtr itemP)
{
  /* Check if this input item comes from a previous output. */

  macroEntryPtr entryPrev;
  int found = 0;
  
  entryPrev = macro_entry_head_s;
  found = 0;
  while (entryPrev != entryP && !found) {
    CheckEntry(entryPrev, itemP, &found);
    entryPrev = entryPrev->next_entry;
  }

} /* MateItem() */

static void FindMates(macroEntryPtr entryP)
{
  macroItemPtr itemP;
  char *tmpFileName;
  
  itemP = entryP->first_item;
  while (itemP != NULL) {
    
    /* Make sure that itemP points to an XSHOWINFILE or XSHOWOUTFILE item
     * (registered in the same order as occuring in the macro-line),
     * or none.
     */
    
    while (itemP != NULL && itemP->type != 1 && itemP->type != 2) {
      itemP = itemP->next_item;
    }
    
    if (itemP == NULL) {
      /* Did not find any (or more) XSHOWINFILE or XSHOWOUTFILE item. */
      
      break;
    }
    
    if (itemP->type == 1) {

      /* This input could come from a previous output. */
      MateItem(entryP, itemP);
  
    } else if (itemP->type == 2) {

      /* This output should go to a temporary file. */
      tmpFileName  = tempnam(NULL, "xshow");
      itemP->item = XtNewString(tmpFileName);

    }
    
    itemP = itemP->next_item;
  } /* each macro item */

} /* FindMates() */

static void FindAllMates(void)
{
  macroEntryPtr entryP;

  /* Run through each macro_entry. */
  for (entryP = macro_entry_head_s;
       entryP != NULL;
       entryP = entryP->next_entry) {

    FindMates(entryP);

  } /* each macro entry */

} /* FindAllMates() */

static void AssignVarNumber(macroItemPtr itemP, int *var_num)
{
  /* No mate. Assign variable number. */
  char var[10];
  
  sprintf(var, "$%d", *var_num);
  (*var_num)++;
  XtFree(itemP->item);
  itemP->item = XtNewString(var);

  (void) strcat(file_args_s, " ");

  if (itemP->type == 1) {
    (void) strcat(file_args_s, XSHOWINFILE);
  } else {
    (void) strcat(file_args_s, XSHOWOUTFILE);
  }

} /* AssignVarNumber() */

static void AssignVarTmpStorage(macroItemPtr itemP, int *var_num)
{
  /* Mate is XSHOWINFILE with variable number assigned.
   * Use temporary storage (add initial macroEntry).
   */
  macroEntryPtr macro_entry;
  macroItemPtr  macro_item;
  
  if (itemP->type != 1) return;

  InitMacroEntry(&macro_entry);
  macro_entry->entry_name = XtNewString("biffcopy");
  
  macro_entry->next_entry = macro_entry_head_s;
  macro_entry_head_s      = macro_entry;
  
  InitMacroItem(&macro_item);
  macro_item->item = XtNewString("biffcopy");
  Add_macro_item(macro_entry, macro_item);
  
  InitMacroItem(&macro_item);
  macro_item->type = 1;
  AssignVarNumber(macro_item, var_num);
  Add_macro_item(macro_entry, macro_item);
  
  InitMacroItem(&macro_item);
  macro_item->item = XtNewString(tempnam(NULL, "xshow"));
  Add_macro_item(macro_entry, macro_item);
  
  XtFree(itemP->item);
  itemP->item = XtNewString(macro_item->item);

} /* AssignVarTmpStorage() */

static void AssignItemVarNumber(macroItemPtr itemP, int *var_num)
{

  if (itemP->type != 1 && itemP->type != 2) return;

  if (itemP->mate == NULL) {

    /* Item does not get input from previous or send output to later. */

    AssignVarNumber(itemP, var_num);

  }
  else if (itemP->mate->item &&
	      strcmp(itemP->mate->item, XSHOWINFILE) == 0) {
    /* Mate is further out in the entry chain. */

    if (itemP->type == 1) {
      /* Both itemP and its mate get input from the same. */

      AssignVarTmpStorage(itemP, var_num);
    }

    /* The case where itemP is output and its mate is input has already
     * been taken care of when mating (using temporary storage).
     */

  }
  else if (itemP->type == 1 && itemP->mate->type == 1) {
    /* Mate is an item in a previous entry. Use the same item text. */

    XtFree(itemP->item);
    itemP->item = XtNewString(itemP->mate->item);

  }
  else if (itemP->mate->item &&
	      strncmp(itemP->mate->item, "$", 1) == 0) {

    /* Mate is an item in a previous entry.
     * This should never happen. */

    fprintf(stderr, "xshow: This macro error should never occur.\n");

  }

} /* AssignItemVarNumber() */

static void AssignVariableNumbers(macroEntryPtr entryP, int *var_num)
{
  /* Search for input and output items. */
  
  macroItemPtr  itemP;
  
  itemP = entryP->first_item;
  while (itemP != NULL) {
    
    /* Skip items which are neither input nor output. */
    
    while (itemP != NULL && itemP->type != 1 && itemP->type != 2) {
      itemP = itemP->next_item;
    }
    
    if (itemP == NULL) {
      /* Did not find any (or more) input or output items. */
      
      break;
    }

    AssignItemVarNumber(itemP, var_num);

    itemP = itemP->next_item;
  }

} /* AssignVariableNumbers() */

static int AssignAllVariableNumbers(void)
{
  macroEntryPtr entryP;
  int var_num = 1;

  for (entryP = macro_entry_head_s;
       entryP != NULL;
       entryP = entryP->next_entry) {
    
    AssignVariableNumbers(entryP, &var_num);
  }

  return(var_num);

} /* AssignAllVariableNumbers() */

static int ProcessMacro(void)
{
  int numbers;

  ENTER_FUNCTION_DEBUG("Macro.c: ProcessMacro");

  FindAllMates();

  numbers = AssignAllVariableNumbers();

  LEAVE_FUNCTION_DEBUG("Macro.c: ProcessMacro");
  return(numbers);

} /* ProcessMacro() */

static void PrintMacro(FILE *file, int numVars)
{
  macroEntryPtr entryP;
  macroItemPtr  itemP;
  time_t now;

  ENTER_FUNCTION_DEBUG("Macro.c: PrintMacro");

  if (macro_entry_head_s == NULL) {
    LEAVE_FUNCTION_DEBUG("Macro.c: PrintMacro");
    return;
  }
  
  time(&now);

  fprintf(file, "%s\n\n%s\n%s%s\n\n%s%d\n%s\n%s\n%s\n%s\n\n",
  "#!/bin/sh",
  "# This macro has been generated automatically by xshow",
  "# CreationTime: ", ctime(&now),
  "if test \"$#\" -ne ", numVars,
  "then",
  "    echo \"$0: Wrong number of arguments.\"",
  "    exit 1",
  "fi");
  
  for (entryP  = macro_entry_head_s;
       entryP != NULL;
       entryP  = entryP->next_entry) {
    
    for (itemP   = entryP->first_item;
	 itemP  != NULL;
	 itemP   = itemP->next_item) {
      
      fprintf(file, "%s ", itemP->item);
    }
    
    fprintf(file, "\n");
  }

  LEAVE_FUNCTION_DEBUG("Macro.c: PrintMacro");

} /* PrintMacro() */

void InitMacroEntry(macroEntryPtr *macro_entry)
{
  *macro_entry               = XtNew(macroEntry);
  (*macro_entry)->entry_name = NULL;
  (*macro_entry)->next_entry = NULL;
  (*macro_entry)->first_item = NULL;

} /* InitMacroEntry() */

void InitMacroItem(macroItemPtr *macro_item)
{
  *macro_item              = XtNew(macroItem);
  (*macro_item)->item      = NULL;
  (*macro_item)->type      = 0;
  (*macro_item)->wid       = NULL;
  (*macro_item)->mate      = NULL;
  (*macro_item)->next_item = NULL;

} /* InitMacroItem() */

void FreeMacro(void)
{
  macroEntryPtr entryP, entryNext;
  macroItemPtr  itemP, itemNext;

  for (entryNext = macro_entry_head_s; entryNext != NULL; ) {
    for (itemNext = entryNext->first_item; itemNext  != NULL; ) {
      itemP    = itemNext;
      itemNext = itemP->next_item;
      XtFree(itemP->item);    itemP->item = NULL;
      XtFree((char *) itemP); itemP       = NULL;
    }
    entryP    = entryNext;
    entryNext = entryP->next_entry;
    XtFree(entryP->entry_name); entryP->entry_name = NULL;
    XtFree((char *) entryP);    entryP             = NULL;
  }

  macro_entry_head_s = NULL;
  current_macro_e = NULL;
  current_macro_prev_s = NULL;

} /* FreeMacro() */



/*L:menufile_dialog*

________________________________________________________________

		menufile_dialog
________________________________________________________________

Name:		menufile_dialog - pop up dialog and ask for name of
                menufile
Syntax:		| static int menufile_dialog(char *new_file);
Return value:	| 0: Dialog aborted. Stop logging, but don't change menufile.
                | 1: Ok. 'new_file' is the name of the menufile to edit.
		| 2: Can not happen.
		| 3: Continue logging.
Author:		Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

static int menufile_dialog(char **new_file)
{
  char dialog_spec[1000], *filenames = NULL, *filename = NULL;
  char *home_menu_file = NULL, *std_menu_files = NULL, *return_text = NULL;
  char *t;
  int status;
  button_table buttons;
  char *radio1_lead  = "# Filename # # r1 # ";
  char *radio0_lead  = "# Filename # # r0 # ";
  char *radio0__lead = "# Filename # # r0- # ";
  char *string_lead  = "# Filename # # s  # ";

  dialog_spec[0] = '\0';

  if (menufilename_e)
    (void) strcat(strcat(dialog_spec, radio1_lead), menufilename_e);

  home_menu_file = envfindfilemulti("HOME", "xshow_menues");
  std_menu_files = envfindfilemulti("XSHOWPATH", "xshow_menues");

  if (home_menu_file != NULL && std_menu_files != NULL)
    filenames =
      (char *) malloc(strlen(home_menu_file) + strlen(std_menu_files) + 2);
  else if (home_menu_file != NULL)
    filenames = (char *) malloc(strlen(home_menu_file) + 1);
  else if (std_menu_files != NULL)
    filenames = (char *) malloc(strlen(std_menu_files) + 1);
  else filenames = (char *) malloc(1);

  filenames[0] = '\0';

  if (home_menu_file) {
    (void) strcat(filenames, home_menu_file);
    free(home_menu_file); home_menu_file = NULL;
    if (std_menu_files) (void) strcat(filenames, ":");
  }
  if (std_menu_files) {
    (void) strcat(filenames, std_menu_files);
    free(std_menu_files); std_menu_files = NULL;
  }
  
  while (filenames != NULL) {
    NextFilename(&filenames, &filename);

    if (filename && access(filename, W_OK) == 0) {
      /* User has write-access to this file. */

      (void) strcat(strcat(dialog_spec, radio0_lead), filename);
    }
    if (filename) {
      free(filename); filename = NULL;
    }
  }

  if (filenames) {
    free(filenames); filenames = NULL;
  }

  if (strlen(dialog_spec) > 0) {
    (void) strcat(strcat(dialog_spec, radio0__lead), "Other");
    (void) strcat(dialog_spec, " # Filename (if \"Other\") # # s # ");
  } else {
    (void) strcat(dialog_spec, string_lead);
  }

  buttons             = init_button_table(NULL, 4);
  buttons[0].name     = "Abort_macro";
  buttons[0].label    = "Abort macro";
  buttons[1].name     = "Accept_macro";
  buttons[1].label    = "Accept, store macro, edit menufile";
  buttons[2].closure  = (XtPointer)
"Choose an xshow menu-file.\n\
A menu entry for the current macro will be appended to the file.\n\
You must have write access to the chosen file.";
  buttons[3].name     = "Continue_macro";
  buttons[3].label    = "Continue macro";
  buttons[3].callback = NULL;
  buttons[3].closure  = NULL;

  status = FormDialog(Control_s, "Supply menu filename",
		      dialog_spec, &return_text, buttons, 4,
		      "Menufile dialog", "Menufile");

  free(buttons); buttons = NULL;

  if (status == 0 || status == 3) {
    /* 0: Aborted dialog (meaning: stop dialog, but don't change menufile).
     * 3: Continue logging.
     */
    *new_file = NULL;
    return(status);
  }

  t           = return_text;
  return_text = remove_quotes_from_word(t);

  if (return_text && sscanf(return_text, "%s", dialog_spec) <= 0) {
    Warning(1, "No filename given in menufile dialog.\n");
    *new_file = NULL;
    free(return_text); return_text = NULL;
    return(status);
  }

  *new_file = XtNewString(tilde_expand(dialog_spec));
  if (t) {
    free(t); t = NULL; return_text = NULL;
  }

  return(status);
	     
} /* menufile_dialog() */



/*L:EditMenuFile*

________________________________________________________________

		EditMenuFile
________________________________________________________________

Name:		EditMenuFile - Append menuentry to end of filename
Syntax:		static int EditMenuFile(char *filename);
Return value:	| 0: File not edited because of open or positioning failure.
                | 1: Ok, file edited
                
Author:		Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

static int EditMenuFile(char *filename)
{
  FILE *menufile;
  char *fname;

  errno = 0;
  if ((menufile = fopen(filename, "a")) == NULL) {
    Message(1, "%s\n", strerror(errno));
    return(0);
  }

  if (fseek(menufile, 0L, 2) != 0) {
    Warning(1, "xshow: Error when positioning in menu file.\n");
    return(0);
  }

  if (ftell(menufile) == 0) {
    /* The file is empty. */

    fprintf(menufile, "%c%s\n", ':', "My programs");
  }

  fname = FilenamePart(logfileName_s);
  fprintf(menufile, " %s ; %s%s\n", fname, logfileName_s, file_args_s);
  fclose(menufile);

  return(1);

} /* EditMenuFile() */



/*L:AddMenuEntry*

________________________________________________________________

		AddMenuEntry
________________________________________________________________

Name:		AddMenuEntry - Ask for name of menu file (cycling through
                write-accessible files in $XSHOWPATH) and append menu
		entry to file.
Syntax:		static int AddMenuEntry(void);
Return value:	| 0: Ok, menu file edited.
                | 1: No menu-file edited, because dialog was aborted, i.e.
		|    stop logging but don't change menu-file.
		| 2: Filename was empty or file could not be opened or
		|    positioned in.
		| 3: Continue logging.
Author:		Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

static int AddMenuEntry(void)
{
  char *return_text = NULL;
  int status;

  status = menufile_dialog(&return_text);
  /* status == 0: Dialog aborted. Stop logging, but don't change menufile.
   * status == 1: Ok. 'return_text' contains the name of the menufile to edit.
   * status == 2: Cannot happen.
   * status == 3: Continue logging.
   */

  if (status == 0) status = 1;
  else if (status == 1 && return_text) {
    /* Menufile has been determined. */

    status = EditMenuFile(return_text);
    /* status == 0: File not edited because of open or positioning failure.
     * status == 1: File edited.
     */

    if (status == 0) status = 2;
    else if (status == 1) {
      Widget form, stdErr, menu;

      /* Editing was successfull. */
      status = 0;

      form   = XtNameToWidget(Control_s, "mainForm");
      stdErr = XtNameToWidget(Control_s, "*stderr");
      menu   = XtNameToWidget(Control_s, "mainMenu");

      XDefineCursor(dpy_s, XtWindow(form),
		    XCreateFontCursor(dpy_s, XC_watch));
      XDefineCursor(dpy_s, XtWindow(stdErr),
		    XCreateFontCursor(dpy_s, XC_watch));
      XDefineCursor(dpy_s, XtWindow(menu),
		    XCreateFontCursor(dpy_s, XC_watch));

      if (menufilename_e == NULL)
	/* No option "-m" used at start of xshow, standard menufiles
	 * must be read. */

	Init_prog(return_text, TRUE);
      else {
	/* Option "-m" used. Do not read standard menufiles. */

	/* Read both the file given by option "-m" and the newly
	 * edited file. */
	char *fname;

	fname = String_cat(menufilename_e, return_text, NULL);
	Init_prog(fname, FALSE);
      } /* No standard files. */

      XDefineCursor(dpy_s, XtWindow(form),   (Cursor) NULL);
      XDefineCursor(dpy_s, XtWindow(stdErr), (Cursor) NULL);
    } /* Editing successfull. */
  } /* Menufile has been determined. */
  else if (status == 1) status = 2; /* Empty filename. */

  if (return_text) { free(return_text); return_text = NULL; }

  return(status);
  
} /* AddMenuEntry() */

void LogMacro(Widget wid, XtPointer client_data, XtPointer call_data)
{
  char mess[80];
  Dimension w;
  int failed = 0;
  log_macro_data *log_macro_dataP;

  ENTER_FUNCTION_DEBUG("Macro.c: LogMacro");

  log_macro_dataP = (log_macro_data *) client_data;
  Control_s       = log_macro_dataP->wid;
  dpy_s           = log_macro_dataP->dpy;
  screen_s        = log_macro_dataP->screen;

  if (make_macro_e) {
    int status, num_args;

    /* Macro logging is on. Stop it? */

    XtVaSetValues(wid, XtNon, make_macro_e, NULL);

    num_args = ProcessMacro();
    status   = AddMenuEntry();
    /* status == 0: Ok, menu file edited.
     * status == 1: No menu-file edited, because dialog was aborted, i.e.
     *              stop logging but don't change menu-file.
     * status == 2: Filename was empty or file could not be opened or
     *              positioned in.
     * status == 3: Continue logging.
     */

    if (status == 3 || status == 2) {
      /* Continue logging. */

      if (file_args_s) free(file_args_s);
      file_args_s = XtMalloc(1024);
      file_args_s[0] = '\0';

      LEAVE_FUNCTION_DEBUG("Macro.c: LogMacro");

      return;
    }

    if (status == 1) {
      /* Aborted menufile dialog. Stop logging, but don't edit menufile. */
      sprintf(mess, "Macro: off");      
    } else if (status == 0) {
      sprintf(mess, "Macro: off");

      PrintMacro(logfile_s, num_args - 1);
      FreeMacro();

      errno = 0;
      if (fclose(logfile_s)) {
	Message(1, "%s\n", strerror(errno));
	failed = 1;
      }
    }
  } else {
    /* Macro logging is off. Start logging. */

    struct stat file_stat;
    char *return_text;

    if (SelectWriteFile(Control_s, writeMacro, &return_text) == 0) {
      failed = 1;
    }

    errno = 0;
    if (failed == 0 && (stat(return_text, &file_stat) != 0)) {
      /* Error */
      if (errno != ENOENT) {
	/* Error occured for existing file. */
	Message(1, "%s\n", strerror(errno));
	failed = 1;
      } else {
	/* File does not exist. */
	
	errno = 0;
	if ((logfile_s = fopen(return_text, "w")) == NULL) {
	  Message(1, "%s\n", strerror(errno));
	  failed = 1;
	} else {
	  logfileName_s = XtNewString(return_text);
	}
      }
    } else if (failed == 0) {
      /* File exists. */
      
      errno = 0;
      if ((logfile_s = fopen(return_text, "w")) == NULL) {
	Message(1, "%s\n", strerror(errno));
	failed = 1;
      } else {
	logfileName_s = XtNewString(return_text);	
      }
    }

    if (failed == 0) {
      sprintf(mess, "Macro: on");
      errno = 0;
      if (chmod(return_text, S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH)
	  != 0) {
	Message(1, "%s\n", strerror(errno));
	failed = 1;
      }
    }

    XtFree(return_text); return_text = NULL;
  }

  if (!failed) {
    make_macro_e = 1 - make_macro_e;

    if (!make_macro_e) {
      XtFree(file_args_s); file_args_s = NULL;
    } else {
      file_args_s = XtMalloc(1024);
      file_args_s[0] = '\0';
    }

  } else {
    make_macro_e = 0;
    sprintf(mess, "Macro: off");
    XtFree(file_args_s); file_args_s = NULL;
    FreeMacro();
  }
    
  XtVaGetValues(wid, XtNwidth, &w, NULL);
  XtVaSetValues(wid, XtNlabel, mess,
		XtNwidth, w,
		XtNon, make_macro_e,
		NULL);

  LEAVE_FUNCTION_DEBUG("Macro.c: LogMacro");

} /* LogMacro() */
