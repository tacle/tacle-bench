/***************************************************************************
*
* Copyright (c) 1998/1999, C-LAB, Paderborn
*
* This file containes a CHaRy description generated from the
* file: NSicherNeu.sea
* The name of the Pr/T-Net was: NSicherNeu
*
* DESCRIPTION: Simulate an extended Petri Net
*
* COMMENTS: Automatically generated code containing large amounts of
* 	    if-statements (more than 250)
*
*
* Modified and maintained by
* Friedhelm Stappert
* C-LAB, Paderborn, Germany
* fst@c-lab.de
*
* Modifications:
* o made the local variables global and volatile,
*   so the compiler won't optimise everything away.
*
***************************************************************************/

/* Remove the following #define for actual WCET analyses! */
/*
#define PROFILING
*/

#ifdef PROFILING
#include <stdio.h>
#endif

#ifdef PROFILING
/* Profiling variables. Remove for actual WCET analyses. */
int main_iters_dummy_i = 0, main_min_dummy_i = 100000, main_max_dummy_i = 0;
#endif


/* Forward function prototypes */

volatile int	P1_is_marked = 3;
volatile long	P1_marking_member_0[3];
volatile int	P2_is_marked = 5;
volatile long	P2_marking_member_0[5];
volatile int	P3_is_marked = 0;
volatile long	P3_marking_member_0[6];

int main( void )
{
  int dummy_i;
  /*   dummy_i = 17; Takes too much time */
  dummy_i = 2;

  #ifdef PROFILING
  main_iters_dummy_i = 0;
  #endif    
  _Pragma("loopbound min 2 max 2")
  while (dummy_i > 0) {
    #ifdef PROFILING
    main_iters_dummy_i++;
    #endif    

    dummy_i--;
    /* Permutation for Place P1 : 0, 1, 2 */
    /* Transition T1 */
    if ( (P1_is_marked >= 3) &&
	(P3_is_marked + 3 <= 6) &&
	(P1_marking_member_0[1] == P1_marking_member_0[2]) ) {

      long	x;
      long	y;
      long	z;

      x = P1_marking_member_0[0];
      y = P1_marking_member_0[1];

      /* Transition condition */
      if (x < y) {

	/* demarking of input places */
	P1_is_marked -= 3;

	/* preaction */
	z = x - y;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = x;
	P3_marking_member_0[P3_is_marked+1] = y;
	P3_marking_member_0[P3_is_marked+2] = z;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P1 : 0, 2, 1 */
    /* Transition T1 */
    if ( (P1_is_marked >= 3) &&
	(P3_is_marked + 3 <= 6) &&
	(P1_marking_member_0[2] == P1_marking_member_0[1]) ) {

      long	x;
      long	y;
      long	z;

      x = P1_marking_member_0[0];
      y = P1_marking_member_0[2];

      /* Transition condition */
      if ((x < y)) {

	/* demarking of input places */
	P1_is_marked -= 3;

	/* preaction */
	z = x - y;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = x;
	P3_marking_member_0[P3_is_marked+1] = y;
	P3_marking_member_0[P3_is_marked+2] = z;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P1 : 1, 0, 2 */
    /* Transition T1 */
    if ( (P1_is_marked >= 3) &&
	(P3_is_marked + 3 <= 6) &&
	(P1_marking_member_0[0] == P1_marking_member_0[2]) ) {

      long	x;
      long	y;
      long	z;

      x = P1_marking_member_0[1];
      y = P1_marking_member_0[0];

      /* Transition condition */
      if (x < y) {

	/* demarking of input places */
	P1_is_marked -= 3;

	/* preaction */
	z = x - y;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = x;
	P3_marking_member_0[P3_is_marked+1] = y;
	P3_marking_member_0[P3_is_marked+2] = z;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P1 : 1, 2, 0 */
    /* Transition T1 */
    if ( (P1_is_marked >= 3) &&
	(P3_is_marked + 3 <= 6) &&
	(P1_marking_member_0[2] == P1_marking_member_0[0])) {

      long	x;
      long	y;
      long	z;

      x = P1_marking_member_0[1];
      y = P1_marking_member_0[2];

      /* Transition condition */
      if ((x < y)) {

	/* demarking of input places */
	P1_is_marked -= 3;

	/* preaction */
	z = x - y;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = x;
	P3_marking_member_0[P3_is_marked+1] = y;
	P3_marking_member_0[P3_is_marked+2] = z;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P1 : 2, 0, 1 */
    /* Transition T1 */
    if ( (P1_is_marked >= 3) &&
	(P3_is_marked + 3 <= 6) &&
	(P1_marking_member_0[0] == P1_marking_member_0[1]) ) {
      long	x;
      long	y;
      long	z;

      x = P1_marking_member_0[2];
      y = P1_marking_member_0[0];

      /* Transition condition */
      if ((x < y)) {

	/* demarking of input places */
	P1_is_marked -= 3;

	/* preaction */
	z = x - y;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = x;
	P3_marking_member_0[P3_is_marked+1] = y;
	P3_marking_member_0[P3_is_marked+2] = z;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P1 : 2, 1, 0 */
    /* Transition T1 */
    if ( (P1_is_marked >= 3) &&
	(P3_is_marked + 3 <= 6) &&
	(P1_marking_member_0[1] == P1_marking_member_0[0]) ) {
      long	x;
      long	y;
      long	z;

      x = P1_marking_member_0[2];
      y = P1_marking_member_0[1];

      /* Transition condition */
      if ((x < y)) {

	/* demarking of input places */
	P1_is_marked -= 3;

	/* preaction */
	z = x - y;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = x;
	P3_marking_member_0[P3_is_marked+1] = y;
	P3_marking_member_0[P3_is_marked+2] = z;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 1, 2, 3 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	(((P3_is_marked + 3) <= 6)) &&
	( ((P2_marking_member_0[1] == P2_marking_member_0[2])) &&
	  ((P2_marking_member_0[1] == P2_marking_member_0[3])) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[0];
      b = P2_marking_member_0[1];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 1, 3, 2 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	(((P3_is_marked + 3) <= 6)) &&
	( (P2_marking_member_0[1] == P2_marking_member_0[3]) &&
	  (P2_marking_member_0[1] == P2_marking_member_0[2]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[0];
      b = P2_marking_member_0[1];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 2, 1, 3 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[2] == P2_marking_member_0[1]) &&
	  (P2_marking_member_0[2] == P2_marking_member_0[3]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[0];
      b = P2_marking_member_0[2];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 2, 3, 1 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[2] == P2_marking_member_0[3]) &&
	  (P2_marking_member_0[2] == P2_marking_member_0[1]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[0];
      b = P2_marking_member_0[2];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 3, 1, 2 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[3] == P2_marking_member_0[1]) &&
	  (P2_marking_member_0[3] == P2_marking_member_0[2]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[0];
      b = P2_marking_member_0[3];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 3, 2, 1 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[3] == P2_marking_member_0[2]) &&
	  (P2_marking_member_0[3] == P2_marking_member_0[1]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[0];
      b = P2_marking_member_0[3];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 1, 0, 2, 3 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[0] == P2_marking_member_0[2]) &&
	  (P2_marking_member_0[0] == P2_marking_member_0[3]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[1];
      b = P2_marking_member_0[0];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 1, 0, 3, 2 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[0] == P2_marking_member_0[3]) &&
	  (P2_marking_member_0[0] == P2_marking_member_0[2]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[1];
      b = P2_marking_member_0[0];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 1, 2, 0, 3 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[2] == P2_marking_member_0[0]) &&
	  (P2_marking_member_0[2] == P2_marking_member_0[3]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[1];
      b = P2_marking_member_0[2];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 1, 2, 3, 0 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[2] == P2_marking_member_0[3]) &&
	  (P2_marking_member_0[2] == P2_marking_member_0[0]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[1];
      b = P2_marking_member_0[2];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 1, 3, 0, 2 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[3] == P2_marking_member_0[0]) &&
	  (P2_marking_member_0[3] == P2_marking_member_0[2]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[1];
      b = P2_marking_member_0[3];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }


    /* Permutation for Place P2 : 1, 3, 2, 0 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[3] == P2_marking_member_0[2]) &&
	  (P2_marking_member_0[3] == P2_marking_member_0[0]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[1];
      b = P2_marking_member_0[3];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }


    /* Permutation for Place P2 : 2, 0, 1, 3 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[0] == P2_marking_member_0[1]) &&
	  (P2_marking_member_0[0] == P2_marking_member_0[3]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[2];
      b = P2_marking_member_0[0];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 2, 0, 3, 1 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[0] == P2_marking_member_0[3]) &&
	  (P2_marking_member_0[0] == P2_marking_member_0[1]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[2];
      b = P2_marking_member_0[0];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 2, 1, 0, 3 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[1] == P2_marking_member_0[0]) &&
	  (P2_marking_member_0[1] == P2_marking_member_0[3]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[2];
      b = P2_marking_member_0[1];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 2, 1, 3, 0 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[1] == P2_marking_member_0[3]) &&
	  (P2_marking_member_0[1] == P2_marking_member_0[0]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[2];
      b = P2_marking_member_0[1];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 2, 3, 0, 1 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[3] == P2_marking_member_0[0]) &&
	  (P2_marking_member_0[3] == P2_marking_member_0[1]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[2];
      b = P2_marking_member_0[3];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 2, 3, 1, 0 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[3] == P2_marking_member_0[1]) &&
	  (P2_marking_member_0[3] == P2_marking_member_0[0]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[2];
      b = P2_marking_member_0[3];

      /* Transition condition */
      if ((b > a)) {
	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 3, 0, 1, 2 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	((P3_is_marked + 3) <= 6) &&
	( (P2_marking_member_0[0] == P2_marking_member_0[1]) &&
	  (P2_marking_member_0[0] == P2_marking_member_0[2]) ) ) {
      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[3];
      b = P2_marking_member_0[0];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }


    /* Permutation for Place P2 : 3, 0, 2, 1 */
    /* Transition T2 */
    if ( (P2_is_marked >= 4) &&
	( (P3_is_marked + 3) <= 6) &&
	( ( P2_marking_member_0[0] == P2_marking_member_0[2]) &&
	  ( P2_marking_member_0[0] == P2_marking_member_0[1]) ) ) {

      long	a;
      long	b;
      long	c;

      a = P2_marking_member_0[3];
      b = P2_marking_member_0[0];

      /* Transition condition */
      if ((b > a)) {

	/* demarking of input places */
	P2_is_marked -= 4;

	/* preaction */
	c = a + b;

	/* marking of output places */
	P3_marking_member_0[P3_is_marked+0] = a;
	P3_marking_member_0[P3_is_marked+1] = b;
	P3_marking_member_0[P3_is_marked+2] = c;
	P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }
  }
  #ifdef PROFILING
  if ( main_iters_dummy_i < main_min_dummy_i )
    main_min_dummy_i = main_iters_dummy_i;
  if ( main_iters_dummy_i > main_max_dummy_i )
    main_max_dummy_i = main_iters_dummy_i;
  #endif

  #ifdef PROFILING
  printf( "main::dummy_i-loop: [%d, %d]\n", main_min_dummy_i, main_max_dummy_i );
  #endif

  dummy_i = 77;

  return dummy_i;
}
