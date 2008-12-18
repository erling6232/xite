
/*C*

________________________________________________________________

        thn_har.c
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


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_FCNTL_H
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/negate.h>
#include <xite/thin.h>



/*********************************************************************** */
/*                                                                       */
/*   This program computes the minimum and maximum lengths along         */
/*   arbitrary directions of a given binary Connected Component.         */
/*   For the Firestone project, the lengths will be used in determining: */
/*     1. whether the CC has a length along any direction that           */
/*        exceeds the requirement                                        */
/*     2. whether the object possesses elongatedness (high aspect        */
/*        ratio).                                                        */
/*                                                                       */
/*   Methodology:                                                        */
/*                                                                       */
/*     Given a binary image within a window (MINREC of a Connected       */
/*     Component) where the background points are assigned value         */
/*     zero and interior points of a CC are assigned a big distance      */
/*     value. A 2D Chamfering is performed to obtain a distance map      */
/*     where each pixel value represents the closest distance to the     */
/*     background. A thinning operation enables us to derive a ridge     */
/*     along which the distance values are collected and analyzed.       */
/*     The maximum length of the object is approximated by               */
/*     the length of the skeleton plus the two distance values on        */
/*     the two ending points of the skeleton. If the standard            */
/*     deviation of the collected distance values is small,              */
/*     (2* average-distance) is used to approximate the width            */
/*     of the object. Otherwise, the estimated average width should      */
/*     not be used to approximate the width of the object.               */
/*                                                                       */
/*************************************************************************/

#define ELE_SIZE 3
#define E_ROWS   ELE_SIZE
#define E_COLS   ELE_SIZE
#define NO_ELE   16

/****************** 16 structuring element used for thinning ****************/

static int har_str_ele_s[16][3][3]=
{{{255, 255, 255}, {  0, 255,   0}, {  0,   0,   0}}, /*J1*/
 {{  0,   0,   0}, {  0,   0,   0}, {255, 255, 255}}, /*K1*/
    
 {{  0, 255,   0}, {  0, 255, 255}, {  0,   0,   0}}, /*J2*/
 {{  0,   0,   0}, {255,   0,   0}, {255, 255,   0}}, /*K2*/
    
 {{255,   0,   0}, {255, 255,   0}, {255,   0,   0}}, /*J3*/
 {{  0,   0, 255}, {  0,   0, 255}, {  0,   0, 255}}, /*K3*/
    
 {{  0, 255,   0}, {255, 255,   0}, {  0,   0,   0}}, /*J4*/
 {{  0,   0,   0}, {  0,   0, 255}, {  0, 255, 255}}, /*K4*/
    
 {{  0,   0, 255}, {  0, 255, 255}, {  0,   0, 255}}, /*J5*/
 {{255,   0,   0}, {255,   0,   0}, {255,   0,   0}}, /*K5*/
    
 {{  0,   0,   0}, {255, 255,   0}, {  0, 255,   0}}, /*J6*/
 {{  0, 255, 255}, {  0,   0, 255}, {  0,   0,   0}}, /*K6*/
    
 {{  0,   0,   0}, {  0, 255,   0}, {255, 255, 255}}, /*J7*/
 {{255, 255, 255}, {  0,   0,   0}, {  0,   0,   0}}, /*K7*/
    
 {{  0,   0,   0}, {  0, 255, 255}, {  0, 255,   0}}, /*J8*/
 {{255, 255,   0}, {255,   0,   0}, {  0,   0,   0}}};/*K8*/



static int har_dif_img (IBAND inband, IBAND other_band, IBAND negated) 
{
   int r, c;
   int temp ;
   int rows, cols;
   
   rows = Iysize(inband);
   cols = Ixsize(inband);
   
   for (r=1; r<=rows; r++)
      for (c=1; c<=cols; c++)
      {   
	 temp = inband[r][c] - other_band[r][c];
	 inband[r][c] = temp ;
	 negated[r][c] = THN_BACKGROUND - temp ;
      } 
   return THN_OK;
   
}   /*  End of har_dif_img()  */



static int har_val_ran(int row, int col, int lr, int hr, int lc, int hc) 
{
    if ( (row < lr) || (row > hr) || (col < lc) || (col > hc) )
         return 0 ;
    else return 1 ;

}   /*   End of har_val_ran()  */



static int har_hit_and_mis (IBAND inband, IBAND negated, int J, int K, IBAND H_M_band, int *empty, IBAND hit_band, IBAND miss_band) 
{
   int r, c, l, m ;
   int flag ;
   int rows, cols;
   
   rows = Iysize(inband);
   cols = Ixsize(inband);
   
   /*  HIT operation */
   
   for (r=1; r<=rows; r++)
      for (c=1; c<=cols; c++)
      {   
	 flag = 1 ;
	 for (l=0; l<E_ROWS; l++)
	    for (m=0; m<E_COLS; m++)
	    {    
	       if ((har_str_ele_s[J][l][m] == 255) &&
		   (har_val_ran(r+l-1, c+m-1,
				1, rows, 1, cols)))    
		  if (inband[r+l-1][c+m-1] == 0) 
		     flag = 0 ;
	    }
	 if (flag) 
	    hit_band[r][c] = 255 ;
	 else 
	    hit_band[r][c] = 0 ;
      }
   
   /*  MISS operation */
   
   for (r=1; r<=rows; r++)
      for (c=1; c<=cols; c++)
      {   flag = 1 ;
	  for (l=0; l<E_ROWS; l++)
	     for (m=0; m<E_COLS; m++)
	     {    if ( (har_str_ele_s[K][l][m] == 255) &&
		      (har_val_ran(r+l-1, c+m-1, 1, rows, 1, cols)))
		     if (negated[r+l-1][c+m-1] == 0) 
			flag = 0 ;
	       }
	  if (flag)
	     miss_band[r][c] = 255 ;
	  else 
	     miss_band[r][c] = 0 ;
       }
   
   /* AND operation */
   
   flag = 1 ;
   for (r=1; r<=rows; r++)
      for (c=1; c<=cols; c++)
	 if ( (hit_band[r][c] == 255) && (miss_band[r][c] == 255) )
	 {  
	    H_M_band[r][c] = 255 ; 
	    flag = 0 ; 
	 } 
	 else 
	    H_M_band[r][c] = 0 ;
   
   empty[0] = flag ;

   return THN_OK;   
   
}  /*  End of hit_and_miss()  */



static int har_situation_i (IBAND inband, IBAND negated, int idx, int *update, IBAND H_M_band, IBAND hit_band, IBAND miss_band)
{
   int   empty ;
   int   J_idx, K_idx ;

   J_idx = idx*2 ; 
   K_idx = J_idx + 1 ;
   
   /* empty=1  iff  (A-J)^(>A-K) == 0  (no change)
      empty=0  iff  (A-J)^(>A-K) != 0 (change exits) 
      */
   
   update[0] = 0 ;
   har_hit_and_mis (inband, negated, J_idx, K_idx, H_M_band, &empty,
		    hit_band, miss_band) ;
   if (!empty) {   
     update[0] = 1 ; 
     har_dif_img (inband, H_M_band, negated) ;
   }
   
   return THN_OK;
   
}   /*  End of har_situation_i()  */



static int har_one_pas (IBAND inband, int *not_finished, IBAND negated, IBAND H_M_band, IBAND hit_band, IBAND miss_band)
{
   int   i;
   int   update_flag;
   int   verbose;

   negate(inband, negated);

   verbose = Verbose();
   not_finished[0] = 0;
   for (i=0; i<8; i++) {   
     if (verbose) printf("Direction %i\n", i);
     har_situation_i (inband, negated, i, &update_flag, 
		      H_M_band, hit_band, miss_band) ;

     if (update_flag) not_finished[0] = 1;
   }
   
   return THN_OK;
}  /* End of har_one_pas()  */



/*F:thn_har*

________________________________________________________________

		thn_har
________________________________________________________________

Name:		thn_har - thinning of binary image using Haralick and 
                Shapiro''s morphological thinning.

Syntax:         | #include <xite/thin.h>
                | 
                | int thn_har( IBAND inband );

Description:    Morphological thinning according to Haralick and Shapiro.

Restrictions:   'inband' must have pixel type unsigned byte. 'inband' is
                assumed to be binary valued, with foreground = 0,
		background = 255.

Return value:   | 0 - Ok
                | 1 - Bad input pixel type

See also:       thn_har(1), thn_lc(3), thn_zs(3)

Reference:	&R. M. Haralick and L. G. Shapiro,
                "Computer and Robot Vision",
                Vol. 1, Chapter 5 (especially 5.10.1),
		Addison-Wesley, Reading, Massachusetts, 1992,
		
Files:	        xite/src/thin/thn_har.c
Author:		Qian Huang (Michigan State University), 
                Øivind Due Trier (University of Oslo).
________________________________________________________________

*/

int thn_har (IBAND inband) 
{
  int not_finished;
  int pass_no;
  char filename[15];
  IBAND negated;
  IBAND H_M_band ;
  IBAND hit_band;
  IBAND miss_band;
  int   rows, cols, verbose;
  
  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "thn_har: Input pixel type must be unsigned byte.\n"));
  
  strcpy (filename,"HAR_pass0.img\0");
  
  rows = Iysize(inband);
  cols = Ixsize(inband);
  
  negated   = (IBAND)Imake_band(Iu_byte_typ, cols, rows);
  H_M_band  = (IBAND)Imake_band(Iu_byte_typ, cols, rows);
  hit_band  = (IBAND)Imake_band(Iu_byte_typ, cols, rows);
  miss_band = (IBAND)Imake_band(Iu_byte_typ, cols, rows);
  
  pass_no      = 0;
  not_finished = 1;
  verbose      = Verbose();
  while (not_finished) {
    pass_no++;
    if (verbose) printf("Pass number %i\n", pass_no);
    har_one_pas (inband, &not_finished, negated,
		 H_M_band, hit_band, miss_band) ; 
    filename[8] = (char)(pass_no + '0');
    /* 
       write_byteband(inband, filename);
       */
  }
  Idel_band(&negated);
  Idel_band(&H_M_band);
  Idel_band(&hit_band);
  Idel_band(&miss_band);
  
  return THN_OK;

}  /*  End of thn_har()  */
