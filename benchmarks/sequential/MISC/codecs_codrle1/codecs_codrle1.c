/* File: codrle1.c
  Author: David Bourgin
  Creation date: 28/1/94
  Last update: 22/9/94
  Purpose: Example of RLE type 1 encoding with a file source to compress.
*/

/* Global variables */
extern char *input;
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
  _Pragma( "loopbound min 9 max 128" )
  for ( i = 0; i < byte_nb_to_write; i++ ) {
    write_byte( array[i] );
  }
}

void rle1encoding( char *input, unsigned int length )
/* Returned parameters: None
   Action: Compresses with RLE type 1 method all bytes read by the function 'read_byte'
   Errors: An input/output error could disturb the running of the program
*/
{
  source_memory_base = (unsigned char*)input;
  source_memory_end = (unsigned char*)(input + length);
  source_ptr = source_memory_base;

  register unsigned char byte1, byte2, frame_size;
  unsigned char array[129];

  if (!end_of_data()) {
    byte1 = read_byte();    /* Is there at least a byte to analyze? */
    frame_size = 1;
    if (!end_of_data()) {
      /* Are there at least two bytes to analyze? */
      byte2 = read_byte();
      frame_size = 2;
      _Pragma( "loopbound min 23 max 23" )
      do {
        if (byte1 == byte2) {
          /* Is there a repetition? */
          _Pragma( "loopbound min 1 max 1" )
          while ((!end_of_data()) && (byte1 == byte2) && (frame_size < 129)) {
            byte2 = read_byte();
            frame_size++;
          }
          if (byte1 == byte2) {
            /* Do we meet only a sequence of bytes? */
            write_byte(126 + frame_size);
            write_byte(byte1);
            if (!end_of_data()) {
              byte1 = read_byte();
              frame_size = 1;
            } else
              frame_size = 0;
          } else {  /* No, then don't handle the last byte */
            write_byte(125 + frame_size);
            write_byte(byte1);
            byte1 = byte2;
            frame_size = 1;
          }
          if (!end_of_data()) {
            byte2 = read_byte();
            frame_size = 2;
          }
        }
        else        /* Prepare the array of comparisons
                       where will be stored all the identical bytes */ { 
          *array = byte1;
          array[1] = byte2;
          _Pragma( "loopbound min 9 max 126" )
          while ((!end_of_data()) && 
              (array[frame_size - 2] != array[frame_size - 1]) && 
              (frame_size < 128)) {
            array[frame_size] = read_byte();
            frame_size++;
          }
          if (array[frame_size - 2] == array[frame_size - 1])
            /* Do we meet a sequence of all different bytes followed by identical byte? */
          { /* Yes, then don't count the two last bytes */
            write_byte(frame_size - 3);
            write_array(array, frame_size - 2);
            byte1 = array[frame_size - 2];
            byte2 = byte1;
            frame_size = 2;
          } else {
            write_byte(frame_size - 1);
            write_array(array, frame_size);
            if (end_of_data())
              frame_size = 0;
            else {
              byte1 = read_byte();
              if (end_of_data())
                frame_size = 1;
              else {
                byte2 = read_byte();
                frame_size = 2;
              }
            }
          }
        }
      } while ((!end_of_data()) || (frame_size >= 2));
    }
    if (frame_size == 1) {
      write_byte(0);
      write_byte(byte1);
    }
  }
}

int main( void )
{
  rle1encoding( input, 600 );
  return 0;
}
