#include "bits.h"
#include "arithm.h"

int atoi ( const char * str )
{
  int result = 0;
  int sign = ( str[0] == '-' ? -1 : 1 );
  
  int readingPos = 0;
  if ( str[0] == '-' || str[0] == '+' ) {
    readingPos++;
  }
  _Pragma( "loopbound min 1 max 1" )
  do {
    result *= 10;
    result += str[readingPos++] - 48;
  } while ( str[readingPos] != 0 );
  
  return sign * result;
}

int strcmp ( const char * str1, const char * str2 )
{
  int pos = 0;
  _Pragma( "loopbound min 1 max 4008" )
  while ( str1[pos] != 0 && str2[pos] != 0 ) {
    if ( str1[pos] != str2[pos] ) {
      return str1[pos] - str2[pos];
    }
    pos++;
  }
  
  if ( str1[pos] != 0 ) {
    return 1;
  } else {
    return -1;
  }
}

int sprintf_d( char *s, int number )
{
  // How many decimal digits do we need?
  char digits = 0;
  int copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    digits++;
    copyOfNumber /= 10;
  } while (copyOfNumber != 0);
  
  char writePos = digits;
  if ( number < 0 ) {
    writePos++;
    s[0] = '-';
  }
  s[writePos] = 0;
      
  copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    s[--writePos] = 48 + ( ( copyOfNumber >= 0 ? 
            copyOfNumber : -copyOfNumber ) % 10 );
    copyOfNumber /= 10;
  } while (copyOfNumber != 0);
  
  return digits + (number < 0 ? 1 : 0);
}
int sprintf_u( char *s, unsigned int number )
{
  // How many decimal digits do we need?
  char digits = 0;
  int copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    digits++;
    copyOfNumber /= 10;
  } while (copyOfNumber != 0);
  
  char writePos = digits; 
  s[writePos] = 0;
      
  copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    s[--writePos] = 48 + ( copyOfNumber % 10 );
    copyOfNumber /= 10;
  } while (copyOfNumber != 0);
  
  return digits;
}

void reset_str_bits( char* str, char* s )
{
  int i;
  _Pragma( "loopbound min 8 max 8" )
  for ( i = 0; i < 8; i++ ) {
    str[i] = 0;
    s[i] = 0;
  }
}

void reset_str_arithm( char* str, char* s, char* d, char* e, char* g )
{
  int i;
  _Pragma( "loopbound min 20 max 20" )
  for ( i = 0; i < 20; i++ ) {
    str[i] = 0;
    s[i] = 0;
  }

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0; i < 4; i++ ) {
    d[i] = 0;
    e[i] = 0;
  }
  
  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i < 6; i++ ) {
    g[i] = 0;
  }
}


int bits_test()
{
  char str[8];
  char str1[8];
  
  int result = 0;
  int i, j;

  // Test 1
  reset_str_bits( str, str1 );

  _Pragma( "loopbound min 64 max 64" )
  for (i = 0; i < sizeof (str) * CHAR_BIT; i++) {
    if (BIT (str, i)) {
      result = 1;
    }
  }
  
  _Pragma( "loopbound min 64 max 64" )
  for (i = 0; i < sizeof (str) * CHAR_BIT; i++) {
    SET_BIT (str, i, 1);
    _Pragma( "loopbound min 64 max 64" )
    for (j = 0; j < sizeof (str) * CHAR_BIT; j++)
      if (j <= i) {
        if (BIT (str, j) == 0)
          result = 1;
      } else if (BIT (str, j))
        result = 1;
  }

  result = 0;
  
  // Test 2
  reset_str_bits( str, str1 );

  _Pragma( "loopbound min 64 max 64" )
  for (i = 0; i < sizeof (str) * CHAR_BIT; i++)
    if (!is_zero_bit_string (str, i, (sizeof (str) * CHAR_BIT - i) / 2 + 1))
      result = 1;
  bit_string_set (str, 13, 1, 35);
  _Pragma( "loopbound min 13 max 13" )
  for (i = 0; i < 13; i++)
    if (!is_zero_bit_string (str, i, 13 - i))
      result = 1;
  _Pragma( "loopbound min 35 max 35" )
  for (i = 13; i < 48; i++)
    if (is_zero_bit_string (str, i, 48 - i))
      result = 1;
  _Pragma( "loopbound min 16 max 16" )
  for (i = 48; i < sizeof (str) * CHAR_BIT; i++)
    if (!is_zero_bit_string (str, i, sizeof (str) * CHAR_BIT - i))
      result = 1;

  result = 0;
  
  // Test 3
  reset_str_bits( str, str1 );

  _Pragma( "loopbound min 42 max 42" )
  for (i = 0; i + i / 2 + 1 < sizeof (str) * CHAR_BIT; i++) {
    bit_string_set (str, i, 1, i / 2 + 1);
    if (!is_zero_bit_string (str, 0, i - 1))
      result = 1;
    if (is_zero_bit_string (str, i, i / 2 + 1))
      result = 1;
    if (!is_zero_bit_string (str, i + i / 2 + 1,
                             sizeof (str) * CHAR_BIT - (i + i / 2 + 1)))
      result = 1;
    bit_string_set (str, 0, 0, sizeof (str) * CHAR_BIT);
  }

  result = 0;
  
  // Test 4
  reset_str_bits( str, str1 );

  bit_string_set (str, 2, 1, 43);
  bit_string_set (str1, 2, 1, 40);
  _Pragma( "loopbound min 42 max 42" )
  for (i = 0; i < 42; i++)
    if (bit_string_comparison (str, i, str1, i, 42 - i) != 0)
      result = 1;
  _Pragma( "loopbound min 43 max 43" )
  for (i = 0; i < 43; i++)
    if (bit_string_comparison (str, i, str1, i, sizeof (str) * CHAR_BIT - i)
        <= 0)
      result = 1;
  _Pragma( "loopbound min 43 max 43" )
  for (i = 0; i < 43; i++)
    if (bit_string_comparison (str1, i, str, i, sizeof (str) * CHAR_BIT - i)
        >= 0)
      result = 1;

  result = 0;
  
  // Test 5
  reset_str_bits( str, str1 );

  bit_string_set (str, 2, 1, 43);
  _Pragma( "loopbound min 59 max 59" )
  for (i = 0; i + 5 < sizeof (str) * CHAR_BIT; i++) {
    bit_string_copy (str1, i + 5, str, i, sizeof (str) * CHAR_BIT - i - 5);
    if (bit_string_comparison (str1, i + 5, str, i,
                               sizeof (str) * CHAR_BIT - i - 5) != 0)
      result = 1;
  }

  result = 0;
  
  // Test 6
  reset_str_bits( str, str1 );

  bit_string_set (str, 2, 1, 43);
  bit_string_set (str1, 2, 1, 43);
  _Pragma( "loopbound min 59 max 59" )
  for (i = 0; i + 5 < sizeof (str) * CHAR_BIT; i++) {
    bit_string_set (str, 0, 0, sizeof (str) * CHAR_BIT);
    bit_string_set (str, 2, 1, 43);
    bit_string_move (str, i + 5, str, i, sizeof (str) * CHAR_BIT - i - 5);
    if (bit_string_comparison (str, i + 5, str1, i,
                               sizeof (str) * CHAR_BIT - i - 5) != 0)
      result = 1;
  }

  result = 0;
  
  // Test 7
  reset_str_bits( str, str1 );

  bit_string_set (str, 2, 1, 43);
  bit_string_set (str1, 2, 1, 43);
  _Pragma( "loopbound min 59 max 59" )
  for (i = 0; i + 5 < sizeof (str) * CHAR_BIT; i++) {
    bit_string_set (str, 0, 0, sizeof (str) * CHAR_BIT);
    bit_string_set (str, 2, 1, 43);
    bit_string_move (str, i, str, i + 5, sizeof (str) * CHAR_BIT - i - 5);
    if (bit_string_comparison (str, i, str1, i + 5,
                               sizeof (str) * CHAR_BIT - i - 5) != 0)
      result = 1;
  }
  
  result = 0;
  
  return result;
}


int arithm_test()
{
  int result = 0;

  // Test 1
  int i;
  char str [20], s[20], d[4], e[4], g[6];

  integer_from_string (4, "-2147483649", d);
  if (!overflow_bit)
    result = 1;
  sprintf_d( str, INT_MIN );
  integer_from_string (4, str, d);
  if (overflow_bit)
    result = 1;
  integer_to_string (4, d, s);
  if (strcmp (s, str) != 0)
    result = 1;
  integer_from_string (4, "2147483648", d);
  if (!overflow_bit)
    result = 1;
  sprintf_d( str, INT_MAX );
  integer_from_string (4, str, d);
  if (overflow_bit)
    result = 1;
  integer_to_string (4, d, s);
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = -2000; i < 2000 ; i++) {
    sprintf_d( str, i );
    integer_from_string (4, str, d);
    if (overflow_bit)
      result = 1;
    integer_to_string (4, d, s);
    if (strcmp (s, str) != 0)
      result = 1;
  }

  result = 0;
  
  // Test 2
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "4294967296", d);
  if (!overflow_bit)
    result = 1;
  sprintf_u( str, UINT_MAX );
  unsigned_integer_from_string (4, str, d);
  if (overflow_bit)
    result = 1;
  unsigned_integer_to_string (4, d, s);
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = 0; i < 4000 ; i++) {
    sprintf_u( str, i );
    unsigned_integer_from_string (4, str, d);
    if (overflow_bit)
      result = 1;
    unsigned_integer_to_string (4, d, s);
    if (strcmp (s, str) != 0)
      result = 1;
  }

  result = 0;
  
  // Test 3
  reset_str_arithm( str, s, d, e, g );

  sprintf_d( str, INT_MAX );
  integer_from_string (4, str, d);
  integer_from_string (4, "1", e);
  add_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_d( str, INT_MAX - 4 );
  integer_from_string (4, str, d);
  sprintf_d( str, 4 );
  integer_from_string (4, str, e);
  add_integer (4, d, e, d);
  if (overflow_bit)
    result = 1;
  integer_to_string (4, d, s);
  sprintf_d( str, INT_MAX );
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = -2000; i < 2000 ; i++) {
    sprintf_d( str, i );
    integer_from_string (4, str, d);
    sprintf_d( str, i + 1 );
    integer_from_string (4, str, e);
    add_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    integer_to_string (4, d, s);
    if (atoi (s) != i + i + 1)
      result = 1;
  }

  result = 0;
  
  // Test 4
  reset_str_arithm( str, s, d, e, g );

  sprintf_u( str, UINT_MAX );
  unsigned_integer_from_string (4, str, d);
  unsigned_integer_from_string (4, "1", e);
  add_unsigned_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_u( str, UINT_MAX - 4 );
  unsigned_integer_from_string (4, str, d);
  sprintf_u( str, 4 );
  unsigned_integer_from_string (4, str, e);
  add_unsigned_integer (4, d, e, d);
  if (overflow_bit)
    result = 1;
  unsigned_integer_to_string (4, d, s);
  sprintf_u( str, UINT_MAX );
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = 0; i < 4000 ; i++) {
    sprintf_u( str, i );
    unsigned_integer_from_string (4, str, d);
    sprintf_u( str, i + 1 );
    unsigned_integer_from_string (4, str, e);
    add_unsigned_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    unsigned_integer_to_string (4, d, s);
    if (atoi (s) != i + i + 1)
      result = 1;
  }

  result = 0;
  
  // Test 5
  reset_str_arithm( str, s, d, e, g );

  sprintf_d( str, INT_MIN );
  integer_from_string (4, str, d);
  integer_from_string (4, "1", e);
  subtract_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_d( str, INT_MIN + 4 );
  integer_from_string (4, str, d);
  sprintf_d( str, 4 );
  integer_from_string (4, str, e);
  subtract_integer (4, d, e, d);
  if (overflow_bit)
    result = 1;
  integer_to_string (4, d, s);
  sprintf_d( str, INT_MIN );
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = -2000; i < 2000 ; i++) {
    sprintf_d( str, i );
    integer_from_string (4, str, d);
    sprintf_d( str, 10 - i );
    integer_from_string (4, str, e);
    subtract_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    integer_to_string (4, d, s);
    if (atoi (s) != i + i - 10)
      result = 1;
  }

  result = 0;
  
  // Test 6
  reset_str_arithm( str, s, d, e, g );

  sprintf_u( str, UINT_MAX - 2 );
  unsigned_integer_from_string (4, str, d);
  sprintf_u( str, UINT_MAX - 1 );
  unsigned_integer_from_string (4, str, e);
  subtract_unsigned_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_u( str, UINT_MAX );
  unsigned_integer_from_string (4, str, d);
  subtract_unsigned_integer (4, d, d, d);
  if (overflow_bit)
    result = 1;
  unsigned_integer_to_string (4, d, s);
  if (strcmp (s, "0") != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = 0; i < 4000 ; i++) {
    sprintf_u( str, i );
    unsigned_integer_from_string (4, str, d);
    sprintf_u( str, i / 2 );
    unsigned_integer_from_string (4, str, e);
    subtract_unsigned_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    unsigned_integer_to_string (4, d, s);
    if (atoi (s) != i - i / 2)
      result = 1;
  }

  result = 0;
  
  // Test 7
  reset_str_arithm( str, s, d, e, g );

  sprintf_d( str, INT_MAX / 2 + 1 );
  integer_from_string (4, str, d);
  integer_from_string (4, "2", e);
  multiply_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_d( str, INT_MIN / 2 - 1 );
  integer_from_string (4, str, d);
  integer_from_string (4, "2", e);
  multiply_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_d( str, INT_MAX / 3 );
  integer_from_string (4, str, d);
  sprintf_d( str, 3 );
  integer_from_string (4, str, e);
  multiply_integer (4, d, e, d);
  if (overflow_bit)
    result = 1;
  integer_to_string (4, d, s);
  sprintf_d( str, (INT_MAX / 3) * 3 );
  if (strcmp (s, str) != 0)
    result = 1;
  sprintf_d( str, INT_MIN / 2 );
  integer_from_string (4, str, d);
  sprintf_d( str, 2 );
  integer_from_string (4, str, e);
  multiply_integer (4, d, e, d);
  if (overflow_bit)
    result = 1;
  integer_to_string (4, d, s);
  sprintf_d( str, (INT_MIN / 2) * 2 );
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = -2000; i < 2000 ; i++) {
    sprintf_d( str, i );
    integer_from_string (4, str, d);
    sprintf_d( str, i + 1000 );
    integer_from_string (4, str, e);
    multiply_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    integer_to_string (4, d, s);
    if (atoi (s) != i * (i + 1000))
      result = 1;
  }

  result = 0;
  
  // Test 8
  reset_str_arithm( str, s, d, e, g );

  sprintf_u( str, UINT_MAX / 5 + 1 );
  unsigned_integer_from_string (4, str, d);
  sprintf_u( str, 5 );
  unsigned_integer_from_string (4, str, e);
  multiply_unsigned_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  sprintf_u( str, UINT_MAX / 2 );
  unsigned_integer_from_string (4, str, d);
  sprintf_u( str, 2 );
  unsigned_integer_from_string (4, str, e);
  multiply_unsigned_integer (4, d, e, d);
  if (overflow_bit)
    result = 1;
  unsigned_integer_to_string (4, d, s);
  sprintf_u( str, (UINT_MAX / 2) * 2 );
  if (strcmp (s, str) != 0)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = 0; i < 4000 ; i++) {
    sprintf_u( str, i );
    unsigned_integer_from_string (4, str, d);
    sprintf_u( str, i / 2 );
    unsigned_integer_from_string (4, str, e);
    multiply_unsigned_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    unsigned_integer_to_string (4, d, s);
    if (atoi (s) != i * (i / 2))
      result = 1;
  }

  result = 0;
  
  // Test 9
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "10", d);
  integer_from_string (4, "0", e);
  divide_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = -2000; i < 2000 ; i++) {
    sprintf_d( str, i );
    integer_from_string (4, str, d);
    sprintf_d( str, (i < 0 ? - i / 20 + 1 : - i / 20 - 1) );
    integer_from_string (4, str, e);
    divide_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    integer_to_string (4, d, s);
    if (atoi (s) != i / (i < 0 ? - i / 20 + 1 : - i / 20 - 1))
      result = 1;
    sprintf_d( str, i );
    integer_from_string (4, str, d);
    divide_integer (4, d, e, e);
    if (overflow_bit)
      result = 1;
    integer_to_string (4, e, s);
    if (atoi (s) != i / (i < 0 ? - i / 20 + 1 : - i / 20 - 1))
      result = 1;
  }

  result = 0;
  
  // Test 10
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "0", e);
  divide_unsigned_integer (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = 0; i < 4000 ; i++) {
    sprintf_u( str, i );
    unsigned_integer_from_string (4, str, d);
    sprintf_u( str, i / 20 + 1 );
    unsigned_integer_from_string (4, str, e);
    divide_unsigned_integer (4, d, e, d);
    if (overflow_bit)
      result = 1;
    unsigned_integer_to_string (4, d, s);
    if (atoi (s) != i / (i / 20 + 1))
      result = 1;
  }
        
  result = 0;
  
  // Test 11
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "0", e);
  unsigned_integer_remainder (4, d, e, d);
  if (!overflow_bit)
    result = 1;
  _Pragma( "loopbound min 4000 max 4000" )
  for (i = 0; i < 4000 ; i++) {
    sprintf_u( str, i );
    unsigned_integer_from_string (4, str, d);
    sprintf_u( str, i / 20 + 1 );
    unsigned_integer_from_string (4, str, e);
    unsigned_integer_remainder (4, d, e, d);
    if (overflow_bit)
      result = 1;
    unsigned_integer_to_string (4, d, s);
    if (atoi (s) != i % (i / 20 + 1))
      result = 1;
  }
  
  result = 0;

  // Test 12
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_shift_right (4, d, 0, d);
  unsigned_integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1348") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_shift_right (4, d, 32, d);
  unsigned_integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "0") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_shift_right (4, d, 8, d);
  unsigned_integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "5") != 0)
    result = 1;
  unsigned_integer_from_string (4, "134890", d);
  unsigned_integer_shift_right (4, d, 13, d);
  unsigned_integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "16") != 0)
    result = 1;
  unsigned_integer_from_string (4, "134890", d);
  unsigned_integer_shift_left (4, d, -13, d);
  unsigned_integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "16") != 0)
    result = 1;

  result = 0;
  
  // Test 13
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "1348", d);
  integer_shift_right (4, d, 0, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1348") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_shift_right (4, d, 32, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "0") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_shift_right (4, d, 8, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "5") != 0)
    result = 1;
  integer_from_string (4, "134890", d);
  integer_shift_right (4, d, 13, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "16") != 0)
    result = 1;
  integer_from_string (4, "134890", d);
  integer_shift_left (4, d, -13, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "16") != 0)
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_shift_right (4, d, 0, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "-1348") != 0)
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_shift_right (4, d, 32, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "-1") != 0)
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_shift_right (4, d, 8, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "-6") != 0)
    result = 1;
  integer_from_string (4, "-134890", d);
  integer_shift_right (4, d, 13, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "-17") != 0)
    result = 1;
  integer_from_string (4, "-134890", d);
  integer_shift_left (4, d, -13, d);
  integer_to_string (4, d, s);
  if (!overflow_bit || strcmp (s, "-17") != 0)
    result = 1;

  result = 0;
  
  // Test 14
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_shift_left (4, d, 0, d);
  unsigned_integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1348") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_shift_left (4, d, 22, d);
  if (!overflow_bit)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_shift_left (4, d, 8, d);
  unsigned_integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "345088") != 0)
    result = 1;
  unsigned_integer_from_string (4, "134890", d);
  unsigned_integer_shift_left (4, d, 13, d);
  unsigned_integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1105018880") != 0)
    result = 1;
  unsigned_integer_from_string (4, "134890", d);
  unsigned_integer_shift_right (4, d, -13, d);
  unsigned_integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1105018880") != 0)
    result = 1;

  result = 0;
  
  // Test 15
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "1348", d);
  integer_shift_left (4, d, 0, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1348") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_shift_left (4, d, 21, d);
  if (!overflow_bit)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_shift_left (4, d, 8, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "345088") != 0)
    result = 1;
  integer_from_string (4, "134890", d);
  integer_shift_left (4, d, 13, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1105018880") != 0)
    result = 1;
  integer_from_string (4, "134890", d);
  integer_shift_right (4, d, -13, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "1105018880") != 0)
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_shift_left (4, d, 0, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "-1348") != 0)
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_shift_left (4, d, 21, d);
  if (!overflow_bit)
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_shift_left (4, d, 8, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "-345088") != 0)
    result = 1;
  integer_from_string (4, "-134890", d);
  integer_shift_left (4, d, 13, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "-1105018880") != 0)
    result = 1;
  integer_from_string (4, "-134890", d);
  integer_shift_right (4, d, -13, d);
  integer_to_string (4, d, s);
  if (overflow_bit || strcmp (s, "-1105018880") != 0)
    result = 1;

  result = 0;
  
  // Test 16
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "-10", d);
  integer_from_string (4, "1348", e);
  if (eq_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_from_string (4, "-1348", e);
  if (!eq_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 17
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "1348", e);
  if (eq_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!eq_unsigned_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 18
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "-10", d);
  integer_from_string (4, "1348", e);
  if (!ne_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_from_string (4, "-1348", e);
  if (ne_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 19
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!ne_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  if (ne_unsigned_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 20
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "-10", d);
  integer_from_string (4, "1348", e);
  if (gt_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_from_string (4, "-1348", e);
  if (gt_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1000000", d);
  integer_from_string (4, "-1348", e);
  if (gt_integer (4, d, e))
    result = 1;
  integer_from_string (4, "1000000", d);
  integer_from_string (4, "1348", e);
  if (!gt_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 21
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "1348", e);
  if (gt_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  if (gt_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1000000", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!gt_unsigned_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 22
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "-10", d);
  integer_from_string (4, "1348", e);
  if (!lt_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_from_string (4, "-1348", e);
  if (lt_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1000000", d);
  integer_from_string (4, "-1348", e);
  if (!lt_integer (4, d, e))
    result = 1;
  integer_from_string (4, "1000000", d);
  integer_from_string (4, "1348", e);
  if (lt_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 23
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!lt_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  if (lt_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1000000", d);
  unsigned_integer_from_string (4, "1348", e);
  if (lt_unsigned_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 24
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "-10", d);
  integer_from_string (4, "1348", e);
  if (ge_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_from_string (4, "-1348", e);
  if (!ge_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1000000", d);
  integer_from_string (4, "-1348", e);
  if (ge_integer (4, d, e))
    result = 1;
  integer_from_string (4, "1000000", d);
  integer_from_string (4, "1348", e);
  if (!ge_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 25
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "1348", e);
  if (ge_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!ge_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1000000", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!ge_unsigned_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 26
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "-10", d);
  integer_from_string (4, "1348", e);
  if (!le_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1348", d);
  integer_from_string (4, "-1348", e);
  if (!le_integer (4, d, e))
    result = 1;
  integer_from_string (4, "-1000000", d);
  integer_from_string (4, "-1348", e);
  if (!le_integer (4, d, e))
    result = 1;
  integer_from_string (4, "1000000", d);
  integer_from_string (4, "1348", e);
  if (le_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 27
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "10", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!le_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  if (!le_unsigned_integer (4, d, e))
    result = 1;
  unsigned_integer_from_string (4, "1000000", d);
  unsigned_integer_from_string (4, "1348", e);
  if (le_unsigned_integer (4, d, e))
    result = 1;

  result = 0;
  
  // Test 28
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "70000", d);
  change_unsigned_integer_size (4, d, 2, d);
  if (!overflow_bit)
    result = 1;
  unsigned_integer_from_string (4, "30000", d);
  change_unsigned_integer_size (4, d, 2, d);
  integer_to_string (2, d, s);
  if (overflow_bit || strcmp (s, "30000") != 0)
    result = 1;
  unsigned_integer_from_string (4, "11230000", g);
  change_unsigned_integer_size (4, g, 6, g);
  integer_to_string (6, g, s);
  if (overflow_bit || strcmp (s, "11230000") != 0)
    result = 1;

  result = 0;
  
  // Test 29
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "40000", d);
  change_integer_size (4, d, 2, d);
  if (!overflow_bit)
    result = 1;
  integer_from_string (4, "-33000", d);
  change_integer_size (4, d, 2, d);
  if (!overflow_bit)
    result = 1;
  integer_from_string (4, "30000", d);
  change_integer_size (4, d, 2, d);
  integer_to_string (2, d, s);
  if (overflow_bit || strcmp (s, "30000") != 0)
    result = 1;
  integer_from_string (4, "-30000", d);
  change_integer_size (4, d, 2, d);
  integer_to_string (2, d, s);
  if (overflow_bit || strcmp (s, "-30000") != 0)
    result = 1;
  integer_from_string (4, "11230000", g);
  change_integer_size (4, g, 6, g);
  integer_to_string (6, g, s);
  if (overflow_bit || strcmp (s, "11230000") != 0)
    result = 1;
  integer_from_string (4, "-11230000", g);
  change_integer_size (4, g, 6, g);
  integer_to_string (6, g, s);
  if (overflow_bit || strcmp (s, "-11230000") != 0)
    result = 1;

  result = 0;
  
  // Test 30
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  unsigned_integer_or (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "0", e);
  unsigned_integer_or (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "4294967295", e);
  unsigned_integer_or (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "4294967295") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "96", e);
  unsigned_integer_or (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "1380") != 0)
    result = 1;

  result = 0;
  
  // Test 31
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "1348", d);
  integer_from_string (4, "1348", e);
  integer_or (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_from_string (4, "0", e);
  integer_or (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_from_string (4, "-1", e);
  integer_or (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "-1") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_from_string (4, "96", e);
  integer_or (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "1380") != 0)
    result = 1;

  result = 0;
  
  // Test 32
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "1348", e);
  unsigned_integer_and (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "0", e);
  unsigned_integer_and (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "0") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "4294967295", e);
  unsigned_integer_and (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_from_string (4, "96", e);
  unsigned_integer_and (4, d, e, e);
  unsigned_integer_to_string (4, e, s);
  if (strcmp (s, "64") != 0)
    result = 1;

  result = 0;
  
  // Test 33
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "1348", d);
  integer_from_string (4, "1348", e);
  integer_and (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_from_string (4, "0", e);
  integer_and (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "0") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_from_string (4, "-1", e);
  integer_and (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "1348") != 0)
    result = 1;
  integer_from_string (4, "1348", d);
  integer_from_string (4, "96", e);
  integer_and (4, d, e, e);
  integer_to_string (4, e, s);
  if (strcmp (s, "64") != 0)
    result = 1;

  result = 0;
  
  // Test 34
  reset_str_arithm( str, s, d, e, g );

  unsigned_integer_from_string (4, "1348", d);
  unsigned_integer_not (4, d, d);
  unsigned_integer_to_string (4, d, s);
  if (strcmp (s, "4294965947") != 0)
    result = 1;
  unsigned_integer_from_string (4, "0", d);
  unsigned_integer_not (4, d, d);
  unsigned_integer_to_string (4, d, s);
  if (strcmp (s, "4294967295") != 0)
    result = 1;
  unsigned_integer_from_string (4, "4294967295", d);
  unsigned_integer_not (4, d, d);
  unsigned_integer_to_string (4, d, s);
  if (strcmp (s, "0") != 0)
    result = 1;

  result = 0;
  
  // Test 35
  reset_str_arithm( str, s, d, e, g );

  integer_from_string (4, "1348", d);
  integer_not (4, d, d);
  integer_to_string (4, d, s);
  if (strcmp (s, "-1349") != 0)
    result = 1;
  integer_from_string (4, "0", d);
  integer_not (4, d, d);
  integer_to_string (4, d, s);
  if (strcmp (s, "-1") != 0)
    result = 1;
  integer_from_string (4, "-1", d);
  integer_not (4, d, d);
  integer_to_string (4, d, s);
  if (strcmp (s, "0") != 0)
    result = 1;

  return result;
}


int main( void )
{
  bits_test();
  arithm_test();
  return 0;
}
