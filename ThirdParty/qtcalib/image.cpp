#include "image.h"

Image::Image()
{
    width = 0;
    height = 0;
    rgb = NULL;
}


Image::~Image(void)
{
    Unload();
}


int Image::Load (int w, int h,unsigned char *r_g_b)
{
    int	j,n;

    width = w; height = h;
    n = width*height;
    if( (rgb=new unsigned char[3*n]) == NULL )
    {
        return 0;
    }

    for(j=0; j<(3*n); j++)
    {
        rgb[j] = r_g_b[j];
    }
    return 1;
}


void  Image::GetDim(int *w, int *h)
{
    *w=width; *h=height;
}


int  Image::IsEmpty(void)
{
    return (rgb==NULL);
}


void  Image::Unload(void)
{
    if (rgb!=NULL) {delete []rgb; rgb=NULL;}
    width = height = 0;
}


float   Image::GetGray(int i,int j)
{
    float red,green,blue;

    red  = (float) rgb[3*(i+j*width)];
    green= (float) rgb[3*(i+j*width)+1];
    blue = (float) rgb[3*(i+j*width)+2];

    return (red+green+blue)/3;
}


float   Image::GetGray(int i)
{
    float red,green,blue;

    red  = (float) rgb[3*i];
    green= (float) rgb[3*i+1];
    blue = (float) rgb[3*i+2];

    return (red+green+blue)/3;
}
