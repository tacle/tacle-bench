/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: isqrt

  Author: unknown

  Function: isqrt calculates the integer square root of a number

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Original name: basicmath_small

  Changes: no major functional changes

  License: this code is FREE with no restrictions

*/

#include "basicmath_libc.h"
#include "snipmath.h"

#define BITSPERLONG 32
#define TOP2BITS(x) ((x & (3L << (BITSPERLONG-2))) >> (BITSPERLONG-2))

/* usqrt:
    ENTRY x: unsigned long
    EXIT  returns floor(sqrt(x) * pow(2, BITSPERLONG/2))

    Since the square root never uses more than half the bits
    of the input, we use the other half of the bits to contain
    extra bits of precision after the binary point.

    EXAMPLE
        suppose BITSPERLONG = 32
        then    usqrt(144) = 786432 = 12 * 65536
                usqrt(32) = 370727 = 5.66 * 65536

    NOTES
        (1) change BITSPERLONG to BITSPERLONG/2 if you do not want
            the answer scaled.  Indeed, if you want n bits of
            precision after the binary point, use BITSPERLONG/2+n.
            The code assumes that BITSPERLONG is even.
        (2) This is really better off being written in assembly.
            The line marked below is really a "arithmetic shift left"
            on the double-long value with r in the upper half
            and x in the lower half.  This operation is typically
            expressible in only one or two assembly instructions.
        (3) Unrolling this loop is probably not a bad idea.

    ALGORITHM
        The calculations are the base-two analogue of the square
        root algorithm we all learned in grammar school.  Since we're
        in base 2, there is only one nontrivial trial multiplier.

        Notice that absolutely no multiplications or divisions are performed.
        This means it'll be fast on a wide range of processors.
*/


/*
  Forward declaration of functions
*/

void isqrt_usqrt( unsigned long x, struct int_sqrt *q );
void isqrt_init( void );
void isqrt_main( void );
int isqrt_return( void );
int main( void );


/*
  Declaration of global variables
*/

int isqrt_i;
struct int_sqrt isqrt_q;
unsigned long isqrt_l;
unsigned long isqrt_checksum;


/*
  Initialization function
*/

void isqrt_init( void )
{
  isqrt_l = 0x3fed0169L;
  isqrt_checksum = 0;
}


/*
  Return function
*/

int isqrt_return( void )
{
  if ( isqrt_checksum == 53364 )
    return 0;
  else
    return -1;
}


/*
  Main functions
*/

void isqrt_usqrt( unsigned long x, struct int_sqrt *q )
{
  unsigned long a = 0L;                   /* accumulator      */
  unsigned long r = 0L;                   /* remainder        */
  unsigned long e = 0L;                   /* trial product    */

  int i;

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < BITSPERLONG; i++ ) { /* NOTE 1 */
    r = ( r << 2 ) + TOP2BITS( x );
    x <<= 2;                            /* NOTE 2 */
    a <<= 1;
    e = ( a << 1 ) + 1;
    if ( r >= e ) {
      r -= e;
      a++;
    }
  }
  basicmath_memcpy( q, &a, sizeof( *q ) );
}


void _Pragma( "entrypoint" ) isqrt_main( void )
{
  /* perform some integer square roots */
  _Pragma( "loopbound min 1000 max 1000" )
  for ( isqrt_i = 1; isqrt_i < 1001; isqrt_i += 1 ) {
    isqrt_usqrt( isqrt_i, &isqrt_q );
    isqrt_checksum += isqrt_q.frac;
    // remainder differs on some machines
  }
  isqrt_usqrt( isqrt_l, &isqrt_q );
  isqrt_checksum += isqrt_q.frac;
}


int main( void )
{
  isqrt_init();
  isqrt_main();
  return isqrt_return();
}

