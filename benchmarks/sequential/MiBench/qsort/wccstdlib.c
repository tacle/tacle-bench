#include "qsort.h"

static int wccstrcmp ( const char * str1, const char * str2 )
{
  int pos = 0;
  _Pragma( "loopbound min 1 max 11" )
  while ( str1[pos] != 0 && str2[pos] != 0 ) {
    if ( str1[pos] != str2[pos] ) {
      return str1[pos] - str2[pos];
    }
    pos++;
  }
  
  if ( str1[pos] != 0 ) {
    return 1;
  } else {
    return -1;
  }
}

// This function is included here because the WCC does not
// support function pointers
int compare_strings(const char *elem1, const char *elem2)
{
  int result;
  
  result = wccstrcmp(elem1, elem2);

  return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}
// This function is included here because the WCC does not
// support function pointers
int compare_vectors(const char *elem1, const char *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1, distance2;
  distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
  distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}

static void swapi(char *ii, char *ij, unsigned long es)
{
  char *i, *j, c;

  i = (char*)ii;
  j = (char*)ij;
  _Pragma( "loopbound min 20 max 20" )
  do {
    c = *i;
    *i++ = *j;
    *j++ = c;
    es -= sizeof(char);
  } while (es != 0);
}

static char* pivot_strings(char *a, unsigned long n, unsigned long es)
{
  long j;
  char *pi, *pj, *pk;

  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */
  if (compare_strings(pi, pj) < 0) {
    if (compare_strings(pi, pk) < 0) {
      if (compare_strings(pj, pk) < 0)
        return pj;
      return pk;
    }
    return pi;
  }
  if (compare_strings(pj, pk) < 0) {
    if (compare_strings(pi, pk) < 0)
      return pi;
    return pk;
  }
  return pj;
}
static char* pivot_vectors(char *a, unsigned long n, unsigned long es)
{
  long j;
  char *pi, *pj, *pk;

  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */
  if (compare_vectors(pi, pj) < 0) {
    if (compare_vectors(pi, pk) < 0) {
      if (compare_vectors(pj, pk) < 0)
        return pj;
      return pk;
    }
    return pi;
  }
  if (compare_vectors(pj, pk) < 0) {
    if (compare_vectors(pi, pk) < 0)
      return pi;
    return pk;
  }
  return pj;
}

void qsorts_strings(char *a, unsigned long n, unsigned long es)
{
  long j;
  char *pi, *pj, *pn;

  _Pragma( "loopbound min 0 max 31" )
  while (n > 1) {
    if (n > 10) {
      pi = pivot_strings(a, n, es);
    } else {
      pi = a + (n >> 1) * es;
    }

    swapi(a, pi, es);
    pi = a;
    pn = a + n * es;
    pj = pn;
    _Pragma( "loopbound min 1 max 170" )
    while (1) {
      _Pragma( "loopbound min 1 max 26" )
      do {
        pi += es;
      } while (pi < pn && compare_strings(pi, a) < 0);
      _Pragma( "loopbound min 1 max 49" )
      do {
        pj -= es;
      } while (pj > a && compare_strings(pj, a) > 0);
      if (pj < pi)
        break;
      swapi(pi, pj, es);
    }
    swapi(a, pj, es);
    j = (pj - a) / es;

    n = n - j - 1;
    if (j >= n) {
      qsorts_strings(a, j, es);
      a += (j + 1) * es;
    } else {
      qsorts_strings(a + (j + 1)*es, n, es);
      n = j;
    }
  }
}
void qsorts_vectors(char *a, unsigned long n, unsigned long es)
{
  long j;
  char *pi, *pj, *pn;

  _Pragma( "loopbound min 0 max 15" )
  while (n > 1) {
    if (n > 10) {
      pi = pivot_vectors(a, n, es);
    } else {
      pi = a + (n >> 1) * es;
    }

    swapi(a, pi, es);
    pi = a;
    pn = a + n * es;
    pj = pn;
    _Pragma( "loopbound min 1 max 250" )
    while (1) {
      _Pragma( "loopbound min 1 max 51" )
      do {
        pi += es;
      } while (pi < pn && compare_vectors(pi, a) < 0);
      _Pragma( "loopbound min 1 max 27" )
      do {
        pj -= es;
      } while (pj > a && compare_vectors(pj, a) > 0);
      if (pj < pi)
        break;
      swapi(pi, pj, es);
    }
    swapi(a, pj, es);
    j = (pj - a) / es;

    n = n - j - 1;
    if (j >= n) {
      qsorts_vectors(a, j, es);
      a += (j + 1) * es;
    } else {
      qsorts_vectors(a + (j + 1)*es, n, es);
      n = j;
    }
  }
}
