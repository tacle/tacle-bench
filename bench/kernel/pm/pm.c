/*

  This program is part of the TACLeBench benchmark suite.
  Version 2.0

  Name: pm

  Author: Hector Chan
          MIT Lincoln Laboratory

  Function: This is the ANSI C Pattern Match kernel. It finds the closest
            match of a pattern from a library of patterns.  The code below
            serves as a reference implemenation of the pattern match kernel.

  Source: HPEC Challenge Benchmark Suite, Pattern Match Kernel Benchmark

  Original name: pm

  Changes: See ChangeLog.txt

  License: BSD 3-clause

*/

/*
  Include section
*/

#include "pm_math.h"
#include "pm_string.h"


/*
  Definition of types
*/

typedef struct pm_float_array_t {
  float        *data;
  void         *datav;
  int          size[ 3 ];
  unsigned int ndims;
  unsigned int rctype;
  char padding[ 4 ];
} pm_float_array_t;


typedef struct pm_data_t {
  float *template_profiles_db; /* the library of patterns */
  float *test_profile_db; /* the test pattern */

  float *template_copy; /* temporary storage for a template */
  float *test_noise_db_array; /* copies of test noise in an array for
                                 fast copy */
  float *MSE_scores; /* the likelihood of the matching between a
                        range shift of the test pattern and the libary */
  float *mag_shift_scores; /* the likelihood of the matching between a
                              magnitude scaling of the test pattern and the
                              libary */
  float *minimum_MSE_score; /* the likelihood of the matching between the
                               test pattern and the libary */
  float *all_shifted_test_db;  /* contains the shiftings of the test pattern */

  unsigned char *template_exceed; /* marking where a library template
                                     exceeds twice the noise level of
                                     the test pattern */
  float *test_exceed_means; /* pixels where test pattern exceeds twice
                               its noise level */

  float shift_ratio; /* determines the number of range shifts */
  int   shift_size; /* the actual number of range shifts */
  int   profile_size; /* the length of the pattern */
  int   num_templates; /* the number of library templates */
  int   elsize; /* the size of a single fp number */
  char padding[ 4 ];
} pm_data_t;


/*
  Forward declaration of functions
*/

void pm_init_lib( pm_float_array_t *lib );
void pm_init_pattern( pm_float_array_t *pattern );
void pm_init_data( pm_data_t *pmdata,
                   pm_float_array_t *lib,
                   pm_float_array_t *pattern );
void pm_clean( pm_data_t *pmdata );
void pm_main( void );
void pm_init( void );
int pm_return( void );
int pm_kernel( pm_data_t *pmdata );


/*
  Declaration of global variables
*/

/* input data */
extern float pm_lib_data[  60  ][  64  ];
extern float pm_pattern_data[  60  ][  64  ];

/* some magic number */
#define pm_MIN_NOISE 1e-10f

/* main data structures used by the benchmark */
static pm_data_t pm_data;
static pm_float_array_t pm_lib;
static float *pm_lib_ptr[  60  ];
static pm_float_array_t pm_pattern;
static float *pm_pattern_ptr[  60  ];
static int pm_result;

/* arrays for the pm_init_data function */
static unsigned char pm_init_array_1[ 64 ];
static float pm_init_array_2[ 21 ];
static float pm_init_array_3[ 64 ];
static float pm_init_array_4[ 64 ];
static float pm_init_array_5[ 21 ];
static float pm_init_array_6[ 21 ];
static float pm_init_array_7[ 72 ];
static float pm_init_array_8[ 110 ];


/*
  Initialization- and return-value-related functions
*/

void pm_init_lib( pm_float_array_t *lib )
{
  int i;
  volatile int do_not_optimize_away = 0;

  lib->rctype = 1;
  lib->ndims = 2;
  lib->size[ 0 ] = 60;
  lib->size[ 1 ] = 64;
  lib->size[ 2 ] = 0;

  _Pragma( "loopbound min 60 max 60" )
  for ( i = 0; i < 60; i++ )
    pm_lib_ptr[ i ] = pm_lib_data[ i ];

  _Pragma( "loopbound min 60 max 60" )
  for ( i = 0; i < 60; i++ )
    pm_lib_ptr[ i ] += do_not_optimize_away;

  lib->data = *pm_lib_ptr;
  lib->datav = ( void * )pm_lib_ptr;
}


void pm_init_pattern( pm_float_array_t *pattern )
{
  int i;
  volatile int do_not_optimize_away = 0;

  pattern->rctype = 1;
  pattern->ndims = 2;
  pattern->size[ 0 ] = 60;
  pattern->size[ 1 ] = 64;
  pattern->size[ 2 ] = 0;

  _Pragma( "loopbound min 60 max 60" )
  for ( i = 0; i < 60; i++ )
    pm_pattern_ptr[ i ] = pm_pattern_data[ i ];

  _Pragma( "loopbound min 60 max 60" )
  for ( i = 0; i < 60; i++ )
    pm_pattern_ptr[ i ] += do_not_optimize_away;

  pattern->data = *pm_pattern_ptr;
  pattern->datav = ( void * )pm_pattern_ptr;
}


void pm_init( void )
{
  pm_math_init();
}


int pm_return( void )
{
  return pm_result - 12;
}


/*
  Core benchmark functions
*/

void _Pragma( "entrypoint" ) pm_main( void )
{
  pm_init_lib( &pm_lib );
  pm_init_pattern( &pm_pattern );
  pm_init_data( &pm_data, &pm_lib, &pm_pattern );
  pm_result = pm_kernel( &pm_data );
  pm_clean( &pm_data );
}


/***********************************************************************/
/* Allocate and initailize the test pattern, the template library, and
   other necessary data structure. */
/***********************************************************************/
void pm_init_data( pm_data_t *pmdata, pm_float_array_t *lib,
                   pm_float_array_t *pattern )
{
  int   elsize = sizeof( float );
  float x;

  /* Getting the input parameters from the PCA C array structure */
  pmdata->profile_size  = lib->size[ 1 ];
  pmdata->num_templates = lib->size[ 0 ];

  pmdata->elsize = elsize;
  pmdata->shift_ratio = 3.0f;

  pmdata->template_profiles_db = lib->data;
  pmdata->test_profile_db = pattern->data;

  /* Equivalent to shift_size = roundf((float)profile_size / shift_ratio) */
  x = ( float )( pmdata->profile_size ) / pmdata->shift_ratio;
  pmdata->shift_size = ( ( x - ( int )( x ) ) < 0.5f ) ?
                       ( int )( pm_floor( x ) ) :
                       ( int )( pm_ceil( x ) );

  pmdata->template_exceed     = pm_init_array_1;
  pmdata->test_exceed_means   = pm_init_array_2;

  pmdata->template_copy       = pm_init_array_3;
  pmdata->test_noise_db_array = pm_init_array_4;

  pmdata->MSE_scores          = pm_init_array_5;
  pmdata->mag_shift_scores    = pm_init_array_6;

  pmdata->minimum_MSE_score   = pm_init_array_7;
  pmdata->all_shifted_test_db = pm_init_array_8;
}


/***********************************************************************/
/* Free up memory for all structures */
/***********************************************************************/
void pm_clean( pm_data_t *pmdata )
{
  pmdata->test_exceed_means = 0;
  pmdata->template_exceed = 0;
  pmdata->template_copy = 0;
  pmdata->test_noise_db_array = 0;
  pmdata->MSE_scores = 0;
  pmdata->mag_shift_scores = 0;
  pmdata->minimum_MSE_score = 0;
  pmdata->all_shifted_test_db = 0;
}


/***********************************************************************/
/* The pattern match kernel overlays two patterns to compute the likelihood
   that the two vectors match. This process is performed on a library of
   patterns. */
/***********************************************************************/
int pm_kernel( pm_data_t *pmdata )
{
  const int elsize = pmdata->elsize; /* size of a single fp number */
  const int shift_size = pmdata->shift_size; /* number of shifting to the
                                                left and right of the test
                                                profile */
  const int profile_size =
    pmdata->profile_size; /* number of pixels in a pattern */
  const int num_templates =
    pmdata->num_templates; /* number of library patterns */
  float *test_profile_db =
    pmdata->test_profile_db; /* the test pattern */
  float *template_profiles_db =
    pmdata->template_profiles_db; /* the library of patterns */
  float *test_noise_db_array =
    pmdata->test_noise_db_array; /* the noise in the test pattern in an array
                                    for fast copy */
  float *all_shifted_test_db =
    pmdata->all_shifted_test_db; /* the shifted test pattern */

  int match_index; /* the index of the most likely template that matches the
                      test pattern */
  int min_MSE_index; /* the index of the range shifts with
                        the lowest mean square error */
  unsigned int num_template_exceed,
           num_test_exceed; /* the number of pixels exceeded the test pattern
                               and a library template */

  unsigned char mag_shift_scores_flag; /* flag that tells if the magnitude
                                          scaling loop has been run
                                          (existed just to save ops) */

  float test_peak,
        template_peak; /* the maximum pixels of the test pattern and a library
                          template pattern */
  float template_noise; /* the noise level of a library template */

  float noise_shift,
        noise_shift2; /* temporary storage for calculating the mse for range shifting */

  float min_MSE, match_score; /* temporary storage for finding the minimum mse */

  float sumWeights_inv = 1.0f / profile_size; /* the inverse of the weights
                                                 used for calculating the mse */
  /* Note: weights for the kernel would be application dependent.
     They are set to 1 for our purposes */

  float mag_db; /* the magnitude shifts in dB */
  float power_shift,
        ave_power_ratio; /* the diff of the avg shifted test profile power to
                            the avg template power */
  float power_ratio; /* the mean power of the pixels of a template that exceeded
                        twice test noise */

  float test_noise = ( pm_pow10f( test_profile_db[ 0 ] * 0.1f )
                       + /* noise level of the test pattern */
                       pm_pow10f( test_profile_db[ profile_size - 1 ] * 0.1f ) ) * 0.5f;

  /* since "shift_size/2" is used a lot, so we create a var to hold it */
  int half_shift_size = ( int )( pm_ceil( ( float )( shift_size ) / 2.0f ) );
  int template_index, current_shift; /* indices */
  int patsize = profile_size * elsize; /* number of bytes of a pattern */

  float *minimum_MSE_score = pmdata->minimum_MSE_score;
  float *MSE_scores = pmdata->MSE_scores;
  float *mag_shift_scores  = pmdata->mag_shift_scores;

  float test_noise_db = ( test_noise == 0.0f ) ? -100.0f : 10.0f *
                        pm_log10f( pm_fabs( test_noise ) ); /* test noise in dB */
  float test_noise_db_plus_3 = test_noise_db + 3.0f; /* twice test noise in the
                                             power domain, approximately +3dB */

  float *template_copy = pmdata->template_copy;
  unsigned char *template_exceed = pmdata->template_exceed;
  float *test_exceed_means = pmdata->test_exceed_means;

  int i, j; /* indices */

  float tmp1; /* temporary storage for calculating the mse for range shifting */
  float sum_exceed; /* the sum of the test pattern pixels exceeded twice
                       test noise */
  float template_exceed_mean = 0; /* the mean of a template pattern pixels
                                     exceeded twice test noise */
  float weighted_MSE; /* temporary storage for computing the weighted MSE */

  /* These pointers are solely used for fast memory access */
  float *cur_tp, *fptr, *fptr2, *fptr3, *endptr;
  unsigned char *bptr;

  /* Having an array of test noise for fast copying of noise returns */
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < profile_size; i++ )
    test_noise_db_array[ i ] = test_noise_db;

  /* Finding the maximum pixels of the test pattern */
  fptr = test_profile_db;
  test_peak = *fptr++;
  _Pragma( "loopbound min 63 max 63" )
  for ( i = 1; i < profile_size; i++, fptr++ ) {
    if ( test_peak < *fptr )
      test_peak = *fptr;
  }

  /* Paddle array for all the possible range shifts. Essentially, we are
     performing the following:

     Adding these two portions to the beginning and end of the test pattern
          |                          |
          V                          V
      |<------>|                 |<------>|

                   __       __
                  |  |     |  |
                 |    |___|    |
                |               |
      _________|                 |_________   <- test noise in dB domain
     ---------------------------------------  <- zero

               |<--------------->|
               original test pattern


     The all_shifted_test_db will be accessed in a sliding window manner.
  */

  pm_memcpy( ( void * ) all_shifted_test_db, ( void * ) test_noise_db_array,
             elsize * half_shift_size );
  pm_memcpy( ( void * ) ( all_shifted_test_db + half_shift_size ),
             ( void * ) test_profile_db, elsize * profile_size );
  pm_memcpy( ( void * ) ( all_shifted_test_db + half_shift_size + profile_size ),
             ( void * ) test_noise_db_array, elsize * half_shift_size );

  /* Set the pixels to test noise in dB domain if pixel is less than test
     noise in dB */
  fptr = all_shifted_test_db + half_shift_size;
  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < profile_size; i++, fptr++ ) {
    if ( *fptr < test_noise_db )
      *fptr = test_noise_db;
  }

  /* Calculating the mean of the pixels that exceeded twice test noise for each
     possible shift of the test profile */
  fptr2 = test_exceed_means;
  _Pragma( "loopbound min 21 max 21" )
  for ( current_shift = 0; current_shift < shift_size; current_shift++ ) {
    /* Pointer arithmetics to find the start and end pointers */
    if ( current_shift < half_shift_size ) {
      endptr = all_shifted_test_db + current_shift + profile_size;
      fptr   = all_shifted_test_db + half_shift_size;
    } else {
      endptr = all_shifted_test_db + half_shift_size + profile_size;
      fptr   = all_shifted_test_db + current_shift;
    }

    /* Summing the pixels that exceed twice test noise for the current shifts */
    sum_exceed = 0.0f;
    num_test_exceed = 0;
    _Pragma( "loopbound min 53 max 64" )
    while ( fptr != endptr ) {
      if ( *fptr > test_noise_db_plus_3 ) {
        num_test_exceed++;
        sum_exceed += *fptr;
      }
      fptr++;
    }
    *fptr2++ = num_test_exceed ?
               sum_exceed / ( float )( num_test_exceed ) :
               0.0f;
  }


  /* Loop over all the templates. Determine the best shift distance, then
     the best gain adjustment. */
  _Pragma( "loopbound min 60 max 60" )
  for ( template_index = 0; template_index < num_templates; template_index++ ) {
    cur_tp = template_profiles_db + ( template_index * profile_size );

    /* Scale the template profile we're currently working on so that its peak
       is equal to the peak of the test profile */

    fptr = cur_tp;
    template_peak = *fptr++;
    _Pragma( "loopbound min 63 max 63" )
    for ( i = 1; i < profile_size; i++, fptr++ ) {
      if ( template_peak < *fptr )
        template_peak = *fptr;
    }

    /* Additively adjust the noise level of this template profile in the
       raw power domain so that its noise level matches the noise level
       of the test profile */

    /* --------------------------------------------------------------------
       Setting up all the constants */

    noise_shift  = test_peak - template_peak;
    pm_memset ( ( void * )template_exceed, 0,
                ( ( int )sizeof( char ) )*profile_size );
    sum_exceed = 0.0f;
    num_template_exceed = 0;

    /* --------------------------------------------------------------------
       The following blocks are optimized code that essentially
       perform the operations immediately below. The calculation of the
       template noise constants is done once the exponentials are complete
    */

    /* template_profile = template_profile + test_peak - template_peak
       template = 10 ^ (template_profile / 10)
       template = template + test_noise - template_noise
       if (input < fp_epsilon) then clip the input to -100 dB
       template = log10( abs(template) )
       template_profile = 10 * template + test_noise_db */

    fptr = cur_tp;
    _Pragma( "loopbound min 64 max 64" )
    for ( i = 0; i < profile_size; i++ ) {
      tmp1 = *fptr + noise_shift;
      *fptr = pm_pow10f( tmp1 * 0.1f );
      fptr++;

    }

    /* Calculates noise levels from first and last elements of the current
       template */

    template_noise = ( cur_tp[ 0 ] + cur_tp[ profile_size - 1 ] ) * 0.5f;
    noise_shift2 = test_noise - template_noise;

    fptr = cur_tp;
    _Pragma( "loopbound min 64 max 64" )
    for ( i = 0; i < profile_size; i++ ) {
      tmp1 = *fptr + noise_shift2;

      if ( tmp1 == 0.0f )
        tmp1 = pm_MIN_NOISE;

      *fptr = 10.0f * pm_log10f( pm_fabs( tmp1 ) ) + test_noise_db;

      /* Because many of the operations in the search for the best shift
         amount depend on knowledge of which pixels in the template
         have values exceeding twice test_noise (recall that 3db is roughly
         equivalent to a doubling of raw power), we'll put those indices in
         template_exceed */

      if ( *fptr > test_noise_db_plus_3 ) {
        template_exceed[ i ] = 1;
        num_template_exceed++;
        sum_exceed += *fptr;
      }

      fptr++;
    }

    /* Note: The following block has 4 different branches:
       1. Both the current template and the test pattern have values exceeded
          twice test noise.
       2. Only the current template has values exceeded twice test noise.
       3. Only the test pattern has values exceeded twice test noise.
       4. Neither the current template nor the test pattern has values
          exceeded twice test noise.
    */

    /* If there is at least one pixel in the template we're
       currently working on whose value exceeds twice test_noise */
    if ( num_template_exceed ) {
      template_exceed_mean = sum_exceed / ( float )( num_template_exceed );
      fptr3 = test_exceed_means;

      _Pragma( "loopbound min 21 max 21" )
      for ( current_shift = 0; current_shift < shift_size;
            current_shift++, fptr3++ ) {
        /* Work on a copy of the template we're currently working on */
        pm_memcpy ( ( void * )template_copy, ( void * )cur_tp, patsize );

        /* If there is at least one pixel in the shifted test profile
           whose value exceeds twice test noise. */
        if ( *fptr3 != 0.0f ) {
          /* CASE 1 */
          /* Considering only those pixels whose powers exceed twice
             test noise, compute the difference of the mean power in
             template we're currently working on. */
          power_ratio = *fptr3 - template_exceed_mean;

          /* Scale template values that exceed twice test noise by power ratio
             and set the values that are less than test noise in db to test
             noise in db */
          fptr  = template_copy;
          bptr  = template_exceed;
          _Pragma( "loopbound min 64 max 64" )
          for ( i = 0; i < profile_size; i++, fptr++ ) {
            if ( *bptr++ )
              *fptr += power_ratio;

            if ( *fptr < test_noise_db )
              *fptr = test_noise_db;
          }
        } /* if (*fptr3 != 0.0f) */
        else {
          /* CASE 2 */
          /* Set those pixels in the template we're currently working on
             whose values are less than test_noise to test_noise. */
          fptr = cur_tp;
          _Pragma( "loopbound min 64 max 64" )
          for ( i = 0; i < profile_size; i++ ) {
            if ( *fptr++ < test_noise_db )
              template_copy[ i ] = test_noise_db;
          }
        } /* else ... if (num_test_exceed) */

        /* Compute the weighted MSE */
        weighted_MSE = 0.0f;
        fptr  = all_shifted_test_db + current_shift;
        fptr2 = template_copy;
        _Pragma( "loopbound min 64 max 64" )
        for ( i = 0; i < profile_size; i++ ) {
          tmp1 = *fptr++ - *fptr2++;
          weighted_MSE += tmp1 * tmp1;
        }

        MSE_scores[ current_shift ] = weighted_MSE * sumWeights_inv;

      } /* for current_shift */
    } else { /* if (num_template_exceed) */
      fptr3 = test_exceed_means;

      _Pragma( "loopbound min 0 max 0" )
      for ( current_shift = 0; current_shift < shift_size; current_shift++ ) {
        /* CASE 3 */
        /* If there is at least one pixel that exceeds twice test noise */
        if ( *fptr3++ != 0.0f )
          fptr2 = cur_tp;
        else {
          /* CASE 4 */
          /* Work on a copy of the template we're currently working on. */
          pm_memcpy ( ( void * )template_copy, ( void * )cur_tp, patsize );

          fptr = cur_tp;
          _Pragma( "loopbound min 0 max 0" )
          for ( i = 0; i < profile_size; i++ ) {
            if ( *fptr++ < test_noise_db )
              template_copy[ i ] = test_noise_db;
          }

          fptr2 = template_copy;
        }

        /* Compute the weighted MSE */
        weighted_MSE = 0.0f;
        fptr  = all_shifted_test_db + current_shift;
        _Pragma( "loopbound min 0 max 0" )
        for ( i = 0; i < profile_size; i++ ) {
          tmp1 = *fptr++ - *fptr2++;
          weighted_MSE += tmp1 * tmp1;
        }

        MSE_scores[ current_shift ] = weighted_MSE * sumWeights_inv;

      } /* for current_shift */
    } /* else .. if (num_template_exceed) */

    /* Finding the minimum MSE for range shifting */
    fptr = MSE_scores;
    min_MSE_index = 0;
    min_MSE = *fptr++;
    _Pragma( "loopbound min 20 max 20" )
    for ( i = 1; i < shift_size; i++, fptr++ ) {
      if ( min_MSE > *fptr ) {
        min_MSE = *fptr;
        min_MSE_index = i;
      }
    }

    /* Work on a copy of the template we're currently working on. */
    pm_memcpy( ( void * )template_copy, ( void * )cur_tp, patsize );

    mag_shift_scores_flag = 1;

    if ( test_exceed_means[ min_MSE_index ] != 0.0f ) {
      if ( num_template_exceed ) {
        /* Compute the difference of the average shifted test profile
           power to the average template power */
        ave_power_ratio = test_exceed_means[ min_MSE_index ]
                          - template_exceed_mean;

        /* Loop over all possible magnitude shifts */
        _Pragma( "loopbound min 21 max 21" )
        for ( j = 0, mag_db = -5.0f; mag_db <= 5.0f; mag_db += 0.5f ) {
          power_shift = ave_power_ratio + mag_db;

          bptr  = template_exceed;
          _Pragma( "loopbound min 64 max 64" )
          for ( i = 0; i < profile_size; i++ ) {
            if ( *bptr++ )
              template_copy[ i ] = cur_tp[ i ] + power_shift;
          }

          /* Compute the weighted MSE */
          weighted_MSE = 0.0f;
          fptr  = all_shifted_test_db + min_MSE_index;
          fptr2 = template_copy;
          _Pragma( "loopbound min 64 max 64" )
          for ( i = 0; i < profile_size; i++ ) {
            tmp1 = *fptr++ - *fptr2++;
            weighted_MSE += tmp1 * tmp1;
          }

          mag_shift_scores[ j++ ] = weighted_MSE * sumWeights_inv;

        } /* for mag_db */
      } /* if (num_template_exceed) */

    } else { /* if (num_test_exceed) */
      /* Set those pixels in the template we're currently working on
         whose values are less than test_noise to test_noise. */
      fptr = cur_tp;
      _Pragma( "loopbound min 64 max 64" )
      for ( i = 0; i < profile_size; i++ ) {
        if ( *fptr++ < test_noise_db )
          template_copy[ i ] = test_noise_db;
      }

      /* Compute the weighted MSE */
      weighted_MSE = 0.0f;
      fptr = all_shifted_test_db + min_MSE_index;
      fptr2 = template_copy;
      _Pragma( "loopbound min 64 max 64" )
      for ( i = 0; i < profile_size; i++ ) {
        tmp1 = *fptr++ - *fptr2++;
        weighted_MSE += tmp1 * tmp1;
      }

      minimum_MSE_score[ template_index ] = weighted_MSE * sumWeights_inv;

      mag_shift_scores_flag = 0;
    } /* if (num_test_exceed) */

    /* If magnitude shifting has performed above */
    if ( mag_shift_scores_flag ) {
      /* Find the minimum MSE for magnitude scaling */
      fptr = mag_shift_scores;
      min_MSE = *fptr++;
      _Pragma( "loopbound min 20 max 20" )
      for ( i = 1; i < 21; i++, fptr++ ) {
        if ( min_MSE > *fptr )
          min_MSE = *fptr;
      }

      minimum_MSE_score[ template_index ] = min_MSE;
    }

  } /* for template_index */

  /* Find the minimum mean square error */
  fptr = minimum_MSE_score;
  match_index = 0;
  match_score = *fptr++;
  _Pragma( "loopbound min 59 max 59" )
  for ( i = 1; i < num_templates; i++, fptr++ ) {
    if ( match_score > *fptr ) {
      match_score = *fptr;
      match_index = i;
    }
  }

  return match_index;
}


/*
  Main function
*/

int main( void )
{
  pm_init();
  pm_main();

  return pm_return();
}
