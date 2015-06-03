/*
 * benchmark program:   real_update.c
 * 
 * benchmark suite:     DSP-kernel
 * 
 * description:         real_update - filter benchmarking
 * 
 * This program performs a real update of the form D = C + A*B,
 * where A, B, C and D are real numbers .
  * 
 * reference code:      target assembly
 * 
 * f. verification:     simulator based
 * 
 *  organization:        Aachen University of Technology - IS2 
 *                       DSP Tools Group
 *                       phone:  +49(241)807887 
 *                       fax:    +49(241)8888195
 *                       e-mail: zivojnov@ert.rwth-aachen.de 
 *
 * author:              Juan Martinez Velarde
 * 
 * history:             11-05-94 creation fixed-point (Martinez Velarde)
 *                      16-03-95 adaption floating-point (Harald L. Schraut)
 *                      12-10-95 changing pin-up (Armin Braun)
 * 
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:44:15 $
 */

#define STORAGE_CLASS register
#define TYPE          float

void pin_down(TYPE *p,TYPE *q,TYPE *r,TYPE *s)
{
  *p = 10;
  *q =  2;
  *r =  1;
  *s =  0;
}


int main()
{
  static TYPE A = 10;
  static TYPE B = 2;
  static TYPE C = 1;
  static TYPE D = 0;

  STORAGE_CLASS TYPE *p_a = &A;
  STORAGE_CLASS TYPE *p_b = &B;
  STORAGE_CLASS TYPE *p_c = &C;
  STORAGE_CLASS TYPE *p_d = &D;

  pin_down(&A,&B,&C,&D);

  *p_d  = *p_c + *p_a * *p_b;

  pin_down(&A,&B,&C,&D);

  return(0);
}



