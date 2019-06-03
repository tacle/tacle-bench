/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: jpeglib.h

  Author: Thomas G. Lane.

  This file is part of the Independent JPEG Group's software.
  For conditions of distribution and use, see the accompanying README file.

  This file defines the application interface for the JPEG library.
  Most applications using the library need only include this file,
  and perhaps jerror.h if they want to know the exact error codes.

  Source: Independent JPEG Group's software

  Changes: no major functional changes

  License:  See the accompanying README file
*/


#ifndef JPEGLIB_H
#define JPEGLIB_H

typedef int CJPEG_WRBMP_FILE;
typedef unsigned int cjpeg_wrbmp_size_t;

/*
   First we include the configuration files that record how this
   installation of the JPEG library is set up.  jconfig.h can be
   generated automatically for many systems.  jmorecfg.h contains
   manual configuration options that most people need not worry about.
*/

#ifndef JCONFIG_INCLUDED  /* in case jinclude.h already did */
#include "jconfig.h"    /* widely used configuration options */
#endif
#include "jmorecfg.h"   /* seldom changed options */

/* Data structures for images (arrays of samples and of DCT coefficients).
   On 80x86 machines, the image arrays are too big for near pointers,
   but the pointer arrays can fit in near memory.
*/

typedef CJPEG_WRBMP_JSAMPLE CJPEG_WRBMP_FAR
*CJPEG_WRBMP_JSAMPROW;  /* ptr to one image row of pixel samples. */
typedef CJPEG_WRBMP_JSAMPROW
*CJPEG_WRBMP_JSAMPARRAY; /* ptr to some rows (a 2-D sample array) */
typedef CJPEG_WRBMP_JSAMPARRAY
*CJPEG_WRBMP_JSAMPIMAGE; /* a 3-D sample array: top index is color */

typedef CJPEG_WRBMP_JCOEF
CJPEG_WRBMP_JBLOCK[ 64 ]; /* one block of coefficients */
typedef CJPEG_WRBMP_JBLOCK CJPEG_WRBMP_FAR
*CJPEG_WRBMP_JBLOCKROW;  /* pointer to one row of coefficient blocks */
typedef CJPEG_WRBMP_JBLOCKROW
*CJPEG_WRBMP_JBLOCKARRAY;   /* a 2-D array of coefficient blocks */
typedef CJPEG_WRBMP_JBLOCKARRAY
*CJPEG_WRBMP_JBLOCKIMAGE; /* a 3-D array of coefficient blocks */

typedef CJPEG_WRBMP_JCOEF CJPEG_WRBMP_FAR
*JCOEFPTR;  /* useful in a couple of places */


/* Types for JPEG compression parameters and working tables. */


/* DCT coefficient quantization tables. */

typedef struct {
  /* This array gives the coefficient quantizers in natural array order
     (not the zigzag order in which they are stored in a JPEG DQT marker).
     CAUTION: IJG versions prior to v6a kept this array in zigzag order.
  */
  CJPEG_WRBMP_UINT16
  quantval[ 64 ];  /* quantization step for each coefficient */
  /* This field is used only during compression.  It's initialized FALSE when
     the table is created, and set TRUE when it's been output to the file.
     You could suppress output of a table by setting this to TRUE.
     (See jpeg_suppress_tables for an example.)
  */
  cjpeg_wrbmp_boolean sent_table;   /* TRUE when table has been output */
} CJPEG_WRBMP_JQUANT_TBL;


/* Huffman coding tables. */

typedef struct {
  /* These two fields directly represent the contents of a JPEG DHT marker */
  CJPEG_WRBMP_UINT8 bits[ 17 ];   /* bits[ k ] = # of symbols with codes of */
  /* length k bits; bits[ 0 ] is unused */
  CJPEG_WRBMP_UINT8
  huffval[ 256 ];   /* The symbols, in order of incr code length */
  /* This field is used only during compression.  It's initialized FALSE when
     the table is created, and set TRUE when it's been output to the file.
     You could suppress output of a table by setting this to TRUE.
     (See jpeg_suppress_tables for an example.)
  */
  cjpeg_wrbmp_boolean sent_table;   /* TRUE when table has been output */
} CJPEG_WRBMP_JHUFF_TBL;


/* Basic info about one component (color channel). */

typedef struct {
  /* These values are fixed over the whole image. */
  /* For compression, they must be supplied by parameter setup; */
  /* for decompression, they are read from the SOF marker. */
  int component_id;   /* identifier for this component (0..255) */
  int component_index;    /* its index in SOF or cinfo->comp_info[  ] */
  int h_samp_factor;    /* horizontal sampling factor (1..4) */
  int v_samp_factor;    /* vertical sampling factor (1..4) */
  int quant_tbl_no;   /* quantization table selector (0..3) */
  /* These values may vary between scans. */
  /* For compression, they must be supplied by parameter setup; */
  /* for decompression, they are read from the SOS marker. */
  /* The decompressor output side may not use these variables. */
  int dc_tbl_no;    /* DC entropy table selector (0..3) */
  int ac_tbl_no;    /* AC entropy table selector (0..3) */

  /* Remaining fields should be treated as private by applications. */

  /* These values are computed during compression or decompression startup: */
  /* Component's size in DCT blocks.
     Any dummy blocks added to complete an MCU are not counted; therefore
     these values do not depend on whether a scan is interleaved or not.
  */
  CJPEG_WRBMP_JDIMENSION width_in_blocks;
  CJPEG_WRBMP_JDIMENSION height_in_blocks;
  /* Size of a DCT block in samples.  Always DCTSIZE for compression.
     For decompression this is the size of the output from one DCT block,
     reflecting any scaling we choose to apply during the IDCT step.
     Values of 1,2,4,8 are likely to be supported.  Note that different
     components may receive different IDCT scalings.
  */
  int DCT_scaled_size;
  /* The downsampled dimensions are the component's actual, unpadded number
     of samples at the main buffer (preprocessing/compression interface), thus
     downsampled_width = ceil(image_width * Hi/Hmax)
     and similarly for height.  For decompression, IDCT scaling is included, so
     downsampled_width = ceil(image_width * Hi/Hmax * DCT_scaled_size/DCTSIZE)
  */
  CJPEG_WRBMP_JDIMENSION downsampled_width;  /* actual width in samples */
  CJPEG_WRBMP_JDIMENSION downsampled_height; /* actual height in samples */
  /* This flag is used only for decompression.  In cases where some of the
     components will be ignored (eg grayscale output from YCbCr image),
     we can skip most computations for the unused components.
  */
  cjpeg_wrbmp_boolean
  component_needed; /* do we need the value of this component? */

  /* These values are computed before starting a scan of the component. */
  /* The decompressor output side may not use these variables. */
  int MCU_width;    /* number of blocks per MCU, horizontally */
  int MCU_height;   /* number of blocks per MCU, vertically */
  int MCU_blocks;   /* MCU_width * MCU_height */
  int MCU_sample_width;   /* MCU width in samples, MCU_width*DCT_scaled_size */
  int last_col_width;   /* # of non-dummy blocks across in last MCU */
  int last_row_height;    /* # of non-dummy blocks down in last MCU */

  /* Saved quantization table for component; NULL if none yet saved.
     See jdinput.c comments about the need for this information.
     This field is currently used only for decompression.
  */
  CJPEG_WRBMP_JQUANT_TBL *quant_table;

  /* Private per-component storage for DCT or IDCT subsystem. */
  void *dct_table;
} cjpeg_wrbmp_jpeg_component_info;


/* The script for encoding a multiple-scan file is an array of these: */

typedef struct {
  int comps_in_scan;    /* number of components encoded in this scan */
  int component_index[ 4 ]; /* their SOF/comp_info[  ] indexes */
  int Ss, Se;     /* progressive JPEG spectral selection parms */
  int Ah, Al;     /* progressive JPEG successive approx. parms */
} cjpeg_wrbmp_jpeg_scan_info;

/* The decompressor can save APPn and COM markers in a list of these: */

typedef struct cjpeg_wrbmp_jpeg_marker_struct CJPEG_WRBMP_FAR
  *jpeg_saved_marker_ptr;

struct cjpeg_wrbmp_jpeg_marker_struct {
  jpeg_saved_marker_ptr next; /* next in list, or NULL */
  CJPEG_WRBMP_UINT8 marker;     /* marker code: JPEG_COM, or JPEG_APP0+n */
  unsigned int original_length; /* # bytes of data in the file */
  unsigned int data_length; /* # bytes of data saved at data[  ] */
  CJPEG_WRBMP_JOCTET CJPEG_WRBMP_FAR
  *data;     /* the data contained in the marker */
  /* the marker length word is not counted in data_length or original_length */
};

/* Known color spaces. */

typedef enum {
  JCS_UNKNOWN,    /* error/unspecified */
  JCS_GRAYSCALE,    /* monochrome */
  JCS_RGB,    /* red/green/blue */
  JCS_YCbCr,    /* Y/Cb/Cr (also known as YUV) */
  JCS_CMYK,   /* C/M/Y/K */
  JCS_YCCK    /* Y/Cb/Cr/K */
} CJPEG_WRBMP_J_COLOR_SPACE;

/* DCT/IDCT algorithm options. */

typedef enum {
  JDCT_ISLOW,   /* slow but accurate integer algorithm */
  JDCT_IFAST,   /* faster, less accurate integer method */
  JDCT_FLOAT    /* floating-point: accurate, fast on fast HW */
} CJPEG_WRBMP_J_DCT_METHOD;

/* Dithering options for decompression. */

typedef enum {
  JDITHER_NONE,   /* no dithering */
  JDITHER_ORDERED,  /* simple ordered dither */
  JDITHER_FS    /* Floyd-Steinberg error diffusion dither */
} CJPEG_WRBMP_J_DITHER_MODE;


/* Common fields between JPEG compression and decompression master structs. */

#define cjpeg_wrbmp_jpeg_common_fields \
  struct cjpeg_wrbmp_jpeg_error_mgr * err;  /* Error handler module */\
  struct cjpeg_wrbmp_jpeg_memory_mgr * mem; /* Memory manager module */\
  struct cjpeg_wrbmp_jpeg_progress_mgr * progress; /* Progress monitor, or NULL if none */\
  void * client_data;   /* Available for use by application */\
  cjpeg_wrbmp_boolean is_decompressor;  /* So common code can tell which is which */\
  int global_state    /* For checking call sequence validity */

/* Routines that are to be used by both halves of the library are declared
   to receive a pointer to this structure.  There are no actual instances of
   jpeg_common_struct, only of jpeg_compress_struct and jpeg_decompress_struct.
*/
struct cjpeg_wrbmp_jpeg_common_struct {
  cjpeg_wrbmp_jpeg_common_fields;   /* Fields common to both master struct types */
  /* Additional fields follow in an actual jpeg_compress_struct or
     jpeg_decompress_struct.  All three structs must agree on these
     initial fields!  (This would be a lot cleaner in C++.)
  */
};

typedef struct cjpeg_wrbmp_jpeg_common_struct
  *cjpeg_wrbmp_j_common_ptr;
typedef struct cjpeg_wrbmp_jpeg_compress_struct
  *cjpeg_wrbmp_j_compress_ptr;
typedef struct cjpeg_wrbmp_jpeg_decompress_struct
  *cjpeg_wrbmp_j_decompress_ptr;


/* Master record for a compression instance */

struct cjpeg_wrbmp_jpeg_compress_struct {
  cjpeg_wrbmp_jpeg_common_fields;   /* Fields shared with jpeg_decompress_struct */

  /* Destination for compressed data */
  struct cjpeg_wrbmp_jpeg_destination_mgr *dest;

  /* Description of source image --- these fields must be filled in by
     outer application before starting compression.  in_color_space must
     be correct before you can even call jpeg_set_defaults().
  */

  CJPEG_WRBMP_JDIMENSION image_width; /* input image width */
  CJPEG_WRBMP_JDIMENSION image_height;  /* input image height */
  int input_components;   /* # of color components in input image */
  CJPEG_WRBMP_J_COLOR_SPACE in_color_space; /* colorspace of input image */

  float input_gamma;    /* image gamma of input image */

  /* Compression parameters --- these fields must be set before calling
     jpeg_start_compress().  We recommend calling jpeg_set_defaults() to
     initialize everything to reasonable defaults, then changing anything
     the application specifically wants to change.  That way you won't get
     burnt when new parameters are added.  Also note that there are several
     helper routines to simplify changing parameters.
  */

  int data_precision;   /* bits of precision in image data */

  int num_components;   /* # of color components in JPEG image */
  CJPEG_WRBMP_J_COLOR_SPACE
  jpeg_color_space; /* colorspace of JPEG image */

  cjpeg_wrbmp_jpeg_component_info *comp_info;
  /* comp_info[ i ] describes component that appears i'th in SOF */

  CJPEG_WRBMP_JQUANT_TBL *quant_tbl_ptrs[ 4 ];
  /* ptrs to coefficient quantization tables, or NULL if not defined */

  CJPEG_WRBMP_JHUFF_TBL *dc_huff_tbl_ptrs[ 4 ];
  CJPEG_WRBMP_JHUFF_TBL *ac_huff_tbl_ptrs[ 4 ];
  /* ptrs to Huffman coding tables, or NULL if not defined */

  CJPEG_WRBMP_UINT8
  arith_dc_L[ 16 ]; /* L values for DC arith-coding tables */
  CJPEG_WRBMP_UINT8
  arith_dc_U[ 16 ]; /* U values for DC arith-coding tables */
  CJPEG_WRBMP_UINT8
  arith_ac_K[ 16 ]; /* Kx values for AC arith-coding tables */

  int num_scans;    /* # of entries in scan_info array */
  const cjpeg_wrbmp_jpeg_scan_info
  *scan_info;  /* script for multi-scan file, or NULL */
  /* The default value of scan_info is NULL, which causes a single-scan
     sequential JPEG file to be emitted.  To create a multi-scan file,
     set num_scans and scan_info to point to an array of scan definitions.
  */

  cjpeg_wrbmp_boolean
  raw_data_in;    /* TRUE=caller supplies downsampled data */
  cjpeg_wrbmp_boolean
  arith_code;   /* TRUE=arithmetic coding, FALSE=Huffman */
  cjpeg_wrbmp_boolean
  optimize_coding;  /* TRUE=optimize entropy encoding parms */
  cjpeg_wrbmp_boolean
  CCIR601_sampling; /* TRUE=first samples are cosited */
  int smoothing_factor;   /* 1..100, or 0 for no input smoothing */
  CJPEG_WRBMP_J_DCT_METHOD dct_method;  /* DCT algorithm selector */

  /* The restart interval can be specified in absolute MCUs by setting
     restart_interval, or in MCU rows by setting restart_in_rows
     (in which case the correct restart_interval will be figured
     for each scan).
  */
  unsigned int restart_interval; /* MCUs per restart, or 0 for no restart */
  int restart_in_rows;    /* if > 0, MCU rows per restart interval */

  /* Parameters controlling emission of special markers. */

  cjpeg_wrbmp_boolean
  write_JFIF_header;  /* should a JFIF marker be written? */
  CJPEG_WRBMP_UINT8
  JFIF_major_version; /* What to write for the JFIF version number */
  CJPEG_WRBMP_UINT8 JFIF_minor_version;
  /* These three values are not used by the JPEG code, merely copied */
  /* into the JFIF APP0 marker.  density_unit can be 0 for unknown, */
  /* 1 for dots/inch, or 2 for dots/cm.  Note that the pixel aspect */
  /* ratio is defined by X_density/Y_density even when density_unit=0. */
  CJPEG_WRBMP_UINT8 density_unit;   /* JFIF code for pixel size units */
  CJPEG_WRBMP_UINT16 X_density;   /* Horizontal pixel density */
  CJPEG_WRBMP_UINT16 Y_density;   /* Vertical pixel density */
  cjpeg_wrbmp_boolean
  write_Adobe_marker; /* should an Adobe marker be written? */

  /* State variable: index of next scanline to be written to
     jpeg_write_scanlines().  Application may use this to control its
     processing loop, e.g., "while (next_scanline < image_height)".
  */

  CJPEG_WRBMP_JDIMENSION next_scanline; /* 0 .. image_height-1  */

  /* Remaining fields are known throughout compressor, but generally
     should not be touched by a surrounding application.
  */

  /*
     These fields are computed during compression startup
  */
  cjpeg_wrbmp_boolean
  progressive_mode; /* TRUE if scan script uses progressive mode */
  int max_h_samp_factor;  /* largest h_samp_factor */
  int max_v_samp_factor;  /* largest v_samp_factor */

  CJPEG_WRBMP_JDIMENSION
  total_iMCU_rows; /* # of iMCU rows to be input to coef ctlr */
  /* The coefficient controller receives data in units of MCU rows as defined
     for fully interleaved scans (whether the JPEG file is interleaved or not).
     There are v_samp_factor * DCTSIZE sample rows of each component in an
     "iMCU" (interleaved MCU) row.
  */

  /*
     These fields are valid during any one scan.
     They describe the components and MCUs actually appearing in the scan.
  */
  int comps_in_scan;    /* # of JPEG components in this scan */
  cjpeg_wrbmp_jpeg_component_info *cur_comp_info[ 4 ];
  /* *cur_comp_info[ i ] describes component that appears i'th in SOS */

  CJPEG_WRBMP_JDIMENSION MCUs_per_row;  /* # of MCUs across the image */
  CJPEG_WRBMP_JDIMENSION
  MCU_rows_in_scan;  /* # of MCU rows in the image */

  int blocks_in_MCU;    /* # of DCT blocks per MCU */
  int MCU_membership[ 10 ];
  /* MCU_membership[ i ] is index in cur_comp_info of component owning */
  /* i'th block in an MCU */

  int Ss, Se, Ah, Al;   /* progressive JPEG parameters for scan */

  cjpeg_wrbmp_jpeg_scan_info
  *script_space;  /* workspace for jpeg_simple_progression */
  int script_space_size;
};


/* Master record for a decompression instance */

struct cjpeg_wrbmp_jpeg_decompress_struct {
  cjpeg_wrbmp_jpeg_common_fields;   /* Fields shared with jpeg_compress_struct */

  /* Source of compressed data */
  struct cjpeg_wrbmp_jpeg_source_mgr *src;

  /* Basic description of image --- filled in by jpeg_read_header(). */
  /* Application may inspect these values to decide how to process image. */

  CJPEG_WRBMP_JDIMENSION
  image_width; /* nominal image width (from SOF marker) */
  CJPEG_WRBMP_JDIMENSION image_height;  /* nominal image height */
  int num_components;   /* # of color components in JPEG image */
  CJPEG_WRBMP_J_COLOR_SPACE
  jpeg_color_space; /* colorspace of JPEG image */

  /* Decompression processing parameters --- these fields must be set before
     calling jpeg_start_decompress().  Note that jpeg_read_header() initializes
     them to default values.
  */

  CJPEG_WRBMP_J_COLOR_SPACE out_color_space; /* colorspace for output */

  unsigned int scale_num, scale_denom; /* fraction by which to scale image */

  float output_gamma;   /* image gamma wanted in output */

  cjpeg_wrbmp_boolean buffered_image; /* TRUE=multiple output passes */
  cjpeg_wrbmp_boolean raw_data_out;   /* TRUE=downsampled data wanted */

  CJPEG_WRBMP_J_DCT_METHOD dct_method;  /* IDCT algorithm selector */
  cjpeg_wrbmp_boolean
  do_fancy_upsampling;  /* TRUE=apply fancy upsampling */
  cjpeg_wrbmp_boolean
  do_block_smoothing; /* TRUE=apply interblock smoothing */

  cjpeg_wrbmp_boolean
  quantize_colors;  /* TRUE=colormapped output wanted */
  /* the following are ignored if not quantize_colors: */
  CJPEG_WRBMP_J_DITHER_MODE
  dither_mode;  /* type of color dithering to use */
  cjpeg_wrbmp_boolean
  two_pass_quantize;  /* TRUE=use two-pass color quantization */
  int desired_number_of_colors; /* max # colors to use in created colormap */
  /* these are significant only in buffered-image mode: */
  cjpeg_wrbmp_boolean
  enable_1pass_quant; /* enable future use of 1-pass quantizer */
  cjpeg_wrbmp_boolean
  enable_EXTERNal_quant;/* enable future use of EXTERNal colormap */
  cjpeg_wrbmp_boolean
  enable_2pass_quant; /* enable future use of 2-pass quantizer */

  /* Description of actual output image that will be returned to application.
     These fields are computed by jpeg_start_decompress().
     You can also use jpeg_calc_output_dimensions() to determine these values
     in advance of calling jpeg_start_decompress().
  */

  CJPEG_WRBMP_JDIMENSION output_width;  /* scaled image width */
  CJPEG_WRBMP_JDIMENSION output_height; /* scaled image height */
  int out_color_components; /* # of color components in out_color_space */
  int output_components;  /* # of color components returned */
  /* output_components is 1 (a colormap index) when quantizing colors;
     otherwise it equals out_color_components.
  */
  int rec_outbuf_height;  /* min recommended height of scanline buffer */
  /* If the buffer passed to jpeg_read_scanlines() is less than this many rows
     high, space and time will be wasted due to unnecessary data copying.
     Usually rec_outbuf_height will be 1 or 2, at most 4.
  */

  /* When quantizing colors, the output colormap is described by these fields.
     The application can supply a colormap by setting colormap non-NULL before
     calling jpeg_start_decompress; otherwise a colormap is created during
     jpeg_start_decompress or jpeg_start_output.
     The map has out_color_components rows and actual_number_of_colors columns.
  */
  int actual_number_of_colors;  /* number of entries in use */
  CJPEG_WRBMP_JSAMPARRAY
  colormap;    /* The color map as a 2-D pixel array */

  /* State variables: these variables indicate the progress of decompression.
     The application may examine these but must not modify them.
  */

  /* Row index of next scanline to be read from jpeg_read_scanlines().
     Application may use this to control its processing loop, e.g.,
     "while (output_scanline < output_height)".
  */
  CJPEG_WRBMP_JDIMENSION output_scanline; /* 0 .. output_height-1  */

  /* Current input scan number and number of iMCU rows completed in scan.
     These indicate the progress of the decompressor input side.
  */
  int input_scan_number;  /* Number of SOS markers seen so far */
  CJPEG_WRBMP_JDIMENSION
  input_iMCU_row;  /* Number of iMCU rows completed */

  /* The "output scan number" is the notional scan being displayed by the
     output side.  The decompressor will not allow output scan/row number
     to get ahead of input scan/row, but it can fall arbitrarily far behind.
  */
  int output_scan_number; /* Nominal scan number being displayed */
  CJPEG_WRBMP_JDIMENSION output_iMCU_row; /* Number of iMCU rows read */

  /* Current progression status.  coef_bits[ c ][ i ] indicates the precision
     with which component c's DCT coefficient i (in zigzag order) is known.
     It is -1 when no data has yet been received, otherwise it is the point
     transform (shift) value for the most recent scan of the coefficient
     (thus, 0 at completion of the progression).
     This pointer is NULL when reading a non-progressive file.
  */
  int ( *coef_bits )[ 64 ]; /* -1 or current Al value for each coef */

  /* Internal JPEG parameters --- the application usually need not look at
     these fields.  Note that the decompressor output side may not use
     any parameters that can change between scans.
  */

  /* Quantization and Huffman tables are carried forward across input
     datastreams when processing abbreviated JPEG datastreams.
  */

  CJPEG_WRBMP_JQUANT_TBL *quant_tbl_ptrs[ 4 ];
  /* ptrs to coefficient quantization tables, or NULL if not defined */

  CJPEG_WRBMP_JHUFF_TBL *dc_huff_tbl_ptrs[ 4 ];
  CJPEG_WRBMP_JHUFF_TBL *ac_huff_tbl_ptrs[ 4 ];
  /* ptrs to Huffman coding tables, or NULL if not defined */

  /* These parameters are never carried across datastreams, since they
     are given in SOF/SOS markers or defined to be reset by SOI.
  */

  int data_precision;   /* bits of precision in image data */

  cjpeg_wrbmp_jpeg_component_info *comp_info;
  /* comp_info[ i ] describes component that appears i'th in SOF */

  cjpeg_wrbmp_boolean
  progressive_mode; /* TRUE if SOFn specifies progressive mode */
  cjpeg_wrbmp_boolean
  arith_code;   /* TRUE=arithmetic coding, FALSE=Huffman */

  CJPEG_WRBMP_UINT8
  arith_dc_L[ 16 ]; /* L values for DC arith-coding tables */
  CJPEG_WRBMP_UINT8
  arith_dc_U[ 16 ]; /* U values for DC arith-coding tables */
  CJPEG_WRBMP_UINT8
  arith_ac_K[ 16 ]; /* Kx values for AC arith-coding tables */

  unsigned int
  restart_interval; /* MCUs per restart interval, or 0 for no restart */

  /* These fields record data obtained from optional markers recognized by
     the JPEG library.
  */
  cjpeg_wrbmp_boolean
  saw_JFIF_marker;  /* TRUE iff a JFIF APP0 marker was found */
  /* Data copied from JFIF marker; only valid if saw_JFIF_marker is TRUE: */
  CJPEG_WRBMP_UINT8 JFIF_major_version; /* JFIF version number */
  CJPEG_WRBMP_UINT8 JFIF_minor_version;
  CJPEG_WRBMP_UINT8 density_unit;   /* JFIF code for pixel size units */
  CJPEG_WRBMP_UINT16 X_density;   /* Horizontal pixel density */
  CJPEG_WRBMP_UINT16 Y_density;   /* Vertical pixel density */
  cjpeg_wrbmp_boolean
  saw_Adobe_marker; /* TRUE iff an Adobe APP14 marker was found */
  CJPEG_WRBMP_UINT8
  Adobe_transform;  /* Color transform code from Adobe marker */

  cjpeg_wrbmp_boolean
  CCIR601_sampling; /* TRUE=first samples are cosited */

  /* Aside from the specific data retained from APPn markers known to the
     library, the uninterpreted contents of any or all APPn and COM markers
     can be saved in a list for examination by the application.
  */
  jpeg_saved_marker_ptr marker_list; /* Head of list of saved markers */

  /* Remaining fields are known throughout decompressor, but generally
     should not be touched by a surrounding application.
  */

  /*
     These fields are computed during decompression startup
  */
  int max_h_samp_factor;  /* largest h_samp_factor */
  int max_v_samp_factor;  /* largest v_samp_factor */

  int min_DCT_scaled_size;  /* smallest DCT_scaled_size of any component */

  CJPEG_WRBMP_JDIMENSION total_iMCU_rows; /* # of iMCU rows in image */
  /* The coefficient controller's input and output progress is measured in
     units of "iMCU" (interleaved MCU) rows.  These are the same as MCU rows
     in fully interleaved JPEG scans, but are used whether the scan is
     interleaved or not.  We define an iMCU row as v_samp_factor DCT block
     rows of each component.  Therefore, the IDCT output contains
     v_samp_factor*DCT_scaled_size sample rows of a component per iMCU row.
  */

  CJPEG_WRBMP_JSAMPLE
  *sample_range_limit;  /* table for fast range-limiting */

  /*
     These fields are valid during any one scan.
     They describe the components and MCUs actually appearing in the scan.
     Note that the decompressor output side must not use these fields.
  */
  int comps_in_scan;    /* # of JPEG components in this scan */
  cjpeg_wrbmp_jpeg_component_info *cur_comp_info[ 4 ];
  /* *cur_comp_info[ i ] describes component that appears i'th in SOS */

  CJPEG_WRBMP_JDIMENSION MCUs_per_row;  /* # of MCUs across the image */
  CJPEG_WRBMP_JDIMENSION
  MCU_rows_in_scan;  /* # of MCU rows in the image */

  int blocks_in_MCU;    /* # of DCT blocks per MCU */
  int MCU_membership[ 10 ];
  /* MCU_membership[ i ] is index in cur_comp_info of component owning */
  /* i'th block in an MCU */

  int Ss, Se, Ah, Al;   /* progressive JPEG parameters for scan */

  /* This field is shared between entropy decoder and marker parser.
     It is either zero or the code of a JPEG marker that has been
     read from the data source, but has not yet been processed.
  */
  int unread_marker;

  /*
     Links to decompression subobjects (methods, private variables of modules)
  */
  struct jpeg_decomp_master *master;
  struct jpeg_d_main_controller *main;
  struct jpeg_d_coef_controller *coef;
  struct jpeg_d_post_controller *post;
  struct jpeg_input_controller *inputctl;
  struct jpeg_marker_reader *marker;
  struct jpeg_entropy_decoder *entropy;
  struct jpeg_inverse_dct *idct;
  struct jpeg_upsampler *upsample;
  struct jpeg_color_deconverter *cconvert;
  struct jpeg_color_quantizer *cquantize;
};


/* "Object" declarations for JPEG modules that may be supplied or called
   directly by the surrounding application.
   As with all objects in the JPEG library, these structs only define the
   publicly visible methods and state variables of a module.  Additional
   private fields may exist after the public ones.
*/


/* Error handler object */

struct cjpeg_wrbmp_jpeg_error_mgr {
  /* Error exit handler: does not return to caller */
  CJPEG_WRBMP_JMETHOD( void, error_exit,
                       ( cjpeg_wrbmp_j_common_ptr cinfo ) );
  /* Conditionally emit a trace or warning message */
  CJPEG_WRBMP_JMETHOD( void, emit_message,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, int msg_level ) );
  /* Routine that actually outputs a trace or error message */
  CJPEG_WRBMP_JMETHOD( void, output_message,
                       ( cjpeg_wrbmp_j_common_ptr cinfo ) );
  /* Format a message string for the most recent JPEG error or message */
  CJPEG_WRBMP_JMETHOD( void, format_message,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, char *buffer ) );
#define JMSG_LENGTH_MAX  200  /* recommended size of format_message buffer */
  /* Reset error state variables at start of a new image */
  CJPEG_WRBMP_JMETHOD( void, reset_error_mgr,
                       ( cjpeg_wrbmp_j_common_ptr cinfo ) );

  /* The message ID code and any parameters are saved here.
     A message can have one string parameter or up to 8 int parameters.
  */
  int msg_code;
#define JMSG_STR_PARM_MAX  80
  /*
    union {
      int i[ 8 ];
      char s[ JMSG_STR_PARM_MAX ];
    } msg_parm;
  */
  /* Standard state variables for error facility */

  int trace_level;    /* max msg_level that will be displayed */

  /* For recoverable corrupt-data errors, we emit a warning message,
     but keep going unless emit_message chooses to abort.  emit_message
     should count warnings in num_warnings.  The surrounding application
     can check for bad data by seeing if num_warnings is nonzero at the
     end of processing.
  */
  long num_warnings;    /* number of corrupt-data warnings */

  /* These fields point to the table(s) of error message strings.
     An application can change the table pointer to switch to a different
     message list (typically, to change the language in which errors are
     reported).  Some applications may wish to add additional error codes
     that will be handled by the JPEG library error mechanism; the second
     table pointer is used for this purpose.

     First table includes all errors generated by JPEG library itself.
     Error code 0 is reserved for a "no such error string" message.
  */
  const char *const *jpeg_message_table;   /* Library errors */
  int last_jpeg_message;    /* Table contains strings 0..last_jpeg_message */
  /* Second table can be added by application (see cjpeg/djpeg for example).
     It contains strings numbered first_addon_message..last_addon_message.
  */
  const char *const *addon_message_table;   /* Non-library errors */
  int first_addon_message;  /* code for first string in addon table */
  int last_addon_message; /* code for last string in addon table */
};


/* Progress monitor object */

struct cjpeg_wrbmp_jpeg_progress_mgr {
  CJPEG_WRBMP_JMETHOD( void, progress_monitor,
                       ( cjpeg_wrbmp_j_common_ptr cinfo ) );

  long pass_counter;    /* work units completed in this pass */
  long pass_limit;    /* total number of work units in this pass */
  int completed_passes;   /* passes completed so far */
  int total_passes;   /* total number of passes expected */
};


/* Data destination object for compression */

struct cjpeg_wrbmp_jpeg_destination_mgr {
  CJPEG_WRBMP_JOCTET
  *next_output_byte;   /* => next byte to write in buffer */
  cjpeg_wrbmp_size_t
  free_in_buffer;  /* # of byte spaces remaining in buffer */

  CJPEG_WRBMP_JMETHOD( void, init_destination,
                       ( cjpeg_wrbmp_j_compress_ptr cinfo ) );
  CJPEG_WRBMP_JMETHOD( cjpeg_wrbmp_boolean, empty_output_buffer,
                       ( cjpeg_wrbmp_j_compress_ptr cinfo ) );
  CJPEG_WRBMP_JMETHOD( void, term_destination,
                       ( cjpeg_wrbmp_j_compress_ptr cinfo ) );
};


/* Data source object for decompression */

struct cjpeg_wrbmp_jpeg_source_mgr {
  const CJPEG_WRBMP_JOCTET
  *next_input_byte;  /* => next byte to read from buffer */
  cjpeg_wrbmp_size_t bytes_in_buffer; /* # of bytes remaining in buffer */

  CJPEG_WRBMP_JMETHOD( void, init_source,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo ) );
  CJPEG_WRBMP_JMETHOD( cjpeg_wrbmp_boolean, fill_input_buffer,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo ) );
  CJPEG_WRBMP_JMETHOD( void, skip_input_data,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo, long num_bytes ) );
  CJPEG_WRBMP_JMETHOD( cjpeg_wrbmp_boolean, resync_to_restart,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo, int desired ) );
  CJPEG_WRBMP_JMETHOD( void, term_source,
                       ( cjpeg_wrbmp_j_decompress_ptr cinfo ) );
};


/* Memory manager object.
   Allocates "small" objects (a few K total), "large" objects (tens of K),
   and "really big" objects (virtual arrays with backing store if needed).
   The memory manager does not allow individual objects to be freed; rather,
   each created object is assigned to a pool, and whole pools can be freed
   at once.  This is faster and more convenient than remembering exactly what
   to free, especially where malloc()/free() are not too speedy.
   NB: alloc routines never return NULL.  They exit to error_exit if not
   successful.
*/

typedef struct jvirt_sarray_control *cjpeg_wrbmp_jvirt_sarray_ptr;
typedef struct jvirt_barray_control *cjpeg_wrbmp_jvirt_barray_ptr;


struct cjpeg_wrbmp_jpeg_memory_mgr {
  /* Method pointers */
  CJPEG_WRBMP_JMETHOD( void *, alloc_small,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, int pool_id,
                         cjpeg_wrbmp_size_t sizeofobject ) );
  CJPEG_WRBMP_JMETHOD( void CJPEG_WRBMP_FAR *, alloc_large,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, int pool_id,
                         cjpeg_wrbmp_size_t sizeofobject ) );
  CJPEG_WRBMP_JMETHOD( CJPEG_WRBMP_JSAMPARRAY, alloc_sarray,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, int pool_id,
                         CJPEG_WRBMP_JDIMENSION samplesperrow,
                         CJPEG_WRBMP_JDIMENSION numrows ) );
  CJPEG_WRBMP_JMETHOD( CJPEG_WRBMP_JBLOCKARRAY, alloc_barray,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, int pool_id,
                         CJPEG_WRBMP_JDIMENSION blocksperrow,
                         CJPEG_WRBMP_JDIMENSION numrows ) );
  CJPEG_WRBMP_JMETHOD( cjpeg_wrbmp_jvirt_sarray_ptr,
                       request_virt_sarray, ( cjpeg_wrbmp_j_common_ptr cinfo,
                           int pool_id,
                           cjpeg_wrbmp_boolean pre_zero,
                           CJPEG_WRBMP_JDIMENSION samplesperrow,
                           CJPEG_WRBMP_JDIMENSION numrows,
                           CJPEG_WRBMP_JDIMENSION maxaccess ) );
  CJPEG_WRBMP_JMETHOD( cjpeg_wrbmp_jvirt_barray_ptr,
                       request_virt_barray, ( cjpeg_wrbmp_j_common_ptr cinfo,
                           int pool_id,
                           cjpeg_wrbmp_boolean pre_zero,
                           CJPEG_WRBMP_JDIMENSION blocksperrow,
                           CJPEG_WRBMP_JDIMENSION numrows,
                           CJPEG_WRBMP_JDIMENSION maxaccess ) );
  CJPEG_WRBMP_JMETHOD( void, realize_virt_arrays,
                       ( cjpeg_wrbmp_j_common_ptr cinfo ) );
  CJPEG_WRBMP_JMETHOD( CJPEG_WRBMP_JSAMPARRAY, access_virt_sarray,
                       ( cjpeg_wrbmp_j_common_ptr cinfo,
                         cjpeg_wrbmp_jvirt_sarray_ptr ptr,
                         CJPEG_WRBMP_JDIMENSION start_row,
                         CJPEG_WRBMP_JDIMENSION num_rows,
                         cjpeg_wrbmp_boolean writable ) );
  CJPEG_WRBMP_JMETHOD( CJPEG_WRBMP_JBLOCKARRAY, access_virt_barray,
                       ( cjpeg_wrbmp_j_common_ptr cinfo,
                         cjpeg_wrbmp_jvirt_barray_ptr ptr,
                         CJPEG_WRBMP_JDIMENSION start_row,
                         CJPEG_WRBMP_JDIMENSION num_rows,
                         cjpeg_wrbmp_boolean writable ) );
  CJPEG_WRBMP_JMETHOD( void, free_pool,
                       ( cjpeg_wrbmp_j_common_ptr cinfo, int pool_id ) );
  CJPEG_WRBMP_JMETHOD( void, self_destruct,
                       ( cjpeg_wrbmp_j_common_ptr cinfo ) );

  /* Limit on memory allocation for this JPEG object.  (Note that this is
     merely advisory, not a guaranteed maximum; it only affects the space
     used for virtual-array buffers.)  May be changed by outer application
     after creating the JPEG object.
  */
  long max_memory_to_use;

  /* Maximum allocation request accepted by alloc_large. */
  long max_alloc_chunk;
};


/* Routine signature for application-supplied marker processing methods.
   Need not pass marker code since it is stored in cinfo->unread_marker.
*/
typedef CJPEG_WRBMP_JMETHOD( cjpeg_wrbmp_boolean,
                             jpeg_marker_parser_method,
                             ( cjpeg_wrbmp_j_decompress_ptr cinfo ) );

/*
   The JPEG library modules define JPEG_INTERNALS before including this file.
   The internal structure declarations are read only when that is true.
   Applications using the library should not include jpegint.h, but may wish
   to include jerror.h.
*/

#ifdef CJPEG_JPEG6B_WRBMP_JPEG_INTERNALS
#include "jpegint.h"    /* fetch private declarations */
#include "jerror.h"   /* fetch error codes too */
#endif

#endif /* JPEGLIB_H */
