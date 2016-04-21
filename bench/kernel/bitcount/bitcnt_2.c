/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: bitcnt_2.c

  Author: Bob Stout & Auke Reitsma

  Function: Test program for bit counting functions

  Source: http://www.snippets.org/

  Changes: no major functional changes

  License: May be used, modified, and re-distributed freely.

*/


#include "bitops.h"

int bitcount_bitcount( long i )
{
  i = ( ( i & 0xAAAAAAAAL ) >> 1 ) + ( i & 0x55555555L );
  i = ( ( i & 0xCCCCCCCCL ) >> 2 ) + ( i & 0x33333333L );
  i = ( ( i & 0xF0F0F0F0L ) >> 4 ) + ( i & 0x0F0F0F0FL );
  i = ( ( i & 0xFF00FF00L ) >> 8 ) + ( i & 0x00FF00FFL );
  i = ( ( i & 0xFFFF0000L ) >> 16 ) + ( i & 0x0000FFFFL );
  return ( int )i;
}
