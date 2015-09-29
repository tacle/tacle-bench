#ifndef __DCODHUFF_H
#define __DCODHUFF_H

/* Error codes returned to the caller */
#define NO_ERROR      0
#define BAD_FILE_NAME 1
#define BAD_ARGUMENT  2
#define BAD_MEM_ALLOC 3

/*  Useful constants */
#define FALSE 0
#define TRUE  1
#define NULL  0

typedef struct s_tree { unsigned int byte;
                             /* A byte has to be coded as an unsigned integer to allow a node to have a value over 255 */
                         struct s_tree *left_ptr,
                                       *right_ptr;
                       } t_tree,*p_tree;
#define TREE_BYTE(ptr_tree)  ((*(ptr_tree)).byte)
#define LEFTPTR_OF_TREE(ptr_tree)  ((*(ptr_tree)).left_ptr)
#define RIGHTPTR_OF_TREE(ptr_tree)  ((*(ptr_tree)).right_ptr)

typedef struct { unsigned char bits[32];
                 unsigned int bits_nb;
                 unsigned char presence;
               } t_bin_val;
#define BITS_BIN_VAL(x)  ((x).bits)
#define NB_BITS_BIN_VAL(x)  ((x).bits_nb)
#define PRESENCE_BIN_VAL(x)  ((x).presence)

#endif
