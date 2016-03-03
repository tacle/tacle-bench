/*
   FILE NAME:   arithm.h

   TITLE:       Include file of package for arbitrary precision integer
                arithmetic

   DESCRIPTION: This header file contains ANSI C prototype definitions of
                the package functions and definitions of external
                variable of the package and C++ classes for arbitrary
                precision integer arithmetic.

*/


#ifndef __ARITHMETIC__
#define __ARITHMETIC__

#ifdef HAVE_LIMITS_H
#include <limits.h>
#else
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#ifndef UCHAR_MAX
#define UCHAR_MAX 255
#endif
#ifndef SCHAR_MAX
#define SCHAR_MAX 127
#endif
#ifndef SCHAR_MIN
#define SCHAR_MIN (-128)
#endif
#ifndef USHRT_MAX
#define USHRT_MAX 65535
#endif
#ifndef SHRT_MAX
#define SHRT_MAX 32767
#endif
#ifndef SHRT_MIN
#define SHRT_MIN (-32768)
#endif
#ifndef UINT_MAX
#define UINT_MAX (INT_MAX * 2U + 1)
#endif
#ifndef INT_MAX
#define INT_MAX 2147483647
#endif
#ifndef INT_MIN
#define INT_MIN (-INT_MAX-1)
#endif
#endif


/* This page contains definitions of variables and macros common for
   all package functions. */

/* The value of macro is suggested to be maximum length of integer operands
   The length of use integers should be not greater than this value. */

#ifndef MAX_INTEGER_OPERAND_SIZE
#define MAX_INTEGER_OPERAND_SIZE 128
#else
#if MAX_INTEGER_OPERAND_SIZE < 16
#define MAX_INTEGER_OPERAND_SIZE 16
#endif
#endif /* #ifndef MAX_INTEGER_OPERAND_SIZE */

/* The following macro value is sign of integer number (0 or 1) given
   as macro parameter. */

#define INTEGER_SIGN(operand) (*(unsigned char *) (operand) >> (CHAR_BIT - 1))

extern int overflow_bit;
extern const unsigned char *zero_constant;

extern void default_arithmetic_overflow_reaction ( void );

extern void
( *set_unsigned_integer_overflow_reaction ( void ( *function ) ( void ) ) ) (
  void );
extern void ( *set_integer_overflow_reaction ( void ( *function ) ( void ) ) ) (
  void );

extern void unsigned_integer_maximum ( int size, void *result );
extern void integer_minimum ( int size, void *result );
extern void integer_maximum ( int size, void *result );

extern void add_unsigned_integer ( int size, const void *op1, const void *op2,
                                   void *result );
extern void add_integer ( int size, const void *op1, const void *op2,
                          void *result );
extern void subtract_unsigned_integer ( int size, const void *op1,
                                        const void *op2, void *result );
extern void subtract_integer ( int size, const void *op1, const void *op2,
                               void *result );
extern void multiply_unsigned_integer ( int size, const void *op1,
                                        const void *op2, void *result );
extern void multiply_integer ( int size, const void *op1, const void *op2,
                               void *result );
extern void divide_unsigned_integer ( int size, const void *op1,
                                      const void *op2, void *result );
extern void divide_integer ( int size, const void *op1, const void *op2,
                             void *result );
extern void unsigned_integer_remainder ( int size, const void *op1,
    const void *op2, void *result );
extern void integer_remainder ( int size, const void *op1, const void *op2,
                                void *result );

extern void unsigned_integer_shift_right ( int size, const void *operand,
    unsigned int bits, void *result );
extern void integer_shift_right ( int size, const void *operand,
                                  unsigned int bits, void *result );
extern void integer_shift_left ( int size, const void *operand,
                                 unsigned int bits, void *result );
extern void unsigned_integer_shift_left ( int size, const void *operand,
    unsigned int bits, void *result );

extern void integer_or ( int size, const void *op1,
                         const void *op2, void *result );
extern void unsigned_integer_or ( int size, const void *op1,
                                  const void *op2, void *result );
extern void integer_and ( int size, const void *op1,
                          const void *op2, void *result );
extern void unsigned_integer_and ( int size, const void *op1,
                                   const void *op2, void *result );
extern void integer_not ( int size, const void *operand, void *result );
extern void unsigned_integer_not ( int size, const void *operand,
                                   void *result );

extern int eq_unsigned_integer ( int size, const void *op1, const void *op2 );
extern int eq_integer ( int size, const void *op1, const void *op2 );
extern int ne_unsigned_integer ( int size, const void *op1, const void *op2 );
extern int ne_integer ( int size, const void *op1, const void *op2 );
extern int gt_unsigned_integer ( int size, const void *op1, const void *op2 );
extern int gt_integer ( int size, const void *op1, const void *op2 );
extern int lt_unsigned_integer ( int size, const void *op1, const void *op2 );
extern int lt_integer ( int size, const void *op1, const void *op2 );
extern int ge_unsigned_integer ( int size, const void *op1, const void *op2 );
extern int ge_integer ( int size, const void *op1, const void *op2 );
extern int le_unsigned_integer ( int size, const void *op1, const void *op2 );
extern int le_integer ( int size, const void *op1, const void *op2 );

extern void change_unsigned_integer_size
( int operand_size, const void *operand, int result_size, void *result );
extern void change_integer_size ( int operand_size, const void *operand,
                                  int result_size, void *result );

extern char *unsigned_integer_to_string ( int size, const void *operand,
    char *result );
extern char *integer_to_string ( int size, const void *operand, char *result );

extern char *unsigned_integer_from_string ( int size, const char *operand,
    void *result );
extern char *integer_from_string ( int size, const char *operand,
                                   void *result );

char isdigit( unsigned char c );
int isspace( int c );

#endif /* #ifndef __ARITHMETIC__ */
