/*
 * benchmark program:   convolution.c
 * 
 * benchmark suite:     DSP-kernel
 * 
 * description:         convolution - filter benchmarking
 * 
 * reference code:      target assembly
 * 
 * f. verification:     none
 * 
 *  organization:        Aachen University of Technology - IS2 
 *                       DSP Tools Group
 *                       phone:  +49(241)807887 
 *                       fax:    +49(241)8888195
 *                       e-mail: zivojnov@ert.rwth-aachen.de 
 *
 * author:              Vojin Zivojnovic
 * 
 * history:             14-1-94 creation (Vojin Zivojnovic)
 *                      18-3-94 asm labels included (Martinez Velarde)
 *
 *                      $Author: lokuciej $
 *                      $Revision: 1.1 $
 *                      $Date: 2009-07-20 21:31:18 $
 */

#define STORAGE_CLASS register
#define TYPE          int
#define LENGTH 16

void pin_down(TYPE * px, TYPE * ph)
{
	STORAGE_CLASS TYPE    i;

  _Pragma("loopbound min 16 max 16")
	for (i = 0; i < LENGTH; ++i) {
		*px++ = 1;
		*ph++ = 1;
	}
}


int main()
{

	static TYPE     x[LENGTH];
	static TYPE     h[LENGTH];

	STORAGE_CLASS TYPE y;
	STORAGE_CLASS TYPE i;
	STORAGE_CLASS TYPE *px = x;
	STORAGE_CLASS TYPE *ph = &h[LENGTH - 1];

	pin_down(&x[0], &h[0]);

	y = 0;

  _Pragma("loopbound min 16 max 16")
	for (i = 0; i < LENGTH; ++i)
		y += *px++ * *ph--;

	return 0;
}



