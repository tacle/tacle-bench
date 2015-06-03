
 /*
   -----------------------------------------------------------------------
   Copyright (c) 2001 Dr Brian Gladman <brg@gladman.uk.net>, Worcester, UK
   
   TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   This software is provided 'as is' with no guarantees of correctness or
   fitness for purpose.
   -----------------------------------------------------------------------
 */

/* Example of the use of the AES (Rijndael) algorithm for file  */
/* encryption.  Note that this is an example application, it is */
/* not intended for real operational use.  The Command line is: */
/*                                                              */
/* aesxam input_file_name output_file_name [D|E] hexadecimalkey */
/*                                                              */
/* where E gives encryption and D decryption of the input file  */
/* into the output file using the given hexadecimal key string  */
/* The later is a hexadecimal sequence of 32, 48 or 64 digits   */
/* Examples to encrypt or decrypt aes.c into aes.enc are:       */
/*                                                              */
/* aesxam file.c file.enc E 0123456789abcdeffedcba9876543210    */
/*                                                              */
/* aesxam file.enc file2.c D 0123456789abcdeffedcba9876543210   */
/*                                                              */
/* which should return a file 'file2.c' identical to 'file.c'   */

#include "aes.h"
#include "my_file.h"
#include "glibc_common.h"

/* A Pseudo Random Number Generator (PRNG) used for the     */
/* Initialisation Vector. The PRNG is George Marsaglia's    */
/* Multiply-With-Carry (MWC) PRNG that concatenates two     */
/* 16-bit MWC generators:                                   */
/*     x(n)=36969 * x(n-1) + carry mod 2^16                 */ 
/*     y(n)=18000 * y(n-1) + carry mod 2^16                 */
/* to produce a combined PRNG with a period of about 2^60.  */  
/* The Pentium cycle counter is used to initialise it. This */
/* is crude but the IV does not need to be secret.          */
 
/* void cycles(unsigned long *rtn)     */
/* {                           // read the Pentium Time Stamp Counter */
/*     __asm */
/*     { */
/*     _emit   0x0f            // complete pending operations */
/*     _emit   0xa2 */
/*     _emit   0x0f            // read time stamp counter */
/*     _emit   0x31 */
/*     mov     ebx,rtn */
/*     mov     [ebx],eax */
/*     mov     [ebx+4],edx */
/*     _emit   0x0f            // complete pending operations */
/*     _emit   0xa2 */
/*     } */
/* } */

#define RAND(a,b) (((a = 36969 * (a & 65535) + (a >> 16)) << 16) + (b = 18000 * (b & 65535) + (b >> 16))  )

void fillrand(unsigned char *buf, int len)
{   static unsigned long a[2], mt = 1, count = 4;
    static char          r[4];
    int                  i;

    if(mt) { 
      mt = 0; 
      /*cycles(a);*/
      a[0]=0xeaf3;
      a[1]=0x35fe;
    }

    _Pragma( "loopbound min 1 max 16" )
    for(i = 0; i < len; ++i)
    {
        if(count == 4)
        {
            *(unsigned long*)r = RAND(a[0], a[1]);
            count = 0;
        }

        buf[i] = r[count++];
    }
}

int encfile(struct my_FILE* fin, struct aes *ctx)
{   unsigned char   inbuf[16], outbuf[16];
    long int        flen;
    unsigned long   i=0, l=0;

    fillrand(outbuf, 16);             /* set an IV for CBC mode           */
    flen = fin->size;
//    fseek(fin, 0, SEEK_END);        /* get the length of the file       */
//    fgetpos(fin, (fpos_t*)&flen);   /* and then reset to start          */
//    fseek(fin, 0, SEEK_SET);

//    fwrite(outbuf, 1, 16, fout);    /* write the IV to the output       */
    fillrand(inbuf, 1);             /* make top 4 bits of a byte random */
    l = 15;                         /* and store the length of the last */
                                    /* block in the lower 4 bits        */
    inbuf[0] = ((char)flen & 15) | (inbuf[0] & ~15);

    _Pragma( "loopbound min 1961 max 1961" )
    while(!my_feof(fin))               /* loop to encrypt the input file   */
    {                                  /* input 1st 16 bytes to buf[1..16] */
        i = my_fread(inbuf + 16 - l, 1, l, fin);  /*  on 1st round byte[0] */
                                                  /* is the length code    */
        if(i < l) break;               /* if end of the input file reached */

        _Pragma( "loopbound min 16 max 16" )
        for(i = 0; i < 16; ++i)        /* xor in previous cipher text      */
            inbuf[i] ^= outbuf[i];

        encrypt(inbuf, outbuf, ctx);   /* and do the encryption            */

//        if(fwrite(outbuf, 1, 16, fout) != 16) {
//          printf("Error writing to output file: %s\n", fn);
//          return -7;
//        }
                                    /* in all but first round read 16   */
        l = 16;                     /* bytes into the buffer            */
    }

    /* except for files of length less than two blocks we now have one  */
    /* byte from the previous block and 'i' bytes from the current one  */
    /* to encrypt and 15 - i empty buffer positions. For files of less  */
    /* than two blocks (0 or 1) we have i + 1 bytes and 14 - i empty    */
    /* buffer position to set to zero since the 'count' byte is extra   */

    if(l == 15)                         /* adjust for extra byte in the */
        ++i;                            /* in the first block           */

    if(i)                               /* if bytes remain to be output */
    {
        _Pragma( "loopbound min 6 max 6" )
        while(i < 16)                   /* clear empty buffer positions */
          inbuf[i++] = 0;

        _Pragma( "loopbound min 16 max 16" )
        for(i = 0; i < 16; ++i)         /* xor in previous cipher text  */
            inbuf[i] ^= outbuf[i]; 

        encrypt(inbuf, outbuf, ctx);    /* encrypt and output it        */

//        if(fwrite(outbuf, 1, 16, fout) != 16) {
//          printf("Error writing to output file: %s\n", fn);
//          return -8;
//        }
    }
    return 0;
}

extern unsigned char data[];

int main() {
    /* this is a pointer to the hexadecimal key digits  */
    char *cp = "1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321";
    char ch;
    unsigned char key[32];
    int i = 0, by = 0, key_len = 0, err = 0;
    struct aes ctx[1];

//--------------------------
    struct my_FILE my_fin;
    my_fin.data = data;
    my_fin.size = 31369;
    my_fin.cur_pos = 0;
//--------------------------

    i = 0;                  /* this is a count for the input digits processed */
    _Pragma( "loopbound min 64 max 64" )
    while(i < 64 && *cp)    /* the maximum key length is 32 bytes and         */
    {                       /* hence at most 64 hexadecimal digits            */
//        ch = toupper(*cp++);            /* process a hexadecimal digit  */
        ch = my_toupper(*cp++);         /* process a hexadecimal digit  */
        if(ch >= '0' && ch <= '9')
            by = (by << 4) + ch - '0';
        else if(ch >= 'A' && ch <= 'F')
            by = (by << 4) + ch - 'A' + 10;
        else                            /* error if not hexadecimal     */
        {
            //printf("key must be in hexadecimal notation\n");
            err = -2;
            return err;
        }

        /* store a key byte for each pair of hexadecimal digits         */
        if(i++ & 1)
            key[i / 2 - 1] = by & 0xff;
    }

    if(*cp)
    {
        //printf("The key value is too long\n");
        err = -3;
        return err;
    }
    else if(i < 32 || (i & 15))
    {
        //printf("The key length must be 32, 48 or 64 hexadecimal digits\n");
        err = -4;
        return err;
    }

    key_len = i / 2;

    /* encryption in Cipher Block Chaining mode */
    set_key(key, key_len, enc, ctx);
    err = encfile(&my_fin, ctx);

    return err;
}

