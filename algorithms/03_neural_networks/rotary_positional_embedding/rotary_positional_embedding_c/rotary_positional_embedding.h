#ifndef EDUJBARRIOS_AI_ROTARY_POSITIONAL_EMBEDDING_H
#define EDUJBARRIOS_AI_ROTARY_POSITIONAL_EMBEDDING_H
#include <stdbool.h>
#include <stddef.h>
/* Rotate even-dimensional finite query/key vectors with pairwise
   positional angles. Caller owns d-sized disjoint output buffers;
   outputs stay unchanged on error. Positions: 0..1,000,000. */
bool rotary_positional_embedding(const double *query, const double *key,
    size_t d, size_t query_position, size_t key_position,
    double *rotated_query, double *rotated_key);
#endif
