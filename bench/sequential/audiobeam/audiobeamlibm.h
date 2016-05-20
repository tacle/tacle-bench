/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: quicksortlibm.c

  Author: Ian Lance Taylor

  Function: IEEE754 software library routines.

  Source: Sun Microsystems and Cygnus

  Original name: Unknown

  Changes: No major functional changes.

  License: See audiobeamlibm.c

*/

#ifndef AUDIOBEAM_LIBM
#define AUDIOBEAM_LIBM

#define audiobeam_M_PI 3.14159265358979323846

static  const float
audiobeam_one    =  1.0f,
audiobeam_tiny   =  1.0e-30f,
audiobeam_half   =  5.0000000000e-01, /* 0x3f000000 */
audiobeam_huge   =  1.0e30,
audiobeam_two8   =  2.5600000000e+02, /* 0x43800000 */
audiobeam_twon8  =  3.9062500000e-03, /* 0x3b800000 */
audiobeam_zero   =  0.0;

#define audiobeam_cos      audiobeam___cosf
#define audiobeam_fabs     audiobeam___fabsf
#define audiobeam_fabsf    audiobeam___fabsf
#define audiobeam_isinf    audiobeam___isinff
#define audiobeam_sqrt     audiobeam___ieee754_sqrtf
#define audiobeam_ceil     audiobeam___ceilf
#define audiobeam_floor    audiobeam___floorf

float   audiobeam___copysignf( float x, float y );
float   audiobeam___cosf( float x );
float   audiobeam___fabsf( float x );
float   audiobeam___floorf( float x );
int     audiobeam___ieee754_rem_pio2f( float x, float *y );
float   audiobeam___ieee754_sqrtf( float x );
int     audiobeam___isinff ( float x );
float   audiobeam___kernel_cosf( float x, float y );
float   audiobeam___kernel_sinf( float x, float y, int iy );
int     audiobeam___kernel_rem_pio2f( float *x, float *y, int e0, int nx,
                                      int prec, const int *ipio2 );
float   audiobeam___scalbnf ( float x, int n );
float   audiobeam___ceilf( float x );
float   audiobeam___floorf( float x );

#endif // AUDIOBEAM_LIBM
