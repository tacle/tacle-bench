/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name:

  Author: William H. Press, Saul A. Teukolsky, William T. Vetterling,
          Brian P. Flannery, "Numerical Recipes in C"

  Function: A function to select the Nth largest number in the floating
            point array arr[].
            The parameters to function select are k and n. Then the function
            selects k-th largest number out of n original numbers.

            The original algorithm was adjusted for inclusion in the
            SNU-RT Benchmark Suite:

            < Features > - restrictions for our experimental environment

            1. Completely structured.
              - There are no unconditional jumps.
              - There are no exit from loop bodies.
                (There are no 'break' or 'return' in loop bodies)
            2. No 'switch' statements.
            3. No 'do..while' statements.
            4. Expressions are restricted.
              - There are no multiple expressions joined by 'or',
                'and' operations.
            5. No library calls.
              - All the functions needed are implemented in the
                source file.

  Source: SNU-RT Benchmark Suite for Worst Case Timing Analysis
          Collected and Modified by S.-S. Lim
          Real-Time Research Group
          Seoul National University

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely, but
           the SNU-RT Benchmark Suite must be acknowledged

*/

/*
  This program is derived from the SNU-RT Benchmark Suite for Worst
  Case Timing Analysis by Sung-Soo Lim
*/

/*
  Forward declaration of functions
*/
void select_init( void );
int select_return( void );
int select_main( void );
int main( void );


/*
  Global variables
*/

float select_selected = 0.0;
float select_arr[ 20 ] = {
  5.0f, 4.0f, 10.3f, 1.1f, 5.7f, 100.0f, 231.0f, 111.0f, 49.5f, 99.0f, 10.0f,
  150.0f, 222.22f, 101.0f, 77.0f, 44.0f, 35.0f, 20.54f, 99.99f, 888.88f };

#define SWAP(a,b) do { temp = (a); (a) = (b); (b) = temp; } while (0)


/*
  Initialization- and return-value-related functions
*/

void select_init( void )
{
  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = (unsigned char *) &select_arr[ 0 ];
  _Pragma( "loopbound min 80 max 80" )
  for ( i = 0; i < sizeof( select_arr ); ++i, ++p )
    *p ^= bitmask;
}


int select_return( void )
{
  return( (int) select_selected );
}


/*
  Algorithm core functions
*/

static float select_select( unsigned long k, unsigned long n )
{
  unsigned long i, ir, j, l, mid;
  float a, temp;
  int flag, flag2;

  l = 0;
  ir = n - 1;
  flag = flag2 = 0;
  _Pragma( "loopbound min 1 max 20" )
  while ( !flag ) {
    if ( ir <= l + 1 ) {
      if ( ir == l + 1 )
        if ( select_arr[ ir ] < select_arr[ l ] ) {
          SWAP( select_arr[ l ], select_arr[ ir ] );
        }
      flag = 1;
    } else {
      if ( !flag ) {
        mid = ( l + ir ) >> 1;
        SWAP( select_arr[ mid ], select_arr[ l + 1 ] );
        if ( select_arr[ l ] > select_arr[ ir ] ) {
          SWAP( select_arr[ l ], select_arr[ ir ] );
        }
        if ( select_arr[ l + 1 ] > select_arr[ ir ] ) {
          SWAP( select_arr[ l + 1 ], select_arr[ ir ] );
        }
        if ( select_arr[ l ] > select_arr[ l + 1 ] ) {
          SWAP( select_arr[ l ], select_arr[ l + 1 ] );
        }
        i = l + 1;
        j = ir;
        a = select_arr[ l + 1 ];
        _Pragma( "loopbound min 1 max 9" )
        flag2 = 0;
        while ( !flag2 ) {
          i++;
          _Pragma( "loopbound min 0 max 18" )
          while ( select_arr[ i ] < a ) i++;
          j--;
          _Pragma( "loopbound min 0 max 18" )
          while ( select_arr[ j ] > a ) j--;
          if ( j < i ) flag2 = 1;
          if ( !flag2 ) {
            SWAP( select_arr[ i ], select_arr[ j ] );
          }
        }

        select_arr[ l + 1 ] = select_arr[ j ];
        select_arr[ j ] = a;

        if ( j >= k )
          ir = j - 1;
        if ( j <= k )
          l = i;
      }
    }
  }

  return( select_arr[ k ] );
}


/*
  Main functions
*/

int _Pragma ( "entrypoint" ) select_main( void )
{
  select_selected = select_select( 7, 20 );
  return( 0 );
}


int main( void )
{
  select_init();
  select_main();

  return( select_return() - 35 != 0 );
}

