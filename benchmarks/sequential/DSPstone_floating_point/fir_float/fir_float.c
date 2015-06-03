/*
 * benchmark program:   fir.c
 * 
 * benchmark suite:     DSP-kernel (floating-point)
 * 
 * description:         fir - filter benchmarking
 * 
 * reference code:      target assembly
 * 
 * f. verification:     simulator
 * 
 *  organization:        Aachen University of Technology - IS2 
 *                       DSP Tools Group
 *                       phone:  +49(241)807887 
 *                       fax:    +49(241)8888195
 *                       e-mail: zivojnov@ert.rwth-aachen.de 
 *
 * author:              Juan Martinez Velarde
 * 
 * history:             12-04-94 creation fixed-point (Martinez Velarde) 
 *                      15-02-95 adapting for floating-point (Harald L. Schraut)
 *
 *
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:43:53 $
 */

#define STORAGE_CLASS register
#define TYPE  float
#define LENGTH 16

void
pin_down(TYPE * px, TYPE * ph, TYPE y)
{
  STORAGE_CLASS TYPE    i;
  _Pragma("loopbound min 16 max 16")
  for (i = 1; i <= LENGTH; i++) 
    {
      *px++ = i;
      *ph++ = i;
    }
}

int
main()
{
  static TYPE  x[LENGTH];
  static TYPE  h[LENGTH];
  
  static TYPE  x0 = 100;

  int i ;
  STORAGE_CLASS TYPE *px, *px2 ;
  STORAGE_CLASS TYPE *ph ;
  STORAGE_CLASS TYPE y;
  
  pin_down(x, h, y);

  ph  = &h[LENGTH-1] ; 
  px  = &x[LENGTH-1]  ; 
  px2 = &x[LENGTH-2]  ; 

  y = 0;

  _Pragma("loopbound min 15 max 15")
  for (i = 0; i < LENGTH - 1; i++)
    {
      y += *ph-- * *px ; 
      *px-- = *px2-- ; 
    }

  y += *ph * *px ;
  *px = x0 ; 
 
  pin_down(x, h, y);

  return 0;
}
