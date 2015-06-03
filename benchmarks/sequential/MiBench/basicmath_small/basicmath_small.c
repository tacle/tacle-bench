#include "wcclibm.h"
#include "snipmath.h"

int main(void)
{
  double  a1 = 1.0f, b1 = -10.5f, c1 = 32.0f, d1 = -30.0f;
  double  a2 = 1.0f, b2 = -4.5f, c2 = 17.0f, d2 = -30.0f;
  double  a3 = 1.0f, b3 = -3.5f, c3 = 22.0f, d3 = -31.0f;
  double  a4 = 1.0f, b4 = -13.7f, c4 = 1.0f, d4 = -35.0f;
  double  x[3];
  double X;
  int     solutions;
  int i;
  unsigned long l = 0x3fed0169L;
  struct int_sqrt q;
  long n = 0;

  /* solve soem cubic functions */
  /* should get 3 solutions: 2, 6 & 2.5   */
  SolveCubic(a1, b1, c1, d1, &solutions, x);

  SolveCubic(a2, b2, c2, d2, &solutions, x);

  SolveCubic(a3, b3, c3, d3, &solutions, x);

  SolveCubic(a4, b4, c4, d4, &solutions, x);

  /* Now solve some random equations */
  _Pragma( "loopbound min 5 max 5" )
  for(a1=1;a1<10;a1+=2) {
    _Pragma( "loopbound min 5 max 5" )
    for(b1=10;b1>0;b1-=2) {
      _Pragma( "loopbound min 7 max 7" )
      for(c1=5;c1<15;c1+=1.5f) {
        _Pragma( "loopbound min 5 max 5" )
        for(d1=-1;d1>-11;d1-=2) {
          SolveCubic(a1, b1, c1, d1, &solutions, x);
        }
      }
    }
  }

  /* perform some integer square roots */
  _Pragma( "loopbound min 1000 max 1000" )
  for (i = 1; i < 1001; i+=1) {
    usqrt(i, &q);
    // remainder differs on some machines
  }

  usqrt(l, &q);

  /* convert some rads to degrees */
  _Pragma( "loopbound min 361 max 361" )
  for (X = 0.0f; X <= 360.0f; X += 1.0f) {
    deg2rad(X);
  }

  _Pragma( "loopbound min 360 max 360" )
  for (X = 0.0f; X <= (2 * PI + 1e-6f); X += (PI / 180)) {
    rad2deg(X);
  }

  return 0;
}
