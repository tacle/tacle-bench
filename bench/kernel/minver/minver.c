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
/*  FILE: minver.c                                                       */
/*  SOURCE : Turbo C Programming for Engineering by Hyun Soo Ahn         */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     Matrix inversion for 3x3 floating point matrix.                   */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
int minver(int row, int col, double eps);
int  mmul(int  row_a, int col_a, int row_b, int col_b);

static double  a[3][3] = {
  3.0, -6.0,  7.0,
  9.0,  0.0, -5.0,
  5.0, -8.0,  6.0,
};
double  b[3][3], c[3][3], aa[3][3], a_i[3][3], e[3][3], det;


double minver_fabs(double n)
{
  double f;

  if (n >= 0) f = n;
  else f = -n;
  return f;
}

int main()
{
	int i, j;
	double eps;
	eps = 1.0e-6;
  _Pragma("loopbound min 3 max 3")
	for(i = 0; i < 3; i++) {
    _Pragma("loopbound min 3 max 3")
	  for(j = 0; j < 3; j++) {
	    aa[i][j] = a[i][j];
    }
  }

	minver(3, 3, eps);
  _Pragma("loopbound min 3 max 3")
	for(i = 0; i < 3; i++) {
    _Pragma("loopbound min 3 max 3")
	  for(j = 0; j < 3; j++) {
	    a_i[i][j] = a[i][j];
    }
  }

	mmul(3, 3, 3, 3);
  return 0;
}


int  mmul(int row_a, int col_a, int row_b, int col_b)
{
  int i, j, k, row_c, col_c;
  double w;

  row_c = row_a;
  col_c = col_b;

  if(row_c < 1 || row_b < 1 || col_c < 1 || col_a != row_b) return(999);

  _Pragma("loopbound min 3 max 3")
  for(i = 0; i < row_c; i++)
  {
    _Pragma("loopbound min 3 max 3")
    for(j = 0; j < col_c; j++)
    {
      w = 0.0;
      _Pragma("loopbound min 3 max 3")
      for(k = 0; k < row_b; k++)
        w += a[i][k] * b[k][j];

      c[i][j] = w;

    }
  }
  return(0);

}


int minver(int row, int col, double eps)
{

	int work[500], i, j, k, r, iw, s, t, u, v;
	double w, wmax, pivot, api, w1;

	if(row < 2 || row > 500 || eps <= 0.0) return(999);
	w1 = 1.0;
  _Pragma("loopbound min 3 max 3")
	for(i = 0; i < row; i++){
	  work[i] = i;
	}
  _Pragma("loopbound min 3 max 3")
	for(k = 0; k < row; k++)
	  {
	    wmax = 0.0;
      _Pragma("loopbound min 1 max 3")
	    for(i = k; i < row; i++)
	      {
		w = minver_fabs(a[i][k]);
		if(w > wmax)
		  {
		    wmax = w;
		    r = i;
		  }
	      }
	    pivot = a[r][k];
	    api = minver_fabs(pivot);
	    if(api <= eps)
	      {
		det = w1;
		return(1);
	      }
	    w1 *= pivot;
	    u = k * col;
	    v = r * col;
	    if(r != k)
	      {
		w1 = -w;
		iw = work[k];
		work[k] = work[r];
		work[r] = iw;
		_Pragma("loopbound min 3 max 3")
		for(j = 0; j < row; j++)
		  {
		    s = u + j;
		    t = v + j;
		    w = a[k][j];
		    a[k][j] = a[r][j];
		    a[r][j] = w;
		  }
	      }
      _Pragma("loopbound min 3 max 3")
	    for(i = 0; i < row; i++) {
	      a[k][i] /= pivot;
	    }
	    _Pragma("loopbound min 3 max 3")
	    for(i = 0; i < row; i++)
	      {
		if(i != k)
		  {
		    v = i * col;
		    s = v + k;
		    w = a[i][k];
		    if(w != 0.0)
		      {
			_Pragma("loopbound min 3 max 3")
			for(j = 0; j < row; j++) {
			  if(j != k) a[i][j] -= w * a[k][j];
			}
			a[i][k] = -w / pivot;

		      }
		  }
	      }
	    a[k][k] = 1.0 / pivot;
	  }
        _Pragma("loopbound min 3 max 3")
	for(i = 0; i < row; )
	  {
      /* The following redundant statement is inserted due to limitations of
       * WCC's flow fact manager. It is required in order to have the flow
       * fact pragma below uniquely attached to the while(1) loop.
       */
            i = i;
            _Pragma("loopbound min 1 max 3")
	    while(1)
	      {
		k = work[i];
		if(k == i) break;
		iw = work[k];
		work[k] = work[i];
		work[i] = iw;
		_Pragma("loopbound min 3 max 3")
		for(j = 0; j < row; j++)
		  {
		    u = j * col;
		    s = u + i;
		    t = u + k;
		    w = a[k][i];
		    a[k][i] = a[k][k];
		    a[k][k] = w;
		  }
	      }
            i++;
	  }
	det = w1;
	return(0);

}
