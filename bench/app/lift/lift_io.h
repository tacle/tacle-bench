#ifndef LIFT_IO_H_WVAZIMT8
#define LIFT_IO_H_WVAZIMT8

#define IN_PINS 10
#define OUT_PINS 4
#define ADC_PINS 4
#define LED_PINS 14

/* Global variables */
extern int ctrl_io_in[IN_PINS];
extern int ctrl_io_out[OUT_PINS];
extern int ctrl_io_analog[ADC_PINS];
extern int ctrl_io_led[LED_PINS];
extern int ctrl_dly1, ctrl_dly2;

/* Simulated hardware */
extern volatile int simio_in;
extern volatile int simio_out;
extern volatile int simio_led;
extern volatile int simio_adc1, simio_adc2, simio_adc3;

/* prototypes */
void io_init(void);
void ctrl_get_vals(void);
void ctrl_set_vals(void);

#endif /* end of include guard: LIFT_IO_H_WVAZIMT8 */
