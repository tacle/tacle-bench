/* stats.c */

#define MAX 1000


/* Statistics Program:
 * This program computes for two arrays of numbers the sum, the
 * mean, the variance, and standard deviation.  It then determines the
 * correlation coefficient between the two arrays.
 */

int Seed;
float ArrayA[MAX], ArrayB[MAX];
float SumA, SumB;
float Coef;

float my_fabs(float n)
{
  float f;

  if (n >= 0) f = n;
  else f = -n;
  return f;
}


float my_sqrtf(float val)
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

	  if (my_fabs(diff) <= min_tol) flag = 1;
	}
	else
	  x =x;

      }
  }
  return (x);
}


void InitSeed ()
/*
 * Initializes the seed used in the random number generator.
 */
{
   Seed = 0;
}


void Calc_Sum_Mean(float *Array, float *Sum,  float *Mean)
{
   int i;

   *Sum = 0;
   _Pragma("loopbound min 1000 max 1000")
   for (i = 0; i < MAX; i++) {
      *Sum += Array[i];
   }
   *Mean = *Sum / MAX;
}


float Square(float x)
{
   return x*x;
}


void Calc_Var_Stddev(float *Array, float Mean, float *Var, float *Stddev)
{
   int i;
   float diffs;

   diffs = 0.0f;
   _Pragma("loopbound min 1000 max 1000")
   for (i = 0; i < MAX; i++) {
      diffs += Square(Array[i] - Mean);
   }
   *Var = diffs/MAX;
   *Stddev = my_sqrtf(*Var);
}


void Calc_LinCorrCoef(float *ArrayA, float *ArrayB, float MeanA, float MeanB /*, Coef*/)
{
   int i;
   float numerator, Aterm, Bterm;

   numerator = 0.0f;
   Aterm = Bterm = 0.0f;
   _Pragma("loopbound min 1000 max 1000")
   for (i = 0; i < MAX; i++) {
      numerator += (ArrayA[i] - MeanA) * (ArrayB[i] - MeanB);
      Aterm += Square(ArrayA[i] - MeanA);
      Bterm += Square(ArrayB[i] - MeanB);
   }

   /* Coef used globally */
   Coef = numerator / (my_sqrtf(Aterm) * my_sqrtf(Bterm));
}


long RandomInteger()
/*
 * Generates random integers between 0 and 8095
 */
{
   Seed = ((Seed * 133) + 81) % 8095;
   return (Seed);
}




void Initialize(float *Array)
{
  register int i;

  _Pragma("loopbound min 1000 max 1000")
  for (i=0; i < MAX; i++)
   Array [i] = i + RandomInteger ()/8095.0f;
}

int main (void)
{
   float MeanA, MeanB, VarA, VarB, StddevA, StddevB /*, Coef*/;

   InitSeed ();

   Initialize(ArrayA);
   Calc_Sum_Mean(ArrayA, &SumA, &MeanA);
   Calc_Var_Stddev(ArrayA, MeanA, &VarA, &StddevA);

   Initialize(ArrayB);
   Calc_Sum_Mean(ArrayB, &SumB, &MeanB);
   Calc_Var_Stddev(ArrayB, MeanB, &VarB, &StddevB);

   /* Coef will have to be used globally in Calc_LinCorrCoef since it would
      be beyond the 6 registers used for passing parameters
   */
   Calc_LinCorrCoef(ArrayA, ArrayB, MeanA, MeanB /*, &Coef*/);

   return 0;
}
