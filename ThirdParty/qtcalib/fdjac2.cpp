#include <math.h>

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

static long int c__1 = 1;

extern double dpmpar_(long int *i);

//cx e cy sao gambiarra
int fdjac2_(int (*fcn) (double *params,double *err), long int *m, long int *n, double *x, double *fvec, double *fjac, long int *ldfjac, long int *iflag, double *epsfcn, double *wa)
{
    /* Initialized data */

    static double zero = 0.;

    /* System generated locals */
    long int fjac_dim1, fjac_offset, i__1, i__2;

    /* Local variables */
    static double temp, h;
    static long int i, j;
    static double epsmch;
    static double eps;

    /* Parameter adjustments */
    --wa;
    fjac_dim1 = *ldfjac;
    fjac_offset = fjac_dim1 + 1;
    fjac -= fjac_offset;
    --fvec;
    --x;

    /* Function Body */

/*     epsmch is the machine precision. */

    epsmch = dpmpar_(&c__1);

    eps = sqrt((max(*epsfcn,epsmch)));
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	temp = x[j];
	h = eps * fabs(temp);
	if (h == zero) {
	    h = eps;
	}
	x[j] = temp + h;
	if (!(*fcn)(&x[1],&wa[1])) return 0; //0,0 GAMBIARRA
	if (*iflag < 0) {
	    goto L30;
	}
	x[j] = temp;
	i__2 = *m;
	for (i = 1; i <= i__2; ++i) {
	    fjac[i + j * fjac_dim1] = (wa[i] - fvec[i]) / h;
	}
    }
L30:
    return 1;
}
