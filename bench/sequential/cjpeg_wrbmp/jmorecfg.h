/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: jmorecfg.h

  Author: Thomas G. Lane.

  This file is part of the Independent JPEG Group's software.
  For conditions of distribution and use, see the accompanying README file.

  This file contains additional configuration options that customize the
  JPEG software for special applications or support machine-dependent
  optimizations.  Most users will not need to touch this file.

  Source: Independent JPEG Group's software

  Changes: no major functional changes

  License:  See the accompanying README file
*/

#ifndef JMORECFG_H
#define JMORECFG_H


#define CJPEG_WRBMP_GETJSAMPLE(value)  ((int) (value))

typedef unsigned char CJPEG_WRBMP_JSAMPLE;
typedef short CJPEG_WRBMP_JCOEF;
typedef unsigned char CJPEG_WRBMP_JOCTET;

/* These typedefs are used for various table entries and so forth.
   They must be at least as wide as specified; but making them too big
   won't cost a huge amount of memory, so we don't provide special
   extraction code like we did for JSAMPLE.  (In other words, these
   typedefs live at a different point on the speed/space tradeoff curve.)
*/

/* UINT8 must hold at least the values 0..255. */

typedef unsigned char CJPEG_WRBMP_UINT8;


/* UINT16 must hold at least the values 0..65535. */

typedef unsigned short CJPEG_WRBMP_UINT16;


/* INT16 must hold at least the values -32768..32767. */

#ifndef XMD_H     /* X11/xmd.h correctly defines INT16 */
typedef short INT16;
#endif

/* INT32 must hold at least signed 32-bit values. */

#ifndef XMD_H     /* X11/xmd.h correctly defines INT32 */
typedef long INT32;
#endif

typedef unsigned int CJPEG_WRBMP_JDIMENSION;

/* This macro is used to declare a "method", that is, a function pointer.
   We want to supply prototype parameters if the compiler can cope.
   Note that the arglist parameter must be parenthesized!
   Again, you can customize this if you need special linkage keywords.
*/

#define EXTERN(type)    extern type

#ifdef CJPEG_WRBMP_HAVE_PROTOTYPES
#define CJPEG_WRBMP_JMETHOD(type,methodname,arglist)  type (*methodname) arglist
#else
#define CJPEG_WRBMP_JMETHOD(type,methodname,arglist)  type (*methodname) ()
#endif


/* Here is the pseudo-keyword for declaring pointers that must be "far"
   on 80x86 machines.  Most of the specialized coding for 80x86 is handled
   by just saying "FAR *" where such a pointer is needed.  In a few places
   explicit coding is needed; see uses of the NEED_FAR_POINTERS symbol.
*/

#ifdef CJPEG_JPEG6B_WRBMP_NEED_FAR_POINTERS
#define CJPEG_WRBMP_FAR  far
#else
#define CJPEG_WRBMP_FAR
#endif

#ifndef CJPEG_JPEG6B_WRBMP_HAVE_BOOLEAN
typedef int cjpeg_wrbmp_boolean;
#endif
#endif
