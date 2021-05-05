#include <math.h>

int qrsolv_(long int *n, double *r, long int *ldr, long int *ipvt, double *diag, double *qtb, double *x, double *sdiag, double *wa)
{
    static long int r_dim1;
    static double temp;
    static long int i, j, k, l;
    static double cotan;
    static long int nsing;
    static double qtbpj;
    static double tan_, cos_, sin_, sum;

    --wa;
    --sdiag;
    --x;
    --qtb;
    --diag;
    --ipvt;
    r_dim1 = *ldr;
    r = r - r_dim1 - 1;

    for (j = 1; j <= *n; ++j) {
        for (i = j; i <= *n; ++i)
            r[i + j * r_dim1] = r[j + i * r_dim1];
        x[j] = r[j + j * r_dim1];
        wa[j] = qtb[j];
    }

    for (j = 1; j <= *n; ++j) {
        l = ipvt[j];
        if (diag[l] != 0) {
            for (k = j; k <= *n; ++k)
                sdiag[k] = 0;
            sdiag[j] = diag[l];
            qtbpj = 0;
            for (k = j; k <= *n; ++k) {
                if (sdiag[k] != 0) {
                    if (fabs(r[k + k * r_dim1]) < fabs(sdiag[k])) {
                        cotan = r[k + k * r_dim1] / sdiag[k];
                        sin_ = 0.5f / ((double)sqrt(0.25 + 0.25 * (cotan * cotan)));
                        cos_ = sin_ * cotan;
                    } else {
                        tan_ = sdiag[k] / r[k + k * r_dim1];
                        cos_ = 0.5f / ((double)sqrt(0.25 + 0.25 * (tan_ * tan_)));
                        sin_ = cos_ * tan_;
                    }
                    r[k + k * r_dim1] = cos_ * r[k + k * r_dim1] + sin_ * sdiag[k];
                    temp = cos_ * wa[k] + sin_ * qtbpj;
                    qtbpj = -sin_ * wa[k] + cos_ * qtbpj;
                    wa[k] = temp;
                    if (*n >= k+1) {
                        for (i = k+1; i <= *n; ++i) {
                            temp = cos_ * r[i + k * r_dim1] + sin_ * sdiag[i];
                            sdiag[i] = -sin_ * r[i + k * r_dim1] + cos_ * sdiag[i];
                            r[i + k * r_dim1] = temp;
                        }
                    }
                }
            }
        }
        sdiag[j] = r[j + j * r_dim1];
        r[j + j * r_dim1] = x[j];
    }
    nsing = *n;
    for (j = 1; j <= *n; ++j) {
        if (sdiag[j] == 0 && nsing == *n)
            nsing = j - 1;
        if (nsing < *n) wa[j] = 0;
    }
    if (nsing >= 1)
        for (k = 1; k <= nsing; ++k) {
            j = nsing - k + 1;
            sum = 0;
            if (nsing >= j+1)
                for (i = j+1; i <= nsing; ++i)
                    sum += r[i + j * r_dim1] * wa[i];
            wa[j] = (wa[j] - sum) / sdiag[j];
        }
    for (j = 1; j <= *n; ++j)
        x[ipvt[j]] = wa[j];
    return 0;
}
