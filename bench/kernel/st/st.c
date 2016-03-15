/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: st

  Author: unknown

  Function: st is a statistics program.
    This program computes for two arrays of numbers the sum, the mean,
    the variance, and standard deviation. It then determines the
    correlation coefficient between the two arrays.

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/st/st.c

  Changes: No major functional changes.

  License: May be used, modified, and re-distributed freely

*/


/*
  Forward declaration of functions
*/

void st_initSeed( void );
long st_randomInteger();
void st_initialize( float * );
void st_init( void );
int st_return( void );
float st_fabs( float );
float st_sqrtf( float );
float st_square( float );
void st_calc_Sum_Mean( float *, float *, float * );
void st_calc_Var_Stddev( float *, float, float *, float * );
void st_calc_LinCorrCoef( float *, float *, float, float, float * );
void st_main( void );
int main( void );


/*
  Declaration of global variables
*/

volatile int st_seed;
float st_arrayA[ 1000 ], st_arrayB[ 1000 ];
float st_sumA, st_sumB;
float st_meanA, st_meanB, st_varA, st_varB, st_stddevA, st_stddevB, st_coef;


/*
  Initialization- and return-value-related functions
*/

/*
  st_initSeed initializes the seed used in the "random" number generator.
*/
void st_initSeed()
{
  st_seed = 0;
}


/*
  st_RandomInteger generates random integers between 0 and 8094.
*/
long st_randomInteger()
{
  st_seed = ( ( st_seed * 133 ) + 81 ) % 8095;
  return ( st_seed );
}


void st_initialize( float *array )
{
  register int i;

  _Pragma( "loopbound min 1000 max 1000" )
  for ( i = 0; i < 1000; i++ )
    array[ i ] = i + st_randomInteger();
}


void st_init()
{
  st_initSeed();
  st_initialize( st_arrayA );
  st_initialize( st_arrayB );
}


int st_return()
{
  float checksum = st_meanA + st_meanB + st_stddevA + st_stddevB + st_coef;
  /* allow rounding errors for the checksum */
  checksum -= 13695.986328;
  return ( ( checksum < 0.000001 && checksum > -0.000001 ) ? 0 : -1 );
}


/*
  Arithmetic math functions
*/

float st_fabs( float n )
{
  float f;

  if ( n >= 0 )
    f = n;
  else
    f = -n;

  return ( f );
}


float st_sqrtf( float val )
{
  float x = val / 10;
  float dx;
  float diff;
  float min_tol = 0.00001f;

  int i, flag = 0;

  if ( val == 0 )
    x = 0;
  else {
    _Pragma( "loopbound min 19 max 19" )
    for ( i = 1; i < 20; i++ ) {
      if ( !flag ) {
        dx = ( val - ( x * x ) ) / ( 2.0f * x );
        x = x + dx;
        diff = val - ( x * x );

        if ( st_fabs( diff ) <= min_tol )
          flag = 1;
      }
    }
  }

  return ( x );
}


float st_square( float x )
{
  return ( x * x );
}


/*
  Algorithm core functions
*/

void st_calc_Sum_Mean( float *array, float *sum, float *mean )
{
  int i;

  *sum = 0;

  _Pragma( "loopbound min 1000 max 1000" )
  for ( i = 0; i < 1000; i++ )
    *sum += array[ i ];
  *mean = *sum / 1000;
}


void st_calc_Var_Stddev( float *array, float mean, float *var, float *stddev )
{
  int i;
  float diffs = 0.0f;

  _Pragma( "loopbound min 1000 max 1000" )
  for ( i = 0; i < 1000; i++ )
    diffs += st_square( array[ i ] - mean );

  *var = diffs / 1000;
  *stddev = st_sqrtf( *var );
}


void st_calc_LinCorrCoef( float *arrayA, float *arrayB, float meanA,
                          float meanB, float *coef )
{
  int i;
  float numerator = 0.0f, Aterm = 0.0f, Bterm = 0.0f;

  _Pragma( "loopbound min 1000 max 1000" )
  for ( i = 0; i < 1000; i++ ) {
    numerator += ( arrayA[ i ] - meanA ) * ( arrayB[ i ] - meanB );
    Aterm += st_square( arrayA[ i ] - meanA );
    Bterm += st_square( arrayB[ i ] - meanB );
  }

  *coef = numerator / ( st_sqrtf( Aterm ) * st_sqrtf( Bterm ) );
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) st_main( void )
{
  st_calc_Sum_Mean( st_arrayA, &st_sumA, &st_meanA );
  st_calc_Var_Stddev( st_arrayA, st_meanA, &st_varA, &st_stddevA );

  st_calc_Sum_Mean( st_arrayB, &st_sumB, &st_meanB );
  st_calc_Var_Stddev( st_arrayB, st_meanB, &st_varB, &st_stddevB );

  st_calc_LinCorrCoef( st_arrayA, st_arrayB, st_meanA, st_meanB, &st_coef );
}


int main( void )
{
  st_init();
  st_main();

  return ( st_return() );
}
