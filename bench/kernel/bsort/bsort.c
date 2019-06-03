/*

  This program is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: bsort

  Author: unknown

  Function: A program for testing the basic loop constructs,
            integer comparisons, and simple array handling by
            sorting 100 integers

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/bsort100/bsort100.c

  Original name: bsort100

  Changes: See ChangeLog.txt

  License: May be used, modified, and re-distributed freely.

*/


/*
  Forward declaration of functions
*/

void bsort_init( void );
void bsort_main( void );
int bsort_return( void );
int bsort_Initialize( int Array[] );
int bsort_BubbleSort( int Array[] );


/*
  Declaration of global variables
*/

#define bsort_SIZE 100

static int bsort_Array[ bsort_SIZE ];


/*
  Initialization- and return-value-related functions
*/

/* Initializes given array with randomly generated integers. */
int bsort_Initialize( int Array[] )
{
  int Index;

  _Pragma( "loopbound min 100 max 100" )
  for ( Index = 0; Index < bsort_SIZE; Index ++ )
    Array[ Index ] = ( Index + 1 ) * -1;

  return 0;
}


void bsort_init( void )
{
  bsort_Initialize( bsort_Array );
}


int bsort_return( void )
{
  int Sorted = 1;
  int Index;

  _Pragma( "loopbound min 99 max 99" )
  for ( Index = 0; Index < bsort_SIZE - 1; Index ++ )
    Sorted = Sorted && ( bsort_Array[ Index ] < bsort_Array[ Index + 1 ] );

  return 1 - Sorted;
}


/*
  Core benchmark functions
*/

/* Sorts an array of integers of size bsort_SIZE in ascending
   order with bubble sort. */
int bsort_BubbleSort( int Array[] )
{
  int Sorted = 0;
  int Temp, Index, i;

  _Pragma( "loopbound min 99 max 99" )
  for ( i = 0; i < bsort_SIZE - 1; i ++ ) {
    Sorted = 1;
    _Pragma( "loopbound min 3 max 99" )
    for ( Index = 0; Index < bsort_SIZE - 1; Index ++ ) {
      if ( Index > bsort_SIZE - i )
        break;
      if ( Array[ Index ] > Array[Index + 1] ) {
        Temp = Array[ Index ];
        Array[ Index ] = Array[ Index + 1 ];
        Array[ Index + 1 ] = Temp;
        Sorted = 0;
      }
    }

    if ( Sorted )
      break;
  }

  return 0;
}


void _Pragma( "entrypoint" ) bsort_main( void )
{
  bsort_BubbleSort( bsort_Array );
}


/*
  Main function
*/

int main( void )
{
  bsort_init();
  bsort_main();

  return bsort_return();
}
