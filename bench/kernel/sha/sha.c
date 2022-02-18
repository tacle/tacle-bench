/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: sha.c

  Author: Peter C. Gutmann's (heavily modified by Uwe Hollerbach)

  NIST Secure Hash Algorithm

  Source: Peter C. Gutmann's implementation as found in Applied Cryptography by Bruce Schneier

  Changes: no major functional changes

  License: GNU Lesser General Public License

*/

#include "memcpy.h"
#include "memset.h"
#include "sha.h"

/*
  Declaration of global variables
*/
struct SHA_INFO sha_info;

/* SHA f()-functions */
#define f1(x,y,z) ((x & y) | (~x & z))
#define f2(x,y,z) (x ^ y ^ z)
#define f3(x,y,z) ((x & y) | (x & z) | (y & z))
#define f4(x,y,z) (x ^ y ^ z)

/* SHA constants */
#define CONST1  0x5a827999L
#define CONST2  0x6ed9eba1L
#define CONST3  0x8f1bbcdcL
#define CONST4  0xca62c1d6L

#define BLOCK_SIZE 8192

/* 32-bit rotate */

#define ROT32(x,n) ((x << n) | (x >> (32 - n)))

#define FUNC(n,i)      \
    temp = ROT32(A,5) + f##n(B,C,D) + E + W[ i ] + CONST##n; \
    E = D; D = C; C = ROT32(B,30); B = A; A = temp

/* do SHA transformation */
void sha_transform( struct SHA_INFO *sha_info )
{
  int i;
  LONG temp, A, B, C, D, E, W[ 80 ];

  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0; i < 16; ++i )
    W[ i ] = sha_info->data[ i ];
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 16; i < 80; ++i )
    W[ i ] = W[ i - 3 ] ^ W[ i - 8 ] ^ W[ i - 14 ] ^ W[ i - 16 ];

  A = sha_info->digest[ 0 ];
  B = sha_info->digest[ 1 ];
  C = sha_info->digest[ 2 ];
  D = sha_info->digest[ 3 ];
  E = sha_info->digest[ 4 ];


  _Pragma( "loopbound min 20 max 20" )
  for ( i = 0; i < 20; ++i ) {
    FUNC( 1, i );
  }
  _Pragma( "loopbound min 20 max 20" )
  for ( i = 20; i < 40; ++i ) {
    FUNC( 2, i );
  }
  _Pragma( "loopbound min 20 max 20" )
  for ( i = 40; i < 60; ++i ) {
    FUNC( 3, i );
  }
  _Pragma( "loopbound min 20 max 20" )
  for ( i = 60; i < 80; ++i ) {
    FUNC( 4, i );
  }
  sha_info->digest[ 0 ] += A;
  sha_info->digest[ 1 ] += B;
  sha_info->digest[ 2 ] += C;
  sha_info->digest[ 3 ] += D;
  sha_info->digest[ 4 ] += E;
}


/* change endianness of data */
void sha_byte_reverse( LONG *buffer, int count )
{
  int i;
  BYTE ct[ 4 ], *cp;

  count /= sizeof( LONG );
  cp = ( BYTE * ) buffer;
  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0; i < count; ++i ) {
    ct[ 0 ] = cp[ 0 ];
    ct[ 1 ] = cp[ 1 ];
    ct[ 2 ] = cp[ 2 ];
    ct[ 3 ] = cp[ 3 ];
    cp[ 0 ] = ct[ 3 ];
    cp[ 1 ] = ct[ 2 ];
    cp[ 2 ] = ct[ 1 ];
    cp[ 3 ] = ct[ 0 ];
    cp += sizeof( LONG );
  }
}

/* initialize the SHA digest */
void sha_init( void )
{
  int i;
  sha_info.digest[ 0 ] = 0x67452301L;
  sha_info.digest[ 1 ] = 0xefcdab89L;
  sha_info.digest[ 2 ] = 0x98badcfeL;
  sha_info.digest[ 3 ] = 0x10325476L;
  sha_info.digest[ 4 ] = 0xc3d2e1f0L;
  sha_info.count_lo = 0L;
  sha_info.count_hi = 0L;
  for ( i = 0; i < 16; i++ )
    sha_info.data[ i ] = 0;
}

size_t sha_fread( void *ptr, size_t size, size_t count,
                  struct SHA_MY_FILE *stream )
{
  unsigned i = stream->cur_pos, i2 = 0;
  size_t number_of_chars_to_read =
    stream->size - stream->cur_pos >= size * count ?
    size * count : stream->size - stream->cur_pos;
  _Pragma( "loopbound min 0 max 8192" )
  while ( i < stream->cur_pos + number_of_chars_to_read )
    ( ( unsigned char * )ptr )[ i2++ ] = stream->data[ i++ ];
  stream->cur_pos += number_of_chars_to_read;
  return ( number_of_chars_to_read );
}

/* update the SHA digest */
void sha_update( struct SHA_INFO *sha_info, BYTE *buffer, int count )
{
  if ( ( sha_info->count_lo + ( ( LONG ) count << 3 ) ) < sha_info->count_lo )
    ++sha_info->count_hi;
  sha_info->count_lo += ( LONG ) count << 3;
  sha_info->count_hi += ( LONG ) count >> 29;
  _Pragma( "loopbound min 8 max 128" )
  while ( count >= SHA_BLOCKSIZE ) {
    sha_glibc_memcpy( sha_info->data, buffer, SHA_BLOCKSIZE );
    sha_byte_reverse( sha_info->data, SHA_BLOCKSIZE );
    sha_transform( sha_info );
    buffer += SHA_BLOCKSIZE;
    count -= SHA_BLOCKSIZE;
  }

  sha_glibc_memcpy( sha_info->data, buffer, count );
}

/* finish computing the SHA digest */
void sha_final( struct SHA_INFO *sha_info )
{
  int count;
  LONG lo_bit_count, hi_bit_count;

  lo_bit_count = sha_info->count_lo;
  hi_bit_count = sha_info->count_hi;
  count = ( int ) ( ( lo_bit_count >> 3 ) & 0x3f );
  ( ( BYTE * ) sha_info->data )[ count++ ] = 0x80;
  if ( count > 56 ) {
    sha_glibc_memset( ( BYTE * ) &sha_info->data + count, 0, 64 - count );
    sha_byte_reverse( sha_info->data, SHA_BLOCKSIZE );
    sha_transform( sha_info );
    sha_glibc_memset( &sha_info->data, 0, 56 );
  } else
    sha_glibc_memset( ( BYTE * ) &sha_info->data + count, 0, 56 - count );

  sha_byte_reverse( sha_info->data, SHA_BLOCKSIZE );
  sha_info->data[ 14 ] = hi_bit_count;
  sha_info->data[ 15 ] = lo_bit_count;
  sha_transform( sha_info );
}

/* compute the SHA digest of a FILE stream */

void sha_stream( struct SHA_INFO *sha_info, struct SHA_MY_FILE *fin )
{
  int i;
  BYTE data[ BLOCK_SIZE ];
  _Pragma( "loopbound min 5 max 5" )
  while ( ( i = sha_fread( data, 1, BLOCK_SIZE, fin ) ) > 0 )
    sha_update( sha_info, data, i );

  sha_final( sha_info );
}

void _Pragma( "entrypoint" ) sha_main( void )
{
  struct SHA_MY_FILE fin;
  fin.data = sha_data;
  fin.size = 32743;  // set size = 3247552 for input_large
  fin.cur_pos = 0;
  sha_stream( &sha_info, &fin );
}

int sha_return( void )
{
  int sum = 0;
  sum = sha_info.data[ 14 ] + sha_info.data[ 15 ];
  return ( sum - 261944 != 0 );
}

int main ( void )
{
  sha_init();
  sha_main();
  return ( sha_return() );
}
