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

  FUNCTION

  The AES algorithm Rijndael implemented for block and key sizes of 128,
  bits (16 bytes) by Brian Gladman.

  This is an implementation of the AES encryption algorithm (Rijndael)
  designed by Joan Daemen and Vincent Rijmen.
*/

#include "aes.h"

#include "aestab.h"

#define four_tables(x,tab,vf,rf,c)  ( tab[ 0 ][ bval(vf(x,0,c),rf(0,c)) ] ^ \
                                      tab[ 1 ][ bval(vf(x,1,c),rf(1,c)) ] ^ \
                                      tab[ 2 ][ bval(vf(x,2,c),rf(2,c)) ] ^ \
                                      tab[ 3 ][ bval(vf(x,3,c),rf(3,c)) ] )

#define vf1(x,r,c)  (x)
#define rf1(r,c)    (r)
#define rf2(r,c)    ((r-c)&3)

#define ls_box(x,c)     four_tables(x,rijndael_enc_fl_tab,vf1,rf2,c)

#define inv_mcol(x)     four_tables(x,rijndael_enc_im_tab,vf1,rf1,0)

/*
  Subroutine to set the block size (if variable) in bytes, legal
  values being 16, 24 and 32.
*/

#define nc   (Ncol)

/*
  Initialise the key schedule from the user supplied key. The key
  length is now specified in bytes - 16, 24 or 32 as appropriate.
  This corresponds to bit lengths of 128, 192 and 256 bits, and
  to Nk values of 4, 6 and 8 respectively.
*/

#define mx(t,f) (*t++ = inv_mcol(*f),f++)
#define cp(t,f) *t++ = *f++

#define cpy(d,s)    do { cp(d,s); cp(d,s); cp(d,s); cp(d,s); } while (0)        //min 1 max 1
#define mix(d,s)    do { mx(d,s); mx(d,s); mx(d,s); mx(d,s); } while (0)        //min 1 max 1

aes_ret rijndael_enc_set_key( byte in_key[  ], const word n_bytes,
                              const enum aes_key f, struct aes *cx )
{
  word    *kf, *kt, rci;

  if ( ( n_bytes & 7 ) || n_bytes < 16 || n_bytes > 32 || ( !( f & 1 ) &&
       !( f & 2 ) ) )
    return ( n_bytes ? cx->mode &= ~0x03, aes_bad : ( aes_ret )( cx->Nkey << 2 ) );

  cx->mode = ( cx->mode & ~0x03 ) | ( ( byte )f & 0x03 );
  cx->Nkey = n_bytes >> 2;
  cx->Nrnd = Nr( cx->Nkey, ( word )nc );

  cx->e_key[ 0 ] = word_in( in_key     );
  cx->e_key[ 1 ] = word_in( in_key +  4 );
  cx->e_key[ 2 ] = word_in( in_key +  8 );
  cx->e_key[ 3 ] = word_in( in_key + 12 );

  kf = cx->e_key;
  kt = kf + nc * ( cx->Nrnd + 1 ) - cx->Nkey;
  rci = 0;
  
  switch ( cx->Nkey ) {
    case 4:
      _Pragma( "loopbound min 0 max 0" )
      do {
        kf[ 4 ] = kf[ 0 ] ^ ls_box( kf[ 3 ], 3 ) ^ rijndael_enc_rcon_tab[ rci++ ];
        kf[ 5 ] = kf[ 1 ] ^ kf[ 4 ];
        kf[ 6 ] = kf[ 2 ] ^ kf[ 5 ];
        kf[ 7 ] = kf[ 3 ] ^ kf[ 6 ];
        kf += 4;
      } while ( kf < kt );
      break;

    case 6:
      cx->e_key[ 4 ] = word_in( in_key + 16 );
      cx->e_key[ 5 ] = word_in( in_key + 20 );
      _Pragma( "loopbound min 0 max 0" )
      do {
        kf[  6 ] = kf[ 0 ] ^ ls_box( kf[ 5 ], 3 ) ^ rijndael_enc_rcon_tab[ rci++ ];
        kf[  7 ] = kf[ 1 ] ^ kf[  6 ];
        kf[  8 ] = kf[ 2 ] ^ kf[  7 ];
        kf[  9 ] = kf[ 3 ] ^ kf[  8 ];
        kf[ 10 ] = kf[ 4 ] ^ kf[  9 ];
        kf[ 11 ] = kf[ 5 ] ^ kf[ 10 ];
        kf += 6;
      } while ( kf < kt );
      break;

    case 8:
      cx->e_key[ 4 ] = word_in( in_key + 16 );
      cx->e_key[ 5 ] = word_in( in_key + 20 );
      cx->e_key[ 6 ] = word_in( in_key + 24 );
      cx->e_key[ 7 ] = word_in( in_key + 28 );
      _Pragma( "loopbound min 7 max 7" )
      do {
        kf[  8 ] = kf[ 0 ] ^ ls_box( kf[ 7 ], 3 ) ^ rijndael_enc_rcon_tab[ rci++ ];
        kf[  9 ] = kf[ 1 ] ^ kf[  8 ];
        kf[ 10 ] = kf[ 2 ] ^ kf[  9 ];
        kf[ 11 ] = kf[ 3 ] ^ kf[ 10 ];
        kf[ 12 ] = kf[ 4 ] ^ ls_box( kf[ 11 ], 0 );
        kf[ 13 ] = kf[ 5 ] ^ kf[ 12 ];
        kf[ 14 ] = kf[ 6 ] ^ kf[ 13 ];
        kf[ 15 ] = kf[ 7 ] ^ kf[ 14 ];
        kf += 8;
      } while ( kf < kt );
      break;
  }

  if ( ( cx->mode & 3 ) != enc ) {
    word    i;

    kt = cx->d_key + nc * cx->Nrnd;
    kf = cx->e_key;

    cpy( kt, kf );
    kt -= 2 * nc;

    _Pragma( "loopbound min 0 max 0" )
    for ( i = 1; i < cx->Nrnd; ++i ) {
      mix( kt, kf );
      kt -= 2 * nc;
    }

    cpy( kt, kf );
  }

  return aes_good;
}

short rijndael_enc_encrypt( unsigned char  in_blk[  ], unsigned char  out_blk[  ],
                            const struct aes *cx )
{
  const unsigned long *kp = cx->e_key;
  if ( !( cx->mode & 1 ) )
    return 0;
  unsigned long  b0[ 4 ];
  b0[ 0 ] = *( unsigned long * )in_blk ^ kp[ 0 ];
  b0[ 1 ] = *( unsigned long * )( in_blk + 4 )^kp[ 1 ];
  b0[ 2 ] = *( unsigned long * )( in_blk + 8 )^kp[ 2 ];
  b0[ 3 ] = *( unsigned long * )( in_blk + 12 )^kp[ 3 ];
  kp += 4;
  unsigned long  b1[ 4 ];
  switch ( cx->Nrnd ) {
    case 14:
      b1[ 0 ] = kp[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = kp[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = kp[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = kp[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 4 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 4 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 4 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 4 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
      kp += 8;
    case 12:
      b1[ 0 ] = kp[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = kp[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = kp[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = kp[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 4 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 4 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 4 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 4 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
      kp += 8;
    case 10:
      b1[ 0 ] = kp[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = kp[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = kp[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = kp[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                        rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                        rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                        rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 4 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 4 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 4 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 4 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
      b1[ 0 ] = ( kp + 8 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = ( kp + 8 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = ( kp + 8 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = ( kp + 8 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                                rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                                rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                                rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 12 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 12 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 12 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 12 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
      b1[ 0 ] = ( kp + 16 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = ( kp + 16 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = ( kp + 16 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = ( kp + 16 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 20 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 20 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 20 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 20 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
      b1[ 0 ] = ( kp + 24 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = ( kp + 24 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = ( kp + 24 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = ( kp + 24 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 28 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 28 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 28 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 28 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
      b1[ 0 ] = ( kp + 32 )[ 0 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 0 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 3 ] >> 24 ) ) ] );
      b1[ 1 ] = ( kp + 32 )[ 1 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 1 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 0 ] >> 24 ) ) ] );
      b1[ 2 ] = ( kp + 32 )[ 2 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 2 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 1 ] >> 24 ) ) ] );
      b1[ 3 ] = ( kp + 32 )[ 3 ] ^ ( rijndael_enc_ft_tab[ 0 ][ ( ( unsigned char )b0[ 3 ] ) ] ^
                                 rijndael_enc_ft_tab[ 1 ][ ( ( unsigned char )( b0[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_ft_tab[ 2 ][ ( ( unsigned char )( b0[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_ft_tab[ 3 ][ ( ( unsigned char )( b0[ 2 ] >> 24 ) ) ] );
      b0[ 0 ] = ( kp + 36 )[ 0 ] ^ ( rijndael_enc_fl_tab[ 0 ][ ( ( unsigned char )b1[ 0 ] ) ] ^
                                 rijndael_enc_fl_tab[ 1 ][ ( ( unsigned char )( b1[ 1 ] >> 8 ) ) ] ^
                                 rijndael_enc_fl_tab[ 2 ][ ( ( unsigned char )( b1[ 2 ] >> 16 ) ) ] ^
                                 rijndael_enc_fl_tab[ 3 ][ ( ( unsigned char )( b1[ 3 ] >> 24 ) ) ] );
      b0[ 1 ] = ( kp + 36 )[ 1 ] ^ ( rijndael_enc_fl_tab[ 0 ][ ( ( unsigned char )b1[ 1 ] ) ] ^
                                 rijndael_enc_fl_tab[ 1 ][ ( ( unsigned char )( b1[ 2 ] >> 8 ) ) ] ^
                                 rijndael_enc_fl_tab[ 2 ][ ( ( unsigned char )( b1[ 3 ] >> 16 ) ) ] ^
                                 rijndael_enc_fl_tab[ 3 ][ ( ( unsigned char )( b1[ 0 ] >> 24 ) ) ] );
      b0[ 2 ] = ( kp + 36 )[ 2 ] ^ ( rijndael_enc_fl_tab[ 0 ][ ( ( unsigned char )b1[ 2 ] ) ] ^
                                 rijndael_enc_fl_tab[ 1 ][ ( ( unsigned char )( b1[ 3 ] >> 8 ) ) ] ^
                                 rijndael_enc_fl_tab[ 2 ][ ( ( unsigned char )( b1[ 0 ] >> 16 ) ) ] ^
                                 rijndael_enc_fl_tab[ 3 ][ ( ( unsigned char )( b1[ 1 ] >> 24 ) ) ] );
      b0[ 3 ] = ( kp + 36 )[ 3 ] ^ ( rijndael_enc_fl_tab[ 0 ][ ( ( unsigned char )b1[ 3 ] ) ] ^
                                 rijndael_enc_fl_tab[ 1 ][ ( ( unsigned char )( b1[ 0 ] >> 8 ) ) ] ^
                                 rijndael_enc_fl_tab[ 2 ][ ( ( unsigned char )( b1[ 1 ] >> 16 ) ) ] ^
                                 rijndael_enc_fl_tab[ 3 ][ ( ( unsigned char )( b1[ 2 ] >> 24 ) ) ] );
  }
  *( unsigned long * )out_blk = ( b0[ 0 ] );
  *( unsigned long * )( out_blk + 4 ) = ( b0[ 1 ] );
  *( unsigned long * )( out_blk + 8 ) = ( b0[ 2 ] );
  *( unsigned long * )( out_blk + 12 ) = ( b0[ 3 ] );
  return aes_good;
}

