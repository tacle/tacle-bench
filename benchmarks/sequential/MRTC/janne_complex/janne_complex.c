/* Remove the following #define for actual WCET analyses! */
/*
#define PROFILING
*/

#ifdef PROFILING
#include <stdio.h>
#endif


/*----------------------------------------------------------------------
 *  WCET Benchmark created by Andreas Ermedahl, Uppsala university, 
 *  May 2000.  
 *
 *  The purpose of this benchmark is to have two loop where the inner 
 *  loops max number of iterations depends on the outer loops current 
 *  iterations.  The results corresponds to something Jannes flow-analysis 
 *  should produce.
 *
 * The example appeard for the first time in: 
 * 
 *  @InProceedings{Ermedahl:Annotations,
 * author =       "A. Ermedahl and J. Gustafsson",
 * title =        "Deriving Annotations for Tight Calculation of Execution Time",
 * year =         1997,
 * month =        aug,
 * booktitle =    EUROPAR97,
 * publisher =    "Springer Verlag",
 * pages =        "1298-1307"
 * }
 *
 * The result of Jannes tool is something like:
 *   outer loop:       1   2   3   4   5   6   7   8   9   10   11 
 *   inner loop max:   5   9   8   7   4   2   1   1   1   1    1 
 *
 *----------------------------------------------------------------------*/

int complex( int a, int b )
{
  #ifdef PROFILING
  /* Profiling variables. Remove for actual WCET analyses. */
  int iters_a = 0, min_a = 100000, max_a = 0;
  int iters_b = 0, min_b = 100000, max_b = 0;
  #endif


  #ifdef PROFILING
  iters_a = 0;
  #endif
  _Pragma("loopbound min 9 max 9")
  while ( a < 30 ) {
    #ifdef PROFILING
    iters_a++;
    #endif

    #ifdef PROFILING
    iters_b = 0;
    #endif
    _Pragma("loopbound min 0 max 9")
    while ( b < a ) {
      #ifdef PROFILING
      iters_b++;
      #endif

      if ( b > 5 )
        b = b * 3; 
      else
        b = b + 2;

      if ( b >= 10 && b <= 12 )
        a = a + 10;
      else 
        a = a + 1;
    }
    
    #ifdef PROFILING
    if ( iters_b < min_b )
      min_b = iters_b;
    if ( iters_b > max_b )
      max_b = iters_b;
    #endif

    a = a + 2; 
    b = b - 10; 
  }
  
  #ifdef PROFILING
  if ( iters_a < min_a )
    min_a = iters_a;
  if ( iters_a > max_a )
    max_a = iters_a;
  #endif

  #ifdef PROFILING
  printf( "a-loop: [%d, %d]\n", min_a, max_a );
  printf( "b-loop: [%d, %d]\n", min_b, max_b );
  #endif

  return 1;
}


int main()
{
  int a = 1, b = 1, answer = 0;

  answer = complex( a, b );

  return answer;
}
