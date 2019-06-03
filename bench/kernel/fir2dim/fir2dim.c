/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: fir2dim

  Author: Juan Martinez Velarde

  Function: prime calculates whether numbers are prime.

  Source: DSP-Stone
          http://www.ice.rwth-aachen.de/research/tools-projects/entry/detail/dspstone/

  Original name: fir2dim_float

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

*/


/*
  Forward declaration of functions
*/

void fir2dim_initSeed( void );
long fir2dim_randomInteger();
void fir2dim_pin_down( float *pimage, float *parray, float *pcoeff,
                       float *poutput );
void fir2dim_init();
int fir2dim_return();
void fir2dim_main();
int main( void );


/*
  Declaration of global variables
*/
static float  fir2dim_coefficients[ 3 * 3 ] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
static float  fir2dim_image[ 4 * 4 ] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static float  fir2dim_array[ 6 * 6 ]  = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static float  fir2dim_output[ 4 * 4 ] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int fir2dim_result;


/*
  Initialization- and return-value-related functions
*/

void fir2dim_init()
{
  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = ( unsigned char * ) &fir2dim_coefficients[  0  ];
  _Pragma( "loopbound min 36 max 36" )
  for ( i = 0; i < sizeof( fir2dim_coefficients ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &fir2dim_image[  0  ];
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < sizeof( fir2dim_image ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &fir2dim_array[  0  ];
  _Pragma( "loopbound min 144 max 144" )
  for ( i = 0; i < sizeof( fir2dim_array ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &fir2dim_output[  0  ];
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < sizeof( fir2dim_output ); ++i, ++p )
    *p ^= bitmask;
}


int fir2dim_return()
{
  return ( fir2dim_result - 14 != 0 );
}


/*
  Helper functions
*/

void fir2dim_pin_down( float *pimage, float *parray, float *pcoeff,
                       float *poutput )
{
  register float    i, f;

  _Pragma( "loopbound min 4 max 4" )
  for ( i = 0 ; i < 4 ; i++ ) {
    _Pragma( "loopbound min 4 max 4" )
    for ( f = 0 ; f < 4 ; f++ )
      *pimage++ = 1 ;
  }

  pimage = pimage - 4 * 4  ;

  _Pragma( "loopbound min 9 max 9" )
  for ( i = 0; i < 3 * 3; i++ )
    *pcoeff++ = 1;

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0 ; i < 6 ; i++ )
    *parray++ = 0 ;

  _Pragma( "loopbound min 4 max 4" )
  for ( f = 0 ; f < 4; f++ ) {
    *parray++ = 0 ;
    _Pragma( "loopbound min 4 max 4" )
    for ( i = 0 ; i < 4 ; i++ )
      *parray++ = *pimage++ ;
    *parray++ = 0 ;
  }

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0 ; i < 6 ; i++ )
    *parray++ = 0 ;

  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0 ; i < 4 * 4; i++ )
    *poutput++ = 0 ;
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) fir2dim_main()
{
  register float *parray  = &fir2dim_array[ 0 ], *parray2, *parray3 ;
  register float *pcoeff  = &fir2dim_coefficients[ 0 ] ;
  register float *poutput = &fir2dim_output[ 0 ]       ;
  int k, f, i;

  fir2dim_pin_down( &fir2dim_image[ 0 ], &fir2dim_array[ 0 ],
                    &fir2dim_coefficients[ 0 ], &fir2dim_output[ 0 ] );

  poutput = &fir2dim_output[ 0 ]       ;

  _Pragma( "loopbound min 4 max 4" )
  for ( k = 0 ; k < 4 ; k++ ) {

    _Pragma( "loopbound min 4 max 4" )
    for ( f = 0 ; f < 4 ; f++ ) {
      pcoeff = &fir2dim_coefficients[ 0 ] ;
      parray = &fir2dim_array[ k * 6 + f ] ;
      parray2 = parray + 6 ;
      parray3 = parray + 6 + 6 ;

      *poutput = 0 ;

      _Pragma( "loopbound min 3 max 3" )
      for ( i = 0 ; i < 3 ; i++ )
        *poutput += *pcoeff++ **parray++ ;

      _Pragma( "loopbound min 3 max 3" )
      for ( i = 0 ; i < 3 ; i++ )
        *poutput += *pcoeff++ **parray2++ ;

      _Pragma( "loopbound min 3 max 3" )
      for ( i = 0 ; i < 3 ; i++ )
        *poutput += *pcoeff++ **parray3++ ;

      poutput++ ;
    }
  }

  fir2dim_result = fir2dim_output[ 0 ] + fir2dim_output[ 5 ] + fir2dim_array[ 9 ];

  fir2dim_pin_down( &fir2dim_image[ 0 ], &fir2dim_array[ 0 ],
                    &fir2dim_coefficients[ 0 ], &fir2dim_output[ 0 ] );
}


int main( void )
{
  fir2dim_init();
  fir2dim_main();

  return ( fir2dim_return() );
}

