#include "quicksort.h"


int quicksort_strcmp( const char *str1, const char *str2 )
{
  int pos = 0;


  _Pragma( "loopbound min 1 max 11" )
  while ( ( str1[ pos ] != 0 ) && ( str2[ pos ] != 0 ) ) {
    if ( str1[ pos ] != str2[ pos ] )
      return( str1[ pos ] - str2[ pos ] );
    pos++;
  }

  if ( str1[ pos ] != 0 )
    return( 1 );
  else
    return( -1 );
}


int quicksort_compare_strings( const char *elem1, const char *elem2 )
{
  int result;


  result = quicksort_strcmp( elem1, elem2 );

  return( ( result < 0 ) ? 1 : ( ( result == 0 ) ? 0 : -1 ) );
}


int quicksort_compare_vectors( const char *elem1, const char *elem2 )
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1, distance2;


  distance1 = ( *( ( struct quicksort_3DVertexStruct * )elem1 ) ).distance;
  distance2 = ( *( ( struct quicksort_3DVertexStruct * )elem2 ) ).distance;

  return(
    ( distance1 > distance2 ) ? 1 : ( ( distance1 == distance2 ) ? 0 : -1 ) );
}


void quicksort_swapi( char *ii, char *ij, unsigned long es )
{
  char *i, *j, c;


  i = (char *) ii;
  j = (char *) ij;

  _Pragma( "loopbound min 20 max 20" )
  do {
    c = *i;
    *i++ = *j;
    *j++ = c;
    es -= sizeof( char );
  } while ( es != 0 );
}


char *quicksort_pivot_strings( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pk;


  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */

  if ( quicksort_compare_strings( pi, pj ) < 0 ) {
    if ( quicksort_compare_strings( pi, pk ) < 0 ) {
      if ( quicksort_compare_strings( pj, pk ) < 0 )
        return( pj );
      return( pk );
    }
    return( pi );
  }

  if ( quicksort_compare_strings( pj, pk ) < 0 ) {
    if ( quicksort_compare_strings( pi, pk ) < 0 )
      return( pi );
    return( pk );
  }

  return( pj );
}


char *quicksort_pivot_vectors( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pk;


  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */

  if ( quicksort_compare_vectors( pi, pj ) < 0 ) {
    if ( quicksort_compare_vectors( pi, pk ) < 0 ) {
      if ( quicksort_compare_vectors( pj, pk ) < 0 )
        return( pj );
      return( pk );
    }
    return( pi );
  }

  if ( quicksort_compare_vectors( pj, pk ) < 0 ) {
    if ( quicksort_compare_vectors( pi, pk ) < 0 )
      return( pi );
    return( pk );
  }

  return( pj );
}
