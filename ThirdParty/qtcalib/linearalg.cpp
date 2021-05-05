#include "linearalg.h"

Vector3 v3_make(Real x, Real y, Real z)
{
	Vector3 v;
	v.x=x;
	v.y=y;
	v.z=z;
	return v;
}

Vector3 v3_scale(Real t, Vector3 v)
{
	Vector3 _v;
	_v.x=v.x*t;
	_v.y=v.y*t;
	_v.z=v.z*t;
	return _v;
}

Vector3 v3_add(Vector3 a, Vector3 b)
{
	Vector3 v;
	v.x=a.x+b.x;
	v.y=a.y+b.y;
	v.z=a.z+b.z;
	return v;
}

Vector3 v3_sub(Vector3 a, Vector3 b)
{
	Vector3 v;
	v.x=a.x-b.x;
	v.y=a.y-b.y;
	v.z=a.z-b.z;
	return v;
}

Vector3 v3_cross(Vector3 u, Vector3 v)
{
	Vector3 r;
	r.x= u.y*v.z - u.z*v.y;
	r.y= u.z*v.x - u.x*v.z;
	r.z= u.x*v.y - u.y*v.x;
	return r;
}

Vector3 v3_unit(Vector3 u)
{
	Vector3 v;
	Real t = v3_norm(u);
	v.x=u.x/t;
	v.y=u.y/t;
	v.z=u.z/t;
	return v;
}

Real v3_dot(Vector3 a, Vector3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}


Real v3_norm(Vector3 v)
{
	return sqrt(SQR(v.x) + SQR(v.y) + SQR(v.z));
}

Real v3_sqrnorm(Vector3 v)
{
	return SQR(v.x) + SQR(v.y) + SQR(v.z);
}

//////////////////////////////////////////////

Vector4 v4_make(Real x, Real y, Real z, Real w)
{
	Vector4 v;
	v.x=x;
	v.y=y;
	v.z=z;
	v.w=w;
	return v;
}

Real v4_dot(Vector4 a, Vector4 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

Vector3 v3_v4conv(Vector4 w)
{
	Vector3 v;
	v.x=w.x;
	v.y=w.y;
	v.z=w.z;
	return v;
}

Vector4 v4_v3conv(Vector3 v)
{
	Vector4 _v;
	_v.x=v.x;
	_v.y=v.y;
	_v.z=v.z;
	_v.w=1.f;
	return _v;
}

//Vector3 v3_m4mult(Vector3 v, Matrix4 m)
Vector3 v3_m3mult(Vector3 v, Matrix4 m)
{
	Vector3 r;
	r.x= m.r1.x*v.x + m.r1.y*v.y + m.r1.z*v.z ;
	r.y= m.r2.x*v.x + m.r2.y*v.y + m.r2.z*v.z ;
	r.z= m.r3.x*v.x + m.r3.y*v.y + m.r3.z*v.z ;
	return r;
}

//Vector4 v4_m4mult(Vector4 w, Matrix4 m)

Matrix4 m4_ident()
{
	Matrix4 m;

	m.r1.x=1.f;
	m.r1.y=0.f;
	m.r1.z=0.f;
	m.r1.w=0.f;

	m.r2.x=0.f;
	m.r2.y=1.f;
	m.r2.z=0.f;
	m.r2.w=0.f;

	m.r3.x=0.f;
	m.r3.y=0.f;
	m.r3.z=1.f;
	m.r3.w=0.f;

	m.r4.x=0.f;
	m.r4.y=0.f;
	m.r4.z=0.f;
	m.r4.w=1.f;
	return m; 
}


//Matrix4 m4_translate(Real tx, Real ty, Real tz){}
//Matrix4 m4_scale(Real sx, Real sy, Real sz){}
//Matrix4 m4_rotate(char axis, Real angle){}

Matrix4 m4_transpose(Matrix4 m)
{
	Matrix4 n;
	n.r1.x=m.r1.x;
	n.r1.y=m.r2.x;
	n.r1.z=m.r3.x;
	n.r1.w=m.r4.x;

	n.r2.x=m.r1.y;
	n.r2.y=m.r2.y;
	n.r2.z=m.r3.y;
	n.r2.w=m.r4.y;

	n.r3.x=m.r1.z;
	n.r3.y=m.r2.z;
	n.r3.z=m.r3.z;
	n.r3.w=m.r4.z;

	n.r4.x=m.r1.w;
	n.r4.y=m.r2.w;
	n.r4.z=m.r3.w;
	n.r4.w=m.r4.w;
	return n; 

}


Matrix4 m4_m4prod(Matrix4 a, Matrix4 b)
{
	Matrix4 n;
	n.r1.x=a.r1.x*b.r1.x +a.r1.y*b.r2.x +a.r1.z*b.r3.x +a.r1.w*b.r4.x;
	n.r1.y=a.r1.x*b.r1.y +a.r1.y*b.r2.y +a.r1.z*b.r3.y +a.r1.w*b.r4.y;
	n.r1.z=a.r1.x*b.r1.z +a.r1.y*b.r2.z +a.r1.z*b.r3.z +a.r1.w*b.r4.z;
	n.r1.w=a.r1.x*b.r1.w +a.r1.y*b.r2.w +a.r1.z*b.r3.w +a.r1.w*b.r4.w;

	n.r2.x=a.r2.x*b.r1.x +a.r2.y*b.r2.x +a.r2.z*b.r3.x +a.r2.w*b.r4.x;
	n.r2.y=a.r2.x*b.r1.y +a.r2.y*b.r2.y +a.r2.z*b.r3.y +a.r2.w*b.r4.y;
	n.r2.z=a.r2.x*b.r1.z +a.r2.y*b.r2.z +a.r2.z*b.r3.z +a.r2.w*b.r4.z;
	n.r2.w=a.r2.x*b.r1.w +a.r2.y*b.r2.w +a.r2.z*b.r3.w +a.r2.w*b.r4.w;

	n.r3.x=a.r3.x*b.r1.x +a.r3.y*b.r2.x +a.r3.z*b.r3.x +a.r3.w*b.r4.x;
	n.r3.y=a.r3.x*b.r1.y +a.r3.y*b.r2.y +a.r3.z*b.r3.y +a.r3.w*b.r4.y;
	n.r3.z=a.r3.x*b.r1.z +a.r3.y*b.r2.z +a.r3.z*b.r3.z +a.r3.w*b.r4.z;
	n.r3.w=a.r3.x*b.r1.w +a.r3.y*b.r2.w +a.r3.z*b.r3.w +a.r3.w*b.r4.w;

	n.r4.x=a.r4.x*b.r1.x +a.r4.y*b.r2.x +a.r4.z*b.r3.x +a.r4.w*b.r4.x;
	n.r4.y=a.r4.x*b.r1.y +a.r4.y*b.r2.y +a.r4.z*b.r3.y +a.r4.w*b.r4.y;
	n.r4.z=a.r4.x*b.r1.z +a.r4.y*b.r2.z +a.r4.z*b.r3.z +a.r4.w*b.r4.z;
	n.r4.w=a.r4.x*b.r1.w +a.r4.y*b.r2.w +a.r4.z*b.r3.w +a.r4.w*b.r4.w;
	return n; 

}
//Vector3 v3_mult(Vector3 a, Vector3 b)

