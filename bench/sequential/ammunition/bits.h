/*
   FILE NAME:   bits.h

   TITLE:       Include file of package for work with bits

   DESCRIPTION:
       This is header file contains macros and the ANSI C prototype
       definitions for the package for work with bits and bit strings
       and C++ class for work with bits and bit strings.  A bit is
       given by address (start address) of byte from which counting
       bits starts and its displacement which is any non negative
       number of bit from the start address.  The most significant bit
       of the start address byte has number 0.  The bit string is
       given by its first bit and its length in bits.

*/

#ifndef __BITS__
#define __BITS__

#include "ammunition_limits.h"

/* This macro value returns bit vlaue (0 or 1) with given bit
   displacement (0, 1, ...).  The macro has side effects!  Value of
   `bit_displacement' must be nonegative and can be greater than
   CHAR_BIT. */

#define BIT(start_byte, bit_displacement)\
  ((((const char *) (start_byte)) [(bit_displacement) / CHAR_BIT]\
    >> (CHAR_BIT - 1 - (bit_displacement) % CHAR_BIT)) & 1)


/* This macro value sets up new value (must be `0' or `1') of a given
   bit (bit displacement starts with 0).  The macro has side effects!
   Value of `bit_displacement' must be nonegative and can be greater
   than CHAR_BIT. */

#define SET_BIT(start_byte, bit_displacement, bit)\
  (((char *) (start_byte)) [(bit_displacement) / CHAR_BIT]\
   = (((char *) (start_byte)) [(bit_displacement) / CHAR_BIT]\
      & ~(1 << (CHAR_BIT - 1 - (bit_displacement) % CHAR_BIT)))\
     | ((bit) << (CHAR_BIT - 1 - (bit_displacement) % CHAR_BIT)))

int ammunition_is_zero_bit_string ( const void *start_byte,
                                    int bit_displacement,
                                    int bit_length );
void ammunition_bit_string_set ( void *start_byte, int bit_displacement,
                                 int bit,
                                 int bit_length );
void ammunition_bit_string_copy ( void *to, int to_bit_displacement,
                                  const void *from, int from_bit_displacement,
                                  int bit_length );
void ammunition_bit_string_move ( void *to, int to_bit_displacement,
                                  const void *from, int from_bit_displacement,
                                  int bit_length );
int ammunition_bit_string_comparison ( const void *str1, int bit_displacement1,
                                       const void *str2, int bit_displacement2,
                                       int bit_length );

#endif /* #ifndef __BITS__ */
