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
  unsigned char bits[32];
  unsigned int bits_nb;
  unsigned char presence;
} t_bin_val;


/*
  Forward declaration of functions
*/

void huff_dec_init( void );
int huff_dec_return( void );
static int huff_dec_end_of_data();
static int huff_dec_read_byte();
static void huff_dec_write_byte(char ch);
static unsigned char huff_dec_read_code_1_bit();
static unsigned int huff_dec_read_code_n_bits( unsigned int n );
static void huff_dec_read_header( t_bin_val codes_table[257] );
static huff_dec_t_tree *huff_dec_tree_encoding( t_bin_val codes_table[257],
                                      huff_dec_t_tree heap[514] );
void huff_dec_main( void );
int main( void );


/*
  Declaration of global variables
*/

static int huff_dec_input_pos;
static int huff_dec_output_pos;
static char huff_dec_output[1024];
unsigned char byte_nb_to_read = 0;
unsigned int val_to_read = 0;


/*
  Initialization- and return-value-related functions
*/

#define huff_dec_plaintext_len 600
static char *huff_dec_plaintext =
  "You are doubtless asking \"How can I reduce the data size without losing "
  "some informations?\". It's easy to answer to this question. I'll only take "
  "an example. I'm sure you have heard about the morse. This system established "
  "in the 19th century use a scheme very close to the huffman one. In the morse "
  "you encode the letters to transmit with two kinds of signs. If you encode "
  "these two sign possibilities in one bit, the symbol 'e' is transmitted in a "
  "single bit and the symbols 'y' and 'z' need four bits. Look at the symbols "
  "in the text you are reading, you'll fast understand the compression ratio...";

#define huff_dec_encoded_len 419
static unsigned char huff_dec_encoded[huff_dec_encoded_len] = { 
  128, 0, 0, 0,  80, 133, 32, 32,  128, 100, 4, 32,  63, 239, 255, 240,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
  4, 7, 174, 21, 129, 234, 5, 112, 132, 201, 21, 66, 43, 228, 84, 134,
  72, 69, 100, 138, 193, 21, 224, 248, 86, 69, 116, 75, 5, 133, 248, 95,
  196, 64, 114, 50, 113, 40, 37, 195, 128, 212, 116, 210, 42, 162, 52, 51,
  12, 132, 120, 245, 99, 37, 33, 48, 39, 135, 173, 34, 167, 94, 87, 142,
  110, 22, 87, 178, 84, 198, 99, 141, 60, 199, 226, 186, 164, 169, 67, 189,
  1, 32, 110, 179, 221, 192, 145, 133, 184, 184, 28, 214, 224, 150, 73, 10,
  242, 20, 83, 152, 252, 26, 223, 9, 143, 149, 183, 226, 149, 131, 175, 215,
  56, 18, 19, 6, 55, 31, 129, 40, 232, 52, 185, 160, 148, 18, 36, 197,
  85, 248, 202, 86, 28, 9, 24, 165, 2, 197, 60, 11, 147, 225, 208, 50,
  99, 174, 155, 76, 224, 72, 197, 193, 189, 184, 121, 94, 17, 213, 120, 35,
  115, 108, 57, 149, 121, 2, 70, 23, 86, 158, 112, 86, 17, 38, 15, 198,
  90, 227, 25, 115, 52, 147, 70, 176, 152, 4, 140, 19, 53, 66, 64, 119,
  65, 61, 190, 15, 60, 56, 27, 168, 215, 194, 175, 55, 193, 218, 41, 224,
  74, 9, 24, 33, 255, 125, 126, 128, 177, 156, 9, 32, 18, 48, 186, 180,
  240, 242, 188, 104, 117, 109, 129, 35, 10, 98, 44, 211, 4, 160, 155, 208,
  119, 146, 9, 100, 144, 9, 42, 132, 243, 22, 48, 191, 3, 159, 224, 220,
  9, 62, 30, 87, 141, 14, 173, 176, 36, 99, 192, 146, 168, 57, 254, 2,
  106, 153, 203, 51, 73, 41, 198, 38, 2, 198, 12, 201, 84, 2, 70, 7,
  235, 178, 180, 12, 102, 4, 152, 38, 244, 29, 228, 139, 88, 76, 14, 7,
  49, 253, 48, 102, 72, 116, 88, 18, 48, 63, 93, 149, 161, 134, 60, 192,
  232, 176, 98, 181, 129, 27, 88, 124, 175, 104, 102, 72, 220, 21, 149, 84,
  227, 136, 18, 48, 63, 93, 149, 161, 137, 128, 72, 192, 178, 96, 135, 149,
  227, 155, 131, 119, 90, 99, 250, 129, 229, 123, 20, 160, 251, 140, 131, 226,
  217, 166, 93, 22, 4, 140, 29, 91, 166, 183, 25, 202, 192, 111, 20, 171,
  142, 57, 83 };


void huff_dec_init( void )
{
  huff_dec_input_pos = 0;
  huff_dec_output_pos = 0;
}


int huff_dec_return( void )
{
  int i;
  _Pragma( "loopbound min 1 max 600" )
  for (i=0; i<huff_dec_plaintext_len; i++) {
    if (huff_dec_plaintext[i] != huff_dec_output[i]) return i+1;
  }
  return 0;
}


/*
  Input / output functions
*/

static int huff_dec_end_of_data()
{
    return huff_dec_input_pos >= huff_dec_encoded_len;
}


static int huff_dec_read_byte()
{
    return huff_dec_encoded[huff_dec_input_pos++];
}


static void huff_dec_write_byte(char ch)
{
  huff_dec_output[huff_dec_output_pos++] = ch;
}


static unsigned char huff_dec_read_code_1_bit()
/* Returned parameters: Returns an unsigned integer with the 0-bit (on the
                        right of the integer) valid
   Action: Reads the next bit in the stream of data to compress
   Errors: An input/output error could disturb the running of the program
   The source must have enough bits to read
*/
{
  if ( byte_nb_to_read ) {
    byte_nb_to_read--;
    return ( ( val_to_read >> byte_nb_to_read ) & 1 );
  } else {
    val_to_read = huff_dec_read_byte();
    byte_nb_to_read = 7;
    return ( ( val_to_read >> 7 ) & 1 );
  }
}


static unsigned int huff_dec_read_code_n_bits( unsigned int n )
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
    while ( ( byte_nb_to_read < 9 ) && ( !huff_dec_end_of_data() ) ) {
      val_to_read = ( val_to_read << 8 ) + huff_dec_read_byte();
      byte_nb_to_read += 8;
    }
    if ( i >= byte_nb_to_read ) {
      result = ( result << byte_nb_to_read ) + val_to_read;
      i -= byte_nb_to_read;
      byte_nb_to_read = 0;
    } else {
      result = ( result << i ) + 
        ( ( val_to_read >> ( byte_nb_to_read - i ) ) & ( ( 1 << i ) - 1 ) );
      byte_nb_to_read -= i;
      i = 0;
    }
  }
  return ( result );
}


static void huff_dec_read_header( t_bin_val codes_table[257] )
/* Returned parameters: The contain of 'codes_table' is modified
   Action: Rebuilds the binary encoding array by using the header
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned int i, j;
  unsigned num_byte;

  _Pragma ( "loopbound min 257 max 257" )
  for ( i = 0; i < 257; i++) {
    codes_table[i].bits_nb = 0;
    _Pragma ( "loopbound min 32 max 32" )
    for ( j = 0; j < 32; j++ ) {
      codes_table[i].bits[j] = 0;
    }
  }

  /* == Decoding of the first part of the header === */
  if ( huff_dec_read_code_1_bit() )
    /* First bit=0 => Present bytes coded on n*8 bits
                =1 => Present bytes coded on 256 bits */
  _Pragma ( "loopbound min 256 max 256" )
    for ( i = 0; i <= 255; i++ )
      codes_table[i].presence = huff_dec_read_code_1_bit();
  else {
    i = huff_dec_read_code_n_bits( 5 ) + 1;
    _Pragma ( "loopbound min 1 max 32" )
    while ( i ) {
      codes_table[huff_dec_read_code_n_bits( 8 )].presence = 1;
      i--;
    }
  }
  codes_table[256].presence = 1;
  /* Presence of a fictive 256-byte is enforced! */

  /* == Decoding the second part of the header == */
  _Pragma ( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ )
    if ( codes_table[i].presence ) {
      if ( huff_dec_read_code_1_bit() )
        /* First bit=0 => 5 bits of binary length-1 followed by a binary word
                    =1 => 8 bits of binary length-1 followed by a binary word */
        j = huff_dec_read_code_n_bits( 8 ) + 1;
      else j = huff_dec_read_code_n_bits( 5 ) + 1;
      codes_table[i].bits_nb = j;
      /* Reading of a binary word */
      num_byte = ( j - 1 ) >> 3;
      if ( j & 7 ) {
        /* Reads the bits that takes less than one byte */
        codes_table[i].bits[num_byte] = 
          ( unsigned char )huff_dec_read_code_n_bits( j & 7 );
        j -= j & 7;
        num_byte--;
      }

      _Pragma ( "loopbound min 0 max 1" )
      while ( j >= 8 ) {
        /* Reads the bits that takes one byte, at least */
        codes_table[i].bits[num_byte] = 
          ( unsigned char )huff_dec_read_code_n_bits( 8 );
        j -= 8;
        num_byte--;
      }
    }
}


static huff_dec_t_tree *huff_dec_tree_encoding( t_bin_val codes_table[257],
                                                huff_dec_t_tree heap[514] )
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

  ptr_tree = &heap[heap_top++];
  ptr_tree->byte = 257;
  ptr_tree->left_ptr = 0;
  ptr_tree->right_ptr = 0;
  _Pragma ( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ ) {
    _Pragma ( "loopbound min 0 max 9" )
    for ( current_node = ptr_tree, j = codes_table[i].bits_nb; j; j-- ) {
      if ( codes_table[i].bits[( j - 1 ) >> 3] & ( 1 << ( (
             j - 1 ) & 7 ) ) )
        if ( current_node->left_ptr == 0 ) {
          current_node->left_ptr = &heap[heap_top++];
          current_node = current_node->left_ptr;
          current_node->left_ptr = 0;
          current_node->right_ptr = 0;
        } else current_node = current_node->left_ptr;
      else
        if ( current_node->right_ptr == 0 ) {
          current_node->right_ptr = &heap[heap_top++];
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
  t_bin_val encoding_table[257];
  huff_dec_t_tree heap[514]; /* space for dynamically allocated nodes */
  huff_dec_t_tree *ptr_tree;
  huff_dec_t_tree *current_node;

  if ( !huff_dec_end_of_data() ) { /* Are there data to compress? */
    huff_dec_read_header( encoding_table );
    ptr_tree = huff_dec_tree_encoding( encoding_table, heap );
    _Pragma ( "loopbound min 602 max 602" )
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
