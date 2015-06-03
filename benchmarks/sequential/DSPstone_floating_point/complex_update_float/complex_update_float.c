/*
 * benchmark program:   complex_update.c
 * 
 * benchmark suite:     DSP-kernel
 * 
 * description:         complex_update - filter benchmarking
 * 
 * This program performs a complex update of the form D = C + A*B,
 * where A, B, C and D are complex numbers .
 * 
 *          A = Ar + j Ai
 *          B = Br + j Bi
 *          C = Cr + j Ci
 *          D = C + A*B =   Dr  +  j Di
 *                      =>  Dr = Cr + Ar*Br - Ai*Bi
 *                      =>  Di = Ci + Ar*Bi + Ai*Br
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
 * history:             11-05-94 creation for fixed-point (Martinez Velarde)
 *                      16-03-95 adaption for floating-point (Harald L. Schraut)
 *
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:43:42 $
 */

#define STORAGE_CLASS register
#define TYPE  float

void
pin_down(TYPE *p, TYPE *a, TYPE *b, TYPE *c)
{
  *a++ = 2; *a = 1;
  *b++ = 2; *b = 5;
  *c++ = 3; *c = 4;
  *p++ = 0; *p = 0; 
}


int main()
{
  static TYPE A[2];
  static TYPE B[2];
  static TYPE C[2];
  static TYPE D[2];

  STORAGE_CLASS TYPE *p_a = &A[0];
  STORAGE_CLASS TYPE *p_b = &B[0];
  STORAGE_CLASS TYPE *p_c = &C[0];
  STORAGE_CLASS TYPE *p_d = &D[0];

  pin_down(D, A, B, C);

  *p_d    = *p_c++ + *p_a++ * *p_b++;
  *p_d++ -=          *p_a   * *p_b--;

  *p_d    = *p_c   + *p_a-- * *p_b++;
  *p_d   +=          *p_a   * *p_b;

  pin_down(D, A, B, C);

  return 0;
}


