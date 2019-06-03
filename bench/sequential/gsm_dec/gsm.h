#ifndef GSM_DEC_GSM_H
#define GSM_DEC_GSM_H

/*
    Interface
*/
typedef struct gsm_state *gsm;
typedef short gsm_signal; /* signed 16 bit */
typedef unsigned char gsm_byte;
typedef gsm_byte gsm_frame[ 33 ]; /* 33 * 8 bits   */

#define GSM_MAGIC 0xD /* 13 kbit/s RPE-LTP */

#endif /* GSM_DEC_GSM_H */
