/*

    This program is part of the TACLeBench benchmark suite.
    Version V 1.x

    Name: minver

    Author: Sung-Soo Lim

    Function: Matrix inversion for 3x3 floating point matrix.

    Source: Turbo C Programming for Engineering by Hyun Soo Ahn

    Changes: a brief summary of major functional changes (not formatting)

    License: general open-source

*/


/*
    Forward declaration of functions
*/

int minver_minver( int row, int col, double eps );
int minver_mmul( int row_a, int col_a, int row_b, int col_b );
double minver_fabs( double n );
void minver_init();
int minver_return();
void minver_main();
int main( void );

/*
    Declaration of global variables
*/

static double  minver_a[ 3 ][ 3 ] = {
  3.0, -6.0,  7.0,
  9.0,  0.0, -5.0,
  5.0, -8.0,  6.0,
};
double minver_b[ 3 ][ 3 ];
double minver_c[ 3 ][ 3 ];
double minver_aa[ 3 ][ 3 ];
double minver_a_i[ 3 ][ 3 ];
double minver_e[ 3 ][ 3 ];
double minver_det;


/*
    Initialization- and return-value-related functions
*/

void minver_init()
{

}


int minver_return()
{
  return 0;
}


/*
    Arithmetic math functions
*/


double minver_fabs( double n )
{
  double f;

  if ( n >= 0 )
    f = n;
  else
    f = -n;
  return f;
}


int  minver_mmul( int row_a, int col_a, int row_b, int col_b )
{
  int i, j, k, row_c, col_c;
  double w;

  row_c = row_a;
  col_c = col_b;

  if ( row_c < 1 || row_b < 1 || col_c < 1 || col_a != row_b )
    return ( 999 );

  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < row_c; i++ ) {
    _Pragma( "loopbound min 3 max 3" )
    for ( j = 0; j < col_c; j++ ) {
      w = 0.0;
      _Pragma( "loopbound min 3 max 3" )
      for ( k = 0; k < row_b; k++ )
        w += minver_a[ i ][ k ] * minver_b[ k ][ j ];

      minver_c[ i ][ j ] = w;

    }
  }
  return ( 0 );

}


int minver_minver( int row, int col, double eps )
{

  int work[ 500 ], i, j, k, r, iw, s, t, u, v;
  double w, wmax, pivot, api, w1;

  if ( row < 2 || row > 500 || eps <= 0.0 )
    return ( 999 );
  w1 = 1.0;
  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < row; i++ )
    work[ i ] = i;
  _Pragma( "loopbound min 3 max 3" )
  for ( k = 0; k < row; k++ ) {
    wmax = 0.0;
    _Pragma( "loopbound min 1 max 3" )
    for ( i = k; i < row; i++ ) {
      w = minver_fabs( minver_a[ i ][ k ] );
      if ( w > wmax ) {
        wmax = w;
        r = i;
      }
    }
    pivot = minver_a[ r ][ k ];
    api = minver_fabs( pivot );
    if ( api <= eps ) {
      minver_det = w1;
      return ( 1 );
    }
    w1 *= pivot;
    u = k * col;
    v = r * col;
    if ( r != k ) {
      w1 = -w;
      iw = work[ k ];
      work[ k ] = work[ r ];
      work[ r ] = iw;
      _Pragma( "loopbound min 3 max 3" )
      for ( j = 0; j < row; j++ ) {
        s = u + j;
        t = v + j;
        w = minver_a[ k ][ j ];
        minver_a[ k ][ j ] = minver_a[ r ][ j ];
        minver_a[ r ][ j ] = w;
      }
    }
    _Pragma( "loopbound min 3 max 3" )
    for ( i = 0; i < row; i++ )
      minver_a[ k ][ i ] /= pivot;
    _Pragma( "loopbound min 3 max 3" )
    for ( i = 0; i < row; i++ ) {
      if ( i != k ) {
        v = i * col;
        s = v + k;
        w = minver_a[ i ][ k ];
        if ( w != 0.0 ) {
          _Pragma( "loopbound min 3 max 3" )
          for ( j = 0; j < row; j++ ) {
            if ( j != k ) minver_a[ i ][ j ] -= w * minver_a[ k ][ j ];
          }
          minver_a[ i ][ k ] = -w / pivot;

        }
      }
    }
    minver_a[ k ][ k ] = 1.0 / pivot;
  }
  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < row; ) {
    /*  The following redundant statement is inserted due to limitations of
        WCC's flow fact manager. It is required in order to have the flow
        fact pragma below uniquely attached to the while(1) loop.
    */
    i = i;
    _Pragma( "loopbound min 1 max 3" )
    while ( 1 ) {
      k = work[ i ];
      if ( k == i ) break;
      iw = work[ k ];
      work[ k ] = work[ i ];
      work[ i ] = iw;
      _Pragma( "loopbound min 3 max 3" )
      for ( j = 0; j < row; j++ ) {
        u = j * col;
        s = u + i;
        t = u + k;
        w = minver_a [k ][ i ];
        minver_a[ k ][ i ] = minver_a[ k ][ k ];
        minver_a[ k ][ k ] = w;
      }
    }
    i++;
  }
  minver_det = w1;
  return ( 0 );

}


/*
    Main functions
*/


void _Pragma( "entrypoint" ) minver_main()
{
  int i, j;
  double eps;
  eps = 1.0e-6;
  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < 3; i++ ) {
    _Pragma( "loopbound min 3 max 3" )
    for ( j = 0; j < 3; j++ )
      minver_aa[ i ][ j ] = minver_a[ i ][ j ];
  }

  minver_minver( 3, 3, eps );
  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < 3; i++ ) {
    _Pragma( "loopbound min 3 max 3" )
    for ( j = 0; j < 3; j++ )
      minver_a_i[ i ][ j ] = minver_a[ i ][ j ];
  }

  minver_mmul( 3, 3, 3, 3 );
}


int main( void )
{
  minver_init();
  minver_main();

  return ( minver_return() );
}





