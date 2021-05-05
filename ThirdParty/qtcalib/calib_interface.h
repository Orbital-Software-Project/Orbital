#ifndef CALIB_INTERFACE_H
#define CALIB_INTERFACE_H

#include <vector>

#include "coord.h"
#include "image.h"

class Calibration
{

public:
    Image    *CalImg;

    point2D  *points, *grid_points;
    point3D  *xwCam;

    int      nx, ny;
    float    dx, dy;
    bool     status;

//    double   Kc[9], Rc[9], Tc[3];
    double *Kc, *Rc, *Tc;

    Calibration(void);

    ~Calibration(void);

    bool CalibCam(void);

    void reset();

    void ComputeGrid(bool calib_type);

    bool SetGrid(std::vector<point2D> m_points, int Nx, int Ny, float Dx, float Dy );

};

#endif // CALIB_INTERFACE_H
