/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: codhuff

  Author: David Bourgin (David.Bourgin@ufrima.imag.fr)

  Function: Example of Huffman encoding

  Source: ftp://turing.imag.fr/pub/compression/ (1994-09-22)

  Original name: codhuff.c

  Changes: no major functional changes

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


typedef struct codhuff_s_tree {
  unsigned int byte;
  /* A byte has to be coded as an unsigned integer to allow a node to have a
    value over 255 */
  unsigned long int weight;
  struct codhuff_s_tree *left_ptr, *right_ptr;
} codhuff_t_tree, *codhuff_p_tree;

typedef struct {
  unsigned char bits[32];
  unsigned int bits_nb;
} codhuff_t_bin_val;


/*
  Forward declaration of functions
*/

void *codhuff_malloc( unsigned int numberOfBytes );
void codhuff_qsort( char *a, unsigned long n, unsigned long es );
static void codhuff_beginning_of_data( void );
static int codhuff_end_of_data( void );
static unsigned char codhuff_read_byte( void );
static void codhuff_write_byte( unsigned char byte );
static void codhuff_write_bin_val( codhuff_t_bin_val bin_val );
static void codhuff_fill_encoding( void );
static void codhuff_write_header( codhuff_t_bin_val codes_table[257] );
int codhuff_weight_tree_comp( char *c1, char *c2 );
static codhuff_p_tree codhuff_build_tree_encoding( void );
static void codhuff_encode_codes_table( codhuff_p_tree tree,
                                        codhuff_t_bin_val codes_table[257],
                                        codhuff_t_bin_val *code_val );
static void codhuff_create_codes_table( codhuff_p_tree tree,
                                        codhuff_t_bin_val codes_table[257] );
void codhuff_huffmanencoding( char *input, unsigned int length );
void codhuff_main( void );
int codhuff_return( void );
int main( void );





/*
  Declaration of global variables
*/

extern char *codhuff_input;
static unsigned char
*codhuff_source_memory_base;  /* Base of the source memory */
static unsigned char *codhuff_source_memory_end;   /* Last address to read.
           source_memory_end=source_memory_base+source_zone_length-1 */
static unsigned char *codhuff_source_ptr;  /* Used in the xxxcoding procedure */
static unsigned char codhuff_byte_nb_to_write = 0;
static unsigned char codhuff_val_to_write = 0;

static unsigned int codhuff_checksum0 = 0;
static unsigned int codhuff_checksum1 = 0;

/*
  Pseudo functions
*/

static void codhuff_beginning_of_data( void )
{
  codhuff_source_ptr = codhuff_source_memory_base;
}


static int codhuff_end_of_data( void )
{
  return codhuff_source_ptr > codhuff_source_memory_end;
}


static unsigned char codhuff_read_byte( void )
{
  return *( codhuff_source_ptr++ );
}


static void codhuff_write_byte( unsigned char byte )
{
  codhuff_checksum0 += byte;
  codhuff_checksum1 += codhuff_checksum0;
}


static void codhuff_write_bin_val( codhuff_t_bin_val bin_val )
/* Returned parameters: None
   Action: Writes in the output stream the value binary-coded into 'bin_val'
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned char bit_indice, bin_pos, pos_byte;

  _Pragma( "loopbound min 1 max 9" )
  for ( bin_pos = ( bin_val.bits_nb - 1 ) & 7,
        pos_byte = ( bin_val.bits_nb - 1 ) >> 3, bit_indice = 1;
        bit_indice <= bin_val.bits_nb; bit_indice++ ) {
    /* Watch for the current bit to write */
    codhuff_val_to_write = ( codhuff_val_to_write << 1 )
                           | ( ( bin_val.bits[pos_byte] >> bin_pos ) & 1 );
    /* Move to the next bit to write */
    if ( !bin_pos ) {
      pos_byte--;
      bin_pos = 7;
    } else
      bin_pos--;
    if ( codhuff_byte_nb_to_write == 7 ) {
      /* Are already 8 bits written? */
      codhuff_write_byte( codhuff_val_to_write );
      codhuff_byte_nb_to_write = 0;
      codhuff_val_to_write = 0;
    } else {
      /* No, then the next writting will be in the next bit */
      codhuff_byte_nb_to_write++;
    }
  }
}


static void codhuff_fill_encoding( void )
/* Returned parameters: None
   Action: Fills the last byte to write in the output stream with zero values
   Errors: An input/output error could disturb the running of the program
*/
{
  if ( codhuff_byte_nb_to_write )
    codhuff_write_byte( codhuff_val_to_write <<
                        ( 8 - codhuff_byte_nb_to_write ) );
}


static void codhuff_write_header( codhuff_t_bin_val codes_table[257] )
/* Returned parameters: None
   Action: Writes the header in the stream of codes
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned int i, j;
  codhuff_t_bin_val bin_val_to_0,
                    bin_val_to_1,
                    bin_val;
  /* Is used to send in binary mode via write_bin_val */

  *bin_val_to_0.bits = 0;
  bin_val_to_0.bits_nb = 1;
  *bin_val_to_1.bits = 1;
  bin_val_to_1.bits_nb = 1;
  _Pragma( "loopbound min 256 max 256" )
  for ( i = 0, j = 0; j <= 255; j++ )
    if ( codes_table[j].bits_nb )
      i++;
  /* From there, i contains the number of bytes of the several non null
     occurrences to encode. First part of the header: Specifies the bytes that
     appear in the source of encoding */
  if ( i < 32 ) {
    /* Encoding of the appeared bytes with a block of bytes */
    codhuff_write_bin_val( bin_val_to_0 );
    bin_val.bits_nb = 5;
    *bin_val.bits = ( unsigned char )( i - 1 );
    codhuff_write_bin_val( bin_val );
    bin_val.bits_nb = 8;
    _Pragma( "loopbound min 256 max 256" )
    for ( j = 0; j <= 255; j++ )
      if ( codes_table[j].bits_nb ) {
        *bin_val.bits = ( unsigned char )j;
        codhuff_write_bin_val( bin_val );
      }
  } else {
    /* Encoding of the appeared bytes with a block of bits */
    codhuff_write_bin_val( bin_val_to_1 );
    _Pragma( "loopbound min 256 max 256" )
    for ( j = 0; j <= 255; j++ )
      if ( codes_table[j].bits_nb )
        codhuff_write_bin_val( bin_val_to_1 );
      else
        codhuff_write_bin_val( bin_val_to_0 );
  };
  /* Second part of the header: Specifies the encoding of the bytes (fictive or
     not) that appear in the source of encoding */
  _Pragma( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ )
    if ( ( j = codes_table[i].bits_nb ) != 0 ) {
      if ( j < 33 ) {
        codhuff_write_bin_val( bin_val_to_0 );
        bin_val.bits_nb = 5;
      } else {
        codhuff_write_bin_val( bin_val_to_1 );
        bin_val.bits_nb = 8;
      }
      *bin_val.bits = ( unsigned char )( j - 1 );
      codhuff_write_bin_val( bin_val );
      codhuff_write_bin_val( codes_table[i] );
    }
}


int codhuff_weight_tree_comp( char *c1, char *c2 )
/* Returned parameters: Returns a comparison status
   Action: Returns a negative, zero or positive integer depending on the weight
           of 'tree2' is less than, equal to, or greater than the weight of
           'tree1'
   Errors: None
*/
{
  codhuff_p_tree *tree1 = ( codhuff_p_tree * )c1;
  codhuff_p_tree *tree2 = ( codhuff_p_tree * )c2;

  return ( ( **tree2 ).weight ^ ( **tree1 ).weight )
         ? ( ( ( **tree2 ).weight < ( **tree1 ).weight ) ? -1 : 1 )
         : 0;
}


static codhuff_p_tree codhuff_build_tree_encoding( void )
/* Returned parameters: Returns a tree of encoding
   Action: Generates an Huffman encoding tree based on the data from the stream
           to compress
   Errors: If no memory is available for the allocations then a 'BAD_MEM_ALLOC'
           exception is raised
*/
{
  unsigned int i;
  codhuff_p_tree occurrences_table[257], ptr_fictive_tree;

  /* Sets up the occurrences number of all bytes to 0 */
  _Pragma( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ ) {
    occurrences_table[i] = ( codhuff_p_tree )codhuff_malloc( sizeof(
                             codhuff_t_tree ) );
    occurrences_table[i]->byte = i;
    occurrences_table[i]->weight = 0;
    occurrences_table[i]->left_ptr = 0;
    occurrences_table[i]->right_ptr = 0;
  }
  /* Valids the occurrences of 'occurrences_table' with regard to the data to
     compressr */
  if ( !codhuff_end_of_data() ) {
    _Pragma( "loopbound min 601 max 601" )
    while ( !codhuff_end_of_data() ) {
      i = codhuff_read_byte();
      occurrences_table[i]->weight++;
    }
    occurrences_table[256]->weight = 1;
    /* Sorts the occurrences table depending on the weight of each character */

    _Pragma( "marker call_qsort" )
    _Pragma( "flowrestriction 1*codhuff_qsort <= 77*call_qsort" )
    codhuff_qsort( ( char * )occurrences_table, 257, sizeof( codhuff_p_tree ) );

    _Pragma( "loopbound min 218 max 218" )
    for ( i = 256; ( i != 0 ) &&
          ( !occurrences_table[i]->weight ); i-- ) {
      ;
    }
    i++;
    /* From there, 'i' gives the number of different bytes with a null
       occurrence in the stream to compress */

    _Pragma( "loopbound min 38 max 38" )
    while ( i > 0 ) {
      /* Looks up (i+1)/2 times the occurrence table to link the nodes in an
         unique tree */
      ptr_fictive_tree =
        ( codhuff_p_tree )codhuff_malloc( sizeof( codhuff_t_tree ) );
      if ( ptr_fictive_tree == 0 ) {
        _Pragma( "loopbound min 257 max 257" )
        for ( i = 0; i <= 256; i++ ) ;
      }
      ptr_fictive_tree->byte = 257;
      ptr_fictive_tree->weight = occurrences_table[--i]->weight;
      ptr_fictive_tree->left_ptr = occurrences_table[i];
      if ( i ) {
        i--;
        ptr_fictive_tree->weight += occurrences_table[i]->weight;
        ptr_fictive_tree->right_ptr = occurrences_table[i];
      } else
        ptr_fictive_tree->right_ptr = 0;
      occurrences_table[i] = ptr_fictive_tree;

      _Pragma( "marker call2_qsort" )
      _Pragma( "flowrestriction 1*codhuff_qsort <= 77*call2_qsort" )
      codhuff_qsort( ( char * )occurrences_table, i + 1,
                     sizeof( codhuff_p_tree ) );

      if ( i )      /* Is there an other node in the occurrence tables? */
        i++;       /* Yes, then takes care to the fictive node */
    }
  }
  return ( *occurrences_table );
}


static void codhuff_encode_codes_table( codhuff_p_tree tree,
                                        codhuff_t_bin_val codes_table[257],
                                        codhuff_t_bin_val *code_val )
/* Returned parameters: The data of 'codes_table' can have been modified
   Action: Stores the encoding tree as a binary encoding table to speed up the
           access. 'val_code' gives the encoding for the current node of the
           tree
   Errors: None
*/
{
  unsigned int i;
  codhuff_t_bin_val tmp_code_val;

  if ( tree->byte == 257 ) {
    if ( tree->left_ptr != 0 )
      /* The sub-trees on left begin with an bit set to 1 */
    {
      tmp_code_val = *code_val;
      _Pragma( "loopbound min 31 max 31" )
      for ( i = 31; i > 0; i-- ) {
        code_val->bits[i] = ( code_val->bits[i] << 1 ) |
                            ( code_val->bits[i - 1] >> 7 );
      }
      *code_val->bits = ( *code_val->bits << 1 ) | 1;
      code_val->bits_nb++;
      codhuff_encode_codes_table( tree->left_ptr, codes_table, code_val );
      *code_val = tmp_code_val;
    };
    if ( tree->right_ptr != 0 )
      /* The sub-trees on right begin with an bit set to 0 */
    {
      tmp_code_val = *code_val;
      _Pragma( "loopbound min 31 max 31" )
      for ( i = 31; i > 0; i-- ) {
        code_val->bits[i] = ( code_val->bits[i] << 1 ) |
                            ( code_val->bits[i - 1] >> 7 );
      }
      *code_val->bits <<= 1;
      code_val->bits_nb++;
      codhuff_encode_codes_table( tree->right_ptr, codes_table, code_val );
      *code_val = tmp_code_val;
    };
  } else
    codes_table[tree->byte] = *code_val;
}


static void codhuff_create_codes_table( codhuff_p_tree tree,
                                        codhuff_t_bin_val codes_table[257] )
/* Returned parameters: The data in 'codes_table' will be modified
   Action: Stores the encoding tree as a binary encoding table to speed up the
          access by calling encode_codes_table
   Errors: None
*/
{
  unsigned int i, j;
  codhuff_t_bin_val code_val;

  for ( i = 0; i < 32; i++ ) {
    code_val.bits[i] = 0;
  }
  code_val.bits_nb = 0;
  for ( j = 0; j < 257; j++ ) {
    for ( i = 0; i < 32; i++ ) {
      codes_table[j].bits[i] = 0;
    }
    codes_table[j].bits_nb = 0;
  }
  _Pragma( "marker call_encode" )
  _Pragma( "flowrestriction 1*encode_codes_table <= 77*call_encode" )
  codhuff_encode_codes_table( tree, codes_table, &code_val );
}


void codhuff_huffmanencoding( char *input, unsigned int length )
/* Returned parameters: None
   Action: Compresses with Huffman method all bytes read by the function
           'read_byte'
   Errors: An input/output error could disturb the running of the program
*/
{
  codhuff_source_memory_base = ( unsigned char * )input;
  codhuff_source_memory_end = ( unsigned char * )( input + length );
  codhuff_source_ptr = codhuff_source_memory_base;

  codhuff_p_tree tree;
  codhuff_t_bin_val encoding_table[257];
  unsigned char byte_read;

  if ( !codhuff_end_of_data() )
    /* Generates only whether there are data */
  {
    tree = codhuff_build_tree_encoding();
    /* Creation of the best adapted tree */
    codhuff_create_codes_table( tree, encoding_table );
    /* Obtains the binary encoding in an array to speed up the accesses */
    codhuff_write_header( encoding_table );
    /* Writes the defintion of the encoding */
    codhuff_beginning_of_data();  /* Real compression of the data */

    _Pragma( "loopbound min 601 max 601" )
    while ( !codhuff_end_of_data() ) {
      byte_read = codhuff_read_byte();
      codhuff_write_bin_val( encoding_table[byte_read] );
    }
    codhuff_write_bin_val( encoding_table[256] );
    /* Code of the end of encoding */
    codhuff_fill_encoding();
    /* Fills the last byte before closing file, if any */
  }
}


void _Pragma( "entrypoint" ) codhuff_main( void )
{
  codhuff_huffmanencoding( codhuff_input, 600 );
}


int codhuff_return( void )
{
  return codhuff_checksum0 ^ codhuff_checksum1;
}


int main( void )
{
  codhuff_main();

  return ( codhuff_return() );
}
