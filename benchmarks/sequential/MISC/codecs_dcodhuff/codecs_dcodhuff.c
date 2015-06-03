/* File: dcodhuff.c
  Author: David Bourgin
  Creation date: 15/2/94
  Last update: 26/9/94
  Purpose: Example of Huffman decoding with a file source to compress.
*/

extern unsigned char input[419];

#include "codecs_dcodhuff.h"
#include "wccmalloc.h"

/* Global variables */
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

static unsigned char byte_nb_to_read = 0;
static unsigned int val_to_read = 0;

static unsigned char read_code_1_bit( void )
/* Returned parameters: Returns an unsigned integer with the 0-bit (on the right of the integer) valid
   Action: Reads the next bit in the stream of data to compress
   Errors: An input/output error could disturb the running of the program
   The source must have enough bits to read
*/
{ if (byte_nb_to_read)
  {
    byte_nb_to_read--;
    return ((val_to_read >> byte_nb_to_read) & 1);
  } else
  {
    val_to_read = read_byte();
    byte_nb_to_read = 7;
    return ((val_to_read >> 7) & 1);
  }
}

static unsigned int read_code_n_bits( unsigned char n )
/* Returned parameters: Returns an unsigned integer with the n-bits (on the right of the integer) valid
   Action: Reads the next n bits in the stream of data to compress
   Errors: An input/output error could disturb the running of the program
   The source must have enough bits to read
*/
{
  register unsigned char i;
  unsigned int result;

  result = 0;
  i = n;
  _Pragma( "loopbound min 1 max 1" )
  while (i) {
    _Pragma( "loopbound min 0 max 2" )
    while ((byte_nb_to_read < 9) && (!end_of_data())) {
      val_to_read = (val_to_read << 8) + read_byte();
      byte_nb_to_read += 8;
    }
    if (i >= byte_nb_to_read) {
      result = (result << byte_nb_to_read) + val_to_read;
      i -= byte_nb_to_read;
      byte_nb_to_read = 0;
    } else {
      result = (result << i) + ((val_to_read >> (byte_nb_to_read - i)) & ((1 << i) - 1));
      byte_nb_to_read -= i;
      i = 0;
    }
  }
  return (result);
}

static void read_header( t_bin_val codes_table[257] )
/* Returned parameters: The contain of 'codes_table' is modified
   Action: Rebuilds the binary encoding array by using the header
   Errors: An input/output error could disturb the running of the program
*/
{
  register unsigned int i, j;
  char num_byte;

  wccbzero((char *)codes_table, 257*sizeof(*codes_table));
  /* == Decoding of the first part of the header === */
  if (read_code_1_bit())
    /* First bit=0 => Present bytes coded on n*8 bits
                =1 => Present bytes coded on 256 bits */
    _Pragma( "loopbound min 256 max 256" )
    for (i = 0;i <= 255;i++)
      PRESENCE_BIN_VAL(codes_table[i]) = read_code_1_bit();
  else {
    i = read_code_n_bits(5) + 1;
    _Pragma( "loopbound min 0 max 0" )
    while (i) {
      PRESENCE_BIN_VAL(codes_table[read_code_n_bits(8)]) = 1;
      i--;
    }
  }
  PRESENCE_BIN_VAL(codes_table[256]) = 1;
  /* Presence of a fictive 256-byte is enforced! */

  /* == Decoding the second part of the header == */
  _Pragma( "loopbound min 257 max 257" )
  for (i = 0;i <= 256;i++)
    if PRESENCE_BIN_VAL(codes_table[i]) {
      if (read_code_1_bit())
        /* First bit=0 => 5 bits of binary length-1 followed by a binary word
                    =1 => 8 bits of binary length-1 followed by a binary word */
        j = read_code_n_bits(8) + 1;
      else
        j = read_code_n_bits(5) + 1;
      NB_BITS_BIN_VAL(codes_table[i]) = j;
      /* Reading of a binary word */
      num_byte = (j - 1) >> 3;
      if (j & 7) {              /* Reads the bits that takes less than one byte */
        BITS_BIN_VAL(codes_table[i])[num_byte] = (unsigned char)read_code_n_bits(j & 7);
        j -= j & 7;
        num_byte--;
      }
      _Pragma( "loopbound min 0 max 1" )
      while (j >= 8) {           /* Reads the bits that takes one byte, at least */
        BITS_BIN_VAL(codes_table[i])[num_byte] = (unsigned char)read_code_n_bits(8);
        j -= 8;
        num_byte--;
      }
    }
}

/*static void suppress_tree( p_tree tree )
{
  if (tree != NULL) {
    suppress_tree(LEFTPTR_OF_TREE(tree));
    suppress_tree(RIGHTPTR_OF_TREE(tree));
  }
}*/

static p_tree tree_encoding( t_bin_val codes_table[257] )
/* Returned parameters: A binary tree is returned
   Action: Returns the decoding binary tree built from 'codes_table'
   Errors: None
*/
{ register unsigned int i;
  unsigned int j;
  p_tree ptr_tree, current_node;

  if ((ptr_tree = (p_tree)wccmalloc(sizeof(t_tree))) == NULL)
  {
  }
  TREE_BYTE(ptr_tree) = 257;
  LEFTPTR_OF_TREE(ptr_tree) = NULL;
  RIGHTPTR_OF_TREE(ptr_tree) = NULL;
  _Pragma( "loopbound min 257 max 257" )
  for (i = 0;i <= 256;i++)
  {
    _Pragma( "loopbound min 0 max 9" )
    for (current_node = ptr_tree, j = NB_BITS_BIN_VAL(codes_table[i]);j;j--) {
      if (BITS_BIN_VAL(codes_table[i])[(j - 1) >> 3] & (1 << ((j - 1) & 7)))
        if (LEFTPTR_OF_TREE(current_node) == NULL) {
          if ((LEFTPTR_OF_TREE(current_node) = (p_tree)wccmalloc(sizeof(t_tree))) == NULL) {
            //suppress_tree(ptr_tree);
          }
          current_node = LEFTPTR_OF_TREE(current_node);
          LEFTPTR_OF_TREE(current_node) = NULL;
          RIGHTPTR_OF_TREE(current_node) = NULL;
        } else
          current_node = LEFTPTR_OF_TREE(current_node);
      else if (RIGHTPTR_OF_TREE(current_node) == NULL) {
        if ((RIGHTPTR_OF_TREE(current_node) = (p_tree)wccmalloc(sizeof(t_tree))) == NULL) {
          //suppress_tree(ptr_tree);
        }
        current_node = RIGHTPTR_OF_TREE(current_node);
        LEFTPTR_OF_TREE(current_node) = NULL;
        RIGHTPTR_OF_TREE(current_node) = NULL;
      } else
        current_node = RIGHTPTR_OF_TREE(current_node);
      if (j == 1)
        TREE_BYTE(current_node) = i;
      else
        TREE_BYTE(current_node) = 257;
    }
  };
  return (ptr_tree);
}

void huffmandecoding( unsigned char *input, unsigned int length )
/* Returned parameters: None
   Action: Decompresses with Huffman method all bytes read by the function 'read_code_1_bit' and 'read_code_n_bits'
   Errors: An input/output error could disturb the running of the program
*/
{
  source_memory_base = (unsigned char*)input;
  source_memory_end = (unsigned char*)(input + length);
  source_ptr = source_memory_base;

  t_bin_val encoding_table[257];
  p_tree ptr_tree, current_node;

  if (!end_of_data())    /* Are there data to compress? */
  { read_header(encoding_table);
    ptr_tree = tree_encoding(encoding_table);
    _Pragma( "loopbound min 602 max 602" )
    do {
      current_node = ptr_tree;
      _Pragma( "loopbound min 3 max 9" )
      while (TREE_BYTE(current_node) == 257) {
        if (read_code_1_bit())
          /* Bit=1 => Got to left in the node of the tree
                =0 => Got to right in the node of the tree */
          current_node = LEFTPTR_OF_TREE(current_node);
        else
          current_node = RIGHTPTR_OF_TREE(current_node);
      }
      if (TREE_BYTE(current_node) <= 255)
        write_byte(TREE_BYTE(current_node));
    } while (TREE_BYTE(current_node) != 256);
    //suppress_tree(ptr_tree);
  }
}

int main( void )
{
  huffmandecoding( input, 419 );
  return 0;
}
