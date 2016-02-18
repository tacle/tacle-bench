/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: epic

  Author: Designed by Eero P. Simoncelli and Edward H. Adelson
          Written by Eero P. Simoncelli
          Developed at the Vision Science Group, The Media Laboratory
          Copyright 1989, Massachusetts Institute of Technology
          All rights reserved.

  Function: Efficient Pyramid Image Coder

  Source: MediaBench
          http://euler.slu.edu/~fritts/mediabench/mb1/index.html

  Original name: epic

  Changes: no major functional changes

  License:
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
*/


#include "epic.h"
#include "epic_data.h"


/*
  Macro definitions
*/

#define abs(x)    (x>=0 ? x : -(x))
#define FILTER 0
#define EXPAND 1
#define IS    ==
#define ISNT  !=
#define AND &&
#define OR ||

#define NUM_LEVELS  4


/*
  Forward declaration of functions
*/

void epic_init( void );
void epic_build_pyr( float *image, int x_size, int y_size, int num_levels,
                     float *lo_filter, float *hi_filter, int filter_size );
void epic_build_level( float *image, int level_x_size, int level_y_size,
                       float *lo_filter, float *hi_filter,
                       int filter_size, float *result_block );
void epic_internal_transpose( float *mat, int rows, int cols );
void epic_internal_filter( float *image, int x_dim, int y_dim, float *filt,
                           float *temp, int x_fdim, int y_fdim,
                           int xgrid_start, int xgrid_step, int ygrid_start,
                           int ygrid_step, float *result, char *edges );
void epic_reflect1( float *filt, int x_dim, int y_dim, int x_pos, int y_pos,
                    float *result, int f_or_e );
void epic_main( void );
int main( void );


/*
  Declaration of global variables
*/

float epic_filtertemp[FILTER_SIZE];
float epic_hi_imagetemp[X_SIZE * Y_SIZE / 2];
float epic_lo_imagetemp[X_SIZE * Y_SIZE / 2];

static float epic_lo_filter[FILTER_SIZE] = {
  -0.0012475221, -0.0024950907, 0.0087309530, 0.0199579580,
  -0.0505290000, -0.1205509700, 0.2930455800,
  0.7061761600,
  0.2930455800, -0.1205509700, -0.0505290000,
  0.0199579580, 0.0087309530, -0.0024950907, -0.0012475221
};

static float epic_hi_filter[FILTER_SIZE] = {
  0.0012475221, -0.0024950907, -0.0087309530, 0.0199579580,
  0.0505290000, -0.1205509700, -0.2930455800,
  0.7061761600,
  -0.2930455800, -0.1205509700, 0.0505290000,
  0.0199579580, -0.0087309530, -0.0024950907, 0.0012475221
};


/*
  Initialization function
*/

void epic_init( void )
{
  int i;

  _Pragma( "loopbound min 4096 max 9801" )
  for ( i = 0; i < X_SIZE * Y_SIZE; ++i )
    epic_image[i] *= SCALE_FACTOR;
}


/*
  Algorithm core functions
*/

/*
  ======================================================================
  epic_build_pyr() -- builds a separable QMF-style pyramid.  The pyramid
  is written over the original image.  NOTE: the image size must be
  divisible by 2^num_levels, but we do not check this here.
  ======================================================================
*/
void epic_build_pyr( float *image, int x_size, int y_size, int num_levels,
                     float *lo_filter, float *hi_filter, int filter_size )
{
  int x_level, y_level, level;

  x_level = x_size;
  y_level = y_size;

  _Pragma( "loopbound min 4 max 4" )
  for ( level = 0;  level < num_levels; ++level )
    epic_build_level( image, x_level, y_level, lo_filter, hi_filter,
                      filter_size, image );
  x_level /= 2;
  y_level /= 2;
}


/*
  ======================================================================
  epic_build_level() -- builds a level of the pyramid by computing 4
  filtered and subsampled images.  Since the convolution is separable,
  image and result-block can point to the same place!  Image order is
  lowpass, horizontal, vertical (transposed), and diagonal.
  ======================================================================
*/
void epic_build_level( float *image, int level_x_size, int level_y_size,
                       float *lo_filter, float *hi_filter,
                       int filter_size, float *result_block )
{
  int total_size = level_x_size * level_y_size;

  /* filter and subsample in the X direction */
  epic_internal_filter ( image, level_x_size, level_y_size,
                         lo_filter, epic_filtertemp, filter_size, 1,
                         0, 2, 0, 1, epic_lo_imagetemp, "epic_reflect1" );
  epic_internal_filter ( image, level_x_size, level_y_size,
                         hi_filter, epic_filtertemp, filter_size, 1,
                         1, 2, 0, 1, epic_hi_imagetemp, "epic_reflect1" );

  level_x_size /= 2;
  /* now filter and subsample in the Y direction */
  epic_internal_filter ( epic_lo_imagetemp, level_x_size,
                         level_y_size, /* lowpass */
                         lo_filter, epic_filtertemp, 1, filter_size,
                         0, 1, 0, 2, result_block, "epic_reflect1" );
  epic_internal_filter ( epic_lo_imagetemp, level_x_size,
                         level_y_size, /* horizontal */
                         hi_filter, epic_filtertemp, 1, filter_size,
                         0, 1, 1, 2, ( result_block += ( total_size / 4 ) ),
                         "epic_reflect1" );
  epic_internal_filter ( epic_hi_imagetemp, level_x_size,
                         level_y_size, /* vertical */
                         lo_filter, epic_filtertemp, 1, filter_size,
                         0, 1, 0, 2, ( result_block += ( total_size / 4 ) ),
                         "epic_reflect1" );
  /* transpose the vertical band for more efficient scanning */
  epic_internal_transpose( result_block, level_y_size / 2, level_x_size );
  epic_internal_filter ( epic_hi_imagetemp, level_x_size,
                         level_y_size, /* diagonal */
                         hi_filter, epic_filtertemp, 1, filter_size,
                         0, 1, 1, 2, ( result_block += ( total_size / 4 ) ),
                         "epic_reflect1" );
}


/*
  ======================================================================
  In-place matrix tranpose algorithm.  Handles non-square matrices,
  too!  Is there a faster algorithm??
  ======================================================================
*/
void epic_internal_transpose( float *mat, int rows, int cols )
{
  register int swap_pos;
  register int modulus = rows * cols - 1;
  register int current_pos;
  register float swap_val;

  /* loop, ignoring first and last elements */
  _Pragma( "loopbound min 1022 max 2399" )
  for ( current_pos = 1; current_pos < modulus; ++current_pos ) {
    /* Compute swap position */
    swap_pos = current_pos;

    _Pragma( "loopbound min 1 max 2" )
    do {
      swap_pos = ( swap_pos * cols ) % modulus;
    } while ( swap_pos < current_pos );

    if ( current_pos != swap_pos ) {
      swap_val = mat[swap_pos];
      mat[swap_pos] = mat[current_pos];
      mat[current_pos] = swap_val;
    }
  }
}


/* --------------------------------------------------------------------
  Correlate FILT with IMAGE, subsampling according to GRID parameters,
  with values placed into result array.  TEMP is a temporary
  array the size of the filter.  EDGES is a string -- see convolve.h.
  The convolution is done in 9 sections, where the border sections use
  specially computed edge-handling filters (see edges.c). The origin
  of the filter is assumed to be (floor(x_fdim/2), floor(y_fdim/2)).
  10/6/89 - approximately optimized the choice of register vars on SPARCS.
  ------------------------------------------------------------------------ */
void epic_internal_filter( float *image, int x_dim, int y_dim, float *filt,
                           float *temp, int x_fdim, int y_fdim,
                           int xgrid_start, int xgrid_step, int ygrid_start,
                           int ygrid_step, float *result, char *edges )
{
  //register double sum;
  register float sum;
  register int x_filt, im_pos, y_filt_lin;
  register int y_im_lin, x_pos, filt_size = x_fdim * y_fdim;
  register int y_pos, res_pos;
  register int last_ctr_col = x_dim - x_fdim;
  int last_ctr_row = ( y_dim - y_fdim ) * x_dim;
  int first_row, first_col;
  int x_fmid = x_fdim / 2;
  int y_fmid = y_fdim / 2;
  int x_stop = x_fdim - x_fmid + 1;
  int y_stop = y_fdim - y_fmid + 1;
  int ygrid_step_full = ygrid_step * x_dim;
  int prev_res_pos, x_res_dim = ( x_dim - xgrid_start + xgrid_step - 1 ) /
                                xgrid_step;
  int rt_edge_res_pos;

  res_pos = 0;

  _Pragma( "loopbound min 1 max 4" )
  for ( y_pos = ygrid_start - y_fmid - 1; y_pos < 0; y_pos += ygrid_step ) {
    _Pragma( "loopbound min 1 max 4" )
    for ( x_pos = xgrid_start - x_fmid;   /* top-left corner */
          x_pos < 1;
          x_pos += xgrid_step ) {
      epic_reflect1( filt, x_fdim, y_fdim, x_pos, y_pos, temp, FILTER );
      sum = 0.0f;
      x_filt = y_im_lin = 0;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        im_pos = y_im_lin;

        _Pragma( "loopbound min 1 max 15" )
        for ( ; x_filt < y_filt_lin; ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
    first_col = x_pos + 1;
    epic_reflect1( filt, x_fdim, y_fdim, 0, y_pos, temp, FILTER );
    _Pragma( "loopbound min 41 max 46" )
    for ( x_pos = first_col;          /* top edge */
          x_pos < last_ctr_col;
          x_pos += xgrid_step ) {
      sum = 0.0f;
      x_filt = y_im_lin = 0;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        im_pos = x_pos + y_im_lin;
        _Pragma( "loopbound min 1 max 15" )
        for ( ; x_filt < y_filt_lin; ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
    rt_edge_res_pos = res_pos + x_res_dim;   /* save this for later ... */
    _Pragma( "loopbound min 1 max 4" )
    for ( x_pos += ( 1 - last_ctr_col );  /* top-right corner */
          x_pos < x_stop;
          x_pos += xgrid_step ) {
      epic_reflect1( filt, x_fdim, y_fdim, x_pos, y_pos, temp, FILTER );
      sum = 0.0f;
      x_filt = y_im_lin = 0;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        im_pos = y_im_lin + last_ctr_col;

        _Pragma( "loopbound min 1 max 15" )
        for ( ; x_filt < y_filt_lin; ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
  }                        /* end top */

  first_row = x_dim * ( y_pos + 1 ); /* need this to go down the sides */
  prev_res_pos = res_pos;
  _Pragma( "loopbound min 1 max 4" )
  for ( x_pos = xgrid_start - x_fmid;      /* left edge */
        x_pos < 1;
        x_pos += xgrid_step ) {
    res_pos = prev_res_pos;
    epic_reflect1( filt, x_fdim, y_fdim, x_pos, 0, temp, FILTER );
    _Pragma( "loopbound min 41 max 97" )
    for ( y_pos = first_row; y_pos < last_ctr_row;
          y_pos += ygrid_step_full ) {
      sum = 0.0f;
      x_filt = 0, y_im_lin = y_pos;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        im_pos = y_im_lin;
        _Pragma( "loopbound min 1 max 15" )
        for ( ; x_filt < y_filt_lin; x_filt++ ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      res_pos += x_res_dim;
    }
    prev_res_pos++;
  }
  epic_reflect1( filt, x_fdim, y_fdim, 0, 0, temp, FILTER );
  _Pragma( "loopbound min 41 max 97" )
  for ( y_pos = first_row; /* center region of image */
        y_pos < last_ctr_row;
        y_pos += ygrid_step_full ) {
    res_pos = prev_res_pos;
    _Pragma( "loopbound min 41 max 46" )
    for ( x_pos = first_col;
          x_pos < last_ctr_col;
          x_pos += xgrid_step ) {
      sum = 0.0f;
      x_filt = 0, y_im_lin = y_pos;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        im_pos = x_pos + y_im_lin;
        _Pragma( "loopbound min 1 max 15" )
        for ( ; x_filt < y_filt_lin; ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
    prev_res_pos += x_res_dim;
  }
  prev_res_pos = rt_edge_res_pos;
  _Pragma( "loopbound min 1 max 4" )
  for ( x_pos += ( 1 - last_ctr_col );           /* right edge */
        x_pos < x_stop;
        x_pos += xgrid_step ) {
    res_pos = prev_res_pos;
    epic_reflect1( filt, x_fdim, y_fdim, x_pos, 0, temp, FILTER );
    _Pragma( "loopbound min 41 max 97" )
    for ( y_pos = first_row; y_pos < last_ctr_row;
          y_pos += ygrid_step_full ) {
      sum = 0.0f;
      x_filt = 0, y_im_lin = y_pos;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim;
            y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        _Pragma( "loopbound min 1 max 15" )
        for ( im_pos = y_im_lin + last_ctr_col;
              x_filt < y_filt_lin;
              ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      res_pos += x_res_dim;
    }
    prev_res_pos++;
  }       /* end mid */

  res_pos -= ( x_res_dim - 1 );          /* go to lower left corner */
  _Pragma( "loopbound min 1 max 4" )
  for ( y_pos = ( ( y_pos - last_ctr_row ) / x_dim ) + 1; /* bottom */
        y_pos < y_stop;
        y_pos += ygrid_step ) {
    _Pragma( "loopbound min 1 max 4" )
    for ( x_pos = xgrid_start - x_fmid;    /* bottom-left corner */
          x_pos < 1;
          x_pos += xgrid_step ) {
      epic_reflect1( filt, x_fdim, y_fdim, x_pos, y_pos, temp, FILTER );
      sum = 0.0f;
      x_filt = 0, y_im_lin = last_ctr_row;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        _Pragma( "loopbound min 1 max 15" )
        for ( im_pos = y_im_lin;
              x_filt < y_filt_lin;
              ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
    epic_reflect1( filt, x_fdim, y_fdim, 0, y_pos, temp, FILTER );
    _Pragma( "loopbound min 41 max 46" )
    for ( x_pos = first_col;        /* bottom edge */
          x_pos < last_ctr_col;
          x_pos += xgrid_step ) {
      sum = 0.0f;
      x_filt = 0, y_im_lin = last_ctr_row;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        _Pragma( "loopbound min 1 max 15" )
        for ( im_pos = x_pos + y_im_lin;
              x_filt < y_filt_lin;
              ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
    _Pragma( "loopbound min 1 max 4" )
    for ( x_pos += 1 - last_ctr_col; /* bottom-right corner */
          x_pos < x_stop;
          x_pos += xgrid_step ) {
      epic_reflect1( filt, x_fdim, y_fdim, x_pos, y_pos, temp, FILTER );
      sum = 0.0f;
      x_filt = 0, y_im_lin = last_ctr_row;
      _Pragma( "loopbound min 1 max 15" )
      for ( y_filt_lin = x_fdim; y_filt_lin <= filt_size;
            y_filt_lin += x_fdim ) {
        _Pragma( "loopbound min 1 max 15" )
        for ( im_pos = y_im_lin + last_ctr_col;
              x_filt < y_filt_lin;
              ++x_filt ) {
          sum += image[im_pos] * temp[x_filt];
          ++im_pos;
        }
        y_im_lin += x_dim;
      }
      result[res_pos] = sum;
      ++res_pos;
    }
  }       /* end bottom */

}     /* end of epic_internal_filter */


/*
  The following function determine how edges are to be handled
  when performing convolutions of images with linear filters.
  Any edge handling function which is local and linear may be defined,
  except (unfortunately) constants cannot be added.  So to treat the
  edges as if the image is surrounded by a gray field, you must paste it
  into a gray image, convolve, and crop it out...
  The main convolution function is called epic_internal_filter.  The idea
  is that the convolution function calls the edge handling function which
  computes a new filter based on the old filter and the distance to the
  edge of the image.  For example, reflection is done by reflecting the
  filter through the appropriate axis and summing.
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
  epic_reflect1() - Reflection through the edge pixels.  This is the right
  thing to do if you are subsampling by 2, since it maintains parity (even
  pixels positions remain even, odd ones remain odd). (note: procedure differs
  depending on f_or_e parameter).  */
void epic_reflect1( float *filt, int x_dim, int y_dim, int x_pos, int y_pos,
                    float *result, int f_or_e )
{
  int filt_sz = x_dim * y_dim;
  register int x_start = 0, y_start = 0, x_stop = x_dim, y_stop = filt_sz;
  register int y_filt, x_filt, y_edge, x_edge;
  register int x_base = ( x_pos > 0 ) ? ( x_dim - 1 ) : 0;
  register int y_base = ( y_pos > 0 ) ? ( x_dim * ( y_dim - 1 ) ) : 0;
  int x_edge_dist = ( x_pos > 0 ) ? ( x_pos - x_dim ) : ( ( x_pos < -1 ) ?
                    ( x_pos + 1 ) : 0 );
  int y_edge_dist = x_dim * ( ( y_pos > 0 ) ? ( y_pos - y_dim ) : ( (
                                y_pos < -1 ) ? ( y_pos + 1 ) : 0 ) );
  int i;
  int mx_pos = ( x_dim / 2 ) + 1;
  int my_pos = ( y_dim / 2 ) + 1;

  _Pragma( "loopbound min 15 max 15" )
  for ( i = 0; i < filt_sz; ++i ) result[i] = 0.0f;

  /* if EXPAND and filter is centered on image edge, do not reflect */
  if ( f_or_e IS EXPAND ) {
    if ( x_pos IS mx_pos ) x_stop = ( x_dim + 1 ) / 2;
    else
      if ( x_pos IS - mx_pos ) {
        x_start = x_dim / 2;
        x_edge_dist = 0;
      }

    if ( y_pos IS my_pos ) y_stop = x_dim * ( ( y_dim + 1 ) / 2 );
    else
      if ( y_pos IS - my_pos ) {
        y_start = x_dim * ( y_dim / 2 );
        y_edge_dist = 0;
      }
  }

  y_edge = y_edge_dist;
  /* reflect at boundary of image */
  _Pragma( "loopbound min 1 max 15" )
  for ( y_filt = y_start; y_filt < y_stop; y_filt += x_dim ) {
    x_edge = x_edge_dist;
    _Pragma( "loopbound min 1 max 15" )
    for ( x_filt = y_filt + x_start; x_filt < y_filt + x_stop; ++x_filt ) {
      result[abs( y_base - abs( y_edge ) ) + abs( x_base - abs( x_edge ) )]
      += filt[x_filt];
      ++x_edge;
    }
    y_edge += x_dim;
  }

  /* if EXPAND and filter is not centered on image edge, mult edge by 2 */
  if ( f_or_e IS EXPAND ) {
    if ( ( abs( x_pos ) ISNT mx_pos ) AND ( x_pos ISNT 0 ) )
      _Pragma( "loopbound min 0 max 0" )
      for ( y_filt = x_base; y_filt < filt_sz; y_filt += x_dim )
        result[y_filt] += result[y_filt];
    if ( ( abs( y_pos ) ISNT my_pos ) AND ( y_pos ISNT 0 ) )
      _Pragma( "loopbound min 0 max 0" )
      for ( x_filt = y_base; x_filt < y_base + x_dim; ++x_filt )
        result[x_filt] += result[x_filt];
  }
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) epic_main( void )
{
  epic_build_pyr( epic_image, X_SIZE, Y_SIZE, NUM_LEVELS, epic_lo_filter,
                  epic_hi_filter, FILTER_SIZE );
}


int main( void )
{
  epic_init();
  epic_main();

  return 0;
}
