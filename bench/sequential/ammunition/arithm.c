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
#include "ammunition_string.h"


/* This variable can have only two values 0 or 1.  The value `1'
   corresponds to overflow.  The variable value are modified by all
   functions of addition, subtract, multiplication, division,
   evaluation of remainder, shift, changing size and transformation of
   string into number fix overflow. */

int ammunition_overflow_bit;


/* The following function adds unsigned integers.  The function
   returns 1 if unsigned integer overflow is fixed, 0 otherwise.
   Result can be placed in any operand. */

int ammunition_add_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int digit_num;
  int carry;
  unsigned int sum;
  _Pragma( "loopbound min 4 max 4" )
  for ( digit_num = size - 1, carry = 0; digit_num >= 0; digit_num-- ) {
    sum = ( ( ( unsigned char * ) op1 ) [ digit_num ]
            + ( ( unsigned char * ) op2 ) [ digit_num ] + carry );
    if ( sum > UCHAR_MAX ) {
      sum -= UCHAR_MAX + 1;
      carry = 1;
    } else
      carry = 0;
    ( ( unsigned char * ) result ) [ digit_num ] = sum;
  }
  return carry != 0;
}

/* The following function adds unsigned integers.  The function
   returns 1 if unsigned integer overflow (the first operand is less
   than the second) is fixed, 0 otherwise.  Result can be placed in
   any operand. */

int ammunition_subtract_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int digit_num;
  int carry;
  int subtraction;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_num = size - 1, carry = 0; digit_num >= 0; digit_num-- ) {
    subtraction = ( ( ( unsigned char * ) op1 ) [ digit_num ]
                    - ( ( unsigned char * ) op2 ) [ digit_num ] - carry );
    if ( subtraction < 0 ) {
      subtraction += UCHAR_MAX + 1;
      carry = 1;
    } else
      carry = 0;
    ( ( unsigned char * ) result ) [ digit_num ] = subtraction;
  }
  return carry != 0;
}

/* The following function makes complementary code of number.  Result
   can be placed in operand. */

void ammunition_make_complementary_code
( int size, const void *operand, void *result )
{
  int digit_num;
  int carry;
  int subtraction;

  _Pragma( "loopbound min 2 max 6" )
  for ( digit_num = size - 1, carry = 0; digit_num >= 0; digit_num-- ) {
    subtraction = ( 0 - ( ( unsigned char * ) operand ) [ digit_num ] - carry );
    if ( subtraction != 0 ) {
      subtraction += UCHAR_MAX + 1;
      carry = 1;
    } else
      carry = 0;
    ( ( unsigned char * ) result ) [ digit_num ] = subtraction;
  }
}

/* The following function multiplys unsigned integer by digit (byte
   size).  The function returns 1 if unsigned integer overflow is
   fixed, 0 otherwise. */

int ammunition_multiply_unsigned_integer_by_digit_without_overflow_reaction
( int size, void *operand, unsigned int digit )
{
  int digit_num;
  unsigned int carry;
  unsigned int sum;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_num = size - 1, carry = 0; digit_num >= 0; digit_num-- ) {
    sum = ( ( ( unsigned char * ) operand ) [ digit_num ] * digit + carry );
    if ( sum > UCHAR_MAX ) {
      carry = sum / ( UCHAR_MAX + 1 );
      sum %= UCHAR_MAX + 1;
    } else
      carry = 0;
    ( ( unsigned char * ) operand ) [ digit_num ] = sum;
  }
  return carry != 0;
}


/* Originally reaction on all integer and unsigned integer overflow is
   equal to the following function.  The function does nothing. */

void
ammunition_arithmetic_overflow_reaction ( void )
{}


/* Originally reaction on all integer and unsigned integer overflow is
   equal to the following function.  The function does nothing. */

void
ammunition_arithmetic_unsigned_overflow_reaction ( void )
{}


/* This page contains functions for arbitrary precision addition. */

/* The function adds unsigned integers and fixes overflow reaction if
   it is needed.  The function makes this with the aid of function
   `add_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
ammunition_add_unsigned_integer ( int size, const void *op1, const void *op2,
                                  void *result )
{
  ammunition_overflow_bit
    = ammunition_add_unsigned_integer_without_overflow_reaction (
        size,  op1, op2, result );
  if ( ammunition_overflow_bit != 0 )
    ammunition_arithmetic_unsigned_overflow_reaction();
}

/* The function adds integers and fixes overflow reaction if it is
   needed.  The function makes this with the aid of function
   `add_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
ammunition_add_integer ( int size, const void *op1, const void *op2,
                         void *result )
{
  int op1_sign;
  int sign_equality;

  op1_sign = INTEGER_SIGN ( op1 );
  sign_equality = INTEGER_SIGN ( op1 ) == INTEGER_SIGN ( op2 );
  ammunition_add_unsigned_integer_without_overflow_reaction (
    size, op1, op2, result );
  ammunition_overflow_bit = sign_equality &&
                            ( op1_sign != INTEGER_SIGN ( result ) );
  if ( ammunition_overflow_bit != 0 )
    ammunition_arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision subtraction. */

/* The function subtracts unsigned integers and fixes overflow
   reaction if it is needed.  The function makes this with the aid of
   function `subtract_unsigned_integer_without_overflow_reaction'.
   Result can be placed in any operand. */

void
ammunition_subtract_unsigned_integer ( int size, const void *op1,
                                       const void *op2,
                                       void *result )
{
  ammunition_overflow_bit
    = ammunition_subtract_unsigned_integer_without_overflow_reaction (
        size, op1, op2, result );
  if ( ammunition_overflow_bit != 0 )
    ammunition_arithmetic_unsigned_overflow_reaction();
}

/* The function subtracts integers and fixes overflow reaction if it
   is needed.  The function makes this with the aid of function
   `subtract_unsigned_integer_without_overflow_reaction'.  Result can
   be placed in any operand. */

void
ammunition_subtract_integer ( int size, const void *op1, const void *op2,
                              void *result )
{
  int op1_sign;
  int sign_unequality;

  op1_sign = INTEGER_SIGN ( op1 );
  sign_unequality = INTEGER_SIGN ( op1 ) != INTEGER_SIGN ( op2 );
  ammunition_subtract_unsigned_integer_without_overflow_reaction (
    size, op1, op2, result );
  ammunition_overflow_bit = sign_unequality &&
                            ( op1_sign != INTEGER_SIGN ( result ) );
  if ( ammunition_overflow_bit != 0 )
    ammunition_arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision multiplication. */

/* The following function multiplys unsigned integers.  The function
   returns 1 if unsigned integer overflow is fixed, 0 otherwise.
   Result can be placed in any operand. */

int ammunition_multiply_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int op1_digit_num;
  int op2_digit_num;
  int carry;
  unsigned long long int partial_sum;
  int result_digit_number;
  int overflow_flag;
  unsigned char long_result [ 2 * MAX_INTEGER_OPERAND_SIZE ];

  ammunition_memset ( long_result + size, 0, ( size_t ) size );
  _Pragma( "loopbound min 4 max 4" )
  for ( op2_digit_num = size - 1; op2_digit_num >= 0; op2_digit_num-- ) {
    if ( ( ( unsigned char * ) op2 ) [ op2_digit_num ] != 0 ) {
      _Pragma( "loopbound min 4 max 4" )
      for ( op1_digit_num = size - 1, carry = 0; op1_digit_num >= 0;
            op1_digit_num-- ) {
        partial_sum
          = ( ( ( unsigned char * ) op1 ) [ op1_digit_num ]
              * ( ( unsigned char * ) op2 ) [ op2_digit_num ]
              + long_result [ op1_digit_num + op2_digit_num + 1 ]
              + carry );
        long_result [ op1_digit_num + op2_digit_num + 1 ]
          = ( unsigned char ) ( partial_sum % ( UCHAR_MAX + 1 ) );
        carry = partial_sum / ( UCHAR_MAX + 1 );
      }
      long_result [ op2_digit_num ] = carry;
    } else
      long_result [ op2_digit_num ] = 0;
  }
  overflow_flag = 0;
  _Pragma( "loopbound min 0 max 4" )
  for ( result_digit_number = size - 1; result_digit_number >= 0;
        result_digit_number-- ) {
    if ( long_result [ result_digit_number ] != 0 ) {
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
ammunition_multiply_unsigned_integer ( int size, const void *op1,
                                       const void *op2,
                                       void *result )
{
  ammunition_overflow_bit =
    ammunition_multiply_unsigned_integer_without_overflow_reaction (
      size, op1, op2, result );
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_unsigned_overflow_reaction();
}

/* The function multiplys integers and fixes overflow reaction if it
   is needed.  The function makes this with the aid of function
   `multiply_unsigned_integer_without_overflow_reaction'.  Result can
   be placed in any operand. */

void
ammunition_multiply_integer ( int size, const void *op1, const void *op2,
                              void *result )
{
  int negative_result_flag;
  unsigned char op1_complementary [ MAX_INTEGER_OPERAND_SIZE ];
  unsigned char op2_complementary [ MAX_INTEGER_OPERAND_SIZE ];
  unsigned const char *abs_op1;
  unsigned const char *abs_op2;
  int unsigned_result_sign;

  negative_result_flag = INTEGER_SIGN ( op1 ) != INTEGER_SIGN ( op2 );
  if ( INTEGER_SIGN ( op1 ) ) {
    /* May be integer overflow. But result is correct because
       it is unsigned. */
    ammunition_make_complementary_code ( size, op1, op1_complementary );
    abs_op1 = ( unsigned const char * )op1_complementary;
  } else
    abs_op1 = ( unsigned const char * )op1;
  if ( INTEGER_SIGN ( op2 ) ) {
    /* May be integer overflow.  But result is correct because
       it is unsigned. */
    ammunition_make_complementary_code ( size, op2, op2_complementary );
    abs_op2 = ( unsigned const char * )op2_complementary;
  } else
    abs_op2 = ( unsigned const char * )op2;
  ammunition_overflow_bit =
    ammunition_multiply_unsigned_integer_without_overflow_reaction (
      size, abs_op1, abs_op2, result );
  unsigned_result_sign = INTEGER_SIGN ( result );
  if ( negative_result_flag )
    ammunition_make_complementary_code ( size, result, result );
  if ( unsigned_result_sign
       && ( !negative_result_flag
            || INTEGER_SIGN ( result ) != unsigned_result_sign ) )
    /* Unsigned result can not be represented as integer. */
    ammunition_overflow_bit = 1;
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision division. */

/* The following function divides unsigned integers.  The function
   returns 1 if unsigned integer overflow (division by zero) is fixed,
   0 otherwise.  Result can be placed in any operand.  See algorithm
   in Knuth's book. */

int ammunition_divide_unsigned_integer_without_overflow_reaction
( int size, const void *op1, const void *op2, void *result )
{
  int scaled_op1_digit_num;
  unsigned int q_approximation;
  int first_nonzero_digit_number;
  int op2_digit_number;
  unsigned int scale;
  unsigned char scaled_op1 [ MAX_INTEGER_OPERAND_SIZE + 1 ];
  unsigned char normalized_op2 [ MAX_INTEGER_OPERAND_SIZE ];
  unsigned char extended_normalized_op2 [ MAX_INTEGER_OPERAND_SIZE + 1 ];

  _Pragma( "loopbound min 3 max 4" )
  for ( op2_digit_number = 0; op2_digit_number < size; op2_digit_number++ ) {
    if ( ( ( unsigned char * ) op2 ) [ op2_digit_number ] != 0 )
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
      int digit_num;
      int digit;
      unsigned long long divisable;
      unsigned long long remainder;

      digit = ( ( unsigned char * ) op2 ) [ first_nonzero_digit_number ];
      ammunition_memcpy ( result, op1, ( size_t ) size );
      remainder = 0;
      _Pragma( "loopbound min 4 max 4" )
      for ( digit_num = 0; digit_num < size; digit_num++ ) {
        divisable = ( remainder * ( UCHAR_MAX + 1 )
                      + ( ( unsigned char * ) result ) [ digit_num ] );
        remainder = divisable % digit;
        ( ( unsigned char * ) result ) [ digit_num ]
          = ( unsigned char ) ( divisable / digit );
      }
      return 0 /* FALSE */;
    }
  /* Normalization of divisor. */
  scale = ( UCHAR_MAX + 1 ) / ( ( ( unsigned char * ) op2 ) [ op2_digit_number ] +
                                1 );
  ammunition_memcpy ( scaled_op1 + 1, op1, ( size_t ) size );
  *scaled_op1 = 0;

  ammunition_multiply_unsigned_integer_by_digit_without_overflow_reaction
  ( size + 1, scaled_op1, scale );

  ammunition_memcpy ( normalized_op2, op2, ( size_t ) size );

  ammunition_multiply_unsigned_integer_by_digit_without_overflow_reaction
  ( size, normalized_op2, scale );

  _Pragma( "loopbound min 0 max 0" )
  for ( scaled_op1_digit_num = 0;
        scaled_op1_digit_num <= first_nonzero_digit_number;
        scaled_op1_digit_num++ ) {
    /* Division of `scaled_op1[ scaled_op1_digit_number ]..scaled_op1[ size ]' by
       `normalized_op2[ first_nonzero_digit_number ]..normalized_op2[ size-1 ]'
       for evaluation of one digit of quotient
       `result[ size-1-first_nonzero_digit_number-scaled_op1_digit_number ]'.
    */
    if ( scaled_op1 [ scaled_op1_digit_num ]
         == normalized_op2 [ first_nonzero_digit_number ] )
      q_approximation = UCHAR_MAX;
    else
      q_approximation
        = ( scaled_op1 [ scaled_op1_digit_num ] * ( UCHAR_MAX + 1 )
            + scaled_op1 [ scaled_op1_digit_num + 1 ] )
          / normalized_op2 [ first_nonzero_digit_number ];

    _Pragma( "loopbound min 0 max 0" )
    while ( normalized_op2 [ first_nonzero_digit_number + 1 ] * q_approximation
            > ( ( ( unsigned long long int ) scaled_op1 [ scaled_op1_digit_num ]
                  * ( UCHAR_MAX + 1 )
                  + scaled_op1 [ scaled_op1_digit_num + 1 ]
                  - q_approximation
                  * normalized_op2 [ first_nonzero_digit_number ] )
                * ( UCHAR_MAX + 1 ) + scaled_op1 [ scaled_op1_digit_num + 2 ] ) )
      q_approximation --;

    /* Multiply and subtract */
    ammunition_memcpy ( extended_normalized_op2 + 1,
                        normalized_op2 + first_nonzero_digit_number,
                        ( size_t ) ( size - first_nonzero_digit_number ) );
    *extended_normalized_op2 = 0;
    ammunition_multiply_unsigned_integer_by_digit_without_overflow_reaction
    ( size - first_nonzero_digit_number + 1, extended_normalized_op2,
      q_approximation );
    if ( ammunition_subtract_unsigned_integer_without_overflow_reaction
         ( size - first_nonzero_digit_number + 1,
           scaled_op1 + scaled_op1_digit_num, extended_normalized_op2,
           scaled_op1 + scaled_op1_digit_num ) ) {
      /* Negative result.  Compensation by addition. */
      q_approximation--;
      ammunition_memcpy ( extended_normalized_op2 + 1,
                          normalized_op2 + first_nonzero_digit_number,
                          ( size_t ) ( size - first_nonzero_digit_number ) );
      *extended_normalized_op2 = 0;

      ammunition_add_unsigned_integer_without_overflow_reaction
      ( size - first_nonzero_digit_number + 1,
        scaled_op1 + scaled_op1_digit_num, extended_normalized_op2,
        scaled_op1 + scaled_op1_digit_num );

    }
    ( ( unsigned char * ) result ) [ size - 1 - first_nonzero_digit_number
                                         + scaled_op1_digit_num ] = q_approximation;
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
ammunition_divide_unsigned_integer ( int size, const void *op1, const void *op2,
                                     void *result )
{
  ammunition_overflow_bit =
    ammunition_divide_unsigned_integer_without_overflow_reaction (
      size, op1, op2, result );
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_unsigned_overflow_reaction();
}

/* The function divides integers and fixes overflow reaction if it is
   needed.  The function makes this with the aid of function
   `divide_unsigned_integer_without_overflow_reaction'.  Result can be
   placed in any operand. */

void
ammunition_divide_integer ( int size, const void *op1, const void *op2,
                            void *result )
{
  int negative_result_flag;
  unsigned char op1_complementary [ MAX_INTEGER_OPERAND_SIZE ];
  unsigned char op2_complementary [ MAX_INTEGER_OPERAND_SIZE ];
  unsigned const char *abs_op1;
  unsigned const char *abs_op2;
  int unsigned_result_sign;

  negative_result_flag = INTEGER_SIGN ( op1 ) != INTEGER_SIGN ( op2 );
  if ( INTEGER_SIGN ( op1 ) ) {
    /* May be integer overflow for minimal int. But result is correct because
       it is unsigned. */
    ammunition_make_complementary_code ( size, op1, op1_complementary );
    abs_op1 = ( unsigned const char * )op1_complementary;
  } else
    abs_op1 = ( unsigned const char * )op1;
  if ( INTEGER_SIGN ( op2 ) ) {
    /* May be integer overflow for minimal int.  But result is correct
       because it is unsigned. */
    ammunition_make_complementary_code ( size, op2, op2_complementary );
    abs_op2 = ( unsigned const char * )op2_complementary;
  } else
    abs_op2 = ( unsigned const char * )op2;
  ammunition_overflow_bit =
    ammunition_divide_unsigned_integer_without_overflow_reaction (
      size, abs_op1, abs_op2, result );
  unsigned_result_sign = INTEGER_SIGN ( result );
  if ( negative_result_flag )
    ammunition_make_complementary_code ( size, result, result );
  if ( unsigned_result_sign
       && ( !negative_result_flag
            || INTEGER_SIGN ( result ) != unsigned_result_sign ) )
    /* Unsigned result can not be represented as integer. */
    ammunition_overflow_bit = 1;
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_overflow_reaction();
}



/* This page contains functions for arbitrary precision evaluation of
   remainder. */

/* The function evaluates remainder of division of unsigned integers
   as `op1 - (op1/op2)*op2' and fixes overflow reaction if it is
   needed.  Result can be placed in any operand. */

void
ammunition_unsigned_integer_remainder ( int size, const void *op1,
                                        const void *op2,
                                        void *result )
{
  unsigned char temporary [ MAX_INTEGER_OPERAND_SIZE ];

  ammunition_divide_unsigned_integer ( size, op1, op2, temporary );
  if ( ammunition_overflow_bit )
    /* Reaction on zero is called from `divide_unsigned_integer'. */
    ammunition_memset ( result, 0, ( size_t ) size );
  else {
    ammunition_multiply_unsigned_integer ( size, temporary, op2, temporary );
    ammunition_subtract_unsigned_integer ( size, op1, temporary, result );
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
ammunition_unsigned_integer_shift_right ( int size, const void *operand,
    int bits, void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;


  if ( bits < 0 )
    ammunition_unsigned_integer_shift_left ( size, operand, -bits, result );
  else {
    ammunition_overflow_bit = 0;
    byte_shift = bits / CHAR_BIT;
    bit_shift = bits % CHAR_BIT;
    _Pragma( "loopbound min 0 max 2" )
    for ( byte_number = ( byte_shift >= size ? 0 : size - byte_shift );
          byte_number < size; byte_number++ )
      if ( ( ( unsigned char * ) operand ) [ byte_number ] != 0 ) {
        ammunition_overflow_bit = 1;
        break;
      } 
    if ( byte_shift >= size )
      ammunition_memset ( result, 0, ( size_t ) size );
    else {
      ammunition_memmove ( ( char * ) result + byte_shift, operand,
                           ( size_t ) ( size - byte_shift ) );
      ammunition_memset ( result, 0, ( size_t ) byte_shift );
      if ( bit_shift == 0 )
        return;
      _Pragma( "loopbound min 3 max 3" )
      for ( byte_number = byte_shift, carry = 0; byte_number < size;
            byte_number++ ) {
        byte = ( ( unsigned char * ) result ) [ byte_number ];
        ( ( unsigned char * ) result ) [ byte_number ]
          = carry | ( byte >> bit_shift );
        carry = ( byte << ( CHAR_BIT - bit_shift ) ) & UCHAR_MAX;
      }
      if ( carry != 0 )
        ammunition_overflow_bit = 1;
    }
    if ( ammunition_overflow_bit )
      ammunition_arithmetic_unsigned_overflow_reaction();
  }
}

/* This function makes right arithmetic shift of integer of given size
   on given number of bits.  If number of bits is negative the
   function makes shift to left actually with the aid of function
   `integer_shift_left'.  The function fixes overflow when result can
   not be represented by number of given size, i.e. in other words the
   opposite shift (to left) results in number not equal to source
   operand.  Result can be placed in operand. */

void
ammunition_integer_shift_right ( int size, const void *operand, int bits,
                                 void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;
  int operand_sign;

  if ( bits < 0 )
    ammunition_integer_shift_left ( size, operand, -bits, result );
  else {
    operand_sign = INTEGER_SIGN ( operand );
    ammunition_overflow_bit = 0;
    byte_shift = bits / CHAR_BIT;
    bit_shift = bits % CHAR_BIT;
    _Pragma( "loopbound min 0 max 2" )
    for ( byte_number = ( byte_shift >= size ? 0 : size - byte_shift );
          byte_number < size; byte_number++ )
      if ( ( ( unsigned char * ) operand ) [ byte_number ] != 0 ) {
        ammunition_overflow_bit = 1;
        break;
      }
    if ( byte_shift >= size )
      ammunition_memset ( result,
                          ( operand_sign ? UCHAR_MAX : 0 ), ( size_t ) size );
    else {
      ammunition_memmove ( ( char * ) result + byte_shift, operand,
                           ( size_t ) ( size - byte_shift ) );
      ammunition_memset ( result, ( operand_sign ? UCHAR_MAX : 0 ),
                          ( size_t ) byte_shift );
      if ( bit_shift == 0 )
        return;
      carry = ( ( ( operand_sign ? UCHAR_MAX : 0 ) << ( CHAR_BIT - bit_shift ) )
                & UCHAR_MAX );
      _Pragma( "loopbound min 3 max 3" )
      for ( byte_number = byte_shift; byte_number < size; byte_number++ ) {
        byte = ( ( unsigned char * ) result ) [ byte_number ];
        ( ( unsigned char * ) result ) [ byte_number ]
          = carry | ( byte >> bit_shift );
        carry = ( byte << ( CHAR_BIT - bit_shift ) ) & UCHAR_MAX;
      }
      if ( carry != 0 )
        ammunition_overflow_bit = 1;
    }
    if ( ammunition_overflow_bit )
      ammunition_arithmetic_overflow_reaction();
  }
}

/* This function makes left shift of unsigned integer of given size on
   given number of bits.  If number of bits is negative the function
   makes shift to left actually with the aid of function
   `unsigned_integer_shift_right'.  The function fixes overflow when
   result can not be represented by number of given size, i.e. i.e. in
   other words the opposite shift (to right) results in number not
   equal to source operand.  Result can be placed in operand. */

void
ammunition_unsigned_integer_shift_left ( int size, const void *operand,
    int bits, void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;

  if ( bits < 0 )
    ammunition_unsigned_integer_shift_right ( size, operand, -bits, result );
  else {
    ammunition_overflow_bit = 0;
    byte_shift = bits / CHAR_BIT;
    bit_shift = bits % CHAR_BIT;
    _Pragma( "loopbound min 0 max 2" )
    for ( byte_number = 0; byte_number < byte_shift && byte_number < size;
          byte_number++ )
      if ( ( ( unsigned char * ) operand ) [ byte_number ] != 0 ) {
        ammunition_overflow_bit = 1;
        break;
      }
    if ( byte_shift >= size )
      ammunition_memset ( result, 0, ( size_t ) size );
    else {
      ammunition_memmove ( result, ( char * ) operand + byte_shift,
                           ( size_t ) ( size - byte_shift ) );
      ammunition_memset ( ( char * ) result + ( size - byte_shift ), 0,
                          ( size_t ) byte_shift );
      if ( bit_shift == 0 )
        return;
      _Pragma( "loopbound min 2 max 3" )
      for ( byte_number = size - byte_shift - 1, carry = 0;
            byte_number >= 0; byte_number-- ) {
        byte = ( ( unsigned char * ) result ) [ byte_number ];
        ( ( unsigned char * ) result ) [ byte_number ]
          = carry | ( byte << bit_shift );
        carry = byte >> ( CHAR_BIT - bit_shift );
      }
      if ( carry != 0 )
        ammunition_overflow_bit = 1;
    }
    if ( ammunition_overflow_bit )
      ammunition_arithmetic_unsigned_overflow_reaction();
  }
}

/* This function makes left arithmetic shift of integer of given size
   on given number of bits.  If number of bits is negative the
   function makes shift to left actually with the aid of function
   `integer_shift_right'.  The function fixes overflow when result can
   not be represented by number of given size, i.e. in other words the
   opposite shift (to right) results in number not equal to source
   operand.  Result can be placed in operand. */

void
ammunition_integer_shift_left ( int size, const void *operand, int bits,
                                void *result )
{
  int byte_number;
  unsigned byte;
  unsigned carry;
  int bit_shift;
  int byte_shift;
  int operand_sign;

  if ( bits < 0 )
    ammunition_integer_shift_right ( size, operand, -bits, result );
  else {
    operand_sign = INTEGER_SIGN ( operand );
    ammunition_overflow_bit = 0;
    byte_shift = bits / CHAR_BIT;
    bit_shift = bits % CHAR_BIT;
    _Pragma( "loopbound min 0 max 2" )
    for ( byte_number = 0; byte_number < byte_shift && byte_number < size;
          byte_number++ )
      if ( ( ( unsigned char * ) operand ) [ byte_number ]
           != ( operand_sign ? UCHAR_MAX : 0 ) ) {
        ammunition_overflow_bit = 1;
        break;
      }
    if ( byte_shift >= size )
      ammunition_memset ( result, 0, ( size_t ) size );
    else {
      ammunition_memmove ( result, ( char * ) operand + byte_shift,
                           ( size_t ) ( size - byte_shift ) );
      ammunition_memset ( ( char * ) result + ( size - byte_shift ), 0,
                          ( size_t ) byte_shift );
      if ( bit_shift == 0 )
        return;
      _Pragma( "loopbound min 2 max 3" )
      for ( byte_number = size - byte_shift - 1, carry = 0;
            byte_number >= 0; byte_number-- ) {
        byte = ( ( unsigned char * ) result ) [ byte_number ];
        ( ( unsigned char * ) result ) [ byte_number ]
          = carry | ( byte << bit_shift );
        carry = byte >> ( CHAR_BIT - bit_shift );
      }
      if ( carry != ( ( unsigned ) ( operand_sign ? UCHAR_MAX : 0 )
                      >> ( CHAR_BIT - bit_shift ) ) )
        ammunition_overflow_bit = 1;
    }
    if ( operand_sign != INTEGER_SIGN ( result ) )
      ammunition_overflow_bit = 1;
    if ( ammunition_overflow_bit )
      ammunition_arithmetic_overflow_reaction();
  }
}



/* This page contains functions for bitwise operations of arbitrary
   precision numbers. */

/* This function makes bitwise `or' of two integers of given size. */

void
ammunition_integer_or ( int size, const void *op1, const void *op2,
                        void *result )
{
  int byte_number;

  _Pragma( "loopbound min 4 max 4" )
  for ( byte_number = 0; byte_number < size; byte_number++ ) {
    ( ( unsigned char * ) result ) [ byte_number ]
      = ( ( unsigned char * ) op1 ) [ byte_number ]
        | ( ( unsigned char * ) op2 ) [ byte_number ];
  }
}

/* This function makes bitwise `or' of two unsigned integers of given
   size. */

void
ammunition_unsigned_integer_or ( int size, const void *op1, const void *op2,
                                 void *result )
{
  ammunition_integer_or ( size, op1, op2, result );
}


/* This function makes bitwise `and' of two integers of given size. */

void
ammunition_integer_and ( int size, const void *op1, const void *op2,
                         void *result )
{
  int byte_number;

  _Pragma( "loopbound min 4 max 4" )
  for ( byte_number = 0; byte_number < size; byte_number++ ) {
    ( ( unsigned char * ) result ) [ byte_number ]
      = ( ( unsigned char * ) op1 ) [ byte_number ]
        & ( ( unsigned char * ) op2 ) [ byte_number ];
  }
}

/* This function makes bitwise `and' of two unsigned integers of given
   size. */

void
ammunition_unsigned_integer_and ( int size, const void *op1, const void *op2,
                                  void *result )
{
  ammunition_integer_and ( size, op1, op2, result );
}


/* This function makes bitwise `not' of integer of given size. */

void
ammunition_integer_not ( int size, const void *operand, void *result )
{
  int byte_number;

  _Pragma( "loopbound min 4 max 4" )
  for ( byte_number = 0; byte_number < size; byte_number++ ) {
    ( ( unsigned char * ) result ) [ byte_number ]
      = ( ( unsigned char * ) operand ) [ byte_number ] ^ UCHAR_MAX;
  }
}

/* This function makes bitwise `not' of unsigned integer of given
   size. */

void
ammunition_unsigned_integer_not ( int size, const void *operand, void *result )
{
  ammunition_integer_not ( size, operand, result );
}



/* This page contains functions for comparison of arbitrary precision
   numbers. */

/* This function compares two unsigned integers of given size on
   equality.  The function returns 1 if unsigned integers are equal, 0
   otherwise. */

int
ammunition_eq_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) == 0;
}

/* This function compares two integers of given size on equality.  The
   function returns 1 if integers are equal, 0 otherwise. */

int
ammunition_eq_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) == 0;
}

/* This function compares two unsigned integers of given size on
   inequality.  The function returns 1 if unsigned integers are not
   equal, 0 otherwise. */

int
ammunition_ne_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) != 0;
}

/* This function compares two integers of given size on inequality.
   The function returns 1 if integers are not equal, 0 otherwise. */

int
ammunition_ne_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_memcmp ( op1, op2, ( size_t ) size ) != 0;
}


/* This function compares two memory parts of given size on that the
   first operand is greater than the second.  The bytes are described
   as unsigned.  The function returns 1 if the first operand is
   greater than the second, - 1 if the first operand is less than the
   second, 0 otherwise. */

int ammunition_bytes_comparison ( const void *op1, const void *op2, int size )
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
ammunition_gt_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_bytes_comparison ( op1, op2, size ) > 0;
}

/* This function compares two integers of given size on that the first
   operand is greater than the second.  The function returns 1 if the
   first integer is greater than the second, 0 otherwise. */

int ammunition_gt_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return ammunition_bytes_comparison ( op1, op2, size ) > 0;
    else
      return 1; /* TRUE */
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 0; /*FALSE*/
    else
      return ammunition_bytes_comparison ( op1, op2, size ) > 0;
}

/* This function compares two unsigned integers of given size on that
   the first operand is less than the second.  The function returns 1
   if the first unsigned integer is less than the second, 0
   otherwise. */

int
ammunition_lt_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_bytes_comparison ( op1, op2, size ) < 0;
}

/* This function compares two integers of given size on that the first
   operand is less than the second.  The function returns 1 if the
   first integer is less than the second, 0 otherwise. */

int
ammunition_lt_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return ammunition_bytes_comparison ( op1, op2, size ) < 0;
    else
      return 0; /*FALSE*/
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 1; /* TRUE */
    else
      return ammunition_bytes_comparison ( op1, op2, size ) < 0;
}

/* This function compares two unsigned integers of given size on that
   the first operand is greater than or equal to the second.  The
   function returns 1 if the first unsigned integer is greater than or
   equal to the second, 0 otherwise. */

int
ammunition_ge_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_bytes_comparison ( op1, op2, size ) >= 0;
}

/* This function compares two integers of given size on that the first
   operand is greater than or equal to the second.  The function
   returns 1 if the first integer is greater than or equal to the
   second, 0 otherwise. */

int
ammunition_ge_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return ammunition_bytes_comparison ( op1, op2, size ) >= 0;
    else
      return 1; /* TRUE */
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 0; /*FALSE*/
    else
      return ammunition_bytes_comparison ( op1, op2, size ) >= 0;
}

/* This function compares two unsigned integers of given size on that
   the first operand is less than or equal to the second.  The
   function returns 1 if the first unsigned integer is less than or
   equal to the second, 0 otherwise. */

int
ammunition_le_unsigned_integer ( int size, const void *op1, const void *op2 )
{
  return ammunition_bytes_comparison ( op1, op2, size ) <= 0;
}

/* This function compares two integers of given size on that the first
   operand is less than or equal to the second.  The function returns
   1 if the first integer is less than or equal to the second, 0
   otherwise. */

int
ammunition_le_integer ( int size, const void *op1, const void *op2 )
{
  if ( INTEGER_SIGN ( op1 ) == 0 ) {
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return ammunition_bytes_comparison ( op1, op2, size ) <= 0;
    else
      return 0; /*FALSE*/
  } else
    if ( INTEGER_SIGN ( op2 ) == 0 )
      return 1; /* TRUE */
    else
      return ammunition_bytes_comparison ( op1, op2, size ) <= 0;
}



/* This page contains functions for changing size of arbitrary
   precision numbers. */

/* The function changes size of unsigned integer.  The function fixes
   overflow when result can not be represented by number of given
   size.  Result can be placed in operand. */

void
ammunition_change_unsigned_integer_size ( int operand_size, const void *operand,
    int result_size, void *result )
{
  int operand_digit_number;

  ammunition_overflow_bit = 0;
  if ( operand_size <= result_size ) {
    ammunition_memmove ( ( char * ) result + result_size - operand_size,
                         operand, ( size_t ) operand_size );
    ammunition_memset ( result, 0, ( size_t ) ( result_size - operand_size ) );
  } else {
    _Pragma( "loopbound min 1 max 2" )
    for ( operand_digit_number = 0;
          operand_digit_number < operand_size - result_size;
          operand_digit_number++ ) {
      if ( ( ( unsigned char * ) operand ) [ operand_digit_number ] != 0 ) {
        ammunition_overflow_bit = 1;
        break;
      }
    }
    ammunition_memmove ( result,
                         ( char * ) operand + operand_size - result_size,
                         ( size_t ) result_size );
  }
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_unsigned_overflow_reaction();
}

/* The function changes size of integer.  The function fixes overflow
   when result can not be represented by number of given size.  Result
   can be placed in operand. */

void
ammunition_change_integer_size ( int operand_size, const void *operand,
                                 int result_size, void *result )
{
  int operand_digit_number;
  int operand_sign;

  ammunition_overflow_bit = 0;
  operand_sign = INTEGER_SIGN ( operand );
  if ( operand_size <= result_size ) {
    ammunition_memmove ( ( char * ) result + result_size - operand_size,
                         operand, ( size_t ) operand_size );
    ammunition_memset ( result, ( operand_sign ? UCHAR_MAX : 0 ),
                        ( size_t ) ( result_size - operand_size ) );
  } else {
    _Pragma( "loopbound min 2 max 2" )
    for ( operand_digit_number = 0;
          operand_digit_number < operand_size - result_size;
          operand_digit_number++ ) {
      if ( ( ( unsigned char * ) operand ) [ operand_digit_number ]
           != ( operand_sign ? UCHAR_MAX : 0 ) ) {
        ammunition_overflow_bit = 1;
        break;
      }
    }
    ammunition_memmove ( result,
                         ( char * ) operand + operand_size - result_size,
                         ( size_t ) result_size );
    if ( operand_sign != INTEGER_SIGN ( result ) )
      ammunition_overflow_bit = 1;
  }
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_overflow_reaction();
}



/* This page contains functions for conversion of arbitrary precision
   numbers to ascii representation. */

/* This function transforms unsigned integer of given size to BASE
   ascii representation.  BASE should be between 2 and 36 including
   them.  Digits more 9 are represented by 'a', 'b' etc.  Sign is
   absent in result string.  The function returns the result
   string. */

char *
ammunition_unsigned_integer_to_based_string ( int size, const void *operand,
    int base,
    char *result )
{
  int digit_num;
  int i;
  unsigned long long divisable;
  unsigned long long remainder;
  int nonzero_flag;
  int length;
  int temporary;
  unsigned char operand_copy [ MAX_INTEGER_OPERAND_SIZE ];

  ammunition_memcpy ( operand_copy, operand, ( size_t ) size );
  length = 0;
  _Pragma( "loopbound min 1 max 10" )
  do {
    nonzero_flag = 0 /* FALSE */;
    _Pragma( "loopbound min 2 max 6" )
    for ( digit_num = 0, remainder = 0; digit_num < size; digit_num++ ) {
      divisable = remainder * ( UCHAR_MAX + 1 ) + operand_copy [ digit_num ];
      remainder = divisable % base;
      operand_copy [ digit_num ] = ( unsigned char ) ( divisable / base );
      if ( operand_copy [ digit_num ] != 0 )
        nonzero_flag = 1 /* TRUE */;
    }
    result [ length++ ] = ( unsigned char ) ( remainder < 10 ? '0' + remainder
                                            : 'a' + remainder - 10 );
  } while ( nonzero_flag ); 
  result [ length ] = '\0';
  _Pragma( "loopbound min 0 max 5" )
  for ( i = 0; i < length / 2; i++ ) {
    temporary = result [ i ];
    result [ i ] = result [ length - i - 1 ];
    result [ length - i - 1 ] = temporary;
  }
  return result;
}


/* This function transforms unsigned integer of given size to decimal
   ascii representation.  Sign is absent in result string.  The
   function returns the result string. */

char *
ammunition_unsigned_integer_to_string ( int size, const void *operand,
                                        char *result )
{
  return ammunition_unsigned_integer_to_based_string ( size, operand, 10,
         result );
}


/* This function transforms integer of given size to BASE ascii
   representation.  BASE should be between 2 and 36 including them.
   Digits more 9 are represented by 'a', 'b' etc.  Sign is present in
   result string only for negative numbers.  The function returns the
   result string. */

char *
ammunition_integer_to_based_string ( int size, const void *operand, int base,
                                     char *result )
{
  unsigned char operand_copy [ MAX_INTEGER_OPERAND_SIZE ];

  if ( !INTEGER_SIGN ( operand ) )
    return ammunition_unsigned_integer_to_based_string ( size, operand, base,
           result );
  ammunition_memcpy ( operand_copy, operand, ( size_t ) size );
  /* May be integer overflow. But result is correct because it is unsigned. */
  ammunition_make_complementary_code ( size, operand_copy, operand_copy );
  *result = '-';
  ammunition_unsigned_integer_to_based_string ( size, operand_copy, base,
      result + 1 );
  return result;
}



/* This function transforms integer of given size to decimal ascii
   representation.  Sign is present in result string only for negative
   numbers.  The function returns the result string. */

char *
ammunition_integer_to_string ( int size, const void *operand, char *result )
{
  return ammunition_integer_to_based_string ( size, operand, 10, result );
}

/* This page contains functions for conversion of decimal ascii
   representation to arbitrary precision numbers.  */

/* The function adds digit (byte size) to unsigned integer.  The
   function returns 1 if unsigned integer overflow is fixed, 0
   otherwise. */

int ammunition_add_digit_to_unsigned_integer_without_overflow_reaction
( int size, void *operand, unsigned int digit )
{
  int digit_num;
  unsigned int carry;
  unsigned int sum;

  _Pragma( "loopbound min 4 max 4" )
  for ( digit_num = size - 1, carry = digit; digit_num >= 0;
        digit_num-- ) {
    sum = ( ( unsigned char * ) operand ) [ digit_num ] + carry;
    if ( sum > UCHAR_MAX ) {
      carry = sum / ( UCHAR_MAX + 1 );
      sum %= UCHAR_MAX + 1;
    } else
      carry = 0;
    ( ( unsigned char * ) operand ) [ digit_num ] = sum;
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

int ammunition_string_to_unsigned_integer_without_overflow_reaction
( int size, const char *operand, void *result, char **first_nondigit )
{
  int overflow_flag;

  ammunition_memset ( result, 0, ( size_t ) size );
  _Pragma( "loopbound min 1 max 10" )
  for ( overflow_flag = 0; ammunition_isdigit ( *operand ); operand++ ) {
    overflow_flag
      = overflow_flag ||
        ammunition_multiply_unsigned_integer_by_digit_without_overflow_reaction
        ( size, result, 10 );
    overflow_flag
      = overflow_flag
        || ammunition_add_digit_to_unsigned_integer_without_overflow_reaction
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
ammunition_unsigned_integer_from_string ( int size, const char *operand,
    void *result )
{
  char *first_nondigit;

  _Pragma( "loopbound min 0 max 0" )
  while ( ammunition_isspace ( *operand ) )
    operand++;
  ammunition_overflow_bit
    = ammunition_string_to_unsigned_integer_without_overflow_reaction
      ( size, operand, result, &first_nondigit );
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_unsigned_overflow_reaction();
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
ammunition_integer_from_string ( int size, const char *operand, void *result )
{
  int negative_number_flag;
  char *first_nondigit;
  int unsigned_result_sign;

  _Pragma( "loopbound min 0 max 0" )
  while ( ammunition_isspace ( *operand ) )
    operand++;
  negative_number_flag = 0; /* FALSE */
  if ( *operand == '+' )
    operand++;
  else
    if ( *operand == '-' ) {
      operand++;
      negative_number_flag = 1; /* TRUE */
    }
  ammunition_overflow_bit
    = ammunition_string_to_unsigned_integer_without_overflow_reaction
      ( size, operand, result, &first_nondigit );
  unsigned_result_sign = INTEGER_SIGN ( result );
  if ( negative_number_flag )
    /* May be integer overflow when `result' is correct.  But result
       is correct because it is unsigned. */
    ammunition_make_complementary_code ( size, result, result );
  ammunition_overflow_bit
    = ammunition_overflow_bit
      || ( unsigned_result_sign
           && ( !negative_number_flag
                || INTEGER_SIGN ( result ) != unsigned_result_sign ) );
  if ( ammunition_overflow_bit )
    ammunition_arithmetic_unsigned_overflow_reaction();
  return first_nondigit;
}

