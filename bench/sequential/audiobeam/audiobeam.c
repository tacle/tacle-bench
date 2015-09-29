#include "audiobeam.h"

extern float input[INPUT_LENGTH];
int input_pos = 0;

// 15-microphone array for data set from Kevin
extern float mic_locations[NUM_MIC][3];
// For Kevin's data
extern float source_location[3];
// center of the array
extern float origin_location[3];


void preprocess_delays(struct PreprocessedDelays prep_delays[], float* delays)
{
  int i;

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < NUM_MIC; i++)
  {
    prep_delays[i].delay = delays[i];
    prep_delays[i].high = (int) ceil(delays[i]);
    prep_delays[i].low = (int) floor(delays[i]);
    prep_delays[i].offset = delays[i] - prep_delays[i].low;
  }
}

/******************************  HELPER FUNCTIONS *************************/
// Parse a line of input samples. Samples are just floats separated by spaces, as follows:
//  -1.8569790e-004 -9.0919049e-004  3.6711283e-004 -1.0073081e-005 ...
// One line per sample, one float per microphone
float* parse_line(float* float_arr, int num_mic)
{
  int i;
  // WCC change: This now just copies from the input array

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < num_mic; i++) {
    float_arr[i] = input[input_pos++];
  }

  return float_arr;
}

// Find the maximum element in an array of floats. The return type is
// a long int... this is because we usually use this to calculate the
// maximum delay for a particular array, and we want that number to be
// the ceiling of the fractional maximum delay.
long int find_max_in_arr(float *arr, int size)
{
  int i;
  float max = 0;

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < size; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }

  return ceil(max);
}

// Find the minimum element in an array of floats. The return type is
// a long int... this is because we usually use this to calculate the
// minimum delay for a particular array, and we want that number to be
// the floor of the fractional minimum delay.
long int find_min_in_arr(float *arr, int size)
{
  int i;
  float min = FLT_MAX;

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < size; i++) {
    if (arr[i] < min ) {
      min = arr[i];
    }
  }

  return floor(min);
}


// Increment with wrap, i.e. if after adding offset to i, i exceeds
// max_i, wrap it around. For example, if i is 8, offset is 12, and
// max_i is 10, the result will come out to be (8+12-10-1) = 9. The
// -1 is because we have to include 0.
int wrapped_inc_offset(int i, int offset, int max_i)
{
  if (i + offset > max_i) {
    return (i + offset - max_i - 1);
  } else {
    return (i + offset);
  }
}

// Decrement with wrap, i.e. if after subtracting offset from i, i is
// less than 0, wrap it around from the other side. For example, if i
// is 8, offset is 12, and max_i is 10, the result will come out to be
// (10-(12-8)+1) = 7. The +1 is because we have to include 0.
int wrapped_dec_offset(int i, int offset, int max_i)
{
  if (i - offset < 0) {
    return (max_i -(offset - i) + 1);
  } else {
    return (i -offset);
  }
}


// Increment with wrap, i.e. if index exceeds max index, wrap back to
// 0
int wrapped_inc(int i, int max_i)
{
  return wrapped_inc_offset(i, 1, max_i);
}


// Decrement with wrap, i.e. if index is below 0, wrap back to max
int wrapped_dec(int i, int max_i)
{
  return wrapped_dec_offset(i, 1, max_i);
}

// Interpolate between two sample values to avoid quantization
// error. The idea is that we have a fractional delay, but obviously
// since the signal is sampled, we can only get real values for the
// signal at whole delay values. So, we can use linear approximation
// to estimate what the value of the signal would have been at the
// fractional delay.
float interpolate(float low_value, float high_value, float offset)
{
  return ((high_value - low_value) * (offset)) + low_value;
}

/******************************  DATA STRUCTURE INITIALIZATION *************************/

// Initialize a DataQueue struct
struct DataQueue* init_data_queue(int max_delay, int num_mic)
{
  int i, j;

  struct DataQueue *queue;
  // Initialize an array big enough to store max_delay+1 samples We
  // want it to be max_delay+1, so that we can keep max_delay
  // previous samples and the current sample


  queue = (struct DataQueue *) wccmalloc(sizeof(struct DataQueue));
  queue->sample_queue = (float**) wccmalloc((max_delay + 1)*sizeof(float*));

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < (max_delay + 1);i++) {
    (queue->sample_queue)[i] = (float*) wccmalloc(num_mic * sizeof(float));
    _Pragma( "loopbound min 15 max 15" )
    for (j = 0; j < num_mic; j++) {
      (queue->sample_queue)[i][j] = 0.0; // Initialize values to 0
    }
  }

  queue->head = 0;
  queue->tail = 0;
  queue->full = 0;

  return queue;
}


// Intitialize the delay struct
struct Delays* init_delays (int num_angles, int num_mic)
{
  struct Delays *delays;
  int i;

  delays = (struct Delays *) wccmalloc(sizeof(struct Delays));

  // Initialize the delays array
  delays->delay_values = (float**) wccmalloc(num_angles*sizeof(float*));

  _Pragma( "loopbound min 1 max 1" )
  for (i = 0; i < (num_angles); i++) {
    delays->delay_values[i] = (float*) wccmalloc(num_mic * sizeof(float));
  }

  return delays;
}

/******************************  DELAY/WEIGHTS/ENERGY MATH *************************/



// Calculate the cartesian distances from the source to each of the
// microphones
void calc_distances(float *source_location, float mic_locations[NUM_MIC][3],
               float *distances,
               int num_mic)
{
  int i;

  float source_x = source_location[0];
  float source_y = source_location[1];

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < num_mic; i++) {
    distances[i] = CARTESIAN_DISTANCE(mic_locations[i][0], mic_locations[i][1], mic_locations[i][2],
                                      source_location[0], source_location[1], source_location[2]);
  }
}


// Calculate the delay (in # of samples) for each one of the microphones
void calc_delays(float *distances, float *delays, int sound_speed, int sampling_rate, int num_mic)
{
  int i;

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < num_mic; i++) {
    delays[i] = (distances[i] / sound_speed) * sampling_rate;
  }
}


// Adjust the delays by subtracting the smallest delay from each
// delay. This is done in order to minimize the size of our queue.
void adjust_delays(float* delays, int num_mic)
{
  int i;
  long int min_delay = find_min_in_arr (delays, num_mic) - 1;

  _Pragma( "loopbound min 15 max 15" )
  for (i = 0; i < num_mic; i++) {
    delays[i] -= min_delay;
  }
}


// Calculate weights according to the Hamming window:
// an = 0.54+0.46cos(pi*n/N)
// where there are 2N+1 microphones
float *calc_weights_lr (int num_mic)
{
  float* weights = (float*) wccmalloc(num_mic * sizeof(float));
  int index = 0;
  int y, z;

  int half = num_mic / 4;

  _Pragma( "loopbound min 0 max 0" )
  for (z = 1; z >= -1; z -= 2) {
    _Pragma( "loopbound min 0 max 0" )
    for (y = 0; y < half; y++) {
      weights[index] = 0.54 + 0.46 * cos(M_PI * y / half);
      index++;
    }
    _Pragma( "loopbound min 0 max 0" )
    for (y = 0; y < half; y++) {
      weights[index] = 0.54 + 0.46 * cos(M_PI * ( -y) / half);
      index++;
    }
  }

  return weights;
}


// Calculate weights according to the Hamming window:
// an = 0.54+0.46cos(pi*n/N)
// where there are 2N+1 microphones
float *calc_weights_left_only (int num_mic)
{
  float* weights = (float*) wccmalloc(num_mic * sizeof(float));
  int index = 0;
  int y;

  int half = num_mic / 2;

  _Pragma( "loopbound min 15 max 15" )
  for (y = -half; y <= half; y++) {
    weights[index] = 0.54 + 0.46 * cos(M_PI * y / half);
    index++;
  }

  return weights;
}



// Calculate the energy in a signal by summing the squares of the values
float calculate_energy(float* samples, int num_samples)
{
  int i;
  float sum = 0.0;

  _Pragma( "loopbound min 0 max 0" )
  for (i = 0; i < num_samples; i++) {
    sum += (samples[i] * samples[i]);
  }

  return sum;
}

/******************************  BEAMFORMING LOOKUP *************************/


// Figure out the first and last microphone index for this tile to process
void figure_which_mics(int tile_num, int* first_mic, int* last_mic)
{
  // Get the "slice" of microphones for this tile to work on. We
  // want to get the ceil of our share of microphones, and the last
  // tile may get less than the others in case the number of
  // microphones does not divide evenly by the number of tiles on
  // the chip

  int slice_size = ceil(((float)NUM_MIC) / ((float)NUM_TILES));

  *first_mic = tile_num * slice_size;
  *last_mic = (tile_num + 1) * slice_size - 1;

  if (*last_mic >= NUM_MIC) {
    *last_mic = NUM_MIC - 1;
  }
}

// Take in a sample and a queue of previous samples, and output a
// beamformed one-channel sample, incorporating timing information
// from max_delay previous samples
float do_beamforming(struct PreprocessedDelays preprocessed_delays[],
                     float **sample_queue,
                     int queue_head, long int max_delay, int num_mic, float* weights)
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
  for (i = 0; i < num_mic; i++) {
    delay_floor = preprocessed_delays[i].low;
    delay_ceil = preprocessed_delays[i].high;

    // Inline wrap around here
    // Low index gets index of sample right before desired sample
    low_index = queue_head + delay_floor;
    if (low_index > max_delay) {
      low_index -= (max_delay + 1);
    }

    // High index gets index of sample right after desired sample
    high_index = queue_head + delay_ceil;
    if (high_index > max_delay) {
      high_index -= (max_delay + 1);
    }

    // i gives the value of the microphone we want. However, since
    // the array only has microphones first_mic to last_mic, we
    // need to offset our index by first_mic

    interpolated_value = INTERPOLATE(sample_queue[low_index][i],
                                     sample_queue[high_index][i],
                                     preprocessed_delays[i].offset);

    // If we have microphone weights, multiply the value by the weight
    if (weights != 0) {
      sum += (interpolated_value * weights[i]);
    } else {
      sum += interpolated_value;
    }
  }

  return sum;

}

void set_mic_location(float locations[NUM_MIC][3], int index, float x, float y, float z)
{
  locations[index][0] = x;
  locations[index][1] = y;
  locations[index][2] = z;
}


/******************************  RUNTIME LOOP *************************/

// Read in lines of the signal and run the beamforming algorithms on
// it.  The main idea is that we have a circular queue of samples. It
// is implemented via an array of max_delay+1 samples, so that we can
// keep max_delay previous samples and the current sample. Once the
// queue is "full", that is, we have max_delay+1 samples in it, we can
// start outputting beamformed results (delay each microphone by the
// appropriate amount by reading that amount into the future).
// Return 1 if end of data, 0 if still more to come!
int process_signal(struct Delays *delays, int num_mic,
                   float sampling_rate, float** beamform_results,
                   struct DataQueue *queue,
                   int num_beams, int window, float* weights)
{
  long int max_delay = 0;
  int i, j;
  int queue_head = 0;
  unsigned char queue_full = 0;
  float time_index = 0;
  float time_index_inc = (1.0 / sampling_rate);

  float value;

  int done = 0;

  // Pre-calulated ceiling, floor and offset values for each delay
  struct PreprocessedDelays preprocessed_delays[NUM_MIC];

  // Pre-calculate delay values
  preprocess_delays(preprocessed_delays, delays->delay_values[0]);

  i = 0;

  // RMR { prime the sample queue (fill in max_delay-1 entries, the last entry is filled in the mail loop)
  _Pragma( "loopbound min 13 max 13" )
  for (i = 0; i < delays->max_delay - 1; i++) {
    if (input_pos < INPUT_LENGTH) {
      parse_line( (queue->sample_queue)[queue->head], NUM_MIC );
    } else {
      return -1;
    }
    queue->head = wrapped_inc(queue->head, delays->max_delay);
  }
  // } RMR

  // If window is <0, keep going until EOF
  _Pragma( "loopbound min 371 max 371" )
  for (i = 0; (i < window) || (window < 0) ; i++) {
    if (input_pos < INPUT_LENGTH) {
      parse_line((queue->sample_queue)[queue->head], NUM_MIC);
    } else {
      done = 1;
      break;
    }

    //        if (queue->full == 1) {
    // output beamformed signal for max_delay samples back (in
    // other words, the sample that's located at
    // (queue->head+1) -- the next sample to get overwritten )

    // Note: we are using wrapped_inc(queue->head,max_delay)
    // rather than queue->head+1 since we don't want to overrun
    // the end of the array, i.e. if queue->head is max_delay,
    // wrapped_inc(queue->head,max_delay) will be one
    //        printf("num_beams: %d\n",num_beams);
    _Pragma( "loopbound min 1 max 1" )
    for (j = 0; j < num_beams; j++) {
      value = do_beamforming(preprocessed_delays,
                             (queue->sample_queue),
                             wrapped_inc(queue->head, delays->max_delay),
                             delays->max_delay, num_mic, weights);


      // normalize value by number of microphones
      value = value / num_mic;
      // It's the jth beam at the ith sample

      if (beamform_results != 0) {
        beamform_results[j][i] = value;
      }
    }

    // Set the tail to the current head and increment the head by
    // one
    queue->tail = queue->head;
    queue->head = wrapped_inc(queue->head, delays->max_delay);

    time_index += time_index_inc;
  }

  return (done);
}


/******************************  EVALUATION LOGIC *************************/


// Calculate the beamforming result for a single point or
// direction. Returns if we are done or not
int calc_beamforming_result(struct Delays *delays,
                            float** beamform_results,
                            float* energies,
                            struct DataQueue *queue,
                            int num_beams, int window,
                            int hamming)
{
  int i;
  int done;
  float* weights = 0;

  if (hamming)
  {
    if ( (NUM_MIC % 2) == 1 ) {
      // Assume Odd numbers are only the left microphones
      weights = calc_weights_left_only(NUM_MIC);
    } else {
      weights = calc_weights_lr(NUM_MIC);
    }
  }

  done = process_signal(delays, NUM_MIC, SAMPLING_RATE,
                        beamform_results,
                        queue, num_beams, window, weights);

  if (beamform_results != 0)
  {
    _Pragma( "loopbound min 1 max 1" )
    for (i = 0; i < num_beams; i++) {
      energies[i] = calculate_energy(beamform_results[i], window);
    }
  }
  return done;
}


void calc_single_pos(float source_location[3], float mic_locations[NUM_MIC][3], int hamming)
{
  float mic_distances[NUM_MIC];
  // Intialize the delays struct for only one set of delays
  struct Delays *delays = init_delays(1, NUM_MIC);
  struct DataQueue *queue;

  float **beamform_results;
  float *energies;
  float *weights = 0;

  beamform_results = (float **) wccmalloc( 1 * sizeof( float* ) );
  beamform_results[0] = (float*) wccmalloc( BUFFER_SIZE * sizeof( float ) );
  energies = (float*) wccmalloc( 1 * sizeof( float* ) );

  // Calculate distances from source to each of mics
  calc_distances(source_location, mic_locations, mic_distances, NUM_MIC);
  calc_delays(mic_distances, delays->delay_values[0], SOUND_SPEED, SAMPLING_RATE, NUM_MIC);
  adjust_delays(delays->delay_values[0], NUM_MIC);

  delays->max_delay = find_max_in_arr (delays->delay_values[0], NUM_MIC);

  queue = init_data_queue(delays->max_delay, NUM_MIC);

  // number of beams = 1, energy window size = -1 results are NULL
  // because we don't necessarily know how many results there will
  // be in advance... since we are going to EOF
  calc_beamforming_result(delays, beamform_results,
                          energies, queue, 1, -1, hamming);
}


int main( void )
{
  char hamming = 1;
  calc_single_pos(source_location, mic_locations, hamming);

  return 0;
}
