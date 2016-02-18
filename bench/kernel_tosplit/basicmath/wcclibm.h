#ifndef _WCCLIBM
#define _WCCLIBM

#define int32_t      int
#define u_int32_t    unsigned int

// Often used variables/consts
static const float one    =  1.0f,
                   tiny   =  1.0e-30f,
                   half   =  5.0000000000e-01, /* 0x3f000000 */
                   huge   =  1.0e30,
                   two    =  2.0,
                   two24  =  16777216.0,       /* 0x4b800000 */
                   zero   =  0.0;

float   basicmath___copysignf(float x, float y);
float   basicmath___cosf(float x);
float   basicmath___fabsf(float x);
float   basicmath___ieee754_acosf(float x);
float   basicmath___ieee754_powf(float x, float y);
int32_t basicmath___ieee754_rem_pio2f(float x, float *y);
float   basicmath___ieee754_sqrtf(float x);
int     basicmath___isinff (float x);
float   basicmath___kernel_cosf(float x, float y);
float   basicmath___kernel_sinf(float x, float y, int iy);
float   basicmath___scalbnf (float x, int n);

#endif // _WCCLIBM
