/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: basicmath

  Author: unknown

  Function: basicmath performs simple mathematical calculations that often
            don't have dedicated hardware support in embedded processors.

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Original name: basicmath_small

  Changes: no major functional changes

  License: this code is FREE with no restrictions

*/


/*
  Include section
*/

#include "wcclibm.h"
#include "snipmath.h"


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) basicmath_main( void )
{
  float a1 = 1.0f, b1 = -10.5f, c1 = 32.0f, d1 = -30.0f;
  float a2 = 1.0f, b2 = -4.5f, c2 = 17.0f, d2 = -30.0f;
  float a3 = 1.0f, b3 = -3.5f, c3 = 22.0f, d3 = -31.0f;
  float a4 = 1.0f, b4 = -13.7f, c4 = 1.0f, d4 = -35.0f;
  float x[3];
  float X, Y = 0;
  int solutions;
  int i;
  unsigned long l = 0x3fed0169L;
  struct int_sqrt q;

  /* solve some cubic functions */
  /* should get 3 solutions: 2, 6 & 2.5   */
  basicmath_solveCubic( a1, b1, c1, d1, &solutions, x );

  basicmath_solveCubic( a2, b2, c2, d2, &solutions, x );

  basicmath_solveCubic( a3, b3, c3, d3, &solutions, x );

  basicmath_solveCubic( a4, b4, c4, d4, &solutions, x );

  /* Now solve some random equations */
  _Pragma( "loopbound min 5 max 5" )
  for ( a1 = 1; a1 < 10; a1 += 2 ) {
    _Pragma( "loopbound min 5 max 5" )
    for ( b1 = 10; b1 > 0; b1 -= 2 ) {
      _Pragma( "loopbound min 7 max 7" )
      for ( c1 = 5; c1 < 15; c1 += 1.5f ) {
        _Pragma( "loopbound min 5 max 5" )
        for ( d1 = -1; d1 > -11; d1 -= 2 )
          basicmath_solveCubic( a1, b1, c1, d1, &solutions, x );
      }
    }
  }

  /* perform some integer square roots */
  _Pragma( "loopbound min 1000 max 1000" )
  for ( i = 1; i < 1001; i += 1 ) {
    basicmath_usqrt( i, &q );
    // remainder differs on some machines
  }

  basicmath_usqrt( l, &q );

  /* convert some rads to degrees */
  _Pragma( "loopbound min 361 max 361" )
  for ( X = 0.0f; X <= 360.0f; X += 1.0f )
    Y += deg2rad( X );

  _Pragma( "loopbound min 360 max 360" )
  for ( X = 0.0f; X <= ( 2 * PI + 1e-6f ); X += ( PI / 180 ) )
    Y += rad2deg( X );
}


int main( void )
{
  basicmath_main();

  return 0;
}
