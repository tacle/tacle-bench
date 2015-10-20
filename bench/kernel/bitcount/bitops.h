/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: bitops.h

  Author: Bob Stout & Auke Reitsma

  Function:

  Source:

  Changes: a brief summary of major functional changes (not formatting)

  License: general open-source

*/

#ifndef BITOPS__H
#define BITOPS__H

/*
**  bitcount_1.c
*/

int bitcount_bit_count( long x );

/*
**  bitcount_2.c
*/

int bitcount_bitcount( long i );

/*
**  bitcount_3.c
*/
void bitcount_init3( void );
int bitcount_BW_btbl_bitcount( long int x );
int bitcount_AR_btbl_bitcount( long int x );

/*
**  bitcount_4.c
*/
void bitcount_init4( void );
int bitcount_ntbl_bitcnt( unsigned long x );
int bitcount_btbl_bitcnt( unsigned long x );

#endif /*  BITOPS__H */
