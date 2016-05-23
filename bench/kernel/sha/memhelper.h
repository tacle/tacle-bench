/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: memhelper.h

  Author: Peter C. Gutmann's (heavily modified by Uwe Hollerbach)

  NIST Secure Hash Algorithm

  Source: Peter C. Gutmann's implementation as found in Applied Cryptography by Bruce Schneier

  License: GNU Lesser General Public License

  Changes: no major functional changes

*/

#ifndef MEM_HELPER_H
#define MEM_HELPER_H

#include "sha.h"

void sha_wordcopy_fwd_aligned( long int dstp, long int srcp, size_t len );

/* Type to use for aligned memory operations.
   This should normally be the biggest type supported by a single load
   and store.  */
#define op_t  unsigned long int
#define OPSIZ (sizeof(op_t))

/* Threshold value for when to enter the unrolled loops.  */
#define OP_T_THRES  16

#define PAGE_COPY_FWD_MAYBE(dstp, srcp, nbytes_left, nbytes)
#define MERGE(w0, sh_1, w1, sh_2) (((w0) >> (sh_1)) | ((w1) << (sh_2)))


/* Copy *up to* NBYTES bytes from SRC_BP to DST_BP, with
   the assumption that DST_BP is aligned on an OPSIZ multiple.  If
   not all bytes could be easily copied, store remaining number of bytes
   in NBYTES_LEFT, otherwise store 0.  */
#define WORD_COPY_FWD(dst_bp, src_bp, nbytes_left, nbytes)          \
    {                       \
      if (src_bp % OPSIZ == 0)                  \
  sha_wordcopy_fwd_aligned (dst_bp, src_bp, (nbytes) / OPSIZ);        \
      else                      \
      src_bp += (nbytes) & -OPSIZ;                \
      dst_bp += (nbytes) & -OPSIZ;                \
      (nbytes_left) = (nbytes) % OPSIZ;               \
    }

#endif  // MEM_HELPER_H

