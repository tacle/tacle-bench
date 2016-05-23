/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: menset.h

  Author: Torbjorn Granlund

  NIST Secure Hash Algorithm

  Copy memory to memory until the specified number of bytes
  has been copied. Overlap is NOT handled correctly.

  Source: GNU C Library

  License: GNU Lesser General Public License

  Changes: no major functional changes
*/
#include "memhelper.h"

void *sha_glibc_memset( void *dstpp, int c, size_t len );

