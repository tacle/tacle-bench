/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: cjpeg_jpeg6b_wrbmp.c

  Author: Thomas G. Lane.

  Function: This file contains routines to write output images in Microsoft "BMP"
  format (MS Windows 3.x and OS/2 1.x flavors).
  Either 8-bit colormapped or 24-bit full-color format can be written.
  No compression is supported.

  These routines may need modification for non-Unix environments or
  specialized applications.  As they stand, they assume output to
  an ordinary stdio stream.

  Source: Independent JPEG Group's software


  Changes: a brief summary of major functional changes (not formatting)

  License: See the accompanying README file.

*/

#include "cdjpeg.h"

#ifdef CJPEG_JPEG6B_WRBMP_BMP_SUPPORTED

/*
  Declaration of global variables
*/
typedef struct {
  struct cjpeg_jpeg6b_wrbmp_djpeg_dest_struct pub; /* public fields */
  cjpeg_jpeg6b_wrbmp_boolean is_os2;   /* saves the OS2 format request flag */
  cjpeg_jpeg6b_wrbmp_jvirt_sarray_ptr
  whole_image; /* needed to reverse row order */
  CJPEG_JPEG6B_WRBMP_JDIMENSION data_width;  /* JSAMPLEs per row */
  CJPEG_JPEG6B_WRBMP_JDIMENSION
  row_width;   /* physical width of one row in the BMP file */
  int pad_bytes;    /* number of padding bytes needed per row */
  CJPEG_JPEG6B_WRBMP_JDIMENSION
  cur_output_row;  /* next row# to write to virtual array */
} cjpeg_jpeg6b_wrbmp_bmp_dest_struct;

typedef cjpeg_jpeg6b_wrbmp_bmp_dest_struct *cjpeg_jpeg6b_wrbmp_bmp_dest_ptr;
extern unsigned char cjpeg_jpeg6b_wrbmp_colormap[3][256];
unsigned char cjpeg_jpeg6b_wrbmp_output_array[6144];
static unsigned char *cjpeg_jpeg6b_wrbmp_jpeg_stream /*= cjpeg_jpeg6b_wrbmp_output_array*/;

struct cjpeg_jpeg6b_wrbmp_jpeg_decompress_struct
  cjpeg_jpeg6b_wrbmp_jpeg_dec_1;
struct cjpeg_jpeg6b_wrbmp_jpeg_decompress_struct
  cjpeg_jpeg6b_wrbmp_jpeg_dec_2;
struct cjpeg_jpeg6b_wrbmp_djpeg_dest_struct
  cjpeg_jpeg6b_wrbmp_djpeg_dest;
cjpeg_jpeg6b_wrbmp_bmp_dest_struct    cjpeg_jpeg6b_wrbmp_bmp_dest;


/*
  Forward declaration of functions
*/
void cjpeg_jpeg6b_wrbmp_initInput( void );
void cjpeg_jpeg6b_wrbmp_finish_output_bmp( cjpeg_jpeg6b_wrbmp_j_decompress_ptr
    cinfo, cjpeg_jpeg6b_wrbmp_djpeg_dest_ptr dinfo );
void cjpeg_jpeg6b_wrbmp_write_colormap( cjpeg_jpeg6b_wrbmp_j_decompress_ptr
                                        cinfo,
                                        cjpeg_jpeg6b_wrbmp_bmp_dest_ptr dest, int map_colors, int map_entry_size,
                                        int cMap );
int cjpeg_jpeg6b_wrbmp_putc_modified( int character );
void cjpeg_jpeg6b_wrbmp_init();
void cjpeg_jpeg6b_wrbmp_main();
int cjpeg_jpeg6b_wrbmp_return();
int main();

/*
   Initialization functions
*/
void cjpeg_jpeg6b_wrbmp_init()
{
  cjpeg_jpeg6b_wrbmp_initInput();

  cjpeg_jpeg6b_wrbmp_jpeg_dec_1.progress                = 0;
  cjpeg_jpeg6b_wrbmp_jpeg_dec_1.output_height           = 30;
  cjpeg_jpeg6b_wrbmp_jpeg_dec_1.actual_number_of_colors = 256;
  cjpeg_jpeg6b_wrbmp_jpeg_dec_1.out_color_components    = 2;

  cjpeg_jpeg6b_wrbmp_jpeg_dec_2.progress                = 0;
  cjpeg_jpeg6b_wrbmp_jpeg_dec_2.output_height           = 30;
  cjpeg_jpeg6b_wrbmp_jpeg_dec_2.actual_number_of_colors = 256;
  cjpeg_jpeg6b_wrbmp_jpeg_dec_2.out_color_components    = 3;

  cjpeg_jpeg6b_wrbmp_jpeg_stream = cjpeg_jpeg6b_wrbmp_output_array;

}

/*
   Calculation functions
*/
int cjpeg_jpeg6b_wrbmp_putc_modified( int character )
{
  *( cjpeg_jpeg6b_wrbmp_jpeg_stream ) = character;

  ++cjpeg_jpeg6b_wrbmp_jpeg_stream;

  return character;
}

void cjpeg_jpeg6b_wrbmp_finish_output_bmp( cjpeg_jpeg6b_wrbmp_j_decompress_ptr
    cinfo,
    cjpeg_jpeg6b_wrbmp_djpeg_dest_ptr dinfo )
{
  CJPEG_JPEG6B_WRBMP_JDIMENSION row;
  cjpeg_jpeg6b_wrbmp_cd_progress_ptr progress =
    ( cjpeg_jpeg6b_wrbmp_cd_progress_ptr ) cinfo->progress;

  // Write the file body from our virtual array
  _Pragma( "loopbound min 30 max 30" )
  for ( row = cinfo->output_height; row > 0; --row ) {
    if ( progress != 0 ) {
      progress->pub.pass_counter = ( long )( cinfo->output_height - row );
      progress->pub.pass_limit = ( long ) cinfo->output_height;
    }
  }

  if ( progress != 0 )
    progress->completed_extra_passes++;
}

void cjpeg_jpeg6b_wrbmp_write_colormap( cjpeg_jpeg6b_wrbmp_j_decompress_ptr
                                        cinfo,
                                        cjpeg_jpeg6b_wrbmp_bmp_dest_ptr dest,
                                        int map_colors, int map_entry_size, int cMap )
{

  int num_colors = cinfo->actual_number_of_colors;
  int i;

  if ( cMap != 0 ) {

    if ( cinfo->out_color_components == 3 ) {
      // Normal case with RGB colormap
      _Pragma( "loopbound min 256 max 256" )
      for ( i = 0; i < num_colors; i++ ) {
        cjpeg_jpeg6b_wrbmp_putc_modified( CJPEG_JPEG6B_WRBMP_GETJSAMPLE(
                                            cjpeg_jpeg6b_wrbmp_colormap[2][i] ) );
        cjpeg_jpeg6b_wrbmp_putc_modified( CJPEG_JPEG6B_WRBMP_GETJSAMPLE(
                                            cjpeg_jpeg6b_wrbmp_colormap[1][i] ) );
        cjpeg_jpeg6b_wrbmp_putc_modified( CJPEG_JPEG6B_WRBMP_GETJSAMPLE(
                                            cjpeg_jpeg6b_wrbmp_colormap[0][i] ) );

        if ( map_entry_size == 4 )
          cjpeg_jpeg6b_wrbmp_putc_modified( 0 );
      }
    } else {
      // Grayscale colormap (only happens with grayscale quantization)
      _Pragma( "loopbound min 256 max 256" )
      for ( i = 0; i < num_colors; i++ ) {

        cjpeg_jpeg6b_wrbmp_putc_modified( CJPEG_JPEG6B_WRBMP_GETJSAMPLE(
                                            cjpeg_jpeg6b_wrbmp_colormap[2][i] ) );
        cjpeg_jpeg6b_wrbmp_putc_modified( CJPEG_JPEG6B_WRBMP_GETJSAMPLE(
                                            cjpeg_jpeg6b_wrbmp_colormap[1][i] ) );
        cjpeg_jpeg6b_wrbmp_putc_modified( CJPEG_JPEG6B_WRBMP_GETJSAMPLE(
                                            cjpeg_jpeg6b_wrbmp_colormap[0][i] ) );

        if ( map_entry_size == 4 )
          cjpeg_jpeg6b_wrbmp_putc_modified( 0 );
      }
    }
  } else {
    // If no colormap, must be grayscale data.  Generate a linear "map".
    _Pragma( "loopbound min 256 max 256" )
    for ( i = 0; i < 256; i++ ) {
      cjpeg_jpeg6b_wrbmp_putc_modified( i );
      cjpeg_jpeg6b_wrbmp_putc_modified( i );
      cjpeg_jpeg6b_wrbmp_putc_modified( i );

      if ( map_entry_size == 4 )
        cjpeg_jpeg6b_wrbmp_putc_modified( 0 );
    }
  }

  // Pad colormap with zeros to ensure specified number of colormap entries.
  _Pragma( "loopbound min 512 max 512" )
  for ( ; i < map_colors; i++ ) {
    cjpeg_jpeg6b_wrbmp_putc_modified( 0 );
    cjpeg_jpeg6b_wrbmp_putc_modified( 0 );
    cjpeg_jpeg6b_wrbmp_putc_modified( 0 );

    if ( map_entry_size == 4 )
      cjpeg_jpeg6b_wrbmp_putc_modified( 0 );
  }
}

void cjpeg_jpeg6b_wrbmp_main()
{
  cjpeg_jpeg6b_wrbmp_finish_output_bmp( &cjpeg_jpeg6b_wrbmp_jpeg_dec_1,
                                        &cjpeg_jpeg6b_wrbmp_djpeg_dest );
  cjpeg_jpeg6b_wrbmp_write_colormap(    &cjpeg_jpeg6b_wrbmp_jpeg_dec_1,
                                        &cjpeg_jpeg6b_wrbmp_bmp_dest, 768, 4, 1 );

  cjpeg_jpeg6b_wrbmp_finish_output_bmp( &cjpeg_jpeg6b_wrbmp_jpeg_dec_2,
                                        &cjpeg_jpeg6b_wrbmp_djpeg_dest );
  cjpeg_jpeg6b_wrbmp_write_colormap(    &cjpeg_jpeg6b_wrbmp_jpeg_dec_2,
                                        &cjpeg_jpeg6b_wrbmp_bmp_dest, 768, 4, 1 );
}

int cjpeg_jpeg6b_wrbmp_return()
{
  return 0;
}

int main( void )
{
  cjpeg_jpeg6b_wrbmp_init();
  cjpeg_jpeg6b_wrbmp_main();
  return ( cjpeg_jpeg6b_wrbmp_return() );
}

#endif /* BMP_SUPPORTED */

