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

//#include <endian.h>
//#include <sys/types.h>

/* A representation of a double as a union. */
union ieee754_double {
  double d;

  /* This is the IEEE 754 double-precision format.  */
  struct {
    /* Together these comprise the mantissa.  */
    unsigned int mantissa1: 32;
    unsigned int mantissa0: 20;
    unsigned int exponent: 11;
    unsigned int negative: 1;
  } ieee;

  /* This format makes it easier to see if a NaN is a signalling NaN.  */
  struct {
    /* Together these comprise the mantissa.  */
    unsigned int mantissa1: 32;
    unsigned int mantissa0: 19;
    unsigned int quiet_nan: 1;
    unsigned int exponent: 11;
    unsigned int negative: 1;
  } ieee_nan;
};

/* The original fdlibm code used statements like:
  n0 = ((*(int*)&one)>>29)^1;   * index of high word *
  ix0 = *(n0+(int*)&x);     * high word of x *
  ix1 = *((1-n0)+(int*)&x);   * low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

/* #if __FLOAT_WORD_ORDER == BIG_ENDIAN */
/* #warning USING Big Endian float word order */
/* typedef union */
/* { */
/*   double value; */
/*   struct */
/*   { */
/*     u_int16_t msw; */
/*     u_int16_t lsw; */
/*   } parts; */
/* } ieeeDoubleShapeType; */

/* #endif */

/* #if __FLOAT_WORD_ORDER == LITTLE_ENDIAN */
/* #warning USING Little Endian float word order */

typedef union {
  double value;
  struct {
    u_int16_t lsw;
    u_int16_t msw;
  } parts;
} ieeeDoubleShapeType;

/* #endif */

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)        \
{               \
  ieeeDoubleShapeType ew_u;         \
  ew_u.value = (d);           \
  (ix0) = ew_u.parts.msw;         \
  (ix1) = ew_u.parts.lsw;         \
}

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)          \
{               \
  ieeeDoubleShapeType gh_u;         \
  gh_u.value = (d);           \
  (i) = gh_u.parts.msw;           \
}

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)         \
{               \
  ieeeDoubleShapeType gl_u;         \
  gl_u.value = (d);           \
  (i) = gl_u.parts.lsw;           \
}

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)         \
{               \
  ieeeDoubleShapeType iw_u;         \
  iw_u.parts.msw = (ix0);         \
  iw_u.parts.lsw = (ix1);         \
  (d) = iw_u.value;           \
}

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)          \
{               \
  ieeeDoubleShapeType sh_u;         \
  sh_u.value = (d);           \
  sh_u.parts.msw = (v);           \
  (d) = sh_u.value;           \
}

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)         \
{               \
  ieeeDoubleShapeType sl_u;         \
  sl_u.value = (d);           \
  sl_u.parts.lsw = (v);           \
  (d) = sl_u.value;           \
}

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union {
  float value;
  u_int32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)         \
{               \
  ieee_float_shape_type gf_u;         \
  gf_u.value = (d);           \
  (i) = gf_u.word;            \
}

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)         \
{               \
  ieee_float_shape_type sf_u;         \
  sf_u.word = (i);            \
  (d) = sf_u.value;           \
}


#endif /* _MATH_PRIVATE_H_ */
