#ifndef POINT_H
#define POINT_H

#include <math.h>


#define DEGTORAD 3.141592654/180.0     //convert deg to radian


// Point data structure 2D and 3D

struct point2D
{
    float x;
    float y;
};

point2D operator + (point2D a, point2D b);
//Calcula a soma de dois vetores .

point2D operator - (point2D a, point2D b);
//Calcula a diferenca de dois pontos ou vetores .

float operator * (point2D a, point2D b);
//Calcula o produto escalar de dois vetores.

point2D operator * (float t,point2D a);
//Calcula o produto entre um real e um vetor.

float operator ^ (point2D a, point2D b);
//Calcula o produto vetorial entre dois vetores.


int operator < (point2D a,point2D b);
//Retorna 1 se a coordenada x de a for menor que a de b.

int operator == (point2D a, point2D b);
//Retorna 1 se os vetores ou pontos a e b forem iguais.

int operator != (point2D a, point2D b);
//Retorna 0 se os vetores ou pontos a e b forem distintos.

float VEC_Norma (point2D a);
//Retorna a norma (comprimemto) de um vetor.

point2D VEC_UnitaryVector(point2D v);
//Retorna o vetor unitario do vetor v-u.

point2D VEC_NormalToVector(point2D u,point2D v);
//Retorna um vetor unitario normal ao vetor v-u.
//O vetor normal esta no sentido anti-horario

double VEC_PseudoAng (point2D a);
//Calcula,nos entido anti-horario, o pseudo angulo
//de um ponto em relacao a origem.

double VEC_OrientedPseudoAng (point2D a,point2D b);
//Calcula, no sentido anti-horario, o pseudo angulo que vai de a ate b.
  
int VEC_XRightY (point2D x, point2D y);
//Retorna 1, se x esta a direita de y.

///////////////////////////////////////////////////////////////////
///////////////// Operacoes do espaco vetorial 3D /////////////////
//////////////////////////////////////////////////////////////////
struct point3D
 {
  double x;//float x;
  double y;//float y;
  double z;//float z;
 };

point3D operator + (point3D a,point3D b);
//Calcula a soma entre dois vetores 3D.

point3D operator - (point3D a,point3D b);
//Calcula a diferença entre dois pontos ou vetores 3D.

point3D operator * (float x,point3D a);
 //Calcula o produto entre um real e um vetor 3D.

float operator * (point3D a,point3D b);
 //Calcula o produto escalar entre dois vetores 3D.

point3D operator ^ (point3D a,point3D b);
 //Calcula o produto vetorial entre dois vetores 3D.

int operator < (point3D a,point3D b);
 //Retorna 1 se a coordenada x de a for menor que a de b.

int operator == (point3D a, point3D b);
//Retorna 1 se os pontos a e b forem iguais.

int operator != (point3D a, point3D b);
//Retorna 1 se os pontos a e b forem diferentes.

float VEC_Norma (point3D a);
//Calcula a norma (comprimento) de um vetor 3D.
float VEC_Norma2 (point3D a);

point3D VEC_UnitaryVector(point3D v);
//Retorna o vetor unitario de v

//float VEC_OrientedVolume (List<point3D> *point, int nfaces);
//Calcula o volume orientado de um sólido.

#endif // POINT_H

