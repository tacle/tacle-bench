/*
 * Math library emulation functions
 *  
 * Copyright (C) 2007 IRIT - UPS <casse@irit.fr>
 *
 * This file is part of papabench.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with papabench; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA. 
 *
 */
#include <math.h>

double pp_atan2(double x, double y)
{
	double coeff_1 = M_PI/4;
	double coeff_2 = 3*coeff_1;
	double abs_y = fabs(y)+1e-10;
	double angle, r;
	if(x>0)
	{
		r = (x - abs_y)/(x + abs_y);
		angle = coeff_1 - coeff_1*r;
	}
	else
	{
		r = (x + abs_y)/(abs_y - x);
		angle = coeff_2 - coeff_1*r;
	}
	if(y<0)
		return (-angle);
	else
		return angle;
}


/* Calculates sin(x), angle x must be in rad.
 * Range: -pi/2 <= x <= pi/2
 * Precision: +/- .000,000,005
 */
 
double pp_sin(double x)
{
	double xi, y, q, q2;
	int sign;

	xi = x; sign = 1;
	while (xi < -1.57079632679489661923) xi += 6.28318530717958647692;
	while (xi > 4.71238898038468985769) xi -= 6.28318530717958647692;
	if (xi > 1.57079632679489661923) {
		xi -= 3.141592653589793238462643;
		sign = -1;
	}
	q = xi / 1.57079632679; q2 = q * q;
	y = ((((.00015148419  * q2
	      - .00467376557) * q2
	      + .07968967928) * q2
	      - .64596371106) * q2
	      +1.57079631847) * q;
	return(sign < 0? -y : y);
}

#define SQRT_PRECISION 5
double pp_sqrt(double n)
{
#if 0
	float x, m;
	int i, e;
	
	/* compute the approximation */
	m = frexpf(n, &e);
	x = ldexp(m, e/2);
	
	/* perform the computation */
	for(i = 0; i < SQRT_PRECISION; i++)
		x = (x + n / x) / 2;
	return x;
#endif
}
