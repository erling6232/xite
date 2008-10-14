
/*

________________________________________________________________

        ascii2biff.c
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



#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/convert.h>
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
#include <xite/blab.h>
#include <xite/utils.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

static char *ascii2biff_errors_s[] = {
  "ascii2biff: OK.\n",
  "ascii2biff: \n",
  "ascii2biff: Can't find file %s.\n",
  "ascii2biff: Bad pixeltype.\n",
  "ascii2biff: Bad xsize or ysize.\n",
  "ascii2biff: Can't calculate image size.\n",
  "ascii2biff: Not enough memory.\n",
};

#ifndef MAIN

#ifndef HIBITL
# define HIBITL (1L << (8 * sizeof(long) - 1))
#endif
#define NOVAL HIBITL
#define ITEM_EOF         0
#define ITEM_NEWLINE     1
#define ITEM_COMMENT     2
#define ITEM_KEYWORD     3
#define ITEM_ARG         4
#define ITEM_AXIS        5
#define ITEM_AXIS_LABELS 6
#define ITEM_AXIS_LABEL  7

static char buffer_s[200];         /* Last item read       */
static int readcr_s = 0;           /* Look ahead CR read   */
static int isint_s;                /* Is integer argument  */

static IPIXTYP pixtype_s;          /* Type                 */
static long xsize_s, ysize_s;      /* Size parameters      */
static long xstart_s, ystart_s;    /* Start parameters     */
static long xmag_s, ymag_s;        /* Mag parameters       */
static long minval_s, maxval_s;    /* Special values       */
static int item_num_s = 1;

static int readitem(FILE *ascii)
{
  int i;
  char *p;

  if (readcr_s) {
    readcr_s = 0;
    return(ITEM_NEWLINE);
  }
  
  /* Skip blanks. */
  do { i=fgetc(ascii);} while((i != '\n') && isspace(i));

  if (i == '\n') {
    item_num_s = 1;
    return(ITEM_NEWLINE);
  }
  if (i == EOF) return(ITEM_EOF);

  p = buffer_s;
  if (i == '#' || i == ';') {
    do { i=fgetc(ascii); } while(i != EOF && i !='\n');
    item_num_s = 1;
    return(ITEM_COMMENT);
  }

  if (i == '$') {
    item_num_s++;
    return(ITEM_KEYWORD);
  }

  if (i == '|' && item_num_s == 1) {
    do { i=fgetc(ascii); } while(i != EOF && i !='\n');
    item_num_s = 1;
    return(ITEM_AXIS_LABELS);
  }

  if (i == '|' && item_num_s == 2) {
    do { i=fgetc(ascii);} while((i != '\n') && i != EOF && isspace(i));
    if (i != '\n' && i != EOF) ungetc(i, ascii);
    item_num_s++;
    return(ITEM_AXIS_LABEL);
  }

  if (i == '-' && item_num_s == 1) {
    int ii;

    if ((ii = fgetc(ascii)) == '-') {
      do { i=fgetc(ascii); } while(i != EOF && i !='\n');
      item_num_s = 1;
      return(ITEM_AXIS);
    } else ungetc(ii, ascii);
  }

  if (i == 'v' && item_num_s == 1) {
    /* Tip of vertical axis when coordinate system is present. */
    return(ITEM_EOF);
  }

  /* Read a word (delimited by blanks). */
  *p++ = i;
  do {i = fgetc(ascii); *p++ = i;} while(!isspace(i) && (i!=EOF));
  if (i == '\n') {
    item_num_s = 1;
    readcr_s = 1;
  } else {
    item_num_s++;
  }
  *(--p) = 0;

  return(ITEM_ARG);
}

static char *key1_s[] =
{
  "",
  "type",
  "size",
  "start",
  "mag",
};

static char *key2_s[] =
{
  "",
  "",
  "",
  "uns_byte",
  "",
  "uns_short",
  "sign_short",
  "integer",
  "real",
  "",
  "",
  "",
};

static int minarr_s[] = {
  0, /* Ibit_typ       */
  0, /* Ibit2_typ      */
  0, /* Inibble_typ    */
  0, /* UNS_BYTE       */
  0, /* SIGN_BYTE      */
  0, /* UNS_SHORT      */
  SIGN_SHORT_MIN,
  INTEGER_MIN,
  0, /* Ireal_typ      */
  0, /* Icomplex_typ   */
  0, /* Idouble_typ    */
  0, /* Id_complex_typ */
};
static int maxarr_s[] = {
  0, /* Ibit_typ       */
  0, /* Ibit2_typ      */
  0, /* Inibble_typ    */
  UNS_BYTE_MAX,
  0, /* SIGN_BYTE      */
  UNS_SHORT_MAX,
  SIGN_SHORT_MAX,
  INTEGER_MAX,
  0, /* Ireal_typ      */
  0, /* Icomplex_typ   */
  0, /* Idouble_typ    */
  0, /* Id_complex_typ */
};

static void setarg(char *arg, int xy, long *xval, long *yval, int v1, int v2)
{
  int w = 0;
  switch (xy)
  {
    case 0:
      if ((*xval != (NOVAL) && *xval != v1)||(*yval != (NOVAL) && *yval != v2)) 
	w = 1;
      *xval = v1;
      *yval = v2;
      break;
    case 'x':
      if (*xval != (NOVAL) && *xval != v1) w = 1;
      *xval = v1;
      break;
    case 'y':
      if (*yval != (NOVAL) && *yval != v1) w = 1;
      *yval = v1;
      break;
  }
  if (w) Warning(0, "ascii2biff: %s parameter modified.\n", arg);
}

static int settype(IPIXTYP typ)
{
  if (typ != Iu_byte_typ  && typ != Is_short_typ &&
      typ != Iu_short_typ && typ != Ireal_typ &&
      typ != Integer_typ)
      return(Warning(0, 
        "ascii2biff: Bad pixeltype \"%s\" ignored.\n", Ipixname(typ)));
  if (pixtype_s != (NOVAL) && typ != pixtype_s)
     Warning(0, "ascii2biff: pixtype modified.\n");
  pixtype_s = typ;
  minval_s  = minarr_s[typ];
  maxval_s  = maxarr_s[typ];
  isint_s   = (typ != Ireal_typ);
  return(0);
}

static int readkeyword(FILE *ascii)
{
  char key[200], *p;
  int i, j, i1, i2, i3, i4;
  int v1, v2, xy;

  i1 = readitem(ascii);
  if (i1 != ITEM_ARG) goto clean;

  p = buffer_s;
  xy = 0;
  if (*p == 'x') xy = *p++;
  else if (*p == 'y') xy = *p++;
  strcpy(key, buffer_s);
  for (i=0; i<=4; i++) {
    if (strcmp(key1_s[i], p) == 0) break;
  }
  if (i>4 || i == 0) goto clean;
  if (i == 1) xy = 't';
  i2 = readitem(ascii);
  if (i2 != ITEM_ARG) goto clean;
  v1 = atoi(buffer_s);
  if (xy == 0) {
    i3 = readitem(ascii);
    if (i3 != ITEM_ARG) goto clean;
    v2 = atoi(buffer_s);
  }
  switch(i)
  {
    case 1:
      for (j=0; j<=11; j++) {
	if (strcmp(key2_s[j], buffer_s) == 0) break;
      }
      if (j>11 && (*buffer_s < '0' || *buffer_s > '9')) j = -1;
      if(j > 11) settype(atoi(buffer_s)); else settype(j);
      break;
    case 2: setarg("size",  xy, &xsize_s,  &ysize_s,  v1, v2); break;
    case 3: setarg("start", xy, &xstart_s, &ystart_s, v1, v2); break;
    case 4: setarg("mag",   xy, &xmag_s,   &ymag_s,   v1, v2); break;
  }
  i4 = readitem(ascii);
  if (i4 == ITEM_EOF || i4 == ITEM_NEWLINE || i4 == ITEM_COMMENT) return(0);
  clean:
  Warning(0, "ascii2biff: Error in keyword \"%s\".\n",
          i1 == ITEM_ARG ? key : "No keyword");
  do 
  {
    i4 = readitem(ascii);
  } while (i4 != ITEM_EOF && i4 != ITEM_NEWLINE && i4 != ITEM_COMMENT);
  return(2);

} /* readkeyword() */



static int myrealloc(char **ptr1, char **ptr2, int *size, size_t ssize)
{
  int new;
  new = 1.4 * *size;
  *ptr1 = (char*) realloc(*ptr1, new*ssize);
  if (ptr2 != NULL) *ptr2 = *ptr1;
  if (*ptr1 == NULL) return(2);
  *size = new;
  return(0);
}

static BiffStatus read_file(FILE *ascii, int **intval, int **rows, int *numi, int *numr)
{
  int *ints, maxints, *row, maxrows, ci, cr, item, num, val;
  float *floats;
  ints = (int*) malloc(10000*sizeof(int)); 

  if (ints == 0) return(Error(2, "ascii2biff: Not enough memory.\n"));
  floats = (float*) ints;
  maxints = 10000;
  ci = 0;
  row  = (int*) malloc(100*sizeof(int));
  if (row == 0) return(Error(2, "ascii2biff: Not enough memory.\n"));
  maxrows = 100;
  cr = 0;
  num = 0;

  while((item = readitem(ascii)) != 10 /* Always true */)
    switch(item)
      {
      case ITEM_EOF:
      case ITEM_NEWLINE:
      case ITEM_COMMENT:
	if (num) {
	  if (cr == maxrows &&
	      myrealloc((char **)&row, NULL, &maxrows, sizeof(int)) == 2) 
	    return(Error(2, "ascii2biff: Not enough memory.\n"));
	  row[cr++] = num;
	  num = 0;
	}
	if (item == ITEM_EOF) {
	  *intval = ints;
	  *rows   = row;
	  *numi   = ci;
	  *numr   = cr;
	  return(0);
	}	
	break;
      case ITEM_ARG:
	if (ci == maxints &&
            myrealloc((char **)&ints, (char **)&floats, &maxints,
		      sizeof(int)) ==2) 
	  return(Error(2, "ascii2biff: Not enough memory.\n"));
	if (isint_s) {
	  val = atoi(buffer_s);
	  if (val == 0) {
	    if (buffer_s[1] == '\0' && buffer_s[0] == '-') val = minval_s;
	    if (buffer_s[1] == '\0' && buffer_s[0] == '+') val = maxval_s;
	  }
	  ints[ci++] = val;
	} else {
	  floats[ci++] = atof(buffer_s);
	}
	num++;
	break;
      case ITEM_KEYWORD:
        readkeyword(ascii);
        break;
      case ITEM_AXIS_LABEL:
	if (ci > 0)  ci--;
	if (num > 0) num--;
	break;
      case ITEM_AXIS:
      case ITEM_AXIS_LABELS:
	break;
      } /* switch, while */
  return(Error(2, "ascii2biff: Unknown error.\n"));
}



/*F:ascii2biff*

________________________________________________________________

		ascii2biff
________________________________________________________________

Name:		ascii2biff - convert an ascii file to BIFF format

Syntax:         | #include <xite/convert.h>
		|
                | int ascii2biff( IBAND* band, char* asciifile,
                |    IPIXTYP pixtyp );

Description:	'ascii2biff' reads 'asciifile' and returns a 'band' of
                'pixtyp'. 'asciifile' = '-' or NULL means 'stdin'.

                If the file contains $size info, use this. Otherwise, count
		the number of elements on each line. Does it look like an
		m x n matrix? If yes, assign m to ysize and n to xsize.
		Otherwise return an error message.

                If 'pixtyp' is equal to Iunknown_typ, use the $type
                info in the file, otherwise set type to signed short.
		
File format:	The file is an ordinary ascii file. Comments start with a
                '#' or a ';' and stop at end of line. Blank lines or comment
		lines are ignored. Lines with $ in coloumn 1 are treated as
		special arguments. The format is

		| $<keyword> <1 or 2 arguments>

		where <keyword> is one of "type", "size", "xsize", "ysize",
		"start", "xstart", "ystart", "mag", "xmag" or "ymag".

		The format for each <keyword> is:
                | $type <uns_byte | uns_short | sign_short | integer | real>
                | $size   <xsize> <ysize>
                | $xsize  <xsize>
                | $ysize  <ysize>
                | $start  <xstart> <ystart>
                | $xstart <xstart>
                | $ystart <ystart>
                | $mag    <xmag> <ymag>
                | $xmag   <xmag>
                | $ymag   <ymag>

                The keywords will be used as image band arguments. If the size
                arguments are omitted, the size is calculated from the
                organization of the file. Default start arguments are
                -(size/2), and default mag arguments are 1.

		Pixels inside a line are separated by blanks. Lines are
		separated by newline. If pixel type is uns_byte, uns_short,
		sign_short or integer, the special value '-' means lowest legal
		value, and '+' means largest legal value.

Examples:       &Example 1

		| #
		| $ size  4 3
		| 3 5 6 4 3 2
		| 7 5 3 2 1 3

		The resulting image:
		| #
		| # 3 5 6 4
		| # 3 2 7 5
		| # 3 2 1 3
		| #

		&Example 2

		| #
		| 3 5 6 4 
		| 3 2 7 5
		| 3 2 1 3

		The resulting image:
		| #
		| # 3 5 6 4
		| # 3 2 7 5
		| # 3 2 1 3

Return value:	| 0 - OK
                | 1 - Not used
		| 2 - Can't find file
		| 3 - Bad pixeltype
		| 4 - Bad xsize or ysize
		| 5 - Can't calculate image size
		| 6 - Not enough memory
Author:		Otto Milvang
________________________________________________________________

*/

int ascii2biff(IBAND *band, char *asciifile, IPIXTYP pixtyp)
{
  FILE *ascii = NULL;
  ISS_BAND iss;
  IUS_BAND ius;
  IR_BAND  ir;
  II_BAND  ii;
  IBAND    ub;
  float *floatval, *fptr;
  int   *intval, *iptr;
  int   *rows;
  int x, y, len, rowlen;

  if (asciifile && (strcmp(asciifile, "-") == 0)) asciifile = NULL;
  Info(1, "Read ascii file \"%s\" into %s image.\n", 
       asciifile ? asciifile : "(stdin)", Ipixname(pixtyp));

  pixtype_s = xsize_s = ysize_s = xstart_s = ystart_s = xmag_s = ymag_s
    = (NOVAL);
  if (pixtyp != Iunknown_typ) settype(pixtyp);

  if (asciifile) ascii = fopen(tilde_expand(asciifile), "r");
  else ascii = stdin;

  if (ascii == NULL) return(Error(2, ascii2biff_errors_s[2], asciifile));
  
  isint_s = (pixtype_s != Ireal_typ);
  read_file(ascii, (int **)&floatval, &rows, &len, &rowlen);
  intval = (int *) floatval;
  if (asciifile) fclose(ascii);

  if (xsize_s == (NOVAL) || ysize_s == (NOVAL)) {
    if (xsize_s == (NOVAL) && ysize_s != (NOVAL)) xsize_s = len/ysize_s;
    if (xsize_s != (NOVAL) && ysize_s == (NOVAL)) ysize_s = len/xsize_s;
    xsize_s = (xsize_s == (NOVAL)) ? (rows[rowlen-1]) : (xsize_s);
    ysize_s = (ysize_s == (NOVAL)) ? (rowlen) : (ysize_s);
    if (xsize_s * ysize_s != len) return(Error(5, ascii2biff_errors_s[5]));
  } else {
    if (xsize_s * ysize_s != len) return(Error(4, ascii2biff_errors_s[4]));
  }
  
  if (pixtype_s == (NOVAL)) {
    Info(0, "ascii2biff: Assume signed short pixeltype.\n");
    pixtype_s = Is_short_typ;
  }
  xstart_s = (xstart_s == (NOVAL)) ? (-(xsize_s/2)) : (xstart_s);
  ystart_s = (ystart_s == (NOVAL)) ? (-(ysize_s/2)) : (ystart_s);
  xmag_s   = (xmag_s == (NOVAL)) ? (1) : (xmag_s);
  ymag_s   = (ymag_s == (NOVAL)) ? (1) : (ymag_s);
  
  *band = Imake_band(pixtype_s, xsize_s, ysize_s);
  if (*band == NULL)  return(Error(6, ascii2biff_errors_s[6]));
  Iset_start(*band, xstart_s, ystart_s);
  Iset_mag(*band, xmag_s, ymag_s);
  Info(0, "Size %d x %d of type %s.\n", xsize_s, ysize_s, Ipixname(pixtype_s));

  switch((int) pixtype_s)
    {
    case Iu_byte_typ:
      ub = *band;
      iptr = intval;
      for (y=1; y<=ysize_s; y++)
	for (x=1; x<=xsize_s; x++)
	  ub[y][x] = *iptr++;
      break;
    case Is_short_typ:
      iss = (ISS_BAND) *band;
      iptr = intval;
      for (y=1; y<=ysize_s; y++)
	for (x=1; x<=xsize_s; x++)
	  iss[y][x] = *iptr++;
      break;
    case Iu_short_typ:
      ius = (IUS_BAND) *band;
      iptr = intval;
      for (y=1; y<=ysize_s; y++)
	for (x=1; x<=xsize_s; x++)
	  ius[y][x] = *iptr++;
      break;
    case Integer_typ:
      ii = (II_BAND) *band;
      iptr = intval;
      for (y=1; y<=ysize_s; y++)
	for (x=1; x<=xsize_s; x++)
	  ii[y][x] = *iptr++;
      break;
    case Ireal_typ:
      ir = (IR_BAND) *band;
      fptr = floatval;
      for (y=1; y<=ysize_s; y++)
	for (x=1; x<=xsize_s; x++)
	  ir[y][x] = *fptr++;
      break;
    }
  if (pixtype_s == Ireal_typ)
    free(floatval); else
    free(intval);
  return(0);
}

#endif /* not MAIN */

#ifdef MAIN

/*P:ascii2biff*

________________________________________________________________

		ascii2biff
________________________________________________________________

Name:		ascii2biff - convert an ascii file to BIFF format

Syntax:		| ascii2biff [-title title] [-pt pixel_type]
                |          <ascii-file> <BIFF-file> 

Description:	Converts an ascii file to BIFF format.

Options:        &pixel_type

                One of
		  &&ub, uns_byte
		  Unsigned byte, default.

		  &&us, uns_short
		  Unsigned short, 16 bits.

		  &&ss, sign_short
		  Signed short, 16 bits.

		  &&i, integer
		  Signed integer, 32 bits.

		  &&r, real
		  Float, 32 bits.

		&title
		Set the output image title to 'title'

                &ascii-file
		Name of input file, "-" for stdin.

		&BIFF-file
		Name of output file, "-" for stdout.

File format:	The file is an ordinary ascii file. Comments start with a
                '#' or a ';' and stop at end of line. Blank lines or comment
		lines are ignored. Lines with $ in coloumn 1 are treated as
		special arguments. The format is

                | $<keyword> <1 or 2 arguments>

		where <keyword> is one of "type", "size", "xsize", "ysize",
		"start", "xstart", "ystart", "mag", "xmag" or "ymag".

		The format for each <keyword> is:
                | $type   <uns_byte, uns_short, sign_short, integer, real>
                | $size   <xsize> <ysize>
                | $xsize  <xsize>
                | $ysize  <ysize>
                | $start  <xstart> <ystart>
                | $xstart <xstart>
                | $ystart <ystart>
                | $mag    <xmag> <ymag>
                | $xmag   <xmag>
                | $ymag   <ymag>

                The keywords will be used as image band arguments. If the size
                arguments are omitted, the size is calculated from the
                organization of the file. Default start arguments are
                -(size/2), and default mag arguments are 1.

		Pixels inside a line are separated by blanks or tabs. Lines
		are separated by newline. If pixel type is uns_byte,
		uns_short, sign_short or integer, the special value '-' means
		lowest legal value, and '+' means largest legal value.

		&Example 1

		| #
		| $ size  4 3		; xsize = 4, ysize = 3
		| 3 5 6 4 3 2
		| 7 5 3 2 1 3

		The resulting image:
		| #
		| # 3 5 6 4
		| # 3 2 7 5
		| # 3 2 1 3
		| #

		&Example 2

		| #
		| 3 5 6 4 
		| 3 2 7 5
		| 3 2 1 3

		The resulting image:
		| #
		| # 3 5 6 4
		| # 3 2 7 5
		| # 3 2 1 3

Restrictions:	Accepts one band images only.
Author:		Otto Milvang, BLAB, Ifi, UiO
                

Examples:	| 1. Convert a formatted text file
                |    ascii2biff -v -pt ss mona.txt mona.img
		|
		| 2. Convert an unformatted text file without write access.
		|    First create a file, for example mona.def, containing:
		|    # Definition file for mona.txt
		|    $type  sign_short  # set pixeltype
		|    $size 1666 1230    # set xsize ysize
		|    # ----------------------------------
		|    Then convert the file with:
		|    cat mona.def mona.txt | ascii2biff  -pt ss - mona.img

________________________________________________________________

*/

int main(int argc, char **argv)
{
  
  IMAGE img;
  IBAND band;
  IPIXTYP pix_type;
  char *title, *pt, *args;
  int stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-title title] [-pt pixel_type] <ascii-file> <BIFF-file>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  title = read_switch(&argc, argv, "-title", 1, "Converted from ascii");
  title = read_switch(&argc, argv, "-t", 1, title);
  pt    = read_switch(&argc, argv, "-pt", 1, "ub");

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  pix_type = IparsePixtyp(pt);
  if (pix_type < Iu_byte_typ || pix_type > Ireal_typ)
    Error(3, ascii2biff_errors_s[3]);

  img    = Init_image(1, title);
  stat   = ascii2biff(&band, argv[1], pix_type);
  img[1] = band;
  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  return(0);
}	

#endif /* MAIN */
