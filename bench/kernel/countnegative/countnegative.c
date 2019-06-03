/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: countnegative

  Author: unknown

  Function: Counts negative and non-negative numbers in a
    matrix. Features nested loops, well-structured code.

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/cnt/cnt.c

  Changes: Changed split between initialization and computation

  License: May be used, modified, and re-distributed freely

*/

/*
  The dimension of the matrix
*/
#define MAXSIZE 20

/*
  Type definition for the matrix
*/
typedef int matrix [ MAXSIZE ][ MAXSIZE ];

/*
  Forward declaration of functions
*/
void countnegative_initSeed( void );
int countnegative_randomInteger( void );
void countnegative_initialize( matrix );
void countnegative_init( void );
int countnegative_return( void );
void countnegative_sum( matrix );
void countnegative_main( void );
int main( void );

/*
  Globals
*/
volatile int countnegative_seed;
matrix countnegative_array;
int countnegative_postotal, countnegative_negtotal;
int countnegative_poscnt, countnegative_negcnt;

/*
  Initializes the seed used in the random number generator.
*/
void countnegative_initSeed ( void )
{
  countnegative_seed = 0;
}

/*
  Generates random integers between 0 and 8094
*/
int countnegative_randomInteger( void )
{
  countnegative_seed = ( ( countnegative_seed * 133 ) + 81 ) % 8095;
  return  countnegative_seed;
}

/*
  Initializes the given array with random integers.
*/
void countnegative_initialize( matrix Array )
{
  register int OuterIndex, InnerIndex;

  _Pragma( "loopbound min 20 max 20" )
  for ( OuterIndex = 0; OuterIndex < MAXSIZE; OuterIndex++ )
    _Pragma( "loopbound min 20 max 20" )
    for ( InnerIndex = 0; InnerIndex < MAXSIZE; InnerIndex++ )
      Array[ OuterIndex ][ InnerIndex ] =  countnegative_randomInteger();
}

void countnegative_init( void )
{
  countnegative_initSeed();
  countnegative_initialize( countnegative_array );
}

int countnegative_return( void )
{
  int checksum = ( countnegative_postotal +
                   countnegative_poscnt +
                   countnegative_negtotal +
                   countnegative_negcnt );

  return ( ( checksum == ( int )0x1778de ) ? 0 : -1 );
}

void countnegative_sum( matrix Array )
{
  register int Outer, Inner;

  int Ptotal = 0; /* changed these to locals in order to drive worst case */
  int Ntotal = 0;
  int Pcnt = 0;
  int Ncnt = 0;

  _Pragma( "loopbound min 20 max 20" )
  for ( Outer = 0; Outer < MAXSIZE; Outer++ )
    _Pragma( "loopbound min 20 max 20" )
    for ( Inner = 0; Inner < MAXSIZE; Inner++ )
      if ( Array[ Outer ][ Inner ] >= 0 ) {
        Ptotal += Array[ Outer ][ Inner ];
        Pcnt++;
      } else {
        Ntotal += Array[ Outer ][ Inner ];
        Ncnt++;
      }

  countnegative_postotal = Ptotal;
  countnegative_poscnt = Pcnt;
  countnegative_negtotal = Ntotal;
  countnegative_negcnt = Ncnt;
}

/*
  The main function
*/
void _Pragma( "entrypoint" ) countnegative_main ( void )
{
  countnegative_sum(  countnegative_array );
}

int main( void )
{
  countnegative_init();
  countnegative_main();

  return ( countnegative_return() );
}
