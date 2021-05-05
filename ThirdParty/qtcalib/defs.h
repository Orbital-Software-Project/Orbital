/*
 * defs.h  
 * (based on GraphicsGems.h, by Andrew Glassner)
 */

#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* absolute value of a */
#define ABS(a)      (((a)<0)?-(a):(a))

/* square a */
#define SQR(a)		((a)*(a))	

/* find maximum of a and b */
#define MAX(a,b)	(((a)>(b))?(a):(b))

/* swap a and b */
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}


typedef double Real;

#endif // DEFS_H

