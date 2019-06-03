/*

    This program is part of the TACLeBench benchmark suite.
    Version V 1.x

    Name: g723_enc

    Author: Unknown

    Function: g723 encoder.

    Source: SUN Microsystems

    Changes: The benchmark was changed to use the g723 encoder

    License: "Unrestricted use" (see license.txt)

*/

/*
  Declaration of data types
*/

/*
   The following is the definition of the state structure
   used by the G.721/G.723 encoder and decoder to preserve their internal
   state between successive calls.  The meanings of the majority
   of the state structure fields are explained in detail in the
   CCITT Recommendation G.721.  The field names are essentially indentical
   to variable names in the bit level description of the coding algorithm
   included in this Recommendation.
*/
struct g723_enc_state_t {
  long yl;  /* Locked or steady state step size multiplier. */
  short yu; /* Unlocked or non-steady state step size multiplier. */
  short dms;  /* Short term energy estimate. */
  short dml;  /* Long term energy estimate. */
  short ap; /* Linear weighting coefficient of 'yl' and 'yu'. */

  short a[ 2 ]; /* Coefficients of pole portion of prediction filter. */
  short b[ 6 ]; /* Coefficients of zero portion of prediction filter. */
  short pk[ 2 ];  /*
        Signs of previous two samples of a partially
        reconstructed signal.
*/
  short dq[ 6 ];  /*
        Previous 6 samples of the quantized difference
        signal represented in an internal floating point
        format.
*/
  short sr[ 2 ];  /*
        Previous 2 samples of the quantized difference
        signal represented in an internal floating point
        format.
*/
  char td;  /* delayed tone detect, new in 1988 version */
};


/*
  Forward declaration of functions
*/

int g723_enc_abs( int num );
void g723_enc_init_state( struct g723_enc_state_t *state_ptr );
int g723_enc_predictor_zero( struct g723_enc_state_t *state_ptr );
int g723_enc_fmult( int an, int srn );
int g723_enc_predictor_pole( struct g723_enc_state_t *state_ptr );
int g723_enc_step_size( struct g723_enc_state_t *state_ptr );
int g723_enc_quantize(
  int   d,  /* Raw difference signal sample */
  int   y,  /* Step size multiplier */
  short   *table, /* quantization table */
  int   size ); /* table size of short integers */
int g723_enc_reconstruct(
  int   sign, /* 0 for non-negative value */
  int   dqln, /* G.72x codeword */
  int   y );  /* Step size multiplier */
void g723_enc_update(
  int   code_size,  /* distinguish 723_40 with others */
  int   y,    /* quantizer step size */
  int   wi,   /* scale factor multiplier */
  int   fi,   /* for long/short term energies */
  int   dq,   /* quantized prediction difference */
  int   sr,   /* reconstructed signal */
  int   dqsez,    /* difference from 2-pole predictor */
  struct g723_enc_state_t *state_ptr ); /* coder state pointer */
int g723_enc_quan(
  int   val,
  short   *table,
  int   size );
int g723_enc_search(
  int   val,
  short   *table,
  int   size );
int g723_enc_alaw2linear( unsigned char a_val );
int g723_enc_ulaw2linear( unsigned char u_val );
int g723_enc_g723_24_encoder(
  int sample,
  int in_coding,
  struct g723_enc_state_t *state_ptr );
int g723_enc_pack_output(
  unsigned  char  code,
  int     bits );

void g723_enc_init();
int g723_enc_return();
void g723_enc_main();
int main( void );

/*
  Declaration of global variables
*/

struct g723_enc_state_t g723_enc_state;

unsigned int g723_enc_INPUT[ 256 ] = {
  51, 17, 31, 53, 95, 17, 70, 22, 49, 12,  8, 39, 28, 37, 99, 54,
  77, 65, 77, 78, 83, 15, 63, 31, 35, 92, 52, 40, 61, 79, 94, 87,
  87, 68, 76, 58, 39, 35, 20, 83, 42, 46, 98, 12, 21, 96, 74, 41,
  78, 76, 96,  2, 32, 76, 24, 59,  4, 96, 32,  5, 44, 92, 57, 12,
  57, 25, 50, 23, 48, 41, 88, 43, 36, 38,  4, 16, 52, 70,  9, 40,
  78, 24, 34, 23, 30, 30, 89,  3, 65, 40, 68, 73, 94, 23, 84, 97,
  78, 43, 68, 81, 16, 28, 13, 87, 75, 21, 14, 29, 81, 22, 56, 72,
  19, 99, 25, 43, 76, 86, 90, 98, 39, 43, 12, 46, 24, 99, 65, 61,
  24, 45, 79,  7, 48, 15, 24, 95, 62, 99, 48, 80, 75, 38, 48, 53,
  9, 60, 35, 14, 78, 71, 45, 71,  9, 97, 55, 74, 58, 64, 78, 18,
  30, 28, 69, 29, 57, 42, 30, 44, 57, 49, 61, 42, 13, 25,  3, 98,
  11, 38, 65, 35, 55, 36, 57, 48, 16, 62, 17, 56, 29, 88, 84, 85,
  90, 60, 54, 16, 66, 69, 26, 10, 82, 19, 42, 35, 84, 13, 26, 17,
  48, 38, 50, 50, 35, 53, 12, 52, 61, 74, 56, 34, 80, 59, 26, 67,
  55, 79, 89, 89,  6, 80, 91, 65, 16, 30, 16, 28, 85, 54,  3, 20,
  2, 36, 62, 52, 55, 15, 83,  3,  2, 38, 62,  2, 63, 92, 37, 73
};



unsigned int g723_enc_OUTPUT[ 256 ];

short g723_enc_power2[ 15 ] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
                             0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000
                            };


/*
   Maps G.723_24 code word to reconstructed scale factor normalized log
   magnitude values.
*/

short g723_enc_qtab_723_24[ 3 ] = {8, 218, 331};

/*
   Maps G.721 code word to reconstructed scale factor normalized log
   magnitude values.
*/
short  g723_enc_dqlntab[ 16 ] = { -2048, 4, 135, 213, 273, 323, 373, 425,
                                425, 373, 323, 273, 213, 135, 4, -2048
                              };

/* Maps G.721 code word to log of scale factor multiplier. */
short  g723_enc_witab[ 16 ] = { -12, 18, 41, 64, 112, 198, 355, 1122,
                              1122, 355, 198, 112, 64, 41, 18, -12
                            };
/*
   Maps G.721 code words to a set of values whose long and short
   term averages are computed and then compared to give an indication
   how stationary (steady state) the signal is.
*/
short g723_enc_fitab[ 16 ] = {0, 0, 0, 0x200, 0x200, 0x200, 0x600, 0xE00,
                            0xE00, 0x600, 0x200, 0x200, 0x200, 0, 0, 0
                           };


/*
  Declaration of macros
*/


#define AUDIO_ENCODING_ULAW (1) /* ISDN u-law */
#define AUDIO_ENCODING_ALAW (2) /* ISDN A-law */
#define AUDIO_ENCODING_LINEAR (3) /* PCM 2's-complement (0-center) */

#define BIAS    (0x84)    /* Bias for linear code. */

#define SIGN_BIT  (0x80)    /* Sign bit for a A-law byte. */
#define QUANT_MASK  (0xf)   /* Quantization field mask. */
#define SEG_SHIFT (4)   /* Left shift for segment number. */
#define SEG_MASK  (0x70)    /* Segment field mask. */

/*
  Arithmetic math functions
*/

/*
   g723_enc_fmult()

   returns the integer product of the 14-bit integer "an" and
   "floating point" representation (4-bit exponent, 6-bit mantessa) "srn".
*/
int g723_enc_fmult(
  int   an,
  int   srn )
{
  short   anmag, anexp, anmant;
  short   wanexp, wanmant;
  short   retval;

  anmag = ( an > 0 ) ? an : ( ( -an ) & 0x1FFF );
  anexp = g723_enc_quan( anmag, g723_enc_power2, 3 ) - 6;
  anmant = ( anmag == 0 ) ? 32 :
           ( anexp >= 0 ) ? anmag >> anexp : anmag << -anexp;
  wanexp = anexp + ( ( srn >> 6 ) & 0xF ) - 13;

  wanmant = ( anmant * ( srn & 077 ) + 0x30 ) >> 4;
  retval = ( wanexp >= 0 ) ? ( ( wanmant << wanexp ) & 0x7FFF ) :
           ( wanmant >> -wanexp );

  return ( ( ( an ^ srn ) < 0 ) ? -retval : retval );
}


/* Manish Verma */
int g723_enc_abs( int num )
{
  return ( num < 0 ) ? -num : num;
}


/*
  Algorithm core functions
*/


/*
   g723_enc_quan()

   quantizes the input val against the table of size short integers.
   It returns i if table[ i - 1 ] <= val < table[ i ].

   Using linear search for simple coding.
*/
int g723_enc_quan(
  int     val,
  short   *table,
  int   size )
{
  int i,
      j = 0,
      k = 1;

  _Pragma( "loopbound min 3 max 15" )
  for ( i = 0; i < size; ++i ) {

    if ( k ) {
      if ( val < *table++ ) {
        j = i;
        k = 0;
      }
    }
  }

  return ( j );
}


/*
   g723_enc_predictor_zero()

   computes the estimated signal from 6-zero predictor.

*/
int
g723_enc_predictor_zero(
  struct g723_enc_state_t *state_ptr )
{
  int   i;
  int   sezi;

  sezi = g723_enc_fmult( state_ptr->b[ 0 ] >> 2, state_ptr->dq[ 0 ] );
  _Pragma( "loopbound min 5 max 5" )
  for ( i = 1; i < 6; i++ )     /* ACCUM */
    sezi += g723_enc_fmult( state_ptr->b[ i ] >> 2, state_ptr->dq[ i ] );

  return ( sezi );
}


/*
   g723_enc_predictor_pole()

   computes the estimated signal from 2-pole predictor.

*/
int
g723_enc_predictor_pole(
  struct g723_enc_state_t *state_ptr )
{
  return ( g723_enc_fmult( state_ptr->a[ 1 ] >> 2, state_ptr->sr[ 1 ] ) +
           g723_enc_fmult( state_ptr->a[ 0 ] >> 2, state_ptr->sr[ 0 ] ) );
}

/*
   g723_enc_step_size()

   computes the quantization step size of the adaptive quantizer.

*/
int
g723_enc_step_size(
  struct g723_enc_state_t *state_ptr )
{
  int   y;
  int   dif;
  int   al;

  if ( state_ptr->ap >= 256 )
    return ( state_ptr->yu );
  else {
    y = state_ptr->yl >> 6;
    dif = state_ptr->yu - y;
    al = state_ptr->ap >> 2;
    if ( dif > 0 )
      y += ( dif * al ) >> 6;
    else
      if ( dif < 0 )
        y += ( dif * al + 0x3F ) >> 6;

    return ( y );
  }
}

/*
   g723_enc_quantize()

   Given a raw sample, 'd', of the difference signal and a
   quantization step size scale factor, 'y', this routine returns the
   ADPCM codeword to which that sample gets quantized.  The step
   size scale factor division operation is done in the log base 2 domain
   as a subtraction.
*/
int
g723_enc_quantize(
  int   d,  /* Raw difference signal sample */
  int   y,  /* Step size multiplier */
  short   *table, /* quantization table */
  int   size )  /* table size of short integers */
{
  short   dqm;  /* Magnitude of 'd' */
  short   exp;  /* Integer part of base 2 log of 'd' */
  short   mant; /* Fractional part of base 2 log */
  short   dl; /* Log of magnitude of 'd' */
  short   dln;  /* Step size scale factor normalized log */
  int   i;

  /*
     LOG

     Compute base 2 log of 'd', and store in 'dl'.
  */
  dqm = g723_enc_abs( d );
  exp = g723_enc_quan( dqm >> 1, g723_enc_power2, 15 );
  mant = ( ( dqm << 7 ) >> exp ) & 0x7F;  /* Fractional portion. */
  dl = ( exp << 7 ) + mant;

  /*
     SUBTB

     "Divide" by step size multiplier.
  */
  dln = dl - ( y >> 2 );

  /*
     QUAN

     Obtain codword i for 'd'.
  */
  i = g723_enc_quan( dln, table, size );

  if ( d < 0 )      /* take 1's complement of i */
    return ( ( size << 1 ) + 1 - i );
  else
    if ( i == 0 )   /* take 1's complement of 0 */
      return ( ( size << 1 ) + 1 ); /* new in 1988 */
    else
      return ( i );
}
/*
   g723_enc_reconstruct()

   Returns reconstructed difference signal 'dq' obtained from
   codeword 'i' and quantization step size scale factor 'y'.
   Multiplication is performed in log base 2 domain as addition.
*/
int
g723_enc_reconstruct(
  int   sign, /* 0 for non-negative value */
  int   dqln, /* G.72x codeword */
  int   y ) /* Step size multiplier */
{
  short   dql;  /* Log of 'dq' magnitude */
  short   dex;  /* Integer part of log */
  short   dqt;
  short   dq; /* Reconstructed difference signal sample */

  dql = dqln + ( y >> 2 );  /* ADDA */

  if ( dql < 0 )
    return ( ( sign ) ? -0x8000 : 0 );
  else {    /* ANTILOG */
    dex = ( dql >> 7 ) & 15;
    dqt = 128 + ( dql & 127 );
    dq = ( dqt << 7 ) >> ( 14 - dex );
    return ( ( sign ) ? ( dq - 0x8000 ) : dq );
  }
}


/*
   g723_enc_update()

   updates the state variables for each output code
*/
void
g723_enc_update(
  int   code_size,  /* distinguish 723_40 with others */
  int   y,    /* quantizer step size */
  int   wi,   /* scale factor multiplier */
  int   fi,   /* for long/short term energies */
  int   dq,   /* quantized prediction difference */
  int   sr,   /* reconstructed signal */
  int   dqsez,    /* difference from 2-pole predictor */
  struct g723_enc_state_t *state_ptr )  /* coder state pointer */
{
  int   cnt;
  short   mag, exp; /* Adaptive predictor, FLOAT A */
  short   a2p;    /* LIMC */
  short   a1ul;   /* UPA1 */
  short   pks1;  /* UPA2 */
  short   fa1;
  char    tr;   /* tone/transition detector */
  short   ylint, thr2, dqthr;
  short     ylfrac, thr1;
  short   pk0;

  pk0 = ( dqsez < 0 ) ? 1 : 0;  /* needed in updating predictor poles */

  mag = dq & 0x7FFF;    /* prediction difference magnitude */
  /* TRANS */
  ylint = state_ptr->yl >> 15;  /* exponent part of yl */
  ylfrac = ( state_ptr->yl >> 10 ) & 0x1F;  /* fractional part of yl */
  thr1 = ( 32 + ylfrac ) << ylint;    /* threshold */
  thr2 = ( ylint > 9 ) ? 31 << 10 : thr1; /* limit thr2 to 31 << 10 */
  dqthr = ( thr2 + ( thr2 >> 1 ) ) >> 1;  /* dqthr = 0.75 * thr2 */
  if ( state_ptr->td == 0 )   /* signal supposed voice */
    tr = 0;
  else
    if ( mag <= dqthr )   /* supposed data, but small mag */
      tr = 0;     /* treated as voice */
    else        /* signal is data (modem) */
      tr = 1;

  /*
     Quantizer scale factor adaptation.
  */

  /* FUNCTW & FILTD & DELAY */
  /* update non-steady state step size multiplier */
  state_ptr->yu = y + ( ( wi - y ) >> 5 );

  /* LIMB */
  if ( state_ptr->yu < 544 )  /* 544 <= yu <= 5120 */
    state_ptr->yu = 544;
  else
    if ( state_ptr->yu > 5120 )
      state_ptr->yu = 5120;

  /* FILTE & DELAY */
  /* update steady state step size multiplier */
  state_ptr->yl += state_ptr->yu + ( ( -state_ptr->yl ) >> 6 );

  /*
     Adaptive predictor coefficients.
  */
  if ( tr == 1 ) {      /* reset a's and b's for modem signal */
    state_ptr->a[ 0 ] = 0;
    state_ptr->a[ 1 ] = 0;
    state_ptr->b[ 0 ] = 0;
    state_ptr->b[ 1 ] = 0;
    state_ptr->b[ 2 ] = 0;
    state_ptr->b[ 3 ] = 0;
    state_ptr->b[ 4 ] = 0;
    state_ptr->b[ 5 ] = 0;
  } else {      /* update a's and b's */
    pks1 = pk0 ^ state_ptr->pk[ 0 ];    /* UPA2 */

    /* update predictor pole a[ 1 ] */
    a2p = state_ptr->a[ 1 ] - ( state_ptr->a[ 1 ] >> 7 );
    if ( dqsez != 0 ) {
      fa1 = ( pks1 ) ? state_ptr->a[ 0 ] : -state_ptr->a[ 0 ];
      if ( fa1 < -8191 )  /* a2p = function of fa1 */
        a2p -= 0x100;
      else
        if ( fa1 > 8191 )
          a2p += 0xFF;
        else
          a2p += fa1 >> 5;
      if ( pk0 ^ state_ptr->pk[ 1 ] )
        /* LIMC */
        if ( a2p <= -12160 )
          a2p = -12288;
        else
          if ( a2p >= 12416 )
            a2p = 12288;
          else
            a2p -= 0x80;
      else
        if ( a2p <= -12416 )
          a2p = -12288;
        else
          if ( a2p >= 12160 )
            a2p = 12288;
          else
            a2p += 0x80;

    }

    /* TRIGB & DELAY */
    state_ptr->a[ 1 ] = a2p;

    /* UPA1 */
    /* update predictor pole a[ 0 ] */
    state_ptr->a[ 0 ] -= state_ptr->a[ 0 ] >> 8;
    if ( dqsez != 0 )  {
      if ( pks1 == 0 )
        state_ptr->a[ 0 ] += 192;
      else
        state_ptr->a[ 0 ] -= 192;
    }

    /* LIMD */
    a1ul = 15360 - a2p;
    if ( state_ptr->a[ 0 ] < -a1ul )
      state_ptr->a[ 0 ] = -a1ul;
    else
      if ( state_ptr->a[ 0 ] > a1ul )
        state_ptr->a[ 0 ] = a1ul;

    /* UPB : update predictor zeros b[ 6 ] */
    _Pragma( "loopbound min 6 max 6" )
    for ( cnt = 0; cnt < 6; cnt++ ) {
      if ( code_size == 5 )   /* for 40Kbps G.723 */
        state_ptr->b[ cnt ] -= state_ptr->b[ cnt ] >> 9;
      else      /* for G.721 and 24Kbps G.723 */
        state_ptr->b[ cnt ] -= state_ptr->b[ cnt ] >> 8;
      if ( dq & 0x7FFF ) {      /* XOR */
        if ( ( dq ^ state_ptr->dq[ cnt ] ) >= 0 )
          state_ptr->b[ cnt ] += 128;
        else
          state_ptr->b[ cnt ] -= 128;
      }

    }
  }

  _Pragma( "loopbound min 5 max 5" )
  for ( cnt = 5; cnt > 0; cnt-- )
    state_ptr->dq[ cnt ] = state_ptr->dq[ cnt - 1 ];
  /* FLOAT A : convert dq[ 0 ] to 4-bit exp, 6-bit mantissa f.p. */
  if ( mag == 0 )
    state_ptr->dq[ 0 ] = ( dq >= 0 ) ? 0x20 : 0xFC20;
  else {
    exp = g723_enc_quan( mag, g723_enc_power2, 15 );
    state_ptr->dq[ 0 ] = ( dq >= 0 ) ?
                       ( exp << 6 ) + ( ( mag << 6 ) >> exp ) :
                       ( exp << 6 ) + ( ( mag << 6 ) >> exp ) - 0x400;

  }

  state_ptr->sr[ 1 ] = state_ptr->sr[ 0 ];
  /* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
  if ( sr == 0 )
    state_ptr->sr[ 0 ] = 0x20;
  else
    if ( sr > 0 ) {
      exp = g723_enc_quan( sr, g723_enc_power2, 15 );
      state_ptr->sr[ 0 ] = ( exp << 6 ) + ( ( sr << 6 ) >> exp );
    } else
      if ( sr > -32768 ) {
        mag = -sr;
        exp = g723_enc_quan( mag, g723_enc_power2, 15 );
        state_ptr->sr[ 0 ] =  ( exp << 6 ) + ( ( mag << 6 ) >> exp ) - 0x400;
      } else
        state_ptr->sr[ 0 ] = 0xFC20;

  /* DELAY A */
  state_ptr->pk[ 1 ] = state_ptr->pk[ 0 ];
  state_ptr->pk[ 0 ] = pk0;

  /* TONE */
  if ( tr == 1 )    /* this sample has been treated as data */
    state_ptr->td = 0;  /* next one will be treated as voice */
  else
    if ( a2p < -11776 ) /* small sample-to-sample correlation */
      state_ptr->td = 1;  /* signal may be data */
    else        /* signal is voice */
      state_ptr->td = 0;

  /*
     Adaptation speed control.
  */
  state_ptr->dms += ( fi - state_ptr->dms ) >> 5;   /* FILTA */
  state_ptr->dml += ( ( ( fi << 2 ) - state_ptr->dml ) >> 7 );  /* FILTB */

  if ( tr == 1 )
    state_ptr->ap = 256;
  else
    if ( y < 1536 )         /* SUBTC */
      state_ptr->ap += ( 0x200 - state_ptr->ap ) >> 4;
    else
      if ( state_ptr->td == 1 )
        state_ptr->ap += ( 0x200 - state_ptr->ap ) >> 4;
      else
        if ( g723_enc_abs( ( state_ptr->dms << 2 ) - state_ptr->dml ) >=
             ( state_ptr->dml >> 3 ) )
          state_ptr->ap += ( 0x200 - state_ptr->ap ) >> 4;
        else
          state_ptr->ap += ( -state_ptr->ap ) >> 4;

}


/*
   g723_enc_alaw2linear() - Convert an A-law value to 16-bit linear PCM

*/
int
g723_enc_alaw2linear(
  unsigned char a_val )
{
  int   t;
  int   seg;

  a_val ^= 0x55;

  t = ( a_val & QUANT_MASK ) << 4;
  seg = ( ( unsigned )a_val & SEG_MASK ) >> SEG_SHIFT;
  switch ( seg ) {
    case 0:
      t += 8;
      break;
    case 1:
      t += 0x108;
      break;
    default:
      t += 0x108;
      t <<= seg - 1;
  }
  return ( ( a_val & SIGN_BIT ) ? t : -t );
}


/*
   g723_enc_ulaw2linear() - Convert a u-law value to 16-bit linear PCM

   First, a biased linear code is derived from the code word. An unbiased
   output can then be obtained by subtracting 33 from the biased code.

   Note that this function expects to be passed the complement of the
   original code word. This is in keeping with ISDN conventions.
*/
int
g723_enc_ulaw2linear(
  unsigned char u_val )
{
  int   t;

  /* Complement to obtain normal u-law value. */
  u_val = ~u_val;

  /*
     Extract and bias the quantization bits. Then
     shift up by the segment number and subtract out the bias.
  */
  t = ( ( u_val & QUANT_MASK ) << 3 ) + BIAS;
  t <<= ( ( unsigned int )u_val & SEG_MASK ) >> SEG_SHIFT;

  return ( ( u_val & SIGN_BIT ) ? ( BIAS - t ) : ( t - BIAS ) );
}


/*
   g723_enc_g723_24_encoder()

   Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
   Returns -1 if invalid input coding value.
*/
int
g723_enc_g723_24_encoder(
  int   sl,
  int   in_coding,
  struct g723_enc_state_t *state_ptr )
{
  short   sei, sezi, se, sez; /* ACCUM */
  short   d;      /* SUBTA */
  short   y;      /* MIX */
  short   sr;     /* ADDB */
  short   dqsez;      /* ADDC */
  short   dq, i;

  switch ( in_coding ) {  /* linearize input sample to 14-bit PCM */
    case AUDIO_ENCODING_ALAW:
      sl = g723_enc_alaw2linear( sl ) >> 2;
      break;
    case AUDIO_ENCODING_ULAW:
      sl = g723_enc_ulaw2linear( sl ) >> 2;
      break;
    case AUDIO_ENCODING_LINEAR:
      sl >>= 2;   /* sl of 14-bit dynamic range */
      break;
    default:
      return ( -1 );
  }

  sezi = g723_enc_predictor_zero( state_ptr );
  sez = sezi >> 1;
  sei = sezi + g723_enc_predictor_pole( state_ptr );
  se = sei >> 1;      /* se = estimated signal */

  d = sl - se;      /* d = estimation diff. */

  /* quantize prediction difference d */
  y = g723_enc_step_size( state_ptr ); /* quantizer step size */
  i = g723_enc_quantize( d, y, g723_enc_qtab_723_24, 3 ); /* i = ADPCM code */
  dq = g723_enc_reconstruct( i & 4, g723_enc_dqlntab[ i ],
                             y ); /* quantized diff. */

  sr = ( dq < 0 ) ? se - ( dq & 0x3FFF ) : se + dq; /* reconstructed signal */

  dqsez = sr + sez - se;    /* pole prediction diff. */

  g723_enc_update( 3, y, g723_enc_witab[ i ], g723_enc_fitab[ i ], dq, sr, dqsez,
                   state_ptr );

  return ( i );
}

/*
   Pack output codes into bytes and write them to stdout.
   Returns 1 if there is residual output, else returns 0.
*/
int
g723_enc_pack_output(
  unsigned  char  code,
  int     bits )
{
  static unsigned int out_buffer = 0;
  static int    out_bits = 0;
  unsigned char   out_byte;
  static int              i = 0;

  out_buffer |= ( code << out_bits );
  out_bits += bits;
  if ( out_bits >= 8 ) {
    out_byte = out_buffer & 0xff;
    out_bits -= 8;
    out_buffer >>= 8;
    //fwrite(&out_byte, sizeof (char), 1, fp_out);
    //fwrite(&out_byte, 1, 1, fp_out);
    g723_enc_OUTPUT[ i ] = out_byte;
    i =  i + 1;
  }

  return ( out_bits > 0 );
}

/*
  Initialization- and return-value-related functions
*/

/*
   g723_enc_init_state()

   This routine initializes and/or resets the g72x_state structure
   pointed to by 'state_ptr'.
   All the initial state values are specified in the CCITT G.721 document.
*/
void
g723_enc_init_state(
  struct g723_enc_state_t *state_ptr )
{
  int   cnta;

  state_ptr->yl = 34816;
  state_ptr->yu = 544;
  state_ptr->dms = 0;
  state_ptr->dml = 0;
  state_ptr->ap = 0;

  _Pragma( "loopbound min 2 max 2" )
  for ( cnta = 0; cnta < 2; cnta++ ) {
    state_ptr->a[ cnta ] = 0;
    state_ptr->pk[ cnta ] = 0;
    state_ptr->sr[ cnta ] = 32;
  }
  _Pragma( "loopbound min 6 max 6" )
  for ( cnta = 0; cnta < 6; cnta++ ) {
    state_ptr->b[ cnta ] = 0;
    state_ptr->dq[ cnta ] = 32;
  }
  state_ptr->td = 0;
}


void g723_enc_init()
{
  int i;
  volatile int x = 0;
  g723_enc_init_state( &g723_enc_state );

  _Pragma( "loopbound min 256 max 256" )
  for ( i = 0; i < 256; i++ )
    g723_enc_INPUT[ i ] += x;
}


int g723_enc_return()
{
  int i;
  int check_sum = 0;

  _Pragma( "loopbound min 256 max 256" )
  for ( i = 0; i < 256; i++ )
    check_sum += g723_enc_OUTPUT[ i ];

  return ( check_sum != 24284 );
}

/*
  Main functions
*/

void _Pragma( "entrypoint" ) g723_enc_main()
{
//  struct g72x_state state;
  short   sample_short; //mv
  unsigned char   code;
  int     resid;
  int     in_coding;
  short    *in_buf;
  int     enc_bits;
  int i = 0;

  enc_bits = 3;
  in_coding = AUDIO_ENCODING_ALAW;
  in_buf = &sample_short;

  _Pragma( "loopbound min 256 max 256" )
  for ( i = 0; i < 256; i++ ) {
    *in_buf = g723_enc_INPUT[ i ];
    code = g723_enc_g723_24_encoder( sample_short, in_coding, &g723_enc_state );
    resid = g723_enc_pack_output( code, enc_bits );
  }

  /* Write zero codes until all residual codes are written out */
  _Pragma( "loopbound min 0 max 0" )
  while ( resid )
    resid = g723_enc_pack_output( 0, enc_bits );
}


int main( void )
{
  g723_enc_init();
  g723_enc_main();

  return ( g723_enc_return() );
}

