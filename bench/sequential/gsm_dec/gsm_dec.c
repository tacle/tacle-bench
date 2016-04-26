/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: gsm_decode

  Author: Jutta Degener and Carsten Bormann,
   Technische Universitaet Berlin

  Function: Decoding of GSM data

  Source: included in MediaBench/gsm

  Original name: gsm_decode

  Changes: no major functional changes

  License: see the accompanying COPYRIGHT file

*/

#define SASR

#include "proto.h"
#include "private.h"
#include "gsm.h"
#include "config.h"
#include "data.h"

#define SAMPLES 20

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

int gsm_decode_main() 
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

void gsm_decode_init(void)
{
  return;
}

int gsm_decode_return(void)
{
  return gsmdata[0];
}

int main( void ) 
{
  gsm_decode_init();
  gsm_decode_main();
  return (gsm_decode_return());
}
