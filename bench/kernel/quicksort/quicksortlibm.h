#ifndef __QUICKSORTLIBM
#define __QUICKSORTLIBM

// Often used variables/consts
#ifdef __STDC__
static  const float
#else
static  float
#endif
one    =  1.0f,
half   =  5.0000000000e-01f, /* 0x3f000000 */
zero   =  0.0f,
huge   =  1.0e30,
two8   =  2.5600000000e+02f, /* 0x43800000 */
twon8  =  3.9062500000e-03f, /* 0x3b800000 */
tiny   =  1.0e-30f,
two    =  2.0,
two24  =  16777216.0; /* 0x4b800000 */

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
