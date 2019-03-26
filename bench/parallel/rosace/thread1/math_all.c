

/* e_acosf.c -- float version of e_acos.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: math_all.c

  Author: Unknown

  Function: IEEE754 software library routines.

  Source: Sun Microsystems

  Original name: math_all.c

  Changes: No major functional changes.

  License: See the terms above.

*/

#define size_t       unsigned long
#define int32_t      int
#define uint32_t     unsigned int
#define u_int16_t    unsigned short
#define u_int32_t    unsigned int

#include "math_private.h"
#include "wcclibm.h"

// The following defines map the math functions to specialized calls.
// The declarations in math.h are bound to the official interface names.
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


// The list of defined functions with their  official interface names
float          __atanf( float x );
float          __ceilf( float x );
float          __copysignf( float x, float y );
float          __cosf( float x );
float          __erff( float x );
float          __fabsf( float x );
float          __floorf( float x );
float          __ieee754_acosf( float x );
float          __ieee754_expf( float x );
float          __ieee754_logf( float x );
float          __ieee754_log10f( float x );
float          __ieee754_powf( float x, float y );
int32_t        __ieee754_rem_pio2f( float x, float *y );
float          __ieee754_sqrtf( float x );
int            __isinff( float x );
float          __kernel_cosf( float x, float y );
float          __kernel_sinf( float x, float y, int iy );
int            __kernel_rem_pio2f( float *x, float *y, int e0, int nx,
                                   int prec, const int32_t *ipio2 );
float          __sinf( float x );
float          __scalbnf( float x, int n );


// Often used variables/consts
#ifdef __STDC__
const float
#else
float
#endif
one    =  1.0f,
tiny   =  1.0e-30f,
half   =  5.0000000000e-01f, /* 0x3f000000 */
huge   =  1.0e30f,
two    =  2.0f,
two24  =  16777216.0f,  /* 0x4b800000 */
two25  =  3.3554432000e+07f, /* 0x4c000000 */
two8   =  2.5600000000e+02f, /* 0x43800000 */
twon8  =  3.9062500000e-03f, /* 0x3b800000 */
zero   =  0.0f;


#ifdef __STDC__
const float
#else
float
#endif
/* one =  1.0000000000e+00f, /\* 0x3F800000 *\/ */
pi =  3.1415925026e+00f, /* 0x40490fda */
pio2_hi =  1.5707962513e+00f, /* 0x3fc90fda */
pio2_lo =  7.5497894159e-08f, /* 0x33a22168 */
pS0 =  1.6666667163e-01f, /* 0x3e2aaaab */
pS1 = -3.2556581497e-01f, /* 0xbea6b090 */
pS2 =  2.0121252537e-01f, /* 0x3e4e0aa8 */
pS3 = -4.0055535734e-02f, /* 0xbd241146 */
pS4 =  7.9153501429e-04f, /* 0x3a4f7f04 */
pS5 =  3.4793309169e-05f, /* 0x3811ef08 */
qS1 = -2.4033949375e+00f, /* 0xc019d139 */
qS2 =  2.0209457874e+00f, /* 0x4001572d */
qS3 = -6.8828397989e-01f, /* 0xbf303361 */
qS4 =  7.7038154006e-02f; /* 0x3d9dc62e */

#ifdef __STDC__
float __ieee754_acosf( float x )
#else
float __ieee754_acosf( x )
float x;
#endif
{
  float z, p, q, r, w, s, c, df;
  int32_t hx, ix;
  GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix == 0x3f800000 ) { /* |x|==1 */
    if ( hx > 0 ) return 0.0f; /* acos(1) = 0  */
    else return pi + ( float )2.0f * pio2_lo; /* acos(-1)= pi */
  } else
    if ( ix > 0x3f800000 ) { /* |x| >= 1 */
      return ( x - x ) / ( x - x ); /* acos(|x|>1) is NaN */
    }
  if ( ix < 0x3f000000 ) { /* |x| < 0.5 */
    if ( ix <= 0x23000000 ) return pio2_hi + pio2_lo; /*if|x|<2**-57*/
    z = x * x;
    p = z * ( pS0 + z * ( pS1 + z * ( pS2 + z * ( pS3 + z *
                                      ( pS4 + z * pS5 ) ) ) ) );
    q = one + z * ( qS1 + z * ( qS2 + z * ( qS3 + z * qS4 ) ) );
    r = p / q;
    return pio2_hi - ( x - ( pio2_lo - x * r ) );
  } else
    if ( hx < 0 ) { /* x < -0.5 */
      z = ( one + x ) * ( float )0.5f;
      p = z * ( pS0 + z * ( pS1 + z * ( pS2 + z * ( pS3 + z *
                                        ( pS4 + z * pS5 ) ) ) ) );
      q = one + z * ( qS1 + z * ( qS2 + z * ( qS3 + z * qS4 ) ) );
      s = __ieee754_sqrtf( z );
      r = p / q;
      w = r * s - pio2_lo;
      return pi - ( float )2.0f * ( s + w );
    } else {      /* x > 0.5 */
      int32_t idf;
      z = ( one - x ) * ( float )0.5f;
      s = __ieee754_sqrtf( z );
      df = s;
      GET_FLOAT_WORD( idf, df );
      SET_FLOAT_WORD( df, idf & 0xfffff000 );
      c  = ( z - df * df ) / ( s + df );
      p = z * ( pS0 + z * ( pS1 + z * ( pS2 + z * ( pS3 + z *
                                        ( pS4 + z * pS5 ) ) ) ) );
      q = one + z * ( qS1 + z * ( qS2 + z * ( qS3 + z * qS4 ) ) );
      r = p / q;
      w = r * s + c;
      return ( float )2.0f * ( df + w );
    }
}
/* e_powf.c -- float version of e_pow.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

/* static const float huge = 1.0e+30f, tiny = 1.0e-30f; */

#ifdef __STDC__
const float
#else
float
#endif
bp[] = {1.0f, 1.5f,},
       dp_h[] = { 0.0f, 5.84960938e-01f,}, /* 0x3f15c000 */
                dp_l[] = { 0.0f, 1.56322085e-06f,}, /* 0x35d1cfdc */
                         /* zero    =  0.0f, */
                         /* one  =  1.0f, */
                         /* two  =  2.0f, */
                         /* two24  =  16777216.0f, /\* 0x4b800000 *\/ */
                         /* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
                         L1  =  6.0000002384e-01f, /* 0x3f19999a */
                         L2  =  4.2857143283e-01f, /* 0x3edb6db7 */
                         L3  =  3.3333334327e-01f, /* 0x3eaaaaab */
                         L4  =  2.7272811532e-01f, /* 0x3e8ba305 */
                         L5  =  2.3066075146e-01f, /* 0x3e6c3255 */
                         L6  =  2.0697501302e-01f, /* 0x3e53f142 */
                         P1   =  1.6666667163e-01f, /* 0x3e2aaaab */
                         P2   = -2.7777778450e-03f, /* 0xbb360b61 */
                         P3   =  6.6137559770e-05f, /* 0x388ab355 */
                         P4   = -1.6533901999e-06f, /* 0xb5ddea0e */
                         P5   =  4.1381369442e-08f, /* 0x3331bb4c */
                         lg2  =  6.9314718246e-01f, /* 0x3f317218 */
                         lg2_h  =  6.93145752e-01f, /* 0x3f317200 */
                         lg2_l  =  1.42860654e-06f, /* 0x35bfbe8c */
                         ovt =  4.2995665694e-08f, /* -(128-log2(ovfl+.5ulp)) */
                         cp    =  9.6179670095e-01f, /* 0x3f76384f =2/(3ln2) */
                         cp_h  =  9.6179199219e-01f, /* 0x3f763800 =head of cp */
                         cp_l  =  4.7017383622e-06f, /* 0x369dc3a0 =tail of cp_h */
                         ivln2    =  1.4426950216e+00f, /* 0x3fb8aa3b =1/ln2 */
                         ivln2_h  =  1.4426879883e+00f, /* 0x3fb8aa00 =16b 1/ln2*/
                         ivln2_l  =  7.0526075433e-06f; /* 0x36eca570 =1/ln2 tail*/

#ifdef __STDC__
float __ieee754_powf( float x, float y )
#else
float __ieee754_powf( x, y )
float x, y;
#endif
{
  float z, ax, z_h, z_l, p_h, p_l;
  float y1, t1, t2, r, s, t, u, v, w;
  int32_t i, j, k, yisint, n;
  int32_t hx, hy, ix, iy, is;

  GET_FLOAT_WORD( hx, x );
  GET_FLOAT_WORD( hy, y );
  ix = hx & 0x7fffffff;
  iy = hy & 0x7fffffff;

  /* y==zero: x**0 = 1 */
  if ( iy == 0 ) return one;

  /* x==+-1 */
  if ( x == 1.0f ) return one;
  if ( x == -1.0f && isinff( y ) ) return one;

  /* +-NaN return x+y */
  if ( ix > 0x7f800000 ||
       iy > 0x7f800000 )
    return x + y;

  /* determine if y is an odd int when x < 0
     yisint = 0 ... y is not an integer
     yisint = 1 ... y is an odd int
     yisint = 2 ... y is an even int
  */
  yisint  = 0;
  if ( hx < 0 ) {
    if ( iy >= 0x4b800000 ) yisint = 2; /* even integer y */
    else
      if ( iy >= 0x3f800000 ) {
        k = ( iy >> 23 ) - 0x7f; /* exponent */
        j = iy >> ( 23 - k );
        if ( ( j << ( 23 - k ) ) == iy ) yisint = 2 - ( j & 1 );
      }
  }

  /* special value of y */
  if ( iy == 0x7f800000 ) { /* y is +-inf */
    if ( ix == 0x3f800000 )
      return  y - y;  /* inf**+-1 is NaN */
    else
      if ( ix > 0x3f800000 ) /* (|x|>1)**+-inf = inf,0 */
        return ( hy >= 0 ) ? y : zero;
      else      /* (|x|<1)**-,+inf = inf,0 */
        return ( hy < 0 ) ? -y : zero;
  }
  if ( iy == 0x3f800000 ) { /* y is  +-1 */
    if ( hy < 0 ) return one / x;
    else return x;
  }
  if ( hy == 0x40000000 ) return x * x; /* y is  2 */
  if ( hy == 0x3f000000 ) { /* y is  0.5 */
    if ( hx >= 0 ) /* x >= +0 */
      return __ieee754_sqrtf( x );
  }

  ax   = fabsf( x );
  /* special value of x */
  if ( ix == 0x7f800000 || ix == 0 || ix == 0x3f800000 ) {
    z = ax;     /*x is +-0,+-inf,+-1*/
    if ( hy < 0 ) z = one / z; /* z = (1/|x|) */
    if ( hx < 0 ) {
      if ( ( ( ix - 0x3f800000 ) | yisint ) == 0 ) {
        z = ( z - z ) / ( z - z ); /* (-1)**non-int is NaN */
      } else
        if ( yisint == 1 )
          z = -z;   /* (x<0)**odd = -(|x|**odd) */
    }
    return z;
  }

  /* (x<0)**(non-int) is NaN */
  if ( ( ( ( ( u_int32_t )hx >> 31 ) - 1 ) | yisint ) == 0 ) return ( x - x ) /
        ( x - x );

  /* |y| is huge */
  if ( iy > 0x4d000000 ) { /* if |y| > 2**27 */
    /* over/underflow if x is not close to one */
    if ( ix < 0x3f7ffff8 ) return ( hy < 0 ) ? huge * huge : tiny * tiny;
    if ( ix > 0x3f800007 ) return ( hy > 0 ) ? huge * huge : tiny * tiny;
    /* now |1-x| is tiny <= 2**-20, suffice to compute
       log(x) by x-x^2/2+x^3/3-x^4/4 */
    t = x - 1;  /* t has 20 trailing zeros */
    w = ( t * t ) * ( ( float )0.5f - t * ( ( float )0.333333333333f - t *
                                            ( float )0.25f ) );
    u = ivln2_h * t; /* ivln2_h has 16 sig. bits */
    v = t * ivln2_l - w * ivln2;
    t1 = u + v;
    GET_FLOAT_WORD( is, t1 );
    SET_FLOAT_WORD( t1, is & 0xfffff000 );
    t2 = v - ( t1 - u );
  } else {
    float s2, s_h, s_l, t_h, t_l;
    n = 0;
    /* take care subnormal number */
    if ( ix < 0x00800000 ) {
      ax *= two24;
      n -= 24;
      GET_FLOAT_WORD( ix, ax );
    }
    n  += ( ( ix ) >> 23 ) - 0x7f;
    j  = ix & 0x007fffff;
    /* determine interval */
    ix = j | 0x3f800000;  /* normalize ix */
    if ( j <= 0x1cc471 ) k = 0; /* |x|<sqrt(3/2) */
    else
      if ( j < 0x5db3d7 ) k = 1; /* |x|<sqrt(3)   */
      else {
        k = 0;
        n += 1;
        ix -= 0x00800000;
      }
    SET_FLOAT_WORD( ax, ix );

    /* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
    u = ax - bp[ k ]; /* bp[0]=1.0, bp[1]=1.5 */
    v = one / ( ax + bp[ k ] );
    s = u * v;
    s_h = s;
    GET_FLOAT_WORD( is, s_h );
    SET_FLOAT_WORD( s_h, is & 0xfffff000 );
    /* t_h=ax+bp[k] High */
    SET_FLOAT_WORD( t_h, ( ( ix >> 1 ) | 0x20000000 ) + 0x0040000 + ( k << 21 ) );
    t_l = ax - ( t_h - bp[ k ] );
    s_l = v * ( ( u - s_h * t_h ) - s_h * t_l );
    /* compute log(ax) */
    s2 = s * s;
    r = s2 * s2 * ( L1 + s2 * ( L2 + s2 * ( L3 + s2 * ( L4 + s2 *
                                            ( L5 + s2 * L6 ) ) ) ) );
    r += s_l * ( s_h + s );
    s2  = s_h * s_h;
    t_h = ( float )3.0f + s2 + r;
    GET_FLOAT_WORD( is, t_h );
    SET_FLOAT_WORD( t_h, is & 0xfffff000 );
    t_l = r - ( ( t_h - ( float )3.0f ) - s2 );
    /* u+v = s*(1+...) */
    u = s_h * t_h;
    v = s_l * t_h + t_l * s;
    /* 2/(3log2)*(s+...) */
    p_h = u + v;
    GET_FLOAT_WORD( is, p_h );
    SET_FLOAT_WORD( p_h, is & 0xfffff000 );
    p_l = v - ( p_h - u );
    z_h = cp_h * p_h; /* cp_h+cp_l = 2/(3*log2) */
    z_l = cp_l * p_h + p_l * cp + dp_l[ k ];
    /* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
    t = ( float )n;
    t1 = ( ( ( z_h + z_l ) + dp_h[ k ] ) + t );
    GET_FLOAT_WORD( is, t1 );
    SET_FLOAT_WORD( t1, is & 0xfffff000 );
    t2 = z_l - ( ( ( t1 - t ) - dp_h[ k ] ) - z_h );
  }

  s = one; /* s (sign of result -ve**odd) = -1 else = 1 */
  if ( ( ( ( ( u_int32_t )hx >> 31 ) - 1 ) | ( yisint - 1 ) ) == 0 )
    s = -one; /* (-ve)**(odd int) */

  /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
  GET_FLOAT_WORD( is, y );
  SET_FLOAT_WORD( y1, is & 0xfffff000 );
  p_l = ( y - y1 ) * t1 + y * t2;
  p_h = y1 * t1;
  z = p_l + p_h;
  GET_FLOAT_WORD( j, z );
  if ( j > 0x43000000 )   /* if z > 128 */
    return s * huge * huge;   /* overflow */
  else
    if ( j == 0x43000000 ) { /* if z == 128 */
      if ( p_l + ovt > z - p_h ) return s * huge * huge; /* overflow */
    } else
      if ( ( j & 0x7fffffff ) > 0x43160000 ) /* z <= -150 */
        return s * tiny * tiny;   /* underflow */
      else
        if ( ( u_int32_t ) j == 0xc3160000 ) { /* z == -150 */
          if ( p_l <= z - p_h ) return s * tiny * tiny; /* underflow */
        }
  /*
     compute 2**(p_h+p_l)
  */
  i = j & 0x7fffffff;
  k = ( i >> 23 ) - 0x7f;
  n = 0;
  if ( i > 0x3f000000 ) { /* if |z| > 0.5, set n = [z+0.5] */
    n = j + ( 0x00800000 >> ( k + 1 ) );
    k = ( ( n & 0x7fffffff ) >> 23 ) - 0x7f; /* new k for n */
    SET_FLOAT_WORD( t, n & ~( 0x007fffff >> k ) );
    n = ( ( n & 0x007fffff ) | 0x00800000 ) >> ( 23 - k );
    if ( j < 0 ) n = -n;
    p_h -= t;
  }
  t = p_l + p_h;
  GET_FLOAT_WORD( is, t );
  SET_FLOAT_WORD( t, is & 0xfffff000 );
  u = t * lg2_h;
  v = ( p_l - ( t - p_h ) ) * lg2 + t * lg2_l;
  z = u + v;
  w = v - ( z - u );
  t  = z * z;
  t1  = z - t * ( P1 + t * ( P2 + t * ( P3 + t * ( P4 + t * P5 ) ) ) );
  r  = ( z * t1 ) / ( t1 - two ) - ( w + z * w );
  z  = one - ( r - z );
  GET_FLOAT_WORD( j, z );
  j += ( n << 23 );
  if ( ( j >> 23 ) <= 0 ) z = __scalbnf( z, n ); /* subnormal output */
  else SET_FLOAT_WORD( z, j );
  return s * z;
}
/* e_rem_pio2f.c -- float version of e_rem_pio2.c
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] =
  "$NetBSD: e_rem_pio2f.c,v 1.5 1995/05/10 20:46:03 jtc Exp $";
#endif

/* __ieee754_rem_pio2f(x,y)

   return the remainder of x rem pi/2 in y[0]+y[1]
   use __kernel_rem_pio2f()
*/


/*
   Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
*/
#ifdef __STDC__
const int32_t two_over_pi[] = {
#else
int32_t two_over_pi[] = {
#endif
  0xA2, 0xF9, 0x83, 0x6E, 0x4E, 0x44, 0x15, 0x29, 0xFC,
  0x27, 0x57, 0xD1, 0xF5, 0x34, 0xDD, 0xC0, 0xDB, 0x62,
  0x95, 0x99, 0x3C, 0x43, 0x90, 0x41, 0xFE, 0x51, 0x63,
  0xAB, 0xDE, 0xBB, 0xC5, 0x61, 0xB7, 0x24, 0x6E, 0x3A,
  0x42, 0x4D, 0xD2, 0xE0, 0x06, 0x49, 0x2E, 0xEA, 0x09,
  0xD1, 0x92, 0x1C, 0xFE, 0x1D, 0xEB, 0x1C, 0xB1, 0x29,
  0xA7, 0x3E, 0xE8, 0x82, 0x35, 0xF5, 0x2E, 0xBB, 0x44,
  0x84, 0xE9, 0x9C, 0x70, 0x26, 0xB4, 0x5F, 0x7E, 0x41,
  0x39, 0x91, 0xD6, 0x39, 0x83, 0x53, 0x39, 0xF4, 0x9C,
  0x84, 0x5F, 0x8B, 0xBD, 0xF9, 0x28, 0x3B, 0x1F, 0xF8,
  0x97, 0xFF, 0xDE, 0x05, 0x98, 0x0F, 0xEF, 0x2F, 0x11,
  0x8B, 0x5A, 0x0A, 0x6D, 0x1F, 0x6D, 0x36, 0x7E, 0xCF,
  0x27, 0xCB, 0x09, 0xB7, 0x4F, 0x46, 0x3F, 0x66, 0x9E,
  0x5F, 0xEA, 0x2D, 0x75, 0x27, 0xBA, 0xC7, 0xEB, 0xE5,
  0xF1, 0x7B, 0x3D, 0x07, 0x39, 0xF7, 0x8A, 0x52, 0x92,
  0xEA, 0x6B, 0xFB, 0x5F, 0xB1, 0x1F, 0x8D, 0x5D, 0x08,
  0x56, 0x03, 0x30, 0x46, 0xFC, 0x7B, 0x6B, 0xAB, 0xF0,
  0xCF, 0xBC, 0x20, 0x9A, 0xF4, 0x36, 0x1D, 0xA9, 0xE3,
  0x91, 0x61, 0x5E, 0xE6, 0x1B, 0x08, 0x65, 0x99, 0x85,
  0x5F, 0x14, 0xA0, 0x68, 0x40, 0x8D, 0xFF, 0xD8, 0x80,
  0x4D, 0x73, 0x27, 0x31, 0x06, 0x06, 0x15, 0x56, 0xCA,
  0x73, 0xA8, 0xC9, 0x60, 0xE2, 0x7B, 0xC0, 0x8C, 0x6B,
};

/* This array is like the one in e_rem_pio2.c, but the numbers are
   single precision and the last 8 bits are forced to 0.  */
#ifdef __STDC__
const int32_t npio2_hw[] = {
#else
int32_t npio2_hw[] = {
#endif
  0x3fc90f00, 0x40490f00, 0x4096cb00, 0x40c90f00, 0x40fb5300, 0x4116cb00,
  0x412fed00, 0x41490f00, 0x41623100, 0x417b5300, 0x418a3a00, 0x4196cb00,
  0x41a35c00, 0x41afed00, 0x41bc7e00, 0x41c90f00, 0x41d5a000, 0x41e23100,
  0x41eec200, 0x41fb5300, 0x4203f200, 0x420a3a00, 0x42108300, 0x4216cb00,
  0x421d1400, 0x42235c00, 0x4229a500, 0x422fed00, 0x42363600, 0x423c7e00,
  0x4242c700, 0x42490f00
};

/*
   invpio2:  24 bits of 2/pi
   pio2_1:   first  17 bit of pi/2
   pio2_1t:  pi/2 - pio2_1
   pio2_2:   second 17 bit of pi/2
   pio2_2t:  pi/2 - (pio2_1+pio2_2)
   pio2_3:   third  17 bit of pi/2
   pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
*/

#ifdef __STDC__
const float
#else
float
#endif
/* zero =  0.0000000000e+00f, /\* 0x00000000 *\/ */
/* half =  5.0000000000e-01f, /\* 0x3f000000 *\/ */
/* two8 =  2.5600000000e+02f, /\* 0x43800000 *\/ */
invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

#ifdef __STDC__
int32_t __ieee754_rem_pio2f( float x, float *y )
#else
int32_t __ieee754_rem_pio2f( x, y )
float x, y[];
#endif
{
  float z, w, t, r, fn;
  float tx[ 3 ];
  int32_t e0, i, j, nx, n, ix, hx;

  GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) { /* |x| ~<= pi/4 , no need for reduction */
    y[ 0 ] = x;
    y[ 1 ] = 0;
    return 0;
  }
  if ( ix < 0x4016cbe4 ) { /* |x| < 3pi/4, special case with n=+-1 */
    if ( hx > 0 ) {
      z = x - pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[ 0 ] = z - pio2_1t;
        y[ 1 ] = ( z - y[ 0 ] ) - pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z -= pio2_2;
        y[ 0 ] = z - pio2_2t;
        y[ 1 ] = ( z - y[ 0 ] ) - pio2_2t;
      }
      return 1;
    } else {  /* negative x */
      z = x + pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[ 0 ] = z + pio2_1t;
        y[ 1 ] = ( z - y[ 0 ] ) + pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z += pio2_2;
        y[ 0 ] = z + pio2_2t;
        y[ 1 ] = ( z - y[ 0 ] ) + pio2_2t;
      }
      return -1;
    }
  }
  if ( ix <= 0x43490f80 ) { /* |x| ~<= 2^7*(pi/2), medium size */
    t  = fabsf( x );
    n  = ( int32_t ) ( t * invpio2 + half );
    fn = ( float )n;
    r  = t - fn * pio2_1;
    w  = fn * pio2_1t; /* 1st round good to 40 bit */
    if ( n < 32 && ( int32_t )( ix & 0xffffff00 ) != npio2_hw[ n - 1 ] ) {
      y[ 0 ] = r - w; /* quick check no cancellation */
    } else {
      u_int32_t high;
      j  = ix >> 23;
      y[ 0 ] = r - w;
      GET_FLOAT_WORD( high, y[ 0 ] );
      i = j - ( ( high >> 23 ) & 0xff );
      if ( i > 8 ) { /* 2nd iteration needed, good to 57 */
        t  = r;
        w  = fn * pio2_2;
        r  = t - w;
        w  = fn * pio2_2t - ( ( t - r ) - w );
        y[ 0 ] = r - w;
        GET_FLOAT_WORD( high, y[ 0 ] );
        i = j - ( ( high >> 23 ) & 0xff );
        if ( i > 25 )  { /* 3rd iteration need, 74 bits acc */
          t  = r; /* will cover all possible cases */
          w  = fn * pio2_3;
          r  = t - w;
          w  = fn * pio2_3t - ( ( t - r ) - w );
          y[ 0 ] = r - w;
        }
      }
    }
    y[ 1 ] = ( r - y[ 0 ] ) - w;
    if ( hx < 0 )  {
      y[ 0 ] = -y[ 0 ];
      y[ 1 ] = -y[ 1 ];
      return -n;
    } else   return n;
  }
  /*
     all other (large) arguments
  */
  if ( ix >= 0x7f800000 ) { /* x is inf or NaN */
    y[ 0 ] = y[ 1 ] = x - x;
    return 0;
  }
  /* set z = scalbn(|x|,ilogb(x)-7) */
  e0  = ( ix >> 23 ) - 134; /* e0 = ilogb(z)-7; */
  SET_FLOAT_WORD( z, ix - ( ( int32_t )( e0 << 23 ) ) );

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0; i < 2; i++ ) {
    tx[ i ] = ( float )( ( int32_t )( z ) );
    z     = ( z - tx[ i ] ) * two8;
  }
  tx[ 2 ] = z;
  nx = 3;
  _Pragma( "loopbound min 0 max 0" )
  while ( tx[ nx - 1 ] == zero )  {
    nx--; /* skip zero term */
  }
  n  =  __kernel_rem_pio2f( tx, y, e0, nx, 2, two_over_pi );
  if ( hx < 0 ) {
    y[ 0 ] = -y[ 0 ];
    y[ 1 ] = -y[ 1 ];
    return -n;
  }
  return n;
}
/* e_sqrtf.c -- float version of e_sqrt.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] = "$NetBSD: e_sqrtf.c,v 1.4 1995/05/10 20:46:19 jtc Exp $";
#endif


/* #ifdef __STDC__ */
/* static const float one = 1.0f, tiny=1.0e-30f; */
/* #else */
/* static float one = 1.0f, tiny=1.0e-30f; */
/* #endif */

#ifdef __STDC__
float __ieee754_sqrtf( float x )
#else
float __ieee754_sqrtf( x )
float x;
#endif
{
  float z;
  int32_t sign = ( int )0x80000000;
  int32_t ix, s, q, m, t, i;
  u_int32_t r;

  GET_FLOAT_WORD( ix, x );

  /* take care of Inf and NaN */
  if ( ( ix & 0x7f800000 ) == 0x7f800000 ) {
    return x * x + x;   /* sqrt(NaN)=NaN, sqrt(+inf)=+inf
             sqrt(-inf)=sNaN */
  }
  /* take care of zero */
  if ( ix <= 0 ) {
    if ( ( ix & ( ~sign ) ) == 0 ) return x; /* sqrt(+-0) = +-0 */
    else
      if ( ix < 0 )
        return ( x - x ) / ( x - x ); /* sqrt(-ve) = sNaN */
  }
  /* normalize x */
  m = ( ix >> 23 );
  if ( m == 0 ) {   /* subnormal x */
    _Pragma( "loopbound min 0 max 0" )
    for ( i = 0; ( ix & 0x00800000 ) == 0; i++ )
      ix <<= 1;
    m -= i - 1;
  }
  m -= 127; /* unbias exponent */
  ix = ( ix & 0x007fffff ) | 0x00800000;
  if ( m & 1 ) /* odd m, double x to make it even */
    ix += ix;
  m >>= 1;  /* m = [m/2] */

  /* generate sqrt(x) bit by bit */
  ix += ix;
  q = s = 0;    /* q = sqrt(x) */
  r = 0x01000000;   /* r = moving bit from right to left */

  _Pragma( "loopbound min 0 max 0" )
  while ( r != 0 ) {
    t = s + r;
    if ( t <= ix ) {
      s    = t + r;
      ix  -= t;
      q   += r;
    }
    ix += ix;
    r >>= 1;
  }

  /* use floating add to find out rounding direction */
  if ( ix != 0 ) {
    z = one - tiny; /* trigger inexact flag */
    if ( z >= one ) {
      z = one + tiny;
      if ( z > one )
        q += 2;
      else
        q += ( q & 1 );
    }
  }
  ix = ( q >> 1 ) + 0x3f000000;
  ix += ( m << 23 );
  SET_FLOAT_WORD( z, ix );
  return z;
}
/* k_cosf.c -- float version of k_cos.c
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] = "$NetBSD: k_cosf.c,v 1.4 1995/05/10 20:46:23 jtc Exp $";
#endif


#ifdef __STDC__
const float
#else
float
#endif
/* one =  1.0000000000e+00, /\* 0x3f800000 *\/ */
C1  =  4.1666667908e-02f, /* 0x3d2aaaab */
C2  = -1.3888889225e-03f, /* 0xbab60b61 */
C3  =  2.4801587642e-05f, /* 0x37d00d01 */
C4  = -2.7557314297e-07f, /* 0xb493f27c */
C5  =  2.0875723372e-09f, /* 0x310f74f6 */
C6  = -1.1359647598e-11f; /* 0xad47d74e */

#ifdef __STDC__
float __kernel_cosf( float x, float y )
#else
float __kernel_cosf( x, y )
float x, y;
#endif
{
  float a, hz, z, r, qx;
  int32_t ix;
  GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* ix = |x|'s high word*/
  if ( ix < 0x32000000 ) { /* if x < 2**27 */
    if ( ( ( int )x ) == 0 ) return one; /* generate inexact */
  }
  z  = x * x;
  r  = z * ( C1 + z * ( C2 + z * ( C3 + z * ( C4 + z * ( C5 + z * C6 ) ) ) ) );
  if ( ix < 0x3e99999a )    /* if |x| < 0.3 */
    return one - ( ( float )0.5f * z - ( z * r - x * y ) );
  else {
    if ( ix > 0x3f480000 )  /* x > 0.78125 */
      qx = ( float )0.28125f;

    else {
      SET_FLOAT_WORD( qx, ix - 0x01000000 ); /* x/4 */
    }
    hz = ( float )0.5f * z - qx;
    a  = one - qx;
    return a - ( hz - ( z * r - x * y ) );
  }
}
/* k_rem_pio2f.c -- float version of k_rem_pio2.c
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] =
  "$NetBSD: k_rem_pio2f.c,v 1.4 1995/05/10 20:46:28 jtc Exp $";
#endif


/* In the float version, the input parameter x contains 8 bit
   integers, not 24 bit integers.  113 bit precision is not supported.  */

#ifdef __STDC__
const int init_jk[] = {4, 7, 9}; /* initial value for jk */
#else
int init_jk[] = {4, 7, 9};
#endif

#ifdef __STDC__
const float PIo2[] = {
#else
float PIo2[] = {
#endif
  1.5703125000e+00f, /* 0x3fc90000 */
  4.5776367188e-04f, /* 0x39f00000 */
  2.5987625122e-05f, /* 0x37da0000 */
  7.5437128544e-08f, /* 0x33a20000 */
  6.0026650317e-11f, /* 0x2e840000 */
  7.3896444519e-13f, /* 0x2b500000 */
  5.3845816694e-15f, /* 0x27c20000 */
  5.6378512969e-18f, /* 0x22d00000 */
  8.3009228831e-20f, /* 0x1fc40000 */
  3.2756352257e-22f, /* 0x1bc60000 */
  6.3331015649e-25f, /* 0x17440000 */
};


#ifdef __STDC__
int __kernel_rem_pio2f( float *x, float *y, int e0, int nx, int prec,
                               const int32_t *ipio2 )
#else
int __kernel_rem_pio2f( x, y, e0, nx, prec, ipio2 )
float x[], y[];
int e0, nx, prec;
int32_t ipio2[];
#endif
{
  int32_t jz, jx, jv, jp, jk, carry, n, iq[ 20 ], i, j, k, m, q0, ih;
  float z, fw, f[ 20 ], fq[ 20 ], q[ 20 ];

  /* initialize jk*/
  jk = init_jk[ prec ];
  jp = jk;

  /* determine jx,jv,q0, note that 3>q0 */
  jx =  nx - 1;
  jv = ( e0 - 3 ) / 8;
  if ( jv < 0 ) jv = 0;
  q0 =  e0 - 8 * ( jv + 1 );

  /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
  j = jv - jx;
  m = jx + jk;
  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0; i <= m; i++, j++ )
    f[i] = ( j < 0 ) ? zero : ( float ) ipio2[ j ];

  /* compute q[0],q[1],...q[jk] */
  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0; i <= jk; i++ ) {
    for ( j = 0, fw = 0.0f; j <= jx; j++ ) fw += x[ j ] * f[ jx + i - j ];
    q[ i ] = fw;
  }

  jz = jk;
recompute:
  ;
  /* distill q[] into iq[] reversingly */
  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0, j = jz, z = q[ jz ]; j > 0; i++, j-- ) {
    fw    =  ( float )( ( int32_t )( twon8 * z ) );
    iq[i] =  ( int32_t )( z - two8 * fw );
    z     =  q[ j - 1 ] + fw;
  }

  /* compute n */
  z  = __scalbnf( z, q0 ); /* actual value of z */
  z -= ( float )8.0f * __floorf( z * ( float )
                                 0.125f ); /* trim off integer >= 8 */
  n  = ( int32_t ) z;
  z -= ( float )n;
  ih = 0;
  if ( q0 > 0 ) { /* need iq[jz-1] to determine n */
    i  = ( iq[ jz - 1 ] >> ( 8 - q0 ) );
    n += i;
    iq[ jz - 1 ] -= i << ( 8 - q0 );
    ih = iq[ jz - 1 ] >> ( 7 - q0 );
  } else
    if ( q0 == 0 ) ih = iq[ jz - 1 ] >> 8;
    else
      if ( z >= ( float )0.5f ) ih = 2;

  if ( ih > 0 ) { /* q > 0.5 */
    n += 1;
    carry = 0;
    _Pragma( "loopbound min 0 max 0" )
    for ( i = 0; i < jz ; i++ ) { /* compute 1-q */
      j = iq[ i ];
      if ( carry == 0 ) {
        if ( j != 0 ) {
          carry = 1;
          iq[ i ] = 0x100 - j;
        }
      } else  iq[ i ] = 0xff - j;

    }
    if ( q0 > 0 ) { /* rare case: chance is 1 in 12 */
      switch ( q0 ) {
        case 1:
          iq[ jz - 1 ] &= 0x7f;
          break;
        case 2:
          iq[ jz - 1 ] &= 0x3f;
          break;
      }
    }
    if ( ih == 2 ) {
      z = one - z;
      if ( carry != 0 ) z -= __scalbnf( one, q0 );
    }
  }

  /* check if recomputation is needed */
  if ( z == zero ) {
    j = 0;
    _Pragma( "loopbound min 0 max 0" )
    for ( i = jz - 1; i >= jk; i-- )
      j |= iq[ i ];
    if ( j == 0 ) { /* need recomputation */
      _Pragma( "loopbound min 0 max 0" )
      for ( k = 1; iq[ jk - k ] == 0; k++ ) {
        ;   /* k = no. of terms needed */
      }

      _Pragma( "loopbound min 0 max 0" )
      for ( i = jz + 1; i <= jz + k; i++ ) { /* add q[jz+1] to q[jz+k] */
        f[ jx + i ] = ( float ) ipio2[ jv + i ];
        _Pragma( "loopbound min 0 max 0" )
        for ( j = 0, fw = 0.0f; j <= jx; j++ ) fw += x[ j ] * f[ jx + i - j ];
        q[i] = fw;
      }
      jz += k;
      goto recompute;
    }
  }

  /* chop off zero terms */
  if ( z == ( float )0.0f ) {
    jz -= 1;
    q0 -= 8;
    _Pragma( "loopbound min 0 max 0" )
    while ( iq[ jz ] == 0 ) {
      jz--;
      q0 -= 8;
    }
  } else { /* break z into 8-bit if necessary */
    z = __scalbnf( z, -q0 );
    if ( z >= two8 ) {
      fw = ( float )( ( int32_t )( twon8 * z ) );
      iq[ jz ] = ( int32_t )( z - two8 * fw );
      jz += 1;
      q0 += 8;
      iq[ jz ] = ( int32_t ) fw;
    } else iq[ jz ] = ( int32_t ) z ;
  }

  /* convert integer "bit" chunk to floating-point value */
  fw = __scalbnf( one, q0 );
  _Pragma( "loopbound min 0 max 0" )
  for ( i = jz; i >= 0; i-- ) {
    q[ i ] = fw * ( float )iq[ i ];
    fw *= twon8;
  }

  /* compute PIo2[0,...,jp]*q[jz,...,0] */
  _Pragma( "loopbound min 0 max 0" )
  for ( i = jz; i >= 0; i-- ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( fw = 0.0f, k = 0; k <= jp && k <= jz - i; k++ )
      fw += PIo2[ k ] * q[ i + k ];
    fq[ jz - i ] = fw;
  }

  /* compress fq[] into y[] */
  switch ( prec ) {
    case 0:
      fw = 0.0f;
      _Pragma( "loopbound min 0 max 0" )
      for ( i = jz; i >= 0; i-- )
        fw += fq[ i ];
      y[ 0 ] = ( ih == 0 ) ? fw : -fw;
      break;
    case 1:
    case 2:
      fw = 0.0f;
      _Pragma( "loopbound min 0 max 0" )
      for ( i = jz; i >= 0; i-- )
        fw += fq[ i ];
      y[ 0 ] = ( ih == 0 ) ? fw : -fw;
      fw = fq[ 0 ] - fw;
      _Pragma( "loopbound min 0 max 0" )
      for ( i = 1; i <= jz; i++ )
        fw += fq[ i ];
      y[ 1 ] = ( ih == 0 ) ? fw : -fw;
      break;
    case 3:;  /* painful */
      _Pragma( "loopbound min 0 max 0" )
      for ( i = jz; i > 0; i-- ) {
        fw      = fq[ i - 1 ] + fq[ i ];
        fq[ i ]  += fq[ i - 1 ] - fw;
        fq[ i - 1 ] = fw;
      }
      _Pragma( "loopbound min 0 max 0" )
      for ( i = jz; i > 1; i-- ) {
        fw      = fq[ i - 1 ] + fq[ i ];
        fq[ i ]  += fq[ i - 1 ] - fw;
        fq[ i - 1 ] = fw;
      }
      _Pragma( "loopbound min 0 max 0" )
      for ( fw = 0.0f, i = jz; i >= 2; i-- )
        fw += fq[ i ];

      if ( ih == 0 ) {
        y[ 0 ] =  fq[ 0 ];
        y[ 1 ] =  fq[ 1 ];
        y[ 2 ] =  fw;
      } else {
        y[ 0 ] = -fq[ 0 ];
        y[ 1 ] = -fq[ 1 ];
        y[ 2 ] = -fw;
      }
  }
  return n & 7;
}
/* k_sinf.c -- float version of k_sin.c
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] = "$NetBSD: k_sinf.c,v 1.4 1995/05/10 20:46:33 jtc Exp $";
#endif


#ifdef __STDC__
const float
#else
float
#endif
/* half =  5.0000000000e-01f,/\* 0x3f000000 *\/ */
S1  = -1.6666667163e-01f, /* 0xbe2aaaab */
S2  =  8.3333337680e-03f, /* 0x3c088889 */
S3  = -1.9841270114e-04f, /* 0xb9500d01 */
S4  =  2.7557314297e-06f, /* 0x3638ef1b */
S5  = -2.5050759689e-08f, /* 0xb2d72f34 */
S6  =  1.5896910177e-10f; /* 0x2f2ec9d3 */

#ifdef __STDC__
float __kernel_sinf( float x, float y, int iy )
#else
float __kernel_sinf( x, y, iy )
float x, y;
int iy;    /* iy=0 if y is zero */
#endif
{
  float z, r, v;
  int32_t ix;
  GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* high word of x */
  if ( ix < 0x32000000 ) { /* |x| < 2**-27 */
    if ( ( int )x == 0 ) return x; /* generate inexact */
  }
  z =  x * x;
  v =  z * x;
  r =  S2 + z * ( S3 + z * ( S4 + z * ( S5 + z * S6 ) ) );
  if ( iy == 0 ) return x + v * ( S1 + z * r );
  else      return x - ( ( z * ( half * y - v * r ) - y ) - v * S1 );
}
/* s_atanf.c -- float version of s_atan.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] = "$NetBSD: s_atanf.c,v 1.4 1995/05/10 20:46:47 jtc Exp $";
#endif


#ifdef __STDC__
const float atanhi[] = {
#else
float atanhi[] = {
#endif
  4.6364760399e-01f, /* atan(0.5)hi 0x3eed6338 */
  7.8539812565e-01f, /* atan(1.0)hi 0x3f490fda */
  9.8279368877e-01f, /* atan(1.5)hi 0x3f7b985e */
  1.5707962513e+00f, /* atan(inf)hi 0x3fc90fda */
};

#ifdef __STDC__
const float atanlo[] = {
#else
float atanlo[] = {
#endif
  5.0121582440e-09f, /* atan(0.5)lo 0x31ac3769 */
  3.7748947079e-08f, /* atan(1.0)lo 0x33222168 */
  3.4473217170e-08f, /* atan(1.5)lo 0x33140fb4 */
  7.5497894159e-08f, /* atan(inf)lo 0x33a22168 */
};

#ifdef __STDC__
const float aT[] = {
#else
float aT[] = {
#endif
  3.3333334327e-01f, /* 0x3eaaaaaa */
  -2.0000000298e-01f, /* 0xbe4ccccd */
  1.4285714924e-01f, /* 0x3e124925 */
  -1.1111110449e-01f, /* 0xbde38e38 */
  9.0908870101e-02f, /* 0x3dba2e6e */
  -7.6918758452e-02f, /* 0xbd9d8795 */
  6.6610731184e-02f, /* 0x3d886b35 */
  -5.8335702866e-02f, /* 0xbd6ef16b */
  4.9768779427e-02f, /* 0x3d4bda59 */
  -3.6531571299e-02f, /* 0xbd15a221 */
  1.6285819933e-02f, /* 0x3c8569d7 */
};

/* #ifdef __STDC__ */
/*  static const float  */
/* #else */
/*  static float  */
/* #endif */
/* one   = 1.0, */
/* huge   = 1.0e30; */

#ifdef __STDC__
float __atanf( float x )
#else
float __atanf( x )
float x;
#endif
{
  float w, s1, s2, z;
  int32_t ix, hx, id;

  GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix >= 0x50800000 ) { /* if |x| >= 2^34 */
    if ( ix > 0x7f800000 )
      return x + x; /* NaN */
    if ( hx > 0 ) return  atanhi[ 3 ] + atanlo[ 3 ];
    else     return -atanhi[ 3 ] - atanlo[ 3 ];
  }
  if ( ix < 0x3ee00000 ) { /* |x| < 0.4375 */
    if ( ix < 0x31000000 ) { /* |x| < 2^-29 */
      if ( huge + x > one ) return x; /* raise inexact */
    }
    id = -1;
  } else {
    x = fabsf( x );
    if ( ix < 0x3f980000 ) {  /* |x| < 1.1875 */
      if ( ix < 0x3f300000 ) { /* 7/16 <=|x|<11/16 */
        id = 0;
        x = ( ( float )2.0f * x - one ) / ( ( float )2.0f + x );
      } else {      /* 11/16<=|x|< 19/16 */
        id = 1;
        x  = ( x - one ) / ( x + one );
      }
    } else {
      if ( ix < 0x401c0000 ) { /* |x| < 2.4375 */
        id = 2;
        x  = ( x - ( float )1.5f ) / ( one + ( float )1.5f * x );
      } else {      /* 2.4375 <= |x| < 2^66 */
        id = 3;
        x  = -( float )1.0f / x;
      }
    }
  }
  /* end of argument reduction */
  z = x * x;
  w = z * z;
  /* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
  s1 = z * ( aT[ 0 ] + w * ( aT[ 2 ] + w * ( aT[ 4 ] + w * ( aT[ 6 ] + w *
                                         ( aT[ 8 ] + w * aT[ 10 ] ) ) ) ) );
  s2 = w * ( aT[ 1 ] + w * ( aT[ 3 ] + w * ( aT[ 5 ] + w * ( aT[ 7 ] + w * aT[ 9 ] ) ) ) );
  if ( id < 0 ) return x - x * ( s1 + s2 );
  else {
    z = atanhi[ id ] - ( ( x * ( s1 + s2 ) - atanlo[ id ] ) - x );
    return ( hx < 0 ) ? -z : z;
  }
}
//weak_alias (__atanf, atanf)
/* s_copysignf.c -- float version of s_copysign.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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
   copysignf(float x, float y)
   copysignf(x,y) returns a value with the magnitude of x and
   with the sign bit of y.
*/


#ifdef __STDC__
float __copysignf( float x, float y )
#else
float __copysignf( x, y )
float x, y;
#endif
{
  u_int32_t ix, iy;
  GET_FLOAT_WORD( ix, x );
  GET_FLOAT_WORD( iy, y );
  SET_FLOAT_WORD( x, ( ix & 0x7fffffff ) | ( iy & 0x80000000 ) );
  return x;
}
//weak_alias (__copysignf, copysignf)
/* s_cosf.c -- float version of s_cos.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

/* #ifdef __STDC__ */
/* static const float one=1.0; */
/* #else */
/* static float one=1.0; */
/* #endif */

#ifdef __STDC__
float __cosf( float x )
#else
float __cosf( x )
float x;
#endif
{
  float y[ 2 ], z = 0.0f;
  int32_t n, ix;

  GET_FLOAT_WORD( ix, x );

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) return __kernel_cosf( x, z );

  /* cos(Inf or NaN) is NaN */
  else
    if ( ix >= 0x7f800000 ) return x - x;

    /* argument reduction needed */
    else {
      n = __ieee754_rem_pio2f( x, y );
      switch ( n & 3 ) {
        case 0:
          return  __kernel_cosf( y[ 0 ], y[ 1 ] );
        case 1:
          return -__kernel_sinf( y[ 0 ], y[ 1], 1 );
        case 2:
          return -__kernel_cosf( y[ 0 ], y[ 1 ] );
        default:
          return  __kernel_sinf( y[ 0 ], y[ 1 ], 1 );
      }
    }
}

/* s_fabsf.c -- float version of s_fabs.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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
   fabsf(x) returns the absolute value of x.
*/


#ifdef __STDC__
float __fabsf( float x )
#else
float __fabsf( x )
float x;
#endif
{
  u_int32_t ix;
  GET_FLOAT_WORD( ix, x );
  SET_FLOAT_WORD( x, ix & 0x7fffffff );
  return x;
}

/* s_floorf.c -- float version of s_floor.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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
   floorf(x)
   Return x rounded toward -inf to integral value
   Method:
    Bit twiddling.
   Exception:
    Inexact flag raised if x not equal to floorf(x).
*/


/* #ifdef __STDC__ */
/* static const float huge = 1.0e30f; */
/* #else */
/* static float huge = 1.0e30f; */
/* #endif */

#ifdef __STDC__
float __floorf( float x )
#else
float __floorf( x )
float x;
#endif
{
  int32_t i0, j0;
  u_int32_t i;
  GET_FLOAT_WORD( i0, x );
  j0 = ( ( i0 >> 23 ) & 0xff ) - 0x7f;
  if ( j0 < 23 ) {
    if ( j0 < 0 ) { /* raise inexact if x != 0 */
      if ( huge + x > ( float )0.0f ) { /* return 0*sign(x) if |x|<1 */
        if ( i0 >= 0 )
          i0 = 0;
        else
          if ( ( i0 & 0x7fffffff ) != 0 )
            i0 = 0xbf800000;
      }
    } else {
      i = ( 0x007fffff ) >> j0;
      if ( ( i0 & i ) == 0 ) return x; /* x is integral */
      if ( huge + x > ( float )0.0f ) { /* raise inexact flag */
        if ( i0 < 0 ) i0 += ( 0x00800000 ) >> j0;
        i0 &= ( ~i );
      }
    }
  } else {
    if ( j0 == 0x80 ) return x + x; /* inf or NaN */
    else return x;    /* x is integral */
  }
  SET_FLOAT_WORD( x, i0 );
  return x;
}
//weak_alias (__floorf, floorf)
/*
   Written by J.T. Conklin <jtc@netbsd.org>.
   Public domain.
*/

/*
   isinff(x) returns 1 if x is inf, -1 if x is -inf, else 0;
   no branching!
*/


int
__isinff ( float x )
{
  int32_t ix, t;
  GET_FLOAT_WORD( ix, x );
  t = ix & 0x7fffffff;
  t ^= 0x7f800000;
  t |= -t;
  return ~( t >> 31 ) & ( ix >> 30 );
}
/* hidden_def (__isinff) */
/* weak_alias (__isinff, isinff) */
/* s_scalbnf.c -- float version of s_scalbn.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifdef __STDC__
const float
#else
float
#endif
twom25  =  2.9802322388e-08f; /* 0x33000000 */

#ifdef __STDC__
float __scalbnf ( float x, int n )
#else
float __scalbnf ( x, n )
float x;
int n;
#endif
{
  int32_t k, ix;
  GET_FLOAT_WORD( ix, x );
  k = ( ix & 0x7f800000 ) >> 23; /* extract exponent */
  if ( k == 0 ) {   /* 0 or subnormal x */
    if ( ( ix & 0x7fffffff ) == 0 ) return x; /* +-0 */
    x *= two25;
    GET_FLOAT_WORD( ix, x );
    k = ( ( ix & 0x7f800000 ) >> 23 ) - 25;
  }
  if ( k == 0xff ) return x + x; /* NaN or Inf */
  k = k + n;
  if ( n > 50000 || k >  0xfe )
    return huge * __copysignf( huge, x ); /* overflow  */
  if ( n < -50000 )
    return tiny * __copysignf( tiny, x ); /*underflow*/
  if ( k > 0 ) {    /* normal result */
    SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
    return x;
  }
  if ( k <= -25 )
    return tiny * __copysignf( tiny, x ); /*underflow*/
  k += 25;        /* subnormal result */
  SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
  return x * twom25;
}
//weak_alias (__scalbnf, scalbnf)



/* e_log10f.c -- float version of e_log10.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifdef __STDC__
const float
#else
float
#endif
ivln10     =  4.3429449201e-01f, /* 0x3ede5bd9 */
log10_2hi  =  3.0102920532e-01f, /* 0x3e9a2080 */
log10_2lo  =  7.9034151668e-07f; /* 0x355427db */

#ifdef __STDC__
float __ieee754_log10f( float x )
#else
float __ieee754_log10f( x )
float x;
#endif
{
  float y, z;
  int32_t i, k, hx;

  GET_FLOAT_WORD( hx, x );

  k = 0;
  if ( hx < 0x00800000 ) {    /* x < 2**-126  */
    if ( ( hx & 0x7fffffff ) == 0 )
      return -two25 / ( x - x ); /* log(+-0)=-inf */
    if ( hx < 0 ) return ( x - x ) / ( x - x ); /* log(-#) = NaN */
    k -= 25;
    x *= two25; /* subnormal number, scale up x */
    GET_FLOAT_WORD( hx, x );
  }
  if ( hx >= 0x7f800000 ) return x + x;
  k += ( hx >> 23 ) - 127;
  i  = ( ( u_int32_t )k & 0x80000000 ) >> 31;
  hx = ( hx & 0x007fffff ) | ( ( 0x7f - i ) << 23 );
  y  = ( float )( k + i );
  SET_FLOAT_WORD( x, hx );
  z  = y * log10_2lo + ivln10 * __ieee754_logf( x );
  return  z + y * log10_2hi;
}


/* e_logf.c -- float version of e_log.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifdef __STDC__
const float
#else
float
#endif
ln2_hi =   6.9313812256e-01f, /* 0x3f317180 */
ln2_lo =   9.0580006145e-06f, /* 0x3717f7d1 */
Lg1 = 6.6666668653e-01f,  /* 3F2AAAAB */
Lg2 = 4.0000000596e-01f,  /* 3ECCCCCD */
Lg3 = 2.8571429849e-01f, /* 3E924925 */
Lg4 = 2.2222198546e-01f, /* 3E638E29 */
Lg5 = 1.8183572590e-01f, /* 3E3A3325 */
Lg6 = 1.5313838422e-01f, /* 3E1CD04F */
Lg7 = 1.4798198640e-01f; /* 3E178897 */

#ifdef __STDC__
float __ieee754_logf( float x )
#else
float __ieee754_logf( x )
float x;
#endif
{
  float hfsq, f, s, z, R, w, t1, t2, dk;
  int32_t k, ix, i, j;

  GET_FLOAT_WORD( ix, x );

  k = 0;
  if ( ix < 0x00800000 ) {    /* x < 2**-126  */
    if ( ( ix & 0x7fffffff ) == 0 )
      return -two25 / ( x - x ); /* log(+-0)=-inf */
    if ( ix < 0 ) return ( x - x ) / ( x - x ); /* log(-#) = NaN */
    k -= 25;
    x *= two25; /* subnormal number, scale up x */
    GET_FLOAT_WORD( ix, x );
  }
  if ( ix >= 0x7f800000 ) return x + x;
  k += ( ix >> 23 ) - 127;
  ix &= 0x007fffff;
  i = ( ix + ( 0x95f64 << 3 ) ) & 0x800000;
  SET_FLOAT_WORD( x, ix | ( i ^ 0x3f800000 ) ); /* normalize x or x/2 */
  k += ( i >> 23 );
  f = x - ( float )1.0;
  if ( ( 0x007fffff & ( 15 + ix ) ) < 16 ) { /* |f| < 2**-20 */
    if ( f == zero ) {
      if ( k == 0 ) return zero;
      else {
        dk = ( float )k;
        return dk * ln2_hi + dk * ln2_lo;
      }
    }
    R = f * f * ( ( float )0.5f - ( float )0.33333333333333333f * f );
    if ( k == 0 ) return f - R;
    else {
      dk = ( float )k;
      return dk * ln2_hi - ( ( R - dk * ln2_lo ) - f );
    }
  }
  s = f / ( ( float )2.0f + f );
  dk = ( float )k;
  z = s * s;
  i = ix - ( 0x6147a << 3 );
  w = z * z;
  j = ( 0x6b851 << 3 ) - ix;
  t1 = w * ( Lg2 + w * ( Lg4 + w * Lg6 ) );
  t2 = z * ( Lg1 + w * ( Lg3 + w * ( Lg5 + w * Lg7 ) ) );
  i |= j;
  R = t2 + t1;
  if ( i > 0 ) {
    hfsq = ( float )0.5f * f * f;
    if ( k == 0 ) return f - ( hfsq - s * ( hfsq + R ) );
    else
      return dk * ln2_hi - ( ( hfsq - ( s * ( hfsq + R ) + dk * ln2_lo ) ) - f );
  } else {
    if ( k == 0 ) return f - s * ( f - R );
    else
      return dk * ln2_hi - ( ( s * ( f - R ) - dk * ln2_lo ) - f );
  }
}


/* s_sinf.c -- float version of s_sin.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifdef __STDC__
float __sinf( float x )
#else
float __sinf( x )
float x;
#endif
{
  float y[ 2 ], z = 0.0;
  int32_t n, ix;

  GET_FLOAT_WORD( ix, x );

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) return __kernel_sinf( x, z, 0 );

  /* sin(Inf or NaN) is NaN */
  else
    if ( ix >= 0x7f800000 ) return x - x;

    /* argument reduction needed */
    else {
      n = __ieee754_rem_pio2f( x, y );
      switch ( n & 3 ) {
        case 0:
          return  __kernel_sinf( y[ 0 ], y[ 1 ], 1 );
        case 1:
          return  __kernel_cosf( y[ 0 ], y[ 1 ] );
        case 2:
          return -__kernel_sinf( y[ 0 ], y[ 1 ], 1 );
        default:
          return -__kernel_cosf( y[ 0 ], y[ 1 ] );
      }
    }
}


/* Single-precision floating point e^x.
   Copyright (C) 1997, 1998, 2005, 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Geoffrey Keating <geoffk@ozemail.com.au>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* How this works:

   The input value, x, is written as

   x = n * ln(2) + t/512 + delta[t] + x;

   where:
   - n is an integer, 127 >= n >= -150;
   - t is an integer, 177 >= t >= -177
   - delta is based on a table entry, delta[t] < 2^-28
   - x is whatever is left, |x| < 2^-10

   Then e^x is approximated as

   e^x = 2^n ( e^(t/512 + delta[t])
               + ( e^(t/512 + delta[t])
                     ( p(x + delta[t] + n * ln(2)) - delta ) ) )

   where
   - p(x) is a polynomial approximating e(x)-1;
   - e^(t/512 + delta[t]) is obtained from a table.

   The table used is the same one as for the double precision version;
   since we have the table, we might as well use it.

   It turns out to be faster to do calculations in double precision than
   to perform an 'accurate table method' expf, because of the range reduction
   overhead (compare exp2f).
*/


const float __exp_deltatable[ 178 ] = {
  0.000000e+00,   1.436239e-11,  -1.851323e-11,   1.250434e-12,
  -4.382040e-10,   4.014913e-13,   6.324634e-12,   1.473544e-12,
  -4.987366e-11,  -1.752952e-12,   6.321584e-13,  -3.260980e-12,
  2.777399e-12,  -3.787255e-11,  -2.182022e-13,  -9.047329e-13,
  7.118903e-12,  -3.555871e-13,  -5.198906e-12,  -4.124305e-13,
  1.940741e-12,   2.811727e-13,  -2.117377e-13,   2.782236e-14,
  1.074722e-13,  -8.844783e-13,  -1.240327e-16,   7.052319e-13,
  6.447490e-13,   4.011210e-13,   5.462592e-13,   1.843201e-12,
  -2.028864e-12,  -2.931258e-13,   8.606021e-12,  -5.626610e-13,
  1.295899e-13,  -2.330782e-12,  -5.478326e-13,   1.845989e-12,
  -4.234531e-12,   1.735903e-12,   1.518525e-13,   2.535486e-12,
  -4.674620e-13,  -1.051410e-12,  -1.666138e-12,  -9.372511e-13,
  3.200735e-12,   2.292755e-12,   2.525703e-12,   5.823944e-13,
  -9.789686e-13,   5.145901e-13,  -4.527062e-12,  -1.683974e-12,
  9.547877e-12,   2.078277e-13,  -2.575768e-12,  -1.026653e-12,
  8.179204e-13,   6.063405e-13,   3.908063e-13,  -2.855789e-13,
  -6.295203e-12,  -2.709950e-13,   4.477842e-14,   4.821464e-13,
  -5.566389e-13,   1.357999e-12,   7.670314e-13,  -9.506315e-12,
  -1.738782e-12,  -8.637127e-13,  -1.933394e-12,  -1.267901e-13,
  -4.426381e-13,   1.260695e-12,  -3.277632e-12,  -1.941204e-12,
  -4.246588e-12,   2.948061e-12,  -2.192153e-13,   1.066989e-12,
  3.009034e-13,   9.622095e-13,   2.332691e-13,  -2.541789e-12,
  -8.988149e-13,  -1.005515e-12,   1.152009e-12,   2.547998e-12,
  -8.128411e-12,  -5.987095e-12,  -1.230903e-12,   1.251520e-12,
  -1.144099e-12,  -1.419784e-14,   6.041652e-13,  -2.428500e-13,
  -6.869299e-12,   2.192179e-13,   3.987357e-13,  -1.482881e-12,
  3.078380e-13,   5.226606e-12,  -3.655583e-13,  -7.399489e-13,
  -2.937312e-13,   8.345330e-13,   8.291675e-13,   6.803750e-13,
  -3.151692e-12,   1.975098e-12,  -1.964634e-09,   1.060808e-12,
  5.123574e-12,   5.405112e-13,   4.460744e-12,   2.254359e-12,
  1.053220e-12,   4.224363e-12,   5.146482e-13,  -5.005111e-14,
  6.731776e-12,  -4.826085e-12,   1.779185e-12,   5.392274e-12,
  4.069803e-12,   5.218838e-13,  -4.566578e-12,   9.349049e-13,
  -2.780717e-12,   1.589945e-12,   1.805627e-12,  -8.564009e-13,
  -9.103265e-13,   1.747946e-12,   7.582737e-13,   1.880665e-12,
  -1.509317e-12,  -8.732831e-12,  -1.582241e-14,  -2.613867e-12,
  5.035911e-13,  -2.209310e-12,  -4.646361e-13,   1.926250e-12,
  1.114618e-12,   2.434626e-12,  -8.526912e-13,  -1.499753e-12,
  -1.008729e-12,  -2.222098e-12,   6.956137e-13,   2.119407e-13,
  1.250643e-12,  -6.520809e-12,   2.357324e-13,  -1.302915e-11,
  -2.031004e-12,  -7.972772e-13,   1.424409e-12,   4.138852e-12,
  -1.058267e-12,  -2.656184e-12,   5.452444e-13,   1.108587e-12,
  1.134814e-11,  -9.126315e-12,   2.758479e-12,  -1.439219e-12,
  3.073733e-12,   2.169448e-12,   3.600419e-15,  -2.955518e-13,
  -1.982464e-12,   2.164243e-12
};

const double __exp_atable[ 355 ] /* __attribute__((mode(DF))) */ = {
  0.707722561055888932371, /* 0x0.b52d4e46605c27ffd */
  0.709106182438804188967, /* 0x0.b587fb96f75097ffb */
  0.710492508843861281234, /* 0x0.b5e2d649899167ffd */
  0.711881545564593931623, /* 0x0.b63dde74d36bdfffe */
  0.713273297897442870573, /* 0x0.b699142f945f87ffc */
  0.714667771153751463236, /* 0x0.b6f477909c4ea0001 */
  0.716064970655995725059, /* 0x0.b75008aec758f8004 */
  0.717464901723956938193, /* 0x0.b7abc7a0eea7e0002 */
  0.718867569715736398602, /* 0x0.b807b47e1586c7ff8 */
  0.720272979947266023271, /* 0x0.b863cf5d10e380003 */
  0.721681137825144314297, /* 0x0.b8c01855195c37ffb */
  0.723092048691992950199, /* 0x0.b91c8f7d213740004 */
  0.724505717938892290800, /* 0x0.b97934ec5002d0007 */
  0.725922150953176470431, /* 0x0.b9d608b9c92ea7ffc */
  0.727341353138962865022, /* 0x0.ba330afcc29e98003 */
  0.728763329918453162104, /* 0x0.ba903bcc8618b7ffc */
  0.730188086709957051568, /* 0x0.baed9b40591ba0000 */
  0.731615628948127705309, /* 0x0.bb4b296f931e30002 */
  0.733045962086486091436, /* 0x0.bba8e671a05617ff9 */
  0.734479091556371366251, /* 0x0.bc06d25dd49568001 */
  0.735915022857225542529, /* 0x0.bc64ed4bce8f6fff9 */
  0.737353761441304711410, /* 0x0.bcc33752f915d7ff9 */
  0.738795312814142124419, /* 0x0.bd21b08af98e78005 */
  0.740239682467211168593, /* 0x0.bd80590b65e9a8000 */
  0.741686875913991849885, /* 0x0.bddf30ebec4a10000 */
  0.743136898669507939299, /* 0x0.be3e38443c84e0007 */
  0.744589756269486091620, /* 0x0.be9d6f2c1d32a0002 */
  0.746045454254026796384, /* 0x0.befcd5bb59baf8004 */
  0.747503998175051087583, /* 0x0.bf5c6c09ca84c0003 */
  0.748965393601880857739, /* 0x0.bfbc322f5b18b7ff8 */
  0.750429646104262104698, /* 0x0.c01c2843f776fffff */
  0.751896761271877989160, /* 0x0.c07c4e5fa18b88002 */
  0.753366744698445112140, /* 0x0.c0dca49a5fb18fffd */
  0.754839601988627206827, /* 0x0.c13d2b0c444db0005 */
  0.756315338768691947122, /* 0x0.c19de1cd798578006 */
  0.757793960659406629066, /* 0x0.c1fec8f623723fffd */
  0.759275473314173443536, /* 0x0.c25fe09e8a0f47ff8 */
  0.760759882363831851927, /* 0x0.c2c128dedc88f8000 */
  0.762247193485956486805, /* 0x0.c322a1cf7d6e7fffa */
  0.763737412354726363781, /* 0x0.c3844b88cb9347ffc */
  0.765230544649828092739, /* 0x0.c3e626232bd8f7ffc */
  0.766726596071518051729, /* 0x0.c44831b719bf18002 */
  0.768225572321911687194, /* 0x0.c4aa6e5d12d078001 */
  0.769727479119219348810, /* 0x0.c50cdc2da64a37ffb */
  0.771232322196981678892, /* 0x0.c56f7b41744490001 */
  0.772740107296721268087, /* 0x0.c5d24bb1259e70004 */
  0.774250840160724651565, /* 0x0.c6354d95640dd0007 */
  0.775764526565368872643, /* 0x0.c6988106fec447fff */
  0.777281172269557396602, /* 0x0.c6fbe61eb1bd0ffff */
  0.778800783068235302750, /* 0x0.c75f7cf560942fffc */
  0.780323364758801041312, /* 0x0.c7c345a3f1983fffe */
  0.781848923151573727006, /* 0x0.c8274043594cb0002 */
  0.783377464064598849602, /* 0x0.c88b6cec94b3b7ff9 */
  0.784908993312207869935, /* 0x0.c8efcbb89cba27ffe */
  0.786443516765346961618, /* 0x0.c9545cc0a88c70003 */
  0.787981040257604625744, /* 0x0.c9b9201dc643bfffa */
  0.789521569657452682047, /* 0x0.ca1e15e92a5410007 */
  0.791065110849462849192, /* 0x0.ca833e3c1ae510005 */
  0.792611669712891875319, /* 0x0.cae8992fd84667ffd */
  0.794161252150049179450, /* 0x0.cb4e26ddbc207fff8 */
  0.795713864077794763584, /* 0x0.cbb3e75f301b60003 */
  0.797269511407239561694, /* 0x0.cc19dacd978cd8002 */
  0.798828200086368567220, /* 0x0.cc8001427e55d7ffb */
  0.800389937624300440456, /* 0x0.cce65ade24d360006 */
  0.801954725261124767840, /* 0x0.cd4ce7a5de839fffb */
  0.803522573691593189330, /* 0x0.cdb3a7c79a678fffd */
  0.805093487311204114563, /* 0x0.ce1a9b563965ffffc */
  0.806667472122675088819, /* 0x0.ce81c26b838db8000 */
  0.808244534127439906441, /* 0x0.cee91d213f8428002 */
  0.809824679342317166307, /* 0x0.cf50ab9144d92fff9 */
  0.811407913793616542005, /* 0x0.cfb86dd5758c2ffff */
  0.812994243520784198882, /* 0x0.d0206407c20e20005 */
  0.814583674571603966162, /* 0x0.d0888e4223facfff9 */
  0.816176213022088536960, /* 0x0.d0f0ec9eb3f7c8002 */
  0.817771864936188586101, /* 0x0.d1597f377d6768002 */
  0.819370636400374108252, /* 0x0.d1c24626a46eafff8 */
  0.820972533518165570298, /* 0x0.d22b41865ff1e7ff9 */
  0.822577562404315121269, /* 0x0.d2947170f32ec7ff9 */
  0.824185729164559344159, /* 0x0.d2fdd60097795fff8 */
  0.825797039949601741075, /* 0x0.d3676f4fb796d0001 */
  0.827411500902565544264, /* 0x0.d3d13d78b5f68fffb */
  0.829029118181348834154, /* 0x0.d43b40960546d8001 */
  0.830649897953322891022, /* 0x0.d4a578c222a058000 */
  0.832273846408250750368, /* 0x0.d50fe617a3ba78005 */
  0.833900969738858188772, /* 0x0.d57a88b1218e90002 */
  0.835531274148056613016, /* 0x0.d5e560a94048f8006 */
  0.837164765846411529371, /* 0x0.d6506e1aac8078003 */
  0.838801451086016225394, /* 0x0.d6bbb1204074e0001 */
  0.840441336100884561780, /* 0x0.d72729d4c28518004 */
  0.842084427144139224814, /* 0x0.d792d8530e12b0001 */
  0.843730730487052604790, /* 0x0.d7febcb61273e7fff */
  0.845380252404570153833, /* 0x0.d86ad718c308dfff9 */
  0.847032999194574087728, /* 0x0.d8d727962c69d7fff */
  0.848688977161248581090, /* 0x0.d943ae49621ce7ffb */
  0.850348192619261200615, /* 0x0.d9b06b4d832ef8005 */
  0.852010651900976245816, /* 0x0.da1d5ebdc22220005 */
  0.853676361342631029337, /* 0x0.da8a88b555baa0006 */
  0.855345327311054837175, /* 0x0.daf7e94f965f98004 */
  0.857017556155879489641, /* 0x0.db6580a7c98f7fff8 */
  0.858693054267390953857, /* 0x0.dbd34ed9617befff8 */
  0.860371828028939855647, /* 0x0.dc4153ffc8b65fff9 */
  0.862053883854957292436, /* 0x0.dcaf90368bfca8004 */
  0.863739228154875360306, /* 0x0.dd1e0399328d87ffe */
  0.865427867361348468455, /* 0x0.dd8cae435d303fff9 */
  0.867119807911702289458, /* 0x0.ddfb9050b1cee8006 */
  0.868815056264353846599, /* 0x0.de6aa9dced8448001 */
  0.870513618890481399881, /* 0x0.ded9fb03db7320006 */
  0.872215502247877139094, /* 0x0.df4983e1380657ff8 */
  0.873920712852848668986, /* 0x0.dfb94490ffff77ffd */
  0.875629257204025623884, /* 0x0.e0293d2f1cb01fff9 */
  0.877341141814212965880, /* 0x0.e0996dd786fff0007 */
  0.879056373217612985183, /* 0x0.e109d6a64f5d57ffc */
  0.880774957955916648615, /* 0x0.e17a77b78e72a7ffe */
  0.882496902590150900078, /* 0x0.e1eb5127722cc7ff8 */
  0.884222213673356738383, /* 0x0.e25c63121fb0c8006 */
  0.885950897802399772740, /* 0x0.e2cdad93ec5340003 */
  0.887682961567391237685, /* 0x0.e33f30c925fb97ffb */
  0.889418411575228162725, /* 0x0.e3b0ecce2d05ffff9 */
  0.891157254447957902797, /* 0x0.e422e1bf727718006 */
  0.892899496816652704641, /* 0x0.e4950fb9713fc7ffe */
  0.894645145323828439008, /* 0x0.e50776d8b0e60fff8 */
  0.896394206626591749641, /* 0x0.e57a1739c8fadfffc */
  0.898146687421414902124, /* 0x0.e5ecf0f97c5798007 */
  0.899902594367530173098, /* 0x0.e660043464e378005 */
  0.901661934163603406867, /* 0x0.e6d3510747e150006 */
  0.903424713533971135418, /* 0x0.e746d78f06cd97ffd */
  0.905190939194458810123, /* 0x0.e7ba97e879c91fffc */
  0.906960617885092856864, /* 0x0.e82e92309390b0007 */
  0.908733756358986566306, /* 0x0.e8a2c6845544afffa */
  0.910510361377119825629, /* 0x0.e9173500c8abc7ff8 */
  0.912290439722343249336, /* 0x0.e98bddc30f98b0002 */
  0.914073998177417412765, /* 0x0.ea00c0e84bc4c7fff */
  0.915861043547953501680, /* 0x0.ea75de8db8094fffe */
  0.917651582652244779397, /* 0x0.eaeb36d09d3137ffe */
  0.919445622318405764159, /* 0x0.eb60c9ce4ed3dffff */
  0.921243169397334638073, /* 0x0.ebd697a43995b0007 */
  0.923044230737526172328, /* 0x0.ec4ca06fc7768fffa */
  0.924848813220121135342, /* 0x0.ecc2e44e865b6fffb */
  0.926656923710931002014, /* 0x0.ed39635df34e70006 */
  0.928468569126343790092, /* 0x0.edb01dbbc2f5b7ffa */
  0.930283756368834757725, /* 0x0.ee2713859aab57ffa */
  0.932102492359406786818, /* 0x0.ee9e44d9342870004 */
  0.933924784042873379360, /* 0x0.ef15b1d4635438005 */
  0.935750638358567643520, /* 0x0.ef8d5a94f60f50007 */
  0.937580062297704630580, /* 0x0.f0053f38f345cffff */
  0.939413062815381727516, /* 0x0.f07d5fde3a2d98001 */
  0.941249646905368053689, /* 0x0.f0f5bca2d481a8004 */
  0.943089821583810716806, /* 0x0.f16e55a4e497d7ffe */
  0.944933593864477061592, /* 0x0.f1e72b028a2827ffb */
  0.946780970781518460559, /* 0x0.f2603cd9fb5430001 */
  0.948631959382661205081, /* 0x0.f2d98b497d2a87ff9 */
  0.950486566729423554277, /* 0x0.f353166f63e3dffff */
  0.952344799896018723290, /* 0x0.f3ccde6a11ae37ffe */
  0.954206665969085765512, /* 0x0.f446e357f66120000 */
  0.956072172053890279009, /* 0x0.f4c12557964f0fff9 */
  0.957941325265908139014, /* 0x0.f53ba48781046fffb */
  0.959814132734539637840, /* 0x0.f5b66106555d07ffa */
  0.961690601603558903308, /* 0x0.f6315af2c2027fffc */
  0.963570739036113010927, /* 0x0.f6ac926b8aeb80004 */
  0.965454552202857141381, /* 0x0.f728078f7c5008002 */
  0.967342048278315158608, /* 0x0.f7a3ba7d66a908001 */
  0.969233234469444204768, /* 0x0.f81fab543e1897ffb */
  0.971128118008140250896, /* 0x0.f89bda33122c78007 */
  0.973026706099345495256, /* 0x0.f9184738d4cf97ff8 */
  0.974929006031422851235, /* 0x0.f994f284d3a5c0008 */
  0.976835024947348973265, /* 0x0.fa11dc35bc7820002 */
  0.978744770239899142285, /* 0x0.fa8f046b4fb7f8007 */
  0.980658249138918636210, /* 0x0.fb0c6b449ab1cfff9 */
  0.982575468959622777535, /* 0x0.fb8a10e1088fb7ffa */
  0.984496437054508843888, /* 0x0.fc07f5602d79afffc */
  0.986421160608523028820, /* 0x0.fc8618e0e55e47ffb */
  0.988349647107594098099, /* 0x0.fd047b83571b1fffa */
  0.990281903873210800357, /* 0x0.fd831d66f4c018002 */
  0.992217938695037382475, /* 0x0.fe01fead3320bfff8 */
  0.994157757657894713987, /* 0x0.fe811f703491e8006 */
  0.996101369488558541238, /* 0x0.ff007fd5744490005 */
  0.998048781093141101932, /* 0x0.ff801ffa9b9280007 */
  1.000000000000000000000, /* 0x1.00000000000000000 */
  1.001955033605393285965, /* 0x1.0080200565d29ffff */
  1.003913889319761887310, /* 0x1.0100802aa0e80fff0 */
  1.005876574715736104818, /* 0x1.01812090377240007 */
  1.007843096764807100351, /* 0x1.020201541aad7fff6 */
  1.009813464316352327214, /* 0x1.0283229c4c9820007 */
  1.011787683565730677817, /* 0x1.030484836910a000e */
  1.013765762469146736174, /* 0x1.0386272b9c077fffe */
  1.015747708536026694351, /* 0x1.04080ab526304fff0 */
  1.017733529475172815584, /* 0x1.048a2f412375ffff0 */
  1.019723232714418781378, /* 0x1.050c94ef7ad5e000a */
  1.021716825883923762690, /* 0x1.058f3be0f1c2d0004 */
  1.023714316605201180057, /* 0x1.06122436442e2000e */
  1.025715712440059545995, /* 0x1.06954e0fec63afff2 */
  1.027721021151397406936, /* 0x1.0718b98f41c92fff6 */
  1.029730250269221158939, /* 0x1.079c66d49bb2ffff1 */
  1.031743407506447551857, /* 0x1.082056011a9230009 */
  1.033760500517691527387, /* 0x1.08a487359ebd50002 */
  1.035781537016238873464, /* 0x1.0928fa93490d4fff3 */
  1.037806524719013578963, /* 0x1.09adb03b3e5b3000d */
  1.039835471338248051878, /* 0x1.0a32a84e9e5760004 */
  1.041868384612101516848, /* 0x1.0ab7e2eea5340ffff */
  1.043905272300907460835, /* 0x1.0b3d603ca784f0009 */
  1.045946142174331239262, /* 0x1.0bc3205a042060000 */
  1.047991002016745332165, /* 0x1.0c4923682a086fffe */
  1.050039859627715177527, /* 0x1.0ccf698898f3a000d */
  1.052092722826109660856, /* 0x1.0d55f2dce5d1dfffb */
  1.054149599440827866881, /* 0x1.0ddcbf86b09a5fff6 */
  1.056210497317612961855, /* 0x1.0e63cfa7abc97fffd */
  1.058275424318780855142, /* 0x1.0eeb23619c146fffb */
  1.060344388322010722446, /* 0x1.0f72bad65714bffff */
  1.062417397220589476718, /* 0x1.0ffa9627c38d30004 */
  1.064494458915699715017, /* 0x1.1082b577d0eef0003 */
  1.066575581342167566880, /* 0x1.110b18e893a90000a */
  1.068660772440545025953, /* 0x1.1193c09c267610006 */
  1.070750040138235936705, /* 0x1.121cacb4959befff6 */
  1.072843392435016474095, /* 0x1.12a5dd543cf36ffff */
  1.074940837302467588937, /* 0x1.132f529d59552000b */
  1.077042382749654914030, /* 0x1.13b90cb250d08fff5 */
  1.079148036789447484528, /* 0x1.14430bb58da3dfff9 */
  1.081257807444460983297, /* 0x1.14cd4fc984c4a000e */
  1.083371702785017154417, /* 0x1.1557d910df9c7000e */
  1.085489730853784307038, /* 0x1.15e2a7ae292d30002 */
  1.087611899742884524772, /* 0x1.166dbbc422d8c0004 */
  1.089738217537583819804, /* 0x1.16f9157586772ffff */
  1.091868692357631731528, /* 0x1.1784b4e533cacfff0 */
  1.094003332327482702577, /* 0x1.18109a360fc23fff2 */
  1.096142145591650907149, /* 0x1.189cc58b155a70008 */
  1.098285140311341168136, /* 0x1.1929370751ea50002 */
  1.100432324652149906842, /* 0x1.19b5eecdd79cefff0 */
  1.102583706811727015711, /* 0x1.1a42ed01dbdba000e */
  1.104739294993289488947, /* 0x1.1ad031c69a2eafff0 */
  1.106899097422573863281, /* 0x1.1b5dbd3f66e120003 */
  1.109063122341542140286, /* 0x1.1beb8f8fa8150000b */
  1.111231377994659874592, /* 0x1.1c79a8dac6ad0fff4 */
  1.113403872669181282605, /* 0x1.1d0809445a97ffffc */
  1.115580614653132185460, /* 0x1.1d96b0effc9db000e */
  1.117761612217810673898, /* 0x1.1e25a001332190000 */
  1.119946873713312474002, /* 0x1.1eb4d69bdb2a9fff1 */
  1.122136407473298902480, /* 0x1.1f4454e3bfae00006 */
  1.124330221845670330058, /* 0x1.1fd41afcbb48bfff8 */
  1.126528325196519908506, /* 0x1.2064290abc98c0001 */
  1.128730725913251964394, /* 0x1.20f47f31c9aa7000f */
  1.130937432396844410880, /* 0x1.21851d95f776dfff0 */
  1.133148453059692917203, /* 0x1.2216045b6784efffa */
  1.135363796355857157764, /* 0x1.22a733a6692ae0004 */
  1.137583470716100553249, /* 0x1.2338ab9b3221a0004 */
  1.139807484614418608939, /* 0x1.23ca6c5e27aadfff7 */
  1.142035846532929888057, /* 0x1.245c7613b7f6c0004 */
  1.144268564977221958089, /* 0x1.24eec8e06b035000c */
  1.146505648458203463465, /* 0x1.258164e8cea85fff8 */
  1.148747105501412235671, /* 0x1.26144a5180d380009 */
  1.150992944689175123667, /* 0x1.26a7793f5de2efffa */
  1.153243174560058870217, /* 0x1.273af1d712179000d */
  1.155497803703682491111, /* 0x1.27ceb43d81d42fff1 */
  1.157756840726344771440, /* 0x1.2862c097a3d29000c */
  1.160020294239811677834, /* 0x1.28f7170a74cf4fff1 */
  1.162288172883275239058, /* 0x1.298bb7bb0faed0004 */
  1.164560485298402170388, /* 0x1.2a20a2ce920dffff4 */
  1.166837240167474476460, /* 0x1.2ab5d86a4631ffff6 */
  1.169118446164539637555, /* 0x1.2b4b58b36d5220009 */
  1.171404112007080167155, /* 0x1.2be123cf786790002 */
  1.173694246390975415341, /* 0x1.2c7739e3c0aac000d */
  1.175988858069749065617, /* 0x1.2d0d9b15deb58fff6 */
  1.178287955789017793514, /* 0x1.2da4478b627040002 */
  1.180591548323240091978, /* 0x1.2e3b3f69fb794fffc */
  1.182899644456603782686, /* 0x1.2ed282d76421d0004 */
  1.185212252993012693694, /* 0x1.2f6a11f96c685fff3 */
  1.187529382762033236513, /* 0x1.3001ecf60082ffffa */
  1.189851042595508889847, /* 0x1.309a13f30f28a0004 */
  1.192177241354644978669, /* 0x1.31328716a758cfff7 */
  1.194507987909589896687, /* 0x1.31cb4686e1e85fffb */
  1.196843291137896336843, /* 0x1.32645269dfd04000a */
  1.199183159977805113226, /* 0x1.32fdaae604c39000f */
  1.201527603343041317132, /* 0x1.339750219980dfff3 */
  1.203876630171082595692, /* 0x1.3431424300e480007 */
  1.206230249419600664189, /* 0x1.34cb8170b3fee000e */
  1.208588470077065268869, /* 0x1.35660dd14dbd4fffc */
  1.210951301134513435915, /* 0x1.3600e78b6bdfc0005 */
  1.213318751604272271958, /* 0x1.369c0ec5c38ebfff2 */
  1.215690830512196507537, /* 0x1.373783a718d29000f */
  1.218067546930756250870, /* 0x1.37d3465662f480007 */
  1.220448909901335365929, /* 0x1.386f56fa770fe0008 */
  1.222834928513994334780, /* 0x1.390bb5ba5fc540004 */
  1.225225611877684750397, /* 0x1.39a862bd3c7a8fff3 */
  1.227620969111500981433, /* 0x1.3a455e2a37bcafffd */
  1.230021009336254911271, /* 0x1.3ae2a8287dfbefff6 */
  1.232425741726685064472, /* 0x1.3b8040df76f39fffa */
  1.234835175450728295084, /* 0x1.3c1e287682e48fff1 */
  1.237249319699482263931, /* 0x1.3cbc5f151b86bfff8 */
  1.239668183679933477545, /* 0x1.3d5ae4e2cc0a8000f */
  1.242091776620540377629, /* 0x1.3df9ba07373bf0006 */
  1.244520107762172811399, /* 0x1.3e98deaa0d8cafffe */
  1.246953186383919165383, /* 0x1.3f3852f32973efff0 */
  1.249391019292643401078, /* 0x1.3fd816ffc72b90001 */
  1.251833623164381181797, /* 0x1.40782b17863250005 */
  1.254280999953110153911, /* 0x1.41188f42caf400000 */
  1.256733161434815393410, /* 0x1.41b943b42945bfffd */
  1.259190116985283935980, /* 0x1.425a4893e5f10000a */
  1.261651875958665236542, /* 0x1.42fb9e0a2df4c0009 */
  1.264118447754797758244, /* 0x1.439d443f608c4fff9 */
  1.266589841787181258708, /* 0x1.443f3b5bebf850008 */
  1.269066067469190262045, /* 0x1.44e183883e561fff7 */
  1.271547134259576328224, /* 0x1.45841cecf7a7a0001 */
  1.274033051628237434048, /* 0x1.462707b2c43020009 */
  1.276523829025464573684, /* 0x1.46ca44023aa410007 */
  1.279019475999373156531, /* 0x1.476dd2045d46ffff0 */
  1.281520002043128991825, /* 0x1.4811b1e1f1f19000b */
  1.284025416692967214122, /* 0x1.48b5e3c3edd74fff4 */
  1.286535729509738823464, /* 0x1.495a67d3613c8fff7 */
  1.289050950070396384145, /* 0x1.49ff3e396e19d000b */
  1.291571087985403654081, /* 0x1.4aa4671f5b401fff1 */
  1.294096152842774794011, /* 0x1.4b49e2ae56d19000d */
  1.296626154297237043484, /* 0x1.4befb10fd84a3fff4 */
  1.299161101984141142272, /* 0x1.4c95d26d41d84fff8 */
  1.301701005575179204100, /* 0x1.4d3c46f01d9f0fff3 */
  1.304245874766450485904, /* 0x1.4de30ec21097d0003 */
  1.306795719266019562007, /* 0x1.4e8a2a0ccce3d0002 */
  1.309350548792467483458, /* 0x1.4f3198fa10346fff5 */
  1.311910373099227200545, /* 0x1.4fd95bb3be8cffffd */
  1.314475201942565174546, /* 0x1.50817263bf0e5fffb */
  1.317045045107389400535, /* 0x1.5129dd3418575000e */
  1.319619912422941299109, /* 0x1.51d29c4f01c54ffff */
  1.322199813675649204855, /* 0x1.527bafde83a310009 */
  1.324784758729532718739, /* 0x1.5325180cfb8b3fffd */
  1.327374757430096474625, /* 0x1.53ced504b2bd0fff4 */
  1.329969819671041886272, /* 0x1.5478e6f02775e0001 */
  1.332569955346704748651, /* 0x1.55234df9d8a59fff8 */
  1.335175174370685002822, /* 0x1.55ce0a4c5a6a9fff6 */
  1.337785486688218616860, /* 0x1.56791c1263abefff7 */
  1.340400902247843806217, /* 0x1.57248376aef21fffa */
  1.343021431036279800211, /* 0x1.57d040a420c0bfff3 */
  1.345647083048053138662, /* 0x1.587c53c5a630f0002 */
  1.348277868295411074918, /* 0x1.5928bd063fd7bfff9 */
  1.350913796821875845231, /* 0x1.59d57c9110ad60006 */
  1.353554878672557082439, /* 0x1.5a8292913d68cfffc */
  1.356201123929036356254, /* 0x1.5b2fff3212db00007 */
  1.358852542671913132777, /* 0x1.5bddc29edcc06fff3 */
  1.361509145047255398051, /* 0x1.5c8bdd032ed16000f */
  1.364170941142184734180, /* 0x1.5d3a4e8a5bf61fff4 */
  1.366837941171020309735, /* 0x1.5de9176042f1effff */
  1.369510155261156381121, /* 0x1.5e9837b062f4e0005 */
  1.372187593620959988833, /* 0x1.5f47afa69436cfff1 */
  1.374870266463378287715, /* 0x1.5ff77f6eb3f8cfffd */
  1.377558184010425845733, /* 0x1.60a7a734a9742fff9 */
  1.380251356531521533853, /* 0x1.6158272490016000c */
  1.382949794301995272203, /* 0x1.6208ff6a8978a000f */
  1.385653507605306700170, /* 0x1.62ba3032c0a280004 */
  1.388362506772382154503, /* 0x1.636bb9a994784000f */
  1.391076802081129493127, /* 0x1.641d9bfb29a7bfff6 */
  1.393796403973427855412, /* 0x1.64cfd7545928b0002 */
  1.396521322756352656542, /* 0x1.65826be167badfff8 */
  1.399251568859207761660, /* 0x1.663559cf20826000c */
  1.401987152677323100733, /* 0x1.66e8a14a29486fffc */
  1.404728084651919228815, /* 0x1.679c427f5a4b6000b */
  1.407474375243217723560, /* 0x1.68503d9ba0add000f */
  1.410226034922914983815, /* 0x1.690492cbf6303fff9 */
  1.412983074197955213304, /* 0x1.69b9423d7b548fff6 */
};

const volatile float TWOM100 = 7.88860905e-31;
const volatile float TWO127 = 1.7014118346e+38;


unsigned long _randseed = 1;

void srandom( unsigned long seed )
{
  _randseed = seed;
}

unsigned long random( void )
{
  long x, hi, lo, t;

  /*
     Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
     From "Random number generators: good ones are hard to find",
     Park and Miller, Communications of the ACM, vol. 31, no. 10,
     October 1988, p. 1195.
  */
  x = _randseed;
  hi = x / 127773;
  lo = x % 127773;
  t = 16807 * lo - 2836 * hi;
  if ( t <= 0 )
    t += 0x7fffffff;
  _randseed = t;
  return ( t );
}


/* s_ceilf.c -- float version of s_ceil.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#if defined(LIBM_SCCS) && !defined(lint)
char rcsid[] = "$NetBSD: s_ceilf.c,v 1.4 1995/05/10 20:46:55 jtc Exp $";
#endif

#ifdef __STDC__
float __ceilf( float x )
#else
float __ceilf( x )
float x;
#endif
{
  int32_t i0, j0;
  u_int32_t i;

  GET_FLOAT_WORD( i0, x );
  j0 = ( ( i0 >> 23 ) & 0xff ) - 0x7f;
  if ( j0 < 23 ) {
    if ( j0 < 0 ) { /* raise inexact if x != 0 */
      if ( huge + x > ( float )0.0 ) { /* return 0*sign(x) if |x|<1 */
        if ( i0 < 0 )
          i0 = 0x80000000;
        else
          if ( i0 != 0 )
            i0 = 0x3f800000;
      }
    } else {
      i = ( 0x007fffff ) >> j0;
      if ( ( i0 & i ) == 0 ) return x; /* x is integral */
      if ( huge + x > ( float )0.0 ) { /* raise inexact flag */
        if ( i0 > 0 ) i0 += ( 0x00800000 ) >> j0;
        i0 &= ( ~i );
      }
    }
  } else {
    if ( j0 == 0x80 ) return x + x; /* inf or NaN */
    else return x;    /* x is integral */
  }
  SET_FLOAT_WORD( x, i0 );
  return x;
}
