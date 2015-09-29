/*
 *  benchmark program  : fft_16_13_fixed_point
 * 
 *  benchmark suite    : DSP-kernel
 *
 *  description        : benchmarking of an integer stage scaling FFT
 *
 *                      To avoid errors caused by overflow and bit growth, 
 *                      the input data is scaled. Bit growth occurs potentially
 *                      at butterfly operations, which involve a complex 
 *                      multiplication, a complex addition and a complex 
 *                      subtraction. Maximal bit growth from butterfly input 
 *                      to butterfly output is two bits. 
 *
 *                      The input data includes enough extra sign bits, called 
 *                      guard bits, to ensure that bit growth never results in 
 *                      overflow (Rabiner and Gold, 1975). Data can grow by a
 *                      maximum factor of 2.4 from butterfly input to output
 *                      (two bits of grow). However, a data value cannot grow by
 *                      maximum amount in two consecutive stages. 
 *                      The number of guard bits necessary to compensate the 
 *                      maximum bit growth in an N-point FFT is (log_2 (N))+1).
 *  
 *                      In a 16-point FFT (requires 4 stages), each of the 
 *                      input samples should contain 5 guard bits. The input 
 *                      data is then restricted to 10 bits, one sign bit and
 *                      nine magnitude bits, in order to prevent an
 *                      overflow from the integer multiplication with the
 *                      precalculed twiddle coefficients.
 *      
 *                      Another method to compensate bit growth is to scale the
 *                      outputs down by a factor of two unconditionally after
 *                      each stage. This approach is called unconditional scaling
 *                      
 *                      Initially, 2 guard bits are included in the input data to 
 *                      accomodate the maximum overflow in the first stage.
 *                      In each butterfly of a stage calculation, the data can 
 *                      grow into the guard bits. To prevent overflow in the next 
 *                      stage, the guard bits are replaced before the next stage is
 *                      executed by shifting the entire block of data one bit
 *                      to the right.
 * 
 *                      Input data should not be restricted to a 1.9 format.
 *                      Input data can be represented in a 1.13 format,that is
 *                      14 significant bits, one sign and 13 magnitude bits. In 
 *                      the FFT calculation, the data loses a total of (log2 N) -1 
 *                      bits because of shifting. Unconditional scaling results 
 *                      in the same number of bits lost as in the input data scaling.
 *                      However, it produces more precise results because the 
 *                      FFT starts with more precise input data. The tradeoff is
 *                      a slower FFT calculation because of the extra cycles needed
 *                      to shift the output of each stage.
 *
 *                      Input data is held on the include file "input16.dat"
 *                      or "input1024.dat" in float format (0 ... 1)
 *                      Data is transformed automatically to 1.Q fract format
 *
 *  reference code     : none
 *
 *  func. verification : comparison with known float 16 point FFT
 *
 *  organization       : Aachen University of Technology - IS2
 *                     : DSP Tools Group
 *                     : phone   : +49(241)807887
 *                     : fax     : +49(241)8888195
 *                     : e-mail  : zivojnov@ert.rwth-aachen.de
 *
 *  author             : Juan Martinez Velarde
 *
 *  history            : 07-02-94 - creation
 *                       16-02-94 - c50 profiling
 *
 */

#define STORAGE_CLASS register
#define TYPE int

#define N_FFT 16
#define NUMBER_OF_BITS 13    /* fract format 1.NUMBER_OF_BITS = 1.13 */

#define BITS_PER_TWID 13     /* bits per twiddle coefficient */
#define SHIFT BITS_PER_TWID  /* fractional shift after each multiplication */

/* precalculated twiddle factors
   for an integer 16 point FFT
   in format 1.13 => table twidtable[2*(N_FFT-1)] ; */
int twidtable[30] = { 8192,     0,  8192,     0,    0,  -8192,  8192,     0,  5792, -5792,
                         0, -8191, -5792, -5792, 8192,      0,  7568, -3134,  5792, -5792,
                      3134, -7568,     0, -8192, -3134, -7568, -5792, -5792, -7568, -3134 };

/* 16 real values as input data in float format */
float input[16] = { 0.243, 0.323, 0.505, -0.176, -0.87, 0.353, -0.344, -0.443, -0.434, -0.32, 0.232, -0.454, -0.32, -0.323, -0.733, 0.54 };

int inputfract[N_FFT];  /* will hold the transformed data */


float exp2f(float x)
{
  int i;
  float ret = 2.0f;
  _Pragma("loopbound min 13 max 13")
  for ( i = 1; i < x; ++i ) {
    ret *= 2.0f;
  }
  return ret;
}


float modff(float x, float *intpart)
{
  if ( intpart ) {
    *intpart = (int)x;
    return x - *intpart;
  } else
    return x;
}


/* conversion function to 1.NUMBER_OF_BITS format */
int convert(float value)
{
  float man, t_val, frac, m, exponent = NUMBER_OF_BITS;
  int rnd_val;
  unsigned long int_val;
  unsigned long pm_val;

  m = exp2f(exponent+1)  - 1;

  t_val = value * m ;
  frac = modff(t_val,&man);
  if (frac < 0.0f) 
    {
      rnd_val = (-1);
      if (frac > -0.5f) rnd_val = 0;
    }
  else
    {
      rnd_val = 1;
      if (frac < 0.5f) rnd_val = 0;
    }
  int_val = man + rnd_val;
  
  pm_val = int_val ; 
  return ((int) (pm_val)) ; 
  
}


void float2fract()
{
  float f ; 
  int   j, i ; 

  _Pragma("loopbound min 16 max 16")
  for (j = 0 ; j < N_FFT ; j++) 
    {
      f = input[j];
      i = convert(f);
      inputfract[j] = i;
    }
}


void fft_bit_reduct(STORAGE_CLASS TYPE *int_pointer)
{
  {
    STORAGE_CLASS TYPE i, j = 0  ; 
    STORAGE_CLASS TYPE tmpr, max = 2, m, n = N_FFT << 1 ;           ;
     
    /* do the bit reversal scramble of the input data */
    _Pragma("loopbound min 16 max 16")
    for (i = 0; i < (n-1) ; i += 2) 
      {
        if (j > i)
          {
            tmpr = *(int_pointer + j) ;
            *(int_pointer + j) = *(int_pointer + i) ;
            *(int_pointer + i) = tmpr ;

            tmpr = *(int_pointer + j + 1) ; 
            *(int_pointer + j + 1) = *(int_pointer + i + 1) ; 
            *(int_pointer + i + 1) = tmpr ; 
          }

          m = N_FFT;

          _Pragma("loopbound min 0 max 4")
          while (m >= 2 && j >= m) 
            {
              j -= m ;
              m >>= 1;
            }
          j += m ;
      }

    {
      STORAGE_CLASS TYPE *data_pointer = &twidtable[0] ; 
      STORAGE_CLASS TYPE *p, *q ; 
      STORAGE_CLASS TYPE tmpi, fr = 0, level, k, l ; 

      _Pragma("loopbound min 4 max 4")
      while (n > max)
        {
          level = max << 1 ;

          _Pragma("loopbound min 1 max 8")
          for (m = 1; m < max; m += 2) 
            {
              l = *(data_pointer + fr) ; 
              k = *(data_pointer + fr + 1) ;
              fr += 2 ; 

              _Pragma("loopbound min 1 max 8")
              for (i = m; i <= n; i += level) 
                {
                  j = i + max;
                  p = int_pointer + j ; 
                  q = int_pointer + i ; 
                  
                  tmpr  = l * *(p-1) ; 
                  tmpr -= (k * *p ) ; 

                  tmpi  = l * *p  ; 
                  tmpi += (k * *(p-1)) ; 

                  tmpr  = tmpr >> SHIFT ; 
                  tmpi  = tmpi >> SHIFT ; 

                  *(p-1) = *(q-1) - tmpr ; 
                  *p     = *q - tmpi ; 
                  
                  *(q-1) += tmpr ;
                  *q     += tmpi ;
                }
            }

          /* implement unconditional bit reduction */

          {
            register int f;

            p = int_pointer;

            _Pragma("loopbound min 32 max 32")
            for (f = 0 ; f < 2 * N_FFT; f++)
              *p++ = *p >> 1;

          }

          max = level;
        }
    }
  }
}


void pin_down(TYPE input_data[])
{
  /* conversion from input16.dat to a 1.13 format */

  float2fract();

  {
    int *pd, *ps, f;

    pd = &input_data[0];
    ps = &inputfract[0];

    _Pragma("loopbound min 16 max 16")
    for (f = 0; f < N_FFT; f++) 
      {
        *pd++ = *ps++  ; /* fill in with real data */
        *pd++ = 0 ;      /* imaginary data is equal zero */
      }
  }
}


int main()
{
  TYPE input_data[2*N_FFT]; 

  pin_down(&input_data[0]) ; 

  fft_bit_reduct(&input_data[0]);  

  return 0;
}



