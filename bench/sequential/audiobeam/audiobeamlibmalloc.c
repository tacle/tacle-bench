#include "audiobeamlibmalloc.h"

#define AUDIOBEAM_HEAP_SIZE 10000

static char audiobeam_simulated_heap[ AUDIOBEAM_HEAP_SIZE ];
static unsigned int audiobeam_freeHeapPos;

void *audiobeam_malloc( unsigned int numberOfBytes )
{
  void *currentPos = ( void * )&audiobeam_simulated_heap[  audiobeam_freeHeapPos  ];
  /* Get a 4-byte address for alignment purposes */
  audiobeam_freeHeapPos += ( ( numberOfBytes + 4 ) & ( unsigned int )0xfffffffc );
  return currentPos;
}
