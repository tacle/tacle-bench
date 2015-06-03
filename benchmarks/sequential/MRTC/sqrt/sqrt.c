/*************************************************************************/
/*                                                                       */
/*   SNU-RT Benchmark Suite for Worst Case Timing Analysis               */
/*   =====================================================               */
/*                              Collected and Modified by S.-S. Lim      */
/*                                           sslim@archi.snu.ac.kr       */
/*                                         Real-Time Research Group      */
/*                                        Seoul National University      */
/*                                                                       */
/*                                                                       */
/*        < Features > - restrictions for our experimental environment   */
/*                                                                       */
/*          1. Completely structured.                                    */
/*               - There are no unconditional jumps.                     */
/*               - There are no exit from loop bodies.                   */
/*                 (There are no 'break' or 'return' in loop bodies)     */
/*          2. No 'switch' statements.                                   */
/*          3. No 'do..while' statements.                                */
/*          4. Expressions are restricted.                               */
/*               - There are no multiple expressions joined by 'or',     */
/*                'and' operations.                                      */
/*          5. No library calls.                                         */
/*               - All the functions needed are implemented in the       */
/*                 source file.                                          */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/*  FILE: sqrt.c                                                         */
/*  SOURCE : Public Domain Code                                          */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     Square root function implemented by Taylor series.                */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

float myfabs(float n)
{
  float f;

  if (n >= 0)
    f = n;
  else
    f = -n;
  return f;
}


float mysqrt(float val)
{
  float x = val/10;
  float dx;
  float diff;
  float min_tol = 0.00001f;
  int i, flag;

  flag = 0;
  if ( val == 0 ) {
    x = 0;
  } else {
    _Pragma("loopbound min 19 max 19")
    for ( i=1; i<20; i++ ) {
      if ( !flag ) {
        dx = ( val - ( x*x ) ) / ( 2.0f * x );
        x = x + dx;
        diff = val - ( x*x );

        if ( myfabs( diff ) <= min_tol )
          flag = 1;
      } else {
        x =x;
      }
    }
  }
  return (x);
}

static float v[] = { 1.1f, 2.2f, 4.4f, 33.33f, 3.14159f, 12345.6789f };

int main(void)
{
  int i;
  _Pragma("loopbound min 6 max 6")
  for ( i = 0; i < ( sizeof( v ) / sizeof( float ) ); i++ ) {
    mysqrt(v[i]);
  }

  return 0;
}
