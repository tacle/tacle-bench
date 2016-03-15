/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: binarysearch

  Author: Sung-Soo Lim <sslim@archi.snu.ac.kr>

  Function: binarysearch performs binary search in an array of 15 integer
    elements.
    This program is completely structured (no unconditional jumps, no exits
    from loop bodies), and does not contain switch statements, no do-while
    loops.

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/bs/bs.c

  Original name: bs

  Changes: No major functional changes.

  License: May be used, modified, and re-distributed freely, but
           the SNU-RT Benchmark Suite must be acknowledged

*/

/*
  This program is derived from the SNU-RT Benchmark Suite for Worst
  Case Timing Analysis by Sung-Soo Lim
*/


/*
  Forward declaration of functions
*/

void bs_initSeed( void );
long bs_randomInteger( void );
void bs_init( void );
int bs_return( void );
int bs_binary_search( int );
void bs_main( void );
int main( void );


/*
  Declaration of global variables
*/

volatile int bs_seed;

struct bs_DATA {
  int key;
  int value;
};

struct bs_DATA bs_data[ 15 ];

int bs_result;


/*
  Initialization- and return-value-related functions
*/

/*
  bs_initSeed initializes the seed used in the "random" number generator.
*/
void bs_initSeed( void )
{
  bs_seed = 0;
}


/*
  bs_RandomInteger generates "random" integers between 0 and 8094.
*/
long bs_randomInteger( void )
{
  bs_seed = ( ( bs_seed * 133 ) + 81 ) % 8095;
  return( bs_seed );
}


void bs_init( void )
{
  int i;

  bs_initSeed();

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < 15; ++i ) {
    bs_data[ i ].key = bs_randomInteger();
    bs_data[ i ].value = bs_randomInteger();
  }
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

void _Pragma( "entrypoint" ) bs_main( void )
{
  bs_result = bs_binary_search( 8 );
}


int main( void )
{
  bs_init();
  bs_main();

  return( bs_return() - 255 );
}
