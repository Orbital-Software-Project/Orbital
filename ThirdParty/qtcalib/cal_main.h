#ifndef CAL_MAIN_H
#define CAL_MAIN_H

#include "matrix.h"
#include "defs.h"


/* An arbitrary tolerance factor */
#define EPSILON		1.0E-8

/* Commonly used macros */
#define SIGNBIT(a)      (((a) > 0) ? 0 : 1)
#define CUB(a)          ((a)*(a)*(a))


/* Some math libraries may not have the sincos() routine */
#ifdef _SINCOS_
void sincos();
#define SINCOS(x,s,c)   sincos(x,&s,&c)
#else
/*double sin(double), cos(double);*/
#define SINCOS(x,s,c)   s=(double)sin(x);c=(double)cos(x)
#endif


/* Parameters controlling MINPACK's lmdif() optimization routine. */
/* See the file lmdif.f for definitions of each parameter.        */
#define REL_SENSOR_TOLERANCE_ftol    0.00001f
#define REL_PARAM_TOLERANCE_xtol     0.0000001f
#define ORTHO_TOLERANCE_gtol         0.0
#define MAXFEV                       (1000*n)
#define EPSFCN                       1.0E-16     /* Do not set to zero! */
#define MODE                         1           /* variables are scalled internally */
#define FACTOR                       100.0 

typedef struct{
    double    Ncx;		/* [sel]     Number of sensor elements in camera's x direction */
    double    Nfx;		/* [pix]     Number of pixels in frame grabber's x direction   */
    double    dx;		/* [mm/sel]  X dimension of camera's sensor element (in mm)    */
    double    dy;		/* [mm/sel]  Y dimension of camera's sensor element (in mm)    */
    double    dpx;		/* [mm/pix]  Effective X dimension of pixel in frame grabber   */
    double    dpy;		/* [mm/pix]  Effective Y dimension of pixel in frame grabber   */
    double    Cx;		/* [pix]     Z axis intercept of camera coordinate system      */
    double    Cy;		/* [pix]     Z axis intercept of camera coordinate system      */
    double    sx;		/* []        Scale factor to compensate for any error in dpx   */
} camera_parameters;

typedef struct {
    int        point_count;	/* [points] 	 */
    double     xw[MAX_POINTS];	/* [mm]          */
    double     yw[MAX_POINTS];	/* [mm]          */
    double     zw[MAX_POINTS];	/* [mm]          */
    double     Xf[MAX_POINTS];	/* [pix]         */
    double     Yf[MAX_POINTS];	/* [pix]         */
} calibration_data;

typedef struct {
    double     f;		/* [mm]          */
    double     kappa1;	        /* [1/mm^2]      */
    double     p1;		/* [1/mm]        */
    double     p2;		/* [1/mm]        */
    double     Tx;		/* [mm]          */
    double     Ty;		/* [mm]          */
    double     Tz;		/* [mm]          */
    double     Rx;		/* [rad]	 */
    double     Ry;		/* [rad]	 */
    double     Rz;		/* [rad]	 */
    double     r1;		/* []            */
    double     r2;		/* []            */
    double     r3;		/* []            */
    double     r4;		/* []            */
    double     r5;		/* []            */
    double     r6;		/* []            */
    double     r7;		/* []            */
    double     r8;		/* []            */
    double     r9;		/* []            */
} calibration_constants;


void initialize_standard_camera (camera_parameters * cp, int x, int y);
int  coplanar_calibration (calibration_data cd,camera_parameters cp,double *Kc, double *Rc,double *Tc);

int  coplanar_calibration_with_full_optimization (calibration_data cd,camera_parameters cp,
												  double *Kc, double *Rc,double *Tc);

void world_coord_to_image_coord (double ,double ,double , int, int, double *,double *);
void image_coord_to_world_coord (double Xfd, double Yfd, double zw, int cx, int cy, double *xw, double *yw);


//Colocamos global pra ser utilizada pelo Tsai do Dragos
void solve_RPY_transform (void);
void apply_RPY_transform (void);


#endif // CAL_MAIN_H
