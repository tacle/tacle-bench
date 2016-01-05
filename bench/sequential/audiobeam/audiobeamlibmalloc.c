#include "audiobeamlibmalloc.h"

char simulated_heap[2652];
unsigned int freeHeapPos;

void *audiobeam_malloc( unsigned int numberOfBytes )
{
  // Get a 4-byte adress for alignment purposes
  unsigned int offset = ( ( unsigned int )simulated_heap + freeHeapPos ) % 4;
  if ( offset ) {
    freeHeapPos += 4 - offset;
  }
  void *currentPos = ( void * )&simulated_heap[freeHeapPos];
  freeHeapPos += numberOfBytes;
  return currentPos;
}
