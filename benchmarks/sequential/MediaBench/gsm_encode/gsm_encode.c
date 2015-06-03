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


//#include "proto.h"


/*
 *	Prototypes from add.c
 */
//extern word	gsm_mult 	P((word a, word b));
//extern word	gsm_mult_r	P((word a, word b));

extern word	gsm_div  	P((word num, word denum));

//extern word	gsm_add 	P(( word a, word b ));
//extern longword gsm_L_add 	P(( longword a, longword b ));

extern word	gsm_sub 	P((word a, word b));
//extern longword gsm_L_sub 	P((longword a, longword b));

//extern word	gsm_abs 	P((word a));

extern word	gsm_norm 	P(( longword a ));

//extern longword gsm_L_asl  	P((longword a, int n));
extern word	gsm_asl 	P((word a, int n));

//extern longword gsm_L_asr  	P((longword a, int n));
extern word	gsm_asr  	P((word a, int n));

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

//# define GSM_L_MULT(a, b) /* word a, word b */	\
//	(((longword)(a) * (longword)(b)) << 1)

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
extern void Gsm_Coder P((
		struct gsm_state	* S,
		word	* s,	/* [0..159] samples		IN	*/
		word	* LARc,	/* [0..7] LAR coefficients	OUT	*/
		word	* Nc,	/* [0..3] LTP lag		OUT 	*/
		word	* bc,	/* [0..3] coded LTP gain	OUT 	*/
		word	* Mc,	/* [0..3] RPE grid selection	OUT     */
		word	* xmaxc,/* [0..3] Coded maximum amplitude OUT	*/
		word	* xMc	/* [13*4] normalized RPE samples OUT	*/));

extern void Gsm_Long_Term_Predictor P((		/* 4x for 160 samples */
		struct gsm_state * S,
		word	* d,	/* [0..39]   residual signal	IN	*/
		word	* dp,	/* [-120..-1] d'		IN	*/
		word	* e,	/* [0..40] 			OUT	*/
		word	* dpp,	/* [0..40] 			OUT	*/
		word	* Nc,	/* correlation lag		OUT	*/
		word	* bc	/* gain factor			OUT	*/));

extern void Gsm_LPC_Analysis P((
		struct gsm_state * S,
		word * s,	 /* 0..159 signals	IN/OUT	*/
	        word * LARc));   /* 0..7   LARc's	OUT	*/

extern void Gsm_Preprocess P((
		struct gsm_state * S,
		word * s, word * so));

extern void Gsm_Short_Term_Analysis_Filter P((
		struct gsm_state * S,
		word	* LARc,	/* coded log area ratio [0..7]  IN	*/
		word	* d	/* st res. signal [0..159]	IN/OUT	*/));

void Gsm_RPE_Encoding P((
		struct gsm_state * S,
		word    * e,            /* -5..-1][0..39][40..44     IN/OUT  */
		word    * xmaxc,        /*                              OUT */
		word    * Mc,           /*                              OUT */
		word    * xMc));        /* [0..12]                      OUT */

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

extern void gsm_encode  GSM_P((gsm, gsm_signal *, gsm_byte  *));

extern int  gsm_explode GSM_P((gsm, gsm_byte   *, gsm_signal *));
extern void gsm_implode GSM_P((gsm, gsm_signal *, gsm_byte   *));

#undef	GSM_P

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

/* add.c */

word gsm_sub (word a, word b)
{
  longword diff = (longword)a - (longword)b;
  return saturate(diff);
}

/*
word gsm_add (word a, word b)
{
  longword sum = (longword)a + (longword)b;
  return saturate(sum);
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
*/
//longword gsm_L_sub (longword a, longword b)
//{
//  if (a >= 0) {
//    if (b >= 0) return a - b;
//    else {
//      /* a>=0, b<0 */
//
//      ulongword A = (ulongword)a + -(b + 1);
//      return A >= MAX_LONGWORD ? MAX_LONGWORD : (A + 1);
//    }
//  }
//  else if (b <= 0) return a - b;
//  else {
//    /* a<0, b>0 */  
//
//    ulongword A = (ulongword)-(a + 1) + b;
//    return A >= MAX_LONGWORD ? MIN_LONGWORD : -(longword)A - 1;
//  }
//}

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

//extern char	* memcpy P((char *, char *, int));

/* gsm_encode.c */
void gsm_encode (gsm s, gsm_signal * source, gsm_byte * c)
{
  word	 	LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xmc[13*4];

  Gsm_Coder(s, source, LARc, Nc, bc, Mc, xmaxc, xmc);


  /*	variable	size

	GSM_MAGIC	4

	LARc[0]		6
	LARc[1]		6
	LARc[2]		5
	LARc[3]		5
	LARc[4]		4
	LARc[5]		4
	LARc[6]		3
	LARc[7]		3

	Nc[0]		7
	bc[0]		2
	Mc[0]		2
	xmaxc[0]	6
	xmc[0]		3
	xmc[1]		3
	xmc[2]		3
	xmc[3]		3
	xmc[4]		3
	xmc[5]		3
	xmc[6]		3
	xmc[7]		3
	xmc[8]		3
	xmc[9]		3
	xmc[10]		3
	xmc[11]		3
	xmc[12]		3

	Nc[1]		7
	bc[1]		2
	Mc[1]		2
	xmaxc[1]	6
	xmc[13]		3
	xmc[14]		3
	xmc[15]		3
	xmc[16]		3
	xmc[17]		3
	xmc[18]		3
	xmc[19]		3
	xmc[20]		3
	xmc[21]		3
	xmc[22]		3
	xmc[23]		3
	xmc[24]		3
	xmc[25]		3

	Nc[2]		7
	bc[2]		2
	Mc[2]		2
	xmaxc[2]	6
	xmc[26]		3
	xmc[27]		3
	xmc[28]		3
	xmc[29]		3
	xmc[30]		3
	xmc[31]		3
	xmc[32]		3
	xmc[33]		3
	xmc[34]		3
	xmc[35]		3
	xmc[36]		3
	xmc[37]		3
	xmc[38]		3

	Nc[3]		7
	bc[3]		2
	Mc[3]		2
	xmaxc[3]	6
  xmc[39]		3
    xmc[40]		3
    xmc[41]		3
    xmc[42]		3
    xmc[43]		3
    xmc[44]		3
    xmc[45]		3
    xmc[46]		3
    xmc[47]		3
    xmc[48]		3
    xmc[49]		3
    xmc[50]		3
    xmc[51]		3
    */


  *c++ =   ((GSM_MAGIC & 0xF) << 4)		/* 1 */
    | ((LARc[0] >> 2) & 0xF);
  *c++ =   ((LARc[0] & 0x3) << 6)
    | (LARc[1] & 0x3F);
  *c++ =   ((LARc[2] & 0x1F) << 3)
    | ((LARc[3] >> 2) & 0x7);
  *c++ =   ((LARc[3] & 0x3) << 6)
    | ((LARc[4] & 0xF) << 2)
    | ((LARc[5] >> 2) & 0x3);
  *c++ =   ((LARc[5] & 0x3) << 6)
    | ((LARc[6] & 0x7) << 3)
    | (LARc[7] & 0x7);
  *c++ =   ((Nc[0] & 0x7F) << 1)
    | ((bc[0] >> 1) & 0x1);
  *c++ =   ((bc[0] & 0x1) << 7)
    | ((Mc[0] & 0x3) << 5)
    | ((xmaxc[0] >> 1) & 0x1F);
  *c++ =   ((xmaxc[0] & 0x1) << 7)
    | ((xmc[0] & 0x7) << 4)
    | ((xmc[1] & 0x7) << 1)
    | ((xmc[2] >> 2) & 0x1);
  *c++ =   ((xmc[2] & 0x3) << 6)
    | ((xmc[3] & 0x7) << 3)
    | (xmc[4] & 0x7);
  *c++ =   ((xmc[5] & 0x7) << 5)			/* 10 */
    | ((xmc[6] & 0x7) << 2)
    | ((xmc[7] >> 1) & 0x3);
  *c++ =   ((xmc[7] & 0x1) << 7)
    | ((xmc[8] & 0x7) << 4)
    | ((xmc[9] & 0x7) << 1)
    | ((xmc[10] >> 2) & 0x1);
  *c++ =   ((xmc[10] & 0x3) << 6)
    | ((xmc[11] & 0x7) << 3)
    | (xmc[12] & 0x7);
  *c++ =   ((Nc[1] & 0x7F) << 1)
    | ((bc[1] >> 1) & 0x1);
  *c++ =   ((bc[1] & 0x1) << 7)
    | ((Mc[1] & 0x3) << 5)
    | ((xmaxc[1] >> 1) & 0x1F);
  *c++ =   ((xmaxc[1] & 0x1) << 7)
    | ((xmc[13] & 0x7) << 4)
    | ((xmc[14] & 0x7) << 1)
    | ((xmc[15] >> 2) & 0x1);
  *c++ =   ((xmc[15] & 0x3) << 6)
    | ((xmc[16] & 0x7) << 3)
    | (xmc[17] & 0x7);
  *c++ =   ((xmc[18] & 0x7) << 5)
    | ((xmc[19] & 0x7) << 2)
    | ((xmc[20] >> 1) & 0x3);
  *c++ =   ((xmc[20] & 0x1) << 7)
    | ((xmc[21] & 0x7) << 4)
    | ((xmc[22] & 0x7) << 1)
    | ((xmc[23] >> 2) & 0x1);
  *c++ =   ((xmc[23] & 0x3) << 6)
    | ((xmc[24] & 0x7) << 3)
    | (xmc[25] & 0x7);
  *c++ =   ((Nc[2] & 0x7F) << 1)			/* 20 */
    | ((bc[2] >> 1) & 0x1);
  *c++ =   ((bc[2] & 0x1) << 7)
    | ((Mc[2] & 0x3) << 5)
    | ((xmaxc[2] >> 1) & 0x1F);
  *c++ =   ((xmaxc[2] & 0x1) << 7)
    | ((xmc[26] & 0x7) << 4)
    | ((xmc[27] & 0x7) << 1)
    | ((xmc[28] >> 2) & 0x1);
  *c++ =   ((xmc[28] & 0x3) << 6)
    | ((xmc[29] & 0x7) << 3)
    | (xmc[30] & 0x7);
  *c++ =   ((xmc[31] & 0x7) << 5)
    | ((xmc[32] & 0x7) << 2)
    | ((xmc[33] >> 1) & 0x3);
  *c++ =   ((xmc[33] & 0x1) << 7)
    | ((xmc[34] & 0x7) << 4)
    | ((xmc[35] & 0x7) << 1)
    | ((xmc[36] >> 2) & 0x1);
  *c++ =   ((xmc[36] & 0x3) << 6)
    | ((xmc[37] & 0x7) << 3)
    | (xmc[38] & 0x7);
  *c++ =   ((Nc[3] & 0x7F) << 1)
    | ((bc[3] >> 1) & 0x1);
  *c++ =   ((bc[3] & 0x1) << 7)
    | ((Mc[3] & 0x3) << 5)
    | ((xmaxc[3] >> 1) & 0x1F);
  *c++ =   ((xmaxc[3] & 0x1) << 7)
    | ((xmc[39] & 0x7) << 4)
    | ((xmc[40] & 0x7) << 1)
    | ((xmc[41] >> 2) & 0x1);
  *c++ =   ((xmc[41] & 0x3) << 6)			/* 30 */
    | ((xmc[42] & 0x7) << 3)
    | (xmc[43] & 0x7);
  *c++ =   ((xmc[44] & 0x7) << 5)
    | ((xmc[45] & 0x7) << 2)
    | ((xmc[46] >> 1) & 0x3);
  *c++ =   ((xmc[46] & 0x1) << 7)
    | ((xmc[47] & 0x7) << 4)
    | ((xmc[48] & 0x7) << 1)
    | ((xmc[49] >> 2) & 0x1);
  *c++ =   ((xmc[49] & 0x3) << 6)
    | ((xmc[50] & 0x7) << 3)
    | (xmc[51] & 0x7);

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

/* decode.c */
/*
 *  4.3 FIXED POINT IMPLEMENTATION OF THE RPE-LTP DECODER
 */

/* code.c */
void Gsm_Coder (

	struct gsm_state	* S,

	word	* s,	/* [0..159] samples		  	IN	*/

/*
 * The RPE-LTD coder works on a frame by frame basis.  The length of
 * the frame is equal to 160 samples.  Some computations are done
 * once per frame to produce at the output of the coder the
 * LARc[1..8] parameters which are the coded LAR coefficients and 
 * also to realize the inverse filtering operation for the entire
 * frame (160 samples of signal d[0..159]).  These parts produce at
 * the output of the coder:
 */

	word	* LARc,	/* [0..7] LAR coefficients		OUT	*/

/*
 * Procedure 4.2.11 to 4.2.18 are to be executed four times per
 * frame.  That means once for each sub-segment RPE-LTP analysis of
 * 40 samples.  These parts produce at the output of the coder:
 */

	word	* Nc,	/* [0..3] LTP lag			OUT 	*/
	word	* bc,	/* [0..3] coded LTP gain		OUT 	*/
	word	* Mc,	/* [0..3] RPE grid selection		OUT     */
	word	* xmaxc,/* [0..3] Coded maximum amplitude	OUT	*/
	word	* xMc	/* [13*4] normalized RPE samples	OUT	*/
)
{
  int	k;
  word	* dp  = S->dp0 + 120;	/* [ -120...-1 ] */
  word	* dpp = dp;		/* [ 0...39 ]	 */

  static word	e [50] = {0};

  word	so[160];

  Gsm_Preprocess			(S, s, so);
  Gsm_LPC_Analysis		(S, so, LARc);
  Gsm_Short_Term_Analysis_Filter	(S, LARc, so);

  _Pragma("loopbound min 4 max 4")
  for (k = 0; k <= 3; k++, xMc += 13) {

    Gsm_Long_Term_Predictor	( S,
	so+k*40, /* d      [0..39] IN	*/
	dp,	  /* dp  [-120..-1] IN	*/
	e + 5,	  /* e      [0..39] OUT	*/
	dpp,	  /* dpp    [0..39] OUT */
	Nc++,
	bc++);

    Gsm_RPE_Encoding	( S,
	e + 5,	/* e	  ][0..39][ IN/OUT */
	xmaxc++, Mc++, xMc );
    /*
     * Gsm_Update_of_reconstructed_short_time_residual_signal
     *			( dpp, e + 5, dp );
     */

    { int i;
      longword ltmp;
      _Pragma("loopbound min 40 max 40")
      for (i = 0; i <= 39; i++) {
	dp[ i ] = GSM_ADD( e[5 + i], dpp[i] );
      }
    }

    dp  += 40;
    dpp += 40;

  }
//  //(void)memcpy( (char *)S->dp0, (char *)(S->dp0 + 160),
//  //	120 * sizeof(*S->dp0) );
}

/* rpe.c */
/*  4.2.13 .. 4.2.17  RPE ENCODING SECTION
 */

/* 4.2.13 */

static void Weighting_filter (
	word	* e,		/* signal [-5..0.39.44]	IN  */
	word		* x		/* signal [0..39]	OUT */
)
/*
 *  The coefficients of the weighting filter are stored in a table
 *  (see table 4.4).  The following scaling is used:
 *
 *	H[0..10] = integer( real_H[ 0..10] * 8192 ); 
 */
{
  /* word			wt[ 50 ]; */

  longword	L_result;
  int		k /* , i */ ;

  /*  Initialization of a temporary working array wt[0...49]
  */

  /* for (k =  0; k <=  4; k++) wt[k] = 0;
   * for (k =  5; k <= 44; k++) wt[k] = *e++;
   * for (k = 45; k <= 49; k++) wt[k] = 0;
   *
   *  (e[-5..-1] and e[40..44] are allocated by the caller,
   *  are initially zero and are not written anywhere.)
   */
  e -= 5;

  /*  Compute the signal x[0..39]
  */ 
  _Pragma("loopbound min 40 max 40")
  for (k = 0; k <= 39; k++) {

    L_result = 8192 >> 1;

    /* for (i = 0; i <= 10; i++) {
     *	L_temp   = GSM_L_MULT( wt[k+i], gsm_H[i] );
     *	L_result = GSM_L_ADD( L_result, L_temp );
     * }
     */

#undef	STEP
#define	STEP( i, H )	(e[ k + i ] * (longword)H)

    /*  Every one of these multiplications is done twice --
     *  but I don't see an elegant way to optimize this. 
     *  Do you?
     */

#ifdef	STUPID_COMPILER
    L_result += STEP(	0, 	-134 ) ;
    L_result += STEP(	1, 	-374 )  ;
    /* + STEP(	2, 	0    )  */
    L_result += STEP(	3, 	2054 ) ;
    L_result += STEP(	4, 	5741 ) ;
    L_result += STEP(	5, 	8192 ) ;
    L_result += STEP(	6, 	5741 ) ;
    L_result += STEP(	7, 	2054 ) ;
    /* + STEP(	8, 	0    )  */
    L_result += STEP(	9, 	-374 ) ;
    L_result += STEP(	10, 	-134 ) ;
#else
    L_result +=
      STEP(	0, 	-134 ) 
      + STEP(	1, 	-374 ) 
      /* + STEP(	2, 	0    )  */
      + STEP(	3, 	2054 ) 
      + STEP(	4, 	5741 ) 
      + STEP(	5, 	8192 ) 
      + STEP(	6, 	5741 ) 
      + STEP(	7, 	2054 ) 
      /* + STEP(	8, 	0    )  */
      + STEP(	9, 	-374 ) 
      + STEP(10, 	-134 )
      ;
#endif
    /* L_result = GSM_L_ADD( L_result, L_result ); (* scaling(x2) *)
     * L_result = GSM_L_ADD( L_result, L_result ); (* scaling(x4) *)
     *
     * x[k] = SASR( L_result, 16 );
     */

    /* 2 adds vs. >>16 => 14, minus one shift to compensate for
     * those we lost when replacing L_MULT by '*'.
     */

    L_result = SASR( L_result, 13 );
    x[k] =  (  L_result < MIN_WORD ? MIN_WORD
	: (L_result > MAX_WORD ? MAX_WORD : L_result ));
  }
}

/* 4.2.14 */

static void RPE_grid_selection (
	word		* x,		/* [0..39]		IN  */ 
	word		* xM,		/* [0..12]		OUT */
	word		* Mc_out	/*			OUT */
)
/*
 *  The signal x[0..39] is used to select the RPE grid which is
 *  represented by Mc.
 */
{
  /* word	temp1;	*/
  int		/* m, */  i;
  longword	L_result, L_temp;
  longword		EM;	/* xxx should be L_EM? */
  word			Mc;

  longword		L_common_0_3;

  EM = 0;
  Mc = 0;

  /* for (m = 0; m <= 3; m++) {
   *	L_result = 0;
   *
   *
   *	for (i = 0; i <= 12; i++) {
   *
   *		temp1    = SASR( x[m + 3*i], 2 );
   *
   *
   *		L_temp   = GSM_L_MULT( temp1, temp1 );
   *		L_result = GSM_L_ADD( L_temp, L_result );
   *	}
   * 
   *	if (L_result > EM) {
   *		Mc = m;
   *		EM = L_result;
   *	}
   * }
   */

#undef	STEP
#define	STEP( m, i )		L_temp = SASR( x[m + 3 * i], 2 );	\
  L_result += L_temp * L_temp;

  /* common part of 0 and 3 */

  L_result = 0;
  STEP( 0, 1 ); STEP( 0, 2 ); STEP( 0, 3 ); STEP( 0, 4 );
  STEP( 0, 5 ); STEP( 0, 6 ); STEP( 0, 7 ); STEP( 0, 8 );
  STEP( 0, 9 ); STEP( 0, 10); STEP( 0, 11); STEP( 0, 12);
  L_common_0_3 = L_result;

  /* i = 0 */

  STEP( 0, 0 );
  L_result <<= 1;	/* implicit in L_MULT */
  EM = L_result;

  /* i = 1 */

  L_result = 0;
  STEP( 1, 0 );
  STEP( 1, 1 ); STEP( 1, 2 ); STEP( 1, 3 ); STEP( 1, 4 );
  STEP( 1, 5 ); STEP( 1, 6 ); STEP( 1, 7 ); STEP( 1, 8 );
  STEP( 1, 9 ); STEP( 1, 10); STEP( 1, 11); STEP( 1, 12);
  L_result <<= 1;
  if (L_result > EM) {
    Mc = 1;
    EM = L_result;
  }

  /* i = 2 */

  L_result = 0;
  STEP( 2, 0 );
  STEP( 2, 1 ); STEP( 2, 2 ); STEP( 2, 3 ); STEP( 2, 4 );
  STEP( 2, 5 ); STEP( 2, 6 ); STEP( 2, 7 ); STEP( 2, 8 );
  STEP( 2, 9 ); STEP( 2, 10); STEP( 2, 11); STEP( 2, 12);
  L_result <<= 1;
  if (L_result > EM) {
    Mc = 2;
    EM = L_result;
  }

  /* i = 3 */

  L_result = L_common_0_3;
  STEP( 3, 12 );
  L_result <<= 1;
  if (L_result > EM) {
    Mc = 3;
    EM = L_result;
  }

  /**/

  /*  Down-sampling by a factor 3 to get the selected xM[0..12]
   *  RPE sequence.
   */
  _Pragma("loopbound min 13 max 13")
  for (i = 0; i <= 12; i ++) xM[i] = x[Mc + 3*i];
  *Mc_out = Mc;

}

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

static void APCM_quantization (
	word		* xM,		/* [0..12]		IN	*/

	word		* xMc,		/* [0..12]		OUT	*/
	word		* mant_out,	/* 			OUT	*/
	word		* exp_out,	/*			OUT	*/
	word		* xmaxc_out	/*			OUT	*/
)
{
  int	i, itest;

  word	xmax, xmaxc, temp, temp1, temp2;
  word	exp, mant;


  /*  Find the maximum absolute value xmax of xM[0..12].
  */

  xmax = 0;

  _Pragma("loopbound min 13 max 13")
    for (i = 0; i <= 12; i++) {
      temp = xM[i];
      temp = GSM_ABS(temp);
      if (temp > xmax) xmax = temp;
    }

  /*  Qantizing and coding of xmax to get xmaxc.
  */

  exp   = 0;
  temp  = SASR( xmax, 9 );
  itest = 0;

  _Pragma("loopbound min 6 max 6")
    for (i = 0; i <= 5; i++) {

      itest |= (temp <= 0);
      temp = SASR( temp, 1 );

      if (itest == 0) exp++;		// exp = add (exp, 1) 
    }

  temp = exp + 5;

  //xmaxc = gsm_add( SASR(xmax, temp), exp << 3 );
  xmaxc = saturate( ( SASR(xmax, temp) + exp << 3  ));

  /*   Quantizing and coding of the xM[0..12] RPE sequence
   *   to get the xMc[0..12]
   */

  APCM_quantization_xmaxc_to_exp_mant( xmaxc, &exp, &mant );

  /*  This computation uses the fact that the decoded version of xmaxc
   *  can be calculated by using the exponent and the mantissa part of
   *  xmaxc (logarithmic table).
   *  So, this method avoids any division and uses only a scaling
   *  of the RPE samples by a function of the exponent.  A direct 
   *  multiplication by the inverse of the mantissa (NRFAC[0..7]
   *  found in table 4.5) gives the 3 bit coded version xMc[0..12]
   *  of the RPE samples.
   */


  /* Direct computation of xMc[0..12] using table 4.5
  */


  temp1 = 6 - exp;		/* normalization by the exponent */
  temp2 = gsm_NRFAC[ mant ];  	/* inverse mantissa 		 */

  _Pragma("loopbound min 13 max 13")
    for (i = 0; i <= 12; i++) {

      temp = xM[i] << temp1;
      temp = GSM_MULT( temp, temp2 );
      temp = SASR(temp, 12);
      xMc[i] = temp + 4;		/* see note below */
    }

  /*  NOTE: This equation is used to make all the xMc[i] positive.
  */

  *mant_out  = mant;
  *exp_out   = exp;
  *xmaxc_out = xmaxc;

}

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

  _Pragma("loopbound min 0 max 3")
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

void Gsm_RPE_Encoding (

	struct gsm_state * S,

	word	* e,		/* -5..-1][0..39][40..44	IN/OUT  */
	word	* xmaxc,	/* 				OUT */
	word	* Mc,		/* 			  	OUT */
	word	* xMc)		/* [0..12]			OUT */
{
  word	x[40];
  word	xM[13], xMp[13];
  word	mant, exp;

  Weighting_filter(e, x);
  RPE_grid_selection(x, xM, Mc);

  APCM_quantization( xM, xMc, &mant, &exp, xmaxc);
  APCM_inverse_quantization(  xMc,  mant,  exp, xMp);

  RPE_grid_positioning( *Mc, xMp, e );

}

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

static void Calculation_of_the_LTP_parameters (
	word	* d,		/* [0..39]	IN	*/
	word	* dp,		/* [-120..-1]	IN	*/
	word		* bc_out,	/* 		OUT	*/
	word		* Nc_out	/* 		OUT	*/
)
{
  int  	k, lambda;
  word		Nc, bc;
  word		wt[40];

  longword	L_max, L_power;
  word		R, S, dmax, scal;
  word	temp;

  /*  Search of the optimum scaling of d[0..39].
  */
  dmax = 0;

  _Pragma("loopbound min 40 max 40")
  for (k = 0; k <= 39; k++) {
    temp = d[k];
    temp = GSM_ABS( temp );
    if (temp > dmax) dmax = temp;
  }

  temp = 0;
  if (dmax == 0) scal = 0;
  else {
    temp = gsm_norm( (longword)dmax << 16 );
  }

  if (temp > 6) scal = 0;
  else scal = 6 - temp;


  /*  Initialization of a working array wt
  */

  _Pragma("loopbound min 40 max 40")
  for (k = 0; k <= 39; k++) wt[k] = SASR( d[k], scal );

  /* Search for the maximum cross-correlation and coding of the LTP lag
  */
  L_max = 0;
  Nc    = 40;	/* index for the maximum cross-correlation */

  _Pragma("loopbound min 81 max 81")
  for (lambda = 40; lambda <= 120; lambda++) {

# undef STEP
# ifdef USE_TABLE_MUL
#		define STEP(k) (table_mul(wt[k], dp[k - lambda]))
# else
#		define STEP(k) (wt[k] * dp[k - lambda])
# endif

    longword L_result;

    L_result  = STEP(0)  ; L_result += STEP(1) ;
    L_result += STEP(2)  ; L_result += STEP(3) ;
    L_result += STEP(4)  ; L_result += STEP(5)  ;
    L_result += STEP(6)  ; L_result += STEP(7)  ;
    L_result += STEP(8)  ; L_result += STEP(9)  ;
    L_result += STEP(10) ; L_result += STEP(11) ;
    L_result += STEP(12) ; L_result += STEP(13) ;
    L_result += STEP(14) ; L_result += STEP(15) ;
    L_result += STEP(16) ; L_result += STEP(17) ;
    L_result += STEP(18) ; L_result += STEP(19) ;
    L_result += STEP(20) ; L_result += STEP(21) ;
    L_result += STEP(22) ; L_result += STEP(23) ;
    L_result += STEP(24) ; L_result += STEP(25) ;
    L_result += STEP(26) ; L_result += STEP(27) ;
    L_result += STEP(28) ; L_result += STEP(29) ;
    L_result += STEP(30) ; L_result += STEP(31) ;
    L_result += STEP(32) ; L_result += STEP(33) ;
    L_result += STEP(34) ; L_result += STEP(35) ;
    L_result += STEP(36) ; L_result += STEP(37) ;
    L_result += STEP(38) ; L_result += STEP(39) ;

    if (L_result > L_max) {

      Nc    = lambda;
      L_max = L_result;
    }
  }

  *Nc_out = Nc;

  L_max <<= 1;

  /*  Rescaling of L_max
  */
  L_max = L_max >> (6 - scal);	/* sub(6, scal) */

  /*   Compute the power of the reconstructed short term residual
   *   signal dp[..]
   */
  L_power = 0;
  _Pragma("loopbound min 40 max 40")
  for (k = 0; k <= 39; k++) {

    longword L_temp;

    L_temp   = SASR( dp[k - Nc], 3 );
    L_power += L_temp * L_temp;
  }
  L_power <<= 1;	/* from L_MULT */

  /*  Normalization of L_max and L_power
  */

  if (L_max <= 0)  {
    *bc_out = 0;
    return;
  }
  if (L_max >= L_power) {
    *bc_out = 3;
    return;
  }

  temp = gsm_norm( L_power );

  R = SASR( L_max   << temp, 16 );
  S = SASR( L_power << temp, 16 );

  /*  Coding of the LTP gain
  */

  /*  Table 4.3a must be used to obtain the level DLB[i] for the
   *  quantization of the LTP gain b to get the coded version bc.
   */
  _Pragma("loopbound min 3 max 3")
  for (bc = 0; bc <= 2; bc++) 
   /* Replaced by macro function. */
   //if (R <= gsm_mult(S, gsm_DLB[bc])) 	
   if (R <= GSM_MULT(S, gsm_DLB[bc])) 	
    break;

  *bc_out = bc;
}

#else	/* USE_FLOAT_MUL */

static void Calculation_of_the_LTP_parameters (
	word	* d,		/* [0..39]	IN	*/
	word	* dp,		/* [-120..-1]	IN	*/
	word		* bc_out,	/* 		OUT	*/
	word		* Nc_out	/* 		OUT	*/
)
{
  int  	k, lambda;
  word		Nc, bc;

  float		wt_float[40];
  float		dp_float_base[120], * dp_float = dp_float_base + 120;

  longword	L_max, L_power;
  word		R, S, dmax, scal;
  word	temp;

  /*  Search of the optimum scaling of d[0..39].
  */
  dmax = 0;

  for (k = 0; k <= 39; k++) {
    temp = d[k];
    temp = GSM_ABS( temp );
    if (temp > dmax) dmax = temp;
  }

  temp = 0;
  if (dmax == 0) scal = 0;
  else {
    temp = gsm_norm( (longword)dmax << 16 );
  }

  if (temp > 6) scal = 0;
  else scal = 6 - temp;

  /*  Initialization of a working array wt
  */

  for (k =    0; k < 40; k++) wt_float[k] =  SASR( d[k], scal );
  for (k = -120; k <  0; k++) dp_float[k] =  dp[k];

  /* Search for the maximum cross-correlation and coding of the LTP lag
  */
  L_max = 0;
  Nc    = 40;	/* index for the maximum cross-correlation */

  for (lambda = 40; lambda <= 120; lambda += 9) {

    /*  Calculate L_result for l = lambda .. lambda + 9.
    */
    float *lp = dp_float - lambda;

    float	W;
    float	a = lp[-8], b = lp[-7], c = lp[-6],
	     d = lp[-5], e = lp[-4], f = lp[-3],
	     g = lp[-2], h = lp[-1];
    float  E; 
    float  S0 = 0, S1 = 0, S2 = 0, S3 = 0, S4 = 0,
	     S5 = 0, S6 = 0, S7 = 0, S8 = 0;

#		undef STEP
#		define	STEP(K, a, b, c, d, e, f, g, h) \
    W = wt_float[K];		\
    E = W * a; S8 += E;		\
    E = W * b; S7 += E;		\
    E = W * c; S6 += E;		\
    E = W * d; S5 += E;		\
    E = W * e; S4 += E;		\
    E = W * f; S3 += E;		\
    E = W * g; S2 += E;		\
    E = W * h; S1 += E;		\
    a  = lp[K];			\
    E = W * a; S0 += E

#		define	STEP_A(K)	STEP(K, a, b, c, d, e, f, g, h)
#		define	STEP_B(K)	STEP(K, b, c, d, e, f, g, h, a)
#		define	STEP_C(K)	STEP(K, c, d, e, f, g, h, a, b)
#		define	STEP_D(K)	STEP(K, d, e, f, g, h, a, b, c)
#		define	STEP_E(K)	STEP(K, e, f, g, h, a, b, c, d)
#		define	STEP_F(K)	STEP(K, f, g, h, a, b, c, d, e)
#		define	STEP_G(K)	STEP(K, g, h, a, b, c, d, e, f)
#		define	STEP_H(K)	STEP(K, h, a, b, c, d, e, f, g)

    STEP_A( 0); STEP_B( 1); STEP_C( 2); STEP_D( 3);
    STEP_E( 4); STEP_F( 5); STEP_G( 6); STEP_H( 7);

    STEP_A( 8); STEP_B( 9); STEP_C(10); STEP_D(11);
    STEP_E(12); STEP_F(13); STEP_G(14); STEP_H(15);

    STEP_A(16); STEP_B(17); STEP_C(18); STEP_D(19);
    STEP_E(20); STEP_F(21); STEP_G(22); STEP_H(23);

    STEP_A(24); STEP_B(25); STEP_C(26); STEP_D(27);
    STEP_E(28); STEP_F(29); STEP_G(30); STEP_H(31);

    STEP_A(32); STEP_B(33); STEP_C(34); STEP_D(35);
    STEP_E(36); STEP_F(37); STEP_G(38); STEP_H(39);

    if (S0 > L_max) { L_max = S0; Nc = lambda;     }
    if (S1 > L_max) { L_max = S1; Nc = lambda + 1; }
    if (S2 > L_max) { L_max = S2; Nc = lambda + 2; }
    if (S3 > L_max) { L_max = S3; Nc = lambda + 3; }
    if (S4 > L_max) { L_max = S4; Nc = lambda + 4; }
    if (S5 > L_max) { L_max = S5; Nc = lambda + 5; }
    if (S6 > L_max) { L_max = S6; Nc = lambda + 6; }
    if (S7 > L_max) { L_max = S7; Nc = lambda + 7; }
    if (S8 > L_max) { L_max = S8; Nc = lambda + 8; }
  }
  *Nc_out = Nc;

  L_max <<= 1;

  /*  Rescaling of L_max
  */
  L_max = L_max >> (6 - scal);	/* sub(6, scal) */

  /*   Compute the power of the reconstructed short term residual
   *   signal dp[..]
   */
  L_power = 0;
  for (k = 0; k <= 39; k++) {

    longword L_temp;

    L_temp   = SASR( dp[k - Nc], 3 );
    L_power += L_temp * L_temp;
  }
  L_power <<= 1;	/* from L_MULT */

  /*  Normalization of L_max and L_power
  */

  if (L_max <= 0)  {
    *bc_out = 0;
    return;
  }
  if (L_max >= L_power) {
    *bc_out = 3;
    return;
  }

  temp = gsm_norm( L_power );

  R = SASR( L_max   << temp, 16 );
  S = SASR( L_power << temp, 16 );

  /*  Coding of the LTP gain
  */

  /*  Table 4.3a must be used to obtain the level DLB[i] for the
   *  quantization of the LTP gain b to get the coded version bc.
   */
  // Replaced by macro function.
  //for (bc = 0; bc <= 2; bc++) if (R <= gsm_mult(S, gsm_DLB[bc])) break;
  for (bc = 0; bc <= 2; bc++) if (R <= GSM_MULT(S, gsm_DLB[bc])) break;
  *bc_out = bc;
}

#ifdef	FAST

static void Fast_Calculation_of_the_LTP_parameters P4((d,dp,bc_out,Nc_out),
	word	* d,		/* [0..39]	IN	*/
	word	* dp,		/* [-120..-1]	IN	*/
	word		* bc_out,	/* 		OUT	*/
	word		* Nc_out	/* 		OUT	*/
)
{
  int  	k, lambda;
  word		Nc, bc;

  float		wt_float[40];
  float		dp_float_base[120], * dp_float = dp_float_base + 120;

  float	L_max, L_power;

  for (k = 0; k < 40; ++k) wt_float[k] = (float)d[k];
  for (k = -120; k <= 0; ++k) dp_float[k] = (float)dp[k];

  /* Search for the maximum cross-correlation and coding of the LTP lag
  */
  L_max = 0;
  Nc    = 40;	/* index for the maximum cross-correlation */

  for (lambda = 40; lambda <= 120; lambda += 9) {

    /*  Calculate L_result for l = lambda .. lambda + 9.
    */
    float *lp = dp_float - lambda;

    float	W;
    float	a = lp[-8], b = lp[-7], c = lp[-6],
	     d = lp[-5], e = lp[-4], f = lp[-3],
	     g = lp[-2], h = lp[-1];
    float  E; 
    float  S0 = 0, S1 = 0, S2 = 0, S3 = 0, S4 = 0,
	     S5 = 0, S6 = 0, S7 = 0, S8 = 0;

#		undef STEP
#		define	STEP(K, a, b, c, d, e, f, g, h) \
    W = wt_float[K];		\
    E = W * a; S8 += E;		\
    E = W * b; S7 += E;		\
    E = W * c; S6 += E;		\
    E = W * d; S5 += E;		\
    E = W * e; S4 += E;		\
    E = W * f; S3 += E;		\
    E = W * g; S2 += E;		\
    E = W * h; S1 += E;		\
    a  = lp[K];			\
    E = W * a; S0 += E

#		define	STEP_A(K)	STEP(K, a, b, c, d, e, f, g, h)
#		define	STEP_B(K)	STEP(K, b, c, d, e, f, g, h, a)
#		define	STEP_C(K)	STEP(K, c, d, e, f, g, h, a, b)
#		define	STEP_D(K)	STEP(K, d, e, f, g, h, a, b, c)
#		define	STEP_E(K)	STEP(K, e, f, g, h, a, b, c, d)
#		define	STEP_F(K)	STEP(K, f, g, h, a, b, c, d, e)
#		define	STEP_G(K)	STEP(K, g, h, a, b, c, d, e, f)
#		define	STEP_H(K)	STEP(K, h, a, b, c, d, e, f, g)

    STEP_A( 0); STEP_B( 1); STEP_C( 2); STEP_D( 3);
    STEP_E( 4); STEP_F( 5); STEP_G( 6); STEP_H( 7);

    STEP_A( 8); STEP_B( 9); STEP_C(10); STEP_D(11);
    STEP_E(12); STEP_F(13); STEP_G(14); STEP_H(15);

    STEP_A(16); STEP_B(17); STEP_C(18); STEP_D(19);
    STEP_E(20); STEP_F(21); STEP_G(22); STEP_H(23);

    STEP_A(24); STEP_B(25); STEP_C(26); STEP_D(27);
    STEP_E(28); STEP_F(29); STEP_G(30); STEP_H(31);

    STEP_A(32); STEP_B(33); STEP_C(34); STEP_D(35);
    STEP_E(36); STEP_F(37); STEP_G(38); STEP_H(39);

    if (S0 > L_max) { L_max = S0; Nc = lambda;     }
    if (S1 > L_max) { L_max = S1; Nc = lambda + 1; }
    if (S2 > L_max) { L_max = S2; Nc = lambda + 2; }
    if (S3 > L_max) { L_max = S3; Nc = lambda + 3; }
    if (S4 > L_max) { L_max = S4; Nc = lambda + 4; }
    if (S5 > L_max) { L_max = S5; Nc = lambda + 5; }
    if (S6 > L_max) { L_max = S6; Nc = lambda + 6; }
    if (S7 > L_max) { L_max = S7; Nc = lambda + 7; }
    if (S8 > L_max) { L_max = S8; Nc = lambda + 8; }
  }
  *Nc_out = Nc;

  if (L_max <= 0.)  {
    *bc_out = 0;
    return;
  }

  /*  Compute the power of the reconstructed short term residual
   *  signal dp[..]
   */
  dp_float -= Nc;
  L_power = 0;
  for (k = 0; k < 40; ++k) {
    float f = dp_float[k];
    L_power += f * f;
  }

  if (L_max >= L_power) {
    *bc_out = 3;
    return;
  }

  /*  Coding of the LTP gain
   *  Table 4.3a must be used to obtain the level DLB[i] for the
   *  quantization of the LTP gain b to get the coded version bc.
   */
  lambda = L_max / L_power * 32768.;
  for (bc = 0; bc <= 2; ++bc) if (lambda <= gsm_DLB[bc]) break;
  *bc_out = bc;
}

#endif	/* FAST 	 */
#endif	/* USE_FLOAT_MUL */


/* 4.2.12 */

static void Long_term_analysis_filtering (
	word		bc,	/* 					IN  */
	word		Nc,	/* 					IN  */
	word	* dp,	/* previous d	[-120..-1]		IN  */
	word	* d,	/* d		[0..39]			IN  */
	word	* dpp,	/* estimate	[0..39]			OUT */
	word	* e	/* long term res. signal [0..39]	OUT */
)
/*
 *  In this part, we have to decode the bc parameter to compute
 *  the samples of the estimate dpp[0..39].  The decoding of bc needs the
 *  use of table 4.3b.  The long term residual signal e[0..39]
 *  is then calculated to be fed to the RPE encoding section.
 */
{
  int      k;
  longword ltmp;

#	undef STEP
#	define STEP(BP)					\
  _Pragma("loopbound min 40 max 40") \
  for (k = 0; k <= 39; k++) {			\
    dpp[k]  = GSM_MULT_R( BP, dp[k - Nc]);	\
    e[k]	= GSM_SUB( d[k], dpp[k] );	\
  }

  switch (bc) {
    case 0:	STEP(  3277 ); break;
    case 1:	STEP( 11469 ); break;
    case 2: STEP( 21299 ); break;
    case 3: STEP( 32767 ); break; 
  }
}

void Gsm_Long_Term_Predictor (

	struct gsm_state	* S,

	word	* d,	/* [0..39]   residual signal	IN	*/
	word	* dp,	/* [-120..-1] d'		IN	*/

	word	* e,	/* [0..39] 			OUT	*/
	word	* dpp,	/* [0..39] 			OUT	*/
	word	* Nc,	/* correlation lag		OUT	*/
	word	* bc	/* gain factor			OUT	*/
)
{

#if defined(FAST) && defined(USE_FLOAT_MUL)
  if (S->fast) 
    Fast_Calculation_of_the_LTP_parameters( d, dp, bc, Nc );
  else 
#endif
    Calculation_of_the_LTP_parameters( d, dp, bc, Nc );

  Long_term_analysis_filtering( *bc, *Nc, dp, d, dpp, e );
}

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


/*   Table 4.5 	 Normalized inverse mantissa used to compute xM/xmax 
 */
/* i		 	0        1    2      3      4      5     6      7   */
word gsm_NRFAC[8] = { 29128, 26215, 23832, 21846, 20165, 18725, 17476, 16384 };


/*   Table 4.6	 Normalized direct mantissa used to compute xM/xmax
 */
/* i                  0      1       2      3      4      5      6      7   */
word gsm_FAC[8]	= { 18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767 };

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


/* 4.2.10 */
static void Short_term_analysis_filtering (
	struct gsm_state * S,
	word	* rp,	/* [0..7]	IN	*/
	int 	k_n, 	/*   k_end - k_start	*/
	word	* s	/* [0..n-1]	IN/OUT	*/
)
/*
 *  This procedure computes the short term residual signal d[..] to be fed
 *  to the RPE-LTP loop from the s[..] signal and from the local rp[..]
 *  array (quantized reflection coefficients).  As the call of this
 *  procedure can be done in many ways (see the interpolation of the LAR
 *  coefficient), it is assumed that the computation begins with index
 *  k_start (for arrays d[..] and s[..]) and stops with index k_end
 *  (k_start and k_end are defined in 4.2.9.1).  This procedure also
 *  needs to keep the array u[0..7] in memory for each call.
 */
{
  word		* u = S->u;
  int		i;
  word		di, zzz, ui, sav, rpi;
  longword 	ltmp;
  int j;

  _Pragma("loopbound min 13 max 120")
  for (j=0; j<k_n; ++j) {
    ++s;

    di = sav = *s;

    _Pragma("loopbound min 8 max 8")
    for (i = 0; i < 8; i++) {		/* YYY */

      ui    = u[i];
      rpi   = rp[i];
      u[i]  = sav;

      zzz   = GSM_MULT_R(rpi, di);
      sav   = GSM_ADD(   ui,  zzz);

      zzz   = GSM_MULT_R(rpi, ui);
      di    = GSM_ADD(   di,  zzz );

    }

    *s = di;
  }
}

#if defined(USE_FLOAT_MUL) && defined(FAST)

static void Fast_Short_term_analysis_filtering (
	struct gsm_state * S,
	word	* rp,	/* [0..7]	IN	*/
	int 	k_n, 	/*   k_end - k_start	*/
	word	* s	/* [0..n-1]	IN/OUT	*/
)
{
  word		* u = S->u;
  int		i;

  float 	  uf[8],
		  rpf[8];

  float scalef = 3.0517578125e-5;
  float		sav, di, temp;

  for (i = 0; i < 8; ++i) {
    uf[i]  = u[i];
    rpf[i] = rp[i] * scalef;
  }
  for (; k_n--; s++) {
    sav = di = *s;
    for (i = 0; i < 8; ++i) {
      float rpfi = rpf[i];
      float ufi  = uf[i];

      uf[i] = sav;
      temp  = rpfi * di + ufi;
      di   += rpfi * ufi;
      sav   = temp;
    }
    *s = di;
  }
  for (i = 0; i < 8; ++i) u[i] = uf[i];
}
#endif /* ! (defined (USE_FLOAT_MUL) && defined (FAST)) */

#if defined(FAST) && defined(USE_FLOAT_MUL)

#endif /* defined(FAST) && defined(USE_FLOAT_MUL) */

void Gsm_Short_Term_Analysis_Filter (

	struct gsm_state * S,

	word	* LARc,		/* coded log area ratio [0..7]  IN	*/
	word	* s		/* signal [0..159]		IN/OUT	*/
)
{
  word		* LARpp_j	= S->LARpp[ S->j      ];
  word		* LARpp_j_1	= S->LARpp[ S->j ^= 1 ];

  word		LARp[8];

#undef	FILTER
#if 	defined(FAST) && defined(USE_FLOAT_MUL)
# 	define	FILTER 	(* (S->fast			\
      ? Fast_Short_term_analysis_filtering	\
      : Short_term_analysis_filtering	))

#else
# 	define	FILTER	Short_term_analysis_filtering
#endif

  Decoding_of_the_coded_Log_Area_Ratios( LARc, LARpp_j );

  Coefficients_0_12(  LARpp_j_1, LARpp_j, LARp );
  LARp_to_rp( LARp );
  FILTER( S, LARp, 13, s);

  Coefficients_13_26( LARpp_j_1, LARpp_j, LARp);
  LARp_to_rp( LARp );
  FILTER( S, LARp, 14, s + 13);

  Coefficients_27_39( LARpp_j_1, LARpp_j, LARp);
  LARp_to_rp( LARp );
  FILTER( S, LARp, 13, s + 27);

  Coefficients_40_159( LARpp_j, LARp);
  LARp_to_rp( LARp );
  FILTER( S, LARp, 120, s + 40);
}

/* lpc.c */
#undef	P

/*
 *  4.2.4 .. 4.2.7 LPC ANALYSIS SECTION
 */

/* 4.2.4 */


static void Autocorrelation (
	word     * s,		/* [0..159]	IN/OUT  */
 	longword * L_ACF)	/* [0..8]	OUT     */
/*
 *  The goal is to compute the array L_ACF[k].  The signal s[i] must
 *  be scaled in order to avoid an overflow situation.
 */
{
	int	k, i;

	word		temp, smax, scalauto;

#ifdef	USE_FLOAT_MUL
	float		float_s[160];
#endif

	/*  Dynamic scaling of the array  s[0..159]
	 */

	/*  Search for the maximum.
	 */
	smax = 0;

	_Pragma("loopbound min 160 max 160")
	for (k = 0; k <= 159; k++) {
		temp = GSM_ABS( s[k] );
		if (temp > smax) smax = temp;
	}

	/*  Computation of the scaling factor.
	 */
	if (smax == 0) scalauto = 0;
	else {
		scalauto = 4 - gsm_norm( (longword)smax << 16 );/* sub(4,..) */
	}

	/*  Scaling of the array s[0...159]
	 */

	if (scalauto > 0) {

# ifdef USE_FLOAT_MUL
#   define SCALE(n)	\
	case n: \
	  _Pragma("loopbound min 160 max 160") \
	  for (k = 0; k <= 159; k++) \
			float_s[k] = (float)	\
				(s[k] = GSM_MULT_R(s[k], 16384 >> (n-1)));\
		break;
# else 
#   define SCALE(n)	\
	case n: \
		_Pragma("loopbound min 160 max 160") \
		for (k = 0; k <= 159; k++) \
			s[k] = GSM_MULT_R( s[k], 16384 >> (n-1) );\
		break;
# endif /* USE_FLOAT_MUL */

		switch (scalauto) {
		SCALE(1)
		SCALE(2)
		SCALE(3)
		SCALE(4)
		}
# undef	SCALE
	}
# ifdef	USE_FLOAT_MUL
	else {
	  _Pragma("loopbound min 160 max 160")
	  for (k = 0; k <= 159; k++) float_s[k] = (float) s[k];
	}
# endif

	/*  Compute the L_ACF[..].
	 */
	{
# ifdef	USE_FLOAT_MUL
		float * sp = float_s;
		float   sl = *sp;

#		define STEP(k)	 L_ACF[k] += (longword)(sl * sp[ -(k) ]);
# else
		word  * sp = s;
		word    sl = *sp;
#undef STEP
#		define STEP(k)	 L_ACF[k] += ((longword)sl * sp[ -(k) ]);
# endif

#	define NEXTI	 sl = *++sp


	_Pragma("loopbound min 9 max 9")
	for (k = 9; k--; L_ACF[k] = 0) ;

	STEP (0);
	NEXTI;
	STEP(0); STEP(1);
	NEXTI;
	STEP(0); STEP(1); STEP(2);
	NEXTI;
	STEP(0); STEP(1); STEP(2); STEP(3);
	NEXTI;
	STEP(0); STEP(1); STEP(2); STEP(3); STEP(4);
	NEXTI;
	STEP(0); STEP(1); STEP(2); STEP(3); STEP(4); STEP(5);
	NEXTI;
	STEP(0); STEP(1); STEP(2); STEP(3); STEP(4); STEP(5); STEP(6);
	NEXTI;
	STEP(0); STEP(1); STEP(2); STEP(3); STEP(4); STEP(5); STEP(6); STEP(7);

	_Pragma("loopbound min 152 max 152")
	for (i = 8; i <= 159; i++) {

		NEXTI;

		STEP(0);
		STEP(1); STEP(2); STEP(3); STEP(4);
		STEP(5); STEP(6); STEP(7); STEP(8);
	}

	_Pragma("loopbound min 9 max 9")
	for (k = 9; k--; L_ACF[k] <<= 1) ; 

	}
	/*   Rescaling of the array s[0..159]
	 */
	if (scalauto > 0) {
		_Pragma("loopbound min 160 max 160")
		for (k = 160; k--; *s++ <<= scalauto) ;
	}
}

#if defined(USE_FLOAT_MUL) && defined(FAST)

static void Fast_Autocorrelation (
	word * s,		/* [0..159]	IN/OUT  */
 	longword * L_ACF)	/* [0..8]	OUT     */
{
  int	k, i;
  float f_L_ACF[9];
  float scale;

  float          s_f[160];
  float *sf = s_f;

  for (i = 0; i < 160; ++i) sf[i] = s[i];
  for (k = 0; k <= 8; k++) {
    float L_temp2 = 0;
    float *sfl = sf - k;
    for (i = k; i < 160; ++i) L_temp2 += sf[i] * sfl[i];
    f_L_ACF[k] = L_temp2;
  }
  scale = MAX_LONGWORD / f_L_ACF[0];

  for (k = 0; k <= 8; k++) {
    L_ACF[k] = f_L_ACF[k] * scale;
  }
}
#endif	/* defined (USE_FLOAT_MUL) && defined (FAST) */

/* 4.2.5 */

static void Reflection_coefficients ( 
	longword	* L_ACF,		/* 0...8	IN	*/
	word	* r			/* 0...7	OUT 	*/
)
{
  int	i, m, n;
  word	temp;
  longword ltmp;
  word		ACF[9];	/* 0..8 */
  word		P[  9];	/* 0..8 */
  word		K[  9]; /* 2..8 */

  /*  Schur recursion with 16 bits arithmetic.
  */

  if (L_ACF[0] == 0) {
    _Pragma("loopbound min 8 max 8")
    for (i = 8; i--; *r++ = 0) ;
    return;
  }

  temp = gsm_norm( L_ACF[0] );

  /* ? overflow ? */
  _Pragma("loopbound min 9 max 9")
  for (i = 0; i <= 8; i++) ACF[i] = SASR( L_ACF[i] << temp, 16 );

  /*   Initialize array P[..] and K[..] for the recursion.
  */

  _Pragma("loopbound min 7 max 7")
  for (i = 1; i <= 7; i++) K[ i ] = ACF[ i ];

  _Pragma("loopbound min 9 max 9")
  for (i = 0; i <= 8; i++) P[ i ] = ACF[ i ];

  /*   Compute reflection coefficients
  */
  _Pragma("loopbound min 8 max 8")
  for (n = 1; n <= 8; n++, r++) {

    temp = P[1];
    temp = GSM_ABS(temp);
    if (P[0] < temp) {
      _Pragma("loopbound min 8 max 8")
      for (i = n; i <= 8; i++) *r++ = 0;
      return;
    }

    *r = gsm_div( temp, P[0] );

    if (P[1] > 0) *r = -*r;		/* r[n] = sub(0, r[n]) */
    if (n == 8) return; 

    /*  Schur recursion
    */
    temp = GSM_MULT_R( P[1], *r );
    P[0] = GSM_ADD( P[0], temp );

    _Pragma("loopbound min 1 max 7")
    for (m = 1; m <= 8 - n; ++m) {
      temp     = GSM_MULT_R( K[ m   ],    *r );
      P[m]     = GSM_ADD(    P[ m+1 ],  temp );

      temp     = GSM_MULT_R( P[ m+1 ],    *r );
      K[m]     = GSM_ADD(    K[ m   ],  temp );
    }
  }
}

/* 4.2.6 */

static void Transformation_to_Log_Area_Ratios (
	word	* r 			/* 0..7	   IN/OUT */
)
/*
 *  The following scaling for r[..] and LAR[..] has been used:
 *
 *  r[..]   = integer( real_r[..]*32768. ); -1 <= real_r < 1.
 *  LAR[..] = integer( real_LAR[..] * 16384 );
 *  with -1.625 <= real_LAR <= 1.625
 */
{
  word	temp;
  int	i;


  /* Computation of the LAR[0..7] from the r[0..7]
  */
  _Pragma("loopbound min 8 max 8")
  for (i = 1; i <= 8; i++, r++) {

    temp = *r;
    temp = GSM_ABS(temp);

    if (temp < 22118) {
      temp >>= 1;
    } else if (temp < 31130) {
      temp -= 11059;
    } else {
      temp -= 26112;
      temp <<= 2;
    }

    *r = *r < 0 ? -temp : temp;
  }
}

/* 4.2.7 */

static void Quantization_and_coding (
	word * LAR    	/* [0..7]	IN/OUT	*/
)
{
  word	temp;
  longword	ltmp;


  /*  This procedure needs four tables; the following equations
   *  give the optimum scaling for the constants:
   *  
   *  A[0..7] = integer( real_A[0..7] * 1024 )
   *  B[0..7] = integer( real_B[0..7] *  512 )
   *  MAC[0..7] = maximum of the LARc[0..7]
   *  MIC[0..7] = minimum of the LARc[0..7]
   */

#	undef STEP
#	define	STEP( A, B, MAC, MIC )		\
  temp = GSM_MULT( A,   *LAR );	\
  temp = GSM_ADD(  temp,   B );	\
  temp = GSM_ADD(  temp, 256 );	\
  temp = SASR(     temp,   9 );	\
  *LAR  =  temp>MAC ? MAC - MIC : (temp<MIC ? 0 : temp - MIC); \
  LAR++;

  STEP(  20480,     0,  31, -32 );
  STEP(  20480,     0,  31, -32 );
  STEP(  20480,  2048,  15, -16 );
  STEP(  20480, -2560,  15, -16 );

  STEP(  13964,    94,   7,  -8 );
  STEP(  15360, -1792,   7,  -8 );
  STEP(   8534,  -341,   3,  -4 );
  STEP(   9036, -1144,   3,  -4 );

#	undef	STEP
}

void Gsm_LPC_Analysis (
	struct gsm_state *S,
	word 		 * s,		/* 0..159 signals	IN/OUT	*/
        word 		 * LARc)	/* 0..7   LARc's	OUT	*/
{
  longword	L_ACF[9];

#if defined(USE_FLOAT_MUL) && defined(FAST)
  if (S->fast) Fast_Autocorrelation (s,	  L_ACF );
  else
#endif
    Autocorrelation			  (s,	  L_ACF	);
  Reflection_coefficients		  (L_ACF, LARc	);
  Transformation_to_Log_Area_Ratios (LARc);
  Quantization_and_coding		  (LARc);
}

/* preprocess.c */
/*	4.2.0 .. 4.2.3	PREPROCESSING SECTION
 *  
 *  	After A-law to linear conversion (or directly from the
 *   	Ato D converter) the following scaling is assumed for
 * 	input to the RPE-LTP algorithm:
 *
 *      in:  0.1.....................12
 *	     S.v.v.v.v.v.v.v.v.v.v.v.v.*.*.*
 *
 *	Where S is the sign bit, v a valid bit, and * a "don't care" bit.
 * 	The original signal is called sop[..]
 *
 *      out:   0.1................... 12 
 *	     S.S.v.v.v.v.v.v.v.v.v.v.v.v.0.0
 */


void Gsm_Preprocess (
	struct gsm_state * S,
	word		 * s,
	word 		 * so )		/* [0..159] 	IN/OUT	*/
{

  word       z1 = S->z1;
  longword L_z2 = S->L_z2;
  word 	   mp = S->mp;

  word 	   	s1;
  longword      L_s2;

  longword      L_temp;

  word		msp, lsp;
  word		SO;

  longword	ltmp;		/* for   ADD */
  ulongword	utmp;		/* for L_ADD */

  int		k = 160;

  _Pragma("loopbound min 160 max 160")
  while (k--) {

    /*  4.2.1   Downscaling of the input signal
    */
    SO = SASR( *s, 3 ) << 2;
    s++;

    /*  4.2.2   Offset compensation
     * 
     *  This part implements a high-pass filter and requires extended
     *  arithmetic precision for the recursive part of this filter.
     *  The input of this procedure is the array so[0...159] and the
     *  output the array sof[ 0...159 ].
     */
    /*   Compute the non-recursive part
    */

    s1 = SO - z1;			/* s1 = gsm_sub( *so, z1 ); */
    z1 = SO;

    /*   Compute the recursive part
    */
    L_s2 = s1;
    L_s2 <<= 15;

    /*   Execution of a 31 bv 16 bits multiplication
    */

    msp = SASR( L_z2, 15 );
    lsp = L_z2-((longword)msp<<15); /* gsm_L_sub(L_z2,(msp<<15)); */

    L_s2  += GSM_MULT_R( lsp, 32735 );
    L_temp = (longword)msp * 32735; /* GSM_L_MULT(msp,32735) >> 1;*/
    L_z2   = GSM_L_ADD( L_temp, L_s2 );

    /*    Compute sof[k] with rounding
    */
    L_temp = GSM_L_ADD( L_z2, 16384 );

    /*   4.2.3  Preemphasis
    */

    msp   = GSM_MULT_R( mp, -28180 );
    mp    = SASR( L_temp, 15 );
    *so++ = GSM_ADD( mp, msp );
  }

  S->z1   = z1;
  S->L_z2 = L_z2;
  S->mp   = mp;
}

/* gsm_bench.c */

int f_fast = 0;
int f_verbose = 0;

struct gsm_state gsmstate;

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

word gsm_asl (word a, int n)
{
  if (n >= 16) return 0;
  if (n <= -16) return -(a < 0);
  if (n < 0) return gsm_asr(a, -n);
  return a << n;
}

/*
longword gsm_L_asl (longword a, int n)
{
  if (n >= 32) return 0;
  if (n <= -32) return -(a < 0);
  if (n < 0) return gsm_L_asr(a, -n);
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

int encode() 
{
  gsm r;
  unsigned i;

  r = create();
  (void)gsm_option(r, GSM_OPT_FAST, &f_fast);
  (void)gsm_option(r, GSM_OPT_VERBOSE, &f_verbose);

  _Pragma("loopbound min 20 max 20")
  for (i=0; i < SAMPLES; i++) {
    gsm_encode(r, pcmdata + i * 160, gsmdata + i * sizeof(gsm_frame));
  }

  return 0;
}

int main( void ) 
{
  return encode();
}


