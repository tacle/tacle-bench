/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: jerror.h

  Author: Thomas G. Lane.

  This file defines the error and message codes for the JPEG library.
  Edit this file to add new codes, or to translate the message strings to
  some other language.
  A set of error-reporting macros are defined too.  Some applications using
  the JPEG library may wish to include this file to get the error codes
  sand/or the macros.

  Source: Independent JPEG Group's software

  Changes: no major functional changes

  License:  See the accompanying README file
*/

/*
   To define the enum list of message codes, include this file without
   defining macro JMESSAGE.  To create a message string table, include it
   again with a suitable JMESSAGE definition (see jerror.c for an example).
*/
#ifndef CJPEG_WRBMP_JMESSAGE
#ifndef JERROR_H
/* First time through, define the enum list */
#define CJPEG_WRBMP_JMAKE_ENUM_LIST
#else
/* Repeated inclusions of this file are no-ops unless JMESSAGE is defined */
#define CJPEG_WRBMP_JMESSAGE(code,string)
#endif /* JERROR_H */
#endif /* JMESSAGE */

#ifdef CJPEG_WRBMP_JMAKE_ENUM_LIST

typedef enum {

#define CJPEG_WRBMP_JMESSAGE(code,string) code ,

#endif /* JMAKE_ENUM_LIST */

  CJPEG_WRBMP_JMESSAGE( JMSG_NOMESSAGE, "Bogus message code %d" ) /* Must be first entry! */

  /* For maintenance convenience, list is alphabetical by message code name */
  CJPEG_WRBMP_JMESSAGE( JERR_ARITH_NOTIMPL,
                        "Sorry, there are legal restrictions on arithmetic coding" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_ALIGN_TYPE, "ALIGN_TYPE is wrong, please fix" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_ALLOC_CHUNK, "MAX_ALLOC_CHUNK is wrong, please fix" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_BUFFER_MODE, "Bogus buffer control mode" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_COMPONENT_ID, "Invalid component ID %d in SOS" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_DCT_COEF, "DCT coefficient out of range" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_DCTSIZE, "IDCT output block size %d not supported" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_HUFF_TABLE, "Bogus Huffman table definition" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_IN_COLORSPACE, "Bogus input colorspace" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_J_COLORSPACE, "Bogus JPEG colorspace" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_LENGTH, "Bogus marker length" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_LIB_VERSION,
                        "Wrong JPEG library version: library is %d, caller expects %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_MCU_SIZE, "Sampling factors too large for interleaved scan" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_POOL_ID, "Invalid memory pool code %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_PRECISION, "Unsupported JPEG data precision %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_PROGRESSION,
                        "Invalid progressive parameters Ss=%d Se=%d Ah=%d Al=%d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_PROG_SCRIPT,
                        "Invalid progressive parameters at scan script entry %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_SAMPLING, "Bogus sampling factors" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_SCAN_SCRIPT, "Invalid scan script at entry %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_STATE, "Improper call to JPEG library in state %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_STRUCT_SIZE,
                        "JPEG parameter struct mismatch: library thinks size is %u, caller expects %u" )
  CJPEG_WRBMP_JMESSAGE( JERR_BAD_VIRTUAL_ACCESS, "Bogus virtual array access" )
  CJPEG_WRBMP_JMESSAGE( JERR_BUFFER_SIZE, "Buffer passed to JPEG library is too small" )
  CJPEG_WRBMP_JMESSAGE( JERR_CANT_SUSPEND, "Suspension not allowed here" )
  CJPEG_WRBMP_JMESSAGE( JERR_CCIR601_NOTIMPL, "CCIR601 sampling not implemented yet" )
  CJPEG_WRBMP_JMESSAGE( JERR_COMPONENT_COUNT, "Too many color components: %d, max %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_CONVERSION_NOTIMPL, "Unsupported color conversion request" )
  CJPEG_WRBMP_JMESSAGE( JERR_DAC_INDEX, "Bogus DAC index %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_DAC_VALUE, "Bogus DAC value 0x%x" )
  CJPEG_WRBMP_JMESSAGE( JERR_DHT_INDEX, "Bogus DHT index %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_DQT_INDEX, "Bogus DQT index %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_EMPTY_IMAGE, "Empty JPEG image (DNL not supported)" )
  CJPEG_WRBMP_JMESSAGE( JERR_EMS_READ, "Read from EMS failed" )
  CJPEG_WRBMP_JMESSAGE( JERR_EMS_WRITE, "Write to EMS failed" )
  CJPEG_WRBMP_JMESSAGE( JERR_EOI_EXPECTED, "Didn't expect more than one scan" )
  CJPEG_WRBMP_JMESSAGE( JERR_FILE_READ, "Input file read error" )
  CJPEG_WRBMP_JMESSAGE( JERR_FILE_WRITE, "Output file write error --- out of disk space?" )
  CJPEG_WRBMP_JMESSAGE( JERR_FRACT_SAMPLE_NOTIMPL, "Fractional sampling not implemented yet" )
  CJPEG_WRBMP_JMESSAGE( JERR_HUFF_CLEN_OVERFLOW, "Huffman code size table overflow" )
  CJPEG_WRBMP_JMESSAGE( JERR_HUFF_MISSING_CODE, "Missing Huffman code table entry" )
  CJPEG_WRBMP_JMESSAGE( JERR_IMAGE_TOO_BIG, "Maximum supported image dimension is %u pixels" )
  CJPEG_WRBMP_JMESSAGE( JERR_INPUT_EMPTY, "Empty input file" )
  CJPEG_WRBMP_JMESSAGE( JERR_INPUT_EOF, "Premature end of input file" )
  CJPEG_WRBMP_JMESSAGE( JERR_MISMATCHED_QUANT_TABLE,
                        "Cannot transcode due to multiple use of quantization table %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_MISSING_DATA, "Scan script does not transmit all data" )
  CJPEG_WRBMP_JMESSAGE( JERR_MODE_CHANGE, "Invalid color quantization mode change" )
  CJPEG_WRBMP_JMESSAGE( JERR_NOTIMPL, "Not implemented yet" )
  CJPEG_WRBMP_JMESSAGE( JERR_NOT_COMPILED, "Requested feature was omitted at compile time" )
  CJPEG_WRBMP_JMESSAGE( JERR_NO_BACKING_STORE, "Backing store not supported" )
  CJPEG_WRBMP_JMESSAGE( JERR_NO_HUFF_TABLE, "Huffman table 0x%02x was not defined" )
  CJPEG_WRBMP_JMESSAGE( JERR_NO_IMAGE, "JPEG datastream contains no image" )
  CJPEG_WRBMP_JMESSAGE( JERR_NO_QUANT_TABLE, "Quantization table 0x%02x was not defined" )
  CJPEG_WRBMP_JMESSAGE( JERR_NO_SOI, "Not a JPEG file: starts with 0x%02x 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JERR_OUT_OF_MEMORY, "Insufficient memory (case %d)" )
  CJPEG_WRBMP_JMESSAGE( JERR_QUANT_COMPONENTS,
                        "Cannot quantize more than %d color components" )
  CJPEG_WRBMP_JMESSAGE( JERR_QUANT_FEW_COLORS, "Cannot quantize to fewer than %d colors" )
  CJPEG_WRBMP_JMESSAGE( JERR_QUANT_MANY_COLORS, "Cannot quantize to more than %d colors" )
  CJPEG_WRBMP_JMESSAGE( JERR_SOF_DUPLICATE, "Invalid JPEG file structure: two SOF markers" )
  CJPEG_WRBMP_JMESSAGE( JERR_SOF_NO_SOS, "Invalid JPEG file structure: missing SOS marker" )
  CJPEG_WRBMP_JMESSAGE( JERR_SOF_UNSUPPORTED, "Unsupported JPEG process: SOF type 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JERR_SOI_DUPLICATE, "Invalid JPEG file structure: two SOI markers" )
  CJPEG_WRBMP_JMESSAGE( JERR_SOS_NO_SOF, "Invalid JPEG file structure: SOS before SOF" )
  CJPEG_WRBMP_JMESSAGE( JERR_TFILE_CREATE, "Failed to create temporary file %s" )
  CJPEG_WRBMP_JMESSAGE( JERR_TFILE_READ, "Read failed on temporary file" )
  CJPEG_WRBMP_JMESSAGE( JERR_TFILE_SEEK, "Seek failed on temporary file" )
  CJPEG_WRBMP_JMESSAGE( JERR_TFILE_WRITE,
                        "Write failed on temporary file --- out of disk space?" )
  CJPEG_WRBMP_JMESSAGE( JERR_TOO_LITTLE_DATA, "Application transferred too few scanlines" )
  CJPEG_WRBMP_JMESSAGE( JERR_UNKNOWN_MARKER, "Unsupported marker type 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JERR_VIRTUAL_BUG, "Virtual array controller messed up" )
  CJPEG_WRBMP_JMESSAGE( JERR_WIDTH_OVERFLOW, "Image too wide for this implementation" )
  CJPEG_WRBMP_JMESSAGE( JERR_XMS_READ, "Read from XMS failed" )
  CJPEG_WRBMP_JMESSAGE( JERR_XMS_WRITE, "Write to XMS failed" )
  CJPEG_WRBMP_JMESSAGE( JMSG_COPYRIGHT, JCOPYRIGHT )
  CJPEG_WRBMP_JMESSAGE( JMSG_VERSION, JVERSION )
  CJPEG_WRBMP_JMESSAGE( JTRC_16BIT_TABLES,
                        "Caution: quantization tables are too coarse for baseline JPEG" )
  CJPEG_WRBMP_JMESSAGE( JTRC_ADOBE,
                        "Adobe APP14 marker: version %d, flags 0x%04x 0x%04x, transform %d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_APP0, "Unknown APP0 marker (not JFIF), length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_APP14, "Unknown APP14 marker (not Adobe), length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_DAC, "Define Arithmetic Table 0x%02x: 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JTRC_DHT, "Define Huffman Table 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JTRC_DQT, "Define Quantization Table %d  precision %d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_DRI, "Define Restart Interval %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_EMS_CLOSE, "Freed EMS handle %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_EMS_OPEN, "Obtained EMS handle %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_EOI, "End Of Image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_HUFFBITS, "        %3d %3d %3d %3d %3d %3d %3d %3d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_JFIF, "JFIF APP0 marker: version %d.%02d, density %dx%d  %d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_JFIF_BADTHUMBNAILSIZE,
                        "Warning: thumbnail image size does not match data length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_JFIF_EXTENSION,
                        "JFIF extension marker: type 0x%02x, length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_JFIF_THUMBNAIL, "    with %d x %d thumbnail image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_MISC_MARKER, "Miscellaneous marker 0x%02x, length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_PARMLESS_MARKER, "Unexpected marker 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JTRC_QUANTVALS, "        %4u %4u %4u %4u %4u %4u %4u %4u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_QUANT_3_NCOLORS, "Quantizing to %d = %d*%d*%d colors" )
  CJPEG_WRBMP_JMESSAGE( JTRC_QUANT_NCOLORS, "Quantizing to %d colors" )
  CJPEG_WRBMP_JMESSAGE( JTRC_QUANT_SELECTED, "Selected %d colors for quantization" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RECOVERY_ACTION, "At marker 0x%02x, recovery action %d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RST, "RST%d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SMOOTH_NOTIMPL,
                        "Smoothing not supported with nonstandard sampling ratios" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SOF, "Start Of Frame 0x%02x: width=%u, height=%u, components=%d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SOF_COMPONENT, "    Component %d: %dhx%dv q=%d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SOI, "Start of Image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SOS, "Start Of Scan: %d components" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SOS_COMPONENT, "    Component %d: dc=%d ac=%d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_SOS_PARAMS, "  Ss=%d, Se=%d, Ah=%d, Al=%d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_TFILE_CLOSE, "Closed temporary file %s" )
  CJPEG_WRBMP_JMESSAGE( JTRC_TFILE_OPEN, "Opened temporary file %s" )
  CJPEG_WRBMP_JMESSAGE( JTRC_THUMB_JPEG,
                        "JFIF extension marker: JPEG-compressed thumbnail image, length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_THUMB_PALETTE,
                        "JFIF extension marker: palette thumbnail image, length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_THUMB_RGB,
                        "JFIF extension marker: RGB thumbnail image, length %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_UNKNOWN_IDS,
                        "Unrecognized component IDs %d %d %d, assuming YCbCr" )
  CJPEG_WRBMP_JMESSAGE( JTRC_XMS_CLOSE, "Freed XMS handle %u" )
  CJPEG_WRBMP_JMESSAGE( JTRC_XMS_OPEN, "Obtained XMS handle %u" )
  CJPEG_WRBMP_JMESSAGE( JWRN_ADOBE_XFORM, "Unknown Adobe color transform code %d" )
  CJPEG_WRBMP_JMESSAGE( JWRN_BOGUS_PROGRESSION,
                        "Inconsistent progression sequence for component %d coefficient %d" )
  CJPEG_WRBMP_JMESSAGE( JWRN_EXTRANEOUS_DATA,
                        "Corrupt JPEG data: %u extraneous bytes before marker 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JWRN_HIT_MARKER, "Corrupt JPEG data: premature end of data segment" )
  CJPEG_WRBMP_JMESSAGE( JWRN_HUFF_BAD_CODE, "Corrupt JPEG data: bad Huffman code" )
  CJPEG_WRBMP_JMESSAGE( JWRN_JFIF_MAJOR, "Warning: unknown JFIF revision number %d.%02d" )
  CJPEG_WRBMP_JMESSAGE( JWRN_JPEG_EOF, "Premature end of JPEG file" )
  CJPEG_WRBMP_JMESSAGE( JWRN_MUST_RESYNC,
                        "Corrupt JPEG data: found marker 0x%02x instead of RST%d" )
  CJPEG_WRBMP_JMESSAGE( JWRN_NOT_SEQUENTIAL, "Invalid SOS parameters for sequential JPEG" )
  CJPEG_WRBMP_JMESSAGE( JWRN_TOO_MUCH_DATA, "Application transferred too many scanlines" )

  #ifdef CJPEG_WRBMP_JMAKE_ENUM_LIST

  JMSG_LASTMSGCODE
} CJPEG_WRBMP_J_MESSAGE_CODE;

#undef CJPEG_WRBMP_JMAKE_ENUM_LIST
  #endif /* JMAKE_ENUM_LIST */

/* Zap JMESSAGE macro so that future re-inclusions do nothing by default */
#undef CJPEG_WRBMP_JMESSAGE
