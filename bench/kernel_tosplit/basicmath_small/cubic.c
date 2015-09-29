/* +++Date last modified: 05-Jul-1997 */

/*
**  CUBIC.C - Solve a cubic polynomial
**  public domain by Ross Cottrell
*/

//#include <stdlib.h>
#include "snipmath.h"

void SolveCubic(double  a,
                double  b,
                double  c,
                double  d,
                int    *solutions,
                double *x)
{
      double    a1 = b/a, a2 = c/a, a3 = d/a;
      double    Q = (a1*a1 - 3.0f*a2)/9.0f;
      double    R = (2.0f*a1*a1*a1 - 9.0f*a1*a2 + 27.0f*a3)/54.0f;
      double    R2_Q3 = R*R - Q*Q*Q;

      double    theta;

      if (R2_Q3 <= 0)
      {
            *solutions = 3;
            theta = acos(R/sqrt(Q*Q*Q));
            x[0] = -2.0f*sqrt(Q)*cos(theta/3.0f) - a1/3.0f;
            x[1] = -2.0f*sqrt(Q)*cos((theta+2.0f*PI)/3.0f) - a1/3.0f;
            x[2] = -2.0f*sqrt(Q)*cos((theta+4.0f*PI)/3.0f) - a1/3.0f;
      }
      else
      {
            *solutions = 1;
            x[0] = pow(sqrt(R2_Q3)+fabs(R), 1/3.0f);
            x[0] += Q/x[0];
            x[0] *= (R < 0.0f) ? 1 : -1;
            x[0] -= a1/3.0f;
      }
}

#ifdef TEST

int main(void)
{
      double  a1 = 1.0f, b1 = -10.5f, c1 = 32.0f, d1 = -30.0f;
      double  a2 = 1.0f, b2 = -4.5f, c2 = 17.0f, d2 = -30.0f;
      double  x[3];
      int     solutions;

      SolveCubic(a1, b1, c1, d1, &solutions, x);

      /* should get 3 solutions: 2, 6 & 2.5   */

      SolveCubic(a2, b2, c2, d2, &solutions, x);

      /* should get 1 solution: 2.5           */

      return 0;
}

#endif /* TEST */
