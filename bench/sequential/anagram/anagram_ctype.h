#ifndef ANAGRAM_CTYPE_H
#define ANAGRAM_CTYPE_H

static int anagram_islower( int c )
{
  return 'a' <= c && c <= 'z';
}

static int anagram_isupper( int c )
{
  return 'A' <= c && c <= 'Z';
}

static int anagram_isalpha( int c )
{
  return anagram_isupper( c ) || anagram_islower( c );
}

static int anagram_tolower( int c )
{
  return anagram_isupper( c ) ? c + ( 'a' - 'A' ) : c;
}

#endif
