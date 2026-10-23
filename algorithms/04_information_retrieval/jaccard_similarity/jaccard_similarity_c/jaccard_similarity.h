#ifndef EDUJBARRIOS_AI_JACCARD_SIMILARITY_H
#define EDUJBARRIOS_AI_JACCARD_SIMILARITY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Inputs are integer-token arrays with declared lengths.
   Null inputs allowed only when their length is zero.
   *out unchanged if memory allocation/validation fails. */
bool jaccard_similarity(const int32_t *left, size_t n_left,
                        const int32_t *right, size_t n_right, double *out);
#endif
