#include "keyword.h"

void wcc_memcpy( void *dest, void *src, int size )
{
  int i;
  _Pragma( "loopbound min 1 max 4" )
  for ( i = 0; i < size; i++ )
    ( ( unsigned char * )dest )[ i ] = ( ( unsigned char * )src )[ i ];
  return;
}
