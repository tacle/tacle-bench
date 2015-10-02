#include <stdio.h>
#include <stdint.h>
#include "lift_io.h"

/* Global variables */
int ctrl_io_in[IN_PINS];
int ctrl_io_out[OUT_PINS];
int ctrl_io_analog[ADC_PINS];
int ctrl_io_led[LED_PINS];
int ctrl_dly1, ctrl_dly2;

/* Simulated hardware */
volatile int simio_in;
volatile int simio_out;
volatile int simio_led;
volatile int simio_adc1, simio_adc2, simio_adc3;

void io_init()
{
  ctrl_dly1 = 0;
  ctrl_dly2 = 0;
}

void ctrl_set_vals()
{
  int val = 0,i;
  _Pragma("loopbound min 4 max 4")
  for (i=OUT_PINS-1; i>=0; --i) {
    val <<= 1;
    val |= ctrl_io_out[i] ? 1 : 0;
  }
  simio_out = val;
  _Pragma("loopbound min 14 max 14")
  for (i=LED_PINS-1; i>=0; --i) {
    val <<= 1;
    val |= ctrl_io_led[i] ? 1 : 0;
  }
  simio_led = val;
}

void ctrl_get_vals()
{
  int i;
  uint16_t in0 = simio_in;
  uint16_t in1 = ctrl_dly1;
  uint16_t in2 = ctrl_dly2;
  ctrl_dly2 = ctrl_dly1;
  ctrl_dly1 = in0;
  /* majority voting for input values
     delays input value change by one period */
  _Pragma("loopbound min 10 max 10")
  for (i=0; i<IN_PINS; ++i) {
    ctrl_io_in[i] = ((in0&1) + (in1&1) + (in2&1)) > 1;
    in0 >>= 1;
    in1 >>= 1;
    in2 >>= 1;
  }
  ctrl_io_analog[0] = simio_adc1;
  ctrl_io_analog[1] = simio_adc2;
  ctrl_io_analog[2] = simio_adc3;
}
