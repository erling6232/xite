
/*

________________________________________________________________

        colorquant.c
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



/* ---------------------------------------------------- */
/*							*/
/*	Program	: colorquant				*/
/*	Author	: Otto Milvang				*/
/*	Date	: 15-May-1987				*/
/*	Version	: 1.0					*/
/*							*/
/* ---------------------------------------------------- */

typedef unsigned char byte, *byteband, *byteimage[];
typedef int *intband, *intimage[];

#define DUMP(e1,e2,e3,e4,e5)  /* printf(e1,e2,e3,e4,e5) */	 
#define MAXBANDS 7
#define COLORS  1024

#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/color.h>
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_TYPES_H

#ifdef MAIN

#define TIMING 1

#if defined(TIMING) && !defined(_WIN32)
#include <sys/times.h>
struct tms t;
static long t1_s,t2_s,t3_s,t4_s,t5_S,t6_s,t7_s, h1,h2,h3,h4,h5,h6;
#define INIT t1_s=t2_s=t3_s=t4_s=t5_S=t6_s=t7_s=0;
#define IN(x) times(&t); x = t.tms_utime;
#define OUT(x,y) times(&t); x = x + t.tms_utime - y;
#define REPORT(text,x) printf("%s = %6.2f s\n",text,(float)x/60.0);
#else
#define INIT
#define IN(x)
#define OUT(x,y)
#define REPORT(text,x)
#endif
 
static int limit_s = 0;

typedef struct 
{  
  long len ,offset;
  long avvik;
  long sum[MAXBANDS];
  long  bs_val;
  long  bs_plan;
  long  minval[MAXBANDS];
  long  maxval[MAXBANDS];
} state;

static long nbands_s = 0;

static char *infile_s, *outfile_s, *colorfile_s;




/*L*

________________________________________________________________

		init_var
________________________________________________________________

Name:		init_var
Syntax:		| static void init_var(ptr_stat, ptr_gptr, min_col, max_col, size)
                | state **ptr_stat;
                | long ** ptr_gptr;
                | long min_col, max_col, size;
Description:
Author:		Otto Milvang
________________________________________________________________

*/

                                  
#ifndef FUNCPROTO
static void init_var(ptr_stat, ptr_gptr, min_col, max_col, size, bands)
state **ptr_stat;
long ** ptr_gptr;
long min_col, max_col, size, bands;
#else /* FUNCPROTO */
static void init_var(state **ptr_stat, long int **ptr_gptr, long int min_col, long int max_col, long int size, long int bands)
#endif /* FUNCPROTO */
{
  long i, count, *gptr;
  state *stat;

  IN(h3)
  
  stat = *ptr_stat = (state *) malloc( sizeof(state) * (max_col - min_col +1 ));
  gptr = *ptr_gptr = (long *)  malloc( sizeof(long)  * size);  

  IF (stat EQ NULL OR gptr EQ NULL)
    Error(2, "Not enough memory\n");
    exit(2);
  ENDIF
 
  stat = *ptr_stat = &stat[-min_col];
  FOR(i=min_col; i LE max_col; i++)
     stat[i].offset = 0;
     stat[i].len    = 0;
  ENDFOR
  stat[min_col].len = size;
  FOR(i=0; i  LT nbands_s; i++)
    stat[min_col].minval[i]  = 0;
    stat[min_col].maxval[i]  = 255;
  ENDFOR

  count = 0;
  while(size--) *gptr++ = count++;
  OUT(t3_s,h3)
}

                              




/*L*

________________________________________________________________

		histogram
________________________________________________________________

Name:		histogram
Syntax:		| static void histogram(hist, image, length);
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
static void histogram(hist, image, length)
  long     *hist;
  byteband image;
  long     length;
#else /* FUNCPROTO */
static void histogram(long int *hist, byteband image, long int length)
#endif /* FUNCPROTO */
{
  unsigned short i;
  long *j;
  IN(h1)
  j = hist; i = 256;
  while(i--) *j++ = 0;
  while(length--) hist[*image++]++;
  OUT(t1_s,h1)
}



/*L*

________________________________________________________________

		statistic
________________________________________________________________

Name:		statistic
Syntax:		| static void statistic(color)
                | state *color;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
static void statistic(color, image)
  state *color;
  byteimage image;
#else /* FUNCPROTO */
static void statistic(state *color, unsigned char **image)
#endif /* FUNCPROTO */
{
  long  plan, i, iant, isum, maxval, minc, maxc, *hptr, *sptr;
  long  av, maxavvik,terskel;
  long h[256], hsum[256];
  IN(h4)
  color -> avvik = -10000000;
  FOR(plan=0; plan LT nbands_s; plan INC)
      DUMP("Plan = %d, Color = %d, Offset = %d, len = %d\n",
         plan, color, color -> offset,color -> len);
      OUT(t4_s,h4)
      histogram(h, &image[plan][color -> offset], color -> len);
      minc = color -> minval[plan];
      while( h[minc] EQ 0 )  minc++;
#ifndef SPLIT
/*      color -> minval[plan] = minc; */
#endif
      maxc = color -> maxval[plan];
      while(h[maxc] EQ 0)  maxc--;
#ifndef SPLIT
/*      color -> maxval[plan] = maxc; */
#endif
IN(h4)
      iant = h[minc];
      isum = hsum[minc] = minc*h[minc];
      IF(minc LT maxc)
          hptr = &h[minc];
          sptr = &hsum[minc+1];
          FOR(i=minc+1; i LE maxc; i INC)
              *sptr++ = (isum += i * *(++hptr));
              *hptr   = (iant += *hptr);
          ENDFOR;
          iant     = h[maxc];
          isum     = hsum[maxc];
          maxval   = maxc;
          maxavvik = 0;
          terskel = (isum/iant) * (isum >> 10);

          hptr = &h[maxc-1];
          sptr = &hsum[maxc-1];

          FOR(i=maxc-1; i GE minc; i DEC)
            av = ( *sptr / *hptr ) * (*sptr >> 10) 
               + ( (isum - *sptr) / (iant - *hptr) ) * ((isum - *sptr) >> 10);
            hptr--;
            sptr--;
            IF(av GT maxavvik)
              maxavvik = av;
              maxval = i;
            ENDIF;
          ENDFOR;
        ELSE
          maxavvik = terskel = 0;
          maxval = minc;
        ENDIF;
      maxavvik = maxavvik-terskel;
          DUMP("Maxavvik ****  = %d \n",maxavvik,0,0,0);
      color -> sum[plan] = isum;
      IF (maxavvik GT color -> avvik)
          color -> avvik  = maxavvik;
          color -> bs_val = maxval;
          color -> bs_plan = plan;
      ENDIF
    ENDFOR
  DUMP(" %d %d %d %d \n",color,color -> bs_plan,color -> bs_val,color -> avvik);
  OUT(t4_s,h4)
}   




/*L*

________________________________________________________________

		find_max
________________________________________________________________

Name:		find_max
Syntax:		| static long find_max(stat, spl_col, plan, min_col, cur_col)
                | state *stat[];
                | long spl_col, plan, min_col, cur_col;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#ifndef FUNCPROTO
static long find_max(stat, spl_col, plan, min_col, cur_col)
state *stat;
long *spl_col, *plan;
long min_col, cur_col;
#else /* FUNCPROTO */
static long find_max(state *stat, long int *spl_col, long int *plan, long int min_col, long int cur_col)
#endif /* FUNCPROTO */
{
  long col, minval;
  state *cur_stat;

  IN(h5)
  cur_stat = &stat[min_col];
  *spl_col = min_col;
  minval   = cur_stat -> avvik;
  *plan    = cur_stat -> bs_plan;
  FOR(col=min_col+1; col LE cur_col; col++)
    cur_stat++;
    IF(cur_stat -> avvik GT minval)
       DUMP(">> %d %f %f\n", col, cur_stat -> avvik, minval,0);
       *spl_col = col;
       *plan    = cur_stat -> bs_plan;
       minval   = cur_stat -> avvik;
     ENDIF
  ENDFOR
  OUT(t5_S,h5)
  return(minval);
}






/*L*

________________________________________________________________

		split_arr
________________________________________________________________

Name:		split_arr
Syntax:		| static ptrdiff_t split_arr(image, gptr,  offset, length, split)
                | byte *image[];
                | long *gptr;
                | long plan, offset, length, split;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
static ptrdiff_t split_arr(image, gptr,  plan, offset, length, split)
  byteimage image;
  long *gptr;
  long plan, offset, length, split;
#else /* FUNCPROTO */
static ptrdiff_t split_arr(unsigned char **image, long int *gptr, long int plan, long int offset, long int length, long int split)
#endif /* FUNCPROTO */
{
  long off[MAXBANDS];
  byte *btr1,*btr2, bval;
  long  *ptr1,*ptr2, ival;
  long i;

  IN(h2)
  for(i=0; i LT nbands_s; i++)
    off[i] = image[i] - image[plan];
  
  btr1 = &image[plan][offset]; 
  btr2 = btr1+length-1;
  ptr1 = &gptr[offset];
  ptr2 = ptr1+length-1;
  LOOP
    LOOP
      EXITIF(*btr1 GT split);
      INC btr1;
      INC ptr1;
    ENDLOOP
    LOOP
      EXITIF(*btr2 LE split);
      DEC btr2;
      DEC ptr2;
    ENDLOOP;
    EXITIF(btr1 GT btr2);
    FOR(i=0; i LT nbands_s; i++)
      ival = off[i];
      bval = btr1[ival];
      btr1[ival] = btr2[ival];
      btr2[ival] = bval;
    ENDFOR
    ival  = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = ival;
  ENDLOOP
  DUMP("Addr1: %d, Addr2: %d, Lengde %d\n",btr1,&image[plan][offset], length,0);
  OUT(t2_s,h2);
  return(btr1-&image[plan][offset]);

}




/*L*

________________________________________________________________

		split
________________________________________________________________

Name:		split
Syntax:		| static void split( image, gptr, fra, til, plan)
                | byte *image[],
                | long *gptr;
                | state *fra, *til;
                | long plan;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
static void split(image, gptr, fra, til, plan)
byteimage image;
long *gptr;
state *fra, *til;
long plan;
#else /* FUNCPROTO */
static void split(unsigned char **image, long int *gptr, state *fra, state *til, long int plan)
#endif /* FUNCPROTO */
{
  long  i, minval;
    minval = fra -> bs_val;
    plan   = fra -> bs_plan;
    i=split_arr(image, gptr, plan, fra->offset, fra->len, minval);
    DUMP("%d bytes er i gruppe 1\n",i,0,0,0);
    til -> offset = fra -> offset+i;
    til -> len = fra -> len-i;
    fra -> len = i;
    FOR (i=0; i LT nbands_s; i++)
      til -> minval[i] = fra -> minval[i];
      til -> maxval[i] = fra -> maxval[i];
    ENDFOR
    fra -> maxval[plan] = minval;
    til -> minval[plan] = minval+1; 
}







/*L*

________________________________________________________________

		test_nabo
________________________________________________________________

Name:		static BiffStatus test_nabo
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#ifdef SPLIT

#ifndef FUNCPROTO
static BiffStatus test_nabo(stat, color_tab, class, a, b)
state *stat;
intimage  color_tab;
long class[256], a,b;
#else /* FUNCPROTO */
static BiffStatus test_nabo(state *stat, int **color_tab, long int *class, long int a, long int b)
#endif /* FUNCPROTO */
{
  long i, ok, diff, sum;
  if (a == b) return(0);
  FOR (i=0; i LT nbands_s; i++)
    ok = 0;
    if ((stat[b].minval[i] - stat[a].maxval[i] GT 1) OR 
        (stat[a].minval[i] - stat[b].maxval[i] GT 1)) break;
    ok = 1;
  ENDFOR
  IF (ok)
    sum = 0;
    FOR (i=0; i LT nbands_s; i++)
      diff = color_tab[i][a] - color_tab[i][b];
      if (diff < 0) diff = - diff;
      sum += diff;
    ENDFOR
    if (sum > limit_s) ok = 0;
  ENDIF
  if (ok AND (b<a)) class[a] = class[b];
  return(ok);
}

#endif /* SPLIT */



/*L*

________________________________________________________________

		reconstr
________________________________________________________________

Name:		reconstr
Syntax:		| static void reconstr(stat, gptr, image, min_col, max_col)
                | state *stat;
                | long *gptr;
                | byte *image[];
                | long min_col, max_col;
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/



#ifndef FUNCPROTO
static void reconstr(stat, gptr, image, min_col, max_col)
  state *stat;
  long *gptr;
  byteband image;
  long min_col, max_col;
#else /* FUNCPROTO */
static void reconstr(state *stat, long int *gptr, byteband image, long int min_col, long int max_col)
#endif /* FUNCPROTO */
{
  long color,ll,pp,off;
  FOR(color = min_col; color LE max_col; color ++)
    ll = stat[color].len;
    pp = stat[color].offset;
    WHILE(ll--)
      off = gptr[pp++];
      image[off] = color;
    ENDWHILE;
  ENDFOR
}

  




/*L*

________________________________________________________________

		compare
________________________________________________________________

Name:		compare
Syntax:		| static int compare(a, b)
                | state *a, *b;
Description:    Compare grey level of a and b. Compare is used by quick-sort.
Return value:   | -1 if a < b
                |  0 if a = b
                |  1 if a > b
Author:		Otto Milvang
________________________________________________________________

*/


#ifdef FUNCPROTO
static int compare(state *a, state *b)
#else
static int compare(a, b)
state *a, *b;
#endif
{
  long arg1,arg2,i;
  arg1 = 0;
  arg2 = 0;
  FOR(i=0; i LT nbands_s; i++)
    arg1 += (int) (256 *  a->sum[i] /  a->len);
    arg2 += (int) (256 *  b->sum[i] /  b->len);
  ENDFOR
  if (arg1 LT arg2) return(-1);
  return(arg1 GT arg2);
}




 
/*L*

________________________________________________________________

		write_anwer
________________________________________________________________

Name:		write_answer
Syntax:		| static void write_answer(stat, gptr, image, band_size, 
                |         c_image, color_tab, min_col, max_col);
                | state *stat;
                | byte *image[], *c_image, *color_tab[];
                | long *gptr, band_size, min_col, max_col; 
Description:    
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
static void write_answer(stat, gptr, image, band_size,  
                c_image, color_tab, min_col, max_col, step_col)
  state * stat;
  byteimage image;
  intimage  color_tab;
  byteband  c_image;
  long *gptr, band_size, min_col, max_col, step_col; 
#else /* FUNCPROTO */
static void write_answer(state *stat, long int *gptr, unsigned char **image, long int band_size, byteband c_image, int **color_tab, long int min_col, long int max_col, long int step_col)
#endif /* FUNCPROTO */
{
  long c, i;
  byte *ptr;
  long ant, class[256];

  IN(h6);
# ifdef FUNCPROTO
  qsort(&stat[min_col], max_col-min_col+1, sizeof(state),
	(int (*)(const void*, const void*)) compare);
# else
  qsort(&stat[min_col], max_col-min_col+1, sizeof(state), compare);
# endif

  reconstr(stat, gptr, c_image, min_col, max_col);

  for(c=min_col; c LE max_col; c++) class[c] = c;
 
  FOR(c=min_col; c LE max_col; c++)
    for(i=0; i LT nbands_s; i++)
      color_tab[i][c] = 256 * (stat[c].sum[i] /  stat[c].len);
    ant = 0;
#ifdef SPLIT
    FOR(i = min_col; i LT c; i++)
      if (test_nabo(stat, color_tab, class, c ,i)) nabo[ant++] = i;
    ENDFOR 
#endif
  ENDFOR;
  ant = 0;
  FOR (c= min_col; c LE max_col; c++)
    if(c EQ class[c]) ant++;
    class[c] = (class[c]-min_col) * step_col + min_col;
  ENDFOR
  if (Verbose()) printf("%ld classes\n", ant);
  ptr = c_image;
  while (band_size--)
  {
    i = class[*ptr];
    *ptr++ = i;
  }

  OUT(t6_s,h6);
}
 



/*L*

________________________________________________________________

		syntese
________________________________________________________________

Name:		syntese
Syntax:		| static void syntese(image, bands, band_size, 
                |         c_image, color_tab, min_col, max_col, step_col);
                | byte *image[], *c_image, *color_tab[];
                | long bands, band_size, min_col, max_col, step_col; 
Description:    
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#ifndef FUNCPROTO
static void syntese(image, bands, band_size, c_image, color_tab, min_col, max_col, step_col, escape_codes)
  byteimage image;
  int *color_tab[];
  byteband c_image;
  long bands, band_size, min_col, max_col, step_col; 
  int escape_codes;
#else /* FUNCPROTO */
static void syntese(unsigned char **image, long int bands, long int band_size, byteband c_image, int **color_tab, long int min_col, long int max_col, long int step_col, int escape_codes)
#endif /* FUNCPROTO */
{
  long cur_col, *gptr, max, spl_col, plan;
  state *stat;

  nbands_s = bands;
  init_var(&stat, &gptr, min_col, max_col, band_size, nbands_s);
  cur_col = min_col;
#ifdef SPLIT
  splitt = Imake_image(1, "Split", 3, 256, 256);
  for (y=1; y<=256; y++) for(x=1; x<=256; x++) splitt[1][y][x] = 255;
#else
  if (Verbose()) printf("Treating color: %3ld\n", cur_col);
#endif
  statistic(&stat[min_col], image);

  while (TRUE) {
      max = find_max(stat, &spl_col, &plan, min_col, cur_col);
      if (max EQ 0) {
        max_col = cur_col;
        Warning(2, "The image had no more than %ld colors.\n", 
                max_col-min_col+1);
        break;
      }
      cur_col++;
#ifdef SPLIT     
      if (plan == 0) {
        for(y=stat[spl_col].minval[1]; y LE stat[spl_col].maxval[1]; y++)
         splitt[1][stat[spl_col].bs_val+1][256-y] = 0;
      } else {
        for(x=stat[spl_col].minval[0]; x LE stat[spl_col].maxval[0]; x++)
         splitt[1][x+1][256-stat[spl_col].bs_val] = 0;
      }
#endif
      split(image, gptr, &stat[spl_col], &stat[cur_col], plan);
#ifndef SPLIT
      /* Escape codes, for use when output is xterm. Does not work when
       * output is to xshow. The code will make each number be printed on top
       * of the previous one.
       */
      if (Verbose() && escape_codes) {
	printf("%c[D%c[D%c[D%3ld",27,27,27,cur_col);
	fflush(stdout);
      }
      else if (Verbose()) printf("%3ld\n", cur_col);
#endif
      statistic(&stat[spl_col], image);
      statistic(&stat[cur_col], image);
      EXITIF (cur_col GE max_col);
  } /* while (TRUE) */
  DUMP("Done \n",0,0,0,0);
  printf("\n");
  write_answer(stat, gptr, image, band_size,
               c_image, color_tab, min_col, max_col, step_col);
#ifdef SPLIT
  unlink ("split.img");
  Iwrite_image(splitt,"split.img");
#endif
}



static IMAGE inb_s, outb_s;
static byte *b_s[MAXBANDS];
static int *coltab_s[MAXBANDS];
static long xsize_s, ysize_s, size_s;


/* ---------------------------------------------------- */
/*							*/
/*	Proc	: static void read_file			*/
/*	Return  : 					*/
/*      Params	: None					*/
/*							*/

#ifndef FUNCPROTO
static void read_file(min_col, max_col)
long min_col, max_col;
#else /* FUNCPROTO */
static void read_file(long int min_col, long int max_col)
#endif /* FUNCPROTO */
{
  long bands, i;
  int *col;

  inb_s  = Iread_image(infile_s);
  if (inb_s == 0) {
    Error(2, "Can't open file %s\n",infile_s);
    exit(2);
  }
  bands = Inbands(inb_s);
  FOR (i=1; i<=bands; i++)
    IF (nbands_s EQ 0)
      xsize_s = Ixsize(inb_s[1]);
      ysize_s = Iysize(inb_s[1]);
      size_s  = xsize_s*ysize_s;
    ENDIF
    b_s[nbands_s] = &inb_s[i][1][1];
    IF (Ixsize(inb_s[i]) NE xsize_s OR Iysize(inb_s[i]) NE ysize_s)
      Error(2, "All bands in input image %s must be of equal size\n",infile_s);
      exit(2);
    ENDIF
    IF (Ipixtyp(inb_s[i]) NE 3)
      Error(2, "All bands in input image %s must be of type unsigned byte\n",
	    infile_s);
      exit(2);
    ENDIF
    col = (int *) malloc((max_col - min_col +1) * sizeof(int));
    coltab_s[nbands_s] = &col[-min_col];

#ifdef TOTAL
    tsize = xsize_s*ysize_s;
    ptr = & inb_s[i][1][1];
    while (tsize--) tot += *ptr++;
#endif
    nbands_s++;
  ENDFOR     
  DUMP("Address %d %d %d\n",&inb_s[1][1][1],&inb_s[2][1][1],&inb_s[3][1][1],0);
#ifdef TOTAL
   printf("Input  image = %d --> %10.2f \n",
	  tot, (float) tot / 512.0 / 512.0);
#endif
}



/*P:colorquant*

________________________________________________________________

		colorquant
________________________________________________________________

Name:		colorquant - qolor quantization and rgb to pseudocolor
                conversion

Syntax:		| colorquant [<option>...] <inimage> <colorfile> <outimage>

Description:	Performs color quantization of 'inimage'. An 'outimage' and
                a corresponding 'colorfile' are produced.

		The 'colorfile' is either a BIFF colortable file or an ascii
		colormap file (depending on options used). The values in
		'outimage' should be used as indices to the
		colortable/colormap.

		If 'inimage' is a true-color image (three bands), then
		'outimage' will be the pseudocolor version of 'inimage'.

		If 'colorfile' is an ascii colormap file and 'inimage' is
		true-color, the map-table may be transformed into a colortable
		by 'mct'.

Options:        &-b start
                Lowest color in output map. Default zero.

		&-s step
		'Step' between each color. Default one.

		&-e stop
		Highest color in output map. Default 255.

		&-c
		Produce BIFF colortable file instead of ascii map-file.
		This is only allowed if 'inimage' is a three-band image,
		regarded as a true color image.

		&-ct title
		'Title' for colortable in 'colorfile'. Default: Title in
		'inimage'.

		&-t title
		'Title' for image in 'outimage'. Default: Title in 'inimage'.

		&-verbose
		Print info

		-esc
		Don''t use escape codes when printing to xterm on verbose.

See also:	makepseudo(1), mct(1), color2ascii(1), xshow(1)

Reference:	Otto Milvang: An adaptive algorithm for color quantization.
                Proceedings of the 5th Scandinavian Conference on Image
                Analysis.

Author:		Otto Milvang
Revised:        Svein Bøe

Examples:	| colorquant -b 10 -e 253 rgb.img m.map pseudo.img
                | mct m.map m.col
		| xshow -ct 256 m.col pseudo.img
		| 
	        | colorquant -b 10 -s 2 -e 253 -c rgb.img m.col pseudo.img
		| xshow  m.col pseudo.img

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  long i, c, min_col, max_col, step_col, col_res;
  FILE *cf;
  char *ctitle, *title;
  int escape_codes;

  INIT
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> <colorfile> <outimage> \n\
      where <option> is one of \n\
        -b <start>  : Lowest color in output \n\
        -s <step>   : Color step in output \n\
        -e <stop>   : Highest color in output \n\
        -esc        : Don't use escape codes when printing on verbose \n\
        -c          : Make BIFF colortable file instead of ascii map-file \n\
        -ct <title> : Title for colortable \n\
        -l <limit>  : Limit \n\
        -t <title>  : Title for image in <outimage> \n\
        -verbose    : Print information during processing\n"));

  Iset_abort(TRUE);
  Iset_message(TRUE);

  if (argc == 1) Usage(1, NULL);

  min_col      = read_iswitch(&argc, argv, "-b", 0);
  max_col      = read_iswitch(&argc, argv, "-e", 255);
  step_col     = read_iswitch(&argc, argv, "-s", 1);
  limit_s      = read_iswitch(&argc, argv, "-l", 1);
  col_res      = read_bswitch(&argc, argv, "-c");
  ctitle       = read_switch( &argc, argv, "-ct", 1, NULL);
  title        = read_switch( &argc, argv, "-t",  1, NULL);
  escape_codes = !read_bswitch(&argc, argv, "-esc");

  if (argc != 4) Usage(2, "Illegal number of arguments.\n");
  infile_s    = argv[1];
  colorfile_s = argv[2];
  outfile_s   = argv[3];

  nbands_s = 0;
  max_col = (max_col - min_col) / step_col + min_col;

  read_file(min_col, max_col);

  if (col_res) {
    if (Inbands(inb_s) != 3)
      Error(1, "Option -c requires three-band input image.\n");
  } 

  if (ctitle == NULL) ctitle = Ititle(inb_s);
  if (title  == NULL) title  = Ititle(inb_s);

  outb_s = Imake_image(1, title, 3, xsize_s, ysize_s);

  syntese(b_s, nbands_s, size_s, &outb_s[1][1][1],
	  coltab_s, min_col, max_col, step_col, escape_codes);

  Iwrite_image(outb_s, outfile_s);

  if (!col_res) {
    /* Ascii map-file. */

    cf = fopen(colorfile_s,"w");

    fprintf(cf, "# %s\n", ctitle);

    for (c=min_col; c <= max_col; c++) {
      fprintf(cf, "%8ld", min_col + (c - min_col) * step_col);

      for(i=0; i LT nbands_s; i++)
        fprintf(cf, " %5d", coltab_s[i][c]);

      fprintf(cf,"\n");
    }
    fclose(cf);
  } else {
    /* Binary BIFF colortable file. */

    Color_cell *ctab;

    ctab = (Color_cell *) calloc(max_col - min_col + 1, sizeof(Color_cell));

    for (c = min_col; c <= max_col; c++) {
      ctab[c].pixel = min_col + (c - min_col) * step_col;
      ctab[c].red   = coltab_s[0][c];
      ctab[c].green = coltab_s[1][c];
      ctab[c].blue  = coltab_s[2][c];
    }

    color_write(colorfile_s, ctab, ctitle, (max_col - min_col + 1));
  }
 
  if (Verbose()) {
    REPORT("Histogram    :",t1_s);
    REPORT("Split_arr    :",t2_s);
    REPORT("Initvar      :",t3_s);
    REPORT("Statistics   :",t4_s);
    REPORT("Find_max     :",t5_S);
    REPORT("Save exit    :",t6_s);
  }
  return(0);
}

#endif /* MAIN */
