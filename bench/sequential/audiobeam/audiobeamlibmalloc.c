#include "audiobeamlibmalloc.h"

char simulated_heap[2652];
unsigned int freeHeapPos;

void *audiobeam_malloc( unsigned int numberOfBytes )
{
  unsigned int offset;
  
  #if __SIZEOF_POINTER__ == 8
    offset = ( ( unsigned long )simulated_heap +  freeHeapPos ) % 4;
  #elif
    offset = ( ( unsigned int )simulated_heap +  freeHeapPos ) % 4;      
  #endif
      
  // Get a 4-byte adress for alignment purposes
  if ( offset ) {
    freeHeapPos += 4 - offset;
  }
  void *currentPos = ( void * )&simulated_heap[freeHeapPos];
  freeHeapPos += numberOfBytes;
  return currentPos;
}
