/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: powerwindow_tmwtypes.h

  Author: CoSys-Lab, University of Antwerp

  Function: headerfile

  Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_tmwtypes.h

  Changes: a brief summary of major functional changes and formatting)

  License: GNU General Public License

*/

#if defined(_MSC_VER)
# pragma once
#endif
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3))
# pragma once
#endif

#ifndef powerwindow_tmwtypes_h
#define powerwindow_tmwtypes_h

#ifndef powerwindow__TMWTYPES__
#define powerwindow__TMWTYPES__
/*
   File    : tmwtypes.h
   Abstract:
        Data types for use with MATLAB/SIMULINK and the Real-Time Workshop.

        When compiling stand-alone model code, data types can be overridden
        via compiler switches.

        Define NO_FLOATS to eliminate reference to powerwindow_real_T, etc.
*/


#define LOGICAL_IS_A_TYPE
#define SPARSE_GENERALIZATION

#ifdef NO_FLOATS
# define double double_not_allowed
# define float  float_not_allowed
#endif /*NO_FLOATS*/


/*
        The following data types cannot be overridden when building MEX files.
*/
#ifdef MATLAB_MEX_FILE
# undef CHARACTER_T
# undef INTEGER_T
# undef BOOLEAN_T
# undef REAL_T
# undef TIME_T
#endif

/*
   The powerwindow_uchar_T, powerwindow_ushort_T and powerwindow_ulong_T types are needed for compilers which do
   not allow defines to be specified, at the command line, with spaces in them.
*/

typedef unsigned char       powerwindow_uchar_T;
typedef unsigned short      powerwindow_ushort_T;
typedef unsigned long       powerwindow_ulong_T;

#if (defined(_MSC_VER) && _MSC_VER >= 1500)      \
|| defined(__x86_64__) || defined(__LP64__)  \
|| defined(__LCC64__)

typedef unsigned long long  powerwindow_ulonglong_T;
#endif



/*=======================================================================*
   Fixed width word size data types:
     powerwindow_int8_T, powerwindow_int16_T, powerwindow_int32_T     - signed 8, 16, or 32 bit integers
     powerwindow_uint8_T, powerwindow_uint16_T, powerwindow_uint32_T  - unsigned 8, 16, or 32 bit integers
     powerwindow_real32_T, powerwindow_real64_T           - 32 and 64 bit floating point numbers
  =======================================================================*/

/* When used with Real Time Workshop generated code, this
   header file can be used with a variety of compilers.

   The compiler could be for an 8 bit embedded processor that
   only had 8 bits per integer and 16 bits per long.
   In that example, a 32 bit integer size is not even available.
   This header file should be robust to that.

   For the case of an 8 bit processor, the preprocessor
   may be limited to 16 bit math like its target.  That limitation
   would mean that 32 bit comparisons can't be done accurately.
   To increase robustness to this, comparisons are done against
   smaller values first.  An inaccurate 32 bit comparison isn't
   attempted if the 16 bit comparison has already succeeded.

   Limitations on preprocessor math can also be stricter than
   for the target.  There are known cases where a compiler
   targeting processors with 64 bit longs can't do accurate
   preprocessor comparisons on more than 32 bits.
*/

/* Determine the number of bits for int, long, short, and char.
   If one fails to be determined, set the number of bits to -1
*/

#ifndef TMW_BITS_PER_INT
# if   INT_MAX  == 0x7FL
#  define TMW_BITS_PER_INT 8
# elif INT_MAX  == 0x7FFFL
#  define TMW_BITS_PER_INT 16
# elif INT_MAX  == 0x7FFFFFFFL
#  define TMW_BITS_PER_INT 32
# else
#  define TMW_BITS_PER_INT -1
# endif
#endif

#ifndef TMW_BITS_PER_LONG
# if   LONG_MAX  == 0x7FL
#  define TMW_BITS_PER_LONG 8
# elif LONG_MAX  == 0x7FFFL
#  define TMW_BITS_PER_LONG 16
# elif LONG_MAX  == 0x7FFFFFFFL
#  define TMW_BITS_PER_LONG 32
# else
#  define TMW_BITS_PER_LONG -1
# endif
#endif

#ifndef TMW_BITS_PER_SHRT
# if   SHRT_MAX  == 0x7FL
#  define TMW_BITS_PER_SHRT 8
# elif SHRT_MAX  == 0x7FFFL
#  define TMW_BITS_PER_SHRT 16
# elif SHRT_MAX  == 0x7FFFFFFFL
#  define TMW_BITS_PER_SHRT 32
# else
#  define TMW_BITS_PER_SHRT -1
# endif
#endif

#ifndef TMW_BITS_PER_SCHAR
# if   SCHAR_MAX  == 0x7FL
#  define TMW_BITS_PER_SCHAR 8
# elif SCHAR_MAX  == 0x7FFFL
#  define TMW_BITS_PER_SCHAR 16
# elif SCHAR_MAX  == 0x7FFFFFFFL
#  define TMW_BITS_PER_SCHAR 32
# else
#  define TMW_BITS_PER_SCHAR -1
# endif
#endif

#ifndef TMW_CHAR_SIGNED
# if SCHAR_MAX == CHAR_MAX
#  define TMW_CHAR_SIGNED 1
# else
#  define TMW_CHAR_SIGNED 0
# endif
#endif

/* It is common for one or more of the integer types
   to be the same size.  For example, on many embedded
   processors, both shorts and ints are 16 bits.  On
   processors used for workstations, it is quite common
   for both int and long to be 32 bits.
     When there is more than one choice for typdef'ing
   a portable type like powerwindow_int16_T or powerwindow_uint32_T, in
   concept, it should not matter which choice is made.
   However, some style guides and some code checking
   tools do identify and complain about seemingly
   irrelevant differences.  For example, a code
   checking tool may complain about an implicit
   conversion from int to short even though both
   are 16 bits.  To reduce these types of
   complaints, it is best to make int the
   preferred choice when more than one is available.
*/

#ifndef INT8_T
# if   TMW_BITS_PER_INT   == 8
#  define  INT8_T int
# elif TMW_BITS_PER_LONG  == 8
#  define  INT8_T long
# elif TMW_BITS_PER_SCHAR == 8
#  define  INT8_T signed char
# elif TMW_BITS_PER_SHRT  == 8
#  define  INT8_T short
# endif
#endif
#ifdef INT8_T
typedef INT8_T powerwindow_int8_T;
#endif



#ifndef UINT8_T
# if   TMW_BITS_PER_INT   == 8
#  define  UINT8_T unsigned int
# elif TMW_BITS_PER_LONG  == 8
#  define  UINT8_T unsigned long
# elif TMW_BITS_PER_SCHAR == 8
#  define  UINT8_T unsigned char
# elif TMW_BITS_PER_SHRT  == 8
#  define  UINT8_T unsigned short
# endif
#endif
#ifdef UINT8_T
typedef UINT8_T powerwindow_uint8_T;
#endif


#ifndef INT16_T
# if   TMW_BITS_PER_INT   == 16
#  define  INT16_T int
# elif TMW_BITS_PER_LONG  == 16
#  define  INT16_T long
# elif TMW_BITS_PER_SCHAR == 16
#  define  INT16_T signed char
# elif TMW_BITS_PER_SHRT  == 16
#  define  INT16_T short
# endif
#endif
#ifdef INT16_T
typedef INT16_T powerwindow_int16_T;
#endif


#ifndef UINT16_T
# if   TMW_BITS_PER_INT   == 16
#  define  UINT16_T unsigned int
# elif TMW_BITS_PER_LONG  == 16
#  define  UINT16_T unsigned long
# elif TMW_BITS_PER_SCHAR == 16
#  define  UINT16_T unsigned char
# elif TMW_BITS_PER_SHRT  == 16
#  define  UINT16_T unsigned short
# endif
#endif
#ifdef UINT16_T
typedef UINT16_T powerwindow_uint16_T;
#endif


#ifndef INT32_T
# if   TMW_BITS_PER_INT   == 32
#  define  INT32_T int
# elif TMW_BITS_PER_LONG  == 32
#  define  INT32_T long
# elif TMW_BITS_PER_SCHAR == 32
#  define  INT32_T signed char
# elif TMW_BITS_PER_SHRT  == 32
#  define  INT32_T short
# endif
#endif
#ifdef INT32_T
typedef INT32_T powerwindow_int32_T;
#endif


#ifndef UINT32_T
# if   TMW_BITS_PER_INT   == 32
#  define  UINT32_T unsigned int
# elif TMW_BITS_PER_LONG  == 32
#  define  UINT32_T unsigned long
# elif TMW_BITS_PER_SCHAR == 32
#   define UINT32_T unsigned char
# elif TMW_BITS_PER_SHRT  == 32
#  define  UINT32_T unsigned short
# endif
#endif
#ifdef UINT32_T
typedef UINT32_T powerwindow_uint32_T;
#endif

/* The following is used to emulate smaller integer types when only
   larger types are available.  For example, compilers for TI C3x/C4x DSPs
   define char and short to be 32 bits, so 8 and 16 bits are not directly
   available.  This target is commonly used with RTW rapid prototyping.
   Other DSPs define char to be 16 bits, so 8 bits is not directly
   available.
*/
#ifndef INT8_T
# ifdef INT16_T
#  define INT8_T INT16_T
typedef INT8_T powerwindow_int8_T;
# else
#  ifdef INT32_T
#   define INT8_T INT32_T
typedef INT8_T powerwindow_int8_T;
#  endif
# endif
#endif

#ifndef UINT8_T
# ifdef UINT16_T
#   define UINT8_T UINT16_T
typedef UINT8_T powerwindow_uint8_T;
# else
#  ifdef UINT32_T
#   define UINT8_T UINT32_T
typedef UINT8_T powerwindow_uint8_T;
#  endif
# endif
#endif

#ifndef INT16_T
# ifdef INT32_T
#   define INT16_T INT32_T
typedef INT16_T powerwindow_int16_T;
# endif
#endif

#ifndef UINT16_T
# ifdef UINT32_T
#   define UINT16_T UINT32_T
typedef UINT16_T powerwindow_uint16_T;
# endif
#endif


#ifndef NO_FLOATS

#ifndef REAL32_T
# ifndef __MWERKS__
#  if FLT_MANT_DIG >= 23
#   define REAL32_T float
#  endif
# else
#  define REAL32_T float
# endif
#endif
#ifdef REAL32_T
typedef REAL32_T powerwindow_real32_T;
#endif


#ifndef REAL64_T
# ifndef __MWERKS__
#  if DBL_MANT_DIG >= 52
#   define REAL64_T double
#  endif
# else
#  define REAL64_T double
# endif
#endif
#ifdef REAL64_T
typedef REAL64_T powerwindow_real64_T;
#endif

#endif /* NO_FLOATS*/

/*=======================================================================*
   Fixed width word size data types:
     powerwindow_int64_T                      - signed 64 bit integers
     powerwindow_uint64_T                     - unsigned 64 bit integers
  =======================================================================*/



#ifndef INT64_T
# if defined(__APPLE__)
#  define INT64_T long long
#  define FMT64 "ll"
#  if defined(__LP64__) && !defined(INT_TYPE_64_IS_LONG)
#    define INT_TYPE_64_IS_LONG
#  endif
# elif defined(__x86_64__) || defined(__LP64__)
#  define INT64_T long
#  define FMT64 "l"
#  if !defined(INT_TYPE_64_IS_LONG)
#    define INT_TYPE_64_IS_LONG
#  endif
# elif defined(_MSC_VER) || (defined(__BORLANDC__) && __BORLANDC__ >= 0x530) \
|| (defined(__WATCOMC__)  && __WATCOMC__  >= 1100)
#  define INT64_T __int64
#  define FMT64 "I64"
# elif defined(__GNUC__) || defined(TMW_ENABLE_INT64) \
|| defined(__LCC64__)
#  define INT64_T long long
#  define FMT64 "ll"
# endif
#endif



#if defined(INT64_T)
# if defined(__GNUC__) && \
((__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >=9)))
__extension__
# endif
typedef INT64_T powerwindow_int64_T;
#endif



#ifndef UINT64_T
# if defined(__APPLE__)
#  define UINT64_T unsigned long long
#  define FMT64 "ll"
#  if defined(__LP64__) && !defined(INT_TYPE_64_IS_LONG)
#    define INT_TYPE_64_IS_LONG
#  endif
# elif defined(__x86_64__) || defined(__LP64__)
#  define UINT64_T unsigned long
#  define FMT64 "l"
#  if !defined(INT_TYPE_64_IS_LONG)
#    define INT_TYPE_64_IS_LONG
#  endif
# elif defined(_MSC_VER) || (defined(__BORLANDC__) && __BORLANDC__ >= 0x530) \
|| (defined(__WATCOMC__)  && __WATCOMC__  >= 1100)
#  define UINT64_T unsigned __int64
#  define FMT64 "I64"
# elif defined(__GNUC__) || defined(TMW_ENABLE_INT64) \
|| defined(__LCC64__)
#  define UINT64_T unsigned long long
#  define FMT64 "ll"
# endif
#endif

#if defined(_WIN64) || (defined(__APPLE__) && defined(__LP64__)) \
|| defined(__x86_64__) \
|| defined(__LP64__)
#  define INT_TYPE_64_IS_SUPPORTED
#endif

#if defined(UINT64_T)
# if defined(__GNUC__) && \
((__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >=9)))
__extension__
# endif
typedef UINT64_T powerwindow_uint64_T;
#endif

/*===========================================================================*
   Format string modifiers for using powerwindow_size_t variables in printf statements.
  ===========================================================================*/

#ifndef FMT_SIZE_T
#  if defined( __GNUC__ ) || defined(_STDC_C99)
#    define FMT_SIZE_T "z"
#  elif defined (__WATCOMC__)
#    define FMT_SIZE_T "l"
#  elif defined (_WIN32 )
#    define FMT_SIZE_T "I"
#  else
#    define FMT_SIZE_T "l"
#  endif
#endif

#ifndef FMT_PTRDIFF_T
#  if defined(__APPLE__)
#    define FMT_PTRDIFF_T "l"
#  elif defined( __GNUC__ ) || defined(_STDC_C99)
#    define FMT_PTRDIFF_T "t"
#  elif defined (__WATCOMC__)
#    define FMT_PTRDIFF_T "l"
#  elif defined (_WIN32 )
#    define FMT_PTRDIFF_T "I"
#  else
#    define FMT_PTRDIFF_T "l"
#  endif
#endif

/*===========================================================================*
   General or logical data types where the word size is not guaranteed.
    powerwindow_real_T     - possible settings include powerwindow_real32_T or powerwindow_real64_T
    powerwindow_time_T     - possible settings include powerwindow_real64_T or powerwindow_uint32_T
    powerwindow_boolean_T
    powerwindow_char_T
    powerwindow_int_T
    powerwindow_uint_T
    powerwindow_byte_T
  ===========================================================================*/

#ifndef NO_FLOATS

#ifndef REAL_T
# ifdef REAL64_T
#  define REAL_T powerwindow_real64_T
# else
#  ifdef REAL32_T
#   define REAL_T powerwindow_real32_T
#  endif
# endif
#endif
#ifdef REAL_T
typedef REAL_T powerwindow_real_T;
#endif

#ifndef TIME_T
# ifdef REAL_T
#  define TIME_T powerwindow_real_T
# endif
#endif
#ifdef TIME_T
typedef TIME_T powerwindow_time_T;
#endif

#endif /* NO_FLOATS */

#ifndef BOOLEAN_T
# if defined(UINT8_T)
#  define BOOLEAN_T UINT8_T
# else
#  define BOOLEAN_T unsigned int
# endif
#endif
typedef BOOLEAN_T powerwindow_boolean_T;


#ifndef CHARACTER_T
# define CHARACTER_T char
#endif
typedef CHARACTER_T powerwindow_char_T;


#ifndef INTEGER_T
# define INTEGER_T int
#endif
typedef INTEGER_T powerwindow_int_T;


#ifndef UINTEGER_T
# define UINTEGER_T unsigned
#endif
typedef UINTEGER_T powerwindow_uint_T;


#ifndef BYTE_T
# define BYTE_T unsigned char
#endif
typedef BYTE_T powerwindow_byte_T;


/*===========================================================================*
   Define Complex Structures
  ===========================================================================*/
#ifndef NO_FLOATS

#ifndef CREAL32_T
#  ifdef REAL32_T
typedef struct {
  powerwindow_real32_T re, im;
} powerwindow_creal32_T;
#    define CREAL32_T creal32_T
#  endif
#endif

#ifndef CREAL64_T
#  ifdef REAL64_T
typedef struct {
  powerwindow_real64_T re, im;
} powerwindow_creal64_T;
#    define CREAL64_T powerwindow_creal64_T
#  endif
#endif

#ifndef CREAL_T
#  ifdef REAL_T
typedef struct {
  powerwindow_real_T re, im;
} powerwindow_creal_T;
#    define CREAL_T powerwindow_creal_T
#  endif
#endif

#endif /* NO_FLOATS */

#ifndef CINT8_T
#  ifdef INT8_T
typedef struct {
  powerwindow_int8_T re, im;
} powerwindow_cint8_T;
#    define CINT8_T powerwindow_cint8_T
#  endif
#endif

#ifndef CUINT8_T
#  ifdef UINT8_T
typedef struct {
  powerwindow_uint8_T re, im;
} powerwindow_cuint8_T;
#    define CUINT8_T powerwindow_cuint8_T
#  endif
#endif

#ifndef CINT16_T
#  ifdef INT16_T
typedef struct {
  powerwindow_int16_T re, im;
} powerwindow_cint16_T;
#    define CINT16_T powerwindow_cint16_T
#  endif
#endif

#ifndef CUINT16_T
#  ifdef UINT16_T
typedef struct {
  powerwindow_uint16_T re, im;
} powerwindow_cuint16_T;
#    define CUINT16_T powerwindow_cuint16_T
#  endif
#endif

#ifndef CINT32_T
#  ifdef INT32_T
typedef struct {
  powerwindow_int32_T re, im;
} powerwindow_cint32_T;
#    define CINT32_T powerwindow_cint32_T
#  endif
#endif

#ifndef CUINT32_T
#  ifdef UINT32_T
typedef struct {
  powerwindow_uint32_T re, im;
} powerwindow_cuint32_T;
#    define CUINT32_T powerwindow_cuint32_T
#  endif
#endif

#ifndef CINT64_T
#  ifdef INT64_T
typedef struct {
  powerwindow_int64_T re, im;
} powerwindow_cint64_T;
#    define CINT64_T powerwindow_cint64_T
#  endif
#endif

#ifndef CUINT64_T
#  ifdef UINT64_T
typedef struct {
  powerwindow_uint64_T re, im;
} powerwindow_cuint64_T;
#    define CUINT64_T powerwindow_cuint64_T
#  endif
#endif

/*=======================================================================*
   Min and Max:
     powerwindow_int8_T, powerwindow_int16_T, powerwindow_int32_T     - signed 8, 16, or 32 bit integers
     powerwindow_uint8_T, powerwindow_uint16_T, powerwindow_uint32_T  - unsigned 8, 16, or 32 bit integers
  =======================================================================*/

#define  powerwindow_MAX_int8_T      ((powerwindow_int8_T)(127))            /* 127  */
#define  powerwindow_MIN_int8_T      ((powerwindow_int8_T)(-128))           /* -128 */
#define  powerwindow_MAX_uint8_T     ((powerwindow_uint8_T)(255))           /* 255  */
#define  powerwindow_MIN_uint8_T     ((powerwindow_uint8_T)(0))

#define  powerwindow_MAX_int16_T     ((powerwindow_int16_T)(32767))         /* 32767 */
#define  powerwindow_MIN_int16_T     ((powerwindow_int16_T)(-32768))        /* -32768 */
#define  powerwindow_MAX_uint16_T    ((powerwindow_uint16_T)(65535))        /* 65535 */
#define  powerwindow_MIN_uint16_T    ((powerwindow_uint16_T)(0))

#define  powerwindow_MAX_int32_T     ((powerwindow_int32_T)(2147483647))    /* 2147483647  */
#define  powerwindow_MIN_int32_T     ((powerwindow_int32_T)(-2147483647-1)) /* -2147483648 */
#define  powerwindow_MAX_uint32_T    ((powerwindow_uint32_T)(0xFFFFFFFFU))  /* 4294967295  */
#define  powerwindow_MIN_uint32_T    ((powerwindow_uint32_T)(0))

#if defined(_MSC_VER) || (defined(__BORLANDC__) && __BORLANDC__ >= 0x530) \
|| (defined(__WATCOMC__)  && __WATCOMC__  >= 1100) \
|| defined(__LCC64__)
#  ifdef INT64_T
#    define  powerwindow_MAX_int64_T     ((powerwindow_int64_T)(9223372036854775807LL))
#    define  powerwindow_MIN_int64_T     ((powerwindow_int64_T)(-9223372036854775807LL-1LL))
#  endif
#  ifdef UINT64_T
#    define  powerwindow_MAX_uint64_T    ((powerwindow_uint64_T)(0xFFFFFFFFFFFFFFFFULL))
#    define  powerwindow_MIN_uint64_T    ((powerwindow_uint64_T)(0))
#  endif
#else
#  ifdef INT64_T
#    ifdef INT_TYPE_64_IS_LONG
#      define  powerwindow_MAX_int64_T     ((powerwindow_int64_T)(9223372036854775807L))
#      define  powerwindow_MIN_int64_T     ((powerwindow_int64_T)(-9223372036854775807L-1L))
#    else
#      define  powerwindow_MAX_int64_T     ((powerwindow_int64_T)(9223372036854775807LL))
#      define  powerwindow_MIN_int64_T     ((powerwindow_int64_T)(-9223372036854775807LL-1LL))
#    endif
#  endif
#  ifdef UINT64_T
#    ifdef INT_TYPE_64_IS_LONG
#      define  powerwindow_MAX_uint64_T    ((powerwindow_uint64_T)(0xFFFFFFFFFFFFFFFFUL))
#      define  powerwindow_MIN_uint64_T    ((powerwindow_uint64_T)(0))
#    else
#      define  powerwindow_MAX_uint64_T    ((powerwindow_uint64_T)(0xFFFFFFFFFFFFFFFFULL))
#      define  powerwindow_MIN_uint64_T    ((powerwindow_uint64_T)(0))
#    endif
#  endif
#endif

#ifdef _MSC_VER
/* Conversion from unsigned __int64 to double is not implemented in windows
   and results in a compile error, thus the value must first be cast to
   signed __int64, and then to double.

   If the 64 bit int value is greater than 2^63-1, which is the signed int64 max,
   the macro below provides a workaround for casting a uint64 value to a double
   in windows.
*/
#  define uint64_to_double(u) ( ((u) > _I64_MAX) ? \
            (double)(__int64)((u) - _I64_MAX - 1) + (double)_I64_MAX + 1: \
            (double)(__int64)(u) )

/* The following inline function should only be used in the macro double_to_uint64,
   as it only handles the specfic range of double between 2^63 and 2^64-1 */
__forceinline
powerwindow_uint64_T double_to_uint64_helper( double d )
{
  union double_to_uint64_union_type {
    double dd;
    powerwindow_uint64_T i64;
  } di;
  di.dd = d;
  return ( ( ( di.i64 & 0x000fffffffffffff ) | 0x0010000000000000 ) << 11 );
}

/* The largest double value that can be cast to uint64 in windows is the
   signed int64 max, which is 2^63-1. The macro below provides
   a workaround for casting large double values to uint64 in windows.
*/
/* The magic number 18446744073709551616.0 is 2^64 */
/* The magic number 9223372036854775808.0 is 2^63 */
#  define double_to_uint64(d) ( ((d) >= 18446744073709551616.0) ? \
            0xffffffffffffffffULL : \
            ((d) < 0.0) ? 0ULL : \
            ((d) >= 9223372036854775808.0) ? \
            double_to_uint64_helper(d) : \
            (unsigned __int64)(d) )
#else
#  define uint64_to_double(u) ((double)(u))
#  if defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__TICCSC__)
/* double_to_uint64 defined only for MSVC and UNIX */
#  else
#  define double_to_uint64(d) ( ((d) > 0xffffffffffffffffULL) ? \
            (unsigned long long) 0xffffffffffffffffULL : \
            ((d) < 0) ? (unsigned long long) 0 : (unsigned long long)(d) )
#  endif
#endif

#if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)

#ifndef _bool_T
#define _bool_T

typedef powerwindow_boolean_T bool;

#ifndef false
#define false (0)
#endif
#ifndef true
#define true (1)
#endif

#endif /* _bool_T */

#endif /* !__cplusplus */

/*
   This software assumes that the code is being compiled on a target using a
   2's complement representation for signed integer values.
*/
#if ((SCHAR_MIN + 1) != -SCHAR_MAX)
#error "This code must be compiled using a 2's complement representation for signed integer values"
#endif

/*
   Maximum length of a MATLAB identifier (function/variable/model)
   including the null-termination character.
*/
#define TMW_NAME_LENGTH_MAX 64


#ifdef MX_COMPAT_32
typedef int powerwindow_mwSize;
typedef int powerwindow_mwIndex;
typedef int powerwindow_mwSignedIndex;
#else
typedef size_t
powerwindow_mwSize;         /* unsigned pointer-width integer */
typedef size_t
powerwindow_mwIndex;        /* unsigned pointer-width integer */
typedef ptrdiff_t
powerwindow_mwSignedIndex;  /* a signed pointer-width integer */
#endif

#if (defined(_LP64) || defined(_WIN64)) && !defined(MX_COMPAT_32)
/* Currently 2^48 based on hardware limitations */
# define powerwindow_MWSIZE_MAX    281474976710655UL
# define powerwindow_MWINDEX_MAX   281474976710655UL
# define powerwindow_MWSINDEX_MAX  281474976710655L
# define powerwindow_MWSINDEX_MIN -281474976710655L
#else
# define powerwindow_MWSIZE_MAX    2147483647UL
# define powerwindow_MWINDEX_MAX   2147483647UL
# define powerwindow_MWSINDEX_MAX  2147483647L
# define powerwindow_MWSINDEX_MIN -2147483647L
#endif
#define powerwindow_MWSIZE_MIN    0UL
#define powerwindow_MWINDEX_MIN   0UL

/** UTF-16 character type */

#if (defined(__cplusplus) && (__cplusplus >= 201103L)) || (defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) && _HAS_CHAR16_T_LANGUAGE_SUPPORT)
typedef char16_t powerwindow_CHAR16_T;
#elif defined(_MSC_VER)
typedef wchar_t powerwindow_CHAR16_T;
#else
typedef UINT16_T powerwindow_CHAR16_T;
#endif

#endif  /* powerwindow__TMWTYPES__ */

#endif /* powerwindow_tmwtypes_h */
