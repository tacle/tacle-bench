/*
* bitonic.c: C reference implementation of Bitonic Sort
* Chris Leger <clleger@cag.lcs.mit.edu>
* $Id: bitonic.c,v 1.2 2009-01-30 15:10:37 lokuciej Exp $
*/

/*
 * Copied almost exactly from http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/bitonicen.htm 
 * That URL also has a nice explanation of the code and theory.
 */

/* Forward declarations: */
void compare(int i, int j, int dir);
void bitonicMerge(int lo, int cnt, int dir);
void bitonicSort(int lo, int cnt, int dir);

/* Globals: */
int numiters = 10;
int a[32];         // the array to be sorted
const int ASCENDING = 1;
const int DESCENDING = 0;

int main(void)
{
  /** Initialize array "a" with data **/
  int i;
  
  _Pragma( "loopbound min 32 max 32" )
  for (i = 0; i < 32; i++) {
    a[i] = (32 - i);
  }
  
  /** When called with parameters lo = 0, cnt = a.length() and dir =
  * ASCENDING, procedure bitonicSort sorts the whole array a. **/
  _Pragma( "marker recSort" )
  bitonicSort(0, 32, ASCENDING);
  _Pragma( "flowrestriction 1*bitonicSort <= 63*recSort" )

  /** Loop through array, printing out each element **/
  _Pragma( "loopbound min 32 max 32" )
  for (i = 0; i < 32; i++) {
  }
  
  return 0;
}

/** A comparator is modelled by the procedure compare, where the
 * parameter dir indicates the sorting direction. If dir is ASCENDING
 * and a[i] > a[j] is true or dir is DESCENDING and a[i] > a[j] is
 * false then a[i] and a[j] are interchanged.
 **/
void compare(int i, int j, int dir)
{
  if (dir == (a[i] > a[j])) {
    int h = a[i];
    a[i] = a[j];
    a[j] = h;
  }
}

/** The procedure bitonicMerge recursively sorts a bitonic sequence in
 * ascending order, if dir = ASCENDING, and in descending order
 * otherwise. The sequence to be sorted starts at index position lo,
 * the number of elements is cnt.
 **/
void bitonicMerge(int lo, int cnt, int dir)
{
  int k = cnt / 2;
  int i;
  _Pragma( "loopbound min 0 max 16" )
  for (i = lo; i < lo + k; i++) {
    compare(i, i + k, dir);
  }
    
  if (k > 1) {
    bitonicMerge(lo, k, dir);
    bitonicMerge(lo + k, k, dir);
  }
}

/** Procedure bitonicSort first produces a bitonic sequence by
 * recursively sorting its two halves in opposite directions, and then
 * calls bitonicMerge.
 **/
void bitonicSort(int lo, int cnt, int dir)
{
  int k = cnt;
  k /= 2;
  _Pragma( "marker recMerge" )

  if (cnt > 1) {
    bitonicSort(lo, k, ASCENDING);
    bitonicSort(lo + k, k, DESCENDING);
  }

  bitonicMerge(lo, cnt, dir);
  _Pragma( "flowrestriction 1*bitonicMerge <= 31*recMerge" )

  return;
}
