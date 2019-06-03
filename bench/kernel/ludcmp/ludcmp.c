/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: ludcmp

  Author: Sung-Soo Lim

  Function: Simultaneous linear equations by LU decomposition.

  Source: SNU-RT Benchmark Suite, via MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/ludcmp/ludcmp.c

  Changes: Moved initialization into separate function.

  License: May be used, modified, and re-distributed freely, but
           the SNU-RT Benchmark Suite must be acknowledged

*/

/*
  This program is derived from the SNU-RT Benchmark Suite for Worst
  Case Timing Analysis by Sung-Soo Lim

  III-4. ludcmp.c : Simultaneous Linear Equations by LU Decomposition
                    (from the book C Programming for EEs by Hyun Soon Ahn)
*/

/*
  Forward declaration of functions
*/

void ludcmp_init( void );
int ludcmp_return( void );
int  ludcmp_test( int n, double eps );
void ludcmp_main( void );
int main( void );

double ludcmp_a[ 50 ][ 50 ], ludcmp_b[ 50 ], ludcmp_x[ 50 ];
int ludcmp_chkerr;

void ludcmp_init( void )
{
  int             i, j, n = 5;
  double          w;
  volatile int    x = 0;

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i <= n; i++ ) {
    w = 0;
    _Pragma( "loopbound min 6 max 6" )
    for ( j = 0; j <= n; j++ ) {
      ludcmp_a[ i ][ j ] = ( i + 1 ) + ( j + 1 );

      if ( i == j )
        ludcmp_a[ i ][ j ] *= 10;
      w += ludcmp_a[ i ][ j ];

      if ( x )
        ludcmp_a[ i ][ j ] += x;
    }

    ludcmp_b[ i ] = w;
    if ( x )
      ludcmp_b[ i ] += x;
  }
}

int ludcmp_return( void )
{
  int i, n = 5;
  double checksum = ludcmp_chkerr;

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i <= n; i++ )
    checksum += ludcmp_x[ i ];

  /* allow rounding errors for the checksum */
  checksum -= 6.0;
  return ( ( checksum < 0.000001 && checksum > -0.000001 ) ? 0 : -1 );
}

double ludcmp_fabs( double n )
{
  double          f;

  if ( n >= 0 )
    f = n;
  else
    f = -n;

  return f;
}

int ludcmp_test( int n, double eps )
{
  int             i, j, k;
  double          w, y[ 100 ];


  if ( n > 99 || eps <= 0 )
    return ( 999 );

  _Pragma( "loopbound min 5 max 5" )
  for ( i = 0; i < n; i++ ) {
    if ( ludcmp_fabs( ludcmp_a[ i ][ i ] ) <= eps )
      return ( 1 );

    _Pragma( "loopbound min 1 max 5" )
    for ( j = i + 1; j <= n; j++ ) {
      w = ludcmp_a[ j ][ i ];

      if ( i != 0 ) {
        _Pragma( "loopbound min 1 max 4" )
        for ( k = 0; k < i; k++ )
          w -= ludcmp_a[ j ][ k ] * ludcmp_a[ k ][ i ];
      }

      ludcmp_a[ j ][ i ] = w / ludcmp_a[ i ][ i ];
    }

    _Pragma( "loopbound min 1 max 5" )
    for ( j = i + 1; j <= n; j++ ) {
      w = ludcmp_a[ i + 1 ][ j ];

      _Pragma( "loopbound min 1 max 5" )
      for ( k = 0; k <= i; k++ )
        w -= ludcmp_a[ i + 1 ][ k ] * ludcmp_a[ k ][ j ];

      ludcmp_a[ i + 1 ][ j ] = w;
    }
  }

  y[ 0 ] = ludcmp_b[ 0 ];

  _Pragma( "loopbound min 5 max 5" )
  for ( i = 1; i <= n; i++ ) {
    w = ludcmp_b[ i ];

    _Pragma( "loopbound min 1 max 5" )
    for ( j = 0; j < i; j++ )
      w -= ludcmp_a[ i ][ j ] * y[ j ];

    y[ i ] = w;
  }

  ludcmp_x[ n ] = y[ n ] / ludcmp_a[ n ][ n ];

  _Pragma( "loopbound min 5 max 5" )
  for ( i = n - 1; i >= 0; i-- ) {
    w = y[ i ];

    _Pragma( "loopbound min 1 max 5" )
    for ( j = i + 1; j <= n; j++ )
      w -= ludcmp_a[ i ][ j ] * ludcmp_x[ j ];

    ludcmp_x[ i ] = w / ludcmp_a[ i ][ i ];
  }

  return ( 0 );
}

void _Pragma( "entrypoint" ) ludcmp_main( void )
{
  int n = 5;
  double eps = 1;
  ludcmp_chkerr = ludcmp_test( n, eps );
}

int main( void )
{
  ludcmp_init();
  ludcmp_main();

  return ( ludcmp_return() );
}
