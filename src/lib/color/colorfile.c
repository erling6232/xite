

/*

________________________________________________________________

        colorfile.c
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



/*F:color_create=colorfile*/
/*F:color_read=colorfile*/
/*F:color_write=colorfile*/
/*F:color_convertfromimg=colorfile*/
/*F:PrintColortable=colorfile*/
/*F:read_colormap=colorfile*/
/*F:colorfile*

________________________________________________________________

		colorfile
________________________________________________________________

Name:		colorfile, color_convertfromimg, color_create, color_read,
                color_write, PrintColortable, read_colormap - Read, write and
		create colortables and colormaps

Syntax:         | #include <xite/color.h>
                |
                | IMAGE color_create( Color_tab coltab,
                |    char* name, int len );
		|
                | BiffStatus color_read( char* file_name,
                |    Color_tab coltab, char** name, int* len );
                |
                | BiffStatus color_write( char* file_name,
                |    Color_tab coltab, char* name, int len );
		|
                | BiffStatus color_convertfromimg( IMAGE img,
                |    Color_tab coltab, char** name, int* len );
		|
                | void PrintColortable( Color_cell* col, int len,
                |    int hex, FILE* fileptr );
                |
		| void read_colormap( FILE *file, Color_cell *ctab,
		|    int *len );

Description:	'color_create' returns an IMAGE containing the 
                colortab (or NULL if error).

		'color_read' reads a colortable from file 'file_name'
		into 'coltab'. The name of the colortable is returned
		in 'name', and the length of the colortable is returned
		in 'len'. The routine returns 'Iok', 'Iopenerr' or
		'Icolpixerr'. The pixelvalue for each colorcell in the file
		is tripled in the returned colortable by shifting and
		OR-ing the original pixelvalue so that the colortable can
		also be used by 24-bit composite-colormap visuals (DirectColor
		and TrueColor).

		'color_write' writes a colortable 'coltab' of length
		'len' to file 'file_name' with title 'name'.
		The routine returns 'Iok', 'Iopenerr' or 'Ibifferr'.
		Only the PseudoColor part of the pixelvalue (maximum 8 bits)
		is stored in the file.

		'color_convertfromimg' converts a color table from IMAGE
		format to colortable format. The pixelvalue for each colorcell
		is tripled in the returned colortable by shifting
		and OR-ing the original pixelvalue so that the colortable can
		also be used by composite-colormap visuals (DirectColor and
		TrueColor).

		'PrintColortable' prints 'col' in an ascii format which can be
		given as input to 'read_colormap' (and the program 'mct(1)').
		The output consists of 4 columns which are colortable
		pixelvalue, red, green and blue (in this order).

		The pixelvalue column printed by 'PrintColortable' can be
		given as decimals or hexadecimals ('hex' nonzero).
		Only the PseudoColor part of the pixelvalue is printed.

		'read_colormap' can read the ascii formatted colormap printed
		by 'PrintColortable'.  The resulting colortable may be
                used on 8-bit PseudoColor visuals as well as on 24-bit
                composite-colormap visuals (DirectColor and TrueColor).

                Each line in the file describes a color in the colortable. The
                line-format is

                | <colornumber> <red-val> <green-val> <blue-val>

                'colornumber' is an unsigned integer. It is used as an index to
                lookup 'red-val', 'green-val' and 'blue-val'.

                'red-val', 'green-val' and 'blue-val' are the intensities of
                the colors in the range 0 - 65535. A special notation
                | c1 r1 g1 b1
                | :
                | c2 r2 g2 b2
                defines a linear stretch in the range 'c1' to 'c2', i.e.
                the colornumbers in the range 'c1' to 'c2' will get red
                intensities in the range 'r1' to 'r2', green in the range 'g1'
                to 'g2' and blue in the range 'b1' to 'b2'. The ranges are
                linearly interpolated.

                Lines with a # in coloumn 1 are treated as comments.

See also:       mct(1), color2ascii(1), color2ascii(3)

Author:		Otto Milvang and Svein Bøe
Bug fix:	24.07.92 Knut-Andreas Lie
Examples:
Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/color.h>
#include <xite/message.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif

void PrintColortable(Color_cell *col, int len, int hex, FILE *fileptr)
{
  int i;
  unsigned long pixel;

  if (hex)
    for (i = 0; i < len; i++) {
      pixel = PseudoBasePart(col[i].pixel);
      fprintf(fileptr, "%#8lx %5d %5d %5d\n", pixel,
	      col[i].red, col[i].green, col[i].blue);
    }
  else
    for (i = 0; i < len; i++) {
      pixel = PseudoBasePart(col[i].pixel);
      fprintf(fileptr, "%8ld %5d %5d %5d\n", pixel,
	      col[i].red, col[i].green, col[i].blue);
    }

} /* PrintColortable */

static BiffStatus band2coltab(IBAND band, Color_tab coltab, int *len)
{
  int ysize, y, pix;

  ysize = Iysize(band);
  for(y=1; y <= ysize; y++) {
    pix = band[y][4] != 0 ? band[y][4] :
      (band[y][2] != 0 ? band[y][2] : band[y][3]);
    coltab[y-1].pixel = ShiftToRed(pix) | ShiftToGreen(pix) | ShiftToBlue(pix);
    coltab[y-1].red   = (band[y][5] << 8) | band[y][6];
    coltab[y-1].green = (band[y][7] << 8) | band[y][8];
    coltab[y-1].blue  = (band[y][9] << 8) | band[y][10];
    coltab[y-1].flags = 7;
    coltab[y-1].pad   = 0;
  }

  *len = ysize;
  return(Iok);

} /* band2coltab */

BiffStatus color_convertfromimg(IMAGE img, Color_cell *coltab, char **name, int *len)
{
  if (img == NULL) return(Iopenerr);

  band2coltab(img[1], coltab, len);
  *name = Ititle(img);

  return(Iok);
}


BiffStatus color_read(char *file_name, Color_cell *coltab, char **name, int *len)
{
  IMAGE img;

  img = Iread_image(file_name);
  if (img == NULL) return(Iopenerr);

  if (Ipixtyp(img[1]) != Iu_color_typ) {
    Idel_image(&img);
    return(Icolpixerr);
  }

  band2coltab(img[1], coltab, len);
  *name = Ititle(img);

  Idel_image(&img);

  return(Iok);
}


IMAGE color_create(Color_cell *coltab, char *name, int len)
{
  IMAGE img;
  IBAND band;
  int y;
  img = Imake_image(1, name ? name : "Color_table", 0x0103, 10, len);
  if(img == 0) return(0);
  band = img[1];
  for (y=1; y <= len; y++)
    {
      band[y][1] = 0;
      band[y][2] = 0;
      band[y][3] = 0;
      band[y][4] = PseudoBasePart(coltab[y-1].pixel);
      band[y][5] = coltab[y-1].red  >> 8;
      band[y][6] = coltab[y-1].red & 0x0ff;
      band[y][7] = coltab[y-1].green  >> 8;
      band[y][8] = coltab[y-1].green & 0x0ff;
      band[y][9] = coltab[y-1].blue  >> 8;
      band[y][10]= coltab[y-1].blue & 0x0ff;
    }
  return(img);
}


BiffStatus color_write(char *file_name, Color_cell *coltab, char *name, int len)
{
  IMAGE img;

  img = color_create(coltab, name,len);
  if (img) {
    int status;

    status = Iwrite_image(img, file_name);
    Idel_image(&img);
    return(status);
  }
  else return(Iopenerr);
}

static char getline(FILE *inp, char *line)
{
  while(1) {
    *line = getc(inp);
    if (*line   == (char) EOF) break;
    if (*line   == '\n') *line=0;
    if (*line++ == '\0') break;
  }

  return(*line);
}

static int a2i(char *str, int line_num)
{
  char *test;
  int val;

  val = strtol(str, &test, 0);
  if (*test != 0) 
    Error(2, "Arguments error in line %d\n\
                   %c%s%c\n", line_num, '"', str,'"');
  return(val);
}

void read_colormap(FILE *file, Color_cell *ctab, int *len)
{
  char line[132], spx[32], sr[32], sg[32], sb[32], rest[132];
  int i, num_cells = 0, line_num = 0;
  int linear = 0;
  int pixel  = 0;
  int red = 0, green = 0, blue = 0, op, or, og, ob;

  while(getline(file, line) != EOF) {
    line_num++;
    /* Comment line? */
    if (line[0] == '#' || line[0] == ';' || line[0] == '!') continue;

    if (line[0] == ':') {
      linear = 1;
    } else {
      /* Save old values. */
      op = pixel;
      or = red;
      og = green;
      ob = blue;

      i = sscanf(line,"%s %s %s %s %s",spx,sr,sg,sb,rest);
      if ((i != 4) && (rest[0] != '#'))
	Error(2, "Wrong number of arguments in line %d\n\
                       %c%s%c\n", line_num, '"', line, '"');

      pixel = a2i(spx, line_num);
      if (pixel > UNS_BYTE_MAX) {
	Warning(1, "%s%d%s",
		"Can not handle colornumber greater than ",
		UNS_BYTE_MAX, ". Using only least significant 8 bits.\n");
	pixel = PseudoBasePart(pixel);
      }

      red   = a2i(sr, line_num);
      green = a2i(sg, line_num);
      blue  = a2i(sb, line_num);

      if (linear) {
	int num_lin;

	num_lin = pixel - ctab[num_cells - 1].pixel;

	if (num_lin) {
	  /* make the entries corresponding to `:' */
	  for (i = num_cells; i < num_cells + num_lin - 1; i++) {
	    ctab[i].pixel = op + (pixel - op) * (i - num_cells + 1) / num_lin;
	    ctab[i].red   = or + (red   - or) * (i - num_cells + 1) / num_lin;
	    ctab[i].green = og + (green - og) * (i - num_cells + 1) / num_lin;
	    ctab[i].blue  = ob + (blue  - ob) * (i - num_cells + 1) / num_lin;
	  }
	  num_cells += num_lin - 1;
	} else Warning(1, "mct: Warning: Interval length = 0.\n");
      }

      ctab[num_cells].pixel = pixel;
      ctab[num_cells].red   = red;
      ctab[num_cells].green = green;
      ctab[num_cells].blue  = blue;
      num_cells++;
      linear = 0;
    } /* not linear */
  } /* while line */

  *len = num_cells;

} /* read_colormap() */
