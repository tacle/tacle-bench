/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include "bitops.h"

#define FUNCS  8

static int bit_shifter(long int x)
{
  int i, n;

  _Pragma( "loopbound min 31 max 31" )
  for (i = n = 0; x && (i < (sizeof(long) * CHAR_BIT)); ++i, x >>= 1)
    n += (int)(x & 1L);
  return n;
}

unsigned long _randseed = 1;
unsigned long random(void)
{
  long x, hi, lo, t;

  /*
   * Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
   * From "Random number generators: good ones are hard to find",
   * Park and Miller, Communications of the ACM, vol. 31, no. 10,
   * October 1988, p. 1195.
   */
  x = _randseed;
  hi = x / 127773;
  lo = x % 127773;
  t = 16807 * lo - 2836 * hi;
  if (t <= 0)
    t += 0x7fffffff;
  _randseed = t;
  return (t);
}

int main( void )
{
  unsigned int i;
  int res;
  unsigned long j, seed;
  unsigned long n = 0;
  unsigned int iterations = 10;

  _Pragma( "loopbound min 8 max 8" )
  for (i = 0; i < FUNCS; i++) {
    _Pragma( "loopbound min 10 max 10" )
    for (j = 0, seed = random(); j < iterations; j++, seed += 13) {
      // The original calls were done by function pointers
      switch (i) {
        case 0: res = bit_count( seed ); break;
        case 1: res = bitcount( seed ); break;
        case 2: {
            _Pragma( "marker call_ntbl" )
            res = ntbl_bitcnt( seed );
            break;
        }
        case 3: {
            _Pragma( "marker call_btbl" )
            res = btbl_bitcnt( seed );
            break;
        }
        case 4: res = ntbl_bitcount( seed ); break;
        case 5: res = BW_btbl_bitcount( seed ); break;
        case 6: res = AR_btbl_bitcount( seed ); break;
        case 7: res = bit_shifter( seed ); break;
        default: return 1;
      }
      n += res;
    }
  }
  _Pragma( "flowrestriction 1*ntbl_bitcnt <= 8*call_ntbl" )
  _Pragma( "flowrestriction 1*btbl_bitcnt <= 4*call_btbl" )
  return 0;
}
