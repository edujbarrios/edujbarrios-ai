#ifndef EDUJBARRIOS_AI_RIDGE_REGRESSION_1D_H
#define EDUJBARRIOS_AI_RIDGE_REGRESSION_1D_H
#include <stdbool.h>
#include <stddef.h>
/* Fit y=intercept+slope*x, penalizing only slope.
   Finite aligned vectors n=1..4096, coordinates in [-1e6,1e6],
   0<=lambda<=1e12. Output pointers are distinct and do not alias
   input. Failure leaves both outputs unchanged. */
bool ridge_regression_1d(const double *x,const double *y,size_t n,
                         double lambda,double *intercept,double *slope);
#endif
