/**********************************************************************\

  SUSAN Version 2l by Stephen Smith
  Oxford Centre for Functional Magnetic Resonance Imaging of the Brain,
  Department of Clinical Neurology, Oxford University, Oxford, UK
  (Previously in Computer Vision and Image Processing Group - now
  Computer Vision and Electro Optics Group - DERA Chertsey, UK)
  Email:    steve@fmrib.ox.ac.uk
  WWW:      http://www.fmrib.ox.ac.uk/~steve

  (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
  Farnborough, Hampshire, GU14 6TD, UK
  DERA WWW site:
  http://www.dera.gov.uk/
  DERA Computer Vision and Electro Optics Group WWW site:
  http://www.dera.gov.uk/imageprocessing/dera/group_home.html
  DERA Computer Vision and Electro Optics Group point of contact:
  Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203

  A UK patent has been granted: "Method for digitally processing
  images to determine the position of edges and/or corners therein for
  guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
  Secretary of State for Defence, UK. 15 January 1997

  This code is issued for research purposes only and remains the
  property of the UK Secretary of State for Defence. This code must
  not be passed on without this header information being kept
  intact. This code must not be sold.

  \**********************************************************************/

/**********************************************************************\

  SUSAN Version 2l
  SUSAN = Smallest Univalue Segment Assimilating Nucleus

  Email:    steve@fmrib.ox.ac.uk
  WWW:      http://www.fmrib.ox.ac.uk/~steve

  Related paper:
  @article{Smith97,
        author = "Smith, S.M. and Brady, J.M.",
        title = "{SUSAN} - A New Approach to Low Level Image Processing",
        journal = "Int. Journal of Computer Vision",
        pages = "45--78",
        volume = "23",
        number = "1",
        month = "May",
        year = 1997}

  To be registered for automatic (bug) updates of SUSAN, send an email.

  Compile with:
  gcc -O4 -o susan susan2l.c -lm

  See following section for different machine information. Please
  report any bugs (and fixes). There are a few optional changes that
  can be made in the "defines" section which follows shortly.

  Usage: type "susan" to get usage. Only PGM format files can be input
  and output. Utilities such as the netpbm package and XV can be used
  to convert to and from other formats. Any size of image can be
  processed.

  This code is written using an emacs folding mode, making moving
  around the different sections very easy. This is why there are
  various marks within comments and why comments are indented.


  SUSAN QUICK:

  This version of the SUSAN corner finder does not do all the
  false-corner suppression and thus is faster and produced some false
  positives, particularly on strong edges. However, because there are
  less stages involving thresholds etc., the corners that are
  correctly reported are usually more stable than those reported with
  the full algorithm. Thus I recommend at least TRYING this algorithm
  for applications where stability is important, e.g., tracking.

  THRESHOLDS:

  There are two thresholds which can be set at run-time. These are the
  brightness threshold (t) and the distance threshold (d).

  SPATIAL CONTROL: d

  In SUSAN smoothing d controls the size of the Gaussian mask; its
  default is 4.0. Increasing d gives more smoothing. In edge finding,
  a fixed flat mask is used, either 37 pixels arranged in a "circle"
  (default), or a 3 by 3 mask which gives finer detail. In corner
  finding, only the larger 37 pixel mask is used; d is not
  variable. In smoothing, the flat 3 by 3 mask can be used instead of
  a larger Gaussian mask; this gives low smoothing and fast operation.

  BRIGHTNESS CONTROL: t

  In all three algorithms, t can be varied (default=20); this is the
  main threshold to be varied. It determines the maximum difference in
  greylevels between two pixels which allows them to be considered
  part of the same "region" in the image. Thus it can be reduced to
  give more edges or corners, i.e. to be more sensitive, and vice
  versa. In smoothing, reducing t gives less smoothing, and vice
  versa. Set t=10 for the test image available from the SUSAN web
  page.

  ITERATIONS:

  With SUSAN smoothing, more smoothing can also be obtained by
  iterating the algorithm several times. This has a different effect
  from varying d or t.

  FIXED MASKS:

  37 pixel mask:    ooo       3 by 3 mask:  ooo
                   ooooo                    ooo
                  ooooooo                   ooo
                  ooooooo
                  ooooooo
                   ooooo
                    ooo

  CORNER ATTRIBUTES dx, dy and I
  (Only read this if you are interested in the C implementation or in
  using corner attributes, e.g., for corner matching)

  Corners reported in the corner list have attributes associated with
  them as well as positions. This is useful, for example, when
  attempting to match corners from one image to another, as these
  attributes can often be fairly unchanged between images. The
  attributes are dx, dy and I. I is the value of image brightness at
  the position of the corner. In the case of susan_corners_quick, dx
  and dy are the first order derivatives (differentials) of the image
  brightness in the x and y directions respectively, at the position
  of the corner. In the case of normal susan corner finding, dx and dy
  are scaled versions of the position of the centre of gravity of the
  USAN with respect to the centre pixel (nucleus).

  BRIGHTNESS FUNCTION LUT IMPLEMENTATION:
  (Only read this if you are interested in the C implementation)

  The SUSAN brightness function is implemented as a LUT
  (Look-Up-Table) for speed. The resulting pointer-based code is a
  little hard to follow, so here is a brief explanation. In
  setup_brightness_lut() the LUT is setup. This mallocs enough space
  for *bp and then repositions the pointer to the centre of the
  malloced space. The SUSAN function e^-(x^6) or e^-(x^2) is
  calculated and converted to a uchar in the range 0-100, for all
  possible image brightness differences (including negative
  ones). Thus bp[ 23 ] is the output for a brightness difference of 23
  greylevels. In the SUSAN algorithms this LUT is used as follows:

  p=in + (i-3)*x_size + j - 1;
  p points to the first image pixel in the circular mask surrounding
  point (x,y).

  cp=bp + in[ i*x_size+j ];
  cp points to a position in the LUT corresponding to the brightness
  of the centre pixel (x,y).

  now for every pixel within the mask surrounding (x,y),
  n+=*(cp-*p++);
  the brightness difference function is found by moving the cp pointer
  down by an amount equal to the value of the pixel pointed to by p,
  thus subtracting the two brightness values and performing the
  exponential function. This value is added to n, the running USAN
  area.

  in SUSAN smoothing, the variable height mask is implemented by
  multiplying the above by the moving mask pointer, reset for each new
  centre pixel.
  tmp = *dpt++ * *(cp-brightness);

  \**********************************************************************/

/**********************************************************************\

  Success has been reported with the following:

  MACHINE  OS         COMPILER

  Sun      4.1.4      bundled C, gcc

  Next

  SGI      IRIX       SGI cc

  DEC      Unix V3.2+

  IBM RISC AIX        gcc

  PC                  Borland 5.0

  PC       Linux      gcc-2.6.3

  PC       Win32      Visual C++ 4.0 (Console Application)

  PC       Win95      Visual C++ 5.0 (Console Application)
                      Thanks to Niu Yongsheng <niuysbit@163.net>:
                      Use the FOPENB option below

  PC       DOS        djgpp gnu C
                      Thanks to Mark Pettovello <mpettove@umdsun2.umd.umich.edu>:
                      Use the FOPENB option below

  HP       HP-UX      bundled cc
                      Thanks to Brian Dixon <briand@hpcvsgen.cv.hp.com>:
                      in ksh:
                      export CCOPTS="-Aa -D_HPUX_SOURCE | -lM"
                      cc -O3 -o susan susan2l.c

  \**********************************************************************/

/**********************************************************************\

  SUSAN Version 2l, 12/2/99
  Changed GNUDOS option to FOPENB.
  (Thanks to Niu Yongsheng <niuysbit@163.net>.)
  Took out redundant "sq=sq/2;".

  SUSAN Version 2k, 19/8/98:
  In corner finding:
  Changed if(yy<sq) {...} else if(xx<sq) {...} to
          if(yy<xx) {...} else {...}
  (Thanks to adq@cim.mcgill.edu - Alain Domercq.)

  SUSAN Version 2j, 22/10/97:
  Fixed (mask_size>x_size) etc. tests in smoothing.
  Added a couple of free() calls for cgx and cgy.
  (Thanks to geoffb@ucs.ed.ac.uk - Geoff Browitt.)

  SUSAN Version 2i, 21/7/97:
  Added information about corner attributes.

  SUSAN Version 2h, 16/12/96:
  Added principle (initial enhancement) option.

  SUSAN Version 2g, 2/7/96:
  Minor superficial changes to code.

  SUSAN Version 2f, 16/1/96:
  Added GNUDOS option (now called FOPENB; see options below).

  SUSAN Version 2e, 9/1/96:
  Added -b option.
  Fixed 1 pixel horizontal offset error for drawing edges.

  SUSAN Version 2d, 27/11/95:
  Fixed loading of certain PGM files in get_image (again!)

  SUSAN Version 2c, 22/11/95:
  Fixed loading of certain PGM files in get_image.
  (Thanks to qu@San-Jose.ate.slb.com - Gongyuan Qu.)

  SUSAN Version 2b, 9/11/95:
  removed "z==" error in edges routines.

  SUSAN Version 2a, 6/11/95:
  Removed a few unnecessary variable declarations.
  Added different machine information.
  Changed "header" in get_image to char.

  SUSAN Version 2, 1/11/95: first combined version able to take any
  image sizes.

  SUSAN "Versions 1", circa 1992: the various SUSAN algorithms were
  developed during my doctorate within different programs and for
  fixed image sizes. The algorithms themselves are virtually unaltered
  between "versions 1" and the combined program, version 2.

  \**********************************************************************/

#include "wcclibm.h"
#include "wccfile.h"
#include "wccmalloc.h"
#define EXP_A 184
#define EXP_C 16249

float susan_expf( float y )
{
  union {
    float d;
    struct {
      #ifdef LITTLE_ENDIAN
      short j, i;
      #else
      short i, j;
      #endif
    } n;
  } eco;
  eco.n.i = EXP_A * ( y ) + ( EXP_C );
  eco.n.j = 0;
  return eco.d;
}

float susan_sqrtf( float val )
{
  float x = val / 10;

  float dx;

  float diff;
  float min_tol = 0.00001f;

  int i, flag;


  flag = 0;
  if ( val == 0 ) x = 0;
  else {
    _Pragma( "loopbound min 19 max 19" )  
    for ( i = 1; i < 20; i++ ) {
      if ( !flag ) {
        dx = ( val - ( x * x ) ) / ( 2.0f * x );
        x = x + dx;
        diff = val - ( x * x );
        if ( fabs( diff ) <= min_tol ) flag = 1;
      }
    }
  }
  return ( x );
}

/* ********** Optional settings */

typedef int        TOTAL_TYPE;

#define SEVEN_SUPP           /* size for non-max corner suppression; SEVEN_SUPP or FIVE_SUPP */
#define MAX_CORNERS   15000  /* max corners per frame */

#define  FTOI(a) ( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )
#define  abs(a)  ( (a) < 0 ? -a : a )
typedef  unsigned char uchar;
typedef  struct {
  int x, y, info, dx, dy, I;
} corner_rep;
typedef  corner_rep CORNER_LIST[ MAX_CORNERS ];

extern char susan_input[ 7292 ];
struct wccFILE susan_file;
float susan_dt;
int susan_bt;
int susan_principle_conf;
int susan_thin_post_proc;
int susan_three_by_three;
int susan_drawing_mode;
int susan_susan_quick;
int susan_max_no_corners;
int susan_max_no_edges;

int susan_getint( struct wccFILE *fd )
{
  int c, i;
  char dummy[ 10000 ];

  c = susan_wccfgetc( fd );
  _Pragma( "loopbound min 0 max 3" )
  while ( 1 ) { /* find next integer */
    if ( c == '#' ) /* if we're at a comment, read to end of line */
      susan_wccfgets( dummy, 9000, fd );
    if ( c == EOF ) {
      /* "Image is not binary PGM." */
    }
    if ( c >= '0' && c <= '9' )
      break;   /* found what we were looking for */
    c = susan_wccfgetc( fd );
  }

  /* we're at the start of a number, continue until we hit a non-number */
  i = 0;
  _Pragma( "loopbound min 1 max 2" )
  while ( 1 ) {
    i = ( i * 10 ) + ( c - '0' );
    c = susan_wccfgetc( fd );
    if ( c == EOF ) return ( i );
    if ( c < '0' || c > '9' ) break;
  }

  return ( i );
}


void susan_get_image( struct wccFILE *fd,
                      unsigned char **in, int *x_size, int *y_size )
{
  char header [ 100 ];

  susan_wccfseek( fd, 0, WCCSEEK_SET );

  /* {{{ read header */

  header[ 0 ] = susan_wccfgetc( fd );
  header[ 1 ] = susan_wccfgetc( fd );
  if ( !( header[ 0 ] == 'P' && header[ 1 ] == '5' ) ) {
    /* "Image does not have binary PGM header." */
  }

  *x_size = susan_getint( fd );
  *y_size = susan_getint( fd );
  // dummy read
  susan_getint( fd );

  /* }}} */

  *in = ( uchar * ) susan_wccmalloc( *x_size * *y_size );

  if ( susan_wccfread( *in, 1, *x_size * *y_size, fd ) == 0 ) {
    /* "Image is wrong size.\n" */
  }
}


void susan_put_image( int x_size, int y_size )
{
  int i;
  _Pragma( "loopbound min 7220 max 7220" )
  for ( i = 0; i < x_size * y_size; i++ )
    ;
}


void susan_int_to_uchar( char *r, uchar *in, int size )
{
  int i, max_r = r[ 0 ], min_r = r[ 0 ];

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0; i < size; i++ ) {
    if ( r[ i ] > max_r )
      max_r = r[ i ];
    if ( r[ i ] < min_r )
      min_r = r[ i ];
  }

  if ( max_r == min_r ) {
    /* Should not occur in TACLeBench. */
    _Pragma( "loopbound min 0 max 0" )
    for ( i = 0; i < size; i++ )
      in[ i ] = ( uchar )( 0 );

    return;
  }
  max_r -= min_r;

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 0; i < size; i++ )
    in[ i ] = ( uchar )( ( int )( ( int )( r[ i ] - min_r ) * 255 ) / max_r );
}


void susan_setup_brightness_lut( uchar **bp, int thresh, int form )
{
  int   k;
  float temp;

  *bp = ( unsigned char * )susan_wccmalloc( 516 );
  *bp = *bp + 258;

  _Pragma( "loopbound min 513 max 513" )
  for ( k = -256; k < 257; k++ ) {
    temp = ( ( float )k ) / ( ( float )thresh );
    temp = temp * temp;
    if ( form == 6 )
      temp = temp * temp * temp;
    temp = 100.0 * susan_expf( -temp );
    *( *bp + k ) = ( uchar )temp;
  }
}


void susan_principle( uchar *in, char *r, uchar *bp,
                      int max_no, int x_size, int y_size )
{
  int   i, j, n;
  uchar *p, *cp;

  susan_wccmemset( r, 0, x_size * y_size * sizeof( int ) );

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 3; i < y_size - 3; i++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( j = 3; j < x_size - 3; j++ ) {
      n = 100;
      p = in + ( i - 3 ) * x_size + j - 1;
      cp = bp + in[ i * x_size + j ];

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 3;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 5;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 6;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += 2;
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 6;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 5;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 3;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );

      if ( n <= max_no )
        r[ i * x_size + j ] = max_no - n;
    }
  }
}


void susan_principle_small( uchar *in, char *r, uchar *bp,
                            int max_no, int x_size, int y_size )
{
  int   i, j, n;
  uchar *p, *cp;

  susan_wccmemset( r, 0, x_size * y_size * sizeof( int ) );

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 1; i < y_size - 1; i++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( j = 1; j < x_size - 1; j++ ) {
      n = 100;
      p = in + ( i - 1 ) * x_size + j - 1;
      cp = bp + in[ i * x_size + j ];

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 2;

      n += *( cp - *p );
      p += 2;
      n += *( cp - *p );
      p += x_size - 2;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );

      if ( n <= max_no )
        r[ i * x_size + j ] = max_no - n;
    }
  }
}


uchar susan_median( uchar *in, int i, int j, int x_size )
{
  int p[ 8 ], k, l, tmp;

  p[ 0 ] = in[ ( i - 1 ) * x_size + j - 1 ];
  p[ 1 ] = in[ ( i - 1 ) * x_size + j   ];
  p[ 2 ] = in[ ( i - 1 ) * x_size + j + 1 ];
  p[ 3 ] = in[ ( i  ) * x_size + j - 1 ];
  p[ 4 ] = in[ ( i  ) * x_size + j + 1 ];
  p[ 5 ] = in[ ( i + 1 ) * x_size + j - 1 ];
  p[ 6 ] = in[ ( i + 1 ) * x_size + j   ];
  p[ 7 ] = in[ ( i + 1 ) * x_size + j + 1 ];

  _Pragma( "loopbound min 0 max 0" )
  for ( k = 0; k < 7; k++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( l = 0; l < ( 7 - k ); l++ ) {
      if ( p[ l ] > p[ l + 1 ] ) {
        tmp = p[ l ];
        p[ l ] = p[ l + 1 ];
        p[ l + 1 ] = tmp;
      }
    }
  }

  return ( ( p[ 3 ] + p[ 4 ] ) / 2 );
}


/* this enlarges "in" so that borders can be dealt with easily */
void susan_enlarge( uchar **in, uchar *tmp_image,
                    int *x_size, int *y_size, int border )
{
  int   i, j;

  _Pragma( "loopbound min 95 max 95" )
  for ( i = 0; i < *y_size; i++ ) { /* copy *in into tmp_image */
    susan_wccmemcpy( tmp_image + ( i + border ) * ( *x_size + 2 * border ) + border,
                     *in + i * *x_size, *x_size );
  }

  _Pragma( "loopbound min 7 max 7" )
  for ( i = 0; i < border;
        i++ ) { /* copy top and bottom rows; invert as many as necessary */
    susan_wccmemcpy( tmp_image + ( border - 1 - i ) * ( *x_size + 2 * border ) +
                     border,
                     *in + i * *x_size, *x_size );
    susan_wccmemcpy( tmp_image + ( *y_size + border + i ) *
                     ( *x_size + 2 * border ) + border,
                     *in + ( *y_size - i - 1 ) * *x_size, *x_size );
  }

  _Pragma( "loopbound min 7 max 7" )
  for ( i = 0; i < border; i++ ) { /* copy left and right columns */
    _Pragma( "loopbound min 109 max 109" )
    for ( j = 0; j < *y_size + 2 * border; j++ ) {
      tmp_image[ j * ( *x_size + 2 * border ) + border - 1 - i ] = tmp_image[ j *
          ( *x_size + 2 * border ) + border + i ];
      tmp_image[ j * ( *x_size + 2 * border ) + *x_size + border + i ] = tmp_image[ j *
          ( *x_size + 2 * border ) + *x_size + border - 1 - i ];
    }
  }

  *x_size += 2 * border; /* alter image size */
  *y_size += 2 * border;
  *in = tmp_image;    /* repoint in */
}


void susan_smoothing( int three_by_three, uchar *in, float dt,
                      int x_size, int y_size, uchar *bp )
{
  float temp;
  int   n_max, increment, mask_size,
        i, j, x, y, area, brightness, tmp, centre;
  uchar *ip, *dp, *dpt, *cp, *out = in,
                              *tmp_image;
  TOTAL_TYPE total;

  /* {{{ setup larger image and border sizes */

  if ( three_by_three == 0 )
    mask_size = ( ( int )( 1.5 * dt ) ) + 1;
  else
    mask_size = 1;

  if ( dt > 15 ) {
    /*  "Distance_thresh too big for integer arithmetic." */
    // Either reduce it to <=15 or recompile with variable "total"
    // as a float: see top "defines" section.
  }

  if ( ( 2 * mask_size + 1 > x_size ) || ( 2 * mask_size + 1 > y_size ) ) {
    /*  "Mask size too big for image." */
  }

  tmp_image = ( uchar * )susan_wccmalloc( ( x_size + mask_size * 2 ) *
                                          ( y_size + mask_size * 2 ) );
  susan_enlarge( &in, tmp_image, &x_size, &y_size, mask_size );
  if ( three_by_three == 0 ) {
    /* large Gaussian masks */
    /* {{{ setup distance lut */

    n_max = ( mask_size * 2 ) + 1;

    increment = x_size - n_max;

    dp     = ( unsigned char * )susan_wccmalloc( n_max * n_max );
    dpt    = dp;
    temp   = -( dt * dt );

    _Pragma( "loopbound min 15 max 15" )
    for ( i = -mask_size; i <= mask_size; i++ ) {
      _Pragma( "loopbound min 15 max 15" )
      for ( j = -mask_size; j <= mask_size; j++ ) {
        x = ( int ) ( 100.0 * susan_expf( ( ( float )( ( i * i ) +
                                            ( j * j ) ) ) / temp ) );
        *dpt++ = ( unsigned char )x;
      }
    }

    /* {{{ main section */
    _Pragma( "loopbound min 95 max 95" )
    for ( i = mask_size; i < y_size - mask_size; i++ ) {
      _Pragma( "loopbound min 76 max 76" )
      for ( j = mask_size; j < x_size - mask_size; j++ ) {
        area = 0;
        total = 0;
        dpt = dp;
        ip = in + ( ( i - mask_size ) * x_size ) + j - mask_size;
        centre = in[ i * x_size + j ];
        cp = bp + centre;
        _Pragma( "loopbound min 15 max 15" )
        for ( y = -mask_size; y <= mask_size; y++ ) {
          _Pragma( "loopbound min 15 max 15" )
          for ( x = -mask_size; x <= mask_size; x++ ) {
            brightness = *ip++;
            tmp = *dpt++ * *( cp - brightness );
            area += tmp;
            total += tmp * brightness;
          }
          ip += increment;
        }
        tmp = area - 10000;
        if ( tmp == 0 )
          *out++ = susan_median( in, i, j, x_size );
        else
          *out++ = ( ( total - ( centre * 10000 ) ) / tmp );
      }
    }
  } else {
    /* 3x3 constant mask */

    /* {{{ main section */
    _Pragma( "loopbound min 15 max 15" )            //neue Änderung min max 107
    for ( i = 1; i < y_size - 1; i++ ) {
      _Pragma( "loopbound min 15 max 15" )          //neue Änderung min max 88
      for ( j = 1; j < x_size - 1; j++ ) {
        area = 0;
        total = 0;
        ip = in + ( ( i - 1 ) * x_size ) + j - 1;
        centre = in[ i * x_size + j ];
        cp = bp + centre;

        brightness = *ip++;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        brightness = *ip++;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        brightness = *ip;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        ip += x_size - 2;
        brightness = *ip++;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        brightness = *ip++;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        brightness = *ip;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        ip += x_size - 2;
        brightness = *ip++;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        brightness = *ip++;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;
        brightness = *ip;
        tmp = *( cp - brightness );
        area += tmp;
        total += tmp * brightness;

        tmp = area - 100;
        if ( tmp == 0 )
          *out++ = susan_median( in, i, j, x_size );
        else
          *out++ = ( total - ( centre * 100 ) ) / tmp;
      }
    }
  }
}


void susan_edge_draw( uchar *in, uchar *mid,
                      int x_size, int y_size, int drawing_mode )
{
  int   i;
  uchar *inp, *midp;

  if ( drawing_mode == 0 ) {
    /* mark 3x3 white block around each edge point */
    midp = mid;
    _Pragma( "loopbound min 7220 max 7220" )
    for ( i = 0; i < x_size * y_size; i++ ) {
      if ( *midp < 8 ) {
        inp = in + ( midp - mid ) - x_size - 1;
        *inp++ = 255;
        *inp++ = 255;
        *inp = 255;
        inp += x_size - 2;
        *inp++ = 255;
        inp++;
        *inp = 255;
        inp += x_size - 2;
        *inp++ = 255;
        *inp++ = 255;
        *inp = 255;
      }
      midp++;
    }
  }

  /* now mark 1 black pixel at each edge point */
  midp = mid;
  _Pragma( "loopbound min 7220 max 7220" )
  for ( i = 0; i < x_size * y_size; i++ ) {
    if ( *midp < 8 )
      *( in + ( midp - mid ) ) = 0;
    midp++;
  }
}


void susan_thin( char *r, uchar *mid, int x_size, int y_size )
{
  int   l[ 9 ], centre,
        b01, b12, b21, b10,
        p1, p2, p3, p4,
        b00, b02, b20, b22,
        m, n, a, b, x, y, i, j;
  uchar *mp;

  _Pragma( "loopbound min 87 max 87" )
  for ( i = 4; i < y_size - 4; i++ ) {
    _Pragma( "loopbound min 68 max 68" )
    for ( j = 4; j < x_size - 4; j++ ) {
      if ( mid[ i * x_size + j ] < 8 ) {
        centre = r[ i * x_size + j ];
        /* {{{ count number of neighbours */

        mp = mid + ( i - 1 ) * x_size + j - 1;

        n = ( *mp < 8 ) +
            ( *( mp + 1 ) < 8 ) +
            ( *( mp + 2 ) < 8 ) +
            ( *( mp + x_size ) < 8 ) +
            ( *( mp + x_size + 2 ) < 8 ) +
            ( *( mp + x_size + x_size ) < 8 ) +
            ( *( mp + x_size + x_size + 1 ) < 8 ) +
            ( *( mp + x_size + x_size + 2 ) < 8 );

        /* {{{ n==0 no neighbours - remove point */

        if ( n == 0 )
          mid[ i * x_size + j ] = 100;

        /* {{{ n==1 - extend line if I can */

        /* extension is only allowed a few times - the value of mid is used to control this */

        if ( ( n == 1 ) && ( mid[ i * x_size + j ] < 6 ) ) {
          /* find maximum neighbour weighted in direction opposite the
             neighbour already present. e.g.
             have: O O O  weight r by 0 2 3
                   X X O              0 0 4
                   O O O              0 2 3     */

          l[ 0 ] = r[ ( i - 1 ) * x_size + j - 1 ];
          l[ 1 ] = r[ ( i - 1 ) * x_size + j ];
          l[ 2 ] = r[ ( i - 1 ) * x_size + j + 1 ];
          l[ 3 ] = r[ ( i  ) * x_size + j - 1 ];
          l[ 4 ] = 0;
          l[ 5 ] = r[ ( i  ) * x_size + j + 1 ];
          l[ 6 ] = r[ ( i + 1 ) * x_size + j - 1 ];
          l[ 7 ] = r[ ( i + 1 ) * x_size + j ];
          l[ 8 ] = r[ ( i + 1 ) * x_size + j + 1 ];

          if ( mid[ ( i - 1 )*x_size + j - 1 ] < 8 )        {
            l[ 0 ] = 0;
            l[ 1 ] = 0;
            l[ 3 ] = 0;
            l[ 2 ] *= 2;
            l[ 6 ] *= 2;
            l[ 5 ] *= 3;
            l[ 7 ] *= 3;
            l[ 8 ] *= 4;
          } else {
            if ( mid[ ( i - 1 )*x_size + j ] < 8 )   {
              l[ 1 ] = 0;
              l[ 0 ] = 0;
              l[ 2 ] = 0;
              l[ 3 ] *= 2;
              l[ 5 ] *= 2;
              l[ 6 ] *= 3;
              l[ 8 ] *= 3;
              l[ 7 ] *= 4;
            } else {
              if ( mid[ ( i - 1 )*x_size + j + 1 ] < 8 ) {
                l[ 2 ] = 0;
                l[ 1 ] = 0;
                l[ 5 ] = 0;
                l[ 0 ] *= 2;
                l[ 8 ] *= 2;
                l[ 3 ] *= 3;
                l[ 7 ] *= 3;
                l[ 6 ] *= 4;
              } else {
                if ( mid[ ( i )*x_size + j - 1 ] < 8 )   {
                  l[ 3 ] = 0;
                  l[ 0 ] = 0;
                  l[ 6 ] = 0;
                  l[ 1 ] *= 2;
                  l[ 7 ] *= 2;
                  l[ 2 ] *= 3;
                  l[ 8 ] *= 3;
                  l[ 5 ] *= 4;
                } else {
                  if ( mid[ ( i )*x_size + j + 1 ] < 8 )   {
                    l[ 5 ] = 0;
                    l[ 2 ] = 0;
                    l[ 8 ] = 0;
                    l[ 1 ] *= 2;
                    l[ 7 ] *= 2;
                    l[ 0 ] *= 3;
                    l[ 6 ] *= 3;
                    l[ 3 ] *= 4;
                  } else {
                    if ( mid[ ( i + 1 )*x_size + j - 1 ] < 8 ) {
                      l[ 6 ] = 0;
                      l[ 3 ] = 0;
                      l[ 7 ] = 0;
                      l[ 0 ] *= 2;
                      l[ 8 ] *= 2;
                      l[ 1 ] *= 3;
                      l[ 5 ] *= 3;
                      l[ 2 ] *= 4;
                    } else {
                      if ( mid[ ( i + 1 )*x_size + j ] < 8 )   {
                        l[ 7 ] = 0;
                        l[ 6 ] = 0;
                        l[ 8 ] = 0;
                        l[ 3 ] *= 2;
                        l[ 5 ] *= 2;
                        l[ 0 ] *= 3;
                        l[ 2 ] *= 3;
                        l[ 1 ] *= 4;
                      } else {
                        if ( mid[ ( i + 1 )*x_size + j + 1 ] < 8 ) {
                          l[ 8 ] = 0;
                          l[ 5 ] = 0;
                          l[ 7 ] = 0;
                          l[ 6 ] *= 2;
                          l[ 2 ] *= 2;
                          l[ 1 ] *= 3;
                          l[ 3 ] *= 3;
                          l[ 0 ] *= 4;
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          m = 0;   /* find the highest point */
          _Pragma( "loopbound min 3 max 3" )
          for ( y = 0; y < 3; y++ )
            _Pragma( "loopbound min 3 max 3" )
            for ( x = 0; x < 3; x++ )
              if ( l[ y + y + y + x ] > m ) {
                m = l[ y + y + y + x ];
                a = y;
                b = x;
              }

          if ( m > 0 ) {
            if ( mid[ i * x_size + j ] < 4 )
              mid[ ( i + a - 1 )*x_size + j + b - 1 ] = 4;
            else
              mid[ ( i + a - 1 )*x_size + j + b - 1 ] = mid[ i * x_size + j ] + 1;
            if ( ( a + a + b ) < 3 ) { /* need to jump back in image */
              i += a - 1;
              j += b - 2;
              if ( i < 4 ) i = 4;
              if ( j < 4 ) j = 4;
            }
          }
        }

        /* {{{ n==2 */

        if ( n == 2 ) {
          /* put in a bit here to straighten edges */
          b00 = mid[ ( i - 1 ) * x_size + j - 1 ] < 8; /* corners of 3x3 */
          b02 = mid[ ( i - 1 ) * x_size + j + 1 ] < 8;
          b20 = mid[ ( i + 1 ) * x_size + j - 1 ] < 8;
          b22 = mid[ ( i + 1 ) * x_size + j + 1 ] < 8;
          if ( ( ( b00 + b02 + b20 + b22 ) == 2 ) && ( ( b00 | b22 ) & ( b02 | b20 ) ) ) {
            /* case: move a point back into line.
                e.g. X O X  CAN  become X X X
                     O X O              O O O
                     O O O              O O O    */
            if ( b00 ) {
              if ( b02 ) {
                x = 0;
                y = -1;
              } else     {
                x = -1;
                y = 0;
              }
            } else {
              if ( b02 ) {
                x = 1;
                y = 0;
              } else     {
                x = 0;
                y = 1;
              }
            }
            if ( ( ( float )r[ ( i + y )*x_size + j + x ] / ( float )centre ) > 0.7 ) {
              if ( ( ( x == 0 ) && ( mid[ ( i + ( 2 * y ) )*x_size + j ] > 7 ) &&
                     ( mid[ ( i + ( 2 * y ) )*x_size + j - 1 ] > 7 ) &&
                     ( mid[ ( i + ( 2 * y ) )*x_size + j + 1 ] > 7 ) ) ||
                   ( ( y == 0 ) && ( mid[ ( i )*x_size + j + ( 2 * x ) ] > 7 ) &&
                     ( mid[ ( i + 1 )*x_size + j + ( 2 * x ) ] > 7 ) &&
                     ( mid[ ( i - 1 )*x_size + j + ( 2 * x ) ] > 7 ) ) ) {
                mid[ ( i )*x_size + j ] = 100;
                mid[ ( i + y )*x_size + j + x ] = 3; /* no jumping needed */
              }
            }
          } else {
            b01 = mid[ ( i - 1 ) * x_size + j   ] < 8;
            b12 = mid[ ( i  ) * x_size + j + 1 ] < 8;
            b21 = mid[ ( i + 1 ) * x_size + j   ] < 8;
            b10 = mid[ ( i  ) * x_size + j - 1 ] < 8;
            /* {{{ right angle ends - not currently used */

            #ifdef IGNORETHIS
            if ( ( b00 & b01 ) | ( b00 & b10 ) | ( b02 & b01 ) | ( b02 & b12 ) |
                 ( b20 & b10 ) | ( b20 & b21 ) | ( b22 & b21 ) | ( b22 & b12 ) ) {
              /* case; right angle ends. clean up.
                 e.g.; X X O  CAN  become X X O
                       O X O              O O O
                       O O O              O O O        */
              if ( ( ( b01 ) & ( mid[ ( i - 2 )*x_size + j - 1 ] > 7 ) &
                     ( mid[ ( i - 2 )*x_size + j ] > 7 ) & ( mid[ ( i - 2 )*x_size + j + 1 ] > 7 )&
                     ( ( b00 & ( ( 2 * r[ ( i - 1 )*x_size + j + 1 ] ) > centre ) ) | ( b02 & ( (
                           2 * r[ ( i - 1 )*x_size + j - 1 ] ) > centre ) ) ) ) |
                   ( ( b10 ) & ( mid[ ( i - 1 )*x_size + j - 2 ] > 7 ) & ( mid[ ( i )*x_size + j - 2 ]
                       > 7 ) & ( mid[ ( i + 1 )*x_size + j - 2 ] > 7 )&
                     ( ( b00 & ( ( 2 * r[ ( i + 1 )*x_size + j - 1 ] ) > centre ) ) | ( b20 & ( (
                           2 * r[ ( i - 1 )*x_size + j - 1 ] ) > centre ) ) ) ) |
                   ( ( b12 ) & ( mid[ ( i - 1 )*x_size + j + 2 ] > 7 ) & ( mid[ ( i )*x_size + j + 2 ]
                       > 7 ) & ( mid[ ( i + 1 )*x_size + j + 2 ] > 7 )&
                     ( ( b02 & ( ( 2 * r[ ( i + 1 )*x_size + j + 1 ] ) > centre ) ) | ( b22 & ( (
                           2 * r[ ( i - 1 )*x_size + j + 1 ] ) > centre ) ) ) ) |
                   ( ( b21 ) & ( mid[ ( i + 2 )*x_size + j - 1 ] > 7 ) & ( mid[ ( i + 2 )*x_size + j ]
                       > 7 ) & ( mid[ ( i + 2 )*x_size + j + 1 ] > 7 )&
                     ( ( b20 & ( ( 2 * r[ ( i + 1 )*x_size + j + 1 ] ) > centre ) ) | ( b22 & ( (
                           2 * r[ ( i + 1 )*x_size + j - 1 ] ) > centre ) ) ) ) ) {
                mid[ ( i )*x_size + j ] = 100;
                if ( b10 & b20 ) j -= 2;
                if ( b00 | b01 | b02 ) {
                  i--;
                  j -= 2;
                }
              }
            }
            #endif

            if ( ( ( b01 + b12 + b21 + b10 ) == 2 ) && ( ( b10 | b12 ) & ( b01 | b21 ) ) &&
                 ( ( b01 & ( ( mid[ ( i - 2 )*x_size + j - 1 ] < 8 ) | ( mid[ ( i - 2 )*x_size + j +
                             1 ] < 8 ) ) ) | ( b10 & ( ( mid[ ( i - 1 )*x_size + j - 2 ] < 8 ) |
                                              ( mid[ ( i + 1 )*x_size + j - 2 ] < 8 ) ) ) |
                   ( b12 & ( ( mid[ ( i - 1 )*x_size + j + 2 ] < 8 ) | ( mid[ ( i + 1 )*x_size + j +
                             2 ] < 8 ) ) ) | ( b21 & ( ( mid[ ( i + 2 )*x_size + j - 1 ] < 8 ) |
                                              ( mid[ ( i + 2 )*x_size + j + 1 ] < 8 ) ) ) ) ) {
              /* case; clears odd right angles.
                 e.g.; O O O  becomes O O O
                       X X O          X O O
                       O X O          O X O     */
              mid[ ( i )*x_size + j ] = 100;
              i--;               /* jump back */
              j -= 2;
              if ( i < 4 ) i = 4;
              if ( j < 4 ) j = 4;
            }
          }
        }

        /* {{{ n>2 the thinning is done here without breaking connectivity */

        if ( n > 2 ) {
          b01 = mid[ ( i - 1 ) * x_size + j   ] < 8;
          b12 = mid[ ( i  ) * x_size + j + 1 ] < 8;
          b21 = mid[ ( i + 1 ) * x_size + j   ] < 8;
          b10 = mid[ ( i  ) * x_size + j - 1 ] < 8;
          if ( ( b01 + b12 + b21 + b10 ) > 1 ) {
            b00 = mid[ ( i - 1 ) * x_size + j - 1 ] < 8;
            b02 = mid[ ( i - 1 ) * x_size + j + 1 ] < 8;
            b20 = mid[ ( i + 1 ) * x_size + j - 1 ] < 8;
            b22 = mid[ ( i + 1 ) * x_size + j + 1 ] < 8;
            p1 = b00 | b01;
            p2 = b02 | b12;
            p3 = b22 | b21;
            p4 = b20 | b10;

            if ( ( ( p1 + p2 + p3 + p4 ) - ( ( b01 & p2 ) + ( b12 & p3 ) + ( b21 & p4 ) +
                                             ( b10 & p1 ) ) ) < 2 ) {
              mid[ ( i )*x_size + j ] = 100;
              i--;
              j -= 2;
              if ( i < 4 ) i = 4;
              if ( j < 4 ) j = 4;
            }
          }
        }
      }
    }
  }
}


void susan_edges( uchar *in, char *r, uchar *mid, uchar *bp,
                  int max_no, int x_size, int y_size )
{
  float z;
  int   do_symmetry, i, j, m, n, a, b, x, y, w;
  uchar c, *p, *cp;

  susan_wccmemset( r, 0, x_size * y_size );

  _Pragma( "loopbound min 89 max 89" )
  for ( i = 3; i < y_size - 3; i++ ) {
    _Pragma( "loopbound min 70 max 70" )
    for ( j = 3; j < x_size - 3; j++ ) {
      n = 100;
      p = in + ( i - 3 ) * x_size + j - 1;
      cp = bp + in[ i * x_size + j ];

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 3;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 5;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 6;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += 2;
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 6;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 5;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 3;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );

      if ( n <= max_no )
        r[ i * x_size + j ] = max_no - n;
    }
  }

  _Pragma( "loopbound min 87 max 87" )
  for ( i = 4; i < y_size - 4; i++ ) {
    _Pragma( "loopbound min 68 max 68" )
    for ( j = 4; j < x_size - 4; j++ ) {
      if ( r[ i * x_size + j ] > 0 ) {
        m = r[ i * x_size + j ];
        n = max_no - m;
        cp = bp + in[ i * x_size + j ];

        if ( n > 600 ) {
          p = in + ( i - 3 ) * x_size + j - 1;
          x = 0;
          y = 0;

          c = *( cp - *p++ );
          x -= c;
          y -= 3 * c;
          c = *( cp - *p++ );
          y -= 3 * c;
          c = *( cp - *p );
          x += c;
          y -= 3 * c;
          p += x_size - 3;

          c = *( cp - *p++ );
          x -= 2 * c;
          y -= 2 * c;
          c = *( cp - *p++ );
          x -= c;
          y -= 2 * c;
          c = *( cp - *p++ );
          y -= 2 * c;
          c = *( cp - *p++ );
          x += c;
          y -= 2 * c;
          c = *( cp - *p );
          x += 2 * c;
          y -= 2 * c;
          p += x_size - 5;

          c = *( cp - *p++ );
          x -= 3 * c;
          y -= c;
          c = *( cp - *p++ );
          x -= 2 * c;
          y -= c;
          c = *( cp - *p++ );
          x -= c;
          y -= c;
          c = *( cp - *p++ );
          y -= c;
          c = *( cp - *p++ );
          x += c;
          y -= c;
          c = *( cp - *p++ );
          x += 2 * c;
          y -= c;
          c = *( cp - *p );
          x += 3 * c;
          y -= c;
          p += x_size - 6;

          c = *( cp - *p++ );
          x -= 3 * c;
          c = *( cp - *p++ );
          x -= 2 * c;
          c = *( cp - *p );
          x -= c;
          p += 2;
          c = *( cp - *p++ );
          x += c;
          c = *( cp - *p++ );
          x += 2 * c;
          c = *( cp - *p );
          x += 3 * c;
          p += x_size - 6;

          c = *( cp - *p++ );
          x -= 3 * c;
          y += c;
          c = *( cp - *p++ );
          x -= 2 * c;
          y += c;
          c = *( cp - *p++ );
          x -= c;
          y += c;
          c = *( cp - *p++ );
          y += c;
          c = *( cp - *p++ );
          x += c;
          y += c;
          c = *( cp - *p++ );
          x += 2 * c;
          y += c;
          c = *( cp - *p );
          x += 3 * c;
          y += c;
          p += x_size - 5;

          c = *( cp - *p++ );
          x -= 2 * c;
          y += 2 * c;
          c = *( cp - *p++ );
          x -= c;
          y += 2 * c;
          c = *( cp - *p++ );
          y += 2 * c;
          c = *( cp - *p++ );
          x += c;
          y += 2 * c;
          c = *( cp - *p );
          x += 2 * c;
          y += 2 * c;
          p += x_size - 3;

          c = *( cp - *p++ );
          x -= c;
          y += 3 * c;
          c = *( cp - *p++ );
          y += 3 * c;
          c = *( cp - *p );
          x += c;
          y += 3 * c;

          z = susan_sqrtf( ( float )( ( x * x ) + ( y * y ) ) );
          if ( z > ( 0.9 * ( float )n ) ) { /* 0.5 */
            do_symmetry = 0;
            if ( x == 0 )
              z = 1000000.0;
            else
              z = ( ( float )y ) / ( ( float )x );
            if ( z < 0 ) {
              z = -z;
              w = -1;
            } else w = 1;
            if ( z < 0.5 ) {
              /* vert_edge */ a = 0;
              b = 1;
            } else {
              if ( z > 2.0 ) {
                /* hor_edge */ a = 1;
                b = 0;
              } else {
                /* diag_edge */ if ( w > 0 ) {
                  a = 1;
                  b = 1;
                } else {
                  a = -1;
                  b = 1;
                }
              }
            }
            if ( ( m > r[ ( i + a )*x_size + j + b ] ) &&
                 ( m >= r[ ( i - a )*x_size + j - b ] ) &&
                 ( m > r[ ( i + ( 2 * a ) )*x_size + j + ( 2 * b ) ] ) &&
                 ( m >= r[ ( i - ( 2 * a ) )*x_size + j - ( 2 * b ) ] ) )
              mid[ i * x_size + j ] = 1;
          } else
            do_symmetry = 1;
        } else
          do_symmetry = 1;

        if ( do_symmetry == 1 ) {
          p = in + ( i - 3 ) * x_size + j - 1;
          x = 0;
          y = 0;
          w = 0;

          /*   |      \
               y  -x-  w
               |        \   */

          c = *( cp - *p++ );
          x += c;
          y += 9 * c;
          w += 3 * c;
          c = *( cp - *p++ );
          y += 9 * c;
          c = *( cp - *p );
          x += c;
          y += 9 * c;
          w -= 3 * c;
          p += x_size - 3;

          c = *( cp - *p++ );
          x += 4 * c;
          y += 4 * c;
          w += 4 * c;
          c = *( cp - *p++ );
          x += c;
          y += 4 * c;
          w += 2 * c;
          c = *( cp - *p++ );
          y += 4 * c;
          c = *( cp - *p++ );
          x += c;
          y += 4 * c;
          w -= 2 * c;
          c = *( cp - *p );
          x += 4 * c;
          y += 4 * c;
          w -= 4 * c;
          p += x_size - 5;

          c = *( cp - *p++ );
          x += 9 * c;
          y += c;
          w += 3 * c;
          c = *( cp - *p++ );
          x += 4 * c;
          y += c;
          w += 2 * c;
          c = *( cp - *p++ );
          x += c;
          y += c;
          w += c;
          c = *( cp - *p++ );
          y += c;
          c = *( cp - *p++ );
          x += c;
          y += c;
          w -= c;
          c = *( cp - *p++ );
          x += 4 * c;
          y += c;
          w -= 2 * c;
          c = *( cp - *p );
          x += 9 * c;
          y += c;
          w -= 3 * c;
          p += x_size - 6;

          c = *( cp - *p++ );
          x += 9 * c;
          c = *( cp - *p++ );
          x += 4 * c;
          c = *( cp - *p );
          x += c;
          p += 2;
          c = *( cp - *p++ );
          x += c;
          c = *( cp - *p++ );
          x += 4 * c;
          c = *( cp - *p );
          x += 9 * c;
          p += x_size - 6;

          c = *( cp - *p++ );
          x += 9 * c;
          y += c;
          w -= 3 * c;
          c = *( cp - *p++ );
          x += 4 * c;
          y += c;
          w -= 2 * c;
          c = *( cp - *p++ );
          x += c;
          y += c;
          w -= c;
          c = *( cp - *p++ );
          y += c;
          c = *( cp - *p++ );
          x += c;
          y += c;
          w += c;
          c = *( cp - *p++ );
          x += 4 * c;
          y += c;
          w += 2 * c;
          c = *( cp - *p );
          x += 9 * c;
          y += c;
          w += 3 * c;
          p += x_size - 5;

          c = *( cp - *p++ );
          x += 4 * c;
          y += 4 * c;
          w -= 4 * c;
          c = *( cp - *p++ );
          x += c;
          y += 4 * c;
          w -= 2 * c;
          c = *( cp - *p++ );
          y += 4 * c;
          c = *( cp - *p++ );
          x += c;
          y += 4 * c;
          w += 2 * c;
          c = *( cp - *p );
          x += 4 * c;
          y += 4 * c;
          w += 4 * c;
          p += x_size - 3;

          c = *( cp - *p++ );
          x += c;
          y += 9 * c;
          w -= 3 * c;
          c = *( cp - *p++ );
          y += 9 * c;
          c = *( cp - *p );
          x += c;
          y += 9 * c;
          w += 3 * c;

          if ( y == 0 )
            z = 1000000.0;
          else
            z = ( ( float )x ) / ( ( float )y );
          if ( z < 0.5 ) {
            /* vertical */ a = 0;
            b = 1;
          } else {
            if ( z > 2.0 ) {
              /* horizontal */ a = 1;
              b = 0;
            } else {
              /* diagonal */ if ( w > 0 ) {
                a = -1;
                b = 1;
              } else {
                a = 1;
                b = 1;
              }
            }
          }
          if ( ( m > r[ ( i + a )*x_size + j + b ] ) &&
               ( m >= r[ ( i - a )*x_size + j - b ] ) &&
               ( m > r[ ( i + ( 2 * a ) )*x_size + j + ( 2 * b ) ] ) &&
               ( m >= r[ ( i - ( 2 * a ) )*x_size + j - ( 2 * b ) ] ) )
            mid[ i * x_size + j ] = 2;
        }
      }
    }
  }
}


void susan_edges_small( uchar *in, char *r, uchar *mid, uchar *bp,
                        int max_no, int x_size, int y_size )
{
  float z;
  int   do_symmetry, i, j, m, n, a, b, x, y, w;
  uchar c, *p, *cp;

  susan_wccmemset( r, 0, x_size * y_size );

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 1; i < y_size - 1; i++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( j = 1; j < x_size - 1; j++ ) {
      n = 100;
      p = in + ( i - 1 ) * x_size + j - 1;
      cp = bp + in[ i * x_size + j ];

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 2;

      n += *( cp - *p );
      p += 2;
      n += *( cp - *p );
      p += x_size - 2;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );

      if ( n <= max_no )
        r[ i * x_size + j ] = max_no - n;
    }
  }

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 2; i < y_size - 2; i++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( j = 2; j < x_size - 2; j++ ) {
      if ( r[ i * x_size + j ] > 0 ) {
        m = r[ i * x_size + j ];
        n = max_no - m;
        cp = bp + in[ i * x_size + j ];

        if ( n > 250 ) {
          p = in + ( i - 1 ) * x_size + j - 1;
          x = 0;
          y = 0;

          c = *( cp - *p++ );
          x -= c;
          y -= c;
          c = *( cp - *p++ );
          y -= c;
          c = *( cp - *p );
          x += c;
          y -= c;
          p += x_size - 2;

          c = *( cp - *p );
          x -= c;
          p += 2;
          c = *( cp - *p );
          x += c;
          p += x_size - 2;

          c = *( cp - *p++ );
          x -= c;
          y += c;
          c = *( cp - *p++ );
          y += c;
          c = *( cp - *p );
          x += c;
          y += c;

          z = susan_sqrtf( ( float )( ( x * x ) + ( y * y ) ) );
          if ( z > ( 0.4 * ( float )n ) ) { /* 0.6 */
            do_symmetry = 0;
            if ( x == 0 )
              z = 1000000.0;
            else
              z = ( ( float )y ) / ( ( float )x );
            if ( z < 0 ) {
              z = -z;
              w = -1;
            } else w = 1;
            if ( z < 0.5 ) {
              /* vert_edge */ a = 0;
              b = 1;
            } else {
              if ( z > 2.0 ) {
                /* hor_edge */ a = 1;
                b = 0;
              } else {
                /* diag_edge */ if ( w > 0 ) {
                  a = 1;
                  b = 1;
                } else {
                  a = -1;
                  b = 1;
                }
              }
            }
            if ( ( m > r[ ( i + a )*x_size + j + b ] ) &&
                 ( m >= r[ ( i - a )*x_size + j - b ] ) )
              mid[ i * x_size + j ] = 1;
          } else
            do_symmetry = 1;
        } else
          do_symmetry = 1;

        if ( do_symmetry == 1 ) {
          p = in + ( i - 1 ) * x_size + j - 1;
          x = 0;
          y = 0;
          w = 0;

          /*   |      \
               y  -x-  w
               |        \   */

          c = *( cp - *p++ );
          x += c;
          y += c;
          w += c;
          c = *( cp - *p++ );
          y += c;
          c = *( cp - *p );
          x += c;
          y += c;
          w -= c;
          p += x_size - 2;

          c = *( cp - *p );
          x += c;
          p += 2;
          c = *( cp - *p );
          x += c;
          p += x_size - 2;

          c = *( cp - *p++ );
          x += c;
          y += c;
          w -= c;
          c = *( cp - *p++ );
          y += c;
          c = *( cp - *p );
          x += c;
          y += c;
          w += c;

          if ( y == 0 )
            z = 1000000.0;
          else
            z = ( ( float )x ) / ( ( float )y );
          if ( z < 0.5 ) {
            /* vertical */ a = 0;
            b = 1;
          } else {
            if ( z > 2.0 ) {
              /* horizontal */ a = 1;
              b = 0;
            } else {
              /* diagonal */ if ( w > 0 ) {
                a = -1;
                b = 1;
              } else {
                a = 1;
                b = 1;
              }
            }
          }
          if ( ( m > r[ ( i + a )*x_size + j + b ] ) &&
               ( m >= r[ ( i - a )*x_size + j - b ] ) )
            mid[ i * x_size + j ] = 2;
        }
      }
    }
  }
}


void susan_corner_draw( uchar *in, CORNER_LIST corner_list,
                        int x_size, int drawing_mode )
{
  uchar *p;
  int   n = 0;

  _Pragma( "loopbound min 0 max 0" )
  while ( corner_list[ n ].info != 7 ) {
    if ( drawing_mode == 0 ) {
      p = in + ( corner_list[ n ].y - 1 ) * x_size + corner_list[ n ].x - 1;
      *p++ = 255;
      *p++ = 255;
      *p = 255;
      p += x_size - 2;
      *p++ = 255;
      *p++ = 0;
      *p = 255;
      p += x_size - 2;
      *p++ = 255;
      *p++ = 255;
      *p = 255;
      n++;
    } else {
      p = in + corner_list[ n ].y * x_size + corner_list[ n ].x;
      *p = 0;
      n++;
    }
  }
}


void susan_corners( uchar *in, char *r, uchar *bp,
                    int max_no, CORNER_LIST corner_list, int x_size, int y_size )
{
  int   n, x, y, sq, xx, yy,
        i, j;
  float divide;
  uchar c, *p, *cp;
  char  *cgx, *cgy;

  susan_wccmemset( r, 0, x_size * y_size );

  cgx = ( char * )susan_wccmalloc( x_size * y_size );
  cgy = ( char * )susan_wccmalloc( x_size * y_size );

  _Pragma( "loopbound min 85 max 85" )
  for ( i = 5; i < y_size - 5; i++ ) {
    _Pragma( "loopbound min 66 max 66" )
    for ( j = 5; j < x_size - 5; j++ ) {
      n = 100;
      p = in + ( i - 3 ) * x_size + j - 1;
      cp = bp + in[ i * x_size + j ];

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 3;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 5;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 6;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      if ( n < max_no ) { /* do this test early and often ONLY to save wasted computation */
        p += 2;
        n += *( cp - *p++ );
        if ( n < max_no ) {
          n += *( cp - *p++ );
          if ( n < max_no ) {
            n += *( cp - *p );
            if ( n < max_no ) {
              p += x_size - 6;

              n += *( cp - *p++ );
              if ( n < max_no ) {
                n += *( cp - *p++ );
                if ( n < max_no ) {
                  n += *( cp - *p++ );
                  if ( n < max_no ) {
                    n += *( cp - *p++ );
                    if ( n < max_no ) {
                      n += *( cp - *p++ );
                      if ( n < max_no ) {
                        n += *( cp - *p++ );
                        if ( n < max_no ) {
                          n += *( cp - *p );
                          if ( n < max_no ) {
                            p += x_size - 5;

                            n += *( cp - *p++ );
                            if ( n < max_no ) {
                              n += *( cp - *p++ );
                              if ( n < max_no ) {
                                n += *( cp - *p++ );
                                if ( n < max_no ) {
                                  n += *( cp - *p++ );
                                  if ( n < max_no ) {
                                    n += *( cp - *p );
                                    if ( n < max_no ) {
                                      p += x_size - 3;

                                      n += *( cp - *p++ );
                                      if ( n < max_no ) {
                                        n += *( cp - *p++ );
                                        if ( n < max_no ) {
                                          n += *( cp - *p );

                                          if ( n < max_no ) {
                                            x = 0;
                                            y = 0;
                                            p = in + ( i - 3 ) * x_size + j - 1;

                                            c = *( cp - *p++ );
                                            x -= c;
                                            y -= 3 * c;
                                            c = *( cp - *p++ );
                                            y -= 3 * c;
                                            c = *( cp - *p );
                                            x += c;
                                            y -= 3 * c;
                                            p += x_size - 3;

                                            c = *( cp - *p++ );
                                            x -= 2 * c;
                                            y -= 2 * c;
                                            c = *( cp - *p++ );
                                            x -= c;
                                            y -= 2 * c;
                                            c = *( cp - *p++ );
                                            y -= 2 * c;
                                            c = *( cp - *p++ );
                                            x += c;
                                            y -= 2 * c;
                                            c = *( cp - *p );
                                            x += 2 * c;
                                            y -= 2 * c;
                                            p += x_size - 5;

                                            c = *( cp - *p++ );
                                            x -= 3 * c;
                                            y -= c;
                                            c = *( cp - *p++ );
                                            x -= 2 * c;
                                            y -= c;
                                            c = *( cp - *p++ );
                                            x -= c;
                                            y -= c;
                                            c = *( cp - *p++ );
                                            y -= c;
                                            c = *( cp - *p++ );
                                            x += c;
                                            y -= c;
                                            c = *( cp - *p++ );
                                            x += 2 * c;
                                            y -= c;
                                            c = *( cp - *p );
                                            x += 3 * c;
                                            y -= c;
                                            p += x_size - 6;

                                            c = *( cp - *p++ );
                                            x -= 3 * c;
                                            c = *( cp - *p++ );
                                            x -= 2 * c;
                                            c = *( cp - *p );
                                            x -= c;
                                            p += 2;
                                            c = *( cp - *p++ );
                                            x += c;
                                            c = *( cp - *p++ );
                                            x += 2 * c;
                                            c = *( cp - *p );
                                            x += 3 * c;
                                            p += x_size - 6;

                                            c = *( cp - *p++ );
                                            x -= 3 * c;
                                            y += c;
                                            c = *( cp - *p++ );
                                            x -= 2 * c;
                                            y += c;
                                            c = *( cp - *p++ );
                                            x -= c;
                                            y += c;
                                            c = *( cp - *p++ );
                                            y += c;
                                            c = *( cp - *p++ );
                                            x += c;
                                            y += c;
                                            c = *( cp - *p++ );
                                            x += 2 * c;
                                            y += c;
                                            c = *( cp - *p );
                                            x += 3 * c;
                                            y += c;
                                            p += x_size - 5;

                                            c = *( cp - *p++ );
                                            x -= 2 * c;
                                            y += 2 * c;
                                            c = *( cp - *p++ );
                                            x -= c;
                                            y += 2 * c;
                                            c = *( cp - *p++ );
                                            y += 2 * c;
                                            c = *( cp - *p++ );
                                            x += c;
                                            y += 2 * c;
                                            c = *( cp - *p );
                                            x += 2 * c;
                                            y += 2 * c;
                                            p += x_size - 3;

                                            c = *( cp - *p++ );
                                            x -= c;
                                            y += 3 * c;
                                            c = *( cp - *p++ );
                                            y += 3 * c;
                                            c = *( cp - *p );
                                            x += c;
                                            y += 3 * c;

                                            xx = x * x;
                                            yy = y * y;
                                            sq = xx + yy;
                                            if ( sq > ( ( n * n ) / 2 ) ) {
                                              if ( yy < xx ) {
                                                divide = ( float )y / ( float )abs( x );
                                                sq = abs( x ) / x;
                                                sq = *( cp - in[ ( i + FTOI( divide ) ) * x_size + j + sq ] ) +
                                                     *( cp - in[ ( i + FTOI( 2 * divide ) ) * x_size + j + 2 * sq ] ) +
                                                     *( cp - in[ ( i + FTOI( 3 * divide ) ) * x_size + j + 3 * sq ] );
                                              } else {
                                                divide = ( float )x / ( float )abs( y );
                                                sq = abs( y ) / y;
                                                sq = *( cp - in[ ( i + sq ) * x_size + j + FTOI( divide ) ] ) +
                                                     *( cp - in[ ( i + 2 * sq ) * x_size + j + FTOI( 2 * divide ) ] ) +
                                                     *( cp - in[ ( i + 3 * sq ) * x_size + j + FTOI( 3 * divide ) ] );
                                              }

                                              if ( sq > 290 ) {
                                                r[ i * x_size + j ] = max_no - n;
                                                cgx[ i * x_size + j ] = ( 51 * x ) / n;
                                                cgy[ i * x_size + j ] = ( 51 * y ) / n;
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  /* to locate the local maxima */
  n = 0;
  _Pragma( "loopbound min 85 max 85" )
  for ( i = 5; i < y_size - 5; i++ ) {
    _Pragma( "loopbound min 66 max 66" )
    for ( j = 5; j < x_size - 5; j++ ) {
      x = r[ i * x_size + j ];
      if ( x > 0 )  {
        /* 5x5 mask */
        #ifdef FIVE_SUPP
        if ( ( x > r[ ( i - 1 )*x_size + j + 2 ] ) &&
             ( x > r[ ( i  )*x_size + j + 1 ] ) &&
             ( x > r[ ( i  )*x_size + j + 2 ] ) &&
             ( x > r[ ( i + 1 )*x_size + j - 1 ] ) &&
             ( x > r[ ( i + 1 )*x_size + j   ] ) &&
             ( x > r[ ( i + 1 )*x_size + j + 1 ] ) &&
             ( x > r[ ( i + 1 )*x_size + j + 2 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j - 2 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j - 1 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j   ] ) &&
             ( x > r[ ( i + 2 )*x_size + j + 1 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j + 2 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j - 2 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j - 1 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j   ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j + 1 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j + 2 ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j - 2 ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j - 1 ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j   ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j + 1 ] ) &&
             ( x >= r[ ( i  )*x_size + j - 2 ] ) &&
             ( x >= r[ ( i  )*x_size + j - 1 ] ) &&
             ( x >= r[ ( i + 1 )*x_size + j - 2 ] ) )
        #endif
        #ifdef SEVEN_SUPP
          if ( ( x > r[ ( i - 3 )*x_size + j - 3 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j - 2 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j - 1 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j   ] ) &&
               ( x > r[ ( i - 3 )*x_size + j + 1 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j + 2 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j + 3 ] ) &&

               ( x > r[ ( i - 2 )*x_size + j - 3 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j - 2 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j - 1 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j   ] ) &&
               ( x > r[ ( i - 2 )*x_size + j + 1 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j + 2 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j + 3 ] ) &&

               ( x > r[ ( i - 1 )*x_size + j - 3 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j - 2 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j - 1 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j   ] ) &&
               ( x > r[ ( i - 1 )*x_size + j + 1 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j + 2 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j + 3 ] ) &&

               ( x > r[ ( i )*x_size + j - 3 ] ) &&
               ( x > r[ ( i )*x_size + j - 2 ] ) &&
               ( x > r[ ( i )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i )*x_size + j + 3 ] ) &&

               ( x >= r[ ( i + 1 )*x_size + j - 3 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j - 2 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j   ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j + 3 ] ) &&

               ( x >= r[ ( i + 2 )*x_size + j - 3 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j - 2 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j   ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j + 3 ] ) &&

               ( x >= r[ ( i + 3 )*x_size + j - 3 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j - 2 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j   ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j + 3 ] ) )
        #endif
          {
            corner_list[ n ].info = 0;
            corner_list[ n ].x = j;
            corner_list[ n ].y = i;
            corner_list[ n ].dx = cgx[ i * x_size + j ];
            corner_list[ n ].dy = cgy[ i * x_size + j ];
            corner_list[ n ].I = in[ i * x_size + j ];
            n++;
            if ( n == MAX_CORNERS ) {
              /*  "Too many corners." */
            }
          }
      }
    }
  }
  corner_list[ n ].info = 7;
}


void susan_corners_quick( uchar *in, char *r, uchar *bp,
                          int max_no, CORNER_LIST corner_list, int x_size, int y_size )
{
  int   n, x, y, i, j;
  uchar *p, *cp;

  susan_wccmemset( r, 0, x_size * y_size );

  _Pragma( "loopbound min 0 max 0" )
  for ( i = 7; i < y_size - 7; i++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( j = 7; j < x_size - 7; j++ ) {
      n = 100;
      p = in + ( i - 3 ) * x_size + j - 1;
      cp = bp + in[ i * x_size + j ];

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 3;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 5;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      p += x_size - 6;

      n += *( cp - *p++ );
      n += *( cp - *p++ );
      n += *( cp - *p );
      if ( n < max_no ) {
        p += 2;
        n += *( cp - *p++ );
        if ( n < max_no ) {
          n += *( cp - *p++ );
          if ( n < max_no ) {
            n += *( cp - *p );
            if ( n < max_no ) {
              p += x_size - 6;

              n += *( cp - *p++ );
              if ( n < max_no ) {
                n += *( cp - *p++ );
                if ( n < max_no ) {
                  n += *( cp - *p++ );
                  if ( n < max_no ) {
                    n += *( cp - *p++ );
                    if ( n < max_no ) {
                      n += *( cp - *p++ );
                      if ( n < max_no ) {
                        n += *( cp - *p++ );
                        if ( n < max_no ) {
                          n += *( cp - *p );
                          if ( n < max_no ) {
                            p += x_size - 5;

                            n += *( cp - *p++ );
                            if ( n < max_no ) {
                              n += *( cp - *p++ );
                              if ( n < max_no ) {
                                n += *( cp - *p++ );
                                if ( n < max_no ) {
                                  n += *( cp - *p++ );
                                  if ( n < max_no ) {
                                    n += *( cp - *p );
                                    if ( n < max_no ) {
                                      p += x_size - 3;

                                      n += *( cp - *p++ );
                                      if ( n < max_no ) {
                                        n += *( cp - *p++ );
                                        if ( n < max_no ) {
                                          n += *( cp - *p );

                                          if ( n < max_no )
                                            r[ i * x_size + j ] = max_no - n;
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  /* to locate the local maxima */
  n = 0;
  _Pragma( "loopbound min 0 max 0" )
  for ( i = 7; i < y_size - 7; i++ ) {
    _Pragma( "loopbound min 0 max 0" )
    for ( j = 7; j < x_size - 7; j++ ) {
      x = r[ i * x_size + j ];
      if ( x > 0 )  {
        /* 5x5 mask */
        #ifdef FIVE_SUPP
        if ( ( x > r[ ( i - 1 )*x_size + j + 2 ] ) &&
             ( x > r[ ( i  )*x_size + j + 1 ] ) &&
             ( x > r[ ( i  )*x_size + j + 2 ] ) &&
             ( x > r[ ( i + 1 )*x_size + j - 1 ] ) &&
             ( x > r[ ( i + 1 )*x_size + j   ] ) &&
             ( x > r[ ( i + 1 )*x_size + j + 1 ] ) &&
             ( x > r[ ( i + 1 )*x_size + j + 2 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j - 2 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j - 1 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j   ] ) &&
             ( x > r[ ( i + 2 )*x_size + j + 1 ] ) &&
             ( x > r[ ( i + 2 )*x_size + j + 2 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j - 2 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j - 1 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j   ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j + 1 ] ) &&
             ( x >= r[ ( i - 2 )*x_size + j + 2 ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j - 2 ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j - 1 ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j   ] ) &&
             ( x >= r[ ( i - 1 )*x_size + j + 1 ] ) &&
             ( x >= r[ ( i  )*x_size + j - 2 ] ) &&
             ( x >= r[ ( i  )*x_size + j - 1 ] ) &&
             ( x >= r[ ( i + 1 )*x_size + j - 2 ] ) )
        #endif
        #ifdef SEVEN_SUPP
          if ( ( x > r[ ( i - 3 )*x_size + j - 3 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j - 2 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j - 1 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j   ] ) &&
               ( x > r[ ( i - 3 )*x_size + j + 1 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j + 2 ] ) &&
               ( x > r[ ( i - 3 )*x_size + j + 3 ] ) &&

               ( x > r[ ( i - 2 )*x_size + j - 3 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j - 2 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j - 1 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j   ] ) &&
               ( x > r[ ( i - 2 )*x_size + j + 1 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j + 2 ] ) &&
               ( x > r[ ( i - 2 )*x_size + j + 3 ] ) &&

               ( x > r[ ( i - 1 )*x_size + j - 3 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j - 2 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j - 1 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j   ] ) &&
               ( x > r[ ( i - 1 )*x_size + j + 1 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j + 2 ] ) &&
               ( x > r[ ( i - 1 )*x_size + j + 3 ] ) &&

               ( x > r[ ( i )*x_size + j - 3 ] ) &&
               ( x > r[ ( i )*x_size + j - 2 ] ) &&
               ( x > r[ ( i )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i )*x_size + j + 3 ] ) &&

               ( x >= r[ ( i + 1 )*x_size + j - 3 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j - 2 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j   ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i + 1 )*x_size + j + 3 ] ) &&

               ( x >= r[ ( i + 2 )*x_size + j - 3 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j - 2 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j   ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i + 2 )*x_size + j + 3 ] ) &&

               ( x >= r[ ( i + 3 )*x_size + j - 3 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j - 2 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j - 1 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j   ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j + 1 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j + 2 ] ) &&
               ( x >= r[ ( i + 3 )*x_size + j + 3 ] ) )
        #endif
          {
            corner_list[ n ].info = 0;
            corner_list[ n ].x = j;
            corner_list[ n ].y = i;
            x = in[ ( i - 2 ) * x_size + j - 2 ] + in[ ( i - 2 ) * x_size + j - 1 ] + in[ ( i -
                2 ) * x_size + j ] + in[ ( i - 2 ) * x_size + j + 1 ] + in[ ( i - 2 ) * x_size + j +
                    2 ] +
                in[ ( i - 1 ) * x_size + j - 2 ] + in[ ( i - 1 ) * x_size + j - 1 ] + in[ ( i - 1 ) *
                    x_size + j ] + in[ ( i - 1 ) * x_size + j + 1 ] + in[ ( i - 1 ) * x_size + j + 2 ] +
                in[ ( i  ) * x_size + j - 2 ] + in[ ( i  ) * x_size + j - 1 ] + in[ ( i  ) * x_size +
                    j ] + in[ ( i  ) * x_size + j + 1 ] + in[ ( i  ) * x_size + j + 2 ] +
                in[ ( i + 1 ) * x_size + j - 2 ] + in[ ( i + 1 ) * x_size + j - 1 ] + in[ ( i + 1 ) *
                    x_size + j ] + in[ ( i + 1 ) * x_size + j + 1 ] + in[ ( i + 1 ) * x_size + j + 2 ] +
                in[ ( i + 2 ) * x_size + j - 2 ] + in[ ( i + 2 ) * x_size + j - 1 ] + in[ ( i + 2 ) *
                    x_size + j ] + in[ ( i + 2 ) * x_size + j + 1 ] + in[ ( i + 2 ) * x_size + j + 2 ];

            corner_list[ n ].I = x / 25;
            /*corner_list[ n ].I=in[ i*x_size+j ];*/
            x = in[ ( i - 2 ) * x_size + j + 2 ] + in[ ( i - 1 ) * x_size + j + 2 ] + in[ ( i ) *
                x_size + j + 2 ] + in[ ( i + 1 ) * x_size + j + 2 ] + in[ ( i + 2 ) * x_size + j +
                    2 ] -
                ( in[ ( i - 2 ) * x_size + j - 2 ] + in[ ( i - 1 ) * x_size + j - 2 ] + in[ ( i ) *
                    x_size + j - 2 ] + in[ ( i + 1 ) * x_size + j - 2 ] + in[ ( i + 2 ) * x_size + j -
                        2 ] );
            x += x + in[ ( i - 2 ) * x_size + j + 1 ] + in[ ( i - 1 ) * x_size + j + 1 ] +
                 in[ ( i ) * x_size + j + 1 ] + in[ ( i + 1 ) * x_size + j + 1 ] + in[ ( i + 2 ) *
                     x_size + j + 1 ] -
                 ( in[ ( i - 2 ) * x_size + j - 1 ] + in[ ( i - 1 ) * x_size + j - 1 ] + in[ ( i ) *
                     x_size + j - 1 ] + in[ ( i + 1 ) * x_size + j - 1 ] + in[ ( i + 2 ) * x_size + j -
                         1 ] );

            y = in[ ( i + 2 ) * x_size + j - 2 ] + in[ ( i + 2 ) * x_size + j - 1 ] + in[ ( i +
                2 ) * x_size + j ] + in[ ( i + 2 ) * x_size + j + 1 ] + in[ ( i + 2 ) * x_size + j +
                    2 ] -
                ( in[ ( i - 2 ) * x_size + j - 2 ] + in[ ( i - 2 ) * x_size + j - 1 ] + in[ ( i - 2 )
                    * x_size + j ] + in[ ( i - 2 ) * x_size + j + 1 ] + in[ ( i - 2 ) * x_size + j +
                        2 ] );
            y += y + in[ ( i + 1 ) * x_size + j - 2 ] + in[ ( i + 1 ) * x_size + j - 1 ] +
                 in[ ( i + 1 ) * x_size + j ] + in[ ( i + 1 ) * x_size + j + 1 ] + in[ ( i + 1 ) *
                     x_size + j + 2 ] -
                 ( in[ ( i - 1 ) * x_size + j - 2 ] + in[ ( i - 1 ) * x_size + j - 1 ] + in[ ( i - 1 )
                     * x_size + j ] + in[ ( i - 1 ) * x_size + j + 1 ] + in[ ( i - 1 ) * x_size + j +
                         2 ] );
            corner_list[ n ].dx = x / 15;
            corner_list[ n ].dy = y / 15;
            n++;
            if ( n == MAX_CORNERS ) {
              /*  "Too many corners." */
            }
          }
      }
    }
  }
  corner_list[ n ].info = 7;
}


void susan_call_susan( struct wccFILE *inputFile, int mode )
{
  uchar  *in, *bp, *mid;
  int   x_size, y_size;
  char  *r;
  CORNER_LIST corner_list;

  susan_get_image( inputFile, &in, &x_size, &y_size );

  if ( susan_dt < 0 ) susan_three_by_three = 1;
  if ( ( susan_principle_conf == 1 ) && ( mode == 0 ) )
    mode = 1;

  switch ( mode ) {
    case 0:
      /* {{{ smoothing */

      susan_setup_brightness_lut( &bp, susan_bt, 2 );
      susan_smoothing( susan_three_by_three, in, susan_dt, x_size, y_size, bp );

      break;
    case 1:
      /* {{{ edges */

      r   = ( char * ) susan_wccmalloc( x_size * y_size );
      susan_setup_brightness_lut( &bp, susan_bt, 6 );

      if ( susan_principle_conf ) {
        if ( susan_three_by_three )
          susan_principle_small( in, r, bp, susan_max_no_edges, x_size, y_size );
        else
          susan_principle( in, r, bp, susan_max_no_edges, x_size, y_size );
        susan_int_to_uchar( r, in, x_size * y_size );
      } else {
        mid = ( uchar * )susan_wccmalloc( x_size * y_size );
        susan_wccmemset( mid, 100, x_size * y_size ); /* note not set to zero */

        if ( susan_three_by_three )
          susan_edges_small( in, r, mid, bp, susan_max_no_edges, x_size, y_size );
        else
          susan_edges( in, r, mid, bp, susan_max_no_edges, x_size, y_size );
        if ( susan_thin_post_proc )
          susan_thin( r, mid, x_size, y_size );
        susan_edge_draw( in, mid, x_size, y_size, susan_drawing_mode );
      }

      break;
    case 2:
      /* {{{ corners */

      r   = ( char * ) susan_wccmalloc( x_size * y_size );
      susan_setup_brightness_lut( &bp, susan_bt, 6 );

      if ( susan_principle_conf ) {
        susan_principle( in, r, bp, susan_max_no_corners, x_size, y_size );
        susan_int_to_uchar( r, in, x_size * y_size );
      } else {
        if ( susan_susan_quick )
          susan_corners_quick( in, r, bp, susan_max_no_corners, corner_list, x_size,
                               y_size );
        else
          susan_corners( in, r, bp, susan_max_no_corners, corner_list, x_size, y_size );
        susan_corner_draw( in, corner_list, x_size, susan_drawing_mode );
      }

      break;
  }

  susan_put_image( x_size, y_size );
}

void susan_init( void )
{
  volatile int a = 0;
  susan_file.data = susan_input;
  susan_file.size = 7292;
  susan_file.size += a;
  susan_file.cur_pos = 0;
  susan_file.cur_pos += a;

  susan_dt = 4.0;
  susan_dt += a;
  susan_bt = 20;
  susan_bt += a;
  susan_principle_conf = 0;
  susan_principle_conf += a;
  susan_thin_post_proc = 1;
  susan_thin_post_proc += a;
  susan_three_by_three = 0;
  susan_three_by_three += a;
  susan_drawing_mode = 0;
  susan_drawing_mode += a;
  susan_susan_quick = 0;
  susan_susan_quick += a;
  susan_max_no_corners = 50;
  susan_max_no_corners += a;
  susan_max_no_edges = 50;
  susan_max_no_edges += a;

  // mode=0; /* Smoothing mode */
  // mode=1; /* Edges mode */
  // mode=2; /* Corners mode */

  // principle=1; /* Output initial enhancement image only; corners or edges mode (default is edges mode) */
  // thin_post_proc=0; /* No post-processing on the binary edge map (runs much faster); edges mode */
  // drawing_mode=1; /* Mark corners/edges with single black points instead of black with white border; corners or edges mode */
  // three_by_three=1; /* Use flat 3x3 mask, edges or smoothing mode */
  // susan_quick=1; /* Use faster (and usually stabler) corner mode; edge-like corner suppression not carried out; corners mode */
  // dt=10.0; /* Distance threshold, smoothing mode, (default=4) */
  // bt=50; /* Brightness threshold, all modes, (default=20) */
}

void _Pragma( "entrypoint" ) susan_main( void )
{
  susan_call_susan( &susan_file, 0 );
  susan_wccfreeall();
  susan_call_susan( &susan_file, 1 );
  susan_wccfreeall();
  susan_call_susan( &susan_file, 2 );
  susan_wccfreeall();
}

int susan_return( void )
{
  return 0;
}

int main( void )
{
  susan_init();
  susan_main();

  return susan_return();
}
