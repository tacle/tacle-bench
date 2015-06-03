
/*!
 ***********************************************************************
 * \file macroblock.c
 *
 * \brief
 *     Decode a Macroblock
 *
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *    - Inge Lille-Langøy               <inge.lille-langoy@telenor.com>
 *    - Rickard Sjoberg                 <rickard.sjoberg@era.ericsson.se>
 *    - Jani Lainema                    <jani.lainema@nokia.com>
 *    - Sebastian Purreiter             <sebastian.purreiter@mch.siemens.de>
 *    - Thomas Wedi                     <wedi@tnt.uni-hannover.de>
 *    - Detlev Marpe                    <marpe@hhi.de>
 *    - Gabi Blaettermann               <blaetter@hhi.de>
 *    - Ye-Kui Wang                     <wyk@ieee.org>
 *    - Lowell Winger                   <lwinger@lsil.com>
 ***********************************************************************
*/
//
//#include "contributors.h"
//
//#include <math.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <string.h>
//
#include "global.h"
//#include "mbuffer.h"
//#include "elements.h"
//#include "errorconcealment.h"
#include "macroblock.h"
//#include "fmo.h"
//#include "cabac.h"
//#include "vlc.h"
//#include "image.h"
//#include "mb_access.h"
//#include "biaridecod.h"
//
//#include "transform8x8.h"
////
//#if TRACE
//#define TRACE_STRING(s) strncpy(currSE.tracestring, s, TRACESTRING_SIZE)
//#else
//#define TRACE_STRING(s) // do nothing
//#endif
//
//extern int last_dquant;
//extern ColocatedParams *Co_located;
// Input data
extern signed char mv_array[65][65][1];
extern short list_imgUV[2][45][45];
extern int img_m7[16][16];

// Output data
char img_mpr[7][7];
char dec_picture_imgUV[2][64][54];

#define min(a,b)   ((a>b) ? b : a)
#define max(a,b)   ((a<b) ? b : a)
#define Clip1_Chr(a) ((a)<0?0:(a))


/*!
***********************************************************************
 \brief
    decode one macroblock
***********************************************************************
*/
int decode_one_macroblock(struct img_par *img,struct inp_par *inp)
{
//
//  int tmp_block[BLOCK_SIZE][BLOCK_SIZE];
//  int tmp_blockbw[BLOCK_SIZE][BLOCK_SIZE];
  int i=0,j=0,ii=0,jj=0,i1=0,j1=0,j4=0,i4=0;
  int uv;
//  int vec1_x=0,vec1_y=0,vec2_x=0,vec2_y=0;
  int ioff,joff;
//  int block8x8;   // needed for ABT
//
  int bw_pred=0, fw_pred=0, pred, ifx;
  int ii0,jj0,ii1,jj1,if1,jf1,if0,jf0;
  int f1_x, f1_y, f2_x, f2_y, f3, f4;
//
//  const byte decode_block_scan[16] = {0,1,4,5,2,3,6,7,8,9,12,13,10,11,14,15};
//
//  Macroblock *currMB   = &img->mb_data[img->current_mb_nr];
  short ref_idx, fw_refframe=-1, bw_refframe=-1;
  int mv_mode, pred_dir, intra_prediction; // = currMB->ref_frame;
  short fw_ref_idx=-1, bw_ref_idx=-1;
//
//  short  *** mv_array, ***fw_mv_array, ***bw_mv_array;
//
//  int mv_scale;
//
  int mb_nr     = 0;
  short dec_picture_ref_idx = 0;

  short active_sps_chroma_format_idc = 1;
  short active_pps_weighted_pred_flag = 0;
  short active_pps_weighted_bipred_idc = 0;

  int smb       = 0; // ((img->type==SP_SLICE) && IS_INTER (currMB)) || (img->type == SI_SLICE && currMB->mb_type == SI4MB);
  int list_offset = 0;
  int max_y_cr = 287;
//
//  StorablePicture **list;
//
  int jf;
//
//  char fw_rFrame=-1,bw_rFrame=-1;
//
//  short pmvfw[2]={0,0},
//        pmvbw[2]={0,0};              
//
  int direct_pdir=-1;
//
  int curr_mb_field = 0;
//
//  byte  **    moving_block;
//  short ****  co_located_mv;
//  char  ***   co_located_ref_idx;
//  int64 ***   co_located_ref_id;
//
//  int need_4x4_transform = (!currMB->luma_transform_size_8x8_flag);
//
  int b8, b4;
  int uv_shift;
  int yuv = 0; //dec_picture->chroma_format_idc - 1;
//
//  // Residue Color Transform
  int residue_transform_flag = 0; // img->residue_transform_flag;
  
  // find out the correct list offsets
  // SNIP.

  //if (dec_picture->chroma_format_idc != YUV400)
  if ( 1 )
  {
    // chroma decoding *******************************************************
    f1_x = 64/img->mb_cr_size_x;
    f2_x=f1_x-1;

    f1_y = 64/img->mb_cr_size_y;
    f2_y=f1_y-1;

    f3=f1_x*f1_y;
    f4=f3>>1;

    _Pragma("loopbound min 2 max 2")
    for(uv=0;uv<2;uv++)
    {
      uv_shift = uv*(img->num_blk8x8_uv/2);
      //intra_prediction = IS_INTRA (currMB);
      // Typical for input input_base_4CIF_96bps.264
      intra_prediction = 0;

//      if (intra_prediction)
//      {
//        intrapred_chroma(img, uv);
//      }

      _Pragma("loopbound min 1 max 1")
      for (b8=0;b8<(img->num_blk8x8_uv/2);b8++)
      {
        _Pragma("loopbound min 4 max 4")
        for(b4=0;b4<4;b4++)
        {
          joff = 0; // subblk_offset_y[yuv][b8][b4];
          j4=img->pix_c_y+joff;
          ioff = 0; // subblk_offset_x[yuv][b8][b4];
          i4=img->pix_c_x+ioff;

          mv_mode  = 1; // currMB->b8mode[block8x8_idx[yuv][b8][b4]];
          pred_dir = -1; // currMB->b8pdir[block8x8_idx[yuv][b8][b4]];
//          assert (pred_dir<=2);


          if (!intra_prediction)
          {
            if (pred_dir != 2)
            {
              //--- FORWARD/BACKWARD PREDICTION ---
//              mv_array = dec_picture->mv[LIST_0 + pred_dir];
//              list = listX[0+list_offset+pred_dir];
              _Pragma("loopbound min 4 max 4")
              for(jj=0;jj<4;jj++)
              {
                jf=((j4+jj)/(img->mb_cr_size_y/4))%64;     // jf  = Subblock_y-coordinate
                _Pragma("loopbound min 4 max 4")
                for(ii=0;ii<4;ii++)
                {
                  ifx=((i4+ii)/(img->mb_cr_size_x/4))%64;  // ifx = Subblock_x-coordinate
                  fw_refframe = ref_idx = 0; // dec_picture->ref_idx[LIST_0+pred_dir][jf][ifx];
                  i1=(i4+ii)*f1_x+mv_array[jf][ifx][0];

                  if (!curr_mb_field) {
                    j1=(j4+jj)*f1_y+mv_array[jf][ifx][1];
                  }
                  else
                  {
                    if (mb_nr%2 == 0) {
                      j1=((img->pix_c_y/2)                   + jj + joff)*f1_y + mv_array[jf][ifx][1];
                    }
                    else {
                      j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + mv_array[jf][ifx][1];
                    }
                    ++mb_nr;
                  }

                  //if (active_sps->chroma_format_idc == 1)
                  if (active_sps_chroma_format_idc == 1)
                    j1 += 0; //list[ref_idx]->chroma_vector_adjustment;

                  ii0=(max (0, min (i1/f1_x,   img->width_cr-1)))%45;
                  jj0=(max (0, min (j1/f1_y,   max_y_cr)))%45;
                  ii1=(max (0, min ((i1+f2_x)/f1_x, img->width_cr-1)))%45;
                  jj1=(max (0, min ((j1+f2_y)/f1_y, max_y_cr)))%45;
                
                  if1=(i1 & f2_x);
                  jf1=(j1 & f2_y);
                  if0=f1_x-if1;
                  jf0=f1_y-jf1;
                
                  if (img->apply_weights)
                  {
                    pred = (if0*jf0*list_imgUV[uv][jj0][ii0]+
                            if1*jf0*list_imgUV[uv][jj0][ii1]+
                            if0*jf1*list_imgUV[uv][jj1][ii0]+
                            if1*jf1*list_imgUV[uv][jj1][ii1]+f4)/f3;

//                    if (((active_pps->weighted_pred_flag&&(img->type==P_SLICE|| img->type == SP_SLICE))||
//                      (active_pps->weighted_bipred_idc==1 && (img->type==B_SLICE))) && curr_mb_field)
//                    {
//                      ref_idx >>=1;
//                    }
//
//                    img->mpr[ii+ioff][jj+joff] = Clip1_Chr(((img->wp_weight[pred_dir][ref_idx][uv+1] * pred  + img->wp_round_chroma)>>img->chroma_log2_weight_denom) + img->wp_offset[pred_dir][ref_idx][uv+1]);
                  }
                  else
                  {
                    //img->mpr[ii+ioff][jj+joff]=(if0*jf0*list[ref_idx]->imgUV[uv][jj0][ii0]+
                    img_mpr[ii+ioff][jj+joff]=(if0*jf0*list_imgUV[uv][jj0][ii0]+
                                                if1*jf0*list_imgUV[uv][jj0][ii1]+
                                                if0*jf1*list_imgUV[uv][jj1][ii0]+
                                                if1*jf1*list_imgUV[uv][jj1][ii1]+f4)/f3;
                  }
                }
              }
            }
            else
            {
              //fw_mv_array = dec_picture->mv[LIST_0];
              //bw_mv_array = dec_picture->mv[LIST_1];

              _Pragma("loopbound min 4 max 4")
              for(jj=0;jj<4;jj++)
              {
                jf=(j4+jj)/(img->mb_cr_size_y/4);     // jf  = Subblock_y-coordinate
                _Pragma("loopbound min 4 max 4")
                for(ii=0;ii<4;ii++)
                {
                  ifx=(i4+ii)/(img->mb_cr_size_x/4);  // ifx = Subblock_x-coordinate
                  direct_pdir = 2;

                  if (mv_mode == 0 && img->direct_spatial_mv_pred_flag)
                  {
                    //===== DIRECT PREDICTION =====
                    //if (dec_picture->ref_idx[LIST_0][2*(jf/2)][(ifx/2)*2]!=-1)
                    if (dec_picture_ref_idx != -1 )
                    {
                      fw_refframe = 0; // dec_picture->ref_idx[LIST_0][2*(jf/2)][(ifx/2)*2];
                      fw_ref_idx = fw_refframe;
                    }
                    //if (dec_picture->ref_idx[LIST_1][2*(jf/2)][(ifx/2)*2]!=-1)
                    if (dec_picture_ref_idx !=-1)
                    {
                      bw_refframe = 0; // dec_picture->ref_idx[LIST_1][2*(jf/2)][(ifx/2)*2];
                      bw_ref_idx = bw_refframe;
                    }

                    //if      (dec_picture->ref_idx[LIST_1][2*(jf/2)][(ifx/2)*2]==-1) direct_pdir = 0;
                    if      (dec_picture_ref_idx==-1) direct_pdir = 0;
                    //else if (dec_picture->ref_idx[LIST_0][2*(jf/2)][(ifx/2)*2]==-1) direct_pdir = 1;
                    else if (dec_picture_ref_idx==-1) direct_pdir = 1;

                    if (direct_pdir == 0 || direct_pdir == 2)
                    {
                      //i1=(img->pix_c_x+ii+ioff)*f1_x+fw_mv_array[jf][ifx][0];
                      i1=(img->pix_c_x+ii+ioff)*f1_x+mv_array[jf][ifx][0];
                    
                      if (!curr_mb_field)
                      {
                        //j1=(img->pix_c_y+jj+joff)*f1_y+fw_mv_array[jf][ifx][1];
                        j1=(img->pix_c_y+jj+joff)*f1_y+mv_array[jf][ifx][1];
                      }
                      else
                      {
                        if (mb_nr%2 == 0) {
                          //j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + fw_mv_array[jf][ifx][1];
                          j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + mv_array[jf][ifx][1];
                        }
                        else {
                          //j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + fw_mv_array[jf][ifx][1];
                          j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + mv_array[jf][ifx][1];
                        }
                      }

                      //if (active_sps->chroma_format_idc == 1)
                      if (active_sps_chroma_format_idc == 1)
                        //j1 += listX[0+list_offset][fw_refframe]->chroma_vector_adjustment;
                        j1 += 0;
                    
                      ii0=(max (0, min (i1/f1_x,   img->width_cr-1)))%45;
                      jj0=(max (0, min (j1/f1_y,   max_y_cr)))%45;
                      ii1=(max (0, min ((i1+f2_x)/f1_x, img->width_cr-1)))%45;
                      jj1=(max (0, min ((j1+f2_y)/f1_y, max_y_cr)))%45;

                    
                      if1=(i1 & f2_x);
                      jf1=(j1 & f2_y);
                      if0=f1_x-if1;
                      jf0=f1_y-jf1;
                    
                      fw_pred=(if0*jf0*list_imgUV[uv][jj0][ii0]+
                               if1*jf0*list_imgUV[uv][jj0][ii1]+
                               if0*jf1*list_imgUV[uv][jj1][ii0]+
                               if1*jf1*list_imgUV[uv][jj1][ii1]+f4)/f3;
                    }
                    if (direct_pdir == 1 || direct_pdir == 2)
                    {
                      //i1=(img->pix_c_x+ii+ioff)*f1_x+bw_mv_array[jf][ifx][0];
                      i1=(img->pix_c_x+ii+ioff)*f1_x+mv_array[jf][ifx][0];
                    
                      if (!curr_mb_field)
                      {
                        //j1=(img->pix_c_y+jj+joff)*f1_y+bw_mv_array[jf][ifx][1];
                        j1=(img->pix_c_y+jj+joff)*f1_y+mv_array[jf][ifx][1];
                      }
                      else
                      {
                        if (mb_nr%2 == 0) {
                          //j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + bw_mv_array[jf][ifx][1];
                          j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + mv_array[jf][ifx][1];
                        }
                        else {
                          //j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + bw_mv_array[jf][ifx][1];
                          j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + mv_array[jf][ifx][1];
                        }
                      }
                      //if (active_sps->chroma_format_idc == 1)
                      if (active_sps_chroma_format_idc == 1)
                        //j1 += listX[1+list_offset][bw_refframe]->chroma_vector_adjustment;
                        j1 += 0;

                      ii0=(max (0, min (i1/f1_x,   img->width_cr-1)))%45;
                      jj0=(max (0, min (j1/f1_y,   max_y_cr)))%45;
                      ii1=(max (0, min ((i1+f2_x)/f1_x, img->width_cr-1)))%45;
                      jj1=(max (0, min ((j1+f2_y)/f1_y, max_y_cr)))%45;
                    
                      if1=(i1 & f2_x);
                      jf1=(j1 & f2_y);
                      if0=f1_x-if1;
                      jf0=f1_y-jf1;
                
                      bw_pred=(if0*jf0*list_imgUV[uv][jj0][ii0]+
                               if1*jf0*list_imgUV[uv][jj0][ii1]+
                               if0*jf1*list_imgUV[uv][jj1][ii0]+
                               if1*jf1*list_imgUV[uv][jj1][ii1]+f4)/f3;
                    }

                  }
                  else
                  {
                    //===== BI-DIRECTIONAL PREDICTION =====
                    fw_refframe = 0; //dec_picture->ref_idx[LIST_0][jf][ifx];
                    bw_refframe = 0; //dec_picture->ref_idx[LIST_1][jf][ifx];

                    fw_ref_idx = fw_refframe;
                    bw_ref_idx = bw_refframe;

                    //i1=(img->pix_c_x+ii+ioff)*f1_x+fw_mv_array[jf][ifx][0];
                    i1=(img->pix_c_x+ii+ioff)*f1_x+mv_array[jf][ifx][0];

                    if (!curr_mb_field)
                    {
                      //j1=(img->pix_c_y+jj+joff)*f1_y+fw_mv_array[jf][ifx][1];
                      j1=(img->pix_c_y+jj+joff)*f1_y+mv_array[jf][ifx][1];
                    }
                    else
                    {
                      if (mb_nr%2 == 0) {
                        //j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + fw_mv_array[jf][ifx][1];
                        j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + mv_array[jf][ifx][1];
                      }
                      else {
                        //j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + fw_mv_array[jf][ifx][1];
                        j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + mv_array[jf][ifx][1];
                      }
                    }

                    //if (active_sps->chroma_format_idc == 1)
                    if (active_sps_chroma_format_idc == 1)
                      //j1 += listX[0+list_offset][fw_refframe]->chroma_vector_adjustment;
                      j1 += 0;

                    ii0=(max (0, min (i1/f1_x,   img->width_cr-1)))%45;
                    jj0=(max (0, min (j1/f1_y,   max_y_cr)))%45;
                    ii1=(max (0, min ((i1+f2_x)/f1_x, img->width_cr-1)))%45;
                    jj1=(max (0, min ((j1+f2_y)/f1_y, max_y_cr)))%45;
                  
                    if1=(i1 & f2_x);
                    jf1=(j1 & f2_y);
                    if0=f1_x-if1;
                    jf0=f1_y-jf1;
                  
                    fw_pred=(if0*jf0*list_imgUV[uv][jj0][ii0]+
                             if1*jf0*list_imgUV[uv][jj0][ii1]+
                             if0*jf1*list_imgUV[uv][jj1][ii0]+
                             if1*jf1*list_imgUV[uv][jj1][ii1]+f4)/f3;
                  
                    //i1=(img->pix_c_x+ii+ioff)*f1_x+bw_mv_array[jf][ifx][0];
                    i1=(img->pix_c_x+ii+ioff)*f1_x+mv_array[jf][ifx][0];
                  
                    if (!curr_mb_field)
                    {
                      //j1=(img->pix_c_y+jj+joff)*f1_y+bw_mv_array[jf][ifx][1];
                      j1=(img->pix_c_y+jj+joff)*f1_y+mv_array[jf][ifx][1];
                    }
                    else
                    {
                      if (mb_nr%2 == 0) {
                        //j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + bw_mv_array[jf][ifx][1];
                        j1=((img->pix_c_y)/2                   + jj + joff)*f1_y + mv_array[jf][ifx][1];
                      }
                      else {
                        //j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + bw_mv_array[jf][ifx][1];
                        j1=((img->pix_c_y-img->mb_cr_size_y)/2 + jj + joff)*f1_y + mv_array[jf][ifx][1];
                      }
                    }

                    //if (active_sps->chroma_format_idc == 1)
                    if (active_sps_chroma_format_idc == 1)
                      j1 += 0; // listX[1+list_offset][bw_refframe]->chroma_vector_adjustment;

                    ii0=(max (0, min (i1/f1_x,   img->width_cr-1)))%45;
                    jj0=(max (0, min (j1/f1_y,   max_y_cr)))%45;
                    ii1=(max (0, min ((i1+f2_x)/f1_x, img->width_cr-1)))%45;
                    jj1=(max (0, min ((j1+f2_y)/f1_y, max_y_cr)))%45;
                  
                    if1=(i1 & f2_x);
                    jf1=(j1 & f2_y);
                    if0=f1_x-if1;
                    jf0=f1_y-jf1;
                  
                    bw_pred=(if0*jf0*list_imgUV[uv][jj0][ii0]+
                             if1*jf0*list_imgUV[uv][jj0][ii1]+
                             if0*jf1*list_imgUV[uv][jj1][ii0]+
                             if1*jf1*list_imgUV[uv][jj1][ii1]+f4)/f3;
                  }

                  if (img->apply_weights)
                  {
                    //if (((active_pps->weighted_pred_flag&&(img->type==P_SLICE|| img->type == SP_SLICE))||
                    //  (active_pps->weighted_bipred_idc==1 && (img->type==B_SLICE))) && curr_mb_field)
                    if (((active_pps_weighted_pred_flag&&(img->type==P_SLICE|| img->type == SP_SLICE))||
                      (active_pps_weighted_bipred_idc==1 && (img->type==B_SLICE))) && curr_mb_field)
                    {
                      fw_ref_idx >>=1;
                      bw_ref_idx >>=1;
                    }

                    if (img->direct_spatial_mv_pred_flag && direct_pdir==1)
                    {
                      img->mpr[ii+ioff][jj+joff]= Clip1_Chr((img->wp_round_chroma)>>img->chroma_log2_weight_denom) + 0;   // Replaced with integer only operations
                    }
                    else if (img->direct_spatial_mv_pred_flag && direct_pdir==0)
                    {
                      img->mpr[ii+ioff][jj+joff]=Clip1_Chr(((img->wp_round_chroma)>>img->chroma_log2_weight_denom));   // Replaced with integer only operations
                    }
                    else
                    {
                      //int wt_list_offset = (active_pps->weighted_bipred_idc==2)?list_offset:0;
                      int wt_list_offset = (active_pps_weighted_bipred_idc==2)?list_offset:0;

                      int alpha_fw = 0; // img->wbp_weight[0+wt_list_offset][fw_ref_idx][bw_ref_idx][uv+1];
                      int alpha_bw = 0; // img->wbp_weight[1+wt_list_offset][fw_ref_idx][bw_ref_idx][uv+1];

                      img->mpr[ii+ioff][jj+joff]= Clip1_Chr((alpha_fw * fw_pred + alpha_bw * bw_pred  + (1<<img->chroma_log2_weight_denom)) >> (img->chroma_log2_weight_denom + 1));
                    }
                  }
                  else
                  {
                    if (img->direct_spatial_mv_pred_flag && direct_pdir==1)
                    {
                      img->mpr[ii+ioff][jj+joff]=bw_pred;
                    }
                    else if (img->direct_spatial_mv_pred_flag && direct_pdir==0)
                    {
                      img->mpr[ii+ioff][jj+joff]=fw_pred;
                    } 
                    else
                    {
                      img->mpr[ii+ioff][jj+joff]=(fw_pred + bw_pred + 1 )/2;
                    }
                  }
                }
              }
            }
          } //if (!intra_prediction)

          if (!smb)
          {
//            itrans(img,ioff,joff, cofuv_blk_x[yuv][b8+uv_shift][b4], cofuv_blk_y[yuv][b8+uv_shift][b4], 1);
            _Pragma("loopbound min4 max 4")
            for(ii=0;ii<4;ii++) {
              jj=0;
              _Pragma("loopbound min4 max 4")
              for(;jj<4;jj++)
              {
                // Residue Color Transform
                if(!residue_transform_flag)
                {
                  //dec_picture->imgUV[uv][j4+jj][i4+ii]=img->m7[ii][jj];
                  // Accessed restricted due to reduced size of output data.
                  dec_picture_imgUV[uv][(j4+jj)%64][(i4+ii)%54]=img_m7[ii][jj];
                } 
               // else
               // {
               //   mprRGB[uv+1][ii+ioff][jj+joff] = img->mpr[ii+ioff][jj+joff];
               //   rec_res[uv+1][ii+ioff][jj+joff] = img->m7[ii][jj];
               // }
              }
            }
          }
        }
      }
    
      if(smb)
      {
//        itrans_sp_chroma(img,2*uv);
        _Pragma("loopbound min 2 max 2")
        for (j=4;j<6;j++)
        {
          joff=(j-4)*4;
          j4=img->pix_c_y+joff;
          _Pragma("loopbound min 2 max 2")
          for(i=0;i<2;i++)
          {
            ioff=i*4;
            i4=img->pix_c_x+ioff;
//            itrans(img,ioff,joff,2*uv+i,j, 1);

            _Pragma("loopbound min 4 max 4")
            for(ii=0;ii<4;ii++)
              _Pragma("loopbound min 4 max 4")
              for(jj=0;jj<4;jj++)
              {
                //dec_picture->imgUV[uv][j4+jj][i4+ii]=img->m7[ii][jj];
                // Accessed restricted due to reduced size of output data.
                dec_picture_imgUV[uv][(j4+jj)%64][(i4+ii)%54]=img_m7[ii][jj];
              }
          }
        }
      }
    }  
  }

 // Residue Color Transform: Code excluded

  return 0;
}

int main( void )
{
  struct img_par img;
  img.mb_cr_size_x = 8;
  img.mb_cr_size_y = 8;
  img.num_blk8x8_uv = 2;
  img.pix_c_x = 256;
  img.pix_c_y = 256;
  img.width_cr = 352;
  img.apply_weights = 0;
  img.direct_spatial_mv_pred_flag = 1;
  img.type = 1;
  img.wp_round_chroma = 0;
  img.chroma_log2_weight_denom = 0;
  
  decode_one_macroblock( &img, 0 );

  return 0;
}
  
