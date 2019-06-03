/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: quicksortlibm.c

  Author: Ian Lance Taylor

  Function: IEEE754 software library routines.

  Source: Sun Microsystems and Cygnus

  Original name: Unknown

  Changes: No major functional changes.

  License: See the terms below.

*/


#include "quicksortlibm.h"
#include "math_private.h"

// Often used variables/consts
#ifdef __STDC__
const float
#else
float
#endif
quicksort_one    =  1.0f,
quicksort_half   =  5.0000000000e-01f, /* 0x3f000000 */
quicksort_zero   =  0.0f,
quicksort_huge   =  1.0e30,
quicksort_tiny   =  1.0e-30f,
quicksort_two    =  2.0,
quicksort_two24  =  16777216.0; /* 0x4b800000 */

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

#ifdef __STDC__
const float
#else
float
#endif
quicksort_bp[  ] = { 1.0f, 1.5f, },
                 quicksort_dp_h[  ] = { 0.0f, 5.84960938e-01f,}, /* 0x3f15c000 */
                                    quicksort_dp_l[  ] = { 0.0f, 1.56322085e-06f,}, /* 0x35d1cfdc */
                                        quicksort_L1 = 6.0000002384e-01f, /* 0x3f19999a */
                                        quicksort_L2 = 4.2857143283e-01f, /* 0x3edb6db7 */
                                        quicksort_L3 = 3.3333334327e-01f, /* 0x3eaaaaab */
                                        quicksort_L4 = 2.7272811532e-01f, /* 0x3e8ba305 */
                                        quicksort_L5 = 2.3066075146e-01f, /* 0x3e6c3255 */
                                        quicksort_L6 = 2.0697501302e-01f, /* 0x3e53f142 */
                                        quicksort_P1 = 1.6666667163e-01f, /* 0x3e2aaaab */
                                        quicksort_P2 = -2.7777778450e-03f, /* 0xbb360b61 */
                                        quicksort_P3 = 6.6137559770e-05f, /* 0x388ab355 */
                                        quicksort_P4 = -1.6533901999e-06f, /* 0xb5ddea0e */
                                        quicksort_P5 = 4.1381369442e-08f, /* 0x3331bb4c */
                                        quicksort_lg2 = 6.9314718246e-01f, /* 0x3f317218 */
                                        quicksort_lg2_h = 6.93145752e-01f, /* 0x3f317200 */
                                        quicksort_lg2_l = 1.42860654e-06f, /* 0x35bfbe8c */
                                        quicksort_ovt = 4.2995665694e-08f, /* -(128-log2(ovfl+.5ulp)) */
                                        quicksort_cp = 9.6179670095e-01f, /* 0x3f76384f =2/(3ln2) */
                                        quicksort_cp_h = 9.6179199219e-01f, /* 0x3f763800 =head of cp */
                                        quicksort_cp_l = 4.7017383622e-06f, /* 0x369dc3a0 =tail of cp_h */
                                        quicksort_ivln2 = 1.4426950216e+00f, /* 0x3fb8aa3b =1/ln2 */
                                        quicksort_ivln2_h = 1.4426879883e+00f, /* 0x3fb8aa00 =16b 1/ln2*/
                                        quicksort_ivln2_l = 7.0526075433e-06f; /* 0x36eca570 =1/ln2 tail*/


#ifdef __STDC__
float quicksort___ieee754_powf( float x, float y )
#else
float quicksort___ieee754_powf( x, y )
float x, y;
#endif
{
  float z, ax, z_h, z_l, p_h, p_l;
  float y1, t1, t2, r, s, t, u, v, w;
  int i, j, k, yisint, n;
  int hx, hy, ix, iy, is;


  QUICKSORT_GET_FLOAT_WORD( hx, x );
  QUICKSORT_GET_FLOAT_WORD( hy, y );
  ix = hx & 0x7fffffff;
  iy = hy & 0x7fffffff;

  /* y==zero: x**0 = 1 */
  if ( iy == 0 )
    return ( quicksort_one );

  /* x==+-1 */
  if ( x == 1.0f )
    return ( quicksort_one );
  if ( ( x == -1.0f ) && quicksort_isinf( y ) )
    return ( quicksort_one );

  /* +-NaN return x+y */
  if ( ( ix > 0x7f800000 ) || ( iy > 0x7f800000 ) )
    return ( x + y );

  /* determine if y is an odd int when x < 0
     yisint = 0 ... y is not an integer
     yisint = 1 ... y is an odd int
     yisint = 2 ... y is an even int
  */
  yisint = 0;
  if ( hx < 0 ) {
    if ( iy >= 0x4b800000 )
      yisint = 2; /* even integer y */
    else
      if ( iy >= 0x3f800000 ) {
        k = ( iy >> 23 ) - 0x7f; /* exponent */
        j = iy >> ( 23 - k );

        if ( ( j << ( 23 - k ) ) == iy )
          yisint = 2 - ( j & 1 );
      }
  }

  /* special value of y */
  if ( iy == 0x7f800000 ) { /* y is +-inf */
    if ( ix == 0x3f800000 )
      return ( y - y ); /* inf**+-1 is NaN */
    else
      if ( ix > 0x3f800000 ) /* (|x|>1)**+-inf = inf,0 */
        return ( ( hy >= 0 ) ? y : quicksort_zero );
      else      /* (|x|<1)**-,+inf = inf,0 */
        return ( ( hy < 0 ) ? -y : quicksort_zero );
  }
  if ( iy == 0x3f800000 ) { /* y is  +-1 */
    if ( hy < 0 )
      return ( quicksort_one / x );
    else
      return ( x );
  }
  if ( hy == 0x40000000 )
    return ( x * x ); /* y is  2 */
  if ( hy == 0x3f000000 ) { /* y is  0.5 */
    if ( hx >= 0 ) /* x >= +0 */
      return ( quicksort___ieee754_sqrtf( x ) );
  }

  ax = quicksort_fabsf( x );
  /* special value of x */
  if ( ( ix == 0x7f800000 ) || ( ix == 0 ) || ( ix == 0x3f800000 ) ) {
    z = ax;     /*x is +-0,+-inf,+-1*/
    if ( hy < 0 )
      z = quicksort_one / z; /* z = (1/|x|) */
    if ( hx < 0 ) {
      if ( ( ( ix - 0x3f800000 ) | yisint ) == 0 )
        z = ( z - z ) / ( z - z ); /* (-1)**non-int is NaN */
      else

        if ( yisint == 1 )
          z = -z;   /* (x<0)**odd = -(|x|**odd) */
    }

    return ( z );
  }

  /* (x<0)**(non-int) is NaN */
  if ( ( ( ( ( unsigned int ) hx >> 31 ) - 1 ) | yisint ) == 0 )
    return ( ( x - x ) / ( x - x ) );

  /* |y| is huge */
  if ( iy > 0x4d000000 ) { /* if |y| > 2**27 */
    /* over/underflow if x is not close to one */
    if ( ix < 0x3f7ffff8 )
      return ( ( hy < 0 ) ?
               quicksort_huge *quicksort_huge : quicksort_tiny * quicksort_tiny );
    if ( ix > 0x3f800007 )
      return ( ( hy > 0 ) ?
               quicksort_huge *quicksort_huge : quicksort_tiny * quicksort_tiny );
    /* now |1-x| is tiny <= 2**-20, suffice to compute
       log(x) by x-x^2/2+x^3/3-x^4/4 */
    t = x - 1;  /* t has 20 trailing zeros */
    w =
      ( t * t ) *
      ( ( float ) 0.5f -
        t * ( ( float ) 0.333333333333f - t * ( float ) 0.25f ) );
    u = quicksort_ivln2_h * t; /* ivln2_h has 16 sig. bits */
    v = t * quicksort_ivln2_l - w * quicksort_ivln2;
    t1 = u + v;
    QUICKSORT_GET_FLOAT_WORD( is, t1 );
    QUICKSORT_SET_FLOAT_WORD( t1, is & 0xfffff000 );
    t2 = v - ( t1 - u );
  } else {
    float s2, s_h, s_l, t_h, t_l;

    n = 0;
    /* take care subnormal number */
    if ( ix < 0x00800000 ) {
      ax *= quicksort_two24;
      n -= 24;
      QUICKSORT_GET_FLOAT_WORD( ix, ax );
    }
    n += ( ( ix ) >> 23 ) - 0x7f;
    j = ix & 0x007fffff;
    /* determine interval */
    ix = j | 0x3f800000;  /* normalize ix */
    if ( j <= 0x1cc471 )
      k = 0; /* |x|<sqrt(3/2) */
    else

      if ( j < 0x5db3d7 )
        k = 1; /* |x|<sqrt(3)   */
      else {
        k = 0;
        n += 1;
        ix -= 0x00800000;
      }
    QUICKSORT_SET_FLOAT_WORD( ax, ix );

    /* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
    u = ax - quicksort_bp[ k ]; /* bp[ 0 ]=1.0, bp[ 1 ]=1.5 */
    v = quicksort_one / ( ax + quicksort_bp[ k ] );
    s = u * v;
    s_h = s;
    QUICKSORT_GET_FLOAT_WORD( is, s_h );
    QUICKSORT_SET_FLOAT_WORD( s_h, is & 0xfffff000 );
    /* t_h=ax+bp[ k ] High */
    QUICKSORT_SET_FLOAT_WORD(
      t_h, ( ( ix >> 1 ) | 0x20000000 ) + 0x0040000 + ( k << 21 ) );
    t_l = ax - ( t_h - quicksort_bp[ k ] );
    s_l = v * ( ( u - s_h * t_h ) - s_h * t_l );
    /* compute log(ax) */
    s2 = s * s;
    r =
      s2 * s2 *
      ( quicksort_L1 + s2 * ( quicksort_L2 + s2 * ( quicksort_L3 + s2 *
                              ( quicksort_L4 + s2 * ( quicksort_L5 + s2 * quicksort_L6 ) ) ) ) );
    r += s_l * ( s_h + s );
    s2 = s_h * s_h;
    t_h = ( float ) 3.0f + s2 + r;
    QUICKSORT_GET_FLOAT_WORD( is, t_h );
    QUICKSORT_SET_FLOAT_WORD( t_h, is & 0xfffff000 );
    t_l = r - ( ( t_h - ( float ) 3.0f ) - s2 );
    /* u+v = s*(1+...) */
    u = s_h * t_h;
    v = s_l * t_h + t_l * s;
    /* 2/(3log2)*(s+...) */
    p_h = u + v;
    QUICKSORT_GET_FLOAT_WORD( is, p_h );
    QUICKSORT_SET_FLOAT_WORD( p_h, is & 0xfffff000 );
    p_l = v - ( p_h - u );
    z_h = quicksort_cp_h * p_h; /* cp_h+cp_l = 2/(3*log2) */
    z_l = quicksort_cp_l * p_h + p_l * quicksort_cp + quicksort_dp_l[ k ];
    /* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
    t = ( float ) n;
    t1 = ( ( ( z_h + z_l ) + quicksort_dp_h[ k ] ) + t );
    QUICKSORT_GET_FLOAT_WORD( is, t1 );
    QUICKSORT_SET_FLOAT_WORD( t1, is & 0xfffff000 );
    t2 = z_l - ( ( ( t1 - t ) - quicksort_dp_h[ k ] ) - z_h );
  }

  s = quicksort_one; /* s (sign of result -ve**odd) = -1 else = 1 */
  if ( ( ( ( ( unsigned int ) hx >> 31 ) - 1 ) | ( yisint - 1 ) ) == 0 )
    s = -quicksort_one; /* (-ve)**(odd int) */

  /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
  QUICKSORT_GET_FLOAT_WORD( is, y );
  QUICKSORT_SET_FLOAT_WORD( y1, is & 0xfffff000 );
  p_l = ( y - y1 ) * t1 + y * t2;
  p_h = y1 * t1;
  z = p_l + p_h;
  QUICKSORT_GET_FLOAT_WORD( j, z );
  if ( j > 0x43000000 )   /* if z > 128 */
    return ( s * quicksort_huge * quicksort_huge );  /* overflow */
  else

    if ( j == 0x43000000 ) { /* if z == 128 */
      if ( p_l + quicksort_ovt > z - p_h )
        return ( s * quicksort_huge * quicksort_huge ); /* overflow */
    } else

      if ( ( j & 0x7fffffff ) > 0x43160000 ) /* z <= -150 */
        return ( s * quicksort_tiny * quicksort_tiny );  /* underflow */
      else

        if ( ( unsigned int ) j == 0xc3160000 ) { /* z == -150 */
          if ( p_l <= z - p_h )
            return ( s * quicksort_tiny * quicksort_tiny ); /* underflow */
        }

  /*
     compute 2**(p_h+p_l)
  */
  i = j & 0x7fffffff;
  k = ( i >> 23 ) - 0x7f;
  n = 0;
  if ( i > 0x3f000000 ) { /* if |z| > 0.5, set n = [ z+0.5 ] */
    n = j + ( 0x00800000 >> ( k + 1 ) );
    k = ( ( n & 0x7fffffff ) >> 23 ) - 0x7f; /* new k for n */
    QUICKSORT_SET_FLOAT_WORD( t, n & ~( 0x007fffff >> k ) );
    n = ( ( n & 0x007fffff ) | 0x00800000 ) >> ( 23 - k );
    if ( j < 0 )
      n = -n;
    p_h -= t;
  }
  t = p_l + p_h;
  QUICKSORT_GET_FLOAT_WORD( is, t );
  QUICKSORT_SET_FLOAT_WORD( t, is & 0xfffff000 );
  u = t * quicksort_lg2_h;
  v = ( p_l - ( t - p_h ) ) * quicksort_lg2 + t * quicksort_lg2_l;
  z = u + v;
  w = v - ( z - u );
  t = z * z;
  t1 =
    z - t * ( quicksort_P1 + t * ( quicksort_P2 + t * ( quicksort_P3 + t *
                                   ( quicksort_P4 + t * quicksort_P5 ) ) ) );
  r  = ( z * t1 ) / ( t1 - quicksort_two ) - ( w + z * w );
  z  = quicksort_one - ( r - z );
  QUICKSORT_GET_FLOAT_WORD( j, z );
  j += ( n << 23 );
  if ( ( j >> 23 ) <= 0 )
    z = quicksort___scalbnf( z, n ); /* subnormal output */
  else
    QUICKSORT_SET_FLOAT_WORD( z, j );

  return ( s * z );
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

#ifdef __STDC__
float quicksort___ieee754_sqrtf( float x )
#else
float quicksort___ieee754_sqrtf( x )
float x;
#endif
{
  float z;
  int sign = ( int ) 0x80000000;
  int ix, s, q, m, t, i;
  unsigned int r;


  QUICKSORT_GET_FLOAT_WORD( ix, x );

  /* take care of Inf and NaN */
  if ( ( ix & 0x7f800000 ) == 0x7f800000 )
    return ( x * x + x );   /* sqrt(NaN)=NaN, sqrt(+inf)=+inf
                              sqrt(-inf)=sNaN */

  /* take care of zero */
  if ( ix <= 0 ) {
    if ( ( ix & ( ~sign ) ) == 0 )
      return ( x ); /* sqrt(+-0) = +-0 */
    else

      if ( ix < 0 )
        return ( ( x - x ) / ( x - x ) ); /* sqrt(-ve) = sNaN */
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
  m >>= 1;  /* m = [ m/2 ] */

  /* generate sqrt(x) bit by bit */
  ix += ix;
  q = s = 0;    /* q = sqrt(x) */
  r = 0x01000000;   /* r = moving bit from right to left */

  _Pragma( "loopbound min 25 max 25" )
  while ( r != 0 ) {
    t = s + r;
    if ( t <= ix ) {
      s = t + r;
      ix -= t;
      q += r;
    }
    ix += ix;
    r >>= 1;
  }

  /* use floating add to find out rounding direction */
  if ( ix != 0 ) {
    z = quicksort_one - quicksort_tiny; /* trigger inexact flag */
    if ( z >= quicksort_one ) {
      z = quicksort_one + quicksort_tiny;
      if ( z > quicksort_one )
        q += 2;
      else
        q += ( q & 1 );
    }
  }
  ix = ( q >> 1 ) + 0x3f000000;
  ix += ( m << 23 );
  QUICKSORT_SET_FLOAT_WORD( z, ix );

  return ( z );
}


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
float quicksort___copysignf( float x, float y )
#else
float quicksort___copysignf( x, y )
float x, y;
#endif
{
  unsigned int ix, iy;


  QUICKSORT_GET_FLOAT_WORD( ix, x );
  QUICKSORT_GET_FLOAT_WORD( iy, y );
  QUICKSORT_SET_FLOAT_WORD( x, ( ix & 0x7fffffff ) | ( iy & 0x80000000 ) );

  return ( x );
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

/* __ieee754_rem_pio2f(x,y)

   return the remainder of x rem pi/2 in y[ 0 ]+y[ 1 ]
   use __kernel_rem_pio2f()
*/


/* This array is like the one in e_rem_pio2.c, but the numbers are
   single precision and the last 8 bits are forced to 0.  */
#ifdef __STDC__
const int quicksort_npio2_hw[  ] = {
#else
int quicksort_npio2_hw[  ] = {
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
quicksort_invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
quicksort_pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
quicksort_pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
quicksort_pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
quicksort_pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
quicksort_pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
quicksort_pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */


#ifdef __STDC__
int quicksort___ieee754_rem_pio2f( float x, float *y )
#else
int quicksort___ieee754_rem_pio2f( x, y )
float x, y[  ];
#endif
{
  float z, w, t, r, fn;
  int i, j, n, ix, hx;


  QUICKSORT_GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) { /* |x| ~<= pi/4 , no need for reduction */
    y[  0  ] = x;
    y[  1  ] = 0;

    return ( 0 );
  }

  if ( ix < 0x4016cbe4 ) { /* |x| < 3pi/4, special case with n=+-1 */
    if ( hx > 0 ) {
      z = x - quicksort_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[  0  ] = z - quicksort_pio2_1t;
        y[  1  ] = ( z - y[  0  ] ) - quicksort_pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z -= quicksort_pio2_2;
        y[  0  ] = z - quicksort_pio2_2t;
        y[  1  ] = ( z - y[  0  ] ) - quicksort_pio2_2t;
      }
      return ( 1 );
    } else {  /* negative x */
      z = x + quicksort_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[  0  ] = z + quicksort_pio2_1t;
        y[  1  ] = ( z - y[  0  ] ) + quicksort_pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z += quicksort_pio2_2;
        y[  0  ] = z + quicksort_pio2_2t;
        y[  1  ] = ( z - y[  0  ] ) + quicksort_pio2_2t;
      }
      return ( -1 );
    }
  }

  if ( ix <= 0x43490f80 ) { /* |x| ~<= 2^7*(pi/2), medium size */
    t = quicksort_fabsf( x );
    n = ( int ) ( t * quicksort_invpio2 + quicksort_half );
    fn = ( float ) n;
    r = t - fn * quicksort_pio2_1;
    w = fn * quicksort_pio2_1t; /* 1st round good to 40 bit */

    if ( ( n < 32 ) && ( ( int ) ( ix & 0xffffff00 ) !=
                         quicksort_npio2_hw[  n - 1  ] ) )
      y[  0  ] = r - w; /* quick check no cancellation */
    else {
      unsigned int high;

      j  = ix >> 23;
      y[  0  ] = r - w;
      QUICKSORT_GET_FLOAT_WORD( high, y[  0  ] );
      i = j - ( ( high >> 23 ) & 0xff );
      if ( i > 8 ) { /* 2nd iteration needed, good to 57 */
        t = r;
        w = fn * quicksort_pio2_2;
        r = t - w;
        w = fn * quicksort_pio2_2t - ( ( t - r ) - w );
        y[  0  ] = r - w;
        QUICKSORT_GET_FLOAT_WORD( high, y[  0  ] );
        i = j - ( ( high >> 23 ) & 0xff );
        if ( i > 25 )  { /* 3rd iteration need, 74 bits acc */
          t  = r; /* will cover all possible cases */
          w = fn * quicksort_pio2_3;
          r = t - w;
          w = fn * quicksort_pio2_3t - ( ( t - r ) - w );
          y[  0  ] = r - w;
        }
      }
    }

    y[  1  ] = ( r - y[  0  ] ) - w;
    if ( hx < 0 )  {
      y[  0  ] = -y[  0  ];
      y[  1  ] = -y[  1  ];
      return ( -n );
    } else
      return ( n );
  }

  /*
     all other (large) arguments
  */
  if ( ix >= 0x7f800000 ) { /* x is inf or NaN */
    y[  0  ] = y[  1  ] = x - x;
    return ( 0 );
  }

  y[  0  ] = y[  1  ] = x - x; /* dummy initialization */
  return ( 0 ); /* doesn't happen for our input */
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

#ifdef __STDC__
const float
#else
float
#endif
quicksort_C1 =  4.1666667908e-02f, /* 0x3d2aaaab */
quicksort_C2 = -1.3888889225e-03f, /* 0xbab60b61 */
quicksort_C3 =  2.4801587642e-05f, /* 0x37d00d01 */
quicksort_C4 = -2.7557314297e-07f, /* 0xb493f27c */
quicksort_C5 =  2.0875723372e-09f, /* 0x310f74f6 */
quicksort_C6 = -1.1359647598e-11f; /* 0xad47d74e */

#ifdef __STDC__
float quicksort___kernel_cosf( float x, float y )
#else
float quicksort___kernel_cosf( x, y )
float x, y;
#endif
{
  float a, hz, z, r, qx;
  int ix;


  QUICKSORT_GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* ix = |x|'s high word*/
  if ( ix < 0x32000000 ) { /* if x < 2**27 */
    if ( ( ( int ) x ) == 0 )
      return ( quicksort_one ); /* generate inexact */
  }

  z = x * x;
  r = z * ( quicksort_C1 + z * ( quicksort_C2 + z * ( quicksort_C3 + z *
                                 ( quicksort_C4 + z * ( quicksort_C5 + z * quicksort_C6 ) ) ) ) );
  if ( ix < 0x3e99999a )    /* if |x| < 0.3 */
    return ( quicksort_one - ( ( float ) 0.5f * z - ( z * r - x * y ) ) );
  else {
    if ( ix > 0x3f480000 )  /* x > 0.78125 */
      qx = ( float ) 0.28125f;
    else {
      QUICKSORT_SET_FLOAT_WORD( qx, ix - 0x01000000 ); /* x/4 */
    }
    hz = ( float ) 0.5f * z - qx;
    a = quicksort_one - qx;
    return ( a - ( hz - ( z * r - x * y ) ) );
  }
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

#ifdef __STDC__
const float
#else
float
#endif
quicksort_S1  = -1.6666667163e-01f, /* 0xbe2aaaab */
quicksort_S2  =  8.3333337680e-03f, /* 0x3c088889 */
quicksort_S3  = -1.9841270114e-04f, /* 0xb9500d01 */
quicksort_S4  =  2.7557314297e-06f, /* 0x3638ef1b */
quicksort_S5  = -2.5050759689e-08f, /* 0xb2d72f34 */
quicksort_S6  =  1.5896910177e-10f; /* 0x2f2ec9d3 */


#ifdef __STDC__
float quicksort___kernel_sinf( float x, float y, int iy )
#else
float quicksort___kernel_sinf( x, y, iy )
float x, y;
int iy;    /* iy=0 if y is zero */
#endif
{
  float z, r, v;
  int ix;


  QUICKSORT_GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* high word of x */
  if ( ix < 0x32000000 ) { /* |x| < 2**-27 */
    if ( ( int ) x == 0 )
      return ( x ); /* generate inexact */
  }

  z = x * x;
  v = z * x;
  r = quicksort_S2 + z * ( quicksort_S3 + z * ( quicksort_S4 + z *
                           ( quicksort_S5 + z * quicksort_S6 ) ) );

  if ( iy == 0 )
    return ( x + v * ( quicksort_S1 + z * r ) );
  else
    return (
             x - ( ( z * ( quicksort_half * y - v * r ) - y ) - v * quicksort_S1 ) );
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

#ifdef __STDC__
const float quicksort_atanhi[  ] = {
#else
float quicksort_atanhi[  ] = {
#endif
  4.6364760399e-01f, /* atan(0.5)hi 0x3eed6338 */
  7.8539812565e-01f, /* atan(1.0)hi 0x3f490fda */
  9.8279368877e-01f, /* atan(1.5)hi 0x3f7b985e */
  1.5707962513e+00f, /* atan(inf)hi 0x3fc90fda */
};


#ifdef __STDC__
const float quicksort_atanlo[  ] = {
#else
float quicksort_atanlo[  ] = {
#endif
  5.0121582440e-09f, /* atan(0.5)lo 0x31ac3769 */
  3.7748947079e-08f, /* atan(1.0)lo 0x33222168 */
  3.4473217170e-08f, /* atan(1.5)lo 0x33140fb4 */
  7.5497894159e-08f, /* atan(inf)lo 0x33a22168 */
};


#ifdef __STDC__
const float quicksort_aT[  ] = {
#else
float quicksort_aT[  ] = {
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


#ifdef __STDC__
float quicksort___atanf( float x )
#else
float quicksort___atanf( x )
float x;
#endif
{
  float w, s1, s2, z;
  int ix, hx, id;


  QUICKSORT_GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix >= 0x50800000 ) { /* if |x| >= 2^34 */
    if ( ix > 0x7f800000 )
      return ( x + x ); /* NaN */
    if ( hx > 0 )
      return ( quicksort_atanhi[  3  ] + quicksort_atanlo[  3  ] );
    else
      return ( -quicksort_atanhi[  3  ] - quicksort_atanlo[  3  ] );
  }
  if ( ix < 0x3ee00000 ) { /* |x| < 0.4375 */
    if ( ix < 0x31000000 ) { /* |x| < 2^-29 */
      if ( quicksort_huge + x > quicksort_one )
        return ( x ); /* raise inexact */
    }
    id = -1;
  } else {
    x = quicksort_fabsf( x );
    if ( ix < 0x3f980000 ) {  /* |x| < 1.1875 */
      if ( ix < 0x3f300000 ) { /* 7/16 <=|x|<11/16 */
        id = 0;
        x = ( ( float ) 2.0f * x - quicksort_one ) / ( ( float ) 2.0f + x );
      } else {      /* 11/16<=|x|< 19/16 */
        id = 1;
        x = ( x - quicksort_one ) / ( x + quicksort_one );
      }
    } else {
      if ( ix < 0x401c0000 ) { /* |x| < 2.4375 */
        id = 2;
        x = ( x - ( float ) 1.5f ) / ( quicksort_one + ( float ) 1.5f * x );
      } else {      /* 2.4375 <= |x| < 2^66 */
        id = 3;
        x  = -( float ) 1.0f / x;
      }
    }
  }
  /* end of argument reduction */
  z = x * x;
  w = z * z;
  /* break sum from i=0 to 10 aT[ i ]z**(i+1) into odd and even poly */
  s1 =
    z * ( quicksort_aT[  0  ] +
          w * ( quicksort_aT[  2  ] +
                w * ( quicksort_aT[  4  ] +
                      w * ( quicksort_aT[  6  ] +
                            w * ( quicksort_aT[  8  ] +
                                  w * quicksort_aT[  10  ] ) ) ) ) );
  s2 =
    w * ( quicksort_aT[  1  ] +
          w * ( quicksort_aT[  3  ] +
                w * ( quicksort_aT[  5  ] +
                      w * ( quicksort_aT[  7  ] +
                            w * quicksort_aT[  9  ] ) ) ) );

  if ( id < 0 )
    return ( x - x * ( s1 + s2 ) );
  else {
    z = quicksort_atanhi[  id  ] -
        ( ( x * ( s1 + s2 ) - quicksort_atanlo[  id  ] ) - x );
    return ( ( hx < 0 ) ? -z : z );
  }
}


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

#ifdef __STDC__
float quicksort___cosf( float x )
#else
float quicksort___cosf( x )
float x;
#endif
{
  float y[  2  ], z = 0.0f;
  int n, ix;


  QUICKSORT_GET_FLOAT_WORD( ix, x );

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 )
    return ( quicksort___kernel_cosf( x, z ) );

  /* cos(Inf or NaN) is NaN */
  else
    if ( ix >= 0x7f800000 )
      return ( x - x );

    /* argument reduction needed */
    else {
      n = quicksort___ieee754_rem_pio2f( x, y );

      switch ( n & 3 ) {
        case 0:
          return ( quicksort___kernel_cosf( y[  0  ], y[  1  ] ) );
        case 1:
          return ( -quicksort___kernel_sinf( y[  0  ], y[  1  ], 1 ) );
        case 2:
          return ( -quicksort___kernel_cosf( y[  0  ], y[  1  ] ) );
        default:
          return ( quicksort___kernel_sinf( y[  0  ], y[  1  ], 1 ) );
      }
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
float quicksort___sinf( float x )
#else
float quicksort___sinf( x )
float x;
#endif
{
  float y[  2  ], z = 0.0;
  int n, ix;


  QUICKSORT_GET_FLOAT_WORD( ix, x );

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 )
    return ( quicksort___kernel_sinf( x, z, 0 ) );

  /* sin(Inf or NaN) is NaN */
  else
    if ( ix >= 0x7f800000 )
      return ( x - x );

    /* argument reduction needed */
    else {
      n = quicksort___ieee754_rem_pio2f( x, y );

      switch ( n & 3 ) {
        case 0:
          return ( quicksort___kernel_sinf( y[  0  ], y[  1  ], 1 ) );
        case 1:
          return ( quicksort___kernel_cosf( y[  0  ], y[  1  ] ) );
        case 2:
          return ( -quicksort___kernel_sinf( y[  0  ], y[  1  ], 1 ) );
        default:
          return ( -quicksort___kernel_cosf( y[  0  ], y[  1  ] ) );
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
float quicksort___fabsf( float x )
#else
float quicksort___fabsf( x )
float x;
#endif
{
  unsigned int ix;


  QUICKSORT_GET_FLOAT_WORD( ix, x );
  QUICKSORT_SET_FLOAT_WORD( x, ix & 0x7fffffff );
  return ( x );
}


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
quicksort_two25 =  3.355443200e+07f,  /* 0x4c000000 */
quicksort_twom25 =  2.9802322388e-08f; /* 0x33000000 */

#ifdef __STDC__
float quicksort___scalbnf( float x, int n )
#else
float quicksort___scalbnf( x, n )
float x;
int n;
#endif
{
  int k, ix;


  QUICKSORT_GET_FLOAT_WORD( ix, x );
  k = ( ix & 0x7f800000 ) >> 23; /* extract exponent */
  if ( k == 0 ) {   /* 0 or subnormal x */
    if ( ( ix & 0x7fffffff ) == 0 )
      return ( x ); /* +-0 */
    x *= quicksort_two25;
    QUICKSORT_GET_FLOAT_WORD( ix, x );
    k = ( ( ix & 0x7f800000 ) >> 23 ) - 25;
  }

  if ( k == 0xff )
    return ( x + x ); /* NaN or Inf */
  k = k + n;
  if ( ( n > 50000 ) || ( k > 0xfe ) )
    /* overflow  */
    return ( quicksort_huge * quicksort___copysignf( quicksort_huge, x ) );
  if ( n < -50000 )
    /* underflow */
    return ( quicksort_tiny * quicksort___copysignf( quicksort_tiny, x ) );
  if ( k > 0 ) {    /* normal result */
    QUICKSORT_SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
    return ( x );
  }

  if ( k <= -25 )
    /* underflow */
    return ( quicksort_tiny * quicksort___copysignf( quicksort_tiny, x ) );
  k += 25;        /* subnormal result */
  QUICKSORT_SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
  return ( x * quicksort_twom25 );
}


/*
   Written by J.T. Conklin <jtc@netbsd.org>.
   Public domain.
*/

/*
   isinff(x) returns 1 if x is inf, -1 if x is -inf, else 0;
   no branching!
*/
int quicksort___isinff( float x )
{
  int ix, t;


  QUICKSORT_GET_FLOAT_WORD( ix, x );
  t = ix & 0x7fffffff;
  t ^= 0x7f800000;
  t |= -t;
  return ( ~( t >> 31 ) & ( ix >> 30 ) );
}
