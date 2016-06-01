/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: bitonic

  Author: Chris Leger

  Function: bitonic implements a recursive sorting network algorithm.

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Changes: no major functional changes

  License: MIT

*/


/*
  Forward declaration of functions
*/
void bitonic_init( void );
int bitonic_return( void );
void bitonic_compare( int i, int j, int dir );
void bitonic_merge( int lo, int cnt, int dir );
void bitonic_sort( int lo, int cnt, int dir );
int main( void );


/*
  Declaration of global variables
*/

int bitonic_numiters = 10;
int bitonic_a[ 32 ];         // the array to be sorted
const int bitonic_ASCENDING = 1;
const int bitonic_DESCENDING = 0;

const int bitonic_CHECKSUM = 55;

/*
  Initialization- and return-value-related functions
*/

void bitonic_init( void )
{
  /** Initialize array "a" with data **/
  int i;

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    bitonic_a[ i ] = ( 32 - i );
}


int bitonic_return( void )
{
  int checksum = 0;

  checksum += bitonic_a[ 0 ] + bitonic_a[ 21 ] + bitonic_a[ 31 ];

  return ( ( checksum == bitonic_CHECKSUM ) ? 0 : -1 );
}


/*
  Algorithm core functions
*/

/** A comparator is modelled by the procedure compare, where the
   parameter dir indicates the sorting direction. If dir is ASCENDING
   and a[i] > a[j] is true or dir is DESCENDING and a[i] > a[j] is
   false then a[i] and a[j] are interchanged.
 **/
void bitonic_compare( int i, int j, int dir )
{
  if ( dir == ( bitonic_a[ i ] > bitonic_a[ j ] ) ) {
    int h = bitonic_a[ i ];
    bitonic_a[ i ] = bitonic_a[ j ];
    bitonic_a[ j ] = h;
  }
}


/** The procedure bitonicMerge recursively sorts a bitonic sequence in
   ascending order, if dir = ASCENDING, and in descending order
   otherwise. The sequence to be sorted starts at index position lo,
   the number of elements is cnt.
 **/
void bitonic_merge( int lo, int cnt, int dir )
{
  int k = cnt / 2;
  int i;
  _Pragma( "loopbound min 0 max 16" )
  for ( i = lo; i < lo + k; i++ )
    bitonic_compare( i, i + k, dir );

  if ( k > 1 ) {
    bitonic_merge( lo, k, dir );
    bitonic_merge( lo + k, k, dir );
  }
}


/** Procedure bitonicSort first produces a bitonic sequence by
   recursively sorting its two halves in opposite directions, and then
   calls bitonicMerge.
 **/
void bitonic_sort( int lo, int cnt, int dir )
{
  int k = cnt;
  k /= 2;
  _Pragma( "marker recMerge" )

  if ( cnt > 1 ) {
    bitonic_sort( lo, k, bitonic_ASCENDING );
    bitonic_sort( lo + k, k, bitonic_DESCENDING );
  }

  bitonic_merge( lo, cnt, dir );
  _Pragma( "flowrestriction 1*bitonicMerge <= 31*recMerge" )

  return;
}


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) bitonic_main( void )
{
  int i;

  /** When called with parameters lo = 0, cnt = a.length() and dir =
    ASCENDING, procedure bitonicSort sorts the whole array a. **/
  _Pragma( "marker recSort" )
  bitonic_sort( 0, 32, bitonic_ASCENDING );
  _Pragma( "flowrestriction 1*bitonicSort <= 63*recSort" )

  /** Loop through array, printing out each element **/
  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ ) {
  }
}


int main( void )
{
  bitonic_init();
  bitonic_main();

  return ( bitonic_return() );
}
