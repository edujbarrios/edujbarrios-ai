#ifndef EDUJBARRIOS_AI_GAUSSIAN_RBF_KERNEL_H
#define EDUJBARRIOS_AI_GAUSSIAN_RBF_KERNEL_H
#include <stdbool.h>
#include <stddef.h>
/* Dense row-major n*d data and n*n result, n=1..128, d=1..32.
   |coordinate|<=1e6, 1e-6<=sigma<=1e6. Caller owns a result
   buffer disjoint from input; entire output unchanged on error. */
bool gaussian_rbf_kernel(const double *points,size_t n,size_t d,
                         double sigma,double *kernel);
#endif
