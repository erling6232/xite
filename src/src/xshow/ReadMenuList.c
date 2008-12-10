
#ifdef MAIN

/*P:ReadMenuList*

________________________________________________________________

		ReadMenuList
________________________________________________________________

Name:		ReadMenuList - Make html-files to reflect xshow menu
                hierarchy

Syntax:		ReadMenuList <menufilename> [<html-filename>]

Description:	Read the menu hierarchy description of an xshow menu-file.
                Print html-files reflecting this hierarchy, one for each
		menu/submenu, unless <html-filename> is given on the
		command-line. In the latter case, all menus are printed in the
		same file. The files are stored in current working
		directory.

Author:		Svein Bøe, Ifi, UiO

Examples:	cd $XITE_DOC; \
                ReadMenuList $XITE_HOME/data/xshow/xshow_menues
Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <ctype.h>
#include <errno.h>
#include <xite/debug.h>
#include <xite/message.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_MALLOC_H
#include XITE_STRING_H
#include <xite/utils.h>
#include <xite/strings.h>

typedef struct _dialog_entry 
{
   char *name;
   char *help_text;
   char *dialog_text;
   struct _dialog_entry *next;
} dialog_entry;

typedef enum {
  UNKNOWN   = -1,
  MENUENTRY = 0,
  MENULINE,
  NEWMENU,
  SUBMENU,
  DIALOG_ENTRY,
  COMMENT
} entry_type;

#define MAX_LEVEL 20

typedef struct _menu
{
  char         *menu_name, *full_command, *command_name;
  struct _menu *children[MAX_LEVEL];
  struct _menu *next;
  entry_type   entryType;
  int          num_children;
} menu;

static char         oldBuf_s[2048];
static dialog_entry *dialog_list_s = NULL;
static char         submenu_indicator_s[4] = "...";
static menu         *toplevel_s = NULL;

static void Make_submenu(char *menuname, menu **current)
{
  menu *new;
  char *ptr, *com;

  ENTER_FUNCTION_DEBUG("ReadMenuList.c: Make_submenu");
  FPRINTF2("    |%s|\n", menuname);

  new = (menu *) malloc(sizeof(menu));

  new->menu_name    = menuname;
  new->num_children = 0;
  new->children[0]  = NULL;
  new->entryType    = SUBMENU;
  new->next         = NULL;

  if (toplevel_s == NULL) {
    toplevel_s = new;
  } else {
    (*current)->next = new;
  }

  ptr = com = (char *) malloc(strlen(new->menu_name) + 1);
  (void) strcpy(com, new->menu_name);
  while (*ptr) {
    if (*ptr && !(isalnum(*ptr) || *ptr == '_')) *ptr = '_';
    ptr++;
  }

  new->command_name = com;
  *current          = new;

  LEAVE_FUNCTION_DEBUG("ReadMenuList.c: Make_submenu");

} /* Make_submenu() */


static void Make_menuentry(char *entryname, char *item, menu *current)
{
  char *com, *ptr;
  menu *men_entry;

  ENTER_FUNCTION_DEBUG("ReadMenuList.c: Make_menuentry");

  FPRINTF2("    |%s|\n", entryname);

  men_entry = (menu *) malloc(sizeof(menu));
  men_entry->menu_name = entryname;

  current->children[current->num_children++] = men_entry;
  men_entry->entryType = MENUENTRY;
  men_entry->full_command = item;

  ptr = com = (char *) malloc(strlen(men_entry->full_command) + 1);
  (void) strcpy(com, men_entry->full_command);
  /* Find end of command-name. */
  while (*ptr && !isspace(*ptr)) ptr++;
  *ptr = 0;

  men_entry->command_name = com;

  LEAVE_FUNCTION_DEBUG("ReadMenuList.c: Make_menuentry");

} /* Make_menuentry() */

static void Make_menumenu(char *entryname, menu **current)
{
  char *com, *ptr;
  menu *men = NULL;

  ENTER_FUNCTION_DEBUG("ReadMenuList.c: Make_menumenu");

  FPRINTF2("    |%s|\n", entryname);
  
  men = (menu *) malloc(sizeof(menu));

  men->menu_name    = entryname;
  men->num_children = 0;
  men->children[0]  = NULL;
  men->entryType    = NEWMENU;

  (*current)->children[(*current)->num_children++] = men;

  ptr = com = (char *) malloc(strlen(men->menu_name) + 1);
  (void) strcpy(com, men->menu_name);
  while (*ptr) {
    if (*ptr && !(isalnum(*ptr) || *ptr == '_')) *ptr = '_';
    ptr++;
  }

  men->command_name = com;

  LEAVE_FUNCTION_DEBUG("ReadMenuList.c: Make_menumenu");

} /* Make_menumenu() */

static void Make_menuline(char *entryname, menu *current)
{
  menu *men_entry;

  ENTER_FUNCTION_DEBUG("ReadMenuList.c: Make_menuline");

  FPRINTF2("    |%s|\n", entryname);

  men_entry = (menu *) malloc(sizeof(menu));
  men_entry->menu_name = entryname;

  men_entry->num_children = 0;
  men_entry->children[0]  = NULL;
  men_entry->entryType    = MENULINE;

  current->children[current->num_children++] = men_entry;

  LEAVE_FUNCTION_DEBUG("ReadMenuList.c: Make_menuline");

} /* Make_menuline() */

static int Make_dialog(char *name, FILE *menu_file)
{
  char buf[5000], *tmp;
  int len, tot, line_num;
  dialog_entry *entry;

  entry = (dialog_entry *) malloc(sizeof(dialog_entry));
  entry->next = dialog_list_s;
  dialog_list_s = entry;
  entry->name = name;
  tmp = buf;
  tot = 5000;
  line_num = 0;

  while(1) {
    line_num++;
    fgets(tmp,tot,menu_file);
    len = strlen(tmp)-1;
    if (len<0) len=0;
    tot -= len;
    tmp[len] = 0;
    if (len>0 && tmp[len-1] != '\\') break;
    tmp[len-1] = '\n';
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

  while(1) {
    line_num++;
    fgets(tmp,tot,menu_file);
    len = strlen(tmp)-1;
    if (len<0) len=0;
    tot -= len;
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

  return(line_num);

} /* Make_dialog() */

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

static int Read_menulist(char *filename)
{
  FILE *menu_file;
  char *next, *command, buf[2048];
  int entry_start, command_start, entry_end, command_end, line_num;
  entry_type kind = UNKNOWN;
  menu *CurrentMenu = NULL;

  menu_file = fopen(filename,"r");
  if(menu_file==0) {
    if (filename)
      Warning(1, "Can't find menufile %s.\n",filename);
    else
      Warning(1, "No menufile to read.\n");
    return(2);
  }

  /* fgets reads chunks up to and including NEWLINE or until 2047 are read*/
  /* Terminates with null character */
  line_num    = 0;
  while(fgets(buf, 2048, menu_file) != NULL) {
    line_num++;
    kind = interpret_line(buf, &entry_start, &entry_end,
			  &command_start, &command_end);

    switch(kind) {
    case MENUENTRY:
      next    = (char *) malloc(entry_end-entry_start+2);
      command = (char *) malloc(command_end-command_start+2);
      strcpy(next, &buf[entry_start]);
      strcpy(command, &buf[command_start]);
      Make_menuentry(next, command, CurrentMenu);

      strcpy(oldBuf_s, next);
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
        Make_submenu(next, &CurrentMenu);
        strcpy(oldBuf_s, next);
        break;
      case NEWMENU:
        Make_menumenu(next, &CurrentMenu);
        strcpy(oldBuf_s, next);
        break;
      case MENULINE:
        Make_menuline(next, CurrentMenu);
        strcpy(oldBuf_s, next);
        break;
      case DIALOG_ENTRY: 
        line_num += Make_dialog(next, menu_file);
        strcpy(oldBuf_s, next);
        break;
      default:
        fprintf(stderr, "%s%d%s\n%s%s\n%s%s\n",
                "Error in menu entry at line no ",
                line_num, ": ", "  ", next, "  Previous: ", oldBuf_s);
        strcpy(oldBuf_s, next);
        break;
      } /* switch() */

      break;
    } /* default: */
    } /* switch() */
    } /* while fgets() */

  fclose(menu_file);
  return(0);
}

static void Init_menu(char *filename)
{
  char *fname, *fname2 = NULL;

  toplevel_s = NULL;

  fname = filename;
  while (fname && *fname) {
    NextFilename(&fname, &fname2);
    if (fname2) Read_menulist(fname2);
  }

  if (fname == filename) Read_menulist(filename);
  if (fname2) free(fname2);

} /* Init_menu() */

static void Print_menu(menu *ptr, FILE *file)
{
  menu **menP;
  char index_filename[132];
  int just_started = 1;
  FILE *fileLocal;

  if (ptr == NULL)  return;

  fileLocal = file;

  if (file == NULL) {
    sprintf(index_filename, "%s%s%s", ptr->command_name, "_Index", ".html");

    if ((file = fopen(index_filename, "w")) == NULL) {
      Message(1, "%s\n", strerror(errno));
      return;
    }

    fprintf(file, "<html>\n<head>\n  <title>\n");
    fprintf(file, "  %s menu for the XITE display program xshow\n",
	    ptr->menu_name); 
    fprintf(file, "  </title>\n");
    fprintf(file, "%s\n%s\n%s%s%s\n%s\n%s\n",
	    "  <meta name=\"author\" content=\"Svein Bøe\">",
	    "  <meta name=\"keywords\" content=\"XITE BIFF xshow\">",
	    "  <meta name=\"description\" content=\"", ptr->menu_name,
	    " menu for the XITE display program xshow\">",
	    "  <link rel=\"up\" href=\"Contents.html\">",
	    "</head>");
    fprintf(file, "\n%s\n\n",
	    "<body background=\"../mona_canny.jpg\">");

  }

  if (ptr->menu_name && strcmp(ptr->menu_name, "Xshow") == 0)
    fprintf(file, "<h2> <a href=\"xshow_1.html\"> %s </a> </h2> \n",
	    ptr->menu_name);
  else fprintf(file, "<h2>%s menu for the XITE display program xshow</h2> \n",
	       ptr->menu_name);

  for (menP = ptr->children; menP - ptr->children < ptr->num_children;
       menP++) {
    if ((*menP)->entryType == MENUENTRY) {
      if (just_started) fprintf(file, "<ul> \n");
      fprintf(file,
	      "  <li> <a href=\"%s%s\"> %s</a>\n",
	      (*menP)->command_name, "_1.html", (*menP)->menu_name);
      just_started = 0;
    } else if ((*menP)->entryType == NEWMENU) {
      if (just_started) fprintf(file, "<ul> \n");
      fprintf(file,
	      "  <li> <a href=\"%s%s%s\"> %s%s</a>\n",
	      (*menP)->command_name, "_Index", ".html", (*menP)->menu_name,
	      submenu_indicator_s);
      just_started = 0;
    } else if ((*menP)->entryType == MENULINE) {
      if (just_started == 1)
	fprintf(file,
		"<h3> %s</h3> \n",
		(*menP)->menu_name);
      else {
	fprintf(file,
		"</ul> \n<h3> %s</h3> \n",
		(*menP)->menu_name);
	just_started = 1;
      }
    }
  } /* for */

  fprintf(file,
	  "</ul> \n");

  if (fileLocal == NULL) {
    fprintf(file,
	    "</body>\n</html>\n");
    fclose(file);
  }

} /* Print_menu() */
  
static void Print_menus(char *filename)
{
  menu *men;
  FILE *file = NULL;

  men = toplevel_s;

  if (filename != NULL) {
    if ((file = fopen(filename, "w")) == NULL) {
      Message(1, "%s\n", strerror(errno));
      return;
    }

    fprintf(file, "<html>\n<head>\n  <title>\n  %s\n  </title>\n",
	    "Menu hierarchy for the XITE display program xshow");

    fprintf(file, "%s\n%s\n%s%s\n%s\n%s\n",
	    "  <meta name=\"author\" content=\"Svein Bøe\">",
	    "  <meta name=\"keywords\" content=\"XITE BIFF xshow\">",
	    "  <meta name=\"description\" content=\"",
	    " Menu hierarchy for the XITE display program xshow\">",
	    "  <link rel=\"up\" href=\"Contents.html\">",
	    "</head>");
    fprintf(file, "\n%s\n\n",
	    "<body background=\"../mona_canny.jpg\">");
  }

  while (men) {
    Print_menu(men, file);
    men = men->next;
  }

  if (filename != NULL) {
    fprintf(file,
	    "</body>\n</html>\n");
    fclose(file);
  }

} /* Print_menus() */

int main(int argc, char **argv)
{
  int dump = 0, prnt = 0;

  Iset_message(1);        /* Automatic error message display,  on */
  Iset_abort(0);          /* Automatic job abortion on error, off */
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <menufilename> [<html-filename>]\n"));

  if (argc == 1) Usage(2, NULL);
  if (argc > 3)  Usage(2, "Too many arguments.\n");

  if (!dump && !prnt) prnt = 1;

  Init_menu(argv[1]);

  if (argc > 2) Print_menus(argv[2]);
  else Print_menus(NULL);

  return(0);
}

#endif /* MAIN */
