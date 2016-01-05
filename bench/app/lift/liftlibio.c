#include "liftlibio.h"

/* Global variables */
int lift_ctrl_io_in[10];
int lift_ctrl_io_out[4];
int lift_ctrl_io_analog[4];
int lift_ctrl_io_led[14];
int lift_ctrl_dly1;
int lift_ctrl_dly2;

/* Simulated hardware */
volatile int lift_simio_in;
volatile int lift_simio_out;
volatile int lift_simio_led;
volatile int lift_simio_adc1;
volatile int lift_simio_adc2;
volatile int lift_simio_adc3;

void lift_io_init()
{
  lift_ctrl_dly1 = 0;
  lift_ctrl_dly2 = 0;
}


void lift_ctrl_set_vals()
{
  int val = 0, i;
  _Pragma( "loopbound min 4 max 4" )
  for ( i = 4 - 1; i >= 0; --i ) {
    val <<= 1;
    val |= lift_ctrl_io_out[i] ? 1 : 0;
  }
  lift_simio_out = val;
  _Pragma( "loopbound min 14 max 14" )
  for ( i = 14 - 1; i >= 0; --i ) {
    val <<= 1;
    val |= lift_ctrl_io_led[i] ? 1 : 0;
  }
  lift_simio_led = val;
  lift_checksum += val;
}


void lift_ctrl_get_vals()
{
  int i;
  unsigned short int in0 = lift_simio_in;
  unsigned short int in1 = lift_ctrl_dly1;
  unsigned short int in2 = lift_ctrl_dly2;
  lift_ctrl_dly2 = lift_ctrl_dly1;
  lift_ctrl_dly1 = in0;
  /* majority voting for input values
     delays input value change by one period */
  _Pragma( "loopbound min 10 max 10" )
  for ( i = 0; i < 10; ++i ) {
    lift_ctrl_io_in[i] = ( ( in0 & 1 ) + ( in1 & 1 ) + ( in2 & 1 ) ) > 1;
    in0 >>= 1;
    in1 >>= 1;
    in2 >>= 1;
  }
  lift_ctrl_io_analog[0] = lift_simio_adc1;
  lift_ctrl_io_analog[1] = lift_simio_adc2;
  lift_ctrl_io_analog[2] = lift_simio_adc3;
}
