/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: huff_dec

  Author: David Bourgin (David.Bourgin@ufrima.imag.fr)

  Function: Example of Huffman decoding

  Source: ftp://turing.imag.fr/pub/compression/ (1994-09-26)

  Original name: dcodhuff.c

  Changes: I/O to char arrays instead of file i/o.
           Dynamic memory allocation replaced by array.

  License:

  The source code files (codrl1.c, dcodrl1.c, codrle2.c, dcodrle2.c, codrle3.c,
  dcodrle3.c, codrle4.c, dcodrle4.c, codhuff.c, dcodhuff.c) are copyrighted.
  They have been uploaded on ftp in turing.imag.fr (129.88.31.7):/pub/compression
  on 22/5/94 and have been modified on 22/9/94.
  (c) David Bourgin - 1994
  The source codes I provide have no buggs (!) but being that I make them
  available for free I have some notes to make. They can change at any time
  without notice. I assume no responsability or liability for any errors or
  inaccurracies, make no warranty of any kind (express, implied or statutory)
  with respect to this publication and expressly disclaim any and all warranties
  of merchantability, fitness for particular purposes. Of course, if you have
  some problems to use the information presented here, I will try to help you if
  I can.

  If you include the source codes in your application, here are the conditions:
  - You have to put my name in the header of your source file (not in the
  excutable program if you don't want) (this item is a must)
  - I would like to see your resulting application, if possible (this item is not
  a must, because some applications must remain secret)
  - Whenever you gain money with your application, I would like to receive a very
  little part in order to be encouraged to update my source codes and to develop
  new schemes (this item is not a must)

*/


/*
  Declaration of types
*/

typedef struct s_tree {
  unsigned int byte; /* A byte has to be coded as an unsigned integer to
                        allow a node to have a value over 255 */
  struct s_tree *left_ptr;
  struct s_tree *right_ptr;
} huff_dec_t_tree;

typedef struct {
  unsigned char bits[ 32 ];
  unsigned int bits_nb;
  unsigned char presence;
} t_bin_val;


/*
  Forward declaration of functions
*/

void huff_dec_init( void );
int huff_dec_return( void );
int huff_dec_end_of_data();
int huff_dec_read_byte();
void huff_dec_write_byte( char ch );
unsigned char huff_dec_read_code_1_bit();
unsigned int huff_dec_read_code_n_bits( unsigned int n );
void huff_dec_read_header( t_bin_val codes_table[ 257 ] );
huff_dec_t_tree *huff_dec_tree_encoding( t_bin_val codes_table[ 257 ],
    huff_dec_t_tree heap[ 514 ] );
void huff_dec_main( void );
int main( void );


/*
  Declaration of global variables
*/

static int huff_dec_input_pos;
static int huff_dec_output_pos;
static char huff_dec_output[ 1024 ];
unsigned char huff_dec_byte_nb_to_read = 0;
unsigned int huff_dec_val_to_read = 0;


/*
  Initialization- and return-value-related functions
*/

#define huff_dec_plaintext_len 600
static const char *huff_dec_plaintext =
  "You are doubtless asking \"How can I reduce the data size without losing "
  "some informations?\". It's easy to answer to this question. I'll only take "
  "an example. I'm sure you have heard about the morse. This system established "
  "in the 19th century use a scheme very close to the huffman one. In the morse "
  "you encode the letters to transmit with two kinds of signs. If you encode "
  "these two sign possibilities in one bit, the symbol 'e' is transmitted in a "
  "single bit and the symbols 'y' and 'z' need four bits. Look at the symbols "
  "in the text you are reading, you'll fast understand the compression ratio...";

#define huff_dec_encoded_len 419
static unsigned char huff_dec_encoded[ huff_dec_encoded_len ] = {
  128, 0, 0, 0, 80, 133, 32, 32, 128, 100, 4, 32, 63, 239, 255, 240,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  4, 7, 167, 21, 129, 232, 69, 120, 132, 217, 20, 162, 19, 164, 39, 133,
  252, 138, 105, 20, 194, 19, 129, 240, 172, 138, 248, 150, 11, 11, 240, 201,
  68, 64, 114, 53, 17, 42, 37, 195, 128, 212, 116, 194, 41, 98, 52, 51,
  12, 132, 112, 244, 3, 36, 33, 52, 39, 135, 164, 33, 62, 156, 87, 14,
  110, 22, 87, 50, 85, 198, 99, 142, 140, 194, 81, 78, 158, 84, 129, 254,
  129, 248, 110, 179, 159, 192, 145, 133, 184, 184, 28, 210, 96, 146, 73, 10,
  226, 21, 83, 152, 74, 13, 111, 132, 199, 202, 219, 241, 74, 193, 167, 105,
  222, 31, 147, 6, 55, 31, 129, 40, 232, 52, 153, 160, 148, 18, 36, 197,
  45, 216, 202, 86, 30, 31, 177, 90, 133, 138, 248, 23, 81, 195, 160, 100,
  215, 93, 50, 185, 225, 251, 23, 6, 230, 225, 229, 112, 71, 80, 96, 141,
  205, 176, 230, 85, 196, 9, 24, 93, 90, 121, 225, 76, 68, 152, 63, 25,
  107, 140, 101, 204, 214, 77, 26, 194, 96, 18, 48, 77, 210, 137, 1, 253,
  4, 230, 248, 56, 240, 224, 111, 163, 95, 10, 12, 223, 7, 234, 167, 129,
  40, 36, 96, 135, 125, 245, 250, 2, 198, 120, 127, 0, 145, 133, 213, 167,
  135, 149, 195, 67, 235, 108, 9, 24, 87, 17, 102, 152, 37, 4, 222, 131,
  188, 144, 73, 36, 128, 73, 20, 81, 152, 177, 133, 248, 28, 165, 131, 120,
  127, 240, 242, 184, 104, 125, 109, 129, 35, 30, 4, 145, 65, 202, 88, 9,
  138, 103, 44, 205, 100, 167, 24, 152, 11, 24, 51, 37, 66, 9, 24, 31,
  174, 202, 212, 49, 152, 18, 96, 155, 208, 119, 146, 45, 97, 48, 56, 28,
  194, 90, 224, 204, 144, 232, 176, 36, 96, 126, 187, 43, 83, 12, 121, 129,
  209, 96, 197, 35, 2, 54, 176, 249, 92, 208, 204, 145, 188, 41, 170, 180,
  71, 16, 36, 96, 126, 187, 43, 83, 19, 0, 145, 129, 100, 209, 15, 43,
  135, 55, 6, 238, 180, 194, 90, 17, 229, 115, 21, 168, 251, 140, 131, 162,
  217, 166, 93, 22, 4, 140, 31, 91, 166, 55, 25, 202, 192, 111, 20, 171,
  207, 39, 192,
};


void huff_dec_init( void )
{
  huff_dec_input_pos = 0;
  huff_dec_output_pos = 0;
}


int huff_dec_return( void )
{
  int i;
  _Pragma( "loopbound min 600 max 600" )
  for ( i = 0; i < huff_dec_plaintext_len; i++ ) {
    if ( huff_dec_plaintext[ i ] != huff_dec_output[ i ] ) return i + 1;
  }
  return 0;
}


/*
  Input / output functions
*/

int huff_dec_end_of_data()
{
  return huff_dec_input_pos >= huff_dec_encoded_len;
}


int huff_dec_read_byte()
{
  return huff_dec_encoded[ huff_dec_input_pos++ ];
}


void huff_dec_write_byte( char ch )
{
  huff_dec_output[ huff_dec_output_pos++ ] = ch;
}


unsigned char huff_dec_read_code_1_bit()
/* Returned parameters: Returns an unsigned integer with the 0-bit (on the
                        right of the integer) valid
   Action: Reads the next bit in the stream of data to compress
   Errors: An input/output error could disturb the running of the program
   The source must have enough bits to read
*/
{
  if ( huff_dec_byte_nb_to_read ) {
    huff_dec_byte_nb_to_read--;
    return ( ( huff_dec_val_to_read >> huff_dec_byte_nb_to_read ) & 1 );
  } else {
    huff_dec_val_to_read = huff_dec_read_byte();
    huff_dec_byte_nb_to_read = 7;
    return ( ( huff_dec_val_to_read >> 7 ) & 1 );
  }
}


unsigned int huff_dec_read_code_n_bits( unsigned int n )
/* Returned parameters: Returns an unsigned integer with the n-bits (on the
                        right of the integer) valid
   Action: Reads the next n bits in the stream of data to compress
   Errors: An input/output error could disturb the running of the program
   The source must have enough bits to read
*/
{
  unsigned int result = 0;
  unsigned i = n;

  _Pragma ( "loopbound min 1 max 1" )
  while ( i ) {
    _Pragma ( "loopbound min 0 max 2" )
    while ( ( huff_dec_byte_nb_to_read < 9 ) && ( !huff_dec_end_of_data() ) ) {
      huff_dec_val_to_read = ( huff_dec_val_to_read << 8 ) + huff_dec_read_byte();
      huff_dec_byte_nb_to_read += 8;
    }
    if ( i >= huff_dec_byte_nb_to_read ) {
      result = ( result << huff_dec_byte_nb_to_read ) + huff_dec_val_to_read;
      i -= huff_dec_byte_nb_to_read;
      huff_dec_byte_nb_to_read = 0;
    } else {
      result = ( result << i ) + ( ( huff_dec_val_to_read >>
                                     ( huff_dec_byte_nb_to_read - i ) ) & ( ( 1 << i ) - 1 ) );
      huff_dec_byte_nb_to_read -= i;
      i = 0;
    }
  }
  return ( result );
}


void huff_dec_read_header( t_bin_val codes_table[ 257 ] )
/* Returned parameters: The contain of 'codes_table' is modified
   Action: Rebuilds the binary encoding array by using the header
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned int i, j;
  unsigned num_byte;

  _Pragma ( "loopbound min 257 max 257" )
  for ( i = 0; i < 257; i++ ) {
    codes_table[ i ].bits_nb = 0;
    _Pragma ( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ )
      codes_table[ i ].bits[ j ] = 0;
  }

  /* == Decoding of the first part of the header === */
  if ( huff_dec_read_code_1_bit() )
    /* First bit=0 => Present bytes coded on n*8 bits
                =1 => Present bytes coded on 256 bits */
    _Pragma ( "loopbound min 256 max 256" )
    for ( i = 0; i <= 255; i++ )
      codes_table[ i ].presence = huff_dec_read_code_1_bit();
  else {
    i = huff_dec_read_code_n_bits( 5 ) + 1;
    _Pragma ( "loopbound min 1 max 32" )
    while ( i ) {
      codes_table[ huff_dec_read_code_n_bits( 8 ) ].presence = 1;
      i--;
    }
  }
  codes_table[ 256 ].presence = 1;
  /* Presence of a fictive 256-byte is enforced! */

  /* == Decoding the second part of the header == */
  _Pragma ( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ )
    if ( codes_table[ i ].presence ) {
      if ( huff_dec_read_code_1_bit() )
        /* First bit=0 => 5 bits of binary length-1 followed by a binary word
                    =1 => 8 bits of binary length-1 followed by a binary word */
        j = huff_dec_read_code_n_bits( 8 ) + 1;
      else j = huff_dec_read_code_n_bits( 5 ) + 1;
      codes_table[ i ].bits_nb = j;
      /* Reading of a binary word */
      num_byte = ( j - 1 ) >> 3;
      if ( j & 7 ) {
        /* Reads the bits that takes less than one byte */
        codes_table[ i ].bits[ num_byte ] =
          ( unsigned char )huff_dec_read_code_n_bits( j & 7 );
        j -= j & 7;
        num_byte--;
      }

      _Pragma ( "loopbound min 0 max 1" )
      while ( j >= 8 ) {
        /* Reads the bits that takes one byte, at least */
        codes_table[ i ].bits[ num_byte ] =
          ( unsigned char )huff_dec_read_code_n_bits( 8 );
        j -= 8;
        num_byte--;
      }
    }
}


huff_dec_t_tree *huff_dec_tree_encoding( t_bin_val codes_table[ 257 ],
    huff_dec_t_tree heap[ 514 ] )
/* Returned parameters: A binary tree is returned
   Action: Returns the decoding binary tree built from 'codes_table'
   Errors: None
*/
{
  unsigned int i;
  unsigned int j;
  unsigned int heap_top = 0;
  huff_dec_t_tree *ptr_tree;
  huff_dec_t_tree *current_node;

  ptr_tree = &heap[ heap_top++ ];
  ptr_tree->byte = 257;
  ptr_tree->left_ptr = 0;
  ptr_tree->right_ptr = 0;
  _Pragma ( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ ) {
    _Pragma ( "loopbound min 0 max 9" )
    for ( current_node = ptr_tree, j = codes_table[ i ].bits_nb; j; j-- ) {
      if ( codes_table[ i ].bits[ ( j - 1 ) >> 3 ] & ( 1 << ( (
             j - 1 ) & 7 ) ) )
        if ( current_node->left_ptr == 0 ) {
          current_node->left_ptr = &heap[ heap_top++ ];
          current_node = current_node->left_ptr;
          current_node->left_ptr = 0;
          current_node->right_ptr = 0;
        } else current_node = current_node->left_ptr;
      else
        if ( current_node->right_ptr == 0 ) {
          current_node->right_ptr = &heap[ heap_top++ ];
          current_node = current_node->right_ptr;
          current_node->left_ptr = 0;
          current_node->right_ptr = 0;
        } else current_node = current_node->right_ptr;
      if ( j == 1 )
        current_node->byte = i;
      else current_node->byte = 257;
    }
  };
  return ( ptr_tree );
}


void _Pragma( "entrypoint" ) huff_dec_main( void )
/* Returned parameters: None
   Action: Decompresses with Huffman method all bytes read by the function
           'read_code_1_bit' and 'read_code_n_bits'
   Errors: An input/output error could disturb the running of the program
*/
{
  t_bin_val encoding_table[ 257 ];
  huff_dec_t_tree heap[ 514 ]; /* space for dynamically allocated nodes */
  huff_dec_t_tree *ptr_tree;
  huff_dec_t_tree *current_node;

  if ( !huff_dec_end_of_data() ) { /* Are there data to compress? */
    huff_dec_read_header( encoding_table );
    ptr_tree = huff_dec_tree_encoding( encoding_table, heap );
    _Pragma ( "loopbound min 601 max 601" )
    do {
      current_node = ptr_tree;
      _Pragma ( "loopbound min 3 max 9" )
      while ( current_node->byte == 257 )
        if ( huff_dec_read_code_1_bit() )
          /* Bit=1 => Got to left in the node of the tree
                =0 => Got to right in the node of the tree */
          current_node = current_node->left_ptr;
        else current_node = current_node->right_ptr;
      if ( current_node->byte <= 255 )
        huff_dec_write_byte( current_node->byte );
    } while ( current_node->byte != 256 );
  }
}


int main( void )
{
  huff_dec_init();
  huff_dec_main();
  return ( huff_dec_return() );
}
