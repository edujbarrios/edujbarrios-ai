#ifndef EDUJBARRIOS_AI_KNN_H
#define EDUJBARRIOS_AI_KNN_H
#include <stdbool.h>
#include <stddef.h>

/* Dataset is n_samples x dimensions in row-major order.
   Query contains dimensions items, results contains at least k indices.
   All buffers must be valid and result must not overlap inputs.
   Returns false without modifying results on any error. */
bool knn(const double *samples, size_t n_samples, size_t dimensions,
         const double *query, size_t k, size_t *results);
#endif
