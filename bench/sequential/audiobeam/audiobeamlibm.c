/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: audiobeamlibm.c

  Author: Ian Lance Taylor and J.T. Conklin

  Function: IEEE754 software library routines.

  Source: Sun Microsystems and Cygnus

  Original name: Unknown

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


#include "audiobeamlibm.h"
#include "audiobeamlibmath.h"


static const int audiobeam_npio2_hw[  ] = {
  0x3fc90f00, 0x40490f00, 0x4096cb00, 0x40c90f00, 0x40fb5300, 0x4116cb00,
  0x412fed00, 0x41490f00, 0x41623100, 0x417b5300, 0x418a3a00, 0x4196cb00,
  0x41a35c00, 0x41afed00, 0x41bc7e00, 0x41c90f00, 0x41d5a000, 0x41e23100,
  0x41eec200, 0x41fb5300, 0x4203f200, 0x420a3a00, 0x42108300, 0x4216cb00,
  0x421d1400, 0x42235c00, 0x4229a500, 0x422fed00, 0x42363600, 0x423c7e00,
  0x4242c700, 0x42490f00
};

static const float
audiobeam_invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
audiobeam_pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
audiobeam_pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
audiobeam_pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
audiobeam_pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
audiobeam_pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
audiobeam_pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

static const float
audiobeam_C1  =  4.1666667908e-02f, /* 0x3d2aaaab */
audiobeam_C2  = -1.3888889225e-03f, /* 0xbab60b61 */
audiobeam_C3  =  2.4801587642e-05f, /* 0x37d00d01 */
audiobeam_C4  = -2.7557314297e-07f, /* 0xb493f27c */
audiobeam_C5  =  2.0875723372e-09f, /* 0x310f74f6 */
audiobeam_C6  = -1.1359647598e-11f; /* 0xad47d74e */

static const float
audiobeam_S1  = -1.6666667163e-01f, /* 0xbe2aaaab */
audiobeam_S2  =  8.3333337680e-03f, /* 0x3c088889 */
audiobeam_S3  = -1.9841270114e-04f, /* 0xb9500d01 */
audiobeam_S4  =  2.7557314297e-06f, /* 0x3638ef1b */
audiobeam_S5  = -2.5050759689e-08f, /* 0xb2d72f34 */
audiobeam_S6  =  1.5896910177e-10f; /* 0x2f2ec9d3 */

static const float
audiobeam_two25   =  3.355443200e+07f,  /* 0x4c000000 */
audiobeam_twom25  =  2.9802322388e-08f; /* 0x33000000 */


int audiobeam___ieee754_rem_pio2f( float x, float *y )
{
  float z, w, t, r, fn;
  int i, j, n = 0, ix, hx;

  AUDIOBEAM_GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) {
    y[ 0 ] = x;
    y[ 1 ] = 0;
    return 0;
  }
  if ( ix < 0x4016cbe4 ) {
    if ( hx > 0 ) {
      z = x - audiobeam_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) {
        y[ 0 ] = z - audiobeam_pio2_1t;
        y[ 1 ] = ( z - y[ 0 ] ) - audiobeam_pio2_1t;
      } else {
        z -= audiobeam_pio2_2;
        y[ 0 ] = z - audiobeam_pio2_2t;
        y[ 1 ] = ( z - y[ 0 ] ) - audiobeam_pio2_2t;
      }
      return 1;
    } else {
      z = x + audiobeam_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) {
        y[ 0 ] = z + audiobeam_pio2_1t;
        y[ 1 ] = ( z - y[ 0 ] ) + audiobeam_pio2_1t;
      } else {
        z += audiobeam_pio2_2;
        y[ 0 ] = z + audiobeam_pio2_2t;
        y[ 1 ] = ( z - y[ 0 ] ) + audiobeam_pio2_2t;
      }
      return -1;
    }
  }
  if ( ix <= 0x43490f80 ) {
    t  = audiobeam_fabsf( x );
    n  = ( int ) ( t * audiobeam_invpio2 + audiobeam_half );
    fn = ( float )n;
    r  = t - fn * audiobeam_pio2_1;
    w  = fn * audiobeam_pio2_1t;
    if ( n < 32 && ( int )( ix & 0xffffff00 ) != audiobeam_npio2_hw[ n - 1 ] )
      y[ 0 ] = r - w;
    else {
      unsigned int high;
      j  = ix >> 23;
      y[ 0 ] = r - w;
      AUDIOBEAM_GET_FLOAT_WORD( high, y[ 0 ] );
      i = j - ( ( high >> 23 ) & 0xff );
      if ( i > 8 ) {
        t  = r;
        w  = fn * audiobeam_pio2_2;
        r  = t - w;
        w  = fn * audiobeam_pio2_2t - ( ( t - r ) - w );
        y[ 0 ] = r - w;
        AUDIOBEAM_GET_FLOAT_WORD( high, y[ 0 ] );
        i = j - ( ( high >> 23 ) & 0xff );
        if ( i > 25 )  {
          t  = r;
          w  = fn * audiobeam_pio2_3;
          r  = t - w;
          w  = fn * audiobeam_pio2_3t - ( ( t - r ) - w );
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
  if ( ix >= 0x7f800000 ) {
    y[ 0 ] = y[ 1 ] = x - x;
    return 0;
  }

  return n;
}


float audiobeam___kernel_cosf( float x, float y )
{
  float a, hz, z, r, qx;
  int ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;
  if ( ix < 0x32000000 ) {
    if ( ( ( int )x ) == 0 ) return audiobeam_one;
  }
  z  = x * x;
  r  = z * ( audiobeam_C1 + z * ( audiobeam_C2 + z * ( audiobeam_C3 + z *
                                  ( audiobeam_C4 + z *
                                    ( audiobeam_C5 + z * audiobeam_C6 ) ) ) ) );
  if ( ix < 0x3e99999a )
    return audiobeam_one - ( ( float )0.5f * z - ( z * r - x * y ) );
  else {
    if ( ix > 0x3f480000 )
      qx = ( float )0.28125f;
    else
      AUDIOBEAM_SET_FLOAT_WORD( qx, ix - 0x01000000 );
    hz = ( float )0.5f * z - qx;
    a  = audiobeam_one - qx;
    return a - ( hz - ( z * r - x * y ) );
  }
}


float audiobeam___kernel_sinf( float x, float y, int iy )
{
  float z, r, v;
  int ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;
  if ( ix < 0x32000000 ) {
    if ( ( int )x == 0 ) return x;
  }
  z =  x * x;
  v =  z * x;
  r =  audiobeam_S2 + z * ( audiobeam_S3 + z * ( audiobeam_S4 + z *
                            ( audiobeam_S5 + z * audiobeam_S6 ) ) );
  if ( iy == 0 ) return x + v * ( audiobeam_S1 + z * r );
  else      return x - ( ( z * ( audiobeam_half * y - v * r ) - y ) - v *
                           audiobeam_S1 );
}


float audiobeam___copysignf( float x, float y )
{
  unsigned int ix, iy;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  AUDIOBEAM_GET_FLOAT_WORD( iy, y );
  AUDIOBEAM_SET_FLOAT_WORD( x, ( ix & 0x7fffffff ) | ( iy & 0x80000000 ) );
  return x;
}


float audiobeam___cosf( float x )
{
  float y[ 2 ], z = 0.0f;
  int n, ix;

  AUDIOBEAM_GET_FLOAT_WORD( ix, x );

  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) return audiobeam___kernel_cosf( x, z );

  else
    if ( ix >= 0x7f800000 ) return x - x;

    else {
      y[ 0 ] = 0.0;
      y[ 1 ] = 0.0;
      n = audiobeam___ieee754_rem_pio2f( x, y );
      switch ( n & 3 ) {
        case 0:
          return  audiobeam___kernel_cosf( y[ 0 ], y[ 1 ] );
        case 1:
          return -audiobeam___kernel_sinf( y[ 0 ], y[ 1 ], 1 );
        case 2:
          return -audiobeam___kernel_cosf( y[ 0 ], y[ 1 ] );
        default:
          return  audiobeam___kernel_sinf( y[ 0 ], y[ 1 ], 1 );
      }
    }
}


float audiobeam___fabsf( float x )
{
  unsigned int ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  AUDIOBEAM_SET_FLOAT_WORD( x, ix & 0x7fffffff );
  return x;
}


float audiobeam___floorf( float x )
{
  int i0, j0;
  unsigned int i;
  AUDIOBEAM_GET_FLOAT_WORD( i0, x );
  j0 = ( ( i0 >> 23 ) & 0xff ) - 0x7f;
  if ( j0 < 23 ) {
    if ( j0 < 0 ) {
      if ( audiobeam_huge + x > ( float )0.0f ) {
        if ( i0 >= 0 )
          i0 = 0;
        else
          if ( ( i0 & 0x7fffffff ) != 0 )
            i0 = 0xbf800000;
      }
    } else {
      i = ( 0x007fffff ) >> j0;
      if ( ( i0 & i ) == 0 ) return x;
      if ( audiobeam_huge + x > ( float )0.0f ) {
        if ( i0 < 0 ) i0 += ( 0x00800000 ) >> j0;
        i0 &= ( ~i );
      }
    }
  } else {
    if ( j0 == 0x80 ) return x + x;
    else return x;
  }
  AUDIOBEAM_SET_FLOAT_WORD( x, i0 );
  return x;
}


int audiobeam___isinff ( float x )
{
  int ix, t;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  t = ix & 0x7fffffff;
  t ^= 0x7f800000;
  t |= -t;
  return ~( t >> 31 ) & ( ix >> 30 );
}


float audiobeam___scalbnf ( float x, int n )
{
  int k, ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  k = ( ix & 0x7f800000 ) >> 23;
  if ( k == 0 ) {
    if ( ( ix & 0x7fffffff ) == 0 ) return x;
    x *= audiobeam_two25;
    AUDIOBEAM_GET_FLOAT_WORD( ix, x );
    k = ( ( ix & 0x7f800000 ) >> 23 ) - 25;
  }
  if ( k == 0xff ) return x + x;
  k = k + n;
  if ( n > 50000 || k >  0xfe )
    return audiobeam_huge * audiobeam___copysignf( audiobeam_huge,
           x );
  if ( n < -50000 )
    return audiobeam_tiny * audiobeam___copysignf( audiobeam_tiny,
           x );
  if ( k > 0 ) {
    AUDIOBEAM_SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
    return x;
  }
  if ( k <= -25 )
    return audiobeam_tiny * audiobeam___copysignf( audiobeam_tiny,
           x );
  k += 25;
  AUDIOBEAM_SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
  return x * audiobeam_twom25;
}


float audiobeam___ceilf( float x )
{
  int i0, j0;
  unsigned int i;

  AUDIOBEAM_GET_FLOAT_WORD( i0, x );
  j0 = ( ( i0 >> 23 ) & 0xff ) - 0x7f;
  if ( j0 < 23 ) {
    if ( j0 < 0 ) {
      if ( audiobeam_huge + x > ( float )0.0 ) {
        if ( i0 < 0 )
          i0 = 0x80000000;
        else
          if ( i0 != 0 )
            i0 = 0x3f800000;
      }
    } else {
      i = ( 0x007fffff ) >> j0;
      if ( ( i0 & i ) == 0 ) return x;
      if ( audiobeam_huge + x > ( float )0.0 ) {
        if ( i0 > 0 ) i0 += ( 0x00800000 ) >> j0;
        i0 &= ( ~i );
      }
    }
  } else {
    if ( j0 == 0x80 ) return x + x;
    else return x;
  }
  AUDIOBEAM_SET_FLOAT_WORD( x, i0 );
  return x;
}


float audiobeam___ieee754_sqrtf( float x )
{
  float z;
  int sign = ( int )0x80000000;
  int ix, s, q, m, t, i;
  unsigned int r;

  AUDIOBEAM_GET_FLOAT_WORD( ix, x );

  if ( ( ix & 0x7f800000 ) == 0x7f800000 )
    return x * x + x;
  if ( ix <= 0 ) {
    if ( ( ix & ( ~sign ) ) == 0 ) return x;
    else
      if ( ix < 0 )
        return ( x - x ) / ( x - x );
  }
  m = ( ix >> 23 );
  if ( m == 0 ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( i = 0; ( ix & 0x00800000 ) == 0; i++ )
      ix <<= 1;
    m -= i - 1;
  }
  m -= 127;
  ix = ( ix & 0x007fffff ) | 0x00800000;
  if ( m & 1 )
    ix += ix;
  m >>= 1;

  ix += ix;
  q = s = 0;
  r = 0x01000000;

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

  if ( ix != 0 ) {
    z = audiobeam_one - audiobeam_tiny;
    if ( z >= audiobeam_one ) {
      z = audiobeam_one + audiobeam_tiny;
      if ( z > audiobeam_one )
        q += 2;
      else
        q += ( q & 1 );
    }
  }
  ix = ( q >> 1 ) + 0x3f000000;
  ix += ( m << 23 );
  AUDIOBEAM_SET_FLOAT_WORD( z, ix );
  return z;
}
