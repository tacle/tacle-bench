/*
 *  benchmark program  : fft_1024_7_fixed_point
 * 
 *  benchmark suite    : DSP-kernel
 *
 *  description        : benchmarking of an integer input scaled FFT 
 *
 *                      To avoid errors caused by overflow and bit growth, 
 *                      the input data is scaled. Bit growth occurs potentially
 *                      at butterfly operations, which involve a complex 
 *                      multiplication, a complex addition and a complex 
 *                      subtraction. Maximal bit growth from butterfly input 
 *                      to butterfly output is two bits. 
 *
 *                      The input data includes enough extra sign bits, called 
 *                      guard bits, to ensure that bit growth never results in 
 *                      overflow.
 *
 *                      The number of guard bits necessary to compensate the 
 *                      maximum bit growth in an N-point FFT is (log_2 (N))+1).
 * 
 *                      In a 16-point FFT (requires 4 stages), each of the 
 *                      input samples must contain 5 guard bits. Indeed, the 
 *                      input data is restricted to 9 bits, in order to prevent
 *                      a overflow from the integer multiplication with the
 *                      7 bit precalculed twiddle coefficients.
 *                     
 *                      Input data is held on the include file "input1024.dat"
 *                      in float format (0 ... 1)
 *                      Data is transformed automatically to 1.9 fract format
 *
 *  reference code     : none
 *
 *  func. verification : comparison with known float 1024 point FFT
 *
 *  organization       : Aachen University of Technology - IS2
 *                     : DSP Tools Group
 *                     : phone   : +49(241)807887
 *                     : fax     : +49(241)8888195
 *                     : e-mail  : zivojnov@ert.rwth-aachen.de
 *
 *  author             : Juan Martinez Velarde
 *
 *  history            : 07-02-94 - creation
 *                       16-02-94 - c50 profiling
 *
 *                       $Author: ionov $
 *                       $Revision: 1.2 $
 *                       $Date: 2009-11-09 12:18:26 $
 */

#define STORAGE_CLASS register
#define TYPE int

#define N_FFT 1024
#define NUMBER_OF_BITS 9     /* fract format 1.NUMBER_OF_BITS = 1.9 */

#define BITS_PER_TWID 7      /* bits per twiddle coefficient */
#define SHIFT BITS_PER_TWID  /* fractional shift after each multiplication */

/* precalculated twiddle factors
   for an integer 1024 point FFT 
   in format 1.7 => table twidtable[2*(N_FFT-1)] ; */
int twidtable[2046]={127, 0, 127, 0, 0, -127, 127, 0, 90, -90, 0, -127, -90, -90, 127, 0, 117, -49, 90, -90, 49, -117, 0, -127, -49, -117, -90, -90,
  -117, -49, 127, 0, 125, -25, 117, -49, 106, -71, 90, -90, 71, -106, 49, -117, 25, -125, 0, -127, -25, -125, -49, -117, -71, -106, -90, -90, -106, -71,
  -117, -49, -125, -25, 127, 0, 126, -12, 125, -25, 122, -37, 117, -49, 112, -60, 106, -71, 98, -81, 90, -90, 81, -98, 71, -106, 60, -112, 49, -117, 37,
  -122, 25, -125, 12, -126, 0, -127, -12, -126, -25, -125, -37, -122, -49, -117, -60, -112, -71, -106, -81, -98, -90, -90, -98, -81, -106, -71, -112,-60,
  -117, -49, -122, -37, -125, -25, -126, -12, 127, 0, 127, -6, 126, -12, 126, -19, 125, -25, 123, -31, 122, -37, 120, -43, 117, -49, 115, -54, 112, -60,
  109, -65, 106, -71, 102, -76, 98, -81, 94, -85, 90, -90, 85, -94, 81, -98, 76, -102, 71, -106, 65, -109, 60, -112, 54, -115, 49, -117, 43, -120, 37,
  -122, 31, -123, 25, -125, 19, -126, 12, -126, 6, -127, 0, -127, -6, -127, -12, -126, -19, -126, -25, -125, -31, -123, -37, -122, -43, -120, -49, -117,
  -54, -115, -60, -112, -65, -109, -71, -106, -76, -102, -81, -98, -85, -94, -90, -90, -94, -85, -98, -81, -102, -76, -106, -71, -109, -65, -112, -60,
  -115, -54, -117, -49, -120, -43, -122, -37, -123, -31, -125, -25, -126, -19, -126, -12, -127, -6, 127, 0, 127, -3, 127, -6, 127, -9, 126, -12, 126,
  -16, 126, -19, 125, -22, 125, -25, 124, -28, 123, -31, 122, -34, 122, -37, 121, -40, 120, -43, 118, -46, 117, -49, 116, -51, 115, -54, 113, -57,
  112, -60, 111, -63, 109, -65, 107, -68, 106, -71, 104, -73, 102, -76, 100, -78, 98, -81, 96, -83, 94, -85, 92, -88, 90, -90, 88, -92, 85, -94, 83,
  -96, 81, -98, 78, -100, 76, -102, 73, -104, 71, -106, 68, -107, 65, -109, 63, -111, 60, -112, 57, -113, 54, -115, 51, -116, 49, -117, 46, -118, 43,
  -120, 40, -121, 37, -122, 34, -122, 31, -123, 28, -124, 25, -125, 22, -125, 19, -126, 16, -126, 12, -126, 9, -127, 6, -127, 3, -127, 0, -127, -3,
  -127, -6, -127, -9, -127, -12, -126, -16, -126, -19, -126, -22, -125, -25, -125, -28, -124, -31, -123, -34, -122, -37, -122, -40, -121, -43, -120,
  -46, -118, -49, -117, -51, -116, -54, -115, -57, -113, -60, -112, -63, -111, -65, -109, -68, -107, -71, -106, -73, -104, -76, -102, -78, -100, -81,
  -98, -83, -96, -85, -94, -88, -92, -90, -90, -92, -88, -94, -85, -96, -83, -98, -81, -100, -78, -102, -76, -104, -73, -106, -71, -107, -68, -109,
  -65, -111, -63, -112, -60, -113, -57, -115, -54, -116, -51, -117, -49, -118, -46, -120, -43, -121, -40, -122, -37, -122, -34, -123, -31, -124, -28,
  -125, -25, -125, -22, -126, -19, -126, -16, -126, -12, -127, -9, -127, -6, -127, -3, 127, 0, 127, -2, 127, -3, 127, -5, 127, -6, 127, -8, 127, -9,
  127, -11, 126, -12, 126, -14, 126, -16, 126, -17, 126, -19, 125, -20, 125, -22, 125, -23, 125, -25, 124, -26, 124, -28, 124, -29, 123, -31, 123, -32,
  122, -34, 122, -35, 122, -37, 121, -38, 121, -40, 120, -41, 120, -43, 119, -44, 118, -46, 118, -47, 117, -49, 117, -50, 116, -51, 115, -53, 115, -54,
  114, -56, 113, -57, 113, -58, 112, -60, 111, -61, 111, -63, 110, -64, 109, -65, 108, -67, 107, -68, 106, -69, 106, -71, 105, -72, 104, -73, 103, -74,
  102, -76, 101, -77, 100, -78, 99, -79, 98, -81, 97, -82, 96, -83, 95, -84, 94, -85, 93, -86, 92, -88, 91, -89, 90, -90, 89, -91, 88, -92, 86, -93, 85,
  -94, 84, -95, 83, -96, 82, -97, 81, -98, 79, -99, 78, -100, 77, -101, 76, -102, 74, -103, 73, -104, 72, -105, 71, -106, 69, -106, 68, -107, 67, -108,
  65, -109, 64, -110, 63, -111, 61, -111, 60, -112, 58, -113, 57, -113, 56, -114, 54, -115, 53, -115, 51, -116, 50, -117, 49, -117, 47, -118, 46, -118,
  44, -119, 43, -120, 41, -120, 40, -121, 38, -121, 37, -122, 35, -122, 34, -122, 32, -123, 31, -123, 29, -124, 28, -124, 26, -124, 25, -125, 23, -125,
  22, -125, 20, -125, 19, -126, 17, -126, 16, -126, 14, -126, 12, -126, 11, -127, 9, -127, 8, -127, 6, -127, 5, -127, 3, -127, 2, -127, 0, -127, -2,-127,
  -3, -127, -5, -127, -6, -127, -8, -127, -9, -127, -11, -127, -12, -126, -14, -126, -16, -126, -17, -126, -19, -126, -20, -125, -22, -125, -23, -125,
  -25, -125, -26, -124, -28, -124, -29, -124, -31, -123, -32, -123, -34, -122, -35, -122, -37, -122, -38, -121, -40, -121, -41, -120, -43, -120, -44,
  -119, -46, -118, -47, -118, -49, -117, -50, -117, -51, -116, -53, -115, -54, -115, -56, -114, -57, -113, -58, -113, -60, -112, -61, -111, -63, -111,
  -64, -110, -65, -109, -67, -108, -68, -107, -69, -106, -71, -106, -72, -105, -73, -104, -74, -103, -76, -102, -77, -101, -78, -100, -79, -99, -81,
  -98, -82, -97, -83, -96, -84, -95, -85, -94, -86, -93, -88, -92, -89, -91, -90, -90, -91, -89, -92, -88, -93, -86, -94, -85, -95, -84, -96, -83,
  -97, -82, -98, -81, -99, -79, -100, -78, -101, -77, -102, -76, -103, -74, -104, -73, -105, -72, -106, -71, -106, -69, -107, -68, -108, -67, -109,
  -65, -110, -64, -111, -63, -111, -61, -112, -60, -113, -58, -113, -57, -114, -56, -115, -54, -115, -53, -116, -51, -117, -50, -117, -49, -118, -47,
  -118, -46, -119, -44, -120, -43, -120, -41, -121, -40, -121, -38, -122, -37, -122, -35, -122, -34, -123, -32, -123, -31, -124, -29, -124, -28, -124,
  -26, -125, -25, -125, -23, -125, -22, -125, -20, -126, -19, -126, -17, -126, -16, -126, -14, -126, -12, -127, -11, -127, -9, -127, -8, -127, -6, -127,
  -5, -127, -3, -127, -2, 127, 0, 127, -1, 127, -2, 127, -2, 127, -3, 127, -4, 127, -5, 127, -5, 127, -6, 127, -7, 127, -8, 127, -9, 127, -9, 127, -10,
  127, -11, 126, -12, 126, -12, 126, -13, 126, -14, 126, -15, 126, -16, 126, -16, 126, -17, 126, -18, 126, -19, 126, -19, 125, -20, 125, -21, 125, -22,
  125, -22, 125, -23, 125, -24, 125, -25, 124, -26, 124, -26, 124, -27, 124, -28, 124, -29, 124, -29, 123, -30, 123, -31, 123, -32, 123, -32, 123, -33,
  122, -34, 122, -35, 122, -35, 122, -36, 122, -37, 121, -38, 121, -38, 121, -39, 121, -40, 120, -41, 120, -41, 120, -42, 120, -43, 119, -44, 119, -44,
  119, -45, 118, -46, 118, -46, 118, -47, 118, -48, 117, -49, 117, -49, 117, -50, 116, -51, 116, -51, 116, -52, 115, -53, 115, -54, 115, -54, 114, -55,
  114, -56, 114, -56, 113, -57, 113, -58, 113, -58, 112, -59, 112, -60, 112, -61, 111, -61, 111, -62, 111, -63, 110, -63, 110, -64, 109, -65, 109, -65,
  109, -66, 108, -67, 108, -67, 107, -68, 107, -69, 106, -69, 106, -70, 106, -71, 105, -71, 105, -72, 104, -72, 104, -73, 103, -74, 103, -74, 102, -75,
  102, -76, 102, -76, 101, -77, 101, -78, 100, -78, 100, -79, 99, -79, 99, -80, 98, -81, 98, -81, 97, -82, 97, -82, 96, -83, 96, -84, 95, -84, 95, -85,
  94, -85, 94, -86, 93, -86, 93, -87, 92, -88, 91, -88, 91, -89, 90, -89, 90, -90, 89, -90, 89, -91, 88, -91, 88, -92, 87, -93, 86, -93, 86, -94, 85,
  -94, 85, -95, 84, -95, 84, -96, 83, -96, 82, -97, 82, -97, 81, -98, 81, -98, 80, -99, 79, -99, 79, -100, 78, -100, 78, -101, 77, -101, 76, -102, 76,
  -102, 75, -102, 74, -103, 74, -103, 73, -104, 72, -104, 72, -105, 71, -105, 71, -106, 70, -106, 69, -106, 69, -107, 68, -107, 67, -108, 67, -108, 66,
  -109, 65, -109, 65, -109, 64, -110, 63, -110, 63, -111, 62, -111, 61, -111, 61, -112, 60, -112, 59, -112, 58, -113, 58, -113, 57, -113, 56, -114, 56,
  -114, 55, -114, 54, -115, 54, -115, 53, -115, 52, -116, 51, -116, 51, -116, 50, -117, 49, -117, 49, -117, 48, -118, 47, -118, 46, -118, 46, -118, 45,
  -119, 44, -119, 44, -119, 43, -120, 42, -120, 41, -120, 41, -120, 40, -121, 39, -121, 38, -121, 38, -121, 37, -122, 36, -122, 35, -122, 35, -122, 34,
  -122, 33, -123, 32, -123, 32, -123, 31, -123, 30, -123, 29, -124, 29, -124, 28, -124, 27, -124, 26, -124, 26, -124, 25, -125, 24, -125, 23, -125, 22,
  -125, 22, -125, 21, -125, 20, -125, 19, -126, 19, -126, 18, -126, 17, -126, 16, -126, 16, -126, 15, -126, 14, -126, 13, -126, 12, -126, 12, -126, 11,
  -127, 10, -127, 9, -127, 9, -127, 8, -127, 7, -127, 6, -127, 5, -127, 5, -127, 4, -127, 3, -127, 2, -127, 2, -127, 1, -127, 0, -127, -1, -127, -2,
  -127, -2, -127, -3, -127, -4, -127, -5, -127, -5, -127, -6, -127, -7, -127, -8, -127, -9, -127, -9, -127, -10, -127, -11, -127, -12, -126, -12, -126,
  -13, -126, -14, -126, -15, -126, -16, -126, -16, -126, -17, -126, -18, -126, -19, -126, -19, -126, -20, -125, -21, -125, -22, -125, -22, -125, -23,
  -125, -24, -125, -25, -125, -26, -124, -26, -124, -27, -124, -28, -124, -29, -124, -29, -124, -30, -123, -31, -123, -32, -123, -32, -123, -33, -123,
  -34, -122, -35, -122, -35, -122, -36, -122, -37, -122, -38, -121, -38, -121, -39, -121, -40, -121, -41, -120, -41, -120, -42, -120, -43, -120, -44,
  -119, -44, -119, -45, -119, -46, -118, -46, -118, -47, -118, -48, -118, -49, -117, -49, -117, -50, -117, -51, -116, -51, -116, -52, -116, -53, -115,
  -54, -115, -54, -115, -55, -114, -56, -114, -56, -114, -57, -113, -58, -113, -58, -113, -59, -112, -60, -112, -61, -112, -61, -111, -62, -111, -63,
  -111, -63, -110, -64, -110, -65, -109, -65, -109, -66, -109, -67, -108, -67, -108, -68, -107, -69, -107, -69, -106, -70, -106, -71, -106, -71, -105,
  -72, -105, -72, -104, -73, -104, -74, -103, -74, -103, -75, -102, -76, -102, -76, -102, -77, -101, -78, -101, -78, -100, -79, -100, -79, -99, -80,
  -99, -81, -98, -81, -98, -82, -97, -82, -97, -83, -96, -84, -96, -84, -95, -85, -95, -85, -94, -86, -94, -86, -93, -87, -93, -88, -92, -88, -91,
  -89, -91, -89, -90, -90, -90, -90, -89, -91, -89, -91, -88, -92, -88, -93, -87, -93, -86, -94, -86, -94, -85, -95, -85, -95, -84, -96, -84, -96,
  -83, -97, -82, -97, -82, -98, -81, -98, -81, -99, -80, -99, -79, -100, -79, -100, -78, -101, -78, -101, -77, -102, -76, -102, -76, -102, -75, -103,
  -74, -103, -74, -104, -73, -104, -72, -105, -72, -105, -71, -106, -71, -106, -70, -106, -69, -107, -69, -107, -68, -108, -67, -108, -67, -109, -66,
  -109, -65, -109, -65, -110, -64, -110, -63, -111, -63, -111, -62, -111, -61, -112, -61, -112, -60, -112, -59, -113, -58, -113, -58, -113, -57, -114,
  -56, -114, -56, -114, -55, -115, -54, -115, -54, -115, -53, -116, -52, -116, -51, -116, -51, -117, -50, -117, -49, -117, -49, -118, -48, -118, -47,
  -118, -46, -118, -46, -119, -45, -119, -44, -119, -44, -120, -43, -120, -42, -120, -41, -120, -41, -121, -40, -121, -39, -121, -38, -121, -38, -122,
  -37, -122, -36, -122, -35, -122, -35, -122, -34, -123, -33, -123, -32, -123, -32, -123, -31, -123, -30, -124, -29, -124, -29, -124, -28, -124, -27,
  -124, -26, -124, -26, -125, -25, -125, -24, -125, -23, -125, -22, -125, -22, -125, -21, -125, -20, -126, -19, -126, -19, -126, -18, -126, -17, -126,
  -16, -126, -16, -126, -15, -126, -14, -126, -13, -126, -12, -126, -12, -127, -11, -127, -10, -127, -9, -127, -9, -127, -8, -127, -7, -127, -6, -127,
  -5, -127, -5, -127, -4, -127, -3, -127, -2, -127, -2, -127, -1 };

/* 1024 real values as input data in float format */
float input[1024] = {
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f,
  0.243f, 0.323f, 0.505f, -0.176f, -0.87f, 0.353f, -0.344f, -0.443f, -0.434f, -0.32f, 0.232f, -0.454f, -0.32f, -0.323f, -0.733f, 0.54f
};

int inputfract[N_FFT];  /* will hold the transformed data */


/* conversion function to 1.NUMBER_OF_BITS format */
float exp2f(float x)
{
  int i;
  float ret = 2.0f;

  _Pragma("loopbound min 9 max 9")
  for ( i = 1; i < x; ++i ) {
    ret *= 2.0f;
  }
  return ret;
}


float modff(float x, float *intpart)
{
  if ( intpart ) {
    *intpart = (int)x;
    return x - *intpart;
  } else
    return x;
}


/* conversion function to 1.NUMBER_OF_BITS format */
int convert(float value)
{
  float man, t_val, frac, m, exponent = NUMBER_OF_BITS;
  int rnd_val;
  unsigned long int_val;
  unsigned long pm_val;

  m = exp2f(exponent+1)  - 1;

  t_val = value * m ;
  frac = modff(t_val,&man);
  if (frac < 0.0f) 
    {
      rnd_val = (-1);
      if (frac > -0.5f) rnd_val = 0;
    }
  else
    {
      rnd_val = 1;
      if (frac < 0.5f) rnd_val = 0;
    }
  int_val = man + rnd_val;
  
  pm_val = int_val ; 
  return ((int) (pm_val)) ; 
  
}


void float2fract()
{
  float f ; 
  int   j, i ; 

  _Pragma("loopbound min 1024 max 1024")
  for (j = 0 ; j < N_FFT ; j++) 
    {
      f = input[j];
      i = convert(f);
      inputfract[j] = i;
    }
}


void fft_inpsca(STORAGE_CLASS TYPE *int_pointer)
{
  {
    STORAGE_CLASS TYPE i, j = 0  ; 
    STORAGE_CLASS TYPE tmpr, max = 2, m, n = N_FFT << 1 ; 

    /* do the bit reversal scramble of the input data */
 
    _Pragma("loopbound min 1024 max 1024")
    for (i = 0; i < (n-1) ; i += 2) 
      {
        if (j > i)
          {
            tmpr = *(int_pointer + j);
            *(int_pointer + j) = *(int_pointer + i) ;
            *(int_pointer + i) = tmpr ; 
            
            tmpr = *(int_pointer + j + 1) ; 
            *(int_pointer + j + 1) = *(int_pointer + i + 1) ; 
            *(int_pointer + i + 1) = tmpr ; 
          }

        m = N_FFT;

        _Pragma("loopbound min 0 max 10")
        while (m >= 2 && j >= m) 
          {
            j -= m ;
            m >>= 1;
          }
        j += m ;
      }

    {
      STORAGE_CLASS TYPE *data_pointer = &twidtable[0] ; 
      STORAGE_CLASS TYPE *p, *q ; 
      STORAGE_CLASS TYPE tmpi, fr = 0, level, k, l ; 

      _Pragma("loopbound min 10 max 10")
      while (n > max)
        {
          level = max << 1;

          _Pragma("loopbound min 1 max 512")
          for (m = 1; m < max; m += 2) 
            {
              l = *(data_pointer + fr);
              k = *(data_pointer + fr + 1);
              fr += 2;

              _Pragma("loopbound min 1 max 512")
              for (i = m; i <= n; i += level) 
                {
                  j = i + max;
                  p = int_pointer + j;
                  q = int_pointer + i;

                  tmpr  = l * *(p-1);
                  tmpr -= (k * *p );

                  tmpi  = l * *p ;
                  tmpi += (k * *(p-1));

                  tmpr  = tmpr >> SHIFT ; 
                  tmpi  = tmpi >> SHIFT ; 

                  *(p-1) = *(q-1) - tmpr ; 
                  *p     = *q - tmpi ; 

                  *(q-1) += tmpr ;
                  *q     += tmpi ; 
                }
            }
          max = level;
        }
    }
  }
}


void pin_down(TYPE input_data[])
{
  /* conversion from input1024.dat to a 1.9 format */

  float2fract();

  {
    int *pd, *ps, f;

    pd = &input_data[0];
    ps = &inputfract[0];

    _Pragma("loopbound min 1024 max 1024")
    for (f = 0; f < N_FFT; f++)
      {
        *pd++ = *ps++  ; /* fill in with real data */
        *pd++ = 0 ;      /* imaginary data is equal zero */
      }
  }
}


int main()
{
  TYPE input_data[2*N_FFT];

  pin_down(&input_data[0]);

  fft_inpsca(&input_data[0]);

  return 0;
}


