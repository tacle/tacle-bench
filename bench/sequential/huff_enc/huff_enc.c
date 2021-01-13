/*

  This program is part of the TACLeBench benchmark suite.
  Version V 2.0

  Name: huff_enc

  Author: David Bourgin (David.Bourgin@ufrima.imag.fr)

  Function: Example of Huffman encoding

  Source: ftp://turing.imag.fr/pub/compression/ (1994-09-22)

  Original name: codhuff.c

  Changes: I/O to char arrays instead of file i/o.
           Dynamic memory allocation replaced by array.
           Explicit sorting algorithm.

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
typedef struct huff_enc_s_tree {
  unsigned int byte; /* A byte has to be coded as an unsigned integer to
                        allow a node to have a value over 255 */
  unsigned long int weight;
  struct huff_enc_s_tree *left_ptr;
  struct huff_enc_s_tree *right_ptr;
} huff_enc_t_tree;

typedef struct {
  unsigned char bits[ 32 ];
  unsigned int bits_nb;
} huff_enc_t_bin_val;


/*
  Forward declaration of functions
*/

void huff_enc_init( void );
int huff_enc_return( void );
void huff_enc_beginning_of_data();
int huff_enc_end_of_data();
int huff_enc_read_byte();
void huff_enc_write_byte( char ch );
void huff_enc_write_bin_val( huff_enc_t_bin_val bin_val );
void huff_enc_fill_encoding( void );
void huff_enc_write_header( huff_enc_t_bin_val codes_table[ 257 ] );
int huff_enc_weighhuff_enc_t_tree_comp( const void *t1, const void *t2 );
void huff_enc_swapi( char *ii, char *ij, unsigned long es );
char *huff_enc_pivot( char *a, unsigned long n, unsigned long es );
void huff_enc_qsort( char *a, unsigned long n, unsigned long es );
huff_enc_t_tree *huff_enc_build_tree_encoding( huff_enc_t_tree heap[ 514 ] );
void huff_enc_encode_codes_table( huff_enc_t_tree *tree,
                                  huff_enc_t_bin_val codes_table[ 257 ], huff_enc_t_bin_val *code_val );
void huff_enc_create_codes_table( huff_enc_t_tree *tree,
                                  huff_enc_t_bin_val codes_table[ 257 ] );
void huff_enc_main();
int main( void );


/*
  Declaration of global variables
*/

static int huff_enc_input_pos;
static int huff_enc_output_pos;
static unsigned char huff_enc_output[ 1024 ];
static unsigned char huff_enc_byte_nb_to_write = 0;
static unsigned char huff_enc_val_to_write = 0;


/*
  Initialization- and return-value-related functions
*/

#define huff_enc_plaintext_len 600
static const char *huff_enc_plaintext =
  "You are doubtless asking \"How can I reduce the data size without losing "
  "some informations?\". It's easy to answer to this question. I'll only take "
  "an example. I'm sure you have heard about the morse. This system established "
  "in the 19th century use a scheme very close to the huffman one. In the morse "
  "you encode the letters to transmit with two kinds of signs. If you encode "
  "these two sign possibilities in one bit, the symbol 'e' is transmitted in a "
  "single bit and the symbols 'y' and 'z' need four bits. Look at the symbols "
  "in the text you are reading, you'll fast understand the compression ratio...";

#define huff_enc_encoded_len 419
static unsigned char huff_enc_encoded[ huff_enc_encoded_len ] = {
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


void huff_enc_init( void )
{
  huff_enc_input_pos = 0;
  huff_enc_output_pos = 0;
}


int huff_enc_return( void )
{
  int i;
  _Pragma( "loopbound min 419 max 419" )
  for ( i = 0; i < huff_enc_encoded_len; i++ ) {
    if ( huff_enc_encoded[ i ] != huff_enc_output[ i ] ) return i + 1;
  }
  return 0;
}


/*
  Input / output functions
*/

void huff_enc_beginning_of_data()
{
  huff_enc_input_pos = 0;
}


int huff_enc_end_of_data()
{
  return huff_enc_input_pos >= huff_enc_plaintext_len;
}


int huff_enc_read_byte()
{
  return huff_enc_plaintext[ huff_enc_input_pos++ ];
}


void huff_enc_write_byte( char ch )
{
  huff_enc_output[ huff_enc_output_pos++ ] = ch;
}


void huff_enc_write_bin_val( huff_enc_t_bin_val bin_val )
/* Returned parameters: None
   Action: Writes in the output stream the value binary-coded into 'bin_val'
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned char bit_indice;
  unsigned char bin_pos = ( bin_val.bits_nb - 1 ) & 7;
  unsigned int pos_byte = ( bin_val.bits_nb - 1 ) >> 3;

  _Pragma( "loopbound min 1 max 9" )
  for ( bit_indice = 1;                                                     
        bit_indice <= bin_val.bits_nb;
        bit_indice++ ) {
    /* Watch for the current bit to write */
    huff_enc_val_to_write = ( huff_enc_val_to_write << 1 ) |
                            ( ( bin_val.bits[ pos_byte ] >> bin_pos ) & 1 );
    /* Move to the next bit to write */
    if ( !bin_pos ) {
      pos_byte--;
      bin_pos = 7;
    } else bin_pos--;
    if ( huff_enc_byte_nb_to_write == 7 ) {
      /* Are already 8 bits written? */
      huff_enc_write_byte( huff_enc_val_to_write );
      huff_enc_byte_nb_to_write = 0;
      huff_enc_val_to_write = 0;
    } else /* No, then the next writting will be in the next bit */
      huff_enc_byte_nb_to_write++;
  }
}


void huff_enc_fill_encoding( void )
/* Returned parameters: None
   Action: Fills the last byte to write in the output stream with zero values
   Errors: An input/output error could disturb the running of the program
*/
{
  if ( huff_enc_byte_nb_to_write )
    huff_enc_write_byte( huff_enc_val_to_write <<
                         ( 8 - huff_enc_byte_nb_to_write ) );
}


void huff_enc_write_header( huff_enc_t_bin_val codes_table[ 257 ] )
/* Returned parameters: None
   Action: Writes the header in the stream of codes
   Errors: An input/output error could disturb the running of the program
*/
{
  unsigned int i, j;
  huff_enc_t_bin_val bin_val_to_0;
  huff_enc_t_bin_val bin_val_to_1;
  huff_enc_t_bin_val bin_val;
  /* Is used to send in binary mode via huff_enc_write_bin_val */

  *bin_val_to_0.bits = 0;
  bin_val_to_0.bits_nb = 1;
  *bin_val_to_1.bits = 1;
  bin_val_to_1.bits_nb = 1;
  _Pragma( "loopbound min 256 max 256" )
  for ( i = 0, j = 0; j <= 255; j++ )
    if ( codes_table[ j ].bits_nb ) i++;
  /* From there, i contains the number of bytes of the several
     non 0 occurrences to encode.
     First part of the header: Specifies the bytes that appear
     in the source of encoding */
  if ( i < 32 ) {
    /* Encoding of the appeared bytes with a block of bytes */
    huff_enc_write_bin_val( bin_val_to_0 );
    bin_val.bits_nb = 5;
    *bin_val.bits = ( unsigned char )( i - 1 );
    huff_enc_write_bin_val( bin_val );
    bin_val.bits_nb = 8;
    _Pragma( "loopbound min 256 max 256" )
    for ( j = 0; j <= 255; j++ )
      if ( codes_table[ j ].bits_nb ) {
        *bin_val.bits = ( unsigned char )j;
        huff_enc_write_bin_val( bin_val );
      }
  } else {
    /* Encoding of the appeared bytes with a block of bits */
    huff_enc_write_bin_val( bin_val_to_1 );
    _Pragma( "loopbound min 256 max 256" )
    for ( j = 0; j <= 255; j++ )
      if ( codes_table[ j ].bits_nb )
        huff_enc_write_bin_val( bin_val_to_1 );
      else huff_enc_write_bin_val( bin_val_to_0 );
  };
  /* Second part of the header: Specifies the encoding of the bytes
     (fictive or not) that appear in the source of encoding */
  _Pragma( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ )
    if ( ( j = codes_table[ i ].bits_nb ) != 0 ) {
      if ( j < 33 ) {
        huff_enc_write_bin_val( bin_val_to_0 );
        bin_val.bits_nb = 5;
      } else {
        huff_enc_write_bin_val( bin_val_to_1 );
        bin_val.bits_nb = 8;
      }
      *bin_val.bits = ( unsigned char )( j - 1 );
      huff_enc_write_bin_val( bin_val );
      huff_enc_write_bin_val( codes_table[ i ] );
    }
}


int huff_enc_weighhuff_enc_t_tree_comp( const void *t1, const void *t2 )
/* Returned parameters: Returns a comparison status
   Action: Returns a negative, zero or positive integer depending on the weight
           of 'tree2' is less than, equal to, or greater than the weight of
           'tree1'
   Errors: None
*/
{
  huff_enc_t_tree *const *tree1 = ( huff_enc_t_tree * const * ) t1;
  huff_enc_t_tree *const *tree2 = ( huff_enc_t_tree * const * ) t2;
  return ( ( *tree2 )->weight ^ ( *tree1 )->weight )
         ? ( ( ( *tree2 )->weight < ( *tree1 )->weight ) ? -1 : 1 ) : 0;
}


void huff_enc_swapi( char *ii, char *ij, unsigned long es )
{
  char *i, *j, c;

  i = ( char * )ii;
  j = ( char * )ij;
  _Pragma( "loopbound min 8 max 8" )
  do {
    c = *i;
    *i++ = *j;
    *j++ = c;
    es -= sizeof( char );
  } while ( es != 0 );
}


char *huff_enc_pivot( char *a, unsigned long n, unsigned long es )
{
  long j;
  char *pi, *pj, *pk;

  j = n / 6 * es;
  pi = a + j;     /* 1/6 */
  j += j;
  pj = pi + j;    /* 1/2 */
  pk = pj + j;    /* 5/6 */
  if ( huff_enc_weighhuff_enc_t_tree_comp( pi, pj ) < 0 ) {
    if ( huff_enc_weighhuff_enc_t_tree_comp( pi, pk ) < 0 ) {
      if ( huff_enc_weighhuff_enc_t_tree_comp( pj, pk ) < 0 )
        return pj;
      return pk;
    }
    return pi;
  }
  if ( huff_enc_weighhuff_enc_t_tree_comp( pj, pk ) < 0 ) {
    if ( huff_enc_weighhuff_enc_t_tree_comp( pi, pk ) < 0 )
      return pi;
    return pk;
  }
  return pj;
}


void huff_enc_qsort( char *a, unsigned long n, unsigned long es )       //wird insgesamt 648 ausgeführt (rekursion)
{
  unsigned long j;
  char *pi, *pj, *pn;
  unsigned int flowfactdummy = 0;
  _Pragma( "loopbound min 0 max 7" )
  while ( n > 1 ) {
    if ( n > 10 )
      pi = huff_enc_pivot( a, n, es );

    else
      pi = a + ( n >> 1 ) * es;

    huff_enc_swapi( a, pi, es );
    pi = a;
    pn = a + n * es;
    pj = pn;
    _Pragma( "loopbound min 0 max 109" )
    while ( 1 ) {
      /* wcc note: this assignment expression was added to avoid assignment of
         multiple loop bound annotations to same loop (cf. Ticket #0002323). */
      flowfactdummy++;
      _Pragma( "loopbound min 1 max 19" )
      do {
        pi += es;
      } while ( pi < pn && huff_enc_weighhuff_enc_t_tree_comp( pi, a ) < 0 );
      _Pragma( "loopbound min 1 max 25" )
      do {
        pj -= es;
      } while ( pj > a && huff_enc_weighhuff_enc_t_tree_comp( pj, a ) > 0 );
      if ( pj < pi )
        break;
      huff_enc_swapi( pi, pj, es );
    }
    huff_enc_swapi( a, pj, es );
    j = ( pj - a ) / es;

    n = n - j - 1;
    if ( j >= n ) {
      huff_enc_qsort( a, j, es );
      a += ( j + 1 ) * es;
    } else {
      huff_enc_qsort( a + ( j + 1 )*es, n, es );
      n = j;
    }
  }
}


huff_enc_t_tree *huff_enc_build_tree_encoding( huff_enc_t_tree heap[ 514 ] )
/* Returned parameters: Returns a tree of encoding
   Action: Generates an Huffman encoding tree based on the data from
           the stream to compress
   Errors: None
*/
{
  unsigned int i;
  unsigned int heap_top = 0;
  huff_enc_t_tree *occurrences_table[ 257 ];
  huff_enc_t_tree *ptr_fictive_tree;

  /* Sets up the occurrences number of all bytes to 0 */
  _Pragma( "loopbound min 257 max 257" )
  for ( i = 0; i <= 256; i++ ) {
    occurrences_table[ i ] = &heap[ heap_top++ ];
    occurrences_table[ i ]->byte = i;
    occurrences_table[ i ]->weight = 0;
    occurrences_table[ i ]->left_ptr = 0;
    occurrences_table[ i ]->right_ptr = 0;
  }
  /* Valids the occurrences of 'occurrences_table' with regard to the data to
     compress */
  if ( !huff_enc_end_of_data() ) {
    _Pragma( "loopbound min 600 max 600" )
    while ( !huff_enc_end_of_data() ) {
      i = huff_enc_read_byte();
      occurrences_table[ i ]->weight++;
    }
    occurrences_table[ 256 ]->weight = 1;

    /* Sorts the occurrences table depending on the weight of each character */
    huff_enc_qsort( ( char * )occurrences_table, 257, sizeof( huff_enc_t_tree * ) );

    _Pragma( "loopbound min 218 max 218" )
    for ( i = 256; ( i != 0 ) && ( !occurrences_table[ i ]->weight ); i-- )
      ; 
    i++;
    /* From there, 'i' gives the number of different bytes with a 0 occurrence
       in the stream to compress */
    _Pragma( "loopbound min 38 max 38" )
    while ( i > 0 ) {
      /* Looks up (i+1)/2 times the occurrence table to link the nodes in an
         unique tree */
      ptr_fictive_tree = &heap[ heap_top++ ];
      ptr_fictive_tree->byte = 257;
      ptr_fictive_tree->weight = occurrences_table[ --i ]->weight;
      ptr_fictive_tree->left_ptr = occurrences_table[ i ];
      if ( i ) {
        i--;
        ptr_fictive_tree->weight += occurrences_table[ i ]->weight;
        ptr_fictive_tree->right_ptr = occurrences_table[ i ];
      } else ptr_fictive_tree->right_ptr = 0;
      occurrences_table[ i ] = ptr_fictive_tree;

      //qsort( ( char * )occurrences_table, i + 1, sizeof( *huff_enc_t_tree ),
      //huff_enc_weighhuff_enc_t_tree_comp );
      huff_enc_qsort( ( char * )occurrences_table, i + 1,
                      sizeof( huff_enc_t_tree * ) );

      if ( i )      /* Is there an other node in the occurrence tables? */
        i++;       /* Yes, then takes care to the fictive node */
    }
  }
  return ( *occurrences_table );
}


void huff_enc_encode_codes_table( huff_enc_t_tree *tree,
                                  huff_enc_t_bin_val codes_table[ 257 ],
                                  huff_enc_t_bin_val *code_val )
/* Returned parameters: The data of 'codes_table' can have been modified
   Action: Stores the encoding tree as a binary encoding table to speed up the
          access. 'val_code' gives the encoding for the current node of the tree
   Errors: None
*/
{
  unsigned int i;
  huff_enc_t_bin_val tmp_code_val;

  if ( tree->byte == 257 ) {
    if ( tree->left_ptr != 0 )
      /* The sub-trees on left begin with an bit set to 1 */
    {
      tmp_code_val = *code_val;
      _Pragma( "loopbound min 31 max 31" )
      for ( i = 31; i > 0; i-- )
        code_val->bits[ i ] = ( code_val->bits[ i ] << 1 ) |
                            ( code_val->bits[ i - 1 ] >> 7 );
      *code_val->bits = ( *code_val->bits << 1 ) | 1;
      code_val->bits_nb++;
      huff_enc_encode_codes_table( tree->left_ptr, codes_table, code_val );
      *code_val = tmp_code_val;
    };
    if ( tree->right_ptr != 0 )
      /* The sub-trees on right begin with an bit set to 0 */
    {
      tmp_code_val = *code_val;
      _Pragma( "loopbound min 31 max 31" )
      for ( i = 31; i > 0; i-- )
        code_val->bits[ i ] = ( code_val->bits[ i ] << 1 ) |
                            ( code_val->bits[ i - 1 ] >> 7 );
      *code_val->bits <<= 1;
      code_val->bits_nb++;
      huff_enc_encode_codes_table( tree->right_ptr, codes_table, code_val );
      *code_val = tmp_code_val;
    };
  } else codes_table[ tree->byte ] = *code_val;
}


void huff_enc_create_codes_table( huff_enc_t_tree *tree,
                                  huff_enc_t_bin_val codes_table[ 257 ] )
/* Returned parameters: The data in 'codes_table' will be modified
   Action: Stores the encoding tree as a binary encoding table to speed up
           the access by calling encode_codes_table
   Errors: None
*/
{
  unsigned int i, j;
  huff_enc_t_bin_val code_val;

  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < 32; i++ )
    code_val.bits[ i ] = 0;
  code_val.bits_nb = 0;
  _Pragma( "loopbound min 257 max 257" )
  for ( j = 0; j < 257; j++ ) {
    _Pragma( "loopbound min 32 max 32" )
    for ( i = 0; i < 32; i++ )
      codes_table[ j ].bits[ i ] = 0;
    codes_table[ j ].bits_nb = 0;
  }
  _Pragma( "marker call_encode" )
  _Pragma( "flowrestriction 1*huff_enc_encode_codes_table <= 77*call_encode" )
  huff_enc_encode_codes_table( tree, codes_table, &code_val );
}


void _Pragma( "entrypoint" ) huff_enc_main()
/* Returned parameters: None
   Action: Compresses with Huffman method all bytes read by the function
           'huff_enc_read_byte'
   Errors: None
*/
{
  huff_enc_t_tree *tree;
  huff_enc_t_tree heap[ 514 ];
  huff_enc_t_bin_val encoding_table[ 257 ];
  unsigned char byte_read;

  if ( !huff_enc_end_of_data() ) {
    /* Generates only whether there are data */
    tree = huff_enc_build_tree_encoding( heap );
    /* Creation of the best adapted tree */
    huff_enc_create_codes_table( tree, encoding_table );
    /* Obtains the binary encoding in an array to speed up the accesses */
    huff_enc_write_header( encoding_table );
    /* Writes the defintion of the encoding */
    huff_enc_beginning_of_data();  /* Real compression of the data */
    _Pragma( "loopbound min 600 max 600" )
    while ( !huff_enc_end_of_data() ) {
      byte_read = huff_enc_read_byte();
      huff_enc_write_bin_val( encoding_table[ byte_read ] );
    }
    huff_enc_write_bin_val( encoding_table[ 256 ] );
    /* Code of the end of encoding */
    huff_enc_fill_encoding();
    /* Fills the last byte before closing file, if any */
  }
}


int main( void )
{
  huff_enc_init();
  huff_enc_main();
  return ( huff_enc_return() );
}
