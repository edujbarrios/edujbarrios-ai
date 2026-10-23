#ifndef EDUJBARRIOS_AI_PCA_2D_H
#define EDUJBARRIOS_AI_PCA_2D_H
#include <stdbool.h>
#include <stddef.h>
/* Read n>=2 row-major (x,y) pairs. Caller provides mean[2], direction[2],
   projection[n], all disjoint from the input and each other.
   Outputs are unchanged on error. */
bool pca_2d(const double *points, size_t n, double *mean,
            double *direction, double *projection);
#endif
