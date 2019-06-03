#ifndef __H264DEC_H
#define __H264DEC_H

typedef enum {
  P_SLICE = 0,
  B_SLICE,
  I_SLICE,
  SP_SLICE,
  SI_SLICE
} h264_dec_SliceType;

// image parameters
typedef struct h264_dec_img_par {
  int direct_spatial_mv_pred_flag;
  int type;
  int width_cr;
  int pix_c_y;
  int pix_c_x;
  unsigned short mpr[ 16 ][ 16 ];
  unsigned int chroma_log2_weight_denom;
  int wp_round_chroma;
  unsigned int apply_weights;
  int num_blk8x8_uv;
  int mb_cr_size_x;
  int mb_cr_size_y;

} h264_dec_ImageParameters;

#endif
