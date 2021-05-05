#ifndef FILE_H
#define FILE_H

#include "linearalg.h"
#include "point.h"
#include <math.h>
#include <stdio.h>
#include <vector>


class CalibFile
{
private:
    double focusDistance;
    double aspectRatio;
    double verticalFilmOffset;
    double horizontalFilmOffset;
    double horizontalFilmAperture;
    double verticalFilmAperture;

    Matrix4 rt, maux;
    Vector3 t, from, d, up;

    Vector3 NUP, DIR, RGT;
    Matrix4 RUDT, DRU;

public:
    CalibFile(void);
    ~CalibFile(void);
    void print_lookat(FILE *arq);
    void light_transform(FILE *arq);

    void lux_first(FILE *arq, float Wc, float Hc, const double *Kc );
    void lux_second(FILE *arq);

    void pbrt_first(FILE *arq, float Wc, float Hc, const double *Kc );
    void pbrt_second(FILE *arq);

    void matrix(FILE *arq, const double *K, const double *R, const double *T);
    void maya(FILE *arq);

    void FirstCamera(float Wc, float Hc, const double *Kc, const double *Rc, const double *Tc);
    void SecondCamera(const double *Re, const double *Te);
    int SaveCalibration(const char *filename, int format, float Wc, float Hc, const double *Kc,
                        const double *Rc, const double *Tc,const double *Ke, const double *Re, const double *Te);

};


#endif // FILE_H
