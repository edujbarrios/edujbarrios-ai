#ifndef EDUJBARRIOS_AI_SILHOUETTE_SCORE_H
#define EDUJBARRIOS_AI_SILHOUETTE_SCORE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* n=2..128, d=1..32, finite row-major points[n*d], labels[n]<n.
   Between 2 and n-1 populated clusters. Caller-owned result unchanged
   on failure and must not alias the input buffers. */
bool silhouette_score(const double *points,const uint32_t *labels,
                      size_t n,size_t d,double *result);
#endif
