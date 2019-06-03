#include "rijndael_dec_libc.h"

int rijndael_dec_toupper( int c )
{
  if ( ( c >= 'a' ) && ( c <= 'z' ) )
    return c - 'a' + 'A';
  return c;
}

unsigned long rijndael_dec_fread( void *ptr, unsigned long size,
                                  unsigned long count, struct rijndael_dec_FILE *stream )
{
  unsigned i = stream->cur_pos, i2 = 0;
  unsigned long number_of_chars_to_read =
    stream->size - stream->cur_pos >= size * count ?
    size * count : stream->size - stream->cur_pos;
  _Pragma( "loopbound min 0 max 16" )
  while ( i < stream->cur_pos + number_of_chars_to_read )
    ( ( unsigned char * )ptr )[ i2++ ] = stream->data[ i++ ];
  stream->cur_pos += number_of_chars_to_read;
  return number_of_chars_to_read;
}

unsigned long rijndael_dec_fwrite( const void *ptr, unsigned long size,
                                   unsigned long count, struct rijndael_dec_FILE *stream )
{
  unsigned i = stream->cur_pos, i2 = 0;
  unsigned long number_of_chars_to_write =
    stream->size - stream->cur_pos >= size * count ?
    size * count : stream->size - stream->cur_pos;
  _Pragma( "loopbound min 0 max 0" )
  while ( i < stream->cur_pos + number_of_chars_to_write )
    stream->data[ i++ ] = ( ( unsigned char * )ptr )[ i2++ ];
  stream->cur_pos += number_of_chars_to_write;
  return number_of_chars_to_write;
}

int rijndael_dec_fseek( struct rijndael_dec_FILE *stream, long int offset,
                        Origin origin )
{
  if ( origin == RIJNDAEL_DEC_SEEK_SET ) {
    stream->cur_pos = offset;
    return 0;
  } else
    if ( origin == RIJNDAEL_DEC_SEEK_CUR ) {
      stream->cur_pos += offset;
      return 0;
    } else
      if ( origin == RIJNDAEL_DEC_SEEK_END ) {
        stream->cur_pos = stream->size + offset;
        return 0;
      }
  return -1;
}

int rijndael_dec_fgetpos( struct rijndael_dec_FILE *stream,
                          unsigned *position )
{
  *position = stream->cur_pos;
  return 0;
}

int rijndael_dec_feof( struct rijndael_dec_FILE *stream )
{
  return stream->cur_pos == stream->size ? 1 : 0;
}
