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
/*  FILE: select.c                                                       */
/*  SOURCE : Numerical Recipes in C - The Second Edition                 */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     A function to select the Nth largest number in the floating poi-  */
/*     nt array arr[].                                                   */
/*     The parameters to function select are k and n. Then the function  */
/*     selects k-th largest number out of n original numbers.            */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;


static float select(unsigned long k, unsigned long n)
{
  float arr[20] = {
    5.0f,   4.0f,  10.3f,   1.1f,   5.7f,
    100.0f, 231.0f, 111.0f,  49.5f,  99.0f,
    10.0f, 150.0f, 222.22f, 101.0f, 77.0f,
    44.0f,  35.0f,  20.54f, 99.99f, 888.88f
  };

  unsigned long i,ir,j,l,mid;
  float a,temp;
  int flag, flag2;

  l=1;
  ir=n;
  flag = flag2 = 0;
  _Pragma("loopbound min 16 max 16")
  while (!flag) {
    if (ir <= l+1) {
      if (ir == l+1)
        if (arr[ir] < arr[l]) {
          SWAP(arr[l],arr[ir])
            }
      flag = 1;
    } else if (!flag) {
      mid=(l+ir) >> 1;
      SWAP(arr[mid],arr[l+1])
        if (arr[l+1] > arr[ir]) {
          SWAP(arr[l+1],arr[ir])
            }
      if (arr[l] > arr[ir]) {
        SWAP(arr[l],arr[ir])
          }
      if (arr[l+1]> arr[l]) {
        SWAP(arr[l+1],arr[l])
          }
      i=l+1;
      j=ir;
      a=arr[l];
      _Pragma("loopbound min 0 max 1")
      while (!flag2) {
        i++;
	_Pragma("loopbound min 1 max 1")
        while (arr[i] < a) i++;
        j--;
	_Pragma("loopbound min 16 max 16")
        while (arr[j] > a) j--;
        if (j < i) flag2 = 1;
        if (!flag2) SWAP(arr[i],arr[j]);

      }
      arr[l]=arr[j];
      arr[j]=a;

      if (j >= k) ir=j-1;
      if (j <= k) l=i;
    }

  }
  return arr[k];
}

int main()
{
  select(10, 20);
  return 0;
}

