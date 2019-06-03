/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: sha.h

  Author: Peter C. Gutmann's (heavily modified by Uwe Hollerbach)

  NIST Secure Hash Algorithm

  Source: Peter C. Gutmann's implementation as found in Applied Cryptography by Bruce Schneier

  Changes: no major functional changes

  License: May be used, modified, and re-distributed freely.
*/

#ifndef SHA_H
#define SHA_H

/* Useful defines & typedefs */

typedef unsigned char BYTE;
typedef unsigned long LONG;
typedef unsigned size_t;

/* Type to use for unaligned operations. */
#define SHA_BLOCKSIZE   64
#define SHA_DIGESTSIZE    20
#define LITTLE_ENDIAN
#define NULL ((void*)0)

extern unsigned volatile char sha_data[ 32743 ];

struct SHA_MY_FILE {
  unsigned volatile char *data;
  size_t size;
  unsigned cur_pos;
};

struct SHA_INFO {
  LONG digest[ 5 ];   /* message digest */
  LONG count_lo, count_hi;  /* 64-bit bit count */
  LONG data[ 16 ];    /* SHA data buffer */
};

/*
  Forward declaration of functions
*/
void sha_transform( struct SHA_INFO * );
void sha_byte_reverse( LONG *buffer, int count );
void sha_init( void );
size_t sha_fread( void *, size_t, size_t, struct SHA_MY_FILE * );
void sha_update( struct SHA_INFO *, BYTE *, int );
void sha_final( struct SHA_INFO * );
void sha_stream( struct SHA_INFO *, struct SHA_MY_FILE * );
void sha_main( void );
int sha_return ( void );
int main( void );

#endif  // SHA_H

