#include <string.h>
#include <math.h>
#include "cal_main.h"

extern int lmdif_(int (*fcn) (double *params, double *err), long int *m, long int *n, double *x, double *fvec, double *ftol, double *xtol, double *gtol, long int *maxfev, 
	double *epsfcn, double *diag, long int *mode, double *factor, long int *nprint, long int *info, long int *nfev, double *fjac, long int *ldfjac, long int *ipvt, 
	double *qtf, double *wa1, double *wa2, double *wa3, double *wa4);

/* Variables used by the subroutines for I/O (perhaps not the best way of doing this) */
calibration_data cd;
calibration_constants cc;
camera_parameters cp;

/* Local working storage */
double    Xd[MAX_POINTS],
          Yd[MAX_POINTS],
          r_squared[MAX_POINTS],
          U[7];


void initialize_standard_camera (camera_parameters * cp, int x, int y)
{ 
  cp->Ncx = x;    /* [sel]        */ 
  cp->Nfx = x;    /* [pix]        */ 
  cp->dx  = 1;    /* [mm/sel]     */ 
  cp->dy  = 1;    /* [mm/sel]     */ 
  cp->dpx = 1;    /* [mm/pix]     */ 
  cp->dpy = 1;    /* [mm/pix]     */ 
  cp->Cx  = x/2;  /* [pix]        */ 
  cp->Cy  = y/2;  /* [pix]        */ 
  cp->sx  = 1.0;  /* []   */ 
} 

/***********************************************************************\
* This routine solves for the roll, pitch and yaw angles (in radians)   *
* for a given orthonormal rotation matrix (from Richard P. Paul,        *
* Robot Manipulators: Mathematics, Programming and Control, p70).       *
* Note 1, should the rotation matrix not be orthonormal these will not  *
* be the "best fit" roll, pitch and yaw angles.                         *
* Note 2, there are actually two possible solutions for the matrix.     *
* The second solution can be found by adding 180 degrees to Rz before   *
* Ry and Rx are calculated.                                             *
\***********************************************************************/
void solve_RPY_transform ()
{
 double sg, cg;

 cc.Rz = atan2 (cc.r4, cc.r1);
 SINCOS (cc.Rz, sg, cg);
 cc.Ry = atan2 (-cc.r7, cc.r1 * cg + cc.r4 * sg);
 cc.Rx = atan2 (cc.r3 * sg - cc.r6 * cg, cc.r5 * cg - cc.r2 * sg);
}

/************************************************************************\
* This routine simply takes the roll, pitch and yaw angles and fills in	 *
* the rotation matrix elements r1-r9.					 *
\************************************************************************/
void apply_RPY_transform ()
{
 double sa, ca, sb, cb, sg, cg;

 SINCOS (cc.Rx, sa, ca);
 SINCOS (cc.Ry, sb, cb);
 SINCOS (cc.Rz, sg, cg);

 cc.r1 = cb * cg;
 cc.r2 = cg * sa * sb - ca * sg;
 cc.r3 = sa * sg + ca * cg * sb;
 cc.r4 = cb * sg;
 cc.r5 = sa * sb * sg + ca * cg;
 cc.r6 = ca * sb * sg - cg * sa;
 cc.r7 = -sb;
 cc.r8 = cb * sa;
 cc.r9 = ca * cb;
}

/***********************************************************************\
* Routines for coplanar camera calibration	 			                        *
\***********************************************************************/
static void cc_compute_Xd_Yd_and_r_squared (void)
{
 int   i;
 double Xd_, Yd_;

 for (i = 0; i < cd.point_count; i++) {
  Xd[i] = Xd_ = cd.Xf[i] - cp.Cx;
  Yd[i] = Yd_ = cd.Yf[i] - cp.Cy;
  r_squared[i] = SQR (Xd_) + SQR (Yd_);
 }
}

static void cc_compute_U ()
{
 int  i;
 double A[MAX_POINTS][5];
 double B[MAX_POINTS];
 for (i = 0; i < cd.point_count; i++) {
  A[i][0] = Yd[i] * cd.xw[i];
  A[i][1] = Yd[i] * cd.yw[i];
  A[i][2] = Yd[i];
  A[i][3] = -Xd[i] * cd.xw[i];
  A[i][4] = -Xd[i] * cd.yw[i];
  B[i] = Xd[i];
 }

 MinQuad5(cd.point_count, A, B);
 U[0] = B[0];
 U[1] = B[1];
 U[2] = B[2];
 U[3] = B[3];
 U[4] = B[4];
}

static void cc_compute_Tx_and_Ty ()
{
 int   i, far_point;
 double Tx, Ty, Ty_squared, x, y, Sr, r1p, r2p, r4p, r5p, r1, r2, r4, r5, distance, far_distance;

 r1p = U[0];
 r2p = U[1];
 r4p = U[3];
 r5p = U[4];

 /* first find the square of the magnitude of Ty */
 if ((fabs (r1p) < EPSILON) && (fabs (r2p) < EPSILON))
  Ty_squared = 1 / (SQR (r4p) + SQR (r5p));
 else if ((fabs (r4p) < EPSILON) && (fabs (r5p) < EPSILON))
  Ty_squared = 1 / (SQR (r1p) + SQR (r2p));
 else if ((fabs (r1p) < EPSILON) && (fabs (r4p) < EPSILON))
  Ty_squared = 1 / (SQR (r2p) + SQR (r5p));
 else if ((fabs (r2p) < EPSILON) && (fabs (r5p) < EPSILON))
  Ty_squared = 1 / (SQR (r1p) + SQR (r4p));
 else {
  Sr = SQR (r1p) + SQR (r2p) + SQR (r4p) + SQR (r5p);
  Ty_squared = ((Sr - sqrt (SQR (Sr) - 4 * SQR (r1p * r5p - r4p * r2p))) / (2 * SQR (r1p * r5p - r4p * r2p)));
 }

 /* find a point that is far from the image center */
 far_distance = 0.0;
 far_point = 0;
 for (i = 0; i < cd.point_count; i++)
  if ((distance = r_squared[i]) > far_distance) {
   far_point = i;
   far_distance = distance;
  }

 /* now find the sign for Ty */
 /* start by assuming Ty > 0 */
 Ty = sqrt (Ty_squared);
 r1 = U[0] * Ty;
 r2 = U[1] * Ty;
 Tx = U[2] * Ty;
 r4 = U[3] * Ty;
 r5 = U[4] * Ty;
 x = r1 * cd.xw[far_point] + r2 * cd.yw[far_point] + Tx;
 y = r4 * cd.xw[far_point] + r5 * cd.yw[far_point] + Ty;

 /* flip Ty if we guessed wrong */
 if ((SIGNBIT (x) != SIGNBIT (Xd[far_point])) || (SIGNBIT (y) != SIGNBIT (Yd[far_point])))
  Ty = -Ty;

 /* update the calibration constants */
 cc.Tx = U[2] * Ty;
 cc.Ty = Ty;
}

////////////////////////////////////////////////////
static void cc_compute_R ()
{
 double r1, r2, r3, r4, r5, r6, r7, r8, r9;

 r1 = U[0] * cc.Ty;
 r2 = U[1] * cc.Ty;
 r3 = sqrt (1 - SQR (r1) - SQR (r2));

 r4 = U[3] * cc.Ty;
 r5 = U[4] * cc.Ty;
 r6 = sqrt (1 - SQR (r4) - SQR (r5));
 if (!SIGNBIT (r1 * r4 + r2 * r5))
  r6 = -r6;

 /* use the outer product of the first two rows to get the last row */
 r7 = r2 * r6 - r3 * r5;
 r8 = r3 * r4 - r1 * r6;
 r9 = r1 * r5 - r2 * r4;

 /* update the calibration constants */
 cc.r1 = r1;
 cc.r2 = r2;
 cc.r3 = r3;
 cc.r4 = r4;
 cc.r5 = r5;
 cc.r6 = r6;
 cc.r7 = r7;
 cc.r8 = r8;
 cc.r9 = r9;

 /* fill in cc.Rx, cc.Ry and cc.Rz */
 solve_RPY_transform ();
}

/////////////////////////////////////////////////////////
static void cc_compute_approximate_f_and_Tz ()
{
 int  i;
 double A[2*MAX_POINTS][2];
 double B[2*MAX_POINTS];
 for (i = 0; i < cd.point_count; i++) {
  A[i][0] = cc.r4 * cd.xw[i] + cc.r5 * cd.yw[i] + cc.Ty;
  A[i][1] = -Yd[i];
  B[i] = (cc.r7 * cd.xw[i] + cc.r8 * cd.yw[i]) * Yd[i];
 }
 MinQuad(cd.point_count, A, B);
 cc.f = B[0];
 cc.Tz = B[1];
 cc.kappa1 = 0.0;
}

///////////////////////////////////////////////////////////////////////////
//incluimos cx e cy para tornar compativel com lmdif

static int cc_compute_exact_f_and_Tz_error (double *params,double *err)
{
 int   i;
 double f, Tz, xc, yc, zc, Xu_1, Yu_1,Xu_2,Yu_2,kappa1,distortion_factor;

 f = params[0];
 Tz = params[1];
 kappa1 = params[2];

 for (i = 0; i < cd.point_count; i++) {
  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  xc = cc.r1 * cd.xw[i] + cc.r2 * cd.yw[i] + cc.Tx;
  yc = cc.r4 * cd.xw[i] + cc.r5 * cd.yw[i] + cc.Ty;
  zc = cc.r7 * cd.xw[i] + cc.r8 * cd.yw[i] + Tz;

  /* convert from camera coordinates to undistorted sensor coordinates */
  Xu_1 = f * xc / zc;
  Yu_1 = f * yc / zc;

  /* convert from distorted sensor coordinates to undistorted sensor coordinates */
  distortion_factor = 1 + kappa1 * (SQR (Xd[i]) + SQR (Yd[i]));
  Xu_2 = Xd[i] * distortion_factor;
  Yu_2 = Yd[i] * distortion_factor;

  /* record the error in the undistorted sensor coordinates */
  err[i] = hypot (Xu_1 - Xu_2, Yu_1 - Yu_2);
 }
 return 1;
}

////////////////////////////////////////////////////////////////

static int cc_compute_exact_f_and_Tz ()
{
#define NPARAMS 3 //2

 int       i;

 /* Parameters needed by MINPACK's lmdif() */
 long int    m = cd.point_count;
 long int    n = NPARAMS;
 double       x[NPARAMS];
 double      *fvec;
 double       ftol = REL_SENSOR_TOLERANCE_ftol;
 double       xtol = REL_PARAM_TOLERANCE_xtol;
 double       gtol = ORTHO_TOLERANCE_gtol;
 long int    maxfev = MAXFEV;
 double       epsfcn = EPSFCN;
 double       diag[NPARAMS];
 long int    mode = MODE;
 double       factor = FACTOR;
 long int    nprint = 0;
 long int    info;
 long int    nfev;
 double      *fjac;
 long int    ldfjac = m;
 long int    ipvt[NPARAMS];
 double       qtf[NPARAMS];
 double       wa1[NPARAMS];
 double       wa2[NPARAMS];
 double       wa3[NPARAMS];
 double      *wa4;

 fvec = (double *)calloc(m, sizeof(double));
 fjac = (double *)calloc(m*n, sizeof(double));
 wa4 = (double *)calloc(m, sizeof(double));

 /* use the current calibration constants as an initial guess */
 x[0] = cc.f;
 x[1] = cc.Tz;
 x[2] = cc.kappa1;

 /* define optional scale factors for the parameters */
 if (mode == 2)
  for (i = 0; i < NPARAMS; i++)
   diag[i] = 1.0;             /* some user-defined values */

 /* perform the optimization */ 
 if (!lmdif_ (cc_compute_exact_f_and_Tz_error,
             &m, &n, x, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
             diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
             ipvt, qtf, wa1, wa2, wa3, wa4)) return 0;

 /* update the calibration constants */
 cc.f = x[0];
 cc.Tz = x[1];
 cc.kappa1 = x[2];

 /* release allocated workspace */
 free (fvec);
 free (fjac);
 free (wa4);
 return 1;

#undef NPARAMS
}

static int cc_nic_optimization_error (double *params, double *err)
{
 int   i;
 double xc, yc, zc, Xd_, Yd_, Xu_1, Yu_1, Xu_2, Yu_2,Rx, Ry, Rz, Tx, Ty, Tz, 
	    kappa1,f, r1, r2, r4, r5, r7, r8, sa, sb, sg, ca, cb, cg,distortion_factor;

 Rx = params[0];
 Ry = params[1];
 Rz = params[2];
 Tx = params[3];
 Ty = params[4];
 Tz = params[5];
 kappa1=params[6];
 f = params[7];

 SINCOS (Rx, sa, ca);
 SINCOS (Ry, sb, cb);
 SINCOS (Rz, sg, cg);
 r1 = cb * cg;
 r2 = cg * sa * sb - ca * sg;
 r4 = cb * sg;
 r5 = sa * sb * sg + ca * cg;
 r7 = -sb;
 r8 = cb * sa;

 for (i = 0; i < cd.point_count; i++) {
  /* convert from world coordinates to camera coordinates */
  /* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
  xc = r1 * cd.xw[i] + r2 * cd.yw[i] + Tx;
  yc = r4 * cd.xw[i] + r5 * cd.yw[i] + Ty;
  zc = r7 * cd.xw[i] + r8 * cd.yw[i] + Tz;

  /* convert from camera coordinates to undistorted sensor plane coordinates */
  Xu_1 = f * xc / zc;
  Yu_1 = f * yc / zc;

  /* convert from 2D image coordinates to distorted sensor coordinates */
  Xd_ = cp.dpx * (cd.Xf[i] - cp.Cx) / cp.sx; 
  Yd_ = cp.dpy * (cd.Yf[i] - cp.Cy);

   /* convert from distorted sensor coordinates to undistorted sensor plane coordinates */
   distortion_factor = 1 + kappa1 * (SQR (Xd_) + SQR (Yd_));
   Xu_2 = Xd_ * distortion_factor;
   Yu_2 = Yd_ * distortion_factor;
 
   /* record the error in the undistorted sensor coordinates */
    err[i] = hypot (Xu_1 - Xu_2, Yu_1 - Yu_2);
 }
 return 1;
}

/////////////////////////////////////////////////////////////
static int cc_nic_optimization ()
{
#define NPARAMS 8//7

 int       i;

 /* Parameters needed by MINPACK's lmdif() */

 long int    m = cd.point_count;
 long int    n = NPARAMS;
 double       x[NPARAMS];
 double      *fvec;
 double       ftol = REL_SENSOR_TOLERANCE_ftol;
 double       xtol = REL_PARAM_TOLERANCE_xtol;
 double       gtol = ORTHO_TOLERANCE_gtol;
 long int    maxfev = MAXFEV;
 double       epsfcn = EPSFCN;
 double       diag[NPARAMS];
 long int    mode = MODE;
 double       factor = FACTOR;
 long int    nprint = 0;
 long int    info;
 long int    nfev;
 double      *fjac;
 long int    ldfjac = m;
 long int    ipvt[NPARAMS];
 double       qtf[NPARAMS];
 double       wa1[NPARAMS];
 double       wa2[NPARAMS];
 double       wa3[NPARAMS];
 double      *wa4;

 if (( fvec = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )  return 0;
 if (( fjac = (double *) calloc ((unsigned int) m*n, (unsigned int) sizeof(double))) == NULL){free(fvec); return 0;}
 if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )	{free(fjac); free(fvec); return 0;}

 /* use the current calibration and camera constants as a starting point */
 x[0] = cc.Rx;
 x[1] = cc.Ry;
 x[2] = cc.Rz;
 x[3] = cc.Tx;
 x[4] = cc.Ty;
 x[5] = cc.Tz;
 x[6] = cc.kappa1;
 x[7] = cc.f;

 /* define optional scale factors for the parameters */
 if ( mode == 2 )
  for (i = 0; i < NPARAMS; i++)
   diag[i] = 1.0;             /* some user-defined values */

 /* perform the optimization */
 if (!lmdif_ (cc_nic_optimization_error,
              &m, &n, x, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
              diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
              ipvt, qtf, wa1, wa2, wa3, wa4)) return 0;

 /* update the calibration and camera constants */
 cc.Rx = x[0];
 cc.Ry = x[1];
 cc.Rz = x[2];
 apply_RPY_transform ();

 cc.Tx = x[3];
 cc.Ty = x[4];
 cc.Tz = x[5];
 cc.kappa1 = x[6];
 cc.f = x[7];

 /* release allocated workspace */
 free (fvec);
 free (fjac);
 free (wa4);
 return 1;

#undef NPARAMS
}

////////////////////////////////////////////////////
int  cc_three_parm_optimization ()
{
 cc_compute_Xd_Yd_and_r_squared ();
 cc_compute_U ();
 cc_compute_Tx_and_Ty ();
 cc_compute_R ();
 cc_compute_approximate_f_and_Tz ();

 if (cc.f < 0.0) {
  /* try the other solution for the orthonormal matrix */
  cc.r3 = -cc.r3;
  cc.r6 = -cc.r6;
  cc.r7 = -cc.r7;
  cc.r8 = -cc.r8;
  solve_RPY_transform ();
  cc_compute_approximate_f_and_Tz ();
  if (cc.f < 0.0)  return 0;
 }
 return cc_compute_exact_f_and_Tz ();
}
///////////////////////////////////////////////////////////////
static int  cc_five_parm_optimization_with_late_distortion_removal_error (double *params, double *err)
{
    int       i;
    double    f,Tz,kappa1,xc,yc,zc,Xu_1,Yu_1,Xu_2,Yu_2,distortion_factor;

    /* in this routine radial lens distortion is only taken into account */
    /* after the rotation and translation constants have been determined */

    f = params[0];
    Tz = params[1];
    kappa1 = params[2];

    cp.Cx = params[3];
    cp.Cy = params[4];

    cc_compute_Xd_Yd_and_r_squared ();
    cc_compute_U ();
    cc_compute_Tx_and_Ty ();
    cc_compute_R ();
    cc_compute_approximate_f_and_Tz ();

    if (cc.f < 0.0) { 		/* try the other solution for the orthonormal matrix */
		cc.r3 = -cc.r3;
		cc.r6 = -cc.r6;
		cc.r7 = -cc.r7;
		cc.r8 = -cc.r8;
		solve_RPY_transform ();

		cc_compute_approximate_f_and_Tz ();
                if (cc.f < 0.0) return 0;
	}

	/* convert from world coordinates to camera coordinates */
	/* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
    for (i = 0; i < cd.point_count; i++) {
		xc = cc.r1 * cd.xw[i] + cc.r2 * cd.yw[i] + cc.Tx;
		yc = cc.r4 * cd.xw[i] + cc.r5 * cd.yw[i] + cc.Ty;
		zc = cc.r7 * cd.xw[i] + cc.r8 * cd.yw[i] + Tz;

		/* convert from camera coordinates to undistorted sensor coordinates */
		Xu_1 = f * xc / zc;
		Yu_1 = f * yc / zc;

		/* convert from distorted sensor coordinates to undistorted sensor coordinates */
		distortion_factor = 1 + kappa1 * r_squared[i];
		Xu_2 = Xd[i] * distortion_factor;
		Yu_2 = Yd[i] * distortion_factor;

		/* record the error in the undistorted sensor coordinates */
		err[i] = hypot (Xu_1 - Xu_2, Yu_1 - Yu_2);
    }
	return 1;
}

////////////////////////////////////////////////////////////////
static int  cc_five_parm_optimization_with_late_distortion_removal ()
{
#define NPARAMS 5

    int       i;
     /* Parameters needed by MINPACK's lmdif() */
    long int  m = cd.point_count, n = NPARAMS;
    double  x[NPARAMS],*fvec, ftol = REL_SENSOR_TOLERANCE_ftol;
    double  xtol = REL_PARAM_TOLERANCE_xtol,gtol = ORTHO_TOLERANCE_gtol;
    long int     maxfev = MAXFEV;
    double  epsfcn = EPSFCN;
    double  diag[NPARAMS];
    long int     mode = MODE;
    double  factor = FACTOR;
    long int     nprint = 0;
    long int     info;
    long int     nfev;
    double *fjac;
    long int     ldfjac = m;
    long int     ipvt[NPARAMS];
    double  qtf[NPARAMS];
    double  wa1[NPARAMS];
    double  wa2[NPARAMS];
    double  wa3[NPARAMS];
    double *wa4;

    /* allocate some workspace */
    if (( fvec = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )  return 0;
    if (( fjac = (double *) calloc ((unsigned int) m*n, (unsigned int) sizeof(double))) == NULL){free(fvec); return 0;}
    if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )	{free(fjac); free(fvec); return 0;}
 
    /* use the current calibration constants as an initial guess */
    x[0] = cc.f;
    x[1] = cc.Tz;
    x[2] = cc.kappa1;
    x[3] = cp.Cx;
    x[4] = cp.Cy;

    /* define optional scale factors for the parameters */
    if ( mode == 2 ) {
        for (i = 0; i < NPARAMS; i++)
            diag[i] = 1.0;             /* some user-defined values */
    }
 
    /* perform the optimization */
    lmdif_ (cc_five_parm_optimization_with_late_distortion_removal_error,
            &m, &n, x, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
            diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
            ipvt, qtf, wa1, wa2, wa3, wa4);
 
    /* update the calibration and camera constants */
    cc.f = x[0];
    cc.Tz = x[1];
    cc.kappa1 = x[2];
    cp.Cx = x[3];
    cp.Cy = x[4];

    /* release allocated workspace */
    free (fvec);
    free (fjac);
    free (wa4);
    return 1;
#undef NPARAMS
}
//////////////////////////////////////////////////////
static void   distorted_to_undistorted_sensor_coord (double Xd,double Yd,double *Xu,double *Yu)
{
    double    distortion_factor;

    /* convert from distorted to undistorted sensor plane coordinates */
    distortion_factor = 1.0 + cc.kappa1 * (SQR (Xd) + SQR (Yd));
    *Xu = Xd * distortion_factor;
    *Yu = Yd * distortion_factor;
}
//////////////////////////////////////////////////////
static void      cc_remove_sensor_plane_distortion_from_Xd_and_Yd ()
{
    int       i;
    double    Xu,Yu;

    for (i = 0; i < cd.point_count; i++) {
		distorted_to_undistorted_sensor_coord (Xd[i], Yd[i], &Xu, &Yu);
		Xd[i] = Xu;   Yd[i] = Yu;
		r_squared[i] = SQR (Xu) + SQR (Yu);
    }
}
/////////////////////////////////////////////////////////
static int  cc_five_parm_optimization_with_early_distortion_removal_error (double *params, double *err)
{
    int   i;
    double    f,Tz,xc,yc,zc,Xu_1,Yu_1,Xu_2, Yu_2;

    /* in this routine radial lens distortion is taken into account */
    /* before the rotation and translation constants are determined */
    /* (this assumes we have the distortion reasonably modelled)    */

    f = params[0];
    Tz = params[1];
    cc.kappa1 = params[2];
    cp.Cx = params[3];
    cp.Cy = params[4];
    cc_compute_Xd_Yd_and_r_squared ();

    /* remove the sensor distortion before computing the translation and rotation stuff */
    cc_remove_sensor_plane_distortion_from_Xd_and_Yd ();
    cc_compute_U ();
    cc_compute_Tx_and_Ty ();
    cc_compute_R ();

    /* we need to do this just to see if we have to flip the rotation matrix */
    cc_compute_approximate_f_and_Tz ();

    if (cc.f < 0) {
		/* try the other solution for the orthonormal matrix */
		cc.r3 = -cc.r3;
		cc.r6 = -cc.r6;
		cc.r7 = -cc.r7;
		cc.r8 = -cc.r8;
		solve_RPY_transform ();

		cc_compute_approximate_f_and_Tz ();
                 if (cc.f < 0.0) return 0;
    }

    /* now calculate the squared error assuming zero distortion */
    for (i = 0; i < cd.point_count; i++) {
		/* convert from world coordinates to camera coordinates */
		/* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
		xc = cc.r1 * cd.xw[i] + cc.r2 * cd.yw[i] + cc.Tx;
		yc = cc.r4 * cd.xw[i] + cc.r5 * cd.yw[i] + cc.Ty;
		zc = cc.r7 * cd.xw[i] + cc.r8 * cd.yw[i] + Tz;

		/* convert from camera coordinates to undistorted sensor coordinates */
		Xu_1 = f * xc / zc;
		Yu_1 = f * yc / zc;

		/* convert from distorted sensor coordinates to undistorted sensor coordinates  */
		/* (already done, actually)							 */
		Xu_2 = Xd[i];
		Yu_2 = Yd[i];

		/* record the error in the undistorted sensor coordinates */
		err[i] = hypot (Xu_1 - Xu_2, Yu_1 - Yu_2);
    }
	return 1;
}
////////////////////////////////////////////////////
static int   cc_five_parm_optimization_with_early_distortion_removal ()
{
#define NPARAMS 5

    int       i;
     /* Parameters needed by MINPACK's lmdif() */
    long int     m = cd.point_count;
    long int     n = NPARAMS;
    double  x[NPARAMS];
    double *fvec;
    double  ftol = REL_SENSOR_TOLERANCE_ftol;
    double  xtol = REL_PARAM_TOLERANCE_xtol;
    double  gtol = ORTHO_TOLERANCE_gtol;
    long int     maxfev = MAXFEV;
    double  epsfcn = EPSFCN;
    double  diag[NPARAMS];
    long int     mode = MODE;
    double  factor = FACTOR;
    long int     nprint = 0;
    long int     info;
    long int     nfev;
    double *fjac;
    long int     ldfjac = m;
    long int     ipvt[NPARAMS];
    double  qtf[NPARAMS];
    double  wa1[NPARAMS];
    double  wa2[NPARAMS];
    double  wa3[NPARAMS];
    double *wa4;

    if (( fvec = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )  return 0;
    if (( fjac = (double *) calloc ((unsigned int) m*n, (unsigned int) sizeof(double))) == NULL){free(fvec); return 0;}
    if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )	{free(fjac); free(fvec); return 0;}

    /* use the current calibration and camera constants as a starting point */
    x[0] = cc.f;
    x[1] = cc.Tz;
    x[2] = cc.kappa1;
    x[3] = cp.Cx;
    x[4] = cp.Cy;

    /* define optional scale factors for the parameters */
    if ( mode == 2 ) {
        for (i = 0; i < NPARAMS; i++)
            diag[i] = 1.0;             /* some user-defined values */
    }
 
    /* perform the optimization */
    lmdif_ (cc_five_parm_optimization_with_early_distortion_removal_error,
            &m, &n, x, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
            diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
            ipvt, qtf, wa1, wa2, wa3, wa4);

    /* update the calibration and camera constants */
    cc.f = x[0];
    cc.Tz = x[1];
    cc.kappa1 = x[2];
    cp.Cx = x[3];
    cp.Cy = x[4];

    /* release allocated workspace */
    free (fvec);
    free (fjac);
    free (wa4);

    return 1;
#undef NPARAMS
}
/////////////////////////////////////////////////////////////////////
static int  cc_full_optimization_error (double * params, double *err)
{
    int     i;
    double  xc,yc,zc,Xd_,Yd_,Xu_1,Yu_1,Xu_2,Yu_2,distortion_factor,Rx,Ry,Rz,
            Tx,Ty,Tz,kappa1,f,Cx,Cy,r1,r2,r4,r5,r7,r8,sa,sb,sg,ca,cb,cg;

    Rx = params[0];    Ry = params[1];    Rz = params[2];
    Tx = params[3];    Ty = params[4];    Tz = params[5];
    kappa1 = params[6];
    f = params[7];
    Cx = params[8];    Cy = params[9];

    SINCOS (Rx, sa, ca);
    SINCOS (Ry, sb, cb);
    SINCOS (Rz, sg, cg);
    r1 = cb * cg;
    r2 = cg * sa * sb - ca * sg;
    r4 = cb * sg;
    r5 = sa * sb * sg + ca * cg;
    r7 = -sb;
    r8 = cb * sa;

    for (i = 0; i < cd.point_count; i++) {
		/* convert from world coordinates to camera coordinates */
		/* Note: zw is implicitly assumed to be zero for these (coplanar) calculations */
		xc = r1 * cd.xw[i] + r2 * cd.yw[i] + Tx;
		yc = r4 * cd.xw[i] + r5 * cd.yw[i] + Ty;
		zc = r7 * cd.xw[i] + r8 * cd.yw[i] + Tz;

		/* convert from camera coordinates to undistorted sensor plane coordinates */
		Xu_1 = f * xc / zc;
		Yu_1 = f * yc / zc;

		/* convert from 2D image coordinates to distorted sensor coordinates */
		Xd_ = cp.dpx * (cd.Xf[i] - Cx) / cp.sx; 
		Yd_ = cp.dpy * (cd.Yf[i] - Cy);

		/* convert from distorted sensor coordinates to undistorted sensor plane coordinates */
		distortion_factor = 1 + kappa1 * (SQR (Xd_) + SQR (Yd_));
		Xu_2 = Xd_ * distortion_factor;
		Yu_2 = Yd_ * distortion_factor;

		/* record the error in the undistorted sensor coordinates */
		err[i] = hypot (Xu_1 - Xu_2, Yu_1 - Yu_2);
    }
	return 1;
}

/////////////////////////////////////////////////////////////////////
static int  cc_full_optimization ()
{
#define NPARAMS 10

    int       i;

    /* Parameters needed by MINPACK's lmdif() */
    long int     m = cd.point_count;
    long int     n = NPARAMS;
    double  x[NPARAMS];
    double *fvec;
    double  ftol = REL_SENSOR_TOLERANCE_ftol;
    double  xtol = REL_PARAM_TOLERANCE_xtol;
    double  gtol = ORTHO_TOLERANCE_gtol;
    long int     maxfev = MAXFEV;
    double  epsfcn = EPSFCN;
    double  diag[NPARAMS];
    long int     mode = MODE;
    double  factor = FACTOR;
    long int     nprint = 0;
    long int     info;
    long int     nfev;
    double *fjac;
    long int     ldfjac = m;
    long int     ipvt[NPARAMS];
    double  qtf[NPARAMS];
    double  wa1[NPARAMS];
    double  wa2[NPARAMS];
    double  wa3[NPARAMS];
    double *wa4;

    if (( fvec = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )  return 0;
    if (( fjac = (double *) calloc ((unsigned int) m*n, (unsigned int) sizeof(double))) == NULL){free(fvec); return 0;}
    if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double))) == NULL )	{free(fjac); free(fvec); return 0;}
    /* use the current calibration and camera constants as a starting point */
    x[0] = cc.Rx;
    x[1] = cc.Ry;
    x[2] = cc.Rz;
    x[3] = cc.Tx;
    x[4] = cc.Ty;
    x[5] = cc.Tz;
    x[6] = cc.kappa1;
    x[7] = cc.f;
    x[8] = cp.Cx;
    x[9] = cp.Cy;

    /* define optional scale factors for the parameters */
    if ( mode == 2 ) {
        for (i = 0; i < NPARAMS; i++)
            diag[i] = 1.0;             /* some user-defined values */
    }
    /* perform the optimization */
    lmdif_ (cc_full_optimization_error,
            &m, &n, x, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
            diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
            ipvt, qtf, wa1, wa2, wa3, wa4);

    /* update the calibration and camera constants */
    cc.Rx = x[0];
    cc.Ry = x[1];
    cc.Rz = x[2];
    apply_RPY_transform ();

    cc.Tx = x[3];
    cc.Ty = x[4];
    cc.Tz = x[5];
    cc.kappa1 = x[6];
    cc.f = x[7];
    cp.Cx = x[8];
    cp.Cy = x[9];

    /* release allocated workspace */
    free (fvec);
    free (fjac);
    free (wa4);
	return 1;

#undef NPARAMS
}
/////////////////////////////////////////////////////////////////////////
static void init_structs(void)
{
    cp.Ncx=cp.Nfx=cp.dx=cp.dy=cp.dpx=cp.dpy=cp.Cx=cp.Cy=0.0; cp.sx=1.0;		
    
	cd.point_count=0;

	cc.f=cc.kappa1=cc.p1=cc.p2=cc.Tx=cc.Ty=cc.Tz=0.0;
    cc.Rx=cc.Ry=cc.Rz=0.0;		
    cc.r1=cc.r2=cc.r3=cc.r4=cc.r5=cc.r6=cc.r7=cc.r8=cc.r9=0.0;
}

/////////////////////////////////////////////////////////////////////////
int  coplanar_calibration (calibration_data cd0,camera_parameters cp0,double *Kc, double *Rc,double *Tc)
{
 int aux;

 init_structs();
 memcpy(&cd, &cd0, sizeof(cd0));
 memcpy(&cp, &cp0, sizeof(cp0));

 aux=cc_three_parm_optimization ();

// Retirado por Bruno Madeira
/*
 Kc[0]=Kc[4]=cc.f;  Kc[2]=cp.Cx; Kc[5]=cp.Cy; Kc[8]=1.0f;
 Kc[7]=cc.kappa1;   Kc[3]=Kc[6]=0.0f;
 Kc[1]=1.0f;
*/


// Inserido por Bruno Madeira
 Kc[0] = cc.f * cp.sx;  Kc[1] = 0.0;      Kc[2] = cp.Cx;
 Kc[3] = 0.0;             Kc[4] = cc.f;   Kc[5] = cp.Cy;
 Kc[6] = 0.0;             Kc[7] = 0.0;      Kc[8] = 1.0;


 Rc[0]=cc.r1;  Rc[1]=cc.r2; Rc[2]=cc.r3;
 Rc[3]=cc.r4;  Rc[4]=cc.r5; Rc[5]=cc.r6;
 Rc[6]=cc.r7;  Rc[7]=cc.r8; Rc[8]=cc.r9;

 Tc[0]=cc.Tx; Tc[1]=cc.Ty; Tc[2]=cc.Tz;

 return aux;
}


////////////////////////////////////////////////////

int coplanar_calibration_with_full_optimization (calibration_data cd0,camera_parameters cp0,double *Kc, double *Rc,double *Tc)
{
 int aux=0;

 init_structs();
 memcpy(&cd, &cd0, sizeof(cd0));
 memcpy(&cp, &cp0, sizeof(cp0));
 
 aux+=cc_three_parm_optimization ();
 //aux+=cc_five_parm_optimization_with_late_distortion_removal ();
 //aux+=cc_five_parm_optimization_with_early_distortion_removal ();

 aux+=cc_nic_optimization ();

 //aux+=cc_full_optimization ();

// Retirado por Bruno Madeira
/* 
 Kc[0]=Kc[4]=cc.f;  Kc[2]=cp.Cx; Kc[5]=cp.Cy; Kc[8]=1.0f;
 Kc[7]=cc.kappa1;   Kc[3]=Kc[6]=0.0f;
 Kc[1]=1.0f;
*/

// Inserido por Bruno Madeira

 Kc[0] = cc.f * cp.sx;  Kc[1] = 0.0;      Kc[2] = cp.Cx;
 Kc[3] = 0.0;             Kc[4] = cc.f;   Kc[5] = cp.Cy;
 Kc[6] = 0.0;             Kc[7] = 0.0;      Kc[8] = 1.0;

// Fim da insercao 

 Rc[0]=cc.r1;  Rc[1]=cc.r2; Rc[2]=cc.r3;
 Rc[3]=cc.r4;  Rc[4]=cc.r5; Rc[5]=cc.r6;
 Rc[6]=cc.r7;  Rc[7]=cc.r8; Rc[8]=cc.r9;

 Tc[0]=cc.Tx; Tc[1]=cc.Ty; Tc[2]=cc.Tz;

 return aux;
}
