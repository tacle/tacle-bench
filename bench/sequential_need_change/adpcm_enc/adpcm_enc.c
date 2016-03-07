/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: adpcm_enc

  Author: Juan Martinez Velarde

  Function: CCITT G.722 ADPCM (Adaptive Differential Pulse Code Modulation)
    algorithm. 16khz sample rate data is stored in the array test_data[SIZE]. 
    Results are stored in the array compressed[SIZE]. 
    Execution time is determined by the constant SIZE (default value is 2000).
    

  Source: C Algorithms for Real-Time DSP by P. M. Embree
    and SNU-RT Benchmark Suite for Worst Case Timing Analysis
    collected and modified by S.-S. Lim <sslim@archi.snu.ac.kr>

  Original name: adpcm_encoder

  Changes: no major functional changes

  License: general open source

*/

/* common sampling rate for sound cards on IBM/PC */
#define SAMPLE_RATE 11025

#define PI 3141
#define SIZE 3
#define IN_END 4


/* COMPLEX STRUCTURE */

typedef struct {
  int real, imag;
} COMPLEX;


/* function prototypes for fft and filter functions */
void fft( COMPLEX *, int );
int fir_filter( int input, int *coef, int n, int *history );
int iir_filter( int input, int *coef, int n, int *history );
int gaussian( void );
int my_abs( int n );

void setup_codec( int ), key_down(), int_enable(), int_disable();
int flags( int );

int getinput( void );
void sendout( int ), flush();

int encode( int, int );
int filtez( int *bpl, int *dlt );
void upzero( int dlt, int *dlti, int *bli );
int filtep( int rlt1, int al1, int rlt2, int al2 );
int quantl( int el, int detl );
/* int invqxl( int il, int detl, int *code_table, int mode ); */
int logscl( int il, int nbl );
int scalel( int nbl, int shift_constant );
int uppol2( int al1, int al2, int plt, int plt1, int plt2 );
int uppol1( int al1, int apl2, int plt, int plt1 );
/* int invqah( int ih, int deth ); */
int logsch( int ih, int nbh );
void reset();
int my_fabs( int n );
int my_cos( int n );
int my_sin( int n );


/* G722 C code */

/* variables for transimit quadrature mirror filter here */
int tqmf[24];

/* QMF filter coefficients:
scaled by a factor of 4 compared to G722 CCITT recommendation */
int h[24] = {
    12,   -44,   -44,   212,    48,  -624,   128,  1448,
  -840, -3220,  3804, 15504, 15504,  3804, -3220,  -840,
  1448,   128,  -624,    48,   212,   -44,   -44,    12
};

int xl,xh;

/* variables for receive quadrature mirror filter here */
int accumc[11], accumd[11];

int xs, xd;

/* variables for encoder (hi and lo) here */

int il, szl, spl, sl, el;

int qq4_code4_table[16] = {
     0,  -20456,  -12896,   -8968,   -6288,   -4240,   -2584,   -1200,
 20456,   12896,    8968,    6288,    4240,    2584,    1200,       0
};

int qq5_code5_table[32] = {
  -280,    -280,  -23352,  -17560,  -14120,  -11664,   -9752,   -8184,
 -6864,   -5712,   -4696,   -3784,   -2960,   -2208,   -1520,    -880,
 23352,   17560,   14120,   11664,    9752,    8184,    6864,    5712,
  4696,    3784,    2960,    2208,    1520,     880,     280,    -280
};

int qq6_code6_table[64] = {
  -136,    -136,    -136,    -136,  -24808,  -21904,  -19008,  -16704,
-14984,  -13512,  -12280,  -11192,  -10232,   -9360,   -8576,   -7856,
 -7192,   -6576,   -6000,   -5456,   -4944,   -4464,   -4008,   -3576,
 -3168,   -2776,   -2400,   -2032,   -1688,   -1360,   -1040,    -728,
 24808,   21904,   19008,   16704,   14984,   13512,   12280,   11192,
 10232,    9360,    8576,    7856,    7192,    6576,    6000,    5456,
  4944,    4464,    4008,    3576,    3168,    2776,    2400,    2032,
  1688,    1360,    1040,     728,     432,     136,    -432,    -136
};

int delay_bpl[6];

int delay_dltx[6];

int wl_code_table[16] = {
   -60,  3042,  1198,   538,   334,   172,    58,   -30,
  3042,  1198,   538,   334,   172,    58,   -30,   -60
};

int wl_table[8] = {
   -60,   -30,    58,   172,   334,   538,  1198,  3042
};

int ilb_table[32] = {
  2048,  2093,  2139,  2186,  2233,  2282,  2332,  2383,
  2435,  2489,  2543,  2599,  2656,  2714,  2774,  2834,
  2896,  2960,  3025,  3091,  3158,  3228,  3298,  3371,
  3444,  3520,  3597,  3676,  3756,  3838,  3922,  4008
};

int nbl; 		 /* delay line */
int al1, al2;
int plt, plt1, plt2;
int rs;
int dlt;
int rlt, rlt1, rlt2;

/* decision levels - pre-multiplied by 8, 0 to indicate end */
int decis_levl[30] = {
   280,   576,   880,  1200,  1520,  1864,  2208,  2584,
  2960,  3376,  3784,  4240,  4696,  5200,  5712,  6288,
  6864,  7520,  8184,  8968,  9752, 10712, 11664, 12896,
 14120, 15840, 17560, 20456, 23352, 32767
};

int detl;

/* quantization table 31 long to make quantl look-up easier,
last entry is for mil=30 case when wd is max */
int quant26bt_pos[31] = {
    61,    60,    59,    58,    57,    56,    55,    54,
    53,    52,    51,    50,    49,    48,    47,    46,
    45,    44,    43,    42,    41,    40,    39,    38,
    37,    36,    35,    34,    33,    32,    32
};

/* quantization table 31 long to make quantl look-up easier,
last entry is for mil=30 case when wd is max */
int quant26bt_neg[31] = {
    63,    62,    31,    30,    29,    28,    27,    26,
    25,    24,    23,    22,    21,    20,    19,    18,
    17,    16,    15,    14,    13,    12,    11,    10,
     9,     8,     7,     6,     5,     4,     4
};


int deth;
int sh;         /* this comes from adaptive predictor */
int eh;

int qq2_code2_table[4] = {
  -7408,   -1616,   7408,  1616
};

int wh_code_table[4] = {
   798,   -214,    798,   -214
};


int dh, ih;
int nbh, szh;
int sph, ph, yh, rh;

int delay_dhx[6];

int delay_bph[6];

int ah1, ah2;
int ph1, ph2;
int rh1, rh2;

/* variables for decoder here */
int ilr, yl, rl;
int dec_deth, dec_detl, dec_dlt;

int dec_del_bpl[6];

int dec_del_dltx[6];

int dec_plt, dec_plt1, dec_plt2;
int dec_szl, dec_spl, dec_sl;
int dec_rlt1, dec_rlt2, dec_rlt;
int dec_al1, dec_al2;
int dl;
int dec_nbl, dec_yh, dec_dh, dec_nbh;

/* variables used in filtez */
int dec_del_bph[6];

int dec_del_dhx[6];

int dec_szh;
/* variables used in filtep */
int dec_rh1, dec_rh2;
int dec_ah1, dec_ah2;
int dec_ph, dec_sph;

int dec_sh, dec_rh;

int dec_ph1, dec_ph2;


/* G722 encode function two ints in, one 8 bit output */

/* put input samples in xin1 = first value, xin2 = second value */
/* returns il and ih stored together */


/* MAX: 1 */
int my_abs( int n )
{
  int m;


  if ( n >= 0 )
    m = n;
  else
    m = -n;

  return m;
}


/* MAX: 1 */
int my_fabs( int n )
{
  int f;


  if ( n >= 0 )
    f = n;
  else
    f = -n;

  return f;
}


int my_sin( int rad )
{
  int diff;
  int app = 0;
  int inc = 1;


  /* MAX dependent on rad's value, say 50 */
  _Pragma("loopbound min 0 max 0")
  while ( rad > 2 * PI ) {
    rad -= 2 * PI;
  }

  /* MAX dependent on rad's value, say 50 */
  _Pragma("loopbound min 0 max 1999")
  while ( rad < -2 * PI ) {
    rad += 2 * PI;
  }
  
  diff = rad;
  app = diff;
  diff = (diff * (-(rad*rad))) / ((2 * inc) * (2 * inc + 1));
  app = app + diff;
  inc++;

  /* REALLY: while(my_fabs(diff) >= 0.00001) { */
  /* MAX: 1000 */
  _Pragma("loopbound min 849 max 2424")
  while ( my_fabs( diff ) >= 1 ) {
    diff = (diff * (-(rad*rad))) / ((2 * inc) * (2 * inc + 1));
    app = app + diff;
    inc++;
  }

  return app;
}


int my_cos( int rad )
{
  return( my_sin( PI / 2 - rad ) );
}


/* MAX: 1 */
int encode( int xin1, int xin2 )
{
  int i;
  int *h_ptr, *tqmf_ptr, *tqmf_ptr1;
  long int xa, xb;
  int decis;


  /* transmit quadrature mirror filters implemented here */
  h_ptr = h;
  tqmf_ptr = tqmf;
  xa = (long)(*tqmf_ptr++) * (*h_ptr++);
  xb = (long)(*tqmf_ptr++) * (*h_ptr++);

  /* main multiply accumulate loop for samples and coefficients */
  /* MAX: 10 */
  _Pragma("loopbound min 10 max 10")
  for ( i = 0; i < 10; i++ ) {
    xa += (long)(*tqmf_ptr++) * (*h_ptr++);
    xb += (long)(*tqmf_ptr++) * (*h_ptr++);
  }
  
  /* final mult/accumulate */
  xa += (long)(*tqmf_ptr++) * (*h_ptr++);
  xb += (long)(*tqmf_ptr) * (*h_ptr++);

  /* update delay line tqmf */
  tqmf_ptr1 = tqmf_ptr - 2;
  /* MAX: 22 */
  _Pragma("loopbound min 22 max 22")
  for ( i = 0; i < 22; i++ ) {
    *tqmf_ptr-- = *tqmf_ptr1--;
  }
  
  *tqmf_ptr-- = xin1;
  *tqmf_ptr = xin2;

  /* scale outputs */
  xl = (xa + xb) >> 15;
  xh = (xa - xb) >> 15;

  /* end of quadrature mirror filter code */

  /* starting with lower sub band encoder */

  /* filtez - compute predictor output section - zero section */
  szl = filtez( delay_bpl, delay_dltx );

  /* filtep - compute predictor output signal (pole section) */
  spl = filtep( rlt1, al1, rlt2, al2 );

  /* compute the predictor output value in the lower sub_band encoder */
  sl = szl + spl;
  el = xl - sl;

  /* quantl: quantize the difference signal */
  il = quantl( el, detl );

  /* invqxl: computes quantized difference signal */
  /* for invqbl, truncate by 2 lsbs, so mode = 3 */
  dlt = ( (long) detl * qq4_code4_table[il >> 2] ) >> 15;

  /* logscl: updates logarithmic quant. scale factor in low sub band */
  nbl = logscl( il, nbl );

  /* scalel: compute the quantizer scale factor in the lower sub band */
  /* calling parameters nbl and 8 (constant such that scalel can be scaleh) */
  detl = scalel( nbl, 8 );

  /* parrec - simple addition to compute recontructed signal for adaptive pred */
  plt = dlt + szl;

  /* upzero: update zero section predictor coefficients (sixth order)*/
  /* calling parameters: dlt, dlt1, dlt2, ..., dlt6 from dlt */
  /*  bpli (linear_buffer in which all six values are delayed */
  /* return params:      updated bpli, delayed dltx */
  upzero( dlt, delay_dltx, delay_bpl );

  /* uppol2- update second predictor coefficient apl2 and delay it as al2 */
  /* calling parameters: al1, al2, plt, plt1, plt2 */
  al2 = uppol2( al1, al2, plt, plt1, plt2 );

  /* uppol1 :update first predictor coefficient apl1 and delay it as al1 */
  /* calling parameters: al1, apl2, plt, plt1 */
  al1 = uppol1( al1, al2, plt, plt1);

  /* recons : compute recontructed signal for adaptive predictor */
  rlt = sl + dlt;

  /* done with lower sub_band encoder; now implement delays for next time*/
  rlt2 = rlt1;
  rlt1 = rlt;
  plt2 = plt1;
  plt1 = plt;

  /* high band encode */

  szh = filtez( delay_bph, delay_dhx );

  sph = filtep( rh1, ah1, rh2, ah2 );

  /* predic: sh = sph + szh */
  sh = sph + szh;
  /* subtra: eh = xh - sh */
  eh = xh - sh;

  /* quanth - quantization of difference signal for higher sub-band */
  /* quanth: in-place for speed params: eh, deth (has init. value) */
  if ( eh >= 0 )
    ih = 3;     /* 2,3 are pos codes */
  else
    ih = 1;     /* 0,1 are neg codes */

  decis = ( 564L * (long)deth ) >> 12L;
  if ( my_abs( eh ) > decis )
    ih--;     /* mih = 2 case */

  /* invqah: compute the quantized difference signal, higher sub-band*/
  dh = ( (long)deth * qq2_code2_table[ih] ) >> 15L ;

  /* logsch: update logarithmic quantizer scale factor in hi sub-band*/
  nbh = logsch( ih, nbh );

  /* note : scalel and scaleh use same code, different parameters */
  deth = scalel( nbh, 10 );

  /* parrec - add pole predictor output to quantized diff. signal */
  ph = dh + szh;

  /* upzero: update zero section predictor coefficients (sixth order) */
  /* calling parameters: dh, dhi, bphi */
  /* return params: updated bphi, delayed dhx */
  upzero( dh, delay_dhx, delay_bph );

  /* uppol2: update second predictor coef aph2 and delay as ah2 */
  /* calling params: ah1, ah2, ph, ph1, ph2 */
  ah2 = uppol2( ah1, ah2, ph, ph1, ph2 );

  /* uppol1:  update first predictor coef. aph2 and delay it as ah1 */
  ah1 = uppol1( ah1, ah2, ph, ph1 );

  /* recons for higher sub-band */
  yh = sh + dh;

  /* done with higher sub-band encoder, now Delay for next time */
  rh2 = rh1;
  rh1 = yh;
  ph2 = ph1;
  ph1 = ph;

  /* multiplex ih and il to get signals together */
  return( il | (ih << 6) );
}


/* clear all storage locations */

void reset(void)
{
  int i;

  detl = dec_detl = 32;   /* reset to min scale factor */
  deth = dec_deth = 8;
  nbl = al1 = al2 = plt1 = plt2 = rlt1 = rlt2 = 0;
  nbh = ah1 = ah2 = ph1 = ph2 = rh1 = rh2 = 0;
  dec_nbl = dec_al1 = dec_al2 = dec_plt1 = dec_plt2 = dec_rlt1 = dec_rlt2 = 0;
  dec_nbh = dec_ah1 = dec_ah2 = dec_ph1 = dec_ph2 = dec_rh1 = dec_rh2 = 0;

  _Pragma("loopbound min 6 max 6")
  for ( i = 0; i < 6; i++) {
    delay_dltx[i] = 0;
    delay_dhx[i] = 0;
    dec_del_dltx[i] = 0;
    dec_del_dhx[i] = 0;
  }

  _Pragma("loopbound min 6 max 6")
  for ( i = 0; i < 6; i++ ) {
    delay_bpl[i] = 0;
    delay_bph[i] = 0;
    dec_del_bpl[i] = 0;
    dec_del_bph[i] = 0;
  }

  _Pragma("loopbound min 23 max 23")
  for ( i = 0; i < 23; i++ ) {
    tqmf[i] = 0;
  }

  _Pragma("loopbound min 11 max 11")
  for ( i = 0; i < 11; i++ ) {
    accumc[i] = 0;
    accumd[i] = 0;
  }

  return;
}


/* filtez - compute predictor output signal (zero section) */
/* input: bpl1-6 and dlt1-6, output: szl */
int filtez( int *bpl, int *dlt )
{
  int i;
  long int zl;


  zl = (long)(*bpl++) * (*dlt++);

  /* MAX: 5 */
  _Pragma("loopbound min 5 max 5")
  for ( i = 1; i < 6; i++ ) {
    zl += (long)(*bpl++) * (*dlt++);
  }

  return( (int)(zl >> 14) );   /* x2 here */
}


/* filtep - compute predictor output signal (pole section) */
/* input rlt1-2 and al1-2, output spl */
int filtep( int rlt1, int al1, int rlt2, int al2 )
{
  long int pl, pl2;


  pl = 2 * rlt1;
  pl = (long) al1 * pl;
  pl2 = 2 * rlt2;
  pl += (long) al2 * pl2;

  return( (int)(pl >> 15) );
}


/* quantl - quantize the difference signal in the lower sub-band */
int quantl( int el, int detl )
{
  int ril, mil;
  long int wd, decis;


  /* abs of difference signal */
  wd = my_abs( el );

  /* determine mil based on decision levels and detl gain */
  /* MAX: 30 */
  _Pragma("loopbound min 1 max 30")
  for ( mil = 0; mil < 30; mil++ ) {
    decis = (decis_levl[mil] * (long)detl) >> 15L;
    if ( wd <= decis )
      break;
  }

  /* if mil=30 then wd is less than all decision levels */
  if ( el >= 0 )
    ril = quant26bt_pos[mil];
  else
    ril = quant26bt_neg[mil];

  return( ril );
}


/* invqxl is either invqbl or invqal depending on parameters passed */
/* returns dlt, code table is pre-multiplied by 8 */

/*    int invqxl(int il,int detl,int *code_table,int mode) */
/*    { */
/*        long int dlt; */
/*       dlt = (long)detl*code_table[il >> (mode-1)]; */
/*        return((int)(dlt >> 15)); */
/*    } */

/* logscl - update log quantizer scale factor in lower sub-band */
/* note that nbl is passed and returned */
int logscl( int il, int nbl )
{
  long int wd;


  wd = ((long)nbl * 127L) >> 7L;   /* leak factor 127/128 */
  nbl = (int)wd + wl_code_table[il >> 2];

  if ( nbl < 0 )
    nbl = 0;
  if ( nbl > 18432 )
    nbl = 18432;

  return( nbl );
}


/* scalel: compute quantizer scale factor in lower or upper sub-band*/
int scalel( int nbl, int shift_constant )
{
  int wd1, wd2, wd3;


  wd1 = (nbl >> 6) & 31;
  wd2 = nbl >> 11;
  wd3 = ilb_table[wd1] >> (shift_constant + 1 - wd2);

  return( wd3 << 3 );
}


/* upzero - inputs: dlt, dlti[0-5], bli[0-5], outputs: updated bli[0-5] */
/* also implements delay of bli and update of dlti from dlt */
void upzero( int dlt, int *dlti, int *bli )
{
  int i, wd2, wd3;


  /*if dlt is zero, then no sum into bli */
  if ( dlt == 0 ) {
    _Pragma("loopbound min 6 max 6")    
    for ( i = 0; i < 6; i++ ) {
      bli[i] = (int)((255L * bli[i]) >> 8L); /* leak factor of 255/256 */
    }

  } else {
    _Pragma("loopbound min 6 max 6")
    for ( i = 0; i < 6; i++ ) {
      if ( (long)dlt * dlti[i] >= 0 )
        wd2 = 128;
      else
        wd2 = -128;

      wd3 = (int)((255L * bli[i]) >> 8L);    /* leak factor of 255/256 */
      bli[i] = wd2 + wd3;
    }
    
  }

  /* implement delay line for dlt */
  dlti[5] = dlti[4];
  dlti[4] = dlti[3];
  dlti[3] = dlti[2];
  dlti[1] = dlti[0];
  dlti[0] = dlt;

  return;
}


/* uppol2 - update second predictor coefficient (pole section) */
/* inputs: al1, al2, plt, plt1, plt2. outputs: apl2 */
int uppol2( int al1, int al2, int plt, int plt1, int plt2 )
{
  long int wd2, wd4;
  int apl2;


  wd2 = 4L * (long)al1;
  if ( (long)plt * plt1 >= 0L )
    wd2 = -wd2;    /* check same sign */
  wd2 = wd2 >> 7;		   /* gain of 1/128 */

  if ( (long)plt * plt2 >= 0L ) {
  wd4 = wd2 + 128;  	   /* same sign case */
  } else {
    wd4 = wd2 - 128;
  }
  apl2 = wd4 + (127L*(long)al2 >> 7L);  /* leak factor of 127/128 */

  /* apl2 is limited to +-.75 */
  if ( apl2 > 12288 )
    apl2 = 12288;
  if ( apl2 < -12288 )
    apl2 = -12288;

  return( apl2 );
}


/* uppol1 - update first predictor coefficient (pole section) */
/* inputs: al1, apl2, plt, plt1. outputs: apl1 */
int uppol1( int al1, int apl2, int plt, int plt1 )
{
  long int wd2;
  int wd3, apl1;


  wd2 = ((long)al1 * 255L) >> 8L;   /* leak factor of 255/256 */
  if ( (long)plt * plt1 >= 0L ) {
    apl1 = (int)wd2 + 192;	  /* same sign case */
  } else {
    apl1 = (int)wd2 - 192;
  }

  /* note: wd3= .9375-.75 is always positive */
  wd3 = 15360 - apl2;		  /* limit value */
  if ( apl1 > wd3 )
    apl1 = wd3;
  if ( apl1 < -wd3 )
    apl1 = -wd3;

  return( apl1 );
}


/* INVQAH: inverse adaptive quantizer for the higher sub-band */
/* returns dh, code table is pre-multiplied by 8 */
/*  int invqah(int ih,int deth) */
/*  { */
/*        long int rdh; */
/*        rdh = ((long)deth*qq2_code2_table[ih]) >> 15L ; */
/*        return((int)(rdh )); */
/*  } */


/* logsch - update log quantizer scale factor in higher sub-band */
/* note that nbh is passed and returned */
int logsch( int ih, int nbh )
{
  int wd;


  wd = ((long)nbh * 127L) >> 7L;       /* leak factor 127/128 */
  nbh = wd + wh_code_table[ih];

  if ( nbh < 0 )
    nbh = 0;
  if ( nbh > 22528 )
    nbh = 22528;

  return( nbh );
}

static int test_data[SIZE * 2], compressed[SIZE];

void adpcm_enc_init(void)
{
  int i, j, f;
  
  /* reset, initialize required memory */
  reset();

  /* read in amplitude and frequency for test data */
  j = 10;
  f = 2000;

  /* 16 KHz sample rate */
  /* XXmain_0, MAX: 2 */
  /* Since the number of times we loop in my_sin depends on the argument we
     add the fact: xxmain_0:[]: */
  _Pragma("loopbound min 3 max 3")
  for ( i = 0 ; i < SIZE ; i++) {
    test_data[i] = (int) j * my_cos( f * PI * i );
  }
}

void _Pragma( "entrypoint" ) adpcm_main(void)
{
  int i;
  /* MAX: 2 */
  _Pragma("loopbound min 2 max 2")
      for ( i = 0 ; i < IN_END ; i += 2 ) {
    compressed[i/2] = encode( test_data[i], test_data[i+1] );
  }
  
}

int adpcm_enc_return(void)
{
  return compressed[0];
}

int main(void)
{
  adpcm_enc_init();
  adpcm_main();
  
  return adpcm_enc_return();
}
