#ifndef GLIBC_COMMON_H
#define GLIBC_COMMON_H

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned size_t;

/* Type to use for unaligned operations. */
typedef unsigned char byte;


#endif  // GLIBC_COMMON_H

