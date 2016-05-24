#ifndef LIFTLIBIO_H
#define LIFTLIBIO_H

/* Global variables */
extern int lift_ctrl_io_in[10];
extern int lift_ctrl_io_out[4];
extern int lift_ctrl_io_analog[4];
extern int lift_ctrl_io_led[16];
extern int lift_ctrl_dly1, lift_ctrl_dly2;

/* Simulated hardware */
extern volatile int lift_simio_in;
extern volatile int lift_simio_out;
extern volatile int lift_simio_led;
extern volatile int lift_simio_adc1;
extern volatile int lift_simio_adc2;
extern volatile int lift_simio_adc3;

/* Checksum */
extern int lift_checksum;

/* prototypes */
void lift_io_init( void );
void lift_ctrl_get_vals( void );
void lift_ctrl_set_vals( void );

#endif
