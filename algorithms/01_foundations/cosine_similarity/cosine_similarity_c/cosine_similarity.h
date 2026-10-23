#ifndef EDUJBARRIOS_AI_COSINE_SIMILARITY_H
#define EDUJBARRIOS_AI_COSINE_SIMILARITY_H

#include <stdbool.h>
#include <stddef.h>

/* Both inputs contain at least n doubles; n > 0. On failure *out is unchanged. */
bool cosine_similarity(const double *left, const double *right,
                       size_t n, double *out);

#endif
