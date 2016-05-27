/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_rtwtypes.h

 Author: CoSys-Lab, University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_rtwtypes.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */
#ifndef powerwindow__RTWTYPES_H__
#define powerwindow__RTWTYPES_H__

/* Logical type definitions */
#if (!defined(__cplusplus))
#  ifndef false
#   define false                       (0U)
#  endif

#  ifndef true
#   define true                        (1U)
#  endif
#endif

#define powerwindow__TMWTYPES__

/*=======================================================================*
 * Target hardware information
 *
 *   Number of bits:     char:   8    short:   16    int:  16
 *                       long:  32
 *                       native word size:   8
 *   Byte ordering: LittleEndian
 *   Signed integer division rounds to: Zero
 *   Shift right on a signed integer as arithmetic shift: on
 *=======================================================================*/

/*=======================================================================*
 * Fixed width word size data types:                                     *
 *   powerwindow_int8_T, powerwindow_int16_T, powerwindow_int32_T     - signed 8, 16, or 32 bit integers     *
 *   powerwindow_uint8_T, powerwindow_uint16_T, powerwindow_uint32_T  - unsigned 8, 16, or 32 bit integers   *
 *   powerwindow_real32_T, powerwindow_real64_T           - 32 and 64 bit floating point numbers *
 *=======================================================================*/
typedef signed char powerwindow_int8_T;
typedef unsigned char powerwindow_uint8_T;
typedef int powerwindow_int16_T;
typedef unsigned int powerwindow_uint16_T;
typedef long powerwindow_int32_T;
typedef unsigned long powerwindow_uint32_T;
typedef float powerwindow_real32_T;
typedef double powerwindow_real64_T;

/*===========================================================================*
 * Generic type definitions: powerwindow_real_T, powerwindow_time_T, powerwindow_boolean_T, powerwindow_int_T, powerwindow_uint_T,       *
 *                           powerwindow_ulong_T, powerwindow_char_T and powerwindow_byte_T.                     *
 *===========================================================================*/
typedef double powerwindow_real_T;
typedef double powerwindow_time_T;
typedef unsigned char powerwindow_boolean_T;
typedef int powerwindow_int_T;
typedef unsigned int powerwindow_uint_T;
typedef unsigned long powerwindow_ulong_T;
typedef char powerwindow_char_T;
typedef unsigned char powerwindow_uchar_T;
typedef powerwindow_char_T powerwindow_byte_T;

/*===========================================================================*
 * Complex number type definitions                                           *
 *===========================================================================*/
#define powerwindow_CREAL_T

typedef struct {
    powerwindow_real32_T re;
    powerwindow_real32_T im;
} powerwindow_creal32_T;

typedef struct {
    powerwindow_real64_T re;
    powerwindow_real64_T im;
} powerwindow_creal64_T;

typedef struct {
    powerwindow_real_T re;
    powerwindow_real_T im;
} powerwindow_creal_T;

#define powerwindow_CINT8_T

typedef struct {
    powerwindow_int8_T re;
    powerwindow_int8_T im;
} powerwindow_cint8_T;

#define powerwindow_CUINT8_T

typedef struct {
    powerwindow_uint8_T re;
    powerwindow_uint8_T im;
} powerwindow_cuint8_T;

#define powerwindow_CINT16_T

typedef struct {
    powerwindow_int16_T re;
    powerwindow_int16_T im;
} powerwindow_cint16_T;

#define powerwindow_CUINT16_T

typedef struct {
    powerwindow_uint16_T re;
    powerwindow_uint16_T im;
} powerwindow_cuint16_T;

#define powerwindow_CINT32_T

typedef struct {
    powerwindow_int32_T re;
    powerwindow_int32_T im;
} powerwindow_cint32_T;

#define powerwindow_CUINT32_T

typedef struct {
    powerwindow_uint32_T re;
    powerwindow_uint32_T im;
} powerwindow_cuint32_T;

/*=======================================================================*
 * Min and Max:                                                          *
 *   powerwindow_int8_T, powerwindow_int16_T, powerwindow_int32_T     - signed 8, 16, or 32 bit integers     *
 *   powerwindow_uint8_T, powerwindow_uint16_T, powerwindow_uint32_T  - unsigned 8, 16, or 32 bit integers   *
 *=======================================================================*/
#define powerwindow_MAX_int8_T                     ((powerwindow_int8_T)(127))
#define powerwindow_MIN_int8_T                     ((powerwindow_int8_T)(-128))
#define powerwindow_MAX_uint8_T                    ((powerwindow_uint8_T)(255U))
#define powerwindow_MIN_uint8_T                    ((powerwindow_uint8_T)(0U))
#define powerwindow_MAX_int16_T                    ((powerwindow_int16_T)(32767))
#define powerwindow_MIN_int16_T                    ((powerwindow_int16_T)(-32768))
#define powerwindow_MAX_uint16_T                   ((powerwindow_uint16_T)(65535U))
#define powerwindow_MIN_uint16_T                   ((powerwindow_uint16_T)(0U))
#define powerwindow_MAX_int32_T                    ((powerwindow_int32_T)(2147483647))
#define powerwindow_MIN_int32_T                    ((powerwindow_int32_T)(-2147483647-1))
#define powerwindow_MAX_uint32_T                   ((powerwindow_uint32_T)(0xFFFFFFFFU))
#define powerwindow_MIN_uint32_T                   ((powerwindow_uint32_T)(0U))

/* Block D-Work pointer type */
typedef void * powerwindow_pointer_T;

/* Simulink specific types */
#ifndef powerwindow___ZERO_CROSSING_TYPES__
#define powerwindow___ZERO_CROSSING_TYPES__

/* Trigger directions: falling, either, and rising */
typedef enum {
    FALLING_ZERO_CROSSING = -1,
    ANY_ZERO_CROSSING = 0,
    RISING_ZERO_CROSSING = 1
} powerwindow_ZCDirection;

/* Previous state of a trigger signal */
typedef powerwindow_uint8_T ZCSigState;

/* Initial value of a trigger zero crossing signal */
#define powerwindow_UNINITIALIZED_ZCSIG            0x03U
#define powerwindow_NEG_ZCSIG                      0x02U
#define powerwindow_POS_ZCSIG                      0x01U
#define powerwindow_ZERO_ZCSIG                     0x00U

/* Current state of a trigger signal */
typedef enum {
    FALLING_ZCEVENT = -1,
    NO_ZCEVENT = 0,
    RISING_ZCEVENT = 1
} powerwindow_ZCEventType;

#endif                                 /* powerwindow___ZERO_CROSSING_TYPES__ */
#endif                                 /* powerwindow__RTWTYPES_H__ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
