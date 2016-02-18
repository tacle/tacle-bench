/*
**  SNIPMATH.H - Header file for SNIPPETS math functions and macros
*/

#ifndef SNIPMATH__H
#define SNIPMATH__H

#include "wcclibm.h"
#include "pi.h"

/*
**  Callable library functions begin here
*/

/* Cubic.C */
void    basicmath_solveCubic( float a, float b, float c,
                              float d, int *solutions,
                              float *x );

/*
**  File: ISQRT.C
*/

struct int_sqrt {
  unsigned short sqrt,
           frac;
};

void basicmath_usqrt( unsigned long x, struct int_sqrt *q );

#endif /* SNIPMATH__H */
