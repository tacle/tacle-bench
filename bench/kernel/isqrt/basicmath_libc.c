/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: basicmath_libc

  Author: unknown

  Function: Implementation of basicmath_memcpy

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Original name: basicmath_small

  Changes: no major functional changes

  License: this code is FREE with no restrictions

*/


void basicmath_memcpy( void *a, const void *b, int c )
{
  char *dest = ( char * ) a;
  char *source = ( char * ) b;
  int copied;
  _Pragma( "loopbound min 4 max 4" )
  for ( copied = 0; copied < c; copied++ ) {
    *dest = *source;
    dest++;
    source++;
  }
}
