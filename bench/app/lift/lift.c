/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: lift

  Author: Martin Schoeberl, Benedikt Huber

  Function: Lift Controler

  Source: C-Port from http://www.jopdesign.com/doc/jembench.pdf

  Original name: run_lift.c

  Changes: no major functional changes

  License: GPL version 3 or later

*/


/*
  Include section
*/

#include "liftlibio.h"
#include "liftlibcontrol.h"


/*
  Forward declaration of functions
*/

void lift_controller();
void lift_init();
void lift_main();
int lift_return();


/*
  Declaration of global variables
*/

int lift_checksum;/* Checksum */


/*
  Initialization- and return-value-related functions
*/

void lift_init()
{
  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = ( unsigned char * ) &lift_ctrl_io_in[ 0 ];
  _Pragma( "loopbound min 40 max 40" )
  for ( i = 0; i < sizeof( lift_ctrl_io_in ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &lift_ctrl_io_out[ 0 ];
  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0; i < sizeof( lift_ctrl_io_out ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &lift_ctrl_io_analog[ 0 ];
  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0; i < sizeof( lift_ctrl_io_analog ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &lift_ctrl_io_led[ 0 ];
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < sizeof( lift_ctrl_io_led ); ++i, ++p )
    *p ^= bitmask;

  lift_checksum = 0;
  lift_ctrl_init();
}


int lift_return()
{
  return ( lift_checksum - 4005888 != 0 );
}


/*
  Algorithm core functions
*/

void lift_controller()
{
  lift_ctrl_get_vals();
  lift_ctrl_loop();
  lift_ctrl_set_vals();
}


/*
  Main functions
*/

void  _Pragma( "entrypoint" ) lift_main()
{
  int i = 0;
  _Pragma( "loopbound min 1001 max 1001" )
  while ( 1 ) {
    /* zero input stimulus */
    lift_simio_in = 0;
    lift_simio_adc1 = 0;
    lift_simio_adc2 = 0;
    lift_simio_adc3 = 0;
    /* run lift_controller */
    lift_controller();
    if ( i++ >= 1000 )
      break;
  }
}


int main( void )
{
  lift_init();
  lift_main();

  return ( lift_return() );
}
