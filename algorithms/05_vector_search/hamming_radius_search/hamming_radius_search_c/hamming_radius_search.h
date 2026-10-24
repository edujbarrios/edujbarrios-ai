#ifndef EDUJBARRIOS_AI_HAMMING_RADIUS_SEARCH_H
#define EDUJBARRIOS_AI_HAMMING_RADIUS_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Input n 30-bit signatures, radius <=30. indices holds at least n entries
   when n>0, or may be NULL when n==0. out_count always required.
   Outputs are unchanged on failure; caller must avoid buffer overlap. */
bool hamming_radius_search(uint32_t query, const uint32_t *signatures,
                           size_t n, unsigned radius, size_t *indices,
                           size_t *out_count);
#endif
