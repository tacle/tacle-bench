/* This must be redefined for each new benchmark */
#define CODHUFF_HEAP_SIZE 10000

char codhuff_heap[CODHUFF_HEAP_SIZE];
unsigned int codhuff_freeHeapPos = 0;

void *codhuff_malloc( unsigned int numberOfBytes )
{
  if ( codhuff_freeHeapPos + numberOfBytes > CODHUFF_HEAP_SIZE ) {
    return 0;
  } else {
    // Get a 4-byte adress for alignment purposes
    unsigned int offset = ( ( unsigned int )codhuff_heap + codhuff_freeHeapPos )
                          % 4;
    if ( offset ) {
      codhuff_freeHeapPos += 4 - offset;
    }
    void *currentPos = ( void * )&codhuff_heap[codhuff_freeHeapPos];
    codhuff_freeHeapPos += numberOfBytes;
    return currentPos;
  }
}
