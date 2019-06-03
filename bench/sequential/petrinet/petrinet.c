/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: Petrinet

  Author: Friedhelm Stappert, C-LAB, Paderborn, Germany

  Function: Simulate an extended Petri Net
            Automatically generated code containing large amounts of
            if-statements (more than 250)

  Source: Mälardalen benchmark suite

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

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
int petrinet_main_iters_dummy_i = 0,
    petrinet_main_min_dummy_i = 100000,
    petrinet_main_max_dummy_i = 0;
#endif

/*
  Forward declaration of functions
*/
void petrinet_init( void );
int petrinet_return( void );
void petrinet_main( void );
int main( void );


volatile int  petrinet_P1_is_marked;
volatile long petrinet_P1_marking_member_0[ 3 ];
volatile int  petrinet_P2_is_marked;
volatile long petrinet_P2_marking_member_0[ 5 ];
volatile int  petrinet_P3_is_marked;
volatile long petrinet_P3_marking_member_0[ 6 ];

const long petrinet_CHECKSUM = 0;

void _Pragma ( "entrypoint" ) petrinet_main( void )
{
  int dummy_i;
  /*   dummy_i = 17; Takes too much time */
  dummy_i = 2;

  #ifdef PROFILING
  main_iters_dummy_i = 0;
  #endif
  _Pragma( "loopbound min 2 max 2" )
  while ( dummy_i > 0 ) {
    #ifdef PROFILING
    main_iters_dummy_i++;
    #endif

    dummy_i--;
    /* Permutation for Place P1 : 0, 1, 2 */
    /* Transition T1 */
    if ( ( petrinet_P1_is_marked >= 3 ) &&
         ( petrinet_P3_is_marked + 3 <= 6 ) &&
         ( petrinet_P1_marking_member_0[ 1 ]
           == petrinet_P1_marking_member_0[ 2 ] ) ) {

      long  x;
      long  y;
      long  z;

      x = petrinet_P1_marking_member_0[ 0 ];
      y = petrinet_P1_marking_member_0[ 1 ];

      /* Transition condition */
      if ( x < y ) {

        /* demarking of input places */
        petrinet_P1_is_marked -= 3;

        /* preaction */
        z = x - y;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = x;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = y;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = z;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P1 : 0, 2, 1 */
    /* Transition T1 */
    if ( ( petrinet_P1_is_marked >= 3 ) &&
         ( petrinet_P3_is_marked + 3 <= 6 ) &&
         ( petrinet_P1_marking_member_0[ 2 ]
           == petrinet_P1_marking_member_0[ 1 ] ) ) {

      long  x;
      long  y;
      long  z;

      x = petrinet_P1_marking_member_0[ 0 ];
      y = petrinet_P1_marking_member_0[ 2 ];

      /* Transition condition */
      if ( ( x < y ) ) {

        /* demarking of input places */
        petrinet_P1_is_marked -= 3;

        /* preaction */
        z = x - y;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = x;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = y;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = z;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P1 : 1, 0, 2 */
    /* Transition T1 */
    if ( ( petrinet_P1_is_marked >= 3 ) &&
         ( petrinet_P3_is_marked + 3 <= 6 ) &&
         ( petrinet_P1_marking_member_0[ 0 ]
           == petrinet_P1_marking_member_0[ 2 ] ) ) {

      long  x;
      long  y;
      long  z;

      x = petrinet_P1_marking_member_0[ 1 ];
      y = petrinet_P1_marking_member_0[ 0 ];

      /* Transition condition */
      if ( x < y ) {

        /* demarking of input places */
        petrinet_P1_is_marked -= 3;

        /* preaction */
        z = x - y;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = x;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = y;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = z;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P1 : 1, 2, 0 */
    /* Transition T1 */
    if ( ( petrinet_P1_is_marked >= 3 ) &&
         ( petrinet_P3_is_marked + 3 <= 6 ) &&
         ( petrinet_P1_marking_member_0[ 2 ]
           == petrinet_P1_marking_member_0[ 0 ] ) ) {

      long  x;
      long  y;
      long  z;

      x = petrinet_P1_marking_member_0[ 1 ];
      y = petrinet_P1_marking_member_0[ 2 ];

      /* Transition condition */
      if ( ( x < y ) ) {

        /* demarking of input places */
        petrinet_P1_is_marked -= 3;

        /* preaction */
        z = x - y;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = x;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = y;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = z;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P1 : 2, 0, 1 */
    /* Transition T1 */
    if ( ( petrinet_P1_is_marked >= 3 ) &&
         ( petrinet_P3_is_marked + 3 <= 6 ) &&
         ( petrinet_P1_marking_member_0[ 0 ]
           == petrinet_P1_marking_member_0[ 1 ] ) ) {
      long  x;
      long  y;
      long  z;

      x = petrinet_P1_marking_member_0[ 2 ];
      y = petrinet_P1_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( x < y ) ) {

        /* demarking of input places */
        petrinet_P1_is_marked -= 3;

        /* preaction */
        z = x - y;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = x;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = y;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = z;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P1 : 2, 1, 0 */
    /* Transition T1 */
    if ( ( petrinet_P1_is_marked >= 3 ) &&
         ( petrinet_P3_is_marked + 3 <= 6 ) &&
         ( petrinet_P1_marking_member_0[ 1 ]
           == petrinet_P1_marking_member_0[ 0 ] ) ) {
      long  x;
      long  y;
      long  z;

      x = petrinet_P1_marking_member_0[ 2 ];
      y = petrinet_P1_marking_member_0[ 1 ];

      /* Transition condition */
      if ( ( x < y ) ) {

        /* demarking of input places */
        petrinet_P1_is_marked -= 3;

        /* preaction */
        z = x - y;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = x;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = y;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = z;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place P2 : 0, 1, 2, 3 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( ( petrinet_P3_is_marked + 3 ) <= 6 ) ) &&
         ( ( ( petrinet_P2_marking_member_0[ 1 ]
               == petrinet_P2_marking_member_0[ 2 ] ) ) &&
           ( ( petrinet_P2_marking_member_0[ 1 ]
               == petrinet_P2_marking_member_0[ 3 ] ) ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 0 ];
      b = petrinet_P2_marking_member_0[ 1 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 0, 1, 3, 2 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( ( petrinet_P3_is_marked + 3 ) <= 6 ) ) &&
         ( ( petrinet_P2_marking_member_0[ 1 ]
             == petrinet_P2_marking_member_0[ 3 ] ) &&
           ( petrinet_P2_marking_member_0[ 1 ]
             == petrinet_P2_marking_member_0[ 2 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 0 ];
      b = petrinet_P2_marking_member_0[ 1 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 0, 2, 1, 3 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 1 ] ) &&
           ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 3 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 0 ];
      b = petrinet_P2_marking_member_0[ 2 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 0, 2, 3, 1 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 3 ] ) &&
           ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 1 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 0 ];
      b = petrinet_P2_marking_member_0[ 2 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 0, 3, 1, 2 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 1 ] ) &&
           ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 2 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 0 ];
      b = petrinet_P2_marking_member_0[ 3 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 0, 3, 2, 1 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 2 ] ) &&
           ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 1 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 0 ];
      b = petrinet_P2_marking_member_0[ 3 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 1, 0, 2, 3 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 2 ] ) &&
           ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 3 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 1 ];
      b = petrinet_P2_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 1, 0, 3, 2 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 3 ] ) &&
           ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 2 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 1 ];
      b = petrinet_P2_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 1, 2, 0, 3 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 0 ] ) &&
           ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 3 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 1 ];
      b = petrinet_P2_marking_member_0[ 2 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 1, 2, 3, 0 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 3 ] ) &&
           ( petrinet_P2_marking_member_0[ 2 ]
             == petrinet_P2_marking_member_0[ 0 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 1 ];
      b = petrinet_P2_marking_member_0[ 2 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 1, 3, 0, 2 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 0 ] ) &&
           ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 2 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 1 ];
      b = petrinet_P2_marking_member_0[ 3 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }


    /* Permutation for Place petrinet_P2 : 1, 3, 2, 0 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 2 ] ) &&
           ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 0 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 1 ];
      b = petrinet_P2_marking_member_0[ 3 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }


    /* Permutation for Place petrinet_P2 : 2, 0, 1, 3 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 1 ] ) &&
           ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 3 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 2 ];
      b = petrinet_P2_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 2, 0, 3, 1 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 3 ] ) &&
           ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 1 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 2 ];
      b = petrinet_P2_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 2, 1, 0, 3 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 1 ]
             == petrinet_P2_marking_member_0[ 0 ] ) &&
           ( petrinet_P2_marking_member_0[ 1 ]
             == petrinet_P2_marking_member_0[ 3 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 2 ];
      b = petrinet_P2_marking_member_0[ 1 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 2, 1, 3, 0 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 1 ]
             == petrinet_P2_marking_member_0[ 3 ] ) &&
           ( petrinet_P2_marking_member_0[ 1 ]
             == petrinet_P2_marking_member_0[ 0 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 2 ];
      b = petrinet_P2_marking_member_0[ 1 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 2, 3, 0, 1 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 0 ] ) &&
           ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 1 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 2 ];
      b = petrinet_P2_marking_member_0[ 3 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 2, 3, 1, 0 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 1 ] ) &&
           ( petrinet_P2_marking_member_0[ 3 ]
             == petrinet_P2_marking_member_0[ 0 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 2 ];
      b = petrinet_P2_marking_member_0[ 3 ];

      /* Transition condition */
      if ( ( b > a ) ) {
        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }

    /* Permutation for Place petrinet_P2 : 3, 0, 1, 2 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 1 ] ) &&
           ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 2 ] ) ) ) {
      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 3 ];
      b = petrinet_P2_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }


    /* Permutation for Place petrinet_P2 : 3, 0, 2, 1 */
    /* Transition T2 */
    if ( ( petrinet_P2_is_marked >= 4 ) &&
         ( ( petrinet_P3_is_marked + 3 ) <= 6 ) &&
         ( ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 2 ] ) &&
           ( petrinet_P2_marking_member_0[ 0 ]
             == petrinet_P2_marking_member_0[ 1 ] ) ) ) {

      long  a;
      long  b;
      long  c;

      a = petrinet_P2_marking_member_0[ 3 ];
      b = petrinet_P2_marking_member_0[ 0 ];

      /* Transition condition */
      if ( ( b > a ) ) {

        /* demarking of input places */
        petrinet_P2_is_marked -= 4;

        /* preaction */
        c = a + b;

        /* marking of output places */
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 0 ] = a;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 1 ] = b;
        petrinet_P3_marking_member_0[ petrinet_P3_is_marked + 2 ] = c;
        petrinet_P3_is_marked += 3;

      } /* end of if (Transition condition) */
    }
  }
  #ifdef PROFILING
  if ( main_iters_dummy_i < main_min_dummy_i )
    main_min_dummy_i = main_iters_dummy_i;
  if ( main_iters_dummy_i > main_max_dummy_i )
    main_max_dummy_i = main_iters_dummy_i;
  #endif

  #ifdef PROFILING
  printf( "main::dummy_i-loop: [%d, %d]\n",
          main_min_dummy_i, main_max_dummy_i );
  #endif

  //dummy_i = 77;
  // TODO: not a good return value
  //return dummy_i;
}


void petrinet_init( void )
{
  petrinet_P1_is_marked = 3;
  petrinet_P2_is_marked = 5;
  petrinet_P3_is_marked = 0;

  /*
    Maybe we should also initialise these arrays, as they may be read
    in the petrinet_main() function before being written.
  */
  /*
    volatile long petrinet_P1_marking_member_0[ 3 ];
    volatile long petrinet_P2_marking_member_0[ 5 ];
    volatile long petrinet_P3_marking_member_0[ 6 ];
  */
}


int petrinet_return( void )
{
  // TODO: use something from the Px_... arrays
  int checksum = 0;
  int i;

  _Pragma( "loopbound min 3 max 3" )
  for ( i = 0; i < 3; ++i )
    checksum += petrinet_P1_marking_member_0[i];

  _Pragma( "loopbound min 5 max 5" )
  for ( i = 0; i < 5; ++i )
    checksum += petrinet_P2_marking_member_0[i];

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0; i < 6; ++i )
    checksum += petrinet_P3_marking_member_0[i];

  return ( ( checksum == petrinet_CHECKSUM ) ? 0 : -1 );
}


int main( void )
{
  petrinet_main();

  return ( petrinet_return() );
}

