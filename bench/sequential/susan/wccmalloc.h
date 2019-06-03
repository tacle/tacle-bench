#ifndef _WCCMALLOC_H
#define _WCCMALLOC_H

void *susan_wccmalloc( unsigned int numberOfBytes );
//! Frees ALL allocated memory space
void susan_wccfreeall( void );
void *susan_wccmemcpy( void *dstpp, const void *srcpp, unsigned int len );
void susan_wccmemset( void *p, int value, unsigned int num );

#endif
