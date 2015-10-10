/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: binarysearch

  Author: unknown

  Function: binarysearch performs binary search in an array of 15 integer
    elements.
    This program is completely structured (no unconditional jumps, no exits
    from loop bodies), and does not contain switch statements, no do-while
    loops.

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/bs/bs.c

  Changes:

  License: general open-source

*/


/*
  Forward declaration of functions
*/

void bs_init( void );
int bs_return( void );
int bs_binary_search( int );
void bs_main( void );
int main( void );


/*
  Declaration of global variables
*/

struct bs_DATA {
  int key;
  int value;
};

struct bs_DATA bs_data[ 15 ];

int bs_result;


/*
  Initialization- and return-value-related functions
*/

void bs_init( void )
{
  bs_data[ 0 ].key = 1;
  bs_data[ 0 ].value = 100;
  bs_data[ 1 ].key = 5;
  bs_data[ 1 ].value = 200;
  bs_data[ 2 ].key = 6;
  bs_data[ 2 ].value = 300;
  bs_data[ 3 ].key = 7;
  bs_data[ 3 ].value = 700;
  bs_data[ 4 ].key = 8;
  bs_data[ 4 ].value = 900;
  bs_data[ 5 ].key = 9;
  bs_data[ 5 ].value = 250;
  bs_data[ 6 ].key = 10;
  bs_data[ 6 ].value = 400;
  bs_data[ 7 ].key = 11;
  bs_data[ 7 ].value = 600;
  bs_data[ 8 ].key = 12;
  bs_data[ 8 ].value = 800;
  bs_data[ 9 ].key = 13;
  bs_data[ 9 ].value = 1500;
  bs_data[ 10 ].key = 14;
  bs_data[ 10 ].value = 1200;
  bs_data[ 11 ].key = 15;
  bs_data[ 11 ].value = 110;
  bs_data[ 12 ].key = 16;
  bs_data[ 12 ].value = 140;
  bs_data[ 13 ].key = 17;
  bs_data[ 13 ].value = 133;
  bs_data[ 14 ].key = 18;
  bs_data[ 14 ].value = 10;
}


int bs_return( void )
{
  return( bs_result );
}


/*
  Algorithm core functions
*/

int bs_binary_search( int x )
{
  int fvalue, mid, up, low;

  low = 0;
  up = 14;
  fvalue = -1;

  _Pragma( "loopbound min 1 max 4" )
  while ( low <= up ) {
    mid = ( low + up ) >> 1;

    if ( bs_data[ mid ].key == x ) {
      /* Item found */
      up = low - 1;
      fvalue = bs_data[ mid ].value;
    } else

    if ( bs_data[ mid ].key > x )
      /* Item not found */
      up = mid - 1;
    else
      low = mid + 1;
  }

  return( fvalue );
}


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) bs_main( void )
{
  bs_result = bs_binary_search( 8 );
}


int main( void )
{
  bs_init();
  bs_main();

  return( bs_return() );
}
