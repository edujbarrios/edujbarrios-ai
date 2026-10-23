#ifndef EDUJBARRIOS_AI_POSTINGS_INTERSECTION_H
#define EDUJBARRIOS_AI_POSTINGS_INTERSECTION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Inputs are strictly increasing unique IDs in 0..INT32_MAX.
   output has min(n_left,n_right) slots if both inputs nonempty;
   may be NULL otherwise. Input/output buffers must not overlap.
   out_count is required and unchanged on failure. */
bool postings_intersection(const uint32_t *left, size_t n_left,
                           const uint32_t *right, size_t n_right,
                           uint32_t *output, size_t *out_count);
#endif
