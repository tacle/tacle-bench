/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: duff

  Author: Jakob Engblom

  Function: Duff's device

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/duff/duff.c

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

*/


/*
  Forward declaration of functions
*/

void duff_copy( char *to, char *from, int count );
void duff_initialize( char *arr, int length );
void duff_init();
void duff_main( void );
int duff_return( void );
int main( void );


/*
  Declaration of global variables
*/

char duff_source[ 100 ];
char duff_target[ 100 ];


/*
  Initialization- and return-value-related functions
*/

void duff_init()
{
  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;

  duff_initialize( duff_source, 100 );

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = ( unsigned char * ) &duff_source[  0  ];
  _Pragma( "loopbound min 400 max 400" )
  for ( i = 0; i < sizeof( duff_source ); ++i, ++p )
    *p ^= bitmask;
}


int duff_return( void )
{
  return ( duff_target[ 28 ] - 72 != 0 );
}


/*
  Algorithm core functions
*/

void duff_initialize( char *arr, int length )
{
  int i;

  _Pragma( "loopbound min 100 max 100" )
  for ( i = 0; i < length; i++ )
    arr[ i ] = length - i;
}


void duff_copy( char *to, char *from, int count )
{
  int n = ( count + 7 ) / 8;

  _Pragma( "marker outside" )
  switch ( count % 8 ) {
    case 0:
      do {
        *to++ = *from++;
      case 7:
        *to++ = *from++;
      case 6:
        *to++ = *from++;
      case 5:
        *to++ = *from++;
      case 4:
        *to++ = *from++;
      case 3:
        *to++ = *from++;
      case 2:
        *to++ = *from++;
      case 1:
        _Pragma( "marker inside" )
        *to++ = *from++;


      } while ( --n > 0 );
  }
  _Pragma( "flowrestriction 1*inside <= 6*outside" )
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) duff_main( void )
{
  duff_copy( duff_target, duff_source, 43 );
}


int main( void )
{
  duff_init();
  duff_main();

  return ( duff_return() );
}

