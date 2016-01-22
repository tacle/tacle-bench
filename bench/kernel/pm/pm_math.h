/*

  This header is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: pm_math.h

  Author: Hector Chan
          MIT Lincoln Laboratory

  Function: This header contains the C math library functions used by pm.

  Source: HPEC Challenge Benchmark Suite, Pattern Match Kernel Benchmark

  Original name: pm

  Changes: See ChangeLog.txt

  License: BSD 3-clause

*/

#ifndef PM_MATH_H
#define PM_MATH_H

/*
  Forward declaration of functions
*/

void pm_math_init( void );
float pm_fabs( float );
float pm_pow10f ( float );
float pm_log10f ( float );
float pm_floor( float );
float pm_ceil( float );

#endif /* PM_MATH_H */
