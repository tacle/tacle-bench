#include "wccfile.h"
#define EOF -1

size_t susan_wccfread( void *ptr, size_t size, size_t count,
                       struct wccFILE *stream )
{
  if ( susan_wccfeof( stream ) )
    return EOF;

  unsigned i = stream->cur_pos, i2 = 0;
  size_t number_of_chars_to_read =
    stream->size - stream->cur_pos >= size * count ?
    size * count : stream->size - stream->cur_pos;
  _Pragma( "loopbound min 7220 max 7220" )
  while ( i < stream->cur_pos + number_of_chars_to_read )
    ( ( unsigned char * )ptr )[ i2++ ] = stream->data[ i++ ];
  stream->cur_pos += number_of_chars_to_read;
  return number_of_chars_to_read;
}

int susan_wccfgetc( struct wccFILE *stream )
{
  if ( susan_wccfeof( stream ) )
    return EOF;

  else
    return stream->data[ stream->cur_pos++ ];
}

char *susan_wccfgets( char *str, int num, struct wccFILE *stream )
{
  if ( !stream || susan_wccfeof( stream ) || !str || num <= 0 )
    return 0;

  int pos = 0;
  _Pragma( "loopbound min 57 max 57" )
  while ( pos < num - 1 && !susan_wccfeof( stream ) ) {
    str[ pos ] = stream->data[ stream->cur_pos ];
    if ( str[ pos ] == '\n' )
      break;

    stream->cur_pos++;
    pos++;
  }
  str[ pos++ ] = '\0';

  return str;
}

int susan_wccfseek( struct wccFILE *stream, long int offset,
                    enum _Origin_ origin )
{
  if ( origin == WCCSEEK_SET ) {
    stream->cur_pos = offset;
    return 0;
  } else
    if ( origin == WCCSEEK_CUR ) {
      stream->cur_pos += offset;
      return 0;
    } else
      if ( origin == WCCSEEK_END ) {
        stream->cur_pos = stream->size + offset;
        return 0;
      }
  return -1;
}


int susan_wccfgetpos( struct wccFILE *stream, unsigned *position )
{
  *position = stream->cur_pos;
  return 0;
}


int susan_wccfeof( struct wccFILE *stream )
{
  return stream->cur_pos == stream->size ? 1 : 0;
}
