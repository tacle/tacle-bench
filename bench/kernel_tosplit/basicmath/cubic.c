/* +++Date last modified: 05-Jul-1997 */

/*
**  CUBIC.C - Solve a cubic polynomial
**  public domain by Ross Cottrell
*/

#include "snipmath.h"

void basicmath_solveCubic( float a, float b, float c, float d,
                           int *solutions, float *x )
{
  float a1 = b / a, a2 = c / a, a3 = d / a;
  float Q = ( a1 * a1 - 3.0f * a2 ) / 9.0f;
  float R = ( 2.0f * a1 * a1 * a1 - 9.0f * a1 * a2 + 27.0f * a3 ) / 54.0f;
  float R2_Q3 = R * R - Q * Q * Q;
  float theta;

  if ( R2_Q3 <= 0 ) {
    *solutions = 3;
    theta = basicmath___ieee754_acosf( R / basicmath___ieee754_sqrtf( Q * Q * Q ) );
    x[0] = -2.0f * basicmath___ieee754_sqrtf( Q ) * basicmath___cosf(
             theta / 3.0f ) - a1 / 3.0f;
    x[1] = -2.0f * basicmath___ieee754_sqrtf( Q ) * basicmath___cosf( (
             theta + 2.0f * PI ) / 3.0f ) - a1 / 3.0f;
    x[2] = -2.0f * basicmath___ieee754_sqrtf( Q ) * basicmath___cosf( (
             theta + 4.0f * PI ) / 3.0f ) - a1 / 3.0f;
  } else {
    *solutions = 1;
    x[0] = basicmath___ieee754_powf( basicmath___ieee754_sqrtf( R2_Q3 ) +
                                     basicmath___fabsf( R ), 1 / 3.0f );
    x[0] += Q / x[0];
    x[0] *= ( R < 0.0f ) ? 1 : -1;
    x[0] -= a1 / 3.0f;
  }
}
