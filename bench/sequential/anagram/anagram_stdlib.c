/*

  This file is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: anagram_stdlib.c

  Author: Raymond Chen

  Function: This file contains the C standard library functions used by anagram.

  Source: anagram

  Original name: anagram

  Changes: See ChangeLog.txt

  License: See anagram.c

*/
#include "anagram_stdlib.h"
#include "anagram_strings.h"

/* Includes anagram_CompareFrequency */
/* This function is included here because the WCC does not */
/* support function pointers */
#include "anagram_compare.h"

void anagram_swapi( char *ii, char *ij, unsigned long es )
{
  char *i, *j, c;

  i = ( char * )ii;
  j = ( char * )ij;
  _Pragma( "loopbound min 4 max 4" )
  do {
    c = *i;
    *i ++ = *j;
    *j ++ = c;
    es -= sizeof( char );
  } while ( es != 0 );
}

char *anagram_pivot( char *a, unsigned long n, unsigned long es )
{
  unsigned long j;
  char *pi, *pj, *pk;

  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */
  if ( anagram_CompareFrequency( pi, pj ) < 0 ) {
    if ( anagram_CompareFrequency( pi, pk ) < 0 ) {
      if ( anagram_CompareFrequency( pj, pk ) < 0 )
        return pj;
      return pk;
    }
    return pi;
  }
  if ( anagram_CompareFrequency( pj, pk ) < 0 ) {
    if ( anagram_CompareFrequency( pi, pk ) < 0 )
      return pi;
    return pk;
  }
  return pj;
}

void anagram_qsorts( char *a, unsigned long n, unsigned long es )
{
  unsigned long j;
  char *pi, *pj, *pn;
  volatile unsigned int flowfactdummy = 0;
  _Pragma( "loopbound min 0 max 3" )
  while ( n > 1 ) {
    if ( n > 10 )
      pi = anagram_pivot( a, n, es );
    else
      pi = a + ( n >> 1 ) * es;

    anagram_swapi( a, pi, es );
    pi = a;
    pn = a + n * es;
    pj = pn;
    _Pragma( "loopbound min 0 max 10" )  
    while ( 1 ) {
      /* wcc note: this assignment expression was added to avoid assignment of
         multiple loop bound annotations to same loop (cf. Ticket #0002323). */
      flowfactdummy ++;
      _Pragma( "loopbound min 1 max 6" )
      do {
        pi += es;
      } while ( pi < pn && anagram_CompareFrequency( pi, a ) < 0 );
      _Pragma( "loopbound min 1 max 7" )
      do {
        pj -= es;
      } while ( pj > a && anagram_CompareFrequency( pj, a ) > 0 );
      if ( pj < pi )
        break;
      anagram_swapi( pi, pj, es );
    }
    anagram_swapi( a, pj, es );
    j = ( unsigned long )( pj - a ) / es;
    n = n - j - 1;
    if ( j >= n ) {
      anagram_qsorts( a, j, es );
      a += ( j + 1 ) * es;
    } else {
      anagram_qsorts( a + ( j + 1 )*es, n, es );
      n = j;
    }
  }
}

void anagram_qsort( void *va, unsigned long n, unsigned long es )
{
  _Pragma( "marker call_qsorts" )
  anagram_qsorts( ( char * )va, n, es );
  _Pragma( "flowrestriction 1*anagram_qsorts <= 17*call_qsorts" );
}


/* This must be redefined for each new benchmark */
#define ANAGRAM_HEAP_SIZE 21000

static char anagram_simulated_heap[ANAGRAM_HEAP_SIZE];
static unsigned int anagram_freeHeapPos;

void *anagram_malloc( unsigned int numberOfBytes )
{
  void *currentPos = ( void * )&anagram_simulated_heap[ anagram_freeHeapPos ];
  /* Get a 4-byte address for alignment purposes */
  //anagram_freeHeapPos += ( ( numberOfBytes + 4 ) & ( unsigned int )0xfffffffc );
  unsigned int rem = ( numberOfBytes & ( unsigned int )0x3 );
  unsigned int adjustment = rem ? 4 - rem : 0;
  anagram_freeHeapPos += numberOfBytes + adjustment;
  return currentPos;
}

void anagram_bzero( char *p, unsigned long len )
{
  unsigned long i;

  _Pragma( "loopbound min 8 max 800" )
  for ( i = 0; i < len; ++ i ){
    *p ++ = '\0';}
}

