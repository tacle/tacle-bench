/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: rijndael_enc

  Author: Dr Brian Gladman

  Function: rijndael_dec is an implementation of the AES decryption
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
#include "rijndael_dec_libc.h"

/*
  Global variable definitions
*/
unsigned char rijndael_dec_key[ 32 ];
int rijndael_dec_key_len;

extern unsigned char rijndael_dec_data[  ];
struct rijndael_dec_FILE rijndael_dec_fin;

int rijndael_dec_checksum = 0;

/*
  Forward declaration of functions
*/
void rijndael_dec_init( void );
int rijndael_dec_return( void );
void rijndael_dec_fillrand( unsigned char *buf, int len );
void rijndael_dec_decfile( struct rijndael_dec_FILE *fin, struct aes *ctx );
void rijndael_dec_main( void );

void rijndael_dec_init( void )
{
  /* create a pseudo-file for the input*/
  rijndael_dec_fin.data = rijndael_dec_data;
  rijndael_dec_fin.size = 32768;
  rijndael_dec_fin.cur_pos = 0;

  unsigned i;
  volatile int x = 0;
  rijndael_dec_fin.size ^= x;
  _Pragma( "loopbound min 32768 max 32768" )
  for ( i = 0; i < rijndael_dec_fin.size; i++ )
    rijndael_dec_fin.data[ i ] ^= x;

  /* this is a pointer to the hexadecimal key digits  */
  const volatile char *cp =
    "1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321";
  char ch;
  int by = 0;

  i = 0;                  /* this is a count for the input digits processed */
  _Pragma( "loopbound min 64 max 64" )
  while ( i < 64 && *cp ) { /* the maximum key length is 32 bytes and       */
    /* hence at most 64 hexadecimal digits            */
    ch = rijndael_dec_toupper( *cp++ );     /* process a hexadecimal digit  */
    if ( ch >= '0' && ch <= '9' )
      by = ( by << 4 ) + ch - '0';
    else
      if ( ch >= 'A' && ch <= 'F' )
        by = ( by << 4 ) + ch - 'A' + 10;
      else {                                /* error if not hexadecimal     */
        rijndael_dec_checksum = -2;
        return;
      }

    /* store a key byte for each pair of hexadecimal digits         */
    if ( i++ & 1 )
      rijndael_dec_key[ i / 2 - 1 ] = by & 0xff;
  }

  if ( *cp ) {
    rijndael_dec_checksum = -3;
    return;
  } else
    if ( i < 32 || ( i & 15 ) ) {
      rijndael_dec_checksum = -4;
      return;
    }

  rijndael_dec_key_len = i / 2;
}

int rijndael_dec_return( void )
{
  return ( ( rijndael_dec_checksum == ( int )262180 ) ? 0 : -1 );
}

void rijndael_dec_decfile( struct rijndael_dec_FILE *fin, struct aes *ctx )
{
  unsigned char inbuf1[ 16 ], inbuf2[ 16 ], outbuf[ 16 ], *bp1, *bp2, *tp;
  int           i;


  rijndael_dec_fread( inbuf1, 1, 16, fin );

  i = rijndael_dec_fread( inbuf2, 1, 16,
                          fin ); /* read 1st encrypted file block    */

  if ( i && i != 16 ) {
    rijndael_dec_checksum = -10;
    return;
  }

  rijndael_dec_decrypt( inbuf2, outbuf,
                        ctx ); /* decrypt it                       */

  rijndael_dec_checksum += outbuf[ 15 ];

  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0; i < 16; ++i )      /* xor with previous input          */
    outbuf[ i ] ^= inbuf1[ i ];

  bp1 = inbuf1;           /* set up pointers to two input buffers     */
  bp2 = inbuf2;

  /* TODO: this is necessarily an input-dependent loop bound */
  _Pragma( "loopbound min 2046 max 2046" )
  while ( 1 ) {
    i = rijndael_dec_fread( bp1, 1, 16, fin );   /* read next encrypted block    */
    /* to first input buffer        */
    if ( i != 16 )      /* no more bytes in input - the decrypted   */
      break;          /* partial final buffer needs to be output  */

    /* if a block has been read the previous block must have been   */
    /* full lnegth so we can now write it out                       */

    rijndael_dec_decrypt( bp1, outbuf, ctx ); /* decrypt the new input block and  */

    rijndael_dec_checksum += outbuf[ 15 ];

    _Pragma( "loopbound min 16 max 16" )
    for ( i = 0; i < 16; ++i )  /* xor it with previous input block */
      outbuf[ i ] ^= bp2[ i ];

    /* swap buffer pointers                */
    tp = bp1, bp1 = bp2, bp2 = tp;
  }
}

void _Pragma( "entrypoint" ) rijndael_dec_main( void )
{
  struct aes ctx[ 1 ];

  /* decryption in Cipher Block Chaining mode */
  rijndael_dec_set_key( rijndael_dec_key, rijndael_dec_key_len, dec, ctx );
  rijndael_dec_decfile( &rijndael_dec_fin, ctx );
}

int main()
{

  rijndael_dec_init();
  rijndael_dec_main();

  return ( rijndael_dec_return() );
}

