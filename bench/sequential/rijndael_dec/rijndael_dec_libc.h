#ifndef RIJNDAEL_DEC_LIBC_H
#define RIJNDAEL_DEC_LIBC_H

int rijndael_dec_toupper ( int c );

enum _Origin_ { RIJNDAEL_DEC_SEEK_SET, RIJNDAEL_DEC_SEEK_CUR, RIJNDAEL_DEC_SEEK_END };
typedef enum _Origin_ Origin;
struct rijndael_dec_FILE {
  unsigned char *data;
  unsigned long size;
  unsigned cur_pos;
};

unsigned long rijndael_dec_fread ( void *ptr, unsigned long size,
                                   unsigned long count, struct rijndael_dec_FILE *stream );
unsigned long rijndael_dec_fwrite ( const void *ptr, unsigned long size,
                                    unsigned long count, struct rijndael_dec_FILE *stream );
int    rijndael_dec_fseek  ( struct rijndael_dec_FILE *stream, long int offset,
                             Origin origin );
int    rijndael_dec_fgetpos( struct rijndael_dec_FILE *stream,
                             unsigned *position );
int    rijndael_dec_feof   ( struct rijndael_dec_FILE *stream );

#endif  // RIJNDAEL_DEC_LIBC_H
