#ifndef AUDIOBEAM_MAIN_H
#define AUDIOBEAM_MAIN_H

struct audiobeam_DataQueue {
  float **sample_queue;
  int head;
  int tail;
  unsigned char full;
};


struct audiobeam_Delays {
  float **delay_values;
  long int max_delay;
};


struct audiobeam_PreprocessedDelays {
  float delay;
  int   low;
  int   high;
  float offset;
};


#undef FLT_MAX
#define FLT_MAX 999e999

#define SOUND_SPEED 342
#define SAMPLING_RATE 16000
#define CARTESIAN_DISTANCE(x1,y1,z1,x2,y2,z2) (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2)));

#define NUM_MIC 15
#define ANGLE_ENERGY_WINDOW_SIZE 400
#define GRID_STEP_SIZE 0.003  // .3cm
#define NUM_DIRS 7
#define NUM_TILES 16

#define MIC_HORIZ_SPACE 0.038257
#define MIC_VERT_SPACE 0.015001
#define TWO23        8388608.0    // 2^23
#define BUFFER_SIZE 384 // No of input-tupels (each with NUM_MIC elements)
#define NUM_MIC_IN_CHAIN 32
#define NUM_BOARDS_IN_CHAIN 16
#define INPUT_LENGTH 5760

#define INTERPOLATE(low_value, high_value, offset) (((high_value-low_value)*(offset)) + low_value)

#endif

