//****************************************************************
// Header file to computing coordinate system transforms
//
// Adelailson Peixoto, 16/10/2003
// Visgraf - IMPA
//
//****************************************************************

#include "coord.h"



void COORD_GetPerspective(point3D p,double *K, double *x,double *y)
{
   *x=K[0]*p.x/p.z+K[2];
   *y=K[0]*p.y/p.z+K[5];
}

/////////////////////////////////////////////////////////////////////////
//Dado pc em relacao ao sistema da camera ou do projetor, retorna suas coordenadas 
//pw em relacao ao sistema do mundo (do objeto): pw= Rc^(-1)*(pc-Tc)

point3D COORD_InvertedConvertion(point3D pc, double *R, double *T)
{
   point3D pw; 
   double den=R[0]*R[4]*R[8]+R[3]*R[7]*R[2]+R[6]*R[5]*R[1]-
	         R[6]*R[4]*R[2]-R[1]*R[3]*R[8]-R[0]*R[7]*R[5];

   pw.x=((pc.x-T[0])*R[4]*R[8]+(pc.y-T[1])*R[7]*R[2]+(pc.z-T[2])*R[5]*R[1]-
	     (pc.z-T[2])*R[4]*R[2]-(pc.y-T[1])*R[1]*R[8]-(pc.x-T[0])*R[5]*R[7])/den;
   pw.y=((pc.y-T[1])*R[0]*R[8]+(pc.z-T[2])*R[3]*R[2]+(pc.x-T[0])*R[6]*R[5]-
	     (pc.y-T[1])*R[6]*R[2]-(pc.x-T[0])*R[3]*R[8]-(pc.z-T[2])*R[0]*R[5])/den;
   pw.z=((pc.z-T[2])*R[0]*R[4]+(pc.x-T[0])*R[3]*R[7]+(pc.y-T[1])*R[6]*R[1]-
		 (pc.x-T[0])*R[6]*R[4]-(pc.z-T[2])*R[3]*R[1]-(pc.y-T[1])*R[0]*R[7])/den;
   
   return pw;
}


/////////////////////////////////////////////////////////////////////////
//Dado pw em relacao ao sistema do mundo (do objeto), retorna suas coordenadas 
//pc em relacao ao sistema da camera: pc =  Rc*pw + Tc

point3D COORD_DirectConvertion(point3D pw, double *R, double *T)
{
   point3D pc; 

   pc.x=R[0]*pw.x+R[1]*pw.y+R[2]*pw.z+T[0];
   pc.y=R[3]*pw.x+R[4]*pw.y+R[5]*pw.z+T[1];
   pc.z=R[6]*pw.x+R[7]*pw.y+R[8]*pw.z+T[2];
   return pc;
}

//////////////////////////////////////////////////////////////////////////////////
void COORD_ConvertAllFromWorld(const point3D *pw, double *R, double *T, int n,point3D *pc)
{
	int i;

	for(i=0; i<n; i++){
	   pc[i].x=R[0]*pw[i].x+R[1]*pw[i].y+R[2]*pw[i].z+T[0];
	   pc[i].y=R[3]*pw[i].x+R[4]*pw[i].y+R[5]*pw[i].z+T[1];
	   pc[i].z=R[6]*pw[i].x+R[7]*pw[i].y+R[8]*pw[i].z+T[2];
	}
}

