/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: jfdctint

  Author: Thomas G. Lane, Public domain JPEG source code.
          Modified by Steven Li at Princeton University.

  Function: JPEG slow-but-accurate integer implementation of the
            forward  DCT (Discrete Cosine Transform) on a 8x8
            pixel block [from original file documentations]

  Source: SNU-RT Benchmark Suite for Worst Case Timing Analysis
          Collected and Modified by S.-S. Lim
          Real-Time Research Group
          Seoul National University

  Changes: Moved initialisation code from jfdctint_main() to jfdctint_init(),
           added checksum calculation in jfdctint_return()

  License: Public Domain

*/


/* Remove the following #define for actual WCET analyses! */
/*
  #define PROFILING
*/
/*
  #ifdef PROFILING
  #include <stdio.h>
  #endif
*/

/*  COMMENTS: Long calculation sequences (i.e., long basic blocks),      */
/*            single-nested loops.                                       */

/**********************************************************************
    Functions to be timed
***********************************************************************/

/* This definitions are added by Steven Li so as to bypass the header
   files.
*/
#define DCT_ISLOW_SUPPORTED
#define DCTSIZE 8
#define BITS_IN_JSAMPLE 8
#define MULTIPLY16C16(var,const)  ((var) * (const))
#define DCTELEM int
#define INT32   int
#define GLOBAL
#define RIGHT_SHIFT(x,shft) ((x) >> (shft))
#define ONE ((INT32) 1)
#define DESCALE(x,n)  RIGHT_SHIFT((x) + (ONE << ((n)-1)), n)
#define SHIFT_TEMPS


/*
   jfdctint.c

   Copyright (C) 1991-1994, Thomas G. Lane.
   This file is part of the Independent JPEG Group's software.
   For conditions of distribution and use, see the accompanying README file.

   This file contains a slow-but-accurate integer implementation of the
   forward DCT (Discrete Cosine Transform).

   A 2-D DCT can be done by 1-D DCT on each row followed by 1-D DCT
   on each column.  Direct algorithms are also available, but they are
   much more complex and seem not to be any faster when reduced to code.

   This implementation is based on an algorithm described in
     C. Loeffler, A. Ligtenberg and G. Moschytz, "Practical Fast 1-D DCT
     Algorithms with 11 Multiplications", Proc. Int'l. Conf. on Acoustics,
     Speech, and Signal Processing 1989 (ICASSP '89), pp. 988-991.
   The primary algorithm described there uses 11 multiplies and 29 adds.
   We use their alternate method with 12 multiplies and 32 adds.
   The advantage of this method is that no data path contains more than one
   multiplication; this allows a very simple and accurate implementation in
   scaled fixed-point arithmetic, with a minimal number of shifts.
*/

#define JPEG_INTERNALS

#ifdef DCT_ISLOW_SUPPORTED


/*
   This module is specialized to the case DCTSIZE = 8.
*/

#if DCTSIZE != 8
Sorry, this code only copes with 8x8 DCTs. /* deliberate syntax err */
#endif


/*
   The poop on this scaling stuff is as follows:

   Each 1-D DCT step produces outputs which are a factor of sqrt(N)
   larger than the true DCT outputs.  The final outputs are therefore
   a factor of N larger than desired; since N=8 this can be cured by
   a simple right shift at the end of the algorithm.  The advantage of
   this arrangement is that we save two multiplications per 1-D DCT,
   because the y0 and y4 outputs need not be divided by sqrt(N).
   In the IJG code, this factor of 8 is removed by the quantization step
   (in jcdctmgr.c), NOT in this module.

   We have to do addition and subtraction of the integer inputs, which
   is no problem, and multiplication by fractional constants, which is
   a problem to do in integer arithmetic.  We multiply all the constants
   by CONST_SCALE and convert them to integer constants (thus retaining
   CONST_BITS bits of precision in the constants).  After doing a
   multiplication we have to divide the product by CONST_SCALE, with proper
   rounding, to produce the correct output.  This division can be done
   cheaply as a right shift of CONST_BITS bits.  We postpone shifting
   as long as possible so that partial sums can be added together with
   full fractional precision.

   The outputs of the first pass are scaled up by PASS1_BITS bits so that
   they are represented to better-than-integral precision.  These outputs
   require BITS_IN_JSAMPLE + PASS1_BITS + 3 bits; this fits in a 16-bit word
   with the recommended scaling.  (For 12-bit sample data, the intermediate
   array is INT32 anyway.)

   To avoid overflow of the 32-bit intermediate results in pass 2, we must
   have BITS_IN_JSAMPLE + CONST_BITS + PASS1_BITS <= 26.  Error analysis
   shows that the values given below are the most effective.
*/

/*
  Forward declaration of functions
*/

void jfdctint_init();
int jfdctint_return();
void jfdctint_main();
int main( void );


#if BITS_IN_JSAMPLE == 8
#define CONST_BITS  13
#define PASS1_BITS  2
#else
#define CONST_BITS  13
#define PASS1_BITS  1   /* lose a little precision to avoid overflow */
#endif

/* Some C compilers fail to reduce "FIX(constant)" at compile time, thus
   causing a lot of useless floating-point operations at run time.
   To get around this we use the following pre-calculated constants.
   If you change CONST_BITS you may want to add appropriate values.
   (With a reasonable C compiler, you can just rely on the FIX() macro...)
*/

#if CONST_BITS == 13
#define FIX_0_298631336  ((INT32)  2446)  /* FIX(0.298631336) */
#define FIX_0_390180644  ((INT32)  3196)  /* FIX(0.390180644) */
#define FIX_0_541196100  ((INT32)  4433)  /* FIX(0.541196100) */
#define FIX_0_765366865  ((INT32)  6270)  /* FIX(0.765366865) */
#define FIX_0_899976223  ((INT32)  7373)  /* FIX(0.899976223) */
#define FIX_1_175875602  ((INT32)  9633)  /* FIX(1.175875602) */
#define FIX_1_501321110  ((INT32)  12299) /* FIX(1.501321110) */
#define FIX_1_847759065  ((INT32)  15137) /* FIX(1.847759065) */
#define FIX_1_961570560  ((INT32)  16069) /* FIX(1.961570560) */
#define FIX_2_053119869  ((INT32)  16819) /* FIX(2.053119869) */
#define FIX_2_562915447  ((INT32)  20995) /* FIX(2.562915447) */
#define FIX_3_072711026  ((INT32)  25172) /* FIX(3.072711026) */
#else
#define FIX_0_298631336  FIX(0.298631336)
#define FIX_0_390180644  FIX(0.390180644)
#define FIX_0_541196100  FIX(0.541196100)
#define FIX_0_765366865  FIX(0.765366865)
#define FIX_0_899976223  FIX(0.899976223)
#define FIX_1_175875602  FIX(1.175875602)
#define FIX_1_501321110  FIX(1.501321110)
#define FIX_1_847759065  FIX(1.847759065)
#define FIX_1_961570560  FIX(1.961570560)
#define FIX_2_053119869  FIX(2.053119869)
#define FIX_2_562915447  FIX(2.562915447)
#define FIX_3_072711026  FIX(3.072711026)
#endif


/* Multiply an INT32 variable by an INT32 constant to yield an INT32 result.
   For 8-bit samples with the recommended scaling, all the variable
   and constant values involved are no more than 16 bits wide, so a
   16x16->32 bit multiply can be used instead of a full 32x32 multiply.
   For 12-bit samples, a full 32-bit multiplication will be needed.
*/

#if BITS_IN_JSAMPLE == 8
#define MULTIPLY(var,const)  MULTIPLY16C16(var,const)
#else
#define MULTIPLY(var,const)  ((var) * (const))
#endif


DCTELEM jfdctint_data[ 64 ];


void jfdctint_init()
{
  #ifdef PROFILING
  /* Profiling variables. Remove for actual WCET analyses. */
  int iters_i = 0, min_i = 100000, max_i = 0;
  #endif

  int i, seed;

  /* Worst case settings */
  /* Set array to random values */
  seed = 1;

  #ifdef PROFILING
  iters_i = 0;
  #endif
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < 64; i++ ) {
    #ifdef PROFILING
    iters_i++;
    #endif

    seed = ( ( seed * 133 ) + 81 ) % 65535;
    jfdctint_data[ i ] = seed;
  }

  #ifdef PROFILING
  if ( iters_i < min_i )
    min_i = iters_i;
  if ( iters_i > max_i )
    max_i = iters_i;
  #endif

  #ifdef PROFILING
  printf( "i-loop: [%d, %d]\n", min_i, max_i );
  #endif
}


int jfdctint_return()
{
  int checksum = 0;
  int i;
  for ( i = 0; i < 64; ++i )
    checksum += jfdctint_data[ i ];
  return checksum;
}


/*
   Perform the forward DCT on one block of samples.
*/

GLOBAL void jfdctint_jpeg_fdct_islow( void )
{
  #ifdef PROFILING
  /* Profiling variables. Remove for actual WCET analyses. */
  int iters_ctr1 = 0, min_ctr1 = 100000, max_ctr1 = 0;
  int iters_ctr2 = 0, min_ctr2 = 100000, max_ctr2 = 0;
  #endif

  INT32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
  INT32 tmp10, tmp11, tmp12, tmp13;
  INT32 z1, z2, z3, z4, z5;
  DCTELEM * dataptr;
  int ctr;
  SHIFT_TEMPS

  /* Pass 1: process rows. */
  /* Note results are scaled up by sqrt(8) compared to a true DCT; */
  /* furthermore, we scale the results by 2**PASS1_BITS. */

  dataptr = jfdctint_data;
  #ifdef PROFILING
  iters_ctr1 = 0;
  #endif
  _Pragma( "loopbound min 8 max 8" )
  for ( ctr = DCTSIZE - 1; ctr >= 0; ctr-- ) {
    #ifdef PROFILING
    iters_ctr1++;
    #endif

    tmp0 = dataptr[ 0 ] + dataptr[ 7 ];
    tmp7 = dataptr[ 0 ] - dataptr[ 7 ];
    tmp1 = dataptr[ 1 ] + dataptr[ 6 ];
    tmp6 = dataptr[ 1 ] - dataptr[ 6 ];
    tmp2 = dataptr[ 2 ] + dataptr[ 5 ];
    tmp5 = dataptr[ 2 ] - dataptr[ 5 ];
    tmp3 = dataptr[ 3 ] + dataptr[ 4 ];
    tmp4 = dataptr[ 3 ] - dataptr[ 4 ];

    tmp10 = tmp0 + tmp3;
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;

    dataptr[ 0 ] = ( DCTELEM ) ( ( tmp10 + tmp11 ) << PASS1_BITS );
    dataptr[ 4 ] = ( DCTELEM ) ( ( tmp10 - tmp11 ) << PASS1_BITS );

    z1 = MULTIPLY( tmp12 + tmp13, FIX_0_541196100 );
    dataptr[ 2 ] = ( DCTELEM ) DESCALE( z1
                                        + MULTIPLY( tmp13, FIX_0_765366865 ),
                                        CONST_BITS - PASS1_BITS );
    dataptr[ 6 ] = ( DCTELEM ) DESCALE( z1
                                        + MULTIPLY( tmp12, - FIX_1_847759065 ),
                                        CONST_BITS - PASS1_BITS );

    z1 = tmp4 + tmp7;
    z2 = tmp5 + tmp6;
    z3 = tmp4 + tmp6;
    z4 = tmp5 + tmp7;
    z5 = MULTIPLY( z3 + z4, FIX_1_175875602 ); /* sqrt(2) * c3 */

    tmp4 = MULTIPLY( tmp4, FIX_0_298631336 ); /* sqrt(2) * (-c1+c3+c5-c7) */
    tmp5 = MULTIPLY( tmp5, FIX_2_053119869 ); /* sqrt(2) * ( c1+c3-c5+c7) */
    tmp6 = MULTIPLY( tmp6, FIX_3_072711026 ); /* sqrt(2) * ( c1+c3+c5-c7) */
    tmp7 = MULTIPLY( tmp7, FIX_1_501321110 ); /* sqrt(2) * ( c1+c3-c5-c7) */
    z1 = MULTIPLY( z1, - FIX_0_899976223 ); /* sqrt(2) * (c7-c3) */
    z2 = MULTIPLY( z2, - FIX_2_562915447 ); /* sqrt(2) * (-c1-c3) */
    z3 = MULTIPLY( z3, - FIX_1_961570560 ); /* sqrt(2) * (-c3-c5) */
    z4 = MULTIPLY( z4, - FIX_0_390180644 ); /* sqrt(2) * (c5-c3) */

    z3 += z5;
    z4 += z5;

    dataptr[ 7 ] = ( DCTELEM ) DESCALE( tmp4 + z1 + z3,
                                        CONST_BITS - PASS1_BITS );
    dataptr[ 5 ] = ( DCTELEM ) DESCALE( tmp5 + z2 + z4,
                                        CONST_BITS - PASS1_BITS );
    dataptr[ 3 ] = ( DCTELEM ) DESCALE( tmp6 + z2 + z3,
                                        CONST_BITS - PASS1_BITS );
    dataptr[ 1 ] = ( DCTELEM ) DESCALE( tmp7 + z1 + z4, CONST_BITS - PASS1_BITS );

    dataptr += DCTSIZE;   /* advance pointer to next row */
  }

  #ifdef PROFILING
  if ( iters_ctr1 < min_ctr1 )
    min_ctr1 = iters_ctr1;
  if ( iters_ctr1 > max_ctr1 )
    max_ctr1 = iters_ctr1;
  #endif

  dataptr = jfdctint_data;
  #ifdef PROFILING
  iters_ctr2 = 0;
  #endif
  _Pragma( "loopbound min 8 max 8" )
  for ( ctr = DCTSIZE - 1; ctr >= 0; ctr-- ) {
    #ifdef PROFILING
    iters_ctr2++;
    #endif

    tmp0 = dataptr[ DCTSIZE * 0 ] + dataptr[ DCTSIZE * 7 ];
    tmp7 = dataptr[ DCTSIZE * 0 ] - dataptr[ DCTSIZE * 7 ];
    tmp1 = dataptr[ DCTSIZE * 1 ] + dataptr[ DCTSIZE * 6 ];
    tmp6 = dataptr[ DCTSIZE * 1 ] - dataptr[ DCTSIZE * 6 ];
    tmp2 = dataptr[ DCTSIZE * 2 ] + dataptr[ DCTSIZE * 5 ];
    tmp5 = dataptr[ DCTSIZE * 2 ] - dataptr[ DCTSIZE * 5 ];
    tmp3 = dataptr[ DCTSIZE * 3 ] + dataptr[ DCTSIZE * 4 ];
    tmp4 = dataptr[ DCTSIZE * 3 ] - dataptr[ DCTSIZE * 4 ];

    tmp10 = tmp0 + tmp3;
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;

    dataptr[ DCTSIZE * 0 ] = ( DCTELEM ) DESCALE( tmp10 + tmp11, PASS1_BITS );
    dataptr[ DCTSIZE * 4 ] = ( DCTELEM ) DESCALE( tmp10 - tmp11, PASS1_BITS );

    z1 = MULTIPLY( tmp12 + tmp13, FIX_0_541196100 );
    dataptr[ DCTSIZE * 2 ] = ( DCTELEM ) DESCALE( z1
                             + MULTIPLY( tmp13,
                                         FIX_0_765366865 ),
                             CONST_BITS + PASS1_BITS );
    dataptr[ DCTSIZE * 6 ] = ( DCTELEM ) DESCALE( z1
                             + MULTIPLY( tmp12,
                                         - FIX_1_847759065 ),
                             CONST_BITS + PASS1_BITS );

    z1 = tmp4 + tmp7;
    z2 = tmp5 + tmp6;
    z3 = tmp4 + tmp6;
    z4 = tmp5 + tmp7;
    z5 = MULTIPLY( z3 + z4, FIX_1_175875602 ); /* sqrt(2) * c3 */

    tmp4 = MULTIPLY( tmp4, FIX_0_298631336 ); /* sqrt(2) * (-c1+c3+c5-c7) */
    tmp5 = MULTIPLY( tmp5, FIX_2_053119869 ); /* sqrt(2) * ( c1+c3-c5+c7) */
    tmp6 = MULTIPLY( tmp6, FIX_3_072711026 ); /* sqrt(2) * ( c1+c3+c5-c7) */
    tmp7 = MULTIPLY( tmp7, FIX_1_501321110 ); /* sqrt(2) * ( c1+c3-c5-c7) */
    z1 = MULTIPLY( z1, - FIX_0_899976223 ); /* sqrt(2) * (c7-c3) */
    z2 = MULTIPLY( z2, - FIX_2_562915447 ); /* sqrt(2) * (-c1-c3) */
    z3 = MULTIPLY( z3, - FIX_1_961570560 ); /* sqrt(2) * (-c3-c5) */
    z4 = MULTIPLY( z4, - FIX_0_390180644 ); /* sqrt(2) * (c5-c3) */

    z3 += z5;
    z4 += z5;

    dataptr[ DCTSIZE * 7 ] = ( DCTELEM ) DESCALE( tmp4 + z1 + z3,
                             CONST_BITS + PASS1_BITS );
    dataptr[ DCTSIZE * 5 ] = ( DCTELEM ) DESCALE( tmp5 + z2 + z4,
                             CONST_BITS + PASS1_BITS );
    dataptr[ DCTSIZE * 3 ] = ( DCTELEM ) DESCALE( tmp6 + z2 + z3,
                             CONST_BITS + PASS1_BITS );
    dataptr[ DCTSIZE * 1 ] = ( DCTELEM ) DESCALE( tmp7 + z1 + z4,
                             CONST_BITS + PASS1_BITS );

    dataptr++;      /* advance pointer to next column */
  }

  #ifdef PROFILING
  if ( iters_ctr2 < min_ctr2 )
    min_ctr2 = iters_ctr2;
  if ( iters_ctr2 > max_ctr2 )
    max_ctr2 = iters_ctr2;
  #endif

  #ifdef PROFILING
  printf( "ctr1-loop: [%d, %d]\n", min_ctr1, max_ctr1 );
  printf( "ctr2-loop: [%d, %d]\n", min_ctr2, max_ctr2 );
  #endif
}

#endif /* DCT_ISLOW_SUPPORTED */

/* Main function
   Time to function execution time using logic analyzer,
   which measures the OFF time of a LED on board.

   The switching latency, including the function call/return time,
   is measured to be equal to 1.1us (22 clock cycles).
*/
int _Pragma ( "entrypoint" ) jfdctint_main( void )
{
  jfdctint_jpeg_fdct_islow();

  return 0;
}


int main( void )
{
  jfdctint_init();
  jfdctint_main();

  return ( jfdctint_return() );
}
