/*----------------------------------------------------------------------
 *  WCET Benchmark created by Jakob Engblom, Uppsala university,
 *  February 2000.
 *
 *  The purpose of this benchmark is to force the compiler to emit an
 *  unstructured loop, which is usually problematic for WCET tools to
 *  handle.
 *
 *  The execution time should be constant.
 *
 *  The original code is "Duff's Device", see the Jargon File, e.g. at
 *  http://www.tf.hut.fi/cgi-bin/jargon.  Created in the early 1980s
 *  as a way to express loop unrolling in C.
 *
 *----------------------------------------------------------------------*/

#define ARRAYSIZE  100
#define INVOCATION_COUNT 43	/* exec time depends on this one! */

void duffcopy( char *to, char *from, int count)
{
  int n = ( count + 7 ) / 8;

  _Pragma("marker outside")  

    switch( count % 8 ) {

      case 0: do {    
                *to++ = *from++;
                case 7:         
                *to++ = *from++;
                case 6:         
                *to++ = *from++;
                case 5:         
                *to++ = *from++;
                case 4:         
                *to++ = *from++;
                case 3:         
                *to++ = *from++;
                case 2:         
                *to++ = *from++;
                case 1:         
                _Pragma("marker inside")
                  *to++ = *from++;


              } while( --n > 0 );
    }
  _Pragma("flowrestriction 1*inside <= 6*outside")
}


void initialize( char *arr, int length)
{
  int i;

  _Pragma("loopbound min 100 max 100")
  for( i = 0; i < length; i++ ) {
      arr[i] = length-i;
  }
}


char source[ARRAYSIZE];
char target[ARRAYSIZE];

int main( void )
{
  initialize( source, ARRAYSIZE );
  duffcopy( source, target, INVOCATION_COUNT );

  return 0;
}

