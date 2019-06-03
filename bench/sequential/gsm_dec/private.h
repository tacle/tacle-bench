#ifndef GSM_DEC_PRIVATE_H
#define GSM_DEC_PRIVATE_H

typedef short     word;   /* 16 bit signed int  */
typedef long      longword; /* 32 bit signed int  */

typedef unsigned short    uword;    /* unsigned word  */
typedef unsigned long   ulongword;  /* unsigned longword  */

/*   Table 4.6   Normalized direct mantissa used to compute xM/xmax
*/
/* i  0      1      2      3      4      5      6      7   */
word gsm_dec_FAC[ 8 ] = {
  18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767
};

struct gsm_state {
  word    dp0[  280  ];
  word    z1;   /* preprocessing.c, Offset_com. */
  longword  L_z2;   /*                  Offset_com. */
  int   mp;   /*                  Preemphasis */
  word   u[ 8 ];   /* short_term_aly_filter.c  */
  word    LARpp[ 2 ][ 8 ];  /*                              */
  word    j;    /*                              */
  word    nrp; /* 40 */ /* long_term.c, synthesis */
  word    v[ 9 ];   /* short_term.c, synthesis  */
  word    msr;    /* decoder.c, Postprocessing  */
};


#define MIN_WORD  ((-32767)-1)
#define MAX_WORD  ( 32767)

#define MIN_LONGWORD  ((-2147483647)-1)
#define MAX_LONGWORD  ( 2147483647)

/* >> is a signed arithmetic shift right */
#define SASR(x, by) ((x) >> (by))

/*   Table 4.3b   Quantization levels of the LTP gain quantizer
*/
/* bc         0          1        2          3      */
word gsm_dec_QLB[ 4 ] = {  3277,    11469,  21299,     32767  };

#endif /* GSM_DEC_PRIVATE_H */
