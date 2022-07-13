/*

  This program is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: anagram

  Author: Raymond Chen

  Function: A program that computes anagrams.

  Source: See below.

  Original name: anagram

  Changes: See ChangeLog.txt

  License: See below.

*/

/*
   Anagram program by Raymond Chen,
   inspired by a similar program by Brian Scearce

   This program is Copyright 1991 by Raymond Chen.
               (rjc@math.princeton.edu)

   This program may be freely distributed provided all alterations
   to the original are clearly indicated as such.
*/

/* There are two tricks.  First is the Basic Idea:

   When the user types in a phrase, the phrase is first preprocessed to
   determine how many of each letter appears.  A bit field is then constructed
   dynamically, such that each field is large enough to hold the next power
   of two larger than the number of times the character appears.  For example,
   if the phrase is hello, world, the bit field would be

     00 00 00 000 000 00 00
      d e   h   l   o  r  w

   The phrase hello, world, itself would be encoded as

     01 01 01 011 010 01 01
      d e   h   l   o  r  w

   and the word hollow would be encoded as

     00 00 01 010 010 00 01
      d  e  h   l   o  r  w

   The top bit of each field is set in a special value called the sign.
   Here, the sign would be

     10 10 10 100 100 10 10
      d  e  h   l   o  r  w


   The reason for packing the values into a bit field is that the operation
   of subtracting out the letters of a word from the current phrase can be
   carried out in parallel.  for example, subtracting the word hello from
   the phrase hello, world, is merely

      d e   h   l   o  r  w
     01 01 01 011 010 01 01 (dehllloorw)
   - 00 00 01 010 010 00 01 (hlloow)
   ========================
     01 01 00 001 000 01 00 (delr)

   Since none of the sign bits is set, the word fits, and we can continue.
   Suppose the next word we tried was hood.

      d e   h   l   o  r  w
     01 01 00 001 000 01 00 (delr)
   - 01 00 01 000 010 00 00 (hood)
   ========================
     00 00 11 000 110 01 00
           ^      ^
   A sign bit is set.  (Two, actually.)  This means that hood does not
   fit in delr, so we skip it and try another word.  (Observe that
   when a sign bit becomes set, it screws up the values for the letters to
   the left of that bit, but that's not important.)

   The inner loop of an anagram program is testing to see if a
   word fits in the collection of untried letters.  Traditional methods
   keep an array of 26 integers, which are then compared in turn.  This
   means that there are 26 comparisons per word.

   This method reduces the number of comparisons to MAX_QUAD, typically 2.
   Instead of looping through an array, we merely perform the indicated
   subtraction and test if any of the sign bits is set.
*/

/* The nuts and bolts:

   The dictionary is loaded and preprocessed.  The preprocessed dictionary
   is a concatenation of copies of the structure:

   struct dictword {
       char bStructureSize;             -- size of this structure
       char cLetters;                   -- number of letters in the word
       char achWord[];                  -- the word itself (0-terminated)
   }

   Since this is a variable-sized structure, we keep its size in the structure
   itself for rapid stepping through the table.

   When a phrase is typed in, it is first preprocessed as described in the
   Basic Idea.  We then go through the dictionary, testing each word.  If
   the word fits in our phrase, we build the bit field for its frequency
   table and add it to the list of candidates.
*/

/*
   The Second Trick:

   Before diving into our anagram search, we then tabulate how many times
   each letter appears in our list of candidates, and sort the table, with
   the rarest letter first.

   We then do our anagram search.

   Like most anagram programs, this program does a depth-first search.
   Although most anagram programs do some sort of heuristics to decide what
   order to place words in the list_of_candidates, the search itself proceeds
   according to a greedy algorithm.  That is, once you find a word that fits,
   subtract it and recurse.

   This anagram program exercises some restraint and does not march down
   every branch that shows itself.  Instead, it only goes down branches
   that use the rarest unused letter.  This helps to find dead ends faster.

   FindAnagram(unused_letters, list_of_candidates) {
    l = the rarest letter as yet unused
    For word in list_of_candidates {
       if word does not fit in unused_letters, go on to the next word.
       if word does not contain l, defer.
        FindAnagram(unused_letters - word, list_of_candidates[word,...])
    }
   }


   The heuristic of the Second Trick can probably be improved.  I invite
   anyone willing to improve it to do so.
*/

/* Before compiling, make sure Quad and MASK_BITS are set properly.  For best
   results, make Quad the largest integer size supported on your machine.
   So if your machine has long longs, make Quad an unsigned long long.
   (I called it Quad because on most machines, the largest integer size
   supported is a four-byte unsigned long.)

   If you need to be able to anagram larger phrases, increase MAX_QUADS.
   If you increase it beyond 4, you'll have to add a few more loop unrolling
   steps to FindAnagram.
*/
#include "anagram_ctype.h"
#include "anagram_stdlib.h"
#include "anagram_strings.h"

#include "anagram_compare.h"

/*
  Defines
*/

#define anagram_DICTWORDS 2279
#define anagram_MASK_BITS 32   /* number of bits in a Quad */
#define anagram_MAX_QUADS 2    /* controls largest phrase */
#define anagram_MAXCAND  100   /* candidates */
#define anagram_MAXSOL   51    /* words in the solution */
#define anagram_ALPHABET 26    /* letters in the alphabet */

#define anagram_OneStep( i ) \
        if ( ( aqNext[ i ] = pqMask[ i ] - pw->aqMask[ i ] ) & anagram_aqMainSign[ i ] ) { \
          ppwStart ++; \
          continue; \
        }


/*
  Type definitions
*/

typedef unsigned int anagram_Quad; /* for building our bit mask */

/* A Word remembers the information about a candidate word. */
typedef struct {
  char *pchWord;                            /* the word itself */
  anagram_Quad aqMask[ anagram_MAX_QUADS ]; /* the word's mask */
  unsigned cchLength;                       /* letters in the word */
  char padding[ 4 ];
} anagram_Word;
typedef anagram_Word *anagram_PWord;
typedef anagram_Word **anagram_PPWord;

/* A Letter remembers information about each letter in the phrase to
   be anagrammed. */
typedef struct {
  unsigned uFrequency; /* how many times it appears */
  unsigned uShift;     /* how to mask */
  unsigned uBits;      /* the bit mask itself */
  unsigned iq;         /* which Quad to inspect? */
} anagram_Letter;
typedef anagram_Letter *anagram_PLetter;


/*
  Forward declaration of functions
*/

void anagram_init( void );
void anagram_main( void );
int anagram_return( void );
int anagram_ch2i( int ch );
void anagram_AddWords( void );
void anagram_BuildMask( char const *pchPhrase );
void anagram_BuildWord( char *pchWord );
void anagram_DumpWords( void );
void anagram_FindAnagram( anagram_Quad *pqMask,
                          anagram_PPWord ppwStart,
                          int iLetter );
anagram_PWord anagram_NewWord( void );
anagram_PWord anagram_NextWord( void );
void anagram_ReadDict( void );
void anagram_Reset( void );
void anagram_SortCandidates( void );


/*
  Declaration of global variables
*/

extern char const *anagram_achPhrase[ 3 ];
extern char const *anagram_dictionary[ anagram_DICTWORDS ];

/* candidates we've found so far */
static anagram_PWord anagram_apwCand[ anagram_MAXCAND ];
/* how many of them? */
static unsigned anagram_cpwCand;

/* statistics on the current phrase */
static anagram_Letter anagram_alPhrase[ anagram_ALPHABET ];

/* number of letters in phrase */
static int anagram_cchPhraseLength;

/* the bit field for the full phrase */
static anagram_Quad anagram_aqMainMask[ anagram_MAX_QUADS ];
/* where the sign bits are */
static anagram_Quad anagram_aqMainSign[ anagram_MAX_QUADS ];

static const int anagram_cchMinLength = 3;

/* auGlobalFrequency counts the number of times each letter appears,
   summed over all candidate words. This is used to decide which letter
   to attack first. */
static unsigned anagram_auGlobalFrequency[ anagram_ALPHABET ];
static int anagram_achByFrequency[ anagram_ALPHABET ]; /* for sorting */

/* the dictionary is read here */
static char *anagram_pchDictionary;

/* the answers */
static anagram_PWord anagram_apwSol[ anagram_MAXSOL ];
static int anagram_cpwLast;

/* buffer to write an answer */
static char anagram_buffer[ 30 ];

/*
  Initialization- and return-value-related functions
*/

/* ReadDict -- read the dictionary file into memory and preprocess it

   A word of length cch in the dictionary is encoded as follows:

      byte 0    = cch + 3
      byte 1    = number of letters in the word
      byte 2... = the word itself, null-terminated

   Observe that cch+3 is the length of the total encoding.  These
   byte streams are concatenated, and terminated with a 0.
*/
void anagram_ReadDict( void )
{
  char *pch;
  char *pchBase;
  unsigned len;
  unsigned cWords = 0;
  unsigned cLetters;
  int i;
  volatile char bitmask = 0;

  len = 0;
  _Pragma( "loopbound min 2279 max 2279" )
  for ( i = 0; i < anagram_DICTWORDS; i ++ ) {
    unsigned strlen = 0;
    _Pragma( "loopbound min 1 max 5" )
    while ( anagram_dictionary[ i ][ strlen ] != 0 )
      strlen ++;
    len += strlen + 3;
  }

  len ++;
  pchBase = anagram_pchDictionary = ( char * )anagram_malloc( len );

  _Pragma( "loopbound min 2279 max 2279" )
  for ( i = 0; i < anagram_DICTWORDS; i ++ ) {
    int index = 0;
    pch = pchBase + 2; /* reserve for length */
    cLetters = 0;

    _Pragma( "loopbound min 1 max 5" )
    while ( anagram_dictionary[ i ][ index ] != '\0' ) {
      if ( anagram_isalpha( anagram_dictionary[ i ][ index ] ) )
        cLetters ++;
      *pch ++ = anagram_dictionary[ i ][ index ];
      index ++;
      *( pch - 1 ) ^= bitmask;
    }
    *pch ++ = '\0';
    *pchBase = ( char )( pch - pchBase );
    pchBase[ 1 ] = ( char )cLetters;
    pchBase = pch;
    cWords ++;
  }

  *pchBase ++ = 0;
}


void anagram_init( void )
{
  anagram_ReadDict();
}


int anagram_return( void )
{
  int i;
  char const *answer = "duke yip arm";

  _Pragma( "loopbound min 12 max 12" )
  for ( i = 0; i < 12; i++ )
    if ( answer[ i ] != anagram_buffer[ i ] )
      return 1;

  return 0;
}


/*
  Core benchmark functions
*/

/* convert letter to index */
int anagram_ch2i( int ch )
{
  return ch - 'a';
}


int anagram_CompareFrequency( char *pch1, char *pch2 )
{
  return anagram_auGlobalFrequency[ ( *(int*) pch1 ) ] <
         anagram_auGlobalFrequency[ ( *(int*) pch2 ) ]
         ? -1 :
         anagram_auGlobalFrequency[ ( *(int*) pch1 ) ] ==
         anagram_auGlobalFrequency[ ( *(int*) pch2 ) ]
         ? 0 : 1;
}


void anagram_Reset( void )
{
  anagram_bzero( ( char * )anagram_alPhrase,
                 sizeof( anagram_Letter ) * anagram_ALPHABET );
  anagram_bzero( ( char * )anagram_aqMainMask,
                 sizeof( anagram_Quad ) * anagram_MAX_QUADS );
  anagram_bzero( ( char * )anagram_aqMainSign,
                 sizeof( anagram_Quad ) * anagram_MAX_QUADS );
  anagram_bzero( ( char * )anagram_auGlobalFrequency,
                 sizeof( unsigned ) * anagram_ALPHABET );
  anagram_bzero( ( char * )anagram_achByFrequency,
                 sizeof( int ) * anagram_ALPHABET );
  anagram_bzero( ( char * )anagram_apwCand,
                 sizeof( anagram_PWord ) * anagram_MAXCAND );
  anagram_cchPhraseLength = 0;
  anagram_cpwCand = 0;
}


void anagram_BuildMask( char const *pchPhrase )
{
  int i;
  int ch;
  unsigned iq;          /* which Quad? */
  unsigned int cbtUsed; /* bits used in the current Quad */
  unsigned int cbtNeed; /* bits needed for current letter */
  anagram_Quad qNeed;   /* used to build the mask */

  /* Tabulate letter frequencies in the phrase */
  anagram_cchPhraseLength = 0;
  _Pragma( "loopbound min 11 max 11" )
  while ( ( ch = *pchPhrase ++ ) != '\0' ) {
    if ( anagram_isalpha( ch ) ) {
      ch = anagram_tolower( ch );
      anagram_alPhrase[  anagram_ch2i( ch )  ].uFrequency ++;
      anagram_cchPhraseLength ++;
    }
  }

  /* Build  masks */
  iq = 0;               /* which quad being used */
  cbtUsed = 0;          /* bits used so far */

  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < anagram_ALPHABET; i ++ ) {
    if ( anagram_alPhrase[ i ].uFrequency == 0 ) {
      anagram_auGlobalFrequency[ i ] = ~0u;  /* to make it sort last */
    } else {
      anagram_auGlobalFrequency[ i ] = 0u;
      _Pragma( "loopbound min 1 max 2" )
      for ( cbtNeed = 1, qNeed = 1;
            anagram_alPhrase[ i ].uFrequency >= qNeed;
            cbtNeed ++, qNeed <<= 1 )
        ;
      if ( cbtUsed + cbtNeed > anagram_MASK_BITS )
        cbtUsed = 0;
      anagram_alPhrase[ i ].uBits = qNeed - 1;
      if ( cbtUsed )
        qNeed <<= cbtUsed;
      anagram_aqMainSign[ iq ] |= qNeed;
      anagram_aqMainMask[ iq ] |=
        ( anagram_Quad )anagram_alPhrase[ i ].uFrequency << cbtUsed;
      anagram_alPhrase[ i ].uShift = cbtUsed;
      anagram_alPhrase[ i ].iq = iq;
      cbtUsed += cbtNeed;
    }
  }
}


anagram_PWord anagram_NewWord( void )
{
  anagram_PWord pw;

  pw = ( anagram_Word * )anagram_malloc( sizeof( anagram_Word ) );
  return pw;
}


/* NextWord -- get another candidate entry, creating if necessary */
anagram_PWord anagram_NextWord( void )
{
  anagram_PWord pw;
  pw = anagram_apwCand[ anagram_cpwCand ++ ];
  if ( pw != 0 )
    return pw;
  anagram_apwCand[ anagram_cpwCand - 1 ] = anagram_NewWord();
  return anagram_apwCand[ anagram_cpwCand - 1 ];
}


/* BuildWord -- build a Word structure from an ASCII word
   If the word does not fit, then do nothing. */
void anagram_BuildWord( char *pchWord )
{
  unsigned char cchFrequency[ anagram_ALPHABET ];
  int i;
  char *pch = pchWord;
  anagram_PWord pw;
  unsigned int cchLength = 0;

  anagram_bzero( ( char * )cchFrequency,
                 sizeof( unsigned char ) * anagram_ALPHABET );

  /* Build frequency table */
  _Pragma( "loopbound min 3 max 5" )
  while ( ( i = *pch ++ ) != '\0' ) {
    if ( !anagram_isalpha( i ) )
      continue;
    i = anagram_ch2i( anagram_tolower( i ) );
    if ( ++ cchFrequency[ i ] > anagram_alPhrase[ i ].uFrequency )
      return ;
    ++ cchLength;
  }

  /* Update global count */
  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < anagram_ALPHABET; i ++ )
    anagram_auGlobalFrequency[ i ] += cchFrequency[ i ];

  /* Create a Word structure and fill it in, including building the
     bitfield of frequencies. */
  pw = anagram_NextWord();
  anagram_bzero( ( char * )( pw->aqMask ),
                 sizeof( anagram_Quad ) * anagram_MAX_QUADS );

  pw->pchWord = pchWord;
  pw->cchLength = cchLength;
  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < anagram_ALPHABET; i ++ ) {
    pw->aqMask[ anagram_alPhrase[i].iq ] |=
      ( anagram_Quad )cchFrequency[ i ] << anagram_alPhrase[ i ].uShift;
  }
}


/* AddWords -- build the list of candidates */
void anagram_AddWords( void )
{
  char *pch = anagram_pchDictionary; /* walk through the dictionary */

  anagram_cpwCand = 0;

  _Pragma( "loopbound min 1967 max 1967" )
  while ( *pch ) {
    if ( ( pch[ 1 ] >= anagram_cchMinLength &&
           pch[ 1 ] + anagram_cchMinLength <= anagram_cchPhraseLength )
         || pch[ 1 ] == anagram_cchPhraseLength )
      anagram_BuildWord( pch + 2 );
    pch += *pch;
  }
}


void anagram_DumpWords( void )
{
  int i, j;
  int offset = 0;
  _Pragma( "loopbound min 3 max 3" )        
  for ( i = 0; i < anagram_cpwLast; i ++ ) {
    _Pragma( "loopbound min 3 max 5" )
    for ( j = 0; anagram_apwSol[ i ]->pchWord[ j ] != '\0'; j ++ )
      anagram_buffer[ offset + j ] = anagram_apwSol[ i ]->pchWord[ j ];
    offset += j;

    anagram_buffer[ offset ++ ] = ' ';
  } 
  anagram_buffer[ offset ++ ] = '\0';
}


void anagram_FindAnagram( anagram_Quad *pqMask, anagram_PPWord ppwStart,
                          int iLetter )
{
  anagram_Quad aqNext[ anagram_MAX_QUADS ];
  register anagram_PWord pw;
  anagram_Quad qMask;
  unsigned iq;
  anagram_PPWord ppwEnd = &anagram_apwCand[ 0 ];
  ppwEnd += anagram_cpwCand;

  _Pragma( "loopbound min 0 max 6" )
  while ( 1 ) {
    iq = anagram_alPhrase[ anagram_achByFrequency[iLetter] ].iq;
    qMask = anagram_alPhrase[ anagram_achByFrequency[iLetter] ].uBits <<
            anagram_alPhrase[ anagram_achByFrequency[iLetter] ].uShift;
    if ( pqMask[ iq ] & qMask )
      break;
    iLetter ++;
  }

  _Pragma( "loopbound min 0 max 11" )
  while ( ppwStart < ppwEnd ) {
    pw = *ppwStart;

    #if anagram_MAX_QUADS > 0
    anagram_OneStep( 0 );
    #endif

    #if anagram_MAX_QUADS > 1
    anagram_OneStep( 1 );
    #endif

    #if anagram_MAX_QUADS > 2
    anagram_OneStep( 2 );
    #endif

    #if anagram_MAX_QUADS > 3
    anagram_OneStep( 3 );
    #endif

    #if anagram_MAX_QUADS > 4
    @@"Add more unrolling steps here, please."@@
    #endif

    /* If the pivot letter isn't present, defer this word until later */
    if ( ( pw->aqMask[ iq ] & qMask ) == 0 ) {
      *ppwStart = *( -- ppwEnd );
      *ppwEnd = pw;
      continue;
    }

    /* If we get here, this means the word fits. */
    anagram_apwSol[ anagram_cpwLast ++ ] = pw;
    if ( anagram_cchPhraseLength -= pw->cchLength ) { /* recurse */
      /* The recursive call scrambles the tail, so we have to be
         pessimistic. */
      ppwEnd = &anagram_apwCand[ 0 ];
      ppwEnd += anagram_cpwCand;
      anagram_FindAnagram( &aqNext[ 0 ], ppwStart, iLetter );
    } else   /* found one */
      anagram_DumpWords();
    anagram_cchPhraseLength += pw->cchLength;
    -- anagram_cpwLast;
    ppwStart ++;
    continue;
  }
}


void anagram_SortCandidates( void )
{
  int i;

  /* Sort the letters by frequency */
  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < anagram_ALPHABET; i ++ )
    anagram_achByFrequency[ i ] = i;
  anagram_qsort( anagram_achByFrequency, anagram_ALPHABET, sizeof( int ) );
}


void _Pragma( "entrypoint" ) anagram_main( void )
{
  int i;

  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < 3; i ++ ) {
    anagram_Reset();
    anagram_BuildMask( anagram_achPhrase[ i ] );
    anagram_AddWords();
    if ( anagram_cpwCand == 0 || anagram_cchPhraseLength == 0 )
      continue;

    anagram_cpwLast = 0;
    anagram_SortCandidates();
    _Pragma( "marker call_find" )
    anagram_FindAnagram( anagram_aqMainMask, anagram_apwCand, 0 );
    _Pragma( "flowrestriction 1*anagram_FindAnagram <= 51*call_find" );
  }
}


/*
  Main function
*/

int main( void )
{
  anagram_init();
  anagram_main();

  return anagram_return();
}
