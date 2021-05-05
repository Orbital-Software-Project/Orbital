#ifndef COORD_H
#define COORD_H

//****************************************************************
// Header file to computing coordinate system transforms
//
// Adelailson Peixoto, 16/10/2003
// Visgraf - IMPA
//
//****************************************************************


#include "point.h"
#include <math.h>

////////////////////////////////////////////////////////////////////////
// DEFINITIONS
////////////////////////////////////////////////////////////////////////

#define COORD_ConvertFromCamToWorld(p,R,T)  COORD_InvertedConvertion(p,R,T)
#define COORD_ConvertFromProjToWorld(p,R,T) COORD_InvertedConvertion(p,R,T)

#define COORD_ConvertFromWorldToCam(p,R,T)  COORD_DirectConvertion(p,R,T)
#define COORD_ConvertFromWorldToProj(p,R,T) COORD_DirectConvertion(p,R,T)
#define COORD_ConvertFromProjToCam(p,R,T)   COORD_DirectConvertion(p,R,T)
#define COORD_ConvertFromCamToProj(p,R,T)   COORD_DirectConvertion(p,R,T)

#define COORD_ConvertAllFromWorldToCam(pw,R,T, n,pc)  COORD_ConvertAllFromWorld(pw,R,T, n,pc)
#define COORD_ConvertAllFromWorldToProj(pw,R,T, n,pc) COORD_ConvertAllFromWorld(pw,R,T, n,pc)

////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
////////////////////////////////////////////////////////////////////////

void COORD_GetPerspective(point3D p,double *K, double *x,double *y);

point3D COORD_InvertedConvertion(point3D pc, double *R, double *T);
point3D COORD_DirectConvertion(point3D pw, double *R, double *T);

void COORD_ConvertAllFromWorld(const point3D *pw, double *R, double *T, int n,point3D *pc);

#endif // COORD_H
