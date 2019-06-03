#ifndef _WCCLIBM
#define _WCCLIBM

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
susan_one    =  1.0f,
susan_half   =  5.0000000000e-01f, /* 0x3f000000 */
susan_zero   =  0.0f,
susan_huge   = 1.0e30,
susan_two8   =  2.5600000000e+02f, /* 0x43800000 */
susan_twon8  =  3.9062500000e-03f; /* 0x3b800000 */

// The following defines map the math functions to specialized calls
#define acos     susan___ieee754_acosf
#define atan     susan___atanf
#define cos      susan___cosf
#define fabs     susan___fabsf
#define fabsf    susan___fabsf
#define isinf    susan___isinff
#define pow      susan___ieee754_powf
#define sqrt     susan___ieee754_sqrtf
#define log10    susan___ieee754_log10f
#define log      susan___ieee754_logf
#define sin      susan___sinf

float   susan___atanf( float x );
float   susan___copysignf( float x, float y );
float   susan___cosf( float x );
float   susan___fabsf( float x );
float   susan___floorf( float x );
float   susan___ieee754_acosf( float x );
float   susan___ieee754_powf( float x, float y );
int32_t susan___ieee754_rem_pio2f( float x, float *y );
float   susan___ieee754_sqrtf( float x );
int     susan___isinff ( float x );
float   susan___kernel_cosf( float x, float y );
float   susan___kernel_sinf( float x, float y, int iy );
int     susan___kernel_rem_pio2f( float *x, float *y, int e0, int nx, int prec,
                                  const int32_t *ipio2 );
float   susan___scalbnf ( float x, int n );
float   susan___ieee754_logf( float x );
float   susan___ieee754_log10f( float x );
float   susan___sinf( float x );

#endif // _WCCLIBM
