/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: cdjpeg.h

  Author: Thomas G. Lane.

  This file is part of the Independent JPEG Group's software.
  For conditions of distribution and use, see the accompanying README file.

  This file contains common declarations for the sample applications
  cjpeg and djpeg.  It is NOT used by the core JPEG library.

  Source: Independent JPEG Group's software

  Changes: no major functional changes

  License:  See the accompanying README file

*/
#ifndef CDJPEG_H
#define CDJPEG_H

#define CJPEG_WRBMP_JPEG_CJPEG_DJPEG  /* define proper options in jconfig.h */
#define CJPEG_WRBMP_JPEG_INTERNAL_OPTIONS /* cjpeg.c,djpeg.c need to see xxx_SUPPORTED */

#include "jpeglib.h"
#include "jerror.h"   /* get library error codes too */
#include "cderror.h"    /* get application-specific error codes */

typedef struct cjpeg_wrbmp_cjpeg_source_struct
  *cjpeg_wrbmp_cjpeg_source_ptr;

struct cjpeg_wrbmp_cjpeg_source_struct {
  CJPEG_WRBMP_JMETHOD( void, start_input,
                       ( cjpeg_wrbmp_j_compress_ptr cinfo,
                         cjpeg_wrbmp_cjpeg_source_ptr sinfo ) );
  CJPEG_WRBMP_JMETHOD( CJPEG_WRBMP_JDIMENSION, get_pixel_rows,
                       ( cjpeg_wrbmp_j_compress_ptr cinfo,
                         cjpeg_wrbmp_cjpeg_source_ptr sinfo ) );
  CJPEG_WRBMP_JMETHOD( void, finish_input,
                       ( cjpeg_wrbmp_j_compress_ptr cinfo,
                         cjpeg_wrbmp_cjpeg_source_ptr sinfo ) );

  CJPEG_WRBMP_FILE *input_file;

  CJPEG_WRBMP_JSAMPARRAY buffer;
  CJPEG_WRBMP_JDIMENSION buffer_height;
};


typedef struct cjpeg_wrbmp_djpeg_dest_struct
  *cjpeg_wrbmp_djpeg_dest_ptr;

struct cjpeg_wrbmp_djpeg_dest_struct {
  CJPEG_WRBMP_JMETHOD( void, start_output,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo,
                         cjpeg_wrbmp_djpeg_dest_ptr dinfo ) );
  /* Emit the specified number of pixel rows from the buffer. */
  CJPEG_WRBMP_JMETHOD( void, put_pixel_rows,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo,
                         cjpeg_wrbmp_djpeg_dest_ptr dinfo,
                         CJPEG_WRBMP_JDIMENSION rows_supplied ) );
  /* Finish up at the end of the image. */
  CJPEG_WRBMP_JMETHOD( void, finish_output,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo,
                         cjpeg_wrbmp_djpeg_dest_ptr dinfo ) );

  /* Target file spec; filled in by djpeg.c after object is created. */
  CJPEG_WRBMP_FILE *output_file;

  /* Output pixel-row buffer.  Created by module init or start_output.
     Width is cinfo->output_width * cinfo->output_components;
     height is buffer_height.
  */
  CJPEG_WRBMP_JSAMPARRAY buffer;
  CJPEG_WRBMP_JDIMENSION buffer_height;
} ;



/*
   cjpeg/djpeg may need to perform extra passes to convert to or from
   the source/destination file format.  The JPEG library does not know
   about these passes, but we'd like them to be counted by the progress
   monitor.  We use an expanded progress monitor object to hold the
   additional pass count.
*/

struct cjpeg_wrbmp_cdjpeg_progress_mgr {
  struct cjpeg_wrbmp_jpeg_progress_mgr
    pub; /* fields known to JPEG library */
  int completed_extra_passes; /* extra passes completed */
  int total_extra_passes; /* total extra */
  /* last printed percentage stored here to avoid multiple printouts */
  int percent_done;
};

typedef struct cjpeg_wrbmp_cdjpeg_progress_mgr
  *cjpeg_wrbmp_cd_progress_ptr;

#endif

