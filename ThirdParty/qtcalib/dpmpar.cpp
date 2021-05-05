double dpmpar_(long int *i)
{
 static struct {
  long int e_1[6];
  double    fill_2[1];
  double    e_3;
 } equiv_2 = { {1018167296, 0, 1048576, 0, 2146435071, -1}, {0}, 0. };

 /* System generated locals */
 double ret_val;

 /* Local variables */
 #define dmach ((double *)&equiv_2)
 #define minmag ((long int *)&equiv_2 + 2)
 #define maxmag ((long int *)&equiv_2 + 4)
 #define mcheps ((long int *)&equiv_2)

 ret_val = dmach[*i - 1];
 return ret_val;
}

#undef mcheps
#undef maxmag
#undef minmag
#undef dmach


