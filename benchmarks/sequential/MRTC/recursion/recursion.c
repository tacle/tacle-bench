/*
 * DESCRIPTION: A simple example of recursive code
 *
 * COMMENTS: Both self-recursion and mutual recursion are used
 */

int fib( int i )
{
  if( i==0 )
    return 1;
  if( i==1 )
    return 1;

  return fib( i-1 ) + fib( i-2 );
}


int main( void )
{
  // WCC frequently has problems with locating the _Pragmas,
  // therfore we had to introduce this blocker variable to be
  // able to place the Pragmas.
  volatile int blocker;
  int In = fib( 10 );
  _Pragma( "marker recursivecall" )
  _Pragma( "flowrestriction 1*fib <= 177*recursivecall" )
  return blocker;
}
