#include "my_file.h"

size_t my_fread(void* ptr, size_t size, size_t count, struct my_FILE* stream)
{
  unsigned i = stream->cur_pos, i2 = 0;
  size_t number_of_chars_to_read =
    stream->size - stream->cur_pos >= size * count ?
    size * count : stream->size - stream->cur_pos;
  _Pragma("loopbound min 0 max 8192")
  while (i < stream->cur_pos + number_of_chars_to_read) {
    ((unsigned char*)ptr)[i2++] = stream->data[i++];
  }
  stream->cur_pos += number_of_chars_to_read;
  return(number_of_chars_to_read);
}

