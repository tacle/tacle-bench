/*
 * benchmark program:   mat1x3.c
 * 
 * benchmark suite:     DSP-kernel
 * 
 * description:         1x3 matrix - multiply benchmarking
 *
 *               |h11 h12 h13|   |x1|   |y1|   | h11*x1+h12*x2+h31*x3 |
 *               |h21 h22 h23| * |x2| = |y2| = | h21*x1+h22*x2+h23*x3 |
 *               |h31 h32 h33|   |x3|   |y3|   | h31*x1+h32*x2+h33*x3 |
 * 
 * Element are to store in following order:
 * 
 * matrix h[9]={h11,h12,h13, h21,h22,h23, h31,h32,h33}
 * vector x[3]={x1,x2,x3}
 * vector y[3]={y1,y1,y3}
 * 
 * reference code:       none
 * 
 * f. verification:      
 * 
 * organization:         Aachen University of Technology - IS2 
 *                       DSP Tools Group
 *                       phone:  +49(241)807887 
 *                       fax:    +49(241)8888195
 *                       e-mail: zivojnov@ert.rwth-aachen.de 
 *
 * author:              Juan Martinez Velarde
 * 
 * history:             31-3-94 creation (Martinez Velarde)
 * 
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:31:40 $
 */

#define STORAGE_CLASS  register
#define TYPE           int


int main()
{
  static TYPE h[9]={1,2,3,1,2,3,3,2,1};
  static TYPE x[3]={1,1,1};
  static TYPE y[3]={0,0,0};

  STORAGE_CLASS TYPE *p_x = &x[0];
  STORAGE_CLASS TYPE *p_h = &h[0];
  STORAGE_CLASS TYPE *p_y = &y[0];

  STORAGE_CLASS TYPE f,i;

  _Pragma("loopbound min 3 max 3")
  for (i = 0 ; i < 3; i++)
    {
      /* p_x points to the beginning of the input vector */
      *p_y = 0;
      p_x = &x[0];

       /* do matrix multiply */
      _Pragma("loopbound min 3 max 3")
      for (f = 0 ; f < 3; f++)
        *p_y += *p_h++ * *p_x++;

      /* next element */
      p_y++;
    }

  return 0;
}
