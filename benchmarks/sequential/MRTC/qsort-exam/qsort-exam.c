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
/*  FILE: qsort-exam.c                                                   */
/*  SOURCE : Numerical Recipes in C - The Second Edition                 */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     Non-recursive version of quick sort algorithm.                    */
/*     This example sorts 20 floating point numbers, arr[].              */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;
#define M 7
#define NSTACK 50

float arr[21] = {
  5.0f, 4.0f, 10.3f, 1.1f, 5.7f, 100.0f, 231.0f, 111.0f, 49.5f, 99.0f,
  10.0f, 150.0f, 222.22f, 101.0f, 77.0f, 44.0f, 35.0f, 20.54f, 99.99f, 88.88f,
  0.0f
};

int istack[100];

void sort(unsigned long n)
{
  unsigned long i,ir=n,j,k,l=1;
  int jstack=0;
  int flag;
  float a,temp;

  _Pragma("marker outside_L1")
  flag = 0;
  _Pragma("loopbound min 11 max 11")
  for (;i;) {
    _Pragma("marker inside_L1")
    if (ir-l < M) {
      _Pragma("loopbound min 0 max 4")
      for (j=l+1;j<=ir;j++) {
	a=arr[j];
	_Pragma("loopbound min 1 max 2")
	for (i=j-1;i>=l;i--) {
	  if (arr[i] <= a) break;
	  arr[i+1]=arr[i];
	}
	arr[i+1]=a;

      }
      if (jstack == 0) break;
      ir=istack[jstack--];
      l=istack[jstack--];
    } else {
      k=(l+ir) >> 1;
      SWAP(arr[k],arr[l+1])
	if (arr[l] > arr[ir]) {
	  SWAP(arr[l],arr[ir])
	}
      if (arr[l+1] > arr[ir]) {
	SWAP(arr[l+1],arr[ir])
      }
      if (arr[l] > arr[l+1]) {
	SWAP(arr[l],arr[l+1])
      }
      _Pragma("marker outside_L2")
      i=l+1;
      j=ir;
      a=arr[l+1];
      _Pragma("loopbound min 1 max 3")
      for (;i;) {
	i++;
	_Pragma("loopbound min 0 max 12")
	while (arr[i] < a) i++;
	j--;
	_Pragma("loopbound min 0 max 16")
	while (j>=0&&arr[j] > a) j--;
	if (j < i) break;
	_Pragma("marker inside_L2")
	SWAP(arr[i],arr[j]);
      }
      arr[l+1]=arr[j];
      arr[j]=a;
      jstack += 2;

      _Pragma("flowrestriction 1*inside_L2 <=  3*outside_L2")


      if (ir-i+1 >= j-l) {
	istack[jstack]=ir;
	istack[jstack-1]=i;
	ir=j-1;
      } else {
	istack[jstack]=j-1;
	istack[jstack-1]=l;
	l=i;
      }
    }
  }

  _Pragma("flowrestriction 1*inside_L1 <=  11*outside_L1")

}

int main(void)
{
  sort(20);
  return 0;
}

