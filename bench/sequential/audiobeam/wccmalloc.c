#include "wccmalloc.h"

#define HEAP_SIZE 2652

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
