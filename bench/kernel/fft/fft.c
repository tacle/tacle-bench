/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: fft

  Author: Juan Martinez Velarde

  Function: benchmarking of an integer stage scaling FFT
    To avoid errors caused by overflow and bit growth,
    the input data is scaled. Bit growth occurs potentially
    at butterfly operations, which involve a complex
    multiplication, a complex addition and a complex
    subtraction. Maximal bit growth from butterfly input
    to butterfly output is two bits.

    The input data includes enough extra sign bits, called
    guard bits, to ensure that bit growth never results in
    overflow (Rabiner and Gold, 1975). Data can grow by a
    maximum factor of 2.4 from butterfly input to output
    (two bits of grow). However, a data value cannot grow by
    maximum amount in two consecutive stages.
    The number of guard bits necessary to compensate the
    maximum bit growth in an N-point FFT is (log_2 (N))+1).

    In a 16-point FFT (requires 4 stages), each of the
    input samples should contain 5 guard bits. The input
    data is then restricted to 10 bits, one sign bit and
    nine magnitude bits, in order to prevent an
    overflow from the integer multiplication with the
    precalculed twiddle coefficients.

    Another method to compensate bit growth is to scale the
    outputs down by a factor of two unconditionally after
    each stage. This approach is called unconditional scaling

    Initially, 2 guard bits are included in the input data to
    accomodate the maximum overflow in the first stage.
    In each butterfly of a stage calculation, the data can
    grow into the guard bits. To prevent overflow in the next
    stage, the guard bits are replaced before the next stage is
    executed by shifting the entire block of data one bit
    to the right.

    Input data should not be restricted to a 1.9 format.
    Input data can be represented in a 1.13 format,that is
    14 significant bits, one sign and 13 magnitude bits. In
    the FFT calculation, the data loses a total of (log2 N) -1
    bits because of shifting. Unconditional scaling results
    in the same number of bits lost as in the input data scaling.
    However, it produces more precise results because the
    FFT starts with more precise input data. The tradeoff is
    a slower FFT calculation because of the extra cycles needed
    to shift the output of each stage.

  Source: DSP-Stone
    http://www.ice.rwth-aachen.de/research/tools-projects/entry/detail/dspstone/

  Original name: fft_1024_13
    (merged main1024_bit_reduct and fft_bit_reduct from DSP-Stone)

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

*/

#define N_FFT 1024
#define NUMBER_OF_BITS 13    /* fract format 1.NUMBER_OF_BITS = 1.13 */

#define BITS_PER_TWID 13     /* bits per twiddle coefficient */
#define SHIFT BITS_PER_TWID  /* fractional shift after each multiplication */

/*
  Forward declaration of functions
*/

float fft_exp2f( float x );
float fft_modff( float x, float *intpart );
int fft_convert( float value );
void fft_bit_reduct( register int *int_pointer );
void fft_pin_down( int input_data[  ] );
void fft_init( void );
void fft_main( void );
int fft_return( void );
int main( void );

/*
  Forward declaration of global variables
*/

int fft_input_data[ 2 * N_FFT ];

/* precalculated twiddle factors
   for an integer 1024 point FFT
   in format 1.13 => table twidtable[ 2*(N_FFT-1) ] ; */
extern int fft_twidtable[ 2046 ];

/* 1024 real values as input data in float format */
extern float fft_input[ 1024 ];

/* will hold the transformed data */
int fft_inputfract[ N_FFT ];


/*
  Algorithm core function
*/

void fft_bit_reduct( register int *int_pointer )
{
  register int i, j = 0  ;
  register int tmpr, max = 2, m, n = N_FFT << 1 ;

  /* do the bit reversal scramble of the input data */
  _Pragma( "loopbound min 1024 max 1024" )
  for ( i = 0; i < ( n - 1 ) ; i += 2 ) {
    if ( j > i ) {
      tmpr = *( int_pointer + j ) ;
      *( int_pointer + j ) = *( int_pointer + i ) ;
      *( int_pointer + i ) = tmpr ;

      tmpr = *( int_pointer + j + 1 ) ;
      *( int_pointer + j + 1 ) = *( int_pointer + i + 1 ) ;
      *( int_pointer + i + 1 ) = tmpr ;
    }

    m = N_FFT;

    _Pragma( "loopbound min 0 max 10" )
    while ( m >= 2 && j >= m ) {
      j -= m ;
      m >>= 1;
    }
    j += m ;
  }

  {
    register int *data_pointer = &fft_twidtable[ 0 ] ;
    register int *p, *q ;
    register int tmpi, fr = 0, level, k, l ;

    _Pragma( "loopbound min 10 max 10" )
    while ( n > max ) {
      level = max << 1;

      _Pragma( "loopbound min 1 max 512" ) 
      for ( m = 1; m < max; m += 2 ) {
        l = *( data_pointer + fr );
        k = *( data_pointer + fr + 1 ) ;
        fr += 2 ;

        _Pragma( "loopbound min 1 max 512" )
        for ( i = m; i <= n; i += level ) {
          j = i + max;
          p = int_pointer + j;
          q = int_pointer + i;

          tmpr  = l * *( p - 1 );
          tmpr -= ( k * *p );

          tmpi  = l * *p;
          tmpi += ( k * *( p - 1 ) );

          tmpr  = tmpr >> SHIFT ;
          tmpi  = tmpi >> SHIFT ;

          *( p - 1 ) = *( q - 1 ) - tmpr ;
          *p     = *q - tmpi ;

          *( q - 1 ) += tmpr ;
          *q     += tmpi ;
        }
      }

      /* implement unconditional bit reduction */

      {
        register int f;

        p = int_pointer;

        _Pragma( "loopbound min 2048 max 2048" )
        for ( f = 0 ; f < 2 * N_FFT; f++ ) {
          *p = *p >> 1;
          p++;
        }
      }

      max = level;
    }
  }
}


/*
  Initialization- and return-value-related functions
*/

/* conversion function to 1.NUMBER_OF_BITS format */
float fft_exp2f( float x )
{
  int i;
  float ret = 2.0f;

  _Pragma( "loopbound min 13 max 13" )
  for ( i = 1; i < x; ++i )
    ret *= 2.0f;

  return ret;
}


float fft_modff( float x, float *intpart )
{
  if ( intpart ) {
    *intpart = ( int )x;
    return x - *intpart;
  } else
    return x;
}


/* conversion function to 1.NUMBER_OF_BITS format */
int fft_convert( float value )
{
  float man, t_val, frac, m, exponent = NUMBER_OF_BITS;
  int rnd_val;
  unsigned long int_val;
  unsigned long pm_val;

  m = fft_exp2f( exponent + 1 )  - 1;

  t_val = value * m ;
  frac = fft_modff( t_val, &man );
  if ( frac < 0.0f ) {
    rnd_val = ( -1 );
    if ( frac > -0.5f ) rnd_val = 0;
  } else {
    rnd_val = 1;
    if ( frac < 0.5f ) rnd_val = 0;
  }
  int_val = (long)man + (long)rnd_val;

  pm_val = int_val ;
  return ( ( int ) ( pm_val ) ) ;
}


void fft_float2fract( void )
{
  float f ;
  int   j, i ;

  _Pragma( "loopbound min 1024 max 1024" )
  for ( j = 0 ; j < N_FFT ; j++ ) {
    f = fft_input[ j ];
    i = fft_convert( f );
    fft_inputfract[ j ] = i;
  }
}


void fft_pin_down( int input_data[  ] )
{
  /* conversion from input to a 1.13 format */
  fft_float2fract() ;

  int *pd, *ps, f;

  pd = &input_data[ 0 ];
  ps = &fft_inputfract[ 0 ];

  _Pragma( "loopbound min 1024 max 1024" )
  for ( f = 0; f < N_FFT; f++ ) {
    *pd++ = *ps++  ; /* fill in with real data */
    *pd++ = 0 ;      /* imaginary data is equal zero */
  }
}


void fft_init( void )
{
  int i;
  volatile int x = 0;

  fft_pin_down( &fft_input_data[ 0 ] );

  /* avoid constant propagation of input values */
  _Pragma( "loopbound min 2046 max 2046" ) 
  for ( i = 0; i < 2 * ( N_FFT - 1 ); i++ ) {
    fft_input_data[ i ] += x;
    fft_twidtable[ i ] += x;
  }
  _Pragma( "loopbound min 2 max 2" )
  for ( ; i < 2 * N_FFT; i++ )
    fft_input_data[ i ] += x;

}


int fft_return( void )
{
  int check_sum = 0;
  int i = 0;

  _Pragma( "loopbound min 2048 max 2048" )
  for ( i = 0; i < 2 * N_FFT; ++i )
    check_sum += fft_input_data[ i ];

  return check_sum != 3968;
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) fft_main( void )
{
  fft_bit_reduct( &fft_input_data[ 0 ] );
}


int main( void )
{
  fft_init();

  fft_main();

  return fft_return();
}
