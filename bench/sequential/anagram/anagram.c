/*

  This program is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: anagram

  Author: Raymond Chen

  Function: A program that computes anagrams.

  Source: unknown

  Original name: anagram

  Changes: See ChangeLog.txt

  License: unknown

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

/* convert letter to index */
static int ch2i( int ch )
{
  return ch - 'a';
}

#define DICTWORDS 2279
extern char *anagram_achPhrase[3];
extern char *anagram_dictionary[DICTWORDS];

typedef unsigned int Quad;              /* for building our bit mask */
#define MASK_BITS 32                    /* number of bits in a Quad */
#define MAX_QUADS 2                     /* controls largest phrase */
#define MAXCAND  100                    /* candidates */
#define MAXSOL   51                     /* words in the solution */
#define ALPHABET 26                     /* letters in the alphabet */

/* A Word remembers the information about a candidate word. */
typedef struct {
  char *pchWord;                        /* the word itself */
  Quad aqMask[MAX_QUADS];               /* the word's mask */
  unsigned cchLength;                   /* letters in the word */
}
Word;
typedef Word *PWord;
typedef Word **PPWord;

static PWord
apwCand[MAXCAND];                 /* candidates we've found so far */
static unsigned cpwCand;                       /* how many of them? */

/* A Letter remembers information about each letter in the phrase to be
   anagrammed. */
typedef struct {
  unsigned uFrequency;                /* how many times it appears */
  unsigned uShift;                    /* how to mask */
  unsigned uBits;                     /* the bit mask itself */
  unsigned iq;                        /* which Quad to inspect? */
}
Letter;
typedef Letter *PLetter;

static Letter alPhrase[ALPHABET]; /* statistics on the current phrase */

static int cchPhraseLength;                    /* number of letters in phrase */

static Quad aqMainMask[MAX_QUADS]; /* the bit field for the full phrase */
static Quad aqMainSign[MAX_QUADS]; /* where the sign bits are */

static const int cchMinLength = 3;

/* auGlobalFrequency counts the number of times each letter appears, summed
   over all candidate words.  This is used to decide which letter to attack
   first.
*/
static unsigned auGlobalFrequency[ALPHABET];
static int achByFrequency[ALPHABET];          /* for sorting */

static char *pchDictionary;                /* the dictionary is read here */


int anagram_CompareFrequency( char *pch1, char *pch2 )
{
  return auGlobalFrequency[ch2i( *pch1 )] < auGlobalFrequency[ch2i( *pch2 )]
         ? -1 :
         auGlobalFrequency[ch2i( *pch1 )] == auGlobalFrequency[ch2i( *pch2 )]
         ? 0 : 1;
}


void Reset( void );
void Reset( void )
{
  anagram_bzero( ( char * )alPhrase, sizeof( Letter )*ALPHABET );
  anagram_bzero( ( char * )aqMainMask, sizeof( Quad )*MAX_QUADS );
  anagram_bzero( ( char * )aqMainSign, sizeof( Quad )*MAX_QUADS );
  anagram_bzero( ( char * )auGlobalFrequency, sizeof( unsigned )*ALPHABET );
  anagram_bzero( ( char * )achByFrequency, sizeof( int )*ALPHABET );
  anagram_bzero( ( char * )apwCand, sizeof( PWord )*MAXCAND );
  cchPhraseLength = 0;
  cpwCand = 0;
}

/* ReadDict -- read the dictionary file into memory and preprocess it

   A word of length cch in the dictionary is encoded as follows:

      byte 0    = cch + 3
      byte 1    = number of letters in the word
      byte 2... = the word itself, null-terminated

   Observe that cch+3 is the length of the total encoding.  These
   byte streams are concatenated, and terminated with a 0.
*/


void ReadDict( void );
void ReadDict( void )
{
  char *pch;
  char *pchBase;
  unsigned len;
  unsigned cWords = 0;
  unsigned cLetters;
  int i;

  len = 0;
  _Pragma( "loopbound min 2279 max 2279" )
  for ( i = 0; i < DICTWORDS; i++ ) {
    unsigned strlen = 0;
    _Pragma( "loopbound min 1 max 5" )
    while ( anagram_dictionary[i][strlen] != 0 )
      strlen++;
    len += strlen + 2;
  }

  pchBase = pchDictionary = ( char * )anagram_malloc( len );

  _Pragma( "loopbound min 2279 max 2279" )
  for ( i = 0; i < DICTWORDS; i++ ) {
    int index = 0;
    pch = pchBase + 2;                /* reserve for length */
    cLetters = 0;

    _Pragma( "loopbound min 1 max 5" )
    while ( anagram_dictionary[i][index] != '\0' ) {
      if ( anagram_isalpha( anagram_dictionary[i][index] ) )
        cLetters++;
      *pch++ = anagram_dictionary[i][index];
      index++;
    }
    *pch++ = '\0';
    *pchBase = ( char )( pch - pchBase );
    pchBase[1] = ( char )cLetters;
    pchBase = pch;
    cWords++;
  }

  *pchBase++ = 0;
}

void BuildMask( char *pchPhrase );
void BuildMask( char *pchPhrase )
{
  int i;
  int ch;
  unsigned iq;                        /* which Quad? */
  unsigned int cbtUsed;                        /* bits used in the current Quad */
  unsigned int
  cbtNeed;                        /* bits needed for current letter */
  Quad qNeed;                         /* used to build the mask */

  /* Tabulate letter frequencies in the phrase */
  cchPhraseLength = 0;
  _Pragma( "loopbound min 11 max 12" )
  while ( ( ch = *pchPhrase++ ) != '\0' ) {
    if ( anagram_isalpha( ch ) ) {
      ch = anagram_tolower( ch );
      alPhrase[ ch2i( ch ) ].uFrequency++;
      cchPhraseLength++;
    }
  }

  /* Build  masks */
  iq = 0;                             /* which quad being used */
  cbtUsed = 0;                        /* bits used so far */

  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < ALPHABET; i++ ) {
    if ( alPhrase[i].uFrequency == 0 ) {
      auGlobalFrequency[i] = ~0u;  /* to make it sort last */
    } else {
      auGlobalFrequency[i] = 0u;
      _Pragma( "loopbound min 1 max 2" )
      for ( cbtNeed = 1, qNeed = 1;
            alPhrase[i].uFrequency >= qNeed;
            cbtNeed++, qNeed <<= 1 )
        ;
      if ( cbtUsed + cbtNeed > MASK_BITS )
        cbtUsed = 0;
      alPhrase[i].uBits = qNeed - 1;
      if ( cbtUsed )
        qNeed <<= cbtUsed;
      aqMainSign[iq] |= qNeed;
      aqMainMask[iq] |= ( Quad )alPhrase[i].uFrequency << cbtUsed;
      alPhrase[i].uShift = cbtUsed;
      alPhrase[i].iq = iq;
      cbtUsed += cbtNeed;
    }
  }
}

PWord NewWord( void );
PWord NewWord( void )
{
  PWord pw;

  pw = ( Word * )anagram_malloc( sizeof( Word ) );
  return pw;
}

/* NextWord -- get another candidate entry, creating if necessary */
PWord NextWord( void );
PWord NextWord( void )
{
  PWord pw;
  pw = apwCand[cpwCand++];
  if ( pw != 0 )
    return pw;
  apwCand[cpwCand - 1] = NewWord();
  return apwCand[cpwCand - 1];
}

/* BuildWord -- build a Word structure from an ASCII word
   If the word does not fit, then do nothing.
*/
void BuildWord( char *pchWord );
void BuildWord( char *pchWord )
{
  unsigned char cchFrequency[ALPHABET];
  int i;
  char *pch = pchWord;
  PWord pw;
  unsigned int cchLength = 0;

  anagram_bzero( ( char * )cchFrequency, sizeof( unsigned char )*ALPHABET );

  /* Build frequency table */
  _Pragma( "loopbound min 3 max 636" )
  while ( ( i = *pch++ ) != '\0' ) {
    if ( !anagram_isalpha( i ) )
      continue;
    i = ch2i( anagram_tolower( i ) );
    if ( ++cchFrequency[i] > alPhrase[i].uFrequency )
      return ;
    ++cchLength;
  }

  /* Update global count */
  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < ALPHABET; i++ )
    auGlobalFrequency[i] += cchFrequency[i];

  /* Create a Word structure and fill it in, including building the
     bitfield of frequencies.
  */
  pw = NextWord();
  anagram_bzero( ( char * )( pw->aqMask ), sizeof( Quad )*MAX_QUADS );

  pw->pchWord = pchWord;
  pw->cchLength = cchLength;
  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < ALPHABET; i++ ) {
    pw->aqMask[alPhrase[i].iq] |=
      ( Quad )cchFrequency[i] << alPhrase[i].uShift;
  }
}

/* AddWords -- build the list of candidates */
void AddWords( void );
void AddWords( void )
{
  char *pch = pchDictionary;      /* walk through the dictionary */

  cpwCand = 0;

  _Pragma( "loopbound min 1967 max 1967" )
  while ( *pch ) {
    if ( ( pch[1] >= cchMinLength && pch[1] + cchMinLength <= cchPhraseLength )
         || pch[1] == cchPhraseLength )
      BuildWord( pch + 2 );
    pch += *pch;
  }
}

static PWord apwSol[MAXSOL];                   /* the answers */
static int cpwLast;

#define OneStep(i) \
    if ((aqNext[i] = pqMask[i] - pw->aqMask[i]) & aqMainSign[i]) { \
        ppwStart++; \
        continue; \
    }


void DumpWords( void );
void DumpWords( void )
{
  int i, j;
  char out[30];
  int offset = 0;
  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < cpwLast; i++ ) {
    _Pragma( "loopbound min 3 max 5" )
    for ( j = 0; apwSol[i]->pchWord[j] != '\0'; j++ )
      out[offset + j] = apwSol[i]->pchWord[j];
    offset += j;

    out[offset++] = ' ';
  }
  out[offset++] = '\0';
}

void FindAnagram( Quad *pqMask, PPWord ppwStart, int iLetter );
void FindAnagram( Quad *pqMask, PPWord ppwStart, int iLetter )
{
  Quad aqNext[MAX_QUADS];
  register PWord pw;
  Quad qMask;
  unsigned iq;
  PPWord ppwEnd = &apwCand[0];
  ppwEnd += cpwCand;

  _Pragma( "loopbound min 1 max 7" )
  while ( 1 ) {
    iq = alPhrase[achByFrequency[iLetter]].iq;
    qMask = alPhrase[achByFrequency[iLetter]].uBits <<
            alPhrase[achByFrequency[iLetter]].uShift;
    if ( pqMask[iq] & qMask )
      break;
    iLetter++;
  }

  _Pragma( "loopbound min 0 max 114" )
  while ( ppwStart < ppwEnd ) {        /* Half of the program execution */
    pw = *ppwStart;                  /* time is spent in these three */

    #if MAX_QUADS > 0

    OneStep( 0 );                   /* lines of code. */
    #endif

    #if MAX_QUADS > 1

    OneStep( 1 );
    #endif

    #if MAX_QUADS > 2

    OneStep( 2 );
    #endif

    #if MAX_QUADS > 3

    OneStep( 3 );
    #endif

    #if MAX_QUADS > 4

    @@"Add more unrolling steps here, please."@@
    #endif

    /* If the pivot letter isn't present, defer this word until later */
    if ( ( pw->aqMask[iq] & qMask ) == 0 ) {
      *ppwStart = *--ppwEnd;
      *ppwEnd = pw;
      continue;
    }

    /* If we get here, this means the word fits. */
    apwSol[cpwLast++] = pw;
    if ( cchPhraseLength -= pw->cchLength ) { /* recurse */
      /* The recursive call scrambles the tail, so we have to be
         pessimistic.
      */
      ppwEnd = &apwCand[0];
      ppwEnd += cpwCand;
      FindAnagram( &aqNext[0],
                   ppwStart, iLetter );
    } else DumpWords();             /* found one */
    cchPhraseLength += pw->cchLength;
    --cpwLast;
    ppwStart++;
    continue;
  }
}

void SortCandidates( void );
void SortCandidates( void )
{
  int i;

  /* Sort the letters by frequency */
  _Pragma( "loopbound min 26 max 26" )
  for ( i = 0; i < ALPHABET; i++ )
    achByFrequency[i] = i;
  anagram_qsort( achByFrequency, ALPHABET, sizeof( int ) );
}

int main( void )
{
  int i;

  ReadDict();

  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < 3; i++ ) {
    Reset();
    BuildMask( anagram_achPhrase[i] );
    AddWords();
    if ( cpwCand == 0 || cchPhraseLength == 0 )
      continue;

    cpwLast = 0;
    SortCandidates();
    _Pragma( "marker call_find" )
    FindAnagram( aqMainMask, apwCand, 0 );
    _Pragma( "flowrestriction 1*FindAnagram <= 51*call_find" )
  }

  return 0;
}
