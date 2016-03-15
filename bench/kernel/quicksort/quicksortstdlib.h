/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: quicksort

  Author: Matthew R. Guthaus

  Function: quicksort applies a recursive quicksort algorithm to two different
    input sets.

  Source: MiBench
          http://wwweb.eecs.umich.edu/mibench

  Original name: qsort

  Changes: No major functional changes.

  License: GPL

*/


#ifndef __QUICKSORTSTDLIB_H
#define __QUICKSORTSTDLIB_H

int quicksort_compare_strings( const char *, const char * );
int quicksort_compare_vectors( const char *, const char * );
void quicksort_swapi( char *, char *, unsigned long );
char *quicksort_pivot_strings( char *, unsigned long, unsigned long );
char *quicksort_pivot_vectors( char *, unsigned long, unsigned long );

#endif
