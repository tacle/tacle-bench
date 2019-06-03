/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: menset.h

  Author: Torbjorn Granlund

  NIST Secure Hash Algorithm

  Copy memory to memory until the specified number of bytes
  has been copied. Overlap is NOT handled correctly.

  Source: GNU C Library

  Changes: no major functional changes

  License: GNU Lesser General Public License
*/
#include "memset.h"
int printf(const char * restrict format, ... );

void *sha_glibc_memset( void *dstpp, int c, size_t len )
{
  long int dstp = ( long int ) dstpp;

  if ( len >= 8 ) {
    size_t xlen;
    op_t cccc;

    cccc = ( unsigned char ) c;
    cccc |= cccc << 8;
    cccc |= cccc << 16;
    if ( OPSIZ > 4 )
      /* Do the shift in two steps to avoid warning if long has 32 bits.  */
      cccc |= ( cccc << 16 ) << 16;

    /* There are at least some bytes to set.
      No need to test for LEN == 0 in this alignment loop.  */
    _Pragma( "loopbound min 3 max 3" )
    while ( dstp % OPSIZ != 0 ) {
      ( ( BYTE * ) dstp )[ 0 ] = c;
      dstp += 1;
      len -= 1;
    }

    /* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
    xlen = len / ( OPSIZ * 8 );
    _Pragma( "loopbound min 0 max 1" )
    while ( xlen > 0 ) {
      ( ( op_t * ) dstp )[ 0 ] = cccc;
      ( ( op_t * ) dstp )[ 1 ] = cccc;
      ( ( op_t * ) dstp )[ 2 ] = cccc;
      ( ( op_t * ) dstp )[ 3 ] = cccc;
      ( ( op_t * ) dstp )[ 4 ] = cccc;
      ( ( op_t * ) dstp )[ 5 ] = cccc;
      ( ( op_t * ) dstp )[ 6 ] = cccc;
      ( ( op_t * ) dstp )[ 7 ] = cccc;
      dstp += 8 * OPSIZ;
      xlen -= 1;
    }
    len %= OPSIZ * 8;

    /* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
    xlen = len / OPSIZ;
    _Pragma( "loopbound min 1 max 2" )      
    while ( xlen > 0 ) {
      ( ( op_t * ) dstp )[ 0 ] = cccc;
      dstp += OPSIZ;
      xlen -= 1;
    }
    len %= OPSIZ;
  }

  /* Write the last few bytes.  */
  _Pragma( "loopbound min 0 max 0" )
  while ( len > 0 ) {
    ( ( BYTE * ) dstp )[ 0 ] = c;
    dstp += 1;
    len -= 1;
  }

  return ( void * )dstpp;
}

