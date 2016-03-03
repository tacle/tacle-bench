/*
  FILE NAME:   arithm.c

  TITLE:       Package for arbitrary precision integer arithmetic

  DESCRIPTION: This abstract data implements arbitrary precision
      integer and unsigned integer numbers by machine independent
      way.  The implementation of the package functions are not
      sufficiently efficient in order to use for run-time.  The
      package functions are oriented to implement constant-folding in
      compilers.  This package is necessary because host machine may
      not support such arithmetic for target machine.  For example,
      VAX does not support does not support more 32-bits integer
      numbers arithmetic.  The numbers are represented by bytes in
      big endian mode, negative integer numbers are represented in
      complementary code.  All sizes are given in bytes and must be
      positive.  Results of executions of all functions can coincide
      with a operand(s).  All functions of addition, subtraction,
      multiplication, division, evaluation of remainder, shift,
      changing size and transformation of string into number fix
      overflow.  The overflow is fixed when result can not be
      represented by number of given size.

*/

#include "arithm.h"


typedef unsigned int size_t;

/* This variable can have only two values 0 or 1.  The value `1'
   corresponds to overflow.  The variable value are modified by all
   functions of addition, subtract, multiplication, division,
   evaluation of remainder, shift, changing size and transformation of
   string into number fix overflow. */

int overflow_bit;

/* This variable stores zero (unsigned) integer of maximal size.
   Remember that this array is initiated by 0. */

static unsigned char zero_constant_itself [MAX_INTEGER_OPERAND_SIZE];

/* This variable represents zero (unsigned) integer of any size. */

unsigned const char *zero_constant = zero_constant_itself;


char isdigit( unsigned char c )
{
  if ( c >= '0' & c <= '9' )
    return 1;
  else
    return 0;
}

int isspace( int c )
{
  return c == ' ' | c == '\t' | c == '\n' | c == '\r';
}



/* This page contains functions common for all package functions. */

static void *
ammunition_memcpy ( void *to, const void *from, size_t size )
{
  char *cto = ( char * ) to;
  const char *cfrom = ( const char * ) from;

  _Pragma( "loopbound min 2 max 6" )
  while ( size > 0 ) {
    *cto++ = *cfrom;
    size--;
  }
  return to;
}


static void *
ammunition_memset ( void *to, int value, size_t size )
{
  char *cto = ( char * ) to;

  _Pragma( "loopbound min 0 max 4" )
  while ( size > 0 ) {
    *cto++ = value;
    size--;
  }
  return to;
}

static int
ammunition_memcmp ( const void *mem1, const void *mem2, size_t size )
{
  const unsigned char *m1 = ( unsigned const char * )mem1;
  const unsigned char *m2 = ( unsigned const char * )mem2;

  _Pragma( "loopbound min 0 max 0" )
  while ( size > 0 ) {
    if ( m1 != m2 )
      return ( m1 < m2 ? -1 : 1 );
    m1++;
    m2++;
    size--;
  }
  return 0;
}


/* The following function is an analog of standard C function
   `memmove'.  The function returns the first operand. */

static void *
ammunition_memmove ( void *s1, const void *s2, size_t n )
{
  int i;

  if ( ( ( char * ) s1 < ( char * ) s2 && ( char * ) s1 + n <= ( char * ) s2 )
       || ( ( char * ) s2 < ( char * ) s1 && ( char * ) s2 + n <= ( char * ) s1 ) )
    return ( void * ) ammunition_memcpy ( s1, s2, n );
  if ( ( char * ) s1 < ( char * ) s2 && ( char * ) s1 + n > ( char * ) s2 ) {
    _Pragma( "loopbound min 0 max 4" )
    for ( i = 0; ( size_t ) i < n; i++ )
      ( ( char * ) s1 ) [i] = ( ( char * ) s2 ) [i];
  } else {
    _Pragma( "loopbound min 0 max 4" )
    for ( i = n - 1; i >= 0; i-- )
      ( ( char * ) s1 )[i] = ( ( char * ) s2 ) [i];
  }
  return s1;
}

/* The following function adds unsigned integers.  The function
   returns 1 if unsigned integer overflow is fixed, 0 otherwise.
   Result can be placed in any operand. */

static int
add_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int digit_number;
  int carry;
  unsigned int sum;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_number = size - 1, carry = 0; digit_number >= 0; digit_number-- ) {
    sum = ( ( ( unsigned char * ) op1 ) [digit_number]
            + ( ( unsigned char * ) op2 ) [digit_number] + carry );
    if ( sum > UCHAR_MAX ) {
      sum -= UCHAR_MAX + 1;
      carry = 1;
    } else
      carry = 0;
    ( ( unsigned char * ) result ) [digit_number] = sum;
  }
  return carry != 0;
}

/* The following function adds unsigned integers.  The function
   returns 1 if unsigned integer overflow (the first operand is less
   than the second) is fixed, 0 otherwise.  Result can be placed in
   any operand. */

static int
subtract_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int digit_number;
  int carry;
  int subtraction;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_number = size - 1, carry = 0; digit_number >= 0; digit_number-- ) {
    subtraction = ( ( ( unsigned char * ) op1 ) [digit_number]
                    - ( ( unsigned char * ) op2 ) [digit_number] - carry );
    if ( subtraction < 0 ) {
      subtraction += UCHAR_MAX + 1;
      carry = 1;
    } else
      carry = 0;
    ( ( unsigned char * ) result ) [digit_number] = subtraction;
  }
  return carry != 0;
}

/* The following function makes complementary code of number.  Result
   can be placed in operand. */

static void
make_complementary_code ( int size, const void *operand, void *result )
{
  int digit_number;
  int carry;
  int subtraction;

  _Pragma( "loopbound min 2 max 6" )
  for ( digit_number = size - 1, carry = 0; digit_number >= 0; digit_number-- ) {
    subtraction = ( 0 - ( ( unsigned char * ) operand ) [digit_number] - carry );
    if ( subtraction != 0 ) {
      subtraction += UCHAR_MAX + 1;
      carry = 1;
    } else
      carry = 0;
    ( ( unsigned char * ) result ) [digit_number] = subtraction;
  }
}

/* The following function multiplys unsigned integer by digit (byte
   size).  The function returns 1 if unsigned integer overflow is
   fixed, 0 otherwise. */

static int
multiply_unsigned_integer_by_digit_without_overflow_reaction
( int size, void *operand, unsigned int digit )
{
  int digit_number;
  unsigned int carry;
  unsigned int sum;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_number = size - 1, carry = 0; digit_number >= 0; digit_number-- ) {
    sum = ( ( ( unsigned char * ) operand ) [digit_number] * digit + carry );
    if ( sum > UCHAR_MAX ) {
      carry = sum / ( UCHAR_MAX + 1 );
      sum %= UCHAR_MAX + 1;
    } else
      carry = 0;
    ( ( unsigned char * ) operand ) [digit_number] = sum;
  }
  return carry != 0;
}


/* Originally reaction on all integer and unsigned integer overflow is
   equal to the following function.  The function does nothing. */

void
arithmetic_overflow_reaction ( void )
{}


/* Originally reaction on all integer and unsigned integer overflow is
   equal to the following function.  The function does nothing. */

void
arithmetic_unsigned_overflow_reaction ( void )
{}


/* This page contains functions creating arbitrary precision minimal
   and maximum constants. */

/* The function creates given size maximal unsigned integer
   constant. */

void
unsigned_integer_maximum ( int size, void *result )
{
  ammunition_memset ( result, UCHAR_MAX, ( size_t ) size );
}

/* The function creates given size minimal integer constant. */

void
integer_minimum ( int size, void *result )
{
  ammunition_memset ( result, 0, ( size_t ) size );
  *( unsigned char * ) result = 1 << ( CHAR_BIT - 1 );
}

/* The function creates given size maximal integer constant. */

void
integer_maximum ( int size, void *result )
{
  ammunition_memset ( result, UCHAR_MAX, ( size_t ) size );
  *( unsigned char * ) result = UCHAR_MAX >> 1;
}



/* This page contains functions for arbitrary precision addition. */

/* The function adds unsigned integers and fixes overflow reaction if
   it is needed.  The function makes this with the aid of function
   `add_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
add_unsigned_integer ( int size, const void *op1, const void *op2,
                       void *result )
{
  overflow_bit
    = add_unsigned_integer_without_overflow_reaction ( size, op1, op2, result );
  if ( overflow_bit != 0 )
    arithmetic_unsigned_overflow_reaction();
}

/* The function adds integers and fixes overflow reaction if it is
   needed.  The function makes this with the aid of function
   `add_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
add_integer ( int size, const void *op1, const void *op2, void *result )
{
  int op1_sign;
  int sign_equality;

  op1_sign = INTEGER_SIGN ( op1 );
  sign_equality = INTEGER_SIGN ( op1 ) == INTEGER_SIGN ( op2 );
  add_unsigned_integer_without_overflow_reaction ( size, op1, op2, result );
  overflow_bit = sign_equality && ( op1_sign != INTEGER_SIGN ( result ) );
  if ( overflow_bit != 0 )
    arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision subtraction. */

/* The function subtracts unsigned integers and fixes overflow
   reaction if it is needed.  The function makes this with the aid of
   function `subtract_unsigned_integer_without_overflow_reaction'.
   Result can be placed in any operand. */

void
subtract_unsigned_integer ( int size, const void *op1, const void *op2,
                            void *result )
{
  overflow_bit
    = subtract_unsigned_integer_without_overflow_reaction ( size, op1, op2,
        result );
  if ( overflow_bit != 0 )
    arithmetic_unsigned_overflow_reaction();
}

/* The function subtracts integers and fixes overflow reaction if it
   is needed.  The function makes this with the aid of function
   `subtract_unsigned_integer_without_overflow_reaction'.  Result can
   be placed in any operand. */

void
subtract_integer ( int size, const void *op1, const void *op2, void *result )
{
  int op1_sign;
  int sign_unequality;

  op1_sign = INTEGER_SIGN ( op1 );
  sign_unequality = INTEGER_SIGN ( op1 ) != INTEGER_SIGN ( op2 );
  subtract_unsigned_integer_without_overflow_reaction ( size, op1, op2, result );
  overflow_bit = sign_unequality && ( op1_sign != INTEGER_SIGN ( result ) );
  if ( overflow_bit != 0 )
    arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision multiplication. */

/* The following function multiplys unsigned integers.  The function
   returns 1 if unsigned integer overflow is fixed, 0 otherwise.
   Result can be placed in any operand. */

static int
multiply_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int op1_digit_number;
  int op2_digit_number;
  int carry;
  unsigned long int partial_sum;
  int result_digit_number;
  int overflow_flag;
  unsigned char long_result [2 * MAX_INTEGER_OPERAND_SIZE];

  ammunition_memset ( long_result + size, 0, ( size_t ) size );
  _Pragma( "loopbound min 4 max 4" )
  for ( op2_digit_number = size - 1; op2_digit_number >= 0; op2_digit_number-- ) {
    if ( ( ( unsigned char * ) op2 ) [op2_digit_number] != 0 ) {
      _Pragma( "loopbound min 4 max 4" )
      for ( op1_digit_number = size - 1, carry = 0; op1_digit_number >= 0;
            op1_digit_number-- ) {
        partial_sum
          = ( ( ( unsigned char * ) op1 ) [op1_digit_number]
              * ( ( unsigned char * ) op2 ) [op2_digit_number]
              + long_result [op1_digit_number + op2_digit_number + 1]
              + carry );
        long_result [op1_digit_number + op2_digit_number + 1]
          = ( unsigned char ) ( partial_sum % ( UCHAR_MAX + 1 ) );
        carry = partial_sum / ( UCHAR_MAX + 1 );
      }
      long_result [op2_digit_number] = carry;
    } else
      long_result [op2_digit_number] = 0;
  }
  overflow_flag = 0;
  _Pragma( "loopbound min 1 max 4" )
  for ( result_digit_number = size - 1; result_digit_number >= 0;
        result_digit_number-- ) {
    if ( long_result [result_digit_number] != 0 ) {
      overflow_flag = 1;
      break;
    }
  }
  ammunition_memcpy ( result, long_result + size, ( size_t ) size );
  return overflow_flag;
}

/* The following function multiplys unsigned integers and fixes
   overflow reaction if it is needed.  The function makes this with
   the aid of function
   `multiply_unsigned_integer_without_overflow_reaction'.  Result can
   be placed in any operand. */

void
multiply_unsigned_integer ( int size, const void *op1, const void *op2,
                            void *result )
{
  overflow_bit =
    multiply_unsigned_integer_without_overflow_reaction ( size, op1, op2,
        result );
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
}

/* The function multiplys integers and fixes overflow reaction if it
   is needed.  The function makes this with the aid of function
   `multiply_unsigned_integer_without_overflow_reaction'.  Result can
   be placed in any operand. */

void
multiply_integer ( int size, const void *op1, const void *op2, void *result )
{
  int negative_result_flag;
  unsigned char op1_complementary [MAX_INTEGER_OPERAND_SIZE];
  unsigned char op2_complementary [MAX_INTEGER_OPERAND_SIZE];
  unsigned const char *abs_op1;
  unsigned const char *abs_op2;
  int unsigned_result_sign;

  negative_result_flag = INTEGER_SIGN ( op1 ) != INTEGER_SIGN ( op2 );
  if ( INTEGER_SIGN ( op1 ) ) {
    /* May be integer overflow. But result is correct because
       it is unsigned. */
    make_complementary_code ( size, op1, op1_complementary );
    abs_op1 = ( unsigned const char * )op1_complementary;
  } else
    abs_op1 = ( unsigned const char * )op1;
  if ( INTEGER_SIGN ( op2 ) ) {
    /* May be integer overflow.  But result is correct because
       it is unsigned. */
    make_complementary_code ( size, op2, op2_complementary );
    abs_op2 = ( unsigned const char * )op2_complementary;
  } else
    abs_op2 = ( unsigned const char * )op2;
  overflow_bit =
    multiply_unsigned_integer_without_overflow_reaction ( size, abs_op1,
        abs_op2, result );
  unsigned_result_sign = INTEGER_SIGN ( result );
  if ( negative_result_flag )
    make_complementary_code ( size, result, result );
  if ( unsigned_result_sign
       && ( !negative_result_flag
            || INTEGER_SIGN ( result ) != unsigned_result_sign ) )
    /* Unsigned result can not be represented as integer. */
    overflow_bit = 1;
  if ( overflow_bit )
    arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision division. */

/* The following function divides unsigned integers.  The function
   returns 1 if unsigned integer overflow (division by zero) is fixed,
   0 otherwise.  Result can be placed in any operand.  See algorithm
   in Knuth's book. */

static int
divide_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int scaled_op1_digit_number;
  unsigned int q_approximation;
  int first_nonzero_digit_number;
  int op2_digit_number;
  unsigned int scale;
  unsigned char scaled_op1 [MAX_INTEGER_OPERAND_SIZE + 1];
  unsigned char normalized_op2 [MAX_INTEGER_OPERAND_SIZE];
  unsigned char extended_normalized_op2 [MAX_INTEGER_OPERAND_SIZE + 1];

  _Pragma( "loopbound min 4 max 4" )
  for ( op2_digit_number = 0; op2_digit_number < size; op2_digit_number++ ) {
    if ( ( ( unsigned char * ) op2 ) [op2_digit_number] != 0 )
      break;
  }
  first_nonzero_digit_number = op2_digit_number;
  if ( first_nonzero_digit_number == size ) {
    /* Zero divisor */
    ammunition_memset ( result, 0, ( size_t ) size );
    return 1 /* TRUE */;
  } else
    if ( first_nonzero_digit_number == size - 1 ) {
      /* Division by digit. */
      int digit_number;
      int digit;
      unsigned long divisable;
      unsigned long remainder;

      digit = ( ( unsigned char * ) op2 ) [first_nonzero_digit_number];
      ammunition_memcpy ( result, op1, ( size_t ) size );
      remainder = 0;
      _Pragma( "loopbound min 4 max 4" )
      for ( digit_number = 0; digit_number < size; digit_number++ ) {
        divisable = ( remainder * ( UCHAR_MAX + 1 )
                      + ( ( unsigned char * ) result ) [digit_number] );
        remainder = divisable % digit;
        ( ( unsigned char * ) result ) [digit_number]
          = ( unsigned char ) ( divisable / digit );
      }
      return 0 /* FALSE */;
    }
  /* Normalization of divisor. */
  scale = ( UCHAR_MAX + 1 ) / ( ( ( unsigned char * ) op2 ) [op2_digit_number] +
                                1 );
  ammunition_memcpy ( scaled_op1 + 1, op1, ( size_t ) size );
  *scaled_op1 = 0;

  multiply_unsigned_integer_by_digit_without_overflow_reaction
  ( size + 1, scaled_op1, scale );

  ammunition_memcpy ( normalized_op2, op2, ( size_t ) size );

  multiply_unsigned_integer_by_digit_without_overflow_reaction
  ( size, normalized_op2, scale );

  _Pragma( "loopbound min 0 max 0" )
  for ( scaled_op1_digit_number = 0;
        scaled_op1_digit_number <= first_nonzero_digit_number;
        scaled_op1_digit_number++ ) {
    /* Division of `scaled_op1[scaled_op1_digit_number]..scaled_op1[size]' by
       `normalized_op2[first_nonzero_digit_number]..normalized_op2[size-1]'
       for evaluation of one digit of quotient
       `result[size-1-first_nonzero_digit_number-scaled_op1_digit_number]'.
    */
    if ( scaled_op1 [scaled_op1_digit_number]
         == normalized_op2 [first_nonzero_digit_number] )
      q_approximation = UCHAR_MAX;
    else
      q_approximation
        = ( scaled_op1 [scaled_op1_digit_number] * ( UCHAR_MAX + 1 )
            + scaled_op1 [scaled_op1_digit_number + 1] )
          / normalized_op2 [first_nonzero_digit_number];

    _Pragma( "loopbound min 0 max 0" )
    while ( normalized_op2 [first_nonzero_digit_number + 1] * q_approximation
            > ( ( ( unsigned long int ) scaled_op1 [scaled_op1_digit_number]
                  * ( UCHAR_MAX + 1 )
                  + scaled_op1 [scaled_op1_digit_number + 1]
                  - q_approximation
                  * normalized_op2 [first_nonzero_digit_number] )
                * ( UCHAR_MAX + 1 ) + scaled_op1 [scaled_op1_digit_number + 2] ) )
      q_approximation --;

    /* Multiply and subtract */
    ammunition_memcpy ( extended_normalized_op2 + 1,
                        normalized_op2 + first_nonzero_digit_number,
                        ( size_t ) ( size - first_nonzero_digit_number ) );
    *extended_normalized_op2 = 0;
    multiply_unsigned_integer_by_digit_without_overflow_reaction
    ( size - first_nonzero_digit_number + 1, extended_normalized_op2,
      q_approximation );
    if ( subtract_unsigned_integer_without_overflow_reaction
         ( size - first_nonzero_digit_number + 1,
           scaled_op1 + scaled_op1_digit_number, extended_normalized_op2,
           scaled_op1 + scaled_op1_digit_number ) ) {
      /* Negative result.  Compensation by addition. */
      q_approximation--;
      ammunition_memcpy ( extended_normalized_op2 + 1,
                          normalized_op2 + first_nonzero_digit_number,
                          ( size_t ) ( size - first_nonzero_digit_number ) );
      *extended_normalized_op2 = 0;

      add_unsigned_integer_without_overflow_reaction
      ( size - first_nonzero_digit_number + 1,
        scaled_op1 + scaled_op1_digit_number, extended_normalized_op2,
        scaled_op1 + scaled_op1_digit_number );

    }
    ( ( unsigned char * ) result ) [size - 1 - first_nonzero_digit_number
                                    + scaled_op1_digit_number] = q_approximation;
  }
  ammunition_memset ( result, 0,
                      ( size_t ) ( size - 1 - first_nonzero_digit_number ) );
  return 0 /* TRUE */;
}

/* The function divides unsigned integers and fixes overflow reaction
   if it is needed.  The function makes this with the aid of function
   `divide_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
divide_unsigned_integer ( int size, const void *op1, const void *op2,
                          void *result )
{
  overflow_bit =
    divide_unsigned_integer_without_overflow_reaction ( size, op1, op2, result );
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
}

/* The function divides integers and fixes overflow reaction if it is
   needed.  The function makes this with the aid of function
   `divide_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
divide_integer ( int size, const void *op1, const void *op2, void *result )
{
  int negative_result_flag;
  unsigned char op1_complementary [MAX_INTEGER_OPERAND_SIZE];
  unsigned char op2_complementary [MAX_INTEGER_OPERAND_SIZE];
  unsigned const char *abs_op1;
  unsigned const char *abs_op2;
  int unsigned_result_sign;

  negative_result_flag = INTEGER_SIGN ( op1 ) != INTEGER_SIGN ( op2 );
  if ( INTEGER_SIGN ( op1 ) ) {
    /* May be integer overflow for minimal int. But result is correct because
       it is unsigned. */
    make_complementary_code ( size, op1, op1_complementary );
    abs_op1 = ( unsigned const char * )op1_complementary;
  } else
    abs_op1 = ( unsigned const char * )op1;
  if ( INTEGER_SIGN ( op2 ) ) {
    /* May be integer overflow for minimal int.  But result is correct
       because it is unsigned. */
    make_complementary_code ( size, op2, op2_complementary );
    abs_op2 = ( unsigned const char * )op2_complementary;
  } else
    abs_op2 = ( unsigned const char * )op2;
  overflow_bit =
    divide_unsigned_integer_without_overflow_reaction ( size, abs_op1,
        abs_op2, result );
  unsigned_result_sign = INTEGER_SIGN ( result );
  if ( negative_result_flag )
    make_complementary_code ( size, result, result );
  if ( unsigned_result_sign
       && ( !negative_result_flag
            || INTEGER_SIGN ( result ) != unsigned_result_sign ) )
    /* Unsigned result can not be represented as integer. */
    overflow_bit = 1;
  if ( overflow_bit )
    arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision evaluation of
   remainder. */

/* The function evaluates remainder of division of unsigned integers
   as `op1 - (op1/op2)*op2' and fixes overflow reaction if it is
   needed.  Result can be placed in any operand. */

void
unsigned_integer_remainder ( int size, const void *op1, const void *op2,
                             void *result )
{
  unsigned char temporary [MAX_INTEGER_OPERAND_SIZE];

  divide_unsigned_integer ( size, op1, op2, temporary );
  if ( overflow_bit )
    /* Reaction on zero is called from `divide_unsigned_integer'. */
    ammunition_memset ( result, 0, ( size_t ) size );
  else {
    multiply_unsigned_integer ( size, temporary, op2, temporary );
    subtract_unsigned_integer ( size, op1, temporary, result );
  }
}

/* The function evaluates remainder of division of integers as `op1 -
   (op1/op2)*op2' and fixes overflow reaction if it is needed.  Result
   can be placed in any operand. */

void
integer_remainder ( int size, const void *op1, const void *op2, void *result )
{
  unsigned char temporary [MAX_INTEGER_OPERAND_SIZE];

  divide_integer ( size, op1, op2, temporary );
  if ( overflow_bit )
    /* Reaction on zero is called from `divide_integer'. */
    ammunition_memset ( result, 0, ( size_t ) size );
  else {
    multiply_integer ( size, temporary, op2, temporary );
    subtract_integer ( size, op1, temporary, result );
  }
}



/* This page contains functions for arbitrary precision number shifts. */

/* This function makes right shift of unsigned integer of given size
   on given number of bits.  If number of bits is negative the
   function makes shift to left actually with the aid of function
   `unsigned_integer_shift_left'.  The function fixes overflow when
   result can not be represented by number of given size, i.e. in
   other words the opposite unsigned shift (to left) results in number
   not equal to source operand.  Result can be placed in operand. */

void
unsigned_integer_shift_right ( int size, const void *operand,
                               unsigned int bits, void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;

  overflow_bit = 0;
  byte_shift = bits / CHAR_BIT;
  bit_shift = bits % CHAR_BIT;
  _Pragma( "loopbound min 0 max 3" )
  for ( byte_number = ( byte_shift >= size ? 0 : size - byte_shift );
        byte_number < size; byte_number++ ) {
    if ( ( ( unsigned char * ) operand ) [byte_number] != 0 ) {
      overflow_bit = 1;
      break;
    }
  }
  if ( byte_shift >= size )
    ammunition_memset ( result, 0, ( size_t ) size );
  else {
    ammunition_memmove ( ( char * ) result + byte_shift, operand,
                         ( size_t ) ( size - byte_shift ) );
    ammunition_memset ( result, 0, ( size_t ) byte_shift );
    if ( bit_shift == 0 )
      return ;
    _Pragma( "loopbound min 3 max 3" )
    for ( byte_number = byte_shift, carry = 0; byte_number < size;
          byte_number++ ) {
      byte = ( ( unsigned char * ) result ) [byte_number];
      ( ( unsigned char * ) result ) [byte_number]
        = carry | ( byte >> bit_shift );
      carry = ( byte << ( CHAR_BIT - bit_shift ) ) & UCHAR_MAX;
    }
    if ( carry != 0 )
      overflow_bit = 1;
  }
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
}

/* This function makes right arithmetic shift of integer of given size
   on given number of bits.  If number of bits is negative the
   function makes shift to left actually with the aid of function
   `integer_shift_left'.  The function fixes overflow when result can
   not be represented by number of given size, i.e. in other words the
   opposite shift (to left) results in number not equal to source
   operand.  Result can be placed in operand. */

void
integer_shift_right ( int size, const void *operand, unsigned int bits,
                      void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;
  int operand_sign;

  operand_sign = INTEGER_SIGN ( operand );
  overflow_bit = 0;
  byte_shift = bits / CHAR_BIT;
  bit_shift = bits % CHAR_BIT;
  _Pragma( "loopbound min 0 max 3" )
  for ( byte_number = ( byte_shift >= size ? 0 : size - byte_shift );
        byte_number < size; byte_number++ ) {
    if ( ( ( unsigned char * ) operand ) [byte_number] != 0 ) {
      overflow_bit = 1;
      break;
    }
  }
  if ( byte_shift >= size )
    ammunition_memset ( result, ( operand_sign ? UCHAR_MAX : 0 ), ( size_t ) size );
  else {
    ammunition_memmove ( ( char * ) result + byte_shift, operand,
                         ( size_t ) ( size - byte_shift ) );
    ammunition_memset ( result, ( operand_sign ? UCHAR_MAX : 0 ),
                        ( size_t ) byte_shift );
    if ( bit_shift == 0 )
      return ;
    carry = ( ( ( operand_sign ? UCHAR_MAX : 0 ) << ( CHAR_BIT - bit_shift ) )
              & UCHAR_MAX );
    _Pragma( "loopbound min 3 max 3" )
    for ( byte_number = byte_shift; byte_number < size; byte_number++ ) {
      byte = ( ( unsigned char * ) result ) [byte_number];
      ( ( unsigned char * ) result ) [byte_number]
        = carry | ( byte >> bit_shift );
      carry = ( byte << ( CHAR_BIT - bit_shift ) ) & UCHAR_MAX;
    }
    if ( carry != 0 )
      overflow_bit = 1;
  }
  if ( overflow_bit )
    arithmetic_overflow_reaction();
}

/* This function makes left shift of unsigned integer of given size on
   given number of bits.  If number of bits is negative the function
   makes shift to left actually with the aid of function
   `unsigned_integer_shift_right'.  The function fixes overflow when
   result can not be represented by number of given size, i.e. i.e. in
   other words the opposite shift (to right) results in number not
   equal to source operand.  Result can be placed in operand. */

void
unsigned_integer_shift_left ( int size, const void *operand,
                              unsigned int bits, void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;

  overflow_bit = 0;
  byte_shift = bits / CHAR_BIT;
  bit_shift = bits % CHAR_BIT;
  _Pragma( "loopbound min 0 max 2" )
  for ( byte_number = 0; byte_number < byte_shift && byte_number < size;
        byte_number++ ) {
    if ( ( ( unsigned char * ) operand ) [byte_number] != 0 ) {
      overflow_bit = 1;
      break;
    }
  }
  if ( byte_shift >= size )
    ammunition_memset ( result, 0, ( size_t ) size );
  else {
    ammunition_memmove ( result, ( char * ) operand + byte_shift,
                         ( size_t ) ( size - byte_shift ) );
    ammunition_memset ( ( char * ) result + ( size - byte_shift ), 0,
                        ( size_t ) byte_shift );
    if ( bit_shift == 0 )
      return ;
    _Pragma( "loopbound min 2 max 3" )
    for ( byte_number = size - byte_shift - 1, carry = 0;
          byte_number >= 0; byte_number-- ) {
      byte = ( ( unsigned char * ) result ) [byte_number];
      ( ( unsigned char * ) result ) [byte_number]
        = carry | ( byte << bit_shift );
      carry = byte >> ( CHAR_BIT - bit_shift );
    }
    if ( carry != 0 )
      overflow_bit = 1;
  }
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
}

/* This function makes left arithmetic shift of integer of given size
   on given number of bits.  If number of bits is negative the
   function makes shift to left actually with the aid of function
   `integer_shift_right'.  The function fixes overflow when result can
   not be represented by number of given size, i.e. in other words the
   opposite shift (to right) results in number not equal to source
   operand.  Result can be placed in operand. */

void
integer_shift_left ( int size, const void *operand, unsigned int bits,
                     void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;
  int operand_sign;

  operand_sign = INTEGER_SIGN ( operand );
  overflow_bit = 0;
  byte_shift = bits / CHAR_BIT;
  bit_shift = bits % CHAR_BIT;
  _Pragma( "loopbound min 0 max 2" )
  for ( byte_number = 0; byte_number < byte_shift && byte_number < size;
        byte_number++ ) {
    if ( ( ( unsigned char * ) operand ) [byte_number]
         != ( operand_sign ? UCHAR_MAX : 0 ) ) {
      overflow_bit = 1;
      break;
    }
  }
  if ( byte_shift >= size )
    ammunition_memset ( result, 0, ( size_t ) size );
  else {
    ammunition_memmove ( result, ( char * ) operand + byte_shift,
                         ( size_t ) ( size - byte_shift ) );
    ammunition_memset ( ( char * ) result + ( size - byte_shift ), 0,
                        ( size_t ) byte_shift );
    if ( bit_shift == 0 )
      return ;
    _Pragma( "loopbound min 2 max 3" )
    for ( byte_number = size - byte_shift - 1, carry = 0;
          byte_number >= 0; byte_number-- ) {
      byte = ( ( unsigned char * ) result ) [byte_number];
      ( ( unsigned char * ) result ) [byte_number]
        = carry | ( byte << bit_shift );
      carry = byte >> ( CHAR_BIT - bit_shift );
    }
    if ( carry != ( ( unsigned ) ( operand_sign ? UCHAR_MAX : 0 )
                    >> ( CHAR_BIT - bit_shift ) ) )
      overflow_bit = 1;
  }
  if ( operand_sign != INTEGER_SIGN ( result ) )
    overflow_bit = 1;
  if ( overflow_bit )
    arithmetic_overflow_reaction();
}



/* This page contains functions for bitwise operations of arbitrary
   precision numbers. */

/* This function makes bitwise `or' of two integers of given size. */

void
integer_or ( int size, const void *op1, const void *op2, void *result )
{
  int byte_number;

  _Pragma( "loopbound min 4 max 4" )
  for ( byte_number = 0; byte_number < size; byte_number++ ) {
    ( ( unsigned char * ) result ) [byte_number]
      = ( ( unsigned char * ) op1 ) [byte_number]
        | ( ( unsigned char * ) op2 ) [byte_number];
  }
}

/* This function makes bitwise `or' of two unsigned integers of given
   size. */

void
unsigned_integer_or ( int size, const void *op1, const void *op2, void *result )
{
  integer_or ( size, op1, op2, result );
}


/* This function makes bitwise `and' of two integers of given size. */

void
integer_and ( int size, const void *op1, const void *op2, void *result )
{
  int byte_number;

  _Pragma( "loopbound min 4 max 4" )
  for ( byte_number = 0; byte_number < size; byte_number++ ) {
    ( ( unsigned char * ) result ) [byte_number]
      = ( ( unsigned char * ) op1 ) [byte_number]
        & ( ( unsigned char * ) op2 ) [byte_number];
  }
}

/* This function makes bitwise `and' of two unsigned integers of given
   size. */

void
unsigned_integer_and ( int size, const void *op1, const void *op2,
                       void *result )
{
  integer_and ( size, op1, op2, result );
}


/* This function makes bitwise `not' of integer of given size. */

void
integer_not ( int size, const void *operand, void *result )
{
  int byte_number;

  _Pragma( "loopbound min 4 max 4" )
  for ( byte_number = 0; byte_number < size; byte_number++ ) {
    ( ( unsigned char * ) result ) [byte_number]
      = ( ( unsigned char * ) operand ) [byte_number] ^ UCHAR_MAX;
  }
}

/* This function makes bitwise `not' of unsigned integer of given
   size. */

void
unsigned_integer_not ( int size, const void *operand, void *result )
{
  integer_not ( size, operand, result );
}



/* This page contains functions for comparison of arbitrary precision
   numbers. */

/* This function compares two unsigned integers of given size on
   equality.  The function returns 1 if unsigned integers are equal, 0
   otherwise. */

int
eq_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) == 0;
}

/* This function compares two integers of given size on equality.  The
   function returns 1 if integers are equal, 0 otherwise. */

int
eq_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) == 0;
}

/* This function compares two unsigned integers of given size on
   inequality.  The function returns 1 if unsigned integers are not
   equal, 0 otherwise. */

int
ne_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) != 0;
}

/* This function compares two integers of given size on inequality.
   The function returns 1 if integers are not equal, 0 otherwise. */

int
ne_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) != 0;
}


/* This function compares two memory parts of given size on that the
   first operand is greater than the second.  The bytes are described
   as unsigned.  The function returns 1 if the first operand is
   greater than the second, - 1 if the first operand is less than the
   second, 0 otherwise. */

static int
bytes_comparison ( const void *op1, const void *op2, int size )
{
  const unsigned char *str1 = ( unsigned const char * )op1;
  const unsigned char *str2 = ( unsigned const char * )op2;

  _Pragma( "loopbound min 1 max 4" )
  while ( size > 0 && *str1 == *str2 ) {
    str1++;
    str2++;
    size--;
  }
  if ( size <= 0 )
    return 0;
  else
    if ( *str1 > *str2 )
      return 1;
    else
      return -1;
}

/* This function compares two unsigned integers of given size on that
   the first operand is greater than the second.  The function returns
   1 if the first unsigned integer is greater than the second, 0
   otherwise. */

int
gt_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return bytes_comparison ( op1, op2, size ) > 0;
}

/* This function compares two integers of given size on that the first
   operand is greater than the second.  The function returns 1 if the
   first integer is greater than the second, 0 otherwise. */

int
gt_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return bytes_comparison ( op1, op2, size ) > 0;
    else
      return 1; /* TRUE */
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 0; /*FALSE*/
    else
      return bytes_comparison ( op1, op2, size ) > 0;
}

/* This function compares two unsigned integers of given size on that
   the first operand is less than the second.  The function returns 1
   if the first unsigned integer is less than the second, 0
   otherwise. */

int
lt_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return bytes_comparison ( op1, op2, size ) < 0;
}

/* This function compares two integers of given size on that the first
   operand is less than the second.  The function returns 1 if the
   first integer is less than the second, 0 otherwise. */

int
lt_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return bytes_comparison ( op1, op2, size ) < 0;
    else
      return 0; /*FALSE*/
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 1; /* TRUE */
    else
      return bytes_comparison ( op1, op2, size ) < 0;
}

/* This function compares two unsigned integers of given size on that
   the first operand is greater than or equal to the second.  The
   function returns 1 if the first unsigned integer is greater than or
   equal to the second, 0 otherwise. */

int
ge_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return bytes_comparison ( op1, op2, size ) >= 0;
}

/* This function compares two integers of given size on that the first
   operand is greater than or equal to the second.  The function
   returns 1 if the first integer is greater than or equal to the
   second, 0 otherwise. */

int
ge_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return bytes_comparison ( op1, op2, size ) >= 0;
    else
      return 1; /* TRUE */
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 0; /*FALSE*/
    else
      return bytes_comparison ( op1, op2, size ) >= 0;
}

/* This function compares two unsigned integers of given size on that
   the first operand is less than or equal to the second.  The
   function returns 1 if the first unsigned integer is less than or
   equal to the second, 0 otherwise. */

int
le_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return bytes_comparison ( op1, op2, size ) <= 0;
}

/* This function compares two integers of given size on that the first
   operand is less than or equal to the second.  The function returns
   1 if the first integer is less than or equal to the second, 0
   otherwise. */

int
le_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return bytes_comparison ( op1, op2, size ) <= 0;
    else
      return 0; /*FALSE*/
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 1; /* TRUE */
    else
      return bytes_comparison ( op1, op2, size ) <= 0;
}



/* This page contains functions for changing size of arbitrary
   precision numbers. */

/* The function changes size of unsigned integer.  The function fixes
   overflow when result can not be represented by number of given
   size.  Result can be placed in operand. */

void
change_unsigned_integer_size ( int operand_size, const void *operand,
                               int result_size, void *result )
{
  int operand_digit_number;

  overflow_bit = 0;
  if ( operand_size <= result_size ) {
    ammunition_memmove ( ( char * ) result + result_size - operand_size, operand,
                         ( size_t ) operand_size );
    ammunition_memset ( result, 0, ( size_t ) ( result_size - operand_size ) );
  } else {
    _Pragma( "loopbound min 2 max 2" )
    for ( operand_digit_number = 0;
          operand_digit_number < operand_size - result_size;
          operand_digit_number++ ) {
      if ( ( ( unsigned char * ) operand ) [operand_digit_number] != 0 ) {
        overflow_bit = 1;
        break;
      }
    }
    ammunition_memmove ( result, ( char * ) operand + operand_size - result_size,
                         ( size_t ) result_size );
  }
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
}

/* The function changes size of integer.  The function fixes overflow
   when result can not be represented by number of given size.  Result
   can be placed in operand. */

void
change_integer_size ( int operand_size, const void *operand,
                      int result_size, void *result )
{
  int operand_digit_number;
  int operand_sign;

  overflow_bit = 0;
  operand_sign = INTEGER_SIGN ( operand );
  if ( operand_size <= result_size ) {
    ammunition_memmove ( ( char * ) result + result_size - operand_size, operand,
                         ( size_t ) operand_size );
    ammunition_memset ( result, ( operand_sign ? UCHAR_MAX : 0 ),
                        ( size_t ) ( result_size - operand_size ) );
  } else {
    _Pragma( "loopbound min 2 max 2" )
    for ( operand_digit_number = 0;
          operand_digit_number < operand_size - result_size;
          operand_digit_number++ ) {
      if ( ( ( unsigned char * ) operand ) [operand_digit_number]
           != ( operand_sign ? UCHAR_MAX : 0 ) ) {
        overflow_bit = 1;
        break;
      }
    }
    ammunition_memmove ( result, ( char * ) operand + operand_size - result_size,
                         ( size_t ) result_size );
    if ( operand_sign != INTEGER_SIGN ( result ) )
      overflow_bit = 1;
  }
  if ( overflow_bit )
    arithmetic_overflow_reaction();
}



/* This page contains functions for conversion of arbitrary precision
   numbers to ascii representation. */

/* This function transforms unsigned integer of given size to decimal
   ascii representation.  Sign is absent in result string.  The
   function returns the result string. */

char *
unsigned_integer_to_string ( int size, const void *operand, char *result )
{
  int digit_number;
  int i;
  unsigned long divisable;
  unsigned long remainder;
  int nonzero_flag;
  int length;
  int temporary;
  unsigned char operand_copy [MAX_INTEGER_OPERAND_SIZE];

  ammunition_memcpy ( operand_copy, operand, ( size_t ) size );
  length = 0;
  _Pragma( "loopbound min 1 max 10" )
  do {
    nonzero_flag = 0 /* FALSE */;
    _Pragma( "loopbound min 2 max 6" )
    for ( digit_number = 0, remainder = 0; digit_number < size; digit_number++ ) {
      divisable = remainder * ( UCHAR_MAX + 1 ) + operand_copy [digit_number];
      remainder = divisable % 10;
      operand_copy [digit_number] = ( unsigned char ) ( divisable / 10 );
      if ( operand_copy [digit_number] != 0 )
        nonzero_flag = 1 /* TRUE */;
    }
    result [length++] = ( unsigned char ) ( '0' + remainder );
  } while ( nonzero_flag );
  result [length] = '\0';
  _Pragma( "loopbound min 0 max 5" )
  for ( i = 0; i < length / 2; i++ ) {
    temporary = result [i];
    result [i] = result [length - i - 1];
    result [length - i - 1] = temporary;
  }
  return result;
}

/* This function transforms integer of given size to decimal ascii
   representation.  Sign is present in result string only for negative
   numbers.  The function returns the result string. */

char *
integer_to_string ( int size, const void *operand, char *result )
{
  unsigned char operand_copy [MAX_INTEGER_OPERAND_SIZE];

  if ( !INTEGER_SIGN ( operand ) )
    return unsigned_integer_to_string ( size, operand, result );
  ammunition_memcpy ( operand_copy, operand, ( size_t ) size );
  /* May be integer overflow. But result is correct because it is unsigned. */
  make_complementary_code ( size, operand_copy, operand_copy );
  *result = '-';
  unsigned_integer_to_string ( size, operand_copy, result + 1 );
  return result;
}



/* This page contains functions for conversion of decimal ascii
   representation to arbitrary precision numbers.  */

/* The function adds digit (byte size) to unsigned integer.  The
   function returns 1 if unsigned integer overflow is fixed, 0
   otherwise. */

static int
add_digit_to_unsigned_integer_without_overflow_reaction
( int size, void *operand, unsigned int digit )
{
  int digit_number;
  unsigned int carry;
  unsigned int sum;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_number = size - 1, carry = digit; digit_number >= 0;
        digit_number-- ) {
    sum = ( ( unsigned char * ) operand ) [digit_number] + carry;
    if ( sum > UCHAR_MAX ) {
      carry = sum / ( UCHAR_MAX + 1 );
      sum %= UCHAR_MAX + 1;
    } else
      carry = 0;
    ( ( unsigned char * ) operand ) [digit_number] = sum;
  }
  return carry != 0;
}

/* This function transforms source string (decimal ascii
   representation without sign) to given size unsigned integer and
   returns pointer to first non digit in the source string through a
   parameter.  If the string started with invalid integer
   representation the result will be zero and returns the operand
   through the parameter.  The function returns 1 if unsigned integer
   overflow is fixed, 0 otherwise. */

static int
string_to_unsigned_integer_without_overflow_reaction
( int size, const char *operand, void *result, char **first_nondigit )
{
  int overflow_flag;

  ammunition_memset ( result, 0, ( size_t ) size );
  _Pragma( "loopbound min 0 max 10" )
  for ( overflow_flag = 0; isdigit ( *operand ); operand++ ) {
    overflow_flag
      = overflow_flag
        || multiply_unsigned_integer_by_digit_without_overflow_reaction
        ( size, result, 10 );
    overflow_flag
      = overflow_flag
        || add_digit_to_unsigned_integer_without_overflow_reaction
        ( size, result, *operand - '0' );
  }
  *first_nondigit = ( char * ) operand;
  return overflow_flag;
}

/* This function skips all white spaces at the begin of source string
   and transforms tail of the source string (decimal ascii
   representation without sign) to given size unsigned integer with
   the aid of function
   `string_to_unsigned_integer_without_overflow_reaction'.  If the
   string started with invalid unsigned integer representation the
   result will be zero.  The function fixes overflow when result can
   not be represented by number of given size.  The function returns
   address of the first nondigit in the source string. */

char *
unsigned_integer_from_string ( int size, const char *operand, void *result )
{
  char *first_nondigit;

  _Pragma( "loopbound min 0 max 0" )
  while ( isspace ( *operand ) )
    operand++;
  overflow_bit
    = string_to_unsigned_integer_without_overflow_reaction
      ( size, operand, result, &first_nondigit );
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
  return first_nondigit;
}

/* This function skips all white spaces at the begin of source string
   and transforms tail of the source string (decimal ascii
   representation with possible sign `+' or `-') to given size integer
   with the aid of function
   `string_to_unsigned_integer_without_overflow_reaction'.  If the
   string started with invalid integer representation the result will
   be zero.  The function fixes overflow when result can not be
   represented by number of given size.  the function returns Address
   of the first nondigit in the source string. */

char *
integer_from_string ( int size, const char *operand, void *result )
{
  int negative_number_flag;
  char *first_nondigit;
  int unsigned_result_sign;

  _Pragma( "loopbound min 0 max 0" )
  while ( isspace ( *operand ) )
    operand++;
  negative_number_flag = 0; /* FALSE */
  if ( *operand == '+' )
    operand++;
  else
    if ( *operand == '-' ) {
      operand++;
      negative_number_flag = 1; /* TRUE */
    }
  overflow_bit
    = string_to_unsigned_integer_without_overflow_reaction
      ( size, operand, result, &first_nondigit );
  unsigned_result_sign = INTEGER_SIGN ( result );
  if ( negative_number_flag )
    /* May be integer overflow when `result' is correct.  But result
       is correct because it is unsigned. */
    make_complementary_code ( size, result, result );
  overflow_bit
    = overflow_bit || ( unsigned_result_sign
                        && ( !negative_number_flag
                             || INTEGER_SIGN ( result ) != unsigned_result_sign ) );
  if ( overflow_bit )
    arithmetic_unsigned_overflow_reaction();
  return first_nondigit;
}

