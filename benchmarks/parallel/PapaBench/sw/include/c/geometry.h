/* 
   geometry.h
   Copyright (C) 2003 Antoine Drouin

 This file is part of paparazzi.

 paparazzi is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 paparazzi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with paparazzi; see the file COPYING.  If not, write to
 the Free Software Foundation, 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.  
*/ 

/*
  geometry :) originated from Pascal Brisset pascal(dot)brisset(at)free(dot)fr
  ocaml code.
*/

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>
#include "angles.h"
/* 
   please use 
   x, y, z, h in meters
   lat, lon in radians
*/
struct double_c2d {
  double x, y;
};

struct double_p2d {
  double r, teta;
};

struct double_e2d {
  double lat, lon;
};

struct double_c3d {
  double x, y, z;
};

struct double_e3d {
  double lat, lon, h;
};



struct double_c2d vector_c2d(const struct double_c2d a, const struct double_c2d b);
struct double_c2d unit_vector_c2d(const struct double_c2d a, const struct double_c2d b);
struct double_c2d normal_vector_c2d(const struct double_c2d u);

double vect_prod_c2d(const struct double_c2d a, const struct double_c2d b);
double scal_prod_c2d(const struct double_c2d a, const struct double_c2d b);  
double module_c2d(const struct double_c2d a);
double norme2_c2d(const struct double_c2d a);
double distance_c2d(const struct double_c2d a, const struct double_c2d b);
void   rotate_c2d(struct double_c2d* a, double alpha);
void   translate_c2d(struct double_c2d* a, const struct double_c2d v);
void   add_c2d_s(struct double_c2d* a, const struct double_c2d b);
struct double_c2d add_c2d(struct double_c2d a, struct double_c2d b);
void   minus_c2d_s(struct double_c2d* a, const struct double_c2d v);
struct double_c2d minus_c2d(struct double_c2d a, const struct double_c2d v);
void   normalise_c2d_s(struct double_c2d* a);
struct double_c2d normalise_c2d(struct double_c2d a);
void   scale_c2d_s(struct double_c2d* v, double factor);
struct double_c2d scale_c2d(struct double_c2d v, double factor);
double angle_c2d(struct double_c2d u, struct double_c2d v);

struct double_p2d p2d_of_c2d(const struct double_c2d c);
struct double_c2d c2d_of_p2d(const struct double_p2d p);
struct double_c2d c2d_of_polar(double r, double teta);
struct double_p2d add_p2d(const struct double_p2d p1, const struct double_p2d p2);
struct double_p2d minus_p2d(const struct double_p2d p1, const struct double_p2d p2);

void  translate_c3d(struct double_c3d* a, const struct double_c3d v);
void  scale_c3d(struct double_c3d* v, double factor);

struct double_c2d lin_interp_c2d(struct double_c2d a, double ta, struct double_c2d b, double tb, double t);


/* rad is an angle in EAST NORTH frame, 
 * returns the wind direction in degrees
 */
double wind_dir_from_angle_rad(double rad);

/* converts between angle in EAST NORTH frame
   and headings (0->N 90->E)               */
double heading_of_to_angle_deg(double angle);
double heading_of_to_angle_rad(double angle);

/*
 * compute bank angle
 */
double compute_phi(struct double_c2d curspeed, struct double_c2d last_speed, double delta_t);

#define SEC_OF_TIMEVAL(tv) (tv.tv_sec + 1e-6 * tv.tv_usec)
#define DELAY_SEC_OF_TIMEVAL(tv1, tv2) (SEC_OF_TIMEVAL(tv1) - SEC_OF_TIMEVAL(tv2))

double mpi_pi(double val);

struct ellipsoid {
  double dx, dy, dz;
  double a;
  double df;
  double e;
};

enum type_ellipsoid {
  NTF,
  WGS84,
  ED50,
  NAD27,
  ELLIPSOID_NB
};

extern const struct ellipsoid ellipsoids[ELLIPSOID_NB];

struct lambert {
  const struct ellipsoid*  ellipsoid;
  double phi0;                  /* radians */
  double lphi0;
  double r0;
  double lambda0;
  long x0;
  long y0;
  long ys;
  long k0;                      /* scale factor */
};


enum type_lambert {
  LAMBERTI,
  LAMBERTII,
  LAMBERTIIE,
  LAMBERTIII,
  LAMBERTIV,
  LAMBERT_NB
};

extern const struct lambert lamberts[LAMBERT_NB];


#define DECIMAL(D,M,S) (D + M / 60. + S / 3600.)
#define RAD_OF_DEG(D) (D*M_PI/180.)
#define DEG_OF_RAD(R) (R/M_PI*180.)
#define LATITUDE_ISOMETRIQUE(PHI,E) (log(tan(M_PI_4 + PHI/2.0)) - E/2.0 * log((1.0+E*sin(PHI)) / (1.0-E*sin(PHI)))) 
#define E_SQUARE(D) (2.0*D - D * D) 
#define E_PRIME_SQUARE(D) (1.0 / (1.0 - D) / (1.0 - D) - 1.0)




/* projections */
void lambert_of_e2d (const enum type_lambert lmb, const enum type_ellipsoid elps, 
		     struct double_e2d world_pos, struct double_c2d *lmb_pos);
void e2d_of_lambert (const enum type_lambert lmb, const enum type_ellipsoid elps,
		     struct double_c2d c_pos, struct double_e2d *e_pos);
/* coordinates */
void e3d_of_c3d(const enum type_ellipsoid elps, struct double_c3d c_pos, struct double_e3d *e_pos);
void e2d_of_e2d(const enum type_ellipsoid dest, const enum type_ellipsoid src, struct double_e2d *e2d_pos);


#endif /* GEOMETRY_H */
