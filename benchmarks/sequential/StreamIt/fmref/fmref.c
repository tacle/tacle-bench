/*
* fmref.c: C reference implementation of FM Radio
* David Maze <dmaze@cag.lcs.mit.edu>
* $Id: fmref.c,v 1.2 2010-10-04 21:21:26 garus Exp $
*/

#include "wcclibm.h"
#ifndef M_PI
#define M_PI        3.1415926535897932384626433832795
#endif

// Defines
#define SAMPLING_RATE 250000000
#define CUTOFF_FREQUENCY 108000000
#define NUM_TAPS 64
#define MAX_AMPLITUDE 27000.0
#define BANDWIDTH 10000
#define DECIMATION 4 
/* Must be at least NUM_TAPS+1: */
#define IN_BUFFER_LEN 200
#define EQUALIZER_BANDS 10


// Type declarations
typedef struct FloatBuffer
{
  float buff[IN_BUFFER_LEN];
  int rpos, rlen;
}
FloatBuffer;
/* Low pass filter: */
typedef struct LPFData
{
  float coeff[NUM_TAPS];
  float freq;
  int taps, decimation;
}
LPFData;
typedef struct EqualizerData
{
  LPFData lpf[EQUALIZER_BANDS + 1];
  FloatBuffer fb[EQUALIZER_BANDS + 1];
  float gain[EQUALIZER_BANDS];
}
EqualizerData;

// Global vars
float lpf_coeff[NUM_TAPS];
float eq_cutoffs[EQUALIZER_BANDS + 1] =
  { 55.000004f, 77.78174f, 110.00001f, 155.56354f, 220.00002f, 311.12695f,
    440.00003f, 622.25415f, 880.00006f, 1244.5078f, 1760.0001f };
static int numiters = 2;

// Forward declarations
void fb_compact(FloatBuffer *fb);
int fb_ensure_writable(FloatBuffer *fb, int amount);
void get_floats(FloatBuffer *fb);
void init_lpf_data(LPFData *data, float freq, int taps, int decimation);
void run_lpf(FloatBuffer *fbin, FloatBuffer *fbout, LPFData *data);
void run_demod(FloatBuffer *fbin, FloatBuffer *fbout);
void begin(void);
void init_equalizer(EqualizerData *data);
void run_equalizer(FloatBuffer *fbin, FloatBuffer *fbout, EqualizerData *data);

int main(void)
{
  begin();
  return 0;
}

FloatBuffer fb1, fb2, fb3, fb4;
LPFData lpf_data;

void begin(void)
{
  int i;
  EqualizerData eq_data;

  fb1.rpos = fb1.rlen = 0;
  fb2.rpos = fb2.rlen = 0;
  fb3.rpos = fb3.rlen = 0;
  fb4.rpos = fb4.rlen = 0;

  init_lpf_data(&lpf_data, CUTOFF_FREQUENCY, NUM_TAPS, DECIMATION);
  init_equalizer(&eq_data);

  /* Startup: */
  get_floats(&fb1);
  /* LPF needs at least NUM_TAPS+1 inputs; get_floats is fine. */
  run_lpf(&fb1, &fb2, &lpf_data);
  /* run_demod needs 1 input, OK here. */
  /* run_equalizer needs 51 inputs (same reason as for LPF).  This means
   * running the pipeline up to demod 50 times in advance: */
  _Pragma( "loopbound min 64 max 64" )
  for (i = 0; i < 64; i++) {
    if (fb1.rlen - fb1.rpos < NUM_TAPS + 1)
      get_floats(&fb1);
    run_lpf(&fb1, &fb2, &lpf_data);
    run_demod(&fb2, &fb3);
  }

  /* Main loop: */
  _Pragma( "loopbound min 2 max 2" )
  while (numiters-- > 0) {
    /* The low-pass filter will need NUM_TAPS+1 items; read them if we
     * need to. */
    if (fb1.rlen - fb1.rpos < NUM_TAPS + 1)
      get_floats(&fb1);
    run_lpf(&fb1, &fb2, &lpf_data);
    run_demod(&fb2, &fb3);
    run_equalizer(&fb3, &fb4, &eq_data);
    
  }
}

void fb_compact(FloatBuffer *fb)
{
  
  int i;
  char *source;
  char *target;
  target = (char*)(fb->buff);
  source = (char*)(fb->buff + fb->rpos);
  _Pragma( "loopbound min 0 max 60" )
  for (i = 0; i < fb->rlen - fb->rpos; i++) {
    target[i] = source[i];
  }
  fb->rlen -= fb->rpos;
  fb->rpos = 0;
}

int fb_ensure_writable(FloatBuffer *fb, int amount)
{
  int available = IN_BUFFER_LEN - fb->rlen;
  if (available >= amount)
    return 1;

  /* Nope, not enough room, move current contents back to the beginning. */
  fb_compact(fb);

  available = IN_BUFFER_LEN - fb->rlen;
  if (available >= amount)
    return 1;

  return 0;
}

void get_floats(FloatBuffer *fb)
{
  static int x = 0;
  fb_compact(fb);

  /* Fill the remaining space in fb with 1.0. */
  _Pragma( "loopbound min 200 max 200" )
  while (fb->rlen < IN_BUFFER_LEN) {
    fb->buff[fb->rlen++] = (float)x;
    x++;
  }
}

void init_lpf_data(LPFData *data, float freq, int taps, int decimation)
{
  /* Assume that CUTOFF_FREQUENCY is non-zero.  See comments in
   * StreamIt LowPassFilter.java for origin. */
  float w = 2 * M_PI * freq / SAMPLING_RATE;
  int i;
  float m = taps - 1.0f;

  data->freq = freq;
  data->taps = taps;
  data->decimation = decimation;

  _Pragma( "loopbound min 64 max 64" )
  for (i = 0; i < taps; i++) {
    if (i - m / 2 == 0.0f)
      data->coeff[i] = w / M_PI;
    else
      data->coeff[i] = sin(w * (i - m / 2)) / M_PI / (i - m / 2) *
                       (0.54f - 0.46f * cos(2 * M_PI * i / m));
  }
}

void run_lpf(FloatBuffer *fbin, FloatBuffer *fbout, LPFData *data)
{
  float sum = 0.0f;
  int i = 0;

  _Pragma( "loopbound min 64 max 64" )
  for (i = 0; i < data->taps; i++) {
    sum += fbin->buff[fbin->rpos + i] * data->coeff[i];
  }

  fbin->rpos += data->decimation + 1;

  /* Check that there's room in the output buffer; move data if necessary. */
  fb_ensure_writable(fbout, 1);
  fbout->buff[fbout->rlen++] = sum;
}

void run_demod(FloatBuffer *fbin, FloatBuffer *fbout)
{
  float temp, gain;
  gain = MAX_AMPLITUDE * SAMPLING_RATE / (BANDWIDTH * M_PI);
  temp = fbin->buff[fbin->rpos] * fbin->buff[fbin->rpos + 1];
  temp = gain * atan(temp);
  fbin->rpos++;
  fb_ensure_writable(fbout, 1);
  fbout->buff[fbout->rlen++] = temp;
}

void init_equalizer(EqualizerData *data)
{
  int i;

  /* Equalizer structure: there are ten band-pass filters, with
   * cutoffs as shown below.  The outputs of these filters get added
   * together.  Each band-pass filter is LPF(high)-LPF(low). */
  _Pragma( "loopbound min 11 max 11" )
  for (i = 0; i < EQUALIZER_BANDS + 1; i++)
    init_lpf_data(&data->lpf[i], eq_cutoffs[i], 64, 0);

  /* Also initialize member buffers. */
  _Pragma( "loopbound min 11 max 11" )
  for (i = 0; i < EQUALIZER_BANDS + 1; i++)
    data->fb[i].rpos = data->fb[i].rlen = 0;

  _Pragma( "loopbound min 10 max 10" )
  for (i = 0; i < EQUALIZER_BANDS; i++) {
    // the gain amplifies the middle bands the most
    float val = (((float)i) - (((float)(EQUALIZER_BANDS - 1)) / 2.0f)) / 5.0f;
    data->gain[i] = val > 0 ? 2.0f - val : 2.0f + val;
  }
}

void run_equalizer(FloatBuffer *fbin, FloatBuffer *fbout, EqualizerData *data)
{
  int i, rpos;
  float lpf_out[EQUALIZER_BANDS + 1];
  float sum = 0.0;

  /* Save the input read location; we can reuse the same input data on all
   * of the LPFs. */
  rpos = fbin->rpos;

  /* Run the child filters. */
  _Pragma( "loopbound min 11 max 11" )
  for (i = 0; i < EQUALIZER_BANDS + 1; i++) {
    fbin->rpos = rpos;
    run_lpf(fbin, &data->fb[i], &data->lpf[i]);
    lpf_out[i] = data->fb[i].buff[data->fb[i].rpos++];
  }

  /* Now process the results of the filters.  Remember that each band is
   * output(hi)-output(lo). */
   _Pragma( "loopbound min 10 max 10" )
  for (i = 0; i < EQUALIZER_BANDS; i++)
    sum += (lpf_out[i + 1] - lpf_out[i]) * data->gain[i];

  /* Write that result.  */
  fb_ensure_writable(fbout, 1);
  fbout->buff[fbout->rlen++] = sum;
}

