#include <math.h>
#include "matrix.h"
#include "defs.h"



#define TOL 1e-10

static int Gauss (int n,                      // [in] Numero de equacoes
                  double a[MAXDIM][MAXDIM],   // [in] Matriz dos coeficientes
                  double *b)                  // [in] Vetor das constantes
// [out] Vetor solucao
// Valor de retorno : 0 => sem solucao ou infinitas solucoes
//                    1 => solucao unica
{
    int    i, j, k;
    int    imax;             // Linha pivot
    double amax, rmax, temp;  // Auxiliares

    for (j=0; j<n-1; j++) {  // Loop nas colunas de a
        rmax = 0.0f;
        imax = j;
        for (i=j; i<n; i++) {   // Loop para determinar maior relacao a[i][j]/a[i][k]
            amax = 0.0f;
            for (k=j; k<n; k++)    // Loop para determinar maior elemento da linha i
                if (ABS(a[i][k]) > amax)
                    amax = ABS(a[i][k]);
            if (amax < TOL)        // Verifica se os elementos da linha sao nulos
                return 0;             // Finaliza subrotina pois o sistema nao tem solucao
            else if ((ABS(a[i][j]) > rmax*amax) && (ABS(a[i][j]) >= TOL)) { // Testa relacao da linha corrente
                rmax = ABS(a[i][j]) / amax;
                imax = i;             // Encontra linha de maior relacao - linha pivot
            }
        }
        if (ABS(a[imax][j])<TOL) {         // Verifica se o pivot e' nulo
            for (k=imax+1; k<n; k++)          // Procura linha com pivot nao nulo
                if (ABS(a[k][j]) < TOL)
                    imax = k;                       // Troca linha j por linha k
            if (ABS(a[imax][j]) < TOL)
                return 0;                        // nao existe solucao unica para o sistema
        }
        if (imax != j) {                   // Troca linha j por linha imax
            for (k=j; k<n; k++)
                SWAP(a[imax][k], a[j][k]);
            SWAP(b[imax], b[j]);
        }
        for (i=j+1; i<n; i++) {            // Anula elementos abaixo da diagonal
            double aux = a[i][j] / a[j][j];
            for (k=j+1; k<n; k++)             // Transforma os demais elementos da linha
                a[i][k] -= aux * a[j][k];
            b[i] -= aux * b[j];
        }
    }
    if (ABS(a[n-1][n-1]) <= TOL)        // Verifica unicidade da solucao
        return 0;                          // Sistema sem solucao
    else {
        b[n-1] /= a[n-1][n-1];             // Calcula ultima coordenada
        for (i=n-2; i>=0; i--) {           // Inicia retro-substituicao
            for (j=i+1; j<n; j++)
                b[i] -= a[i][j] * b[j];
            b[i] /= a[i][i];
        }
    }
    return 1;                           // Finaliza a subrotina com solucao unica
}



int MinQuad(int n, double A[2*MAX_POINTS][2], double B[2*MAX_POINTS])
{
    int i, j, k;
    double d, *pB;

    double X[MAXDIM], M[MAXDIM][MAXDIM];


    for (i=0; i<2; i++)
        for (j=0; j<2; j++) {
            M[i][j] = A[0][j]*A[0][i];
            for (k=1; k<n; k++)
                M[i][j] += A[k][j]*A[k][i];
        }

    if ((d = M[0][0]*M[1][1] - M[0][1]*M[1][0]) == 0) return 0;

    pB = B;
    X[0] = A[0][0] * *pB,
            X[1] = A[0][1] * *pB;
    for (k=1, pB++; k<n; k++, pB++) {
        X[0] += A[k][0] * *pB,
                X[1] += A[k][1] * *pB;
    }

    B[0] = ( M[1][1]*X[0] - M[0][1]*X[1])/d,
            B[1] = (-M[1][0]*X[0] + M[0][0]*X[1])/d;

    return 1;
}

void MinQuad5(int n, double A[MAX_POINTS][5], double B[MAX_POINTS])
{
    int    i, j, k;
    double *pX, *pB;
    double X[MAXDIM], M[MAXDIM][MAXDIM];

    for (i=0; i<5; i++)
        for (j=0; j<5; j++) {
            M[i][j] = A[0][j]*A[0][i];
            for (k=1; k<n; k++)
                M[i][j] += A[k][j]*A[k][i];
        }

    pX = X;
    for (i=0; i<5; i++, pX++) {
        pB = B;
        *pX = A[0][i] * *pB;
        for (k=1, pB++; k<n; k++, pB++)
            *pX += A[k][i] * *pB;
    }

    Gauss (5, M, X);

    B[0] = X[0];
    B[1] = X[1];
    B[2] = X[2];
    B[3] = X[3];
    B[4] = X[4];
}
