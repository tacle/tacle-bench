#ifndef MY_FILE_H
#define MY_FILE_H

#include "glibc_common.h"

enum _Origin_ { MY_SEEK_SET, MY_SEEK_CUR, MY_SEEK_END };
typedef enum _Origin_ Origin;

struct my_FILE {
    unsigned char* data;
    size_t size;
    unsigned cur_pos;
};

size_t my_fread        (void* ptr, size_t size, size_t count, struct my_FILE* stream);
size_t my_fwrite (const void* ptr, size_t size, size_t count, struct my_FILE* stream);
int    my_fseek  (struct my_FILE* stream, long int offset, Origin origin);
int    my_fgetpos(struct my_FILE* stream, unsigned* position);
int    my_feof   (struct my_FILE* stream);

#endif  // MY_FILE_H

