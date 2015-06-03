/* File: dcodrle1.c
  Author: David Bourgin
  Creation date: 1/2/94
  Last update: 22/9/94
  Purpose: Example of RLE type 1 decoding with a file source to decompress
*/

/* Global variables */
extern unsigned char input[612];
static unsigned char *source_memory_base;  /* Base of the source memory */
static unsigned char *source_memory_end;   /* Last address to read.
           source_memory_end=source_memory_base+source_zone_length-1 */
static unsigned char *source_ptr;  /* Used in the xxxcoding procedure */

/* Pseudo procedures */
static int end_of_data( void )
{
  return source_ptr > source_memory_end;
}
static unsigned char read_byte( void )
{
  return *(source_ptr++);
}
static void write_byte( unsigned char byte )
{
}
static void write_array( unsigned char *array, unsigned int byte_nb_to_write)
{
  unsigned int i;
  _Pragma( "loopbound min 2 max 3" )
  for ( i = 0; i < byte_nb_to_write; i++ ) {
    write_byte( array[i] );
  }
}
static void write_block( int byte, unsigned int time_nb )
{
  unsigned int i;
  unsigned char array_to_write[129];

  _Pragma( "loopbound min 2 max 3" )
  for ( i = 0; i < time_nb; i++ ) {
    array_to_write[i] = byte;
  }
  write_array(array_to_write, (time_nb));
}

void rle1decoding( unsigned char *input, unsigned int length )
/* Returned parameters: None
   Action: Decompresses with RLE type 1 method all bytes read by the function read_byte
   Erreurs: An input/output error could disturb the running of the program
*/
{
  source_memory_base = (unsigned char*)input;
  source_memory_end = (unsigned char*)(input + length);
  source_ptr = source_memory_base;

  unsigned char header;
  register unsigned char i;

  _Pragma( "loopbound min 24 max 24" )
  while (!end_of_data()) {
    header = read_byte();
    switch (header & 128) {
      case 0:
        if (!end_of_data())
          _Pragma( "loopbound min 9 max 128" )
          for (i = 0;i <= header;i++)
            write_byte(read_byte());
        /* else INVALID FILE */
        break;
      case 128: if (!end_of_data())
            write_block(read_byte(), (header & 127) + 2);
        /* else INVALID FILE */
    }
  }
}

int main( void )
{
  rle1decoding( input, 612 );
  return 0;
}
