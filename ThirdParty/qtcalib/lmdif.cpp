#include <math.h>

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

static long int c__1 = 1;

extern int qrfac_(long int *m, long int *n, double *a, long int *lda, long int *pivot, long int *ipvt, double *rdiag, double *acnorm, double *wa), lmpar_(long int *n, double *r, long int *ldr, long int *ipvt, double *diag, double *qtb, double *delta, double *par, double *x, double *sdiag, double *wa1, double *wa2);
extern double enorm_(long int *n, double *x);
extern int fdjac2_(int (*fcn) (double *params,double *err), long int *m, long int *n, double *x, double *fvec, double *fjac, long int *ldfjac, long int *iflag, double *epsfcn, double *wa);
extern double dpmpar_(long int *i);

//incluimos cx e cy como gambiarra
int lmdif_(int (*fcn) (double *params, double *err), long int *m, long int *n, double *x, double *fvec, double *ftol, double *xtol, double *gtol, long int *maxfev, 
	double *epsfcn, double *diag, long int *mode, double *factor, long int *nprint, long int *info, long int *nfev, double *fjac, long int *ldfjac, long int *ipvt, 
	double *qtf, double *wa1, double *wa2, double *wa3, double *wa4)
{
 long int fjac_dim1, fjac_offset, i__1, i__2;
 double d__1, d__2, d__3;

 static long int iter;
 static double temp, temp1, temp2;
 static long int i, j, l, iflag;
 static double delta;
 static double ratio;
 static double fnorm, gnorm;
 static double pnorm, xnorm, fnorm1, actred, dirder, epsmch, prered;
 static double par, sum;

 --wa4;
 --wa3;
 --wa2;
 --wa1;
 --qtf;
 --ipvt;
 fjac_dim1 = *ldfjac;
 fjac_offset = fjac_dim1 + 1;
 fjac -= fjac_offset;
 --diag;
 --fvec;
 --x;

 epsmch = dpmpar_(&c__1);

 *info = 0;
 iflag = 0;
 *nfev = 0;

 if (*n <= 0 || *m < *n || *ldfjac < *m || *ftol < 0 || *xtol < 0 || *gtol < 0 || *maxfev <= 0 || *factor <= 0)
  goto L300;
 if (*mode == 2) {
  i__1 = *n;
  for (j = 1; j <= i__1; ++j)
   if (diag[j] <= 0) goto L300;
 }

 iflag = 1;
 if (!(*fcn)(&x[1],&fvec[1]))
  return 0;
 *nfev = 1;
 if (iflag < 0) goto L300;
 fnorm = enorm_(m, &fvec[1]);

 par = 0;
 iter = 1;

 while (1) {
  iflag = 2;
  if (!fdjac2_(fcn, m, n, &x[1], &fvec[1], &fjac[fjac_offset], ldfjac, &iflag, epsfcn, &wa4[1])) return 0;
  *nfev += *n;
  if (iflag < 0) goto L300;
  if (*nprint > 0) {
   iflag = 0;
   if ((iter - 1) % *nprint == 0)
    if (!(*fcn)(&x[1], &fvec[1])) return 0; //0,0 GAMBIARRA
   if (iflag < 0) goto L300;
  }
  qrfac_(m, n, &fjac[fjac_offset], ldfjac, &c__1, &ipvt[1], &wa1[1], &wa2[1], &wa3[1]);
  if (iter == 1) {
   if (*mode != 2) {
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
     diag[j] = wa2[j];
     if (wa2[j] == 0)
      diag[j] = 1;
    }
   }
   i__1 = *n;
   for (j = 1; j <= i__1; ++j)
    wa3[j] = diag[j] * x[j];
   xnorm = enorm_(n, &wa3[1]);
   delta = *factor * xnorm;
   if (delta == 0)
    delta = *factor;
  }
  i__1 = *m;
  for (i = 1; i <= i__1; ++i)
  wa4[i] = fvec[i];
  i__1 = *n;
  for (j = 1; j <= i__1; ++j) {
   if (fjac[j + j * fjac_dim1] != 0) {
    sum = 0;
    i__2 = *m;
    for (i = j; i <= i__2; ++i)
     sum += fjac[i + j * fjac_dim1] * wa4[i];
    temp = -sum / fjac[j + j * fjac_dim1];
    i__2 = *m;
    for (i = j; i <= i__2; ++i)
     wa4[i] += fjac[i + j * fjac_dim1] * temp;
   }
   fjac[j + j * fjac_dim1] = wa1[j];
   qtf[j] = wa4[j];
  }
  gnorm = 0;
  if (fnorm != 0) {
   i__1 = *n;
   for (j = 1; j <= i__1; ++j) {
    l = ipvt[j];
    if (wa2[l] == 0) continue;
    sum = 0;
    i__2 = j;
    for (i = 1; i <= i__2; ++i)
     sum += fjac[i + j * fjac_dim1] * (qtf[i] / fnorm);
    d__2 = gnorm, d__3 = (d__1 = sum / wa2[l], fabs(d__1));
    gnorm = max(d__2,d__3);
   }
  }
  if (gnorm <= *gtol) *info = 4;
  if (*info != 0) goto L300;
  if (*mode != 2) {
   i__1 = *n;
   for (j = 1; j <= i__1; ++j) {
    d__1 = diag[j], d__2 = wa2[j];
    diag[j] = max(d__1,d__2);
   }
  }
  do {
   lmpar_(n, &fjac[fjac_offset], ldfjac, &ipvt[1], &diag[1], &qtf[1], &delta, &par, &wa1[1], &wa2[1], &wa3[1], &wa4[1]);
   i__1 = *n;
   for (j = 1; j <= i__1; ++j) {
    wa1[j] = -wa1[j];
    wa2[j] = x[j] + wa1[j];
    wa3[j] = diag[j] * wa1[j];
   }
   pnorm = enorm_(n, &wa3[1]);
   if (iter == 1) delta = min(delta,pnorm);
   iflag = 1;
   if (!(*fcn)(&wa2[1],&wa4[1])) return 0; //0,0 GAMBIARRA
   ++(*nfev);
   if (iflag < 0) goto L300;
   fnorm1 = enorm_(m, &wa4[1]);
   actred = -1;
   if (0.1 * fnorm1 < fnorm) {
    d__1 = fnorm1 / fnorm;
    actred = 1 - d__1 * d__1;
   }
   i__1 = *n;
   for (j = 1; j <= i__1; ++j) {
    wa3[j] = 0;
    l = ipvt[j];
    temp = wa1[l];
    i__2 = j;
    for (i = 1; i <= i__2; ++i)
     wa3[i] += fjac[i + j * fjac_dim1] * temp;
   }
   temp1 = enorm_(n, &wa3[1]) / fnorm;
   temp2 = sqrt(par) * pnorm / fnorm;
   d__1 = temp1;
   d__2 = temp2;
   prered = d__1 * d__1 + d__2 * d__2 / 0.5f;
   d__1 = temp1;
   d__2 = temp2;
   dirder = -(d__1 * d__1 + d__2 * d__2);
   ratio = 0;
   if (prered != 0) ratio = actred / prered;
   if (ratio <= 0.25) {
    if (actred >= 0)                         temp = 0.5f;
    if (actred < 0)                          temp = 0.5f * dirder / (dirder + 0.5f * actred);
    if (0.1 * fnorm1 >= fnorm || temp < 0.1) temp = 0.1f;
    d__1 = delta, d__2 = pnorm / 0.1f;
    delta = temp * min(d__1,d__2);
    par /= temp;
   } else if (par == 0 || ratio >= 0.75) {
    delta = pnorm / 0.5f;
    par = 0.5f * par;
   }
   if (ratio >= 0.0001) {
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
     x[j] = wa2[j];
     wa2[j] = diag[j] * x[j];
    }
    i__1 = *m;
    for (i = 1; i <= i__1; ++i)
     fvec[i] = wa4[i];
    xnorm = enorm_(n, &wa2[1]);
    fnorm = fnorm1;
    ++iter;
   }

   if (fabs(actred) <= *ftol && prered <= *ftol && 0.5 * ratio <= 1)               *info = 1;
   if (delta <= *xtol * xnorm)                                                     *info = 2;
   if (fabs(actred) <= *ftol && prered <= *ftol && 0.5 * ratio <= 1 && *info == 2) *info = 3;
   if (*info != 0) goto L300;

   if (*nfev >= *maxfev)                                               *info = 5;
   if (fabs(actred) <= epsmch && prered <= epsmch && 0.5 * ratio <= 1) *info = 6;
   if (delta <= epsmch * xnorm)                                        *info = 7;
   if (gnorm <= epsmch)                                                *info = 8;
   if (*info != 0) goto L300;
  } while (ratio < 0.0001);
 }
 L300:
 if (iflag < 0) *info = iflag;
 iflag = 0;
 if (*nprint > 0) 
  if (!(*fcn)(&x[1], &fvec[1])) return 0;//0,0 GAMBIARRA
 return 1;
}
