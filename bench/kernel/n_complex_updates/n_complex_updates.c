/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: n_complex_updates

  Author: Juan Martinez Velarde

  Function: n_complex_updates is a program for filter benchmarking.
    This program performs n complex updates of the form
              D(i) = C(i) + A(i)*B(i),
    where A(i), B(i), C(i) and D(i) are complex numbers,
    and i = 1,...,N
             A(i) = Ar(i) + j Ai(i)
             B(i) = Br(i) + j Bi(i)
             C(i) = Cr(i) + j Ci(i)
             D(i) = C(i) + A(i)*B(i) =   Dr(i)  +  j Di(i)
                         =>  Dr(i) = Cr(i) + Ar(i)*Br(i) - Ai(i)*Bi(i)
                         =>  Di(i) = Ci(i) + Ar(i)*Bi(i) + Ai(i)*Br(i)

  Source: DSP-Stone
    http://www.ice.rwth-aachen.de/research/tools-projects/entry/detail/dspstone/

  Original name: n_complex_updates_float

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

*/

#define STORAGE_CLASS register
#define TYPE          float
#define N             16

void n_complex_updates_pin_down( TYPE *pa, TYPE *pb, TYPE *pc, TYPE *pd )
{
  STORAGE_CLASS int i ;

  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0 ; i < N ; i++ ) {
    *pa++ = 2 ;
    *pa++ = 1 ;
    *pb++ = 2 ;
    *pb++ = 5 ;
    *pc++ = 3 ;
    *pc++ = 4 ;
    *pd++ = 0 ;
    *pd++ = 0 ;
  }
}

static TYPE A[2 * N], B[2 * N], C[2 * N], D[2 * N] ;

void n_complex_updates_init( void )
{
  return;
}

void _Pragma( "entrypoint" ) n_complex_updates_main( void )
{
  STORAGE_CLASS TYPE *p_a = &A[0], *p_b = &B[0] ;
  STORAGE_CLASS TYPE *p_c = &C[0], *p_d = &D[0] ;
  int i ;

  n_complex_updates_pin_down( &A[0], &B[0], &C[0], &D[0] ) ;

  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0 ; i < N ; i++, p_a++ ) {
    *p_d    = *p_c++ + *p_a++ * *p_b++ ;
    *p_d++ -=          *p_a   * *p_b-- ;

    *p_d    = *p_c++ + *p_a-- * *p_b++ ;
    *p_d++ +=          *p_a++ * *p_b++ ;
  }

  n_complex_updates_pin_down( &A[0], &B[0], &C[0], &D[0] ) ;
}



int main( void )
{
  n_complex_updates_init();
  n_complex_updates_main();

  return ( A[0] > B[0] );
}
