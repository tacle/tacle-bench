/* +++Date last modified: 05-Jul-1997 */

/*
**  SNIPTYPE.H - Include file for SNIPPETS data types and commonly used macros
*/

#ifndef SNIPTYPE__H
#define SNIPTYPE__H

/*#if !defined(WIN32) && !defined(_WIN32) && !defined(__NT__) \
      && !defined(_WINDOWS)
      #if !defined(OS2)*/
  typedef unsigned char  BYTE;
  typedef unsigned long  DWORD;
/* #endif*/
 typedef unsigned short WORD;
/*#else
 #define WIN32_LEAN_AND_MEAN
 #define NOGDI
 #define NOSERVICE
 #undef INC_OLE1
 #undef INC_OLE2
 #include <windows.h>
 #define HUGE
 #endif*/

#define NUL '\0'
#define TOBOOL(x) (!(!(x)))

#endif /* SNIPTYPE__H */
