/*

  This file is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: pm_libm.c

  Author: Hector Chan
          MIT Lincoln Laboratory

  Function: This file contains the C math library functions used by pm.

  Source: HPEC Challenge Benchmark Suite, Pattern Match Kernel Benchmark

  Original name: pm

  Changes: See ChangeLog.txt

  License: BSD 3-clause

*/

/*
  Include section
*/

#include "pm_math.h"


/*
  Declaration of global variables
*/

#define pm_LOG10 2.302585093f

/* The coefficients for the pm_log10f and pm_pow10f functions below */
static float pm_pow_coeff[ 19 ];
static float pm_log_coeff[ 16 ];


/*
  Math functions
*/

/***********************************************************************/
/* We found out the bottle neck of this kernel was in the pow and log
   functions. Therefore, we have implemented our own log and pow, instead
   of using the float fp ones in the standard C math libary. This function
   sets up the coefficients for the single fp log and pow functions. */
/***********************************************************************/
void pm_math_init( void )
{
  pm_pow_coeff[ 0 ]  = 0.5f;             /* 1/2! */
  pm_pow_coeff[ 1 ]  = 0.166666667f;     /* 1/3! */
  pm_pow_coeff[ 2 ]  = 0.041666666f;     /* 1/4! */
  pm_pow_coeff[ 3 ]  = 8.333333333e-3f;
  pm_pow_coeff[ 4 ]  = 1.388888889e-3f;
  pm_pow_coeff[ 5 ]  = 1.984126984e-4f;
  pm_pow_coeff[ 6 ]  = 2.480158730e-5f;
  pm_pow_coeff[ 7 ]  = 2.755731922e-6f;
  pm_pow_coeff[ 8 ]  = 2.755731922e-7f;
  pm_pow_coeff[ 9 ]  = 2.505210839e-8f;
  pm_pow_coeff[ 10 ] = 2.087675699e-9f;
  pm_pow_coeff[ 11 ] = 1.605904384e-10f;
  pm_pow_coeff[ 12 ] = 1.147074560e-11f;
  pm_pow_coeff[ 13 ] = 7.647163732e-13f;
  pm_pow_coeff[ 14 ] = 4.779477332e-14f;
  pm_pow_coeff[ 15 ] = 2.811457254e-15f;
  pm_pow_coeff[ 16 ] = 1.561920697e-16f;
  pm_pow_coeff[ 17 ] = 8.220635247e-18f;
  pm_pow_coeff[ 18 ] = 4.110317623e-19f;

  pm_log_coeff[ 0 ]  = 0.333333333f;     /* 1/3 */
  pm_log_coeff[ 1 ]  = 0.2f;             /* 1/5 */
  pm_log_coeff[ 2 ]  = 0.142857143f;     /* 1/7 */
  pm_log_coeff[ 3 ]  = 0.111111111f;     /* 1/9 */
  pm_log_coeff[ 4 ]  = 9.090909091e-2f;  /* 1/11 */
  pm_log_coeff[ 5 ]  = 7.692307692e-2f;  /* 1/13 */
  pm_log_coeff[ 6 ]  = 6.666666667e-2f;  /* 1/15 */
  pm_log_coeff[ 7 ]  = 5.882352941e-2f;  /* 1/17 */
  pm_log_coeff[ 8 ]  = 5.263157895e-2f;  /* 1/19 */
  pm_log_coeff[ 9 ]  = 4.761904762e-2f;  /* 1/21 */
  pm_log_coeff[ 10 ] = 4.347826087e-2f;  /* 1/23 */
  pm_log_coeff[ 11 ] = 0.04f;            /* 1/25 */
  pm_log_coeff[ 12 ] = 3.703703704e-2f;  /* 1/27 */
  pm_log_coeff[ 13 ] = 3.448275862e-2f;  /* 1/29 */
  pm_log_coeff[ 14 ] = 3.225806452e-2f;  /* 1/31 */
  pm_log_coeff[ 15 ] = 3.030303030e-2f;  /* 1/33 */
}


/***********************************************************************/
/* This single fp pow base 10 function implements the corresponding
   Taylor series.  The loop has been unrolled to save ops. */
/***********************************************************************/
float pm_pow10f ( float exp )
{
  float mul = exp * pm_LOG10;
  float const term = exp * pm_LOG10;
  float ans = 1.0f;
  float const *fptr = pm_pow_coeff;

  ans += mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;

  return ans;
}


/***********************************************************************/
/* This single fp log base 10 function implements the corresponding
   Taylor series. The loop has been unrolled to save ops. */
/***********************************************************************/
float pm_log10f ( float exp )
{
  float mul = ( exp - 1.0f ) / ( exp + 1.0f );
  float ans = 0.0f;
  float const *fptr = pm_log_coeff;
  float const term = mul * mul;

  ans  = mul;
  mul *= term;

  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;
  mul *= term;
  ans += *fptr++ * mul;

  ans *= 0.86858896381f;  /* ans = ans * 2 / log(10) */

  return ans;
}


float pm_fabs( float n )
{
  if ( n >= 0 )
    return n;
  else
    return -n;
}


float pm_floor( float arg )
{
  if ( arg >= 0 ) return ( int )arg;
  return -( ( int )( -arg ) + 1 );
}


float pm_ceil( float arg )
{
  if ( arg > 0 ) return ( int )( arg + 1 );
  return ( int )( arg );
}
