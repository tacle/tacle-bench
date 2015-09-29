/* File: codhuff.c
  Author: David Bourgin (David.Bourgin@ufrima.imag.fr)
  Creation date: 7/2/94
  Last update: 22/9/94
  Purpose: Example of Huffman encoding with a file source to compress.
*/

#include "codecs_codhuff.h"
#include "wccstdlib.h"
#include "wccmalloc.h"


/* Global variables */
extern char *input;
static unsigned char *source_memory_base;  /* Base of the source memory */
static unsigned char *source_memory_end;   /* Last address to read.
           source_memory_end=source_memory_base+source_zone_length-1 */
static unsigned char *source_ptr;  /* Used in the xxxcoding procedure */

/* Pseudo procedures */
static void beginning_of_data( void )
{
  source_ptr = source_memory_base;
}
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

static unsigned char byte_nb_to_write = 0,
                                        val_to_write = 0;

static void write_bin_val( t_bin_val bin_val )
/* Returned parameters: None
   Action: Writes in the output stream the value binary-coded into 'bin_val'
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned char bit_indice, bin_pos;
  char pos_byte;

  _Pragma( "loopbound min 1 max 9" )
  for (bin_pos = (BITS_NB_BIN_VAL(bin_val) - 1) & 7,
       pos_byte = (BITS_NB_BIN_VAL(bin_val) - 1) >> 3, bit_indice = 1;
       bit_indice <= BITS_NB_BIN_VAL(bin_val); bit_indice++) {
    /* Watch for the current bit to write */
    val_to_write = (val_to_write << 1) | ((BITS_BIN_VAL(bin_val)[pos_byte] >> bin_pos) & 1);
    /* Move to the next bit to write */
    if (!bin_pos) {
      pos_byte--;
      bin_pos = 7;
    } else
      bin_pos--;
    if (byte_nb_to_write == 7)
      /* Are already 8 bits written? */
    { write_byte(val_to_write);
      byte_nb_to_write = 0;
      val_to_write = 0;
    } else                 /* No, then the next writting will be in the next bit */
      byte_nb_to_write++;
  }
}

static void fill_encoding( void )
/* Returned parameters: None
   Action: Fills the last byte to write in the output stream with zero values
   Errors: An input/output error could disturb the running of the program
*/
{ if (byte_nb_to_write)
    write_byte(val_to_write << (8 - byte_nb_to_write));
}

static void write_header( t_bin_val codes_table[257] )
/* Returned parameters: None
   Action: Writes the header in the stream of codes
   Errors: An input/output error could disturb the running of the program
*/
{
  register unsigned int i, j;
  t_bin_val bin_val_to_0,
  bin_val_to_1,
  bin_val;         /* Is used to send in binary mode via write_bin_val */

  *BITS_BIN_VAL(bin_val_to_0) = 0;
  BITS_NB_BIN_VAL(bin_val_to_0) = 1;
  *BITS_BIN_VAL(bin_val_to_1) = 1;
  BITS_NB_BIN_VAL(bin_val_to_1) = 1;
  _Pragma( "loopbound min 256 max 256" )
  for (i = 0, j = 0;j <= 255;j++)
    if BITS_NB_BIN_VAL(codes_table[j])
      i++;
  /* From there, i contains the number of bytes of the several non null occurrences to encode */
  /* First part of the header: Specifies the bytes that appear in the source of encoding */
  if (i < 32) {                       /* Encoding of the appeared bytes with a block of bytes */
    write_bin_val(bin_val_to_0);
    BITS_NB_BIN_VAL(bin_val) = 5;
    *BITS_BIN_VAL(bin_val) = (unsigned char)(i - 1);
    write_bin_val(bin_val);
    BITS_NB_BIN_VAL(bin_val) = 8;
    _Pragma( "loopbound min 256 max 256" )
    for (j = 0;j <= 255;j++)
      if BITS_NB_BIN_VAL(codes_table[j]) {
        *BITS_BIN_VAL(bin_val) = (unsigned char)j;
        write_bin_val(bin_val);
      }
  } else {                     /* Encoding of the appeared bytes with a block of bits */
    write_bin_val(bin_val_to_1);
    _Pragma( "loopbound min 256 max 256" )
    for (j = 0;j <= 255;j++)
      if BITS_NB_BIN_VAL(codes_table[j])
        write_bin_val(bin_val_to_1);
      else
        write_bin_val(bin_val_to_0);
  };
  /* Second part of the header: Specifies the encoding of the bytes (fictive or not) that appear in the source of encoding */
  _Pragma( "loopbound min 257 max 257" )
  for (i = 0;i <= 256;i++)
    if ( j = BITS_NB_BIN_VAL(codes_table[i]) ) {
      if (j < 33) {
        write_bin_val(bin_val_to_0);
        BITS_NB_BIN_VAL(bin_val) = 5;
      } else {
        write_bin_val(bin_val_to_1);
        BITS_NB_BIN_VAL(bin_val) = 8;
      }
      *BITS_BIN_VAL(bin_val) = (unsigned char)(j - 1);
      write_bin_val(bin_val);
      write_bin_val(codes_table[i]);
    }
}

static p_tree build_tree_encoding( void )
/* Returned parameters: Returns a tree of encoding
   Action: Generates an Huffman encoding tree based on the data from the stream to compress
   Errors: If no memory is available for the allocations then a 'BAD_MEM_ALLOC' exception is raised
*/
{ register unsigned int i;
  p_tree occurrences_table[257],
  ptr_fictive_tree;

  /* Sets up the occurrences number of all bytes to 0 */
  _Pragma( "loopbound min 257 max 257" )
  for (i = 0;i <= 256;i++)
  {
    if ((occurrences_table[i] = (p_tree)wccmalloc(sizeof(t_tree))) == NULL) {
    }
    BYTE_OF_TREE(occurrences_table[i]) = i;
    WEIGHT_OF_TREE(occurrences_table[i]) = 0;
    LEFTPTR_OF_TREE(occurrences_table[i]) = NULL;
    RIGHTPTR_OF_TREE(occurrences_table[i]) = NULL;
  }
  /* Valids the occurrences of 'occurrences_table' with regard to the data to compressr */
  if (!end_of_data())
  {
    _Pragma( "loopbound min 601 max 601" )
    while (!end_of_data()) {
      i = read_byte();
      WEIGHT_OF_TREE(occurrences_table[i])++;
    }
    WEIGHT_OF_TREE(occurrences_table[256]) = 1;
    /* Sorts the occurrences table depending on the weight of each character */

    _Pragma( "marker call_qsort" )
    _Pragma( "flowrestriction 1*qsorts <= 77*call_qsort" )
    qsorts((char*)occurrences_table, 257, sizeof(p_tree));

    _Pragma( "loopbound min 218 max 218" )
    for (i = 256;(i != 0) && (!WEIGHT_OF_TREE(occurrences_table[i]));i--) {
      ;
    }
    i++;
    /* From there, 'i' gives the number of different bytes with a null occurrence in the stream to compress */

    _Pragma( "loopbound min 38 max 38" )
    while (i > 0) {
      /* Looks up (i+1)/2 times the occurrence table to link the nodes in an uniq tree */
      if ((ptr_fictive_tree = (p_tree)wccmalloc(sizeof(t_tree))) == NULL) {
        _Pragma( "loopbound min 257 max 257" )
        for (i = 0;i <= 256;i++) ;
      }
      BYTE_OF_TREE(ptr_fictive_tree) = 257;
      WEIGHT_OF_TREE(ptr_fictive_tree) = WEIGHT_OF_TREE(occurrences_table[--i]);
      LEFTPTR_OF_TREE(ptr_fictive_tree) = occurrences_table[i];
      if (i)
      {
        i--;
        WEIGHT_OF_TREE(ptr_fictive_tree) += WEIGHT_OF_TREE(occurrences_table[i]);
        RIGHTPTR_OF_TREE(ptr_fictive_tree) = occurrences_table[i];
      } else
        RIGHTPTR_OF_TREE(ptr_fictive_tree) = NULL;
      occurrences_table[i] = ptr_fictive_tree;

      _Pragma( "marker call2_qsort" )
      _Pragma( "flowrestriction 1*qsorts <= 77*call2_qsort" )
      qsorts((char*)occurrences_table, i + 1, sizeof(p_tree));

      if (i)        /* Is there an other node in the occurrence tables? */
        i++;       /* Yes, then takes care to the fictive node */
    }
  }
  return (*occurrences_table);
}

static void encode_codes_table( p_tree tree, t_bin_val codes_table[257],
  t_bin_val *code_val )
/* Returned parameters: The data of 'codes_table' can have been modified
   Action: Stores the encoding tree as a binary encoding table to speed up the access.
   'val_code' gives the encoding for the current node of the tree
   Errors: None
*/
{
  register unsigned int i;
  t_bin_val tmp_code_val;

  if (BYTE_OF_TREE(tree) == 257) {
    if (LEFTPTR_OF_TREE(tree) != NULL)
      /* The sub-trees on left begin with an bit set to 1 */
    { tmp_code_val = *code_val;
      _Pragma( "loopbound min 31 max 31" )
      for (i = 31;i > 0;i--) {
        BITS_BIN_VAL(*code_val)[i] = (BITS_BIN_VAL(*code_val)[i] << 1) |
          (BITS_BIN_VAL(*code_val)[i - 1] >> 7);
      }
      *BITS_BIN_VAL(*code_val) = (*BITS_BIN_VAL(*code_val) << 1) | 1;
      BITS_NB_BIN_VAL(*code_val)++;
      encode_codes_table(LEFTPTR_OF_TREE(tree), codes_table, code_val);
      *code_val = tmp_code_val;
    };
    if (RIGHTPTR_OF_TREE(tree) != NULL)
      /* The sub-trees on right begin with an bit set to 0 */
    { tmp_code_val = *code_val;
      _Pragma( "loopbound min 31 max 31" )
      for (i = 31;i > 0;i--) {
        BITS_BIN_VAL(*code_val)[i] = (BITS_BIN_VAL(*code_val)[i] << 1) |
          (BITS_BIN_VAL(*code_val)[i - 1] >> 7);
      }
      *BITS_BIN_VAL(*code_val) <<= 1;
      BITS_NB_BIN_VAL(*code_val)++;
      encode_codes_table(RIGHTPTR_OF_TREE(tree), codes_table, code_val);
      *code_val = tmp_code_val;
    };
  } else
    codes_table[BYTE_OF_TREE(tree)] = *code_val;
}

static void create_codes_table( p_tree tree, t_bin_val codes_table[257] )
/* Returned parameters: The data in 'codes_table' will be modified
   Action: Stores the encoding tree as a binary encoding table to speed up the access by calling encode_codes_table
   Errors: None
*/
{
  t_bin_val code_val;

  wccbzero((char *)&code_val, sizeof(code_val));
  wccbzero((char *)codes_table, 257*sizeof(*codes_table));
  _Pragma( "marker call_encode" )
  _Pragma( "flowrestriction 1*encode_codes_table <= 77*call_encode" )
  encode_codes_table(tree, codes_table, &code_val);
}

void huffmanencoding( char* input, unsigned int length )
/* Returned parameters: None
   Action: Compresses with Huffman method all bytes read by the function 'read_byte'
   Errors: An input/output error could disturb the running of the program
*/
{
  source_memory_base = (unsigned char*)input;
  source_memory_end = (unsigned char*)(input + length);
  source_ptr = source_memory_base;

  p_tree tree;
  t_bin_val encoding_table[257];
  unsigned char byte_read;

  if (!end_of_data())
    /* Generates only whether there are data */
  { tree = build_tree_encoding();
    /* Creation of the best adapted tree */
    create_codes_table(tree, encoding_table);
    /* Obtains the binary encoding in an array to speed up the accesses */
    write_header(encoding_table);
    /* Writes the defintion of the encoding */
    beginning_of_data();  /* Real compression of the data */

    _Pragma( "loopbound min 601 max 601" )
    while (!end_of_data()) {
      byte_read = read_byte();
      write_bin_val(encoding_table[byte_read]);
    }
    write_bin_val(encoding_table[256]);
    /* Code of the end of encoding */
    fill_encoding();
    /* Fills the last byte before closing file, if any */
  }
}

int main( void )
{
  huffmanencoding( input, 600 );
  return 0;
}
