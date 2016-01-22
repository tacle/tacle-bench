/*
  Include section
*/

#include "pm_string.h"


/*
  Standard library functions
*/

void pm_memcpy( void *dest, void *src, int size )
{
  int i;
  _Pragma( "loopbound min 44 max 256" )
  for ( i = 0; i < size; i++ )
    ( ( unsigned char * )dest )[i] = ( ( unsigned char * )src )[i];
  return;
}


void pm_memset( void *s, int c, int n )
{
  int i;
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < n; i++ )
    ( ( unsigned char * )s )[i] = (unsigned char)c;
  return;
}

