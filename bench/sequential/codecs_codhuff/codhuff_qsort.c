/* To avoid functional pointers, this is a specialised qsort() that calls
   weight_tree_comp() for comparison */

int codhuff_weight_tree_comp( char *c1, char *c2 );


static void codhuff_swapi( char *ii, char *ij, unsigned long es )
{
  char *i, *j, c;

  i = ( char * )ii;
  j = ( char * )ij;
  _Pragma( "loopbound min 4 max 4" )
  do {
    c = *i;
    *i++ = *j;
    *j++ = c;
    es -= sizeof( char );
  } while ( es != 0 );
}


static char *codhuff_pivot( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pk;

  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */
  if ( codhuff_weight_tree_comp( pi, pj ) < 0 ) {
    if ( codhuff_weight_tree_comp( pi, pk ) < 0 ) {
      if ( codhuff_weight_tree_comp( pj, pk ) < 0 )
        return pj;
      return pk;
    }
    return pi;
  }
  if ( codhuff_weight_tree_comp( pj, pk ) < 0 ) {
    if ( codhuff_weight_tree_comp( pi, pk ) < 0 )
      return pi;
    return pk;
  }
  return pj;
}


void codhuff_qsort( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pn;
  unsigned int flowfactdummy = 0;

  _Pragma( "loopbound min 0 max 8" )
  while ( n > 1 ) {
    if ( n > 10 ) {
      pi = codhuff_pivot( a, n, es );
    } else {
      pi = a + ( n >> 1 ) * es;
    }

    codhuff_swapi( a, pi, es );
    pi = a;
    pn = a + n * es;
    pj = pn;
    _Pragma( "loopbound min 1 max 110" )
    while ( 1 ) {
      /* wcc note: this assignment expression was added to avoid assignment of
       * multiple loop bound annotations to same loop (cf. Ticket #0002323). */
      flowfactdummy++;
      _Pragma( "loopbound min 1 max 19" )
      do {
        pi += es;
      } while ( pi < pn && codhuff_weight_tree_comp( pi, a ) < 0 );
      _Pragma( "loopbound min 1 max 25" )
      do {
        pj -= es;
      } while ( pj > a && codhuff_weight_tree_comp( pj, a ) > 0 );
      if ( pj < pi )
        break;
      codhuff_swapi( pi, pj, es );
    }
    codhuff_swapi( a, pj, es );
    j = ( pj - a ) / es;

    n = n - j - 1;
    if ( j >= n ) {
      codhuff_qsort( a, j, es );
      a += ( j + 1 ) * es;
    } else {
      codhuff_qsort( a + ( j + 1 )*es, n, es );
      n = j;
    }
  }
}
