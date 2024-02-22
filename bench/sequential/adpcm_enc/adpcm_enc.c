/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: adpcm_enc

  Author: Sung-Soo Lim

  Function: CCITT G.722 ADPCM (Adaptive Differential Pulse Code Modulation)
    algorithm. 16khz sample rate data is stored in the array test_data[ SIZE ].
    Results are stored in the array compressed[ SIZE ].
    Execution time is determined by the constant SIZE (default value is 2000).


  Source: C Algorithms for Real-Time DSP by P. M. Embree
    and SNU-RT Benchmark Suite for Worst Case Timing Analysis
    collected and modified by S.-S. Lim <sslim@archi.snu.ac.kr>

  Original name: adpcm_encoder

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely, but the
           SNU-RT Benchmark Suite must be acknowledged

*/


/* common sampling rate for sound cards on IBM/PC */
#define SAMPLE_RATE 11025

#define PI 3141
#define SIZE 3
#define IN_END 4


/*
  Forward declaration of functions
*/

int adpcm_enc_encode( int, int );
int adpcm_enc_filtez( int *bpl, int *dlt );
void adpcm_enc_upzero( int dlt, int *dlti, int *bli );
int adpcm_enc_filtep( int rlt1, int al1, int rlt2, int al2 );
int adpcm_enc_quantl( int el, int detl );
int adpcm_enc_logscl( int il, int nbl );
int adpcm_enc_scalel( int nbl, int shift_constant );
int adpcm_enc_uppol2( int al1, int al2, int plt, int plt1, int plt2 );
int adpcm_enc_uppol1( int al1, int apl2, int plt, int plt1 );
int adpcm_enc_logsch( int ih, int nbh );
void adpcm_enc_reset();
int adpcm_enc_fabs( int n );
int adpcm_enc_cos( int n );
int adpcm_enc_sin( int n );
int adpcm_enc_abs( int n );
void adpcm_enc_init( void );
void adpcm_enc_main( void );
int adpcm_enc_return( void );
int main( void );

/*
  Forward declaration of global variables
*/

int adpcm_enc_test_data[ SIZE * 2 ], adpcm_enc_compressed[ SIZE ];


/* G722 C code */

/* variables for transimit quadrature mirror filter here */
int adpcm_enc_tqmf[ 24 ];

/* QMF filter coefficients:
  scaled by a factor of 4 compared to G722 CCITT recommendation */
int adpcm_enc_h[ 24 ] = {
  12,   -44,   -44,   212,    48,  -624,   128,  1448,
  -840, -3220,  3804, 15504, 15504,  3804, -3220,  -840,
  1448,   128,  -624,    48,   212,   -44,   -44,    12
};

int adpcm_enc_xl, adpcm_enc_xh;

/* variables for encoder (hi and lo) here */

int adpcm_enc_il, adpcm_enc_szl, adpcm_enc_spl, adpcm_enc_sl, adpcm_enc_el;

int adpcm_enc_qq4_code4_table[ 16 ] = {
  0,  -20456,  -12896,   -8968,   -6288,   -4240,   -2584,   -1200,
  20456,   12896,    8968,    6288,    4240,    2584,    1200,       0
};

int adpcm_enc_qq5_code5_table[ 32 ] = {
  -280,    -280,  -23352,  -17560,  -14120,  -11664,   -9752,   -8184,
    -6864,   -5712,   -4696,   -3784,   -2960,   -2208,   -1520,    -880,
    23352,   17560,   14120,   11664,    9752,    8184,    6864,    5712,
    4696,    3784,    2960,    2208,    1520,     880,     280,    -280
  };

int adpcm_enc_qq6_code6_table[ 64 ] = {
  -136,    -136,    -136,    -136,  -24808,  -21904,  -19008,  -16704,
    -14984,  -13512,  -12280,  -11192,  -10232,   -9360,   -8576,   -7856,
    -7192,   -6576,   -6000,   -5456,   -4944,   -4464,   -4008,   -3576,
    -3168,   -2776,   -2400,   -2032,   -1688,   -1360,   -1040,    -728,
    24808,   21904,   19008,   16704,   14984,   13512,   12280,   11192,
    10232,    9360,    8576,    7856,    7192,    6576,    6000,    5456,
    4944,    4464,    4008,    3576,    3168,    2776,    2400,    2032,
    1688,    1360,    1040,     728,     432,     136,    -432,    -136
  };

int adpcm_enc_delay_bpl[ 6 ];

int adpcm_enc_delay_dltx[ 6 ];

int adpcm_enc_wl_code_table[ 16 ] = {
  -60,  3042,  1198,   538,   334,   172,    58,   -30,
    3042,  1198,   538,   334,   172,    58,   -30,   -60
  };

int adpcm_enc_ilb_table[ 32 ] = {
  2048,  2093,  2139,  2186,  2233,  2282,  2332,  2383,
  2435,  2489,  2543,  2599,  2656,  2714,  2774,  2834,
  2896,  2960,  3025,  3091,  3158,  3228,  3298,  3371,
  3444,  3520,  3597,  3676,  3756,  3838,  3922,  4008
};

int adpcm_enc_nbl;     /* delay line */
int adpcm_enc_al1, adpcm_enc_al2;
int adpcm_enc_plt, adpcm_enc_plt1, adpcm_enc_plt2;
int adpcm_enc_dlt;
int adpcm_enc_rlt, adpcm_enc_rlt1, adpcm_enc_rlt2;

/* decision levels - pre-multiplied by 8, 0 to indicate end */
int adpcm_enc_decis_levl[ 30 ] = {
  280,   576,   880,  1200,  1520,  1864,  2208,  2584,
  2960,  3376,  3784,  4240,  4696,  5200,  5712,  6288,
  6864,  7520,  8184,  8968,  9752, 10712, 11664, 12896,
  14120, 15840, 17560, 20456, 23352, 32767
};

int adpcm_enc_detl;

/* quantization table 31 long to make quantl look-up easier,
  last entry is for mil=30 case when wd is max */
int adpcm_enc_quant26bt_pos[ 31 ] = {
  61,    60,    59,    58,    57,    56,    55,    54,
  53,    52,    51,    50,    49,    48,    47,    46,
  45,    44,    43,    42,    41,    40,    39,    38,
  37,    36,    35,    34,    33,    32,    32
};

/* quantization table 31 long to make quantl look-up easier,
  last entry is for mil=30 case when wd is max */
int adpcm_enc_quant26bt_neg[ 31 ] = {
  63,    62,    31,    30,    29,    28,    27,    26,
  25,    24,    23,    22,    21,    20,    19,    18,
  17,    16,    15,    14,    13,    12,    11,    10,
  9,     8,     7,     6,     5,     4,     4
};


int adpcm_enc_deth;
int adpcm_enc_sh;         /* this comes from adaptive predictor */
int adpcm_enc_eh;

int adpcm_enc_qq2_code2_table[ 4 ] = {
  -7408,   -1616,   7408,  1616
  };

int adpcm_enc_wh_code_table[ 4 ] = {
  798,   -214,    798,   -214
};


int adpcm_enc_dh, adpcm_enc_ih;
int adpcm_enc_nbh, adpcm_enc_szh;
int adpcm_enc_sph, adpcm_enc_ph, adpcm_enc_yh;

int adpcm_enc_delay_dhx[ 6 ];
int adpcm_enc_delay_bph[ 6 ];

int adpcm_enc_ah1, adpcm_enc_ah2;
int adpcm_enc_ph1, adpcm_enc_ph2;
int adpcm_enc_rh1, adpcm_enc_rh2;


/* G722 encode function two ints in, one 8 bit output */

/* put input samples in xin1 = first value, xin2 = second value */
/* returns il and ih stored together */


/* MAX: 1 */
int adpcm_enc_abs( int n )
{
  int m;


  if ( n >= 0 )
    m = n;
  else
    m = -n;

  return m;
}


/* MAX: 1 */
int adpcm_enc_fabs( int n )
{
  int f;


  if ( n >= 0 )
    f = n;
  else
    f = -n;

  return f;
}

#pragma GCC push_options
#pragma GCC optimize "-fwrapv"
int adpcm_enc_sin( int rad )
{
  int diff;
  int app = 0;
  int inc = 1;


  /* MAX dependent on rad's value, say 50 */
  _Pragma( "loopbound min 0 max 0" )
  while ( rad > 2 * PI )
    rad -= 2 * PI;

  /* MAX dependent on rad's value, say 50 */
  _Pragma( "loopbound min 0 max 1999" )
  while ( rad < -2 * PI )
    rad += 2 * PI;

  diff = rad;
  app = diff;
  diff = ( diff * ( -( rad * rad ) ) ) / ( ( 2 * inc ) * ( 2 * inc + 1 ) );
  app = app + diff;
  inc++;

  /* REALLY: while(my_fabs(diff) >= 0.00001) { */
  /* MAX: 1000 */
  _Pragma( "loopbound min 849 max 2424" )
  while ( adpcm_enc_fabs( diff ) >= 1 ) {
    diff = ( diff * ( -( rad * rad ) ) ) / ( ( 2 * inc ) * ( 2 * inc + 1 ) );
    app = app + diff;
    inc++;
  }

  return app;
}
#pragma GCC pop_options


int adpcm_enc_cos( int rad )
{
  return ( adpcm_enc_sin( PI / 2 - rad ) );
}


/* MAX: 1 */
int adpcm_enc_encode( int xin1, int xin2 )
{
  int i;
  int *h_ptr, *tqmf_ptr, *tqmf_ptr1;
  long long int xa, xb;
  int decis;


  /* transmit quadrature mirror filters implemented here */
  h_ptr = adpcm_enc_h;
  tqmf_ptr = adpcm_enc_tqmf;
  xa = ( long long )( *tqmf_ptr++ ) * ( *h_ptr++ );
  xb = ( long long )( *tqmf_ptr++ ) * ( *h_ptr++ );

  /* main multiply accumulate loop for samples and coefficients */
  /* MAX: 10 */
  _Pragma( "loopbound min 10 max 10" )
  for ( i = 0; i < 10; i++ ) {
    xa += ( long long )( *tqmf_ptr++ ) * ( *h_ptr++ );
    xb += ( long long )( *tqmf_ptr++ ) * ( *h_ptr++ );
  }

  /* final mult/accumulate */
  xa += ( long long )( *tqmf_ptr++ ) * ( *h_ptr++ );
  xb += ( long long )( *tqmf_ptr ) * ( *h_ptr++ );

  /* update delay line tqmf */
  tqmf_ptr1 = tqmf_ptr - 2;
  /* MAX: 22 */
  _Pragma( "loopbound min 22 max 22" )
  for ( i = 0; i < 22; i++ )
    *tqmf_ptr-- = *tqmf_ptr1--;

  *tqmf_ptr-- = xin1;
  *tqmf_ptr = xin2;

  /* scale outputs */
  adpcm_enc_xl = ( xa + xb ) >> 15;
  adpcm_enc_xh = ( xa - xb ) >> 15;

  /* end of quadrature mirror filter code */

  /* starting with lower sub band encoder */

  /* filtez - compute predictor output section - zero section */
  adpcm_enc_szl = adpcm_enc_filtez( adpcm_enc_delay_bpl, adpcm_enc_delay_dltx );

  /* filtep - compute predictor output signal (pole section) */
  adpcm_enc_spl = adpcm_enc_filtep( adpcm_enc_rlt1, adpcm_enc_al1, adpcm_enc_rlt2,
                                    adpcm_enc_al2 );

  /* compute the predictor output value in the lower sub_band encoder */
  adpcm_enc_sl = adpcm_enc_szl + adpcm_enc_spl;
  adpcm_enc_el = adpcm_enc_xl - adpcm_enc_sl;

  /* quantl: quantize the difference signal */
  adpcm_enc_il = adpcm_enc_quantl( adpcm_enc_el, adpcm_enc_detl );

  /* invqxl: computes quantized difference signal */
  /* for invqbl, truncate by 2 lsbs, so mode = 3 */
  adpcm_enc_dlt = ( ( long long ) adpcm_enc_detl *
                    adpcm_enc_qq4_code4_table[ adpcm_enc_il >> 2 ] ) >> 15;

  /* logscl: updates logarithmic quant. scale factor in low sub band */
  adpcm_enc_nbl = adpcm_enc_logscl( adpcm_enc_il, adpcm_enc_nbl );

  /* scalel: compute the quantizer scale factor in the lower sub band */
  /* calling parameters nbl and 8 (constant such that scalel can be scaleh) */
  adpcm_enc_detl = adpcm_enc_scalel( adpcm_enc_nbl, 8 );

  /* parrec - simple addition to compute recontructed signal for adaptive pred */
  adpcm_enc_plt = adpcm_enc_dlt + adpcm_enc_szl;

  /* upzero: update zero section predictor coefficients (sixth order)*/
  /* calling parameters: dlt, dlt1, dlt2, ..., dlt6 from dlt */
  /*  bpli (linear_buffer in which all six values are delayed */
  /* return params:      updated bpli, delayed dltx */
  adpcm_enc_upzero( adpcm_enc_dlt, adpcm_enc_delay_dltx, adpcm_enc_delay_bpl );

  /* uppol2- update second predictor coefficient apl2 and delay it as al2 */
  /* calling parameters: al1, al2, plt, plt1, plt2 */
  adpcm_enc_al2 = adpcm_enc_uppol2( adpcm_enc_al1, adpcm_enc_al2, adpcm_enc_plt,
                                    adpcm_enc_plt1, adpcm_enc_plt2 );

  /* uppol1 :update first predictor coefficient apl1 and delay it as al1 */
  /* calling parameters: al1, apl2, plt, plt1 */
  adpcm_enc_al1 = adpcm_enc_uppol1( adpcm_enc_al1, adpcm_enc_al2, adpcm_enc_plt,
                                    adpcm_enc_plt1 );

  /* recons : compute recontructed signal for adaptive predictor */
  adpcm_enc_rlt = adpcm_enc_sl + adpcm_enc_dlt;

  /* done with lower sub_band encoder; now implement delays for next time*/
  adpcm_enc_rlt2 = adpcm_enc_rlt1;
  adpcm_enc_rlt1 = adpcm_enc_rlt;
  adpcm_enc_plt2 = adpcm_enc_plt1;
  adpcm_enc_plt1 = adpcm_enc_plt;

  /* high band encode */

  adpcm_enc_szh = adpcm_enc_filtez( adpcm_enc_delay_bph, adpcm_enc_delay_dhx );

  adpcm_enc_sph = adpcm_enc_filtep( adpcm_enc_rh1, adpcm_enc_ah1, adpcm_enc_rh2,
                                    adpcm_enc_ah2 );

  /* predic: sh = sph + szh */
  adpcm_enc_sh = adpcm_enc_sph + adpcm_enc_szh;
  /* subtra: eh = xh - sh */
  adpcm_enc_eh = adpcm_enc_xh - adpcm_enc_sh;

  /* quanth - quantization of difference signal for higher sub-band */
  /* quanth: in-place for speed params: eh, deth (has init. value) */
  if ( adpcm_enc_eh >= 0 )
    adpcm_enc_ih = 3;     /* 2,3 are pos codes */
  else
    adpcm_enc_ih = 1;     /* 0,1 are neg codes */

  decis = ( 564L * ( long long )adpcm_enc_deth ) >> 12L;
  if ( adpcm_enc_abs( adpcm_enc_eh ) > decis )
    adpcm_enc_ih--;     /* mih = 2 case */

  /* invqah: compute the quantized difference signal, higher sub-band*/
  adpcm_enc_dh = ( ( long long )adpcm_enc_deth *
                   adpcm_enc_qq2_code2_table[ adpcm_enc_ih ] ) >> 15L ;

  /* logsch: update logarithmic quantizer scale factor in hi sub-band*/
  adpcm_enc_nbh = adpcm_enc_logsch( adpcm_enc_ih, adpcm_enc_nbh );

  /* note : scalel and scaleh use same code, different parameters */
  adpcm_enc_deth = adpcm_enc_scalel( adpcm_enc_nbh, 10 );

  /* parrec - add pole predictor output to quantized diff. signal */
  adpcm_enc_ph = adpcm_enc_dh + adpcm_enc_szh;

  /* upzero: update zero section predictor coefficients (sixth order) */
  /* calling parameters: dh, dhi, bphi */
  /* return params: updated bphi, delayed dhx */
  adpcm_enc_upzero( adpcm_enc_dh, adpcm_enc_delay_dhx, adpcm_enc_delay_bph );

  /* uppol2: update second predictor coef aph2 and delay as ah2 */
  /* calling params: ah1, ah2, ph, ph1, ph2 */
  adpcm_enc_ah2 = adpcm_enc_uppol2( adpcm_enc_ah1, adpcm_enc_ah2, adpcm_enc_ph,
                                    adpcm_enc_ph1, adpcm_enc_ph2 );

  /* uppol1:  update first predictor coef. aph2 and delay it as ah1 */
  adpcm_enc_ah1 = adpcm_enc_uppol1( adpcm_enc_ah1, adpcm_enc_ah2, adpcm_enc_ph,
                                    adpcm_enc_ph1 );

  /* recons for higher sub-band */
  adpcm_enc_yh = adpcm_enc_sh + adpcm_enc_dh;

  /* done with higher sub-band encoder, now Delay for next time */
  adpcm_enc_rh2 = adpcm_enc_rh1;
  adpcm_enc_rh1 = adpcm_enc_yh;
  adpcm_enc_ph2 = adpcm_enc_ph1;
  adpcm_enc_ph1 = adpcm_enc_ph;

  /* multiplex ih and il to get signals together */
  return ( adpcm_enc_il | ( adpcm_enc_ih << 6 ) );
}


/* filtez - compute predictor output signal (zero section) */
/* input: bpl1-6 and dlt1-6, output: szl */
int adpcm_enc_filtez( int *bpl, int *dlt )
{
  int i;
  long long int zl;


  zl = ( long long )( *bpl++ ) * ( *dlt++ );

  /* MAX: 5 */
  _Pragma( "loopbound min 5 max 5" )
  for ( i = 1; i < 6; i++ )
    zl += ( long long )( *bpl++ ) * ( *dlt++ );

  return ( ( int )( zl >> 14 ) ); /* x2 here */
}


/* filtep - compute predictor output signal (pole section) */
/* input rlt1-2 and al1-2, output spl */
int adpcm_enc_filtep( int rlt1, int al1, int rlt2, int al2 )
{
  long long int pl, pl2;


  pl = 2 * rlt1;
  pl = ( long long ) al1 * pl;
  pl2 = 2 * rlt2;
  pl += ( long long ) al2 * pl2;

  return ( ( int )( pl >> 15 ) );
}


/* quantl - quantize the difference signal in the lower sub-band */
int adpcm_enc_quantl( int el, int detl )
{
  int ril, mil;
  long long int wd, decis;


  /* abs of difference signal */
  wd = adpcm_enc_abs( el );

  /* determine mil based on decision levels and detl gain */
  /* MAX: 30 */
  _Pragma( "loopbound min 1 max 30" )
  for ( mil = 0; mil < 30; mil++ ) {
    decis = ( adpcm_enc_decis_levl[ mil ] * ( long long )detl ) >> 15L;
    if ( wd <= decis )
      break;
  }

  /* if mil=30 then wd is less than all decision levels */
  if ( el >= 0 )
    ril = adpcm_enc_quant26bt_pos[ mil ];
  else
    ril = adpcm_enc_quant26bt_neg[ mil ];

  return ( ril );
}


/* invqxl is either invqbl or invqal depending on parameters passed */
/* returns dlt, code table is pre-multiplied by 8 */

/*    int invqxl(int il,int detl,int *code_table,int mode) */
/*    { */
/*        long long int dlt; */
/*       dlt = (long long)detl*code_table[ il >> (mode-1) ]; */
/*        return((int)(dlt >> 15)); */
/*    } */

/* logscl - update log quantizer scale factor in lower sub-band */
/* note that nbl is passed and returned */
int adpcm_enc_logscl( int il, int nbl )
{
  long long int wd;


  wd = ( ( long long )nbl * 127L ) >> 7L; /* leak factor 127/128 */
  nbl = ( int )wd + adpcm_enc_wl_code_table[ il >> 2 ];

  if ( nbl < 0 )
    nbl = 0;
  if ( nbl > 18432 )
    nbl = 18432;

  return ( nbl );
}


/* scalel: compute quantizer scale factor in lower or upper sub-band*/
int adpcm_enc_scalel( int nbl, int shift_constant )
{
  int wd1, wd2, wd3;


  wd1 = ( nbl >> 6 ) & 31;
  wd2 = nbl >> 11;
  wd3 = adpcm_enc_ilb_table[ wd1 ] >> ( shift_constant + 1 - wd2 );

  return ( wd3 << 3 );
}


/* upzero - inputs: dlt, dlti[ 0-5 ], bli[ 0-5 ], outputs: updated bli[ 0-5 ] */
/* also implements delay of bli and update of dlti from dlt */
void adpcm_enc_upzero( int dlt, int *dlti, int *bli )
{
  int i, wd2, wd3;


  /*if dlt is zero, then no sum into bli */
  if ( dlt == 0 ) {
    _Pragma( "loopbound min 6 max 6" )
    for ( i = 0; i < 6; i++ ) {
      bli[ i ] = ( int )( ( 255L * bli[ i ] ) >> 8L ); /* leak factor of 255/256 */
    }

  } else {
    _Pragma( "loopbound min 6 max 6" )
    for ( i = 0; i < 6; i++ ) {
      if ( ( long long )dlt * dlti[ i ] >= 0 )
        wd2 = 128;
      else
        wd2 = -128;

      wd3 = ( int )( ( 255L * bli[ i ] ) >> 8L ); /* leak factor of 255/256 */
      bli[ i ] = wd2 + wd3;
    }

  }

  /* implement delay line for dlt */
  dlti[ 5 ] = dlti[ 4 ];
  dlti[ 4 ] = dlti[ 3 ];
  dlti[ 3 ] = dlti[ 2 ];
  dlti[ 1 ] = dlti[ 0 ];
  dlti[ 0 ] = dlt;

  return;
}


/* uppol2 - update second predictor coefficient (pole section) */
/* inputs: al1, al2, plt, plt1, plt2. outputs: apl2 */
int adpcm_enc_uppol2( int al1, int al2, int plt, int plt1, int plt2 )
{
  long long int wd2, wd4;
  int apl2;


  wd2 = 4L * ( long long )al1;
  if ( ( long long )plt * plt1 >= 0L )
    wd2 = -wd2;    /* check same sign */
  wd2 = wd2 >> 7;      /* gain of 1/128 */

  if ( ( long long )plt * plt2 >= 0L ) {
    wd4 = wd2 + 128;       /* same sign case */
  } else
    wd4 = wd2 - 128;
  apl2 = wd4 + ( 127L * ( long long )al2 >> 7L ); /* leak factor of 127/128 */

  /* apl2 is limited to +-.75 */
  if ( apl2 > 12288 )
    apl2 = 12288;
  if ( apl2 < -12288 )
    apl2 = -12288;

  return ( apl2 );
}


/* uppol1 - update first predictor coefficient (pole section) */
/* inputs: al1, apl2, plt, plt1. outputs: apl1 */
int adpcm_enc_uppol1( int al1, int apl2, int plt, int plt1 )
{
  long long int wd2;
  int wd3, apl1;


  wd2 = ( ( long long )al1 * 255L ) >> 8L; /* leak factor of 255/256 */
  if ( ( long long )plt * plt1 >= 0L ) {
    apl1 = ( int )wd2 + 192;  /* same sign case */
  } else
    apl1 = ( int )wd2 - 192;

  /* note: wd3= .9375-.75 is always positive */
  wd3 = 15360 - apl2;     /* limit value */
  if ( apl1 > wd3 )
    apl1 = wd3;
  if ( apl1 < -wd3 )
    apl1 = -wd3;

  return ( apl1 );
}


/* INVQAH: inverse adaptive quantizer for the higher sub-band */
/* returns dh, code table is pre-multiplied by 8 */
/*  int invqah(int ih,int deth) */
/*  { */
/*        long long int rdh; */
/*        rdh = ((long long)deth*qq2_code2_table[ ih ]) >> 15L ; */
/*        return((int)(rdh )); */
/*  } */


/* logsch - update log quantizer scale factor in higher sub-band */
/* note that nbh is passed and returned */
int adpcm_enc_logsch( int ih, int nbh )
{
  int wd;


  wd = ( ( long long )nbh * 127L ) >> 7L;   /* leak factor 127/128 */
  nbh = wd + adpcm_enc_wh_code_table[ ih ];

  if ( nbh < 0 )
    nbh = 0;
  if ( nbh > 22528 )
    nbh = 22528;

  return ( nbh );
}


/*
  Initialization- and return-value-related functions
*/

/* clear all storage locations */

void adpcm_enc_reset( void )
{
  int i;

  adpcm_enc_detl = 32;   /* reset to min scale factor */
  adpcm_enc_deth = 8;
  adpcm_enc_nbl = adpcm_enc_al1 = adpcm_enc_al2 = adpcm_enc_plt1 = adpcm_enc_plt2
                                  = adpcm_enc_rlt1 = adpcm_enc_rlt2 = 0;
  adpcm_enc_nbh = adpcm_enc_ah1 = adpcm_enc_ah2 = adpcm_enc_ph1 = adpcm_enc_ph2 =
                                    adpcm_enc_rh1 = adpcm_enc_rh2 = 0;

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i < 6; i++ ) {
    adpcm_enc_delay_dltx[ i ] = 0;
    adpcm_enc_delay_dhx[ i ] = 0;
  }

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i < 6; i++ ) {
    adpcm_enc_delay_bpl[ i ] = 0;
    adpcm_enc_delay_bph[ i ] = 0;
  }

  _Pragma( "loopbound min 23 max 23" )
  for ( i = 0; i < 23; i++ )
    adpcm_enc_tqmf[ i ] = 0;

  return;
}


void adpcm_enc_init( void )
{
  int i, j, f;
  volatile int x = 0;

  /* reset, initialize required memory */
  adpcm_enc_reset();

  /* read in amplitude and frequency for test data */
  j = 10;
  f = 2000;

  /* 16 KHz sample rate */
  /* XXmain_0, MAX: 2 */
  /* Since the number of times we loop in my_sin depends on the argument we
     add the fact: xxmain_0:[  ]: */
  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0 ; i < SIZE ; i++ ) {
    adpcm_enc_test_data[ i ] = ( int ) j * adpcm_enc_cos( f * PI * i );

    /* avoid constant-propagation optimizations */
    adpcm_enc_test_data[ i ] += x;
  }
}


int adpcm_enc_return( void )
{
  int i;
  int check_sum = 0;

  _Pragma( "loopbound min 2 max 2" )
  for ( i = 0 ; i < IN_END ; i += 2 )
    check_sum += adpcm_enc_compressed[ i / 2 ];

  return check_sum != 385;
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) adpcm_enc_main( void )
{
  int i;
  /* MAX: 2 */
  _Pragma( "loopbound min 2 max 2" )
  for ( i = 0 ; i < IN_END ; i += 2 )
    adpcm_enc_compressed[ i / 2 ] = adpcm_enc_encode( adpcm_enc_test_data[ i ],
                                  adpcm_enc_test_data[ i + 1 ] );

}

int main( void )
{
  adpcm_enc_init();
  adpcm_enc_main();

  return adpcm_enc_return();
}
