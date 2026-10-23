#ifndef EDUJBARRIOS_AI_K_MEANS_H
#define EDUJBARRIOS_AI_K_MEANS_H
#include <stdbool.h>
#include <stddef.h>

/* samples is row-major (n x d). Output centroids is row-major (k x d),
   assignments has n zero-based cluster indices. Caller guarantees buffers
   are large enough and do not overlap. On error, outputs are unchanged. */
bool k_means(const double *samples, size_t n, size_t d,
             size_t k, size_t max_iterations,
             double *centroids_out, size_t *assignments_out);
#endif
