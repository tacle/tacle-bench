#include "wccmalloc.h"

// This must be redefined for each new benchmark
#define HEAP_SIZE 30000

char susan_simulated_heap[ HEAP_SIZE ];
unsigned int susan_freeHeapPos;

void *susan_wccmalloc( unsigned int numberOfBytes )
{
  // Get a 4-byte adress for alignment purposes
  unsigned int offset = ( ( unsigned long )susan_simulated_heap +
                          susan_freeHeapPos ) % 4;
  if ( offset )
    susan_freeHeapPos += 4 - offset;
  void *currentPos = ( void * )&susan_simulated_heap[ susan_freeHeapPos ];
  susan_freeHeapPos += numberOfBytes;
  return currentPos;
}
void susan_wccfreeall( void )
{
  susan_freeHeapPos = 0;
}

void *susan_wccmemcpy( void *dstpp, const void *srcpp, unsigned int len )
{
  unsigned long int dstp = ( long int ) dstpp;
  unsigned long int srcp = ( long int ) srcpp;

  _Pragma( "loopbound min 76 max 76" )
  while ( len > 0 ) {
    char __x = ( ( char * ) srcp )[ 0 ];
    srcp += 1;
    len -= 1;
    ( ( char * ) dstp )[ 0 ] = __x;
    dstp += 1;
  }

  return dstpp;
}

void susan_wccmemset( void *p, int value, unsigned int num )
{
  unsigned long i;
  char *char_ptr = ( char * )p;

  _Pragma( "loopbound min 7220 max 7220" )
  for ( i = 0; i < num; ++i )
    *char_ptr++ = ( unsigned char )value;
}
