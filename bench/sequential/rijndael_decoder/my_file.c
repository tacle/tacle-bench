#include "my_file.h"

size_t my_fread(void* ptr, size_t size, size_t count, struct my_FILE* stream) {
    unsigned i = stream->cur_pos, i2 = 0;
    size_t number_of_chars_to_read =
        stream->size - stream->cur_pos >= size * count ?
            size * count : stream->size - stream->cur_pos;
    _Pragma("loopbound min 0 max 16")
    while (i < stream->cur_pos + number_of_chars_to_read)
        ((unsigned char*)ptr)[i2++] = stream->data[i++];
    stream->cur_pos += number_of_chars_to_read;
    return number_of_chars_to_read;
}

size_t my_fwrite(const void* ptr, size_t size, size_t count, struct my_FILE* stream) {
    unsigned i = stream->cur_pos, i2 = 0;
    size_t number_of_chars_to_write =
        stream->size - stream->cur_pos >= size * count ?
            size * count : stream->size - stream->cur_pos;
    _Pragma("loopbound min 0 max 16")
    while (i < stream->cur_pos + number_of_chars_to_write) {
        stream->data[i++] = ((unsigned char*)ptr)[i2++];
    }
    stream->cur_pos += number_of_chars_to_write;
    return number_of_chars_to_write;
}

int my_fseek(struct my_FILE* stream, long int offset, Origin origin) {
    if (origin == MY_SEEK_SET) {
        stream->cur_pos = offset;
        return 0;
    } else if (origin == MY_SEEK_CUR) {
        stream->cur_pos += offset;
        return 0;
    } else if (origin == MY_SEEK_END) {
        stream->cur_pos = stream->size + offset;
        return 0;
    }
    return -1;
}

int my_fgetpos(struct my_FILE* stream, unsigned* position) {
    *position = stream->cur_pos;
    return 0;
}

int my_feof(struct my_FILE* stream) {
    return stream->cur_pos == stream->size ? 1 : 0;
}

