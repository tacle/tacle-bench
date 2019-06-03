/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: rad2deg

  Author: unknown

  Function: rad2deg performs conversion of radiant to degree

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Original name: basicmath_small

  Changes: no major functional changes

  License: this code is FREE with no restrictions

*/


#include "pi.h"

#define rad2deg(r) ((r)*180/PI)


/*
  Forward declaration of functions
*/

void rad2deg_init( void );
void rad2deg_main( void );
int rad2deg_return( void );
int main( void );


/*
  Declaration of global variables
*/

float rad2deg_X, rad2deg_Y;


/*
  Initialization function
*/

void rad2deg_init( void )
{
  rad2deg_X = 0;
  rad2deg_Y = 0;
}


/*
  Return function
*/

int rad2deg_return( void )
{
  int temp = rad2deg_Y;

  if ( temp == 64620 )
    return 0;
  else
    return -1;
}


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) rad2deg_main( void )
{
  _Pragma( "loopbound min 360 max 360" )
  for ( rad2deg_X = 0.0f; rad2deg_X <= ( 2 * PI + 1e-6f ); rad2deg_X += ( PI / 180 ) )
    rad2deg_Y += rad2deg( rad2deg_X );
}


int main( void )
{
  rad2deg_init();
  rad2deg_main();
  return rad2deg_return();
}
