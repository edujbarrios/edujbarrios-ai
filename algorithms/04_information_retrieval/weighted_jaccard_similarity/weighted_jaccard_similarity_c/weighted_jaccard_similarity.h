#ifndef EDUJBARRIOS_AI_WEIGHTED_JACCARD_SIMILARITY_H
#define EDUJBARRIOS_AI_WEIGHTED_JACCARD_SIMILARITY_H
#include <stdbool.h>
#include <stddef.h>

/* Nonempty aligned finite nonnegative weight vectors. No output on error. */
bool weighted_jaccard_similarity(const double *a, const double *b,
                                 size_t n, double *result);
#endif
