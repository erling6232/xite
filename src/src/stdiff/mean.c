
/*

________________________________________________________________

        mean.c
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/stdiff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H



#ifndef MAIN

/*L*

________________________________________________________________

		update_line
________________________________________________________________

Name:		update_line
Syntax:		
Description:	Update sum 'x' for a 'line' of length 'width'. 
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#ifndef FUNCPROTO
#define update_line_template(name, pixtyp, calctyp) \
static void name(line, x, width, add) \
pixtyp *line; \
calctyp *x; \
int width, add; \
{ \
  line++; \
  if (add) { \
    while(width--) { \
      *x += *line++; x++; \
    } \
  } else { \
    while(width--) { \
      *x -= *line++; x++; \
    } \
  } \
}
#else /* FUNCPROTO */
#define update_line_template(name, pixtyp, calctyp) \
static void name(pixtyp *line, calctyp *x, int width, int add) \
{ \
  line++; \
  if (add) { \
    while(width--) { \
      *x += *line++; x++; \
    } \
  } else { \
    while(width--) { \
      *x -= *line++; x++; \
    } \
  } \
}
#endif /* FUNCPROTO */

update_line_template(update_line_byte,        UNS_BYTE,   int)
update_line_template(update_line_signed_byte, SIGN_BYTE,  int)
update_line_template(update_line_short,       SIGN_SHORT, int)
update_line_template(update_line_unsigned,    UNS_SHORT,  int)
update_line_template(update_line_integer,     INTEGER,    INTEGER)
update_line_template(update_line_real,        REAL,       DOUBLE)
update_line_template(update_line_double,      DOUBLE,     DOUBLE)



/*L*

________________________________________________________________

		mean_line
________________________________________________________________

Name:		mean_line
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#ifndef FUNCPROTO
#define mean_line_template(name, pixtyp, calctyp) \
static void name(result, input, x, width, deltax, deltay) \
pixtyp *result, *input; \
calctyp *x; \
int width, deltax, deltay; \
{ \
  int N, i; \
  calctyp *xx, sumx; \
  \
  result++; input++; \
  sumx = 0;  N = 0; \
  xx = x; \
  if (deltax > width) deltax = width-1; \
  if ((deltax & 1) == 0) deltax++; \
  \
  for (i=1; i<=deltax; i++) \
    { \
      N      += deltay; \
      sumx   += *xx++; \
      if (i & 1) *result++ = sumx/N; \
    } \
  width -= deltax; \
  while (width--) \
    { \
      sumx   += *xx++; \
      sumx   -= *x++; \
      *result++ = sumx/N; \
    } \
  while (deltax--) \
    { \
      N      -= deltay; \
      sumx   -= *x++; \
      if (deltax & 1) *result++ = sumx/N; \
    } \
}
#else /* FUNCPROTO */
#define mean_line_template(name, pixtyp, calctyp) \
static void name(pixtyp *result, pixtyp *input, calctyp *x, int width, int deltax, int deltay) \
{ \
  int N, i; \
  calctyp *xx, sumx; \
  \
  result++; input++; \
  sumx = 0;  N = 0; \
  xx = x; \
  if (deltax > width) deltax = width-1; \
  if ((deltax & 1) == 0) deltax++; \
  \
  for (i=1; i<=deltax; i++) \
    { \
      N      += deltay; \
      sumx   += *xx++; \
      if (i & 1) *result++ = sumx/N; \
    } \
  width -= deltax; \
  while (width--) \
    { \
      sumx   += *xx++; \
      sumx   -= *x++; \
      *result++ = sumx/N; \
    } \
  while (deltax--) \
    { \
      N      -= deltay; \
      sumx   -= *x++; \
      if (deltax & 1) *result++ = sumx/N; \
    } \
}
#endif /* FUNCPROTO */

mean_line_template(mean_line_byte,        UNS_BYTE,   int)
mean_line_template(mean_line_signed_byte, SIGN_BYTE,  int)
mean_line_template(mean_line_short,       SIGN_SHORT, int)
mean_line_template(mean_line_unsigned,    UNS_SHORT,  int)
mean_line_template(mean_line_integer,     INTEGER,    INTEGER)
mean_line_template(mean_line_real,        REAL,       DOUBLE)
mean_line_template(mean_line_double,      DOUBLE,     DOUBLE)



/*F:mean*

________________________________________________________________

		mean
________________________________________________________________

Name:		mean - compute local mean

Syntax:         | #include <xite/stdiff.h>
		|
                | int mean( IBAND input, IBAND output,
                |    int deltax, int deltay );
Description:	| Local Mean over deltax deltay.
		| deltax  = Local area width
		| deltay  = Local area height

Restrictions:   Only non-complex pixel-types.

Return value:   | 0 => ok
                | 1 => Illegal pixel-type
                | 2 => Different input pixel-types in 'input' and 'output'
                | 3 => Bad deltax value
                | 4 => Bad deltay value

Author:		Otto Milvang
Revised:        Svein Bøe, Ifi, UiO (to allow all non-complex pixel types)

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
#define mean_template(name, bandtyp, pixtyp, calctyp, update_fun, mean_fun) \
static int name(input, output, deltax, deltay) \
bandtyp input, output; \
int deltax, deltay; \
{ \
  int  i; \
  calctyp *x; \
  pixtyp **first, **last; \
  int width, height; \
  \
  if (Ipixtyp((IBAND) input) != Ipixtyp((IBAND) output)) \
    return(Error(2, "mean: Different input pixel types.\n")); \
  \
  if (deltax < 1)  return(Error(3, "mean: Bad deltax value\n")); \
  if (deltay < 1)  return(Error(4, "mean: Bad deltay value\n")); \
  \
  width  = Ixsize((IBAND) input); \
  height = Iysize((IBAND) input); \
  \
  input++; output++; \
  first = last = input;  \
  x  = (calctyp *) malloc(width*sizeof(calctyp)); \
  for(i=0; i<width; i++) x[i] = 0; \
  for (i=1; i<= deltay; i++) \
    { \
      update_fun(*first++, x, width, 1); \
      if ( i & 1) mean_fun(*output++, *input++, x, width, deltax, i); \
    } \
  height -= deltay; \
  while(height--) \
    { \
      update_fun(*first++, x, width, 1); \
      update_fun(*last++, x,  width, 0); \
      mean_fun(*output++, *input++, x, width, deltax, deltay); \
    } \
  while(deltay--) \
    { \
      update_fun(*last++, x, width, 0); \
      if (deltay & 1) \
	mean_fun(*output++, *input++, x, width, deltax, deltay); \
  \
    } \
  free(x); \
  return(0); \
}
#else /* FUNCPROTO */
#define mean_template(name, bandtyp, pixtyp, calctyp, update_fun, mean_fun) \
static int name(bandtyp input, bandtyp output, int deltax, int deltay) \
{ \
  int  i; \
  calctyp *x; \
  pixtyp **first, **last; \
  int width, height; \
  \
  if (Ipixtyp((IBAND) input) != Ipixtyp((IBAND) output)) \
    return(Error(2, "mean: Different input pixel types.\n")); \
  \
  if (deltax < 1)  return(Error(3, "mean: Bad deltax value\n")); \
  if (deltay < 1)  return(Error(4, "mean: Bad deltay value\n")); \
  \
  width  = Ixsize((IBAND) input); \
  height = Iysize((IBAND) input); \
  \
  input++; output++; \
  first = last = input;  \
  x  = (calctyp *) malloc(width*sizeof(calctyp)); \
  for(i=0; i<width; i++) x[i] = 0; \
  for (i=1; i<= deltay; i++) \
    { \
      update_fun(*first++, x, width, 1); \
      if ( i & 1) mean_fun(*output++, *input++, x, width, deltax, i); \
    } \
  height -= deltay; \
  while(height--) \
    { \
      update_fun(*first++, x, width, 1); \
      update_fun(*last++, x,  width, 0); \
      mean_fun(*output++, *input++, x, width, deltax, deltay); \
    } \
  while(deltay--) \
    { \
      update_fun(*last++, x, width, 0); \
      if (deltay & 1) \
	mean_fun(*output++, *input++, x, width, deltax, deltay); \
  \
    } \
  free(x); \
  return(0); \
}
#endif /* FUNCPROTO */

mean_template(mean_byte, IUB_BAND, UNS_BYTE,int, update_line_byte, mean_line_byte)
mean_template(mean_signed_byte, ISB_BAND, SIGN_BYTE, int, update_line_signed_byte, mean_line_signed_byte)
mean_template(mean_short, ISS_BAND, SIGN_SHORT, int, update_line_short, mean_line_short)
mean_template(mean_unsigned, IUS_BAND, UNS_SHORT, int, update_line_unsigned, mean_line_unsigned)
mean_template(mean_integer, II_BAND, INTEGER, INTEGER, update_line_integer, mean_line_integer)
mean_template(mean_real, IR_BAND, REAL, DOUBLE, update_line_real, mean_line_real)
mean_template(mean_double, ID_BAND, DOUBLE, DOUBLE, update_line_double, mean_line_double)


#ifndef FUNCPROTO
int mean(input, output, deltax, deltay)
IBAND input, output;
int deltax, deltay;
#else /* FUNCPROTO */
int mean(IBAND input, IBAND output, int deltax, int deltay)
#endif /* FUNCPROTO */
{
  int stat = 0;

  switch((int) Ipixtyp(input))
    {
    case Iu_byte_typ:
      stat = mean_byte(input, output, deltax, deltay);
      break;
    case Is_byte_typ:
      stat =
	mean_signed_byte((ISB_BAND) input, (ISB_BAND) output, deltax, deltay);
      break;
    case Iu_short_typ:
      stat =
	mean_unsigned((IUS_BAND) input, (IUS_BAND) output, deltax, deltay);
      break;
    case Is_short_typ:
      stat = mean_short((ISS_BAND) input, (ISS_BAND) output, deltax, deltay);
      break;
    case Integer_typ:
      stat = mean_integer((II_BAND) input, (II_BAND) output, deltax, deltay);
      break;
    case Ireal_typ:
      stat = mean_real((IR_BAND) input, (IR_BAND) output, deltax, deltay);
      break;
    case Idouble_typ:
      stat = mean_double((ID_BAND) input, (ID_BAND) output, deltax, deltay);
      break;
    default: stat = 1; break;
    }

  return(stat);

} /* mean() */

#endif /* not MAIN */

/*P:mean*

________________________________________________________________

		mean
________________________________________________________________

Name:		mean - compute local mean

Syntax:		mean <infile> <outfile> [deltax [deltay]] 

Description:	| Local Mean 
		| deltax  = Local area width (default = 3)
		| deltay  = Local area height (default = deltax)

Restrictions:   Only non-complex pixel-types.

Author:		Otto Milvang, Ifi, UiO
Revised:        Svein Bøe, Ifi, UiO (to allow all non-complex pixel types)

Examples:	| mean mona.img monamin.img
                | mean mona.img monamin.img 5
                | mean mona.img monamin.img 3 9

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE input, output;
  int i, dx, dy;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <input> <output> [<dx> [<dy>]]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if ((argc < 3) || (argc > 5)) Usage(1, "Illegal number of arguments.\n");

  if (argc >= 4) dx = atoi(argv[3]); else dx = 3;
  if (argc == 5) dy = atoi(argv[4]); else dy = dx;

  input  = Iread_image(argv[1]);
  output = Icopy_init(input);  

  for (i=1; i<=Inbands(input); i++)
    mean(input[i], output[i], dx, dy);

  Ihistory(output, argv[0], args); 
  Iwrite_image(output, argv[2]);

  return(0);
}
     
#endif /* MAIN */
