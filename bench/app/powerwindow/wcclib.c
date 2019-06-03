#include "wcclib.h"


void *memset( void *s, int c, size_t n )
{
  unsigned char *p = s;

  _Pragma( "loopbound min 1 max 368" )
  while ( n-- )
    *p++ = ( unsigned char ) c;
  return ( s );
}
