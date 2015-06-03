/* MDH WCET BENCHMARK SUITE. File version $Id: fac.c,v 1.5 2011-01-10 14:46:54 falk Exp $ */
/*
 * Changes: CS 2006/05/19: Changed loop bound from constant to variable.
 */

int fac (int n)
{
  if (n == 0)
     return 1;
  else
     return (n * fac (n-1));
}

int main (void)
{
  int i;
  int s = 0;
  volatile int n;

  n = 5;

  _Pragma("loopbound min 6 max 6")
  for (i = 0;  i <= n; i++) {
      _Pragma( "marker recursivecall" )
      s += fac (i);
      _Pragma( "flowrestriction 1*fac <= 6*recursivecall" )
  }

  return (s);
}

