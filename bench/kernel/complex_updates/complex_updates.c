/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: complex_updates

  Author: Juan Martinez Velarde

  Function: complex_updates is a program for filter benchmarking.
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

void complex_updates_pin_down(TYPE *pa, TYPE *pb, TYPE *pc, TYPE *pd) {
  STORAGE_CLASS int i;

  _Pragma("loopbound min 16 max 16")
  for (i = 0; i < N; i++) {
    *pa++ = 2;
    *pa++ = 1;
    *pb++ = 2;
    *pb++ = 5;
    *pc++ = 3;
    *pc++ = 4;
    *pd++ = 0;
    *pd++ = 0;
  }
}

TYPE complex_updates_A[2 * N], complex_updates_B[2 * N],
    complex_updates_C[2 * N], complex_updates_D[2 * N];

void complex_updates_init(void) {
  int i;
  volatile float x = 0;

  complex_updates_pin_down(&complex_updates_A[0], &complex_updates_B[0],
                           &complex_updates_C[0], &complex_updates_D[0]);

  /* avoid constant propagation */
  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0 ; i < N ; i++){
    complex_updates_A[i] += x;
    complex_updates_B[i] += x;
    complex_updates_C[i] += x;
    complex_updates_D[i] += x;
  }
}

void _Pragma("entrypoint") complex_updates_main(void) {
  STORAGE_CLASS TYPE *p_a = &complex_updates_A[0], *p_b = &complex_updates_B[0];
  STORAGE_CLASS TYPE *p_c = &complex_updates_C[0], *p_d = &complex_updates_D[0];
  int i;

  _Pragma( "loopbound min 16 max 16" )
  for ( i = 0 ; i < N ; i++, p_a++ ) {
    *p_d    = *p_c++ + *p_a++ * *p_b++ ;
    *p_d++ -=          *p_a   * *p_b-- ;

    *p_d    = *p_c++ + *p_a-- * *p_b++ ;
    *p_d++ +=          *p_a++ * *p_b++ ;
  }

}

int complex_updates_return(void) {
  float check_sum = 0;
  int i;

  for (i = 0; i < N; i++) {
    check_sum += complex_updates_D[i];
  }

  return (check_sum != 144.0f);
}

int main(void) {
  complex_updates_init();

  complex_updates_main();

  return complex_updates_return();
}
