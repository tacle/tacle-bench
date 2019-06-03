
/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: cderror.h

  Author: Thomas G. Lane.

  This file is part of the Independent JPEG Group's software.
  For conditions of distribution and use, see the accompanying README file.

  This file defines the error and message codes for the cjpeg/djpeg
  applications.  These strings are not needed as part of the JPEG library
  proper.
  Edit this file to add new codes, or to translate the message strings to
  some other language.

  Source: Independent JPEG Group's software

  Changes: no major functional changes

  License:  See the accompanying README file

*/

#ifndef CJPEG_WRBMP_JMESSAGE
#ifndef CDERROR_H
#define CDERROR_H
/*First time through, define the enum list*/
#define CJPEG_WRBMP_JMAKE_ENUM_LIST
#else
/* Repeated inclusions of this file are no-ops unless JMESSAGE is defined*/
#define CJPEG_WRBMP_JMESSAGE(code,string)
#endif
#endif

#ifdef CJPEG_WRBMP_JMAKE_ENUM_LIST

typedef enum {

#define CJPEG_WRBMP_JMESSAGE(code,string)  code ,

#endif

  CJPEG_WRBMP_JMESSAGE( JMSG_FIRSTADDONCODE = 1000, NULL )  //Must be first entry!

  #ifdef CJPEG_WRBMP_BMP_SUPPORTED
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_BADCMAP, "Unsupported BMP colormap format" )
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_BADDEPTH, "Only 8- and 24-bit BMP files are supported" )
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_BADHEADER, "Invalid BMP file: bad header length" )
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_BADPLANES, "Invalid BMP file: biPlanes not equal to 1" )
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_COLORSPACE, "BMP output must be grayscale or RGB" )
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_COMPRESSED, "Sorry, compressed BMPs not yet supported" )
  CJPEG_WRBMP_JMESSAGE( JERR_BMP_NOT, "Not a BMP file - does not start with BM" )
  CJPEG_WRBMP_JMESSAGE( JTRC_BMP, "%ux%u 24-bit BMP image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_BMP_MAPPED, "%ux%u 8-bit colormapped BMP image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_BMP_OS2, "%ux%u 24-bit OS2 BMP image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_BMP_OS2_MAPPED, "%ux%u 8-bit colormapped OS2 BMP image" )
  #endif

  #ifdef CJPEG_WRBMP_GIF_SUPPORTED
  CJPEG_WRBMP_JMESSAGE( JERR_GIF_BUG, "GIF output got confused" )
  CJPEG_WRBMP_JMESSAGE( JERR_GIF_CODESIZE, "Bogus GIF codesize %d" )
  CJPEG_WRBMP_JMESSAGE( JERR_GIF_COLORSPACE, "GIF output must be grayscale or RGB" )
  CJPEG_WRBMP_JMESSAGE( JERR_GIF_IMAGENOTFOUND, "Too few images in GIF file" )
  CJPEG_WRBMP_JMESSAGE( JERR_GIF_NOT, "Not a GIF file" )
  CJPEG_WRBMP_JMESSAGE( JTRC_GIF, "%ux%ux%d GIF image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_GIF_BADVERSION,
                        "Warning: unexpected GIF version number '%c%c%c'" )
  CJPEG_WRBMP_JMESSAGE( JTRC_GIF_EXTENSION, "Ignoring GIF extension block of type 0x%02x" )
  CJPEG_WRBMP_JMESSAGE( JTRC_GIF_NONSQUARE, "Caution: nonsquare pixels in input" )
  CJPEG_WRBMP_JMESSAGE( JWRN_GIF_BADDATA, "Corrupt data in GIF file" )
  CJPEG_WRBMP_JMESSAGE( JWRN_GIF_CHAR, "Bogus char 0x%02x in GIF file, ignoring" )
  CJPEG_WRBMP_JMESSAGE( JWRN_GIF_ENDCODE, "Premature end of GIF image" )
  CJPEG_WRBMP_JMESSAGE( JWRN_GIF_NOMOREDATA, "Ran out of GIF bits" )
  #endif

  #ifdef CJPEG_WRBMP_PPM_SUPPORTED
  CJPEG_WRBMP_JMESSAGE( JERR_PPM_COLORSPACE, "PPM output must be grayscale or RGB" )
  CJPEG_WRBMP_JMESSAGE( JERR_PPM_NONNUMERIC, "Nonnumeric data in PPM file" )
  CJPEG_WRBMP_JMESSAGE( JERR_PPM_NOT, "Not a PPM/PGM file" )
  CJPEG_WRBMP_JMESSAGE( JTRC_PGM, "%ux%u PGM image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_PGM_TEXT, "%ux%u text PGM image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_PPM, "%ux%u PPM image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_PPM_TEXT, "%ux%u text PPM image" )
  #endif

  #ifdef RLE_SUPPORTED
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_BADERROR, "Bogus error code from RLE library" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_COLORSPACE, "RLE output must be grayscale or RGB" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_DIMENSIONS, "Image dimensions (%ux%u) too large for RLE" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_EMPTY, "Empty RLE file" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_EOF, "Premature EOF in RLE header" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_MEM, "Insufficient memory for RLE header" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_NOT, "Not an RLE file" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_TOOMANYCHANNELS, "Cannot handle %d output channels for RLE" )
  CJPEG_WRBMP_JMESSAGE( JERR_RLE_UNSUPPORTED, "Cannot handle this RLE setup" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RLE, "%ux%u full-color RLE file" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RLE_FULLMAP, "%ux%u full-color RLE file with map of length %d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RLE_GRAY, "%ux%u grayscale RLE file" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RLE_MAPGRAY, "%ux%u grayscale RLE file with map of length %d" )
  CJPEG_WRBMP_JMESSAGE( JTRC_RLE_MAPPED, "%ux%u colormapped RLE file with map of length %d" )
  #endif

  #ifdef CJPEG_WRBMP_TARGA_SUPPORTED
  CJPEG_WRBMP_JMESSAGE( JERR_TGA_BADCMAP, "Unsupported Targa colormap format" )
  CJPEG_WRBMP_JMESSAGE( JERR_TGA_BADPARMS, "Invalid or unsupported Targa file" )
  CJPEG_WRBMP_JMESSAGE( JERR_TGA_COLORSPACE, "Targa output must be grayscale or RGB" )
  CJPEG_WRBMP_JMESSAGE( JTRC_TGA, "%ux%u RGB Targa image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_TGA_GRAY, "%ux%u grayscale Targa image" )
  CJPEG_WRBMP_JMESSAGE( JTRC_TGA_MAPPED, "%ux%u colormapped Targa image" )
  #else
  CJPEG_WRBMP_JMESSAGE( JERR_TGA_NOTCOMP, "Targa support was not compiled" )
  #endif

  CJPEG_WRBMP_JMESSAGE( JERR_BAD_CMAP_FILE,
                        "Color map file is invalid or of unsupported format" )
  CJPEG_WRBMP_JMESSAGE( JERR_TOO_MANY_COLORS,
                        "Output file format cannot handle %d colormap entries" )
  CJPEG_WRBMP_JMESSAGE( JERR_UNGETC_FAILED, "ungetc failed" )
  #ifdef CJPEG_WRBMP_TARGA_SUPPORTED
  CJPEG_WRBMP_JMESSAGE( JERR_UNKNOWN_FORMAT,
                        "Unrecognized input file format --- perhaps you need -targa" )
  #else
  CJPEG_WRBMP_JMESSAGE( JERR_UNKNOWN_FORMAT, "Unrecognized input file format" )
  #endif
  CJPEG_WRBMP_JMESSAGE( JERR_UNSUPPORTED_FORMAT, "Unsupported output file format" )

  #ifdef CJPEG_WRBMP_JMAKE_ENUM_LIST

  JMSG_LASTADDONCODE
} CJPEG_WRBMP_ADDON_MESSAGE_CODE;

#undef CJPEG_WRBMP_JMAKE_ENUM_LIST
  #endif

/* Zap JMESSAGE macro so that future re-inclusions do nothing by default*/
#undef CJPEG_WRBMP_JMESSAGE
