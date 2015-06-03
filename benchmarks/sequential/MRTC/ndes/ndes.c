/*
 * DESCRIPTION: Complex embedded code
 *
 * COMMENTS: A lot of bit manipulation, shifts, array and matrix calculations
 *
*/ 

/* Remove the following #define for actual WCET analyses! */
/*
#define PROFILING
*/

#ifdef PROFILING
#include <stdio.h>
#endif

#ifdef PROFILING
/* Profiling variables. Remove for actual WCET analyses. */
int des_iters_j1 = 0, des_min_j1 = 100000, des_max_j1 = 0;
int des_iters_j2 = 0, des_min_j2 = 100000, des_max_j2 = 0;
int des_iters_i1 = 0, des_min_i1 = 100000, des_max_i1 = 0;
int des_iters_j3 = 0, des_min_j3 = 100000, des_max_j3 = 0;
int des_iters_i2 = 0, des_min_i2 = 100000, des_max_i2 = 0;
int des_iters_j4 = 0, des_min_j4 = 100000, des_max_j4 = 0;
#endif

#ifdef PROFILING
/* Profiling variables. Remove for actual WCET analyses. */
int ks_iters_i = 0, ks_min_i = 100000, ks_max_i = 0;
int ks_iters_j = 0, ks_min_j = 100000, ks_max_j = 0;
#endif

#ifdef PROFILING
/* Profiling variables. Remove for actual WCET analyses. */
int cyfun_iters_j1 = 0, cyfun_min_j1 = 100000, cyfun_max_j1 = 0;
int cyfun_iters_j2 = 0, cyfun_min_j2 = 100000, cyfun_max_j2 = 0;
int cyfun_iters_jj = 0, cyfun_min_jj = 100000, cyfun_max_jj = 0;
int cyfun_iters_j3 = 0, cyfun_min_j3 = 100000, cyfun_max_j3 = 0;
#endif

/* All output disabled for wcsim */

/* A read from this address will result in an known value of 1 */
#define KNOWN_VALUE 1

#define WORSTCASE 1

typedef struct IMMENSE { unsigned long l, r; } immense;
typedef struct GREAT { unsigned long l, c, r; } great;

unsigned long bit[33];

static immense icd;
static char ipc1[57]={0,57,49,41,33,25,17,9,1,58,50,
   42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,
   52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,
   30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
static char ipc2[49]={0,14,17,11,24,1,5,3,28,15,6,21,
   10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,
   37,47,55,30,40,51,45,33,48,44,49,39,56,34,
   53,46,42,50,36,29,32};

/* Forward fuuntion prototypes */
void des(immense inp, immense key, int * newkey, int isw, immense * out);
void cyfun(unsigned long ir, great k, unsigned long * iout);
unsigned long getbit(immense source, int bitno, int nbits);
void ks(/*immense key, */int n, great * kn);


void des( immense inp, immense key, int * newkey, int isw, immense * out ) 
{
  static char ip[65] =
  {0,58,50,42,34,26,18,10,2,60,52,44,36,
    28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,
    32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,
    27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,
    31,23,15,7};
  
  static char ipm[65]=
  {0,40,8,48,16,56,24,64,32,39,7,47,15,
    55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,
    53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,
    51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,
    49,17,57,25};
  static great kns[17];
#ifdef WORSTCASE
  static int initflag=1;
#else
  static int initflag=0;
#endif
  int ii,i,j,k;
  unsigned long ic,shifter;
  immense itmp;
  great pg;

  if (initflag) {
    initflag = 0;
    bit[1] = shifter = 1L;

    #ifdef PROFILING
    des_iters_j1 = 0;
    #endif    
    _Pragma("loopbound min 31 max 31")
    for( j = 2; j <= 32; j++ ) {
      #ifdef PROFILING
      des_iters_j1++;
      #endif    
      bit[j] = (shifter <<= 1);
    }
    #ifdef PROFILING
    if ( des_iters_j1 < des_min_j1 )
      des_min_j1 = des_iters_j1;
    if ( des_iters_j1 > des_max_j1 )
      des_max_j1 = des_iters_j1;
    #endif
    
  }
  
  if ( *newkey ) {
    *newkey = 0;
    icd.r = icd.l = 0L;
    
    #ifdef PROFILING
    des_iters_j2 = 0;
    #endif    
    _Pragma("loopbound min 28 max 28")
    for ( j = 28, k = 56; j >= 1; j--, k-- ) {
      #ifdef PROFILING
      des_iters_j2++;
      #endif    
      icd.r = (icd.r << 1) | getbit(key,ipc1[j],32);
      icd.l = (icd.l <<= 1) | getbit(key,ipc1[k],32);
    }
    #ifdef PROFILING
    if ( des_iters_j2 < des_min_j2 )
      des_min_j2 = des_iters_j2;
    if ( des_iters_j2 > des_max_j2 )
      des_max_j2 = des_iters_j2;
    #endif

    #ifdef PROFILING
    des_iters_i1 = 0;
    #endif    
    _Pragma("loopbound min 16 max 16")
    for( i = 1; i <= 16; i++ ) {
      #ifdef PROFILING
      des_iters_i1++;
      #endif    
      pg = kns[i]; 
      ks(/* key,*/ i, &pg); 
      kns[i] = pg;
    }
    #ifdef PROFILING
    if ( des_iters_i1 < des_min_i1 )
      des_min_i1 = des_iters_i1;
    if ( des_iters_i1 > des_max_i1 )
      des_max_i1 = des_iters_i1;
    #endif
  }
  
  itmp.r = itmp.l = 0L;

  #ifdef PROFILING
  des_iters_j3 = 0;
  #endif    
  _Pragma("loopbound min 32 max 32")
  for ( j = 32, k = 64; j >= 1; j--,k-- ) {
    #ifdef PROFILING
    des_iters_j3++;
    #endif    
    itmp.r = (itmp.r <<= 1) | getbit(inp,ip[j],32);
    itmp.l = (itmp.l <<= 1) | getbit(inp,ip[k],32);
  }
  #ifdef PROFILING
  if ( des_iters_j3 < des_min_j3 )
    des_min_j3 = des_iters_j3;
  if ( des_iters_j3 > des_max_j3 )
    des_max_j3 = des_iters_j3;
  #endif
  
  #ifdef PROFILING
  des_iters_i2 = 0;
  #endif    
  _Pragma("loopbound min 16 max 16")
  for ( i = 1; i <= 16; i++ ) {
    #ifdef PROFILING
    des_iters_i2++;
    #endif    
    ii = (isw == 1 ? 17-i : i);
    cyfun(itmp.l, kns[ii], &ic);
    ic ^= itmp.r;
    itmp.r=itmp.l;
    itmp.l=ic;
  }
  #ifdef PROFILING
  if ( des_iters_i2 < des_min_i2 )
    des_min_i2 = des_iters_i2;
  if ( des_iters_i2 > des_max_i2 )
    des_max_i2 = des_iters_i2;
  #endif
  
  ic = itmp.r;
  itmp.r = itmp.l;
  itmp.l = ic;
  (*out).r = (*out).l = 0L;
  
  #ifdef PROFILING
  des_iters_j4 = 0;
  #endif    
  _Pragma("loopbound min 32 max 32")
  for ( j = 32, k = 64; j >= 1; j--, k-- ) {
    #ifdef PROFILING
    des_iters_j4++;
    #endif    
    (*out).r = ((*out).r <<= 1) | getbit(itmp,ipm[j],32);
    (*out).l = ((*out).l <<= 1) | getbit(itmp,ipm[k],32);
  }
  #ifdef PROFILING
  if ( des_iters_j4 < des_min_j4 )
    des_min_j4 = des_iters_j4;
  if ( des_iters_j4 > des_max_j4 )
    des_max_j4 = des_iters_j4;
  #endif
}


void cyfun( unsigned long ir, great k, unsigned long * iout ) 
{
  static char iet[49]={0,32,1,2,3,4,5,4,5,6,7,8,9,8,9,
    10,11,12,13,12,13,14,15,16,17,16,17,18,19,
    20,21,20,21,22,23,24,25,24,25,26,27,28,29,
    28,29,30,31,32,1};
  static char ipp[33]={0,16,7,20,21,29,12,28,17,1,15,
    23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,
    30,6,22,11,4,25};
  static char is[16][4][9]={
    0,14,15,10,7,2,12,4,13,0,0,3,13,13,14,10,13,1,
    0,4,0,13,10,4,9,1,7,0,15,13,1,3,11,4,6,2,
    0,4,1,0,13,12,1,11,2,0,15,13,7,8,11,15,0,15,
    0,1,14,6,6,2,14,4,11,0,12,8,10,15,8,3,11,1,
    0,13,8,9,14,4,10,2,8,0,7,4,0,11,2,4,11,13,
    0,14,7,4,9,1,15,11,4,0,8,10,13,0,12,2,13,14,
    0,1,14,14,3,1,15,14,4,0,4,7,9,5,12,2,7,8,
    0,8,11,9,0,11,5,13,1,0,2,1,0,6,7,12,8,7,
    0,2,6,6,0,7,9,15,6,0,14,15,3,6,4,7,4,10,
    0,13,10,8,12,10,2,12,9,0,4,3,6,10,1,9,1,4,
    0,15,11,3,6,10,2,0,15,0,2,2,4,15,7,12,9,3,
    0,6,4,15,11,13,8,3,12,0,9,15,9,1,14,5,4,10,
    0,11,3,15,9,11,6,8,11,0,13,8,6,0,13,9,1,7,
    0,2,13,3,7,7,12,7,14,0,1,4,8,13,2,15,10,8,
    0,8,4,5,10,6,8,13,1,0,1,14,10,3,1,5,10,4,
    0,11,1,0,13,8,3,14,2,0,7,2,7,8,13,10,7,13,
    0,3,9,1,1,8,0,3,10,0,10,12,2,4,5,6,14,12,
    0,15,5,11,15,15,7,10,0,0,5,11,4,9,6,11,9,15,
    0,10,7,13,2,5,13,12,9,0,6,0,8,7,0,1,3,5,
    0,12,8,1,1,9,0,15,6,0,11,6,15,4,15,14,5,12,
    0,6,2,12,8,3,3,9,3,0,12,1,5,2,15,13,5,6,
    0,9,12,2,3,12,4,6,10,0,3,7,14,5,0,1,0,9,
    0,12,13,7,5,15,4,7,14,0,11,10,14,12,10,14,12,11,
    0,7,6,12,14,5,10,8,13,0,14,12,3,11,9,7,15,0,
    0,5,12,11,11,13,14,5,5,0,9,6,12,1,3,0,2,0,
    0,3,9,5,5,6,1,0,15,0,10,0,11,12,10,6,14,3,
    0,9,0,4,12,0,7,10,0,0,5,9,11,10,9,11,15,14,
    0,10,3,10,2,3,13,5,3,0,0,5,5,7,4,0,2,5,
    0,0,5,2,4,14,5,6,12,0,3,11,15,14,8,3,8,9,
    0,5,2,14,8,0,11,9,5,0,6,14,2,2,5,8,3,6,
    0,7,10,8,15,9,11,1,7,0,8,5,1,9,6,8,6,2,
    0,0,15,7,4,14,6,2,8,0,13,9,12,14,3,13,12,11};
  static char ibin[16]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};
  great ie;
  unsigned long itmp,ietmp1,ietmp2;
  char iec[9];
  int jj,irow,icol,iss,j,l,m;
  unsigned long *p;

  p = bit;
  ie.r=ie.c=ie.l=0;
  
  #ifdef PROFILING
  cyfun_iters_j1 = 0;
  #endif    
  _Pragma("loopbound min 16 max 16")
  for ( j = 16, l = 32, m = 48; j >= 1; j--,l--,m-- ) {
    #ifdef PROFILING
    cyfun_iters_j1++;
    #endif    
    ie.r = (ie.r << 1) | (p[iet[j]] & ir ? 1 : 0);
    ie.c = (ie.c << 1) | (p[iet[l]] & ir ? 1 : 0);
    ie.l = (ie.l << 1) | (p[iet[m]] & ir ? 1 : 0);
  }
  #ifdef PROFILING
  if ( cyfun_iters_j1 < cyfun_min_j1 )
    cyfun_min_j1 = cyfun_iters_j1;
  if ( cyfun_iters_j1 > cyfun_max_j1 )
    cyfun_max_j1 = cyfun_iters_j1;
  #endif
  
  ie.r ^= k.r;
  ie.c ^= k.c;
  ie.l ^= k.l;
  ietmp1=((unsigned long) ie.c << 16)+(unsigned long) ie.r;
  ietmp2=((unsigned long) ie.l << 8)+((unsigned long) ie.c >> 8);
  
  #ifdef PROFILING
  cyfun_iters_j2 = 0;
  #endif    
  _Pragma("loopbound min 4 max 4")
  for ( j = 1, m = 5; j <= 4; j++, m++ ) {
    #ifdef PROFILING
    cyfun_iters_j2++;
    #endif    
    iec[j]=ietmp1 & 0x3fL;
    iec[m]=ietmp2 & 0x3fL;
    ietmp1 >>= 6;
    ietmp2 >>= 6;
  }
  #ifdef PROFILING
  if ( cyfun_iters_j2 < cyfun_min_j2 )
    cyfun_min_j2 = cyfun_iters_j2;
  if ( cyfun_iters_j2 > cyfun_max_j2 )
    cyfun_max_j2 = cyfun_iters_j2;
  #endif
  
  itmp = 0L;
  
  #ifdef PROFILING
  cyfun_iters_jj = 0;
  #endif    
  _Pragma("loopbound min 8 max 8")
  for ( jj = 8; jj >= 1; jj-- ) {
    #ifdef PROFILING
    cyfun_iters_jj++;
    #endif    
    j = iec[jj];
    irow = ((j & 0x1) << 1)+((j & 0x20) >> 5);
    icol = ((j & 0x2) << 2)+(j & 0x4)
      +((j & 0x8) >> 2)+((j & 0x10) >> 4);
    iss = is[icol][irow][jj];
    itmp = (itmp << 4) | ibin[iss];
  }
  #ifdef PROFILING
  if ( cyfun_iters_jj < cyfun_min_jj )
    cyfun_min_jj = cyfun_iters_jj;
  if ( cyfun_iters_jj > cyfun_max_jj )
    cyfun_max_jj = cyfun_iters_jj;
  #endif
  
  *iout = 0L;
  p = bit;
  
  #ifdef PROFILING
  cyfun_iters_j3 = 0;
  #endif    
  _Pragma("loopbound min 32 max 32")
  for ( j = 32; j >= 1; j-- ) {
    #ifdef PROFILING
    cyfun_iters_j3++;
    #endif    
    *iout = (*iout <<= 1) | (p[ipp[j]] & itmp ? 1 : 0);
  }
  #ifdef PROFILING
  if ( cyfun_iters_j3 < cyfun_min_j3 )
    cyfun_min_j3 = cyfun_iters_j3;
  if ( cyfun_iters_j3 > cyfun_max_j3 )
    cyfun_max_j3 = cyfun_iters_j3;
  #endif
}

unsigned long getbit( immense source, int bitno, int nbits ) 
{
  if (bitno <= nbits)
    return bit[bitno] & source.r ? 1L : 0L;
  else
    return bit[bitno-nbits] & source.l ? 1L : 0L;
}


void ks( /*immense key, */int n, great * kn ) 
{
  int i,j,k,l;

  if ( n == 1 || n == 2 || n == 9 || n == 16 ) {
    icd.r = (icd.r | ((icd.r & 1L) << 28)) >> 1;
    icd.l = (icd.l | ((icd.l & 1L) << 28)) >> 1;
  }
  else {
    #ifdef PROFILING
    ks_iters_i = 0;
    #endif    
    _Pragma("loopbound min 2 max 2")
    for ( i = 1; i <= 2; i++ ) {
      #ifdef PROFILING
      ks_iters_i++;
      #endif
      icd.r = (icd.r | ((icd.r & 1L) << 28)) >> 1;
      icd.l = (icd.l | ((icd.l & 1L) << 28)) >> 1;
    }
    #ifdef PROFILING
    if ( ks_iters_i < ks_min_i )
      ks_min_i = ks_iters_i;
    if ( ks_iters_i > ks_max_i )
      ks_max_i = ks_iters_i;
    #endif
  }
  
  (*kn).r = (*kn).c = (*kn).l = 0;
  
  #ifdef PROFILING
  ks_iters_j = 0;
  #endif    
  _Pragma("loopbound min 16 max 16")
  for ( j = 16, k = 32, l = 48; j >= 1; j--,k--,l--) {
    #ifdef PROFILING
    ks_iters_j++;
    #endif    
    (*kn).r = ((*kn).r <<= 1) | (unsigned short)
      getbit(icd,ipc2[j],28);
    (*kn).c = ((*kn).c <<= 1) | (unsigned short)
      getbit(icd,ipc2[k],28);
    (*kn).l = ((*kn).l <<= 1) | (unsigned short)
      getbit(icd,ipc2[l],28);
  }
  #ifdef PROFILING
  if ( ks_iters_j < ks_min_j )
    ks_min_j = ks_iters_j;
  if ( ks_iters_j > ks_max_j )
    ks_max_j = ks_iters_j;
  #endif
}


#ifdef WORSTCASE
   int value = 1;
#else
   int value = 0;
#endif

int main( void )
{
  immense inp, key, out;
  int newkey, isw;

  inp.l = KNOWN_VALUE * 35;
  inp.r = KNOWN_VALUE * 26;
  key.l = KNOWN_VALUE * 2;
  key.r = KNOWN_VALUE * 16;

  newkey = value;
  isw = value;

  des( inp, key, &newkey, isw, &out );


  #ifdef PROFILING
  printf( "des::j1-loop: [%d, %d]\n", des_min_j1, des_max_j1 );
  printf( "des::j2-loop: [%d, %d]\n", des_min_j2, des_max_j2 );
  printf( "des::i1-loop: [%d, %d]\n", des_min_i1, des_max_i1 );
  printf( "des::j3-loop: [%d, %d]\n", des_min_j3, des_max_j3 );
  printf( "des::i2-loop: [%d, %d]\n", des_min_i2, des_max_i2 );
  printf( "des::j4-loop: [%d, %d]\n", des_min_j4, des_max_j4 );
  #endif

  #ifdef PROFILING
  printf( "ks::i-loop: [%d, %d]\n", ks_min_i, ks_max_i );
  printf( "ks::j-loop: [%d, %d]\n", ks_min_j, ks_max_j );
  #endif

  #ifdef PROFILING
  printf( "cyfun::j1-loop: [%d, %d]\n", cyfun_min_j1, cyfun_max_j1 );
  printf( "cyfun::j2-loop: [%d, %d]\n", cyfun_min_j2, cyfun_max_j2 );
  printf( "cyfun::jj-loop: [%d, %d]\n", cyfun_min_jj, cyfun_max_jj );
  printf( "cyfun::j3-loop: [%d, %d]\n", cyfun_min_j3, cyfun_max_j3 );
  #endif

  return 0;
}
