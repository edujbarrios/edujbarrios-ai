#ifndef EDUJBARRIOS_AI_DBSCAN_H
#define EDUJBARRIOS_AI_DBSCAN_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major points[n*d], n=1..1024, d=1..64, finite epsilon>0,
   min_samples=1..n. Caller provides output labels[n], disjoint from
   points and unchanged on failure. Noise=-1; clusters start at 0. */
bool dbscan(const double *points,size_t n,size_t d,double epsilon,
            size_t min_samples,int *labels);
#endif
