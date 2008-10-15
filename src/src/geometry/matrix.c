

/*C*

________________________________________________________________

        matrix
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
#include <xite/includes.h>
#include <stdlib.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/message.h>
#include "matrix.h"

#define MAXTEMP 2000
#define NULLGRENSE 0.0000001
#define M_elem(M,I,J) M->elem[(I)*(M->cols)+(J)]

static xiteMatrix M_tempmat[MAXTEMP];
static int M_tempant = 0;

int MRows(xiteMatrix m)
{
  return(m->rows);
}

int MCols(xiteMatrix m)
{
  return(m->cols);
}

static void MError(char *s)
{
  Error(3, "matrix: %s\n", s);
  exit(1);
}

static xiteMatrix M_new(int l, int s)
{
  xiteMatrix m;

  m = (xiteMatrix) malloc(sizeof(xiteMatrixRec));
  m->elem = (M_elemtype *) calloc(l*s, sizeof(M_elemtype));
  m->rows = l;
  m->cols = s;
  if (M_tempant < MAXTEMP) M_tempmat[M_tempant++] = m;

  return m;
}

static int MMerkTemp(void)
{
  return M_tempant; 
}

static void MNoTemp(xiteMatrix a)
{
  int i;

  for (i=0; i<M_tempant; i++) 
    if (M_tempmat[i] == a) {
      M_tempmat[i] = M_tempmat[--M_tempant];
      break;
    }
}

void MFree(xiteMatrix a)
{
  MNoTemp(a);
  free(a->elem);
  free(a);
}

static void MFreeTemp(int tempstart)
{
  int i;

  for (i=tempstart; i<M_tempant; i++) MFree(M_tempmat[i]);
  M_tempant = tempstart;
}

static void MShowTemp(int detail)
{
  int i, b = 0;

  for (i=0; i<M_tempant; i++) 
    b += M_tempmat[i]->rows * M_tempmat[i]->cols;

  printf("Temp: #mat=%d #elem=%d\n", M_tempant, b);

  if (detail) {
    for (i=0; i<M_tempant; i++) 
      printf("%d: %dx%d\n", i, M_tempmat[i]->rows, M_tempmat[i]->cols);
    printf("\n");
  }
}


xiteMatrix MData(int l, int s, void *data)
{
  xiteMatrix m;

  m = (xiteMatrix) malloc(sizeof(xiteMatrixRec));
  m->rows = l;
  m->cols = s;
  m->elem = (M_elemtype *) data;

  return m;
}

static xiteMatrix MNull(int l, int s)
{
  return M_new(l,s);
}

static xiteMatrix MIdent(int l, int s)
{
  xiteMatrix m;
  int i, k;

  m = MNull(l,s);
  k = l < s ? l : s;

  for (i=0; i<k; i++) M_elem(m,i,i) = 1;

  return m;
}

static xiteMatrix MKonst(int l, int s, double v)
{
  xiteMatrix m;
  int i,j;

  m = MNull(l,s);
  for (i=0; i<l; i++) 
    for (j=0; j<s; j++) 
      M_elem(m,i,j) = v;

  return m;
}

static xiteMatrix MTransp(xiteMatrix a)
{
  xiteMatrix m;
  int i,j;

  m = MNull(a->cols, a->rows);
  for (i=0; i<a->cols; i++)
    for (j=0; j<a->rows; j++)
      M_elem(m, i, j) = M_elem(a, j, i);

  return m;
}

static xiteMatrix MAdd(xiteMatrix a, xiteMatrix b)
{
  xiteMatrix m;
  int i,j;

  if (a->rows!=b->rows || a->cols!=b->cols)
    MError("MAdd: Wrong dimensions");

  m = MNull(a->rows, a->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      M_elem(m,i,j) = M_elem(a,i,j) + M_elem(b,i,j);

  return m;
}

static xiteMatrix MSub(xiteMatrix a, xiteMatrix b)
{
  xiteMatrix m;
  int i,j;

  if (a->rows!=b->rows || a->cols!=b->cols)
    MError("MAdd: Wrong dimensions");

  m = MNull(a->rows, a->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      M_elem(m,i,j) = M_elem(a,i,j) - M_elem(b,i,j);

  return m;
}

xiteMatrix MMult(xiteMatrix a, xiteMatrix b)
{
  xiteMatrix m;
  int i,j,k;

  if (a->cols != b->rows) MError("MMult: Wrong dimensions");

  m = MNull(a->rows, b->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<b->cols; j++)
      for (k=0; k<a->cols; k++)
	M_elem(m,i,j) += M_elem(a,i,k) * M_elem(b,k,j);

  return m;
}

static xiteMatrix MRealMult(xiteMatrix a, double c)
{
  xiteMatrix m;
  int i,j;

  m = MNull(a->rows, a->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      M_elem(m,i,j) = c * M_elem(a,i,j);

  return m;
}

static xiteMatrix MSubMatrix(xiteMatrix a, int i1, int i2, int j1, int j2)
{
  xiteMatrix m;
  int i,j;

  if (i1>i2 || j1>j2) MError("MSubMatrix: Wrong section");

  m = MNull(i2-i1, j2-j1);
  for (i=i1; i<i2 ; i++)
    for (j=j1; j<j2; j++)
      M_elem(m,i-i1,j-j1) = M_elem(a,i,j);

  return m;
}
  
static xiteMatrix MRowConcat(xiteMatrix a, xiteMatrix b)
{
  xiteMatrix m;
  int i,j;

  if (a->cols != b->cols) MError("MRowConcat: Wrong dimensions");

  m = MNull(a->rows+b->rows, a->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      M_elem(m,i,j) = M_elem(a,i,j);

  for (i=0; i<b->rows; i++)
    for (j=0; j<b->cols; j++)
      M_elem(m,a->rows+i,j) = M_elem(b,i,j);

  return m;
}

static xiteMatrix MColConcat(xiteMatrix a, xiteMatrix b)
{
  xiteMatrix m;
  int i,j;

  if (a->rows != b->rows) MError("MColConcat: Wrong dimensions");

  m = MNull(a->rows, a->cols+b->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      M_elem(m,i,j) = M_elem(a,i,j);

  for (i=0; i<b->rows; i++)
    for (j=0; j<b->cols; j++)
      M_elem(m,i,a->cols+j) = M_elem(b,i,j);

  return m;
}

static double MNorm(xiteMatrix a)
{
  int i,j;
  double s = 0;

  if (a->rows!=1 && a->cols!=1) MError("MNorm: Matrix not vector");

  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      s += (M_elem(a,i,j)*M_elem(a,i,j)) ;

  return sqrt(s);
}

static double MNorm2(xiteMatrix a)
{
  int i,j;
  double s = 0;

  if (a->rows!=1 && a->cols!=1) MError("MNorm2: Matrix not vector");

  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      s += (M_elem(a,i,j) * M_elem(a,i,j));

  return s;
}

static xiteMatrix MIMinus(xiteMatrix a)
{
  xiteMatrix m;
  int i,j;

  m = MNull(a->rows,a->cols);
  for (i=0; i<a->rows; i++)
    for (j=0; j<a->cols; j++)
      if (i == j) M_elem(m,i,j) = 1 - M_elem(a,i,j);
      else M_elem(m,i,j) = - M_elem(a,i,j);

  return m;
}

xiteMatrix MPseudoInv(xiteMatrix A)
{
  xiteMatrix Ap;
  int tempstart;

  tempstart = MMerkTemp();
  if (A->cols == 1) {
    int i;
    double v = 0;

    for (i=0; i<A->rows; i++) v += (M_elem(A,i,0) * M_elem(A,i,0));
    if (fabs(v) < NULLGRENSE) Ap = MTransp(A);
    else Ap = MRealMult(MTransp(A),1/v);
  }
  else {
    xiteMatrix Ak_1, Ak_1p, ak, pk, pk_teller;
    double pk_nevner;

    Ak_1 = MSubMatrix(A, 0, A->rows, 0, A->cols-1);
    Ak_1p = MPseudoInv(Ak_1);
    ak = MSubMatrix(A, 0, A->rows, A->cols-1, A->cols);
    pk_teller = MMult(MIMinus(MMult(Ak_1,Ak_1p)),ak);
    pk_nevner = MNorm2(pk_teller);
    if (fabs(pk_nevner) < NULLGRENSE) {
      pk_teller = MMult(MMult(MTransp(Ak_1p),Ak_1p),ak);
      pk_nevner = 1 + MNorm2(MMult(Ak_1p,ak));
    }
    pk = MRealMult(pk_teller,1/pk_nevner);
    Ap = MRowConcat(MMult(Ak_1p, MIMinus(MMult(ak,MTransp(pk)))),
		      MTransp(pk));
  }
  MNoTemp(Ap);
  MFreeTemp(tempstart);
  return Ap;
}


static void MPrint(xiteMatrix a)
{
  int i,j;

  for (i=0; i<a->rows; i++) {
    for (j=0; j<a->cols; j++) printf("%15.12f ", M_elem(a,i,j));
    printf("\n");
  }
  printf("\n");
}
