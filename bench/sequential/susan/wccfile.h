#ifndef WCC_FILE_H
#define WCC_FILE_H

enum _Origin_ { WCCSEEK_SET, WCCSEEK_CUR, WCCSEEK_END };
typedef enum _Origin_ Origin;
typedef unsigned int size_t;

#define EOF -1

struct wccFILE {
  char *data;
  size_t size;
  unsigned cur_pos;
};

size_t susan_wccfread         ( void *ptr, size_t size, size_t count,
                                struct wccFILE *stream );
int    susan_wccfseek         ( struct wccFILE *stream, long int offset,
                                Origin origin );
int    susan_wccfgetpos       ( struct wccFILE *stream, unsigned *position );
int    susan_wccfeof          ( struct wccFILE *stream );
int    susan_wccfgetc         ( struct wccFILE *stream );
char  *susan_wccfgets         ( char *str, int num, struct wccFILE *stream );

#endif

