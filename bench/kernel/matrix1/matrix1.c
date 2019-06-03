/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: matrix1

  Author: Juan Martinez Velarde

  Function: Generic matrix - multiply benchmarking

    This program performs a matrix multiplication of the form C=AB,
    where A and B are two dimensional matrices of arbitrary dimension.
    The only restriction os that the inner dimension of the arrays must
    be greater than 1.

      A[ X x Y ] * B[ Y x Z ] = C[ X x Z ]

                       |a11     a12     ..      a1y|
                       |a21     a22     ..      a2y|
      matrix A[ X x Y ]= |..      ..      ..     ..  |
                       |a(x-1)1 a(x-1)2 ..  a(x-1)y|
                       |ax1     ax2     ..      axy|


                       |b11     b12     ..     b1z|
                       |b21     b22     ..     b2z|
      matrix B[ Y x Z ]= |..      ..      ..     .. |
                       |b(y-1)1 b(y-1)2 .. b(y-1)z|
                       |by1     by2     ..     byz|

                       |c11     c12     ..     c1z|
                       |c21     c22     ..     c2z|
      matrix C[ X x Z ]= |..      ..      ..     .. |
                       |c(x-1)1 c(x-1)2 .. c(x-1)z|
                       |cx1     cx2     ..     cxz|

      matrix elements are stored as

      A[ X x Y ] = { a11, a12, .. , a1y,
                   a21, a22, .. , a2y,
                   ...,
                   ax1, ax2, .. , axy}

      B[ Y x Z ] = { b11, b21, .., b(y-1)1, by1, b12, b22, .. , b(y-1)z, byz }

      C[ X x Z ] = { c11, c21, .. , c(x-1)1, cx1, c12, c22, .. ,c(x-1)z, cxz }

  Source: DSP-Stone
          http://www.ice.rwth-aachen.de/research/tools-projects/entry/detail/dspstone

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

*/


/*
  Macro definitions
*/

#define X 10 /* first dimension of array A */
#define Y 10 /* second dimension of array A, first dimension of array B */
#define Z 10 /* second dimension of array B */


/*
  Forward declaration of functions
*/

void matrix1_pin_down( int A[  ], int B[  ], int C[  ] );
void matrix1_init( void );
void matrix1_main( void );
int main( void );


/*
  Declaration of global variables
*/

int matrix1_A[ X * Y ];
int matrix1_B[ Y * Z ];
int matrix1_C[ X * Z ];


/*
  Initialization functions
*/

void matrix1_pin_down( int A[  ], int B[  ], int C[  ] )
{
  int i;
  volatile int x = 1;

  _Pragma( "loopbound min 100 max 100" )
  for ( i = 0 ; i < X * Y; i++ )
    A[ i ] = x ;

  _Pragma( "loopbound min 100 max 100" )
  for ( i = 0 ; i < Y * Z ; i++ )
    B[ i ] = x ;

  _Pragma( "loopbound min 100 max 100" )
  for ( i = 0 ; i < X * Z ; i++ )
    C[ i ] = 0 ;
}


void matrix1_init( void )
{
  matrix1_pin_down( &matrix1_A[ 0 ], &matrix1_B[ 0 ], &matrix1_C[ 0 ] );
}

/*
  Return function
*/

int matrix1_return( void )
{
  int i;
  int checksum = 0;

  _Pragma( "loopbound min 100 max 100" )
  for ( i = 0; i < X * Z; i++ )
    checksum += matrix1_C[ i ];

  return ( checksum ==  1000 ? 0 : -1 );
}


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) matrix1_main( void )
{
  register int *p_a = &matrix1_A[ 0 ];
  register int *p_b = &matrix1_B[ 0 ];
  register int *p_c = &matrix1_C[ 0 ];

  register int f, i, k;

  _Pragma( "loopbound min 10 max 10" )
  for ( k = 0; k < Z; k++ ) {
    p_a = &matrix1_A[ 0 ];                /* point to the beginning of array A */

    _Pragma( "loopbound min 10 max 10" )
    for ( i = 0; i < X; i++ ) {
      p_b = &matrix1_B[ k * Y ];          /* take next column */

      *p_c = 0;
      _Pragma( "loopbound min 10 max 10" )
      for ( f = 0; f < Y; f++ ) /* do multiply */
        *p_c += *p_a++ * *p_b++;

      p_c++;
    }
  }
}


int main( void )
{
  matrix1_init();
  matrix1_main();

  return matrix1_return();
}
