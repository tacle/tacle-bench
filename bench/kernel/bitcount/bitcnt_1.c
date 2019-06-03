/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: bitcnt_1.c

  Author: Ratko Tomic

  Function: Test program for bit counting functions

  Source: http://www.snippets.org/.

  Changes: no major functional changes

  License: May be used, modified, and re-distributed freely.

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
