

/*

________________________________________________________________

        biff2ps.c
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

static char *Id = "$Id$, Blab, UiO";



/*P:biff2ps*

________________________________________________________________

		biff2ps
________________________________________________________________

Name:		biff2ps - convert BIFF image to PostScript

Syntax:		| biff2ps [<option>...] <inputfile> <outputfile>
             
Filenames:	Use "-" or "-0" for 'inputfile' to get stdin. Use "-" or "-1"
                for 'outputfile' to get stdout. Options '-i' and '-o' take
		precedence over 'inputfile' and 'outputfile' respectively.

Description:	Convert an image to PostScript code.
                The PostScript file must be printed on a PostScript
                printer. Color printers need color images. In this case,
		give a three band input image, and use the '-rgb' option. 

		The resulting code can be used to include images in e. g.
		a 'LaTeX' document with the filter 'dvips'.

		Default size (-m 1.0) is 72 pixels per inch if image is
		"small". If image is "large", then fill the page vertically
		or horizontally. Default position is to center image
		vertically and horizontally.

Options:	Some of the options accept text which is literal PostScript
                code. Certain predefined PostScript macros can be used in
		this case. Which options accept this, is mentioned in the
		description of each option.

		&Predefined PostScript macros
		1 pt equals 1/72 inch.

		&&xsize
		Width of image in pt.

		&&ysixe
		Height of image in pt.

		&&xboxsize
		Width of box which contains the image (or all the images) on
		the page, in pt.

		&&yboxsize
		Height of box which contains the image (or all the images) on
		the page, in pt.

		&&hfontsize
		Header font size.

		&&ffontsize
		Footnote font size.

		&&xcenter
		Center of text.
                
		&The options are

                &&-c int
                Number of gray levels (2, 4, 8, 16 or 256), default: 256.

		&&-m float
		Magnify image with factor 'float'.

                &&-w float
		Set image width to 'float' cm.

                &&-h float 
		Set image height to 'float' cm.

                &&-x float
		Set left marg  to 'float' cm.

		&&-y float
		Set bottom marg to 'float' cm.

		&&-i filename
		Set input 'filename'. Overrides 'inputfile'.

                &&-o filename
		Set output 'filename'. Overrides 'outputfile'.

		&&-q pos

		Print image in cell given by 'pos'.

		Imagine that the paper is a matrix consisting of one or more
		columns and one or more rows. 'pos' determines the matrix
		dimensions and in which cell (matrix element) the image will
		be printed.

		'pos' must be an 8-character string consisting of 4 parts,
		each part consisting of 2 characters. The first character (of
		the two in a part) must be either h (for height or vertical
		matrix dimension), w (for width or horizontal matrix
		dimension), x (for horizontal cell number or matrix
		coordinate) or y (for vertical cell number or matrix
		coordinate). The second character (of the two in a part) must
		be a single digit. Coordinates start with zero.

		The default value of 'pos' is "h1w1x0y0". This means that the
		paper is considered as a single cell and that the image will
		be printed in this cell.

		With 'pos' equal to "h1w2x1y0", the paper is considered to
		consist of two horizontally adjacent cells. The image will be
		printed in the rightmost cell.

		Several images can be printed on the same page, each image
		positioned relative to the others with this option. See
		the section 'Examples' below.

		&&-p
		Don''t print prolog.

		&&-t
		Don''t print trailer.

		&&-ht text
		Set header to 'text'. It is printed with the PostScript code
		(<ht>) <hx> <hy> moveto show.

		&&-hs text
		Set header font-size. Accepts predefined PostScript macros or
		numerical value (number of points (1/72 inch)). Default: 25.

		&&-hy text
		Set vertical position for header. Accepts predefined
		PostScript macros or numerical value (number of points (1/72
		inch)). Default: "ysize 10 add".

		&&-hx text
		Set horizontal position for header. Accepts predefined
		PostScript macros or numerical value (number of points (1/72
		inch)). Default: "xcenter".

		&&-ft text
		Set footnote to 'text'. It is printed with the PostScript code
		(<ft>) <fx> <fy> moveto show.

		&&-fs text
		Set footnote font-size. Accepts predefined PostScript macros or
		numerical value (number of points (1/72 inch)). Default: 15.

		&&-fy text
		Set vertical position for footnote. Accepts predefined
		PostScript macros or numerical value (number of points (1/72
		inch)). Default: "-10 ffontsize sub".

		&&-fx text
		Set horizontal position for footnote. Accepts predefined
		PostScript macros or numerical value (number of points (1/72
		inch)). Default: "xcenter".

		&&-bg int
		Set background pixel value to 'int'. This colors the whole
		page except for the image itself. Note that it does not
		change pixels in the image itself and has nothing to do with
		what is considered as foreground/object and background in
		the BIFF image.

		This option is of limited value and should NOT be used
		if the PostScript code is to be included in another document.

		&&-rgb
		Interpret three bands as rgb.

		&&-copies n
		Print 'n' copies.

See also:	pseudo2rgb(1)

Restrictions:   Most black-and-white printers will not accept color
                PostScript code.

Author:		Otto Milvang, Tor Lønnestad and Svein Bøe, BLAB, Ifi, UiO

Examples:       &black-and-white image with default size and position
		| biff2ps mona.img mona.ps
		&black-and-white image with specified size (in cm)
		| biff2ps -w 5.0 -h 7.0 mona.img mona.ps
		&black-and-white image with half of default size
		biff2ps -m 0.5 mona.img mona.ps
		&Color coding with default size and position
		biff2ps -rgb monargb.img monargb.ps

		-&Set up 4 images on one page with 1 title and 4 subtitles
		| (biff2ps img1 - -m 0.5 -q w2h2x0y0 -ft "Pos 0 0" -t;\\
		| biff2ps img2 - -m 0.5 -q w2h2x1y0 -ft "Pos 1 0" -p -t;\\
		| biff2ps img3 - -m 0.5 -q w2h2x1y1 -ft "Pos 1 1" -p -t;\\
		| biff2ps img4 - -m 0.5 -q w2h2x0y1 -ft "Pos 0 1" -p \\
		|         -ht "IMAGE" -hx "xcenter xboxsize 2 div add" \\
		|         -hy "ysize 25 add") > img.ps

		The above command is legal in the UNIX shells csh and tcsh.
		The ";" means that the commands are executed sequentially. The
		enclosing parentheses mean that the four commands are
		considered as one. The ">" means to send the output from the
		complete command to a file. The options '-t' and '-p' are used
		to ensure that the PostScript prolog and trailer each occurs
		only once in the resulting output.
		
Id: 		$Id$
________________________________________________________________

*/

#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_TIME_H
#include XITE_UNISTD_H

#define	INCH		* 72
#define	PG_SIZE_X	( 595 )	/* Canvas is 7.5" x 9.0" area on a
					 * 8.2" x 12.7" paper */
#define	PG_SIZE_Y	( 842 )
#define MAXBOX_X	( 7.5 INCH )
#define MAXBOX_Y	( 9.0 INCH )
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef char   tstring[132];

static FILE    *outfile;
static char    hex[17]  = "0123456789ABCDEF";
static int     flag_c   = 256;
static int     flag_b   = 1; /*will be removed*/
static float   flag_m   = 1.0;
static float   flag_w   = 0.0;
static float   flag_h   = 0.0;
static float   flag_x   = -1.0;
static float   flag_y   = -1.0;
static int     flag_p   = 0;
static int     flag_t   = 0;
static char *  flag_q   = NULL;
static int     flag_qh  = 1;
static int     flag_qw  = 1;
static int     flag_qx  = 0;
static int     flag_qy  = 0;
static int     flag_bg  = -1;
static int     flag_rgb = 0;
static int     flag_cp  = 1;
static char *  flag_ht  = NULL;
static char *  flag_ft  = NULL;
static char *  flag_hs  = NULL;
static char *  flag_fs  = NULL;
static char *  flag_hx  = NULL;
static char *  flag_hy  = NULL;
static char *  flag_fx  = NULL;
static char *  flag_fy  = NULL;
static tstring flag_i   = "";
static tstring flag_o   = "";

static void print_bg(void);



/*L*

________________________________________________________________

		print_header
________________________________________________________________

Name:		print_header
Syntax:		| static void print_header(title, width, height, depth)
                | char *title
                | int width, height, depth;
Description:    Print postscript Adobe header
Return value:
Author:		Otto Milvang
Id:             $Id$
________________________________________________________________

*/


static void print_header(char *title, int width, int height, int depth)
{
  time_t now;
  int xoff, yoff, xscale, yscale, scale, maxx, maxy;
  maxx = MAXBOX_X/flag_qw;
  maxy = MAXBOX_Y/flag_qh;

  xscale = flag_w > 0.0 ? flag_w * 72.0 / 2.54 : width  * flag_m;
  yscale = flag_h > 0.0 ? flag_h * 72.0 / 2.54 : height * flag_m;
  scale = ((xscale > maxx) OR (yscale > maxy));
  if (scale) {
    if (maxx/xscale < maxy/yscale) {
      yscale = (int) ( (double) maxx/xscale*yscale);
      xscale = maxx;
    } else {
      xscale = (int) ( (double) maxy/yscale*xscale);
      yscale = maxy;
    }
  }
  xoff = flag_x > 0.0 ? flag_x * 72.0 / 2.54 : 
    (PG_SIZE_X - MAXBOX_X)/2 + maxx * flag_qx + (maxx - xscale) / 2;  
  yoff = flag_y > 0.0 ? flag_y * 72.0 / 2.54 : 
    (PG_SIZE_Y - MAXBOX_Y)/2 + maxy * flag_qy + (maxy - yscale) / 2;
  
  time(&now);
  if (NOT flag_p) {
    fprintf(outfile, "%%!PS-Adobe-2.0\n");
    fprintf(outfile, "%%%%DocumentFonts: Courier\n");
    fprintf(outfile, "%%%%Title: %s\n", 
       title ? (strlen(title) ? title : "biffimage") : "biffimage" );
    fprintf(outfile, "%%%%Creator: biff2ps\n");
    fprintf(outfile, "%%%%CreationDate: %s\n", ctime(&now));
    fprintf(outfile, "%%%%For: Postscript Printer\n");
    fprintf(outfile, "%%%%Pages: 1\n");
    fprintf(outfile, "%%%%BoundingBox: %d %d %d %d\n",
	  xoff, yoff + (flag_ft ?  -25 : 0), 
	  xoff+xscale, yoff + yscale + (flag_ht ? 35 : 0));
    fprintf(outfile, "%%%%EndComments\n\n");

    fprintf(outfile, "%%%%BeginProlog\n");
    fprintf(outfile, "%%%%IncludeResource: encoding ISOLatin1Encoding\n");
    fprintf(outfile, "%%%%BeginResource: procset newencode 1.0 0\n");
    fprintf(outfile, "/NE { \n");
    fprintf(outfile, "   findfont begin\n");
    fprintf(outfile, "      currentdict dup length dict begin\n");
    fprintf(outfile, "	  { %%forall\n");
    fprintf(outfile, "            1 index/FID ne {def} {pop pop} ifelse\n");
    fprintf(outfile, "	    } forall\n");
    fprintf(outfile, "         /FontName exch def\n");
    fprintf(outfile, "         /Encoding exch def\n");
    fprintf(outfile, "         currentdict dup\n");
    fprintf(outfile, "      end\n");
    fprintf(outfile, "   end\n");
    fprintf(outfile, "   /FontName get exch definefont pop\n");
    fprintf(outfile, "   } bind def\n");
    fprintf(outfile, "%%%%EndResource\n");
    fprintf(outfile, "ISOLatin1Encoding /Times-Roman-ISO/Times-Roman NE\n");


/*  fprintf(outfile, "initgraphics\n"); DVIPS DOESN'T LIKE THIS */
    fprintf(outfile, "save\n");
    fprintf(outfile, "%%%%EndProlog\n");
    fprintf(outfile, "%%%%Page: 1 1\n");
  }
  fprintf(outfile, "gsave\n");
  if (NOT flag_rgb)
    fprintf(outfile, "/picstr %d string def\n",width);
  else {
    fprintf(outfile, "/rstr %d string def\n",width);
    fprintf(outfile, "/gstr %d string def\n",width);
    fprintf(outfile, "/bstr %d string def\n",width);
  }
  fprintf(outfile, "/xsize %d def\n",xscale);
  fprintf(outfile, "/ysize %d def\n",yscale);
  fprintf(outfile, "/xboxsize %d def\n",maxx);
  fprintf(outfile, "/yboxsize %d def\n",maxy);
  fprintf(outfile, "/hfontsize %s def\n", flag_hs);
  fprintf(outfile, "/ffontsize %s def\n", flag_fs);
  fprintf(outfile,
	  "/xcenter {dup stringwidth pop xsize exch sub 2 div} def\n");

  print_bg();

  fprintf(outfile, " %d %d translate\n newpath\n", xoff, yoff);

  if (flag_ht) {
    fprintf(outfile, "newpath 0 0\n moveto\n");
    fprintf(outfile, "/Times-Roman-ISO findfont %s scalefont setfont\n",
	    flag_hs);
    fprintf(outfile, "(%s)\n %s\n %s\n rmoveto\n show\n newpath\n\n", 
	    flag_ht, flag_hx, flag_hy);
  }

  if (flag_ft) {
    fprintf(outfile, "newpath 0 0\n moveto\n");
    fprintf(outfile, "/Times-Roman-ISO findfont %s scalefont setfont\n",
	    flag_fs);
    fprintf(outfile, "(%s)\n %s\n %s\n rmoveto\n show\n newpath\n\n", 
	    flag_ft, flag_fx, flag_fy);
  }

  fprintf(outfile, " 0 0 moveto \n %d %d scale\n\n", xscale, yscale);
  if (NOT flag_rgb) {
    if (depth < 4)
      fprintf(outfile, "%d %d %d \n", width, height, 4);
    else
      fprintf(outfile, "%d %d %d \n", width, height, depth);
    fprintf(outfile, "[ %d 0 0 -%d 0 %d ]\n", width, height, height);
    fprintf(outfile, "{currentfile picstr readhexstring pop} image\n");
  } else {
    fprintf(outfile, "%d %d 8 \n", width, height);
    fprintf(outfile, "[ %d 0 0 -%d 0 %d ]\n", width, height, height);
    fprintf(outfile, "{currentfile rstr readhexstring pop}\n");
    fprintf(outfile, "{currentfile gstr readhexstring pop}\n");
    fprintf(outfile, "{currentfile bstr readhexstring pop}\n");
    fprintf(outfile, "true 3\n");
    fprintf(outfile, "colorimage\n");
  }
}



/*L*

________________________________________________________________

		print_bg
________________________________________________________________

Name:		print_bg
Syntax:		| static void print_bg()
Description:    if (flag_bg in 0..255) print background 
                else if (flag_bg EQ -1) don't print background
		else error
Return value:   None
Author:		Tor Lønnestad, BLAB, Ifi, UiO
Id: 	        $Id$
________________________________________________________________

*/


static void print_bg(void)
{
  tstring t;

  if (flag_bg != -1) {
    if ((flag_bg < 0) OR (flag_bg > 255)) {
      sprintf(t, "%d", flag_bg);
      Usage(1, "-bg = %s, unknown value.\n", t);
    } else {
      fprintf(outfile, "newpath\n");
      fprintf(outfile, "  0 0 moveto\n");
      fprintf(outfile, "  0 %d rlineto\n", (int)PG_SIZE_Y);
      fprintf(outfile, "  %d 0 rlineto\n", (int)PG_SIZE_X);
      fprintf(outfile, "  0 %d rlineto\n", (int)-PG_SIZE_Y);
      fprintf(outfile, "closepath\n");
      fprintf(outfile, "%.2f setgray\n", flag_bg/255.0);
      fprintf(outfile, "fill\n\n");
    }
  }
}





/*L*

________________________________________________________________

		print_data
________________________________________________________________

Name:		print_data
Syntax:		| static void print_data(iband, depth)
                | IBAND iband;
		| int depth;
Description:    Print data file
Return value: 
Author:		Otto Milvang
Id: 	        $Id$
________________________________________________________________

*/



static void print_data(IBAND iband, int depth)
{ 
  int xsize, ysize, size, val;
  unsigned char *ptr;
  tstring t;
  xsize = Ixsize(iband);
  ysize = Iysize(iband);
  size = xsize * ysize;
  ptr = &iband[1][1];
  switch(depth)
  { case 1:
        while(size--)
 	{
	  val = *ptr++;
	  /* Divide by 128 ==> [0,1], mult. by 15 ==> [0,15] */
	  fprintf(outfile,"%c",hex[15 * ((int) (val >> 7))]);
	  if ((size & 15) EQ 0)
	    fprintf(outfile,"\n");
	}
        break;
    case 2:
        while(size--)
 	{
	  val = *ptr++;
	  /* Divide by 64 ==> [0,1,2,3], mult. by 5 ==> [0,5,10,15] */
	  fprintf(outfile,"%c",hex[5 * ((int) (val >> 6))]);
	  if ((size & 15) EQ 0)
	    fprintf(outfile,"\n");
	}
        break;
    case 3:
        while(size--)
 	{
	  val = *ptr++;
	  /* Divide by 32 ==> [0,..,7], mult. by 2 ==> [0,2,4,..,12,14] */
	  fprintf(outfile,"%c",hex[2 * ((int) (val >> 5))]);
	  if ((size & 15) EQ 0)
	    fprintf(outfile,"\n");
	}
        break;
    case 4:
        while(size--)
 	{
	  val = *ptr++;
	  fprintf(outfile,"%c",hex[val >> 4]);
	  if ((size & 15) EQ 0)
	    fprintf(outfile,"\n");
	}
        break;
    case 8:
        while(size--) 
	{
 	  val = *ptr++;
	  fprintf(outfile,"%c%c",hex[val >> 4], hex[val & 15]);
	  if ((size & 15) EQ 0) fprintf(outfile,"\n");
	}
        break;
   default:
     sprintf(t, "%d", flag_c);
     Usage(1, "-c = %s, unknown value.\n", t);
     break;
} }




/*L*

________________________________________________________________

		print_rgb_data
________________________________________________________________

Name:		print_rgb_data
Syntax:		| static void print_rgb_data(img, depth)
                | IMAGE img;
		| int depth;
Description:    Print rgb data file
Return value: 
Author:		Tor Lønnestad
Id: 	        $Id$
________________________________________________________________

*/



static void print_rgb_data(IMAGE img, int depth)
{ 
  int xsize, ysize, x, y, val, color, size=16;
  tstring t;
  xsize = MIN(MIN(Ixsize(img[1]), Ixsize(img[2])), Ixsize(img[3]));
  ysize = MIN(MIN(Iysize(img[1]), Iysize(img[2])), Iysize(img[3]));
  switch (depth)
  { case 1:
      FOR (y=1; y LE ysize; y++)
        FOR (color=1; color LE 3; color++)
          FOR (x=1; x LE xsize; x++)
            val = img[color][y][x];
            fprintf(outfile,"%c",hex[15 * ((int) (val >> 7))]);
            IF (NOT --size) fprintf(outfile,"\n"); size=16; ENDIF;
          ENDFOR;
        ENDFOR;
      ENDFOR;
      break;
    case 2:
      FOR (y=1; y LE ysize; y++)
        FOR (color=1; color LE 3; color++)
          FOR (x=1; x LE xsize; x++)
            val = img[color][y][x];
            fprintf(outfile,"%c",hex[5 * ((int) (val >> 6))]);
            IF (NOT --size) fprintf(outfile,"\n"); size=16; ENDIF;
          ENDFOR;
        ENDFOR;
      ENDFOR;
      break;
    case 3:
      FOR (y=1; y LE ysize; y++)
        FOR (color=1; color LE 3; color++)
          FOR (x=1; x LE xsize; x++)
            val = img[color][y][x];
            fprintf(outfile,"%c",hex[2 * ((int) (val >> 5))]);
            IF (NOT --size) fprintf(outfile,"\n"); size=16; ENDIF;
          ENDFOR;
        ENDFOR;
      ENDFOR;
      break;
    case 4:
      FOR (y=1; y LE ysize; y++)
        FOR (color=1; color LE 3; color++)
          FOR (x=1; x LE xsize; x++)
            val = img[color][y][x];
            fprintf(outfile,"%c",hex[val >> 4]);
            IF (NOT --size) fprintf(outfile,"\n"); size=16; ENDIF;
          ENDFOR;
        ENDFOR;
      ENDFOR;
      break;
    case 8:
      FOR (y=1; y LE ysize; y++)
        FOR (color=1; color LE 3; color++)
          FOR (x=1; x LE xsize; x++)
            val = img[color][y][x];
            fprintf(outfile,"%c%c",hex[val >> 4], hex[val & 15]);
            IF (NOT --size) fprintf(outfile,"\n"); size=16; ENDIF;
          ENDFOR;
        ENDFOR;
      ENDFOR;
      break;
    default:
      sprintf(t, "%d", flag_c);
      Usage(1, "-c = %s, unknown value.\n", t);
      break;
  }
}




/*L*

________________________________________________________________

		print_tail
________________________________________________________________

Name:		print_tail
Syntax:	 	| static void print_tail() 
Description:
Return value: 
Author:		Otto Milvang
________________________________________________________________

*/



static void print_tail(void)
{
  fprintf(outfile,"\n");
  fprintf(outfile,"grestore\n");
  if (! flag_t) {
    fprintf(outfile,"restore\n");
    if (flag_cp > 1) fprintf(outfile,"/#copies %d def\n", flag_cp);
    fprintf(outfile,"showpage\n");
    if (flag_cp > 1) fprintf(outfile,"/#copies %d def\n", 1);
    fprintf(outfile,"\n");
    fprintf(outfile, "%%%%Trailer\n");
    fprintf(outfile, "%%%%Pages: 1\n");
    fprintf(outfile,"\n");
  }
}




int main(int argc, char **argv)
{
  IMAGE img;
  int depth, xsize, ysize; 
  char c;

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<options>] <infilename> <outfilename>\n\
       <options>:\n\
        -c <int>    - Number of grey levels (2, 4, 8, 16 or 256)\n\
                      (default: 256)\n\
        -m <float>  - Magnify image with factor <float>\n\
        -w <float>  - Set image width to <float> cm\n\
        -h <float>  - Set image height to <float cm>\n\
        -x <float>  - Set left marg  to <float> cm\n\
        -y <float>  - Set bottom marg to <float> cm\n\
        -q <pos>    - Print image in cell given by <pos>\n\
        -p          - Don't print prolog\n\
        -t          - Don't print trailer\n\
        -i <file>   - Set input filename \n\
        -o <file>   - Set output filename\n\
        -ht <text>  - Set header\n\
        -hs <text>  - Header size (default: 25)\n\
        -hy <text>  - Header ypos (default: ysize 10 add)\n\
        -hx <text>  - Header xpos (default: center)\n\
        -ft <text>  - Set footnote \n\
        -fs <text>  - Footnote size (default: 15)\n\
        -fy <text>  - Footnote ypos (default: -10)\n\
        -fx <text>  - Footnote xpos (default: center)\n\
	-bg <int>   - Background pixel value <int>\n\
        -copies <n> - Print n copies\n\
	-rgb        - Interpret three bands as rgb\n\
       \n\
       <infilename> \n\
          Use '-' or '-0' for stdin \n\
          Option -i takes precedence \n\
       <outfilename> \n\
          Use '-' or '-1' for stdout.\n\
          Option -o takes precedence \n"));

  if (argc == 1) Usage(1, NULL);

  flag_b = atoi(read_switch(&argc, argv, "-b", TRUE, "0")); /*will be removed*/
  if (flag_b)
    Usage(1, "switch -b no longer supported. Use filename:band.\n");
  flag_c = atoi(read_switch(&argc, argv, "-c", TRUE, "256"));
  flag_h = atof(read_switch(&argc, argv, "-h", TRUE, "0.0"));
  flag_w = atof(read_switch(&argc, argv, "-w", TRUE, "0.0"));
  flag_m = atof(read_switch(&argc, argv, "-m", TRUE, "1.0"));
  flag_x = atof(read_switch(&argc, argv, "-x", TRUE, "-1.0"));
  flag_y = atof(read_switch(&argc, argv, "-y", TRUE, "-1.0"));
  flag_bg = atoi(read_switch(&argc, argv, "-bg", TRUE, "-1"));
  flag_cp = atoi(read_switch(&argc, argv, "-copies", TRUE, "1"));
  flag_rgb = (read_switch(&argc, argv, "-rgb", FALSE, NULL) NE NULL);
  flag_p = (read_switch(&argc, argv, "-p", FALSE, NULL) NE NULL);
  flag_t = (read_switch(&argc, argv, "-t", FALSE, NULL) NE NULL);
  flag_q = read_switch(&argc, argv, "-q", TRUE, NULL);
  flag_ht = read_switch(&argc, argv, "-ht", TRUE, NULL);
  flag_hs = read_switch(&argc, argv, "-hs", TRUE, "25");
  flag_hx = read_switch(&argc, argv, "-hx", TRUE, "xcenter");
  flag_hy = read_switch(&argc, argv, "-hy", TRUE, "ysize 10 add");
  flag_ft = read_switch(&argc, argv, "-ft", TRUE, NULL);
  flag_fs = read_switch(&argc, argv, "-fs", TRUE, "15");
  flag_fx = read_switch(&argc, argv, "-fx", TRUE, "xcenter"); 
  flag_fy = read_switch(&argc, argv, "-fy", TRUE, "-10 ffontsize sub");

  strcpy(flag_i, read_switch(&argc, argv, "-i", TRUE, ""));
  strcpy(flag_o, read_switch(&argc, argv, "-o", TRUE, ""));

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");
  if (strlen(flag_i) EQ 0) strcpy(flag_i, argv[1]);
  if (strlen(flag_o) EQ 0) strcpy(flag_o, argv[2]);

  if ((img = Iread_image(flag_i)) EQ 0) Error(1, "Can't find %s.\n", flag_i);
  if (Ipixtyp(img[1]) != Iu_byte_typ)
    Error(1, "Can only handle unsigned byte images.\n");

  if (!strcmp(flag_o, "-") || !strcmp(flag_o, "-1")) outfile = stdout;
  else outfile = fopen(flag_o, "w");

  if (outfile EQ NULL) Error(1, "Can't open outfile %s.\n", flag_o);
  /* Depth represents number of bits necessary for representing each pixel
     as hexadecimal digits. A hex digit is minimum 4 bits. Depth=2 is just to
     distinguish between dividing by 16 (depth=4) or by 128 (depth=2). */

  switch(flag_c)
  { case 2:
      depth = 1; break;
    case 4:
      depth = 2; break;
    case 8:
      depth = 3; break;
    case 16:
      depth = 4; break;
    default:
      depth = 8; break;
    }

  if (flag_q) {
    while (*flag_q) {
	c = *flag_q++;
	switch(c)
	    {
	      case 'h': flag_qh = atoi(flag_q); break;
	      case 'w': flag_qw = atoi(flag_q); break;
	      case 'x': flag_qx = atoi(flag_q); break;
	      case 'y': flag_qy = atoi(flag_q); break;
	      default: break;
	    }
      }
  }

  if (flag_rgb && Inbands(img) < 3) {
    Warning(1, "RGB coding requires three bands. Ignoring option \"-rgb\".\n");
    flag_rgb = 0;
  }

  if (flag_rgb) {
    xsize = MIN(MIN(Ixsize(img[1]), Ixsize(img[2])), Ixsize(img[3]));
    ysize = MIN(MIN(Iysize(img[1]), Iysize(img[2])), Iysize(img[3]));
    print_header(Ititle(img), xsize, ysize, depth);
    print_rgb_data(img, depth);
    print_tail();
  } else {
    print_header(Ititle(img), Ixsize(img[1]), Iysize(img[1]), depth);
    print_data(img[1], depth);
    print_tail();
  }

  return(0);
}
