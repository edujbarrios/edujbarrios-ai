#ifndef EDUJBARRIOS_AI_WEIGHTED_KNN_REGRESSION_H
#define EDUJBARRIOS_AI_WEIGHTED_KNN_REGRESSION_H
#include <stdbool.h>
#include <stddef.h>
/* Training n×d flat row-major, n=1..128, d=1..16, all finite
   coordinates and targets [-1000,1000], k=1..n. Input arrays must
   not overlap the distinct prediction output. Failure preserves output. */
bool weighted_knn_regression(const double *train,const double *targets,
                             const double *query,size_t n,size_t d,size_t k,
                             double *prediction_out);
#endif
