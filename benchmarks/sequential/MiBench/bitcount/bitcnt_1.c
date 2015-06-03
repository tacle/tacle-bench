/* +++Date last modified: 05-Jul-1997 */

/*
**  Bit counter by Ratko Tomic
*/

#include "bitops.h"

int bit_count(long x)
{
  int n = 0;
  /*
  ** The loop will execute once for each bit of x set, this is in average
  ** twice as fast as the shift/test method.
  */
  if (x) {
    _Pragma( "loopbound min 3 max 8" )
    do {
      n++;
    } while (0 != (x = x & (x - 1))) ;
  }
  return (n);
}
