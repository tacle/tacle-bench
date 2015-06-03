#ifndef _WCCLIBM
#define _WCCLIBM

#define double       float
#define size_t       unsigned long
#define int32_t      int
#define uint32_t     unsigned int
#define u_int16_t    unsigned short
#define u_int32_t    unsigned int

#define ieee_double_shape_type float
#define M_PI 3.14159265358979323846

// Often used variables/consts
#ifdef __STDC__
static	const float	
#else
static	float	
#endif
one	   =  1.0f,
tiny   =  1.0e-30f,
half   =  5.0000000000e-01, /* 0x3f000000 */
huge   =  1.0e30,
two8   =  2.5600000000e+02, /* 0x43800000 */
twon8  =  3.9062500000e-03, /* 0x3b800000 */
zero   =  0.0;

// The following defines map the math functions to specialized calls
#define cos      __cosf
#define fabs     __fabsf
#define fabsf    __fabsf
#define isinf    __isinff
#define sqrt     __ieee754_sqrtf
#define ceil     __ceilf
#define floor    __floorf
#define sqrt     __ieee754_sqrtf

float   __copysignf(float x, float y);
float   __cosf(float x);
float   __fabsf(float x);
float   __floorf(float x);
int32_t __ieee754_rem_pio2f(float x, float *y);
float   __ieee754_sqrtf(float x);
int     __isinff (float x);
float   __kernel_cosf(float x, float y);
float   __kernel_sinf(float x, float y, int iy);
int     __kernel_rem_pio2f(float *x, float *y, int e0, int nx, int prec, const int32_t *ipio2);
float   __scalbnf (float x, int n);
float   __ceilf(float x);
float   __floorf(float x);

#endif // _WCCLIBM
