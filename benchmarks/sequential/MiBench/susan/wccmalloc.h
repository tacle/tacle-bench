#ifndef _WCCMALLOC_H
#define _WCCMALLOC_H

void* wccmalloc( unsigned int numberOfBytes );
//! Frees ALL allocated memory space
void wccfreeall( void );
void *wccmemcpy( void* dstpp, const void* srcpp, unsigned int len );
void wccmemset( void *p, int value, unsigned int num );

#endif
