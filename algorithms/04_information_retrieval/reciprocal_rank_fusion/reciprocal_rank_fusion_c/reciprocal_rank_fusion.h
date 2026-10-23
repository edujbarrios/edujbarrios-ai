#ifndef EDUJBARRIOS_AI_RECIPROCAL_RANK_FUSION_H
#define EDUJBARRIOS_AI_RECIPROCAL_RANK_FUSION_H
#include <stdbool.h>
#include <stddef.h>

/* Zero-based, unique, nonnegative int IDs per list (one list may be empty).
   Results capacity must be >= len_a+len_b, no alias with input.
   On error, both out and out_count stay unchanged. */
bool reciprocal_rank_fusion(const int *a, size_t len_a,
                            const int *b, size_t len_b,
                            int *out, size_t *out_count);
#endif
