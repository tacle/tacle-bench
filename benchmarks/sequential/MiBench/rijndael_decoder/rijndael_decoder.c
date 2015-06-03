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

int decfile(struct my_FILE* fin, struct aes *ctx)
{   unsigned char inbuf1[16], inbuf2[16], outbuf[16], *bp1, *bp2, *tp;
    int           i, l, flen;


    my_fread(inbuf1, 1, 16, fin);

    i = my_fread(inbuf2, 1, 16, fin);  /* read 1st encrypted file block    */

    if(i && i != 16)
    {
        return -10;
    }

    decrypt(inbuf2, outbuf, ctx);   /* decrypt it                       */

    _Pragma("loopbound min 16 max 16")
    for(i = 0; i < 16; ++i)         /* xor with previous input          */
        outbuf[i] ^= inbuf1[i];

    flen = outbuf[0] & 15;  /* recover length of the last block and set */
    l = 15;                 /* the count of valid bytes in block to 15  */                              
    bp1 = inbuf1;           /* set up pointers to two input buffers     */
    bp2 = inbuf2;

    _Pragma("loopbound min 19491 max 91491")
    while(1)
    {
        i = my_fread(bp1, 1, 16, fin);     /* read next encrypted block    */
//        i = fread(bp1, 1, 16, fin);     /* read next encrypted block    */
                                        /* to first input buffer        */
        if(i != 16)         /* no more bytes in input - the decrypted   */
            break;          /* partial final buffer needs to be output  */

        /* if a block has been read the previous block must have been   */
        /* full lnegth so we can now write it out                       */

        decrypt(bp1, outbuf, ctx);  /* decrypt the new input block and  */

        _Pragma("loopbound min 16 max 16")
        for(i = 0; i < 16; ++i)     /* xor it with previous input block */
            outbuf[i] ^= bp2[i];
        
        /* set byte count to 16 and swap buffer pointers                */

        l = i; tp = bp1, bp1 = bp2, bp2 = tp;
    }

    /* we have now output 16 * n + 15 bytes of the file with any left   */
    /* in outbuf waiting to be output. If x bytes remain to be written, */
    /* we know that (16 * n + x + 15) % 16 = flen, giving x = flen + 1  */
    /* But we must also remember that the first block is offset by one  */
    /* in the buffer - we use the fact that l = 15 rather than 16 here  */  

    l = (l == 15 ? 1 : 0); flen += 1 - l;

    return 0;
}

extern unsigned char data_enc[];

int main() {
    /* this is a pointer to the hexadecimal key digits  */
    char *cp = "1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321";
    char ch;
    unsigned char key[32];
    int i = 0, by = 0, key_len = 0, err = 0;
    struct aes ctx[1];

//--------------------------
    struct my_FILE my_fin;
    my_fin.data = data_enc;
    my_fin.size = 32768;
    my_fin.cur_pos = 0;
//--------------------------

    i = 0;          /* this is a count for the input digits processed   */
    _Pragma("loopbound min 64 max 64")
    while(i < 64 && *cp)    /* the maximum key length is 32 bytes and   */
    {                       /* hence at most 64 hexadecimal digits      */
//        ch = toupper(*cp++);            /* process a hexadecimal digit  */
        ch = my_toupper(*cp++);          /* process a hexadecimal digit  */
        if(ch >= '0' && ch <= '9')
            by = (by << 4) + ch - '0';
        else if(ch >= 'A' && ch <= 'F')
            by = (by << 4) + ch - 'A' + 10;
        else                            /* error if not hexadecimal     */
        {
            err = -2;
            return err;
        }
        
        /* store a key byte for each pair of hexadecimal digits         */
        if(i++ & 1) 
            key[i / 2 - 1] = by & 0xff; 
    }

    if(*cp)
    {
        err = -3; 
        return err;
    }
    else if(i < 32 || (i & 15))
    {
        err = -4; 
        return err;
    }

    key_len = i / 2;


    /* decryption in Cipher Block Chaining mode */
    set_key(key, key_len, dec, ctx);
    err = decfile(&my_fin, ctx);

    return err;
}

