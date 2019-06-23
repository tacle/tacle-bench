/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: lms

  Author: Jörg Mische

  Function: LMS adaptive signal enhancement

  Source: Completely rewritten for TACLeBench to avoid license issues.
          It has the same functionality as lms.c from the book
          "C Algorithms for Real-Time DSP" by Paul M. Embree, which
          has been used in WCET benchmarking for many years.

  Original name: lms.c

  Changes: Simplified generation of the input (noisy sinus wave).
           No static variables.

  License: ISC (simplified BSD)

*/

/*
  Copyright (c) 2016 Jörg Mische

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
  OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/


#define N 201
#define L 20
#define SAMPLING 5


float lms_input[ N + 1 ], lms_output[ N + 1 ];


/* The following table can be calculated by
   for (i=0; i<=SAMPLING; i++)
     lms_sintab[ k ] = sqrt(2.0) * sin(PI * i / (2*SAMPLING));
*/
double lms_sintab[ SAMPLING + 1 ] = {
  0.00000000000000000,
  0.43701603620715901,
  0.83125389555938600,
  1.14412282743652560,
  1.34499703920997637,
  1.41421356237309381,
};


double lms_sinus( int i )
{
  int s = i % ( 4 * SAMPLING );
  if ( s >= ( 2 * SAMPLING ) )
    return -lms_sintab[ ( s > 3 * SAMPLING ) ?
                                            ( 4 * SAMPLING - s ) : ( s - 2 * SAMPLING ) ];
  return lms_sintab[ ( s > SAMPLING ) ? ( 2 * SAMPLING - s ) : s ];
}


void lms_init( void )
{
  unsigned long seed = 1;
  int k;

  lms_input[ 0 ] = 0.0;
  {
    double v1, v2, r;
    const double scaleFactor = 0.000000000931322574615478515625;
    do {
      // generate two random numbers between -1.0 and +1.0
      seed = seed * 1103515245 + 12345;
      v1 = ( seed & 0x00007fffffff ) * scaleFactor - 1.0;
      seed = seed * 1103515245 + 12345;
      v2 = ( seed & 0x00007fffffff ) * scaleFactor - 1.0;
      r = v1 * v1 + v2 * v2;
    } while ( r > 1.0 );
    // radius < 1

    // remap v1 and v2 to two Gaussian numbers
    double noise = 1 / r; // approximation of sqrt(0.96) * sqrt(-log(r)/r);
    lms_input[1] = lms_sinus(1) + noise * v2;
  }

  _Pragma( "loopbound min 100 max 100" )
  for ( k = 2 ; k < N ; k += 2 ) {
    double v1, v2, r;
    const double scaleFactor = 0.000000000931322574615478515625;
    do {
      // generate two random numbers between -1.0 and +1.0
      seed = seed * 1103515245 + 12345;
      v1 = ( seed & 0x00007fffffff ) * scaleFactor - 1.0;
      seed = seed * 1103515245 + 12345;
      v2 = ( seed & 0x00007fffffff ) * scaleFactor - 1.0;
      r = v1 * v1 + v2 * v2;
    } while ( r > 1.0 );
    // radius < 1

    // remap v1 and v2 to two Gaussian numbers
    double noise = 1 / r; // approximation of sqrt(0.96) * sqrt(-log(r)/r);
    lms_input[ k ] = lms_sinus(k) + noise * v2;
    lms_input[ k + 1 ] = lms_sinus(k + 1) + noise * v1;
  }

}


float lms_calc( float x,
                float d,
                float b[  ],
                int l,
                float mu,
                float alpha,
                float history[  ],
                float *sigma )
{
  int i;

  // shift history
  _Pragma( "loopbound min 20 max 20" )
  for ( i = l ; i >= 1 ; i-- )
    history[ i ] = history[ i - 1 ];
  history[ 0 ] = x;

  // calculate filter
  float y = 0.0;
  *sigma = alpha * x * x + ( 1 - alpha ) * ( *sigma );

  _Pragma( "loopbound min 21 max 21" )
  for ( i = 0 ; i <= l ; i++ )
    y += b[ i ] * history[ i ];

  // update coefficients
  float e = mu * ( d - y ) / ( *sigma );

  _Pragma( "loopbound min 21 max 21" )
  for ( i = 0 ; i <= l ; i++ )
    b[ i ] += e * history[ i ];

  return y;
}


void _Pragma( "entrypoint" ) lms_main( void )
{
  int i;
  float b[ L + 1 ];
  float history[ L + 1 ];
  float sigma = 2.0;

  _Pragma( "loopbound min 21 max 21" )
  for ( i = 0; i <= L; i++ ) {
    b[ i ] = 0.0;
    history[ i ] = 0.0;
  }

  _Pragma( "loopbound min 201 max 201" )
  for ( i = 0 ; i < N ; i++ ) {
    lms_output[ i ] = lms_calc( lms_input[ i ],
                              lms_input[ i + 1 ],
                              b, L, 0.02 / ( L + 1 ), 0.01,
                              history, &sigma );
  }
}


int lms_return( void )
{
  int i;
  double sum = 0.0;

  _Pragma( "loopbound min 201 max 201" )
  for ( i = 0 ; i < N ; i++ ) {
      sum += lms_output[i];
  }

  return ( int )( 1000000.0 * ( sum + 4.705719 ) );
  // How did this 'correct value' come to be? The previous calculation contained UB.
  // correct value: -4.505242517625447362661361694336
}


int main()
{
  lms_init();
  lms_main();
  return ( lms_return() );
}

