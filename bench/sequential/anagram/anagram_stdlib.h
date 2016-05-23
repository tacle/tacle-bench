/*

  This header is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: anagram_stdlib.h

  Author: Raymond Chen

  Function: This header contains some C standard library functions used by anagram.

  Source: unknown

  Original name: anagram

  Changes: See ChangeLog.txt

  License: See anagram.c

*/

#ifndef ANAGRAM_STDLIB_H
#define ANAGRAM_STDLIB_H

void *anagram_malloc( unsigned int numberOfBytes );

void anagram_qsort( void *va, unsigned long n, unsigned long es );

#endif
