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
/*  FILE: fft1.c                                                         */
/*  SOURCE : Turbo C Programming for Engineering by Hyun Soon Ahn        */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     FFT using Cooly-Turkey algorithm.                                 */
/*     There are two inputs, ar[] and ai[]. ar[] is real number parts    */
/*     of input array and the ai[] is imaginary number parts of input.   */
/*     The function fft1 process FFT or inverse FFT according to the    .*/
/*     parameter flag. (FFT with flag=0, inverse FFT with flag=1).       */
/*                                                                       */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

#define PI 3.14159f
#define M_PI 3.14159f


float ar[8];
float ai[8] = {0.0f,  };

int fft1(int n, int flag);


static float fabs(float n)
{
  float f;

  if (n >= 0) f = n;
  else f = -n;
  return f;
}

static float log(float n)
{
  return(4.5f);
}


static float sin(float rad)
{
  float app;

  float diff;
  int inc = 1;

  _Pragma("loopbound min 0 max 0")
  while (rad > 2*PI) {
    rad -= 2*PI;
  }

  _Pragma("loopbound min 0 max 0")
  while (rad < -2*PI) {
    rad += 2*PI;
  }
  app = diff = rad;
  diff = (diff * (-(rad*rad))) /
    ((2.0f * inc) * (2.0f * inc + 1.0f));
  app = app + diff;
  inc++;

  _Pragma("loopbound min 0 max 8")
  while(fabs(diff) >= 0.00001f) {
    diff = (diff * (-(rad*rad))) /
      ((2.0f * inc) * (2.0f * inc + 1.0f));
    app = app + diff;
    inc++;
  }

  return(app);
}


static float cos(float rad)
{

  return (sin (PI / 2.0f - rad));
}


int main(void)
{
  int  i, n = 8, flag, chkerr;
  /* ar  */
  _Pragma("loopbound min 8 max 8")
  for(i = 0; i < n; i++) {
    ar[i] = cos(2*M_PI*i/n);
  }


  /* forward fft */
  flag = 0;
  chkerr = fft1(n, flag);

  /* inverse fft */
  flag = 1;
  chkerr = fft1(n, flag);

  return 0;
}


int fft1(int n, int flag)
{

  int i, j, k, it, xp, xp2, j1, j2, iter;
  float sign, w, wr, wi, dr1, dr2, di1, di2, tr, ti, arg;

  if(n < 2) return(999);
/*
  iter = log((float)n)/log(2.0f);
*/
  iter = 2.079442f/0.693147f;
  j = 1;

  _Pragma("loopbound min 3 max 3")
  for(i = 0; i < iter; i++) {
    j *= 2;
  }
  if(fabs(n-j) > 1.0e-6f)
    return(1);

  /*  Main FFT Loops  */
  sign = ((flag == 1) ? 1.0f : -1.0f);
  xp2 = n;

  _Pragma("loopbound min 3 max 3")
  for(it = 0; it < iter; it++)
  {
    xp = xp2;
    xp2 /= 2;
    w = PI / xp2;
    _Pragma("loopbound min 1 max 4")
    for(k = 0; k < xp2; k++)
    {
      arg = k * w;
      wr = cos(arg);
      wi = sign * sin(arg);
      i = k - xp;
      _Pragma("loopbound min 1 max 4")
      for(j = xp; j <= n; j += xp)
      {
        j1 = j + i;
        j2 = j1 + xp2;
        dr1 = ar[j1];
        dr2 = ar[j2];
        di1 = ai[j1];
        di2 = ai[j2];
        tr = dr1 - dr2;
        ti = di1 - di2;
        ar[j1] = dr1 + dr2;
        ai[j1] = di1 + di2;
        ar[j2] = tr * wr - ti * wi;
        ai[j2] = ti * wr + tr * wi;
      }
    }
  }
  /*  Digit Reverse Counter  */

  j1 = n / 2;
  j2 = n - 1;
  j = 1;
  _Pragma("loopbound min 7 max 7")
  for(i = 1; i <= j2; i++)
  {
    if(i < j)
    {
      tr = ar[j-1];
      ti = ai[j-1];
      ar[j-1] = ar[i-1];
      ai[j-1] = ai[i-1];
      ar[i-1] = tr;
      ai[i-1] = ti;
    }
    k = j1;
    _Pragma("loopbound min 0 max 2")
    while(k < j)
    {
      j -= k;
      k /= 2;
    }
    j += k;
  }
  if(flag == 0) return(0);
  w = n;
  _Pragma("loopbound min 8 max 8")
  for(i = 0; i < n; i++)
  {
    ar[i] /= w;
    ai[i] /= w;
  }
  return(0);
}
