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

  Name: wcclibm.c

  Author: Unknown

  Function: IEEE754 software library routines.

  Source: Sun Microsystems

  Original name: wcclibm.c

  Changes: No major functional changes.

  License: See the terms above.

*/

#include "wcclibm.h"
#include "math_private.h"


/* e_acosf.c -- float version of e_acos.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

static const float basicmath_pi =  3.1415925026e+00f,      /* 0x40490fda */
                   basicmath_pio2_hi =  1.5707962513e+00f, /* 0x3fc90fda */
                   basicmath_pio2_lo =  7.5497894159e-08f, /* 0x33a22168 */
                   basicmath_pS0 =  1.6666667163e-01f,     /* 0x3e2aaaab */
                   basicmath_pS1 = -3.2556581497e-01f,     /* 0xbea6b090 */
                   basicmath_pS2 =  2.0121252537e-01f,     /* 0x3e4e0aa8 */
                   basicmath_pS3 = -4.0055535734e-02f,     /* 0xbd241146 */
                   basicmath_pS4 =  7.9153501429e-04f,     /* 0x3a4f7f04 */
                   basicmath_pS5 =  3.4793309169e-05f,     /* 0x3811ef08 */
                   basicmath_qS1 = -2.4033949375e+00f,     /* 0xc019d139 */
                   basicmath_qS2 =  2.0209457874e+00f,     /* 0x4001572d */
                   basicmath_qS3 = -6.8828397989e-01f,     /* 0xbf303361 */
                   basicmath_qS4 =  7.7038154006e-02f;     /* 0x3d9dc62e */

float basicmath___ieee754_acosf( float x )
{
  float z, p, q, r, w, s, c, df;
  int32_t hx, ix;
  GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix == 0x3f800000 ) { /* |x|==1 */
    if ( hx > 0 ) return 0.0f; /* acos(1) = 0  */
    else return basicmath_pi + ( float )2.0f * basicmath_pio2_lo; /* acos(-1)= pi */
  } else
    if ( ix > 0x3f800000 ) { /* |x| >= 1 */
      return ( x - x ) / ( x - x ); /* acos(|x|>1) is NaN */
    }
  if ( ix < 0x3f000000 ) { /* |x| < 0.5 */
    if ( ix <= 0x23000000 ) return basicmath_pio2_hi +
                                     basicmath_pio2_lo; /*if|x|<2**-57*/
    z = x * x;
    p = z * ( basicmath_pS0 + z * ( basicmath_pS1 + z * ( basicmath_pS2 + z *
                                    ( basicmath_pS3 + z *
                                      ( basicmath_pS4 + z * basicmath_pS5 ) ) ) ) );
    q = basicmath_one + z * ( basicmath_qS1 + z * ( basicmath_qS2 + z *
                              ( basicmath_qS3 + z * basicmath_qS4 ) ) );
    r = p / q;
    return basicmath_pio2_hi - ( x - ( basicmath_pio2_lo - x * r ) );
  } else
    if ( hx < 0 ) { /* x < -0.5 */
      z = ( basicmath_one + x ) * ( float )0.5f;
      p = z * ( basicmath_pS0 + z * ( basicmath_pS1 + z * ( basicmath_pS2 + z *
                                      ( basicmath_pS3 + z *
                                        ( basicmath_pS4 + z * basicmath_pS5 ) ) ) ) );
      q = basicmath_one + z * ( basicmath_qS1 + z * ( basicmath_qS2 + z *
                                ( basicmath_qS3 + z * basicmath_qS4 ) ) );
      s = basicmath___ieee754_sqrtf( z );
      r = p / q;
      w = r * s - basicmath_pio2_lo;
      return basicmath_pi - ( float )2.0f * ( s + w );
    } else {      /* x > 0.5 */
      int32_t idf;
      z = ( basicmath_one - x ) * ( float )0.5f;
      s = basicmath___ieee754_sqrtf( z );
      df = s;
      GET_FLOAT_WORD( idf, df );
      SET_FLOAT_WORD( df, idf & 0xfffff000 );
      c  = ( z - df * df ) / ( s + df );
      p = z * ( basicmath_pS0 + z * ( basicmath_pS1 + z * ( basicmath_pS2 + z *
                                      ( basicmath_pS3 + z *
                                        ( basicmath_pS4 + z * basicmath_pS5 ) ) ) ) );
      q = basicmath_one + z * ( basicmath_qS1 + z * ( basicmath_qS2 + z *
                                ( basicmath_qS3 + z * basicmath_qS4 ) ) );
      r = p / q;
      w = r * s + c;
      return ( float )2.0f * ( df + w );
    }
}


/* e_powf.c -- float version of e_pow.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

static const float basicmath_bp[] = {1.0f, 1.5f,},
                                    basicmath_dp_h[] = { 0.0f, 5.84960938e-01f,}, /* 0x3f15c000 */
                                        basicmath_dp_l[] = { 0.0f, 1.56322085e-06f,}, /* 0x35d1cfdc */
                                            /* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
                                            basicmath_L1      =  6.0000002384e-01f, /* 0x3f19999a */
                                            basicmath_L2      =  4.2857143283e-01f, /* 0x3edb6db7 */
                                            basicmath_L3      =  3.3333334327e-01f, /* 0x3eaaaaab */
                                            basicmath_L4      =  2.7272811532e-01f, /* 0x3e8ba305 */
                                            basicmath_L5      =  2.3066075146e-01f, /* 0x3e6c3255 */
                                            basicmath_L6      =  2.0697501302e-01f, /* 0x3e53f142 */
                                            basicmath_P1      =  1.6666667163e-01f, /* 0x3e2aaaab */
                                            basicmath_P2      = -2.7777778450e-03f, /* 0xbb360b61 */
                                            basicmath_P3      =  6.6137559770e-05f, /* 0x388ab355 */
                                            basicmath_P4      = -1.6533901999e-06f, /* 0xb5ddea0e */
                                            basicmath_P5      =  4.1381369442e-08f, /* 0x3331bb4c */
                                            basicmath_lg2     =  6.9314718246e-01f, /* 0x3f317218 */
                                            basicmath_lg2_h   =  6.93145752e-01f,   /* 0x3f317200 */
                                            basicmath_lg2_l   =  1.42860654e-06f,   /* 0x35bfbe8c */
                                            basicmath_ovt     =  4.2995665694e-08f, /* -(128-log2(ovfl+.5ulp)) */
                                            basicmath_cp      =  9.6179670095e-01f, /* 0x3f76384f =2/(3ln2) */
                                            basicmath_cp_h    =  9.6179199219e-01f, /* 0x3f763800 =head of cp */
                                            basicmath_cp_l    =  4.7017383622e-06f, /* 0x369dc3a0 =tail of cp_h */
                                            basicmath_ivln2   =  1.4426950216e+00f, /* 0x3fb8aa3b =1/ln2 */
                                            basicmath_ivln2_h =  1.4426879883e+00f, /* 0x3fb8aa00 =16b 1/ln2*/
                                            basicmath_ivln2_l =  7.0526075433e-06f; /* 0x36eca570 =1/ln2 tail*/

float basicmath___ieee754_powf( float x, float y )
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
  if ( iy == 0 ) return basicmath_one;

  /* x==+-1 */
  if ( x == 1.0f ) return basicmath_one;
  if ( x == -1.0f && basicmath___isinff( y ) ) return basicmath_one;

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
        return ( hy >= 0 ) ? y : basicmath_zero;
      else      /* (|x|<1)**-,+inf = inf,0 */
        return ( hy < 0 ) ? -y : basicmath_zero;
  }
  if ( iy == 0x3f800000 ) { /* y is  +-1 */
    if ( hy < 0 ) return basicmath_one / x;
    else return x;
  }
  if ( hy == 0x40000000 ) return x * x; /* y is  2 */
  if ( hy == 0x3f000000 ) { /* y is  0.5 */
    if ( hx >= 0 ) /* x >= +0 */
      return basicmath___ieee754_sqrtf( x );
  }

  ax   = basicmath___fabsf( x );
  /* special value of x */
  if ( ix == 0x7f800000 || ix == 0 || ix == 0x3f800000 ) {
    z = ax;     /*x is +-0,+-inf,+-1*/
    if ( hy < 0 ) z = basicmath_one / z; /* z = (1/|x|) */
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
    if ( ix < 0x3f7ffff8 ) return ( hy < 0 ) ? basicmath_huge * basicmath_huge :
                                    basicmath_tiny * basicmath_tiny;
    if ( ix > 0x3f800007 ) return ( hy > 0 ) ? basicmath_huge * basicmath_huge :
                                    basicmath_tiny * basicmath_tiny;
    /* now |1-x| is tiny <= 2**-20, suffice to compute
       log(x) by x-x^2/2+x^3/3-x^4/4 */
    t = x - 1;  /* t has 20 trailing zeros */
    w = ( t * t ) * ( ( float )0.5f - t * ( ( float )0.333333333333f - t *
                                            ( float )0.25f ) );
    u = basicmath_ivln2_h * t; /* ivln2_h has 16 sig. bits */
    v = t * basicmath_ivln2_l - w * basicmath_ivln2;
    t1 = u + v;
    GET_FLOAT_WORD( is, t1 );
    SET_FLOAT_WORD( t1, is & 0xfffff000 );
    t2 = v - ( t1 - u );
  } else {
    float s2, s_h, s_l, t_h, t_l;
    n = 0;
    /* take care subnormal number */
    if ( ix < 0x00800000 ) {
      ax *= basicmath_two24;
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
    u = ax - basicmath_bp[ k ]; /* bp[0]=1.0, bp[1]=1.5 */
    v = basicmath_one / ( ax + basicmath_bp[ k ] );
    s = u * v;
    s_h = s;
    GET_FLOAT_WORD( is, s_h );
    SET_FLOAT_WORD( s_h, is & 0xfffff000 );
    /* t_h=ax+bp[k] High */
    SET_FLOAT_WORD( t_h, ( ( ix >> 1 ) | 0x20000000 ) + 0x0040000 + ( k << 21 ) );
    t_l = ax - ( t_h - basicmath_bp[ k ] );
    s_l = v * ( ( u - s_h * t_h ) - s_h * t_l );
    /* compute log(ax) */
    s2 = s * s;
    r = s2 * s2 * ( basicmath_L1 + s2 * ( basicmath_L2 + s2 *
                                          ( basicmath_L3 + s2 * ( basicmath_L4 + s2 *
                                              ( basicmath_L5 + s2 * basicmath_L6 ) ) ) ) );
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
    z_h = basicmath_cp_h * p_h; /* cp_h+cp_l = 2/(3*log2) */
    z_l = basicmath_cp_l * p_h + p_l * basicmath_cp + basicmath_dp_l[ k ];
    /* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
    t = ( float )n;
    t1 = ( ( ( z_h + z_l ) + basicmath_dp_h[ k ] ) + t );
    GET_FLOAT_WORD( is, t1 );
    SET_FLOAT_WORD( t1, is & 0xfffff000 );
    t2 = z_l - ( ( ( t1 - t ) - basicmath_dp_h[ k ] ) - z_h );
  }

  s = basicmath_one; /* s (sign of result -ve**odd) = -1 else = 1 */
  if ( ( ( ( ( u_int32_t )hx >> 31 ) - 1 ) | ( yisint - 1 ) ) == 0 )
    s = -basicmath_one; /* (-ve)**(odd int) */

  /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
  GET_FLOAT_WORD( is, y );
  SET_FLOAT_WORD( y1, is & 0xfffff000 );
  p_l = ( y - y1 ) * t1 + y * t2;
  p_h = y1 * t1;
  z = p_l + p_h;
  GET_FLOAT_WORD( j, z );
  if ( j > 0x43000000 )   /* if z > 128 */
    return s * basicmath_huge * basicmath_huge;   /* overflow */
  else
    if ( j == 0x43000000 ) { /* if z == 128 */
      if ( p_l + basicmath_ovt > z - p_h ) return s * basicmath_huge *
            basicmath_huge; /* overflow */
    } else
      if ( ( j & 0x7fffffff ) > 0x43160000 ) /* z <= -150 */
        return s * basicmath_tiny * basicmath_tiny;   /* underflow */
      else
        if ( ( u_int32_t ) j == 0xc3160000 ) { /* z == -150 */
          if ( p_l <= z - p_h ) return s * basicmath_tiny *
                                         basicmath_tiny; /* underflow */
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
  u = t * basicmath_lg2_h;
  v = ( p_l - ( t - p_h ) ) * basicmath_lg2 + t * basicmath_lg2_l;
  z = u + v;
  w = v - ( z - u );
  t  = z * z;
  t1  = z - t * ( basicmath_P1 + t * ( basicmath_P2 + t * ( basicmath_P3 + t *
                                       ( basicmath_P4 + t * basicmath_P5 ) ) ) );
  r  = ( z * t1 ) / ( t1 - basicmath_two ) - ( w + z * w );
  z  = basicmath_one - ( r - z );
  GET_FLOAT_WORD( j, z );
  j += ( n << 23 );
  if ( ( j >> 23 ) <= 0 ) z = basicmath___scalbnf( z, n ); /* subnormal output */
  else SET_FLOAT_WORD( z, j );
  return s * z;
}


/* e_rem_pio2f.c -- float version of e_rem_pio2.c
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

/* basicmath___ieee754_rem_pio2f(x,y)

   return the remainder of x rem pi/2 in y[0]+y[1]
*/

/* This array is like the one in e_rem_pio2.c, but the numbers are
   single precision and the last 8 bits are forced to 0.  */
static const int32_t basicmath_npio2_hw[] = {
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

static const float basicmath_invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
                   basicmath_pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
                   basicmath_pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
                   basicmath_pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
                   basicmath_pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
                   basicmath_pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
                   basicmath_pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

int32_t basicmath___ieee754_rem_pio2f( float x, float *y )
{
  float z, w, t, r, fn;
  int32_t i, j, n, ix, hx;

  GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) { /* |x| ~<= pi/4 , no need for reduction */
    y[ 0 ] = x;
    y[ 1 ] = 0;
    return 0;
  }
  if ( ix < 0x4016cbe4 ) { /* |x| < 3pi/4, special case with n=+-1 */
    if ( hx > 0 ) {
      z = x - basicmath_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[ 0 ] = z - basicmath_pio2_1t;
        y[ 1 ] = ( z - y[ 0 ] ) - basicmath_pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z -= basicmath_pio2_2;
        y[ 0 ] = z - basicmath_pio2_2t;
        y[ 1 ] = ( z - y[ 0 ] ) - basicmath_pio2_2t;
      }
      return 1;
    } else {  /* negative x */
      z = x + basicmath_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[ 0 ] = z + basicmath_pio2_1t;
        y[ 1 ] = ( z - y[ 0 ] ) + basicmath_pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z += basicmath_pio2_2;
        y[ 0 ] = z + basicmath_pio2_2t;
        y[ 1 ] = ( z - y[ 0 ] ) + basicmath_pio2_2t;
      }
      return -1;
    }
  }
  if ( ix <= 0x43490f80 ) { /* |x| ~<= 2^7*(pi/2), medium size */
    t  = basicmath___fabsf( x );
    n  = ( int32_t ) ( t * basicmath_invpio2 + basicmath_half );
    fn = ( float )n;
    r  = t - fn * basicmath_pio2_1;
    w  = fn * basicmath_pio2_1t; /* 1st round good to 40 bit */
    if ( n < 32 && ( int32_t )( ix & 0xffffff00 ) != basicmath_npio2_hw[n - 1] ) {
      y[ 0 ] = r - w; /* quick check no cancellation */
    } else {
      u_int32_t high;
      j  = ix >> 23;
      y[ 0 ] = r - w;
      GET_FLOAT_WORD( high, y[ 0 ] );
      i = j - ( ( high >> 23 ) & 0xff );
      if ( i > 8 ) { /* 2nd iteration needed, good to 57 */
        t  = r;
        w  = fn * basicmath_pio2_2;
        r  = t - w;
        w  = fn * basicmath_pio2_2t - ( ( t - r ) - w );
        y[ 0 ] = r - w;
        GET_FLOAT_WORD( high, y[ 0 ] );
        i = j - ( ( high >> 23 ) & 0xff );
        if ( i > 25 )  { /* 3rd iteration need, 74 bits acc */
          t  = r; /* will cover all possible cases */
          w  = fn * basicmath_pio2_3;
          r  = t - w;
          w  = fn * basicmath_pio2_3t - ( ( t - r ) - w );
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

  // This will never happen in basicmath_small, because
  // up to this point we have already returned a value
  // for each of the possible inputs
  y[ 0 ] = y[ 1 ] = x - x; /* dummy initialization */
  return 0;
}


/* e_sqrtf.c -- float version of e_sqrt.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

float basicmath___ieee754_sqrtf( float x )
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

  _Pragma( "loopbound min 25 max 25" )
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
    z = basicmath_one - basicmath_tiny; /* trigger inexact flag */
    if ( z >= basicmath_one ) {
      z = basicmath_one + basicmath_tiny;
      if ( z > basicmath_one )
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

static const float basicmath_C1  =  4.1666667908e-02f, /* 0x3d2aaaab */
                   basicmath_C2  = -1.3888889225e-03f, /* 0xbab60b61 */
                   basicmath_C3  =  2.4801587642e-05f, /* 0x37d00d01 */
                   basicmath_C4  = -2.7557314297e-07f, /* 0xb493f27c */
                   basicmath_C5  =  2.0875723372e-09f, /* 0x310f74f6 */
                   basicmath_C6  = -1.1359647598e-11f; /* 0xad47d74e */

float basicmath___kernel_cosf( float x, float y )
{
  float a, hz, z, r, qx;
  int32_t ix;
  GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* ix = |x|'s high word*/
  if ( ix < 0x32000000 ) { /* if x < 2**27 */
    if ( ( ( int )x ) == 0 ) return basicmath_one; /* generate inexact */
  }
  z  = x * x;
  r  = z * ( basicmath_C1 + z * ( basicmath_C2 + z * ( basicmath_C3 + z *
                                  ( basicmath_C4 + z * ( basicmath_C5 + z * basicmath_C6 ) ) ) ) );
  if ( ix < 0x3e99999a )    /* if |x| < 0.3 */
    return basicmath_one - ( ( float )0.5f * z - ( z * r - x * y ) );
  else {
    if ( ix > 0x3f480000 )  /* x > 0.78125 */
      qx = ( float )0.28125f;
    else {
      SET_FLOAT_WORD( qx, ix - 0x01000000 ); /* x/4 */
    }
    hz = ( float )0.5f * z - qx;
    a  = basicmath_one - qx;
    return a - ( hz - ( z * r - x * y ) );
  }
}


/* k_sinf.c -- float version of k_sin.c
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

static const float basicmath_S1 = -1.6666667163e-01f, /* 0xbe2aaaab */
                   basicmath_S2 =  8.3333337680e-03f, /* 0x3c088889 */
                   basicmath_S3 = -1.9841270114e-04f, /* 0xb9500d01 */
                   basicmath_S4 =  2.7557314297e-06f, /* 0x3638ef1b */
                   basicmath_S5 = -2.5050759689e-08f, /* 0xb2d72f34 */
                   basicmath_S6 =  1.5896910177e-10f; /* 0x2f2ec9d3 */

float basicmath___kernel_sinf( float x, float y, int iy )
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
  r =  basicmath_S2 + z * ( basicmath_S3 + z * ( basicmath_S4 + z *
                            ( basicmath_S5 + z * basicmath_S6 ) ) );
  if ( iy == 0 ) return x + v * ( basicmath_S1 + z * r );
  else      return x - ( ( z * ( basicmath_half * y - v * r ) - y ) - v *
                           basicmath_S1 );
}


/* s_copysignf.c -- float version of s_copysign.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

/*
   copysignf(float x, float y)
   copysignf(x,y) returns a value with the magnitude of x and
   with the sign bit of y.
*/


float basicmath___copysignf( float x, float y )
{
  u_int32_t ix, iy;
  GET_FLOAT_WORD( ix, x );
  GET_FLOAT_WORD( iy, y );
  SET_FLOAT_WORD( x, ( ix & 0x7fffffff ) | ( iy & 0x80000000 ) );
  return x;
}


/* s_cosf.c -- float version of s_cos.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

float basicmath___cosf( float x )
{
  float y[ 2 ], z = 0.0f;
  int32_t n, ix;

  GET_FLOAT_WORD( ix, x );

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) return basicmath___kernel_cosf( x, z );

  /* cos(Inf or NaN) is NaN */
  else
    if ( ix >= 0x7f800000 ) return x - x;

    /* argument reduction needed */
    else {
      n = basicmath___ieee754_rem_pio2f( x, y );
      switch ( n & 3 ) {
        case 0:
          return  basicmath___kernel_cosf( y[ 0 ], y[ 1 ] );
        case 1:
          return -basicmath___kernel_sinf( y[ 0 ], y[ 1 ], 1 );
        case 2:
          return -basicmath___kernel_cosf( y[ 0 ], y[ 1 ] );
        default:
          return  basicmath___kernel_sinf( y[ 0 ], y[ 1 ], 1 );
      }
    }
}


/* s_fabsf.c -- float version of s_fabs.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

/*
   fabsf(x) returns the absolute value of x.
*/

float basicmath___fabsf( float x )
{
  u_int32_t ix;
  GET_FLOAT_WORD( ix, x );
  SET_FLOAT_WORD( x, ix & 0x7fffffff );
  return x;
}


/*
   isinff(x) returns 1 if x is inf, -1 if x is -inf, else 0;
   no branching!
*/

int basicmath___isinff ( float x )
{
  int32_t ix, t;
  GET_FLOAT_WORD( ix, x );
  t = ix & 0x7fffffff;
  t ^= 0x7f800000;
  t |= -t;
  return ~( t >> 31 ) & ( ix >> 30 );
}


/* s_scalbnf.c -- float version of s_scalbn.c.
   Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
*/

static const float basicmath_two25 =  3.355443200e+07f,  /* 0x4c000000 */
                   basicmath_twom25 =  2.9802322388e-08f; /* 0x33000000 */

float basicmath___scalbnf ( float x, int n )
{
  int32_t k, ix;
  GET_FLOAT_WORD( ix, x );
  k = ( ix & 0x7f800000 ) >> 23; /* extract exponent */
  if ( k == 0 ) {   /* 0 or subnormal x */
    if ( ( ix & 0x7fffffff ) == 0 ) return x; /* +-0 */
    x *= basicmath_two25;
    GET_FLOAT_WORD( ix, x );
    k = ( ( ix & 0x7f800000 ) >> 23 ) - 25;
  }
  if ( k == 0xff ) return x + x; /* NaN or Inf */
  k = k + n;
  if ( n > 50000 || k >  0xfe )
    return basicmath_huge * basicmath___copysignf( basicmath_huge,
           x ); /* overflow  */
  if ( n < -50000 )
    return basicmath_tiny * basicmath___copysignf( basicmath_tiny,
           x ); /*underflow*/
  if ( k > 0 ) {    /* normal result */
    SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
    return x;
  }
  if ( k <= -25 )
    return basicmath_tiny * basicmath___copysignf( basicmath_tiny,
           x ); /*underflow*/
  k += 25;        /* subnormal result */
  SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
  return x * basicmath_twom25;
}
