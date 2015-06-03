/* gsm_encode.c */
/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */
#define SASR

/************************ #include "proto.h" *******************************/
#ifndef	PROTO_H
#define	PROTO_H

#if __cplusplus
#	define	NeedFunctionPrototypes	1
#endif

#if __STDC__
#	define	NeedFunctionPrototypes	1
#endif

#ifdef	_NO_PROTO
#	undef	NeedFunctionPrototypes
#endif

#undef	P	/* gnu stdio.h actually defines this... 	*/
#undef	P0
#undef	P1
#undef	P2
#undef	P3
#undef	P4
#undef	P5
#undef	P6
#undef	P7
#undef	P8

#if NeedFunctionPrototypes

#	define	P( protos )	protos

#	define	P0()				(void)
#	define	P1(x, a)			(a)
#	define	P2(x, a, b)			(a, b)
#	define	P3(x, a, b, c)			(a, b, c)
#	define	P4(x, a, b, c, d)		(a, b, c, d)	
#	define	P5(x, a, b, c, d, e)		(a, b, c, d, e)
#	define	P6(x, a, b, c, d, e, f)		(a, b, c, d, e, f)
#	define	P7(x, a, b, c, d, e, f, g)	(a, b, c, d, e, f, g)
#	define	P8(x, a, b, c, d, e, f, g, h)	(a, b, c, d, e, f, g, h)

#else /* !NeedFunctionPrototypes */

#	define	P( protos )	( /* protos */ )

#	define	P0()				()
#	define	P1(x, a)			x a;
#	define	P2(x, a, b)			x a; b;
#	define	P3(x, a, b, c)			x a; b; c;
#	define	P4(x, a, b, c, d)		x a; b; c; d;
#	define	P5(x, a, b, c, d, e)		x a; b; c; d; e;
#	define	P6(x, a, b, c, d, e, f)		x a; b; c; d; e; f;
#	define	P7(x, a, b, c, d, e, f, g)	x a; b; c; d; e; f; g;
#	define	P8(x, a, b, c, d, e, f, g, h)	x a; b; c; d; e; f; g; h;

#endif  /* !NeedFunctionPrototypes */

#endif	/* PROTO_H */

/********************** end #include "proto.h" *****************************/


/********************#include "private.h" **********************************/
#ifndef	PRIVATE_H
#define	PRIVATE_H

typedef short			word;		/* 16 bit signed int	*/
typedef long			longword;	/* 32 bit signed int	*/

typedef unsigned short		uword;		/* unsigned word	*/
typedef unsigned long		ulongword;	/* unsigned longword	*/

/*   Table 4.6	 Normalized direct mantissa used to compute xM/xmax
 */
/* i                  0      1       2      3      4      5      6      7   */
word gsm_FAC[8]	= { 18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767 };

struct gsm_state {

	word		dp0[ 280 ];

	word		z1;		/* preprocessing.c, Offset_com. */
	longword	L_z2;		/*                  Offset_com. */
	int		mp;		/*                  Preemphasis	*/

	word		u[8];		/* short_term_aly_filter.c	*/
	word		LARpp[2][8]; 	/*                              */
	word		j;		/*                              */

	word		nrp; /* 40 */	/* long_term.c, synthesis	*/
	word		v[9];		/* short_term.c, synthesis	*/
	word		msr;		/* decoder.c,	Postprocessing	*/

	char		verbose;	/* only used if !NDEBUG		*/
	char		fast;		/* only used if FAST		*/

};


#define	MIN_WORD	((-32767)-1)
#define	MAX_WORD	( 32767)

#define	MIN_LONGWORD	((-2147483647)-1)
#define	MAX_LONGWORD	( 2147483647)

#ifdef	SASR		/* >> is a signed arithmetic shift right */
#undef	SASR
#define	SASR(x, by)	((x) >> (by))
#endif	/* SASR */


int f_fast = 0;
int f_verbose = 0;

/* 4.2.18 */

/*  This procedure adds the reconstructed long term residual signal
 *  ep[0..39] to the estimated signal dpp[0..39] from the long term
 *  analysis filter to compute the reconstructed short term residual
 *  signal dp[-40..-1]; also the reconstructed short term residual
 *  array dp[-120..-41] is updated.
 */

#if 0	/* Has been inlined in code.c */
void Gsm_Update_of_reconstructed_short_time_residual_signal P3((dpp, ep, dp),
	word	* dpp,		/* [0...39]	IN	*/
	word	* ep,		/* [0...39]	IN	*/
	word	* dp)		/* [-120...-1]  IN/OUT 	*/
{
	int 		k;

	for (k = 0; k <= 79; k++) 
		dp[ -120 + k ] = dp[ -80 + k ];

	for (k = 0; k <= 39; k++)
		dp[ -40 + k ] = gsm_add( ep[k], dpp[k] );
}
#endif	/* Has been inlined in code.c */

/* long_term.c */
#ifdef	USE_TABLE_MUL

unsigned int umul_table[ 513 ][ 256 ];

# define umul(x9, x15)	\
	((int)(umul_table[x9][x15 & 0x0FF] + (umul_table[x9][ x15 >> 8 ] << 8)))

# define table_mul(a, b)	\
	( (a < 0)  ? ((b < 0) ? umul(-a, -b) : -umul(-a, b))	\
	  	   : ((b < 0) ? -umul(a, -b) :  umul(a, b)))

#endif /* USE_TABLE_MUL */



/*
 *  4.2.11 .. 4.2.12 LONG TERM PREDICTOR (LTP) SECTION
 */


/*
 * This procedure computes the LTP gain (bc) and the LTP lag (Nc)
 * for the long term analysis filter.   This is done by calculating a
 * maximum of the cross-correlation function between the current
 * sub-segment short term residual signal d[0..39] (output of
 * the short term analysis filter; for simplification the index
 * of this array begins at 0 and ends at 39 for each sub-segment of the
 * RPE-LTP analysis) and the previous reconstructed short term
 * residual signal dp[ -120 .. -1 ].  A dynamic scaling must be
 * performed to avoid overflow.
 */

 /* This procedure exists in four versions.  First, the two integer
  * versions with or without table-multiplication (as one function);
  * then, the two floating point versions (as another function), with
  * or without scaling.
  */

#ifndef  USE_FLOAT_MUL

#else	/* USE_FLOAT_MUL */

#ifdef	FAST
#endif	/* FAST 	 */

#endif	/* USE_FLOAT_MUL */


/* table.c */
#define	GSM_TABLE_C
/*  Table 4.1  Quantization of the Log.-Area Ratios
 */
/* i 		     1      2      3        4      5      6        7       8 */
word gsm_A[8]   = {20480, 20480, 20480,  20480,  13964,  15360,   8534,  9036};
word gsm_B[8]   = {    0,     0,  2048,  -2560,     94,  -1792,   -341, -1144};
word gsm_MIC[8] = { -32,   -32,   -16,    -16,     -8,     -8,     -4,    -4 };
word gsm_MAC[8] = {  31,    31,    15,     15,      7,      7,      3,     3 };


/*  Table 4.2  Tabulation  of 1/A[1..8]
 */
word gsm_INVA[8]={ 13107, 13107,  13107, 13107,  19223, 17476,  31454, 29708 };


/*   Table 4.3a  Decision level of the LTP gain quantizer
 */
/*  bc		      0	        1	  2	     3			*/
word gsm_DLB[4] = {  6554,    16384,	26214,	   32767	};


/*   Table 4.3b   Quantization levels of the LTP gain quantizer
 */
/* bc		      0          1        2          3			*/
word gsm_QLB[4] = {  3277,    11469,	21299,	   32767	};


/*   Table 4.4	 Coefficients of the weighting filter
 */
/* i		    0      1   2    3   4      5      6     7   8   9    10  */
word gsm_H[11] = {-134, -374, 0, 2054, 5741, 8192, 5741, 2054, 0, -374, -134 };


/*
 *  Inlined functions from add.h 
 */

/* 
 * #define GSM_MULT_R(a, b) (* word a, word b, !(a == b == MIN_WORD) *)	\
 *	(0x0FFFF & SASR(((longword)(a) * (longword)(b) + 16384), 15))
 */
#define GSM_MULT_R(a, b) /* word a, word b, !(a == b == MIN_WORD) */	\
	(SASR( ((longword)(a) * (longword)(b) + 16384), 15 ))

# define GSM_MULT(a,b)	 /* word a, word b, !(a == b == MIN_WORD) */	\
	(SASR( ((longword)(a) * (longword)(b)), 15 ))

# define GSM_L_MULT(a, b) /* word a, word b */	\
	(((longword)(a) * (longword)(b)) << 1)

# define GSM_L_ADD(a, b)	\
	( (a) <  0 ? ( (b) >= 0 ? (a) + (b)	\
		 : (utmp = (ulongword)-((a) + 1) + (ulongword)-((b) + 1)) \
		   >= MAX_LONGWORD ? MIN_LONGWORD : -(longword)utmp-2 )   \
	: ((b) <= 0 ? (a) + (b)   \
	          : (utmp = (ulongword)(a) + (ulongword)(b)) >= MAX_LONGWORD \
		    ? MAX_LONGWORD : utmp))

/*
 * # define GSM_ADD(a, b)	\
 * 	((ltmp = (longword)(a) + (longword)(b)) >= MAX_WORD \
 * 	? MAX_WORD : ltmp <= MIN_WORD ? MIN_WORD : ltmp)
 */
/* Nonportable, but faster: */

#define	GSM_ADD(a, b)	\
	((ulongword)((ltmp = (longword)(a) + (longword)(b)) - MIN_WORD) > \
		MAX_WORD - MIN_WORD ? (ltmp > 0 ? MAX_WORD : MIN_WORD) : ltmp)

# define GSM_SUB(a, b)	\
	((ltmp = (longword)(a) - (longword)(b)) >= MAX_WORD \
	? MAX_WORD : ltmp <= MIN_WORD ? MIN_WORD : ltmp)

# define GSM_ABS(a)	((a) < 0 ? ((a) == MIN_WORD ? MAX_WORD : -(a)) : (a))

#define	saturate(x) 	\
	((x) < MIN_WORD ? MIN_WORD : (x) > MAX_WORD ? MAX_WORD: (x))

/* Use these if necessary:

# define GSM_MULT_R(a, b)	gsm_mult_r(a, b)
# define GSM_MULT(a, b)		gsm_mult(a, b)
# define GSM_L_MULT(a, b)	gsm_L_mult(a, b)

# define GSM_L_ADD(a, b)	gsm_L_add(a, b)
# define GSM_ADD(a, b)		gsm_add(a, b)
# define GSM_SUB(a, b)		gsm_sub(a, b)

# define GSM_ABS(a)		gsm_abs(a)

*/

/*
 *  More prototypes from implementations..
 */

/*
 *  Tables from table.c
 */
#ifndef	GSM_TABLE_C

extern word gsm_A[8], gsm_B[8], gsm_MIC[8], gsm_MAC[8];
extern word gsm_INVA[8];
extern word gsm_DLB[4], gsm_QLB[4];
extern word gsm_H[11];
extern word gsm_NRFAC[8];
extern word gsm_FAC[8];

#endif	/* GSM_TABLE_C */

/*
 *  Debugging
 */
#ifdef NDEBUG

#	define	gsm_debug_words(a, b, c, d)		/* nil */
#	define	gsm_debug_longwords(a, b, c, d)		/* nil */
#	define	gsm_debug_word(a, b)			/* nil */
#	define	gsm_debug_longword(a, b)		/* nil */

#else	/* !NDEBUG => DEBUG */

	extern void  gsm_debug_words     P((char * name, int, int, word *));
	extern void  gsm_debug_longwords P((char * name, int, int, longword *));
	extern void  gsm_debug_longword  P((char * name, longword));
	extern void  gsm_debug_word      P((char * name, word));

#endif /* !NDEBUG */

#endif	/* PRIVATE_H */

/**************** end #include "private.h" **********************************/

/********************* #include "gsm.h" ************************************/
#ifndef	GSM_H
#define	GSM_H

#ifdef __cplusplus
#	define	NeedFunctionPrototypes	1
#endif

#if __STDC__
#	define	NeedFunctionPrototypes	1
#endif

#ifdef _NO_PROTO
#	undef	NeedFunctionPrototypes
#endif

#undef	GSM_P	/* gnu stdio.h actually defines this... 	*/

#if NeedFunctionPrototypes
#	define	GSM_P( protos )	protos
#else
#	define  GSM_P( protos )	( /* protos */ )
#endif

/*
 *	Interface
 */

typedef struct gsm_state * 	gsm;
typedef short		   	gsm_signal;		/* signed 16 bit */
typedef unsigned char		gsm_byte;
typedef gsm_byte 		gsm_frame[33];		/* 33 * 8 bits	 */

#define	GSM_MAGIC	0xD			  	/* 13 kbit/s RPE-LTP */

#define	GSM_PATCHLEVEL	6
#define	GSM_MINOR	0
#define	GSM_MAJOR	1

#define	GSM_OPT_VERBOSE	1
#define	GSM_OPT_FAST	2


extern int  gsm_explode GSM_P((gsm, gsm_byte   *, gsm_signal *));
extern void gsm_implode GSM_P((gsm, gsm_signal *, gsm_byte   *));


#endif	/* GSM_H */

/******************* end #include "gsm.h" **********************************/


/************************** #include "config.h" ****************************/
#ifndef	CONFIG_H
#define	CONFIG_H

//#define	SIGHANDLER_T	int  	// signal handlers are void
//#define HAS_SYSV_SIGNAL	1 	// sigs not blocked/reset

#define	HAS_STDLIB_H	1		/* /usr/include/stdlib.h	*/
//#define	HAS_LIMITS_H	1	// /usr/include/limits.h	
#define	HAS_FCNTL_H	1		/* /usr/include/fcntl.h		*/
//#deefine	HAS_ERRNO_DECL	1 	// errno.h declares errno

#define	HAS_FSTAT 	1		/* fstat syscall		*/
#define	HAS_FCHMOD 	1		/* fchmod syscall		*/
#define	HAS_FCHOWN 	1		/* fchown syscall		*/

#define	HAS_STRING_H 	1		/* /usr/include/string.h 	*/
//#define	HAS_STRINGS_H	1	// /usr/include/strings.h 

#define	HAS_UNISTD_H	1		/* /usr/include/unistd.h	*/
#define	HAS_UTIME	1		/* POSIX utime(path, times)	*/
//#define	HAS_UTIMES	1	//use utimes()	syscall instead	
#define	HAS_UTIME_H	1		/* UTIME header file		*/
//#define	HAS_UTIMBUF	1 	//struct utimbuf?
//#define	HAS_UTIMEUSEC   1	//microseconds in utimbuf?

#endif	/* CONFIG_H */
/************************ end #include "config.h" **************************/

/************************ #include "../data/data.h" ************************/
#define SAMPLES 20

gsm_signal pcmdata[]  = {
  (short)0x0000, (short)0x0000, (short)0x0010, (short)0x0010, (short)0x0010, (short)0x0020, (short)0x0020, (short)0x0018,
  (short)0x0028, (short)0x0020, (short)0x0020, (short)0x0028, (short)0x0028, (short)0x0020, (short)0x0030, (short)0x0030,
  (short)0x0028, (short)0x0010, (short)0x0008, (short)0x0000, (short)0x0050, (short)0x0060, (short)0x0058, (short)0x00D0,
  (short)0x00E0, (short)0x00D0, (short)0x0118, (short)0x0128, (short)0x0118, (short)0x0128, (short)0x0110, (short)0x0100,
  (short)0x00A0, (short)0x0058, (short)0x0048, (short)0x0058, (short)0x0060, (short)0x0058, (short)0x0050, (short)0x0048,
  (short)0x0040, (short)0x0030, (short)0x0020, (short)0x0010, (short)0x0008, (short)0xFFF8, (short)0xFFE8, (short)0xFFE0,
  (short)0xFFD8, (short)0xFFC8, (short)0xFFC0, (short)0xFFC0, (short)0xFF98, (short)0xFF78, (short)0xFF78, (short)0xFFC8,
  (short)0x0000, (short)0x0010, (short)0x0040, (short)0x0060, (short)0x0068, (short)0x0078, (short)0x0078, (short)0x0070,
  (short)0x00A8, (short)0x00C8, (short)0x00C8, (short)0x00E0, (short)0x00F0, (short)0x00E8, (short)0x00F8, (short)0x00F8,
  (short)0x00F0, (short)0x00E0, (short)0x00C8, (short)0x00B8, (short)0x00E8, (short)0x0100, (short)0x00F8, (short)0x00E8,
  (short)0x00D8, (short)0x00C0, (short)0x00A8, (short)0x0020, (short)0xFFC0, (short)0xFFA0, (short)0xFFA0, (short)0xFFA8,
  (short)0xFFB0, (short)0xFFD0, (short)0xFFF8, (short)0x0000, (short)0x0020, (short)0x0030, (short)0x0030, (short)0x0030,
  (short)0x0028, (short)0x0020, (short)0xFFF0, (short)0xFFD0, (short)0xFFC8, (short)0xFFC8, (short)0xFFD0, (short)0xFFD8,
  (short)0xFFE8, (short)0xFFF8, (short)0xFFF8, (short)0x0008, (short)0x0018, (short)0x0018, (short)0x0078, (short)0x00B8,
  (short)0x00C0, (short)0x0100, (short)0x0130, (short)0x0128, (short)0x0108, (short)0x00D8, (short)0x00C0, (short)0x0078,
  (short)0x0038, (short)0x0020, (short)0x0020, (short)0x0000, (short)0xFFE0, (short)0xFFE0, (short)0xFFD8, (short)0xFFC8,
  (short)0xFFC8, (short)0xFFA0, (short)0xFF88, (short)0xFF98, (short)0xFF80, (short)0xFF70, (short)0xFF80, (short)0xFF78,
  (short)0xFF78, (short)0xFF90, (short)0xFF80, (short)0xFF78, (short)0xFF78, (short)0xFF50, (short)0xFF30, (short)0xFF50,
  (short)0xFF38, (short)0xFF30, (short)0xFF40, (short)0xFF58, (short)0xFF70, (short)0xFF80, (short)0xFF50, (short)0xFF38,
  (short)0xFF40, (short)0xFF18, (short)0xFF00, (short)0xFF08, (short)0xFF40, (short)0xFF68, (short)0xFF80, (short)0xFF88,
  (short)0xFF88, (short)0xFF88, (short)0xFF88, (short)0xFFB8, (short)0xFFE0, (short)0xFFF0, (short)0xFFD0, (short)0xFFB8,
  (short)0xFFB8, (short)0xFF90, (short)0xFF70, (short)0xFF70, (short)0xFF50, (short)0xFF40, (short)0xFF40, (short)0xFF58,
  (short)0xFF70, (short)0xFF80, (short)0xFFC8, (short)0x0000, (short)0x0018, (short)0x0030, (short)0x0048, (short)0x0048,
  (short)0x0028, (short)0x0008, (short)0xFFF8, (short)0xFFD8, (short)0xFFC8, (short)0xFFB8, (short)0xFF98, (short)0xFF78,
  (short)0xFF70, (short)0xFFF0, (short)0x0058, (short)0x0088, (short)0x00B8, (short)0x00D0, (short)0x00D8, (short)0x00E8,
  (short)0x0138, (short)0x0160, (short)0x0158, (short)0x0170, (short)0x0178, (short)0x0160, (short)0x0168, (short)0x0160,
  (short)0x0140, (short)0x0118, (short)0x00F0, (short)0x00C8, (short)0x0098, (short)0x0078, (short)0x0060, (short)0x0018,
  (short)0xFFC0, (short)0xFF90, (short)0xFF48, (short)0xFF00, (short)0xFEE8, (short)0xFEC8, (short)0xFEB8, (short)0xFEB8,
  (short)0xFEA0, (short)0xFE88, (short)0xFE80, (short)0xFEB8, (short)0xFEF8, (short)0xFF38, (short)0xFFA0, (short)0xFFE8,
  (short)0x0008, (short)0x0030, (short)0x0058, (short)0x0068, (short)0x0068, (short)0x0070, (short)0x0068, (short)0x0050,
  (short)0x0040, (short)0x0040, (short)0x0020, (short)0x0000, (short)0xFFE8, (short)0xFFF0, (short)0xFFF8, (short)0xFFF8,
  (short)0x0038, (short)0x0068, (short)0x0078, (short)0x0038, (short)0x0008, (short)0xFFF0, (short)0xFFE0, (short)0xFFD8,
  (short)0xFFD8, (short)0xFFE0, (short)0xFFD0, (short)0xFFC8, (short)0x0000, (short)0x0030, (short)0x0048, (short)0x0068,
  (short)0x0080, (short)0x0088, (short)0x0088, (short)0x0088, (short)0x0088, (short)0x0088, (short)0x0088, (short)0x0078,
  (short)0x0098, (short)0x00B0, (short)0x00B8, (short)0x0098, (short)0x0070, (short)0x0058, (short)0x0060, (short)0x0078,
  (short)0x00A8, (short)0x00B8, (short)0x00A8, (short)0x00A0, (short)0x0080, (short)0x0068, (short)0x0060, (short)0x0058,
  (short)0x0048, (short)0x0030, (short)0x0038, (short)0x0038, (short)0x0030, (short)0x0050, (short)0x0058, (short)0x0060,
  (short)0x0030, (short)0x0008, (short)0xFFF8, (short)0xFF90, (short)0xFF48, (short)0xFF28, (short)0xFF10, (short)0xFEF8,
  (short)0xFEF0, (short)0xFED8, (short)0xFEB0, (short)0xFEB0, (short)0xFEA8, (short)0xFEB8, (short)0xFED8, (short)0xFEF8,
  (short)0xFF10, (short)0xFF20, (short)0xFF40, (short)0xFF58, (short)0xFF80, (short)0xFFA0, (short)0xFFB8, (short)0xFFC8,
  (short)0xFFD8, (short)0xFFE0, (short)0xFFF0, (short)0x0048, (short)0x0098, (short)0x00B0, (short)0x0068, (short)0x0018,
  (short)0xFFF8, (short)0xFFE8, (short)0xFFF0, (short)0xFFF8, (short)0x0020, (short)0x0038, (short)0x0038, (short)0x0050,
  (short)0x0068, (short)0x0070, (short)0x0068, (short)0x0060, (short)0x0060, (short)0x0038, (short)0x0020, (short)0x0018,
  (short)0x0040, (short)0x0060, (short)0x0068, (short)0x0040, (short)0x0010, (short)0x0000, (short)0xFFB0, (short)0xFF78,
  (short)0xFF70, (short)0xFF90, (short)0xFFA8, (short)0xFFC8, (short)0xFF98, (short)0xFF50, (short)0xFF50, (short)0xFF50,
  (short)0xFF58, (short)0xFF68, (short)0xFF48, (short)0xFF20, (short)0xFF18, (short)0xFF38, (short)0xFF60, (short)0xFF70,
  (short)0xFF80, (short)0xFF98, (short)0xFFA0, (short)0xFFB8, (short)0xFFD0, (short)0xFFE0, (short)0x0018, (short)0x0048,
  (short)0x0058, (short)0x00B0, (short)0x00F8, (short)0x0108, (short)0x0118, (short)0x0120, (short)0x0118, (short)0x0130,
  (short)0x0148, (short)0x0140, (short)0x0130, (short)0x0120, (short)0x0108, (short)0x0098, (short)0x0038, (short)0x0018,
  (short)0xFFD0, (short)0xFF90, (short)0xFF80, (short)0xFF58, (short)0xFF38, (short)0xFF30, (short)0xFF48, (short)0xFF68,
  (short)0xFF78, (short)0xFF88, (short)0xFFB8, (short)0xFFD8, (short)0xFFE8, (short)0xFFD8, (short)0xFFF0, (short)0x0010,
  (short)0x0020, (short)0x0020, (short)0x0018, (short)0x0028, (short)0x0030, (short)0x0030, (short)0x0038, (short)0x0060,
  (short)0x0080, (short)0x0080, (short)0x00B0, (short)0x00D8, (short)0x00D0, (short)0x00B8, (short)0x00A8, (short)0x00A8,
  (short)0x00A0, (short)0x0090, (short)0x0078, (short)0x0070, (short)0x0068, (short)0x0048, (short)0x0018, (short)0x0008,
  (short)0x0008, (short)0x0000, (short)0x0000, (short)0xFFE8, (short)0xFFB0, (short)0xFF90, (short)0xFF88, (short)0xFF70,
  (short)0xFF60, (short)0xFF60, (short)0xFF90, (short)0xFFC0, (short)0xFFD0, (short)0xFFD8, (short)0xFFE0, (short)0xFFE8,
  (short)0x0018, (short)0x0050, (short)0x0058, (short)0x0030, (short)0x0008, (short)0x0000, (short)0x0018, (short)0x0038,
  (short)0x0038, (short)0x0048, (short)0x0050, (short)0x0050, (short)0x0020, (short)0x0000, (short)0xFFF8, (short)0xFFB0,
  (short)0xFF70, (short)0xFF68, (short)0xFFB0, (short)0xFFE8, (short)0xFFF8, (short)0xFFF8, (short)0xFFF8, (short)0xFFF0,
  (short)0x0030, (short)0x0070, (short)0x0090, (short)0x0098, (short)0x0098, (short)0x0090, (short)0x00A0, (short)0x00B0,
  (short)0x00B8, (short)0x00C0, (short)0x00C0, (short)0x00A8, (short)0x0098, (short)0x0088, (short)0x0078, (short)0x0050,
  (short)0x0030, (short)0x0020, (short)0xFFD8, (short)0xFF98, (short)0xFF88, (short)0xFF50, (short)0xFF20, (short)0xFF18,
  (short)0xFEF8, (short)0xFEE0, (short)0xFEE8, (short)0xFE70, (short)0xFE08, (short)0xFE00, (short)0xFE48, (short)0xFE98,
  (short)0xFEB8, (short)0xFEE8, (short)0xFF10, (short)0xFF28, (short)0xFF18, (short)0xFF10, (short)0xFF18, (short)0xFF48,
  (short)0xFF70, (short)0xFF88, (short)0xFFE0, (short)0x0028, (short)0x0040, (short)0x0058, (short)0x0068, (short)0x0070,
  (short)0x0078, (short)0x0070, (short)0x0068, (short)0x0068, (short)0x0078, (short)0x0080, (short)0x0080, (short)0x0088,
  (short)0x0088, (short)0x0080, (short)0x0058, (short)0x0030, (short)0x0020, (short)0x0018, (short)0x0018, (short)0x0018,
  (short)0x0050, (short)0x0090, (short)0x00A0, (short)0x0080, (short)0x0060, (short)0x0050, (short)0x0030, (short)0x0018,
  (short)0x0010, (short)0x0028, (short)0x0038, (short)0x0038, (short)0x0018, (short)0xFFF8, (short)0xFFF0, (short)0x0000,
  (short)0x0020, (short)0x0020, (short)0x0030, (short)0x0030, (short)0x0030, (short)0x0040, (short)0x0050, (short)0x0050,
  (short)0x0050, (short)0x0048, (short)0x0048, (short)0x0048, (short)0x0048, (short)0x0048, (short)0x0078, (short)0x00A0,
  (short)0x00A8, (short)0x00C0, (short)0x00C8, (short)0x00C0, (short)0x00D0, (short)0x00E0, (short)0x00D8, (short)0x00E8,
  (short)0x00F0, (short)0x00E0, (short)0x0100, (short)0x0118, (short)0x0110, (short)0x0100, (short)0x00F0, (short)0x00D8,
  (short)0x0090, (short)0x0048, (short)0x0028, (short)0x0020, (short)0x0020, (short)0x0020, (short)0x0038, (short)0x0050,
  (short)0x0050, (short)0x0050, (short)0x0048, (short)0x0040, (short)0x0050, (short)0x0060, (short)0x0060, (short)0x0040,
  (short)0xFFC0, (short)0xFF58, (short)0xFF40, (short)0xFF90, (short)0xFFE8, (short)0x0000, (short)0x0020, (short)0x0030,
  (short)0x0030, (short)0x0068, (short)0x0098, (short)0x00A8, (short)0x0110, (short)0x0168, (short)0x0170, (short)0x0148,
  (short)0x0118, (short)0x00F0, (short)0x00E8, (short)0x00E0, (short)0x00D0, (short)0x0098, (short)0x0060, (short)0x0040,
  (short)0x0000, (short)0xFFD8, (short)0xFFD8, (short)0xFFC0, (short)0xFFB0, (short)0xFFB0, (short)0xFF78, (short)0xFF30,
  (short)0xFF10, (short)0xFEF0, (short)0xFEE8, (short)0xFEF0, (short)0xFEC8, (short)0xFED0, (short)0xFEF8, (short)0xFF00,
  (short)0xFF10, (short)0xFF20, (short)0xFF50, (short)0xFF78, (short)0xFF90, (short)0xFF80, (short)0xFF70, (short)0xFF70,
  (short)0xFF80, (short)0xFF98, (short)0xFFA0, (short)0xFFB8, (short)0xFFD0, (short)0xFFD8, (short)0xFFF0, (short)0x0000,
  (short)0x0008, (short)0x0028, (short)0x0048, (short)0x0058, (short)0x0078, (short)0x0070, (short)0x0058, (short)0x0068,
  (short)0x0098, (short)0x00B8, (short)0x00D8, (short)0x00F0, (short)0x00F0, (short)0x00E8, (short)0x00F8, (short)0x0100,
  (short)0x00D8, (short)0x00D0, (short)0x00C8, (short)0x00E8, (short)0x0100, (short)0x00F0, (short)0x00E0, (short)0x00C8,
  (short)0x00B8, (short)0x00A0, (short)0x0078, (short)0x0058, (short)0x0038, (short)0x0020, (short)0x0010, (short)0x0010,
  (short)0x0018, (short)0x0010, (short)0x0010, (short)0x0010, (short)0x0018, (short)0x0028, (short)0x0008, (short)0xFFE0,
  (short)0xFFC8, (short)0xFF80, (short)0xFF48, (short)0xFF38, (short)0xFF40, (short)0xFF48, (short)0xFF48, (short)0xFF70,
  (short)0xFF90, (short)0xFFA8, (short)0xFFB8, (short)0xFFC0, (short)0xFFC8, (short)0xFFC0, (short)0xFFC0, (short)0xFFC0,
  (short)0xFFB0, (short)0xFFA0, (short)0xFFA0, (short)0xFFA0, (short)0xFFA8, (short)0xFFB0, (short)0xFF68, (short)0xFF28,
  (short)0xFF08, (short)0xFEF8, (short)0xFEF8, (short)0xFEE8, (short)0xFEE0, (short)0xFED8, (short)0xFEA8, (short)0xFE98,
  (short)0xFEA8, (short)0xFEA8, (short)0xFEA0, (short)0xFEA0, (short)0xFED0, (short)0xFF00, (short)0xFF30, (short)0xFF28,
  (short)0xFF38, (short)0xFF58, (short)0xFF48, (short)0xFF40, (short)0xFF48, (short)0xFFB0, (short)0x0010, (short)0x0038,
  (short)0x0028, (short)0x0010, (short)0x0008, (short)0x0050, (short)0x00A0, (short)0x00B8, (short)0x00A0, (short)0x0080,
  (short)0x0070, (short)0x0090, (short)0x00B0, (short)0x00B0, (short)0x00B8, (short)0x00B8, (short)0x00B0, (short)0x00C0,
  (short)0x00D0, (short)0x00C8, (short)0x00A0, (short)0x0068, (short)0x0038, (short)0xFFF0, (short)0xFFB0, (short)0xFF88,
  (short)0xFF78, (short)0xFF68, (short)0xFF60, (short)0xFF90, (short)0xFFC0, (short)0xFFE0, (short)0x0000, (short)0x0020,
  (short)0x0030, (short)0x00A0, (short)0x0110, (short)0x0138, (short)0x0140, (short)0x0148, (short)0x0148, (short)0x0110,
  (short)0x00E8, (short)0x00C0, (short)0x00A0, (short)0x0088, (short)0x0068, (short)0x0008, (short)0xFFB0, (short)0xFF88,
  (short)0xFF58, (short)0xFF30, (short)0xFF20, (short)0xFEF8, (short)0xFED8, (short)0xFED8, (short)0xFF00, (short)0xFF20,
  (short)0xFF38, (short)0xFF50, (short)0xFF68, (short)0xFF88, (short)0xFFA0, (short)0xFFB8, (short)0x0020, (short)0x0080,
  (short)0x00A0, (short)0x00D8, (short)0x0100, (short)0x0100, (short)0x0138, (short)0x0168, (short)0x0148, (short)0x0128,
  (short)0x0120, (short)0x00F8, (short)0x00E8, (short)0x00E0, (short)0x00C0, (short)0x00A8, (short)0x00B0, (short)0x0098,
  (short)0x0070, (short)0x0048, (short)0x0030, (short)0xFFD0, (short)0xFF60, (short)0xFF48, (short)0xFF10, (short)0xFEA8,
  (short)0xFEA8, (short)0xFEC0, (short)0xFEC0, (short)0xFEE8, (short)0xFEB0, (short)0xFE58, (short)0xFE88, (short)0xFED0,
  (short)0xFEB8, (short)0xFE48, (short)0xFE58, (short)0xFEE8, (short)0xFF28, (short)0xFF18, (short)0xFF60, (short)0x00A0,
  (short)0x01A0, (short)0x0188, (short)0x0178, (short)0x0208, (short)0x0208, (short)0x0100, (short)0x0018, (short)0xFFE0,
  (short)0xFEE0, (short)0xFD68, (short)0xFD00, (short)0xFD60, (short)0xFD70, (short)0xFDA8, (short)0xFF00, (short)0x00A0,
  (short)0x0170, (short)0x0210, (short)0x02D8, (short)0x0310, (short)0x0218, (short)0x00A0, (short)0xFFA0, (short)0xFDF0,
  (short)0xFBD8, (short)0xFB08, (short)0xF9C0, (short)0xF830, (short)0xF8D8, (short)0xFCC0, (short)0x0038, (short)0x01A0,
  (short)0x0380, (short)0x0A18, (short)0x0F50, (short)0x0DB0, (short)0x0C30, (short)0x0E18, (short)0x0CA8, (short)0x0570,
  (short)0xFF98, (short)0xFE38, (short)0xFBA0, (short)0xF700, (short)0xF5D0, (short)0xF7C8, (short)0xF9A8, (short)0xFB48,
  (short)0xFBB0, (short)0xFC78, (short)0xFF00, (short)0xFE98, (short)0xFB20, (short)0xFA48, (short)0xFAC0, (short)0xF8C8,
  (short)0xF6E0, (short)0xF9C0, (short)0xFE08, (short)0xFF80, (short)0x0428, (short)0x0B70, (short)0x0E18, (short)0x0D38,
  (short)0x0D38, (short)0x0C28, (short)0x01D0, (short)0xF578, (short)0xF108, (short)0xFB50, (short)0x0498, (short)0x0428,
  (short)0x0CE8, (short)0x2190, (short)0x29F0, (short)0x22E0, (short)0x1F68, (short)0x2050, (short)0x1810, (short)0x0710,
  (short)0xFA98, (short)0xF438, (short)0xEE68, (short)0xE950, (short)0xEBC8, (short)0xF538, (short)0xFEB8, (short)0x0240,
  (short)0x0460, (short)0x09D0, (short)0x0978, (short)0xFFF8, (short)0xF810, (short)0xF190, (short)0xE8D0, (short)0xE290,
  (short)0xDF60, (short)0xDFF0, (short)0xE668, (short)0xEC20, (short)0xF138, (short)0xFAC0, (short)0x04F0, (short)0x08D0,
  (short)0x08C8, (short)0x0B18, (short)0x09F8, (short)0x0230, (short)0xFA38, (short)0xFA68, (short)0xFC78, (short)0xF9B8,
  (short)0xF850, (short)0xFEA8, (short)0x05B8, (short)0x0690, (short)0x02E8, (short)0x0268, (short)0x0498, (short)0xFCB0,
  (short)0xF018, (short)0xEDF8, (short)0x0090, (short)0x0F48, (short)0x0C70, (short)0x1278, (short)0x27B8, (short)0x2EA0,
  (short)0x21F8, (short)0x1920, (short)0x1918, (short)0x1530, (short)0x0638, (short)0xF858, (short)0xF720, (short)0xF9F8,
  (short)0xF600, (short)0xF850, (short)0x0590, (short)0x0EE0, (short)0x1000, (short)0x10D8, (short)0x1460, (short)0x10F8,
  (short)0x0500, (short)0xFBC0, (short)0xF7A8, (short)0xF250, (short)0xEC00, (short)0xEB30, (short)0xF1C8, (short)0xF920,
  (short)0xFC90, (short)0x0190, (short)0x0A60, (short)0x0E80, (short)0x0DB0, (short)0x0AD8, (short)0x0690, (short)0x0168,
  (short)0xFF20, (short)0xFBD0, (short)0xF6F8, (short)0xF660, (short)0xF680, (short)0xF5B0, (short)0xF7C0, (short)0xF120,
  (short)0xEA90, (short)0xF030, (short)0xEC18, (short)0xE190, (short)0xE558, (short)0xFF20, (short)0x1090, (short)0x0C50,
  (short)0x1248, (short)0x2788, (short)0x2AD0, (short)0x1628, (short)0x08F0, (short)0x0BA8, (short)0x0538, (short)0xEF48,
  (short)0xE410, (short)0xEB10, (short)0xEF68, (short)0xEA28, (short)0xEC40, (short)0xFC18, (short)0x08A8, (short)0x0818,
  (short)0x0778, (short)0x0858, (short)0x02F8, (short)0xF8E8, (short)0xF1F0, (short)0xEF40, (short)0xECD0, (short)0xE958,
  (short)0xEA70, (short)0xF260, (short)0xFAF0, (short)0xFFA0, (short)0x04A0, (short)0x0CF8, (short)0x10F8, (short)0x0EA0,
  (short)0x0D48, (short)0x0BE8, (short)0x05E0, (short)0x03B0, (short)0x0358, (short)0xFF18, (short)0xFB40, (short)0xF9B0,
  (short)0xF9C0, (short)0xF7C0, (short)0xEE90, (short)0xEAA0, (short)0xEE00, (short)0xE888, (short)0xE200, (short)0xEF00,
  (short)0x0948, (short)0x1400, (short)0x1270, (short)0x1D88, (short)0x2CD8, (short)0x2488, (short)0x0DA8, (short)0x04B8,
  (short)0x0548, (short)0xF7B0, (short)0xE3F0, (short)0xE268, (short)0xEFF8, (short)0xF5A0, (short)0xF320, (short)0xFC68,
  (short)0x0BF0, (short)0x0FA0, (short)0x0A50, (short)0x01F8, (short)0xFE60, (short)0xFC48, (short)0xF340, (short)0xEB28,
  (short)0xED58, (short)0xF3C0, (short)0xF5B8, (short)0xF738, (short)0x00F8, (short)0x0C70, (short)0x0E90, (short)0x0DE8,
  (short)0x1190, (short)0x12B0, (short)0x1058, (short)0x0B98, (short)0x0638, (short)0x0868, (short)0x0998, (short)0x02B0,
  (short)0xFE50, (short)0x0120, (short)0x02A0, (short)0xFC90, (short)0xF810, (short)0xF9D0, (short)0xF818, (short)0xF290,
  (short)0xF240, (short)0xF6D0, (short)0x0A48, (short)0x1AD8, (short)0x1840, (short)0x1C18, (short)0x2B18, (short)0x29F0,
  (short)0x1608, (short)0x08B8, (short)0x0778, (short)0x0128, (short)0xF118, (short)0xE868, (short)0xEDA0, (short)0xF310,
  (short)0xF248, (short)0xF558, (short)0x0058, (short)0x0970, (short)0x0688, (short)0x0108, (short)0xFD08, (short)0xF988,
  (short)0xF558, (short)0xF0A0, (short)0xF0B0, (short)0xF540, (short)0xF6E8, (short)0xFCA0, (short)0x0758, (short)0x0CD0,
  (short)0x0F60, (short)0x1338, (short)0x1458, (short)0x1278, (short)0x0FD0, (short)0x0CA8, (short)0x0D50, (short)0x0D10,
  (short)0x0798, (short)0x0398, (short)0x0428, (short)0x04F0, (short)0x0278, (short)0xFF98, (short)0x0178, (short)0x0088,
  (short)0xFB08, (short)0xF660, (short)0xF1A8, (short)0xEF18, (short)0xF9E8, (short)0x0C00, (short)0x11C8, (short)0x1260,
  (short)0x1B60, (short)0x21B0, (short)0x18E0, (short)0x0B08, (short)0x04C8, (short)0x0078, (short)0xF730, (short)0xEF60,
  (short)0xEB18, (short)0xEC10, (short)0xF290, (short)0xF800, (short)0xFB60, (short)0xFF60, (short)0x0080, (short)0xFFA8,
  (short)0xFB08, (short)0xF1A8, (short)0xED10, (short)0xEFF0, (short)0xEED0, (short)0xEB10, (short)0xEFE8, (short)0xF8F0,
  (short)0xFDE0, (short)0x0298, (short)0x0528, (short)0x0598, (short)0x0928, (short)0x0A30, (short)0x0670, (short)0x08E8,
  (short)0x0BC0, (short)0x0698, (short)0x0210, (short)0x0390, (short)0x0560, (short)0x0288, (short)0xF910, (short)0xF468,
  (short)0xF560, (short)0xF3E0, (short)0xEE10, (short)0xE8B0, (short)0xE508, (short)0xEED0, (short)0x03E0, (short)0x0638,
  (short)0xFFA8, (short)0x0BB8, (short)0x2078, (short)0x1FA8, (short)0x0EF0, (short)0x0648, (short)0x05C8, (short)0xFF18,
  (short)0xF588, (short)0xEE20, (short)0xED88, (short)0xF5A0, (short)0xFBA8, (short)0xFBC0, (short)0xFA98, (short)0xFA20,
  (short)0xF7D8, (short)0xF2D0, (short)0xEF48, (short)0xE998, (short)0xE378, (short)0xE530, (short)0xE868, (short)0xE890,
  (short)0xEDD0, (short)0xF798, (short)0xFBC0, (short)0xFD20, (short)0x0178, (short)0x0490, (short)0x04A0, (short)0x0758,
  (short)0x0858, (short)0x0490, (short)0x04F8, (short)0x0858, (short)0x06F0, (short)0x05F8, (short)0x0450, (short)0x0098,
  (short)0xFE60, (short)0xFDA0, (short)0xF9E0, (short)0xF358, (short)0xEDC0, (short)0xF308, (short)0xFFE0, (short)0x0018,
  (short)0xFB80, (short)0x0948, (short)0x1DB8, (short)0x1D08, (short)0x0F88, (short)0x0B48, (short)0x0C50, (short)0x09C0,
  (short)0xFF78, (short)0xF1A0, (short)0xEF28, (short)0xF6B8, (short)0xF9F0, (short)0xF6F0, (short)0xF688, (short)0xF9E0,
  (short)0xF9C0, (short)0xF4C8, (short)0xEBD8, (short)0xE7E8, (short)0xEBE0, (short)0xE8C8, (short)0xE100, (short)0xE518,
  (short)0xF0B8, (short)0xF728, (short)0xF770, (short)0xF878, (short)0xFF58, (short)0x06B0, (short)0x0430, (short)0x0060,
  (short)0x0390, (short)0x0A18, (short)0x0B98, (short)0x06C8, (short)0x0710, (short)0x0CF0, (short)0x08D0, (short)0x01F8,
  (short)0x0280, (short)0x0238, (short)0xFD78, (short)0xF868, (short)0xF198, (short)0xF670, (short)0x0930, (short)0x0A78,
  (short)0xFB38, (short)0x04F0, (short)0x1EB8, (short)0x1E98, (short)0x0F68, (short)0x0EC8, (short)0x1548, (short)0x1480,
  (short)0x0C60, (short)0x00B0, (short)0xFEF8, (short)0x0830, (short)0x0838, (short)0x0160, (short)0x0380, (short)0x07E8,
  (short)0x0270, (short)0xFBA0, (short)0xF9C0, (short)0xF450, (short)0xEE08, (short)0xED08, (short)0xEE10, (short)0xEF20,
  (short)0xF1C0, (short)0xF800, (short)0xFE70, (short)0x00B0, (short)0x02D8, (short)0x07C8, (short)0x09F0, (short)0x09A8,
  (short)0x0A60, (short)0x0B28, (short)0x0C80, (short)0x0D58, (short)0x0BD0, (short)0x0A48, (short)0x0900, (short)0x0768,
  (short)0x03D0, (short)0x00E0, (short)0xFFF8, (short)0xFBD8, (short)0xF5E8, (short)0xFE18, (short)0x0FE8, (short)0x1060,
  (short)0x05C8, (short)0x1078, (short)0x2638, (short)0x2580, (short)0x1740, (short)0x14E8, (short)0x19D0, (short)0x17D8,
  (short)0x0E10, (short)0x0270, (short)0x0120, (short)0x0900, (short)0x0870, (short)0x0290, (short)0x03A0, (short)0x0600,
  (short)0x0100, (short)0xFE28, (short)0xFF28, (short)0xF838, (short)0xF0B8, (short)0xF238, (short)0xF530, (short)0xF440,
  (short)0xF440, (short)0xFA38, (short)0x0198, (short)0x03A8, (short)0x03D0, (short)0x0780, (short)0x0AB8, (short)0x0B58,
  (short)0x0B10, (short)0x0AD8, (short)0x0A08, (short)0x0878, (short)0x07C8, (short)0x0648, (short)0x01A0, (short)0xFF48,
  (short)0xFE58, (short)0xFA68, (short)0xF7D0, (short)0xF758, (short)0xF470, (short)0xF5B0, (short)0x02A8, (short)0x0A58,
  (short)0x0448, (short)0x07C8, (short)0x1708, (short)0x1970, (short)0x0EC8, (short)0x0A40, (short)0x0CD0, (short)0x0D28,
  (short)0x0838, (short)0x0010, (short)0xFAE0, (short)0xFCB0, (short)0xFEB8, (short)0xFCE8, (short)0xFBA8, (short)0xFD10,
  (short)0xFBC8, (short)0xF910, (short)0xF960, (short)0xF830, (short)0xF4D8, (short)0xF500, (short)0xF860, (short)0xF9F0,
  (short)0xFB58, (short)0xFE48, (short)0x0050, (short)0x0418, (short)0x0910, (short)0x0940, (short)0x0830, (short)0x0AC8,
  (short)0x0C88, (short)0x0A50, (short)0x07C0, (short)0x0700, (short)0x0590, (short)0x0268, (short)0xFFF0, (short)0xFBA8,
  (short)0xF720, (short)0xF698, (short)0xF2E0, (short)0xEB68, (short)0xEDA0, (short)0xFC00, (short)0x0358, (short)0xFF30,
  (short)0x0398, (short)0x1220, (short)0x1860, (short)0x1368, (short)0x10C0, (short)0x12F0, (short)0x12A0, (short)0x0E08,
  (short)0x0780, (short)0x0010, (short)0xFAD8, (short)0xF990, (short)0xF7E0, (short)0xF278, (short)0xEE10, (short)0xEB98,
  (short)0xE7A0, (short)0xE6F8, (short)0xEA30, (short)0xE980, (short)0xE420, (short)0xE440, (short)0xEBA8, (short)0xEF98,
  (short)0xEF68, (short)0xF288, (short)0xF7A8, (short)0xFC90, (short)0x01F8, (short)0x0528, (short)0x0630, (short)0x08E8,
  (short)0x0C98, (short)0x0D50, (short)0x0B98, (short)0x0920, (short)0x0678, (short)0x03F0, (short)0x0260, (short)0xFE00,
  (short)0xF810, (short)0xF4B8, (short)0xF0C0, (short)0xEB68, (short)0xEF58, (short)0xFAE8, (short)0xFDE0, (short)0xF680,
  (short)0xF910, (short)0x06E0, (short)0x0C20, (short)0x05D8, (short)0x0408, (short)0x05C8, (short)0x0450, (short)0x02D0,
  (short)0x0128, (short)0xFB78, (short)0xF668, (short)0xF430, (short)0xF150, (short)0xED90, (short)0xE870, (short)0xE448,
  (short)0xE2E0, (short)0xE048, (short)0xDDD0, (short)0xDF08, (short)0xE0E0, (short)0xE098, (short)0xE258, (short)0xE520,
  (short)0xE6A8, (short)0xEA28, (short)0xEF88, (short)0xF2A8, (short)0xF548, (short)0xFBA8, (short)0x01C8, (short)0x03F8,
  (short)0x0748, (short)0x0C88, (short)0x0E98, (short)0x0DB8, (short)0x0D98, (short)0x0D50, (short)0x0B68, (short)0x0970,
  (short)0x06C0, (short)0x0238, (short)0xFE18, (short)0xFB08, (short)0xF820, (short)0xF780, (short)0xF970, (short)0xF9B0,
  (short)0xF880, (short)0xFA28, (short)0x0028, (short)0x0698, (short)0x0948, (short)0x08D0, (short)0x09E0, (short)0x0DD0,
  (short)0x1010, (short)0x0D40, (short)0x0958, (short)0x0728, (short)0x0308, (short)0xFDA0, (short)0xF9F8, (short)0xF418,
  (short)0xEC98, (short)0xE8B8, (short)0xE618, (short)0xE200, (short)0xDED0, (short)0xDF48, (short)0xE100, (short)0xE180,
  (short)0xE160, (short)0xE3C8, (short)0xE898, (short)0xEDD8, (short)0xF250, (short)0xF558, (short)0xFB00, (short)0x02F8,
  (short)0x07B0, (short)0x0B80, (short)0x1108, (short)0x1518, (short)0x1660, (short)0x1770, (short)0x1870, (short)0x16F8,
  (short)0x1300, (short)0x0F78, (short)0x0FC0, (short)0x1070, (short)0x0CE8, (short)0x0AF8, (short)0x0BD8, (short)0x0D28,
  (short)0x10A8, (short)0x1370, (short)0x10A0, (short)0x1040, (short)0x1518, (short)0x1740, (short)0x1550, (short)0x1398,
  (short)0x10E0, (short)0x0AC8, (short)0x0640, (short)0x0348, (short)0xFD18, (short)0xF658, (short)0xF1D8, (short)0xEC20,
  (short)0xE760, (short)0xE550, (short)0xE4B8, (short)0xE418, (short)0xE438, (short)0xE508, (short)0xE738, (short)0xEB18,
  (short)0xF0C8, (short)0xF618, (short)0xF988, (short)0xFEC8, (short)0x0518, (short)0x09D8, (short)0x1118, (short)0x17F0,
  (short)0x1BB0, (short)0x1E28, (short)0x2120, (short)0x23D8, (short)0x2638, (short)0x2418, (short)0x2080, (short)0x1D30,
  (short)0x1CE8, (short)0x1D98, (short)0x1CA8, (short)0x1AD8, (short)0x1960, (short)0x17F8, (short)0x1A40, (short)0x1CF8,
  (short)0x1D38, (short)0x1C30, (short)0x1A68, (short)0x1860, (short)0x1480, (short)0x1020, (short)0x0B68, (short)0x03E8,
  (short)0xFBA8, (short)0xF508, (short)0xEE40, (short)0xE820, (short)0xE338, (short)0xDE88, (short)0xDA30, (short)0xD7D0,
  (short)0xD728, (short)0xD7D8, (short)0xD998, (short)0xDC10, (short)0xDFB0, (short)0xE470, (short)0xE948, (short)0xEF98,
  (short)0xF5F0, (short)0xFC38, (short)0x0228, (short)0x0798, (short)0x0D98, (short)0x1320, (short)0x1760, (short)0x1A70,
  (short)0x1BE0, (short)0x1CC0, (short)0x1D98, (short)0x1A88, (short)0x1658, (short)0x12A0, (short)0x1180, (short)0x10A8,
  (short)0x0ED0, (short)0x0CC8, (short)0x0AD8, (short)0x0920, (short)0x0B70, (short)0x0E30, (short)0x0EE8, (short)0x0D80,
  (short)0x0BE0, (short)0x0AC0, (short)0x09B8, (short)0x0890, (short)0x0690, (short)0x01F8, (short)0xFD30, (short)0xF9F0,
  (short)0xF5B0, (short)0xF188, (short)0xEE38, (short)0xE9E8, (short)0xE5E8, (short)0xE3E0, (short)0xE4A0, (short)0xE608,
  (short)0xE738, (short)0xE858, (short)0xE980, (short)0xEC20, (short)0xF030, (short)0xF450, (short)0xF878, (short)0xFC68,
  (short)0xFF68, (short)0x03C8, (short)0x08B8, (short)0x0D00, (short)0x1038, (short)0x12D8, (short)0x1490, (short)0x1648,
  (short)0x16B8, (short)0x1468, (short)0x1160, (short)0x0FA8, (short)0x1038, (short)0x1058, (short)0x0F88, (short)0x0E50,
  (short)0x0CC8, (short)0x0CC0, (short)0x0FC0, (short)0x1220, (short)0x12A0, (short)0x10F8, (short)0x0F20, (short)0x0D28,
  (short)0x0C78, (short)0x0BB8, (short)0x08D0, (short)0x01C8, (short)0xFB38, (short)0xF660, (short)0xF330, (short)0xF078,
  (short)0xEC28, (short)0xE6C8, (short)0xE2C0, (short)0xE050, (short)0xDFC8, (short)0xE038, (short)0xE160, (short)0xE300,
  (short)0xE568, (short)0xE6B8, (short)0xE9A0, (short)0xED50, (short)0xF238, (short)0xF6D8, (short)0xFB08, (short)0xFF10,
  (short)0x02E8, (short)0x06A0, (short)0x0AC0, (short)0x0DC8, (short)0x1010, (short)0x1168, (short)0x1018, (short)0x0E90,
  (short)0x0BF8, (short)0x0B08, (short)0x0A50, (short)0x09F0, (short)0x0960, (short)0x0888, (short)0x0808, (short)0x09C8,
  (short)0x0BA8, (short)0x0EE8, (short)0x1070, (short)0x10D0, (short)0x0F58, (short)0x0D60, (short)0x0BA0, (short)0x0A60,
  (short)0x08F0, (short)0x0608, (short)0xFFB0, (short)0xF938, (short)0xF360, (short)0xF030, (short)0xEE20, (short)0xEB70,
  (short)0xE7A8, (short)0xE410, (short)0xE140, (short)0xDFC8, (short)0xDFF8, (short)0xE1F0, (short)0xE448, (short)0xE6D0,
  (short)0xE780, (short)0xE9E8, (short)0xECF0, (short)0xF248, (short)0xF730, (short)0xFBC0, (short)0xFF80, (short)0x0310,
  (short)0x0670, (short)0x0A98, (short)0x0D88, (short)0x0FD8, (short)0x10C0, (short)0x0EB0, (short)0x0C48, (short)0x08B8,
  (short)0x0998, (short)0x0AC0, (short)0x0C68, (short)0x0B78, (short)0x09C8, (short)0x0838, (short)0x08F8, (short)0x0A80,
  (short)0x0CA0, (short)0x0E10, (short)0x0E48, (short)0x0D58, (short)0x0A28, (short)0x0750, (short)0x04F0, (short)0x0228,
  (short)0xFEE8, (short)0xFA80, (short)0xF468, (short)0xEED0, (short)0xEAE0, (short)0xE8B8, (short)0xE718, (short)0xE5B0,
  (short)0xE4A8, (short)0xE410, (short)0xE480, (short)0xE548, (short)0xE738, (short)0xE9B0, (short)0xED80, (short)0xF0B8,
  (short)0xF480, (short)0xF7B0, (short)0xFB70, (short)0xFED0, (short)0x0328, (short)0x0720, (short)0x0A98, (short)0x0E00,
  (short)0x10F8, (short)0x12E0, (short)0x12A8, (short)0x11B0, (short)0x0F58, (short)0x0F38, (short)0x0E88, (short)0x0F08,
  (short)0x0FC0, (short)0x0FF0, (short)0x10B8, (short)0x1138, (short)0x1198, (short)0x13D0, (short)0x1638, (short)0x17E8,
  (short)0x1758, (short)0x1628, (short)0x1460, (short)0x10E8, (short)0x0CA0, (short)0x0848, (short)0x0280, (short)0xFC90,
  (short)0xF700, (short)0xF0F8, (short)0xEB18, (short)0xE638, (short)0xE1B8, (short)0xDE78, (short)0xDC58, (short)0xDBB8,
  (short)0xDC28, (short)0xDDB0, (short)0xE030, (short)0xE330, (short)0xE6F0, (short)0xEC20, (short)0xF210, (short)0xF7C0,
  (short)0xFCE0, (short)0x0150, (short)0x0570, (short)0x08F0, (short)0x0C70, (short)0x0F50, (short)0x12B8, (short)0x1560,
  (short)0x16E0, (short)0x1630, (short)0x14E8, (short)0x1298, (short)0x11B8, (short)0x1170, (short)0x11B8, (short)0x11C0,
  (short)0x0FE8, (short)0x0E58, (short)0x0CB8, (short)0x0C50, (short)0x0D68, (short)0x0E98, (short)0x0E30, (short)0x0C28,
  (short)0x0A10, (short)0x06D8, (short)0x02E0, (short)0xFEA0, (short)0xFA18, (short)0xF4E8, (short)0xF018, (short)0xEB68,
  (short)0xE6E8, (short)0xE310, (short)0xDFC8, (short)0xDD38, (short)0xDBF8, (short)0xDC38, (short)0xDDD0, (short)0xE070,
  (short)0xE390, (short)0xE760, (short)0xEB88, (short)0xEF20, (short)0xF378, (short)0xF830, (short)0xFCE0, (short)0x00F8,
  (short)0x0480, (short)0x0768, (short)0x0968, (short)0x0AE0, (short)0x0BB8, (short)0x0C10, (short)0x0BB0, (short)0x0A78,
  (short)0x08E0, (short)0x06E8, (short)0x0540, (short)0x0870, (short)0x0BE0, (short)0x0ED0, (short)0x0E40, (short)0x0D10,
  (short)0x0CC8, (short)0x0E28, (short)0x0FA0, (short)0x0FB0, (short)0x0F18, (short)0x0DD0, (short)0x0BC8, (short)0x08E8,
  (short)0x0628, (short)0x0300, (short)0xFF18, (short)0xFB40, (short)0xF780, (short)0xF388, (short)0xF028, (short)0xED80,
  (short)0xEB18, (short)0xE968, (short)0xE8C0, (short)0xE738, (short)0xE658, (short)0xE698, (short)0xE888, (short)0xEB38,
  (short)0xEDA0, (short)0xF178, (short)0xF5B8, (short)0xFA28, (short)0xFEA8, (short)0x0300, (short)0x06C8, (short)0x0960,
  (short)0x0B70, (short)0x0CE0, (short)0x0D70, (short)0x0D50, (short)0x0C60, (short)0x0890, (short)0x0418, (short)0x0028,
  (short)0x01D0, (short)0x03F8, (short)0x05A8, (short)0x0700, (short)0x0808, (short)0x09A0, (short)0x0B18, (short)0x0CC8,
  (short)0x0D90, (short)0x0E68, (short)0x0EC0, (short)0x0E30, (short)0x0C28, (short)0x09D8, (short)0x0730, (short)0x0308,
  (short)0xFED8, (short)0xFAC0, (short)0xF598, (short)0xF0D8, (short)0xECE0, (short)0xEAA8, (short)0xE948, (short)0xE8D0,
  (short)0xE850, (short)0xE888, (short)0xE910, (short)0xEAD0, (short)0xED68, (short)0xF018, (short)0xF350, (short)0xF6B8,
  (short)0xFAE0, (short)0xFF00, (short)0x02D8, (short)0x05E8, (short)0x0830, (short)0x09F8, (short)0x0B08, (short)0x0B80,
  (short)0x0B60, (short)0x0988, (short)0x0648, (short)0x02D0, (short)0x0150, (short)0x01E8, (short)0x0270, (short)0x03E0,
  (short)0x0538, (short)0x0658, (short)0x0918, (short)0x0C00, (short)0x0E88, (short)0x10B8, (short)0x12A0, (short)0x13E0,
  (short)0x1488, (short)0x1448, (short)0x1368, (short)0x1120, (short)0x0DD0, (short)0x0A40, (short)0x0608, (short)0x0148,
  (short)0xFC80, (short)0xF860, (short)0xF4D8, (short)0xF1C0, (short)0xF008, (short)0xEF38, (short)0xEE78, (short)0xEE98,
  (short)0xEF90, (short)0xF170, (short)0xF390, (short)0xF5C0, (short)0xF888, (short)0xFB48, (short)0xFDF0, (short)0x0078,
  (short)0x03D0, (short)0x06C8, (short)0x08F8, (short)0x0AA0, (short)0x0BC8, (short)0x0C48, (short)0x0B30, (short)0x0978,
  (short)0x06A8, (short)0x0530, (short)0x03F0, (short)0x0438, (short)0x03C0, (short)0x0350, (short)0x0360, (short)0x04E8,
  (short)0x0698, (short)0x07D0, (short)0x08D0, (short)0x0998, (short)0x0A70, (short)0x0B48, (short)0x0B70, (short)0x0AD0,
  (short)0x09C0, (short)0x0890, (short)0x0730, (short)0x0588, (short)0x0358, (short)0x0140, (short)0xFF58, (short)0xFD40,
  (short)0xFB68, (short)0xF9E8, (short)0xF828, (short)0xF6D0, (short)0xF608, (short)0xF5D8, (short)0xF610, (short)0xF668,
  (short)0xF778, (short)0xF8E8, (short)0xFA48, (short)0xFCC8, (short)0xFF50, (short)0x01C8, (short)0x0428, (short)0x0640,
  (short)0x07D0, (short)0x09D0, (short)0x0B40, (short)0x0BF8, (short)0x0C30, (short)0x0C08, (short)0x0B08, (short)0x0988,
  (short)0x07C0, (short)0x0670, (short)0x0608, (short)0x0590, (short)0x0588, (short)0x05B0, (short)0x05E0, (short)0x06B8,
  (short)0x0748, (short)0x0758, (short)0x0700, (short)0x06A8, (short)0x0620, (short)0x05D8, (short)0x0590, (short)0x0528,
  (short)0x03A8, (short)0x0240, (short)0x0108, (short)0xFF38, (short)0xFD50, (short)0xFBA0, (short)0xFA38, (short)0xF920,
  (short)0xF860, (short)0xF6E8, (short)0xF640, (short)0xF628, (short)0xF680, (short)0xF720, (short)0xF800, (short)0xF8E0,
  (short)0xF9A0, (short)0xFA78, (short)0xFB88, (short)0xFD20, (short)0xFEA0, (short)0x0008, (short)0x0110, (short)0x0200,
  (short)0x0360, (short)0x04E0, (short)0x0608, (short)0x0738, (short)0x0838, (short)0x08D8, (short)0x0828, (short)0x0738,
  (short)0x0600, (short)0x04A8, (short)0x02E0, (short)0x0130, (short)0xFFA0, (short)0xFF48, (short)0xFF10, (short)0xFEF0,
  (short)0xFF30, (short)0xFFD0, (short)0x0090, (short)0x0090, (short)0x0070, (short)0x0060, (short)0xFFE8, (short)0xFF50,
  (short)0xFEB8, (short)0xFE98, (short)0xFE88, (short)0xFE80, (short)0xFE58, (short)0xFE50, (short)0xFE58, (short)0xFDB0,
  (short)0xFD08, (short)0xFC80, (short)0xFAF8, (short)0xF988, (short)0xF860, (short)0xF798, (short)0xF720, (short)0xF6E8,
  (short)0xF728, (short)0xF7C0, (short)0xF8A8, (short)0xF8F8, (short)0xF960, (short)0xFA18, (short)0xFAC0, (short)0xFB58,
  (short)0xFC18, (short)0xFCE0, (short)0xFDA0, (short)0xFE20, (short)0xFE88, (short)0xFEF8, (short)0xFEF0, (short)0xFEC8,
  (short)0xFEA8, (short)0xFDE0, (short)0xFD10, (short)0xFC70, (short)0xFBA8, (short)0xFB10, (short)0xFAB8, (short)0xFAA0,
  (short)0xFAD0, (short)0xFB18, (short)0xFA90, (short)0xFA18, (short)0xFA10, (short)0xFA80, (short)0xFB10, (short)0xFB88,
  (short)0xFC90, (short)0xFDB8, (short)0xFEB8, (short)0xFF80, (short)0x0058, (short)0x0138, (short)0x0118, (short)0x00C8,
  (short)0x00C0, (short)0xFF98, (short)0xFE30, (short)0xFD38, (short)0xFC68, (short)0xFB78, (short)0xFAB8, (short)0xFAE8,
  (short)0xFB78, (short)0xFBD0, (short)0xFBE8, (short)0xFC18, (short)0xFC98, (short)0xFD28, (short)0xFD48, (short)0xFD68,
  (short)0xFD68, (short)0xFD90, (short)0xFDB8, (short)0xFD90, (short)0xFD68, (short)0xFD78, (short)0xFCA0, (short)0xFB70,
  (short)0xFAD0, (short)0xF9F0, (short)0xF870, (short)0xF748, (short)0xF748, (short)0xF770, (short)0xF748, (short)0xF720,
  (short)0xF7A8, (short)0xF878, (short)0xF930, (short)0xF998, (short)0xFA38, (short)0xFC10, (short)0xFDA0, (short)0xFE70,
  (short)0x0030, (short)0x0248, (short)0x03A0, (short)0x0568, (short)0x0738, (short)0x0870, (short)0x0960, (short)0x0A10,
  (short)0x0A40, (short)0x0A28, (short)0x09B8, (short)0x08E8, (short)0x07E8, (short)0x06E0, (short)0x0588, (short)0x0430,
  (short)0x0300, (short)0x0260, (short)0x01D0, (short)0x0118, (short)0xFFB0, (short)0xFE98, (short)0xFE18, (short)0xFDA0,
  (short)0xFD08, (short)0xFCB8, (short)0xFCF8, (short)0xFD60, (short)0xFD90, (short)0xFD90, (short)0xFDD8, (short)0xFE50,
  (short)0xFDA0, (short)0xFCE0, (short)0xFCC0, (short)0xFCE8, (short)0xFCB0, (short)0xFC60, (short)0xFC70, (short)0xFCB8,
  (short)0xFCE0, (short)0xFD40, (short)0xFDD8, (short)0xFE68, (short)0xFF78, (short)0x0068, (short)0x0108, (short)0x0278,
  (short)0x03A0, (short)0x0420, (short)0x0590, (short)0x0708, (short)0x07B8, (short)0x07D8, (short)0x0808, (short)0x0838,
  (short)0x07D8, (short)0x06E8, (short)0x0600, (short)0x05B0, (short)0x0518, (short)0x0410, (short)0x02A0, (short)0x0198,
  (short)0x00D0, (short)0x00C8, (short)0x00B0, (short)0x0068, (short)0x00C0, (short)0x0150, (short)0x0180, (short)0x0220,
  (short)0x02D8, (short)0x0340, (short)0x0360, (short)0x0380, (short)0x0380, (short)0x0338, (short)0x02C8, (short)0x02B8,
  (short)0x0280, (short)0x0200, (short)0x0100, (short)0x0098, (short)0x0080, (short)0x0020, (short)0xFFF0, (short)0x0000,
  (short)0x0020, (short)0x0098, (short)0x0120, (short)0x0170, (short)0x0230, (short)0x02F0, (short)0x0350, (short)0x0480,
  (short)0x05B8, (short)0x0650, (short)0x06A8, (short)0x0738, (short)0x0798, (short)0x07B0, (short)0x07C0, (short)0x0798,
  (short)0x0668, (short)0x0598, (short)0x0530, (short)0x04C8, (short)0x0410, (short)0x0350, (short)0x0278, (short)0x01D8,
  (short)0x0148, (short)0x0080, (short)0x0000, (short)0xFFC0, (short)0xFFD8, (short)0xFFA8, (short)0xFF60, (short)0xFF80,
  (short)0x0018, (short)0x0070, (short)0xFFE0, (short)0xFF88, (short)0xFFC0, (short)0xFF38, (short)0xFE98, (short)0xFE50,
  (short)0xFE10, (short)0xFDD8, (short)0xFD90, (short)0xFD30, (short)0xFDB8, (short)0xFE68, (short)0xFE70, (short)0xFE60,
  (short)0xFE70, (short)0xFED0, (short)0xFF90, (short)0xFFE0, (short)0xFFF0, (short)0x00A8, (short)0x0168, (short)0x01D0,
  (short)0x01F8, (short)0x0210, (short)0x0278, (short)0x0268, (short)0x0208, (short)0x0220, (short)0x01F8, (short)0x0198,
  (short)0x0158, (short)0x0100, (short)0x00C0, (short)0x00A0, (short)0x0018, (short)0xFF98, (short)0xFF28, (short)0xFEC0,
  (short)0xFE80, (short)0xFE60, (short)0xFD88, (short)0xFCF0, (short)0xFCC8, (short)0xFC70, (short)0xFC10, (short)0xFBC8,
  (short)0xFBB0, (short)0xFBE8, (short)0xFBE8, (short)0xFB80, (short)0xFB88, (short)0xFB40, (short)0xFB18, (short)0xFB20,
  (short)0xFAB8, (short)0xFA50, (short)0xFA50, (short)0xFAB8, (short)0xFAF8, (short)0xFB18, (short)0xFBB0, (short)0xFC88,
  (short)0xFD10, (short)0xFD40, (short)0xFD98, (short)0xFE38, (short)0xFEE0, (short)0xFEF8, (short)0xFEF0, (short)0xFF18,
  (short)0xFF18, (short)0xFF18, (short)0xFF68, (short)0xFF98, (short)0xFF98, (short)0xFFD0, (short)0xFFF8, (short)0x0048,
  (short)0x0038, (short)0x0008, (short)0x0008, (short)0xFFE0, (short)0xFFB0, (short)0xFFB8, (short)0xFED0, (short)0xFE18,
  (short)0xFE18, (short)0xFDF0, (short)0xFE38, (short)0xFE90, (short)0xFE90, (short)0xFDA8, (short)0xFD48, (short)0xFD70,
  (short)0xFD68, (short)0xFD00, (short)0xFCB8, (short)0xFCB8, (short)0xFCF8, (short)0xFD00, (short)0xFC30, (short)0xFBD0,
  (short)0xFC10, (short)0xFC20, (short)0xFBE0, (short)0xFBA8, (short)0xFC30, (short)0xFD00, (short)0xFD50, (short)0xFD90,
  (short)0xFE10, (short)0xFEA8, (short)0xFF40, (short)0xFFA0, (short)0xFFD0, (short)0xFFC8, (short)0xFFC8, (short)0xFFD8,
  (short)0xFFA0, (short)0xFF98, (short)0xFFB8, (short)0x0050, (short)0x00B8, (short)0x00B0, (short)0x01B0, (short)0x02E0,
  (short)0x0318, (short)0x0330, (short)0x02E0, (short)0x02C8, (short)0x0278, (short)0x0150, (short)0x0050, (short)0xFFC0,
  (short)0xFF88, (short)0xFF18, (short)0xFE90, (short)0xFE40, (short)0xFE30, (short)0xFDE8, (short)0xFDD0, (short)0xFD70,
  (short)0xFD48, (short)0xFD10, (short)0xFC98, (short)0xFC38, (short)0xFC38, (short)0xFC78, (short)0xFC98, (short)0xFCF0,
  (short)0xFDA8, (short)0xFE48, (short)0xFEC8, (short)0xFF30, (short)0xFF98, (short)0x0000, (short)0x0050, (short)0x0058,
  (short)0x00A8, (short)0x00E8, (short)0x00D0, (short)0x0138, (short)0x01E0, (short)0x0218, (short)0x0208, (short)0x0230,
  (short)0x0258, (short)0x0248, (short)0x02B0, (short)0x0318, (short)0x0330, (short)0x0358, (short)0x0380, (short)0x0378,
  (short)0x0408, (short)0x0480, (short)0x0460, (short)0x03C8, (short)0x0318, (short)0x02B0, (short)0x01E8, (short)0x00B8,
  (short)0xFFD8, (short)0xFF30, (short)0xFEC8, (short)0xFE60, (short)0xFE60, (short)0xFE78, (short)0xFE78, (short)0xFDC0,
  (short)0xFD70, (short)0xFD50, (short)0xFD08, (short)0xFC88, (short)0xFC28, (short)0xFC98, (short)0xFD18, (short)0xFD60,
  (short)0xFD60, (short)0xFDD8, (short)0xFE90, (short)0xFEE8, (short)0xFF10, (short)0xFF58, (short)0xFF90, (short)0xFFB8,
  (short)0xFFE0, (short)0xFFF0, (short)0xFFF0, (short)0x00D0, (short)0x0190, (short)0x01C8, (short)0x0180, (short)0x0188,
  (short)0x01B0, (short)0x0238, (short)0x0298, (short)0x02B8, (short)0x0268, (short)0x0258, (short)0x0258, (short)0x0230,
  (short)0x0228, (short)0x0230, (short)0x0258, (short)0x0248, (short)0x01F8, (short)0x0150, (short)0x00C8, (short)0x0058,
  (short)0x0058, (short)0x0038, (short)0x0000, (short)0xFF50, (short)0xFF00, (short)0xFEF8, (short)0xFE80, (short)0xFDB8,
  (short)0xFD70, (short)0xFD00, (short)0xFC90, (short)0xFC40, (short)0xFC28, (short)0xFC58, (short)0xFC98, (short)0xFD10,
  (short)0xFD78, (short)0xFDE0, (short)0xFE80, (short)0xFF08, (short)0xFF60, (short)0xFFD0, (short)0x0030, (short)0x0068,
  (short)0x0110, (short)0x0198, (short)0x01C0, (short)0x0208, (short)0x0260, (short)0x0280, (short)0x0320, (short)0x0390,
  (short)0x0398, (short)0x0410, (short)0x0488, (short)0x04A0, (short)0x0448, (short)0x0408, (short)0x03E0, (short)0x03C8,
  (short)0x0398, (short)0x0350, (short)0x0308, (short)0x02C8, (short)0x0278, (short)0x01D8, (short)0x0148, (short)0x00E8,
  (short)0x0040, (short)0xFFA0, (short)0xFF50, (short)0xFDC0, (short)0xFC88, (short)0xFC30, (short)0xFB88, (short)0xFAA8,
  (short)0xFA50, (short)0xFA30, (short)0xFA40, (short)0xFA70, (short)0xFAB8, (short)0xFAE0, (short)0xFB28, (short)0xFB58,
  (short)0xFB80, (short)0xFBB0, (short)0xFC00, (short)0xFC80, (short)0xFCF0, (short)0xFDB8, (short)0xFE58, (short)0xFED8,
  (short)0x0008, (short)0x0100, (short)0x0180, (short)0x01D0, (short)0x0210, (short)0x0248, (short)0x0238, (short)0x0200,
  (short)0x01D0, (short)0x02D0, (short)0x03A0, (short)0x03D8, (short)0x03C0, (short)0x03D8, (short)0x03F8, (short)0x0370,
  (short)0x02C0, (short)0x0258, (short)0x01B8, (short)0x0120, (short)0x0090, (short)0x0088, (short)0x00A8, (short)0x00A8,
  (short)0x0088, (short)0x0068, (short)0x0060, (short)0xFFE0, (short)0xFF00, (short)0xFE50, (short)0xFDC8, (short)0xFCF0,
  (short)0xFC30, (short)0xFBB0, (short)0xFBD8, (short)0xFC20, (short)0xFC58, (short)0xFC30, (short)0xFC40, (short)0xFC78,
  (short)0xFCC0, (short)0xFCE8, (short)0xFD10, (short)0xFD48, (short)0xFD88, (short)0xFDE8, (short)0xFF10, (short)0x0020,
  (short)0x0110, (short)0x01B8, (short)0x0248, (short)0x02C0, (short)0x0358, (short)0x03B8, (short)0x03C8, (short)0x0320,
  (short)0x0288, (short)0x0280, (short)0x0300, (short)0x0340, (short)0x0320, (short)0x0380, (short)0x03F8, (short)0x0418,
  (short)0x0378, (short)0x02E0, (short)0x0288, (short)0x0280, (short)0x0238, (short)0x01D0, (short)0x0168, (short)0x0138,
  (short)0x0110, (short)0x0140, (short)0x0148, (short)0x0150, (short)0x00A8, (short)0x0010, (short)0xFFB0, (short)0xFEB8,
  (short)0xFDE0, (short)0xFD48, (short)0xFCE8, (short)0xFCA8, (short)0xFC78, (short)0xFC48, (short)0xFC50, (short)0xFC70,
  (short)0xFCA8, (short)0xFCE8, (short)0xFD28, (short)0xFDD0, (short)0xFE70, (short)0xFED8, (short)0x0040, (short)0x0188,
  (short)0x0258, (short)0x03C0, (short)0x04F0, (short)0x05B8, (short)0x0638, (short)0x0670, (short)0x0690, (short)0x0708,
  (short)0x0708, (short)0x06B8, (short)0x0660, (short)0x0650, (short)0x0630, (short)0x05C8, (short)0x0578, (short)0x0548,
  (short)0x0508, (short)0x0470, (short)0x03D0, (short)0x0350, (short)0x0278, (short)0x01A0, (short)0x00F8, (short)0x00B0,
  (short)0x0078, (short)0x0030, (short)0xFFE8, (short)0xFFC8, (short)0xFFB8, (short)0xFED0, (short)0xFE08, (short)0xFD98,
  (short)0xFC70, (short)0xFB60, (short)0xFAA8, (short)0xFA10, (short)0xF9B8, (short)0xF980, (short)0xF9A0, (short)0xFA00,
  (short)0xFA68, (short)0xFB90, (short)0xFCB8, (short)0xFD98, (short)0xFE68, (short)0xFF18, (short)0xFFC0, (short)0x0078,
  (short)0x00F8, (short)0x0218, (short)0x0320, (short)0x03C0, (short)0x0478, (short)0x0510, (short)0x0570, (short)0x05D8,
  (short)0x05E0, (short)0x05B8, (short)0x0508, (short)0x0468, (short)0x03E0, (short)0x02F0, (short)0x0218, (short)0x0168,
  (short)0x00F0, (short)0x0060, (short)0xFFD0, (short)0xFF58, (short)0xFEC0, (short)0xFE48, (short)0xFDB0, (short)0xFD58,
  (short)0xFD38, (short)0xFCD8, (short)0xFC80, (short)0xFC50, (short)0xFC08, (short)0xFB48, (short)0xFA98, (short)0xF9F8,
  (short)0xF8F8, (short)0xF810, (short)0xF7F8, (short)0xF818, (short)0xF848, (short)0xF8E8, (short)0xF9E0, (short)0xFB08,
  (short)0xFC38, (short)0xFD10, (short)0xFDE8, (short)0xFF10, (short)0xFFD0, (short)0x0048, (short)0x00E0, (short)0x0160,
  (short)0x01B8, (short)0x01C8, (short)0x01E0, (short)0x0200, (short)0x0228, (short)0x0240, (short)0x0240, (short)0x0240,
  (short)0x0260, (short)0x0280, (short)0x0280, (short)0x02F0, (short)0x0370, (short)0x03C8, (short)0x03C8, (short)0x03A8,
  (short)0x03A0, (short)0x02F8, (short)0x0220, (short)0x0150, (short)0x0098, (short)0xFFE0, (short)0xFF20, (short)0xFEA0,
  (short)0xFE50, (short)0xFE18, (short)0xFD38, (short)0xFC60, (short)0xFBE0, (short)0xFAC8, (short)0xF9A0, (short)0xF8B8,
  (short)0xF830, (short)0xF888, (short)0xF8B8, (short)0xF908, (short)0xFA80, (short)0xFBF8, (short)0xFD48, (short)0xFEC8,
  (short)0x0040, (short)0x01B0, (short)0x0298, (short)0x0338, (short)0x03C0, (short)0x0470, (short)0x0520, (short)0x0588,
  (short)0x0610, (short)0x0688, (short)0x06C8, (short)0x0670, (short)0x05E8, (short)0x0578, (short)0x0580, (short)0x0578,
  (short)0x0528, (short)0x0498, (short)0x0408, (short)0x0390, (short)0x03F8, (short)0x0458, (short)0x0488, (short)0x0468,
  (short)0x0450, (short)0x0458, (short)0x03A8, (short)0x02D0, (short)0x0210, (short)0x0158, (short)0x0088, (short)0xFFA8,
  (short)0xFF00, (short)0xFE88, (short)0xFE30, (short)0xFD88, (short)0xFCB8, (short)0xFC28, (short)0xFB30, (short)0xF9F0,
  (short)0xF8E8, (short)0xF890, (short)0xF890, (short)0xF8C0, (short)0xF978, (short)0xFA78, (short)0xFBE8, (short)0xFD20,
  (short)0xFE28, (short)0xFF60, (short)0x00D8, (short)0x0220, (short)0x02F8, (short)0x0378, (short)0x03E0, (short)0x0438,
  (short)0x0488, (short)0x0498, (short)0x04A8, (short)0x0480, (short)0x0440, (short)0x03C0, (short)0x02D8, (short)0x01E8,
  (short)0x0140, (short)0x00D8, (short)0x0068, (short)0xFFE0, (short)0x0068, (short)0x0130, (short)0x0228, (short)0x0260,
  (short)0x0278, (short)0x02D0, (short)0x02D8, (short)0x0290, (short)0x01E0, (short)0x00D0, (short)0xFFE0, (short)0xFEF8,
  (short)0xFE08, (short)0xFD28, (short)0xFC88, (short)0xFBE0, (short)0xFB60, (short)0xFAD8, (short)0xFA08, (short)0xF978,
  (short)0xF8E8, (short)0xF8B0, (short)0xF8B0, (short)0xF8D0, (short)0xF9D0, (short)0xFAF8, (short)0xFC18, (short)0xFDB0,
  (short)0xFF38, (short)0x00A0, (short)0x01F8, (short)0x02F8, (short)0x03C0, (short)0x0460, (short)0x04B8, (short)0x04C8,
  (short)0x04C8, (short)0x04C0, (short)0x0498, (short)0x0490, (short)0x0478, (short)0x0448, (short)0x0420, (short)0x03F8,
  (short)0x0328, (short)0x0238, (short)0x01B0, (short)0x0170, (short)0x0128, (short)0x0090, (short)0x00E8, (short)0x01B8,
  (short)0x02B8, (short)0x0280, (short)0x0218, (short)0x0218, (short)0x01F0, (short)0x0148, (short)0x0000, (short)0xFEC0,
  (short)0xFE08, (short)0xFD70, (short)0xFCA0, (short)0xFBF0, (short)0xFBC0, (short)0xFBA0, (short)0xFB80, (short)0xFB18,
  (short)0xFB28, (short)0xFB98, (short)0xFBC0, (short)0xFBD0, (short)0xFC08, (short)0xFC78, (short)0xFDC8, (short)0xFEC8,
  (short)0xFF78, (short)0x00D0, (short)0x0238, (short)0x0360, (short)0x0398, (short)0x0360, (short)0x0368, (short)0x0380,
  (short)0x0318, (short)0x0250, (short)0x0208, (short)0x0220, (short)0x0218, (short)0x01F0, (short)0x01C8, (short)0x0210,
  (short)0x0270, (short)0x0270, (short)0x0240, (short)0x0290, (short)0x0310, (short)0x0360, (short)0x0340, (short)0x0310,
  (short)0x0318, (short)0x0320, (short)0x02D8, (short)0x0240, (short)0x0158, (short)0x00A0, (short)0x0008, (short)0xFF30,
  (short)0xFE50, (short)0xFDA8, (short)0xFD28, (short)0xFCC8, (short)0xFC60, (short)0xFBA8, (short)0xFB40, (short)0xFB10,
  (short)0xFB18, (short)0xFB28, (short)0xFB48, (short)0xFB68, (short)0xFBA8, (short)0xFBF8, (short)0xFCB8, (short)0xFD78,
  (short)0xFE00, (short)0xFE88, (short)0xFF30, (short)0xFF98, (short)0xFFC8, (short)0xFFE8, (short)0x0050, (short)0x00B0,
  (short)0x00E0, (short)0x0040, (short)0xFF68, (short)0xFED8, (short)0xFEE8, (short)0xFEE0, (short)0xFE90, (short)0xFEA8,
  (short)0xFF88, (short)0x0080, (short)0x0188, (short)0x0208, (short)0x0290, (short)0x0390, (short)0x0438, (short)0x0450,
  (short)0x0428, (short)0x03F8, (short)0x03E0, (short)0x0388, (short)0x02E0, (short)0x0240, (short)0x0190, (short)0x00D0,
  (short)0x0000, (short)0x0000, (short)0x0018, (short)0x00FF, (short)0x0068, (short)0x00FE, (short)0x00F8, (short)0x00FD
};

gsm_byte gsmdata[]  = {
  0xD5, 0x1F, 0x74, 0x21, 0xA0, 0x50, 0x40, 0xC9, 0x24, 0x7B, 0xFA, 0x6B, 0x52, 0xE0, 0xB6, 0xD6,
  0x8E, 0xB9, 0x2B, 0xAE, 0xE0, 0x8B, 0x23, 0x52, 0x3B, 0x13, 0x86, 0xE0, 0x14, 0x4A, 0x41, 0x44,
  0x32, 0xD3, 0xA1, 0x83, 0xA1, 0x1D, 0xA6, 0x80, 0xBA, 0xD2, 0x96, 0x26, 0xFB, 0x84, 0x80, 0x6D,
  0x9C, 0x25, 0x1D, 0x9B, 0xAA, 0xC0, 0xBB, 0x4C, 0x95, 0xB9, 0x53, 0xAE, 0xA0, 0xB6, 0xE4, 0x46,
  0x37, 0x1B, 0xD4, 0xA5, 0x7B, 0x1D, 0x22, 0x97, 0x00, 0xBA, 0xA5, 0x6D, 0xD2, 0xA1, 0x7E, 0xC0,
  0xB9, 0x25, 0xD2, 0xB4, 0x94, 0x9E, 0xE0, 0x3E, 0xDE, 0xED, 0xD6, 0xD2, 0xE2, 0xC0, 0xD7, 0x5D,
  0x8D, 0x59, 0xAC, 0xD3, 0xE4, 0x83, 0x95, 0x59, 0xC0, 0xA1, 0x48, 0xD2, 0x66, 0xC7, 0x2C, 0x9E,
  0xA0, 0x2A, 0xD3, 0xEE, 0x45, 0x1C, 0x80, 0xE0, 0x6B, 0x34, 0x8C, 0x4B, 0x29, 0xCB, 0x00, 0xBA,
  0xF6, 0x0D, 0x26, 0x9A, 0xD3, 0xA4, 0x82, 0x9D, 0x63, 0x7A, 0xC0, 0x67, 0x24, 0xBA, 0xD6, 0x7C,
  0xC2, 0xC0, 0x37, 0x20, 0x4F, 0x10, 0xE0, 0xC7, 0x80, 0x6A, 0x77, 0x63, 0xBE, 0x6B, 0x5A, 0xC0,
  0xB5, 0x34, 0xD1, 0x34, 0x9C, 0xD4, 0xE8, 0x56, 0xB2, 0x58, 0x5F, 0x00, 0xB7, 0xAF, 0x92, 0x12,
  0x90, 0xD5, 0xA4, 0x39, 0x23, 0x4E, 0x46, 0x87, 0x51, 0xAC, 0xD8, 0xDB, 0x6D, 0xCB, 0x17, 0x50,
  0x89, 0x7B, 0x44, 0x28, 0x03, 0x6B, 0xD5, 0xA9, 0x36, 0x36, 0xD9, 0x6B, 0xA8, 0x93, 0x3A, 0x96,
  0xEE, 0xFF, 0x67, 0x8B, 0x36, 0xDA, 0x09, 0xB4, 0x99, 0x67, 0x2B, 0x88, 0xE4, 0xB5, 0xA5, 0xDA,
  0x65, 0x47, 0xDA, 0x1E, 0x96, 0xFA, 0xEC, 0xD5, 0xA9, 0x45, 0x63, 0x1A, 0xCB, 0xC9, 0x48, 0x9D,
  0x83, 0x5F, 0x6F, 0xCB, 0x08, 0x1B, 0x97, 0xC9, 0x18, 0x0A, 0x63, 0xCB, 0xA6, 0xE1, 0x84, 0xF5,
  0x62, 0x61, 0x6A, 0x84, 0xDC, 0xB6, 0x37, 0x9E, 0xD6, 0xAB, 0x3C, 0x53, 0x93, 0xC1, 0x2A, 0xAA,
  0x81, 0x8D, 0x6B, 0x65, 0x60, 0xA8, 0xFB, 0x2E, 0x22, 0x59, 0x74, 0x61, 0xA6, 0x5D, 0x73, 0x94,
  0xF8, 0xE4, 0xC1, 0x46, 0x26, 0x5E, 0x8A, 0x86, 0xED, 0xD4, 0xA6, 0x2D, 0x57, 0x6B, 0xBE, 0xE8,
  0x58, 0xDA, 0x3D, 0x98, 0x99, 0xBE, 0xA8, 0xC2, 0xDB, 0x6A, 0x2E, 0x51, 0x62, 0xE5, 0x80, 0x58,
  0x76, 0xB8, 0xE4, 0x6C, 0x84, 0xCA, 0x98, 0x06, 0x0B, 0xFC, 0xD2, 0x66, 0x7C, 0x62, 0x3A, 0x5B,
  0xC5, 0xDF, 0x7D, 0x75, 0x49, 0x1E, 0x52, 0xC7, 0x55, 0xF7, 0x84, 0xA6, 0xDA, 0x5D, 0x43, 0x26,
  0x85, 0x98, 0xD8, 0x8F, 0xB6, 0xC5, 0x28, 0xEB, 0x3E, 0x75, 0x04, 0xD2, 0x27, 0xBA, 0x2A, 0x2B,
  0xB7, 0x03, 0x13, 0x45, 0x35, 0x1B, 0x78, 0x5F, 0xC3, 0xBA, 0xDB, 0xAE, 0x27, 0xC2, 0x5E, 0xA4,
  0x50, 0x8C, 0x8A, 0xBB, 0x4F, 0x60, 0xC3, 0xEE, 0x41, 0x46, 0x4A, 0xDF, 0xD2, 0x27, 0xB2, 0xAD,
  0xEB, 0x5F, 0x43, 0x4C, 0x6A, 0x09, 0x2A, 0xCC, 0xB7, 0x47, 0x2A, 0xB9, 0x91, 0xB6, 0xD4, 0x5B,
  0x25, 0x58, 0xD8, 0xFD, 0x46, 0x95, 0x5A, 0xC3, 0x27, 0x5B, 0x3F, 0xFB, 0x12, 0xD2, 0x26, 0xC3,
  0xA9, 0xA1, 0xB6, 0xA2, 0xCB, 0x1B, 0xD0, 0x73, 0xE4, 0xBA, 0xA1, 0xE9, 0x05, 0xBE, 0x79, 0x23,
  0xA4, 0xC2, 0x3A, 0x4B, 0x11, 0xE5, 0x68, 0xC4, 0xC1, 0xBA, 0xC1, 0xCC, 0x8B, 0x02, 0xD2, 0x63,
  0x6C, 0xEE, 0x19, 0x5E, 0xE1, 0xB6, 0x4C, 0x1A, 0xB4, 0x5E, 0xF0, 0xC2, 0x27, 0x20, 0x55, 0xBD,
  0x6D, 0x64, 0xE1, 0xC7, 0x45, 0xA9, 0x65, 0x6D, 0x7D, 0x42, 0x56, 0xD8, 0xB2, 0xB6, 0xEC, 0xD3,
  0x61, 0x5B, 0x62, 0x61, 0x60, 0xA1, 0x5B, 0xD6, 0x15, 0x29, 0x09, 0x6C, 0xA1, 0x3E, 0xAD, 0x65,
  0x34, 0xC3, 0xC0, 0xC1, 0x22, 0x6D, 0x4C, 0x57, 0x10, 0xDB, 0x41, 0xD2, 0xE1, 0x77, 0x64, 0xF7,
  0xD3, 0x21, 0x73, 0xA9, 0x29, 0x58, 0xC1, 0xA1, 0x5A, 0x52, 0xB7, 0x32, 0x64, 0xC1, 0x67, 0x42,
  0x74, 0x2C, 0xDC, 0x61, 0x61, 0x65, 0x8B, 0xCB, 0x04, 0xE5, 0x60, 0xC1, 0xC9, 0x5E, 0x8E, 0x36,
  0x83, 0xD2, 0xA2, 0x83, 0xA9, 0xD9, 0xCD, 0x21, 0xB9, 0x25, 0xCD, 0xE6, 0x1D, 0x60, 0xA1, 0xB4,
  0xAA, 0x8F, 0xBA, 0x75, 0xC3, 0x01, 0x0B, 0x3B, 0x51, 0xDB, 0xEC, 0x62, 0xE1, 0x38, 0xCD, 0x40,
  0x3B, 0xD3, 0xD2, 0x26, 0x94, 0x29, 0xD2, 0x61, 0x21, 0x6B, 0x4A, 0x8D, 0x24, 0xB5, 0xBB, 0x21,
  0x12, 0xA5, 0x99, 0xA5, 0x1A, 0xCA, 0xA1, 0xEF, 0x5D, 0xAA, 0xAE, 0xD3, 0x64, 0xE1, 0xA3, 0x6B,
  0xAE, 0x35, 0x39, 0xD2, 0x66, 0x73, 0xB6, 0x90, 0xC6, 0xC1, 0x32, 0xD1, 0xBA, 0xC9, 0x25, 0x65,
  0x81, 0xA8, 0xD2, 0xB1, 0xE7, 0x18, 0xBE, 0xC0, 0xFC, 0xE4, 0x85, 0xB5, 0x06, 0xB4, 0x81, 0x35,
  0x46, 0xB6, 0xC8, 0x9B
};
/********************** end #include "../data/data.h" **********************/

static unsigned char bitoff[ 256 ] = {
	 8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
	 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 *	Prototypes 
 */
extern word	gsm_add 	P(( word a, word b ));
extern word	gsm_sub 	P((word a, word b));
extern word	gsm_mult 	P((word a, word b));
extern word	gsm_mult_r	P((word a, word b));
extern word	gsm_abs 	P((word a));
extern longword gsm_L_mult 	P((word a, word b));
extern longword gsm_L_add 	P(( longword a, longword b ));
extern longword gsm_L_sub 	P((longword a, longword b));
extern word	gsm_norm 	P(( longword a ));
extern longword gsm_L_asl  	P((longword a, int n));
extern word	gsm_asl 	P((word a, int n));
extern longword gsm_L_asr  	P((longword a, int n));
extern word	gsm_div  	P((word num, word denum));
int gsm_option (gsm r, int opt, int * val);
static void Decoding_of_the_coded_Log_Area_Ratios (
	word 	* LARc,		/* coded log area ratio	[0..7] 	IN	*/
	word	* LARpp);	/* out: decoded ..			*/
static void Coefficients_0_12 (
	word * LARpp_j_1,
	word * LARpp_j,
	word * LARp);
static void LARp_to_rp (
	word * LARp);	/* [0..7] IN/OUT  */
int decode();
extern int  gsm_decode  GSM_P((gsm, gsm_byte   *, gsm_signal *));
extern void Gsm_Decoder P((
		struct gsm_state * S,
		word	* LARcr,	/* [0..7]		IN	*/
		word	* Ncr,		/* [0..3] 		IN 	*/
		word	* bcr,		/* [0..3]		IN	*/
		word	* Mcr,		/* [0..3] 		IN 	*/
		word	* xmaxcr,	/* [0..3]		IN 	*/
		word	* xMcr,		/* [0..13*4]		IN	*/
		word	* s));		/* [0..159]		OUT 	*/
extern void Gsm_Long_Term_Synthesis_Filtering P((
		struct gsm_state* S,
		word	Ncr,
		word	bcr,
		word	* erp,		/* [0..39]		  IN 	*/
		word	* drp)); 	/* [-120..-1] IN, [0..40] OUT 	*/
void Gsm_RPE_Decoding P((
	struct gsm_state *S,
		word xmaxcr,
		word Mcr,
		word * xMcr,  /* [0..12], 3 bits             IN      */
		word * erp)); /* [0..39]                     OUT     */
static void RPE_grid_positioning (
	word		Mc,		/* grid position	IN	*/
	word	* xMp,		/* [0..12]		IN	*/
	word	* ep		/* [0..39]		OUT	*/
);
static void APCM_inverse_quantization (
	word	* xMc,	/* [0..12]			IN 	*/
	word		mant,
	word		exp,
	word	* xMp);	/* [0..12]			OUT 	*/
extern word	gsm_asr  	P((word a, int n));
static void APCM_quantization_xmaxc_to_exp_mant (
	word		xmaxc,		/* IN 	*/
	word		* exp_out,	/* OUT	*/
	word		* mant_out );	/* OUT  */
static void Postprocessing (
	struct gsm_state	* S,
	word 		* s);
static void Coefficients_13_26 (
	word * LARpp_j_1,
	word * LARpp_j,
	word * LARp);
static void Coefficients_40_159 (
	word * LARpp_j,
	word * LARp);
static void Short_term_synthesis_filtering (
	struct gsm_state * S,
	word	* rrp,	/* [0..7]	IN	*/
	int	k,	/* k_end - k_start	*/
	word	* wt,	/* [0..k-1]	IN	*/
	word	* sr	/* [0..k-1]	OUT	*/
);
void Gsm_Short_Term_Synthesis_Filter (
	struct gsm_state * S,
	word	* LARcr,	/* received log area ratios [0..7] IN  */
	word	* wt,		/* received d [0..159]		   IN  */
	word	* s		/* signal   s [0..159]		  OUT  */
);
static void Coefficients_27_39 (
	word * LARpp_j_1,
	word * LARpp_j,
	word * LARp);
gsm create(void);
int main(void);

#undef	GSM_P

/* add.c */

word gsm_add (word a, word b)
{
  longword sum = (longword)a + (longword)b;
  return saturate(sum);
}

word gsm_sub (word a, word b)
{
  longword diff = (longword)a - (longword)b;
  return saturate(diff);
}

word gsm_mult (word a, word b)
{
  if (a == MIN_WORD && b == MIN_WORD) return MAX_WORD;
  else return SASR( (longword)a * (longword)b, 15 );
}

word gsm_mult_r (word a, word b)
{
  if (b == MIN_WORD && a == MIN_WORD) return MAX_WORD;
  else {
    longword prod = (longword)a * (longword)b + 16384;
    prod >>= 15;
    return prod & 0xFFFF;
  }
}

word gsm_abs (word a)
{
  return a < 0 ? (a == MIN_WORD ? MAX_WORD : -a) : a;
}

longword gsm_L_mult (word a, word b)
{
  return ((longword)a * (longword)b) << 1;
}

longword gsm_L_add (longword a, longword b)
{
  if (a < 0) {
    if (b >= 0) return a + b;
    else {
      ulongword A = (ulongword)-(a + 1) + (ulongword)-(b + 1);
      return A >= MAX_LONGWORD ? MIN_LONGWORD :-(longword)A-2;
    }
  }
  else if (b <= 0) return a + b;
  else {
    ulongword A = (ulongword)a + (ulongword)b;
    return A > MAX_LONGWORD ? MAX_LONGWORD : A;
  }
}

longword gsm_L_sub (longword a, longword b)
{
  if (a >= 0) {
    if (b >= 0) return a - b;
    else {
      /* a>=0, b<0 */

      ulongword A = (ulongword)a + -(b + 1);
      return A >= MAX_LONGWORD ? MAX_LONGWORD : (A + 1);
    }
  }
  else if (b <= 0) return a - b;
  else {
    /* a<0, b>0 */  

    ulongword A = (ulongword)-(a + 1) + b;
    return A >= MAX_LONGWORD ? MIN_LONGWORD : -(longword)A - 1;
  }
}

word gsm_norm (longword a )
/*
 * the number of left shifts needed to normalize the 32 bit
 * variable L_var1 for positive values on the interval
 *
 * with minimum of
 * minimum of 1073741824  (01000000000000000000000000000000) and 
 * maximum of 2147483647  (01111111111111111111111111111111)
 *
 *
 * and for negative values on the interval with
 * minimum of -2147483648 (-10000000000000000000000000000000) and
 * maximum of -1073741824 ( -1000000000000000000000000000000).
 *
 * in order to normalize the result, the following
 * operation must be done: L_norm_var1 = L_var1 << norm( L_var1 );
 *
 * (That's 'ffs', only from the left, not the right..)
 */
{
  if (a < 0) {
    if (a <= -1073741824) return 0;
    a = ~a;
  }

  return    a & 0xffff0000 
    ? ( a & 0xff000000
	?  -1 + bitoff[ 0xFF & (a >> 24) ]
	:   7 + bitoff[ 0xFF & (a >> 16) ] )
    : ( a & 0xff00
	?  15 + bitoff[ 0xFF & (a >> 8) ]
	:  23 + bitoff[ 0xFF & a ] );
}

longword gsm_L_asl (longword a, int n)
{
  if (n >= 32) return 0;
  if (n <= -32) return -(a < 0);
  if (n < 0) return gsm_L_asr(a, -n);
  return a << n;
}

word gsm_asl (word a, int n)
{
  if (n >= 16) return 0;
  if (n <= -16) return -(a < 0);
  if (n < 0) return gsm_asr(a, -n);
  return a << n;
}

longword gsm_L_asr (longword a, int n)
{
  if (n >= 32) return -(a < 0);
  if (n <= -32) return 0;
  if (n < 0) return a << -n;

#	ifdef	SASR
  return a >> n;
#	else
  if (a >= 0) return a >> n;
  else return -(longword)( -(ulongword)a >> n );
#	endif
}

/* 
 *  (From p. 46, end of section 4.2.5)
 *
 *  NOTE: The following lines gives [sic] one correct implementation
 *  	  of the div(num, denum) arithmetic operation.  Compute div
 *        which is the integer division of num by denum: with denum
 *	  >= num > 0
 */
word gsm_div ( word num, word denum)
{
  longword	L_num   = num;
  longword	L_denum = denum;
  word		div 	= 0;
  int		k 	= 15;

  /* The parameter num sometimes becomes zero.
   * Although this is explicitly guarded against in 4.2.5,
   * we assume that the result should then be zero as well.
   */

  if (num == 0)
    return 0;

  _Pragma("loopbound min 15 max 15")
  while (k--) {
    div   <<= 1;
    L_num <<= 1;

    if (L_num >= L_denum) {
      L_num -= L_denum;
      div++;
    }
  }

  return div;
}

/* gms_options.c */
int gsm_option (gsm r, int opt, int * val)
{
  int 	result = -1;

  switch (opt) {
    case GSM_OPT_VERBOSE:
#ifndef	NDEBUG
      result = r->verbose;
      if (val) r->verbose = *val;
#endif
      break;

    case GSM_OPT_FAST:

#if	defined(FAST) && defined(USE_FLOAT_MUL)
      result = r->fast;
      if (val) r->fast = !!*val;
#endif
      break;

    default:
      break;
  }

  return result;
}

/* short_term.c */
/*
 *  SHORT TERM ANALYSIS FILTERING SECTION
 */

/* 4.2.8 */

static void Decoding_of_the_coded_Log_Area_Ratios (
	word 	* LARc,		/* coded log area ratio	[0..7] 	IN	*/
	word	* LARpp)	/* out: decoded ..			*/
{
  word	temp1 /* , temp2 */;
  long	ltmp;	/* for GSM_ADD */

  /*  This procedure requires for efficient implementation
   *  two tables.
   *
   *  INVA[1..8] = integer( (32768 * 8) / real_A[1..8])
   *  MIC[1..8]  = minimum value of the LARc[1..8]
   */

  /*  Compute the LARpp[1..8]
  */

  /* 	for (i = 1; i <= 8; i++, B++, MIC++, INVA++, LARc++, LARpp++) {
   *
   *		temp1  = GSM_ADD( *LARc, *MIC ) << 10;
   *		temp2  = *B << 1;
   *		temp1  = GSM_SUB( temp1, temp2 );
   *
   *		temp1  = GSM_MULT_R( *INVA, temp1 );
   *		*LARpp = GSM_ADD( temp1, temp1 );
   *	}
   */


#undef	STEP
#define	STEP( B, MIC, INVA )	\
  temp1    = GSM_ADD( *LARc++, MIC ) << 10;	\
  temp1    = GSM_SUB( temp1, B << 1 );		\
  temp1    = GSM_MULT_R( INVA, temp1 );		\
  *LARpp++ = GSM_ADD( temp1, temp1 );

  STEP(      0,  -32,  13107 );
  STEP(      0,  -32,  13107 );
  STEP(   2048,  -16,  13107 );
  STEP(  -2560,  -16,  13107 );

  STEP(     94,   -8,  19223 );
  STEP(  -1792,   -8,  17476 );
  STEP(   -341,   -4,  31454 );
  STEP(  -1144,   -4,  29708 );

  /* NOTE: the addition of *MIC is used to restore
   * 	 the sign of *LARc.
   */
}

/* 4.2.9 */
/* Computation of the quantized reflection coefficients 
 */

/* 4.2.9.1  Interpolation of the LARpp[1..8] to get the LARp[1..8]
 */

/*
 *  Within each frame of 160 analyzed speech samples the short term
 *  analysis and synthesis filters operate with four different sets of
 *  coefficients, derived from the previous set of decoded LARs(LARpp(j-1))
 *  and the actual set of decoded LARs (LARpp(j))
 *
 * (Initial value: LARpp(j-1)[1..8] = 0.)
 */

static void Coefficients_0_12 (
	word * LARpp_j_1,
	word * LARpp_j,
	word * LARp)
{
  int 	i;
  longword ltmp;

  _Pragma("loopbound min 8 max 8")
  for (i = 1; i <= 8; i++, LARp++, LARpp_j_1++, LARpp_j++) {
    *LARp = GSM_ADD( SASR( *LARpp_j_1, 2 ), SASR( *LARpp_j, 2 ));
    *LARp = GSM_ADD( *LARp,  SASR( *LARpp_j_1, 1));
  }
}

/* 4.2.9.2 */

static void LARp_to_rp (
	word * LARp)	/* [0..7] IN/OUT  */
/*
 *  The input of this procedure is the interpolated LARp[0..7] array.
 *  The reflection coefficients, rp[i], are used in the analysis
 *  filter and in the synthesis filter.
 */
{
  int 		i;
  word		temp;
  longword	ltmp;

  _Pragma("loopbound min 8 max 8")
  for (i = 1; i <= 8; i++, LARp++) {

    /* temp = GSM_ABS( *LARp );
     *
     * if (temp < 11059) temp <<= 1;
     * else if (temp < 20070) temp += 11059;
     * else temp = GSM_ADD( temp >> 2, 26112 );
     *
     * *LARp = *LARp < 0 ? -temp : temp;
     */

    if (*LARp < 0) {
      temp = *LARp == MIN_WORD ? MAX_WORD : -(*LARp);
      *LARp = - ((temp < 11059) ? temp << 1
	  : ((temp < 20070) ? temp + 11059
	    :  GSM_ADD( temp >> 2, 26112 )));
    } else {
      temp  = *LARp;
      *LARp =    (temp < 11059) ? temp << 1
	: ((temp < 20070) ? temp + 11059
	    :  GSM_ADD( temp >> 2, 26112 ));
    }
  }
}

int decode() 
{
  gsm r;
  unsigned i;

  word * d;

  r = create(); //OK

  d = r->dp0 + 120;

  (void)gsm_option(r, GSM_OPT_FAST, &f_fast);
  (void)gsm_option(r, GSM_OPT_VERBOSE, &f_verbose);

  _Pragma("loopbound min 1 max 1")
  for (i=0; i < SAMPLES; i++) {
    if (gsm_decode(r, gsmdata + i * sizeof(gsm_frame), pcmdata + i * 160)) return 1;
  }

  return 0;
}

/* gsm_decode.c */
int gsm_decode (gsm s, gsm_byte * c, gsm_signal * target)
{
  word  	LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xmc[13*4];

  /* GSM_MAGIC  = (*c >> 4) & 0xF; */

  if (((*c >> 4) & 0x0F) != GSM_MAGIC) return -1;

  LARc[0]  = (*c++ & 0xF) << 2;		/* 1 */
  LARc[0] |= (*c >> 6) & 0x3;
  LARc[1]  = *c++ & 0x3F;
  LARc[2]  = (*c >> 3) & 0x1F;
  LARc[3]  = (*c++ & 0x7) << 2;
  LARc[3] |= (*c >> 6) & 0x3;
  LARc[4]  = (*c >> 2) & 0xF;
  LARc[5]  = (*c++ & 0x3) << 2;
  LARc[5] |= (*c >> 6) & 0x3;
  LARc[6]  = (*c >> 3) & 0x7;
  LARc[7]  = *c++ & 0x7;
  Nc[0]  = (*c >> 1) & 0x7F;
  bc[0]  = (*c++ & 0x1) << 1;
  bc[0] |= (*c >> 7) & 0x1;
  Mc[0]  = (*c >> 5) & 0x3;
  xmaxc[0]  = (*c++ & 0x1F) << 1;
  xmaxc[0] |= (*c >> 7) & 0x1;
  xmc[0]  = (*c >> 4) & 0x7;
  xmc[1]  = (*c >> 1) & 0x7;
  xmc[2]  = (*c++ & 0x1) << 2;
  xmc[2] |= (*c >> 6) & 0x3;
  xmc[3]  = (*c >> 3) & 0x7;
  xmc[4]  = *c++ & 0x7;
  xmc[5]  = (*c >> 5) & 0x7;
  xmc[6]  = (*c >> 2) & 0x7;
  xmc[7]  = (*c++ & 0x3) << 1;		/* 10 */
  xmc[7] |= (*c >> 7) & 0x1;
  xmc[8]  = (*c >> 4) & 0x7;
  xmc[9]  = (*c >> 1) & 0x7;
  xmc[10]  = (*c++ & 0x1) << 2;
  xmc[10] |= (*c >> 6) & 0x3;
  xmc[11]  = (*c >> 3) & 0x7;
  xmc[12]  = *c++ & 0x7;
  Nc[1]  = (*c >> 1) & 0x7F;
  bc[1]  = (*c++ & 0x1) << 1;
  bc[1] |= (*c >> 7) & 0x1;
  Mc[1]  = (*c >> 5) & 0x3;
  xmaxc[1]  = (*c++ & 0x1F) << 1;
  xmaxc[1] |= (*c >> 7) & 0x1;
  xmc[13]  = (*c >> 4) & 0x7;
  xmc[14]  = (*c >> 1) & 0x7;
  xmc[15]  = (*c++ & 0x1) << 2;
  xmc[15] |= (*c >> 6) & 0x3;
  xmc[16]  = (*c >> 3) & 0x7;
  xmc[17]  = *c++ & 0x7;
  xmc[18]  = (*c >> 5) & 0x7;
  xmc[19]  = (*c >> 2) & 0x7;
  xmc[20]  = (*c++ & 0x3) << 1;
  xmc[20] |= (*c >> 7) & 0x1;
  xmc[21]  = (*c >> 4) & 0x7;
  xmc[22]  = (*c >> 1) & 0x7;
  xmc[23]  = (*c++ & 0x1) << 2;
  xmc[23] |= (*c >> 6) & 0x3;
  xmc[24]  = (*c >> 3) & 0x7;
  xmc[25]  = *c++ & 0x7;
  Nc[2]  = (*c >> 1) & 0x7F;
  bc[2]  = (*c++ & 0x1) << 1;		/* 20 */
  bc[2] |= (*c >> 7) & 0x1;
  Mc[2]  = (*c >> 5) & 0x3;
  xmaxc[2]  = (*c++ & 0x1F) << 1;
  xmaxc[2] |= (*c >> 7) & 0x1;
  xmc[26]  = (*c >> 4) & 0x7;
  xmc[27]  = (*c >> 1) & 0x7;
  xmc[28]  = (*c++ & 0x1) << 2;
  xmc[28] |= (*c >> 6) & 0x3;
  xmc[29]  = (*c >> 3) & 0x7;
  xmc[30]  = *c++ & 0x7;
  xmc[31]  = (*c >> 5) & 0x7;
  xmc[32]  = (*c >> 2) & 0x7;
  xmc[33]  = (*c++ & 0x3) << 1;
  xmc[33] |= (*c >> 7) & 0x1;
  xmc[34]  = (*c >> 4) & 0x7;
  xmc[35]  = (*c >> 1) & 0x7;
  xmc[36]  = (*c++ & 0x1) << 2;
  xmc[36] |= (*c >> 6) & 0x3;
  xmc[37]  = (*c >> 3) & 0x7;
  xmc[38]  = *c++ & 0x7;
  Nc[3]  = (*c >> 1) & 0x7F;
  bc[3]  = (*c++ & 0x1) << 1;
  bc[3] |= (*c >> 7) & 0x1;
  Mc[3]  = (*c >> 5) & 0x3;
  xmaxc[3]  = (*c++ & 0x1F) << 1;
  xmaxc[3] |= (*c >> 7) & 0x1;
  xmc[39]  = (*c >> 4) & 0x7;
  xmc[40]  = (*c >> 1) & 0x7;
  xmc[41]  = (*c++ & 0x1) << 2;
  xmc[41] |= (*c >> 6) & 0x3;
  xmc[42]  = (*c >> 3) & 0x7;
  xmc[43]  = *c++ & 0x7;			/* 30  */
  xmc[44]  = (*c >> 5) & 0x7;
  xmc[45]  = (*c >> 2) & 0x7;
  xmc[46]  = (*c++ & 0x3) << 1;
  xmc[46] |= (*c >> 7) & 0x1;
  xmc[47]  = (*c >> 4) & 0x7;
  xmc[48]  = (*c >> 1) & 0x7;
  xmc[49]  = (*c++ & 0x1) << 2;
  xmc[49] |= (*c >> 6) & 0x3;
  xmc[50]  = (*c >> 3) & 0x7;
  xmc[51]  = *c & 0x7;			/* 33 */

  Gsm_Decoder(s, LARc, Nc, bc, Mc, xmaxc, xmc, target);

  return 0;
}

void Gsm_Decoder (
    struct gsm_state	* S,

    word		* LARcr,	/* [0..7]		IN	*/

    word		* Ncr,		/* [0..3] 		IN 	*/
    word		* bcr,		/* [0..3]		IN	*/
    word		* Mcr,		/* [0..3] 		IN 	*/
    word		* xmaxcr,	/* [0..3]		IN 	*/
    word		* xMcr,		/* [0..13*4]		IN	*/

    word		* s)		/* [0..159]		OUT 	*/
{
  int		j, k;
  word		erp[40], wt[160];
  word		* drp = S->dp0 + 120;

  _Pragma("loopbound min 4 max 4")
  for (j=0; j <= 3; j++, xmaxcr++, bcr++, Ncr++, Mcr++, xMcr += 13) {

    Gsm_RPE_Decoding( S, *xmaxcr, *Mcr, xMcr, erp );
    Gsm_Long_Term_Synthesis_Filtering( S, *Ncr, *bcr, erp, drp );

    _Pragma("loopbound min 40 max 40")
    for (k = 0; k <= 39; k++){
      wt[ j * 40 + k ] =  drp[ k ];
    }
  }

  Gsm_Short_Term_Synthesis_Filter( S, LARcr, wt, s );
  Postprocessing(S, s);
}

/* 4.3.2 */
void Gsm_Long_Term_Synthesis_Filtering (
	struct gsm_state	* S,

	word			Ncr,
	word			bcr,
	word		* erp,		/* [0..39]		  IN */
	word		* drp		/* [-120..-1] IN, [0..40] OUT */
)
/*
 *  This procedure uses the bcr and Ncr parameter to realize the
 *  long term synthesis filtering.  The decoding of bcr needs
 *  table 4.3b.
 */
{
  longword	ltmp;	/* for ADD */
  int 		k;
  word			brp, drpp, Nr;

  /*  Check the limits of Nr.
  */
  Nr = Ncr < 40 || Ncr > 120 ? S->nrp : Ncr;
  S->nrp = Nr;

  /*  Decoding of the LTP gain bcr
  */
  brp = gsm_QLB[ bcr ];

  /*  Computation of the reconstructed short term residual 
   *  signal drp[0..39]
   */
  //

  _Pragma("loopbound min 40 max 40")
  for (k = 0; k <= 39; k++) {
    drpp   = GSM_MULT_R( brp, drp[ k - Nr ] );
    drp[k] = GSM_ADD( erp[k], drpp );
  }

  /*
   *  Update of the reconstructed short term residual signal
   *  drp[ -1..-120 ]
   */

  _Pragma("loopbound min 120 max 120")
  for (k = 0; k <= 119; k++) drp[ -120 + k ] = drp[ -80 + k ];
}

void Gsm_RPE_Decoding (
	struct gsm_state	* S,

	word 		xmaxcr,
	word		Mcr,
	word		* xMcr,  /* [0..12], 3 bits 		IN	*/
	word		* erp	 /* [0..39]			OUT 	*/
)
{
  word	exp, mant;
  word	xMp[ 13 ];

  APCM_quantization_xmaxc_to_exp_mant( xmaxcr, &exp, &mant );
  APCM_inverse_quantization( xMcr, mant, exp, xMp );
  RPE_grid_positioning( Mcr, xMp, erp );

}

/* 4.2.17 */
static void RPE_grid_positioning (
	word		Mc,		/* grid position	IN	*/
	word	* xMp,		/* [0..12]		IN	*/
	word	* ep		/* [0..39]		OUT	*/
)
/*
 *  This procedure computes the reconstructed long term residual signal
 *  ep[0..39] for the LTP analysis filter.  The inputs are the Mc
 *  which is the grid position selection and the xMp[0..12] decoded
 *  RPE samples which are upsampled by a factor of 3 by inserting zero
 *  values.
 */
{
  int	i = 13;

  //
  // TODO: rewritten Duff's device for WCET analysis!
  //
  switch (Mc) {
    case 3: *ep++ = 0;
    case 2: *ep++ = 0;
    case 1: *ep++ = 0;
    case 0: *ep++ = *xMp++;
            i--;
  }

  _Pragma("loopbound min 12 max 12")
  do {
    *ep++ = 0;
    *ep++ = 0;
    *ep++ = *xMp++;
  } while (--i);

  _Pragma("loopbound min 0 max 2")
  while (++Mc < 4) *ep++ = 0;

}
/*
{
  int	i = 13;

  //
  //TODO: removed for WCET analysis
//_Pragma("marker outside")  
  switch (Mc) {
    case 3: *ep++ = 0;
    case 2:  
	    _Pragma("loopbound min 13 max 13")
	    do {
	       *ep++ = 0;
	       case 1:         *ep++ = 0;
	       case 0:         
			       //_Pragma("marker inside")
			       *ep++ = *xMp++;
	     } while (--i);
  }
  
  //_Pragma("flowrestriction 1*inside <=  13*outside")

  _Pragma("loopbound min 0 max 3")
  while (++Mc < 4) *ep++ = 0;

}
*/

/* 4.2.16 */
static void APCM_inverse_quantization (
	word	* xMc,	/* [0..12]			IN 	*/
	word		mant,
	word		exp,
	word	* xMp)	/* [0..12]			OUT 	*/
/* 
 *  This part is for decoding the RPE sequence of coded xMc[0..12]
 *  samples to obtain the xMp[0..12] array.  Table 4.6 is used to get
 *  the mantissa of xmaxc (FAC[0..7]).
 */
{
  int	i;
  word	temp, temp1, temp2, temp3;
  longword	ltmp, temp_sat;

  temp1 = gsm_FAC[ mant ];	/* see 4.2-15 for mant */
  temp2 = gsm_sub( 6, exp );	/* see 4.2-15 for exp  */
  temp3 = gsm_asl( 1, gsm_sub( temp2, 1 ));

  _Pragma("loopbound min 13 max 13")
  for (i = 13; i--;) {

    /* temp = gsm_sub( *xMc++ << 1, 7 ); */
    temp = (*xMc++ << 1) - 7;	        /* restore sign   */

    temp <<= 12;				/* 16 bit signed  */
    temp = GSM_MULT_R( temp1, temp );
    temp = GSM_ADD( temp, temp3 );
    *xMp++ = gsm_asr( temp, temp2 );
  }
}

/*
 *  4.2.4 .. 4.2.7 LPC ANALYSIS SECTION
 */
word gsm_asr (word a, int n)
{
  if (n >= 16) return -(a < 0);
  if (n <= -16) return 0;
  if (n < 0) return a << -n;

#	ifdef	SASR
  return a >> n;
#	else
  if (a >= 0) return a >> n;
  else return -(word)( -(uword)a >> n );
#	endif
}


/* rpe.c */
/* 4.12.15 */
static void APCM_quantization_xmaxc_to_exp_mant (
	word		xmaxc,		/* IN 	*/
	word		* exp_out,	/* OUT	*/
	word		* mant_out )	/* OUT  */
{
  word	exp, mant;

  /* Compute exponent and mantissa of the decoded version of xmaxc
  */
  exp = 0;
  if (xmaxc > 15) exp = SASR(xmaxc, 3) - 1;
  mant = xmaxc - (exp << 3);

  if (mant == 0) {
    exp  = -4;
    mant = 7;
  }
  else {
    _Pragma("loopbound min 0 max 3")
      while (mant <= 7) {
	mant = mant << 1 | 1;
	exp--;
      }
    mant -= 8;
  }

  *exp_out  = exp;
  *mant_out = mant;

}

/* decode.c */
/*
 *  4.3 FIXED POINT IMPLEMENTATION OF THE RPE-LTP DECODER
 */
static void Postprocessing (
	struct gsm_state	* S,
	word 		* s)
{
  int		k;
  word		msr = S->msr;
  longword	ltmp;	/* for GSM_ADD */
  word		tmp;

  _Pragma("loopbound min 159 max 159")
  for (k = 160; --k; ++s) {
    tmp = GSM_MULT_R( msr, 28180 );
    msr = GSM_ADD(*s, tmp);  	   /* Deemphasis 	     */
    *s  = GSM_ADD(msr, msr) & 0xFFF8;  /* Truncation & Upscaling */
  }
  S->msr = msr;
}

static void Coefficients_13_26 (
	word * LARpp_j_1,
	word * LARpp_j,
	word * LARp)
{
  int i;
  longword ltmp;
  _Pragma("loopbound min 8 max 8")
  for (i = 1; i <= 8; i++, LARpp_j_1++, LARpp_j++, LARp++) {
    *LARp = GSM_ADD( SASR( *LARpp_j_1, 1), SASR( *LARpp_j, 1 ));
  }
}

static void Coefficients_40_159 (
	word * LARpp_j,
	word * LARp)
{
  int i;

  _Pragma("loopbound min 8 max 8")
  for (i = 1; i <= 8; i++, LARp++, LARpp_j++) {
    *LARp = *LARpp_j; 
  }
}

static void Short_term_synthesis_filtering (
	struct gsm_state * S,
	word	* rrp,	/* [0..7]	IN	*/
	int	k,	/* k_end - k_start	*/
	word	* wt,	/* [0..k-1]	IN	*/
	word	* sr	/* [0..k-1]	OUT	*/
)
{
  word		* v = S->v;
  int		i;
  word		sri, tmp1, tmp2;
  longword	ltmp;	/* for GSM_ADD  & GSM_SUB */
  int j;

  _Pragma("loopbound min 12 max 118")
  while (--k) {
    sri = *wt++;
    _Pragma("loopbound min 8 max 8")
    for (i = 8; i--;) {

      /* sri = GSM_SUB( sri, gsm_mult_r( rrp[i], v[i] ) );
      */
      tmp1 = rrp[i];
      tmp2 = v[i];
      tmp2 =  ( tmp1 == MIN_WORD && tmp2 == MIN_WORD
	  ? MAX_WORD
	  : 0x0FFFF & (( (longword)tmp1 * (longword)tmp2
	      + 16384) >> 15)) ;

      sri  = GSM_SUB( sri, tmp2 );

      /* v[i+1] = GSM_ADD( v[i], gsm_mult_r( rrp[i], sri ) );
      */
      tmp1  = ( tmp1 == MIN_WORD && sri == MIN_WORD
	  ? MAX_WORD
	  : 0x0FFFF & (( (longword)tmp1 * (longword)sri
	      + 16384) >> 15)) ;

      v[i+1] = GSM_ADD( v[i], tmp1);
    }
    *sr++ = v[0] = sri;
  }
}


void Gsm_Short_Term_Synthesis_Filter (
	struct gsm_state * S,

	word	* LARcr,	/* received log area ratios [0..7] IN  */
	word	* wt,		/* received d [0..159]		   IN  */

	word	* s		/* signal   s [0..159]		  OUT  */
)
{
  word		* LARpp_j	= S->LARpp[ S->j     ];
  word		* LARpp_j_1	= S->LARpp[ S->j ^=1 ];

  word		LARp[8];

#undef	FILTER
#if 	defined(FAST) && defined(USE_FLOAT_MUL)

# 	define	FILTER 	(* (S->fast			\
      ? Fast_Short_term_synthesis_filtering	\
      : Short_term_synthesis_filtering	))
#else
#	define	FILTER	Short_term_synthesis_filtering
#endif

  Decoding_of_the_coded_Log_Area_Ratios( LARcr, LARpp_j );

  Coefficients_0_12( LARpp_j_1, LARpp_j, LARp );
  LARp_to_rp( LARp );
  FILTER( S, LARp, 13, wt, s );

  Coefficients_13_26( LARpp_j_1, LARpp_j, LARp);
  LARp_to_rp( LARp );
  FILTER( S, LARp, 14, wt + 13, s + 13 );

  Coefficients_27_39( LARpp_j_1, LARpp_j, LARp);
  LARp_to_rp( LARp );
  FILTER( S, LARp, 13, wt + 27, s + 27 );

  Coefficients_40_159( LARpp_j, LARp );
  LARp_to_rp( LARp );
  FILTER(S, LARp, 120, wt + 40, s + 40);
}


static void Coefficients_27_39 (
	word * LARpp_j_1,
	word * LARpp_j,
	word * LARp)
{
  int i;
  longword ltmp;

  _Pragma("loopbound min 8 max 8")
  for (i = 1; i <= 8; i++, LARpp_j_1++, LARpp_j++, LARp++) {
    *LARp = GSM_ADD( SASR( *LARpp_j_1, 2 ), SASR( *LARpp_j, 2 ));
    *LARp = GSM_ADD( *LARp, SASR( *LARpp_j, 1 ));
  }
}


struct gsm_state gsmstate;

gsm create() 
{
  int i;
  gsm r;

  r = &gsmstate;

  _Pragma("loopbound min 648 max 648")
  for(i=0; i < sizeof(*r); i++) 
    ((char *)r)[i]=0;

  r->nrp = 40;

  return r;
}

int main( void ) 
{
  return decode();
}


/* lpc.c */
#undef	P



//extern char	* memcpy P((char *, char *, int));



/* code.c */




#if defined(FAST) && defined(USE_FLOAT_MUL)

static void Fast_Short_term_synthesis_filtering (
	struct gsm_state * S,
	word	* rrp,	/* [0..7]	IN	*/
	int	k,	/* k_end - k_start	*/
	word	* wt,	/* [0..k-1]	IN	*/
	word	* sr	/* [0..k-1]	OUT	*/
)
{
  word		* v = S->v;
  int		i;

  float va[9], rrpa[8];
  float scalef = 3.0517578125e-5, temp;

  _Pragma("loopbound min 8 max 8")
  for (i = 0; i < 8; ++i) {
    va[i]   = v[i];
    rrpa[i] = (float)rrp[i] * scalef;
  }
  
  _Pragma("loopbound min 13 max 120")
  while (--k) {
    float sri = *wt++;
    for (i = 8; i--;) {
      sri -= rrpa[i] * va[i];
      if     (sri < -32768.) sri = -32768.;
      else if (sri > 32767.) sri =  32767.;

      temp = va[i] + rrpa[i] * sri;
      if     (temp < -32768.) temp = -32768.;
      else if (temp > 32767.) temp =  32767.;
      va[i+1] = temp;
    }
    *sr++ = va[0] = sri;
  }
  for (i = 0; i < 9; ++i) v[i] = va[i];
}

#endif /* defined(FAST) && defined(USE_FLOAT_MUL) */
