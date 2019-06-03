int main( int argc, char **argv, char **envp );

static char *argv[  ] = { "", 0 };
static char *envp[  ] = { 0 };

void _start( void )
{
  __asm( "lis 1," STACK ">>16" );
  __asm( "addis 1,1," STACK "&0xffff" );
  main( 1, argv, envp );
  while ( 1 );
}
