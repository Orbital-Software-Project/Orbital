#include "point.h"

////////////////////////////////////////////////////////////////
//////////////////IMPLEMENTACAO 2D/////////////////////////////
///////////////////////////////////////////////////////////////

point2D operator + (point2D a, point2D b)
{
    point2D s;

    s.x=a.x+b.x;
    s.y=a.y+b.y;
    return(s);
}

///////////////////////////////////////////////////////////////

point2D operator - (point2D a, point2D b)
{
    point2D d;

    d.x=a.x-b.x;
    d.y=a.y-b.y;
    return(d);
}

/////////////////////////////////////////////////////////////

float operator * (point2D a, point2D b)
{

    return(a.x*b.x + a.y*b.y);

}
////////////////////////////////////////////////////////////////

point2D operator * (float t,point2D a)
{
    point2D aux;
    aux.x = a.x * t;
    aux.y = a.y * t;
    return(aux);
}
//////////////////////////////////////////////////////////////////

float operator ^ (point2D a, point2D b)
{

    return (a.x*b.y - a.y*b.x);

}
//////////////////////////////////////////////////////////////////

int operator < (point2D a, point2D b)
{

    return (a.x < b.x);

}
////////////////////////////////////////////////////////////////////

int operator == (point2D a, point2D b)
{
    if ( (a.x==b.x) && (a.y==b.y) ) return (1); else return (0);
}

//////////////////////////////////////////////////////////////////

int operator != (point2D a, point2D b)
{
    if ( (a.x!=b.x) || (a.y!=b.y) ) return (1); else return (0);
}

/////////////////////////////////////////////////////////////

float VEC_Norma(point2D a)
{
    float norma;
    norma=float(sqrt(a.x*a.x + a.y*a.y));
    return (norma);

}
///////////////////////////////////////////////////////////////
point2D VEC_UnitaryVector(point2D v)
{
    point2D unitary;
    float norma;

    norma= VEC_Norma(v);
    unitary.x= v.x/norma;
    unitary.y= v.y/norma;
    return(unitary);
}
////////////////////////////////////////////////////////////// 
point2D VEC_NormalToVector(point2D u,point2D v)
{
    point2D normal,uniNormal;

    normal.x=(v-u).y;
    normal.y=(-1)*((v-u).x);
    uniNormal= VEC_UnitaryVector(normal);
    return(uniNormal);
}
//////////////////////////////////////////////////////////////

double VEC_PseudoAng(point2D a)
{
    double ang=0.0;

    if (a.x==0.0 && a.y==0.0)
        return (ang);

    if ( (fabs(a.y)<fabs(a.x)) )
        if (a.x>0.0)
        {
            if (a.y>=0)
                ang=(double)a.y/(double)a.x;
            else ang= 8.0 + (double)a.y/(double)a.x;
        }
        else ang= 4.0 + (double)a.y/(double)a.x;
    else
        if (a.y>0)
            ang = 2.0 - (double)a.x/(double)a.y;
        else ang= 6.0 - (double)a.x/(double)a.y;

    return(ang);
}

/////////////////////////////////////////////////////////////////

double VEC_OrientedPseudoAng(point2D a, point2D b)
{
    double ang;
    double paa= VEC_PseudoAng(a);
    double pab= VEC_PseudoAng(b);

    if(pab>=paa)
        ang=pab-paa;
    else
        ang=8- paa + pab;

    return (ang);

}
/////////////////////////////////////////////////////////////////

int VEC_XRightY (point2D x, point2D y)
{
    float opa= VEC_OrientedPseudoAng(x,y);
    if (opa<4)
        return(1);
    else return (0);
}


/////////////////////////////////////////////////////////////////
////////////////////IMPLEMENTACAO 3D/////////////////////////////
/////////////////////////////////////////////////////////////////

point3D operator + (point3D a,point3D b)
{
    point3D s;

    s.x=a.x+b.x;
    s.y=a.y+b.y;
    s.z=a.z+b.z;

    return(s);
}

/////////////////////////////////////////////////////////////

point3D operator - (point3D a,point3D b)
{
    point3D d;

    d.x=a.x-b.x;
    d.y=a.y-b.y;
    d.z=a.z-b.z;

    return(d);
}

//////////////////////////////////////////////////////////

float operator * (point3D a,point3D b) //produto escalar
{
    return(a.x*b.x + a.y*b.y + a.z*b.z);
}

////////////////////////////////////////////////////////
point3D operator * (float x,point3D a)
{
    point3D aux;
    aux.x = a.x * x;
    aux.y = a.y * x;
    aux.z = a.z * x;
    return(aux);
}
///////////////////////////////////////////////
point3D operator ^ (point3D a,point3D b)  //produto vetorial
{
    point3D pv;

    pv.x=a.y*b.z - a.z*b.y;

    pv.y=a.z*b.x - a.x*b.z;

    pv.z=a.x*b.y - a.y*b.x;

    return (pv);
}
////////////////////////////////////////////////////////////////////

int operator < (point3D a, point3D b)
{

    return (a.x < b.x);

}
///////////////////////////////////////////////////////////////////

int operator == (point3D a, point3D b)
{
    if ( (a.x==b.x) && (a.y==b.y) && (a.z==b.z) ) return (1); else return (0);
}
//////////////////////////////////////////////////////////////////

int operator != (point3D a, point3D b)
{
    if ( (a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) ) return (1); else return (0);
}
////////////////////////////////////////////////////////////////////////

float VEC_Norma (point3D a)
{
    float norma;
    norma=float(sqrt(a.x*a.x + a.y*a.y + a.z*a.z));
    return (norma);
}
////////////////////////////////////////////////////////////////////////

float VEC_Norma2 (point3D a)
{
    float norma;
    norma=a.x*a.x + a.y*a.y + a.z*a.z;
    return (norma);
}
/////////////////////////////////////////////////////////////////////

point3D VEC_UnitaryVector(point3D v)
{
    point3D unitary;
    double  norma;

    norma= sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    unitary.x= v.x/norma;
    unitary.y= v.y/norma;
    unitary.z= v.z/norma;
    return(unitary);
}
/*///////////////////////////////////////////////////////////

float OrientedVolume(List<point3D> *ponto, int nfaces)
 {
  int i,j,j1=0,vertices;
  point3D area;
  float volume=0.0;
  area.x=0.0;area.y=0.0;area.z=0.0;
  for ( i=0 ; i<nfaces ; i++)
 {
   vertices=ponto[i].NumberOfNodes();
   for (j=0 ; j<vertices ; j++)
  {
   j1=(j+1)%vertices;
   area=area+((*ponto[i][j])^(*ponto[i][j1]));
  }
   volume+=1.0/6.0*((*ponto[i][j1])*area);
   area.x=0.0;area.y=0.0;area.y=0.0;area.z=0.0;
 }

 return (volume);

  } */
