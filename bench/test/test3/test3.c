/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: test3

  Author: Rathijit Sen

  Function: This testcase walks through parts of 8 arrays in a 10 x 10 grid of
    functions. function f_i_j calls f_i_j+1 and f_i+1,j except at the grid
    boundaries.

  Source: Universitaet des Saarlandes, Saarbruecken, Germany
          Compiler Research Group

  Original name: test3

  Changes:
          26-10-2007 Creation at Saarbruecken

  License: GPL

*/


/*
  Forward declaration of functions
*/

void test3_initSeed();
int test3_randomInteger( void );
void test3_init( void );
int test3_return( void );
void test3_func_10_10( void );
void test3_func_9_10( void );
void test3_func_8_10( void );
void test3_func_7_10( void );
void test3_func_6_10( void );
void test3_func_5_10( void );
void test3_func_4_10( void );
void test3_func_3_10( void );
void test3_func_2_10( void );
void test3_func_1_10( void );
void test3_func_0_10( void );
void test3_func_10_9( void );
void test3_func_9_9( void );
void test3_func_8_9( void );
void test3_func_7_9( void );
void test3_func_6_9( void );
void test3_func_5_9( void );
void test3_func_4_9( void );
void test3_func_3_9( void );
void test3_func_2_9( void );
void test3_func_1_9( void );
void test3_func_0_9( void );
void test3_func_10_8( void );
void test3_func_9_8( void );
void test3_func_8_8( void );
void test3_func_7_8( void );
void test3_func_6_8( void );
void test3_func_5_8( void );
void test3_func_4_8( void );
void test3_func_3_8( void );
void test3_func_2_8( void );
void test3_func_1_8( void );
void test3_func_0_8( void );
void test3_func_10_7( void );
void test3_func_9_7( void );
void test3_func_8_7( void );
void test3_func_7_7( void );
void test3_func_6_7( void );
void test3_func_5_7( void );
void test3_func_4_7( void );
void test3_func_3_7( void );
void test3_func_2_7( void );
void test3_func_1_7( void );
void test3_func_0_7( void );
void test3_func_10_6( void );
void test3_func_9_6( void );
void test3_func_8_6( void );
void test3_func_7_6( void );
void test3_func_6_6( void );
void test3_func_5_6( void );
void test3_func_4_6( void );
void test3_func_3_6( void );
void test3_func_2_6( void );
void test3_func_1_6( void );
void test3_func_0_6( void );
void test3_func_10_5( void );
void test3_func_9_5( void );
void test3_func_8_5( void );
void test3_func_7_5( void );
void test3_func_6_5( void );
void test3_func_5_5( void );
void test3_func_4_5( void );
void test3_func_3_5( void );
void test3_func_2_5( void );
void test3_func_1_5( void );
void test3_func_0_5( void );
void test3_func_10_4( void );
void test3_func_9_4( void );
void test3_func_8_4( void );
void test3_func_7_4( void );
void test3_func_6_4( void );
void test3_func_5_4( void );
void test3_func_4_4( void );
void test3_func_3_4( void );
void test3_func_2_4( void );
void test3_func_1_4( void );
void test3_func_0_4( void );
void test3_func_10_3( void );
void test3_func_9_3( void );
void test3_func_8_3( void );
void test3_func_7_3( void );
void test3_func_6_3( void );
void test3_func_5_3( void );
void test3_func_4_3( void );
void test3_func_3_3( void );
void test3_func_2_3( void );
void test3_func_1_3( void );
void test3_func_0_3( void );
void test3_func_10_2( void );
void test3_func_9_2( void );
void test3_func_8_2( void );
void test3_func_7_2( void );
void test3_func_6_2( void );
void test3_func_5_2( void );
void test3_func_4_2( void );
void test3_func_3_2( void );
void test3_func_2_2( void );
void test3_func_1_2( void );
void test3_func_0_2( void );
void test3_func_10_1( void );
void test3_func_9_1( void );
void test3_func_8_1( void );
void test3_func_7_1( void );
void test3_func_6_1( void );
void test3_func_5_1( void );
void test3_func_4_1( void );
void test3_func_3_1( void );
void test3_func_2_1( void );
void test3_func_1_1( void );
void test3_func_0_1( void );
void test3_func_10_0( void );
void test3_func_9_0( void );
void test3_func_8_0( void );
void test3_func_7_0( void );
void test3_func_6_0( void );
void test3_func_5_0( void );
void test3_func_4_0( void );
void test3_func_3_0( void );
void test3_func_2_0( void );
void test3_func_1_0( void );
void test3_func_0_0( void );
void test3_main( void );
int main( void );


/*
  Declaration of global variables
*/

volatile int test3_seed;
int test3_array1[ 32 ][ 32 ];
int test3_array2[ 32 ][ 32 ];
int test3_array3[ 32 ][ 32 ];
int test3_array4[ 32 ][ 32 ];
int test3_array5[ 32 ][ 32 ];
int test3_array6[ 32 ][ 32 ];
int test3_array7[ 32 ][ 32 ];
int test3_array8[ 32 ][ 32 ];
int test3_result;


/*
  Initialization- and return-value-related functions
*/

/*
  test3_initSeed initializes the seed used in the "random" number generator.
*/
void test3_initSeed()
{
  test3_seed = 0;
}


/*
  test3_RandomInteger generates random integers between 0 and 8094.
*/
int test3_randomInteger()
{
  test3_seed = ( ( test3_seed * 133 ) + 81 ) % 8095;
  return ( test3_seed );
}


void test3_init( void )
{
  int i, j;


  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array1[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array2[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array3[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array4[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array5[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array6[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array7[ i ][ j ] = test3_randomInteger();

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    _Pragma( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      test3_array8[ i ][ j ] = test3_randomInteger();

  test3_result = 0;
}


int test3_return( void )
{
  return ( test3_result );
}


/*
  Algorithm core functions
*/

void test3_func_10_10( void )
{
  int i, x;
  int *p = &test3_array5[ 10 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_10( void )
{
  int i, x;
  int *p = &test3_array4[ 9 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_10();
}


void test3_func_8_10( void )
{
  int i, x;
  int *p = &test3_array3[ 8 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_10();
}


void test3_func_7_10( void )
{
  int i, x;
  int *p = &test3_array2[ 7 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_10();
}


void test3_func_6_10( void )
{
  int i, x;
  int *p = &test3_array1[ 6 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_10();
}


void test3_func_5_10( void )
{
  int i, x;
  int *p = &test3_array8[ 5 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_10();
}


void test3_func_4_10( void )
{
  int i, x;
  int *p = &test3_array7[ 4 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_10();
}


void test3_func_3_10( void )
{
  int i, x;
  int *p = &test3_array6[ 3 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_10();
}


void test3_func_2_10( void )
{
  int i, x;
  int *p = &test3_array5[ 2 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_10();
}


void test3_func_1_10( void )
{
  int i, x;
  int *p = &test3_array4[ 1 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_10();
}


void test3_func_0_10( void )
{
  int i, x;
  int *p = &test3_array3[ 0 ][ 10 ];


  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_10();
}


void test3_func_10_9( void )
{
  int i, x;
  int *p = &test3_array4[ 10 ][ 9 ];


  test3_func_10_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_9( void )
{
  int i, x;
  int *p = &test3_array3[ 9 ][ 9 ];


  test3_func_9_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_9();
}


void test3_func_8_9( void )
{
  int i, x;
  int *p = &test3_array2[ 8 ][ 9 ];


  test3_func_8_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_9();
}


void test3_func_7_9( void )
{
  int i, x;
  int *p = &test3_array1[ 7 ][ 9 ];


  test3_func_7_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_9();
}


void test3_func_6_9( void )
{
  int i, x;
  int *p = &test3_array8[ 6 ][ 9 ];


  test3_func_6_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_9();
}


void test3_func_5_9( void )
{
  int i, x;
  int *p = &test3_array7[ 5 ][ 9 ];


  test3_func_5_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_9();
}


void test3_func_4_9( void )
{
  int i, x;
  int *p = &test3_array6[ 4 ][ 9 ];


  test3_func_4_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_9();
}


void test3_func_3_9( void )
{
  int i, x;
  int *p = &test3_array5[ 3 ][ 9 ];


  test3_func_3_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_9();
}


void test3_func_2_9( void )
{
  int i, x;
  int *p = &test3_array4[ 2 ][ 9 ];


  test3_func_2_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_9();
}


void test3_func_1_9( void )
{
  int i, x;
  int *p = &test3_array3[ 1 ][ 9 ];


  test3_func_1_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_9();
}


void test3_func_0_9( void )
{
  int i, x;
  int *p = &test3_array2[ 0 ][ 9 ];


  test3_func_0_10();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_9();
}


void test3_func_10_8( void )
{
  int i, x;
  int *p = &test3_array3[ 10 ][ 8 ];


  test3_func_10_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_8( void )
{
  int i, x;
  int *p = &test3_array2[ 9 ][ 8 ];


  test3_func_9_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_8();
}


void test3_func_8_8( void )
{
  int i, x;
  int *p = &test3_array1[ 8 ][ 8 ];


  test3_func_8_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_8();
}


void test3_func_7_8( void )
{
  int i, x;
  int *p = &test3_array8[ 7 ][ 8 ];


  test3_func_7_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_8();
}


void test3_func_6_8( void )
{
  int i, x;
  int *p = &test3_array7[ 6 ][ 8 ];


  test3_func_6_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_8();
}


void test3_func_5_8( void )
{
  int i, x;
  int *p = &test3_array6[ 5 ][ 8 ];


  test3_func_5_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_8();
}


void test3_func_4_8( void )
{
  int i, x;
  int *p = &test3_array5[ 4 ][ 8 ];


  test3_func_4_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_8();
}


void test3_func_3_8( void )
{
  int i, x;
  int *p = &test3_array4[ 3 ][ 8 ];


  test3_func_3_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_8();
}


void test3_func_2_8( void )
{
  int i, x;
  int *p = &test3_array3[ 2 ][ 8 ];


  test3_func_2_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_8();
}


void test3_func_1_8( void )
{
  int i, x;
  int *p = &test3_array2[ 1 ][ 8 ];


  test3_func_1_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_8();
}


void test3_func_0_8( void )
{
  int i, x;
  int *p = &test3_array1[ 0 ][ 8 ];


  test3_func_0_9();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_8();
}


void test3_func_10_7( void )
{
  int i, x;
  int *p = &test3_array2[ 10 ][ 7 ];


  test3_func_10_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_7( void )
{
  int i, x;
  int *p = &test3_array1[ 9 ][ 7 ];


  test3_func_9_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_7();
}


void test3_func_8_7( void )
{
  int i, x;
  int *p = &test3_array8[ 8 ][ 7 ];


  test3_func_8_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_7();
}


void test3_func_7_7( void )
{
  int i, x;
  int *p = &test3_array7[ 7 ][ 7 ];


  test3_func_7_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_7();
}


void test3_func_6_7( void )
{
  int i, x;
  int *p = &test3_array6[ 6 ][ 7 ];


  test3_func_6_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_7();
}


void test3_func_5_7( void )
{
  int i, x;
  int *p = &test3_array5[ 5 ][ 7 ];


  test3_func_5_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_7();
}


void test3_func_4_7( void )
{
  int i, x;
  int *p = &test3_array4[ 4 ][ 7 ];


  test3_func_4_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_7();
}


void test3_func_3_7( void )
{
  int i, x;
  int *p = &test3_array3[ 3 ][ 7 ];


  test3_func_3_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_7();
}


void test3_func_2_7( void )
{
  int i, x;
  int *p = &test3_array2[ 2 ][ 7 ];


  test3_func_2_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_7();
}


void test3_func_1_7( void )
{
  int i, x;
  int *p = &test3_array1[ 1 ][ 7 ];


  test3_func_1_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_7();
}


void test3_func_0_7( void )
{
  int i, x;
  int *p = &test3_array8[ 0 ][ 7 ];


  test3_func_0_8();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_7();
}


void test3_func_10_6( void )
{
  int i, x;
  int *p = &test3_array1[ 10 ][ 6 ];


  test3_func_10_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_6( void )
{
  int i, x;
  int *p = &test3_array8[ 9 ][ 6 ];


  test3_func_9_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_6();
}


void test3_func_8_6( void )
{
  int i, x;
  int *p = &test3_array7[ 8 ][ 6 ];


  test3_func_8_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_6();
}


void test3_func_7_6( void )
{
  int i, x;
  int *p = &test3_array6[ 7 ][ 6 ];


  test3_func_7_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_6();
}


void test3_func_6_6( void )
{
  int i, x;
  int *p = &test3_array5[ 6 ][ 6 ];


  test3_func_6_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_6();
}


void test3_func_5_6( void )
{
  int i, x;
  int *p = &test3_array4[ 5 ][ 6 ];


  test3_func_5_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_6();
}


void test3_func_4_6( void )
{
  int i, x;
  int *p = &test3_array3[ 4 ][ 6 ];


  test3_func_4_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_6();
}


void test3_func_3_6( void )
{
  int i, x;
  int *p = &test3_array2[ 3 ][ 6 ];


  test3_func_3_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_6();
}


void test3_func_2_6( void )
{
  int i, x;
  int *p = &test3_array1[ 2 ][ 6 ];


  test3_func_2_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_6();
}


void test3_func_1_6( void )
{
  int i, x;
  int *p = &test3_array8[ 1 ][ 6 ];


  test3_func_1_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_6();
}


void test3_func_0_6( void )
{
  int i, x;
  int *p = &test3_array7[ 0 ][ 6 ];


  test3_func_0_7();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_6();
}


void test3_func_10_5( void )
{
  int i, x;
  int *p = &test3_array8[ 10 ][ 5 ];


  test3_func_10_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_5( void )
{
  int i, x;
  int *p = &test3_array7[ 9 ][ 5 ];


  test3_func_9_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_5();
}


void test3_func_8_5( void )
{
  int i, x;
  int *p = &test3_array6[ 8 ][ 5 ];


  test3_func_8_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_5();
}


void test3_func_7_5( void )
{
  int i, x;
  int *p = &test3_array5[ 7 ][ 5 ];


  test3_func_7_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_5();
}


void test3_func_6_5( void )
{
  int i, x;
  int *p = &test3_array4[ 6 ][ 5 ];


  test3_func_6_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_5();
}


void test3_func_5_5( void )
{
  int i, x;
  int *p = &test3_array3[ 5 ][ 5 ];


  test3_func_5_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_5();
}


void test3_func_4_5( void )
{
  int i, x;
  int *p = &test3_array2[ 4 ][ 5 ];


  test3_func_4_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_5();
}


void test3_func_3_5( void )
{
  int i, x;
  int *p = &test3_array1[ 3 ][ 5 ];


  test3_func_3_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_5();
}


void test3_func_2_5( void )
{
  int i, x;
  int *p = &test3_array8[ 2 ][ 5 ];


  test3_func_2_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_5();
}


void test3_func_1_5( void )
{
  int i, x;
  int *p = &test3_array7[ 1 ][ 5 ];


  test3_func_1_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_5();
}


void test3_func_0_5( void )
{
  int i, x;
  int *p = &test3_array6[ 0 ][ 5 ];


  test3_func_0_6();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_5();
}


void test3_func_10_4( void )
{
  int i, x;
  int *p = &test3_array7[ 10 ][ 4 ];


  test3_func_10_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_4( void )
{
  int i, x;
  int *p = &test3_array6[ 9 ][ 4 ];


  test3_func_9_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_4();
}


void test3_func_8_4( void )
{
  int i, x;
  int *p = &test3_array5[ 8 ][ 4 ];


  test3_func_8_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_4();
}


void test3_func_7_4( void )
{
  int i, x;
  int *p = &test3_array4[ 7 ][ 4 ];


  test3_func_7_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_4();
}


void test3_func_6_4( void )
{
  int i, x;
  int *p = &test3_array3[ 6 ][ 4 ];


  test3_func_6_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_4();
}


void test3_func_5_4( void )
{
  int i, x;
  int *p = &test3_array2[ 5 ][ 4 ];


  test3_func_5_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_4();
}


void test3_func_4_4( void )
{
  int i, x;
  int *p = &test3_array1[ 4 ][ 4 ];


  test3_func_4_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_4();
}


void test3_func_3_4( void )
{
  int i, x;
  int *p = &test3_array8[ 3 ][ 4 ];


  test3_func_3_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_4();
}


void test3_func_2_4( void )
{
  int i, x;
  int *p = &test3_array7[ 2 ][ 4 ];


  test3_func_2_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_4();
}


void test3_func_1_4( void )
{
  int i, x;
  int *p = &test3_array6[ 1 ][ 4 ];


  test3_func_1_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_4();
}


void test3_func_0_4( void )
{
  int i, x;
  int *p = &test3_array5[ 0 ][ 4 ];


  test3_func_0_5();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_4();
}


void test3_func_10_3( void )
{
  int i, x;
  int *p = &test3_array6[ 10 ][ 3 ];


  test3_func_10_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_3( void )
{
  int i, x;
  int *p = &test3_array5[ 9 ][ 3 ];


  test3_func_9_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_3();
}


void test3_func_8_3( void )
{
  int i, x;
  int *p = &test3_array4[ 8 ][ 3 ];


  test3_func_8_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_3();
}


void test3_func_7_3( void )
{
  int i, x;
  int *p = &test3_array3[ 7 ][ 3 ];


  test3_func_7_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_3();
}


void test3_func_6_3( void )
{
  int i, x;
  int *p = &test3_array2[ 6 ][ 3 ];


  test3_func_6_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_3();
}


void test3_func_5_3( void )
{
  int i, x;
  int *p = &test3_array1[ 5 ][ 3 ];


  test3_func_5_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_3();
}


void test3_func_4_3( void )
{
  int i, x;
  int *p = &test3_array8[ 4 ][ 3 ];


  test3_func_4_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_3();
}


void test3_func_3_3( void )
{
  int i, x;
  int *p = &test3_array7[ 3 ][ 3 ];


  test3_func_3_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_3();
}


void test3_func_2_3( void )
{
  int i, x;
  int *p = &test3_array6[ 2 ][ 3 ];


  test3_func_2_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_3();
}


void test3_func_1_3( void )
{
  int i, x;
  int *p = &test3_array5[ 1 ][ 3 ];


  test3_func_1_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_3();
}


void test3_func_0_3( void )
{
  int i, x;
  int *p = &test3_array4[ 0 ][ 3 ];


  test3_func_0_4();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_3();
}


void test3_func_10_2( void )
{
  int i, x;
  int *p = &test3_array5[ 10 ][ 2 ];


  test3_func_10_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_2( void )
{
  int i, x;
  int *p = &test3_array4[ 9 ][ 2 ];


  test3_func_9_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_2();
}


void test3_func_8_2( void )
{
  int i, x;
  int *p = &test3_array3[ 8 ][ 2 ];


  test3_func_8_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_2();
}


void test3_func_7_2( void )
{
  int i, x;
  int *p = &test3_array2[ 7 ][ 2 ];


  test3_func_7_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_2();
}


void test3_func_6_2( void )
{
  int i, x;
  int *p = &test3_array1[ 6 ][ 2 ];


  test3_func_6_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_2();
}


void test3_func_5_2( void )
{
  int i, x;
  int *p = &test3_array8[ 5 ][ 2 ];


  test3_func_5_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_2();
}


void test3_func_4_2( void )
{
  int i, x;
  int *p = &test3_array7[ 4 ][ 2 ];


  test3_func_4_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_2();
}


void test3_func_3_2( void )
{
  int i, x;
  int *p = &test3_array6[ 3 ][ 2 ];


  test3_func_3_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_2();
}


void test3_func_2_2( void )
{
  int i, x;
  int *p = &test3_array5[ 2 ][ 2 ];


  test3_func_2_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_2();
}


void test3_func_1_2( void )
{
  int i, x;
  int *p = &test3_array4[ 1 ][ 2 ];


  test3_func_1_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_2();
}


void test3_func_0_2( void )
{
  int i, x;
  int *p = &test3_array3[ 0 ][ 2 ];


  test3_func_0_3();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_2();
}


void test3_func_10_1( void )
{
  int i, x;
  int *p = &test3_array4[ 10 ][ 1 ];


  test3_func_10_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_1( void )
{
  int i, x;
  int *p = &test3_array3[ 9 ][ 1 ];


  test3_func_9_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_1();
}


void test3_func_8_1( void )
{
  int i, x;
  int *p = &test3_array2[ 8 ][ 1 ];


  test3_func_8_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_1();
}


void test3_func_7_1( void )
{
  int i, x;
  int *p = &test3_array1[ 7 ][ 1 ];


  test3_func_7_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_1();
}


void test3_func_6_1( void )
{
  int i, x;
  int *p = &test3_array8[ 6 ][ 1 ];


  test3_func_6_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_1();
}


void test3_func_5_1( void )
{
  int i, x;
  int *p = &test3_array7[ 5 ][ 1 ];


  test3_func_5_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_1();
}


void test3_func_4_1( void )
{
  int i, x;
  int *p = &test3_array6[ 4 ][ 1 ];


  test3_func_4_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_1();
}


void test3_func_3_1( void )
{
  int i, x;
  int *p = &test3_array5[ 3 ][ 1 ];


  test3_func_3_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_1();
}


void test3_func_2_1( void )
{
  int i, x;
  int *p = &test3_array4[ 2 ][ 1 ];


  test3_func_2_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_1();
}


void test3_func_1_1( void )
{
  int i, x;
  int *p = &test3_array3[ 1 ][ 1 ];


  test3_func_1_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_1();
}


void test3_func_0_1( void )
{
  int i, x;
  int *p = &test3_array2[ 0 ][ 1 ];


  test3_func_0_2();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_1();
}


void test3_func_10_0( void )
{
  int i, x;
  int *p = &test3_array3[ 10 ][ 0 ];


  test3_func_10_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }
}


void test3_func_9_0( void )
{
  int i, x;
  int *p = &test3_array2[ 9 ][ 0 ];


  test3_func_9_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_10_0();
}


void test3_func_8_0( void )
{
  int i, x;
  int *p = &test3_array1[ 8 ][ 0 ];


  test3_func_8_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_9_0();
}


void test3_func_7_0( void )
{
  int i, x;
  int *p = &test3_array8[ 7 ][ 0 ];


  test3_func_7_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_8_0();
}


void test3_func_6_0( void )
{
  int i, x;
  int *p = &test3_array7[ 6 ][ 0 ];


  test3_func_6_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_7_0();
}


void test3_func_5_0( void )
{
  int i, x;
  int *p = &test3_array6[ 5 ][ 0 ];


  test3_func_5_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_6_0();
}


void test3_func_4_0( void )
{
  int i, x;
  int *p = &test3_array5[ 4 ][ 0 ];


  test3_func_4_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_5_0();
}


void test3_func_3_0( void )
{
  int i, x;
  int *p = &test3_array4[ 3 ][ 0 ];


  test3_func_3_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_4_0();
}


void test3_func_2_0( void )
{
  int i, x;
  int *p = &test3_array3[ 2 ][ 0 ];


  test3_func_2_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_3_0();
}


void test3_func_1_0( void )
{
  int i, x;
  int *p = &test3_array2[ 1 ][ 0 ];


  test3_func_1_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_2_0();
}


void test3_func_0_0( void )
{
  int i, x;
  int *p = &test3_array1[ 0 ][ 0 ];


  test3_func_0_1();

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0, x = 8; i < 4; i++, x >>= 1 ) {
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
    test3_result += *p;
    p += x;
  }

  test3_func_1_0();
}


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) test3_main( void )
{
  test3_func_0_0();
}


int main()
{
  test3_init();
  test3_main();

  return ( test3_return() - 1377313800 != 0 );
}
