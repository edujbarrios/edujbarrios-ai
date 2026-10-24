#ifndef EDUJBARRIOS_AI_KNN_REGRESSION_H
#define EDUJBARRIOS_AI_KNN_REGRESSION_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major training[n*d], targets[n], query[d]. n 1..4096, d 1..64.
   Return unweighted nearest-k target average. Caller-owned prediction
   scalar is disjoint from all inputs and unchanged on failure. */
bool knn_regression(const double *training,const double *targets,
                    size_t n,size_t d,const double *query,
                    size_t k,double *prediction);
#endif
