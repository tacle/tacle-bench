#ifndef __QUICKSORTSTDLIB_H
#define __QUICKSORTSTDLIB_H

int quicksort_compare_strings( const char *, const char * );
int quicksort_compare_vectors( const char *, const char * );
void quicksort_swapi( char *, char *, unsigned long );
char *quicksort_pivot_strings( char *, unsigned long, unsigned long );
char *quicksort_pivot_vectors( char *, unsigned long, unsigned long );

#endif
