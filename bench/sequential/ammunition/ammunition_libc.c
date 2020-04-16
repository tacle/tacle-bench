/*
  Include section
*/

#include "ammunition_string.h"
#include "ammunition_stdio.h"
#include "ammunition_stdlib.h"


/*
  Standard library functions
*/

char ammunition_isdigit( unsigned char c )
{
  if ( ( c >= '0' ) & ( c <= '9' ) )
    return 1;
  else
    return 0;
}

int ammunition_isspace( int c )
{
  return ( c == ' ' ) | ( c == '\t' ) | ( c == '\n' ) | ( c == '\r' );
}

void *ammunition_memcpy( void *dest, const void *src, size_t size )
{
  size_t i;
  _Pragma( "loopbound min 2 max 6" )
  for ( i = 0; i < size; i++ )
    ( ( unsigned char * )dest )[ i ] = ( ( unsigned char * )src )[ i ];

  return dest;
}


void *ammunition_memset( void *s, int c, size_t n )
{
  size_t i;
  _Pragma( "loopbound min 0 max 4" )
  for ( i = 0; i < n; i++ )
    ( ( unsigned char * )s )[ i ] = ( unsigned char )c;

  return s;
}


int ammunition_memcmp ( const void *mem1, const void *mem2, size_t size )
{
  const unsigned char *p1 = ( const unsigned char * ) mem1,
                       *p2 = ( const unsigned char * ) mem2;
  _Pragma( "loopbound min 0 max 4" )
  while ( size-- )
    if ( *p1 != *p2 )
      return ( *p1 - *p2 );
    else
      p1++, p2++;
  return 0;
}


/* The following function is an analog of standard C function
   `memmove'.  The function returns the first operand. */

void *ammunition_memmove ( void *s1, const void *s2, size_t n )
{
  int i;

  if ( ( ( char * ) s1 < ( char * ) s2 && ( char * ) s1 + n <= ( char * ) s2 )
       || ( ( char * ) s2 < ( char * ) s1
            && ( char * ) s2 + n <= ( char * ) s1 ) )
    return ( void * ) ammunition_memcpy ( s1, s2, n );
  if ( ( char * ) s1 < ( char * ) s2 && ( char * ) s1 + n > ( char * ) s2 ) {
    _Pragma( "loopbound min 0 max 4" )
    for ( i = 0; ( size_t ) i < n; i++ )
      ( ( char * ) s1 ) [ i ] = ( ( char * ) s2 ) [ i ];
  } else {
    _Pragma( "loopbound min 0 max 4" )
    for ( i = n - 1; i >= 0; i-- )
      ( ( char * ) s1 )[ i ] = ( ( char * ) s2 ) [ i ];
  }
  return s1;
}

int ammunition_strcmp ( const char *str1, const char *str2 )
{
  _Pragma( "loopbound min 1 max 11" )
  while ( *str1 && ( *str1 == *str2 ) )
    str1++, str2++;
  return *( const unsigned char * )str1 - *( const unsigned char * )str2;
}

int ammunition_atoi ( const char *str )
{
  int result = 0;
  int sign = ( str[ 0 ] == '-' ? -1 : 1 );

  int readingPos = 0;
  if ( str[ 0 ] == '-' || str[ 0 ] == '+' )
    readingPos++;
  _Pragma( "loopbound min 1 max 7" )
  do {
    result *= 10;
    result += str[ readingPos++ ] - 48;
  } while ( str[ readingPos ] != 0 );

  return sign * result;
}


int ammunition_sprintf_d( char *s, int number )
{
  /* How many decimal digits do we need? */
  char digits = 0;
  unsigned char writePos = 0;
  long long copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    digits++;
    copyOfNumber /= 10;
  } while ( copyOfNumber != 0 );

  writePos = digits;
  if ( number < 0 ) {
    writePos++;
    s[ 0 ] = '-';
  }
  s[ writePos ] = 0;

  copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    s[ --writePos ] = 48 + ( ( copyOfNumber >= 0 ?
                             copyOfNumber : -copyOfNumber ) % 10 );
    copyOfNumber /= 10;
  } while ( copyOfNumber != 0 );

  return digits + ( number < 0 ? 1 : 0 );
}


int ammunition_sprintf_u( char *s, unsigned int number )
{
  /* How many decimal digits do we need? */
  char digits = 0;
  unsigned char writePos = 0;
  unsigned long long copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    digits++;
    copyOfNumber /= 10;
  } while ( copyOfNumber != 0 );

  writePos = digits;
  s[ writePos ] = 0;

  copyOfNumber = number;
  _Pragma( "loopbound min 1 max 10" )
  do {
    s[ --writePos ] = 48 + ( copyOfNumber % 10 );
    copyOfNumber /= 10;
  } while ( copyOfNumber != 0 );

  return digits;
}
