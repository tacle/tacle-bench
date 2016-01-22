extern unsigned auGlobalFrequency[26];

/* This function is included here because the WCC does not */
/* support function pointers */
int CompareFrequency( char *pch1, char *pch2 )
{
  return auGlobalFrequency[*pch1] < auGlobalFrequency[*pch2]
         ? -1 :
         auGlobalFrequency[*pch1] == auGlobalFrequency[*pch2]
         ? 0 : 1;
}

static void swapi( char *ii, char *ij, unsigned long es )
{
  char *i, *j, c;

  i = ( char * )ii;
  j = ( char * )ij;
  _Pragma( "loopbound min 1 max 1" )
  do {
    c = *i;
    *i++ = *j;
    *j++ = c;
    es -= sizeof( char );
  } while ( es != 0 );
}

static char *pivot( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pk;

  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */
  if ( CompareFrequency( pi, pj ) < 0 ) {
    if ( CompareFrequency( pi, pk ) < 0 ) {
      if ( CompareFrequency( pj, pk ) < 0 )
        return pj;
      return pk;
    }
    return pi;
  }
  if ( CompareFrequency( pj, pk ) < 0 ) {
    if ( CompareFrequency( pi, pk ) < 0 )
      return pi;
    return pk;
  }
  return pj;
}

static void qsorts( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pn;
  volatile unsigned int flowfactdummy = 0;

  _Pragma( "loopbound min 0 max 6" )
  while ( n > 1 ) {
    if ( n > 10 )
      pi = pivot( a, n, es );
    else
      pi = a + ( n >> 1 ) * es;

    swapi( a, pi, es );
    pi = a;
    pn = a + n * es;
    pj = pn;
    _Pragma( "loopbound min 1 max 11" )
    while ( 1 ) {
      /* wcc note: this assignment expression was added to avoid assignment of
         multiple loop bound annotations to same loop (cf. Ticket #0002323). */
      flowfactdummy++;
      _Pragma( "loopbound min 1 max 5" )
      do {
        pi += es;
      } while ( pi < pn && CompareFrequency( pi, a ) < 0 );
      _Pragma( "loopbound min 1 max 4" )
      do {
        pj -= es;
      } while ( pj > a && CompareFrequency( pj, a ) > 0 );
      if ( pj < pi )
        break;
      swapi( pi, pj, es );
    }
    swapi( a, pj, es );
    j = ( pj - a ) / es;

    n = n - j - 1;
    if ( j >= n ) {
      qsorts( a, j, es );
      a += ( j + 1 ) * es;
    } else {
      qsorts( a + ( j + 1 )*es, n, es );
      n = j;
    }
  }
}

void wccqsort( void *va, unsigned long n, unsigned long es )
{
  _Pragma( "marker call_qsorts" )
  qsorts( ( char * )va, n, es );
  _Pragma( "flowrestriction 1*qsorts <= 17*call_qsorts" )
}


#include "anagram_stdlib.h"
#include "anagram_strings.h"

/* This must be redefined for each new benchmark */
#define HEAP_SIZE 17000

char simulated_heap[HEAP_SIZE];
unsigned int freeHeapPos;

void *wccmalloc( unsigned int numberOfBytes )
{
  /* Get a 4-byte adress for alignment purposes */
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

