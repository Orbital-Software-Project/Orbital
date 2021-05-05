//#include <QDebug>
#include "image.h"
#include "calib_interface.h"
#include "calib.h"



Calibration::Calibration()
{ 

    status = false;
    CalImg = NULL;
    Kc = Rc = Tc = NULL;

    points = grid_points = NULL;
    xwCam = NULL;

}

void Calibration::reset()
{
    if(!points)
        delete[] points;
    if(!grid_points)
        delete[] grid_points;

    if(!Kc) delete[] Kc;
    if(!Rc) delete[] Rc;
    if(!Tc) delete[] Tc;

    Kc = Rc = Tc = NULL;
    points = grid_points = NULL;
    status = false;
}

bool Calibration::SetGrid(std::vector<point2D> m_points, int Nx, int Ny, float Dx, float Dy)
{
    nx = Nx;
    ny = Ny;
    dx = Dx;
    dy = Dy;

    if(!points)
        delete[] points;

    if( (points = new point2D[4]) == NULL)
        return false;

    if( m_points.size() != 4 )
        return false;

    for ( int i=0 ; i<4 ; i++ )
    {
        points[i].x= float(m_points[3-i].x);
        points[i].y= float(m_points[3-i].y);
    }

    if(grid_points != NULL)
        delete[] grid_points;

    if( ( grid_points = new point2D[ (nx+1)*(ny+1) ] ) == NULL )
        return false;

    return true;
}


void Calibration::ComputeGrid(bool calib_type)
{
    CAL_ExtractGrid (CalImg, points, nx, ny, grid_points, calib_type);
}


bool Calibration::CalibCam()
{
    status = false;

    if (CalImg == NULL)
        return false;

    int   wimg, himg;
    int   n = (nx+1)*(ny+1);

    xwCam = new point3D[n];

    CalImg->GetDim(&wimg,&himg);

    CAL_DetectCameraPoints(nx, ny, dx, dy, xwCam);

    if (grid_points == NULL)
        return false;

    if (Kc == NULL) Kc = new double[9];
    if (Rc == NULL) Rc = new double[9];
    if (Tc == NULL) Tc = new double[3];

    if(!CAL_CalibrateCamera( grid_points, xwCam, nx, ny, (wimg-1)/2.0, (himg-1)/2.0, Kc, Rc, Tc) )
    {
        return false;
    }

    status = true;
    return true;
}
