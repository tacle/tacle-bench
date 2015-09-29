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
/*  FILE: lms.c                                                          */
/*  SOURCE : C Algorithms for Real-Time DSP by P. M. Embree              */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     An LMS adaptive signal enhancement. The input signal is a sine    */
/*     wave with added white noise.                                      */
/*     The detailed description is in the program source code.           */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
#define RAND_MAX 32768
#define PI 3.14159265358979323846f

float lms(float x,float d,float *b,int l, float mu,float alpha);


#define N 201
#define L 20            /* filter order, (length L+1) */

/* set convergence parameter */
float mu = 0.01f;


static int rand()
{
  static unsigned long next = 1;

  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

static float log(float r)
{
  return 4.5f;
}

static float fabs(float n)
{
  float f;

  if (n >= 0) f = n;
  else f = -n;
  return f;
}

static float sqrt(float val)
{
  float x = val/10;

  float dx;

  float diff;
  float min_tol = 0.00001f;

  int i, flag;


  flag = 0;
  if (val == 0 ) x = 0;
  else {
    _Pragma("loopbound min 19 max 19")
    for (i=1;i<20;i++)
      {
	if (!flag) {
	  dx = (val - (x*x)) / (2.0f * x);
	  x = x + dx;
	  diff = val - (x*x);
	  if (fabs(diff) <= min_tol) flag = 1;
	}
	else
	  x =x;
        }
  }
  return (x);
}


static float sin(float rad)
{
  float app;

  float diff;
  int inc = 1;

  _Pragma("loopbound min 0 max 10")
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
  _Pragma("loopbound min 0 max 11")
  while(fabs(diff) >= 0.00001f) {
    diff = (diff * (-(rad*rad))) /
      ((2.0f * inc) * (2.0f * inc + 1.0f));
    app = app + diff;
    inc++;
   }

  return(app);
}

static float rconst1;
static float rconst2;

static float gaussian()
{
    static int ready = 0;       /* flag to indicated stored value */
    static float gstore;        /* place to store other value */
    rconst1 = (float)(2.0f/RAND_MAX);
    rconst2 = (float)(RAND_MAX/2.0f);
    float v1,v2,r,fac;
    float gaus;

/* make two numbers if none stored */
    if(ready == 0) {
            v1 = (float)rand() - rconst2;
            v2 = (float)rand() - rconst2;
            v1 *= rconst1;
            v2 *= rconst1;
            r = v1*v1 + v2*v2;
        _Pragma("loopbound min 0 max 2")
        while (r > 1.0f) {
            v1 = (float)rand() - rconst2;
            v2 = (float)rand() - rconst2;
            v1 *= rconst1;
            v2 *= rconst1;
            r = v1*v1 + v2*v2;

        }       /* make radius less than 1 */

/* remap v1 and v2 to two Gaussian numbers */
        fac = sqrt(-2.0f*log(r)/r);
        gstore = v1*fac;        /* store one */
        gaus = v2*fac;          /* return one */
        ready = 1;              /* set ready flag */
    }

    else {
        ready = 0;      /* reset ready flag for next pair */
        gaus = gstore;  /* return the stored one */
    }

    return gaus;
}


int main()
{

    static float d[N],b[21];
    float signal_amp,noise_amp,arg,x,y;
    int k;
/* create signal plus noise */
    signal_amp = sqrt(2.0f);
    noise_amp = 0.2f * sqrt(12.0f);
    arg = 2.0f * PI / 20.0f;
    _Pragma("loopbound min 201 max 201")
    for(k = 0 ; k < N ; k++) {
        d[k] = signal_amp*sin(arg*k) + noise_amp*gaussian();
    }

/* scale based on L */
    mu = 2.0f * mu/(L+1);

    x = 0.0f;
    _Pragma("loopbound min 201 max 201")
    for(k = 0 ; k < N ; k++) {
        lms(x,d[k],b,L,mu,0.01f);
/* delay x one sample */
        x = d[k];
    }
    return 0;
}

/*
      function lms(x,d,b,l,mu,alpha)

Implements NLMS Algorithm b(k+1)=b(k)+2*mu*e*x(k)/((l+1)*sig)

x      = input data
d      = desired signal
b[0:l] = Adaptive coefficients of lth order fir filter
l      = order of filter (> 1)
mu     = Convergence parameter (0.0 to 1.0)
alpha  = Forgetting factor   sig(k)=alpha*(x(k)**2)+(1-alpha)*sig(k-1)
         (>= 0.0 and < 1.0)

returns the filter output
*/

float lms(float x,float d,float *b,int l,
                  float mu,float alpha)
{
    int ll;
    float e,mu_e,lms_const,y;
    static float px[51];      /* max L = 50 */
    static float sigma = 2.0f; /* start at 2 and update internally */

    px[0]=x;

/* calculate filter output */
    y=b[0]*px[0];
#ifdef DEBUG
    printf("l=%d\n",l);
#endif
    _Pragma("loopbound min 20 max 20")
    for(ll = 1 ; ll <= l ; ll++) {
        y=y+b[ll]*px[ll];
    }
/* error signal */
    e=d-y;

/* update sigma */
    sigma=alpha*(px[0]*px[0])+(1-alpha)*sigma;
    mu_e=mu*e/sigma;

/* update coefficients */
    _Pragma("loopbound min 21 max 21")
    for(ll = 0 ; ll <= l ; ll++) {
        b[ll]=b[ll]+mu_e*px[ll];
    }
/* update history */
    _Pragma("loopbound min 20 max 20")
    for(ll = l ; ll >= 1 ; ll--) {
        px[ll]=px[ll-1];
    }

    return(y);
}
