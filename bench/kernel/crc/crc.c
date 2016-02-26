/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: crc

  Author: unknown

  Function: crc is a demonstration for CRC (Cyclic Redundancy Check) operation.
  The CRC is manipulated as two functions, icrc1 and icrc.
  icrc1 is for one character and icrc uses icrc1 for a string.
  The input string is stored in array lin[].
  icrc is called two times, one for X-Modem string CRC and the
  other for X-Modem packet CRC.

  Source: SNU-RT Benchmark Suite for Worst Case Timing Analysis

  Changes: no major functional change

  License: general open-source

*/

/*
  Declaration of macros
*/

#define CRC_LOBYTE(x) ((unsigned char)((x) & 0xFF))
#define CRC_HIBYTE(x) ((unsigned char)((x) >> 8))


/*
  Forward declaration of functions
*/

void crc_init();
unsigned short crc_icrc1( unsigned short crc, unsigned char onech );
unsigned short crc_icrc( unsigned short crc, unsigned long len, short jinit,
                         int jrev );
int crc_main( void );

/*
  Declaration of global variables
*/

unsigned char crc_lin[ 256 ] = "asdffeagewaHAFEFaeDsFEawFdsFaefaeerdjgp";

/*
  Initialization function
*/

/*
  crc_init initializes the string.
*/

void crc_init()
{
  unsigned char tmp_lin[ 256 ] = "asdffeagewaHAFEFaeDsFEawFdsFaefaeerdjgp";
  int i;

  for ( i = 0 ; i < 256 ; i++ )
    crc_lin[ i ] = tmp_lin[ i ];
}


/*
  Algorithm core functions
*/

unsigned short crc_icrc1( unsigned short crc, unsigned char onech )
{
  int i;
  unsigned short ans = ( crc ^ onech << 8 );

  _Pragma( "loopbound min 8 max 8" )
  for ( i = 0; i < 8; i++ ) {
    if ( ans & 0x8000 )
      ans = ( ans << 1 ) ^ 4129;
    else
      ans <<= 1;
  }
  return ans;
}


unsigned short crc_icrc( unsigned short crc, unsigned long len, short jinit,
                         int jrev )
{
  static unsigned short icrctb[ 256 ], init = 0;
  static unsigned char rchr[ 256 ];
  unsigned short tmp1, tmp2, j, cword = crc;
  static unsigned char it[ 16 ] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };

  if ( !init ) {
    init = 1;
    _Pragma( "loopbound min 256 max 256" )
    for ( j = 0; j <= 255; j++ ) {
      icrctb[ j ] = crc_icrc1( j << 8, ( unsigned char )0 );
      rchr[ j ] = ( unsigned char )( it[ j & 0xF ] << 4 | it[ j >> 4 ] );
    }
  }

  if ( jinit >= 0 )
    cword = ( ( unsigned char ) jinit ) | ( ( ( unsigned char ) jinit ) << 8 );
  else
    if ( jrev < 0 )
      cword = rchr[ CRC_HIBYTE( cword ) ] | rchr[ CRC_LOBYTE( cword ) ] << 8;

  _Pragma( "loopbound min 40 max 42" )
  for ( j = 1; j <= len; j++ ) {

    if ( jrev < 0 )
      tmp1 = rchr[ crc_lin[ j ] ] ^ CRC_HIBYTE( cword );
    else
      tmp1 = crc_lin[ j ] ^ CRC_HIBYTE( cword );

    cword = icrctb[ tmp1 ] ^ CRC_LOBYTE( cword ) << 8;
  }

  if ( jrev >= 0 )
    tmp2 = cword;
  else
    tmp2 = rchr[ CRC_HIBYTE( cword ) ] | rchr[ CRC_LOBYTE( cword ) ] << 8;

  return ( tmp2 );
}


int _Pragma( "entrypoint" ) crc_main( void )
{
  unsigned short i1, i2;
  unsigned long n;

  n = 40;
  crc_lin[ n + 1 ] = 0;

  i1 = crc_icrc(  0, n, ( short )0, 1 );

  crc_lin[ n + 1 ] = CRC_HIBYTE( i1 );
  crc_lin[ n + 2 ] = CRC_LOBYTE( i1 );

  i2 = crc_icrc( i1, n + 2, ( short )0, 1 );

  return i2;
}

int main( void )
{
  int res;
  crc_init();
  res = crc_main();
  return res;
}


