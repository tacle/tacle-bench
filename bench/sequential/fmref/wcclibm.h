#ifndef _WCCLIBM
#define _WCCLIBM

#define size_t       unsigned long
#define int32_t      int
#define uint32_t     unsigned int
#define u_int16_t    unsigned short
#define u_int32_t    unsigned int

// Often used variables/consts
#ifdef __STDC__
static  const float
#else
static  float
#endif
fmref_one    =  1.0f,
fmref_half   =  5.0000000000e-01f, /* 0x3f000000 */
fmref_zero   =  0.0f,
fmref_huge   = 1.0e30,
fmref_two8   =  2.5600000000e+02f, /* 0x43800000 */
fmref_twon8  =  3.9062500000e-03f; /* 0x3b800000 */

// The following defines map the math functions to specialized calls
#define acos     fmref___ieee754_acosf
#define atan     fmref___atanf
#define cos      fmref___cosf
#define fabs     fmref___fabsf
#define fabsf    fmref___fabsf
#define isinf    fmref___isinff
#define pow      fmref___ieee754_powf
#define sqrt     fmref___ieee754_sqrtf
#define log10    fmref___ieee754_log10f
#define log      fmref___ieee754_logf
#define sin      fmref___sinf

float   fmref___atanf( float x );
float   fmref___copysignf( float x, float y );
float   fmref___cosf( float x );
float   fmref___fabsf( float x );
float   fmref___floorf( float x );
float   fmref___ieee754_acosf( float x );
float   fmref___ieee754_powf( float x, float y );
int32_t fmref___ieee754_rem_pio2f( float x, float *y );
float   fmref___ieee754_sqrtf( float x );
int     fmref___isinff ( float x );
float   fmref___kernel_cosf( float x, float y );
float   fmref___kernel_sinf( float x, float y, int iy );
int     fmref___kernel_rem_pio2f( float *x, float *y, int e0, int nx, int prec,
                                  const int32_t *ipio2 );
float   fmref___scalbnf ( float x, int n );
float   fmref___ieee754_logf( float x );
float   fmref___ieee754_log10f( float x );
float   fmref___sinf( float x );

#endif // _WCCLIBM
