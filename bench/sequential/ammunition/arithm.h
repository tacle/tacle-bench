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

#include "ammunition_limits.h"


/* This page contains definitions of variables and macros common for
   all package functions. */

/* The value of macro is suggested to be maximum length of integer operands
   The length of use integers should be not greater than this value. */

#define MAX_INTEGER_OPERAND_SIZE 128

/* The following macro value is sign of integer number (0 or 1) given
   as macro parameter. */

#define INTEGER_SIGN(operand) (*(unsigned char *) (operand) >> (CHAR_BIT - 1))

extern int ammunition_overflow_bit;

extern void ammunition_add_unsigned_integer ( int size, const void *op1,
    const void *op2,
    void *result );
extern void ammunition_add_integer ( int size, const void *op1, const void *op2,
                                     void *result );
extern void ammunition_subtract_unsigned_integer ( int size, const void *op1,
    const void *op2, void *result );
extern void ammunition_subtract_integer ( int size, const void *op1,
    const void *op2,
    void *result );
extern void ammunition_multiply_unsigned_integer ( int size, const void *op1,
    const void *op2, void *result );
extern void ammunition_multiply_integer ( int size, const void *op1,
    const void *op2,
    void *result );
extern void ammunition_divide_unsigned_integer ( int size, const void *op1,
    const void *op2, void *result );
extern void ammunition_divide_integer ( int size, const void *op1,
                                        const void *op2,
                                        void *result );
extern void ammunition_unsigned_integer_remainder ( int size, const void *op1,
    const void *op2, void *result );

extern void ammunition_unsigned_integer_shift_right ( int size,
    const void *operand,
    int bits, void *result );
extern void ammunition_integer_shift_right ( int size, const void *operand,
    int bits, void *result );
extern void ammunition_integer_shift_left ( int size, const void *operand,
    int bits, void *result );
extern void ammunition_unsigned_integer_shift_left ( int size,
    const void *operand,
    int bits, void *result );

extern void ammunition_integer_or ( int size, const void *op1,
                                    const void *op2, void *result );
extern void ammunition_unsigned_integer_or ( int size, const void *op1,
    const void *op2, void *result );
extern void ammunition_integer_and ( int size, const void *op1,
                                     const void *op2, void *result );
extern void ammunition_unsigned_integer_and ( int size, const void *op1,
    const void *op2, void *result );
extern void ammunition_integer_not ( int size, const void *operand,
                                     void *result );
extern void ammunition_unsigned_integer_not ( int size, const void *operand,
    void *result );

extern int ammunition_eq_unsigned_integer ( int size, const void *op1,
    const void *op2 );
extern int ammunition_eq_integer ( int size, const void *op1, const void *op2 );
extern int ammunition_ne_unsigned_integer ( int size, const void *op1,
    const void *op2 );
extern int ammunition_ne_integer ( int size, const void *op1, const void *op2 );
extern int ammunition_gt_unsigned_integer ( int size, const void *op1,
    const void *op2 );
extern int ammunition_gt_integer ( int size, const void *op1, const void *op2 );
extern int ammunition_lt_unsigned_integer ( int size, const void *op1,
    const void *op2 );
extern int ammunition_lt_integer ( int size, const void *op1, const void *op2 );
extern int ammunition_ge_unsigned_integer ( int size, const void *op1,
    const void *op2 );
extern int ammunition_ge_integer ( int size, const void *op1, const void *op2 );
extern int ammunition_le_unsigned_integer ( int size, const void *op1,
    const void *op2 );
extern int ammunition_le_integer ( int size, const void *op1, const void *op2 );

extern void ammunition_change_unsigned_integer_size
( int operand_size, const void *operand, int result_size, void *result );
extern void ammunition_change_integer_size ( int operand_size,
    const void *operand,
    int result_size, void *result );

extern char *ammunition_unsigned_integer_to_string ( int size,
    const void *operand,
    char *result );
extern char *ammunition_integer_to_string ( int size, const void *operand,
    char *result );

extern char *ammunition_unsigned_integer_from_string ( int size,
    const char *operand,
    void *result );
extern char *ammunition_integer_from_string ( int size, const char *operand,
    void *result );

char ammunition_isdigit( unsigned char c );
int ammunition_isspace( int c );

#endif /* #ifndef __ARITHMETIC__ */
