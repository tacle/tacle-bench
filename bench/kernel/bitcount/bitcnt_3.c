/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: bitcnt_3.c

  Author: Bob Stout & Auke Reitsma

  Function: Bit counting functions using table lookup

  Source: http://www.snippets.org/

  Changes: no major functional changes

  License: May be used, modified, and re-distributed freely.

*/


#include "bitops.h"

static char bitcount_bits[ 256 ];

/*
**  Count bits in each nybble
**
**  Note: Only the first 16 table entries are used, the rest could be
**        omitted.
*/

void bitcount_init3( void )
{
  int volatile i = 0;
  char bitcount_bits_tmp[ 256 ] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
  };

  for ( i = 0; i < 256; i++ )
    bitcount_bits[ i ] = bitcount_bits_tmp[ i ];
}

int bitcount_ntbl_bitcount( long int x )
{
  return
    bitcount_bits[  ( int ) ( x & 0x0000000FUL )  ] +
    bitcount_bits[  ( int )( ( x & 0x000000F0UL ) >> 4 )  ] +
    bitcount_bits[  ( int )( ( x & 0x00000F00UL ) >> 8 )  ] +
    bitcount_bits[  ( int )( ( x & 0x0000F000UL ) >> 12 ) ] +
    bitcount_bits[  ( int )( ( x & 0x000F0000UL ) >> 16 ) ] +
    bitcount_bits[  ( int )( ( x & 0x00F00000UL ) >> 20 ) ] +
    bitcount_bits[  ( int )( ( x & 0x0F000000UL ) >> 24 ) ] +
    bitcount_bits[  ( int )( ( x & 0xF0000000UL ) >> 28 ) ];
}

/*
**  Count bits in each byte
**
**  by Bruce Wedding, works best on Watcom & Borland
*/

int bitcount_BW_btbl_bitcount( long int x )
{
  union {
    unsigned char ch[ 4 ];
    long y;
  } U;

  U.y = x;

  return bitcount_bits[  U.ch[ 0 ]  ] + bitcount_bits[  U.ch[ 1 ]  ] +
         bitcount_bits[  U.ch[ 3 ]  ] + bitcount_bits[  U.ch[ 2 ]  ];
}

/*
**  Count bits in each byte
**
**  by Auke Reitsma, works best on Microsoft, Symantec, and others
*/

int bitcount_AR_btbl_bitcount( long int x )
{
  unsigned char *ptr = ( unsigned char * ) & x ;
  int accu ;

  accu = bitcount_bits[  *ptr++  ];
  accu += bitcount_bits[  *ptr++  ];
  accu += bitcount_bits[  *ptr++  ];
  accu += bitcount_bits[  *ptr  ];
  return accu;
}
