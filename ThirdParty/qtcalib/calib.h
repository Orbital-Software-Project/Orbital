#ifndef CALIB_H
#define CALIB_H

//****************************************************************
// Header file containing the management of all events
//
// Adelailson Peixoto e Fabio Franco, 13/05/2003
// Visgraf - IMPA
//
//****************************************************************


#include "image.h"
#include "point.h"
#include <math.h>

#include "cal_main.h"

/*/////////////////////////////////////////////////*/
/*	     CAMERA CALIBRATION		           */
/*/////////////////////////////////////////////////*/

void  CAL_ExtractGrid (Image *img, point2D *points, int nx, int ny, point2D *imgGrid, bool calib_type);
void  CAL_DetectCameraPoints (int nx, int ny, float dx, float dy, point3D *wldGrid);
int   CAL_CalibrateCamera(point2D *imgGrid, point3D *wldGrid, int nx, int ny, double cx,double cy,double *K, double *R, double *T);

#endif // CALIB_H

