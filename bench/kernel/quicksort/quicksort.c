#include "wcclibm.h"
#include "quicksort.h"
#include "wccstdlib.h"

#define NUM_STRINGS 681
#define NUM_VECTORS 1000

extern const char *input_string[NUM_STRINGS];
extern const unsigned int input_vector[NUM_VECTORS*3];

int main( void ) {
  struct my3DVertexStruct vectors[NUM_VECTORS];
  char strings[NUM_STRINGS][STRING_LENGTH];
  unsigned int i, j;
  unsigned int x,y,z;
  unsigned int read_counter = 0;

  // Init arrays
  _Pragma( "loopbound min 681 max 681" )
  for ( i = 0; i < NUM_STRINGS; i++ ) {
    _Pragma( "loopbound min 2 max 13" )
    for ( j = 0; j < STRING_LENGTH - 1; j++ ) {
      strings[i][j] = input_string[i][j];
      if ( input_string[i][j] == '\0' ) {
        break;
      }
    }
    // Terminate with '\0' anyways
    strings[i][STRING_LENGTH - 1] = '\0';
  }
  _Pragma( "loopbound min 1000 max 1000" )
  for ( i = 0; i < NUM_VECTORS; i++ ) {
    x = vectors[i].x = input_vector[read_counter++];
	  y = vectors[i].y = input_vector[read_counter++];
	  z = vectors[i].z = input_vector[read_counter++];
	  vectors[i].distance = sqrt( pow( x, 2 ) + pow( y, 2 ) + pow( z, 2 ) );
  }
  _Pragma( "marker recursivecall" )
  _Pragma( "flowrestriction 1*qsorts_strings <= 521*recursivecall" )
  qsorts_strings(*strings, NUM_STRINGS, sizeof(char[STRING_LENGTH]));

  _Pragma( "marker recursivecall2" )
  _Pragma( "flowrestriction 1*qsorts_vectors <= 650*recursivecall2" )
  qsorts_vectors( (char*)vectors, NUM_VECTORS, sizeof(struct my3DVertexStruct) );

  return 0;
}
