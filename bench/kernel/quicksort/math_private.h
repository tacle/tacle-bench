/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: math_private.h

  Author: Unknown

  Function: IEEE754 software library routines.

  Source: Sun Microsystems

  Original name: fdlibm.h

  Changes: No major functional changes.

  License: See the terms below.

*/


/*
   ====================================================
   Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.

   Developed at SunPro, a Sun Microsystems, Inc. business.
   Permission to use, copy, modify, and distribute this
   software is freely granted, provided that this notice
   is preserved.
   ====================================================
*/

/*
   from: @(#)fdlibm.h 5.1 93/09/24
*/

#ifndef _MATH_PRIVATE_H_
#define _MATH_PRIVATE_H_

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union {
  float value;
  unsigned int word;
} quicksort_ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define QUICKSORT_GET_FLOAT_WORD(i,d)       \
{               \
  quicksort_ieee_float_shape_type gf_u;       \
  gf_u.value = (d);           \
  (i) = gf_u.word;            \
}

/* Set a float from a 32 bit int.  */

#define QUICKSORT_SET_FLOAT_WORD(d,i)       \
{               \
  quicksort_ieee_float_shape_type sf_u;       \
  sf_u.word = (i);            \
  (d) = sf_u.value;           \
}

#endif /* _MATH_PRIVATE_H_ */
