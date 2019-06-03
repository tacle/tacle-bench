/*

  This header is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: anagram_ctype.h

  Author: Raymond Chen

  Function: This header contains some C library functions used by anagram.

  Source: unknown

  Original name: anagram

  Changes: See ChangeLog.txt

  License: See anagram.c

*/

#ifndef ANAGRAM_CTYPE_H
#define ANAGRAM_CTYPE_H

int anagram_islower( int c )
{
  return 'a' <= c && c <= 'z';
}

int anagram_isupper( int c )
{
  return 'A' <= c && c <= 'Z';
}

int anagram_isalpha( int c )
{
  return anagram_isupper( c ) || anagram_islower( c );
}

int anagram_tolower( int c )
{
  return anagram_isupper( c ) ? c + ( 'a' - 'A' ) : c;
}

#endif
