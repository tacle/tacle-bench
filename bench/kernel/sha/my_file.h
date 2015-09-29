#ifndef MY_FILE_H
#define MY_FILE_H

#include "glibc_common.h"

struct my_FILE {
    unsigned char* data;
    size_t size;
    unsigned cur_pos;
};

size_t my_fread(void* ptr, size_t size, size_t count, struct my_FILE* stream);

#endif  // MY_FILE_H

