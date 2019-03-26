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

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: wcclibm.h

  Author: Unknown

  Function: IEEE754 software library routines.

  Source: Sun Microsystems

  Original name: wcclibm.h

  Changes: No major functional changes.

  License: See the terms above.

*/

#ifndef _WCCLIBM
#define _WCCLIBM

#define int32_t      int
#define u_int32_t    unsigned int

// Often used variables/consts
static const float basicmath_one    =  1.0f,
                   basicmath_tiny   =  1.0e-30f,
                   basicmath_half   =  5.0000000000e-01, /* 0x3f000000 */
                   basicmath_huge   =  1.0e30,
                   basicmath_two    =  2.0,
                   basicmath_two24  =  16777216.0,       /* 0x4b800000 */
                   basicmath_zero   =  0.0;                  
                   
#define __ieee754_acosf  acosf
#define __atanf          atanf
#define __ceilf          ceilf
#define __cosf           cosf
#define __fabsf          fabsf
#define __isinff         isinff
#define __ieee754_expf   expf
#define __erff           erff
#define __ieee754_powf   powf
#define __ieee754_sqrtf  sqrtf
#define __ieee754_log10f log10f
#define __ieee754_logf   logf
#define __sinf           sinf
#define __floorf         floorf

                   
float          __atanf(float x);
float          __ceilf(float x);
static float   __copysignf(float x, float y);
float          __cosf(float x);
float          __erff(float x);
float          __fabsf(float x);
float          __floorf(float x);
float          __ieee754_acosf(float x);
float          __ieee754_expf(float x);
float          __ieee754_logf(float x);
float          __ieee754_log10f(float x);
float          __ieee754_powf(float x, float y);
static int32_t __ieee754_rem_pio2f(float x, float *y);
float          __ieee754_sqrtf(float x);
int            __isinff(float x);
static float   __kernel_cosf(float x, float y);
static float   __kernel_sinf(float x, float y, int iy);
static int     __kernel_rem_pio2f(float *x, float *y, int e0, int nx, 
                                  int prec, const int32_t *ipio2);
float          __sinf(float x);
float          __scalbnf(float x, int n);                   

#endif // _WCCLIBM
