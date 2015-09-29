/* +++Date last modified: 05-Jul-1997 */

/*
**  Macros and prototypes for bit operations
**
**  public domain for SNIPPETS by:
**    Scott Dudley
**    Auke Reitsma
**    Ratko Tomic
**    Aare Tali
**    J. Blauth
**    Bruce Wedding
**    Bob Stout
*/

#ifndef BITOPS__H
#define BITOPS__H

#include "sniptype.h"                           /* For TOBOOL()         */

typedef unsigned int size_t;

/*
**  Macros to manipulate bits in any integral data type.
*/

#define BitSet(arg,posn) ((arg) | (1L << (posn)))
#define BitClr(arg,posn) ((arg) & ~(1L << (posn)))
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn)))
#define BitTst(arg,posn) TOBOOL((arg) & (1L << (posn)))

/*
**  Macros to manipulate bits in an array of char.
**  These macros assume CHAR_BIT is one of either 8, 16, or 32.
*/

#define CHAR_BIT 8
#define MASK  CHAR_BIT-1
#define SHIFT ((CHAR_BIT==8)?3:(CHAR_BIT==16)?4:8)

#define BitOff(a,x)  ((void)((a)[(x)>>SHIFT] &= ~(1 << ((x)&MASK))))
#define BitOn(a,x)   ((void)((a)[(x)>>SHIFT] |=  (1 << ((x)&MASK))))
#define BitFlip(a,x) ((void)((a)[(x)>>SHIFT] ^=  (1 << ((x)&MASK))))
#define IsBit(a,x)   ((a)[(x)>>SHIFT]        &   (1 << ((x)&MASK)))

/*
**  BITCNT_1.C
*/

int bit_count(long x);

/*
**  BITCNT_2.C
*/

int bitcount(long i);

/*
**  BITCNT_3.C
*/

int ntbl_bitcount(long int x);
int BW_btbl_bitcount(long int x);
int AR_btbl_bitcount(long int x);

/*
**  BITCNT_4.C
*/

int ntbl_bitcnt(unsigned long x);
int btbl_bitcnt(unsigned long x);

#endif /*  BITOPS__H */
