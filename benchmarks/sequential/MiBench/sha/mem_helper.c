#include "mem_helper.h"

/* _wordcopy_fwd_aligned -- Copy block beginning at SRCP to
   block beginning at DSTP with LEN `op_t' words (not LEN bytes!).
   Both SRCP and DSTP should be aligned for memory operations on `op_t's.
*/

void _wordcopy_fwd_aligned(long int dstp, long int srcp, size_t len)
{
  op_t a0 = 0;
  op_t a1 = 0;
  char switch_target;
  
  switch (len % 8) {
    case 2:
      a0 = ((op_t *) srcp)[0];
      srcp -= 6 * OPSIZ;
      dstp -= 7 * OPSIZ;
      len += 6;
      switch_target = 1; 
      break;
    case 3:
      a1 = ((op_t *) srcp)[0];
      srcp -= 5 * OPSIZ;
      dstp -= 6 * OPSIZ;
      len += 5;
      switch_target = 2; 
      break;
    case 4:
      a0 = ((op_t *) srcp)[0];
      srcp -= 4 * OPSIZ;
      dstp -= 5 * OPSIZ;
      len += 4;
      switch_target = 3; 
      break;
    case 5:
      a1 = ((op_t *) srcp)[0];
      srcp -= 3 * OPSIZ;
      dstp -= 4 * OPSIZ;
      len += 3;
      switch_target = 4; 
      break;
    case 6:
      a0 = ((op_t *) srcp)[0];
      srcp -= 2 * OPSIZ;
      dstp -= 3 * OPSIZ;
      len += 2;
      switch_target = 5;
      break;
    case 7:
      a1 = ((op_t *) srcp)[0];
      srcp -= 1 * OPSIZ;
      dstp -= 2 * OPSIZ;
      len += 1;
      switch_target = 6; 
      break;
    case 0:
      if (OP_T_THRES <= 3 * OPSIZ && len == 0)
        return;
      a0 = ((op_t *) srcp)[0];
      srcp -= 0 * OPSIZ;
      dstp -= 1 * OPSIZ;
      switch_target = 7; 
      break;
    case 1:
      a1 = ((op_t *) srcp)[0];
      srcp -=-1 * OPSIZ;
      dstp -= 0 * OPSIZ;
      len -= 1;
      if (OP_T_THRES <= 3 * OPSIZ && len == 0) {
        ((op_t *) dstp)[0] = a1;
        return;
      } else {
        switch_target = 8;
        break;
      }
  }

  _Pragma("loopbound min 1 max 2")
  do {
    switch(switch_target) {
      case 8:
      a0 = ((op_t *) srcp)[0];
      ((op_t *) dstp)[0] = a1;
      case 7:
      a1 = ((op_t *) srcp)[1];
      ((op_t *) dstp)[1] = a0;
      case 6:
      a0 = ((op_t *) srcp)[2];
      ((op_t *) dstp)[2] = a1;
      case 5:
      a1 = ((op_t *) srcp)[3];
      ((op_t *) dstp)[3] = a0;
      case 4:
      a0 = ((op_t *) srcp)[4];
      ((op_t *) dstp)[4] = a1;
      case 3:
      a1 = ((op_t *) srcp)[5];
      ((op_t *) dstp)[5] = a0;
      case 2:
      a0 = ((op_t *) srcp)[6];
      ((op_t *) dstp)[6] = a1;
      case 1:
      a1 = ((op_t *) srcp)[7];
      ((op_t *) dstp)[7] = a0;
    }
    
    srcp += 8 * OPSIZ;
    dstp += 8 * OPSIZ;
    len -= 8;
    switch_target = 8;
  }
  while (len != 0);

  ((op_t *) dstp)[0] = a1;
}

/* _wordcopy_fwd_dest_aligned -- Copy block beginning at SRCP to
   block beginning at DSTP with LEN `op_t' words (not LEN bytes!).
   DSTP should be aligned for memory operations on `op_t's, but SRCP must
   *not* be aligned.  */

void _wordcopy_fwd_dest_aligned(void)
{
  // This whole method is never executed for our input data
  // it contained the same structure as _wordcopy_fwd_aligned
  // and thus we could not figure out the flowrestriction without
  // this method being executed. Therefore it was deleted.
}

