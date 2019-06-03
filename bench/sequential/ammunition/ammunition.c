/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: ammunition

  Author: Vladimir Makarov <vmakarov@gcc.gnu.org>

  Function: Tests reusable packages bits and arith
    bits: Work with bit strings (copying, moving, setting, testing, comparison).
    arith: Implementing host machine-independently arbitrary precision integer
      numbers arithmetic.  The implementation of the package functions are not
      sufficiently efficient in order to use for run-time.  The package
      functions are oriented to implement constant-folding in compilers,
      cross-compilers.

  Source: DINO programming language repository
          https://github.com/dino-lang/dino/

  Changes: no major functional changes

  License: GPL 2 and LGPL 2

*/

#include "bits.h"
#include "arithm.h"
#include "ammunition_stdlib.h"
#include "ammunition_stdio.h"
#include "ammunition_string.h"

/*
  Forward declaration of functions
*/

void ammunition_reset_str_bits( char *str, char *s );
void ammunition_reset_str_arithm( char *str, char *s, char *d, char *e,
                                  char *g );
int ammunition_bits_test();
int ammunition_arithm_test();
void ammunition_init( void );
int ammunition_return( void );
void ammunition_main( void );
int main( void );


/*
  Forward declaration of global variables
*/

int ammunition_result;


/*
  Core functions
*/

void ammunition_reset_str_bits( char *str, char *s )
{
  int i;
  _Pragma( "loopbound min 8 max 8" )
  for ( i = 0; i < 8; i++ ) {
    str[ i ] = 0;
    s[ i ] = 0;
  }
}


void ammunition_reset_str_arithm( char *str, char *s, char *d, char *e,
                                  char *g )
{
  int i;
  _Pragma( "loopbound min 20 max 20" )
  for ( i = 0; i < 20; i++ ) {
    str[ i ] = 0;
    s[ i ] = 0;
  }

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0; i < 4; i++ ) {
    d[ i ] = 0;
    e[ i ] = 0;
  }

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i < 6; i++ )
    g[ i ] = 0;
}


int ammunition_bits_test()
{
  char str[ 8 ];
  char str1[ 8 ];

  int result = 0;
  unsigned int i, j;

  /* Test 1 */
  ammunition_reset_str_bits( str, str1 );

  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < sizeof ( str ) * CHAR_BIT; i++ ) {
    if ( BIT ( str, i ) )
      result = 1;
  }

  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < sizeof ( str ) * CHAR_BIT; i++ ) {
    SET_BIT ( str, i, 1 );
    _Pragma( "loopbound min 64 max 64" )
    for ( j = 0; j < sizeof ( str ) * CHAR_BIT; j++ )
      if ( j <= i ) {
        if ( BIT ( str, j ) == 0 )
          result = 1;
      } else
        if ( BIT ( str, j ) )
          result = 1;
  }

  /* Test 2 */
  ammunition_reset_str_bits( str, str1 );

  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < sizeof ( str ) * CHAR_BIT; i++ )
    if ( !ammunition_is_zero_bit_string (
           str, i, ( sizeof ( str ) * CHAR_BIT - i ) / 2 + 1 ) )
      result = 1;
  ammunition_bit_string_set ( str, 13, 1, 35 );
  _Pragma( "loopbound min 13 max 13" )
  for ( i = 0; i < 13; i++ )
    if ( !ammunition_is_zero_bit_string ( str, i, 13 - i ) )
      result = 1;
  _Pragma( "loopbound min 35 max 35" )
  for ( i = 13; i < 48; i++ )
    if ( ammunition_is_zero_bit_string ( str, i, 48 - i ) )
      result = 1;
  _Pragma( "loopbound min 16 max 16" )
  for ( i = 48; i < sizeof ( str ) * CHAR_BIT; i++ )
    if ( !ammunition_is_zero_bit_string ( str, i,
                                          sizeof ( str ) * CHAR_BIT - i ) )
      result = 1;

  /* Test 3 */
  ammunition_reset_str_bits( str, str1 );

  _Pragma( "loopbound min 42 max 42" )
  for ( i = 0; i + i / 2 + 1 < sizeof ( str ) * CHAR_BIT; i++ ) {
    ammunition_bit_string_set ( str, i, 1, i / 2 + 1 );
    if ( !ammunition_is_zero_bit_string ( str, 0, i - 1 ) )
      result = 1;
    if ( ammunition_is_zero_bit_string ( str, i, i / 2 + 1 ) )
      result = 1;
    if ( !ammunition_is_zero_bit_string (
           str, i + i / 2 + 1, sizeof ( str ) * CHAR_BIT - ( i + i / 2 + 1 ) ) )
      result = 1;
    ammunition_bit_string_set ( str, 0, 0, sizeof ( str ) * CHAR_BIT );
  }

  /* Test 4 */
  ammunition_reset_str_bits( str, str1 );

  ammunition_bit_string_set ( str, 2, 1, 43 );
  ammunition_bit_string_set ( str1, 2, 1, 40 );
  _Pragma( "loopbound min 42 max 42" )
  for ( i = 0; i < 42; i++ )
    if ( ammunition_bit_string_comparison ( str, i, str1, i, 42 - i ) != 0 )
      result = 1;
  _Pragma( "loopbound min 43 max 43" )
  for ( i = 0; i < 43; i++ )
    if ( ammunition_bit_string_comparison ( str, i, str1, i,
                                            sizeof ( str ) * CHAR_BIT - i )
         <= 0 )
      result = 1;
  _Pragma( "loopbound min 43 max 43" )
  for ( i = 0; i < 43; i++ )
    if ( ammunition_bit_string_comparison ( str1, i, str, i,
                                            sizeof ( str ) * CHAR_BIT - i )
         >= 0 )
      result = 1;

  /* Test 5 */
  ammunition_reset_str_bits( str, str1 );

  ammunition_bit_string_set ( str, 2, 1, 43 );
  _Pragma( "loopbound min 59 max 59" )
  for ( i = 0; i + 5 < sizeof ( str ) * CHAR_BIT; i++ ) {
    ammunition_bit_string_copy ( str1, i + 5, str, i,
                                 sizeof ( str ) * CHAR_BIT - i - 5 );
    if ( ammunition_bit_string_comparison (
           str1, i + 5, str, i, sizeof ( str ) * CHAR_BIT - i - 5 ) != 0 )
      result = 1;
  }

  /* Test 6 */
  ammunition_reset_str_bits( str, str1 );

  ammunition_bit_string_set ( str, 2, 1, 43 );
  ammunition_bit_string_set ( str1, 2, 1, 43 );
  _Pragma( "loopbound min 59 max 59" )
  for ( i = 0; i + 5 < sizeof ( str ) * CHAR_BIT; i++ ) {
    ammunition_bit_string_set ( str, 0, 0, sizeof ( str ) * CHAR_BIT );
    ammunition_bit_string_set ( str, 2, 1, 43 );
    ammunition_bit_string_move ( str, i + 5, str, i,
                                 sizeof ( str ) * CHAR_BIT - i - 5 );
    if ( ammunition_bit_string_comparison (
           str, i + 5, str1, i, sizeof ( str ) * CHAR_BIT - i - 5 ) != 0 )
      result = 1;
  }

  /* Test 7 */
  ammunition_reset_str_bits( str, str1 );

  ammunition_bit_string_set ( str, 2, 1, 43 );
  ammunition_bit_string_set ( str1, 2, 1, 43 );
  _Pragma( "loopbound min 59 max 59" )
  for ( i = 0; i + 5 < sizeof ( str ) * CHAR_BIT; i++ ) {
    ammunition_bit_string_set ( str, 0, 0, sizeof ( str ) * CHAR_BIT );
    ammunition_bit_string_set ( str, 2, 1, 43 );
    ammunition_bit_string_move ( str, i, str, i + 5,
                                 sizeof ( str ) * CHAR_BIT - i - 5 );
    if ( ammunition_bit_string_comparison (
           str, i, str1, i + 5, sizeof ( str ) * CHAR_BIT - i - 5 ) != 0 )
      result = 1;
  }

  return result;
}


int ammunition_arithm_test()
{
  int result = 0;

  /* Test 1 */
  int i;
  char str [ 20 ], s[ 20 ], d[ 4 ], e[ 4 ], g[ 6 ];

  ammunition_integer_from_string ( 4, "-2147483649", d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_d( str, INT_MIN );
  ammunition_integer_from_string ( 4, str, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "2147483648", d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_d( str, INT_MAX );
  ammunition_integer_from_string ( 4, str, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = -2000; i < 2000 ; i++ ) {
    ammunition_sprintf_d( str, i );
    ammunition_integer_from_string ( 4, str, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_integer_to_string( 4, d, s );
    if ( ammunition_strcmp ( s, str ) != 0 )
      result = 1;
  }

  /* Test 2 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "4294967296", d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_u( str, UINT_MAX );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = 0; i < 4000 ; i++ ) {
    ammunition_sprintf_u( str, i );
    ammunition_unsigned_integer_from_string ( 4, str, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_unsigned_integer_to_string ( 4, d, s );
    if ( ammunition_strcmp ( s, str ) != 0 )
      result = 1;
  }

  /* Test 3 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_sprintf_d( str, INT_MAX );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_integer_from_string ( 4, "1", e );
  ammunition_add_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_d( str, INT_MAX - 4 );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_sprintf_d( str, 4 );
  ammunition_integer_from_string ( 4, str, e );
  ammunition_add_integer ( 4, d, e, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_integer_to_string( 4, d, s );
  ammunition_sprintf_d( str, INT_MAX );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = -2000; i < 2000 ; i++ ) {
    ammunition_sprintf_d( str, i );
    ammunition_integer_from_string ( 4, str, d );
    ammunition_sprintf_d( str, i + 1 );
    ammunition_integer_from_string ( 4, str, e );
    ammunition_add_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_integer_to_string( 4, d, s );
    if ( ammunition_atoi ( s ) != i + i + 1 )
      result = 1;
  }

  /* Test 4 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_sprintf_u( str, UINT_MAX );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  ammunition_unsigned_integer_from_string ( 4, "1", e );
  ammunition_add_unsigned_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_u( str, UINT_MAX - 4 );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  ammunition_sprintf_u( str, 4 );
  ammunition_unsigned_integer_from_string ( 4, str, e );
  ammunition_add_unsigned_integer ( 4, d, e, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_unsigned_integer_to_string ( 4, d, s );
  ammunition_sprintf_u( str, UINT_MAX );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = 0; i < 4000 ; i++ ) {
    ammunition_sprintf_u( str, i );
    ammunition_unsigned_integer_from_string ( 4, str, d );
    ammunition_sprintf_u( str, i + 1 );
    ammunition_unsigned_integer_from_string ( 4, str, e );
    ammunition_add_unsigned_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_unsigned_integer_to_string ( 4, d, s );
    if ( ammunition_atoi ( s ) != i + i + 1 )
      result = 1;
  }

  /* Test 5 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_sprintf_d( str, INT_MIN );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_integer_from_string ( 4, "1", e );
  ammunition_subtract_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_d( str, INT_MIN + 4 );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_sprintf_d( str, 4 );
  ammunition_integer_from_string ( 4, str, e );
  ammunition_subtract_integer ( 4, d, e, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_integer_to_string( 4, d, s );
  ammunition_sprintf_d( str, INT_MIN );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = -2000; i < 2000 ; i++ ) {
    ammunition_sprintf_d( str, i );
    ammunition_integer_from_string ( 4, str, d );
    ammunition_sprintf_d( str, 10 - i );
    ammunition_integer_from_string ( 4, str, e );
    ammunition_subtract_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_integer_to_string( 4, d, s );
    if ( ammunition_atoi ( s ) != i + i - 10 )
      result = 1;
  }

  /* Test 6 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_sprintf_u( str, UINT_MAX - 2 );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  ammunition_sprintf_u( str, UINT_MAX - 1 );
  ammunition_unsigned_integer_from_string ( 4, str, e );
  ammunition_subtract_unsigned_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_u( str, UINT_MAX );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  ammunition_subtract_unsigned_integer ( 4, d, d, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = 0; i < 4000 ; i++ ) {
    ammunition_sprintf_u( str, i );
    ammunition_unsigned_integer_from_string ( 4, str, d );
    ammunition_sprintf_u( str, i / 2 );
    ammunition_unsigned_integer_from_string ( 4, str, e );
    ammunition_subtract_unsigned_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_unsigned_integer_to_string ( 4, d, s );
    if ( ammunition_atoi ( s ) != i - i / 2 )
      result = 1;
  }

  /* Test 7 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_sprintf_d( str, INT_MAX / 2 + 1 );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_integer_from_string ( 4, "2", e );
  ammunition_multiply_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_d( str, INT_MIN / 2 - 1 );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_integer_from_string ( 4, "2", e );
  ammunition_multiply_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_d( str, INT_MAX / 3 );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_sprintf_d( str, 3 );
  ammunition_integer_from_string ( 4, str, e );
  ammunition_multiply_integer ( 4, d, e, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_integer_to_string( 4, d, s );
  ammunition_sprintf_d( str, ( INT_MAX / 3 ) * 3 );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  ammunition_sprintf_d( str, INT_MIN / 2 );
  ammunition_integer_from_string ( 4, str, d );
  ammunition_sprintf_d( str, 2 );
  ammunition_integer_from_string ( 4, str, e );
  ammunition_multiply_integer ( 4, d, e, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_integer_to_string( 4, d, s );
  ammunition_sprintf_d( str, ( INT_MIN / 2 ) * 2 );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = -2000; i < 2000 ; i++ ) {
    ammunition_sprintf_d( str, i );
    ammunition_integer_from_string ( 4, str, d );
    ammunition_sprintf_d( str, i + 1000 );
    ammunition_integer_from_string ( 4, str, e );
    ammunition_multiply_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_integer_to_string( 4, d, s );
    if ( ammunition_atoi ( s ) != i * ( i + 1000 ) )
      result = 1;
  }

  /* Test 8 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_sprintf_u( str, UINT_MAX / 5 + 1 );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  ammunition_sprintf_u( str, 5 );
  ammunition_unsigned_integer_from_string ( 4, str, e );
  ammunition_multiply_unsigned_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_sprintf_u( str, UINT_MAX / 2 );
  ammunition_unsigned_integer_from_string ( 4, str, d );
  ammunition_sprintf_u( str, 2 );
  ammunition_unsigned_integer_from_string ( 4, str, e );
  ammunition_multiply_unsigned_integer ( 4, d, e, d );
  if ( ammunition_overflow_bit )
    result = 1;
  ammunition_unsigned_integer_to_string ( 4, d, s );
  ammunition_sprintf_u( str, ( UINT_MAX / 2 ) * 2 );
  if ( ammunition_strcmp ( s, str ) != 0 )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = 0; i < 4000 ; i++ ) {
    ammunition_sprintf_u( str, i );
    ammunition_unsigned_integer_from_string ( 4, str, d );
    ammunition_sprintf_u( str, i / 2 );
    ammunition_unsigned_integer_from_string ( 4, str, e );
    ammunition_multiply_unsigned_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_unsigned_integer_to_string ( 4, d, s );
    if ( ammunition_atoi ( s ) != i * ( i / 2 ) )
      result = 1;
  }

  /* Test 9 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "10", d );
  ammunition_integer_from_string ( 4, "0", e );
  ammunition_divide_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = -2000; i < 2000 ; i++ ) {
    ammunition_sprintf_d( str, i );
    ammunition_integer_from_string ( 4, str, d );
    ammunition_sprintf_d( str, ( i < 0 ? - i / 20 + 1 : - i / 20 - 1 ) );
    ammunition_integer_from_string ( 4, str, e );
    ammunition_divide_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_integer_to_string( 4, d, s );
    if ( ammunition_atoi ( s ) != i / ( i < 0 ? - i / 20 + 1 : - i / 20 - 1 ) )
      result = 1;
    ammunition_sprintf_d( str, i );
    ammunition_integer_from_string ( 4, str, d );
    ammunition_divide_integer ( 4, d, e, e );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_integer_to_string( 4, e, s );
    if ( ammunition_atoi ( s ) != i / ( i < 0 ? - i / 20 + 1 : - i / 20 - 1 ) )
      result = 1;
  }

  /* Test 10 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "0", e );
  ammunition_divide_unsigned_integer ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = 0; i < 4000 ; i++ ) {
    ammunition_sprintf_u( str, i );
    ammunition_unsigned_integer_from_string ( 4, str, d );
    ammunition_sprintf_u( str, i / 20 + 1 );
    ammunition_unsigned_integer_from_string ( 4, str, e );
    ammunition_divide_unsigned_integer ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_unsigned_integer_to_string ( 4, d, s );
    if ( ammunition_atoi ( s ) != i / ( i / 20 + 1 ) )
      result = 1;
  }

  /* Test 11 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "0", e );
  ammunition_unsigned_integer_remainder ( 4, d, e, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for ( i = 0; i < 4000 ; i++ ) {
    ammunition_sprintf_u( str, i );
    ammunition_unsigned_integer_from_string ( 4, str, d );
    ammunition_sprintf_u( str, i / 20 + 1 );
    ammunition_unsigned_integer_from_string ( 4, str, e );
    ammunition_unsigned_integer_remainder ( 4, d, e, d );
    if ( ammunition_overflow_bit )
      result = 1;
    ammunition_unsigned_integer_to_string ( 4, d, s );
    if ( ammunition_atoi ( s ) != i % ( i / 20 + 1 ) )
      result = 1;
  }

  /* Test 12 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_shift_right ( 4, d, 0, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_shift_right ( 4, d, 32, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_shift_right ( 4, d, 8, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "5" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "134890", d );
  ammunition_unsigned_integer_shift_right ( 4, d, 13, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "16" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "134890", d );
  ammunition_unsigned_integer_shift_left ( 4, d, -13, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "16" ) != 0 )
    result = 1;

  /* Test 13 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_shift_right ( 4, d, 0, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_shift_right ( 4, d, 32, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_shift_right ( 4, d, 8, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "5" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "134890", d );
  ammunition_integer_shift_right ( 4, d, 13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "16" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "134890", d );
  ammunition_integer_shift_left ( 4, d, -13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "16" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_shift_right ( 4, d, 0, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_shift_right ( 4, d, 32, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "-1" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_shift_right ( 4, d, 8, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "-6" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-134890", d );
  ammunition_integer_shift_right ( 4, d, 13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "-17" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-134890", d );
  ammunition_integer_shift_left ( 4, d, -13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( !ammunition_overflow_bit || ammunition_strcmp ( s, "-17" ) != 0 )
    result = 1;

  /* Test 14 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_shift_left ( 4, d, 0, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_shift_left ( 4, d, 22, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_shift_left ( 4, d, 8, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "345088" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "134890", d );
  ammunition_unsigned_integer_shift_left ( 4, d, 13, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1105018880" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "134890", d );
  ammunition_unsigned_integer_shift_right ( 4, d, -13, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1105018880" ) != 0 )
    result = 1;

  /* Test 15 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_shift_left ( 4, d, 0, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_shift_left ( 4, d, 21, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_shift_left ( 4, d, 8, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "345088" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "134890", d );
  ammunition_integer_shift_left ( 4, d, 13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1105018880" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "134890", d );
  ammunition_integer_shift_right ( 4, d, -13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "1105018880" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_shift_left ( 4, d, 0, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_shift_left ( 4, d, 21, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_shift_left ( 4, d, 8, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-345088" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-134890", d );
  ammunition_integer_shift_left ( 4, d, 13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-1105018880" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-134890", d );
  ammunition_integer_shift_right ( 4, d, -13, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-1105018880" ) != 0 )
    result = 1;

  /* Test 16 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "-10", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( ammunition_eq_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( !ammunition_eq_integer ( 4, d, e ) )
    result = 1;

  /* Test 17 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_eq_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_eq_unsigned_integer ( 4, d, e ) )
    result = 1;

  /* Test 18 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "-10", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( !ammunition_ne_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( ammunition_ne_integer ( 4, d, e ) )
    result = 1;

  /* Test 19 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_ne_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_ne_unsigned_integer ( 4, d, e ) )
    result = 1;

  /* Test 20 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "-10", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( ammunition_gt_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( ammunition_gt_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1000000", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( ammunition_gt_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "1000000", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( !ammunition_gt_integer ( 4, d, e ) )
    result = 1;

  /* Test 21 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_gt_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_gt_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1000000", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_gt_unsigned_integer ( 4, d, e ) )
    result = 1;

  /* Test 22 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "-10", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( !ammunition_lt_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( ammunition_lt_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1000000", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( !ammunition_lt_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "1000000", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( ammunition_lt_integer ( 4, d, e ) )
    result = 1;

  /* Test 23 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_lt_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_lt_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1000000", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_lt_unsigned_integer ( 4, d, e ) )
    result = 1;

  /* Test 24 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "-10", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( ammunition_ge_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( !ammunition_ge_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1000000", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( ammunition_ge_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "1000000", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( !ammunition_ge_integer ( 4, d, e ) )
    result = 1;

  /* Test 25 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_ge_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_ge_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1000000", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_ge_unsigned_integer ( 4, d, e ) )
    result = 1;

  /* Test 26 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "-10", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( !ammunition_le_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1348", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( !ammunition_le_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "-1000000", d );
  ammunition_integer_from_string ( 4, "-1348", e );
  if ( !ammunition_le_integer ( 4, d, e ) )
    result = 1;
  ammunition_integer_from_string ( 4, "1000000", d );
  ammunition_integer_from_string ( 4, "1348", e );
  if ( ammunition_le_integer ( 4, d, e ) )
    result = 1;

  /* Test 27 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "10", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_le_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( !ammunition_le_unsigned_integer ( 4, d, e ) )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1000000", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  if ( ammunition_le_unsigned_integer ( 4, d, e ) )
    result = 1;

  /* Test 28 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "70000", d );
  ammunition_change_unsigned_integer_size ( 4, d, 2, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "30000", d );
  ammunition_change_unsigned_integer_size ( 4, d, 2, d );
  ammunition_integer_to_string( 2, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "30000" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "11230000", g );
  ammunition_change_unsigned_integer_size ( 4, g, 6, g );
  ammunition_integer_to_string( 6, g, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "11230000" ) != 0 )
    result = 1;

  /* Test 29 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "40000", d );
  ammunition_change_integer_size ( 4, d, 2, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_integer_from_string ( 4, "-33000", d );
  ammunition_change_integer_size ( 4, d, 2, d );
  if ( !ammunition_overflow_bit )
    result = 1;
  ammunition_integer_from_string ( 4, "30000", d );
  ammunition_change_integer_size ( 4, d, 2, d );
  ammunition_integer_to_string( 2, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "30000" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-30000", d );
  ammunition_change_integer_size ( 4, d, 2, d );
  ammunition_integer_to_string( 2, d, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-30000" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "11230000", g );
  ammunition_change_integer_size ( 4, g, 6, g );
  ammunition_integer_to_string( 6, g, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "11230000" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-11230000", g );
  ammunition_change_integer_size ( 4, g, 6, g );
  ammunition_integer_to_string( 6, g, s );
  if ( ammunition_overflow_bit || ammunition_strcmp ( s, "-11230000" ) != 0 )
    result = 1;

  /* Test 30 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  ammunition_unsigned_integer_or ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "0", e );
  ammunition_unsigned_integer_or ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "4294967295", e );
  ammunition_unsigned_integer_or ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "4294967295" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "96", e );
  ammunition_unsigned_integer_or ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "1380" ) != 0 )
    result = 1;

  /* Test 31 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "1348", e );
  ammunition_integer_or ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "0", e );
  ammunition_integer_or ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "-1", e );
  ammunition_integer_or ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "-1" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "96", e );
  ammunition_integer_or ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "1380" ) != 0 )
    result = 1;

  /* Test 32 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "1348", e );
  ammunition_unsigned_integer_and ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "0", e );
  ammunition_unsigned_integer_and ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "4294967295", e );
  ammunition_unsigned_integer_and ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_from_string ( 4, "96", e );
  ammunition_unsigned_integer_and ( 4, d, e, e );
  ammunition_unsigned_integer_to_string ( 4, e, s );
  if ( ammunition_strcmp ( s, "64" ) != 0 )
    result = 1;

  /* Test 33 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "1348", e );
  ammunition_integer_and ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "0", e );
  ammunition_integer_and ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "-1", e );
  ammunition_integer_and ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "1348" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_from_string ( 4, "96", e );
  ammunition_integer_and ( 4, d, e, e );
  ammunition_integer_to_string( 4, e, s );
  if ( ammunition_strcmp ( s, "64" ) != 0 )
    result = 1;

  /* Test 34 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_unsigned_integer_from_string ( 4, "1348", d );
  ammunition_unsigned_integer_not ( 4, d, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_strcmp ( s, "4294965947" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "0", d );
  ammunition_unsigned_integer_not ( 4, d, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_strcmp ( s, "4294967295" ) != 0 )
    result = 1;
  ammunition_unsigned_integer_from_string ( 4, "4294967295", d );
  ammunition_unsigned_integer_not ( 4, d, d );
  ammunition_unsigned_integer_to_string ( 4, d, s );
  if ( ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;

  /* Test 35 */
  ammunition_reset_str_arithm( str, s, d, e, g );

  ammunition_integer_from_string ( 4, "1348", d );
  ammunition_integer_not ( 4, d, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_strcmp ( s, "-1349" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "0", d );
  ammunition_integer_not ( 4, d, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_strcmp ( s, "-1" ) != 0 )
    result = 1;
  ammunition_integer_from_string ( 4, "-1", d );
  ammunition_integer_not ( 4, d, d );
  ammunition_integer_to_string( 4, d, s );
  if ( ammunition_strcmp ( s, "0" ) != 0 )
    result = 1;

  return result;
}


/*
  Initialization- and return-value-related functions
*/

void ammunition_init( void )
{
  ammunition_result = 0;
}

int ammunition_return( void )
{
  return ammunition_result;
}

/*
  Main functions
*/

void _Pragma( "entrypoint" ) ammunition_main( void )
{
  ammunition_result |= ammunition_bits_test();
  ammunition_result |= ammunition_arithm_test();
}


int main( void )
{
  ammunition_init();
  ammunition_main();

  return ( ammunition_return() );
}
