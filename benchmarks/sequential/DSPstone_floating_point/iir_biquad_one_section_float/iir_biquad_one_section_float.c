/*
 *  benchmark program  : biquad_one_section.c
 *
 *  benchmark suite    : DSP-kernel
 *
 *  description        : benchmarking of an one iir biquad
 *                       
 *
 *	The equations of the filter are:
 *       w(n) =    x(n) - a1*w(n-1) - a2*w(n-2)
 *       y(n) = b0*w(n) + b1*w(n-1) + b2*w(n-2)
 *
 *		             w(n)
 *   x (n)------(-)---------->-|->----b0---(+)-------> y(n)
 *               A             |            A
 *               |           |1/z|          |
 *               |             | w(n-1)     |
 *               |             v            |
 *               |-<--a1-----<-|->----b1-->-|
 *               |             |            |
 *               |           |1/z|          |
 *               |             | w(n-2)     |
 *               |             v            |
 *               |-<--a2-----<--->----b2-->-|
 * 
 *     The values w(n-1) and w(n-2) are stored in w1 and w2
 * 
 *                                              
 *  reference code     
 *
 *  func. verification from separate computation
 *
 *  organization       Aachen University of Technology - IS2
 *                     DSP Tools Group
 *                     phone   : +49(241)807887
 *                     fax     : +49(241)8888195
 *                     e-mail  : zivojnov@ert.rwth-aachen.de
 *
 *  author             Juan Martinez Velarde
 *
 *  history            19-03-95 creation fixed-point (Martinez Velarde)
 *                     16-03-95 adaption floating-point (Harald L. Schraut)
 *
 *                     $Author: lokuciej $
 *                     $Revision: 1.1 $
 *                     $Date: 2009-07-20 21:43:58 $
 */

#define STORAGE_CLASS register
#define TYPE          float

TYPE 
pin_down(TYPE x)
{
  return ((TYPE) 7) ;
}


TYPE main()
{
  STORAGE_CLASS TYPE y, w;

  static TYPE x = 7, w1= 7 , w2 = 7;
  static TYPE b0 = 7, b1 = 7 , b2 = 7;
  static TYPE a1 = 7, a2 = 7;

  w  = x - a1 * w1;
  w -= a2 * w2;
  y  = b0 * w;
  y += b1 * w1;
  y += b2 * w2;

  w2 = w1;
  w1 = w;

  x  = pin_down(x);
  w1 = pin_down(w1);
  w2 = pin_down(w2);

  return((TYPE) y);
}



