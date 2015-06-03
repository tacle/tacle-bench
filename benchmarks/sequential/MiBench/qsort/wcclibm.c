#include "wcclibm.h"
#include "math_private.h"

/* e_powf.c -- float version of e_pow.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifdef __STDC__
static const float
#else
static float
#endif
bp[] = {1.0f, 1.5f,},
dp_h[] = { 0.0f, 5.84960938e-01f,}, /* 0x3f15c000 */
dp_l[] = { 0.0f, 1.56322085e-06f,}, /* 0x35d1cfdc */
L1  =  6.0000002384e-01f, /* 0x3f19999a */
L2  =  4.2857143283e-01f, /* 0x3edb6db7 */
L3  =  3.3333334327e-01f, /* 0x3eaaaaab */
L4  =  2.7272811532e-01f, /* 0x3e8ba305 */
L5  =  2.3066075146e-01f, /* 0x3e6c3255 */
L6  =  2.0697501302e-01f, /* 0x3e53f142 */
P1   =  1.6666667163e-01f, /* 0x3e2aaaab */
P2   = -2.7777778450e-03f, /* 0xbb360b61 */
P3   =  6.6137559770e-05f, /* 0x388ab355 */
P4   = -1.6533901999e-06f, /* 0xb5ddea0e */
P5   =  4.1381369442e-08f, /* 0x3331bb4c */
lg2  =  6.9314718246e-01f, /* 0x3f317218 */
lg2_h  =  6.93145752e-01f, /* 0x3f317200 */
lg2_l  =  1.42860654e-06f, /* 0x35bfbe8c */
ovt =  4.2995665694e-08f, /* -(128-log2(ovfl+.5ulp)) */
cp    =  9.6179670095e-01f, /* 0x3f76384f =2/(3ln2) */
cp_h  =  9.6179199219e-01f, /* 0x3f763800 =head of cp */
cp_l  =  4.7017383622e-06f, /* 0x369dc3a0 =tail of cp_h */
ivln2    =  1.4426950216e+00f, /* 0x3fb8aa3b =1/ln2 */
ivln2_h  =  1.4426879883e+00f, /* 0x3fb8aa00 =16b 1/ln2*/
ivln2_l  =  7.0526075433e-06f; /* 0x36eca570 =1/ln2 tail*/

#ifdef __STDC__
	float __ieee754_powf(float x, float y)
#else
	float __ieee754_powf(x,y)
	float x, y;
#endif
{
	float z,ax,z_h,z_l,p_h,p_l;
	float y1,t1,t2,r,s,t,u,v,w;
	int32_t i,j,k,yisint,n;
	int32_t hx,hy,ix,iy,is;

	GET_FLOAT_WORD(hx,x);
	GET_FLOAT_WORD(hy,y);
	ix = hx&0x7fffffff;  iy = hy&0x7fffffff;

    /* y==zero: x**0 = 1 */
	if(iy==0) return one;

    /* x==+-1 */
	if(x == 1.0f) return one;
	if(x == -1.0f && isinf(y)) return one;

    /* +-NaN return x+y */
	if(ix > 0x7f800000 ||
	   iy > 0x7f800000)
		return x+y;

    /* determine if y is an odd int when x < 0
     * yisint = 0	... y is not an integer
     * yisint = 1	... y is an odd int
     * yisint = 2	... y is an even int
     */
	yisint  = 0;
	if(hx<0) {
	    if(iy>=0x4b800000) yisint = 2; /* even integer y */
	    else if(iy>=0x3f800000) {
		k = (iy>>23)-0x7f;	   /* exponent */
		j = iy>>(23-k);
		if((j<<(23-k))==iy) yisint = 2-(j&1);
	    }
	}

    /* special value of y */
	if (iy==0x7f800000) {	/* y is +-inf */
	    if (ix==0x3f800000)
	        return  y - y;	/* inf**+-1 is NaN */
	    else if (ix > 0x3f800000)/* (|x|>1)**+-inf = inf,0 */
	        return (hy>=0)? y: zero;
	    else			/* (|x|<1)**-,+inf = inf,0 */
	        return (hy<0)?-y: zero;
	}
	if(iy==0x3f800000) {	/* y is  +-1 */
	    if(hy<0) return one/x; else return x;
	}
	if(hy==0x40000000) return x*x; /* y is  2 */
	if(hy==0x3f000000) {	/* y is  0.5 */
	    if(hx>=0)	/* x >= +0 */
	    return __ieee754_sqrtf(x);
	}

	ax   = fabsf(x);
    /* special value of x */
	if(ix==0x7f800000||ix==0||ix==0x3f800000){
	    z = ax;			/*x is +-0,+-inf,+-1*/
	    if(hy<0) z = one/z;	/* z = (1/|x|) */
	    if(hx<0) {
		if(((ix-0x3f800000)|yisint)==0) {
		    z = (z-z)/(z-z); /* (-1)**non-int is NaN */
		} else if(yisint==1)
		    z = -z;		/* (x<0)**odd = -(|x|**odd) */
	    }
	    return z;
	}

    /* (x<0)**(non-int) is NaN */
	if(((((u_int32_t)hx>>31)-1)|yisint)==0) return (x-x)/(x-x);

    /* |y| is huge */
	if(iy>0x4d000000) { /* if |y| > 2**27 */
	/* over/underflow if x is not close to one */
	    if(ix<0x3f7ffff8) return (hy<0)? huge*huge:tiny*tiny;
	    if(ix>0x3f800007) return (hy>0)? huge*huge:tiny*tiny;
	/* now |1-x| is tiny <= 2**-20, suffice to compute
	   log(x) by x-x^2/2+x^3/3-x^4/4 */
	    t = x-1;		/* t has 20 trailing zeros */
	    w = (t*t)*((float)0.5f-t*((float)0.333333333333f-t*(float)0.25f));
	    u = ivln2_h*t;	/* ivln2_h has 16 sig. bits */
	    v = t*ivln2_l-w*ivln2;
	    t1 = u+v;
	    GET_FLOAT_WORD(is,t1);
	    SET_FLOAT_WORD(t1,is&0xfffff000);
	    t2 = v-(t1-u);
	} else {
	    float s2,s_h,s_l,t_h,t_l;
	    n = 0;
	/* take care subnormal number */
	    if(ix<0x00800000)
		{ax *= two24; n -= 24; GET_FLOAT_WORD(ix,ax); }
	    n  += ((ix)>>23)-0x7f;
	    j  = ix&0x007fffff;
	/* determine interval */
	    ix = j|0x3f800000;		/* normalize ix */
	    if(j<=0x1cc471) k=0;	/* |x|<sqrt(3/2) */
	    else if(j<0x5db3d7) k=1;	/* |x|<sqrt(3)   */
	    else {k=0;n+=1;ix -= 0x00800000;}
	    SET_FLOAT_WORD(ax,ix);

	/* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
	    u = ax-bp[k];		/* bp[0]=1.0, bp[1]=1.5 */
	    v = one/(ax+bp[k]);
	    s = u*v;
	    s_h = s;
	    GET_FLOAT_WORD(is,s_h);
	    SET_FLOAT_WORD(s_h,is&0xfffff000);
	/* t_h=ax+bp[k] High */
	    SET_FLOAT_WORD(t_h,((ix>>1)|0x20000000)+0x0040000+(k<<21));
	    t_l = ax - (t_h-bp[k]);
	    s_l = v*((u-s_h*t_h)-s_h*t_l);
	/* compute log(ax) */
	    s2 = s*s;
	    r = s2*s2*(L1+s2*(L2+s2*(L3+s2*(L4+s2*(L5+s2*L6)))));
	    r += s_l*(s_h+s);
	    s2  = s_h*s_h;
	    t_h = (float)3.0f+s2+r;
	    GET_FLOAT_WORD(is,t_h);
	    SET_FLOAT_WORD(t_h,is&0xfffff000);
	    t_l = r-((t_h-(float)3.0f)-s2);
	/* u+v = s*(1+...) */
	    u = s_h*t_h;
	    v = s_l*t_h+t_l*s;
	/* 2/(3log2)*(s+...) */
	    p_h = u+v;
	    GET_FLOAT_WORD(is,p_h);
	    SET_FLOAT_WORD(p_h,is&0xfffff000);
	    p_l = v-(p_h-u);
	    z_h = cp_h*p_h;		/* cp_h+cp_l = 2/(3*log2) */
	    z_l = cp_l*p_h+p_l*cp+dp_l[k];
	/* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
	    t = (float)n;
	    t1 = (((z_h+z_l)+dp_h[k])+t);
	    GET_FLOAT_WORD(is,t1);
	    SET_FLOAT_WORD(t1,is&0xfffff000);
	    t2 = z_l-(((t1-t)-dp_h[k])-z_h);
	}

	s = one; /* s (sign of result -ve**odd) = -1 else = 1 */
	if(((((u_int32_t)hx>>31)-1)|(yisint-1))==0)
	    s = -one;	/* (-ve)**(odd int) */

    /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
	GET_FLOAT_WORD(is,y);
	SET_FLOAT_WORD(y1,is&0xfffff000);
	p_l = (y-y1)*t1+y*t2;
	p_h = y1*t1;
	z = p_l+p_h;
	GET_FLOAT_WORD(j,z);
	if (j>0x43000000)				/* if z > 128 */
	    return s*huge*huge;				/* overflow */
	else if (j==0x43000000) {			/* if z == 128 */
	    if(p_l+ovt>z-p_h) return s*huge*huge;	/* overflow */
	}
	else if ((j&0x7fffffff)>0x43160000)		/* z <= -150 */
	    return s*tiny*tiny;				/* underflow */
	else if ((u_int32_t) j==0xc3160000){		/* z == -150 */
	    if(p_l<=z-p_h) return s*tiny*tiny;		/* underflow */
	}
    /*
     * compute 2**(p_h+p_l)
     */
	i = j&0x7fffffff;
	k = (i>>23)-0x7f;
	n = 0;
	if(i>0x3f000000) {		/* if |z| > 0.5, set n = [z+0.5] */
	    n = j+(0x00800000>>(k+1));
	    k = ((n&0x7fffffff)>>23)-0x7f;	/* new k for n */
	    SET_FLOAT_WORD(t,n&~(0x007fffff>>k));
	    n = ((n&0x007fffff)|0x00800000)>>(23-k);
	    if(j<0) n = -n;
	    p_h -= t;
	}
	t = p_l+p_h;
	GET_FLOAT_WORD(is,t);
	SET_FLOAT_WORD(t,is&0xfffff000);
	u = t*lg2_h;
	v = (p_l-(t-p_h))*lg2+t*lg2_l;
	z = u+v;
	w = v-(z-u);
	t  = z*z;
	t1  = z - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
	r  = (z*t1)/(t1-two)-(w+z*w);
	z  = one-(r-z);
	GET_FLOAT_WORD(j,z);
	j += (n<<23);
	if((j>>23)<=0) z = __scalbnf(z,n);	/* subnormal output */
	else SET_FLOAT_WORD(z,j);
	return s*z;
}


/* e_sqrtf.c -- float version of e_sqrt.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#ifdef __STDC__
	float __ieee754_sqrtf(float x)
#else
	float __ieee754_sqrtf(x)
	float x;
#endif
{
	float z;
	int32_t sign = (int)0x80000000; 
	int32_t ix,s,q,m,t,i;
	u_int32_t r;

	GET_FLOAT_WORD(ix,x);

    /* take care of Inf and NaN */
	if((ix&0x7f800000)==0x7f800000) {			
	    return x*x+x;		/* sqrt(NaN)=NaN, sqrt(+inf)=+inf
					   sqrt(-inf)=sNaN */
	} 
    /* take care of zero */
	if(ix<=0) {
	    if((ix&(~sign))==0) return x;/* sqrt(+-0) = +-0 */
	    else if(ix<0)
		return (x-x)/(x-x);		/* sqrt(-ve) = sNaN */
	}
    /* normalize x */
	m = (ix>>23);
	if(m==0) {				/* subnormal x */
      _Pragma( "loopbound min 0 max 0" )
	    for(i=0;(ix&0x00800000)==0;i++) {
        ix<<=1;
      }
	    m -= i-1;
	}
	m -= 127;	/* unbias exponent */
	ix = (ix&0x007fffff)|0x00800000;
	if(m&1)	/* odd m, double x to make it even */
	    ix += ix;
	m >>= 1;	/* m = [m/2] */

    /* generate sqrt(x) bit by bit */
	ix += ix;
	q = s = 0;		/* q = sqrt(x) */
	r = 0x01000000;		/* r = moving bit from right to left */

  _Pragma( "loopbound min 25 max 25" )
	while(r!=0) {
	    t = s+r; 
	    if(t<=ix) { 
		s    = t+r; 
		ix  -= t; 
		q   += r; 
	    } 
	    ix += ix;
	    r>>=1;
	}

    /* use floating add to find out rounding direction */
	if(ix!=0) {
	    z = one-tiny; /* trigger inexact flag */
	    if (z>=one) {
	        z = one+tiny;
		if (z>one)
		    q += 2;
		else
		    q += (q&1);
	    }
	}
	ix = (q>>1)+0x3f000000;
	ix += (m <<23);
	SET_FLOAT_WORD(z,ix);
	return z;
}


/* s_copysignf.c -- float version of s_copysign.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * copysignf(float x, float y)
 * copysignf(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */


#ifdef __STDC__
	float __copysignf(float x, float y)
#else
	float __copysignf(x,y)
	float x,y;
#endif
{
	u_int32_t ix,iy;
	GET_FLOAT_WORD(ix,x);
	GET_FLOAT_WORD(iy,y);
	SET_FLOAT_WORD(x,(ix&0x7fffffff)|(iy&0x80000000));
        return x;
}


/* e_rem_pio2f.c -- float version of e_rem_pio2.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: e_rem_pio2f.c,v 1.5 1995/05/10 20:46:03 jtc Exp $";
#endif

/* __ieee754_rem_pio2f(x,y)
 *
 * return the remainder of x rem pi/2 in y[0]+y[1]
 * use __kernel_rem_pio2f()
 */


/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
 */
#ifdef __STDC__
static const int32_t two_over_pi[] = {
#else
static int32_t two_over_pi[] = {
#endif
0xA2, 0xF9, 0x83, 0x6E, 0x4E, 0x44, 0x15, 0x29, 0xFC,
0x27, 0x57, 0xD1, 0xF5, 0x34, 0xDD, 0xC0, 0xDB, 0x62,
0x95, 0x99, 0x3C, 0x43, 0x90, 0x41, 0xFE, 0x51, 0x63,
0xAB, 0xDE, 0xBB, 0xC5, 0x61, 0xB7, 0x24, 0x6E, 0x3A,
0x42, 0x4D, 0xD2, 0xE0, 0x06, 0x49, 0x2E, 0xEA, 0x09,
0xD1, 0x92, 0x1C, 0xFE, 0x1D, 0xEB, 0x1C, 0xB1, 0x29,
0xA7, 0x3E, 0xE8, 0x82, 0x35, 0xF5, 0x2E, 0xBB, 0x44,
0x84, 0xE9, 0x9C, 0x70, 0x26, 0xB4, 0x5F, 0x7E, 0x41,
0x39, 0x91, 0xD6, 0x39, 0x83, 0x53, 0x39, 0xF4, 0x9C,
0x84, 0x5F, 0x8B, 0xBD, 0xF9, 0x28, 0x3B, 0x1F, 0xF8,
0x97, 0xFF, 0xDE, 0x05, 0x98, 0x0F, 0xEF, 0x2F, 0x11,
0x8B, 0x5A, 0x0A, 0x6D, 0x1F, 0x6D, 0x36, 0x7E, 0xCF,
0x27, 0xCB, 0x09, 0xB7, 0x4F, 0x46, 0x3F, 0x66, 0x9E,
0x5F, 0xEA, 0x2D, 0x75, 0x27, 0xBA, 0xC7, 0xEB, 0xE5,
0xF1, 0x7B, 0x3D, 0x07, 0x39, 0xF7, 0x8A, 0x52, 0x92,
0xEA, 0x6B, 0xFB, 0x5F, 0xB1, 0x1F, 0x8D, 0x5D, 0x08,
0x56, 0x03, 0x30, 0x46, 0xFC, 0x7B, 0x6B, 0xAB, 0xF0,
0xCF, 0xBC, 0x20, 0x9A, 0xF4, 0x36, 0x1D, 0xA9, 0xE3,
0x91, 0x61, 0x5E, 0xE6, 0x1B, 0x08, 0x65, 0x99, 0x85,
0x5F, 0x14, 0xA0, 0x68, 0x40, 0x8D, 0xFF, 0xD8, 0x80,
0x4D, 0x73, 0x27, 0x31, 0x06, 0x06, 0x15, 0x56, 0xCA,
0x73, 0xA8, 0xC9, 0x60, 0xE2, 0x7B, 0xC0, 0x8C, 0x6B,
};

/* This array is like the one in e_rem_pio2.c, but the numbers are
   single precision and the last 8 bits are forced to 0.  */
#ifdef __STDC__
static const int32_t npio2_hw[] = {
#else
static int32_t npio2_hw[] = {
#endif
0x3fc90f00, 0x40490f00, 0x4096cb00, 0x40c90f00, 0x40fb5300, 0x4116cb00,
0x412fed00, 0x41490f00, 0x41623100, 0x417b5300, 0x418a3a00, 0x4196cb00,
0x41a35c00, 0x41afed00, 0x41bc7e00, 0x41c90f00, 0x41d5a000, 0x41e23100,
0x41eec200, 0x41fb5300, 0x4203f200, 0x420a3a00, 0x42108300, 0x4216cb00,
0x421d1400, 0x42235c00, 0x4229a500, 0x422fed00, 0x42363600, 0x423c7e00,
0x4242c700, 0x42490f00
};

/*
 * invpio2:  24 bits of 2/pi
 * pio2_1:   first  17 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 17 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  17 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */

#ifdef __STDC__
static const float
#else
static float
#endif
/* zero =  0.0000000000e+00f, /\* 0x00000000 *\/ */
/* half =  5.0000000000e-01f, /\* 0x3f000000 *\/ */
/* two8 =  2.5600000000e+02f, /\* 0x43800000 *\/ */
invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

#ifdef __STDC__
	int32_t __ieee754_rem_pio2f(float x, float *y)
#else
	int32_t __ieee754_rem_pio2f(x,y)
	float x,y[];
#endif
{
	float z,w,t,r,fn;
	float tx[3];
	int32_t e0,i,j,nx,n,ix,hx;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix<=0x3f490fd8)   /* |x| ~<= pi/4 , no need for reduction */
	    {y[0] = x; y[1] = 0; return 0;}
	if(ix<0x4016cbe4) {  /* |x| < 3pi/4, special case with n=+-1 */
	    if(hx>0) {
		z = x - pio2_1;
		if((ix&0xfffffff0)!=0x3fc90fd0) { /* 24+24 bit pi OK */
		    y[0] = z - pio2_1t;
		    y[1] = (z-y[0])-pio2_1t;
		} else {		/* near pi/2, use 24+24+24 bit pi */
		    z -= pio2_2;
		    y[0] = z - pio2_2t;
		    y[1] = (z-y[0])-pio2_2t;
		}
		return 1;
	    } else {	/* negative x */
		z = x + pio2_1;
		if((ix&0xfffffff0)!=0x3fc90fd0) { /* 24+24 bit pi OK */
		    y[0] = z + pio2_1t;
		    y[1] = (z-y[0])+pio2_1t;
		} else {		/* near pi/2, use 24+24+24 bit pi */
		    z += pio2_2;
		    y[0] = z + pio2_2t;
		    y[1] = (z-y[0])+pio2_2t;
		}
		return -1;
	    }
	}
	if(ix<=0x43490f80) { /* |x| ~<= 2^7*(pi/2), medium size */
	    t  = fabsf(x);
	    n  = (int32_t) (t*invpio2+half);
	    fn = (float)n;
	    r  = t-fn*pio2_1;
	    w  = fn*pio2_1t;	/* 1st round good to 40 bit */
	    if(n<32&&(int32_t)(ix&0xffffff00)!=npio2_hw[n-1]) {
		y[0] = r-w;	/* quick check no cancellation */
	    } else {
	        u_int32_t high;
	        j  = ix>>23;
	        y[0] = r-w;
		GET_FLOAT_WORD(high,y[0]);
	        i = j-((high>>23)&0xff);
	        if(i>8) {  /* 2nd iteration needed, good to 57 */
		    t  = r;
		    w  = fn*pio2_2;
		    r  = t-w;
		    w  = fn*pio2_2t-((t-r)-w);
		    y[0] = r-w;
		    GET_FLOAT_WORD(high,y[0]);
		    i = j-((high>>23)&0xff);
		    if(i>25)  {	/* 3rd iteration need, 74 bits acc */
		    	t  = r;	/* will cover all possible cases */
		    	w  = fn*pio2_3;
		    	r  = t-w;
		    	w  = fn*pio2_3t-((t-r)-w);
		    	y[0] = r-w;
		    }
		}
	    }
	    y[1] = (r-y[0])-w;
	    if(hx<0) 	{y[0] = -y[0]; y[1] = -y[1]; return -n;}
	    else	 return n;
	}
    /*
     * all other (large) arguments
     */
	if(ix>=0x7f800000) {		/* x is inf or NaN */
	    y[0]=y[1]=x-x; return 0;
	}
	return 0; /* doesn't happen for our input */
}

/* k_cosf.c -- float version of k_cos.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: k_cosf.c,v 1.4 1995/05/10 20:46:23 jtc Exp $";
#endif


#ifdef __STDC__
static const float 
#else
static float 
#endif
/* one =  1.0000000000e+00, /\* 0x3f800000 *\/ */
C1  =  4.1666667908e-02f, /* 0x3d2aaaab */
C2  = -1.3888889225e-03f, /* 0xbab60b61 */
C3  =  2.4801587642e-05f, /* 0x37d00d01 */
C4  = -2.7557314297e-07f, /* 0xb493f27c */
C5  =  2.0875723372e-09f, /* 0x310f74f6 */
C6  = -1.1359647598e-11f; /* 0xad47d74e */

#ifdef __STDC__
	float __kernel_cosf(float x, float y)
#else
	float __kernel_cosf(x, y)
	float x,y;
#endif
{
	float a,hz,z,r,qx;
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;			/* ix = |x|'s high word*/
	if(ix<0x32000000) {			/* if x < 2**27 */
	    if(((int)x)==0) return one;		/* generate inexact */
	}
	z  = x*x;
	r  = z*(C1+z*(C2+z*(C3+z*(C4+z*(C5+z*C6)))));
	if(ix < 0x3e99999a) 			/* if |x| < 0.3 */ 
	    return one - ((float)0.5f*z - (z*r - x*y));
	else {
	    if(ix > 0x3f480000) {		/* x > 0.78125 */
		qx = (float)0.28125f;
	    } else {
	        SET_FLOAT_WORD(qx,ix-0x01000000);	/* x/4 */
	    }
	    hz = (float)0.5f*z-qx;
	    a  = one-qx;
	    return a - (hz - (z*r-x*y));
	}
}

/* k_sinf.c -- float version of k_sin.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: k_sinf.c,v 1.4 1995/05/10 20:46:33 jtc Exp $";
#endif


#ifdef __STDC__
static const float 
#else
static float 
#endif
/* half =  5.0000000000e-01f,/\* 0x3f000000 *\/ */
S1  = -1.6666667163e-01f, /* 0xbe2aaaab */
S2  =  8.3333337680e-03f, /* 0x3c088889 */
S3  = -1.9841270114e-04f, /* 0xb9500d01 */
S4  =  2.7557314297e-06f, /* 0x3638ef1b */
S5  = -2.5050759689e-08f, /* 0xb2d72f34 */
S6  =  1.5896910177e-10f; /* 0x2f2ec9d3 */

#ifdef __STDC__
	float __kernel_sinf(float x, float y, int iy)
#else
	float __kernel_sinf(x, y, iy)
	float x,y; int iy;		/* iy=0 if y is zero */
#endif
{
	float z,r,v;
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;			/* high word of x */
	if(ix<0x32000000)			/* |x| < 2**-27 */
	   {if((int)x==0) return x;}		/* generate inexact */
	z	=  x*x;
	v	=  z*x;
	r	=  S2+z*(S3+z*(S4+z*(S5+z*S6)));
	if(iy==0) return x+v*(S1+z*r);
	else      return x-((z*(half*y-v*r)-y)-v*S1);
}
/* s_atanf.c -- float version of s_atan.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: s_atanf.c,v 1.4 1995/05/10 20:46:47 jtc Exp $";
#endif


#ifdef __STDC__
static const float atanhi[] = {
#else
static float atanhi[] = {
#endif
  4.6364760399e-01f, /* atan(0.5)hi 0x3eed6338 */
  7.8539812565e-01f, /* atan(1.0)hi 0x3f490fda */
  9.8279368877e-01f, /* atan(1.5)hi 0x3f7b985e */
  1.5707962513e+00f, /* atan(inf)hi 0x3fc90fda */
};

#ifdef __STDC__
static const float atanlo[] = {
#else
static float atanlo[] = {
#endif
  5.0121582440e-09f, /* atan(0.5)lo 0x31ac3769 */
  3.7748947079e-08f, /* atan(1.0)lo 0x33222168 */
  3.4473217170e-08f, /* atan(1.5)lo 0x33140fb4 */
  7.5497894159e-08f, /* atan(inf)lo 0x33a22168 */
};

#ifdef __STDC__
static const float aT[] = {
#else
static float aT[] = {
#endif
  3.3333334327e-01f, /* 0x3eaaaaaa */
 -2.0000000298e-01f, /* 0xbe4ccccd */
  1.4285714924e-01f, /* 0x3e124925 */
 -1.1111110449e-01f, /* 0xbde38e38 */
  9.0908870101e-02f, /* 0x3dba2e6e */
 -7.6918758452e-02f, /* 0xbd9d8795 */
  6.6610731184e-02f, /* 0x3d886b35 */
 -5.8335702866e-02f, /* 0xbd6ef16b */
  4.9768779427e-02f, /* 0x3d4bda59 */
 -3.6531571299e-02f, /* 0xbd15a221 */
  1.6285819933e-02f, /* 0x3c8569d7 */
};

/* #ifdef __STDC__ */
/* 	static const float  */
/* #else */
/* 	static float  */
/* #endif */
/* one   = 1.0, */
/* huge   = 1.0e30; */

#ifdef __STDC__
	float __atanf(float x)
#else
	float __atanf(x)
	float x;
#endif
{
	float w,s1,s2,z;
	int32_t ix,hx,id;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix>=0x50800000) {	/* if |x| >= 2^34 */
	    if(ix>0x7f800000)
		return x+x;		/* NaN */
	    if(hx>0) return  atanhi[3]+atanlo[3];
	    else     return -atanhi[3]-atanlo[3];
	} if (ix < 0x3ee00000) {	/* |x| < 0.4375 */
	    if (ix < 0x31000000) {	/* |x| < 2^-29 */
		if(huge+x>one) return x;	/* raise inexact */
	    }
	    id = -1;
	} else {
	x = fabsf(x);
	if (ix < 0x3f980000) {		/* |x| < 1.1875 */
	    if (ix < 0x3f300000) {	/* 7/16 <=|x|<11/16 */
		id = 0; x = ((float)2.0f*x-one)/((float)2.0f+x); 
	    } else {			/* 11/16<=|x|< 19/16 */
		id = 1; x  = (x-one)/(x+one); 
	    }
	} else {
	    if (ix < 0x401c0000) {	/* |x| < 2.4375 */
		id = 2; x  = (x-(float)1.5f)/(one+(float)1.5f*x);
	    } else {			/* 2.4375 <= |x| < 2^66 */
		id = 3; x  = -(float)1.0f/x;
	    }
	}}
    /* end of argument reduction */
	z = x*x;
	w = z*z;
    /* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
	s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
	s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
	if (id<0) return x - x*(s1+s2);
	else {
	    z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
	    return (hx<0)? -z:z;
	}
}
//weak_alias (__atanf, atanf)

/* s_cosf.c -- float version of s_cos.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* #ifdef __STDC__ */
/* static const float one=1.0; */
/* #else */
/* static float one=1.0; */
/* #endif */

#ifdef __STDC__
	float __cosf(float x)
#else
	float __cosf(x)
	float x;
#endif
{
	float y[2],z=0.0f;
	int32_t n,ix;

	GET_FLOAT_WORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffff;
	if(ix <= 0x3f490fd8) return __kernel_cosf(x,z);

    /* cos(Inf or NaN) is NaN */
	else if (ix>=0x7f800000) return x-x;

    /* argument reduction needed */
	else {
	    n = __ieee754_rem_pio2f(x,y);
	    switch(n&3) {
		case 0: return  __kernel_cosf(y[0],y[1]);
		case 1: return -__kernel_sinf(y[0],y[1],1);
		case 2: return -__kernel_cosf(y[0],y[1]);
		default:
		        return  __kernel_sinf(y[0],y[1],1);
	    }
	}
}

/* s_sinf.c -- float version of s_sin.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

 #ifdef __STDC__
	float __sinf(float x)
#else
	float __sinf(x)
	float x;
#endif
{
	float y[2],z=0.0;
	int32_t n, ix;

	GET_FLOAT_WORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffff;
	if(ix <= 0x3f490fd8) return __kernel_sinf(x,z,0);

    /* sin(Inf or NaN) is NaN */
	else if (ix>=0x7f800000) return x-x;

    /* argument reduction needed */
	else {
	    n = __ieee754_rem_pio2f(x,y);
	    switch(n&3) {
		case 0: return  __kernel_sinf(y[0],y[1],1);
		case 1: return  __kernel_cosf(y[0],y[1]);
		case 2: return -__kernel_sinf(y[0],y[1],1);
		default:
			return -__kernel_cosf(y[0],y[1]);
	    }
	}
}

/* s_fabsf.c -- float version of s_fabs.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * fabsf(x) returns the absolute value of x.
 */


#ifdef __STDC__
	float __fabsf(float x)
#else
	float __fabsf(x)
	float x;
#endif
{
	u_int32_t ix;
	GET_FLOAT_WORD(ix,x);
	SET_FLOAT_WORD(x,ix&0x7fffffff);
        return x;
}


/* s_scalbnf.c -- float version of s_scalbn.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: s_scalbnf.c,v 1.4 1995/05/10 20:48:10 jtc Exp $";
#endif


#ifdef __STDC__
static const float
#else
static float
#endif
two25   =  3.355443200e+07f,	/* 0x4c000000 */
twom25  =  2.9802322388e-08f;	/* 0x33000000 */

#ifdef __STDC__
	float __scalbnf (float x, int n)
#else
	float __scalbnf (x,n)
	float x; int n;
#endif
{
	int32_t k,ix;
	GET_FLOAT_WORD(ix,x);
        k = (ix&0x7f800000)>>23;		/* extract exponent */
        if (k==0) {				/* 0 or subnormal x */
            if ((ix&0x7fffffff)==0) return x; /* +-0 */
	    x *= two25;
	    GET_FLOAT_WORD(ix,x);
	    k = ((ix&0x7f800000)>>23) - 25;
	    }
        if (k==0xff) return x+x;		/* NaN or Inf */
        k = k+n;
        if (n> 50000 || k >  0xfe)
	  return huge*__copysignf(huge,x); /* overflow  */
	if (n< -50000)
	  return tiny*__copysignf(tiny,x);	/*underflow*/
        if (k > 0) 				/* normal result */
	    {SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23)); return x;}
        if (k <= -25)
	    return tiny*__copysignf(tiny,x);	/*underflow*/
        k += 25;				/* subnormal result */
	SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
        return x*twom25;
}


/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

/*
 * isinff(x) returns 1 if x is inf, -1 if x is -inf, else 0;
 * no branching!
 */
int __isinff (float x)
{
	int32_t ix,t;
	GET_FLOAT_WORD(ix,x);
	t = ix & 0x7fffffff;
	t ^= 0x7f800000;
	t |= -t;
	return ~(t >> 31) & (ix >> 30);
}
