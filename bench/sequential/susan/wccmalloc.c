#include "wccmalloc.h"

// This must be redefined for each new benchmark
#define HEAP_SIZE 30000

char simulated_heap[HEAP_SIZE];
unsigned int freeHeapPos;

void* wccmalloc( unsigned int numberOfBytes )
{
  // Get a 4-byte adress for alignment purposes
  unsigned int offset = ( (unsigned int)simulated_heap + freeHeapPos ) % 4;
  if ( offset ) {
    freeHeapPos += 4 - offset;
  }
  void* currentPos = (void*)&simulated_heap[freeHeapPos];
  freeHeapPos += numberOfBytes;
  return currentPos;
}
void wccfreeall( void ) 
{
  freeHeapPos = 0;
}

void* wccmemcpy(void* dstpp, const void* srcpp, unsigned int len)
{
  unsigned long int dstp = (long int) dstpp;
  unsigned long int srcp = (long int) srcpp;

  _Pragma("loopbound min 76 max 76")
  while (len > 0) {
    char __x = ((char *) srcp)[0];
    srcp += 1;
    len -= 1;
    ((char *) dstp)[0] = __x;
    dstp += 1;
  }

  return dstpp;
}

void wccmemset( void *p, int value, unsigned int num )
{
  unsigned long i;
  char *char_ptr = (char*)p;

  _Pragma( "loopbound min 7220 max 28880" )
  for ( i = 0; i < num; ++i ) {
    *char_ptr++ = (unsigned char)value;
  }
}
