#ifndef _WCCLIB
#define _WCCLIB

#define size_t       unsigned long
#define int32_t      int
#define uint32_t     unsigned int
#define u_int16_t    unsigned short
#define u_int32_t    unsigned int

#define NULL ( (void *) 0)

void *wcclib_memset( void *s, int c, size_t n );

#endif // _WCCLIB
