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

#include "wcclibm.h"

/* A union which permits us to convert between a float and a 32 bit int. */

typedef union {
  float value;
  u_int32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float. */

#define GET_FLOAT_WORD(i,d)   \
{                             \
  ieee_float_shape_type gf_u; \
  gf_u.value = (d);           \
  (i) = gf_u.word;            \
}

/* Set a float from a 32 bit int. */

#define SET_FLOAT_WORD(d,i)   \
{                             \
  ieee_float_shape_type sf_u; \
  sf_u.word = (i);            \
  (d) = sf_u.value;           \
}

#endif /* _MATH_PRIVATE_H_ */
