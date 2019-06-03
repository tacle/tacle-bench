/*
**  CUBIC.C - Solve a cubic polynomial
**  public domain by Ross Cottrell
*/

/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: cubic

  Author: Ross Cottrell

  Function: cubic solves some cubic functions

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Original name: basicmath_small

  Changes: no major functional changes

  License: this code is FREE with no restrictions

*/


#include "wcclibm.h"
#include "snipmath.h"


/*
  Forward declaration of functions
*/

void cubic_solveCubic( float a, float b, float c, float d,
                 int *solutions, float *x );
void cubic_main( void );
void cubic_init( void );
int cubic_return( void );
int main( void );


/*
  Declaration of global variables
*/

float cubic_a1, cubic_b1, cubic_c1, cubic_d1;
float cubic_a2, cubic_b2, cubic_c2, cubic_d2;
float cubic_a3, cubic_b3, cubic_c3, cubic_d3;
float cubic_a4, cubic_b4, cubic_c4, cubic_d4;
float cubic_x[3];
float cubic_X, cubic_Y;
int cubic_solutions;
int cubic_checksum;


/*
  Initialization function
*/

void cubic_init( void )
{
  cubic_a1 = 1.0f, cubic_b1 = -10.5f, cubic_c1 = 32.0f, cubic_d1 = -30.0f;
  cubic_a2 = 1.0f, cubic_b2 = -4.5f, cubic_c2 = 17.0f, cubic_d2 = -30.0f;
  cubic_a3 = 1.0f, cubic_b3 = -3.5f, cubic_c3 = 22.0f, cubic_d3 = -31.0f;
  cubic_a4 = 1.0f, cubic_b4 = -13.7f, cubic_c4 = 1.0f, cubic_d4 = -35.0f;
  cubic_X = 0, cubic_Y = 0;
  cubic_checksum = 0;
}


/*
  Return function
*/

int cubic_return( void )
{
  if ( cubic_checksum == 1051 )
    return 0;
  else
    return -1;
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) cubic_main( void )
{
  /* solve some cubic functions */
  /* should get 3 solutions: 2, 6 & 2.5   */
  cubic_solveCubic( cubic_a1, cubic_b1, cubic_c1, cubic_d1, &cubic_solutions, cubic_x );
  cubic_checksum += cubic_solutions;
  cubic_solveCubic( cubic_a2, cubic_b2, cubic_c2, cubic_d2, &cubic_solutions, cubic_x );
  cubic_checksum += cubic_solutions;
  cubic_solveCubic( cubic_a3, cubic_b3, cubic_c3, cubic_d3, &cubic_solutions, cubic_x );
  cubic_checksum += cubic_solutions;
  cubic_solveCubic( cubic_a4, cubic_b4, cubic_c4, cubic_d4, &cubic_solutions, cubic_x );
  cubic_checksum += cubic_solutions;

  /* Now solve some random equations */
  _Pragma( "loopbound min 5 max 5" )
  for ( cubic_a1 = 1; cubic_a1 < 10; cubic_a1 += 2 ) {
    _Pragma( "loopbound min 5 max 5" )
    for ( cubic_b1 = 10; cubic_b1 > 0; cubic_b1 -= 2 ) {
      _Pragma( "loopbound min 7 max 7" )
      for ( cubic_c1 = 5; cubic_c1 < 15; cubic_c1 += 1.5f ) {
        _Pragma( "loopbound min 5 max 5" )
        for ( cubic_d1 = -1; cubic_d1 > -11; cubic_d1 -= 2 ) {
          cubic_solveCubic( cubic_a1, cubic_b1, cubic_c1, cubic_d1, &cubic_solutions, cubic_x );
          cubic_checksum += cubic_solutions;
        }
      }
    }
  }
}


void cubic_solveCubic( float a, float b, float c, float d,
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


int main( void )
{
  cubic_init();
  cubic_main();
  return cubic_return();
}
