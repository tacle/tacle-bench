/*

  This header is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: pm_string.h

  Author: Hector Chan
          MIT Lincoln Laboratory

  Function: This header contains the C standard library functions used by pm.

  Source: HPEC Challenge Benchmark Suite, Pattern Match Kernel Benchmark

  Original name: pm

  Changes: See ChangeLog.txt

  License: BSD 3-clause

*/

#ifndef PM_STRING_H
#define PM_STRING_H

/*
  Forward declaration of functions
*/

void pm_memcpy( void *, void *, int );
void pm_memset( void *, int, int );

#endif /* PM_STRING_H */

