#include "glibc_common.h"

int my_toupper(int c) {
    if ((c >= 'a') && (c <= 'z'))
        return c - 'a' + 'A';
    return c;
}

