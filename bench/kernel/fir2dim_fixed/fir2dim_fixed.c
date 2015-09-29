/*
 * benchmark program:   fir2dim.c
 * 
 * benchmark suite:     DSP-kernel
 * 
 * description:         fir2dim - filter benchmarking
 *
 * The image is an array IMAGEDIM * IMAGEDIM pixels. To provide 
 * conditions for the FIR filtering, the image is surrounded by a
 * set of zeros such that the image is actually stored as a 
 * ARRAYDIM * ARRAYDIM = (IMAGEDIM + 2) * (IMAGEDIM + 2) array
 *
 *          <--ARRAYDIM-->  
 *         |0 0 0 .... 0 0| A
 *         |0 x x .... x 0| |
 *         |0 x x .... x 0| ARRAY_
 *         |0 image area 0| DIM
 *         |0 x x .... x 0| |
 *         |0 x x .... x 0| |
 *         |0 0 0 .... 0 0| V
 * 
 * The image (with boundary) is stored in row major storage. The
 * first element is array(1,1) followed by array(1,2). The last
 * element of the first row is array(1,514) following by the 
 * beginning of the next column array(2,1).
 *
 * The two dimensional FIR uses a 3x3 coefficient mask:
 *
 *         |c11 c12 c13|
 *         |c21 c22 c23|
 *         |c31 c32 c33|
 *
 * The output image is of dimension IMAGEDIM * IMAGEDIM.
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
 * history:             15-5-94 creation (Martinez Velarde) 
 *
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:31:25 $
 */

#define STORAGE_CLASS register
#define TYPE          int
#define IMAGEDIM      4
#define ARRAYDIM      (IMAGEDIM + 2)
#define COEFFICIENTS  3

void
pin_down(TYPE *pimage, TYPE *parray, TYPE *pcoeff, TYPE *poutput)
{
  STORAGE_CLASS TYPE    i,f;

  _Pragma("loopbound min 4 max 4")
  for (i = 0 ; i < IMAGEDIM ; i++)
    {
      _Pragma("loopbound min 4 max 4")
      for (f = 0 ; f < IMAGEDIM ; f++)
        *pimage++ = 1 ; 
    }

  pimage = pimage - IMAGEDIM*IMAGEDIM  ;

  _Pragma("loopbound min 9 max 9")
  for (i = 0; i < COEFFICIENTS*COEFFICIENTS; i++)
    *pcoeff++ = 1;

  _Pragma("loopbound min 6 max 6")
  for (i = 0 ; i < ARRAYDIM ; i++)
    *parray++ = 0 ; 

  _Pragma("loopbound min 4 max 4")
  for (f = 0 ; f < IMAGEDIM; f++)
    {
      *parray++ = 0 ;
      _Pragma("loopbound min 4 max 4")
      for (i = 0 ; i < IMAGEDIM ; i++)
        *parray++ = *pimage++ ;
      *parray++ = 0 ;
    }

  _Pragma("loopbound min 6 max 6")
  for (i = 0 ; i < ARRAYDIM ; i++)
    *parray++ = 0 ; 

  _Pragma("loopbound min 16 max 16")
  for (i = 0 ; i < IMAGEDIM * IMAGEDIM; i++)
    *poutput++ = 0 ;
}


int main()
{
  static TYPE  coefficients[COEFFICIENTS*COEFFICIENTS] ; 
  static TYPE  image[IMAGEDIM*IMAGEDIM]  ;
  static TYPE  array[ARRAYDIM*ARRAYDIM]  ;
  static TYPE  output[IMAGEDIM*IMAGEDIM] ; 

  STORAGE_CLASS TYPE *pimage  = &image[0]        ;
  STORAGE_CLASS TYPE *parray  = &array[0], *parray2, *parray3 ; 
  STORAGE_CLASS TYPE *pcoeff  = &coefficients[0] ;
  STORAGE_CLASS TYPE *poutput = &output[0]       ;  
  STORAGE_CLASS TYPE k, f, i;
    
  pin_down(&image[0], &array[0], &coefficients[0], &output[0]);

  pimage  = &image[0]        ; 
  parray  = &array[0]        ; 
  pcoeff  = &coefficients[0] ; 
  poutput = &output[0]       ; 

  _Pragma("loopbound min 4 max 4")
  for (k = 0 ; k < IMAGEDIM ; k++)
    {

      _Pragma("loopbound min 4 max 4")
      for (f = 0 ; f < IMAGEDIM ; f++)
        {
          pcoeff = &coefficients[0] ; 
          parray = &array[k*ARRAYDIM + f] ;
          parray2 = parray + ARRAYDIM ; 
          parray3 = parray + ARRAYDIM + ARRAYDIM ; 

          *poutput = 0 ;

          _Pragma("loopbound min 3 max 3")
          for (i = 0 ; i < 3 ; i++)
            *poutput += *pcoeff++ * *parray++ ;

          _Pragma("loopbound min 3 max 3")
          for (i = 0 ; i < 3 ; i++)
            *poutput += *pcoeff++ * *parray2++ ;

          _Pragma("loopbound min 3 max 3")
          for (i = 0 ; i < 3 ; i++)
            *poutput += *pcoeff++ * *parray3++ ;

          poutput++ ;
        }
    }

  pin_down(&image[0], &array[0], &coefficients[0], &output[0]);
  return 0;
}



