#ifndef EDUJBARRIOS_AI_HAMMING_TOP_K_H
#define EDUJBARRIOS_AI_HAMMING_TOP_K_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Rank n 30-bit candidate signatures relative to query. Output has >=k
   indices and does not overlap candidates. On failure output is unchanged. */
bool hamming_top_k(uint32_t query, const uint32_t *candidates,
                   size_t n, size_t k, size_t *indices);
#endif
