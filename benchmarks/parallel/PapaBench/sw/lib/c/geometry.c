/* 
   geometry.c
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

#include "geometry.h"

//#include <math.h>
#include "math_papabench.h"

const  struct ellipsoid ellipsoids[ELLIPSOID_NB] = {
   {-168.,  -60.,   320., 6378249.2, 0.0034075495234250643, 0.08248325676},  /* NTF   */
   {   0.,    0.,     0., 6378137.0, 0.0033528106647474805, 0.08181919106},  /* WGS84 */
   {-87.0,  -98., -121.0, 6378388.0, 0.003367003367003367,  0.08199188998},  /* ED50  */
   {  0.0, 125.0,  194.0, 6378067.6, 0.0033155460257,       0.08181919106}   /* NAD27 */
};
/* From http://www.tandt.be/wis/WiS/eqntf.html and http://www.ign.fr/MP/GEOD/geodesie/coordonnees.html */ 
const struct lambert lamberts[LAMBERT_NB] = {
  {                                           /* LAMBERTI   */
    &ellipsoids[NTF],
    RAD_OF_DEG(DECIMAL(49., 30., 0.)),    /* phi0    */
    0.991996665,                          /* lphi0   */
    5457616.674,                          /* r0      */
    RAD_OF_DEG(DECIMAL(2., 20., 14.025)), /* lambda0 */
    600000,                               /* x0      */
    200000,                               /* y0      */
    5657617,                              /* ys      */
    0.99987734                            /* k0      */
  },
  {                                           /* LAMBERTII  */
    &ellipsoids[NTF], 
    RAD_OF_DEG(DECIMAL(46., 48., 0.)),    /* phi0    */
    0.921557361,                          /* lphi0   */
    5999695.77,                           /* r0      */
    RAD_OF_DEG(DECIMAL(2., 20., 14.025)), /* lambda0 */
    600000,                               /* x0      */
    2200000,                              /* y0      */
    6199696,                              /* ys      */
    0.99987742                            /* k0      */
  },
  {                                           /* LAMBERTIIE */
    &ellipsoids[NTF], 
    RAD_OF_DEG(DECIMAL(46., 48., 0.)),    /* phi0    */
    0.921557361,                          /* lphi0   */
    5999695.77,                           /* r0      */
    RAD_OF_DEG(DECIMAL(2., 20., 14.025)), /* lambda0 */
    600000,                               /* x0      */
    2200000,                              /* y0      */
    8199696,                              /* ys      */
    0.99987742                            /* k0      */
  },
  {                                           /* LAMBERTIII */
    &ellipsoids[NTF], 
    RAD_OF_DEG(DECIMAL(44., 6., 0.)),    /* phi0    */
    0.921557361,                          /* lphi0   */
    6591905.08,                           /* r0      */
    RAD_OF_DEG(DECIMAL(2., 20., 14.025)), /* lambda0 */
    600000,                               /* x0      */
    3200000,                              /* y0      */
    6791905,                              /* ys      */
    0.99987750                            /* k0      */
  },
  {                                           /* LAMBERTIV  */
    &ellipsoids[NTF], 
    RAD_OF_DEG(DECIMAL(42., 9., 54.)),    /* phi0    */
    0.921557361,                          /* lphi0   */
    7053300.18,                           /* r0      */
    RAD_OF_DEG(DECIMAL(2., 20., 14.025)), /* lambda0 */
    234,                                  /* x0      */ /* FIXME mmmmmmm suspicious */
    4185861,                              /* y0      */
    7239162,                              /* ys      */
    0.99994471                            /* k0      */
  },
};

double distance_c2d(const struct double_c2d a, const struct double_c2d b) { 
  return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

struct double_c2d vector_c2d(const struct double_c2d a, const struct double_c2d b) {
  struct double_c2d v;
  v.x = b.x - a.x; 
  v.y = b.y - a.y;
  return v;
}

struct double_c2d unit_vector_c2d(const struct double_c2d a, const struct double_c2d b) {
  struct double_c2d v = vector_c2d(a, b);
  scale_c2d_s(&v, 1/module_c2d(v));
  return v;
}

struct double_c2d normal_vector_c2d(const struct double_c2d u) {
  struct double_c2d v;
  v.x = -u.y;
  v.y = u.x;
  return v;
}

double vect_prod_c2d(const struct double_c2d a, const struct double_c2d b) {
  return a.x * b.y - a.y * b.x;
}

double scal_prod_c2d(const struct double_c2d a, const struct double_c2d b) {
  return  a.x * b.x + a.y * b.y;
}

void rotate_c2d(struct double_c2d* a, double alpha) {
  double tmp;
  tmp  = a->x*cos(alpha)-a->y*sin(alpha);
  a->y = a->x*sin(alpha)+a->y*cos(alpha);
  a->x = tmp;
}

double norme2_c2d(const struct double_c2d a) {
  return a.x*a.x+a.y*a.y;
}

double module_c2d(const struct double_c2d a) {
  return sqrt(norme2_c2d(a));
}

void   translate_c2d(struct double_c2d* a, const struct double_c2d v) {
  add_c2d_s(a, v);
}

void add_c2d_s(struct double_c2d* a, const struct double_c2d b) {
  a->x = a->x + b.x;
  a->y = a->y + b.y;
}

struct double_c2d add_c2d(struct double_c2d a, const struct double_c2d b) {
  add_c2d_s(&a, b);
  return a;
}

void normalise_c2d_s(struct double_c2d* a) {
  scale_c2d_s(a, 1/module_c2d(*a));
}

struct double_c2d normalise_c2d(struct double_c2d a) {
  normalise_c2d_s(&a);
  return a;
}

void minus_c2d_s(struct double_c2d* a, const struct double_c2d b) {
  a->x = a->x - b.x;
  a->y = a->y - b.y;
}

struct double_c2d minus_c2d(struct double_c2d a, const struct double_c2d b) {
  minus_c2d_s(&a, b);
  return a;
}

void scale_c2d_s(struct double_c2d* v, double factor) {
  v->x = v->x*factor;
  v->y = v->y*factor;
}

struct double_c2d scale_c2d(struct double_c2d v, double factor) {
  scale_c2d_s(&v, factor);
  return v;
}



#define MPI_PI(val)\
{\
  while (val> M_PI) val -= 2. * M_PI;\
  while (val<-M_PI) val += 2. * M_PI;\
}

double mpi_pi(double val) {
  MPI_PI(val);
  return val;
};

struct double_c2d lin_interp_c2d(struct double_c2d a, double ta, struct double_c2d b, double tb, double t) {
  double delta_t = tb - ta;
  struct double_c2d res;
  res.x = a.x + (t-ta)/delta_t*(b.x-a.x);
  res.y = a.y + (t-ta)/delta_t*(b.y-a.y);
  return res;
}

double wind_dir_from_angle_deg(double deg) {
  double w = 180. + 90. - deg;
  while(w>360) w -= 360;
  return w;
}

double wind_dir_from_angle_rad(double rad) {
  double w = M_PI + M_PI_2 - rad;
  while(w>2*M_PI) w -= (2*M_PI);
  return w;
}

double heading_of_to_angle_deg(double angle) {
  double a = 90. - angle + 360.;
  while (a>=180.) a-=360.;
  return a;
}

double heading_of_to_angle_rad(double angle) {
  double two_pi = 2 * M_PI;
  double a = M_PI_2 - angle + two_pi;
  while (a >= two_pi) a-= two_pi;
  return a;
}



#define G 9.81
#define ACCEL_DOWN 0
double compute_phi(struct double_c2d curspeed, struct double_c2d last_speed, double delta_t) {
  double accel_norm, accel_tgt, phi;
  struct double_c2d accel = minus_c2d(curspeed, last_speed);
  //  printf("%lf %lf %lf %lf %lf\n", curspeed.x, curspeed.y, last_speed.x, last_speed.y, delta_t);
  scale_c2d_s( &accel, 1/delta_t);
  accel_norm = vect_prod_c2d(accel, curspeed) / module_c2d(curspeed); 
  accel_tgt  = scal_prod_c2d(accel, curspeed) / module_c2d(curspeed);
  phi = -atan2(accel_norm, G - ACCEL_DOWN);
  return phi;
}

double angle_c2d(struct double_c2d u, struct double_c2d v) {
  double alpha1 = atan2(u.y, u.x);
  double alpha2 = atan2(v.y, v.x);
  double alpha = alpha2 - alpha1;
  MPI_PI(alpha);
  return alpha;
}

struct double_p2d p2d_of_c2d(const struct double_c2d c) {
  struct double_p2d p;
  p.r = module_c2d(c);
  p.teta = atan2(c.y, c.x);
  return p;
}

struct double_c2d c2d_of_p2d(const struct double_p2d p) {
  struct double_c2d c;
  c.x = p.r * cos(p.teta);
  c.y = p.r * sin(p.teta);
  return c;
}

struct double_p2d add_p2d(const struct double_p2d p1, const struct double_p2d p2) {
/*   p.r = sqrt(p1.r*p1.r + p2.r*p2.r + 2 * p1.r * p2.r * cos ( p1.teta - p2.teta )); */
/*   p.teta = 0; */
  struct double_c2d c1 = c2d_of_p2d(p1);
  struct double_c2d c2 = c2d_of_p2d(p2);
  struct double_c2d c3 = add_c2d(c1, c2);
  struct double_p2d p = p2d_of_c2d(c3);
  return p;
}


struct double_p2d minus_p2d(const struct double_p2d p1, const struct double_p2d p2) {
  struct double_c2d c1 = c2d_of_p2d(p1);
  struct double_c2d c2 = c2d_of_p2d(p2);
  struct double_c2d c3 = minus_c2d(c1, c2);
  struct double_p2d p = p2d_of_c2d(c3);
  return p;
}


struct double_c2d c2d_of_polar(double r, double teta) {
  struct double_c2d res = {r*cos(teta), r*sin(teta)};
  return res;
}

void  translate_c3d(struct double_c3d* a, const struct double_c3d v) {
  a->x = a->x + v.x;
  a->y = a->y + v.y;
  a->z = a->z + v.z;
}

void  scale_c3d(struct double_c3d* v, double factor) {
  v->x = v->x*factor;
  v->y = v->y*factor;
  v->z = v->z*factor;
}

static double inverse_latitude_isometrique(double lat, double e, double epsilon) {
  double exp_l = exp(lat);
  double phi0 = 2. * atan(exp_l -M_PI_2);
  double loop(double phi) {
    double sin_phi = e * sin(phi);
    double new_phi = 2. * atan (pow((1. + sin_phi) / (1. - sin_phi), e/2) * exp_l) - M_PI_2;
    if (fabs(phi - new_phi) < epsilon) return new_phi;
    return loop(new_phi);
  }
  return loop(phi0);
}

static double lambert_n (const enum type_lambert lmb) { return sin(lamberts[lmb].phi0); }
static double lambert_c (const enum type_lambert lmb) { 
  return lamberts[lmb].r0 * exp(lamberts[lmb].phi0 * lambert_n(lmb));
}

void lambert_of_e2d (const enum type_lambert lmb, const enum type_ellipsoid elps, 
		     struct double_e2d e2d_pos, struct double_c2d *lmb_pos) {
  double n, ll, r, gamma;
  e2d_of_e2d(NTF, elps, &e2d_pos);
  n  = lambert_n(lmb);
  ll = LATITUDE_ISOMETRIQUE(e2d_pos.lat, lamberts[lmb].ellipsoid->e);
  r  = lamberts[lmb].r0 * exp(lamberts[lmb].lphi0 * sin(lamberts[lmb].phi0)) * exp(-ll*n);
  gamma = (e2d_pos.lon - lamberts[lmb].lambda0) * n;
  lmb_pos->x = (double)lamberts[lmb].x0 + r * sin(gamma);
  lmb_pos->y = (double)lamberts[lmb].ys - r * cos(gamma);
}

void e2d_of_lambert (const enum type_lambert lmb, const enum type_ellipsoid elps,
		     struct double_c2d c_pos, struct double_e2d *e_pos) {
  double dx = c_pos.x - lamberts[lmb].x0;
  double dy = c_pos.y - lamberts[lmb].ys;
  double r = sqrt(dx*dx + dy*dy);
  double gamma = atan2(dx, -dy);
  double lambda = lamberts[lmb].lambda0 + gamma / lambert_n(lmb);
  double ll = -1. / lambert_n(lmb) * log(fabs(r/lambert_c(lmb)));
  double phi = inverse_latitude_isometrique(ll, lamberts[lmb].ellipsoid->e, 1e-11);
  e_pos->lon = lambda; e_pos->lat = phi;
  e2d_of_e2d(elps, NTF, e_pos);
}

void e2d_of_e2d(const enum type_ellipsoid dest, const enum type_ellipsoid src, struct double_e2d *e2d_pos) {
  if (dest != src) {
    double sin_lat = sin(e2d_pos->lat);
    double cos_lat = cos(e2d_pos->lat);
    double sin_lon = sin(e2d_pos->lon);
    double cos_lon = cos(e2d_pos->lon);
    double d16 = E_SQUARE(ellipsoids[src].df);
    double d17 = E_SQUARE(ellipsoids[dest].df);
    double d18 = ellipsoids[src].a / sqrt (1.0 - d16 * sin_lat * sin_lat);
    double d20 = d18 * cos_lat * cos_lon;
    double d21 = d18 * cos_lat * sin_lon;
    double d22 = d18 * (1.0 - d16) * sin_lat;
    double d23 = d20 - ellipsoids[dest].dx + ellipsoids[src].dx;
    double d24 = d21 - ellipsoids[dest].dy + ellipsoids[src].dy;
    double d25 = d22 - ellipsoids[dest].dz + ellipsoids[src].dz;
    double d26 = sqrt (d23 * d23 + d24 * d24);
    double d27 = atan2(d25, d26*(1.0 - ellipsoids[dest].df));
    double d28 = ellipsoids[dest].a *(1.0 - ellipsoids[dest].df);
    double d29 = E_PRIME_SQUARE(ellipsoids[dest].df);
    double d3 = atan2((d25 + d29 * d28 * sin(d27) * sin(d27) * sin(d27)), 
		      (d26 - d17 * ellipsoids[dest].a * cos(d27) * cos(d27) * cos(d27)));
    double d4 = atan2(d24, d23);
    e2d_pos->lon = d4;
    e2d_pos->lat = d3;
  }
}


/* 
   converts 3d cartesian coordinate to 3d ellipsoidal
*/
void e3d_of_c3d(const enum type_ellipsoid elps, struct double_c3d c_pos, struct double_e3d *e_pos) {
#define WGS_84_SMA 6378137.00
#define WGS_84_SMB 6356752.31
#define E2 ((WGS_84_SMA*WGS_84_SMA-WGS_84_SMB*WGS_84_SMB)/(WGS_84_SMA*WGS_84_SMA))
#define PREC 0.000000000001
  double dist   = sqrt(c_pos.x*c_pos.x+c_pos.y*c_pos.y);
  double v, old_phi, new_phi = atan2((double)c_pos.z, dist*(1-E2));
  do {
    old_phi = new_phi;
    v = WGS_84_SMA / sqrt(1 - (E2 * sin(old_phi) * sin(old_phi)));
    new_phi = atan2(((double)c_pos.z + E2 * v * sin (old_phi)), dist);
  }
  while (fabs(new_phi-old_phi)>PREC);
  e_pos->lat = new_phi;
  e_pos->lon = atan2(c_pos.y,c_pos.x);
  e_pos->h = (dist / cos (new_phi)) - v;
}



