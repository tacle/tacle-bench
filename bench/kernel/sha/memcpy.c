/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: memcpy.c

  Author: Torbjorn Granlund

  NIST Secure Hash Algorithm

  Copy memory to memory until the specified number of bytes
  has been copied. Overlap is NOT handled correctly.

  Source: GNU C Library

  Changes: no major functional changes

  License: Copyright (C) 1991, 1997, 2003 Free Software Foundation, Inc.
*/

#include "memcpy.h"

void *sha_glibc_memcpy( void *dstpp, const void *srcpp, size_t len )
{
  unsigned long int dstp = ( long int ) dstpp;
  unsigned long int srcp = ( long int ) srcpp;
  size_t __nbytes;

  /* Copy from the beginning to the end.  */

  /* If there not too few bytes to copy, use word copy.  */
  if ( len >= OP_T_THRES ) {
    /* Copy just a few bytes to make DSTP aligned.  */
    len -= ( -dstp ) % OPSIZ;

    __nbytes = ( -dstp ) % OPSIZ;
    _Pragma( "loopbound min 0 max 0" )
    while ( __nbytes > 0 ) {
      BYTE __x = ( ( BYTE * ) srcp )[ 0 ];
      srcp += 1;
      __nbytes -= 1;
      ( ( BYTE * ) dstp )[ 0 ] = __x;
      dstp += 1;
    }

    /* Copy whole pages from SRCP to DSTP by virtual address manipulation,
      as much as possible.  */

    PAGE_COPY_FWD_MAYBE ( dstp, srcp, len, len );

    /* Copy from SRCP to DSTP taking advantage of the known alignment of
      DSTP.  Number of bytes remaining is put in the third argument,
      i.e. in LEN.  This number may vary from machine to machine.  */

    WORD_COPY_FWD ( dstp, srcp, len, len );

    /* Fall out and copy the tail.  */
  }

  /* There are just a few bytes to copy.  Use byte memory operations.  */
  __nbytes = len;
  _Pragma( "loopbound min 0 max 7" )
  while ( __nbytes > 0 ) {
    BYTE __x = ( ( BYTE * ) srcp )[ 0 ];
    srcp += 1;
    __nbytes -= 1;
    ( ( BYTE * ) dstp )[ 0 ] = __x;
    dstp += 1;
  }

  return dstpp;
}

