
/*  copyright 1990  Richard H. Krukar all rights reserved

	Permission granted to buy, sell, or steal this software is granted.
    The author retains the right to distribute this software freely, but
    is not responsible for it's quality or maintainance. */

/*  A split radix ( 2/4 ) IFFT based on ( reference here )  This routine
is meant to be fairly efficient, but not exeptionally.  Recursion is
used to sort out the butterflys.  This should not matter because as
soon as the length reaches 128, everything is inlined elsewhere */


#include "fft_L.h"

void idintime(struct complex_ri *x, int length, float *wtab)
{
    int qlen, step1, step2, c1=0, c2=0, s1, s2;
    struct complex_ri *ptr1, *ptr2, *ptr3, *ptr4, tmp;

    if(length<= UNROL ) {
	idint( (float *) x, length, wtab);
	return; }

    s1 = s2 = 3*TLEN/4;
    step1 = TLEN/length;
    step2 = 3*step1;

    qlen = length>>2;
    for(ptr1 = x; ptr1 < &x[qlen]; ptr1++) {
	ptr2 = ptr1+qlen;
	ptr3 = ptr2+qlen;
	ptr4 = ptr3+qlen;

	tmp.r = ptr2->r - ptr4->r; /* First Butterfly */
	tmp.i = ptr2->i - ptr4->i;
	ptr2->r += ptr4->r;
	ptr2->i += ptr4->i;

	ptr4->r = -tmp.i; ptr4->i = tmp.r; /* mult by j */

	tmp.r = ptr1->r - ptr3->r; /* Second Butterfly */
	tmp.i = ptr1->i - ptr3->i;
	ptr1->r += ptr3->r;
	ptr1->i += ptr3->i;

	ptr3->r = tmp.r + ptr4->r; /* Third Butterfly */
	ptr3->i = tmp.i + ptr4->i;
	ptr4->r = tmp.r - ptr4->r;
	ptr4->i = tmp.i - ptr4->i;
	
/* Now multiply ptr3 and ptr4 by the appropriate weights */

	tmp.r = ptr3->r*wtab[c1] - ptr3->i*wtab[s1];
	ptr3->i = ptr3->r*wtab[s1] + ptr3->i*wtab[c1];
	ptr3->r = tmp.r;

	tmp.r = ptr4->r*wtab[c2] - ptr4->i*wtab[s2];
	ptr4->i = ptr4->r*wtab[s2] + ptr4->i*wtab[c2];
	ptr4->r = tmp.r;

	c1 += step1; s1 += step1;
	c2 += step2; s2 += step2;
	if(s2 >= TLEN) s2 -=TLEN;
    }
    idintime(&x[0], length>>1, wtab);
    idintime(&x[(length>>1)], length>>2, wtab);
    idintime(&x[((length*3)>>2)], length>>2, wtab);
}
