

/*

________________________________________________________________

        Xprog.c
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
#include <sys/wait.h>
#include <xite/biff.h>
#include <X11/Intrinsic.h>
#include "Xprog.h"
#include <X11/StringDefs.h>
#include <xite/message.h>
#include "Xbuild_menu.h"
#include "Xcontrol.h"
#include <xite/Xdialog.h>
#include "xshow.h"
#include XITE_STDIO_H
#include XITE_UNISTD_H
#include XITE_FILE_H
#include XITE_MALLOC_H
#include XITE_STRING_H
#include <xite/Xpty.h>
#include <xite/utils.h>
#include "Macro.h"
#include <xite/debug.h>
#include XITE_PARAM_H
#include <xite/strings.h>

#define NOT_EXPECTING 0
#define EXPECTING     1

extern Widget Control_e;
extern int    make_macro_e;
extern macroEntryPtr current_macro_e;
extern Boolean dump_commands_e;

/* The following variables keep track of whether the images expected by the
 * last started job/child have all been sent. A new job/child should not
 * start until all the images have been sent.

 * A list of all running jobs is maintained in Xpty.c.
 */
static int
  inimages_s, /* Number of input images for last/current job/child. */
  opipe_s,    /* Pipe for job/child input images. */
  button_state_s = NOT_EXPECTING, /* Last job/child still waiting for input. */
  current_job_s; /* Last job/child. */
static program *current_prog_s; /* Last/current job/child. */



/*F:Init_prog=XshowProgram*/
/*F:Prog_button=XshowProgram*/
/*F:ProgClose=XshowProgram*/
/*F:Prog_mouse=XshowProgram*/
/*F:ProgOpen=XshowProgram*/
/*F:Start_program=XshowProgram*/
/*F:XshowProgram*

________________________________________________________________

		XshowProgram
________________________________________________________________

Name:		XshowProgram, Init_prog, Prog_button, ProgClose, Prog_mouse,
                ProgOpen, Start_program - Run program from xshow menues.
Syntax:         | #include "Xprog.h"
                |
                | void Init_prog( char* filename,
                |    int envir_also );
                |
                | int Prog_button( Widget wid, IMAGE img,
                |    IBAND band, int button, int xo, int yo,
                |    int xw, int yw );
                |
                | int ProgOpen( program* prog );
		|
                | int ProgClose( program* prog );
		|
                | void Prog_mouse( Widget wid, XtPointer c_data,
                |    XtPointer call_data);
		|
                | void Start_program( char* client_data );
		|
Description:	'Init_prog' assumes that 'filename' is the name of a menu
                file. This is read. If 'envir_also' is nonzero, also
		.xshowrc and xshow_menues in the directories given by
		the environment variable $XSHOWPATH are read.

                'Prog_button' responds to a mouse button click in the image
		widget. Left button selects a subband, middle button selects
		the band in the widget and right button selects the whole
		image. The selected band/image is written to a pipe for
                a subsequent read by another function.

		'ProgOpen' prints a start-message and displays "JC" in the
		mouse button widget.

		'ProgClose' prints a finished-message and displays an "N" in
		the mouse button widget.

		'Prog_mouse' prints an abort-message and displays an "N" in
		the mouse button widget.

		'Start_program' interprets the xshow menu file, opens the
		necessary dialogs, retrieves user input and starts a
		program. If a previous program is still running, then
		print a message in the error message part of the control
		window, and do not start a new program.

Author:		Otto Milvang
Revised:        Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

static int String_is_duplicate(char *string, char *strings)
{
  char *next_string;
  
  /* Check if 'string' is also in the list 'strings' (don't want it to
   * appear more than once). */
  
  while (strings && *strings) {
    NextFilename(&strings, &next_string);
    if (next_string && strcmp(next_string, string) == 0) {
      string = NULL;
      break;
    }
    if (next_string) {
      free(next_string); next_string = NULL;
    }
  } /* while (strings) */

  if (next_string) {
    free(next_string); next_string = NULL;
  }

  if (string) return(0); /* Not duplicate. */
  else return(1);

} /* String_is_duplicate() */

void Init_prog(char *filename, int envir_also)
{
  char *flist = NULL, *frc = NULL, *fmen = NULL, *fname = NULL;

  ENTER_FUNCTION_DEBUG("Xprog.c: Init_prog");

  if (envir_also) {
    frc  = envfindfilemulti("XSHOWPATH", ".xshowrc");
    fmen = envfindfilemulti("XSHOWPATH", "xshow_menues");

    if (!frc && !fmen)
      Warning(1, "%s%s\n",
	      "No menufile found in XSHOWPATH. ",
	      "Check environment variable XSHOWPATH.");

    if ((flist = frc) != NULL && filename != NULL) {
      if (String_is_duplicate(filename, flist)) filename = NULL;
    }
    if ((flist = fmen) != NULL && filename != NULL) {
      if (String_is_duplicate(filename, flist)) filename = NULL;
    }
  }

  fname = String_cat(frc, fmen, filename);
  Init_menu(fname, "toolsmenu", (Widget) Control_e);

  if (fname) {free(fname); fname = NULL;}
  if (frc)   {free(frc);   frc   = NULL;}
  if (fmen)  {free(fmen);  fmen  = NULL;}

  LEAVE_FUNCTION_DEBUG("Xprog.c: Init_prog");
}



int ProgOpen(program *prog)
{
  ENTER_FUNCTION_DEBUG("Xprog.c: ProgOpen");

  Message(0, "Job %5d started  - %s\n", prog->jobno, prog->jobname);
  Message_jobs(1);

  if (prog->inimages) {
    button_state_s = EXPECTING;
    Message_mouse("JC");
    inimages_s = prog->inimages; /* # images expected by child program. */
    opipe_s    = prog->ch_imgin; /* Pipe for child image input. */
    current_prog_s = prog;
  }
  current_job_s = prog->jobno;
  
  LEAVE_FUNCTION_DEBUG("Xprog.c: ProgOpen");
  return(0);
}

int ProgClose(program *prog)
{
  ENTER_FUNCTION_DEBUG("Xprog.c: ProgClose");

  if (prog->terminated == 1) {
    if (prog->jobno) {
      Message_jobs(-1);
      Message(0, "Job %5d finished - %s\n",prog->jobno, prog->jobname);
      if (prog->outimages > 0) {
      }
      while(wait3( (int*)0, WNOHANG, NULL) > 0);
    }
    if (prog->jobno == current_job_s) {
      button_state_s = NOT_EXPECTING;
      Message_mouse(" N");   
    }
  }

  LEAVE_FUNCTION_DEBUG("Xprog.c: ProgClose");
  return(0);
}

static int  ReadImage(program *prog)
{
  int status, mess;

  ENTER_FUNCTION_DEBUG("Xprog.c: ReadImage");
  FPRINTF2("    prog->ch_imgout: %d\n",
	   prog->ch_imgout);

  mess = Iset_message(False);

  status = Read_ipipe(prog->ch_imgout, NULL, prog);

  if (status == 1) prog->num_colortabs_read++;
  if (status == 2) prog->num_ovltabs_read++;
  if (status == 3) prog->num_overlays_read++;
  if (status >= 4) prog->num_images_read += status - 3;

  Iset_message(mess);

  LEAVE_FUNCTION_DEBUG("Xprog.c: ReadImage");
  
  return(status);
}

static char *parseCode(char *code)
{
  if (!strncmp(code,XTERMOUT,strlen(XTERMOUT)))         return(XTERMOUT);
  if (!strncmp(code,XSHOWINFILE,strlen(XSHOWINFILE)))   return(XSHOWINFILE);
  if (!strncmp(code,XSHOWOUTFILE,strlen(XSHOWOUTFILE))) return(XSHOWOUTFILE);
  if (!strncmp(code,readBIFF,strlen(readBIFF)))         return(readBIFF);
  if (!strncmp(code,readColortab,strlen(readColortab))) return(readColortab);
  if (!strncmp(code,readColormap,strlen(readColormap))) return(readColormap);
  if (!strncmp(code,readTIFF,strlen(readTIFF)))         return(readTIFF);
  if (!strncmp(code,readMAT,strlen(readMAT)))           return(readMAT);
  if (!strncmp(code,readPS,strlen(readPS)))             return(readPS);
  if (!strncmp(code,readfile,strlen(readfile)))         return(readfile);
  if (!strncmp(code,writeBIFF,strlen(writeBIFF)))       return(writeBIFF);
  if (!strncmp(code,writeColortab,strlen(writeColortab)))
    return(writeColortab);
  if (!strncmp(code,writeColormap,strlen(writeColormap)))
    return(writeColormap);
  if (!strncmp(code,writeTIFF,strlen(writeTIFF)))       return(writeTIFF);
  if (!strncmp(code,writeMAT,strlen(writeMAT)))         return(writeMAT);
  if (!strncmp(code,writePS,strlen(writePS)))           return(writePS);
  if (!strncmp(code,writefile,strlen(writefile)))       return(writefile);
  if (!strncmp(code,DIALOG,strlen(DIALOG)))             return(DIALOG);

  return(NULL);

} /* parseCode() */

static void filterDialogReturn(char *return_text, char **out, char *start)
{
  /* Filter out everything between matching pairs of '#'.
   * This is only necessary for old-style dialogs, those which CallDialog()
   * processed with SimpleDialog(), not FormDialog(). */

  int cpy;
  char *output;

  output = *out;
  cpy = 2;
  while(*return_text) {
    /* Skip all text between the first and second '#' (while cpy < 0). */
    if (*return_text == '#') {
      cpy = -cpy;
      return_text++;
    } else { 
      if (cpy > 0 && (output - start) < 1024) {
	if (isspace(*return_text)) *return_text=' '; else cpy = 1;
	if (cpy == 1) *output++ = *return_text;
	if (*return_text == ' ') cpy++;
      }
      return_text++;
    }
  } /* while */

  *out = output;
  return;

} /* filterDialogReturn() */

static int processDialog(char **in, char **out, char *start)
{
  char *input, *dialog_name, *return_text, *command_name;
  int status;

  input = *in;

  /* Find name of dialog. */
  dialog_name = nextWord(input);
  input += strlen(dialog_name);

  command_name = nextWord(start);

  status = CallDialog(dialog_name, command_name, &return_text);
  FPRINTF3("    Start prog, status %d, name %s\n", status, dialog_name);
  if (status == -1) {
    Warning(1,"%s: can't find dialog.\n", dialog_name);
    *in = input;
    return(status);
  }
  if (status == 0) {
    /* Aborted. */

    if (command_name != NULL)
      Message(0, "%s: aborted.\n", command_name);

    Remove_current_entry();
    *in = input;
    return(status);
  }

  filterDialogReturn(return_text, out, start);

  *in = input;
  return(status);

} /* processDialog() */

static int processCode(char **in, char **out, char *start)
{
  /* Start of a menu-file code. */

  char *code, *input, *output;

  input  = *in;
  output = *out;
  code   = parseCode(input);

  if (code == NULL) {
    Error(2, "Error in menu file!!\n");
    *output++ = *input++;
  } else {
    char *return_text;
    int len, status;

    if (!strcmp(code, DIALOG)) {
      return_text = XtNewString(code);

    } else if (code[0] == '<') {
      return_text = XtNewString(code);

    } else if (dump_commands_e) {
      return_text = XtNewString(code);
    }
    else if (!strncmp((code+1), "read", strlen("read"))) {
      /* Get input filename. */

      if ((status = SelectReadFile(Control_e, code, &return_text)) == 0) {
	*in  = input;
	*out = output;
	return(status);
      }
    } else if (!strncmp((code+1), "write", strlen("write"))) {

      if ((status = SelectWriteFile(Control_e, code, &return_text)) == 0) {
	*in  = input;
	*out = output;
	return(status);
      }
    }

    len   = strlen(return_text);
    input = &input[strlen(code)];

    if (!strcmp(code, DIALOG)) {
      status = processDialog(&input, &output, start);
      if (status != 1) return(status);
    } else {
      if (output - start + len < 1024) strcpy(output, return_text);
      output = &output[len];
    }
  } /* code != NULL */

  *in  = input;
  *out = output;

  return(1);

} /* processCode() */

static void substEnv(char **vec, int vnr, macroItemPtr *macro_item)
{
  int i;
  char *env, *tmp;
  macroItemPtr m_item;

  m_item = *macro_item;

  for (i=0; i<vnr; i++) {

    if (strncmp(vec[i],"$XSHOWPATH/",11) == 0) {
      env = (char *) envfindfile("XSHOWPATH", &vec[i][11]);
      if (env == NULL) continue;
      tmp = (char *) malloc(strlen(env)+1);
      if (!tmp) {
	Warning(1,"Unable to allocate memory for execv\n");
	return;
      }
      strcpy(tmp, env);
      free(env); env = NULL;
      vec[i] = tmp;
      if (make_macro_e) {
	XtFree((char *) m_item->item);
	m_item->item = XtNewString(tmp);
      }
    }

    if (make_macro_e) m_item = m_item->next_item;

  } /* for i */

} /* substEnv() */

void Start_program(char *client_data)
{
  char *qmark, data[1024], *entry, *tmp;
  char *vec[132];
  int vnr = 0;
  macroItemPtr macro_item;
  macroEntryPtr macro_entry;

  ENTER_FUNCTION_DEBUG("Xprog.c: Start_program");
  FPRINTF2("    client_data: %s\n", client_data);

  if (button_state_s == EXPECTING) {
    Warning(1, "%s\n%s\n",
	    "Some other program is still active or waiting for input.",
	    "Try again when this has finished.");
    LEAVE_FUNCTION_DEBUG("Xprog.c: Start_program");
    return;
  }

  qmark = client_data; /* Move until question mark found. */
  tmp = data;          /* Move forward while copying characters. */

  if (make_macro_e) {
    InitMacroEntry(&macro_entry);
    Add_macro_entry(macro_entry);
  }

  /* Search for and process special codes used in menu-files. */

  while (tmp - data < 1024 && *qmark != 0) {
    if (*qmark != '?' && *qmark != '<') {
      /* Not the start of a menu-file code.
       * Copy character to final command. */

      *tmp++ = *qmark++;
    } else
    if (processCode(&qmark, &tmp, data) != 1) return;

  } /* while */

  *tmp = '\0';

  /* "data" is the original entry with the results of ?read-, ?write- and
   * ?dialog-codes inserted.
   */

  entry = (char *) malloc(strlen(data)+1);
  if (!entry) {
    Warning(1, "Unable to allocate memory for execv\n");
    return;
  }
  strcpy(entry, data);
  FPRINTF2("    entry: %s\n", entry);
  
  /* Let each entry of 'vec' point to a word (space-delimited). */
  split_cmd_line(data, vec);

  if (make_macro_e) {
    int i = 0;

    while (vec[i]) {
      InitMacroItem(&macro_item);
      macro_item->item = XtNewString(vec[i]);

      if (!strncmp(macro_item->item, XSHOWINFILE, strlen(XSHOWINFILE))) {
	macro_item->type      = 1;
      } else if (!strncmp(macro_item->item, XSHOWOUTFILE,
			  strlen(XSHOWOUTFILE))) {
	macro_item->type      = 2;
      } else {
	macro_item->type      = 0;
      }
      
      macro_item->next_item = NULL;
      Add_macro_item(macro_entry, macro_item);

      i++;

    } /* while vec[i] */

    macro_entry->entry_name = XtNewString(vec[0]);
  }

  /* Allow the use of the environment variable $XSHOWPATH when referring
   * to filenames. Substitute with the value of $XSHOWPATH.
   */

  if (make_macro_e) macro_item = macro_entry->first_item;

  substEnv(vec, vnr, &macro_item);

  LEAVE_FUNCTION_DEBUG("Xprog.c: Start_program");

  if (!dump_commands_e) {
    if (XptyStartProgram(entry, vec, 0,  ProgOpen, ProgClose,
			 NULL, NULL, ReadImage))
      Warning(1, "Job failed - %s\n", entry);
  } else {
    int i = 0;

    while (vec[i] != NULL) {
      /* Check for more than one space-separated word, and quote if true. */
      tmp = strchr(vec[i], ' ');

      if (tmp) fprintf(stderr, "\\\"");
      fprintf(stderr, "%s", vec[i++]);
      if (tmp) fprintf(stderr, "\\\"");
      if (vec[i] != NULL) fprintf(stderr, " ");
    }
    fprintf(stderr, "\n");
  }

} /* Start_program() */



int Prog_button(Widget wid, IMAGE img, IBAND band, int button, int xo, int yo, int xw, int yw)
{
  char name[6];
  int x, y;
  IMAGE img2;
  IBAND tmp;

  if (button_state_s == NOT_EXPECTING) return(NOT_EXPECTING);

  ENTER_FUNCTION_DEBUG("Xprog.c: Prog_button");
  FPRINTF2("    Button sendimg %d\n",inimages_s);

  sprintf(name,"-%d", opipe_s);
  switch(button)
    {
    case 1:
      x  = xo + xw - 1;
      y  = yo + yw - 1;
      FPRINTF3("    xo=%d, x=%d, ",xo ,x); 
      FPRINTF3("    yo=%d, y=%d\n ",yo,y); 
      if ( (xo-x) * (xo-x) < 1 && (yo-y) * (yo-y) < 1) break;
      img2 = Init_image(1,Ititle(img));
      Icopy_text(img, img2);
      tmp = Imake_subband(band,
       x-xo+1 > 0 ? xo : x, y-yo+1 > 0 ? yo : y, 
         x-xo+1 > 0 ? x-xo+1 : xo-x+1, y-yo+1 > 0 ? y-yo+1 : yo-y+1); 
      /* img2[1] = Imake_band(Ipixtyp(tmp), Ixsize(tmp), Iysize(tmp));
      Iset_start(img2[1], Ixstart(tmp), Iystart(tmp));
      Icopy_band(tmp, img2[1]); */
      img2[1] = tmp;
      Iwrite_image(img2, name);
      inimages_s--;
      break;
    case 2:
      img2 = Init_image(1,Ititle(img));
      Icopy_text(img, img2);
      img2[1] = band;
      Iwrite_image(img2, name);
      inimages_s--;
      break;
    case 3:
      Iwrite_image(img, name);
      inimages_s--;
      break;
    }

  Add_macro_widget(current_macro_e, wid);

  if (inimages_s == 0) {
      button_state_s = NOT_EXPECTING;
      Message_mouse(" N");
      close(opipe_s); /* Pipe for child program image input. */
    }

  LEAVE_FUNCTION_DEBUG("Xprog.c: Prog_button");

  return(EXPECTING);
}



extern void Prog_mouse(Widget wid, XtPointer c_data, XtPointer call_data)
{
  ENTER_FUNCTION_DEBUG("Xprog.c: Prog_mouse");

  button_state_s = NOT_EXPECTING;
  Message_mouse(" N");
  XptyKill(current_prog_s);
  Remove_current_entry();
  Message_jobs(-1);

  Message(0, "Job %5d aborted  - %s\n", current_prog_s->jobno, 
	  current_prog_s->jobname);
  while(wait3( (int*)0, WNOHANG, NULL) > 0);

  LEAVE_FUNCTION_DEBUG("Xprog.c: Prog_mouse");
}
