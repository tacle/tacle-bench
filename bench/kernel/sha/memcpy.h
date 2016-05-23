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

#include "memhelper.h"
#include "sha.h"

void *sha_glibc_memcpy( void *dstpp, const void *srcpp, size_t len );

