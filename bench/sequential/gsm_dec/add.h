#ifndef GSM_DEC_ADD_H
#define GSM_DEC_ADD_H

#define GSM_MULT_R(a, b) /* word a, word b, !(a == b == MIN_WORD) */  \
  (SASR( ((longword)(a) * (longword)(b) + 16384), 15 ))

# define GSM_MULT(a,b)   /* word a, word b, !(a == b == MIN_WORD) */  \
  (SASR( ((longword)(a) * (longword)(b)), 15 ))

# define GSM_L_MULT(a, b) /* word a, word b */  \
  (((longword)(a) * (longword)(b)) << 1)

# define GSM_L_ADD(a, b)  \
  ( (a) <  0 ? ( (b) >= 0 ? (a) + (b) \
     : (utmp = (ulongword)-((a) + 1) + (ulongword)-((b) + 1)) \
       >= MAX_LONGWORD ? MIN_LONGWORD : -(longword)utmp-2 )   \
  : ((b) <= 0 ? (a) + (b)   \
            : (utmp = (ulongword)(a) + (ulongword)(b)) >= MAX_LONGWORD \
        ? MAX_LONGWORD : utmp))

/*
   # define GSM_ADD(a, b) \
    ((ltmp = (longword)(a) + (longword)(b)) >= MAX_WORD \
    ? MAX_WORD : ltmp <= MIN_WORD ? MIN_WORD : ltmp)
*/
/* Nonportable, but faster: */

#define GSM_ADD(a, b) \
  ((ulongword)((ltmp = (longword)(a) + (longword)(b)) - MIN_WORD) > \
    MAX_WORD - MIN_WORD ? (ltmp > 0 ? MAX_WORD : MIN_WORD) : ltmp)

# define GSM_SUB(a, b)  \
  ((ltmp = (longword)(a) - (longword)(b)) >= MAX_WORD \
  ? MAX_WORD : ltmp <= MIN_WORD ? MIN_WORD : ltmp)

# define GSM_ABS(a) ((a) < 0 ? ((a) == MIN_WORD ? MAX_WORD : -(a)) : (a))

#define saturate(x)   \
  ((x) < MIN_WORD ? MIN_WORD : (x) > MAX_WORD ? MAX_WORD: (x))

/* Use these if necessary:

  # define GSM_MULT_R(a, b) gsm_mult_r(a, b)
  # define GSM_MULT(a, b)   gsm_mult(a, b)
  # define GSM_L_MULT(a, b) gsm_L_mult(a, b)

  # define GSM_L_ADD(a, b)  gsm_L_add(a, b)
  # define GSM_ADD(a, b)    gsm_add(a, b)
  # define GSM_SUB(a, b)    gsm_sub(a, b)

  # define GSM_ABS(a)   gsm_abs(a)

*/

#endif /* GSM_DEC_ADD_H */
