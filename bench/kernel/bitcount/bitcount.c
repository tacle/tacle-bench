/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: bitcount.c

  Author: Bob Stout & Auke Reitsma

  Function: test program for bit counting functions

  Source: www.snippest.com

  Changes: no major functional changes

  License: May be used, modified, and re-distributed freely.

*/


#include "bitops.h"

#define FUNCS  8

/*
   Global variables
*/

unsigned long bitcount_randseed;
int bitcount_res;
unsigned long bitcount_seed;
unsigned long bitcount_n;
unsigned int bitcount_iterations;

/*
   First declaration of the functions
*/
int bitcount_bit_shifter( long int x );
unsigned long bitcount_random( void );
void bitcount_main();
int bitcount_return();
void bitcount_init();
int main( void );


int bitcount_bit_shifter( long int x )
{
  int n;
  unsigned int i;

  _Pragma( "loopbound min 31 max 31" )
  for ( i = n = 0; x && ( i < ( sizeof( long ) * 8 ) ); ++i, x >>= 1 )
    n += ( int )( x & 1L );
  return n;
}

int bitcount_return()
{
  return ( bitcount_n + ( -1095 ) ) != 0;
}

void bitcount_init()
{
  bitcount_randseed = 1;
  bitcount_n = 0;
  bitcount_iterations = 10;

  bitcount_init3();
  bitcount_init4();
}

unsigned long bitcount_random( void )
{
  long x, hi, lo, t;

  /*
     Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
     From "Random number generators: good ones are hard to find",
     Park and Miller, Communications of the ACM, vol. 31, no. 10,
     October 1988, p. 1195.
  */
  x = bitcount_randseed;
  hi = x / 127773;
  lo = x % 127773;
  t = 16807 * lo - 2836 * hi;
  if ( t <= 0 )
    t += 0x7fffffff;
  bitcount_randseed = t;
  return ( t );
}

void _Pragma( "entrypoint" ) bitcount_main()
{
  unsigned int i, j;
  _Pragma( "loopbound min 8 max 8" )
  for ( i = 0; i < FUNCS; i++ ) {
    _Pragma( "loopbound min 10 max 10" )
    for ( j = 0, bitcount_seed = bitcount_random(); j < bitcount_iterations;
          j++, bitcount_seed += 13 ) {
      // The original calls were done by function pointers
      switch ( i ) {
        case 0:
          bitcount_res = bitcount_bit_count( bitcount_seed );
          break;
        case 1:
          bitcount_res = bitcount_bitcount( bitcount_seed );
          break;
        case 2: {
            _Pragma( "marker call_ntbl" )
            bitcount_res = bitcount_ntbl_bitcnt( bitcount_seed );
            break;
          }
        case 3: {
            _Pragma( "marker call_btbl" )
            bitcount_res = bitcount_btbl_bitcnt( bitcount_seed );
            break;
          }
        case 4:
          bitcount_res = bitcount_ntbl_bitcount( bitcount_seed );
          break;
        case 5:
          bitcount_res = bitcount_BW_btbl_bitcount( bitcount_seed );
          break;
        case 6:
          bitcount_res = bitcount_AR_btbl_bitcount( bitcount_seed );
          break;
        case 7:
          bitcount_res = bitcount_bit_shifter( bitcount_seed );
          break;
        default:
          break;
      }
      bitcount_n += bitcount_res;
    }
  }
  _Pragma( "flowrestriction 1*ntbl_bitcount <= 8*call_ntbl" )
  _Pragma( "flowrestriction 1*btbl_bitcount <= 4*call_btbl" )
}

int main( void )
{
  bitcount_init();
  bitcount_main();

  return ( bitcount_return() );
}
