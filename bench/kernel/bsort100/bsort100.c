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


#define KNOWN_VALUE 1

#define WORSTCASE 1
#define FALSE 0
#define TRUE 1
#define NUMELEMS 100
#define MAXDIM   (NUMELEMS+1)


static int bsort_Array[MAXDIM];
static int factor;

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
  int  Index, fact;

  #ifdef WORSTCASE
  factor = -1;
  #else
  factor = 1;
  #endif

  fact = factor;

  _Pragma( "loopbound min 100 max 100" )
  for ( Index = 1; Index <= NUMELEMS; Index++ )
    Array[Index] = Index * fact * KNOWN_VALUE;

  return 0;
}



int BubbleSort( int Array[] )
/*
   Sorts an array of integers of size NUMELEMS in ascending order.
*/
{
  int Sorted = FALSE;
  int Temp, Index, i;

  _Pragma( "loopbound min 99 max 99" )
  for ( i = 1; i <= NUMELEMS - 1; i++ ) {
    Sorted = TRUE;
    _Pragma( "loopbound min 2 max 99" )
    for ( Index = 1; Index <= NUMELEMS - 1; Index ++ ) {
      if ( Index > NUMELEMS - i )
        break;
      if ( Array[Index] > Array[Index + 1] ) {
        Temp = Array[Index];
        Array[Index] = Array[Index + 1];
        Array[Index + 1] = Temp;
        Sorted = FALSE;
      }
    }

    if ( Sorted )
      break;
  }

  return 0;
}

