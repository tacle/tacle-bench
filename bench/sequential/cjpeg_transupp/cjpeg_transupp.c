/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: cjpeg_transupp

  Author: Thomas G. Lane

  Function: This file contains image transformation routines and other utility
    code used by the jpegtran sample application. These are NOT part of the core
    JPEG library. But we keep these routines separate from jpegtran.c to ease
    the task of maintaining jpegtran-like programs that have other user
    interfaces.

  Source: MediaBench II
          http://euler.slu.edu/~fritts/mediabench (mirror)

  Original name: cjpeg

  Changes: No major functional changes.

  License: See the accompanying README file.

*/


/*
  Include section
*/

#include "jpeglib.h"


/*
  Forward declaration of functions
*/

void cjpeg_transupp_initSeed( void );
signed char cjpeg_transupp_randomInteger( void );
void cjpeg_transupp_init( void );
int cjpeg_transupp_return( void );
void cjpeg_transupp_do_flip_v( j_compress_ptr );
void cjpeg_transupp_do_rot_90( j_compress_ptr );
void cjpeg_transupp_do_rot_180( j_compress_ptr );
void cjpeg_transupp_do_rot_270( j_compress_ptr );
void cjpeg_transupp_do_transverse( j_compress_ptr );
void cjpeg_transupp_main( void );
int main( void );


/*
  Declaration of global variables
*/

volatile int cjpeg_transupp_seed;

signed char cjpeg_transupp_input[ 256 ];
signed char cjpeg_transupp_input2[ 80 ];
signed char cjpeg_transupp_input3[ 65 ];
signed char cjpeg_transupp_input3_2[ 65 ];
signed char cjpeg_transupp_input4[ 64 ];
signed char cjpeg_transupp_input5[ 65 ];
signed char cjpeg_transupp_input5_2[ 65 ];

/* Output arrays replace writing of results into a file. */
signed char cjpeg_transupp_output_data[ 512 ];
signed char cjpeg_transupp_output_data2[ 512 ];
signed char cjpeg_transupp_output_data3[ 512 ];
signed char cjpeg_transupp_output_data4[ 512 ];
signed char cjpeg_transupp_output_data5[ 512 ];

struct jpeg_compress_struct cjpeg_transupp_dstinfo;


/*
  Initialization- and return-value-related functions
*/

void cjpeg_transupp_initSeed( void )
{
  cjpeg_transupp_seed = 0;
}


/*
  cjpeg_transupp_RandomInteger generates random integers between -128 and 127.
*/
signed char cjpeg_transupp_randomInteger( void )
{
  cjpeg_transupp_seed = ( ( ( cjpeg_transupp_seed * 133 ) + 81 ) % 256 ) - 128;
  return ( cjpeg_transupp_seed );
}


void cjpeg_transupp_init( void )
{
  register int i;


  cjpeg_transupp_dstinfo.max_h_samp_factor = 2;
  cjpeg_transupp_dstinfo.max_v_samp_factor = 2;
  cjpeg_transupp_dstinfo.num_components = 3;

  cjpeg_transupp_initSeed();

  _Pragma( "loopbound min 256 max 256" )
  for ( i = 0; i < 256; i++ )
    cjpeg_transupp_input[ i ] = cjpeg_transupp_randomInteger();

  _Pragma( "loopbound min 80 max 80" )
  for ( i = 0; i < 80; i++ )
    cjpeg_transupp_input2[ i ] = cjpeg_transupp_randomInteger();

  _Pragma( "loopbound min 65 max 65" )
  for ( i = 0; i < 65; i++ )
    cjpeg_transupp_input3[ i ] = cjpeg_transupp_randomInteger();

  _Pragma( "loopbound min 65 max 65" )
  for ( i = 0; i < 65; i++ )
    cjpeg_transupp_input3_2[ i ] = cjpeg_transupp_randomInteger();

  _Pragma( "loopbound min 64 max 64" )
  for ( i = 0; i < 64; i++ )
    cjpeg_transupp_input4[ i ] = cjpeg_transupp_randomInteger();

  _Pragma( "loopbound min 65 max 65" )
  for ( i = 0; i < 65; i++ )
    cjpeg_transupp_input5[ i ] = cjpeg_transupp_randomInteger();

  _Pragma( "loopbound min 65 max 65" )
  for ( i = 0; i < 65; i++ )
    cjpeg_transupp_input5_2[ i ] = cjpeg_transupp_randomInteger();
}


int cjpeg_transupp_return( void )
{
  int checksum = 0;
  unsigned int i;


  _Pragma( "loopbound min 512 max 512" )
  for ( i = 0; i < 512; i++ )
    checksum += cjpeg_transupp_output_data[ i ];

  _Pragma( "loopbound min 512 max 512" )
  for ( i = 0; i < 512; i++ )
    checksum += cjpeg_transupp_output_data2[ i ];

  _Pragma( "loopbound min 512 max 512" )
  for ( i = 0; i < 512; i++ )
    checksum += cjpeg_transupp_output_data3[ i ];

  _Pragma( "loopbound min 512 max 512" )
  for ( i = 0; i < 512; i++ )
    checksum += cjpeg_transupp_output_data4[ i ];

  _Pragma( "loopbound min 512 max 512" )
  for ( i = 0; i < 512; i++ )
    checksum += cjpeg_transupp_output_data5[ i ];

  return ( checksum );
}


/*
  Algorithm core functions
*/

/*
  Vertical flip
*/
void cjpeg_transupp_do_flip_v( j_compress_ptr dstinfo )
{
  unsigned int MCU_rows, comp_height, dst_blk_x, dst_blk_y;
  int ci, i, j, offset_y;
  JCOEFPTR src_ptr, dst_ptr;


  /*
    We output into a separate array because we can't touch different rows of the
    source virtual array simultaneously. Otherwise, this is a pretty
    straightforward analog of horizontal flip.
    Within a DCT block, vertical mirroring is done by changing the signs of
    odd-numbered rows.
    Partial iMCUs at the bottom edge are copied verbatim.
  */
  MCU_rows = dstinfo->image_height / ( dstinfo->max_v_samp_factor * DCTSIZE );

  int compptr_v_samp_factor = 8;
  unsigned int compptr_height_in_blocks = 19;
  unsigned int compptr_width_in_blocks = 29;

  _Pragma( "loopbound min 3 max 3" )
  for ( ci = 0; ci < dstinfo->num_components;
        ci++, compptr_v_samp_factor = 1, compptr_width_in_blocks = 15 ) {
    comp_height = MCU_rows * compptr_v_samp_factor;

    compptr_height_in_blocks = 10;
    _Pragma( "loopbound min 2 max 10" )
    for ( dst_blk_y = 0; dst_blk_y < compptr_height_in_blocks;
          dst_blk_y += compptr_v_samp_factor ) {

      _Pragma( "loopbound min 1 max 8" )
      for ( offset_y = 0; offset_y < compptr_v_samp_factor; offset_y++ ) {
        if ( dst_blk_y < comp_height ) {

          /* Row is within the mirrorable area. */
          _Pragma( "loopbound min 15 max 29" )
          for ( dst_blk_x = 0; dst_blk_x < compptr_width_in_blocks;
                dst_blk_x++ ) {

            src_ptr = cjpeg_transupp_input;
            dst_ptr = cjpeg_transupp_output_data;

            _Pragma( "loopbound min 4 max 4" )
            for ( i = 0; i < DCTSIZE; i += 2 ) {

              /* copy even row */
              j = 0;
              _Pragma( "loopbound min 8 max 8" )
              do {
                if ( dst_blk_x < comp_height )
                  *dst_ptr++ = *src_ptr++;
                j++;
              } while ( j < DCTSIZE );

              /* copy odd row with sign change */
              j = 0;
              _Pragma( "loopbound min 8 max 8" )
              do {
                if ( dst_blk_x < comp_height )
                  *dst_ptr++ = - *src_ptr++;
                j++;
              } while ( j < DCTSIZE );
            }
          }
        }
      }
    }
  }
}


/*
  90 degree rotation is equivalent to
   1. Transposing the image;
   2. Horizontal mirroring.
  These two steps are merged into a single processing routine.
*/
void cjpeg_transupp_do_rot_90( j_compress_ptr dstinfo )
{
  unsigned int MCU_cols, comp_width, dst_blk_x, dst_blk_y;
  int ci, i, j, offset_x, offset_y;
  JCOEFPTR src_ptr, dst_ptr;


  /*
    Because of the horizontal mirror step, we can't process partial iMCUs at the
    (output) right edge properly. They just get transposed and not mirrored.
  */
  MCU_cols = dstinfo->image_width / ( dstinfo->max_h_samp_factor * DCTSIZE );

  int compptr_h_samp_factor = 2;
  int compptr_v_samp_factor = 8;
  unsigned int compptr_height_in_blocks = 29;
  unsigned int compptr_width_in_blocks = 19;


  _Pragma( "loopbound min 3 max 3" )
  for ( ci = 0; ci < dstinfo->num_components;
        ci++, compptr_h_samp_factor = compptr_v_samp_factor = 1,
        compptr_height_in_blocks = 15, compptr_width_in_blocks = 10 ) {

    comp_width = MCU_cols * compptr_h_samp_factor;

    _Pragma( "loopbound min 4 max 15" )
    for ( dst_blk_y = 0; dst_blk_y < compptr_height_in_blocks;
          dst_blk_y += compptr_v_samp_factor ) {

      offset_y = 0;
      _Pragma( "loopbound min 1 max 8" )
      for ( ; offset_y < compptr_v_samp_factor; offset_y++ ) {
        dst_blk_x = 0;
        _Pragma( "loopbound min 10 max 10" )
        for ( ; dst_blk_x < compptr_width_in_blocks;
              dst_blk_x += compptr_h_samp_factor ) {

          offset_x = 0;
          _Pragma( "loopbound min 1 max 2" )
          for ( ; offset_x < compptr_h_samp_factor; offset_x++ ) {

            src_ptr = cjpeg_transupp_input2;

            if ( dst_blk_x < comp_width ) {

              /* Block is within the mirrorable area. */
              dst_ptr = cjpeg_transupp_output_data2;

              _Pragma( "loopbound min 4 max 4" )
              for ( i = 0; i < DCTSIZE; i++ ) {
                j = 0;
                _Pragma( "loopbound min 8 max 8" )
                for ( ; j < DCTSIZE; j++ )
                  dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];

                i++;

                _Pragma( "loopbound min 8 max 8" )
                for ( j = 0; j < DCTSIZE; j++ )
                  dst_ptr[ j * DCTSIZE + i ] = -src_ptr[ i * DCTSIZE + j ];
              }
            } else {
              /* Edge blocks are transposed but not mirrored. */
              dst_ptr = cjpeg_transupp_output_data2;

              _Pragma( "loopbound min 8 max 8" )
              for ( i = 0; i < DCTSIZE; i++ )
                j = 0;
              _Pragma( "loopbound min 8 max 8" )
              for ( ; j < DCTSIZE; j++ ) {
                if ( dst_blk_y < comp_width )
                  dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
              }
            }
          }
        }
      }
    }
  }
}


/*
  270 degree rotation is equivalent to
    1. Horizontal mirroring;
    2. Transposing the image.
  These two steps are merged into a single processing routine.
*/
void cjpeg_transupp_do_rot_270( j_compress_ptr dstinfo )
{
  unsigned int MCU_rows, comp_height, dst_blk_x, dst_blk_y;
  int ci, i, j, offset_x, offset_y;
  JCOEFPTR src_ptr, dst_ptr;


  /*
    Because of the horizontal mirror step, we can't process partial iMCUs at the
    (output) bottom edge properly. They just get transposed and not mirrored.
  */
  MCU_rows = dstinfo->image_height / ( dstinfo->max_v_samp_factor * DCTSIZE );

  int compptr_h_samp_factor = 2;
  int compptr_v_samp_factor = 8;
  unsigned int compptr_height_in_blocks = 29;
  unsigned int compptr_width_in_blocks = 19;

  _Pragma( "loopbound min 3 max 3" )
  for ( ci = 0; ci < dstinfo->num_components;
        ci++, compptr_h_samp_factor = compptr_v_samp_factor = 1,
        compptr_height_in_blocks = 15, compptr_width_in_blocks = 10 ) {

    comp_height = MCU_rows * compptr_v_samp_factor;

    _Pragma( "loopbound min 4 max 15" )
    for ( dst_blk_y = 0; dst_blk_y < compptr_height_in_blocks;
          dst_blk_y += compptr_v_samp_factor ) {

      offset_y = 0;
      _Pragma( "loopbound min 1 max 8" )
      for ( ; offset_y < compptr_v_samp_factor; offset_y++ ) {
        dst_blk_x = 0;
        _Pragma( "loopbound min 10 max 10" )
        for ( ; dst_blk_x < compptr_width_in_blocks;
              dst_blk_x += compptr_h_samp_factor ) {

          offset_x = 0;
          _Pragma( "loopbound min 1 max 2" )
          for ( ; offset_x < compptr_h_samp_factor; offset_x++ ) {

            dst_ptr = cjpeg_transupp_output_data3;

            if ( dst_blk_y < comp_height ) {

              /* Block is within the mirrorable area. */
              src_ptr = cjpeg_transupp_input3;

              _Pragma( "loopbound min 8 max 8" )
              for ( i = 0; i < DCTSIZE; i++ ) {
                j = 0;
                _Pragma( "loopbound min 4 max 4" )
                for ( ; j < DCTSIZE; j++ ) {
                  dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
                  j++;
                  dst_ptr[ j * DCTSIZE + i ] = -src_ptr[ i * DCTSIZE + j ];
                }
              }
            } else {

              /* Edge blocks are transposed but not mirrored. */
              src_ptr = cjpeg_transupp_input3_2;

              _Pragma( "loopbound min 8 max 8" )
              for ( i = 0; i < DCTSIZE; i++ )
                j = 0;
              _Pragma( "loopbound min 8 max 8" )
              for ( ; j < DCTSIZE; j++ ) {
                if ( dst_blk_y < comp_height )
                  dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
              }
            }
          }
        }
      }
    }
  }
}


/*
  180 degree rotation is equivalent to
    1. Vertical mirroring;
    2. Horizontal mirroring.
  These two steps are merged into a single processing routine.
*/
void cjpeg_transupp_do_rot_180( j_compress_ptr dstinfo )
{
  unsigned int MCU_cols, MCU_rows, comp_width, comp_height, dst_blk_x,
           dst_blk_y;
  int ci, i, j, offset_y;
  JCOEFPTR src_ptr, dst_ptr;

  int compptr_h_samp_factor = 2;
  int compptr_v_samp_factor = 8;
  unsigned int compptr_width_in_blocks = 29;
  unsigned int compptr_height_in_blocks = 19;


  MCU_cols = dstinfo->image_width / ( dstinfo->max_h_samp_factor * DCTSIZE );
  MCU_rows = dstinfo->image_height / ( dstinfo->max_v_samp_factor * DCTSIZE );

  _Pragma( "loopbound min 3 max 3" )
  for ( ci = 0; ci < dstinfo->num_components; ci++,
        compptr_h_samp_factor = compptr_v_samp_factor = 1,
        compptr_width_in_blocks  = 15, compptr_height_in_blocks = 10 ) {

    comp_width  = MCU_cols * compptr_h_samp_factor;
    comp_height = MCU_rows * compptr_v_samp_factor;

    _Pragma( "loopbound min 3 max 10" )
    for ( dst_blk_y = 0; dst_blk_y < compptr_height_in_blocks;
          dst_blk_y += compptr_v_samp_factor ) {
      offset_y = 0;
      _Pragma( "loopbound min 1 max 8" )
      for ( ; offset_y < compptr_v_samp_factor; offset_y++ ) {
        if ( dst_blk_y < comp_height ) {

          /* Row is within the mirrorable area. */

          /* Process the blocks that can be mirrored both ways. */
          _Pragma( "loopbound min 14 max 28" )
          for ( dst_blk_x = 0; dst_blk_x < comp_width; dst_blk_x++ ) {
            dst_ptr = cjpeg_transupp_output_data4;
            src_ptr = cjpeg_transupp_input4;

            _Pragma( "loopbound min 4 max 4" )
            for ( i = 0; i < DCTSIZE; i += 2 ) {
              j = 0;
              /* For even row, negate every odd column. */
              _Pragma( "loopbound min 4 max 4" )
              for ( ; j < DCTSIZE; j += 2 ) {
                if ( dst_blk_x < comp_height ) {
                  *dst_ptr++ = *src_ptr++;
                  *dst_ptr++ = - *src_ptr++;
                }
              }

              j = 0;
              /* For odd row, negate every even column. */
              _Pragma( "loopbound min 4 max 4" )
              for ( ; j < DCTSIZE; j += 2 ) {
                if ( dst_blk_x < comp_height ) {
                  *dst_ptr++ = - *src_ptr++;
                  *dst_ptr++ = *src_ptr++;
                }
              }
            }
          }

          /* Any remaining right-edge blocks are only mirrored vertically. */
          _Pragma( "loopbound min 1 max 1" )
          for ( ; dst_blk_x < compptr_width_in_blocks; dst_blk_x++ ) {

            dst_ptr = cjpeg_transupp_output_data4;
            src_ptr = cjpeg_transupp_input4;
            _Pragma( "loopbound min 4 max 4" )
            for ( i = 0; i < DCTSIZE; i += 2 ) {
              j = 0;
              _Pragma( "loopbound min 8 max 8" )
              for ( ; j < DCTSIZE; j++ )
                *dst_ptr++ = *src_ptr++;
              j = 0;
              _Pragma( "loopbound min 8 max 8" )
              for ( ; j < DCTSIZE; j++ )
                *dst_ptr++ = - *src_ptr++;
            }
          }
        } else {

          /* Remaining rows are just mirrored horizontally. */
          dst_blk_x = 0;
          /* Process the blocks that can be mirrored. */
          _Pragma( "loopbound min 14 max 14" )
          do {
            dst_ptr = cjpeg_transupp_output_data4;
            src_ptr = cjpeg_transupp_input4;

            i = 0;
            _Pragma( "loopbound min 32 max 32" )
            while ( i < DCTSIZE2 ) {
              *dst_ptr++ = *src_ptr++;
              *dst_ptr++ = - *src_ptr++;
              i += 2;
              dst_ptr += 0;
            }
            dst_blk_x++;
            dst_ptr += 0;
          } while ( dst_blk_x < comp_width );

          /* Any remaining right-edge blocks are only copied. */
          _Pragma( "loopbound min 1 max 1" )
          for ( ; dst_blk_x < compptr_width_in_blocks; dst_blk_x++ ) {

            dst_ptr = cjpeg_transupp_output_data4;
            src_ptr = cjpeg_transupp_input4;
            _Pragma( "loopbound min 1 max 1" )
            do {
              *dst_ptr++ = *src_ptr++;
              i++;
            } while ( i < DCTSIZE2 );
          }
        }
      }
    }
  }
}


/*
  Transverse transpose is equivalent to
    1. 180 degree rotation;
    2. Transposition;
  or
    1. Horizontal mirroring;
    2. Transposition;
    3. Horizontal mirroring.
  These steps are merged into a single processing routine.
*/
void cjpeg_transupp_do_transverse( j_compress_ptr dstinfo )
{
  unsigned int MCU_cols, MCU_rows, comp_width, comp_height, dst_blk_x,
           dst_blk_y;
  int ci, i, j, offset_x, offset_y;
  JCOEFPTR src_ptr, dst_ptr;

  int compptr_h_samp_factor = 2;
  int compptr_v_samp_factor = 8;
  unsigned int compptr_height_in_blocks = 29;
  unsigned int compptr_width_in_blocks = 19;


  MCU_cols = dstinfo->image_width / ( dstinfo->max_h_samp_factor * DCTSIZE );
  MCU_rows = dstinfo->image_height / ( dstinfo->max_v_samp_factor * DCTSIZE );

  _Pragma( "loopbound min 3 max 3" )
  for ( ci = 0; ci < dstinfo->num_components; ci++,
        compptr_h_samp_factor = compptr_v_samp_factor = 1,
        compptr_height_in_blocks = 15, compptr_width_in_blocks  = 10 ) {

    comp_width  = MCU_cols * compptr_h_samp_factor;
    comp_height = MCU_rows * compptr_v_samp_factor;

    _Pragma( "loopbound min 4 max 15" )
    for ( dst_blk_y = 0; dst_blk_y < compptr_height_in_blocks;
          dst_blk_y += compptr_v_samp_factor ) {
      offset_y = 0;
      _Pragma( "loopbound min 1 max 8" )
      do {
        dst_blk_x = 0;
        _Pragma( "loopbound min 5 max 10" )
        do {
          offset_x = 0;
          _Pragma( "loopbound min 1 max 2" )
          for ( ; offset_x < compptr_h_samp_factor; offset_x++ ) {

            if ( dst_blk_y < comp_height ) {
              src_ptr = cjpeg_transupp_input5;

              if ( dst_blk_x < comp_width ) {
                /* Block is within the mirrorable area. */
                dst_ptr = cjpeg_transupp_output_data5;

                _Pragma( "loopbound min 4 max 4" )
                for ( i = 0; i < DCTSIZE; i++ ) {
                  j = 0;
                  _Pragma( "loopbound min 4 max 4" )
                  for ( ; j < DCTSIZE; j++ ) {
                    if ( dst_blk_y < comp_width )
                      dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
                    j++;
                    dst_ptr[ j * DCTSIZE + i ] = -src_ptr[ i * DCTSIZE + j ];
                  }
                  i++;
                  _Pragma( "loopbound min 4 max 4" )
                  for ( j = 0; j < DCTSIZE; j++ ) {
                    if ( dst_blk_y < comp_width )
                      dst_ptr[ j * DCTSIZE + i ] = -src_ptr[ i * DCTSIZE + j ];
                    j++;
                    dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
                  }
                }
              } else {
                /* Right-edge blocks are mirrored in y only */
                dst_ptr = cjpeg_transupp_output_data5;
                _Pragma( "loopbound min 8 max 8" )
                for ( i = 0; i < DCTSIZE; i++ ) {
                  j = 0;
                  _Pragma( "loopbound min 4 max 4" )
                  for ( ; j < DCTSIZE; j++ ) {
                    if ( dst_blk_y < comp_width )
                      dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
                    j++;
                    dst_ptr[ j * DCTSIZE + i ] = -src_ptr[ i * DCTSIZE + j ];
                  }
                }
              }
            } else {
              src_ptr = cjpeg_transupp_input5_2;

              if ( dst_blk_x < comp_width ) {
                /* Bottom-edge blocks are mirrored in x only */
                dst_ptr = cjpeg_transupp_output_data5;

                _Pragma( "loopbound min 4 max 4" )
                for ( i = 0; i < DCTSIZE; i++ ) {
                  j = 0;
                  _Pragma( "loopbound min 8 max 8" )
                  for ( ; j < DCTSIZE; j++ )
                    dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
                  i++;
                  _Pragma( "loopbound min 8 max 8" )
                  for ( j = 0; j < DCTSIZE; j++ )
                    dst_ptr[ j * DCTSIZE + i ] = -src_ptr[ i * DCTSIZE + j ];
                }
              } else {
                /* At lower right corner, just transpose, no mirroring */
                dst_ptr = cjpeg_transupp_output_data5;
                _Pragma( "loopbound min 8 max 8" )
                for ( i = 0; i < DCTSIZE; i++ ) {
                  j = 0;
                  _Pragma( "loopbound min 8 max 8" )
                  for ( ; j < DCTSIZE; j++ )
                    dst_ptr[ j * DCTSIZE + i ] = src_ptr[ i * DCTSIZE + j ];
                }
              }
            }
            dst_blk_x += compptr_h_samp_factor;
          }
        } while ( dst_blk_x < compptr_width_in_blocks );
        offset_y++;
      } while ( offset_y < compptr_v_samp_factor );
    }
  }
}


/*
  Main functions
*/

void _Pragma ( "entrypoint" ) cjpeg_transupp_main( void )
{
  cjpeg_transupp_dstinfo.image_width = 227;
  cjpeg_transupp_dstinfo.image_height = 149;

  cjpeg_transupp_do_flip_v( &cjpeg_transupp_dstinfo );

  cjpeg_transupp_dstinfo.image_width = 149;
  cjpeg_transupp_dstinfo.image_height = 227;

  cjpeg_transupp_do_rot_90( &cjpeg_transupp_dstinfo );
  cjpeg_transupp_do_rot_270( &cjpeg_transupp_dstinfo );

  cjpeg_transupp_dstinfo.image_width = 227;
  cjpeg_transupp_dstinfo.image_height = 149;

  cjpeg_transupp_do_rot_180( &cjpeg_transupp_dstinfo );

  cjpeg_transupp_dstinfo.image_width = 149;
  cjpeg_transupp_dstinfo.image_height = 227;

  cjpeg_transupp_do_transverse( &cjpeg_transupp_dstinfo );
}


int main( void )
{
  cjpeg_transupp_init();
  cjpeg_transupp_main();

  return ( cjpeg_transupp_return() - 1624 != 0 );
}
