/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: bitcnt_1.c

  Author: Bob Stout & Auke Reitsma

  Function:

  Source:

  Changes: a brief summary of major functional changes (not formatting)

  License: general open-source

*/

#include "bitops.h"

int bitcount_bit_count( long x )
{
  int n = 0;
  /*
  ** The loop will execute once for each bit of x set, this is in average
  ** twice as fast as the shift/test method.
  */
  if ( x ) {
    _Pragma( "loopbound min 3 max 8" )
    do {
      n++;
    } while ( 0 != ( x = x & ( x - 1 ) ) ) ;
  }
  return ( n );
}
