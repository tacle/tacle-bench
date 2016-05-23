/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: cderror.h

  Author: Thomas G. Lane.

  This file is part of the Independent JPEG Group's software.
  For conditions of distribution and use, see the accompanying README file.

  Source: Independent JPEG Group's software

  Changes: no major functional changes

  License:  See the accompanying README files
*/

#ifndef JCONFIG_H
#define JCONFIG_H

#define CJPEG_WRBMP_HAVE_PROTOTYPES
#define CJPEG_WRBMP_HAVE_UNSIGNED_CHAR
#define CJPEG_WRBMP_HAVE_UNSIGNED_SHORT
#undef void
#undef const
#undef CHAR_IS_UNSIGNED
#define CJPEG_WRBMP_HAVE_STDDEF_H
#define CJPEG_WRBMP_HAVE_STDLIB_H
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
#undef CJPEG_JPEG6B_WRBMP_NEED_FAR_POINTERS
#undef NEED_SHORT_EXTERNAL_NAMES
/* Define this if you get warnings about undefined structures. */
#undef INCOMPLETE_TYPES_BROKEN

#ifdef CJPEG_JPEG6B_WRBMP_JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED
#define INLINE __inline__
/* These are for configuring the JPEG memory manager. */
#undef DEFAULT_MAX_MEM
#undef NO_MKTEMP

#endif /* JPEG_INTERNALS */

#ifdef CJPEG_WRBMP_JPEG_CJPEG_DJPEG

#define CJPEG_WRBMP_BMP_SUPPORTED   /* BMP image file format */
#define CJPEG_WRBMP_GIF_SUPPORTED   /* GIF image file format */
#define CJPEG_WRBMP_PPM_SUPPORTED   /* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED    /* Utah RLE image file format */
#define CJPEG_WRBMP_TARGA_SUPPORTED   /* Targa image file format */

#undef TWO_FILE_COMMANDLINE
#undef NEED_SIGNAL_CATCHER
#undef DONT_USE_B_MODE

/* Define this if you want percent-done progress reports from cjpeg/djpeg. */
#undef PROGRESS_REPORT

#endif /* JPEG_CJPEG_DJPEG */

#endif
