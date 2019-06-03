#ifndef MATH_H_
#define MATH_H_

#define M_PI 3.141592653589793238462643
#define HALFPI 1.57079632679489661923
#define sin(x)    (pp_sin(x))
#define atan2(x,y)  (pp_atan2(x,y))
#define sqrt(x)   (pp_sqrt(x))
#define cos(x)    (pp_sin(x+HALFPI))  /*cosinus [ radians ]*/
#define fabs(x)   (pp_fabs(x))
#define pp_fabs(x)  ((x)< 0. ? -(x) : (x))  /*floating absolute value*/
#define Max(x,y)  (pp_Max(x,y))
#define pp_Max(x,y) ((x) >= (y) ? (x) : (y))
#define Min(x,y)  (pp_Min(x,y))
#define pp_Min(x,y) ((x) <= (y) ? (x) : (y))

extern double pp_sin( double x );
extern double pp_sqrt( double x );
extern double pp_atan2( double x, double y );

#endif /*MATH_H_*/
