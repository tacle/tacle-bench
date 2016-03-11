#include "audiobeamlibm.h"
#include "audiobeamlibmath.h"

/* e_rem_pio2f.c -- float version of e_rem_pio2.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: e_rem_pio2f.c,v 1.5 1995/05/10 20:46:03 jtc Exp $";
#endif

/* This array is like the one in e_rem_pio2.c, but the numbers are
   single precision and the last 8 bits are forced to 0.  */
#ifdef __STDC__
static const int audiobeam_npio2_hw[] = {
#else
static int audiobeam_npio2_hw[] = {
#endif
  0x3fc90f00, 0x40490f00, 0x4096cb00, 0x40c90f00, 0x40fb5300, 0x4116cb00,
  0x412fed00, 0x41490f00, 0x41623100, 0x417b5300, 0x418a3a00, 0x4196cb00,
  0x41a35c00, 0x41afed00, 0x41bc7e00, 0x41c90f00, 0x41d5a000, 0x41e23100,
  0x41eec200, 0x41fb5300, 0x4203f200, 0x420a3a00, 0x42108300, 0x4216cb00,
  0x421d1400, 0x42235c00, 0x4229a500, 0x422fed00, 0x42363600, 0x423c7e00,
  0x4242c700, 0x42490f00
};

/*
 * audiobeam_invpio2:  24 bits of 2/pi
 * pio2_1:   first  17 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 17 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  17 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */

#ifdef __STDC__
static const float
#else
static float
#endif
/* zero =  0.0000000000e+00f, /\* 0x00000000 *\/ */
/* audiobeam_half =  5.0000000000e-01f, /\* 0x3f000000 *\/ */
/* two8 =  2.5600000000e+02f, /\* 0x43800000 *\/ */
audiobeam_invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
audiobeam_pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
audiobeam_pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
audiobeam_pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
audiobeam_pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
audiobeam_pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
audiobeam_pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

#ifdef __STDC__
int audiobeam___ieee754_rem_pio2f( float x, float *y )
#else
int audiobeam___ieee754_rem_pio2f( x, y )
float x, y[];
#endif
{
  float z, w, t, r, fn;
  int i, j, n = 0, ix, hx;

  AUDIOBEAM_GET_FLOAT_WORD( hx, x );
  ix = hx & 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) { /* |x| ~<= pi/4 , no need for reduction */
    y[0] = x;
    y[1] = 0;
    return 0;
  }
  if ( ix < 0x4016cbe4 ) { /* |x| < 3pi/4, special case with n=+-1 */
    if ( hx > 0 ) {
      z = x - audiobeam_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[0] = z - audiobeam_pio2_1t;
        y[1] = ( z - y[0] ) - audiobeam_pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z -= audiobeam_pio2_2;
        y[0] = z - audiobeam_pio2_2t;
        y[1] = ( z - y[0] ) - audiobeam_pio2_2t;
      }
      return 1;
    } else {  /* negative x */
      z = x + audiobeam_pio2_1;
      if ( ( ix & 0xfffffff0 ) != 0x3fc90fd0 ) { /* 24+24 bit pi OK */
        y[0] = z + audiobeam_pio2_1t;
        y[1] = ( z - y[0] ) + audiobeam_pio2_1t;
      } else {    /* near pi/2, use 24+24+24 bit pi */
        z += audiobeam_pio2_2;
        y[0] = z + audiobeam_pio2_2t;
        y[1] = ( z - y[0] ) + audiobeam_pio2_2t;
      }
      return -1;
    }
  }
  if ( ix <= 0x43490f80 ) { /* |x| ~<= 2^7*(pi/2), medium size */
    t  = audiobeam_fabsf( x );
    n  = ( int ) ( t * audiobeam_invpio2 + audiobeam_half );
    fn = ( float )n;
    r  = t - fn * audiobeam_pio2_1;
    w  = fn * audiobeam_pio2_1t; /* 1st round good to 40 bit */
    if ( n < 32 && ( int )( ix & 0xffffff00 ) != audiobeam_npio2_hw[n - 1] ) {
      y[0] = r - w; /* quick check no cancellation */
    } else {
      unsigned int high;
      j  = ix >> 23;
      y[0] = r - w;
      AUDIOBEAM_GET_FLOAT_WORD( high, y[0] );
      i = j - ( ( high >> 23 ) & 0xff );
      if ( i > 8 ) { /* 2nd iteration needed, good to 57 */
        t  = r;
        w  = fn * audiobeam_pio2_2;
        r  = t - w;
        w  = fn * audiobeam_pio2_2t - ( ( t - r ) - w );
        y[0] = r - w;
        AUDIOBEAM_GET_FLOAT_WORD( high, y[0] );
        i = j - ( ( high >> 23 ) & 0xff );
        if ( i > 25 )  { /* 3rd iteration need, 74 bits acc */
          t  = r; /* will cover all possible cases */
          w  = fn * audiobeam_pio2_3;
          r  = t - w;
          w  = fn * audiobeam_pio2_3t - ( ( t - r ) - w );
          y[0] = r - w;
        }
      }
    }
    y[1] = ( r - y[0] ) - w;
    if ( hx < 0 )  {
      y[0] = -y[0];
      y[1] = -y[1];
      return -n;
    } else   return n;
  }
  /*
   * all other (large) arguments
   */
  if ( ix >= 0x7f800000 ) { /* x is inf or NaN */
    y[0] = y[1] = x - x;
    return 0;
  }

  return n;
}


/* k_cosf.c -- float version of k_cos.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: k_cosf.c,v 1.4 1995/05/10 20:46:23 jtc Exp $";
#endif


#ifdef __STDC__
static const float
#else
static float
#endif
/* audiobeam_one =  1.0000000000e+00, /\* 0x3f800000 *\/ */
audiobeam_C1  =  4.1666667908e-02f, /* 0x3d2aaaab */
audiobeam_C2  = -1.3888889225e-03f, /* 0xbab60b61 */
audiobeam_C3  =  2.4801587642e-05f, /* 0x37d00d01 */
audiobeam_C4  = -2.7557314297e-07f, /* 0xb493f27c */
audiobeam_C5  =  2.0875723372e-09f, /* 0x310f74f6 */
audiobeam_C6  = -1.1359647598e-11f; /* 0xad47d74e */

#ifdef __STDC__
float audiobeam___kernel_cosf( float x, float y )
#else
float audiobeam___kernel_cosf( x, y )
float x, y;
#endif
{
  float a, hz, z, r, qx;
  int ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* ix = |x|'s high word*/
  if ( ix < 0x32000000 ) { /* if x < 2**27 */
    if ( ( ( int )x ) == 0 ) return audiobeam_one; /* generate inexact */
  }
  z  = x * x;
  r  = z * ( audiobeam_C1 + z * ( audiobeam_C2 + z * ( audiobeam_C3 + z *
                                  ( audiobeam_C4 + z * ( audiobeam_C5 + z * audiobeam_C6 ) ) ) ) );
  if ( ix < 0x3e99999a )    /* if |x| < 0.3 */
    return audiobeam_one - ( ( float )0.5f * z - ( z * r - x * y ) );
  else {
    if ( ix > 0x3f480000 ) { /* x > 0.78125 */
      qx = ( float )0.28125f;
    } else {
      AUDIOBEAM_SET_FLOAT_WORD( qx, ix - 0x01000000 ); /* x/4 */
    }
    hz = ( float )0.5f * z - qx;
    a  = audiobeam_one - qx;
    return a - ( hz - ( z * r - x * y ) );
  }
}

/* k_sinf.c -- float version of k_sin.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: k_sinf.c,v 1.4 1995/05/10 20:46:33 jtc Exp $";
#endif


#ifdef __STDC__
static const float
#else
static float
#endif
/* audiobeam_half =  5.0000000000e-01f,/\* 0x3f000000 *\/ */
S1  = -1.6666667163e-01f, /* 0xbe2aaaab */
S2  =  8.3333337680e-03f, /* 0x3c088889 */
S3  = -1.9841270114e-04f, /* 0xb9500d01 */
S4  =  2.7557314297e-06f, /* 0x3638ef1b */
S5  = -2.5050759689e-08f, /* 0xb2d72f34 */
S6  =  1.5896910177e-10f; /* 0x2f2ec9d3 */

#ifdef __STDC__
float audiobeam___kernel_sinf( float x, float y, int iy )
#else
float audiobeam___kernel_sinf( x, y, iy )
float x, y;
int iy;    /* iy=0 if y is zero */
#endif
{
  float z, r, v;
  int ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  ix &= 0x7fffffff;     /* high word of x */
  if ( ix < 0x32000000 ) { /* |x| < 2**-27 */
    if ( ( int )x == 0 ) return x; /* generate inexact */
  }
  z =  x * x;
  v =  z * x;
  r =  S2 + z * ( S3 + z * ( S4 + z * ( S5 + z * S6 ) ) );
  if ( iy == 0 ) return x + v * ( S1 + z * r );
  else      return x - ( ( z * ( audiobeam_half * y - v * r ) - y ) - v * S1 );
}


/* s_copysignf.c -- float version of s_copysign.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_copysignf.c,v 1.4 1995/05/10 20:46:59 jtc Exp $";
#endif

/*
 * copysignf(float x, float y)
 * copysignf(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */


#ifdef __STDC__
float audiobeam___copysignf( float x, float y )
#else
float audiobeam___copysignf( x, y )
float x, y;
#endif
{
  unsigned int ix, iy;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  AUDIOBEAM_GET_FLOAT_WORD( iy, y );
  AUDIOBEAM_SET_FLOAT_WORD( x, ( ix & 0x7fffffff ) | ( iy & 0x80000000 ) );
  return x;
}
//weak_alias (__copysignf, copysignf)
/* s_cosf.c -- float version of s_cos.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_cosf.c,v 1.4 1995/05/10 20:47:03 jtc Exp $";
#endif


/* #ifdef __STDC__ */
/* static const float audiobeam_one=1.0; */
/* #else */
/* static float audiobeam_one=1.0; */
/* #endif */

#ifdef __STDC__
float audiobeam___cosf( float x )
#else
float audiobeam___cosf( x )
float x;
#endif
{
  float y[2], z = 0.0f;
  int n, ix;

  AUDIOBEAM_GET_FLOAT_WORD( ix, x );

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if ( ix <= 0x3f490fd8 ) return audiobeam___kernel_cosf( x, z );

  /* cos(Inf or NaN) is NaN */
  else
    if ( ix >= 0x7f800000 ) return x - x;

    /* argument reduction needed */
    else {
      n = audiobeam___ieee754_rem_pio2f( x, y );
      switch ( n & 3 ) {
        case 0:
          return  audiobeam___kernel_cosf( y[0], y[1] );
        case 1:
          return -audiobeam___kernel_sinf( y[0], y[1], 1 );
        case 2:
          return -audiobeam___kernel_cosf( y[0], y[1] );
        default:
          return  audiobeam___kernel_sinf( y[0], y[1], 1 );
      }
    }
}

/* s_fabsf.c -- float version of s_fabs.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_fabsf.c,v 1.4 1995/05/10 20:47:15 jtc Exp $";
#endif

/*
 * fabsf(x) returns the absolute value of x.
 */


#ifdef __STDC__
float audiobeam___fabsf( float x )
#else
float audiobeam___fabsf( x )
float x;
#endif
{
  unsigned int ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  AUDIOBEAM_SET_FLOAT_WORD( x, ix & 0x7fffffff );
  return x;
}

/* s_floorf.c -- float version of s_floor.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_floorf.c,v 1.4 1995/05/10 20:47:22 jtc Exp $";
#endif

/*
 * floorf(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *  Bit twiddling.
 * Exception:
 *  Inexact flag raised if x not equal to floorf(x).
 */


/* #ifdef __STDC__ */
/* static const float audiobeam_huge = 1.0e30f; */
/* #else */
/* static float audiobeam_huge = 1.0e30f; */
/* #endif */

#ifdef __STDC__
float audiobeam___floorf( float x )
#else
float audiobeam___floorf( x )
float x;
#endif
{
  int i0, j0;
  unsigned int i;
  AUDIOBEAM_GET_FLOAT_WORD( i0, x );
  j0 = ( ( i0 >> 23 ) & 0xff ) - 0x7f;
  if ( j0 < 23 ) {
    if ( j0 < 0 ) { /* raise inexact if x != 0 */
      if ( audiobeam_huge + x > ( float )0.0f ) { /* return 0*sign(x) if |x|<1 */
        if ( i0 >= 0 ) {
          i0 = 0;
        } else
          if ( ( i0 & 0x7fffffff ) != 0 ) {
            i0 = 0xbf800000;
          }
      }
    } else {
      i = ( 0x007fffff ) >> j0;
      if ( ( i0 & i ) == 0 ) return x; /* x is integral */
      if ( audiobeam_huge + x > ( float )0.0f ) { /* raise inexact flag */
        if ( i0 < 0 ) i0 += ( 0x00800000 ) >> j0;
        i0 &= ( ~i );
      }
    }
  } else {
    if ( j0 == 0x80 ) return x + x; /* inf or NaN */
    else return x;    /* x is integral */
  }
  AUDIOBEAM_SET_FLOAT_WORD( x, i0 );
  return x;
}


/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_isinff.c,v 1.3 1995/05/11 23:20:21 jtc Exp $";
#endif

/*
 * isinff(x) returns 1 if x is inf, -1 if x is -inf, else 0;
 * no branching!
 */


int
__isinff ( float x )
{
  int ix, t;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  t = ix & 0x7fffffff;
  t ^= 0x7f800000;
  t |= -t;
  return ~( t >> 31 ) & ( ix >> 30 );
}
/* hidden_def (__isinff) */
/* weak_alias (__isinff, isinff) */
/* s_scalbnf.c -- float version of s_scalbn.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_scalbnf.c,v 1.4 1995/05/10 20:48:10 jtc Exp $";
#endif


#ifdef __STDC__
static const float
#else
static float
#endif
two25   =  3.355443200e+07f,  /* 0x4c000000 */
twom25  =  2.9802322388e-08f; /* 0x33000000 */
/* audiobeam_huge   = 1.0e+30, */
/* audiobeam_tiny   = 1.0e-30; */

#ifdef __STDC__
float audiobeam___scalbnf ( float x, int n )
#else
float audiobeam___scalbnf ( x, n )
float x;
int n;
#endif
{
  int k, ix;
  AUDIOBEAM_GET_FLOAT_WORD( ix, x );
  k = ( ix & 0x7f800000 ) >> 23; /* extract exponent */
  if ( k == 0 ) {   /* 0 or subnormal x */
    if ( ( ix & 0x7fffffff ) == 0 ) return x; /* +-0 */
    x *= two25;
    AUDIOBEAM_GET_FLOAT_WORD( ix, x );
    k = ( ( ix & 0x7f800000 ) >> 23 ) - 25;
  }
  if ( k == 0xff ) return x + x; /* NaN or Inf */
  k = k + n;
  if ( n > 50000 || k >  0xfe )
    return audiobeam_huge * audiobeam___copysignf( audiobeam_huge,
           x ); /* overflow  */
  if ( n < -50000 )
    return audiobeam_tiny * audiobeam___copysignf( audiobeam_tiny,
           x ); /*underflow*/
  if ( k > 0 ) {    /* normal result */
    AUDIOBEAM_SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
    return x;
  }
  if ( k <= -25 )
    return audiobeam_tiny * audiobeam___copysignf( audiobeam_tiny,
           x ); /*underflow*/
  k += 25;        /* subnormal result */
  AUDIOBEAM_SET_FLOAT_WORD( x, ( ix & 0x807fffff ) | ( k << 23 ) );
  return x * twom25;
}
//weak_alias (__scalbnf, scalbnf)


/* s_ceilf.c -- float version of s_ceil.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: s_ceilf.c,v 1.4 1995/05/10 20:46:55 jtc Exp $";
#endif

#ifdef __STDC__
float audiobeam___ceilf( float x )
#else
float audiobeam___ceilf( x )
float x;
#endif
{
  int i0, j0;
  unsigned int i;

  AUDIOBEAM_GET_FLOAT_WORD( i0, x );
  j0 = ( ( i0 >> 23 ) & 0xff ) - 0x7f;
  if ( j0 < 23 ) {
    if ( j0 < 0 ) { /* raise inexact if x != 0 */
      if ( audiobeam_huge + x > ( float )0.0 ) { /* return 0*sign(x) if |x|<1 */
        if ( i0 < 0 ) {
          i0 = 0x80000000;
        } else
          if ( i0 != 0 ) {
            i0 = 0x3f800000;
          }
      }
    } else {
      i = ( 0x007fffff ) >> j0;
      if ( ( i0 & i ) == 0 ) return x; /* x is integral */
      if ( audiobeam_huge + x > ( float )0.0 ) { /* raise inexact flag */
        if ( i0 > 0 ) i0 += ( 0x00800000 ) >> j0;
        i0 &= ( ~i );
      }
    }
  } else {
    if ( j0 == 0x80 ) return x + x; /* inf or NaN */
    else return x;    /* x is integral */
  }
  AUDIOBEAM_SET_FLOAT_WORD( x, i0 );
  return x;
}


/* e_sqrtf.c -- float version of e_sqrt.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char audiobeam_rcsid[] =
  "$NetBSD: e_sqrtf.c,v 1.4 1995/05/10 20:46:19 jtc Exp $";
#endif


/* #ifdef __STDC__ */
/* static const float audiobeam_one = 1.0f, audiobeam_tiny=1.0e-30f; */
/* #else */
/* static float audiobeam_one = 1.0f, audiobeam_tiny=1.0e-30f; */
/* #endif */

#ifdef __STDC__
float audiobeam___ieee754_sqrtf( float x )
#else
float audiobeam___ieee754_sqrtf( x )
float x;
#endif
{
  float z;
  int sign = ( int )0x80000000;
  int ix, s, q, m, t, i;
  unsigned int r;

  AUDIOBEAM_GET_FLOAT_WORD( ix, x );

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
    for ( i = 0; ( ix & 0x00800000 ) == 0; i++ ) {
      ix <<= 1;
    }
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
    z = audiobeam_one - audiobeam_tiny; /* trigger inexact flag */
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
