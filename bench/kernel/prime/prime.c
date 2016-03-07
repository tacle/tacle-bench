/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: prime

  Author: unknown

  Function: prime calculates whether numbers are prime.

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/prime/prime.c

  Changes: no major functional changes

  License: may be used, modified, and re-distributed freely

*/


/*
  Forward declaration of functions
*/

unsigned char prime_divides ( unsigned int n, unsigned int m );
unsigned char prime_even ( unsigned int n );
unsigned char prime_prime ( unsigned int n );
void prime_swap ( unsigned int *a, unsigned int *b );
void prime_init ();
int prime_return ();
void prime_main ();
int main( void );


/*
  Declaration of global variables
*/

int prime_result;


/*
  Initialization- and return-value-related functions
*/

void prime_init () {}

int prime_return ()
{
  return prime_result;
}


/*
  Algorithm core functions
*/

unsigned char prime_divides ( unsigned int n, unsigned int m )
{
  return ( m % n == 0 );
}


unsigned char prime_even ( unsigned int n )
{
  return ( prime_divides ( 2, n ) );
}


unsigned char prime_prime ( unsigned int n )
{
  unsigned int i;
  if ( prime_even ( n ) )
    return ( n == 2 );
  _Pragma( "loopbound min 73 max 357" )
  for ( i = 3; i * i <= n; i += 2 ) {
    if ( prime_divides ( i, n ) ) /* ai: loop here min 0 max 357 end; */
      return 0;
  }
  return ( n > 1 );
}


void prime_swap ( unsigned int *a, unsigned int *b )
{
  unsigned int tmp = *a;
  *a = *b;
  *b = tmp;
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) prime_main()
{
  unsigned int x =  21649;
  unsigned int y = 513239;
  prime_swap ( &x, &y );

  prime_result = !( prime_prime( x ) && prime_prime( y ) );
}


int main( void )
{
  prime_init();
  prime_main();

  return ( prime_return() );
}


