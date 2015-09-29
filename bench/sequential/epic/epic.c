

///// epic.h ////
////#include "epic.h"
/*
---------------------------------------------------------------------
---		 EPIC (Efficient Pyramid Image Coder)             ---
---	 Designed by Eero P. Simoncelli and Edward H. Adelson     ---
---		    Written by Eero P. Simoncelli                 ---
---  Developed at the Vision Science Group, The Media Laboratory  ---
---	Copyright 1989, Massachusetts Institute of Technology     ---
---			 All rights reserved.                     ---
---------------------------------------------------------------------

Permission to use, copy, or modify this software and its documentation
for educational and research purposes only and without fee is hereby
granted, provided that this copyright notice appear on all copies and
supporting documentation.  For any other uses of this software, in
original or modified form, including but not limited to distribution
in whole or in part, specific prior permission must be obtained from
M.I.T. and the authors.  These programs shall not be used, rewritten,
or adapted as the basis of a commercial software or hardware product
without first obtaining appropriate licenses from M.I.T.  M.I.T. makes
no representations about the suitability of this software for any
purpose.  It is provided "as is" without express or implied warranty.

---------------------------------------------------------------------
*/

//extern float image[65536];
extern float image[];

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
   of compression.  It should not be more than [2^(8*sizeof(BinValueType))]/256. */
#define SCALE_FACTOR 128

/* This number must be consistent with the filters that are
   hardwired into epic.c */
#define FILTER_SIZE 15

/* Log (base 2) of MAX_IMAGE_DIM^2: (bits required to store the dimensions) */
#define LOG_MAX_IMAGE_SIZE 32

/* The type of the quantized images. Must be SIGNED, and capable of holding
   values  in the range [-MAX_BINS, MAX_BINS] */
typedef short BinIndexType;

/* The type used to represent the binsizes. Should be UNSIGNED. If this is
   changed, be sure to change the places in epic.c and unepic.c where
   binsizes are written or read from files.  */
typedef unsigned short BinValueType;

/* Number of possible values for a symbol.  This must be at least
   (MAX_BINS * 4)  (one sign bit, one tag bit)... */
#define NUM_SYMBOL_VALUES 65536

/* The symbols encoded by the Huffman coder.  Should be unsigned,
   and capable of holding NUM_SYMBOL_VALUES.
   If this is changed, change the places in huffman.c, and unepic.c
   where symbols are written or read from files. */
typedef unsigned short SymbolType;

typedef unsigned char Byte;

/* the data type of the encoded stream */
typedef Byte CodeType;

/* ============= FILE BYTE TAGS =============== */
#define EPIC_ID_TAG   '\377'   /* eight bits on */
#define BIN_INFO_TAG  '\200'    /* high bit is 1 */

/* coded data block tags: high bit must be 0! */
#define RAW_DATA_TAG        '\000'
#define HUFFMAN_DATA_TAG    '\001'

/* ============== FILE IO MACROS ============== */
/*  We write everything as bytes for portability. For the            */
/*  files to be machine independent, we write the bytes of           */
/*  longer quantities (like shorts and ints) in a fixed order.       */
/*  WE ASSUME THAT:                                                  */
/*     - short quantities are at least 2 bytes.                      */
/*     - int quantitites are at least 4 bytes.                       */
/*  **** The arrays are written incorrectly and need to be fixed!    */
/*  Temporary variables are defined in utilities.c                   */

/* If non-zero, unepic will write a PGM file.  Otherwise, writes a   */
/* raw byte file.  */
#define WRITE_PGM_FILE 1

#define write_byte(val, stream) temp_byte = (Byte) val; \
  fwrite(&temp_byte,1,1,stream)
#define write_short(val,stream) temp_short = (unsigned short) val; \
  temp_byte = (temp_short & 0xFF00)>>8; fwrite(&temp_byte,1,1,stream); \
  temp_byte = (temp_short & 0x00FF); fwrite(&temp_byte,1,1,stream)
#define write_int(val,stream) temp_int = (unsigned int) val; \
  temp_byte = (temp_int & 0xFF000000)>>24; fwrite(&temp_byte,1,1,stream); \
  temp_byte = (temp_int & 0x00FF0000)>>16; fwrite(&temp_byte,1,1,stream); \
  temp_byte = (temp_int & 0x0000FF00)>>8;  fwrite(&temp_byte,1,1,stream); \
  temp_byte = (temp_int & 0x000000FF);     fwrite(&temp_byte,1,1,stream)
#define write_array(ptr, size, stream) fwrite( ptr, 1, sizeof(*ptr)*size, stream)

#define read_byte(sym, stream) fread(&temp_byte,1,1,stream); sym=temp_byte
#define read_short(sym, stream) \
  fread(&temp_byte,1,1,stream); temp_short = temp_byte; temp_short <<=8; \
  fread(&temp_byte,1,1,stream); temp_short |= temp_byte; sym=temp_short
#define read_int(sym, stream) \
  fread(&temp_byte,1,1,stream); temp_int  = temp_byte; temp_int <<= 8; \
  fread(&temp_byte,1,1,stream); temp_int |= temp_byte; temp_int <<= 8; \
  fread(&temp_byte,1,1,stream); temp_int |= temp_byte; temp_int <<= 8; \
  fread(&temp_byte,1,1,stream); temp_int |= temp_byte; sym = temp_int;
#define read_array(ptr, size, stream)  fread(ptr, 1, sizeof(*ptr) * size, stream)

extern Byte temp_byte;
extern short temp_short;
extern int temp_int;

extern float *ReadMatrixFromPGMStream();

/* ============= FUNCTION DECLARATIONS ============= */
/*    These functions are defined in utilities.c     */
//#include <stdio.h>
extern char *check_malloc();
//extern FILE *check_fopen();
extern char *concatenate();


/* ============= STRUCTURES ============== */

struct code_node   /* A node of a huffman tree used for decoding  */
  {
  struct code_node *zero_child, *one_child;
  SymbolType symbol;
  };

extern struct code_node *read_huffman_tree();
//end epic.h ///
//#include "epic_data.h"

// convolve.h //////
//#include "convolve.h"
/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  File: convolve.h
;;;  Author: Simoncelli
;;;  Description: Header file for convolve.c
;;;  Creation Date:
;;;  ----------------------------------------------------------------
;;;    Object-Based Vision and Image Understanding System (OBVIUS),
;;;      Copyright 1988, Vision Science Group,  Media Laboratory,
;;;              Massachusetts Institute of Technology.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define abs(x)	  (x>=0 ? x : -(x))
#define ROOT2 1.4142135623730951f
#define FILTER 0
#define EXPAND 1
#define IS    ==
#define ISNT  !=
#define AND &&
#define OR ||

typedef  int (*fptr)();

typedef struct
  {
  char *name;
  fptr func;
  } EDGE_HANDLER;

fptr edge_function();
//int internal_filter(), internal_expand();


/* Local Variables: */
/* buffer-read-only: t */
/* End: */
//end convolve.h ////


#define NUM_LEVELS	4

#define SCALE_FACTOR    128

/* Function prototypes. */
void build_pyr(float *image, int x_size, int y_size, int num_levels, float *lo_filter,
    float *hi_filter, int filter_size );

void build_level(float *image, int level_x_size, int level_y_size, float *lo_filter,
    float *hi_filter, int filter_size, float *result_block);

void internal_transpose( register float *mat, int rows, register int cols );

void reflect1(register float *filt, register int x_dim, int y_dim, int x_pos, int y_pos,
    register float *result, int f_or_e);

void internal_filter(register float *image, register int x_dim, register int y_dim, float *filt,
    register float * temp, register int x_fdim, register int y_fdim, int xgrid_start,
    int xgrid_step, int ygrid_start, int ygrid_step, register float *result, char *edges);

/////// build_pyr.c
/*
---------------------------------------------------------------------
---		 EPIC (Efficient Pyramid Image Coder)             ---
---	 Designed by Eero P. Simoncelli and Edward H. Adelson     ---
---		    Written by Eero P. Simoncelli                 ---
---  Developed at the Vision Science Group, The Media Laboratory  ---
---	Copyright 1989, Massachusetts Institute of Technology     ---
---			 All rights reserved.                     ---
---------------------------------------------------------------------

Permission to use, copy, or modify this software and its documentation
for educational and research purposes only and without fee is hereby
granted, provided that this copyright notice appear on all copies and
supporting documentation.  For any other uses of this software, in
original or modified form, including but not limited to distribution
in whole or in part, specific prior permission must be obtained from
M.I.T. and the authors.  These programs shall not be used, rewritten,
or adapted as the basis of a commercial software or hardware product
without first obtaining appropriate licenses from M.I.T.  M.I.T. makes
no representations about the suitability of this software for any
purpose.  It is provided "as is" without express or implied warranty.

---------------------------------------------------------------------
*/


#define X_SIZE	64
#define Y_SIZE	64

float filtertemp[FILTER_SIZE];
float hi_imagetemp[X_SIZE*Y_SIZE/2];
float lo_imagetemp[X_SIZE*Y_SIZE/2];

/*
======================================================================
build_pyr() -- builds a separable QMF-style pyramid.  The pyramid is
written over the original image.  NOTE: the image size must be
divisible by 2^num_levels, but we do not check this here.
======================================================================
*/
void build_pyr( float *image, int x_size, int y_size, int num_levels, float *lo_filter, float *hi_filter, int filter_size)
{
  int x_level, y_level, level;

  x_level = x_size;
  y_level = y_size;

  _Pragma("loopbound min 4 max 4")
    for (level = 0;  level < num_levels; ++level)
    build_level(image, x_level, y_level, lo_filter, hi_filter, filter_size, image);
  x_level /= 2;
  y_level /= 2;
}

/*
======================================================================
build_level() -- builds a level of the pyramid by computing 4
filtered and subsampled images.  Since the convolution is separable,
image and result-block can point to the same place!  Image order is
lowpass, horizontal, vertical (transposed), and diagonal.
======================================================================
*/
void build_level(float *image, int level_x_size, int level_y_size, float *lo_filter, float *hi_filter,
	    int filter_size, float *result_block)
{
  //  float *hi_imagetemp, *lo_imagetemp, *filtertemp;
  int total_size = level_x_size*level_y_size;

  //  filtertemp = (float *) check_malloc (filter_size*sizeof(float));
  //  hi_imagetemp = (float *) check_malloc (total_size*sizeof(float)/2);
  //  lo_imagetemp = (float *) check_malloc (total_size*sizeof(float)/2);
  /* filter and subsample in the X direction */
  internal_filter (image, level_x_size, level_y_size,
      lo_filter, filtertemp, filter_size, 1,
      0, 2, 0, 1, lo_imagetemp, "reflect1");
  internal_filter (image, level_x_size, level_y_size,
      hi_filter, filtertemp, filter_size, 1,
      1, 2, 0, 1, hi_imagetemp, "reflect1");

  level_x_size /= 2;
  /* now filter and subsample in the Y direction */
  internal_filter (lo_imagetemp, level_x_size, level_y_size, /* lowpass */
      lo_filter, filtertemp, 1, filter_size,
      0, 1, 0, 2, result_block, "reflect1");
  internal_filter (lo_imagetemp, level_x_size, level_y_size, /* horizontal */
      hi_filter, filtertemp, 1, filter_size,
      0, 1, 1, 2, (result_block += (total_size/4)), "reflect1");
  internal_filter (hi_imagetemp, level_x_size, level_y_size,  /* vertical */
      lo_filter, filtertemp, 1, filter_size,
      0, 1, 0, 2, (result_block += (total_size/4)), "reflect1");
  /* transpose the vertical band for more efficient scanning */
  internal_transpose(result_block, level_y_size/2, level_x_size);
  internal_filter (hi_imagetemp, level_x_size, level_y_size,  /* diagonal */
      hi_filter, filtertemp, 1, filter_size,
      0, 1, 1, 2, (result_block += (total_size/4)), "reflect1");
  //  check_free ((char *) filtertemp);
  //  check_free ((char *) hi_imagetemp);
  //  check_free ((char *) lo_imagetemp);
}

/*
======================================================================
In-place matrix tranpose algorithm.  Handles non-square matrices,
too!  Is there a faster algorithm??
======================================================================
*/
void internal_transpose( float *mat, int rows, int cols )
{
  register int swap_pos;
  register int modulus = rows*cols - 1;
  register int current_pos;
  register float swap_val;

  /* loop, ignoring first and last elements */
  _Pragma("loopbound min 1022 max 2399")
    for (current_pos=1; current_pos<modulus; ++current_pos)
    {
      /* Compute swap position */
      swap_pos = current_pos;

      _Pragma("loopbound min 1 max 2")
	do
	{
	  swap_pos = (swap_pos * cols) % modulus;
	}
	while (swap_pos < current_pos);

      if ( current_pos != swap_pos )
      {
	swap_val = mat[swap_pos];
	mat[swap_pos] = mat[current_pos];
	mat[current_pos] = swap_val;
      }
    }
}
///////////////////////////////////////////////////////////

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  File: convolve.c
;;;  Author: Eero Simoncelli
;;;  Description: General convolution code for 2D images
;;;  Creation Date: Spring, 1987.
;;;  ----------------------------------------------------------------
;;;    Object-Based Vision and Image Understanding System (OBVIUS),
;;;      Copyright 1988, Vision Science Group,  Media Laboratory,
;;;              Massachusetts Institute of Technology.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

//extern int reflect1();

/* --------------------------------------------------------------------
  Correlate FILT with IMAGE, subsampling according to GRID parameters,
  with values placed into result array.  TEMP is a temporary
  array the size of the filter.  EDGES is a string -- see convolve.h.
  The convolution is done in 9 sections, where the border sections use
  specially computed edge-handling filters (see edges.c). The origin
  of the filter is assumed to be (floor(x_fdim/2), floor(y_fdim/2)).
  10/6/89 - approximately optimized the choice of register vars on SPARCS.
------------------------------------------------------------------------ */

void internal_filter(float *image, int x_dim, int y_dim, float *filt, float *temp, int x_fdim, int y_fdim,
		int xgrid_start, int xgrid_step, int ygrid_start, int ygrid_step, float * result, char *edges)
{

  //register double sum;
  register float sum;
  register int x_filt, im_pos, y_filt_lin;
  register int y_im_lin, x_pos, filt_size = x_fdim*y_fdim;
  register int y_pos, res_pos;
  register int last_ctr_col = x_dim - x_fdim;
  int last_ctr_row = (y_dim - y_fdim) * x_dim;
  int first_row, first_col;
  int x_fmid = x_fdim/2;
  int y_fmid = y_fdim/2;
  int x_stop = x_fdim - x_fmid + 1;
  int y_stop = y_fdim - y_fmid + 1;
  int ygrid_step_full = ygrid_step*x_dim;
  int prev_res_pos, x_res_dim = (x_dim-xgrid_start+xgrid_step-1)/xgrid_step;
  int rt_edge_res_pos;
  //  fptr reflect = edge_function(edges);  /* look up edge-handling function */

  /*  if (!reflect)
      {
      fprintf(stderr,"Unknown edge handler: %s\n",edges);
      return(-1);
      }
      */
  res_pos=0;

  _Pragma("loopbound min 1 max 4")
    for (y_pos=ygrid_start-y_fmid-1; y_pos<0; y_pos+=ygrid_step)
    {
      _Pragma("loopbound min 1 max 4")
	for (x_pos=xgrid_start-x_fmid;        /* top-left corner */
	    x_pos<1;
	    x_pos+=xgrid_step)
	{
	  //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  sum=0.0f;
	  x_filt=y_im_lin=0;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {

	      im_pos=y_im_lin;

	      _Pragma("loopbound min 1 max 15")
		for (; x_filt<y_filt_lin; ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
      first_col = x_pos+1;
      //    (*reflect)(filt,x_fdim,y_fdim,0,y_pos,temp,FILTER);
      reflect1(filt,x_fdim,y_fdim,0,y_pos,temp,FILTER);
      _Pragma("loopbound min 41 max 46")
	for (x_pos=first_col;	            /* top edge */
	    x_pos<last_ctr_col;
	    x_pos+=xgrid_step)
	{
	  sum=0.0f;
	  x_filt=y_im_lin=0;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {
	      im_pos=x_pos+y_im_lin;
	      _Pragma("loopbound min 1 max 15")
		for (; x_filt<y_filt_lin; ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
      rt_edge_res_pos = res_pos + x_res_dim;   /* save this for later ... */
      _Pragma("loopbound min 1 max 4")
	for (x_pos+=(1-last_ctr_col);         /* top-right corner */
	    x_pos<x_stop;
	    x_pos+=xgrid_step)
	{
	  //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  sum=0.0f;
	  x_filt=y_im_lin=0;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {

	      im_pos=y_im_lin+last_ctr_col;

	      _Pragma("loopbound min 1 max 15")
		for (; x_filt<y_filt_lin; ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
    }                        /* end top */

  first_row = x_dim*(y_pos+1);   /* need this to go down the sides */
  prev_res_pos = res_pos;
  _Pragma("loopbound min 1 max 4")
    for (x_pos=xgrid_start-x_fmid;           /* left edge */
	x_pos<1;
	x_pos+=xgrid_step)
    {
      res_pos = prev_res_pos;
      //    (*reflect)(filt,x_fdim,y_fdim,x_pos,0,temp,FILTER);
      reflect1(filt,x_fdim,y_fdim,x_pos,0,temp,FILTER);
      _Pragma("loopbound min 41 max 97")
	for (y_pos=first_row; y_pos<last_ctr_row; y_pos+=ygrid_step_full)
	{
	  sum=0.0f;
	  x_filt=0,y_im_lin=y_pos;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {
	      im_pos=y_im_lin;
	      _Pragma("loopbound min 1 max 15")
		for (; x_filt<y_filt_lin; x_filt++) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  res_pos+=x_res_dim;
	}
      prev_res_pos++;
    }
  //(*reflect)(filt,x_fdim,y_fdim,0,0,temp,FILTER);
  reflect1(filt,x_fdim,y_fdim,0,0,temp,FILTER);
  _Pragma("loopbound min 41 max 97")
    for (y_pos=first_row;		/* center region of image */
	y_pos<last_ctr_row;
	y_pos+=ygrid_step_full)
    {
      res_pos = prev_res_pos;
      _Pragma("loopbound min 41 max 46")
	for (x_pos=first_col;
	    x_pos<last_ctr_col;
	    x_pos+=xgrid_step)
	{
	  sum=0.0f;
	  x_filt=0,y_im_lin=y_pos;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim){
	      im_pos=x_pos+y_im_lin;
	      _Pragma("loopbound min 1 max 15")
		for (; x_filt<y_filt_lin; ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
      prev_res_pos+=x_res_dim;
    }
  prev_res_pos = rt_edge_res_pos;
  _Pragma("loopbound min 1 max 4")
    for (x_pos+=(1-last_ctr_col);                  /* right edge */
	x_pos<x_stop;
	x_pos+=xgrid_step)
    {
      res_pos = prev_res_pos;
      //      (*reflect)(filt,x_fdim,y_fdim,x_pos,0,temp,FILTER);
      reflect1(filt,x_fdim,y_fdim,x_pos,0,temp,FILTER);
      _Pragma("loopbound min 41 max 97")
	for (y_pos=first_row; y_pos<last_ctr_row; y_pos+=ygrid_step_full)
	{
	  sum=0.0f;
	  x_filt=0,y_im_lin=y_pos;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim;
		y_filt_lin<=filt_size;
		y_filt_lin+=x_fdim) {
	      _Pragma("loopbound min 1 max 15")
		for (im_pos=y_im_lin+last_ctr_col;
		    x_filt<y_filt_lin;
		    ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  res_pos+=x_res_dim;
	}
      prev_res_pos++;
    }				/* end mid */

  res_pos -= (x_res_dim - 1);            /* go to lower left corner */
  _Pragma("loopbound min 1 max 4")
    for (y_pos=((y_pos-last_ctr_row)/x_dim)+1;     	/* bottom */
	y_pos<y_stop;
	y_pos+=ygrid_step)
    {
      _Pragma("loopbound min 1 max 4")
	for (x_pos=xgrid_start-x_fmid;         /* bottom-left corner */
	    x_pos<1;
	    x_pos+=xgrid_step)
	{
	  //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  sum=0.0f;
	  x_filt=0,y_im_lin=last_ctr_row;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {
	      _Pragma("loopbound min 1 max 15")
		for (im_pos=y_im_lin;
		    x_filt<y_filt_lin;
		    ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
      //    (*reflect)(filt,x_fdim,y_fdim,0,y_pos,temp,FILTER);
      reflect1(filt,x_fdim,y_fdim,0,y_pos,temp,FILTER);
      _Pragma("loopbound min 41 max 46")
	for (x_pos=first_col;		        /* bottom edge */
	    x_pos<last_ctr_col;
	    x_pos+=xgrid_step)
	{
	  sum=0.0f;
	  x_filt=0,y_im_lin=last_ctr_row;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {
	      _Pragma("loopbound min 1 max 15")
		for (im_pos=x_pos+y_im_lin;
		    x_filt<y_filt_lin;
		    ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
      _Pragma("loopbound min 1 max 4")
	for (x_pos+=1-last_ctr_col;	/* bottom-right corner */
	    x_pos<x_stop;
	    x_pos+=xgrid_step)
	{
	  //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,FILTER);
	  sum=0.0f;
	  x_filt=0,y_im_lin=last_ctr_row;
	  _Pragma("loopbound min 1 max 15")
	    for (y_filt_lin=x_fdim; y_filt_lin<=filt_size; y_filt_lin+=x_fdim) {
	      _Pragma("loopbound min 1 max 15")
		for (im_pos=y_im_lin+last_ctr_col;
		    x_filt<y_filt_lin;
		    ++x_filt) {
		  sum+=image[im_pos]*temp[x_filt];
		  ++im_pos;
		}
	      y_im_lin+=x_dim;
	    }
	  result[res_pos] = sum;
	  ++res_pos;
	}
    }				/* end bottom */

}			/* end of internal_filter */


/* --------------------------------------------------------------------

  Upsample image according to grid parameters and then convolve with
  filt, adding values into result array.

  WARNING: this subroutine ADDS the expanded image into the result, so
  the user must zero the result before invocation!

------------------------------------------------------------------------ */

/*
void internal_expand(float *image, float *filt, float *temp, int x_fdim, int y_fdim, int xgrid_start, int xgrid_step,
		  int ygrid_start, int ygrid_step, float *result, int x_dim, int y_dim, char *edges)
{
  //register double val;
  register float val;
  register int x_filt, res_pos, y_filt_lin;
  register int y_res_lin, x_pos, filt_size = x_fdim*y_fdim;
  register int y_pos, im_pos;
  register int last_ctr_col = x_dim - x_fdim;
  int last_ctr_row = (y_dim - y_fdim) * x_dim;
  int first_col, first_row;
  int x_fmid = x_fdim/2;
  int y_fmid = y_fdim/2;
  int x_stop = x_fdim - x_fmid + 1;
  int y_stop = y_fdim - y_fmid + 1;
  int ygrid_step_full = ygrid_step*x_dim;
  int prev_im_pos, x_im_dim = (x_dim-xgrid_start+xgrid_step-1)/xgrid_step;
  int rt_edge_im_pos;
  //*  fptr reflect = edge_function(edges);
  //    if (!reflect)
  //    {
  //    fprintf(stderr,"Unknown edge handler: %s\n",edges);
  //    return(-1);
  //    }

  for (y_pos=ygrid_start-y_fmid-1,im_pos=0; //* top
      y_pos<0;
      y_pos+=ygrid_step)
  {
    for (x_pos=xgrid_start-x_fmid-1; //* upper-left corner
	x_pos<0;
	x_pos+=xgrid_step,++im_pos)
    {
      //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      val = image[im_pos];
      for (y_filt_lin=y_res_lin=0;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=y_res_lin,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos] += val*temp[x_filt];
    }
    first_col = x_pos+1;
    //      (*reflect)(filt,x_fdim,y_fdim,0,y_pos,temp,EXPAND);
    reflect1(filt,x_fdim,y_fdim,0,y_pos,temp,EXPAND);
    for (x_pos=first_col;		//* top edge
	x_pos<last_ctr_col;
	x_pos+=xgrid_step,++im_pos)
    {
      val = image[im_pos];
      for (y_filt_lin=y_res_lin=0;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=x_pos+y_res_lin,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos]  += val*temp[x_filt];
    }
    rt_edge_im_pos = im_pos + x_im_dim;
    for (x_pos+=(1-last_ctr_col);	//* upper-right corner
	x_pos<x_stop;
	x_pos+=xgrid_step,++im_pos)
    {
      //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      val = image[im_pos];
      for (y_filt_lin=y_res_lin=0;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=y_res_lin+last_ctr_col,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos] += val*temp[x_filt];
    }
  }				    //* end top

  first_row = x_dim*(y_pos+1);      //* need this to go down sides
  prev_im_pos = im_pos;
  for (x_pos=xgrid_start-x_fmid-1;  //* left edge
      x_pos<0;
      x_pos+=xgrid_step)
  {
    im_pos = prev_im_pos;
    //      (*reflect)(filt,x_fdim,y_fdim,x_pos,0,temp,EXPAND);
    reflect1(filt,x_fdim,y_fdim,x_pos,0,temp,EXPAND);
    for (y_pos=first_row;
	y_pos<last_ctr_row;
	y_pos+=ygrid_step_full, im_pos+=x_im_dim)
    {
      val = image[im_pos];
      for (y_filt_lin=0,y_res_lin=y_pos;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=y_res_lin,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos] += val*temp[x_filt];
    }
    prev_im_pos++;
  }
  //  (*reflect)(filt,x_fdim,y_fdim,0,0,temp,EXPAND);
  reflect1(filt,x_fdim,y_fdim,0,0,temp,EXPAND);
  for (y_pos=first_row;		//* center region of image
      y_pos<last_ctr_row;
      y_pos+=ygrid_step_full)
  {
    im_pos = prev_im_pos;
    for (x_pos=first_col;
	x_pos<last_ctr_col;
	x_pos+=xgrid_step,++im_pos)
    {
      val = image[im_pos];
      for (y_filt_lin=0,y_res_lin=y_pos;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=x_pos+y_res_lin,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos]  += val*temp[x_filt];
    }
    prev_im_pos+=x_im_dim;
  }
  prev_im_pos = rt_edge_im_pos;
  for (x_pos+=(1-last_ctr_col);	//* right edge
      x_pos<x_stop;
      x_pos+=xgrid_step)
  {
    im_pos = prev_im_pos;
    //      (*reflect)(filt,x_fdim,y_fdim,x_pos,0,temp,EXPAND);
    reflect1(filt,x_fdim,y_fdim,x_pos,0,temp,EXPAND);
    for (y_pos=first_row;
	y_pos<last_ctr_row;
	y_pos+=ygrid_step_full, im_pos+=x_im_dim)
    {
      val = image[im_pos];
      for (y_filt_lin=0,y_res_lin=y_pos;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=y_res_lin+last_ctr_col,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos] += val*temp[x_filt];
    }
    prev_im_pos++;
  }				//* end mid

  im_pos -= (x_im_dim -1);
  for (y_pos=((y_pos-last_ctr_row)/x_dim)+1;
      y_pos<y_stop;		//* bottom
      y_pos+=ygrid_step)
  {
    for (x_pos=xgrid_start-x_fmid-1;
	x_pos<0;
	x_pos+=xgrid_step,++im_pos)
    {
      //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      val = image[im_pos];
      for (y_filt_lin=0,y_res_lin=last_ctr_row;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=y_res_lin,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos] += val*temp[x_filt];
    }
    //      (*reflect)(filt,x_fdim,y_fdim,0,y_pos,temp,EXPAND);
    reflect1(filt,x_fdim,y_fdim,0,y_pos,temp,EXPAND);
    for (x_pos=first_col;
	x_pos<last_ctr_col;
	x_pos+=xgrid_step,++im_pos)
    {
      val = image[im_pos];
      for (y_filt_lin=0, y_res_lin=last_ctr_row;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=x_pos+y_res_lin,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    ++x_filt,++res_pos)
	result[res_pos]  += val*temp[x_filt];
    }
    for (x_pos+=1-last_ctr_col;
	x_pos<x_stop;
	x_pos+=xgrid_step,im_pos++)
    {
      //	  (*reflect)(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      reflect1(filt,x_fdim,y_fdim,x_pos,y_pos,temp,EXPAND);
      val = image[im_pos];
      for (y_filt_lin=0,y_res_lin=last_ctr_row;
	  y_filt_lin<filt_size;
	  y_res_lin+=x_dim,y_filt_lin+=x_fdim)
	for (res_pos=y_res_lin+last_ctr_col,x_filt=y_filt_lin;
	    x_filt<y_filt_lin+x_fdim;
	    x_filt++,res_pos++)
	result[res_pos] += val*temp[x_filt];
    }
  }				//* end bottom
}				//* end of internal_expand
*/
///////////////////////////////////////////////////////////////

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  File: edges.c
;;;  Author: Simoncelli
;;;  Description: Edge handling routines for use with convolve.c
;;;  Creation Date:
;;;  ----------------------------------------------------------------
;;;    Object-Based Vision and Image Understanding System (OBVIUS),
;;;      Copyright 1988, Vision Science Group,  Media Laboratory,
;;;              Massachusetts Institute of Technology.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

/*
This file contains functions which determine how edges are to be
handled when performing convolutions of images with linear filters.
Any edge handling function which is local and linear may be defined,
except (unfortunately) constants cannot be added.  So to treat the
edges as if the image is surrounded by a gray field, you must paste it
into a gray image, convolve, and crop it out...
The main convolution function is called internal_filter and is defined
in the file convolve.c.  The idea is that the convolution function
calls the edge handling function which computes a new filter based on
the old filter and the distance to the edge of the image.  For
example, reflection is done by reflecting the filter through the
appropriate axis and summing.  Currently defined functions are listed
below.
*/

#define sgn(a)  ( ((a)>0)?1:(((a)<0)?-1:0) )
#define clip(a,mn,mx)  ( ((a)<(mn))?(mn):(((a)>=(mx))?(mx-1):(a)) )
//
//int reflect2(), repeat(), zero(), extend(), nocompute();
//int ereflect(), predict();
//
//static EDGE_HANDLER edge_foos[] =
//  {
//    { "reflect1", reflect1 },   /* reflect about edge pixels - new name */
//    { "reflect2", reflect2 },   /* standard reflection */
//    { "repeat",   repeat   },   /* repeat edge pixel */
//    { "zero",     zero     },   /* zero outside of image */
//    { "extend",   extend   },   /* extend (reflect & invert) */
//    { "dont-compute", nocompute }, /* ignore edge (zero output for filter near edge) */
//    { "predict",  predict  },   /* predict based on portion covered by filt */
//    { "ereflect", ereflect },	/* orthogonal QMF reflection */
//    { "treflect", reflect1 }	/* reflect about edge pixels - old name */
//  };
//
/*
Function looks up an edge handler id string in the structure above, and
returns the associated function
*/
/*
fptr edge_function(edges)
  char *edges;
  {
  int i;
  for (i = 0; i<sizeof(edge_foos)/sizeof(EDGE_HANDLER); i++)
    if (!strcmp(edges,edge_foos[i].name))
      return(edge_foos[i].func);
#ifdef WITHIO
  printf("No such edge handler routine!\n");
#endif
  return(0);
  }
*/
/*
---------------- EDGE HANDLER ARGUMENTS ------------------------
filt - floating point array of filter taps.
x_dim, y_dim - x and y dimensions of filt.
x_pos - position of filter relative to the horizontal image edges. Negative
       values indicate left edge, positive indicate right edge.  Zero
       indicates that the filter is not touching either edge.  An absolute
       value of 1 indicates that the edge tap of the filter is over the
       edge pixel of the image.
y_pos - analogous to x_pos.
result - floating point array where the resulting filter will go.  The edge
       of this filter will be aligned with the image for application...
f_or_e - equal to one of the two constants EXPAND or FILTER.
--------------------------------------------------------------------
*/


/* --------------------------------------------------------------------
zero() - Zero outside of image.  Discontinuous, but adds zero energy. */

/*
void zero(float *filt, int x_dim, int y_dim, int x_pos, int y_pos, float * result, int f_or_e)
{
  register int y_filt,x_filt, y_res,x_res;
  int filt_sz = x_dim*y_dim;
  int x_start = ((x_pos>0)?(x_pos-1):((x_pos<0)?(x_pos+1):0));
  int y_start = x_dim * ((y_pos>0)?(y_pos-1):((y_pos<0)?(y_pos+1):0));
  int i;

  for (i=0; i<filt_sz; i++) result[i] = 0.0;

  for (y_filt=0, y_res=y_start;
      y_filt<filt_sz;
      y_filt+=x_dim, y_res+=x_dim)
    if ((y_res >= 0) AND (y_res < filt_sz))
    for (x_filt=y_filt, x_res=x_start;
	x_filt<y_filt+x_dim;
	x_filt++, x_res++)
    if ((x_res >= 0) AND (x_res < x_dim))
    result[y_res+x_res] = filt[x_filt];
}
*/

/* --------------------------------------------------------------------
repeat() - repeat edge pixel.  Continuous, but content is usually
different from image.
*/

/*
void repeat(float *filt, int x_dim, int y_dim, int x_pos, int y_pos, float *result, int f_or_e)
{
  register int y_filt,x_filt, y_res,x_res;
  int filt_sz = x_dim*y_dim;
  int x_start = ((x_pos>0)?(x_pos-1):((x_pos<0)?(x_pos+1):0));
  int y_start = x_dim * ((y_pos>0)?(y_pos-1):((y_pos<0)?(y_pos+1):0));
  int i;

  for (i=0; i<filt_sz; i++) result[i] = 0.0;

  for (y_filt=0, y_res=y_start;
      y_filt<filt_sz;
      y_filt+=x_dim, y_res+=x_dim)
    for (x_filt=y_filt, x_res=x_start;
	x_filt<y_filt+x_dim;
	x_filt++, x_res++)
    result[((y_res>=0)?((y_res<filt_sz)?y_res:(filt_sz-x_dim)):0) //* clip y_res
    + ((x_res>=0)?((x_res<x_dim)?x_res:(x_dim-1)):0)]      //* clip x_res
    += filt[x_filt];
}
*/

/* --------------------------------------------------------------------
reflect2() - "Normal" image reflection.  The edge pixel is repeated,
then the next pixel, etc.  Continuous, attempting to maintain
"similar" content, but discontinuous first derivative.
*/

/*
void reflect2(float *filt, int x_dim, int y_dim, int x_pos, int y_pos, float *result, int f_or_e)
{
  register int y_filt,x_filt, y_edge,x_edge;
  register int x_base = (x_pos>0)?(x_dim-1):0;
  register int y_base = (y_pos>0)?(x_dim*(y_dim-1)):0;
  int filt_sz = x_dim*y_dim;
  int x_edge_dist = (x_pos>0)?(x_pos-x_dim-1):(x_pos+1);
  int y_edge_dist = x_dim * ((y_pos>0)?(y_pos-y_dim-1):(y_pos+1));
  int i;

  _Pragma("loopbound min 15 max 15")
    for (i=0; i<filt_sz; ++i) result[i] = 0.0;

  y_edge=y_edge_dist;
  _Pragma("loopbound min 1 max 15")
    for (y_filt=0; y_filt<filt_sz; y_filt+=x_dim)
    {
      if (y_edge IS 0) y_edge+=x_dim;
      x_edge=x_edge_dist;
      _Pragma("loopbound min 1 max 15")
	for (x_filt=y_filt; x_filt<y_filt+x_dim; ++x_filt)
	{
	  if (x_edge IS 0) x_edge++;
	  result[abs(y_base-abs(y_edge)+x_dim) + abs(x_base-abs(x_edge)+1)]
	    += filt[x_filt];
	  ++x_edge;
	}
      y_edge+=x_dim;
    }
}
*/

/* --------------------------------------------------------------------
reflect1() - Reflection through the edge pixels.  This is the right thing
to do if you are subsampling by 2, since it maintains parity (even
pixels positions remain even, odd ones remain odd). (note: procedure differs
depending on f_or_e parameter).  */

void reflect1(float *filt, int x_dim, int y_dim, int x_pos, int y_pos,float *result,int f_or_e)
{
  int filt_sz = x_dim*y_dim;
  register int x_start = 0, y_start = 0, x_stop = x_dim, y_stop = filt_sz;
  register int y_filt,x_filt, y_edge,x_edge;
  register int x_base = (x_pos>0)?(x_dim-1):0;
  register int y_base = (y_pos>0)?(x_dim*(y_dim-1)):0;
  int x_edge_dist = (x_pos>0)?(x_pos-x_dim):((x_pos<-1)?(x_pos+1):0);
  int y_edge_dist = x_dim * ((y_pos>0)?(y_pos-y_dim):((y_pos<-1)?(y_pos+1):0));
  int i;
  int mx_pos = (x_dim/2)+1;
  int my_pos = (y_dim/2)+1;

  _Pragma("loopbound min 15 max 15")
    for (i=0; i<filt_sz; ++i) result[i] = 0.0f;

  /* if EXPAND and filter is centered on image edge, do not reflect */
  if (f_or_e IS EXPAND)
  {
    if (x_pos IS mx_pos) x_stop = (x_dim+1)/2;
    else if (x_pos IS -mx_pos) { x_start = x_dim/2; x_edge_dist = 0; }

    if (y_pos IS my_pos) y_stop = x_dim*((y_dim+1)/2);
    else if (y_pos IS -my_pos) { y_start = x_dim*(y_dim/2); y_edge_dist = 0;}
  }

  y_edge=y_edge_dist;
  /* reflect at boundary of image */
  _Pragma("loopbound min 1 max 15")
    for (y_filt=y_start; y_filt<y_stop; y_filt+=x_dim) {
      x_edge=x_edge_dist;
      _Pragma("loopbound min 1 max 15")
	for (x_filt=y_filt+x_start; x_filt<y_filt+x_stop; ++x_filt) {
	  result[abs(y_base-abs(y_edge)) + abs(x_base-abs(x_edge))]
	    += filt[x_filt];
	  ++x_edge;
	}
      y_edge+=x_dim;
    }

  /* if EXPAND and filter is not centered on image edge, mult edge by 2 */
  if (f_or_e IS EXPAND)
  {
    if ( (abs(x_pos) ISNT mx_pos) AND (x_pos ISNT 0) )
      _Pragma("loopbound min 0 max 0")
      for (y_filt=x_base; y_filt<filt_sz; y_filt+=x_dim)
      result[y_filt] += result[y_filt];
    if ( (abs(y_pos) ISNT my_pos) AND (y_pos ISNT 0) )
      _Pragma("loopbound min 0 max 0")
      for (x_filt=y_base; x_filt<y_base+x_dim; ++x_filt)
      result[x_filt] += result[x_filt];
  }
}

/* --------------------------------------------------------------------
extend() - Extend image by reflecting and inverting about edge pixel
value.  Maintains continuity in intensity AND first derivative (but
not higher derivs).
*/

/*
void extend(float *filt, int x_dim,int y_dim,int x_pos,int y_pos, float *result,int f_or_e)
{
  int filt_sz = x_dim*y_dim;
  register int x_start = 0, y_start = 0, x_stop = x_dim, y_stop = filt_sz;
  register int y_filt,x_filt, y_edge,x_edge;
  register int x_base = (x_pos>0)?(x_dim-1):0;
  register int y_base = (y_pos>0)?(x_dim*(y_dim-1)):0;
  int x_edge_dist = (x_pos>0)?(x_pos-x_dim):((x_pos<-1)?(x_pos+1):0);
  int y_edge_dist = x_dim * ((y_pos>0)?(y_pos-y_dim):((y_pos<-1)?(y_pos+1):0));
  int i;
  int mx_pos = (x_dim/2)+1;
  int my_pos = (y_dim/2)+1;

  for (i=0; i<filt_sz; i++) result[i] = 0.0;

  //* if EXPAND and filter is centered on image edge, do not reflect
  if (f_or_e IS EXPAND)
  {
    if (x_pos IS mx_pos) x_stop = (x_dim+1)/2;
    else if (x_pos IS -mx_pos) { x_start = x_dim/2; x_edge_dist = 0; }

    if (y_pos IS my_pos) y_stop = x_dim*((y_dim+1)/2);
    else if (y_pos IS -my_pos) { y_start = x_dim*(y_dim/2); y_edge_dist = 0;}
  }

  //* reflect at boundary of image
  for (y_filt=y_start, y_edge=y_edge_dist;
      y_filt<y_stop;
      y_filt+=x_dim, y_edge+=x_dim)
    for (x_filt=y_filt+x_start, x_edge=x_edge_dist;
	x_filt<y_filt+x_stop;
	++x_filt, ++x_edge)
    if (((!y_base AND (sgn(y_edge) IS -1)) //* y overhanging
	  OR
	  (y_base AND (sgn(y_edge) IS 1)))
	ISNT			             //* XOR
	((!x_base AND (sgn(x_edge) IS -1)) //* x overhanging
	 OR
	 (x_base AND (sgn(x_edge) IS 1))))
    {
      result[abs(y_base-abs(y_edge)) + abs(x_base-abs(x_edge))]
	-= filt[x_filt];
      result[clip(y_base+y_edge,0,y_dim) + clip(x_base+x_edge,0,x_dim)]
	+= filt[x_filt] + filt[x_filt];
    }
    else result[abs(y_base-abs(y_edge)) + abs(x_base-abs(x_edge))]
      += filt[x_filt];

}
*/

/* --------------------------------------------------------------------
nocompute() - Return zero for values where filter hangs over the edge.
*/
/*
void nocompute(float *filt,int x_dim,int y_dim,int x_pos,int y_pos,float *result,int f_or_e)
{
  register int i;
  register int size = x_dim*y_dim;

  if ( (x_pos>1) OR (x_pos<-1) OR (y_pos>1) OR (y_pos<-1) )
    for (i=0; i<size; ++i)  result[i] = 0.0;
  else
    for (i=0; i<size; ++i)  result[i] = filt[i];
}
*/

// Sqrt implementation
float sqrt_x (float y)
{
  float x, z, tempf;
  unsigned long *tfptr = ((unsigned long *)&tempf) + 1;

  tempf = y;
  *tfptr = (0xbfcdd90af - *tfptr)>>1; /* estimate of 1/sqrt(y) */
  x =  tempf;
  z =  y*0.5f;                        /* hoist out the ?/2?    */
  x = (1.5f*x) - (x*x)*(x*z);         /* iteration formula     */
  x = (1.5f*x) - (x*x)*(x*z);
  x = (1.5f*x) - (x*x)*(x*z);
  x = (1.5f*x) - (x*x)*(x*z);
  x = (1.5f*x) - (x*x)*(x*z);
  return x*y;
}


/* --------------------------------------------------------------------
Reflect, multiplying tap of filter which is at the edge of the image
by root 2.  This maintains orthogonality of QMF-style filters, but it
is not useful for most applications.
*/

/*
void ereflect(float *filt,int x_dim,int y_dim,int x_pos,int y_pos,float *result,int f_or_e)
{
  register int y_filt,x_filt, y_edge,x_edge;
  register int x_base = (x_pos>0)?(x_dim-1):0;
  register int y_base = x_dim * ( (y_pos>0)?(y_dim-1):0 );
  int filt_sz = x_dim*y_dim;
  int x_edge_dist = (x_pos>1)?(x_pos-x_dim):((x_pos<-1)?(x_pos+1):0);
  int y_edge_dist = x_dim * ( (y_pos>1)?(y_pos-y_dim):((y_pos<-1)?(y_pos+1):0) );
  int i;
  //double norm,onorm;
  float norm,onorm;

  for (i=0; i<filt_sz; ++i) result[i] = 0.0;

  //* reflect at boundary
  for (y_filt=0, y_edge=y_edge_dist;
      y_filt<filt_sz;
      y_filt+=x_dim, y_edge+=x_dim)
    for (x_filt=y_filt, x_edge=x_edge_dist;
	x_filt<y_filt+x_dim;
	++x_filt, ++x_edge)
    result[abs(y_base-abs(y_edge)) + abs(x_base-abs(x_edge))]
    += filt[x_filt];

  //* now multiply edge by root 2
  if (x_pos ISNT 0)
    for (y_filt=x_base; y_filt<filt_sz; y_filt+=x_dim)
    result[y_filt] *= ROOT2;
  if (y_pos ISNT 0)
    for (x_filt=y_base; x_filt<y_base+x_dim; ++x_filt)
    result[x_filt] *= ROOT2;

  //* now normalize to norm of original filter
  for (norm=0.0,i=0; i<filt_sz; ++i)
    norm += (result[i]*result[i]);
  //norm=sqrt(norm);
  //norm=testsqrt(norm);
  norm=sqrt_x(norm);

  for (onorm=0.0,i=0; i<filt_sz; ++i)
    onorm += (filt[i]*filt[i]);
  //onorm = sqrt(onorm);
  //onorm = testsqrt(onorm);
  onorm = sqrt_x(onorm);

  norm = norm/onorm;
  for (i=0; i<filt_sz; ++i)
    result[i] /= norm;
}
*/

/* --------------------------------------------------------------------
predict() - version of prediction.  Like zero, but multiplies the result by
the reciprocal of the percentage of filter being used.  (i.e. if 50% of the
filter is hanging over the edge of the image, multiply the taps being used by
2.  *** Fix this to divide by the squared sum of taps being used and multiply by
the squared sum of ALL taps.
*/

/*
void predict(float *filt,int x_dim,int y_dim,int x_pos,int y_pos,float *result,int f_or_e)
{
  register int y_filt,x_filt, y_res,x_res;
  register float taps_used = 0; //* int ***
  register float fraction;
  int filt_sz = x_dim*y_dim;
  int x_start = ((x_pos>0)?(x_pos-1):((x_pos<0)?(x_pos+1):0));
  int y_start = x_dim * ((y_pos>0)?(y_pos-1):((y_pos<0)?(y_pos+1):0));
  int i;

  for (i=0; i<filt_sz; ++i) result[i] = 0.0;

  for (y_filt=0, y_res=y_start;
      y_filt<filt_sz;
      y_filt+=x_dim, y_res+=x_dim)
    if ((y_res >= 0) AND (y_res < filt_sz)) {
      x_res=x_start;
      for (x_filt=y_filt; x_filt<y_filt+x_dim; ++x_filt) {
	if ((x_res >= 0) AND (x_res < x_dim))
	{
	  result[y_res+x_res] = filt[x_filt];
	  taps_used += filt[x_filt]; //* 1.0 ***
	}
	++x_res;
      }
    }

  if (f_or_e IS FILTER)
  {
    fraction = ( (float) 2.0 ) / ( (float) taps_used );
    for (i=0; i<filt_sz; ++i) result[i] *= fraction;
  }
}
*/

///////////////////////////////////////////////////////////////

static float lo_filter[FILTER_SIZE] =
  {-0.0012475221, -0.0024950907, 0.0087309530, 0.0199579580,
   -0.0505290000, -0.1205509700, 0.2930455800,
    0.7061761600,
    0.2930455800, -0.1205509700, -0.0505290000,
    0.0199579580, 0.0087309530, -0.0024950907, -0.0012475221};

static float hi_filter[FILTER_SIZE] =
  { 0.0012475221, -0.0024950907, -0.0087309530, 0.0199579580,
    0.0505290000, -0.1205509700, -0.2930455800,
    0.7061761600,
   -0.2930455800, -0.1205509700, 0.0505290000,
    0.0199579580, -0.0087309530, -0.0024950907, 0.0012475221};

int main(void )
{
  int i;

  _Pragma("loopbound min 4096 max 9801")
  for (i=0;i<X_SIZE*Y_SIZE;++i)
    image[i] *= SCALE_FACTOR;
  build_pyr(image, X_SIZE, Y_SIZE, NUM_LEVELS, lo_filter, hi_filter, FILTER_SIZE);

  return 0;
}
