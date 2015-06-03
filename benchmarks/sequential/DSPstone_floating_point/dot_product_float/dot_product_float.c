/*
 * benchmark program:   dot_product.c
 * 
 * benchmark suite:     DSP-kernel
 * 
 * description:         dot product benchmarking
 *
 * This program performs a dot product of the form Z=AB,
 * where A is a [1x2] vector and B is a [2x1] vector.
 * 
 *          A[1 x 2] * B[2 x 1] = Z
 *                  
 * vector A[1 x 2]= |a1 a2|
 *
 * vector B[2 x 1]= | b1 |
 *                  | b2 |
 *
 * dot product Z = a1*b1 + a2* b2
 * 
 * vector elements are stored as
 *
 * A[1 x 2] = { a1, a1 }
 * 
 * B[2 x 1] = { b1, b2 }
 * 
 * 
 * reference code:       none
 * 
 * f. verification:      with printf function
 * 
 * organization:         Aachen University of Technology - IS2 
 *                       DSP Tools Group
 *                       phone:  +49(241)807887 
 *                       fax:    +49(241)8888195
 *                       e-mail: zivojnov@ert.rwth-aachen.de 
 *
 * author:              Juan Martinez Velarde
 * 
 * history:             10-05-94 C Code creation fixed-point (Martinez Velarde)
 *                      16-03-95 adaption for floating-point (Harald L. Schraut)
 *
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:43:48 $
 */

#define STORAGE_CLASS  register
#define TYPE           float

void pin_down(TYPE *Z, TYPE *A, TYPE *B)
{
  *A++ = 2; *A = 1;
  *B++ = 2; *B = 5;
  *Z = 0 ; 
}

int main()
{ 
  static  TYPE A[2];
  static  TYPE B[2];
  static  TYPE Z;

  STORAGE_CLASS TYPE *p_a = &A[0];
  STORAGE_CLASS TYPE *p_b = &B[0];
  STORAGE_CLASS TYPE *p_z = &Z;

  int f;

  pin_down(&Z, A, B);

  _Pragma("loopbound min 2 max 2")
  for (f=0;f<2;f++)
    *p_z  += *p_a++ * *p_b++ ;

  pin_down(&Z, A, B);

  return 0;
}



