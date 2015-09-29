#ifndef SHA_H
#define SHA_H

#include "my_file.h"

/* NIST Secure Hash Algorithm */
/* heavily modified from Peter C. Gutmann's implementation */

/* Useful defines & typedefs */

typedef unsigned char BYTE;
typedef unsigned long LONG;

#define SHA_BLOCKSIZE		64
#define SHA_DIGESTSIZE		20

struct SHA_INFO {
    LONG digest[5];		/* message digest */
    LONG count_lo, count_hi;	/* 64-bit bit count */
    LONG data[16];		/* SHA data buffer */
};

extern void sha_init(struct SHA_INFO *);
extern void sha_update(struct SHA_INFO *, BYTE *, int);
extern void sha_final(struct SHA_INFO *);

extern void sha_stream(struct SHA_INFO *, struct my_FILE *);

#endif  // SHA_H

