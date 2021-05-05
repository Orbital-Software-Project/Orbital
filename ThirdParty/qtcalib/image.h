#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>

class Image
{
private:
    int  width;
    int  height;
    unsigned char *rgb;

public:
    Image(void);
    ~Image(void);
    int Load ( int w, int h,unsigned char *r_g_b);
    void GetDim(int *w, int *h);
    float GetGray(int i,int j);
    float GetGray(int i);
    int  IsEmpty(void);
    void Unload(void);
};

#endif // IMAGE_H
