/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: rijndael_enc

  Author: Dr Brian Gladman

  Function: rijndael_enc is an implementation of the AES encryption
            algorithm (Rijndael).

  Source: security section of MiBench

  Changes: Add computation of a checksum, refactoring

  License: see below

*/

/*
  -----------------------------------------------------------------------
  Copyright (c) 2001 Dr Brian Gladman <brg@gladman.uk.net>, Worcester, UK

  TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  This software is provided 'as is' with no guarantees of correctness or
  fitness for purpose.
  -----------------------------------------------------------------------
*/

#include "aes.h"
#include "rijndael_enc_libc.h"

/*
  Global variable definitions
*/
unsigned char rijndael_enc_key[ 32 ];
int rijndael_enc_key_len;

extern unsigned char rijndael_enc_data[  ];
struct rijndael_enc_FILE rijndael_enc_fin;

int rijndael_enc_checksum = 0;

/*
  Forward declaration of functions
*/
void rijndael_enc_init( void );
int rijndael_enc_return( void );
void rijndael_enc_fillrand( unsigned char *buf, int len );
void rijndael_enc_encfile( struct rijndael_enc_FILE *fin, struct aes *ctx );
void rijndael_enc_main( void );

void rijndael_enc_init( void )
{
  /* create a pseudo-file for the input*/
  rijndael_enc_fin.data = rijndael_enc_data;
  rijndael_enc_fin.size = 31369;
  rijndael_enc_fin.cur_pos = 0;

  unsigned i;
  volatile int x = 0;
  rijndael_enc_fin.size ^= x;
  _Pragma( "loopbound min 31369 max 31369" )
  for ( i = 0; i < rijndael_enc_fin.size; i++ )
    rijndael_enc_fin.data[ i ] ^= x;

  /* this is a pointer to the hexadecimal key digits  */
  const volatile char *cp =
    "1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321";
  char ch;
  int by = 0;

  i = 0;                  /* this is a count for the input digits processed */
  _Pragma( "loopbound min 64 max 64" )
  while ( i < 64 && *cp ) { /* the maximum key length is 32 bytes and       */
    /* hence at most 64 hexadecimal digits            */
    ch = rijndael_enc_toupper( *cp++ );     /* process a hexadecimal digit  */
    if ( ch >= '0' && ch <= '9' )
      by = ( by << 4 ) + ch - '0';
    else
      if ( ch >= 'A' && ch <= 'F' )
        by = ( by << 4 ) + ch - 'A' + 10;
      else {                                /* error if not hexadecimal     */
        rijndael_enc_checksum = -2;
        return;
      }

    /* store a key byte for each pair of hexadecimal digits         */
    if ( i++ & 1 )
      rijndael_enc_key[ i / 2 - 1 ] = by & 0xff;
  }

  if ( *cp ) {
    rijndael_enc_checksum = -3;
    return;
  } else
    if ( i < 32 || ( i & 15 ) ) {
      rijndael_enc_checksum = -4;
      return;
    }

  rijndael_enc_key_len = i / 2;
}

int rijndael_enc_return( void )
{
  return ( ( rijndael_enc_checksum == ( int )249509 ) ? 0 : -1 );
}

/* A Pseudo Random Number Generator (PRNG) used for the     */
/* Initialisation Vector. The PRNG is George Marsaglia's    */
/* Multiply-With-Carry (MWC) PRNG that concatenates two     */
/* 16-bit MWC generators:                                   */
/*     x(n)=36969 * x(n-1) + carry mod 2^16                 */
/*     y(n)=18000 * y(n-1) + carry mod 2^16                 */
/* to produce a combined PRNG with a period of about 2^60.  */

#define RAND(a,b) (((a = 36969 * (a & 65535) + (a >> 16)) << 16) + (b = 18000 * (b & 65535) + (b >> 16))  )

void rijndael_enc_fillrand( unsigned char *buf, int len )
{
  static unsigned long a[ 2 ], mt = 1, count = 4;
  static char          r[ 4 ];
  int                  i;

  if ( mt ) {
    mt = 0;
    a[ 0 ] = 0xeaf3;
    a[ 1 ] = 0x35fe;
  }

  _Pragma( "loopbound min 1 max 16" )
  for ( i = 0; i < len; ++i ) {
    if ( count == 4 ) {
      *( unsigned long * )r = RAND( a[ 0 ], a[ 1 ] );
      count = 0;
    }

    buf[ i ] = r[ count++ ];
  }
}

void rijndael_enc_encfile( struct rijndael_enc_FILE *fin, struct aes *ctx )
{
  unsigned char   inbuf[ 16 ], outbuf[ 16 ];
  long int        flen;
  unsigned long   i = 0, l = 0;

  rijndael_enc_fillrand( outbuf,
                         16 );    /* set an IV for CBC mode           */
  flen = fin->size;

  rijndael_enc_fillrand( inbuf,
                         1 );     /* make top 4 bits of a byte random */
  l = 15;                         /* and store the length of the last */
  /* block in the lower 4 bits        */
  inbuf[ 0 ] = ( ( char )flen & 15 ) | ( inbuf[ 0 ] & ~15 );

  /* TODO: this is necessarily an input-dependent loop bound */
  _Pragma( "loopbound min 1960 max 1960" )
  while ( !rijndael_enc_feof(
            fin ) ) {             /* loop to encrypt the input file   */
    /* input 1st 16 bytes to buf[ 1..16 ] */
    i = rijndael_enc_fread( inbuf + 16 - l, 1, l, fin ); /* on 1st round byte[ 0 ] */
    /* is the length code    */
    if ( i < l ) break;           /* if end of the input file reached */

    _Pragma( "loopbound min 16 max 16" )
    for ( i = 0; i < 16; ++i )    /* xor in previous cipher text      */
      inbuf[ i ] ^= outbuf[ i ];

    rijndael_enc_encrypt( inbuf, outbuf,
                          ctx );  /* and do the encryption            */

    rijndael_enc_checksum += outbuf[ 15 ];

    /* in all but first round read 16   */
    l = 16;                       /* bytes into the buffer            */
  }

  /* except for files of length less than two blocks we now have one  */
  /* byte from the previous block and 'i' bytes from the current one  */
  /* to encrypt and 15 - i empty buffer positions. For files of less  */
  /* than two blocks (0 or 1) we have i + 1 bytes and 14 - i empty    */
  /* buffer position to set to zero since the 'count' byte is extra   */

  if ( l == 15 )                      /* adjust for extra byte in the */
    ++i;                              /* in the first block           */

  if ( i ) {                          /* if bytes remain to be output */
    _Pragma( "loopbound min 6 max 6" )
    while ( i < 16 )                  /* clear empty buffer positions */
      inbuf[ i++ ] = 0;

    _Pragma( "loopbound min 16 max 16" )
    for ( i = 0; i < 16; ++i )        /* xor in previous cipher text  */
      inbuf[ i ] ^= outbuf[ i ];

    rijndael_enc_encrypt( inbuf, outbuf, ctx ); /* encrypt and output it */

    rijndael_enc_checksum += outbuf[ 15 ];
  }
}

void _Pragma( "entrypoint" ) rijndael_enc_main( void )
{
  struct aes ctx[ 1 ];

  /* encryption in Cipher Block Chaining mode */
  rijndael_enc_set_key( rijndael_enc_key, rijndael_enc_key_len, enc, ctx );
  rijndael_enc_encfile( &rijndael_enc_fin, ctx );
}

int main( void )
{
  rijndael_enc_init();
  rijndael_enc_main();

  return ( rijndael_enc_return() );
}

