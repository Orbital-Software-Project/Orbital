#include <math.h>

double enorm_(long int *n, double *x)
{
 static double rdwarf = (double)3.834e-20;
 static double rgiant = (double)1.304e19;
 double ret_val = 0., d__1;
 static double xabs, x1max, x3max;
 static long int i;
 static double s1, s2, s3, agiant;

 --x;
 s1 = 0.0;
 s2 = 0.0;
 s3 = 0.0;
 x1max = 0.0;
 x3max = 0.0;
 agiant = rgiant / *n;
 for (i = 1; i <= *n; ++i) {
  xabs = fabs(x[i]);
  if (xabs <= rdwarf || xabs >= agiant) {
   if (xabs > rdwarf) {
    if (xabs > x1max) {
     d__1 = x1max / xabs;
     s1 = 1.0 + s1 * (d__1 * d__1);
     x1max = xabs;
    } else {
     d__1 = xabs / x1max;
     s1 += d__1 * d__1;
    }
   } else {
    if (xabs > x3max) {
     d__1 = x3max / xabs;
     s3 = 1.0 + s3 * (d__1 * d__1);
     x3max = xabs;
    } else if (xabs != 0.0) {
     d__1 = xabs / x3max;
     s3 += d__1 * d__1;
    }
   }
  } else
   s2 += xabs * xabs;
 }
 if (s1 != 0.0)
     ret_val = x1max * sqrt(s1 + s2 / x1max / x1max);
 else if (s2 != 0.0) {
     if (s2 >= x3max) {
         d__1 = x3max * s3;
         ret_val = sqrt(s2 * (1.0 + x3max / s2 * d__1));
     }
     if (s2 < x3max)
         ret_val = sqrt(x3max * (s2 / x3max + x3max * s3));
 } else
     ret_val = x3max * sqrt(s3);
 return ret_val;
}
