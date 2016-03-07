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

void fir2dim_pin_down( float *pimage, float *parray, float *pcoeff,
                       float *poutput );
void fir2dim_init();
int fir2dim_return();
void fir2dim_main();
int main( void );


/*
  Declaration of global variables
*/

int fir2dim_result;


/*
  Initialization- and return-value-related functions
*/

void fir2dim_init() {}


int fir2dim_return()
{
  return fir2dim_result;
}


/*
  Helper functions
*/

void pin_down( float *pimage, float *parray, float *pcoeff, float *poutput )
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
  {
    static float  coefficients[3 * 3] ;
    static float  image[4 * 4]  ;
    static float  array[6 * 6]  ;
    static float  output[4 * 4] ;

    register float *pimage  = &image[0]        ;
    register float *parray  = &array[0], *parray2, *parray3 ;
    register float *pcoeff  = &coefficients[0] ;
    register float *poutput = &output[0]       ;
    int k, f, i;

    pin_down( &image[0], &array[0], &coefficients[0], &output[0] );

    pimage  = &image[0]        ;
    parray  = &array[0]        ;
    pcoeff  = &coefficients[0] ;
    poutput = &output[0]       ;

    _Pragma( "loopbound min 4 max 4" )
    for ( k = 0 ; k < 4 ; k++ ) {

      _Pragma( "loopbound min 4 max 4" )
      for ( f = 0 ; f < 4 ; f++ ) {
        pcoeff = &coefficients[0] ;
        parray = &array[k * 6 + f] ;
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

    fir2dim_result = output[0] + output[5] + array[9];

    pin_down( &image[0], &array[0], &coefficients[0], &output[0] );
  }
}


int main( void )
{
  fir2dim_init();
  fir2dim_main();

  return ( fir2dim_return() );
}

