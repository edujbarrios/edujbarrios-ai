#ifndef EDUJBARRIOS_AI_POSTINGS_DIFFERENCE_H
#define EDUJBARRIOS_AI_POSTINGS_DIFFERENCE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Included/excluded lists are sorted unique IDs 0..INT32_MAX.
   output has capacity >= n_included unless n_included==0 (may be NULL).
   out_count is required; outputs unchanged on invalid input.
   Input and output buffers must not overlap. */
bool postings_difference(const uint32_t *included, size_t n_included,
                         const uint32_t *excluded, size_t n_excluded,
                         uint32_t *output, size_t *out_count);
#endif
