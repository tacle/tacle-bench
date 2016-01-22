#include "wccmalloc.h"

// This must be redefined for each new benchmark
#define HEAP_SIZE 17000

char simulated_heap[HEAP_SIZE];
unsigned int freeHeapPos;

void *wccmalloc( unsigned int numberOfBytes )
{
  // Get a 4-byte adress for alignment purposes
  unsigned int offset = ( ( unsigned int )simulated_heap + freeHeapPos ) % 4;
  if ( offset )
    freeHeapPos += 4 - offset;
  void *currentPos = ( void * )&simulated_heap[freeHeapPos];
  freeHeapPos += numberOfBytes;
  return currentPos;
}

void wccbzero( char *p, unsigned long len )
{
  unsigned long i;

  _Pragma( "loopbound min 8 max 416" )
  for ( i = 0; i < len; ++i )
    *p++ = '\0';
}

