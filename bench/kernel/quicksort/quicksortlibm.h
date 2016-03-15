/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: quicksortlibm.c

  Author: Ian Lance Taylor

  Function: IEEE754 software library routines.

  Source: Sun Microsystems and Cygnus

  Original name: Unknown

  Changes: No major functional changes.

  License: See quicksortlibm.c

*/


#ifndef __QUICKSORTLIBM
#define __QUICKSORTLIBM

// The following defines map the math functions to specialized calls
#define quicksort_acos     quicksort___ieee754_acosf
#define quicksort_atan     quicksort___atanf
#define quicksort_cos      quicksort___cosf
#define quicksort_fabs     quicksort___fabsf
#define quicksort_fabsf    quicksort___fabsf
#define quicksort_isinf    quicksort___isinff
#define quicksort_pow      quicksort___ieee754_powf
#define quicksort_sqrt     quicksort___ieee754_sqrtf
#define quicksort_log10    quicksort___ieee754_log10f
#define quicksort_log      quicksort___ieee754_logf
#define quicksort_sin      quicksort___sinf

float quicksort___atanf( float );
float quicksort___copysignf( float, float );
float quicksort___cosf( float );
float quicksort___fabsf( float );
float quicksort___floorf( float );
float quicksort___ieee754_acosf( float );
float quicksort___ieee754_powf( float, float );
int quicksort___ieee754_rem_pio2f( float, float * );
float quicksort___ieee754_sqrtf( float );
int quicksort___isinff( float );
float quicksort___kernel_cosf( float, float );
float quicksort___kernel_sinf( float, float, int );
int quicksort___kernel_rem_pio2f( float *, float *, int, int, int,
                                  const int * );
float quicksort___scalbnf( float, int );
float quicksort___ieee754_logf( float );
float quicksort___ieee754_log10f( float );
float quicksort___sinf( float );

#endif // __QUICKSORTLIBM
