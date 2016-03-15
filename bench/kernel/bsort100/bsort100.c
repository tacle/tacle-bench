/*

  This program is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: bsort

  Author: unknown

  Function: A program for testing the basic loop constructs, integer
            comparisons, and simple array handling by sorting 100 integers

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/bsort100/bsort100.c

  Original name: bsort100

  Changes: See ChangeLog.txt

  License: unknown

*/


#define bsort_SIZE 100


static int bsort_Array[bsort_SIZE];

/* Forward function prototypes */
int Initialize( int Array[] );
int BubbleSort( int Array[] );


int main( void )
{
  Initialize( bsort_Array );

  BubbleSort( bsort_Array );

  return 0;
}


int Initialize( int Array[] )
/*
   Initializes given array with randomly generated integers.
*/
{
  int  Index;

  _Pragma( "loopbound min 100 max 100" )
  for ( Index = 0; Index < bsort_SIZE; Index++ )
    Array[Index] = ( Index + 1 ) * -1;

  return 0;
}



int BubbleSort( int Array[] )
/*
   Sorts an array of integers of size NUMELEMS in ascending order.
*/
{
  int Sorted = 0;
  int Temp, Index, i;

  _Pragma( "loopbound min 99 max 99" )
  for ( i = 0; i < bsort_SIZE - 1; i++ ) {
    Sorted = 1;
    _Pragma( "loopbound min 2 max 99" )
    for ( Index = 0; Index < bsort_SIZE - 1; Index ++ ) {
      if ( Index > bsort_SIZE - i )
        break;
      if ( Array[Index] > Array[Index + 1] ) {
        Temp = Array[Index];
        Array[Index] = Array[Index + 1];
        Array[Index + 1] = Temp;
        Sorted = 0;
      }
    }

    if ( Sorted )
      break;
  }

  return 0;
}

