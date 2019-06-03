/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: iir

  Author: Juan Martinez Velarde

  Function:
    The equations of each biquad section filter are:
      w(n) =    x(n) - ai1*w(n-1) - ai2*w(n-2)
      y(n) = b0*w(n) + bi1*w(n-1) + bi2*w(n-2)

    Biquads are sequentally positioned. Input sample for biquad i is
    xi-1(n). Output sample for biquad i is xi(n).
    System input sample is x0(n). System output sample is xN(n) = y(n)
    for N biquads.

    Each section performs following filtering (biquad i) :

                              wi(n)
      xi-1(n) ---(-)---------->-|->---bi0---(+)-------> xi(n)
                  A             |            A
                  |           |1/z|          |
                  |             | wi(n-1)    |
                  |             v            |
                  |-<--ai1----<-|->---bi1-->-|
                  |             |            |
                  |           |1/z|          |
                  |             | wi(n-2)    |
                  |             v            |
                  |-<--ai2----<--->---bi2-->-|

    The values wi(n-1) and wi(n-2) are stored in wi1 and wi2

  Source: DSPstone
          http://www.ice.rwth-aachen.de/research/tools-projects/entry/detail/dspstone

  Original name: iir_N_sections_float

  Changes:
           24-03-94 creation fixed-point (Martinez Velarde)
           16-03-95 adaption floating-point (Harald L. Schraut)

  License: may be used, modified, and re-distributed freely

*/


/*
  Forward declaration of functions
*/

void iir_init( void );
int iir_return( void );
void iir_main( void );
int main( void );


/*
  Declaration of global variables
*/

volatile float iir_wi[ 2 * 4 ];
volatile float iir_coefficients[ 5 * 4 ];
float iir_x;


/*
  Initialization- and return-value-related functions
*/

void iir_init( void )
{
  int f;
  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;


  _Pragma( "loopbound min 20 max 20" )
  for ( f = 0 ; f < 5 * 4; f++ )
    iir_coefficients[ f ] = 7;

  _Pragma( "loopbound min 8 max 8" )
  for ( f = 0 ; f < 2 * 4; f++ )
    iir_wi[ f ] = 0;

  iir_x = ( float ) 1;

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = ( unsigned char * ) &iir_coefficients[ 0 ];
  _Pragma( "loopbound min 80 max 80" )
  for ( i = 0; i < sizeof( iir_coefficients ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &iir_wi[ 0 ];
  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < sizeof( iir_wi ); ++i, ++p )
    *p ^= bitmask;
}


int iir_return( void )
{
  float checksum = 0.0;
  int f;


  _Pragma( "loopbound min 8 max 8" )
  for ( f = 0 ; f < 2 * 4; f++ )
    checksum += iir_wi[ f ];

  return ( ( int ) checksum );
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) iir_main( void )
{
  register float w;
  int f;
  register volatile float *ptr_coeff, *ptr_wi1, *ptr_wi2;
  register float y;


  ptr_coeff = &iir_coefficients[ 0 ];
  ptr_wi1 = &iir_wi[ 0 ];
  ptr_wi2 = &iir_wi[ 1 ];

  y = iir_x ;

  _Pragma( "loopbound min 4 max 4" )
  for ( f = 0 ; f < 4 ; f++ ) {
    w = y - *ptr_coeff++ * *ptr_wi1;
    w -= *ptr_coeff++ * *ptr_wi2;

    y = *ptr_coeff++ * w;
    y += *ptr_coeff++ * *ptr_wi1;
    y += *ptr_coeff++ * *ptr_wi2;

    *ptr_wi2++ = *ptr_wi1;
    *ptr_wi1++ = w;

    ptr_wi2++;
    ptr_wi1++;
  }
}


int main( void )
{
  iir_init();
  iir_main();

  return ( iir_return() - 400 != 0 );
}
