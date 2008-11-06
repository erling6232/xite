

/*C*

________________________________________________________________

        mallocp
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




/*F:MallocFloatP=MallocIntP*/
/*F:MallocIntP*

________________________________________________________________

		MallocIntP
________________________________________________________________

Name:		MallocFloatP, MallocIntP - Allocate memory for a number
                and return pointer
Syntax:		| #include <xite/mallocp.h>
		| 
		| int *MallocIntP( int i );
		| int *MallocFloatP( int i );
Description:	Allocates memory for an integer or a float and returns a
                pointer to it.
                Can be used e.g. to supply the "closure" argument to the
		XtAddCallback() procedure.
Diagnostics:	As for malloc(3).
Return value:	Pointer to the allocated number or NULL if allocation failed.
Author:		Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/mallocp.h>

int *MallocIntP(int i)
{
  int *pi;
  
  pi = (int *) malloc(sizeof(i));
  if (pi) *pi = i;
  
  return(pi);
} /* MallocIntP() */

float *MallocFloatP(float f)
{
  float *pf;
  
  pf = (float *) malloc(sizeof(f));
  if (pf) *pf = f;
  
  return(pf);
} /* MallocFloatP() */
