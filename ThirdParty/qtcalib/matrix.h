#ifndef MATRIX_H
#define MATRIX_H

#define MAXPTS 30
#define MAXDIM 9+MAXPTS

/* Maximum number of data points allowed */ /*Used in Cal_main.h*/
#define MAX_POINTS	500

int   MinQuad(int n, double A[2*MAX_POINTS][2], double B[2*MAX_POINTS]);
void  MinQuad5(int n, double A[MAX_POINTS][5], double B[MAX_POINTS]);

#endif // MATRIX_H


