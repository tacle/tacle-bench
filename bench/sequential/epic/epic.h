#ifndef __EPIC_H_
#define __EPIC_H_

#define EPIC_VERSION 1.1

/* ============= FUNDAMENTAL LIMITATIONS ============= */

/* Maximum x- or y-size of image */
#define MAX_IMAGE_DIM 16384

/* Maximum number of pyramid levels (value 3*levs+1 stored in 5 bits).
   This doesn't need to be larger than log2(MAX_IMAGE_DIM/FILTER_SIZE). */
#define MAX_LEVELS 10

/* Maximum number of quantization bins.  This essentially determines
   the maximum depth image to be represented. */
#define MAX_BINS 511

/* ============= SECONDARY (derived) LIMITATIONS ============= */

/* This number determines the precision of the stored binsizes:
   stored coefficients are accurate to +/- (1/SCALE_FACTOR).
   On the other hand, this number also will limit the maximum amount
   of compression.
   It should not be more than [ 2^(8*sizeof(BinValueType)) ]/256. */
#define SCALE_FACTOR 128

/* This number must be consistent with the filters that are
   hardwired into epic.c */
#define FILTER_SIZE 15

/* Log (base 2) of MAX_IMAGE_DIM^2: (bits required to store the dimensions) */
#define LOG_MAX_IMAGE_SIZE 32

/* The type of the quantized images. Must be SIGNED, and capable of holding
   values  in the range [ -MAX_BINS, MAX_BINS ] */
typedef short BinIndexType;

/* The type used to represent the binsizes. Should be UNSIGNED. If this is
   changed, be sure to change the places in epic.c and unepic.c where
   binsizes are written or read from files.  */
typedef unsigned short BinValueType;

/* Number of possible values for a symbol.  This must be at least
   (MAX_BINS * 4)  (one sign bit, one tag bit)... */
#define NUM_SYMBOL_VALUES 65536

/* Function prototypes. */
void epic_build_pyr( float *image, int x_size, int y_size, int num_levels,
                     float *lo_filter, float *hi_filter, int filter_size );

void epic_build_level( float *image, int level_x_size, int level_y_size,
                       float *lo_filter, float *hi_filter,
                       int filter_size, float *result_block );

void epic_internal_transpose( register float *mat, int rows,
                              register int cols );

void epic_reflect1( register float *filt, register int x_dim, int y_dim,
                    int x_pos, int y_pos, register float *result, int f_or_e );

void epic_internal_filter( register float *image, register int x_dim,
                           register int y_dim, float *filt,
                           register float *temp, register int x_fdim,
                           register int y_fdim, int xgrid_start,
                           int xgrid_step, int ygrid_start, int ygrid_step,
                           register float *result );

void epic_reflect1( float *filt, int x_dim, int y_dim, int x_pos, int y_pos,
                    float *result, int f_or_e );

#endif // __EPIC_H_
