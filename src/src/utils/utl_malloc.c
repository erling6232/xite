/* $Id$ */
/*F:Mmatrix_3d=utl_malloc*/
/*F:Mmatrix_2d=utl_malloc*/
/*F:Mmatrix_1d=utl_malloc*/
/*F:Fmatrix_3d=utl_malloc*/
/*F:Fmatrix_2d=utl_malloc*/
/*F:Fmatrix_1d=utl_malloc*/
/*F:utl_malloc*

________________________________________________________________

		utl_malloc
________________________________________________________________

Name:		utl_malloc, Mmatrix_3d, Fmatrix_3d, Mmatrix_2d, Fmatrix_2d,
                Mmatrix_1d, Fmatrix_1d - allocates memory for various purposes
Syntax:         | #include <xite/utl_malloc.h>
                |
                | void *Mmatrix_1d( int i_first, int i_last,
                |    int i_element_size, int i_clear );
                | void *Mmatrix_2d( int i_row_min, int i_row_max,
                |    int i_col_min, int i_col_max,
                |    int i_element_size, int i_clear );
                | void *Mmatrix_3d( int i_x_min, int i_x_max,
                |    int i_y_min, int i_y_max, int i_z_min,
                |    int i_z_max, int i_element_size,
                |    int i_clear );
                | void *Fmatrix_1d( char* array );
                | void *Fmatrix_2d( char* i_matrix,
                |    char* i_rows );
                | void *Fmatrix_3d( char* i_cube, char* i_matrix,
                |    char* i_rows );
Description: These are various memory (de)allocation routines, all of them 
  using malloc (or calloc).   'element_size' is the size of the elements to
  be allocated; i.e sizeof(int) for integers. 'clear' is one if the 
  array or matrix is to be filled by  zeroes.

  Mmatrix_1d returns a pointer to an array of (last - start + 1) elements.
  After the call 
  | a = Mmatrix_1d(1,4,sizeof(int),1)
  a[1] will be the first integer element of the array, a[4] will be the
  last. All elements will be initialized to 0.

  Mmatrix_2d will return a pointer to a two-dimensional 
  rectangular matrix allocated
  in one chunk of memory. 'row_start' and 'row_last' indicates first and last 
  element of the rows. 

  Mmatrix_3d will return a pointer to a three-dimensional rectangular cube
  allocated in one chunk of memory. 'x_start', 'x_last', 'y_start', 'y_last',
  'z_start' and 'z_last' gives the dimension of the cube.

  Fmatrix_1d frees the memory allocated by Mmatrix_1d. The adress of the
  first dataelement must be supplied.

  Fmatrix_2d frees the memory a||ocated by Mmatrix_2d. Parameters
  are adress of first matrix element, and adress of first row. See
  the example section for usage.

  Fmatrix_3d frees the memory allocated by Mmatrix_3d. Parameters are adress
  of first cube element, adress of first matrix element, and adress of first
  row.

Return value: A suitable pointer  is returned. If no storage 
  was available, or if routine deallocates memory, returns NULL.
  All Fmatrix routines return NULL.
Examples:  
  | double **a; 
  |    Allocte room for a [0..2][0..8] double
		|    precision matrix
  | a = Mmatrix_2d(0,2,0,8,sizeof(double),1);
  |    Free a.
  | a = Fmatrix_2d( &a[0][0], &a[0]);
Linking: cc progname.c -lutl
Restrictions: Assumes all pointers have the same size (ok on decstations 
  and suns).
Hints: Always use row_start and col_start = 0: It will save you problems in 
  the long run. If you insist on offsets, remember to use 
  &array[row_start, col_start] if the address of the first element is needed.
Bugs: Compilers and lint might complain about mixing of data-types. Just
  ignore them.
Author:		gutorm hogasen, NCC
See Also: malloc(3), free(3), calloc(3)
Date: Last update 7/1/91
File: $Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/utl_malloc.h>
#include XITE_STDIO_H
#include XITE_MALLOC_H

#ifndef FUNCPROTO
void *Mmatrix_1d(  i_first,
		   i_last,
		   i_element_size,
		   i_clear)
int i_first, i_last, i_element_size, i_clear;
#else /* FUNCPROTO */
void *Mmatrix_1d(int i_first, int i_last, int i_element_size, int i_clear)
#endif /* FUNCPROTO */
{
  int elements;
  char *a;			/* because we will do pointer-arithmetic */

  elements = i_last - i_first + 1;
  if (i_clear)
    a =  (char *) calloc( (unsigned)elements, (unsigned)i_element_size );
  else
    a =  (char *) malloc( (unsigned) elements * i_element_size );
  if (!a)
    {
      fprintf(stderr," Allocation failure in Mmatrix_1d()\n");
      return(NULL);
    }
		/* move the pointer backwards so a[i_first] is first element*/
  a -= i_first * i_element_size;
  return a;
}				/* end of Mmatrix_1d */

#ifndef FUNCPROTO
void *Mmatrix_2d(i_row_min,
		 i_row_max,
		 i_col_min,
		 i_col_max,
		 i_element_size,
		 i_clear)
int i_row_min, i_row_max, i_col_min, i_col_max, i_element_size, i_clear;
#else /* FUNCPROTO */
void *Mmatrix_2d(int i_row_min, int i_row_max, int i_col_min, int i_col_max, int i_element_size, int i_clear)
#endif /* FUNCPROTO */
{
  int i, cols, rows;
  char **m;			/* because we will do pointer-arithmetic */
  char *p;			

  cols = i_col_max - i_col_min + 1;
  rows = i_row_max - i_row_min + 1;
  /* Allocate pointers to rows. Allocate one more row to be able to NULL
     terminate it.  */
  m = (void *) malloc( (unsigned) (rows +1) * sizeof(void *) );
  if (!m)
    {
      fprintf(stderr," Allocation failure 1 in Mmatrix_2d()\n");
      return(NULL);
    }

  /* Allocate the matrix */
  if (i_clear)
    *m = (char *) calloc( (unsigned) rows * cols, (unsigned) i_element_size );
  else
    *m =  (char *) malloc( (unsigned)rows * cols * i_element_size );
  if (!*m)
    {
      fprintf(stderr," Allocation failure 2 in Mmatrix_2d()\n");
      return(NULL);
    }

  p = *m;			/* p is current row */
  m -= i_row_min;
 
  /* Initialize rowpointers */
  for (i = i_row_min; i <= i_row_max; i++, p += cols*i_element_size)
  {
    m[i] = p;
    m[i] -= i_col_min*i_element_size;
  }
  /* NULL terminated rowpointer_array: This will crash the program if
     m[i_row_max + 1][x] is adressed.
  */
  m[i_row_max + 1] = NULL;

  /* Return pointer to array of pointers to rows */
  return m;
}				/* end of Mmatrix_2d */

#ifndef FUNCPROTO
void *Mmatrix_3d(i_x_min,
		 i_x_max,
		 i_y_min,
		 i_y_max,
		 i_z_min,
		 i_z_max,
		 i_element_size,
		 i_clear)
int i_x_min, i_x_max, i_y_min, i_y_max, i_z_min, i_z_max, i_element_size,
    i_clear;
#else /* FUNCPROTO */
void *Mmatrix_3d(int i_x_min, int i_x_max, int i_y_min, int i_y_max, int i_z_min, int i_z_max, int i_element_size, int i_clear)
#endif /* FUNCPROTO */
{
  int i, j, xs, ys, zs;
  char ***m;			/* because we will do pointer-arithmetic */
  char *p;

  xs = i_x_max - i_x_min + 1;
  ys = i_y_max - i_y_min + 1;
  zs = i_z_max - i_z_min + 1;

/* Allocate pointers to lines in the z-direction. This is done by making a
   xs*ys matrix of pointers. */
  m = Mmatrix_2d(i_x_min,i_x_max+1,i_y_min,i_y_max+1,sizeof(void *),0);

  if (!m) {
    fprintf(stderr, "Allocation failure 1 in Mmatrix_3d()\n");
    return(NULL);
  }

/* Allocate the cube: */
  if (i_clear)
    m[i_x_min][i_y_min] = (char *) calloc((unsigned) xs*ys*zs, (unsigned) i_element_size);
  else
    m[i_x_min][i_y_min] = (char *) malloc((unsigned) xs*ys*zs*i_element_size);

  if (!m) {
    fprintf(stderr, "Allocation failure 2 in Mmatrix_3d()\n");
    return(NULL);
  }

  p = m[i_x_min][i_y_min];

  for (i = i_x_min; i<= i_x_max; i++)
    for (j = i_y_min; j<=i_y_max; j++) {
      m[i][j] = p;
      m[i][j] = m[i][j] - i_z_min * i_element_size;
      p = p + zs*i_element_size;
    }

  for (i=i_x_min; i<=i_x_max; i++)
    m[i][i_y_max+1] = NULL;

  for (i=i_y_min; i<=i_y_max; i++)
    m[i_x_max+1][i] = NULL;

  return m;
}                                /*End of Mmatrix_3d */


#ifndef FUNCPROTO
void *Fmatrix_1d(array)
char *array;
#else /* FUNCPROTO */
void *Fmatrix_1d(char *array)
            
#endif /* FUNCPROTO */
/*
  Frees memory allocated by Mmatrix_1d. Returns 0.
  Example: 
     double *m;
     m = Mmatrix_1d(0,5,,sizeof(double),0);
     Fmatrix_1d( &m[0] );
*/
{
  free(array);
  return 0;
}				/* end of Fmatrix_2d */

#ifndef FUNCPROTO
void *Fmatrix_2d( i_matrix,
	       i_rows)
char * i_matrix, *i_rows;
#else /* FUNCPROTO */
void *Fmatrix_2d(char *i_matrix, char *i_rows)
                         
#endif /* FUNCPROTO */

/*
  Frees memory allocated by Mmatrix_2d. Returns 0.
  Example: 
     double **m;
     m = Mmatrix_2d(0,5,0,5,sizeof(double),0);
     Fmatrix_2d( &m[0][0], &m[0] );
*/
{
  free( i_matrix );
  free( i_rows);
  return 0;
}				/* end of Fmatrix_2d */


#ifndef FUNCPROTO
void *Fmatrix_3d(i_cube,
	       i_matrix,
	       i_rows)
char * i_cube, *i_matrix, *i_rows;
#else /* FUNCPROTO */
void *Fmatrix_3d(char *i_cube, char *i_matrix, char *i_rows)
                                  
#endif /* FUNCPROTO */
/* Frees memory allocated by Mmatrix_3d. Returns 0.
   Example:
      double ***m;
      m = Mmatrix_3d(0,5,0,5,0,5,sizeof(double),0);
      m = Fmatrix_3d(&m[0][0][0], &m[0][0],
                                 &m[0]);
*/
{
  free(i_cube);
  free(i_matrix);
  free(i_rows);

  return 0;
}                                /* end of Fmatrix_3d */

#ifdef TESTING
int main()
{
  double ***a, *p;
  int i,j,k,n;
  printf("Allokverer en 3x5x4 matrise\n"),
  a = Mmatrix_3d(2,4,1,5,3,6,sizeof(double),0);
  
  for ( i = 2, n = 1; i <= 4; i++)
    for ( j = 1; j <=5; j++)
      for (k = 3; k <= 6; k++, n++)
	a[i][j][k] = n/10.0;
  
  for (p = &a[2][1][3], i = 2; i <=4; i++, printf("\n"))
    for ( j = 1; j <=5; j++, printf("\n"))
      for ( k=3; k <=6; k++, p++)
	printf("%g ",*p);

  printf("\n");
  Fmatrix_3d( &a[2][1][3], &a[2][1], &a[2]);
  return(0);
}
#endif
