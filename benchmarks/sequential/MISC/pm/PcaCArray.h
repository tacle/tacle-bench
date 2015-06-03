/******************************************************************************
** File: PcaCArray.h
**
** HPEC Challenge Benchmark Suite
** Common Header File
**
** Contents:
**    Definition and methods of the Generic C Array package.
** Description:
**    A PcaArray struct encapsulates a multi-dimensional array of data (up 
**    to three dimensions). This structure is used solely as a standard 
**    means to interface the I/O between Matlab and the PCA C kernels. It 
**    supports data type including int and float. Complex data are
**    stored in an interleaved manner in memory (i.e. r0,i0,r1,i1,...). For
**    details, please see the comment above each macro.
**
** Author: Hector Chan
**         MIT Lincoln Laboratory
**
******************************************************************************/
/******************************************************************************
** MODIFIED BY: Rathijit Sen                                                          
**         		Universität des Saarlandes											 
**         		Saarbrücken															 
**                                                                               
** MODIFIED ON: 18 OCTOBER 2007                                                   		
**                                                                               
** COMMENTS:    removed dynamic alloc, file I/O
******************************************************************************/

#ifndef PCA_CARRAY_H
#define PCA_CARRAY_H

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <time.h>

#define NULL 0

/* Constants for identifying REAL/COMPLEX data */
#define PCA_REAL    1
#define PCA_COMPLEX 2

/* Boolean true/false */
#define PCA_FALSE 0
#define PCA_TRUE  1

#define LOG10 2.302585093f
#define MIN_NOISE 1e-10f

/* 32-bit unsigned integer type */
/* This typedef ensures the kernels/pregenerated data works independent of the 
   32-bit architecture where the kernels/data are generated and tested. If
   unsigned int does not equals to four bytes, please modify the following line
   and supply a data type that is four bytes. */
typedef unsigned int uint32;
typedef unsigned char uchar;
typedef unsigned int  uint;

/* For internal use */
union FourByteWord {
  unsigned char s[4];
  uint32 l;
};

/*************************************************/
/* Structures for Complex int and float */
typedef struct ComplexInt {
  int r, i;
} ComplexInt;

typedef struct ComplexFloat {
  float r, i;
} ComplexFloat;

/*************************************************/
/* C Array structure for int and float */
typedef struct PcaCArrayInt {
  int               *data;                  /* pointer to the start of the memory block */
  void              *datav;                 /* void pointer to a multi-dimensional array */
  unsigned int       size[3];               /* size of each dimension */
  unsigned int       ndims;                 /* number of dimensions for this array */
  unsigned int       rctype;                /* data is real/complex - PCA_REAL/PCA_COMPLEX */
} PcaCArrayInt;

typedef struct PcaCArrayFloat {
  float               *data;
  void                *datav;
  unsigned int         size[3];
  unsigned int         ndims;
  unsigned int         rctype;
} PcaCArrayFloat;

/*************************************************/
typedef struct {
  float *template_profiles_db; /* the library of patterns */
  float *test_profile_db;      /* the test pattern */

  float *template_copy;        /* temporary storage for a template */
  float *test_noise_db_array;  /* copies of test noise in an array for 
                                  fast copy */
  float *MSE_scores;           /* the likelihood of the matching between a 
                                  range shift of the test pattern and the libary */
  float *mag_shift_scores;     /* the likelihood of the matching between a 
                                  magnitude scaling of the test pattern and the libary */
  float *minimum_MSE_score;    /* the likelihood of the matching between the
                                  test pattern and the libary */
  float *all_shifted_test_db;  /* contains the shiftings of the test pattern */

  uchar *template_exceed;      /* marking where a library template 
                                  exceeds twice the noise level of the test pattern */
  float *test_exceed_means;    /* pixels where test pattern exceeds twice 
                                  its noise level */

  float shift_ratio;           /* determines the number of range shifts */
  int   shift_size;            /* the actual number of range shifts */
  int   profile_size;          /* the length of the pattern */
  int   num_templates;         /* the number of library templates */
  int   elsize;                /* the size of a single fp number */
} PmData;

/*************************************************/
/* swap_endian(type, relt, elt)
 *  - type can be int or float
 *  - relt is the returned data that has the correct endianness 
 *  - elt is the piece of data that requires swapping its endianness
 *
 *  Note: This macro swaps the endianness of data, 
 *        and it is meant strictly for internal use.
 */
#define swap_endian(type, relt, elt) \
{ union { \
     unsigned char s[sizeof(type)]; \
     type          t; \
   } val, uelt; \
   unsigned int x; \
   \
   uelt.t = elt; \
   for (x=0; x<sizeof(type); x++) \
     val.s[x] = uelt.s[sizeof(type)-1-x]; \
   \
   relt = val.t; }

#endif
/* ----------------------------------------------------------------------------
Copyright (c) 2006, Massachusetts Institute of Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are  
met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the Massachusetts Institute of Technology nor  
       the names of its contributors may be used to endorse or promote 
       products derived from this software without specific prior written 
       permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF  
THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------- */
