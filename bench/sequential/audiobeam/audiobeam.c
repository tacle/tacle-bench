/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: audiobeam

  Author: Eugene Weinstein

  Function: Audio beam former

  Source: StreamIt
          http://groups.csail.mit.edu/cag/streamit/

  Changes: no functional changes

  License: see license.txt

*/


/*
  Include section
*/

#include "audiobeamlibm.h"
#include "audiobeamlibmalloc.h"
#include "audiobeam.h"

/*
  Forward declaration of functions
*/

void audiobeam_init();
int audiobeam_return();
void audiobeam_main( void );
int main( void );
void audiobeam_preprocess_delays( struct audiobeam_PreprocessedDelays
                                  prep_delays[  ], float *delays );
float *audiobeam_parse_line( float *float_arr, int num_mic );
long int audiobeam_find_max_in_arr( float *arr, int size );
long int audiobeam_find_min_in_arr( float *arr, int size );
int audiobeam_wrapped_inc_offset( int i, int offset, int max_i );
int audiobeam_wrapped_dec_offset( int i, int offset, int max_i );
int audiobeam_wrapped_inc( int i, int max_i );
int audiobeam_wrapped_dec( int i, int max_i );
struct audiobeam_DataQueue *audiobeam_init_data_queue( int max_delay,
    int num_mic );
struct audiobeam_Delays *audiobeam_init_delays ( int num_angles, int num_mic );
void audiobeam_calc_distances( float *source_location,
                               float audiobeam_mic_locations[ 15 ][ 3 ],
                               float *distances,
                               int num_mic );
void audiobeam_calc_delays( float *distances, float *delays, int sound_speed,
                            int sampling_rate, int num_mic );
void audiobeam_adjust_delays( float *delays, int num_mic );
float *audiobeam_calc_weights_lr ( int num_mic );
float *audiobeam_calc_weights_left_only ( int num_mic );
float audiobeam_calculate_energy( float *samples, int num_samples );
float audiobeam_do_beamforming( struct audiobeam_PreprocessedDelays
                                preprocessed_delays[  ],
                                float **sample_queue,
                                int queue_head,
                                long int max_delay,
                                int num_mic,
                                float *weights );
int audiobeam_process_signal( struct audiobeam_Delays *delays, int num_mic,
                              float sampling_rate, float **beamform_results,
                              struct audiobeam_DataQueue *queue,
                              int num_beams, int window, float *weights );
int audiobeam_calc_beamforming_result( struct audiobeam_Delays *delays,
                                       float **beamform_results,
                                       float *energies,
                                       struct audiobeam_DataQueue *queue,
                                       int num_beams, int window,
                                       int hamming );
void audiobeam_calc_single_pos( float source_location[ 3 ],
                                float audiobeam_mic_locations[ 15 ][ 3 ],
                                int hamming );


/*
  Declaration of global variables
*/

extern float audiobeam_input[ 5760 ];
extern float audiobeam_mic_locations[ 15 ][ 3 ];
extern float audiobeam_source_location[ 3 ];
extern float audiobeam_origin_location[ 3 ];
int audiobeam_input_pos;
int audiobeam_checksum;


/*
  Initialization- and return-value-related functions
*/

void audiobeam_init()
{
  audiobeam_input_pos = 0;
  audiobeam_checksum = 0;

  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = ( unsigned char * ) &audiobeam_input[  0  ];
  _Pragma( "loopbound min 23040 max 23040" )
  for ( i = 0; i < sizeof( audiobeam_input ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &audiobeam_mic_locations[  0  ];
  _Pragma( "loopbound min 180 max 180" )
  for ( i = 0; i < sizeof( audiobeam_mic_locations ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &audiobeam_source_location[  0  ];
  _Pragma( "loopbound min 12 max 12" )
  for ( i = 0; i < sizeof( audiobeam_source_location ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &audiobeam_origin_location[  0  ];
  _Pragma( "loopbound min 12 max 12" )
  for ( i = 0; i < sizeof( audiobeam_origin_location ); ++i, ++p )
    *p ^= bitmask;
}


int audiobeam_return()
{
  return ( audiobeam_checksum + 1 != 0 );
}


/*
  Algorithm core functions
*/

void audiobeam_preprocess_delays( struct audiobeam_PreprocessedDelays
                                  prep_delays[  ], float *delays )
{
  int i;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < 15; i++ ) {
    prep_delays[ i ].delay = delays[ i ];
    prep_delays[ i ].high = ( int ) audiobeam_ceil( delays[ i ] );
    prep_delays[ i ].low = ( int ) audiobeam_floor( delays[ i ] );
    prep_delays[ i ].offset = delays[ i ] - prep_delays[ i ].low;
  }
}


float *audiobeam_parse_line( float *float_arr, int num_mic )
{
  int i;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < num_mic; i++ )
    float_arr[ i ] = audiobeam_input[ audiobeam_input_pos++ ];

  return float_arr;
}


long int audiobeam_find_max_in_arr( float *arr, int size )
{
  int i;
  float max = 0;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < size; i++ ) {
    if ( arr[ i ] > max )
      max = arr[ i ];
  }

  return audiobeam_ceil( max );
}


long int audiobeam_find_min_in_arr( float *arr, int size )
{
  int i;
  float min = arr[ 0 ];

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < size; i++ ) {
    if ( arr[ i ] < min )
      min = arr[ i ];
  }

  return audiobeam_floor( min );
}


int audiobeam_wrapped_inc_offset( int i, int offset, int max_i )
{
  if ( i + offset > max_i )
    return ( i + offset - max_i - 1 );
  else
    return ( i + offset );
}


int audiobeam_wrapped_dec_offset( int i, int offset, int max_i )
{
  if ( i - offset < 0 )
    return ( max_i - ( offset - i ) + 1 );
  else
    return ( i - offset );
}


int audiobeam_wrapped_inc( int i, int max_i )
{
  return audiobeam_wrapped_inc_offset( i, 1, max_i );
}


int audiobeam_wrapped_dec( int i, int max_i )
{
  return audiobeam_wrapped_dec_offset( i, 1, max_i );
}


struct audiobeam_DataQueue *audiobeam_init_data_queue( int max_delay,
    int num_mic )
{
  int i, j;

  struct audiobeam_DataQueue *queue;
  queue = ( struct audiobeam_DataQueue * ) audiobeam_malloc( sizeof(
            struct audiobeam_DataQueue ) );
  queue->sample_queue = ( float ** ) audiobeam_malloc( ( max_delay + 1 )
                        * sizeof( float * ) );

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < ( max_delay + 1 ); i++ ) {
    ( queue->sample_queue )[ i ] = ( float * ) audiobeam_malloc( num_mic
                                 * sizeof( float ) );
    _Pragma( "loopbound min 15 max 15" )
    for ( j = 0; j < num_mic; j++ ) {
      ( queue->sample_queue )[ i ][ j ] = 0.0; // Initialize values to 0
    }
  }

  queue->head = 0;
  queue->tail = 0;
  queue->full = 0;

  return queue;
}


struct audiobeam_Delays *audiobeam_init_delays ( int num_angles, int num_mic )
{
  struct audiobeam_Delays *delays;
  int i;

  delays = ( struct audiobeam_Delays * ) audiobeam_malloc( sizeof(
             struct audiobeam_Delays ) );

  // Initialize the delays array
  delays->delay_values = ( float ** ) audiobeam_malloc( num_angles
                         * sizeof( float * ) );

  _Pragma( "loopbound min 1 max 1" )
  for ( i = 0; i < ( num_angles ); i++ ) {
    delays->delay_values[ i ] = ( float * ) audiobeam_malloc( num_mic
                              * sizeof( float ) );
  }

  return delays;
}

void audiobeam_calc_distances( float *source_location,
                               float audiobeam_mic_locations[ 15 ][ 3 ],
                               float *distances,
                               int num_mic )
{
  int i;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < num_mic; i++ ) {
    distances[ i ] = ( audiobeam_sqrt( ( audiobeam_mic_locations[ i ][ 0 ]
                                       - source_location[ 0 ] ) *
                                     ( audiobeam_mic_locations[ i ][ 0 ]
                                       - source_location[ 0 ] ) +
                                     ( audiobeam_mic_locations[ i ][ 1 ]
                                       - source_location[ 1 ] ) *
                                     ( audiobeam_mic_locations[ i ][ 1 ]
                                       - source_location[ 1 ] ) +
                                     ( audiobeam_mic_locations[ i ][ 2 ]
                                       - source_location[ 2 ] ) *
                                     ( audiobeam_mic_locations[ i ][ 2 ]
                                       - source_location[ 2 ] ) ) );
  }
}


void audiobeam_calc_delays( float *distances, float *delays, int sound_speed,
                            int sampling_rate, int num_mic )
{
  int i;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < num_mic; i++ )
    delays[ i ] = ( distances[ i ] / sound_speed ) * sampling_rate;
}


void audiobeam_adjust_delays( float *delays, int num_mic )
{
  int i;
  long int min_delay = audiobeam_find_min_in_arr ( delays, num_mic ) - 1;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < num_mic; i++ )
    delays[ i ] -= min_delay;
}


float *audiobeam_calc_weights_lr ( int num_mic )
{
  float *weights = ( float * ) audiobeam_malloc( num_mic * sizeof( float ) );
  int index = 0;
  int y, z;

  int half = num_mic / 4;

  _Pragma( "loopbound min 0 max 0" )
  for ( z = 1; z >= -1; z -= 2 ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( y = 0; y < half; y++ ) {
      weights[ index ] = 0.54 + 0.46 * audiobeam_cos( audiobeam_M_PI * y
                       / half );
      index++;
    }
    _Pragma( "loopbound min 0 max 0" )
    for ( y = 0; y < half; y++ ) {
      weights[ index ] = 0.54 + 0.46 * audiobeam_cos( audiobeam_M_PI * ( -y )
                       / half );
      index++;
    }
  }

  return weights;
}


float *audiobeam_calc_weights_left_only ( int num_mic )
{
  float *weights = ( float * ) audiobeam_malloc( num_mic * sizeof( float ) );
  int index = 0;
  int y;

  int half = num_mic / 2;

  _Pragma( "loopbound min 15 max 15" )
  for ( y = -half; y <= half; y++ ) {
    weights[ index ] = 0.54 + 0.46 * audiobeam_cos( audiobeam_M_PI * y / half );
    index++;
  }

  return weights;
}


float audiobeam_calculate_energy( float *samples, int num_samples )
{
  int i;
  float sum = 0.0;

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0; i < num_samples; i++ )
    sum += ( samples[ i ] * samples[ i ] );

  return sum;
}


float audiobeam_do_beamforming( struct audiobeam_PreprocessedDelays
                                preprocessed_delays[  ],
                                float **sample_queue,
                                int queue_head,
                                long int max_delay,
                                int num_mic,
                                float *weights )
{
  int i;
  float sum = 0;
  int delay_floor;
  int delay_ceil;
  int low_index;
  int high_index;
  float interpolated_value;

  // add up all the num_mic delayed samples
  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < num_mic; i++ ) {
    delay_floor = preprocessed_delays[ i ].low;
    delay_ceil = preprocessed_delays[ i ].high;

    // Inline wrap around here
    // Low index gets index of sample right before desired sample
    low_index = queue_head + delay_floor;
    if ( low_index > max_delay )
      low_index -= ( max_delay + 1 );

    // High index gets index of sample right after desired sample
    high_index = queue_head + delay_ceil;
    if ( high_index > max_delay )
      high_index -= ( max_delay + 1 );

    // i gives the value of the microphone we want. However, since
    // the array only has microphones first_mic to last_mic, we
    // need to offset our index by first_mic

    interpolated_value = ( ( ( sample_queue[ high_index ][ i ] -
                               sample_queue[ low_index ][ i ] )
                             * ( preprocessed_delays[ i ].offset ) )
                           + sample_queue[ low_index ][ i ] );

    // If we have microphone weights, multiply the value by the weight
    if ( weights != 0 )
      sum += ( interpolated_value * weights[ i ] );
    else
      sum += interpolated_value;
  }

  return sum;

}


int audiobeam_process_signal( struct audiobeam_Delays *delays, int num_mic,
                              float sampling_rate, float **beamform_results,
                              struct audiobeam_DataQueue *queue,
                              int num_beams, int window, float *weights )
{
  int i, j;
  float time_index = 0;
  float time_index_inc = ( 1.0 / sampling_rate );

  float value;

  int done = 0;

  struct audiobeam_PreprocessedDelays preprocessed_delays[ 15 ];

  audiobeam_preprocess_delays( preprocessed_delays, delays->delay_values[ 0 ] );

  _Pragma( "loopbound min 13 max 13" )
  for ( i = 0; i < delays->max_delay - 1; i++ ) {
    if ( audiobeam_input_pos < 5760 )
      audiobeam_parse_line( ( queue->sample_queue )[ queue->head ], 15 );
    else
      return -1;
    queue->head = audiobeam_wrapped_inc( queue->head, delays->max_delay );
  }
  _Pragma( "loopbound min 371 max 371" )
  for ( i = 0; ( i < window ) || ( window < 0 ) ; i++ ) {
    if ( audiobeam_input_pos < 5760 )
      audiobeam_parse_line( ( queue->sample_queue )[ queue->head ], 15 );
    else {
      done = 1;
      break;
    }

    _Pragma( "loopbound min 1 max 1" )
    for ( j = 0; j < num_beams; j++ ) {
      value = audiobeam_do_beamforming( preprocessed_delays,
                                        ( queue->sample_queue ),
                                        audiobeam_wrapped_inc( queue->head,
                                            delays->max_delay ),
                                        delays->max_delay, num_mic, weights );


      value = value / num_mic;

      if ( beamform_results != 0 )
        beamform_results[ j ][ i ] = value;
    }

    queue->tail = queue->head;
    queue->head = audiobeam_wrapped_inc( queue->head, delays->max_delay );

    time_index += time_index_inc;
  }

  return ( done );
}


int audiobeam_calc_beamforming_result( struct audiobeam_Delays *delays,
                                       float **beamform_results,
                                       float *energies,
                                       struct audiobeam_DataQueue *queue,
                                       int num_beams, int window,
                                       int hamming )
{
  int i;
  int done;
  float *weights = 0;

  if ( hamming ) {
    if ( ( 15 % 2 ) == 1 )
      weights = audiobeam_calc_weights_left_only( 15 );
    else
      weights = audiobeam_calc_weights_lr( 15 );
  }

  done = audiobeam_process_signal( delays, 15, 16000,
                                   beamform_results,
                                   queue, num_beams, window, weights );

  if ( beamform_results != 0 ) {
    _Pragma( "loopbound min 1 max 1" )
    for ( i = 0; i < num_beams; i++ )
      energies[ i ] = audiobeam_calculate_energy( beamform_results[ i ], window );
  }
  return done;
}


void audiobeam_calc_single_pos( float source_location[ 3 ],
                                float audiobeam_mic_locations[ 15 ][ 3 ],
                                int hamming )
{
  float mic_distances[ 15 ];
  struct audiobeam_Delays *delays = audiobeam_init_delays( 1, 15 );
  struct audiobeam_DataQueue *queue;

  float **beamform_results;
  float *energies;

  beamform_results = ( float ** ) audiobeam_malloc( 1 * sizeof( float * ) );
  beamform_results[ 0 ] = ( float * ) audiobeam_malloc( 384 * sizeof( float ) );
  energies = ( float * ) audiobeam_malloc( 1 * sizeof( float * ) );

  // Calculate distances from source to each of mics
  audiobeam_calc_distances( source_location, audiobeam_mic_locations,
                            mic_distances, 15 );

  audiobeam_calc_delays( mic_distances,
                         delays->delay_values[ 0 ],
                         342, 16000, 15 );

  audiobeam_adjust_delays( delays->delay_values[ 0 ], 15 );

  delays->max_delay = audiobeam_find_max_in_arr ( delays->delay_values[ 0 ], 15 );

  queue = audiobeam_init_data_queue( delays->max_delay, 15 );

  audiobeam_calc_beamforming_result( delays, beamform_results,
                                     energies, queue, 1, -1, hamming );

  audiobeam_checksum += beamform_results[ 0 ][ 0 ] * 1000;
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) audiobeam_main( void )
{
  char hamming = 1;
  audiobeam_calc_single_pos( audiobeam_source_location,
                             audiobeam_mic_locations,
                             hamming );
}


int main( void )
{
  audiobeam_init();
  audiobeam_main();

  return ( audiobeam_return() );
}

