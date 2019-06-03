/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: jpeglib

  Author: Thomas G. Lane

  Function: This file defines the application interface for the JPEG library.
    Most applications using the library need only include this file, and perhaps
    jerror.h if they want to know the exact error codes.

  Source: MediaBench II
          http://euler.slu.edu/~fritts/mediabench (mirror)

  Original name: cjpeg

  Changes: No major functional changes.

  License: See the accompanying README file.

*/


#ifndef JPEGLIB_H
#define JPEGLIB_H

/*
  Various constants determining the sizes of things.
  All of these are specified by the JPEG standard, so don't change them if you
  want to be compatible.
*/

/* The basic DCT block is 8x8 samples */
#define DCTSIZE       8

/* DCTSIZE squared; # of elements in a block */
#define DCTSIZE2      64


/*
  Data structures for images (arrays of samples and of DCT coefficients).
  On 80x86 machines, the image arrays are too big for near pointers, but the
  pointer arrays can fit in near memory.
*/

/* ptr to one image row of pixel samples. */
typedef unsigned char *JSAMPROW;

/* ptr to some rows (a 2-D sample array) */
typedef JSAMPROW *JSAMPARRAY;

/* one block of coefficients */
typedef signed char JBLOCK[DCTSIZE2];

/* pointer to one row of coefficient blocks */
typedef JBLOCK *JBLOCKROW;

/* a 2-D array of coefficient blocks */
typedef JBLOCKROW *JBLOCKARRAY;

/* useful in a couple of places */
typedef signed char *JCOEFPTR;


/*
  DCT coefficient quantization tables.
*/

typedef struct {
  /* quantization step for each coefficient */
  /*
    This array gives the coefficient quantizers in natural array order (not the
    zigzag order in which they are stored in a JPEG DQT marker).
    CAUTION: IJG versions prior to v6a kept this array in zigzag order.
  */
  unsigned short quantval[ DCTSIZE2 ];

  /* 1 when table has been output */
  /*
    This field is used only during compression. It's initialized 0 when the
    table is created, and set 1 when it's been output to the file. You could
    suppress output of a table by setting this to 1. (See jpeg_suppress_tables
    for an example.)
  */
  int sent_table;
} JQUANT_TBL;


/*
  Huffman coding tables.
*/

typedef struct {
  /* These two fields directly represent the contents of a JPEG DHT marker */

  /* bits[k] = # of symbols with codes of */
  /* length k bits; bits[0] is unused */
  unsigned char bits[ 17 ];

  /* The symbols, in order of incr code length */
  /*
    This field is used only during compression. It's initialized 0 when the
    table is created, and set 1 when it's been output to the file. You could
    suppress output of a table by setting this to 1. (See jpeg_suppress_tables
    for an example.)
  */
  unsigned char huffval[ 256 ];

  /* 1 when table has been output */
  int sent_table;
} JHUFF_TBL;


/*
  Basic info about one component (color channel).
*/

typedef struct {
  /*
    These values are fixed over the whole image. For compression, they must be
    supplied by parameter setup; for decompression, they are read from the SOF
    marker.
  */

  /* identifier for this component (0..255) */
  int component_id;

  /* its index in SOF or cinfo->comp_info[] */
  int component_index;

  /* horizontal sampling factor (1..4) */
  int h_samp_factor;

  /* vertical sampling factor (1..4) */
  int v_samp_factor;

  /* quantization table selector (0..3) */
  int quant_tbl_no;

  /*
    These values may vary between scans. For compression, they must be supplied
    by parameter setup; for decompression, they are read from the SOS marker.
    The decompressor output side may not use these variables.
  */

  /* DC entropy table selector (0..3) */
  int dc_tbl_no;

  /* AC entropy table selector (0..3) */
  int ac_tbl_no;

  /* Remaining fields should be treated as private by applications. */

  /*
    These values are computed during compression or decompression startup:
    Component's size in DCT blocks. Any dummy blocks added to complete an MCU
    are not counted; therefore these values do not depend on whether a scan is
    interleaved or not.
  */
  unsigned int width_in_blocks;
  unsigned int height_in_blocks;

  /*
    Size of a DCT block in samples. Always DCTSIZE for compression. For
    decompression this is the size of the output from one DCT block, reflecting
    any scaling we choose to apply during the IDCT step. Values of 1,2,4,8 are
    likely to be supported. Note that different components may receive different
    IDCT scalings.
  */
  int DCT_scaled_size;

  /*
    The downsampled dimensions are the component's actual, unpadded number of
    samples at the main buffer (preprocessing/compression interface), thus
    downsampled_width = ceil(image_width * Hi/Hmax) and similarly for height.
    For decompression, IDCT scaling is included, so
    downsampled_width = ceil(image_width * Hi/Hmax * DCT_scaled_size/DCTSIZE)
  */

  /* actual width in samples */
  unsigned int downsampled_width;

  /* actual height in samples */
  unsigned int downsampled_height;

  /*
    This flag is used only for decompression. In cases where some of the
    components will be ignored (eg grayscale output from YCbCr image), we can
    skip most computations for the unused components.
  */

  /* do we need the value of this component? */
  int component_needed;

  /*
    These values are computed before starting a scan of the component. The
    decompressor output side may not use these variables.
  */

  /* number of blocks per MCU, horizontally */
  int MCU_width;

  /* number of blocks per MCU, vertically */
  int MCU_height;

  /* MCU_width * MCU_height */
  int MCU_blocks;

  /* MCU width in samples, MCU_width*DCT_scaled_size */
  int MCU_sample_width;

  /* # of non-dummy blocks across in last MCU */
  int last_col_width;

  /* # of non-dummy blocks down in last MCU */
  int last_row_height;

  /*
    Saved quantization table for component; (void*)0 if none yet saved. See
    jdinput.c comments about the need for this information. This field is
    currently used only for decompression.
  */
  JQUANT_TBL *quant_table;

  /* Private per-component storage for DCT or IDCT subsystem. */
  void *dct_table;
} jpeg_component_info;


/*
  The script for encoding a multiple-scan file is an array of these:
*/

typedef struct {
  /* number of components encoded in this scan */
  int comps_in_scan;

  /* their SOF/comp_info[] indexes */
  int component_index[ 4 ];

  /* progressive JPEG spectral selection parms */
  int Ss, Se;

  /* progressive JPEG successive approx. parms */
  int Ah, Al;
} jpeg_scan_info;


/*
  Known color spaces.
*/

typedef enum {
  /* error/unspecified */
  JCS_UNKNOWN,

  /* monochrome */
  JCS_GRAYSCALE,

  /* red/green/blue */
  JCS_RGB,

  /* Y/Cb/Cr (also known as YUV) */
  JCS_YCbCr,

  /* C/M/Y/K */
  JCS_CMYK,

  /* Y/Cb/Cr/K */
  JCS_YCCK
} J_COLOR_SPACE;


/*
  DCT/IDCT algorithm options.
*/

typedef enum {
  /* slow but accurate integer algorithm */
  JDCT_ISLOW,

  /* faster, less accurate integer method */
  JDCT_IFAST,

  /* floating-point: accurate, fast on fast HW */
  JDCT_FLOAT
} J_DCT_METHOD;


/*
  Common fields between JPEG compression and decompression master structs.
*/

#define jpeg_common_fields \
  /* Error handler module */\
  struct jpeg_error_mgr *err; \
  /* Memory manager module */\
  struct jpeg_memory_mgr *mem; \
  /* Progress monitor, or (void*)0 if none */\
  struct jpeg_progress_mgr *progress; \
  /* Available for use by application */\
  void *client_data; \
  /* So common code can tell which is which */\
  int is_decompressor; \
  /* For checking call sequence validity */\
  int global_state


/*
  Routines that are to be used by both halves of the library are declared to
  receive a pointer to this structure. There are no actual instances of
  jpeg_common_struct, only of jpeg_compress_struct and jpeg_decompress_struct.
*/

struct jpeg_common_struct {
  /* Fields common to both master struct types */
  jpeg_common_fields;

  /*
    Additional fields follow in an actual jpeg_compress_struct or
    jpeg_decompress_struct. All three structs must agree on these initial
    fields!  (This would be a lot cleaner in C++.)
  */
};

typedef struct jpeg_common_struct *j_common_ptr;
typedef struct jpeg_compress_struct *j_compress_ptr;
typedef struct jpeg_decompress_struct *j_decompress_ptr;


/*
  Master record for a compression instance
*/

struct jpeg_compress_struct {
  /* Fields shared with jpeg_decompress_struct */
  jpeg_common_fields;

  /* Destination for compressed data */
  struct jpeg_destination_mgr *dest;

  /*
    Description of source image --- these fields must be filled in by outer
    application before starting compression. in_color_space must be correct
    before you can even call jpeg_set_defaults().
  */

  /* input image width */
  unsigned int image_width;

  /* input image height */
  unsigned int image_height;

  /* # of color components in input image */
  int input_components;

  /* colorspace of input image */
  J_COLOR_SPACE in_color_space;

  /* image gamma of input image */
  double input_gamma;

  /*
    Compression parameters --- these fields must be set before calling
    jpeg_start_compress(). We recommend calling jpeg_set_defaults() to
    initialize everything to reasonable defaults, then changing anything the
    application specifically wants to change. That way you won't get burnt when
    new parameters are added. Also note that there are several helper routines
    to simplify changing parameters.
  */

  /* bits of precision in image data */
  int data_precision;

  /* # of color components in JPEG image */
  int num_components;

  /* colorspace of JPEG image */
  J_COLOR_SPACE jpeg_color_space;

  /* comp_info[i] describes component that appears i'th in SOF */
  jpeg_component_info *comp_info;

  /* ptrs to coefficient quantization tables, or (void*)0 if not defined */
  JQUANT_TBL *quant_tbl_ptrs[ 4 ];

  /* ptrs to Huffman coding tables, or (void*)0 if not defined */
  JHUFF_TBL *dc_huff_tbl_ptrs[ 4 ];
  JHUFF_TBL *ac_huff_tbl_ptrs[ 4 ];

  /* L values for DC arith-coding tables */
  unsigned char arith_dc_L[ 16 ];

  /* U values for DC arith-coding tables */
  unsigned char arith_dc_U[ 16 ];

  /* Kx values for AC arith-coding tables */
  unsigned char arith_ac_K[ 16 ];

  /* # of entries in scan_info array */
  int num_scans;

  /*
    script for multi-scan file, or (void*)0
    The default value of scan_info is (void*)0, which causes a single-scan
    sequential JPEG file to be emitted. To create a multi-scan file, set
    num_scans and scan_info to point to an array of scan definitions.
  */
  const jpeg_scan_info *scan_info;

  /* 1=caller supplies downsampled data */
  int raw_data_in;

  /* 1=arithmetic coding, 0=Huffman */
  int arith_code;

  /* 1=optimize entropy encoding parms */
  int optimize_coding;

  /* 1=first samples are cosited */
  int CCIR601_sampling;

  /* 1..100, or 0 for no input smoothing */
  int smoothing_factor;

  /* DCT algorithm selector */
  J_DCT_METHOD dct_method;


  /*
    The restart interval can be specified in absolute MCUs by setting
    restart_interval, or in MCU rows by setting restart_in_rows (in which case
    the correct restart_interval will be figured for each scan).
  */

  /* MCUs per restart, or 0 for no restart */
  unsigned int restart_interval;

  /* if > 0, MCU rows per restart interval */
  int restart_in_rows;


  /*
    Parameters controlling emission of special markers.
  */

  /* should a JFIF marker be written? */
  int write_JFIF_header;

  /* What to write for the JFIF version number */
  unsigned char JFIF_major_version;
  unsigned char JFIF_minor_version;

  /*
    These three values are not used by the JPEG code, merely copied into the
    JFIF APP0 marker. density_unit can be 0 for unknown,  1 for dots/inch, or 2
    for dots/cm. Note that the pixel aspect ratio is defined by
    X_density/Y_density even when density_unit=0.
  */

  /* JFIF code for pixel size units */
  unsigned char density_unit;

  /* Horizontal pixel density */
  unsigned short X_density;

  /* Vertical pixel density */
  unsigned short Y_density;

  /* should an Adobe marker be written? */
  int write_Adobe_marker;

  /*
    State variable: index of next scanline to be written to
    jpeg_write_scanlines(). Application may use this to control its processing
    loop, e.g., "while (next_scanline < image_height)".
  */

  /* 0 .. image_height-1  */
  unsigned int next_scanline;


  /*
    Remaining fields are known throughout compressor, but generally should not
    be touched by a surrounding application.
  */

  /*
    These fields are computed during compression startup
  */

  /* 1 if scan script uses progressive mode */
  int progressive_mode;

  /* largest h_samp_factor */
  int max_h_samp_factor;

  /* largest v_samp_factor */
  int max_v_samp_factor;

  /*
    # of iMCU rows to be input to coef ctlr
    The coefficient controller receives data in units of MCU rows as defined for
    fully interleaved scans (whether the JPEG file is interleaved or not). There
    are v_samp_factor * DCTSIZE sample rows of each component in an "iMCU"
    (interleaved MCU) row.
  */
  unsigned int total_iMCU_rows;

  /*
     These fields are valid during any one scan. They describe the components
     and MCUs actually appearing in the scan.
  */

  /* # of JPEG components in this scan */
  int comps_in_scan;

  /* *cur_comp_info[i] describes component that appears i'th in SOS */
  jpeg_component_info *cur_comp_info[ 4 ];

  /* # of MCUs across the image */
  unsigned int MCUs_per_row;

  /* # of MCU rows in the image */
  unsigned int MCU_rows_in_scan;

  /* # of DCT blocks per MCU */
  int blocks_in_MCU;

  /*
    MCU_membership[i] is index in cur_comp_info of component owning i'th block
    in an MCU
  */
  int MCU_membership[ 10 ];

  /* progressive JPEG parameters for scan */
  int Ss, Se, Ah, Al;

  /*
     Links to compression subobjects (methods and private variables of modules)
  */

  struct jpeg_comp_master *master;
  struct jpeg_c_main_controller *main;
  struct jpeg_c_prep_controller *prep;
  struct jpeg_c_coef_controller *coef;
  struct jpeg_marker_writer *marker;
  struct jpeg_color_converter *cconvert;
  struct jpeg_downsampler *downsample;
  struct jpeg_forward_dct *fdct;
  struct jpeg_entropy_encoder *entropy;

  /* workspace for jpeg_simple_progression */
  jpeg_scan_info *script_space;

  int script_space_size;
};


/*
  "Object" declarations for JPEG modules that may be supplied or called directly
  by the surrounding application. As with all objects in the JPEG library, these
  structs only define the publicly visible methods and state variables of a
  module. Additional private fields may exist after the public ones.
*/

/*
  Error handler object
*/

struct jpeg_error_mgr {
  /* Error exit handler: does not return to caller */
  void ( *error_exit ) ( j_common_ptr cinfo );

  /* Conditionally emit a trace or warning message */
  void ( *emit_message ) ( j_common_ptr cinfo, int msg_level );

  /* Routine that actually outputs a trace or error message */
  void ( *output_message ) ( j_common_ptr cinfo );

  /* Format a message string for the most recent JPEG error or message */
  void ( *format_message ) ( j_common_ptr cinfo, char *buffer );

  /* Reset error state variables at start of a new image */
  void ( *reset_error_mgr ) ( j_common_ptr cinfo );

  /*
    The message ID code and any parameters are saved here. A message can have
    one string parameter or up to 8 int parameters.
  */
  int msg_code;

  union {
    int i[ 8 ];
    char s[ 80 ];
  } msg_parm;


  /*
    Standard state variables for error facility
  */

  /* max msg_level that will be displayed */
  int trace_level;

  /*
    For recoverable corrupt-data errors, we emit a warning message, but keep
    going unless emit_message chooses to abort. emit_message should count
    warnings in num_warnings. The surrounding application can check for bad data
    by seeing if num_warnings is nonzero at the end of processing.
  */

  /* number of corrupt-data warnings */
  long num_warnings;

  /*
    These fields point to the table(s) of error message strings. An application
    can change the table pointer to switch to a different message list
    (typically, to change the language in which errors are reported). Some
    applications may wish to add additional error codes that will be handled by
    the JPEG library error mechanism; the second table pointer is used for this
    purpose.

     First table includes all errors generated by JPEG library itself. Error
     code 0 is reserved for a "no such error string" message.
  */

  /* Library errors */
  const char *const *jpeg_message_table;

  /* Table contains strings 0..last_jpeg_message */
  int last_jpeg_message;

  /*
    Second table can be added by application (see cjpeg/djpeg for example). It
    contains strings numbered first_addon_message..last_addon_message.
  */

  /* Non-library errors */
  const char *const *addon_message_table;

  /* code for first string in addon table */
  int first_addon_message;

  /* code for last string in addon table */
  int last_addon_message;
};


/*
  Progress monitor object
*/

struct jpeg_progress_mgr {
  void ( *progress_monitor ) ( j_common_ptr cinfo );

  /* work units completed in this pass */
  long pass_counter;

  /* total number of work units in this pass */
  long pass_limit;

  /* passes completed so far */
  int completed_passes;

  /* total number of passes expected */
  int total_passes;
};


/*
  Data destination object for compression
*/

struct jpeg_destination_mgr {
  /* => next byte to write in buffer */
  unsigned char *next_output_byte;

  /* # of byte spaces remaining in buffer */
  long unsigned int free_in_buffer;

  void ( *init_destination ) ( j_compress_ptr cinfo );
  int ( *empty_output_buffer ) ( j_compress_ptr cinfo );
  void ( *term_destination ) ( j_compress_ptr cinfo );
};


/*
  Memory manager object.
  Allocates "small" objects (a few K total), "large" objects (tens of K), and
  "really big" objects (virtual arrays with backing store if needed). The memory
  manager does not allow individual objects to be freed; rather, each created
  object is assigned to a pool, and whole pools can be freed at once. This is
  faster and more convenient than remembering exactly what to free, especially
  where malloc()/free() are not too speedy.
  NB: alloc routines never return (void*)0. They exit to error_exit if not
  successful.
*/

typedef struct jvirt_sarray_control *jvirt_sarray_ptr;
typedef struct jvirt_barray_control *jvirt_barray_ptr;

struct jpeg_memory_mgr {
  /*
    Method pointers
  */
  void *( *alloc_small ) (
    j_common_ptr cinfo, int pool_id, long unsigned int sizeofobject );

  void *( *alloc_large ) (
    j_common_ptr cinfo, int pool_id, long unsigned int sizeofobject );

  JSAMPARRAY ( *alloc_sarray ) (
    j_common_ptr cinfo, int pool_id, unsigned int samplesperrow,
    unsigned int numrows );

  JBLOCKARRAY ( *alloc_barray )(
    j_common_ptr cinfo, int pool_id, unsigned int blocksperrow,
    unsigned int numrows );

  jvirt_sarray_ptr ( *request_virt_sarray ) (
    j_common_ptr cinfo, int pool_id, int pre_zero, unsigned int samplesperrow,
    unsigned int numrows, unsigned int maxaccess );

  jvirt_barray_ptr ( *request_virt_barray ) (
    j_common_ptr cinfo, int pool_id, int pre_zero, unsigned int blocksperrow,
    unsigned int numrows, unsigned int maxaccess );

  void ( *realize_virt_arrays ) ( j_common_ptr cinfo );

  JSAMPARRAY ( *access_virt_sarray ) (
    j_common_ptr cinfo, jvirt_sarray_ptr ptr, unsigned int start_row,
    unsigned int num_rows, int writable );

  JBLOCKARRAY ( *access_virt_barray ) (
    j_common_ptr cinfo, jvirt_barray_ptr ptr, unsigned int start_row,
    unsigned int num_rows, int writable );

  void ( *free_pool ) ( j_common_ptr cinfo, int pool_id );

  void ( *self_destruct ) ( j_common_ptr cinfo );

  /*
    Limit on memory allocation for this JPEG object. (Note that this is merely
    advisory, not a guaranteed maximum; it only affects the space used for
    virtual-array buffers.) May be changed by outer application after creating
    the JPEG object.
  */
  long max_memory_to_use;

  /* Maximum allocation request accepted by alloc_large. */
  long max_alloc_chunk;
};

#endif /* JPEGLIB_H */
