/*
  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: recursion

  Author: unknown

  Function: recursion is a recursion program.
    This program computes the Fibonacci number recursively.

  Source: MRTC
          http://www.mrtc.mdh.se/projects/wcet/wcet_bench/recursion/recursion.c

  Changes: no major functional changes

  License: May be used, modified, and re-distributed freely.

*/


/*
   Global Variables
*/
int recursion_result;
int recursion_input;

/*
  Forward declaration of functions
*/
int recursion_fib( int i );
void recursion_main( void );
void recursion_init( void );
int recursion_return( void );
int main ( void );


void recursion_init()
{
  int volatile temp_input = 10;
  recursion_input = temp_input;
}


int recursion_fib( int i )
{
  if ( i == 0 )
    return 1;
  if ( i == 1 )
    return 1;

  return recursion_fib( i - 1 ) + recursion_fib( i - 2 );
}

int recursion_return()
{
  return ( recursion_result  + ( -89 ) ) != 0;
}

void _Pragma( "entrypoint" ) recursion_main( void )
{
  _Pragma( "marker recursivecall" )
  _Pragma( "flowrestriction 1*fib <= 177*recursivecall" )
  recursion_result = recursion_fib( recursion_input );
}

int main( void )
{
  recursion_init();
  recursion_main();
  return ( recursion_return() );
}
