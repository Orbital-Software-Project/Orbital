#include "file.h"

#define SCALE .01
#define FRAMES_PER_SECOND 24.0
#define INCHES_TO_MM 25.40 


CalibFile::CalibFile()
{

}

CalibFile::~CalibFile()
{

}

void CalibFile::FirstCamera(float Wc, float Hc, const double *Kc, const double *Rc, const double *Tc)
{
    verticalFilmOffset     = ( Hc/2. - Kc[5]  )*SCALE;
    horizontalFilmOffset   = ( Wc/2. - Kc[2]  )*SCALE;
    horizontalFilmAperture = Wc*SCALE;
    verticalFilmAperture   = Hc*SCALE;
    focusDistance          = Kc[0]*SCALE*INCHES_TO_MM;

    rt.r1.x=Rc[0]; rt.r1.y=Rc[1]; rt.r1.z=Rc[2];
    rt.r2.x=Rc[3]; rt.r2.y=Rc[4]; rt.r2.z=Rc[5];
    rt.r3.x=Rc[6]; rt.r3.y=Rc[7]; rt.r3.z=Rc[8];

    rt.r1.w=Tc[0]; rt.r2.w=Tc[1]; rt.r3.w=Tc[2];

    t = v3_make(  rt.r1.w, rt.r2.w, rt.r3.w );
    maux = rt;
    maux.r1.w = maux.r2.w =  maux.r3.w = 0.;
    maux = m4_transpose( maux );
    from = v3_scale( -1., v3_m3mult(t, maux) );

    d  = v3_make( rt.r3.x, rt.r3.y, rt.r3.z );
    up = v3_make( -rt.r2.x, -rt.r2.y, -rt.r2.z );

}


void CalibFile::SecondCamera( const double *Re, const double *Te)
{
    Matrix4 A = { {0.f, 0.f, 1.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}};
    rt.r1.x=Re[0]; rt.r1.y=Re[1]; rt.r1.z=Re[2];
    rt.r2.x=Re[3]; rt.r2.y=Re[4]; rt.r2.z=Re[5];
    rt.r3.x=Re[6]; rt.r3.y=Re[7]; rt.r3.z=Re[8];

    rt.r1.w=Te[0]; rt.r2.w=Te[1]; rt.r3.w=Te[2];

    t = v3_make(  rt.r1.w, rt.r2.w, rt.r3.w );
    maux = rt;
    maux.r1.w = maux.r2.w =  maux.r3.w = 0.;
    maux = m4_transpose( maux );
    from = v3_scale( -1., v3_m3mult(t, maux) );

    d = v3_make( rt.r3.x, rt.r3.y, rt.r3.z );
    up = v3_make( -rt.r2.x, -rt.r2.y, -rt.r2.z );

    NUP = v3_unit(up);
    DIR = v3_unit(d);
    RGT = v3_cross(DIR, NUP);
    RUDT= m4_ident();
    RUDT.r1 = v4_v3conv(RGT);
    RUDT.r2 = v4_v3conv(NUP);
    RUDT.r3 = v4_v3conv(DIR);
    RUDT.r1.w = RUDT.r2.w = RUDT.r3.w = 0.f;
    DRU = m4_m4prod(A, RUDT);
}

int CalibFile::SaveCalibration(const char *filename, int format, float Wc, float Hc, const double *Kc,
                               const double *Rc, const double *Tc, const double *Ke, const double *Re, const double *Te)
{

    FILE *arq;

    // TMP
   // if( (arq=fopen_s(filename,"w")) == NULL ) return 0;

    if( (Kc != NULL) && (Rc != NULL) && (Tc != NULL) )
    {
        FirstCamera(Wc, Hc, Kc, Rc, Tc);
        switch (format) {
        case 0: fprintf(arq,"# CAMERA CALIBRATION\n\n");
            matrix(arq, Kc, Rc, Tc);
            break;
        case 1: maya(arq);
            break;
        case 2: pbrt_first(arq, Wc, Hc, Kc);
            break;
        case 3: lux_first(arq, Wc, Hc, Kc);
            break;
        default : break;
        }
    }

    if( (Ke != NULL) && (Re != NULL) && (Te != NULL) )
    {
        SecondCamera( Re, Te);
        switch (format) {
        case 0: fprintf(arq,"# LIGHT CAMERA CALIBRATION\n\n");
            matrix(arq, Ke, Re, Te);
            break;
        case 1:
            break;
        case 2: pbrt_second(arq);
            break;
        case 3: lux_second(arq);
            break;
        default : break;
        }
    }

    //fclose(arq);
    return 1;
}

void CalibFile::print_lookat(FILE *arq)
{
    fprintf(arq, "%lf %lf %lf\n", from.x, from.y, from.z);
    fprintf(arq, "%lf %lf %lf\n", from.x+d.x, from.y+d.y, from.z+d.z);
    fprintf(arq, "%lf %lf %lf\n", up.x, up.y, up.z);
}

void CalibFile::light_transform(FILE *arq)
{
    fprintf(arq, "%lf %lf %lf %lf\n", DRU.r1.x ,DRU.r1.y ,DRU.r1.z ,DRU.r1.w);
    fprintf(arq, "%lf %lf %lf %lf\n", DRU.r2.x ,DRU.r2.y ,DRU.r2.z ,DRU.r2.w);
    fprintf(arq, "%lf %lf %lf %lf\n", DRU.r3.x ,DRU.r3.y ,DRU.r3.z ,DRU.r3.w);
    fprintf(arq, "%lf %lf %lf %lf\n", DRU.r4.x ,DRU.r4.y ,DRU.r4.z ,DRU.r4.w);
}

void CalibFile::lux_first(FILE *arq, float Wc, float Hc, const double *Kc )
{
    fprintf(arq, "# LUX CAMERA\n\n#LookAt\n");
    print_lookat(arq);

    fprintf(arq, "\n#float focaldistance\n%lf\n", focusDistance );
    fprintf(arq, "#float fov\n%lf\n", atan ( MAX(Wc,Hc)/(2.0*Kc[0]) ) * 360.0/3.14159265 );
    fprintf(arq, "#int resolution\n%d %d\n", (int)Wc, (int)Hc);
}

void CalibFile::lux_second(FILE *arq)
{
    fprintf(arq, "\n\n# LIGHT TRANSFORM\n\n");
    fprintf(arq, "#Transform\n");
    light_transform(arq);
}

void CalibFile::pbrt_first(FILE *arq, float Wc, float Hc, const double *Kc )
{
    fprintf(arq, "# PBRT CAMERA\n\nLookAt\n");
    print_lookat(arq);

    fprintf(arq, "\n\"float focaldistance\" [%lf]\n", focusDistance );
    fprintf(arq, "\"float fov\" [%lf] \n", atan ( MAX(Wc,Hc)/(2.0*Kc[0]) ) * 360.0/3.14159265 );
}

void CalibFile::pbrt_second(FILE *arq)
{
    fprintf(arq, "\n\n# LIGHT TRANSFORM\n\nTransform  [\n");
    light_transform(arq);
    fprintf(arq, "]");
}


void CalibFile::matrix(FILE *arq, const double *K, const double *R, const double *T)
{
    fprintf(arq,"K: \n");
    fprintf(arq,"%lf %lf %lf \n", K[0], K[1], K[2]);
    fprintf(arq,"%lf %lf %lf \n", K[3], K[4], K[5]);
    fprintf(arq,"%lf %lf %lf \n\n", K[6], K[7], K[8]);

    fprintf(arq,"R: \n");
    fprintf(arq,"%lf %lf %lf \n", R[0], R[1], R[2]);
    fprintf(arq,"%lf %lf %lf \n", R[3], R[4], R[5]);
    fprintf(arq,"%lf %lf %lf \n\n", R[6], R[7], R[8]);

    fprintf(arq,"T: \n");
    fprintf(arq,"%lf %lf %lf \n\n", T[0], T[1], T[2]);
}


void CalibFile::maya(FILE *arq)
{
    fprintf(arq, "MAYA CAMERA\n\n");
    fprintf(arq, "camera -focalLength %lf ", focusDistance );
    fprintf(arq, "-verticalFilmOffset %lf -horizontalFilmOffset %lf ", verticalFilmOffset, horizontalFilmOffset );
    fprintf(arq, "-verticalFilmAperture %lf -horizontalFilmAperture %lf ", verticalFilmAperture, horizontalFilmAperture);
    fprintf(arq, "-p 0 0 0;\n" );

    //original
    // fprintf(arq, "viewPlace -eye %lf %lf %lf -viewDirection %lf %lf %lf -up %lf %lf %lf;\n", from.x, from.y, from.z,
    //                                                 d.x, d.y, d.z, up.x, up.y, up.z );

    //For Luxrender maya plugin
    fprintf(arq, "viewPlace -eye %lf %lf %lf -viewDirection %lf %lf %lf -up %lf %lf %lf;\n", from.x, from.z, -from.y,
            d.x, d.z, -d.y, up.x, up.z, -up.y );

}
