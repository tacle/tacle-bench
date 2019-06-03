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

  1. FUNCTION

  The AES algorithm Rijndael implemented for block and key sizes of
  128 bits (16 bytes) by Brian Gladman.

  This is an implementation of the AES encryption algorithm (Rijndael)
  designed by Joan Daemen and Vincent Rijmen.

  2. THE CIPHER INTERFACE

  byte                    (an unsigned 8-bit type)
  word                    (an unsigned 32-bit type)
  aes_ret:                (a signed 16 bit type for function return values)
       aes_good            (value != 0, a good return)
       aes_bad             (value == 0, an error return)
  enum aes_key:           (encryption direction)
       enc                 (set key for encryption)
       dec                 (set key for decryption)
       both                (set key for both)
  class or struct aes     (structure for context)

  C subroutine calls:

  aes_ret set_blk(const word block_length, aes *cx)  (variable block size)
  aes_ret set_key(const byte key[  ], const word key_length,
                  const enum aes_key direction, aes *cx)
  aes_ret encrypt(const byte input_blk[  ], byte output_blk[  ], const aes *cx)
  aes_ret decrypt(const byte input_blk[  ], byte output_blk[  ], const aes *cx)

  IMPORTANT NOTE: If you are using this C interface and your compiler does
  not set the memory used for objects to zero before use, you will need to
  ensure that cx.mode is set to zero before using the C subroutine calls.

  The block length inputs to set_block and set_key are in numbers of
  BYTES, not bits.  The calls to subroutines must be made in the above
  order but multiple calls can be made without repeating earlier calls
  if their parameters have not changed. If the cipher block length is
  variable but set_blk has not been called before cipher operations a
  value of 16 is assumed (that is, the AES block size). In contrast to
  earlier versions the block and key length parameters are now checked
  for correctness and the encryption and decryption routines check to
  ensure that an appropriate key has been set before they are called.

*/

#ifndef _AES_H
#define _AES_H

/* The only supported block size for the benchmark is 16 */
#define BLOCK_SIZE  16

/*
  The number of key schedule words for different block and key lengths
  (allowing for the method of computation which requires the length to
  be a multiple of the key length):

  Key Schedule    key length (bytes)
  Length          16  20  24  28  32
               ---------------------
  block     16 |  44  60  54  56  64
  length    20 |  60  60  66  70  80
  (bytes)   24 |  80  80  78  84  96
            28 | 100 100 102  98 112
            32 | 120 120 120 126 120

  Rcon Table      key length (bytes)
  Length          16  20  24  28  32
               ---------------------
  block     16 |  10   9   8   7   7
  length    20 |  14  11  10   9   9
  (bytes)   24 |  19  15  12  11  11
            28 |  24  19  16  13  13
            32 |  29  23  19  17  14

  The following values assume that the key length will be variable and may
  be of maximum length (32 bytes).

  Nk = number_of_key_bytes / 4
  Nc = number_of_columns_in_state / 4
  Nr = number of encryption/decryption rounds
  Rc = number of elements in rcon table
  Ks = number of 32-bit words in key schedule
*/

#define Nr(Nk,Nc)   ((Nk > Nc ? Nk : Nc) + 6)
#define Rc(Nk,Nc)   ((Nb * (Nr(Nk,Nc) + 1) - 1) / Nk)
#define Ks(Nk,Nc)   (Nk * (Rc(Nk,Nc) + 1))

#define RC_LENGTH   5 * BLOCK_SIZE / 4 - (BLOCK_SIZE == 16 ? 10 : 11)
#define KS_LENGTH   4 * BLOCK_SIZE

/* End of configuration options, but see also aes.c */

typedef unsigned char   byte;           /* must be an 8-bit storage unit */
typedef unsigned long   word;           /* must be a 32-bit storage unit */
typedef short           aes_ret;        /* function return value         */

#define aes_bad     0
#define aes_good    1

/*
  upr(x,n): rotates bytes within words by n positions, moving bytes
  to higher index positions with wrap around into low positions
  ups(x,n): moves bytes by n positions to higher index positions in
  words but without wrap around
  bval(x,n): extracts a byte from a word
*/

#define upr(x,n)        (((x) << 8 * (n)) | ((x) >> (32 - 8 * (n))))
#define ups(x,n)        ((x) << 8 * (n))
#define bval(x,n)       ((byte)((x) >> 8 * (n)))
#define byte_swap(x)    (upr(x,1) & 0x00ff00ff | upr(x,3) & 0xff00ff00)
#define bytes2word(b0, b1, b2, b3) ((word)(b3) << 24 | (word)(b2) << 16 | \
                                    (word)(b1) << 8 | (b0))

#define word_in(x)      *(word*)(x)
#define word_out(x,v)   *(word*)(x) = (v)

enum aes_const  {   Nrow =  4,  /* the number of rows in the cipher state       */
                    Mcol =  8,  /* maximum number of columns in the state       */
                    Ncol =  BLOCK_SIZE / 4,
                    Shr0 =  0,  /* the cyclic shift values for rows 0, 1, 2 & 3 */
                    Shr1 =  1,
                    Shr2 =  BLOCK_SIZE == 32 ? 3 : 2,
                    Shr3 =  BLOCK_SIZE == 32 ? 4 : 3
                };

enum aes_key    {   enc  =  1,  /* set if encryption is needed */
                    dec  =  2,  /* set if decryption is needed */
                    both =  3   /* set if both are needed      */
                };

struct aes {
  word    Nkey;               /* the number of words in the key input block */
  word    Nrnd;               /* the number of cipher rounds                */
  word    e_key[ KS_LENGTH ];   /* the encryption key schedule                */
  word    d_key[ KS_LENGTH ];   /* the decryption key schedule                */
  byte    mode;               /* encrypt, decrypt or both                   */
};

aes_ret rijndael_dec_set_key( byte key[  ], const word n_bytes,
                              const enum aes_key f, struct aes *cx );
aes_ret rijndael_dec_decrypt( const byte in_blk[  ], byte out_blk[  ],
                              const struct aes *cx );

#endif
