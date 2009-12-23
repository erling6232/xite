
/*P:biffwrite*

________________________________________________________________

		biffwrite
________________________________________________________________

Name:		biffwrite - write text on a biff image
Syntax:         | biffwrite [<option>...] <input biff file> 
                |   <output biff file> <textfile>
                | biffwrite [<option>...] <input biff file> 
		|   <output biff file> -t "<text>"

Options:        &-band band-number
		What image band to write the text to. Default 1.

		&-pos xstart ystart
		The position of the text window in the image. Default: `1 1`,
		the upper left corner of the image.

		&-relx char
		Chooses if the x coordinate given by the '-pos' switch shall
		indicate the leftmost pixel of the text window (-relx l),
		the rightmost (-relx r), or the center pixel (-relx c).
		Default: -relx l.

		&-fb fg bg
		Set the color of the text. 'fg' determines foreground color,
		'bg' determines background color. Default (0 255).

		&-font bdf-file
		Set the character font. 'bdf-file' (Bitmap Distribution Fomat
		file) is the full filename of some .bdf font. Default:
		helvB12.bdf (Helvetica bold 12pt).

		&-nob
		Don''t write the background pixels of the text on the image.

		&-t text
		The 'text' to write on the BIFF image.

Description:    'biffwrite' reads from a textfile or from the text after the
		`-t` switch, and writes the characters on a biff image band.
		Only the ASCII numbers defined in the font will be written on
		the image. Other characters (such as `Tab`) will be ignored.
		The exception is the `newline` character which advances the
		writing point to the beginning of the next line.

		'textfile' is the name of the file where the text is
		contained. One must either give the name of a textfile or give
		the `-t` switch in a call.

		The same effects as the options give, can also be achieved
		by giving commands in the text file. If a line is started
		with a $, the program will look upon the rest of the line as
		a command. The following commands can be used:

		| $band <band_nr>
		| $pos <xstart> <ystart>
		| $relx <l/c/r>
		| $font <name of a Bitmap distribution font file>
		| $fb <foreground colour> <background colour> 
		| $nob 
		| $ob

		The first six commands have the same meaning and format
		as the corresponding options, and $ob are just the
		reverse of $nob. `$$` in the start of a line means that
		the first character to write on the picture is `$`.

Restrictions:	Only `Bitmap Distribution Format` font files can be used.
Author:		Øyvind Hillesund, NR, Copyright NR
Developed by:	BILD, Norsk Regnesentral, Oslo, Norway
Examples:       | biffwrite image.biff newimage.biff textfile.txt 

                | biffwrite image.biff newimage.biff textfile.txt
		|   -font /local/X11R5/mit/fonts/bdf/75dpi/courB10.bdf
		|   -pos 23 30 -band 3 -fb 255 0 -nob

		| zcat pant.2.biff.Z | biffwrite - - -t "HEADING" \
		|   -relx c |xshow

Id:		$Id$

________________________________________________________________

*/

static char *Id="$Id$";

#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_MKTEMP_H
#include <stdlib.h>
#include XITE_STRING_H
#include <xite/biff.h>
#include <xite/readarg.h>
#include <xite/draw.h>
#include <xite/message.h>

#define BDF_FOREGROUND 0           /* Must be the same as FOREGROUND and */
#define BDF_BACKGROUND 255         /* BACKGROUND in 'bdf2biff.c'         */
#define DEFAULTFONT        "/local/src/x11/mit/fonts/bdf/75dpi/helvB12.bdf"

static int band_nr, FOREGR, BACKGR, xstart, ystart, cxstart, cystart, noback;
static int cheight, cwidth;
static IMAGE img, chars;
static IBAND band;
static char relx, font_file[80];

static void readcommand(char line[])
{
  char command[20];
  int nr;
  sscanf(line,"%s",command);

  if (strcmp(command, "$fb") == 0) /* Foreground and background colour */
    { 
      if (sscanf(line, "%s %d %d", command, &FOREGR, &BACKGR) !=3)
	{
	  fprintf(stderr,"\nCouldn't read $fb from textfile\n");
	}
    }

  if (strcmp(command, "$pos") == 0)  /* position */
    {
      if (sscanf(line, "%s %d %d", command, &xstart, &ystart) !=3)
	{ 
	  fprintf(stderr,"\nCouldn't read $pos from textfile\n");
	}
    }

  if (strcmp(command, "$relx") == 0)
    {
      if (sscanf(line, "%s %c ", command, &relx) !=2)
	{ 
	  fprintf(stderr,"\nCouldn't read $relx from textfile\n");
	}
    }

  if (strcmp(command, "$nob") == 0)    /* Do not write the background pixels */
      noback = 1;


  if (strcmp(command, "$ob") == 0)    /* Do write the background pixels */
      noback = 0;

  if (strcmp(command, "$band") == 0)  /* Band number */
    {
      if (sscanf(line, "%s %d", command, &band_nr) != 2)
	{ 
	  fprintf(stderr,"Couldn't read $band from textfile");
	  exit(1);
	}
      if (band_nr <= Inbands(img))
	band = img[band_nr];
    }
  if (strcmp(command, "$font") == 0)  /* New font */
    {
      if (sscanf(line, "%s %s", command, font_file) != 2)
	{ 
	  fprintf(stderr,"Couldn't read $font from textfile");
	  exit(1);
	}
      Idel_image(&chars);
      chars = bdf2biff(font_file);   /* Making the biff font image */
      
      nr = 32;               /* Finding the size of the characters. */
      do 
	{
	  cheight = Iysize(chars[nr]);
	  cwidth = Ixsize(chars[nr]);
	  nr++;
	} while (cheight == 1 && cwidth == 1);    
      
    }
}

int main(int argc, char *argv[])
{
  char text_file[70], biff_file[70], newimage_file[70], buf[300];
  unsigned char c, text[300];
  FILE *textpt;
  int  nr, length = 0, len;
  int x, y, OOR;
  int fd = -1;
  
  OOR = 0;          /* 'Out Of Range' */
  
  Iset_message(TRUE);  /* warnings from blab-software */
  Iset_abort(TRUE);    /* but don't abort */

  InitMessage(&argc, argv, xite_app_std_usage_text(
  "Usage: %s [<options>] <input_biff_image> <output_biff_file> <textfile>\n\
or: \n\
       %s [<options>] <input_biff_image> <output_biff_file> -t \"<text>\"\n\
   where <options> can be:\n\
	[-band <band number> (1)]\n\
	[-pos <xstart> <ystart>  (1 1)]\n\
	[-relx |l| |c| |r|]  (left)\n\
	[-fb <foreground colour> <background colour>  (0 255)]\n\
	[-font <Bitmap Distribution Fomat file (.bdf)> (helvB12.bdf)]\n\
	[-nob]\n\n\
   The two BIFF files can be substituted by standard input and \n\
   standard output by writing `-` instead of a file name.\n\
   Either a textfile name or a `-t` switch must be given in the call.\n\
   Defaults for the switches are in paranthesis.\n\
   (See manpage for details)\n"));

  if (argc == 1) Usage(1, NULL);

  sscanf(read_switch(&argc, argv, "-band", 1, "1"),"%d", &band_nr); 
  sscanf(read_switch(&argc, argv, "-pos", 2,"1 1"),"%d %d", &xstart, &ystart);
  sscanf(read_switch(&argc, argv, "-font", 1, DEFAULTFONT),"%s", font_file);
  sscanf(read_switch(&argc, argv, "-fb", 2, "0 255"),"%d %d",&FOREGR,&BACKGR);
  noback = (read_switch(&argc, argv, "-nob", 0, NULL) != NULL);
  sscanf(read_switch(&argc, argv, "-relx", 1, "l"),"%c", &relx);
  if ((relx != 'l') && (relx != 'c') && (relx != 'r')) relx = 'l' ;

  sprintf((char *) text, "%s", read_switch(&argc, argv, "-t", 1, ""));

  if (strcmp((char *) text, "") != 0) {
    /*char *filename;*/

    if (argc != 3) Usage(1, "Illegal number of arguments.\n");
    /*
    filename = tempnam(NULL, NULL);
    textpt   = fopen(filename, "w");
     */
    fd = mkstemp("bwXXXXXX");
    textpt = fdopen(fd, "w+");
    fprintf(textpt,"%s\n",text);
    rewind(textpt);
  } else {
    if (argc != 4) Usage(1, "Illegal number of arguments.\n");
    sprintf(text_file,"%s",argv[3]);
    if ((textpt = fopen(text_file,"r")) == NULL) {
      Error(2, "Unable to open text file %s\n", text_file);
    }
  }
  
  sprintf(biff_file, "%s", argv[1]);
  if ((img = Iread_image(biff_file))== NULL) {
    Error(2, "Unable to open image %s.\n", biff_file);
  }  
  
  sprintf(newimage_file, "%s", argv[2]);

  if (band_nr > Inbands(img)) {
    Error(2, "Band number too large.\n");
  }
  band = img[band_nr];

  chars = bdf2biff(font_file);   /* Making the biff font image */
  nr = 32;               /* Finding the size of the characters. */
  do 
    {
      cheight = Iysize(chars[nr]);
      cwidth = Ixsize(chars[nr]);
      nr++;
    } while (cheight == 1 && cwidth == 1);    
  
  cxstart = xstart;
  cystart = ystart;
  while (fgets((char *) text, 300, textpt) != NULL) /*For each line in the text file */
    {
      if ( text[0] == '$' && text[1] != '$') /* if command */
	readcommand((char *) text);
      else
	{    
	  if (text[0] == '$' && text[1] == '$')       /* if `$$` */
	    for (nr = 0; nr < strlen((char *) text) ; nr++)
	      text[nr] = text[nr + 1];

	  /* find string length */
	  len = strlen((char *) text);
	  while (len && text[len-1] == '\n') text[--len] = 0;
          for (nr = 0, length=0; nr < len; nr++)
	    {
	      c = text[nr];
	      if (Ixmag(chars[c]) != 65535) 
		  length = length + Ixmag(chars[c]);
	    }
	  cystart = ystart;
	  if (relx == 'l')
	    cxstart = xstart;
	  if (relx == 'c')
	    cxstart = xstart - (length / 2);
	  if (relx == 'r')
	    cxstart = xstart - length;

	  if (noback == 0)      /* if write on background */
	    for (x = cxstart; x <= cxstart + length + 2; x++)
	      for (y = cystart; y < cystart + cheight; y++)
                if (x <= Ixsize(band)  &&  y <= Iysize(band) && x > 0 && y > 0)
		  band[y][x] = BACKGR;
	  for (nr = 0; nr < len; nr++)
	    {
	      c = text[nr];
	      if (Ixsize(chars[c]) != 1 || Iysize(chars[c]) != 1)
		{                  /* character defined in font */
		  for (x = cxstart; x < cxstart + cwidth; x++)
		    {
		      for (y = cystart; y < cystart + cheight; y++)
			{
			  if (x <= Ixsize(band)  &&  y <= Iysize(band)
			      && x > 0 && y > 0)
			    {
			      if (chars[c][y-cystart+1][x-cxstart+1] == 
				  BDF_FOREGROUND)
				band[y][x] = FOREGR;

			    }
			  else
			    {
			      OOR = 1;
			      break;
			    }
			}
		    }
		  cxstart = cxstart + Ixmag(chars[c]);
		}
	    }
	  ystart = ystart + cheight;
	}
    }             /* end of while loop */
  if (OOR == 1)
  fprintf(stderr,"\n%s: The text ran outside the image\n", argv[0]);
  rewind(textpt);
  Iappend_line(img," ");
  Iappend_line(img,"Written on image:");
  while (fgets(buf,300,textpt) != NULL)
    {  
      sprintf(buf,"%s\b",buf);
      Iappend_line(img,buf);
    }
  Iwrite_image(img,newimage_file);
  Idel_image(&chars);
  fclose(textpt);
  return 0;
}
