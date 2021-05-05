//////////////////////////////////////////////////////////////////
//****************************************************************
// Source file containing the management of all events
//
// Adelailson Peixoto and Fabio Franco, 13/05/2003
// Visgraf - IMPA
//
//****************************************************************
//////////////////////////////////////////////////////////////////

#include "calib.h"



/*////////////////////////////////////////*/
/*    Declaration of Local Functions      */
/*////////////////////////////////////////*/

static void  cal_CornerFinder(Image *img,point2D *points, int num_points, point2D *p_subpix);
static int   cal_Intersect(point2D p0, point2D p1, point2D p2, point2D p3, point2D *p);  
static void  cal_WarpLine(point2D p0, point2D p1, point2D p2, int n, point2D *vpoints);
static void  cal_ApplyConvolution(float **SI, int nx, int ny,float *vIx, float *vIy);
static void  cal_Gradient(float *SI,int nx, int ny, point2D **grad);
static void  cal_Smooth(float **SI, int n_sx, int n_sy);

/*////////////////////////////////////////////////////////*/
/*              Implementation of Local Functions         */
/*////////////////////////////////////////////////////////*/

static void cal_ApplyConvolution(float **SI, int n_sx, int n_sy, float *vIx, float *vIy)
{

    int i,j, Iprv,Inxt,J, Jprv, Jnxt, prv,nxt,k=0;
    float  *auxSI;

    //Aplying convolution in X direction
    for(j=0; j<n_sx; j++)
        for(i=0; i<n_sy; i++){
            if(i==0)        {Iprv=prv=0;} else {Iprv=i-1; prv=1;}
            if(i==(n_sy-1)) {Inxt=nxt=0;} else {Inxt=i+1; nxt=1;}
            J=j*n_sy;
            (*SI)[i+J] = (*SI)[Iprv+J]*vIx[0]*prv + (*SI)[i+J]*vIx[1] + (*SI)[Inxt+J]*vIx[2]*nxt;
        }

    //Aplying convolution in X direction
    for(j=0; j<n_sx; j++){
        if(j==0)        {Jprv=prv=0;} else {Jprv=j-1; prv=1;}
        if(j==(n_sx-1)) {Jnxt=nxt=0;} else {Jnxt=j+1; nxt=1;}
        J=j*n_sx;
        for(i=0; i<n_sy; i++)
            (*SI)[i+J] = (*SI)[Iprv+J]*vIy[0]*prv + (*SI)[i+J]*vIy[1] + (*SI)[Inxt+J]*vIy[2]*nxt;
    }

    //Eliminating SI borders
    if((auxSI=new float[(n_sx-2)*(n_sy-2)])==NULL) return;
    for(j=1; j<(n_sx-1); j++)
        for(i=1; i<(n_sy-1); i++)
            auxSI[k++]=(*SI)[i+j*n_sy];

    delete[](*SI);
    (*SI)=auxSI;
}
//////////////////////////////////////////////////////////////////////////
static void  cal_Smooth(float **SI, int nx, int ny)
{
    int i,j,num,ind;
    float *S=new float[nx*ny],a;

    for(j=0; j<ny; j++)
        for(i=0; i<nx; i++){
            ind = i+j*nx; num=0; a=0.0f;

            if(i>0 && j>0)		{a+=(*SI)[ind-1-nx]; num++;}
            if(j>0)				{a+=(*SI)[ind-nx]; num++;}
            if(i<nx-1 && j>0)		{a+=(*SI)[ind+1-nx]; num++;}
            if(i>0)				{a+=(*SI)[ind-1];num++;}
            if(i<nx-1)				{a+=(*SI)[ind+1]; num++;}
            if(i>0 && j<ny-1)		{a+=(*SI)[ind-1+nx]; num++;}
            if(j<ny-1)			{a+=(*SI)[ind+nx]; num++;}
            if(i<nx-1 && j<ny-1)	{a+=(*SI)[ind+1+nx];num++;}
            a+=(*SI)[ind]; num++;
            S[ind]=a/num;
        }
    delete[](*SI); (*SI)=S;
}
//////////////////////////////////////////////////////////////////////////

static void cal_Gradient(float *SI,int n_sx, int n_sy, point2D **grad)
{
    int     i,j,k=0, ind_prv, ind_nxt, dx,dy, Jprv,Jnxt,Iprv,Inxt;
    point2D *aux_grad;

    for(j=0; j<n_sx; j++)
        for(i=0; i<n_sy; i++){

            if(i==0)             {Iprv=0;   Inxt=1;   dx=1;} //forward
            else if(i==(n_sy-1)) {Iprv=i-1; Inxt=i;   dx=1;} //backward
            else                 {Iprv=i-1; Inxt=i+1; dx=2;} //central

            ind_prv=Iprv+j*n_sy;  ind_nxt=Inxt+j*n_sy;
            (*grad)[k].x=(SI[ind_nxt]- SI[ind_prv])/dx;

            if(j==0)             {Jprv=0;   Jnxt=1;   dy=1;} //forward
            else if(j==(n_sx-1)) {Jprv=j-1; Jnxt=j;   dy=1;} //backward
            else                 {Jprv=j-1; Jnxt=j+1; dy=2;} //central

            ind_prv=i+Jprv*n_sy;  ind_nxt=i+Jnxt*n_sy;
            (*grad)[k++].y=(SI[ind_nxt]- SI[ind_prv])/dy;
        }

    //Eliminacao das bordas de grad
    if((aux_grad=new point2D[(n_sx-2)*(n_sy-2)])==NULL) return;
    for(j=1,k=0; j<(n_sx-1); j++)
        for(i=1; i<(n_sy-1); i++)
            aux_grad[k++]=(*grad)[i+j*n_sy];

    delete[](*grad);
    (*grad)=aux_grad;

}
//////////////////////////////////////////////////////////////////////////////////////////

/** \brief Given 4 corners on the grid, finds the same points with subpixel precision
 *  Adapted from the MATLAB implementation */
static void  cal_CornerFinder(Image *img,point2D *points, int num_points, point2D *p_subpix)
{
  int      wintx=5,winty=5,i,j,k=0, compt, crIx,crIy,xmax,xmin,ymin,ymax,n;
  float    *offx,*offy, tol=0.15f, //resolution=0.8f,
		   cIx,cIy,itIx,itIy,
		   vIx[3], vIy[3];				       //Convolution kernels at x and y 
  float    *SI,							       //Points neighborhood
	       *px, *py;					       //Matrix created by offx+cix and offy+cIy
  point2D  bb,xc2,*grad;					       //SI gradient
  float    *gradXX,*gradXY,*gradYY,a,b,c,delta;
  int      wid,hei,out=0;                       

  img->GetDim(&wid,&hei);                               //Image size

  for(i=0; i<num_points; i++) p_subpix[i]=points[i];    //Start p_subpix with the original points


  
  offx=new float[(2*wintx+1)*(2*winty+1)];
  offy=new float[(2*wintx+1)*(2*winty+1)];
  px=new float[(2*wintx+1)*(2*winty+1)];
  py=new float[(2*wintx+1)*(2*winty+1)];
  if(offx==NULL || offy==NULL || px==NULL || py==NULL) return;

  //Calculating offx and offy
  for(j=0; j<(2*wintx+1); j++)
	  for(i=0; i<(2*winty+1); i++){
		  offy[i+j*(2*winty+1)]= (float)j-wintx;
		  offx[i+j*(2*winty+1)]= (float)i-winty;
	  }

  //For each point return its subpixel precision
  for(n=0; n<num_points; n++){
	  //tol=resolution+1.0f;  
	  compt=0; out=0;

	  while((!out) && (compt<20)){
		  //Rounding to the nearest interger
		  cIx=p_subpix[n].x;   cIy=p_subpix[n].y;
		  crIx= (int)p_subpix[n].x;  crIy=(int)p_subpix[n].y;
		  if((cIx - crIx)>0.5f) crIx++;  
		  if((cIy - crIy)>0.5f) crIy++;  

		  //Convolution mask calculation
		  if((itIx=cIx - crIx)>0) {vIx[0]=itIx;  vIx[1]=1-itIx;   vIx[2]=0;}
		  else					  {vIx[0]=0;     vIx[1]=1+itIx;   vIx[2]=-itIx;}
		  if((itIy=cIy - crIy)>0) {vIy[0]=itIy;  vIy[1]=1-itIy;   vIy[2]=0;}
		  else					  {vIy[0]=0;     vIy[1]=1+itIy;   vIy[2]=-itIy;}

		  //Calculating convolution limits
		  if((crIx-wintx-2)<0/*1*/)              {xmin=0            /*1*/;    xmax= 2*wintx+4     /*5*/;}  //Left image border
		  else if((crIx+wintx+2)>(wid-1)/*wid*/) {xmax=wid-1        /*wid*/;  xmin=wid-2*wintx-5  /*4*/;}  //Right image border
		  else                                   {xmin=crIx-wintx-3 /*2*/;    xmax=crIx+wintx+1   /*2*/;}  //Image interior point
		  if((crIy-winty-2)<0/*1*/)              {ymin=0            /*1*/;    ymax=2*winty+4      /*5*/;}  //Left image border
		  else if((crIy+winty+2)> (hei-1)/*hei*/){ymax=hei-1        /*hei*/;  ymin= hei-2*winty-5 /*4*/;}  //Right image border
		  else                                   {ymin=crIy-winty-3 /*2*/;    ymax= crIy+winty+1  /*2*/;}  //Image interior point

		  //Neighborhood matrix calculation
		  if((SI=new float[(xmax-xmin+1)*(ymax-ymin+1)])==NULL) return;
		  k=0;
		  for(j=ymin; j<=ymax; j++)
			  for(i=xmin; i<=xmax; i++,k++)
				  SI[k]=img->GetGray(i,j);

  		  cal_Smooth(&SI,xmax-xmin+1,ymax-ymin+1);
			  
		
		cal_ApplyConvolution(&SI,xmax-xmin+1,ymax-ymin+1,vIx,vIy);


		if((grad=new point2D[(xmax-xmin-1)*(ymax-ymin-1)])==NULL) return;
		cal_Gradient(SI,xmax-xmin-1,ymax-ymin-1,&grad);
		

	          //Calculating px and py: cIx + offx and cIy + offy
		  int ind_p;
		  for(j=0; j<(2*wintx+1); j++)
	         for(i=0; i<(2*winty+1); i++){
				ind_p=i+j*(2*winty+1);
		        px[ind_p] = offx[ind_p] + cIx;
		        py[ind_p] = offy[ind_p] + cIy;
			 }

		  //Calculating gradXX, gradXY and gradYY
		  if((gradXX=new float[(xmax-xmin-3)*(ymax-ymin-3)])==NULL) return;
		  if((gradXY=new float[(xmax-xmin-3)*(ymax-ymin-3)])==NULL) return;
		  if((gradYY=new float[(xmax-xmin-3)*(ymax-ymin-3)])==NULL) return;
		  for(i=0; i<(xmax-xmin-3)*(ymax-ymin-3); i++){
			  gradXX[i]=grad[i].x*grad[i].x;
			  gradXY[i]=grad[i].x*grad[i].y;
			  gradYY[i]=grad[i].y*grad[i].y;
		  }

		  bb.x=bb.y=0.0;  a=b=c=0.0;
		  for(i=0;i<(2*wintx+1)*(2*winty+1); i++){
             bb.x += gradXX[i]*px[i]+gradXY[i]*py[i];                 //Resulting X coordinate
			 bb.y += gradXY[i]*px[i]+gradYY[i]*py[i];     //Resulting Y coordinate

			 a+=gradXX[i];  b+=gradXY[i];   c+=gradYY[i]; //Coeficients for 2 degree equation
		  }

		  //Calculation the subpixel precision for the point
		  delta= a*c-b*b;
		  if(delta< 0.00001){ //No solution. Using the same precision
             xc2=p_subpix[n];   
		  }
		  else{               //Calculating new precision
			  xc2.x=(c*bb.x-b*bb.y)/delta;
			  xc2.y=(a*bb.y-b*bb.x)/delta;
		  }
		  //tol= VEC_Norma(p_subpix[n]-xc2); //Newest minus older precison
		  out=((p_subpix[n].x-xc2.x)<=tol)&&((p_subpix[n].y-xc2.y)<=tol);
		  p_subpix[n]=xc2;
		  compt++;

		  delete[] SI; delete[] gradXX; delete[] gradXY; delete[] gradYY;

	  }//while
  }//for
  

  delete[] offx; delete[] offy;
  delete[] px;   delete[] py;  
}

/*///////////////////////////////////////////////////////////////*/
//****************************************************************
/*   AUX FUNCTIONS FOR CAMERA/PROJETOR  CALIBRATION		 */
//****************************************************************
/*///////////////////////////////////////////////////////////////*/

static void cal_WarpLine(point2D a, point2D b, point2D c, int n, point2D *vpoints)
{
	point2D c1,c2,c3,c4;
	int     i;
	float   d, dinc=1.0f/(float)n;

	if((a.x!=b.x) &&(b.x!=c.x)){
		c1.x = a.x*(b.x-c.x);
                c2.x = 2.f*a.x*c.x - c.x*b.x - a.x*b.x;
		c3.x = b.x-c.x;
                c4.x = a.x - 2.f*b.x + c.x;
	}
	else{
		c1.x=a.x;	c2.x=0.0f;
		c3.x=1.0f;	c4.x=0.0f;
	}
	if((a.y!=b.y) &&(b.y!=c.y)){
		c1.y = a.y*(b.y-c.y);
                c2.y = 2.f*a.y*c.y - c.y*b.y - a.y*b.y;
		c3.y = b.y-c.y;
                c4.y = a.y - 2.f*b.y + c.y;
	}
	else{
		c1.y=a.y;	c2.y=0.0f;
		c3.y=1.0f;	c4.y=0.0f;
	}

	for(i=0, d=0.0f; i<=n; d+=dinc, i++){
      vpoints[i].x = (c1.x + d*c2.x)/(c3.x + d*c4.x);
	  vpoints[i].y = (c1.y + d*c2.y)/(c3.y + d*c4.y);
	}
}

////////////////////////////////////////////////////////////////////////
/** \brief Returns, at p, the intersection point of p0p1 and p2p3 lines */
static int cal_Intersect(point2D p0, point2D p1, point2D p2, point2D p3, point2D *p)
{
  float   t, coef;

  p->x=0.0f; p->y=0.0f;
  coef=(p3.x-p2.x)*(p1.y-p0.y)-(p3.y-p2.y)*(p1.x-p0.x);

  //Converging lines
  if ((coef<-0.0000001) || coef>0.0000001){
    t=((p3.y-p2.y)*(p0.x-p2.x)-(p3.x-p2.x)*(p0.y-p2.y))/coef;
    *p=p0 + t*(p1-p0);
	return 1;
  }

  //Parallel lines
  point2D center;
  cal_Intersect(p0,p2, p1, p3, &center);
  *p = center + (float)10000*(p1-p0);
  return 0;
}

/////////////////////////////////////////////////////////////
/** \brief Extracts the grid from the four corners selected*/
void  CAL_ExtractGrid (Image *img, point2D *points, int nx, int ny, point2D *xc, bool calib_type)
{
    point2D  p_subpix[4]; //points with a subpixel precision
    int      i,j,k;
    point2D  cent,o1,o2,a,b,c,d;
    point2D  *XX=NULL,*s1=NULL, *s2=NULL, *s3=NULL, *s4=NULL;

    if(calib_type) {
        for(i=0; i<4; i++) p_subpix[i] = points[i];
    }
    else {
        cal_CornerFinder(img,points,4,p_subpix);
        for(i=0; i<4; i++) points[i] = p_subpix[i];
    }

    //Calculating the grid intersections
    cal_Intersect(p_subpix[0],p_subpix[2],p_subpix[1],p_subpix[3], &cent);
    cal_Intersect(p_subpix[0],p_subpix[3],p_subpix[1],p_subpix[2], &o1);
    cal_Intersect(p_subpix[0],p_subpix[1],p_subpix[2],p_subpix[3], &o2);
    cal_Intersect(p_subpix[0],p_subpix[1],o1,cent, &a);
    cal_Intersect(p_subpix[2],p_subpix[3],o1,cent, &c);
    cal_Intersect(p_subpix[1],p_subpix[2],o2,cent, &b);
    cal_Intersect(p_subpix[0],p_subpix[3],o2,cent, &d);

    s1=new point2D[nx+1];  s2=new point2D[ny+1];
    s3=new point2D[nx+1];  s4=new point2D[ny+1];
    if(s1==NULL || s2==NULL || s3==NULL || s4==NULL) return;

    cal_WarpLine(p_subpix[0],a,p_subpix[1], nx, s1);
    cal_WarpLine(p_subpix[1],b,p_subpix[2], ny, s2);
    cal_WarpLine(p_subpix[3],c,p_subpix[2], nx, s3);
    cal_WarpLine(p_subpix[0],d,p_subpix[3], ny, s4);

    if((XX=new point2D[(nx+1)*(ny+1)])==NULL) return;

    for(k=ny,i=0; k>=0; k--,i++)
        for(j=0; j<=nx; j++)
            cal_Intersect(s1[j],s3[j],s2[k],s4[k], &(XX[j+i*(nx+1)]));

    if(calib_type){
        for(i=0; i<(nx+1)*(ny+1); i++)
            xc[i] = XX[i];
    }
    else
        cal_CornerFinder(img,XX,(nx+1)*(ny+1),xc);


    if(XX!=NULL) delete[]XX; if(s1!=NULL) delete[]s1;
    if(s2!=NULL) delete[]s2; if(s3!=NULL) delete[]s3; delete[]s4;
}

/*///////////////////////////////////////////////////////////////*/
//****************************************************************
/*                CAMERA CALIBRATION		                 */
//****************************************************************
/*///////////////////////////////////////////////////////////////*/

/** \brief Computes grid coord in relation to the  object (pattern)  frame */
void  CAL_DetectCameraPoints (int nx, int ny, float dx, float dy, point3D *xw)
{
  int     i,j,ind;

  for(j=0; j<=ny; j++)
	  for(i=0; i<=nx; i++){
		  ind=i+j*(nx+1);
		  xw[ind].x = i*dx;
		  xw[ind].y = j*dy;
		  xw[ind].z = 0.0;
	  }
}

/////////////////////////////////////////////////////////////////////
/** \brief Calibrates the camera using the coplanar calibration function (TSAI)
 *  Calibrate the camera from the grid imgGrid, selected from an image, and from
 *  the grid wldGrid, selected from the pattern in a 3D system.
 * 
 * \param nx grid width
 * \param ny grid height. 
 * \param flag_opt indicates if the calibration is optimized
 * \param K intrinsic parameters
 * \param R rotation matrix 
 * \param T translation matrix
 */
int CAL_CalibrateCamera(point2D *imgGrid, point3D *wldGrid, int nx, int ny,
                                                 double cx,double cy,double *K, double *R, double *T)
{
	calibration_data  cd; //structure defined in cal_main
	camera_parameters cp;
	int    i, res,n=(nx+1)*(ny+1);


	for(i=0; i<n; i++){
		cd.xw[i]=wldGrid[i].x;
		cd.yw[i]=wldGrid[i].y;
		cd.zw[i]=wldGrid[i].z;
		cd.Xf[i]=imgGrid[i].x;
		cd.Yf[i]=imgGrid[i].y;

	}
	cd.point_count=(nx+1)*(ny+1);

        cp.Cx=cx; cp.Cy=cy;

        // Modif by Bruno Madeira
        // cp.sx=(4./3.)*((double)cy/cx);
        cp.sx=1.0;

        cp.dpx=1.0; cp.dpy=1.0; cp.dx=1.0; cp.dy=1.0;
        
        res=coplanar_calibration_with_full_optimization (cd,cp,K,R,T);

        return res;
}
