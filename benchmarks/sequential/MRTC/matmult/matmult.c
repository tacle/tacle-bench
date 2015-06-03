/*----------------------------------------------------------------------*
 *
 * DESCRIPTION: Matrix multiplication of two 20x20 matrice
 *
 * COMMNETS: Multiple calls to the same function, nested function calls,
 * 	     triple-nested loops.
 *
 * To make this program compile under our assumed embedded environment,
 * we had to make several changes:
 * - Declare all functions in ANSI style, not K&R.
 *   this includes adding return types in all cases!
 * - Declare function prototypes
 * - Disable all output
 * - Disable all UNIX-style includes
 *
 * This is a program that was developed from mm.c to matmult.c by
 * Thomas Lundqvist at Chalmers.
 *
 *----------------------------------------------------------------------*/


/*
 * MATRIX MULTIPLICATION BENCHMARK PROGRAM:
 * This program multiplies 2 square matrices resulting in a 3rd
 * matrix. It tests a compiler's speed in handling multidimensional
 * arrays and simple arithmetic.
 */

#define UPPERLIMIT 20

typedef int matrix [UPPERLIMIT][UPPERLIMIT];

int Seed;
matrix ArrayA, ArrayB, ResultArray;

/* Forward function prototypes */
void Multiply( matrix A, matrix B, matrix Res );
void InitSeed( void );
void Test( matrix A, matrix B, matrix Res );
void Initialize( matrix Array );
int RandomInteger( void );

int main( void )
{
  InitSeed();
  
  Test(ArrayA, ArrayB, ResultArray);

  return 0;
}


void InitSeed( void )
/*
 * Initializes the seed used in the random number generator.
 */
{
  /* 
     changed Thomas Ls code to something simpler.
     Seed = KNOWN_VALUE - 1; 
  */

  Seed = 0;
}


void Test(matrix A, matrix B, matrix Res)
/*
 * Runs a multiplication test on an array.  Calculates and prints the
 * time it takes to multiply the matrices.
 */
{
  Initialize( A );

  Initialize( B );

  Multiply( A, B, Res );
}


void Initialize( matrix Array )
/*
 * Intializes the given array with random integers.
 */
{
  int OuterIndex, InnerIndex;


  _Pragma("loopbound min 20 max 20")
  for ( OuterIndex = 0; OuterIndex < UPPERLIMIT; OuterIndex++ ) {
    _Pragma("loopbound min 20 max 20")
    for ( InnerIndex = 0; InnerIndex < UPPERLIMIT; InnerIndex++ ) {
      Array[OuterIndex][InnerIndex] = RandomInteger();
    }
  }
}


int RandomInteger( void )
/*
 * Generates random integers between 0 and 8095
 */
{
  Seed = ((Seed * 133) + 81) % 8095;
  return (Seed);
}


void Multiply( matrix A, matrix B, matrix Res )
/*
 * Multiplies arrays A and B and stores the result in ResultArray.
 */
{
  register int Outer, Inner, Index;

  _Pragma("loopbound min 20 max 20")
  for ( Outer = 0; Outer < UPPERLIMIT; Outer++ ) {
    _Pragma("loopbound min 20 max 20")
    for ( Inner = 0; Inner < UPPERLIMIT; Inner++ ) {
      Res [Outer][Inner] = 0;

      _Pragma("loopbound min 20 max 20")
      for ( Index = 0; Index < UPPERLIMIT; Index++ ) {

	Res[Outer][Inner]  +=
	  A[Outer][Index] * B[Index][Inner];
      }
    }
  }
}

