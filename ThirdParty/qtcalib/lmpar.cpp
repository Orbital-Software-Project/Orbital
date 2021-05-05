#include <math.h>

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

static long int c__2 = 2;

extern double enorm_(long int *n, double *x);
extern double dpmpar_(long int *i);
extern int qrsolv_(long int *n, double *r, long int *ldr, long int *ipvt, double *diag, double *qtb, double *x, double *sdiag, double *wa);

int lmpar_(long int *n, double *r, long int *ldr, long int *ipvt, double *diag, double *qtb, double *delta, double *par, double *x, double *sdiag, double *wa1, double *wa2)
{
    /* System generated locals */
    long int r_dim1, r_offset, i__1, i__2;
    double d__1, d__2;

    /* Local variables */
    static double parc, parl;
    static long int iter;
    static double temp, paru;
    static long int i, j, k, l;
    static double dwarf;
    static long int nsing;
    static double gnorm, fp;
    static double dxnorm;
    static long int jm1, jp1;
    static double sum;

    /* Parameter adjustments */
    --wa2;
    --wa1;
    --sdiag;
    --x;
    --qtb;
    --diag;
    --ipvt;
    r_dim1 = *ldr;
    r_offset = r_dim1 + 1;
    r -= r_offset;

    /* Function Body */

/*     dwarf is the smallest positive magnitude. */

    dwarf = dpmpar_(&c__2);

/*     compute and store in x the gauss-newton direction. if the */
/*     jacobian is rank-deficient, obtain a least squares solution. */

    nsing = *n;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] = qtb[j];
	if (r[j + j * r_dim1] == 0 && nsing == *n) {
	    nsing = j - 1;
	}
	if (nsing < *n) {
	    wa1[j] = 0;
	}
    }
    if (nsing < 1) {
	goto L50;
    }
    i__1 = nsing;
    for (k = 1; k <= i__1; ++k) {
	j = nsing - k + 1;
	wa1[j] /= r[j + j * r_dim1];
	temp = wa1[j];
	jm1 = j - 1;
	if (jm1 < 1) {
	    goto L30;
	}
	i__2 = jm1;
	for (i = 1; i <= i__2; ++i) {
	    wa1[i] -= r[i + j * r_dim1] * temp;
	}
L30:
	;
    }
L50:
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	x[l] = wa1[j];
    }

/*     initialize the iteration counter. */
/*     evaluate the function at the origin, and test */
/*     for acceptance of the gauss-newton direction. */

    iter = 0;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa2[j] = diag[j] * x[j];
    }
    dxnorm = enorm_(n, &wa2[1]);
    fp = dxnorm - *delta;
    if (fp <= 0.1 * *delta) {
	goto L220;
    }

/*     if the jacobian is not rank deficient, the newton */
/*     step provides a lower bound, parl, for the 0 of */
/*     the function. otherwise set this bound to 0. */

    parl = 0;
    if (nsing < *n) {
	goto L120;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	wa1[j] = diag[l] * (wa2[l] / dxnorm);
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sum = 0;
	jm1 = j - 1;
	if (jm1 < 1) {
	    goto L100;
	}
	i__2 = jm1;
	for (i = 1; i <= i__2; ++i) {
	    sum += r[i + j * r_dim1] * wa1[i];
	}
L100:
	wa1[j] = (wa1[j] - sum) / r[j + j * r_dim1];
    }
    temp = enorm_(n, &wa1[1]);
    parl = fp / *delta / temp / temp;
L120:

/*     calculate an upper bound, paru, for the 0 of the function. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sum = 0;
	i__2 = j;
	for (i = 1; i <= i__2; ++i) {
	    sum += r[i + j * r_dim1] * qtb[i];
	}
	l = ipvt[j];
	wa1[j] = sum / diag[l];
    }
    gnorm = enorm_(n, &wa1[1]);
    paru = gnorm / *delta;
    if (paru == 0) {
	paru = dwarf / min(*delta,0.1f);
    }

/*     if the input par lies outside of the interval (parl,paru), */
/*     set par to the closer endpoint. */

    *par = max(*par,parl);
    *par = min(*par,paru);
    if (*par == 0) {
	*par = gnorm / dxnorm;
    }

/*     beginning of an iteration. */

L150:
    ++iter;

/*        evaluate the function at the current value of par. */

    if (*par == 0) {
/* Computing MAX */
	d__1 = dwarf, d__2 = 0.001f * paru;
	*par = max(d__1,d__2);
    }
    temp = sqrt(*par);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] = temp * diag[j];
    }
    qrsolv_(n, &r[r_offset], ldr, &ipvt[1], &wa1[1], &qtb[1], &x[1], &sdiag[1]
	    , &wa2[1]);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa2[j] = diag[j] * x[j];
    }
    dxnorm = enorm_(n, &wa2[1]);
    temp = fp;
    fp = dxnorm - *delta;

/*        if the function is small enough, accept the current value */
/*        of par. also test for the exceptional cases where parl */
/*        is 0 or the number of iterations has reached 10. */

    if ( (fabs(fp) <= 0.1 * *delta) ||( (parl == 0) && (fp <= temp) && (temp < 0) )||
             (iter == 10 ) ) {
	goto L220;
    }

/*        compute the newton correction. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	wa1[j] = diag[l] * (wa2[l] / dxnorm);
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] /= sdiag[j];
	temp = wa1[j];
	jp1 = j + 1;
	if (*n < jp1) {
	    goto L200;
	}
	i__2 = *n;
	for (i = jp1; i <= i__2; ++i) {
	    wa1[i] -= r[i + j * r_dim1] * temp;
	}
L200:
	;
    }
    temp = enorm_(n, &wa1[1]);
    parc = fp / *delta / temp / temp;

/*        depending on the sign of the function, update parl or paru. */

    if (fp > 0) {
	parl = max(parl,*par);
    }
    if (fp < 0) {
	paru = min(paru,*par);
    }

/*        compute an improved estimate for par. */

/* Computing MAX */
    d__1 = parl, d__2 = *par + parc;
    *par = max(d__1,d__2);

/*        end of an iteration. */

    goto L150;
L220:

/*     termination. */

    if (iter == 0) {
	*par = 0;
    }
    return 0;
}
