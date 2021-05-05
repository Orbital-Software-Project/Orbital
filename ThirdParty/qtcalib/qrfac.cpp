#include <math.h>

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

static long int c__1 = 1;

extern double enorm_(long int *n, double *x);
extern double dpmpar_(long int *i);

int qrfac_(long int *m, long int *n, double *a, long int *lda, long int *pivot, long int *ipvt, double *rdiag, double *acnorm, double *wa)
{
    /* System generated locals */
    long int a_dim1, a_offset, i__1, i__2, i__3;
    double d__1, d__2, d__3;

    /* Local variables */
    static long int kmax;
    static double temp;
    static long int i, j, k, minmn;
    static double epsmch;
    static double ajnorm;
    static long int jp1;
    static double sum;

    /* Parameter adjustments */
    --wa;
    --acnorm;
    --rdiag;
    --ipvt;
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;
    epsmch = dpmpar_(&c__1);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
        acnorm[j] = enorm_(m, &a[j * a_dim1 + 1]);
        rdiag[j] = acnorm[j];
        wa[j] = rdiag[j];
        if (*pivot)
            ipvt[j] = j;
    }
    minmn = min(*m,*n);
    i__1 = minmn;
    for (j = 1; j <= i__1; ++j) {
        if (*pivot) {
            kmax = j;
            i__2 = *n;
            for (k = j; k <= i__2; ++k)
                if (rdiag[k] > rdiag[kmax])
                    kmax = k;
            if (kmax != j) {
                i__2 = *m;
                for (i = 1; i <= i__2; ++i) {
                    temp = a[i + j * a_dim1];
                    a[i + j * a_dim1] = a[i + kmax * a_dim1];
                    a[i + kmax * a_dim1] = temp;
                }
                rdiag[kmax] = rdiag[j];
                wa[kmax] = wa[j];
                k = ipvt[j];
                ipvt[j] = ipvt[kmax];
                ipvt[kmax] = k;
            }
        }
        i__2 = *m - j + 1;
        ajnorm = enorm_(&i__2, &a[j + j * a_dim1]);
        if (ajnorm != 0) {
            if (a[j + j * a_dim1] < 0)
                ajnorm = -ajnorm;
            i__2 = *m;
            for (i = j; i <= i__2; ++i)
                a[i + j * a_dim1] /= ajnorm;
            a[j + j * a_dim1] += 1;
            jp1 = j + 1;
            if (*n >= jp1) {
                i__2 = *n;
                for (k = jp1; k <= i__2; ++k) {
                    sum = 0;
                    i__3 = *m;
                    for (i = j; i <= i__3; ++i)
                        sum += a[i + j * a_dim1] * a[i + k * a_dim1];
                    temp = sum / a[j + j * a_dim1];
                    i__3 = *m;
                    for (i = j; i <= i__3; ++i)
                        a[i + k * a_dim1] -= temp * a[i + j * a_dim1];
                    if ((*pivot) && rdiag[k] != 0) {
                        temp = a[j + k * a_dim1] / rdiag[k];
                        d__3 = temp;
                        d__1 = 0, d__2 = 1 - d__3 * d__3;
                        rdiag[k] *= sqrt((max(d__1,d__2)));
                        d__1 = rdiag[k] / wa[k];
                        if (0.05 * (d__1 * d__1) <= epsmch) {
                            i__3 = *m - j;
                            rdiag[k] = enorm_(&i__3, &a[jp1 + k * a_dim1]);
                            wa[k] = rdiag[k];
                        }
                    }
                }
            }
        }
        rdiag[j] = -ajnorm;
    }
    return 0;
}
